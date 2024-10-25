#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"

int HASH_TABLE_SIZE = 8;

HashTable* create_hash_table() {
    HashTable* newHT = malloc(sizeof(HashTable));
    if (newHT == NULL) {
        fprintf(stderr, "Failed to allocate memory for the hash table.\n");
        exit(EXIT_FAILURE);
    }
    TOTAL_BYTES += sizeof(HashTable);

    newHT->bucket = malloc(sizeof(HashNode*) * HASH_TABLE_SIZE);
    if (newHT->bucket == NULL) {
        fprintf(stderr, "Failed to allocate memory for the hash table buckets.\n");
        exit(EXIT_FAILURE);
    }
    TOTAL_BYTES += sizeof(HashNode*) * HASH_TABLE_SIZE;

    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        newHT->bucket[i] = NULL;
    }
    newHT->itemsCount = 0;
    newHT->tableSize = HASH_TABLE_SIZE;

    return newHT;
}

int hash_code(char* id) {
    int sumAsciiCodes = 0;
    for (int i = 0; id[i] != '\0'; i++) {
        sumAsciiCodes += id[i];
    }

    return sumAsciiCodes % HASH_TABLE_SIZE;
}

void insert_to_hash_table(HashTable** hashTable, Node* node) {
    int key = hash_code(node->id);

    if (search_hash_table(*hashTable, node->id) != NULL) {
        printf("The node: %s exists in the HT\n", node->id);
        return;
    }

    HashNode* newHN = malloc(sizeof(HashNode));
    if (newHN == NULL) {
        fprintf(stderr, "Failed to allocate memory for the hash node.\n");
        exit(EXIT_FAILURE);
    }
    TOTAL_BYTES += sizeof(HashNode);
    strcpy(newHN->id, node->id);
    newHN->node = node;
    newHN->next = (*hashTable)->bucket[key];
    (*hashTable)->bucket[key] = newHN;

    (*hashTable)->itemsCount++;

    float loadingFactor = (float)(*hashTable)->itemsCount / HASH_TABLE_SIZE;
    if (loadingFactor > 0.6) {
        double_hash_table(hashTable);
    }
}

Node* search_hash_table(HashTable* hashTable, char* id) {
    int key = hash_code(id);

    HashNode* bucketNode = hashTable->bucket[key];
    while (bucketNode != NULL) {
        if (strcmp(bucketNode->id, id) == 0) {
            return bucketNode->node;
        }
        bucketNode = bucketNode->next;
    }
    return NULL;
}

void double_hash_table(HashTable** hashTable) {
    HASH_TABLE_SIZE *= 2;

    HashTable* newHT = create_hash_table();

    for (int i = 0; i < HASH_TABLE_SIZE / 2; i++) {
        HashNode* bucketNode = (*hashTable)->bucket[i];
        while (bucketNode != NULL) {
            Node* tempNode = bucketNode->node;
            insert_to_hash_table(&newHT, tempNode);
            bucketNode = bucketNode->next;
        }
    }

    free_hash_table(*hashTable);

    // print_hash_table(newHT);
    printf("performing resizing\n");

    *hashTable = newHT;
    print_hash_table(*hashTable);
}

void delete_from_hash_table(HashTable* hashTable, Node* node) {
    int key = hash_code(node->id);

    if (search_hash_table(hashTable, node->id) == NULL) {
        printf("The node: %s does not exists in the HT\n", node->id);
        return;
    }

    HashNode* currentNode = hashTable->bucket[key];
    HashNode* previousNode = NULL;
     
    while (currentNode != NULL) {
        if (strcmp(currentNode->id, node->id) == 0) {
            // First node in the bucket
            if (previousNode == NULL) {
                hashTable->bucket[key] = currentNode->next;
            } else {
                previousNode->next = currentNode->next;
            }
            
            free(currentNode);
            hashTable->itemsCount--;
            printf("Node with ID %s deleted from hash table.\n", node->id);
            return;
        }

        previousNode = currentNode;
        currentNode = currentNode->next;
    }
    printf("Node with ID %s not found in hash table.\n", node->id);
}

void free_hash_table(HashTable* hashTable) {
    for (int i = 0; i < hashTable->tableSize; i++) {
        HashNode* bucketNode = hashTable->bucket[i];
        while (bucketNode != NULL) {
            HashNode* temp = bucketNode;
            bucketNode = bucketNode->next;
            free(temp);  // Free the bucketNode HashNode
        }
    }
    free(hashTable->bucket); // Free the array of buckets
    free(hashTable); 
}

void print_hash_table(HashTable* hashTable) {
    printf("Hash Table:\n");

    for (int i = 0; i < hashTable->tableSize; i++) {
        HashNode* bucketNode = hashTable->bucket[i];

        if (bucketNode != NULL) {
            printf("Bucket %d: ", i);
            while (bucketNode != NULL) {
                printf("[Key: %d, ID: %s] -> ", hash_code(bucketNode->id), bucketNode->id);
                bucketNode = bucketNode->next;
            }
            printf("NULL\n"); // End of the linked list in the bucket
        } else {
            printf("Bucket %d: Empty\n", i);
        }
    }
}