#include <ctime>
#include <iostream>
#include <random>
#include <string>

using namespace std;

// the idea of this program is make a graph generator, and a dijkstra algorithm
// on the generated graph, the graph is implemented via a linked list, where
// only the existing edges are represented

typedef struct edge
{ // struct to represent edges
	unsigned int vortex_index;
	unsigned int edge_weight;
	struct edge *next; // edges are supposed to go from lower vortex indexes, to higher vortex indexes
} edge;

typedef struct vortex
{ // struct to represent vortexes
	unsigned int vortex_index;
	edge *edge_ptr;
	struct vortex *next;
} vortex;

class list_graph
{
public:
	list_graph(int vortex_number, string graph_name); // constructor
	~list_graph();					  // destructor

	void print_vortexnumber();
	void generate_random_edges(unsigned int cp_probability);     // probability in percentage (from 0 to 100) of the graph to be convex
	void print_graph_edges();				     // function to print the current graph edges in a simple console format
	void find_reachable_nodes(int *ptr, unsigned int base_node); // finds reachable nodes in the graphic, from the base node, ptr is an initialized to -1 array, 0 means reachable
	void search_shortest_path_djakstra(unsigned int base_vortex, unsigned int goal_vortex);

private:
	string graph_name;
	int vortex_number;
	vortex *graph_head;

	void add_edge_private(vortex &Vortex, unsigned int vortex_index_to, unsigned int edge_weight);
};

// Constructor implementation
list_graph::list_graph(int vortex_number, string graph_name)
{
	this->vortex_number = vortex_number;
	this->graph_name = graph_name;
	this->graph_head = nullptr;
	vortex *current_vortex, *previous_vortex;

	for (int i = 0; i < this->vortex_number; ++i)
	{ // generate the graph with the vortex number we indicated
		if (i == 0)
		{
			current_vortex = new vortex;
			graph_head = current_vortex;
			graph_head->next = nullptr;
			graph_head->edge_ptr = nullptr;
			graph_head->vortex_index = i;
		}
		else
		{
			previous_vortex = current_vortex;
			current_vortex = new vortex;
			previous_vortex->next = current_vortex;
			current_vortex->next = nullptr;
			current_vortex->edge_ptr = nullptr;
			current_vortex->vortex_index = i;
		}
	}
}

// Destructor implementation
list_graph::~list_graph()
{
	vortex *current_vortex = graph_head;
	while (current_vortex != nullptr)
	{
		edge *current_edge = current_vortex->edge_ptr;
		while (current_edge != nullptr)
		{
			edge *next_edge = current_edge->next;
			delete current_edge;
			current_edge = next_edge;
		}
		vortex *next_vortex = current_vortex->next;
		delete current_vortex;
		current_vortex = next_vortex;
	}
}

// Prints the number of vertices in the graph
void list_graph::print_vortexnumber()
{
	cout << "El número de vértices del grafo " << this->graph_name << " es "
	     << this->vortex_number << endl;
}

// Private method to add an edge to a vertex (unmodified version)
void list_graph::add_edge_private(vortex &Vortex, unsigned int vortex_index_to, unsigned int edge_weight)
{
	edge *current_edge;
	if (Vortex.edge_ptr == nullptr)
	{
		Vortex.edge_ptr = new edge;
		Vortex.edge_ptr->vortex_index = vortex_index_to;
		Vortex.edge_ptr->edge_weight = edge_weight;
		Vortex.edge_ptr->next = nullptr;
	}
	else
	{
		current_edge = Vortex.edge_ptr;
		Vortex.edge_ptr = new edge;
		Vortex.edge_ptr->vortex_index = vortex_index_to;
		Vortex.edge_ptr->edge_weight = edge_weight;
		Vortex.edge_ptr->next = current_edge;
	}
}

// Generates random edges on the graph
void list_graph::generate_random_edges(unsigned int cp_probability)
{
	srand(time(0));
	vortex *current_vortex = this->graph_head;
	unsigned int random_int, edge_max = 0;
	for (int i = 0; i < this->vortex_number; ++i)
	{
		for (int j = this->vortex_number - 1; j > edge_max; --j)
		{
			if ((rand() % 100) <= cp_probability)
			{
				add_edge_private((*current_vortex), j, (rand() % 10));
			}
		}
		current_vortex = current_vortex->next;
		++edge_max;
	}
}

// Prints in a readable format, the existing edges of an already created graph
void list_graph::print_graph_edges()
{
	vortex *iterator_vortex = graph_head;
	edge *iterator_edge;
	for (int i = 0; i < this->vortex_number; ++i)
	{
		iterator_edge = iterator_vortex->edge_ptr;
		do
		{
			if (iterator_edge != nullptr)
			{
				cout << "Edge from " << i << " to " << iterator_edge->vortex_index << " with weight: " << iterator_edge->edge_weight << endl;
				iterator_edge = iterator_edge->next;
			}
		} while (iterator_edge != nullptr);
		iterator_vortex = iterator_vortex->next;
	}
}

void list_graph::find_reachable_nodes(int *ptr, unsigned int base_node) // finds the reachable nodes from a base node in the current graph, expects an array initialized with -1
{
	if (ptr[base_node] == -1) // if node is still not reached, setup as reached node
		ptr[base_node] = 0;
	else
		return;

	vortex *current_vortex = this->graph_head;
	edge *current_edge;
	for (int i = 0; i <= base_node; ++i)
	{
		current_edge = current_vortex->edge_ptr;
		while (current_edge != nullptr)
		{
			if (ptr[current_edge->vortex_index] == -1 && ptr[current_vortex->vortex_index] == 0)
			{
				find_reachable_nodes(ptr, current_edge->vortex_index);
			}
			if (ptr[current_edge->vortex_index] == 0 && ptr[current_vortex->vortex_index] == -1)
			{
				find_reachable_nodes(ptr, current_vortex->vortex_index);
			}
			current_edge = current_edge->next;
		}
		current_vortex = current_vortex->next;
	}
}

void list_graph::search_shortest_path_djakstra(unsigned int base_vortex, unsigned int goal_vortex)
{
	int *visited_vortexs = new int[this->vortex_number]; // currently visited vortexs,0 means not visited, -1 means not reachable from base node
	for (int i = 0; i < this->vortex_number; visited_vortexs[i++] = -1)
		;					       // setup all vortex to not reachable intially
	int *distance_frombase = new int[this->vortex_number]; // distance to vortex from base vortex
	distance_frombase[base_vortex] = 0;		       // distance from base to base is obviusly 0
}

int main()
{
	cout << "Here goes my C++ graph program! \n";
	const int graph_size = 8;
	list_graph migrafo(graph_size, "grafo1");
	migrafo.print_vortexnumber();
	migrafo.generate_random_edges(15);
	migrafo.print_graph_edges();
	int *ptr = new int[graph_size];
	for (int i = 0; i < graph_size; ptr[i++] = -1)
		;
	migrafo.find_reachable_nodes(ptr, 0);
	for (int i = 0; i < graph_size; i++)
	{
		if (ptr[i] == -1)
		{
			cout << "el nodo :" << i << " no es accesible\n";
		}
	}
	return 0;
}
