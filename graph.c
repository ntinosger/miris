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

    graph->nodes = NULL;
    return graph;
}

// Function to add the node into the graph and return it
Node* add_node(Graph* graph, const char* id) {
    Node* newNode = create_node(id);
    newNode->next = graph->nodes;
    graph->nodes = newNode;
    return newNode;
}

// Function to remove the node from the graph and the helper hash table
void delete_node(Graph* graph, HashTable* hashTable, char* idToDelete) {
    Node* nodeToDelete = search_hash_table(hashTable, idToDelete);

    if (nodeToDelete == NULL) {
        printf("Non-existing node: %s\n", idToDelete);
        return;
    }

    // Delete incoming edges
    Node* tempNode = graph->nodes;
    while (tempNode != NULL) {
        delete_edge_from_node(tempNode, idToDelete);
        tempNode = tempNode->next;
    }

    // Delete outgoing edges
    Edge* edgeCurrent = nodeToDelete->edges;
    while (edgeCurrent != NULL) {
        Edge* edgeToDelete = edgeCurrent;
        edgeCurrent = edgeCurrent->next;
        free_edge(edgeToDelete);
    }

    // Unlink the node from the graph and point to the next node in the graph
    Node* previousNode = NULL;
    tempNode = graph->nodes;
    while (tempNode != NULL && tempNode != nodeToDelete) {
        previousNode = tempNode;
        tempNode = tempNode->next;
    }

    if (tempNode == nodeToDelete) {
        if (previousNode == NULL) {
            // The delete node is the first node in the list
            graph->nodes = nodeToDelete->next;
        } else {
            // Link the previous node to skip the delete node
            previousNode->next = nodeToDelete->next;
        }
    }

    // Remove the node from the hash table
    delete_from_hash_table(hashTable, nodeToDelete);

    // Free the memory of the delete node
    free(nodeToDelete);
}

// Function to create a new edge
void add_edge(const Graph* graph, HashTable* hashTable, char* fromNodeId, char* toNodeId, const double amount, const char* date) {
    Node* fromNode = search_hash_table(hashTable, fromNodeId);
    Edge* newEdge = create_edge(fromNodeId, toNodeId, amount, date);
    newEdge->next = fromNode->edges;
    fromNode->edges = newEdge;
}

// Function to modify an existing edge
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

    // Loop through the edges of the from Node to find the one that needs to change
    while (currentEdge != NULL) {
        if (strcmp(currentEdge->nodeTo, toNodeId) == 0 && currentEdge->amount == sum && strcmp(currentEdge->date, date) == 0) {
            currentEdge->amount = sum1;
            strcpy(currentEdge->date, date1);
            return;
        }
        currentEdge = currentEdge->next;
    }

    // The edge to be modified doesnt exist
    printf("Non-existing edge: %s %s %.2f %s\n", fromNodeId, toNodeId, sum, date);
}

// Function to find and print all the edges of a node
void find_all_edges(Graph* graph, HashTable* hashTable, char* nodeId) {
    Node* node = search_hash_table(hashTable, nodeId);
    if (node == NULL) {
        printf("Non-existing node: %s\n", nodeId);
        return;
    }

    Edge* currentEdge = node->edges;
    // The node doesnt have any edges
    if (currentEdge == NULL) {
        return;
    }

    // Print all edges of node
    while (currentEdge != NULL) {
        printf("%s %s %.2f %s\n", node->id, currentEdge->nodeTo, currentEdge->amount, currentEdge->date);
        currentEdge = currentEdge->next;
    }
}

// Function to find all incoming edges of a node
void find_all_incoming_edges(Graph* graph, HashTable* hashTable, char* nodeId) {
    Node* node = search_hash_table(hashTable, nodeId);
    if (node == NULL) {
        printf("Non-existing node: %s\n", nodeId);
        return;
    }

    // Loop through the nodes of the graph to find the edges pointing to node
    Node* currentNode = graph->nodes;
    while (currentNode != NULL) {
        Edge* currentEdge = currentNode->edges;
        while (currentEdge != NULL) {
            if (strcmp(currentEdge->nodeTo, nodeId) == 0) {
                printf("%s %s %.2f %s\n", currentNode->id, currentEdge->nodeTo, currentEdge->amount, currentEdge->date);
            }
            currentEdge = currentEdge->next;
        }
        currentNode = currentNode->next;
    }
}

