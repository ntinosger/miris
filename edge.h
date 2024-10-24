#ifndef EDGE_H
#define EDGE_H

typedef struct Edge {
    char* nodeFrom;
    char* nodeTo;
    double amount;
    char date[11];
    struct Edge* next;
} Edge;

size_t TOTAL_BYTES;

// Create the edge
Edge* create_edge (char* from, char* to, double amount, const char* date);

// Free the memory of the edge
void free_edge(Edge* edge);

#endif //EDGE_H
