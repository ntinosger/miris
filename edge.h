#ifndef EDGE_H
#define EDGE_H

typedef struct Edge {
    char* nodeFrom;
    char* nodeTo;
    double amount;
    char date[11];  // To be reviewed
    struct Edge* next;
} Edge;

// Create the edge
Edge* create_edge (char* from, char* to, double amount, const char* date);

// Free the memory of the edge
void free_edge(Edge* edge);

#endif //EDGE_H
