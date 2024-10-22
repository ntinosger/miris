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

void delete_node(Graph* graph, const char* idToDelete) {
    Node* nodeToDelete = graph->nodes;
    Node* prev = NULL;
    
    // Βρες τον κόμβο προς διαγραφή
    while (nodeToDelete != NULL && strcmp(nodeToDelete->id, idToDelete) != 0) {
        prev = nodeToDelete;
        nodeToDelete = nodeToDelete->next;
    }

    // Αν δεν βρεθεί ο κόμβος
    if (nodeToDelete == NULL) {
        printf("Node %s does not exist.\n", idToDelete);
        return;
    }

    Node* temp = graph->nodes;
    while (temp != NULL) {
        delete_edge_from_node(temp, idToDelete);
        temp = temp->next;
    }

    // Διαγραφή όλων των εξερχόμενων ακμών
    Edge* edgeCurrent = nodeToDelete->edges;
    while (edgeCurrent != NULL) {
        Edge* edgeToDelete = edgeCurrent;
        edgeCurrent = edgeCurrent->next;
        free_edge(edgeToDelete);
    }

    // Διαγραφή του κόμβου από τη λίστα κόμβων
    if (prev == NULL) {
        // Ο κόμβος είναι ο πρώτος στη λίστα
        graph->nodes = nodeToDelete->next;
    } else {
        prev->next = nodeToDelete->next;
    }

    // Απελευθέρωση μνήμης του κόμβου
    free(nodeToDelete);
    printf("Node %s deleted successfully.\n", idToDelete);
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

void delete_edge(Graph* graph ,char* fromNodeId, char* toNodeId) {
    Node* fromNode = find_node(graph, fromNodeId);
    Node* toNode = find_node(graph, toNodeId);
    
    if (fromNode == NULL) {
        printf("The node %s does not exist\n", fromNodeId);
    }

    if (toNode == NULL) {
        printf("The node %s does not exist\n", toNodeId);
    }

    Edge* currentEdge = fromNode->edges;
    Edge* prevEdge = NULL;

    // Iterate through the edges of Ni to find the one pointing to Nj
    while (currentEdge != NULL) {
        if (strcmp(currentEdge->nodeTo, toNodeId) == 0) {
            // Edge from Ni to Nj found, remove it
            if (prevEdge == NULL) {
                // Edge is the first in the adjacency list
                fromNode->edges = currentEdge->next;
            } else {
                // Edge is somewhere in the middle or at the end
                prevEdge->next = currentEdge->next;
            }
            printf("Deleted edge from %s to %s.\n", fromNodeId, toNodeId);
            free(currentEdge);  // Free the edge memory
            return;
        }
        prevEdge = currentEdge;
        currentEdge = currentEdge->next;
    }

    // If we reached here, no edge between Ni and Nj was found
    printf("No edge found between %s and %s.\n", fromNodeId, toNodeId);
}

void delete_edge_from_node(Node* fromNode, const char* toNodeName) {
    Edge* current = fromNode->edges;
    Edge* prev = NULL;

    while (current != NULL && strcmp(current->nodeTo, toNodeName) != 0) {
        prev = current;
        current = current->next;
    }

    if (current != NULL) {
        if (prev == NULL) {
            fromNode->edges = current->next;
        } else {
            prev->next = current->next;
        }
        free_edge(current);
        printf("Edge to %s deleted.\n", toNodeName);
    }
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
