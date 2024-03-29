#ifndef TURTLE_HASHMAP_PROCVAR_H
#define TURTLE_HASHMAP_PROCVAR_H
#include <malloc.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Hashmap for procedure & variables entry
typedef struct pair {
    char *key;
    union {
        double d;
        struct ast_node *ast_node;
    } data;
    struct pair *next;
} hashmap_procvar_entry_t;

// Hashmap for procedure & variables
typedef struct {
    hashmap_procvar_entry_t **buckets;
    size_t len;
    size_t capacity;
} hashmap_procvar_t;

unsigned long hashmap_procvar_hash(const char *key);
hashmap_procvar_t *hashmap_procvar_create(size_t capacity);
void hashmap_procvar_destroy(hashmap_procvar_t *map);
void hashmap_procvar_set_proc(hashmap_procvar_t *map, const char *key, struct ast_node *node);
void hashmap_procvar_set_var(hashmap_procvar_t *map, const char *key, double value);
void hashmap_procvar_grow(hashmap_procvar_t *map);
struct ast_node *hashmap_procvar_get_proc(hashmap_procvar_t *map,const char *key);
double hashmap_procvar_get_var(const hashmap_procvar_t *hashmap, const char *key);
void hashmap_procvar_remove(hashmap_procvar_t *map, char *key);
void hashmap_procvar_dump(hashmap_procvar_t *map);

#endif /* TURTLE_HASHMAP_PROCVAR_H */