#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <limits>

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
	void generate_random_edges(unsigned int cp_probability); // probability in percentage (from 0 to 100) of the graph to be convex
	void print_graph_edges();				 // function to print the current graph edges in a simple console format
	int search_shortest_distance_djakstra(unsigned int base_vortex, unsigned int goal_vortex);
	int *get_full_reachable_vortexs(int base_node);

private:
	string graph_name;
	int vortex_number;
	vortex *graph_head;

	void add_edge_private(vortex &Vortex, unsigned int vortex_index_to, unsigned int edge_weight);
	void full_reachable_vortexs(int *ptr, unsigned int base_node); // finds reachable nodes in the graphic, from the base node, ptr is an initialized to -1 array, 0 means reachable
	void reach_vortex(unsigned int current_node,unsigned int current_distance_frombase, int *distance_array); // distance array expected to be initialized to -1 when the distance is infinite
	int check_all_vortex_visited(int *visited_vortex); //expects an array of visited vortex, 1 means visited, 0 not visited, -1 unreachable
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

//////////////////////////////////////PRIVATE METHODS////////////////////////////////////////////////////////////////

void list_graph::add_edge_private(vortex &Vortex, unsigned int vortex_index_to, unsigned int edge_weight)
{
	// Ensure that Vortex is the vertex with the lower index
	unsigned int from_index = Vortex.vortex_index;
	unsigned int to_index = vortex_index_to;

	// Create a new edge
	edge *new_edge = new edge;
	new_edge->vortex_index = to_index;
	new_edge->edge_weight = edge_weight;
	new_edge->next = nullptr;

	// If the vertex has no edges, simply add the new edge
	if (Vortex.edge_ptr == nullptr)
	{
		Vortex.edge_ptr = new_edge;
	}
	else
	{
		edge *current_edge = Vortex.edge_ptr;
		edge *previous_edge = nullptr;

		// Find the correct position to insert the new edge
		while (current_edge != nullptr && current_edge->vortex_index < to_index)
		{
			previous_edge = current_edge;
			current_edge = current_edge->next;
		}

		// Insertion into the list
		if (previous_edge == nullptr) // Inserting at the beginning
		{
			new_edge->next = Vortex.edge_ptr;
			Vortex.edge_ptr = new_edge;
		}
		else // Inserting between existing edges
		{
			new_edge->next = current_edge;
			previous_edge->next = new_edge;
		}
	}
}

void list_graph::full_reachable_vortexs(int *ptr, unsigned int base_node) // finds the reachable nodes from a base node in the current graph, expects an array initialized with -1
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
				full_reachable_vortexs(ptr, current_edge->vortex_index);
			}
			if (ptr[current_edge->vortex_index] == 0 && ptr[current_vortex->vortex_index] == -1)
			{
				full_reachable_vortexs(ptr, current_vortex->vortex_index);
			}
			current_edge = current_edge->next;
		}
		current_vortex = current_vortex->next;
	}
}

void list_graph::reach_vortex(unsigned int current_node,unsigned int current_distance_frombase, int *distance_array) // receives a current node, the distance to that node from base, an and array of distances
{												    // the distance array is suposed to be initialized already , with -1 representing infinite
	vortex *current_vortex = this->graph_head;
	edge *current_edge;
	for (int i = 0; i <= current_node; i++)
	{
		current_edge = current_vortex->edge_ptr;
		while (current_edge != nullptr)
		{
			if (current_vortex->vortex_index != current_node && current_edge->vortex_index == current_node)
			{
				if (current_distance_frombase + current_edge->edge_weight < distance_array[current_vortex->vortex_index] || distance_array[current_vortex->vortex_index] == -1)
				{
					distance_array[current_vortex->vortex_index] = current_distance_frombase + current_edge->edge_weight;
				}
			}
			else if (current_vortex->vortex_index == current_node)
			{
				if (current_distance_frombase + current_edge->edge_weight < distance_array[current_edge->vortex_index] || distance_array[current_edge->vortex_index] == -1)
				{
					distance_array[current_edge->vortex_index] = current_distance_frombase + current_edge->edge_weight;
				}
			}
			current_edge = current_edge->next;
		}
		current_vortex = current_vortex->next;
	}
}

int list_graph::check_all_vortex_visited(int *visited_vortex){
	for(int i = 0; i < this->vortex_number ; i++){
		if(visited_vortex[i] == 0){
			return 0;
		}
	}
	return 1;
}

//////////////////////////////////////PUBLIC METHODS////////////////////////////////////////////////////////////////

// Prints the number of vertices in the graph
void list_graph::print_vortexnumber()
{
	cout << "El número de vértices del grafo " << this->graph_name << " es "
	     << this->vortex_number << endl;
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
				add_edge_private((*current_vortex), j, (rand() % 9) +1);
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
				cout << "Edge between " << i << " and " << iterator_edge->vortex_index << " with weight: " << iterator_edge->edge_weight << endl;
				iterator_edge = iterator_edge->next;
			}
		} while (iterator_edge != nullptr);
		iterator_vortex = iterator_vortex->next;
	}
}

int *list_graph::get_full_reachable_vortexs(int base_vortex)
{ // returns an array of size this->vortex_number with 0 on reachable nodes, and -1on unreachable from base_node
	int *ptr = new int[this->vortex_number];
	for (int i = 0; i < vortex_number; ptr[i++] = -1)
		;
	full_reachable_vortexs(ptr, base_vortex);
	return ptr;
}

int list_graph::search_shortest_distance_djakstra(unsigned int base_vortex, unsigned int goal_vortex)
{
	int *visitable_vortexs = get_full_reachable_vortexs(base_vortex); // put reachable nodes from base vortex to 0, and unreachable nodes to -1, reserved on heap memory
	if (visitable_vortexs[goal_vortex] == -1)
	{
		return -1; // goal vortex not reachable from base vortex, so we return error code
	}
	int *distance_frombase = new int[this->vortex_number];		      // distance to vortex from base vortex
	for (int i = 0; i < this->vortex_number; distance_frombase[i++] = -1); // set -1 as infinite distance from base for starting case;
	distance_frombase[base_vortex] = 0;	 // distance from base to base is obviusly 0
	unsigned int current_node = base_vortex; // current node we are exploring
	unsigned int current_lower_distance;     //lowest distance in a non visted node atm

	while(!check_all_vortex_visited(visitable_vortexs) && distance_frombase[goal_vortex] != 0){
		reach_vortex(current_node,distance_frombase[current_node],distance_frombase);
		visitable_vortexs[current_node] = 1;
		current_lower_distance = numeric_limits<int>::max();
		for(int i = 0 ; i < this->vortex_number ; ++i){
			if(visitable_vortexs[i] == 0 && distance_frombase[i] < current_lower_distance){
				current_node = i;
				current_lower_distance = distance_frombase[i];
			}
		}
	}
	current_lower_distance = distance_frombase[goal_vortex];
	return current_lower_distance;
}

int main()
{
	cout << "Here goes my C++ graph program! \n";
	const int graph_size = 6;
	list_graph migrafo(graph_size, "grafo1");
	migrafo.print_vortexnumber();
	migrafo.generate_random_edges(50);
	migrafo.print_graph_edges();
	int *ptr = migrafo.get_full_reachable_vortexs(0);

	for (int i = 0; i < graph_size; i++)
	{
		if (ptr[i] == -1)
		{
			cout << "el nodo :" << i << " no es accesible\n";
		}
	}

	int distance = migrafo.search_shortest_distance_djakstra(0,5);
	
	cout << "la distancia entre el nodo base y el destino es : " << distance << endl;
	return 0;
}
