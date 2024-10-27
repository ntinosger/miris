#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include "edge.h"
#include "hashTable.h"

typedef struct Graph {
    Node* nodes; // Root of the linked list of nodes
} Graph;

Graph* create_graph();

Node* add_node(Graph* graph, const char* id);

void delete_node(Graph* graph, HashTable* hashTable, char* idToDelete);

Node* find_node(Graph *grpah, char *searchingId);

void add_edge(const Graph* graph, HashTable* hashTable, char* fromNodeId, char* toNodeId, double amount, const char* date);

void modify_edge(Graph* graph, HashTable* hashTable, char* fromNodeId, char* toNodeId, const double sum, const double sum1, const char* date, const char* date1);

void find_all_edges(Graph* graph, HashTable* hashTable, char* nodeId);

void find_all_incoming_edges(Graph* graph, HashTable* hashTable, char* nodeId);

void delete_edge(Graph* graph, HashTable* hashTable, char* fromNodeId, char* toNodeId);

void delete_edge_from_node(Node* fromNode, char* toNodeName);

void print_cycle(Graph* pathGraph);

void dfs_check_cycle(HashTable* hashTable, Node* node, HashTable* visitedNodes, Graph* currentPath, HashTable* currentPathHT);

void find_circles(HashTable* hashTable, char* id);

void print_graph(const Graph* graph);

void free_graph(Graph* graph);

#endif //GRAPH_H
