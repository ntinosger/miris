#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "files.h"

void import_from_file(Graph* graph, HashTable** hashTable, char* filepath) {
    FILE* inputFile = fopen(filepath, "r");

    if (inputFile == NULL) {
        printf("File could not be opened\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
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
            add_node(graph, nodeFrom);
            insert_to_hash_table(hashTable, graph->nodes);
            searchingNodeA = find_node(graph, nodeFrom); // Find it again after insertion
        }

        if (searchingNodeB == NULL) {
            add_node(graph, nodeTo);
            insert_to_hash_table(hashTable, graph->nodes);
            searchingNodeB = find_node(graph, nodeTo); // Find it again after insertion
        }

        // Add an edge between the two nodes with the sum and date
        add_edge(graph, searchingNodeA->id, searchingNodeB->id, sum, dateStr);
        
    }

    fclose(inputFile);
}

void write_to_file(Graph* graph, char* filepath) {
    FILE* outputFile = fopen(filepath, "w");

    if (outputFile == NULL) {
        printf("File could not be created\n");
        return;
    }

    Node* currentNode = graph->nodes;
    while (currentNode != NULL) {
        Edge* currentEdge = currentNode->edges;
        while (currentEdge != NULL) {
            fprintf(outputFile, "%s %s %.2f %s\n", currentNode->id, currentEdge->nodeTo, currentEdge->amount, currentEdge->date);
            currentEdge = currentEdge->next;
        }
                if (currentEdge == NULL) {
            fprintf(outputFile, "%s\n", currentNode->id);
        }
        currentNode = currentNode->next;
    }

    fclose(outputFile);
}