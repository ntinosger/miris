#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "edge.h"

Node* create_node(const char* id) {
    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Failed to allocate memory for node.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->id, id);
    newNode->edges = NULL;
    newNode->next = NULL;
    return newNode;
}

void free_node(Node* node) {
    // Free all edges of the node
    Edge* currentEdge = node->edges;
    while (currentEdge != NULL) {
        Edge* temp = currentEdge;
        currentEdge = currentEdge->next;
        free_edge(temp);
    }
    free(node);
}
