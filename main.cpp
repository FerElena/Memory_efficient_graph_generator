#include "graph.h"

// trivial testing main, atm just stupid machine
int main()
{
	cout << "Here goes the graph machine! \n";
	const int graph_size = 100;
	list_graph migrafo(graph_size, "grafo1");
	migrafo.print_vortexnumber();
	/*
	migrafo.add_edge(0, 1, 1);
	migrafo.add_edge(3, 1, 1);
	migrafo.add_edge(4, 3, 4);
	migrafo.add_edge(4, 5, 1);
	migrafo.add_edge(0, 2, 7);
	migrafo.add_edge(5, 2, 7);
	migrafo.print_graph_edges();*/

	migrafo.generate_random_edges(1);
	migrafo.print_graph_edges();
	int *ptr = migrafo.get_full_reachable_vortexs(0);

	for (int i = 0; i < graph_size; i++)
	{
		if (ptr[i] == -1)
		{
			cout << "vortex :" << i << " not accesible\n";
		}
	}

	int distance = migrafo.search_shortest_distance_dijkstra(0, 5);

	cout << "distance between base vortex and goal vortex is : " << distance << endl;
	return 0;
}
