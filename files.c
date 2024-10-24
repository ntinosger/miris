#include <stdio.h>
#include <stdlib.h>

#include "files.h"

void import_from_file(Graph* graph, char* filepath) {
    FILE* inputFile = fopen(filepath, "r");

    if (inputFile == NULL) {
        printf("File could not be opened\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), inputFile)) {
        printf("%s\n");
    }

    fclose(inputFile);
}