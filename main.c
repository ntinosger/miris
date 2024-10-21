#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"

int main() {
    Graph* graph = create_graph();

    // Add some nodes (accounts)
    add_node(graph, 1, "Alice");
    add_node(graph, 2, "Bob");

    // Add some edges (transactions)
    struct tm date = {0};
    date.tm_year = 2023 - 1900; // Year since 1900
    date.tm_mon = 9 - 1;        // Month (0-11)
    date.tm_mday = 19;          // Day of the month

    add_edge(graph, 1, 2, 50.0, "2023-10-02");
    add_edge(graph, 2, 1, 30.0, "2023-10-03");

    // Print the graph
    print_graph(graph);

    // Free the memory
    free_graph(graph);

    return 0;
}
