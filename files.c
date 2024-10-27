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

        // Get the from node
        char* nodeFromId = strtok(line, " ");
        // Get the to node
        char* nodeToId = strtok(NULL, " ");
        // Get the amount
        char* sumStr = strtok(NULL, " ");
        // Get the date
        char* dateStr = strtok(NULL, " ");

        float sum = atof(sumStr);

        // Search for the nodes in the hash table
        Node* searchingNodeFrom = search_hash_table((*hashTable), nodeFromId);
        Node* searchingNodeTo = search_hash_table((*hashTable), nodeToId);

        // If the nodes don't exist, add them
        if (searchingNodeFrom == NULL) {
            searchingNodeFrom = add_node(graph, nodeFromId);
            insert_to_hash_table(hashTable, searchingNodeFrom);
        }

        if (searchingNodeTo == NULL) {
            searchingNodeTo = add_node(graph, nodeToId);
            insert_to_hash_table(hashTable, searchingNodeTo);
        }


        // Add the edge with the sum and date
        add_edge(graph, *hashTable, searchingNodeFrom->id, searchingNodeTo->id, sum, dateStr);
        
    }

    fclose(inputFile);
}

void write_to_file(Graph* graph, char* filepath) {
    FILE* outputFile = fopen(filepath, "w");

    if (outputFile == NULL) {
        printf("File could not be created\n");
        return;
    }

    // Write the graph into the file
    Node* currentNode = graph->nodes;
    while (currentNode != NULL) {
        Edge* currentEdge = currentNode->edges;
        if (currentEdge == NULL) {
            fprintf(outputFile, "%s\n", currentNode->id);
        }
        while (currentEdge != NULL) {
            fprintf(outputFile, "%s %s %.2f %s\n", currentNode->id, currentEdge->nodeTo, currentEdge->amount, currentEdge->date);
            currentEdge = currentEdge->next;
        }
        currentNode = currentNode->next;
    }

    fclose(outputFile);
}