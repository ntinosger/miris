#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

Graph* create_graph() {
    Graph* graph = malloc(sizeof(Graph));
    if (graph == NULL) {
        fprintf(stderr, "Failed to allocate memory for the graph.\n");
        exit(EXIT_FAILURE);
    }
    TOTAL_BYTES += sizeof(Graph);
    // printf("graph.c %zu\n", TOTAL_BYTES);
    graph->nodes = NULL;
    return graph;
}

Node* add_node(Graph* graph, const char* id) {
    Node* newNode = create_node(id);
    newNode->next = graph->nodes;
    graph->nodes = newNode;
    return newNode;
}

void delete_node(Graph* graph, HashTable* hashTable, char* idToDelete) {
    Node* nodeToDelete = search_hash_table(hashTable, idToDelete);

    // Αν δεν βρεθεί ο κόμβος
    if (nodeToDelete == NULL) {
        printf("Non-existing node: %s\n", idToDelete);
        return;
    }

    Node* tempNode = graph->nodes;
    while (tempNode != NULL) {
        delete_edge_from_node(tempNode, idToDelete);
        tempNode = tempNode->next;
    }

    // Διαγραφή όλων των εξερχόμενων ακμών
    Edge* edgeCurrent = nodeToDelete->edges;
    while (edgeCurrent != NULL) {
        Edge* edgeToDelete = edgeCurrent;
        edgeCurrent = edgeCurrent->next;
        free_edge(edgeToDelete);
    }

    Node* previousNode = NULL;
    tempNode = graph->nodes;
    while (tempNode != NULL && tempNode != nodeToDelete) {
        previousNode = tempNode;
        tempNode = tempNode->next;
    }

    if (tempNode == nodeToDelete) {
        if (previousNode == NULL) {
            // nodeToDelete is the first node in the list
            graph->nodes = nodeToDelete->next;
        } else {
            // Update the previous node to skip nodeToDelete
            previousNode->next = nodeToDelete->next;
        }
    }

    // Remove the node from the hash table
    delete_from_hash_table(hashTable, nodeToDelete);

    free(nodeToDelete);
}

void add_edge(const Graph* graph, char* fromNodeId, char* toNodeId, const double amount, const char* date) {
    Node* currentNode = graph->nodes;
    while (currentNode != NULL) {
        if (strcmp(currentNode->id, fromNodeId) == 0) {
            Edge* newEdge = create_edge(fromNodeId, toNodeId, amount, date);
            newEdge->next = currentNode->edges;
            currentNode->edges = newEdge;
            return;
        }
        currentNode = currentNode->next;
    }
}

void modify_edge(Graph* graph, HashTable* hashTable, char* fromNodeId, char* toNodeId, const double sum, const double sum1, const char* date, const char* date1) {
    Node* fromNode = search_hash_table(hashTable, fromNodeId);
    Node* toNode = search_hash_table(hashTable, toNodeId);
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
            return;
        }
        currentEdge = currentEdge->next;
    }

    // If we reached here, no matching edge was found
    printf("Non-existing edge: %s %s %.2f %s\n", fromNodeId, toNodeId, sum, date);
}

void find_all_edges(Graph* graph, HashTable* hashTable, char* nodeId) {
    // Find node nodeId in the graph
    Node* node = search_hash_table(hashTable, nodeId);

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
    while (currentEdge != NULL) {
        printf("%s %s %.2f %s\n", node->id, currentEdge->nodeTo, currentEdge->amount, currentEdge->date);
        currentEdge = currentEdge->next;
    }
}

void find_all_incoming_edges(Graph* graph, HashTable* hashTable, char* nodeId) {
    // Find node with nodeId in the graph (to make sure it exists)
    Node* node = search_hash_table(hashTable, nodeId);

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
                printf("%s %s %.2f %s\n", currentNode->id, currentEdge->nodeTo, currentEdge->amount, currentEdge->date);
            }
            currentEdge = currentEdge->next;
        }
        currentNode = currentNode->next;
    }
}

