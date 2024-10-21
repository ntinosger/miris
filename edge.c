#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edge.h"

Edge* create_edge(const int from, const int to, const double amount, const char* date) {
    Edge* new_edge = malloc(sizeof(Edge));
    if (new_edge == NULL) {
        fprintf(stderr, "Failed to allocate memory for edge.\n");
        exit(EXIT_FAILURE);
    }
    new_edge->nodeFrom = from;
    new_edge->nodeTo = to;
    new_edge->amount = amount;
    strcpy(new_edge->date, date);
    new_edge->next = NULL;
    return new_edge;
}

void free_edge(Edge* edge) {
    free(edge);
}
