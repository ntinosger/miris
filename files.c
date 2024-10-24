#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"

void import_from_file(Graph* graph, char* filepath) {
    FILE* inputFile = fopen(filepath, "r");

    if (inputFile == NULL) {
        printf("File could not be opened\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
        printf("%s\n", line);
        line[strcspn(line, "\n")] = 0;

        // Get the first node (Ni)
        char* nodeFrom = strtok(line, " ");
        // Get the second node (Nj)
        char* nodeTo = strtok(NULL, " ");
        // Get the sum
        char* sumStr = strtok(NULL, " ");
        // Get the date
        char* dateStr = strtok(NULL, " ");

        float sum = atof(sumStr);

        Node* searchingNodeA = find_node(graph, nodeFrom);
        Node* searchingNodeB = find_node(graph, nodeTo);

        // Check if the nodes exist, otherwise add them
        if (searchingNodeA == NULL) {
            printf("Node %s does not exist. Adding to graph.\n", nodeFrom);                    add_node(graph, nodeFrom);
            searchingNodeA = find_node(graph, nodeFrom); // Find it again after insertion
        }

        if (searchingNodeB == NULL) {
            printf("Node %s does not exist. Adding to graph.\n", nodeTo);
            add_node(graph, nodeTo);
            searchingNodeB = find_node(graph, nodeTo); // Find it again after insertion
        }

        // Add an edge between the two nodes with the sum and date
        add_edge(graph, searchingNodeA->id, searchingNodeB->id, sum, dateStr);
        printf("Edge inserted between %s and %s with sum: %.2f on date: %s\n", nodeFrom, nodeTo, sum, dateStr);
    }

    fclose(inputFile);
}