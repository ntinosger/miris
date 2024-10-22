#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include "edge.h"

typedef struct Graph {
    Node* nodes; // Head of the linked list of nodes
} Graph;

// Initialize a new graph
Graph* create_graph();

// Add a new node to the graph
void add_node(Graph* graph, const char* id);

// Check if node exists and return it, else return NULL
Node* find_node(Graph *grpah, char *searchingId);

// Add a new edge to the graph
void add_edge(const Graph* graph, char* fromNodeId, char* toNodeId, double amount, const char* date);

// Print the graph
void print_graph(const Graph* graph);

// Free the memory of the graph
void free_graph(Graph* graph);

#endif //GRAPH_H
