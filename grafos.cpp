#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <limits>

using namespace std;

// author is: Fernando Elena Benavente , the purpose of this code is simply learn C++ in a fun way, as the same time I make usefull code that I can use in a future
// the idea of this code is make a graph generator, and a dijkstra algorithm path finder, also a user can create its own graphs, and find shortests paths/ unreachable vortexs in a graph
// on the generated graph, the graph is implemented via a adyascence linked list, where only the existing edges are represented once, on the lower index vortex, saving memory costs
// this implementation is very efficient for very disperse graphs, as it saves a lot in memory, more suitable for real life problems (ejm diferent cities connected, or a social networks contacts)
// the implementation is as lower level as possible in C++, to optimize speed and memory usage, vector class has been not used for this purpose


 // struct to represent edges
typedef struct edge
{
	unsigned int vortex_index;
	unsigned int edge_weight;
	struct edge *next; // edges are supposed to go from lower vortex indexes, to higher vortex indexes
} edge;

// struct to represent vortexes
typedef struct vortex
{ 
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
	void print_graph_edges();				 // function to print the current graph edges in a simple console format
	int add_edge(unsigned int vortex1, unsigned int vortex2, unsigned int weight);    // creates an edge between 2 vortexs on this graph
	void generate_random_edges(unsigned int cp_probability); // generate random edges on an already created graph, cp_probability is from 0 to 100, the more probability the more complete the graph is
	int search_shortest_distance_dijkstra(unsigned int base_vortex, unsigned int goal_vortex); // shortest paths between 2 vortexs using dijkstra
	int *get_full_reachable_vortexs(int base_node); // returns a dynamic array of size this->vortex_number with the unreachable/reachable vortexs

private:
	string graph_name; // graph name
	int vortex_number; // number of vortexes in this graph
	vortex *graph_head; // head of the linked list wich represents the graph

	void add_edge_private(vortex &Vortex, unsigned int vortex_index_to, unsigned int edge_weight); // adds an edge between 2 vortexes with a weight, low level private function
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

// ads an edge between 2 vortexs with a weight
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

	// If the vortex has no edges, simply add the new edge
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

// finds the reachable nodes from a base node in the current graph, expects an array initialized with -1
void list_graph::full_reachable_vortexs(int *ptr, unsigned int base_node) 
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

// receives a current node, the distance to that node from base, an and array of distances, used mainly for dijkstra algorithm
// the distance array is suposed to be initialized already , with -1 representing infinite
void list_graph::reach_vortex(unsigned int current_node,unsigned int current_distance_frombase, int *distance_array) 
{												   
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

// checks if every vortex is visted, from an already initiatied vortex array 
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
	cout << "number of graph vortexs " << this->graph_name << " is "
	     << this->vortex_number << endl;
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

//adds an edge between 2 vortexs to a graph, if the edge already exists, just update the weight of the edge
int list_graph::add_edge(unsigned int vortex1, unsigned int vortex2 , unsigned int weight){
	if(vortex1 >=this->vortex_number || vortex2 >= this->vortex_number){
		return -1; // vortex index does not exist on this graph, so nothing is done
	}
	unsigned int low_vortex,high_vortex;
	if(vortex1 < vortex2){ // check which one is the lower index vortex
		low_vortex = vortex1;
		high_vortex = vortex2;
	}
	else if(vortex2 < vortex1){
		low_vortex = vortex2;
		high_vortex = vortex1;
	}
	else{
		return -2; // you cannot add and edge to the same vortex as base and goal
	}

	vortex *iterator_vortex = this->graph_head;
	edge *iterator_edge;
	for(int i = 0 ; i < low_vortex ; i++){ // finds the vortex to add the edge
		if(iterator_vortex->vortex_index == low_vortex){
			break;
		}
		iterator_vortex = iterator_vortex->next;
	}

	iterator_edge = iterator_vortex->edge_ptr;
	while(iterator_edge != nullptr){ // checks the edge does not already exists
		if(iterator_edge->vortex_index == high_vortex){ // if edge already exists, update the weight 
			iterator_edge->edge_weight = weight;
			return 1;
		}
		iterator_edge = iterator_edge->next;
	}

	add_edge_private((*iterator_vortex),high_vortex,weight); // if the edge does not exists, add a new edge
	return 1;
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
// returns an array of size this->vortex_number with 0 on reachable nodes, and -1on unreachable from base_node
int *list_graph::get_full_reachable_vortexs(int base_vortex) 
{ 
	int *ptr = new int[this->vortex_number];
	for (int i = 0; i < vortex_number; ptr[i++] = -1)
		;
	full_reachable_vortexs(ptr, base_vortex);
	return ptr;
}

//searchs the shortest distance between to vortex using dijkstra algorithm, returns -1 if goal vortex is unreachable
int list_graph::search_shortest_distance_dijkstra(unsigned int base_vortex, unsigned int goal_vortex)
{
	int *visitable_vortexs = get_full_reachable_vortexs(base_vortex);
	if (visitable_vortexs[goal_vortex] == -1)
	{
		delete[] visitable_vortexs;
		return -1; // Goal vortex not reachable
	}
	int *distance_frombase = new int[this->vortex_number];
	int *predecessor = new int[this->vortex_number]; // Array to track predecessors

	for (int i = 0; i < this->vortex_number; i++) {
		distance_frombase[i] = -1;
		predecessor[i] = -1; // Initialize predecessors to -1
	}
	distance_frombase[base_vortex] = 0;
	unsigned int current_node = base_vortex;
	unsigned int current_lower_distance;

	while (!check_all_vortex_visited(visitable_vortexs) && distance_frombase[goal_vortex] != 0) {
		reach_vortex(current_node, distance_frombase[current_node], distance_frombase);

		// Update predecessors for undirected graph
		vortex *current_vortex = this->graph_head;
		for (int i = 0; i < current_node; ++i) current_vortex = current_vortex->next;
		edge *current_edge = current_vortex->edge_ptr;

		while (current_edge != nullptr) {
			if (distance_frombase[current_edge->vortex_index] != -1 &&
				distance_frombase[current_node] + current_edge->edge_weight == distance_frombase[current_edge->vortex_index]) {
				// Update predecessor regardless of index order
				predecessor[current_edge->vortex_index] = current_node;
			}
			if (distance_frombase[current_node] == distance_frombase[current_edge->vortex_index] + current_edge->edge_weight) {
				predecessor[current_node] = current_edge->vortex_index;
			}
			current_edge = current_edge->next;
		}

		visitable_vortexs[current_node] = 1;
		current_lower_distance = numeric_limits<int>::max();

		for (int i = 0; i < this->vortex_number; ++i) {
			if (visitable_vortexs[i] == 0 && distance_frombase[i] < current_lower_distance) {
				current_node = i;
				current_lower_distance = distance_frombase[i];
			}
		}
	}
	current_lower_distance = distance_frombase[goal_vortex];

	// Print the shortest path
	if (current_lower_distance != numeric_limits<int>::max()) {
		cout << "Shortest path from " << base_vortex << " to " << goal_vortex << ": ";
		int trace_node = goal_vortex;
		while (trace_node != -1) {
			cout << trace_node;
			trace_node = predecessor[trace_node];
			if (trace_node != -1) cout << " <- ";
		}
		cout << endl;
	} else {
		cout << "No path from " << base_vortex << " to " << goal_vortex << " found." << endl;
	}

	delete[] visitable_vortexs;
	delete[] distance_frombase;
	delete[] predecessor;
	return current_lower_distance;
}



//trivial testing main, atm just stupid machine
int main()
{
	cout << "Here goes the graph machine! \n";
	const int graph_size = 6;
	list_graph migrafo(graph_size, "grafo1");
	migrafo.print_vortexnumber();
	/*migrafo.add_edge(0,1,1);
	migrafo.add_edge(3,1,1);
	migrafo.add_edge(4,3,4);
	migrafo.add_edge(4,5,10);
	migrafo.add_edge(0,2,7);
	migrafo.add_edge(5,2,7);
	migrafo.print_graph_edges();*/

	
	migrafo.generate_random_edges(40);
	migrafo.print_graph_edges();
	int *ptr = migrafo.get_full_reachable_vortexs(0);

	for (int i = 0; i < graph_size; i++)
	{
		if (ptr[i] == -1)
		{
			cout << "vortex :" << i << " not accesible\n";
		}
	}

	

	int distance = migrafo.search_shortest_distance_dijkstra(5,0);
	
	cout << "distance between base vortex and goal vortex is : " << distance << endl;
	return 0;
}
