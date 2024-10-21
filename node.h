#ifndef NODE_H
#define NODE_H

// This structure represents the account of the user
typedef struct Node {
    int id;
    char name[10];
    struct Edge* edges; // The edges starting from this node
    struct Node* next;
} Node;

// Create the node
Node* create_node(int id, const char* name);

// Free the memory of the node
void free_node(Node* node);

#endif //NODE_H
