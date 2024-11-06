#ifndef GRAPH_H
#define GRAPH_H

// author is: Fernando Elena Benavente , the purpose of this code is simply learn C++ in a fun way, as the same time I make usefull code that I can use in a future
// the idea of this code is make a graph generator, and a dijkstra algorithm path finder, also a user can create its own graphs, and find shortests paths/ unreachable vortexs in a graph
// on the generated graph, the graph is implemented via a adyascence linked list, where only the existing edges are represented once, on the lower index vortex, saving memory costs
// this implementation is very efficient for very disperse graphs, as it saves a lot in memory, more suitable for real life problems (ejm diferent cities connected, or a social networks contacts)
// the implementation is as lower level as possible in C++, to optimize speed and memory usage, vector class has been not used for this purpose

#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <limits>

using namespace std;

// Struct to represent edges
typedef struct edge
{
	unsigned int vortex_index;
	unsigned int edge_weight;
	struct edge *next; // edges are supposed to go from lower vortex indexes, to higher vortex indexes
} edge;

// Struct to represent vortexes
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
	void print_graph_edges();								   // function to print the current graph edges in a simple console format
	int add_edge(unsigned int vortex1, unsigned int vortex2, unsigned int weight);		   // creates an edge between 2 vortexes on this graph
	int remove_edge(unsigned int vortex1, unsigned int vortex2);				   // removes an edge between 2 vortexes on this graph
	void generate_random_edges(unsigned int cp_probability);				   // generate random edges on an already created graph
	int search_shortest_distance_dijkstra(unsigned int base_vortex, unsigned int goal_vortex); // shortest paths between 2 vortexes using dijkstra
	int *get_full_reachable_vortexs(int base_node);						   // returns a dynamic array of size this->vortex_number with the unreachable/reachable vortexes

private:
	string graph_name;  // graph name
	int vortex_number;  // number of vortexes in this graph
	vortex *graph_head; // head of the linked list which represents the graph

	void add_edge_private(vortex &Vortex, unsigned int vortex_index_to, unsigned int edge_weight);		   // low-level function to add an edge
	void remove_edge_private(vortex &Vortex, unsigned int vortex_index_to);					   // low-level function to remove an edge
	void full_reachable_vortexs(int *ptr, unsigned int base_node);						   // finds reachable nodes in the graph, uses backtracking
	void reach_vortex(unsigned int current_node, unsigned int current_distance_frombase, int *distance_array); // used for dijkstra
	int check_all_vortex_visited(int *visited_vortex);							   // checks if every vortex is visited
};

#endif
