#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "edge.h"

size_t TOTAL_BYTES = 0;

Edge* create_edge(char* from, char* to, const double amount, const char* date) {
    Edge* newEdge = malloc(sizeof(Edge));
    if (newEdge == NULL) {
        fprintf(stderr, "Failed to allocate memory for edge.\n");
        exit(EXIT_FAILURE);
    }
    TOTAL_BYTES += sizeof(Edge);
    // printf("edge.c %zu\n", TOTAL_BYTES);
    newEdge->nodeFrom = from;
    newEdge->nodeTo = to;
    newEdge->amount = amount;
    strcpy(newEdge->date, date);
    newEdge->next = NULL;
    return newEdge;
}

void free_edge(Edge* edge) {
    free(edge);
}
