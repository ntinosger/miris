#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"

// Create the node
Node* create_node(const char* id) {
    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Failed to allocate memory for node.\n");
        exit(EXIT_FAILURE);
    }
    TOTAL_BYTES += sizeof(Node);

    strcpy(newNode->id, id);
    newNode->edges = NULL;
    newNode->next = NULL;
    return newNode;
}

// Free the node
void free_node(Node* node) {
    TOTAL_BYTES -= sizeof(Node);

    // Free the edges of the node first
    Edge* currentEdge = node->edges;
    while (currentEdge != NULL) {
        Edge* tempEdge = currentEdge;
        currentEdge = currentEdge->next;
        free_edge(tempEdge);
    }
    free(node);
}