// Function to remove an edge from a node
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

    // Loop through the edges of the node from
    while (currentEdge != NULL) {
        if (strcmp(currentEdge->nodeTo, toNodeId) == 0) {
            // Found the edge between the from node and the to node
            if (previousEdge == NULL) {
                fromNode->edges = currentEdge->next;
            } else {
                previousEdge->next = currentEdge->next;
            }
            
            // Free the edge
            free(currentEdge);  
            return;
        }
        previousEdge = currentEdge;
        currentEdge = currentEdge->next;
    }
}

// Helper function to remove all incoming edges
void delete_edge_from_node(Node* fromNode, char* toNodeId) {
    Edge* currentEdge = fromNode->edges;
    Edge* previousEdge = NULL;

    // The same code to remove an edge
    while (currentEdge != NULL && strcmp(currentEdge->nodeTo, toNodeId) != 0) {
        previousEdge = currentEdge;
        currentEdge = currentEdge->next;
    }

    if (currentEdge != NULL) {
        if (previousEdge == NULL) {
            fromNode->edges = currentEdge->next;
        } else {
            previousEdge->next = currentEdge->next;
        }
        free_edge(currentEdge);
    }
}

//  Helper function to print the cycle
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
        printArray[index] = currentNode->id;
        index++;
        currentNode = currentNode->next;
    }
    
    // Print the cycle
    for (int i = counter - 1; i >= 0; i--) {
        printf("%s, ", printArray[i]);
    }
    printf("%s\n", printArray[counter - 1]);

    free(printArray);
}

// Helper function to remove the node after the dfs exploration
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
            // The node is the first node in the list
            path->nodes = currentNode->next;
        } else {
            // Update the previous node to skip the node
            previousNode->next = currentNode->next;
        }
    }

    // Remove it also from the path hash table
    delete_from_hash_table(pathHT, nodeToDelete);

    // Free the node
    free(currentNode);
}

// Function to implement the dfs recursive algorithm to check for cycles
void dfs_check_cycle(HashTable* hashTable, Node* node, HashTable* visitedNodes, Graph* currentPath, HashTable* currentPathHT) {
    // Add current node in visited hash table, in the current path graph and in the current path hash table
    insert_to_hash_table(&currentPathHT, node);
    insert_to_hash_table(&visitedNodes, node);
    add_node(currentPath, node->id);

    Edge* edge = node->edges;
    while (edge != NULL) {
        Node* nextNode = search_hash_table(hashTable, edge->nodeTo);

        if (nextNode != NULL) {
            // Check if the next node is in the current path
            if (search_hash_table(currentPathHT, nextNode->id) != NULL) {
                // Cycle found
                print_cycle(currentPath);
            } else if (search_hash_table(visitedNodes, nextNode->id) == NULL) {
                // If not in visited hash table, continue DFS exploration
                dfs_check_cycle(hashTable, nextNode, visitedNodes, currentPath, currentPathHT);
            }
        }
        edge = edge->next;
    }

    // After the exploration, remove the node from the structs and free its memory
    delete_for_dfs(currentPath, currentPathHT, node->id);
    delete_from_hash_table(visitedNodes, node);
}

// Function for cycling detection
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

// Not used, slow search of the graph O(n)
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

// Helper function to print the graph
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

// Free the graph
void free_graph(Graph* graph) {
    TOTAL_BYTES -= sizeof(Graph);

    Node* currentNode = graph->nodes;
    while (currentNode != NULL) {
        Node* temp = currentNode;
        currentNode = currentNode->next;
        free_node(temp);
    }
    free(graph);
}
