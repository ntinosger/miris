#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include "edge.h"
#include "hashTable.h"

typedef struct Graph {
    Node* nodes; // Head of the linked list of nodes
} Graph;

// Initialize a new graph
Graph* create_graph();

// Add a new node to the graph
Node* add_node(Graph* graph, const char* id);

// Delete a node
void delete_node(Graph* graph, HashTable* hashTable, char* idToDelete);

// Check if node exists and return it, else return NULL
Node* find_node(Graph *grpah, char *searchingId);

// Add a new edge to the graph
void add_edge(const Graph* graph, char* fromNodeId, char* toNodeId, double amount, const char* date);

void modify_edge(Graph* graph, HashTable* hashTable, char* fromNodeId, char* toNodeId, const double sum, const double sum1, const char* date, const char* date1);

void find_all_edges(Graph* graph, HashTable* hashTable, char* nodeId);

void find_all_incoming_edges(Graph* graph, HashTable* hashTable, char* nodeId);

void delete_edge(Graph* graph, HashTable* hashTable, char* fromNodeId, char* toNodeId);

// Delete the edge from the id
void delete_edge_from_node(Node* fromNode, const char* toNodeName);

// Print the graph
void print_graph(const Graph* graph);

// Free the memory of the graph
void free_graph(Graph* graph);

#endif //GRAPH_H
