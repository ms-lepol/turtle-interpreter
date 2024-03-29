#include "hashmap_procvar.h"
#include <math.h>
#include <stdlib.h>

//Create a new hashmap
hashmap_procvar_t *hashmap_procvar_create(size_t capacity) {
    //Allocate memory for the hashmap
    hashmap_procvar_t *hashmap = malloc(sizeof(hashmap_procvar_t));
    hashmap->buckets = calloc(capacity, sizeof(hashmap_procvar_entry_t *));
    hashmap->len = 0;
    hashmap->capacity = capacity;
    return hashmap;
}

//DJB2 hash function - 
unsigned long hashmap_procvar_hash(const char *key) {
    //DJB2 hash function
    unsigned long hash = 5381; //Initial value of the hash function 5381 because it is a prime number and it is used in the original implementation - from http://www.cse.yorku.ca/~oz/hash.html
    int c;
    //Iterate over the key and calculate the hash
    while ((c = *key++)) {
        //hash * 33 + c
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

//Get the value of a key in the hashmap
struct ast_node* hashmap_procvar_get_proc(hashmap_procvar_t *hashmap, const char *key) {
    //Calculate the hash of the key
    unsigned long hash = hashmap_procvar_hash(key) % hashmap->capacity;
    //Iterate over the linked list in the bucket
    hashmap_procvar_entry_t *entry = hashmap->buckets[hash];
    while (entry) {
        //If the key is found, return the value
        if (strcmp(entry->key, key) == 0) {
            return entry->data.ast_node;
        }
        entry = entry->next;
    }
    //If the key is not found, return NULL
    return NULL;
}

double hashmap_procvar_get_var(const hashmap_procvar_t *hashmap, const char *key) {
    //Calculate the hash of the key
    unsigned long hash = hashmap_procvar_hash(key) % hashmap->capacity;
    //Iterate over the linked list in the bucket
    hashmap_procvar_entry_t *entry = hashmap->buckets[hash];
    while (entry) {
        //If the key is found, return the value
        if (strcmp(entry->key, key) == 0) {
            return entry->data.d;
        }
        entry = entry->next;
    }
    //If the key is not found, return 0
    return NAN;
}

//Set the value of a key in the hashmap
void hashmap_procvar_set_proc(hashmap_procvar_t *hashmap, const char *key, struct ast_node *node) {
    //Calculate the hash of the key
    unsigned long hash = hashmap_procvar_hash(key) % hashmap->capacity;
    //Iterate over the linked list in the bucket
    hashmap_procvar_entry_t *entry = hashmap->buckets[hash];
    while (entry) {
        //If the key is found, update the value
        if (strcmp(entry->key, key) == 0) {
            entry->data.ast_node = node;
            return;
        }
        entry = entry->next;
    }
    //If the key is not found, add it to the hashmap
    //Create a new entry
    entry = malloc(sizeof(hashmap_procvar_entry_t));
    entry->key = strdup(key);
    entry->data.ast_node = node;
    entry->next = hashmap->buckets[hash];
    hashmap->buckets[hash] = entry;
    hashmap->len++;
    //Grow the hashmap if it is full
    if (hashmap->len == hashmap->capacity) {
        hashmap_procvar_grow(hashmap);
    }
}

void hashmap_procvar_set_var(hashmap_procvar_t *hashmap, const char *key, double number) {
    //Calculate the hash of the key
    unsigned long hash = hashmap_procvar_hash(key) % hashmap->capacity;
    //Iterate over the linked list in the bucket
    hashmap_procvar_entry_t *entry = hashmap->buckets[hash];
    while (entry) {
        //If the key is found, update the value
        if (strcmp(entry->key, key) == 0) {
            entry->data.d = number;
            return;
        }
        entry = entry->next;
    }
    //If the key is not found, add it to the hashmap
    //Create a new entry
    entry = malloc(sizeof(hashmap_procvar_entry_t));
    entry->key = strdup(key);
    entry->data.d = number;
    entry->next = hashmap->buckets[hash];
    hashmap->buckets[hash] = entry;
    hashmap->len++;
    //Grow the hashmap if it is full
    if (hashmap->len == hashmap->capacity) {
        hashmap_procvar_grow(hashmap);
    }
}

//Destroy the hashmap
void hashmap_procvar_destroy(hashmap_procvar_t *hashmap) {
    //Iterate over the buckets and free the entries
    for (size_t i = 0; i < hashmap->capacity; i++) {
        hashmap_procvar_entry_t *entry = hashmap->buckets[i];
        //Iterate over the linked list in the bucket
        while (entry) {
            hashmap_procvar_entry_t *next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }
    //Free the buckets and the hashmap
    free(hashmap->buckets);
    free(hashmap);
}

void hashmap_procvar_grow(hashmap_procvar_t *hashmap) {
    //Create a new hashmap with double the capacity
    hashmap_procvar_t *new_hashmap = hashmap_procvar_create(hashmap->capacity * 2);
    for (size_t i = 0; i < hashmap->capacity; i++) {
        //Iterate over the linked list in the bucket
        hashmap_procvar_entry_t *entry = hashmap->buckets[i];
        while (entry) {
            if (entry->data.ast_node) {
                //Add the entry to the new hashmap and free the old entry
                hashmap_procvar_set_proc(new_hashmap, entry->key, entry->data.ast_node);
            }
            else {
                hashmap_procvar_set_var(new_hashmap, entry->key, entry->data.d);
            }
            hashmap_procvar_entry_t *next = entry->next;
            free(entry->key);
            free(entry);
            entry = next;
        }
    }
    //Swap the old hashmap with the new one and free the old one
    free(hashmap->buckets);
    hashmap->buckets = new_hashmap->buckets;
    hashmap->len = new_hashmap->len;
    hashmap->capacity = new_hashmap->capacity;
    free(new_hashmap);
}

//Dump the keys in the hashmap
void hashmap_procvar_dump(hashmap_procvar_t *hashmap) {
    //Iterate over the buckets and print the keys
    for (size_t i = 0; i < hashmap->capacity; i++) {
        //Iterate over the linked list in the bucket
        hashmap_procvar_entry_t *entry = hashmap->buckets[i];
        //Iterate over the linked list in the bucket
        while (entry) {
            //Print the key
            printf("%s\n", entry->key);
            entry = entry->next;
            
        }
    }
}


