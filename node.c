#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node.h"
#include "edge.h"

Node* create_node(const int id, const char* name) {
    Node* newNode = malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Failed to allocate memory for node.\n");
        exit(EXIT_FAILURE);
    }
    newNode->id = id;
    strcpy(newNode->name, name);
    newNode->edges = NULL;
    newNode->next = NULL;
    return newNode;
}

void free_node(Node* node) {
    // Free all edges of the node
    Edge* current_edge = node->edges;
    while (current_edge != NULL) {
        Edge* temp = current_edge;
        current_edge = current_edge->next;
        free(temp);
    }
    free(node);
}
