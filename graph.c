#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

Graph* create_graph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (graph == NULL) {
        fprintf(stderr, "Failed to allocate memory for graph.\n");
        exit(EXIT_FAILURE);
    }
    graph->nodes = NULL;
    return graph;
}

void add_node(Graph* graph, const int id, const char* name) {
    Node* new_node = create_node(id, name);
    new_node->next = graph->nodes;
    graph->nodes = new_node;
}

void add_edge(const Graph* graph, const int from, const int to, const double amount, const char* date) {
    Node* current_node = graph->nodes;
    while (current_node != NULL) {
        if (current_node->id == from) {
            Edge* new_edge = create_edge(from, to, amount, date);
            new_edge->next = current_node->edges;
            current_node->edges = new_edge;
            return;
        }
        current_node = current_node->next;
    }
    fprintf(stderr, "Source node not found in the graph.\n");
}

void print_graph(const Graph* graph) {
    Node* current_node = graph->nodes;
    while (current_node != NULL) {
        printf("Account %d (%s):\n", current_node->id, current_node->name);
        Edge* current_edge = current_node->edges;
        while (current_edge != NULL) {
            printf("  Transaction to Account %d: %.2f on %s\n", current_edge->nodeTo, current_edge->amount, current_edge->date);
            current_edge = current_edge->next;
        }
        current_node = current_node->next;
    }
}

void free_graph(Graph* graph) {
    Node* current_node = graph->nodes;
    while (current_node != NULL) {
        Node* temp = current_node;
        current_node = current_node->next;
        free_node(temp);
    }
    free(graph);
}
