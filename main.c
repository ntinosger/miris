#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "graph.h"
#include "files.h"
#include "hashTable.h"

int main(int argc, char *argv[]) {
    char* outputFilepath = NULL;

    // Create the structs
    Graph* graph = create_graph();
    HashTable* hashTable = create_hash_table();

    // The first item of *argv is the executable name, so the i start from 1
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            import_from_file(graph, &hashTable, argv[i + 1]);
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            outputFilepath = argv[i + 1];
        }
    }

    // The program runs until exit is pressed
    while (1) {
        printf("miris> ");

        char input[50];
        fgets(input, sizeof(input), stdin);
        // Remove the \n from the string to avoid errors
        input[strcspn(input, "\n")] = 0;

        // Get the first word (miris action)
        char* action = strtok(input, " ");

        if (strcmp(action, "i") == 0 || strcmp(action, "insert") == 0) {
            char* parameter = strtok(NULL, " ");
            while (parameter != NULL) {
                if (search_hash_table(hashTable, parameter) == NULL) {
                    Node* node = add_node(graph, parameter);
                    insert_to_hash_table(&hashTable, node);               
                    if (strcmp(graph->nodes->id, parameter) == 0) {
                        printf("Succ: %s\n", graph->nodes->id);
                    }
                } else {
                    printf("The node already exists\n");
                }
                parameter = strtok(NULL, " ");
            }
        } else if (strcmp(action, "n") == 0 || strcmp(action, "insert2") == 0) {
            // Get the from node
            char* nodeFromId = strtok(NULL, " ");
            // Get the to node
            char* nodeToId = strtok(NULL, " ");
            // Get the sum
            char* sumStr = strtok(NULL, " ");
            // Get the date
            char* dateStr = strtok(NULL, " ");

            if (nodeFromId && nodeToId && sumStr && dateStr) {
                // Convert sum to a float
                float sum = atof(sumStr);

                // Search for the nodes in the hash table
                Node* searchingNodeFrom = search_hash_table(hashTable, nodeFromId);
                Node* searchingNodeTo = search_hash_table(hashTable, nodeToId);

                // If the nodes don't exist, add them
                if (searchingNodeFrom == NULL) {
                    searchingNodeFrom = add_node(graph, nodeFromId);
                    insert_to_hash_table(&hashTable, searchingNodeFrom);
                }

                if (searchingNodeTo == NULL) {
                    searchingNodeTo = add_node(graph, nodeToId);
                    insert_to_hash_table(&hashTable, searchingNodeTo);
                }

                // Add the edge with the sum and date
                add_edge(graph, hashTable, searchingNodeFrom->id, searchingNodeTo->id, sum, dateStr);
            } else {
                printf("Format error: n Ni Nj sum date\n");
            }
        } else if (strcmp(action, "d") == 0 || strcmp(action, "delete") == 0) {
            char* parameter = strtok(NULL, " ");
            while (parameter != NULL) {
                delete_node(graph, hashTable, parameter);

                parameter = strtok(NULL, " ");
            }
        } else if (strcmp(action, "l") == 0 || strcmp(action, "delete2") == 0) { 
            char* fromNodeId = strtok(NULL, " ");
            char* toNodeId = strtok(NULL, " ");
            delete_edge(graph, hashTable, fromNodeId, toNodeId);

        } else if (strcmp(action, "m") == 0 || strcmp(action, "modify") == 0) {
            // Get the from node
            char* fromNodeId = strtok(NULL, " ");
            // Get the to node
            char* toNodeId = strtok(NULL, " ");
            // Get the sum
            char* sum = strtok(NULL, " ");
            // Get the sum1
            char* sum1 = strtok(NULL, " ");
            // Get the date
            char* date = strtok(NULL, " ");
            // Get the date1
            char* date1 = strtok(NULL, " ");

            if (fromNodeId && toNodeId && sum && sum1 && date && date1) {
                // Convert sums to a float
                float sumFloat = atof(sum);
                float sum1Float = atof(sum1);

                modify_edge(graph, hashTable, fromNodeId, toNodeId, sumFloat, sum1Float, date, date1);
            } else {
                printf("Format error: m Ni Nj sum sum1 date date1\n");
            }

        } else if (strcmp(action, "f") == 0 || strcmp(action, "find") == 0) {
            char* nodeId = strtok(NULL, " ");
            if (nodeId != NULL) {
                char* formatExtraParameter = strtok(NULL, " ");
                if (formatExtraParameter != NULL) {
                    printf("Format error: f Ni\n");
                    continue;
                }
                find_all_edges(graph, hashTable, nodeId);
            }
        } else if (strcmp(action, "r") == 0 || strcmp(action, "receiving") == 0) {

            char* nodeId = strtok(NULL, " ");
            if (nodeId != NULL) {
                char* formatExtraParameter = strtok(NULL, " ");
                if (formatExtraParameter != NULL) {
                    printf("Format error: r Ni\n");
                    continue;
                }
                find_all_incoming_edges(graph, hashTable, nodeId);
            }
        } else if (strcmp(action, "c") == 0 || strcmp(action, "circlefind") == 0) {

            char* nodeId = strtok(NULL, " ");
            if (nodeId != NULL) {
                char* formatExtraParameter = strtok(NULL, " ");
                if (formatExtraParameter != NULL) {
                    printf("Format error: c Ni\n");
                    continue;
                }
                find_circles(hashTable, nodeId);
            }
        } else if (strcmp(action, "e") == 0 || strcmp(action, "exit") == 0) {
            // Print the structs, for debugging only
            // print_graph(graph);
            // print_hash_table(hashTable);

            // Write to output file
            if (outputFilepath != NULL) {
                write_to_file(graph, outputFilepath);
            }

            printf("%zu Bytes released\n", TOTAL_BYTES);

            // Free the memory
            free_graph(graph);
            free_hash_table(hashTable);

            break;
        } else {
            printf("Unrecognized command\n");
        }
    }

    return 0;
}