void delete_edge(Graph* graph, HashTable* hashTable, char* fromNodeId, char* toNodeId) {
    Node* fromNode = search_hash_table(hashTable, fromNodeId);
    Node* toNode = search_hash_table(hashTable, toNodeId);
    
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
    Edge* previousEdge = NULL;

    // Iterate through the edges of fromNodeId to find the one pointing to toNodeId
    while (currentEdge != NULL) {
        if (strcmp(currentEdge->nodeTo, toNodeId) == 0) {
            // Edge from fromNodeId to toNodeId found, remove it
            if (previousEdge == NULL) {
                // Edge is the first in the adjacency list
                fromNode->edges = currentEdge->next;
            } else {
                // Edge is somewhere in the middle or at the end
                previousEdge->next = currentEdge->next;
            }
            printf("Deleted edge from %s to %s.\n", fromNodeId, toNodeId);
            free(currentEdge);  // Free the edge memory
            return;
        }
        previousEdge = currentEdge;
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

void print_cycle(Graph* pathGraph) {
    Node* currentNode = pathGraph->nodes;
    
    // Looping the first time to count the nodes in the graph
    int counter = 0;
    while (currentNode != NULL) {
        counter++;
        currentNode = currentNode->next;
    }

    currentNode = pathGraph->nodes;
    
    // Using an array to print the nodes in the reverse order
    char** printArray = malloc(sizeof(char*) * counter);
    if (printArray == NULL) {
        fprintf(stderr, "Failed to allocate memory for the print printArray.\n");
        return;
    }

    int index = 0;
    while (currentNode != NULL) {
        printArray[index] = currentNode->id;  // Store current node ID
        index++;
        currentNode = currentNode->next;
    }
    
    for (int i = counter - 1; i >= 0; i--) {
        printf("%s, ", printArray[i]);
    }
    printf("%s\n", printArray[counter - 1]);

    free(printArray);
}

void delete_for_dfs(Graph* path, HashTable* pathHT, char* idToDelete) {
    Node* nodeToDelete = search_hash_table(pathHT, idToDelete);
    if (nodeToDelete == NULL) {
        printf("Non-existing node: %s\n", idToDelete);
        return;
    }

    Node* currentNode = path->nodes;

    // This while is never used, it's just for error control
    Node* previousNode = NULL;
    while (currentNode != NULL && strcmp(currentNode->id, idToDelete) != 0) {
        previousNode = currentNode;
        currentNode = currentNode->next;
    }

    if (strcmp(currentNode->id, nodeToDelete->id) == 0) {
        if (previousNode == NULL) {
            // nodeToDelete is the first node in the list
            path->nodes = currentNode->next;
        } else {
            // Update the previous node to skip nodeToDelete
            previousNode->next = currentNode->next;
        }
    }

    delete_from_hash_table(pathHT, nodeToDelete);

    free(currentNode);
}


void dfs_check_cycle(HashTable* hashTable, Node* node, HashTable* visitedNodes, Graph* currentPath, HashTable* currentPathHT) {
    // Add current node in visited hash table and in the current path graph
    insert_to_hash_table(&currentPathHT, node);
    insert_to_hash_table(&visitedNodes, node);
    add_node(currentPath, node->id);

    Edge* edge = node->edges;
    while (edge != NULL) {
        Node* neighborNode = search_hash_table(hashTable, edge->nodeTo);

        if (neighborNode != NULL) {
            // Check if the neighbor is in the current path
            if (search_hash_table(currentPathHT, neighborNode->id) != NULL) {
                // Cycle detected
                print_cycle(currentPath);
            } else if (search_hash_table(visitedNodes, neighborNode->id) == NULL) {
                // If not visited, continue DFS
                dfs_check_cycle(hashTable, neighborNode, visitedNodes, currentPath, currentPathHT);
            }
        }
        edge = edge->next; // Move to the next edge
    }

    delete_for_dfs(currentPath, currentPathHT, node->id);
    delete_from_hash_table(visitedNodes, node);
}

void find_circles(HashTable* hashTable, char* id) {
    Node* node = search_hash_table(hashTable, id);
    if (node == NULL) {
        printf("Node %s not found in the graph.\n", id);
        return;
    }

    // Using both a graph and a hash table to track the current path
    Graph* currentPath = create_graph();
    HashTable* currentPathHT = create_hash_table();
    HashTable* visitedNodes = create_hash_table();

    dfs_check_cycle(hashTable, node, visitedNodes, currentPath, currentPathHT);

    // Free all the structs used for dfs
    free_hash_table(visitedNodes);
    free_hash_table(currentPathHT);
    free_graph(currentPath);
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
        Edge* currentEdge = currentNode->edges;
        while (currentEdge != NULL) {
            printf("%s %s %.2f %s\n", currentNode->id, currentEdge->nodeTo, currentEdge->amount, currentEdge->date);
            currentEdge = currentEdge->next;
        }
        if (currentEdge == NULL) {
            printf("%s\n", currentNode->id);
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
