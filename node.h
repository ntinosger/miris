#ifndef NODE_H
#define NODE_H

#include "node.h"
#include "edge.h"

// This structure represents the account of the user
typedef struct Node {
    char id[10];
    struct Edge* edges; // The edges starting from this node
    struct Node* next;
} Node;

// Create the node
Node* create_node(const char* id);

// Free the memory of the node
void free_node(Node* node);

#endif //NODE_H
