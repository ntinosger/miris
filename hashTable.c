#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"

// Starting size of 8 items in the hash table
int HASH_TABLE_SIZE = 8;

// Create the hash table
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

// Function to perform the hashing of an id using its ASCII code because the IDs are char (not int)
int hash_code(char* id) {
    int sumAsciiCodes = 0;
    for (int i = 0; id[i] != '\0'; i++) {
        sumAsciiCodes += id[i];
    }

    return sumAsciiCodes % HASH_TABLE_SIZE;
}

// Function to insert a new node into the hashing table
void insert_to_hash_table(HashTable** hashTable, Node* node) {
    // Get the hash code
    int key = hash_code(node->id);

    // Check if the node already exists in the hash table to avoid double insert
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
    
    // Insert the values in the new HashNode
    strcpy(newHN->id, node->id);
    newHN->node = node;
    newHN->next = (*hashTable)->bucket[key];

    // Insert the HashNode in the right bucket using the hash key
    (*hashTable)->bucket[key] = newHN;

    // Increase the counter
    (*hashTable)->itemsCount++;

    // Check if resizing is needed
    float loadingFactor = (float)(*hashTable)->itemsCount / HASH_TABLE_SIZE;
    if (loadingFactor > 0.6) {
        double_hash_table(hashTable);
    }
}

// Function to search in the hash table
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

// Function to double the size of the hash table
void double_hash_table(HashTable** hashTable) {
    HASH_TABLE_SIZE *= 2;

    HashTable* newHT = create_hash_table();

    // Copy the old nodes into the new Hash Table 
    for (int i = 0; i < HASH_TABLE_SIZE / 2; i++) {
        HashNode* bucketNode = (*hashTable)->bucket[i];
        while (bucketNode != NULL) {
            Node* tempNode = bucketNode->node;
            insert_to_hash_table(&newHT, tempNode);
            bucketNode = bucketNode->next;
        }
    }

    // Free the old hash table memory
    free_hash_table(*hashTable);

    // Point the old hash table to the new
    *hashTable = newHT;
}

// Function to remove a node from the hash table
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
            if (previousNode == NULL) {
                hashTable->bucket[key] = currentNode->next;
            } else {
                previousNode->next = currentNode->next;
            }
            
            free(currentNode);
            TOTAL_BYTES -= sizeof(HashNode);
            hashTable->itemsCount--;
            return;
        }

        previousNode = currentNode;
        currentNode = currentNode->next;
    }
}

// Free the hash table
void free_hash_table(HashTable* hashTable) {
    for (int i = 0; i < hashTable->tableSize; i++) {
        HashNode* bucketNode = hashTable->bucket[i];
        while (bucketNode != NULL) {
            HashNode* tempNode = bucketNode;
            bucketNode = bucketNode->next;
            // Free the bucketNode HashNode
            free(tempNode);
            TOTAL_BYTES -= sizeof(HashNode);  
        }
    }
    
    // Free the array of buckets
    free(hashTable->bucket); 
    TOTAL_BYTES -= sizeof(HashNode*) * hashTable->tableSize;
    free(hashTable);
    TOTAL_BYTES -= sizeof(HashTable);
}

// Helper function to print the hash table
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