#ifndef GRAPH_H
#define GRAPH_H

/**
 * @file graph.h
 * @brief Graph class for representing an undirected graph using an adjacency linked list and implementing Dijkstra's algorithm for finding shortest paths.
 *
 * @author Fernando Elena Benavente
 * 
 * The purpose of this code is to learn C++ in a fun way, while creating useful code that can be reused in the future. This code is focused on implementing a graph generator and a Dijkstra algorithm pathfinder. Users can create their own graphs and find shortest paths or unreachable nodes in the generated graph.
 * 
 * The graph is implemented using an adjacency linked list where only existing edges are represented, stored on the lower index vortex (node), to save memory. This implementation is particularly efficient for sparse graphs, which are common in real-world applications such as representing city networks or social network contacts.
 * 
 * The implementation focuses on being as low-level as possible to optimize speed and memory usage, deliberately avoiding high-level C++ data structures like the `vector` class to maintain control over memory management and performance.
 */

#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <limits>

using namespace std;

/**
 * @struct edge
 * @brief Represents an edge in the graph.
 *
 * An edge connects two vertices (vortexes). It stores the index of the target vortex, the weight of the edge, and a pointer to the next edge in the list.
 */
typedef struct edge
{
    unsigned int vortex_index;  /**< Index of the target vortex in the graph. */
    unsigned int edge_weight;   /**< Weight of the edge connecting two vortexes. */
    struct edge *next;          /**< Pointer to the next edge in the list (adjacent edges). */
} edge;

/**
 * @struct vortex
 * @brief Represents a vortex (vertex) in the graph.
 *
 * A vortex is a node in the graph that can have multiple outgoing edges. It stores the vortex's index, a pointer to the first edge in the adjacency list, and a pointer to the next vortex in the list.
 */
typedef struct vortex
{
    unsigned int vortex_index;  /**< Index of the vortex in the graph. */
    edge *edge_ptr;             /**< Pointer to the first edge connected to this vortex. */
    struct vortex *next;        /**< Pointer to the next vortex in the graph. */

} vortex;

/**
 * @class list_graph
 * @brief Represents an undirected graph using an adjacency linked list structure.
 *
 * This class allows users to create and manipulate a graph, adding and removing edges, generating random edges, and applying Dijkstra's algorithm to find shortest paths between vortexes. The graph is represented by an adjacency list, where each vortex has a linked list of edges.
 */
class list_graph
{
public:
    /**
     * @brief Constructs a graph with a specified number of vortexes and a name.
     *
     * Initializes the graph with the specified number of vortexes, each with an empty adjacency list. The graph is named according to the provided string.
     * 
     * @param vortex_number Number of vortexes in the graph.
     * @param graph_name The name of the graph.
     */
    list_graph(int vortex_number, string graph_name);

    /**
     * @brief Destructor that frees dynamically allocated memory.
     *
     * The destructor deallocates memory for all vortexes and edges in the graph, ensuring proper cleanup.
     */
    ~list_graph();

    /**
     * @brief Prints the total number of vortexes in the graph.
     *
     * Displays the number of vortexes in the graph to the console.
     */
    void print_vortexnumber();

    /**
     * @brief Prints the edges of the graph in a readable format.
     *
     * Displays each edge in the graph, showing the vortexes it connects and the edge weights.
     */
    void print_graph_edges();

    /**
     * @brief Adds a new vortex to the graph.
     * 
     * Creates a new vortex with the given index and adds it to the graph. 
     * The vortex is added at the end of the list.
     * 
     * @param vortex_index The index of the new vortex.
     * @return An integer indicating success (0) or failure (-1).
     */
    int add_vortex(unsigned int vortex_index);

    /**
     * @brief Removes an existing vortex from the graph.
     * 
     * Removes a vortex and all its associated edges from the graph. 
     * The vortex is removed from the list, and the memory is freed.
     * 
     * @param vortex_index The index of the vortex to be removed.
     * @return An integer indicating success (0) or failure (-1).
     */
    int remove_vortex(unsigned int vortex_index);
    
