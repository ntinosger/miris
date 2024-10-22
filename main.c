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

    printf("Waiting for input action, press exit to terminate\n");
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
