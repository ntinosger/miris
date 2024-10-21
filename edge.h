#ifndef EDGE_H
#define EDGE_H

#include <time.h>

typedef struct Edge {
    int nodeFrom;
    int nodeTo;
    double amount;
    char date[11];  // To be reviewed
    struct Edge* next;
} Edge;

// Create the edge
Edge* create_edge (int from, int to, double amount, const char* date);

// Free the memory of the edge
void free_edge(Edge* edge);

#endif //EDGE_H