    /**
     * @brief Adds an edge between two vortexes in the graph.
     *
     * Creates an edge between two vortexes with the specified weight. The edge is added to the adjacency list of the lower-index vortex.
     * 
     * @param vortex1 The index of the first vortex.
     * @param vortex2 The index of the second vortex.
     * @param weight The weight of the edge.
     * 
     * @return 0 on success, or an error code if the edge could not be added.
     */
    int add_edge(unsigned int vortex1, unsigned int vortex2, unsigned int weight);

    /**
     * @brief Removes an edge between two vortexes in the graph.
     *
     * Removes the edge connecting the two specified vortexes.
     * 
     * @param vortex1 The index of the first vortex.
     * @param vortex2 The index of the second vortex.
     * 
     * @return 0 on success, or an error code if the edge could not be removed.
     */
    int remove_edge(unsigned int vortex1, unsigned int vortex2);

    /**
     * @brief Generates random edges in the graph with a given probability.
     *
     * Randomly generates edges between vortexes in the graph, with the probability of each edge being created determined by the provided parameter.
     * 
     * @param cp_probability The probability (0 to 100) that each possible edge will be generated.
     */
    void generate_random_edges(unsigned int cp_probability);

    /**
     * @brief Finds the shortest path between two vortexes using Dijkstra's algorithm.
     *
     * Implements Dijkstra's algorithm to find the shortest path from a base vortex to a goal vortex in the graph. It computes the shortest distance and updates the distance array.
     * 
     * @param base_vortex The index of the starting vortex.
     * @param goal_vortex The index of the goal vortex.
     * 
     * @return 0 if successful, or an error code if the algorithm failed.
     */
    int search_shortest_distance_dijkstra(unsigned int base_vortex, unsigned int goal_vortex);

    /**
     * @brief Retrieves all reachable vortexes from a given base vortex.
     *
     * This function returns a dynamically allocated array where each element represents the reachability of a vortex from the base vortex. A value of 1 indicates the vortex is reachable, and 0 indicates it is not.
     * 
     * @param base_node The index of the base vortex.
     * 
     * @return A dynamically allocated array representing reachable vortexes.
     */
    int *get_full_reachable_vortexs(int base_node);

private:
    string graph_name;  /**< The name of the graph. */
    int vortex_number;  /**< The number of vortexes in the graph. */
    vortex *graph_head; /**< Pointer to the first vortex in the graph. */

    /**
     * @brief Private function to add an edge to a specific vortex.
     *
     * This low-level function adds an edge to the adjacency list of a given vortex.
     * 
     * @param Vortex The vortex to which the edge will be added.
     * @param vortex_index_to The index of the target vortex.
     * @param edge_weight The weight of the edge.
     */
    void add_edge_private(vortex &Vortex, unsigned int vortex_index_to, unsigned int edge_weight);

    /**
     * @brief Private function to remove an edge from a specific vortex.
     *
     * This low-level function removes an edge from the adjacency list of a given vortex.
     * 
     * @param Vortex The vortex from which the edge will be removed.
     * @param vortex_index_to The index of the target vortex.
     */
    void remove_edge_private(vortex &Vortex, unsigned int vortex_index_to);

    /**
     * @brief Finds all reachable vortexes from a given base vortex using backtracking.
     *
     * This function uses backtracking to explore all reachable nodes from a given base node and stores the results in the provided array.
     * 
     * @param ptr Pointer to an array where reachability results will be stored.
     * @param base_node The index of the base vortex.
     */
    void full_reachable_vortexs(int *ptr, unsigned int base_node);

    /**
     * @brief A helper function used in Dijkstra's algorithm to reach each vortex.
     *
     * This function is used in the Dijkstra algorithm to explore each reachable vortex from the current node, updating the distance array.
     * 
     * @param current_node The current vortex being processed.
     * @param current_distance_frombase The distance from the base node to the current vortex.
     * @param distance_array The array storing the shortest distances to each vortex.
     */
    void reach_vortex(unsigned int current_node, unsigned int current_distance_frombase, int *distance_array);

    /**
     * @brief Checks if all vortexes have been visited during Dijkstra's algorithm.
     *
     * This function checks whether all vortexes in the graph have been visited.
     * 
     * @param visited_vortex The array indicating whether each vortex has been visited.
     * 
     * @return 1 if all vortexes are visited, or 0 if some vortexes are unvisited.
     */
    int check_all_vortex_visited(int *visited_vortex);
};

#endif
