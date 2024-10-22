#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graph.h"

int main(int argc, char *argv[]) {
    // for (int i = 0; i < argc; i++) {
    //     if (strcmp(argv[i], "exit") == 0) {
    //         exit(0);
    //     }
    // }

    Graph* graph = create_graph();

    printf("Waiting for input action, press 'exit' to terminate\n");
    char input[50];

    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    while (strcmp(input, "exit") != 0) {
        if (input[0] == 'i') {
            printf("Insert trying\n");

            char* inputParameters = input + 2;

            char* parameter = strtok(inputParameters, " ");
            while (parameter != NULL) {
                printf("parameter: %s\n", parameter);

                add_node(graph, parameter);

                parameter = strtok(NULL, " ");
            }
        } else if (input[0] == 'n') {
            char* inputParameters = input + 2;

            // Get the first node (Ni)
            char* nodeA = strtok(inputParameters, " ");
            // Get the second node (Nj)
            char* nodeB = strtok(NULL, " ");
            // Get the sum
            char* sumStr = strtok(NULL, " ");
            // Get the date
            char* dateStr = strtok(NULL, " ");

            if (nodeA && nodeB && sumStr && dateStr) {
                printf("NodeA: %s, NodeB: %s, Sum: %s, Date: %s\n", nodeA, nodeB, sumStr, dateStr);

                // Convert sum to a numerical value (assuming it's a float)
                float sum = atof(sumStr);

                // Search for both nodes in the graph
                Node* searchingNodeA = find_node(graph, nodeA);
                Node* searchingNodeB = find_node(graph, nodeB);

                // Check if the nodes exist, otherwise add them
                if (searchingNodeA == NULL) {
                    printf("Node %s does not exist. Adding to graph.\n", nodeA);
                    add_node(graph, nodeA);
                    searchingNodeA = find_node(graph, nodeA); // Find it again after insertion
                }

                if (searchingNodeB == NULL) {
                    printf("Node %s does not exist. Adding to graph.\n", nodeB);
                    add_node(graph, nodeB);
                    searchingNodeB = find_node(graph, nodeB); // Find it again after insertion
                }

                // Add an edge between the two nodes with the sum and date
                add_edge(graph, searchingNodeA->id, searchingNodeB->id, sum, dateStr);
                printf("Edge inserted between %s and %s with sum: %.2f on date: %s\n", nodeA, nodeB, sum, dateStr);
            } else {
                printf("Invalid input. Please provide nodes, sum, and date.\n");
            }
        }

        printf("Input: %s\n", input);
        printf("Waiting for input action, press 'exit' to terminate\n");

        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

    }




    // Add some nodes (accounts)
    add_node(graph, "C123");
    add_node(graph, "B234");

    // Add some edges (transactions)
    struct tm date = {0};
    date.tm_year = 2023 - 1900; // Year since 1900
    date.tm_mon = 9 - 1;        // Month (0-11)
    date.tm_mday = 19;          // Day of the month

    add_edge(graph, "C123", "B234", 50.0, "2023-10-02");
    add_edge(graph, "B234", "C123", 30.0, "2023-10-03");

    // Print the graph
    print_graph(graph);

    // Free the memory
    free_graph(graph);

    return 0;
}
