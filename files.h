#ifndef FILES_H
#define FILES_H

#include "graph.h"
#include "hashTable.h"

void import_from_file(Graph* graph, HashTable** HashTable, char* filepath);
void write_to_file(Graph* graph, char* filepath);

#endif //FILES_H