#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

#include <string.h>

Graph* create_graph() {
    Graph* graph = malloc(sizeof(Graph));
    if (graph == NULL) {
        fprintf(stderr, "Failed to allocate memory for graph.\n");
        exit(EXIT_FAILURE);
    }
    graph->nodes = NULL;
    return graph;
}

void add_node(Graph* graph, const char* id) {
    Node* tempNode = create_node(id);
    tempNode->next = graph->nodes;
    graph->nodes = tempNode;
}

void add_edge(const Graph* graph, char* fromNodeId, char* toNodeId, const double amount, const char* date) {
    Node* currentNode = graph->nodes;
    while (currentNode != NULL) {
        if (strcmp(currentNode->id, fromNodeId) == 0) {
            Edge* new_edge = create_edge(fromNodeId, toNodeId, amount, date);
            new_edge->next = currentNode->edges;
            currentNode->edges = new_edge;
            return;
        }
        currentNode = currentNode->next;
    }
    fprintf(stderr, "Source node not found in the graph.\n");
}

Node* find_node(Graph *graph, char *searchingId) {
    Node* firstNode = graph->nodes;
    while (firstNode != NULL) {
        if (strcmp(firstNode->id, searchingId) == 0) {
            return firstNode;
        }
        firstNode = firstNode->next;
    }
    return NULL;
}

void print_graph(const Graph* graph) {
    printf("in print_graph\n");
    Node* currentNode = graph->nodes;
    while (currentNode != NULL) {
        printf("Account (%s):\n", currentNode->id);
        Edge* current_edge = currentNode->edges;
        while (current_edge != NULL) {
            printf("  Transaction to Account %s: %.2f on %s\n", current_edge->nodeTo, current_edge->amount, current_edge->date);
            current_edge = current_edge->next;
        }
        currentNode = currentNode->next;
    }
}

void free_graph(Graph* graph) {
    Node* currentNode = graph->nodes;
    while (currentNode != NULL) {
        Node* temp = currentNode;
        currentNode = currentNode->next;
        free_node(temp);
    }
    free(graph);
}
