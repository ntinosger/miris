#ifndef EDGE_H
#define EDGE_H

// This struct represents a transaction between two users
typedef struct Edge {
    char* nodeFrom;
    char* nodeTo;
    double amount;
    char date[11];
    struct Edge* next;
} Edge;

size_t TOTAL_BYTES;

Edge* create_edge (char* from, char* to, double amount, const char* date);

void free_edge(Edge* edge);

#endif //EDGE_H
