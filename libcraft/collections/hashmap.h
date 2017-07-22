#ifndef LIBCRAFT_COLLECTIONS_HASHMAP_H
#define LIBCRAFT_COLLECTIONS_HASHMAP_H

struct hashmap;
typedef struct hashmap hashmap_t;

hashmap_t *hashmap_create(int (*compare)(void *, void *), void (*free_key)(void *));

void hashmap_destroy(hashmap_t *map);

float hashmap_get_usage(hashmap_t *map);

void hashmap_optimize(hashmap_t *map);

int hashmap_insert(hashmap_t *map, void *key, void *value);

int hashmap_remove(hashmap_t *map, void *key);

void *hashmap_get(hashmap_t *map, void *key);

#endif
