#include "graph.h"

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
	// Create a new edge
	edge *new_edge = new edge;
	new_edge->vortex_index = vortex_index_to;
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
		while (current_edge != nullptr && current_edge->vortex_index < vortex_index_to)
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

void list_graph::remove_edge_private(vortex &Vortex, unsigned int vortex_index_to)
{
	edge *current_edge = Vortex.edge_ptr;
	edge *previous_edge = nullptr;

	// Find the correct position to delete the edge
	while (current_edge != nullptr && current_edge->vortex_index != vortex_index_to)
	{
		previous_edge = current_edge;
		current_edge = current_edge->next;
	}

	if (current_edge == nullptr) // if not existant edge, do nothing
		return;

	// If the edge to delete is the first edge
	if (previous_edge == nullptr)
	{
		Vortex.edge_ptr = current_edge->next;
	}
	else
	{ // Link previous edge to the next edge, bypassing current edge
		previous_edge->next = current_edge->next;
	}

	// Delete the current edge
	delete current_edge;
}

// finds the reachable nodes from a base node in the current graph, expects an array initialized with -1, uses backtracking algorithm
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
void list_graph::reach_vortex(unsigned int current_node, unsigned int current_distance_frombase, int *distance_array)
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
int list_graph::check_all_vortex_visited(int *visited_vortex)
{
	for (int i = 0; i < this->vortex_number; i++)
	{
		if (visited_vortex[i] == 0)
		{
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

// Function to add a vortex (vertex) to the graph in a sorted order
int list_graph::add_vortex(unsigned int vortex_index)
{
	// Check if the vertex already exists
	vortex *temp = graph_head;
	while (temp != nullptr)
	{
		if (temp->vortex_index == vortex_index)
		{
			return -1; // Vertex already exists
		}
		temp = temp->next;
	}

	// Create a new vertex
	vortex *new_vortex = new vortex;
	new_vortex->vortex_index = vortex_index;
	new_vortex->edge_ptr = nullptr;

	// If the list is empty, set the new vortex as the head
	if (graph_head == nullptr || graph_head->vortex_index > vortex_index)
	{
		new_vortex->next = graph_head;
		graph_head = new_vortex;
	}
	else
	{
		// Traverse the list and insert the new vortex in the correct sorted position
		vortex *current = graph_head;
		while (current->next != nullptr && current->next->vortex_index < vortex_index)
		{
			current = current->next;
		}
		new_vortex->next = current->next;
		current->next = new_vortex;
	}

	vortex_number++; // Increment the vortex count
	return 0;	 // Vertex added successfully
}

// Function to remove a vortex (vertex) from the graph
int list_graph::remove_vortex(unsigned int vortex_index)
{
	vortex *current = graph_head;
	vortex *previous = nullptr;

	// Traverse the list to find the vortex to remove
	while (current != nullptr && current->vortex_index != vortex_index)
	{
		previous = current;
		current = current->next;
	}

	// If the vortex is not found, return an error
	if (current == nullptr)
	{
		return -1; // Vertex does not exist
	}

	// Remove all edges associated with this vortex
	while (current->edge_ptr != nullptr)
	{
		edge *temp_edge = current->edge_ptr;
		current->edge_ptr = current->edge_ptr->next;
		delete temp_edge;
	}

	// Remove any edges that point to this vortex
	vortex *temp = graph_head;
	while (temp != nullptr)
	{
		remove_edge_private(*temp, vortex_index); // Remove edges from other vertices
		temp = temp->next;
	}

	// Remove the vortex itself
	if (previous == nullptr)
	{
		// If the vortex to remove is the head of the list
		graph_head = current->next;
	}
	else
	{
		previous->next = current->next;
	}

	delete current;
	vortex_number--; // Decrease the vortex count
	return 0;	 // Vertex removed successfully
}

// adds an edge between 2 vortexs to a graph, if the edge already exists, just update the weight of the edge
int list_graph::add_edge(unsigned int vortex1, unsigned int vortex2, unsigned int weight)
{
	if (vortex1 >= this->vortex_number || vortex2 >= this->vortex_number)
	{
		return -1; // vortex index does not exist on this graph, so nothing is done
	}
	unsigned int low_vortex, high_vortex;
	if (vortex1 < vortex2)
	{ // check which one is the lower index vortex
		low_vortex = vortex1;
		high_vortex = vortex2;
	}
	else if (vortex2 < vortex1)
	{
		low_vortex = vortex2;
		high_vortex = vortex1;
	}
	else
	{
		return -2; // you cannot add and edge to the same vortex as base and goal
	}

	vortex *iterator_vortex = this->graph_head;
	unsigned char found = 0;

	for (int i = 0; i < this->vortex_number && iterator_vortex != nullptr; i++)
	{ // finds the base vortex to add the edge
		if (iterator_vortex->vortex_index == high_vortex)
		{
			found = 1;
			break;
		}
		iterator_vortex = iterator_vortex->next;
	}
	if (!found)
	{
		return -3; // not existing high vortex
	}
	iterator_vortex = this->graph_head;
	found = 0;
	for (int i = 0; i < this->vortex_number && iterator_vortex != nullptr; i++)
	{ // finds the base vortex to add the edge
		if (iterator_vortex->vortex_index == low_vortex)
		{
			found = 1;
			break;
		}
		iterator_vortex = iterator_vortex->next;
	}
	if (!found)
	{
		return -4; // not existing low vortex
	}

	edge *iterator_edge;
	iterator_edge = iterator_vortex->edge_ptr;
	while (iterator_edge != nullptr)
	{ // checks the edge does not already exists
		if (iterator_edge->vortex_index == high_vortex)
		{ // if edge already exists, update the weight
			iterator_edge->edge_weight = weight;
			return 1;
		}
		iterator_edge = iterator_edge->next;
	}

	add_edge_private((*iterator_vortex), high_vortex, weight); // if the edge does not exists, add a new edge
	return 1;
}

// removes an edge between 2 vortexs only if the edge exists
int list_graph::remove_edge(unsigned int vortex1, unsigned int vortex2)
{
	if (vortex1 >= this->vortex_number || vortex2 >= this->vortex_number)
	{
		return -1; // vortex index does not exist on this graph, so nothing is done
	}
	unsigned int low_vortex, high_vortex;
	if (vortex1 < vortex2)
	{ // check which one is the lower index vortex
		low_vortex = vortex1;
		high_vortex = vortex2;
	}
	else if (vortex2 < vortex1)
	{
		low_vortex = vortex2;
		high_vortex = vortex1;
	}
	else
	{
		return -2; // you cannot add and edge to the same vortex as base and goal
	}
	vortex *iterator_vortex = this->graph_head;
	unsigned char found = 0;

	for (int i = 0; i < this->vortex_number && iterator_vortex != nullptr; i++)
	{ // finds the base vortex to add the edge
		if (iterator_vortex->vortex_index == high_vortex)
		{
			found = 1;
			break;
		}
		iterator_vortex = iterator_vortex->next;
	}
	if (!found)
	{
		return -3; // not existing high vortex
	}
	iterator_vortex = this->graph_head;
	found = 0;
	for (int i = 0; i < this->vortex_number && iterator_vortex != nullptr; i++)
	{ // finds the base vortex to add the edge
		if (iterator_vortex->vortex_index == low_vortex)
		{
			found = 1;
			break;
		}
		iterator_vortex = iterator_vortex->next;
	}
	if (!found)
	{
		return -4; // not existing low vortex
	}

	remove_edge_private((*iterator_vortex), high_vortex);
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
				add_edge_private((*current_vortex), j, (rand() % 9) + 1);
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

// This function implements Dijkstra's algorithm to find the shortest path in an undirected graph
// from a starting vertex, base_vortex, to a target vertex, goal_vortex. Unlike the previous version, 
// this function no longer pre-checks reachability using an auxiliary function. Instead, it directly 
// determines accessibility as it calculates the shortest paths.

// The algorithm initializes two main arrays: distance_frombase, which stores the minimum distances 
// from base_vortex to each vertex (initialized to infinity), and predecessor, which tracks the preceding 
// vertex in the shortest path for each reachable vertex. Additionally, a visited array marks nodes 
// as they are fully processed to avoid redundant calculations.

// Starting from base_vortex with a distance of zero, the function iteratively selects the unvisited 
// vertex with the smallest known distance. For each neighbor of the current node, if the calculated 
// path provides a shorter route than previously recorded, it updates both the neighbor's distance 
// and sets the current node as its predecessor. This ensures that each node's predecessor represents 
// the shortest path back to base_vortex.

// The loop continues until either all reachable vertices are visited or the goal_vortex is reached. 
// After completing the search, the algorithm checks if goal_vortex has a finite distance; if not, 
// it returns -1, indicating that the goal_vortex is inaccessible from base_vortex. Otherwise, it 
// backtracks from goal_vortex to base_vortex using the predecessor array, reconstructing and printing 
// the shortest path.

// Finally, the function frees the dynamically allocated memory for the distance_frombase, predecessor, 
// and visited arrays and returns the shortest path distance from base_vortex to goal_vortex. If 
// goal_vortex is inaccessible, the function returns -1.


int list_graph::search_shortest_distance_dijkstra(unsigned int base_vortex, unsigned int goal_vortex) {
    int *distance_frombase = new int[this->vortex_number];
    int *predecessor = new int[this->vortex_number]; // Array to track predecessors
    bool *visited = new bool[this->vortex_number](); // Track visited nodes

    for (int i = 0; i < this->vortex_number; i++) {
        distance_frombase[i] = numeric_limits<int>::max(); // Initial distance to infinity
        predecessor[i] = -1; // Initialize predecessors to -1
    }
    distance_frombase[base_vortex] = 0;

    unsigned int current_node = base_vortex;
    unsigned int current_lower_distance;

    while (true) {
        reach_vortex(current_node, distance_frombase[current_node], distance_frombase);

        // Update predecessors for undirected graph
        vortex *current_vortex = this->graph_head;
        for (int i = 0; i < current_node; ++i)
            current_vortex = current_vortex->next;
        edge *current_edge = current_vortex->edge_ptr;

        while (current_edge != nullptr) {
            if (distance_frombase[current_edge->vortex_index] != numeric_limits<int>::max() &&
                distance_frombase[current_node] + current_edge->edge_weight == distance_frombase[current_edge->vortex_index]) {
                predecessor[current_edge->vortex_index] = current_node;
            }
            if (distance_frombase[current_node] == distance_frombase[current_edge->vortex_index] + current_edge->edge_weight) {
                predecessor[current_node] = current_edge->vortex_index;
            }
            current_edge = current_edge->next;
        }

        visited[current_node] = true;

        if (current_node == goal_vortex || distance_frombase[current_node] == numeric_limits<int>::max()) {
            break; // Exit if reached the goal or no further reachable nodes
        }

        // Find the unvisited node with the smallest distance
        current_lower_distance = numeric_limits<int>::max();
        for (int i = 0; i < this->vortex_number; ++i) {
            if (!visited[i] && distance_frombase[i] < current_lower_distance) {
                current_node = i;
                current_lower_distance = distance_frombase[i];
            }
        }
        
        if (current_lower_distance == numeric_limits<int>::max()) {
            break; // If no unvisited nodes with finite distance, exit
        }
    }

    current_lower_distance = distance_frombase[goal_vortex];

    // Print the shortest path if reachable
    if (current_lower_distance != numeric_limits<int>::max()) {
        cout << "Shortest path from " << base_vortex << " to " << goal_vortex << ": ";
        int trace_node = goal_vortex;
        while (trace_node != -1) {
            cout << trace_node;
            trace_node = predecessor[trace_node];
            if (trace_node != -1)
                cout << " <- ";
        }
        cout << endl;
    } else {
        cout << "No path from " << base_vortex << " to " << goal_vortex << " found." << endl;
    }

    delete[] distance_frombase;
    delete[] predecessor;
    delete[] visited;
    return current_lower_distance != numeric_limits<int>::max() ? current_lower_distance : -1;
}
