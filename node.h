#ifndef NODE_H
#define NODE_H

#include "node.h"
#include "edge.h"

// This struct represents the account of the user
typedef struct Node {
    char id[10];                // The id is char
    struct Edge* edges;         // The edges starting from this node
    struct Node* next;
} Node;

Node* create_node(const char* id);

void free_node(Node* node);

#endif //NODE_H
