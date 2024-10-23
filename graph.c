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

void modify_edge(Graph* graph, char* fromNodeId, char* toNodeId, const double sum, const double sum1, const char* date, const char* date1) {
    Node* fromNode = find_node(graph, fromNodeId);
    Node* toNode = find_node(graph, toNodeId);
    if (fromNode == NULL || toNode == NULL) {
        printf("Non-existing node(s): ");
        if (fromNode == NULL) {
            printf("%s ", fromNodeId);
        } 
        if (toNode == NULL) {
            printf("%s", toNodeId);
        }
        printf("\n");
        return;
    }

    Edge* currentEdge = fromNode->edges;

    // Iterate through the edges of fromNodeId to find the one pointing to toNodeId with the given sum and date
    while (currentEdge != NULL) {
        if (strcmp(currentEdge->nodeTo, toNodeId) == 0 && currentEdge->amount == sum && strcmp(currentEdge->date, date) == 0) {
            // Found the matching edge, modify it
            currentEdge->amount = sum1;
            strcpy(currentEdge->date, date1);
            printf("Modified edge from %s to %s with new sum: %.2f and new date: %s\n", fromNodeId, toNodeId, sum1, date1);
            return;
        }
        currentEdge = currentEdge->next;
    }

    // If we reached here, no matching edge was found
    printf("Non-existing edge: %s %s %.2f %s\n", fromNodeId, toNodeId, sum, date);
}

void find_all_edges(Graph* graph, char* nodeId) {
    // Find node nodeId in the graph
    Node* node = find_node(graph, nodeId);

    if (node == NULL) {
        printf("Non-existing node: %s\n", nodeId);
        return;
    }

    Edge* currentEdge = node->edges;

    // If there are no outgoing edges
    if (currentEdge == NULL) {
        return;
    }

    // Print all outgoing edges of node nodeId
    printf("Outgoing transactions for node %s:\n", nodeId);
    while (currentEdge != NULL) {
        printf("To: %s, Ammount: %.2f, Date: %s\n", currentEdge->nodeTo, currentEdge->amount, currentEdge->date);
        currentEdge = currentEdge->next;
    }
}

void find_all_incoming_edges(Graph* graph, char* nodeId) {
    // Find node with nodeId in the graph (to make sure it exists)
    Node* node = find_node(graph, nodeId);

    if (node == NULL) {
        printf("Non-existing node: %s\n", nodeId);
        return;
    }

    // Iterate through all nodes to find edges pointing to nodeId
    Node* currentNode = graph->nodes;
    while (currentNode != NULL) {
        Edge* currentEdge = currentNode->edges;
        while (currentEdge != NULL) {
            if (strcmp(currentEdge->nodeTo, nodeId) == 0) {  // Edge points to nodeId
                printf("From: %s, Sum: %.2f, Date: %s\n", currentNode->id, currentEdge->amount, currentEdge->date);
            }
            currentEdge = currentEdge->next;
        }
        currentNode = currentNode->next;
    }
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

    // Iterate through the edges of fromNodeId to find the one pointing to toNodeId
    while (currentEdge != NULL) {
        if (strcmp(currentEdge->nodeTo, toNodeId) == 0) {
            // Edge from fromNodeId to toNodeId found, remove it
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

    // If we reached here, no edge between fromNodeId and toNodeId was found
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
