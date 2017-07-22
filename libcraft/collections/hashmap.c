#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <libcraft/collections/hashmap.h>
#include <libcraft/log.h>

struct hashmap_entry;
typedef struct hashmap_entry hashmap_entry_t;
struct hashmap_entry {
    void *key;
    void *value;
    hashmap_entry_t *above;
    hashmap_entry_t *below;
};

struct hashmap {
    size_t size;
    size_t branches;
    hashmap_entry_t *root;
    int (*compare)(void *, void *);
    void (*free_key)(void *);
};

void hashmap_destroy_entry(hashmap_t *map, hashmap_entry_t *entry);
int hashmap_insert_entry(hashmap_t *map, hashmap_entry_t *branch, hashmap_entry_t *leaf, int depth);
int hashmap_remove_entry(hashmap_t *map, hashmap_entry_t **branch, void *key);
void *hashmap_get_entry(hashmap_t *map, hashmap_entry_t *branch, void *key);

hashmap_t *hashmap_create(int (*compare)(void *, void *), void (*free_key)(void *)) {
    if (compare && free_key) {
        hashmap_t *map = (hashmap_t *) malloc(sizeof(hashmap_t));
        if (map) {
            map->size = 0;
            map->branches = 0;
            map->root = NULL;
            map->compare = compare;
            map->free_key = free_key;
        } else {
            ERROR(ENOBUFS);
        }
        return map;
    } else {
        ERROR(EINVAL);
        return NULL;
    }
}

void hashmap_destroy(hashmap_t *map) {
    if (map) {
        if (map->root) {
            hashmap_destroy_entry(map, map->root);
        }
        free(map);
    } else {
        ERROR(EINVAL);
    }
}

void hashmap_destroy_entry(hashmap_t *map, hashmap_entry_t *entry) {
    if (map && entry) {
        if (entry->above) {
            hashmap_destroy_entry(map, entry->above);
        }
        if (entry->below) {
            hashmap_destroy_entry(map, entry->below);
        }
        map->free_key(entry->key);
        free(entry);
    } else {
        ERROR(EINVAL);
    }
}

float hashmap_get_usage(hashmap_t *map) {
    if (map) {
        if (map->size) {
            return ((float) map->size) / exp2f(map->branches);
        } else {
            return 1.f;
        }
    } else {
        ERROR(EINVAL);
        return 1.f;
    }
}

void hashmap_optimize(hashmap_t *map) {
    NOT_IMPLEMENTED(hashmap_optimize)
}

int hashmap_insert(hashmap_t *map, void *key, void *value) {
    if (map) {
        hashmap_entry_t *leaf = (hashmap_entry_t *) malloc(sizeof(hashmap_entry_t));
        if (leaf) {
            leaf->key = key;
            leaf->value = value;
            leaf->above = NULL;
            leaf->below = NULL;
            if (map->root) {
                return hashmap_insert_entry(map, map->root, leaf, 2);
            } else {
                map->root = leaf;
                map->branches = 1;
                return 0;
            }
        } else {
            return ERROR(ENOBUFS);
        }
    } else {
        return ERROR(EINVAL);
    }
}

int hashmap_insert_entry(hashmap_t *map, hashmap_entry_t *branch, hashmap_entry_t *leaf, int depth) {
    if (map && branch && leaf && depth > 0) {
        int diff = map->compare(branch->key, leaf->key);
        if (diff < 0) {
            if (branch->below) {
                return hashmap_insert_entry(map, branch->below, leaf, depth + 1);
            } else {
                branch->below = leaf;
            }
        } else if (diff == 0) {
            return ERROR(EISCONN);
        } else if (branch->above) {
            return hashmap_insert_entry(map, branch->above, leaf, depth + 1);
        } else {
            branch->above = leaf;
        }
        if (depth > map->branches) {
            map->branches = depth;
        }
        return 0;
    } else {
        return ERROR(EINVAL);
    }
}

int hashmap_remove(hashmap_t *map, void *key) {
    if (map) {
        if (map->root) {
            return hashmap_remove_entry(map, &map->root, key);
        } else {
            return ERROR(ENOTCONN);
        }
    } else {
        return ERROR(EINVAL);
    }
}

int hashmap_remove_entry(hashmap_t *map, hashmap_entry_t **branch, void *key) {
    if (map && branch && *branch) {
        hashmap_entry_t *entry = *branch;
        int diff = map->compare(entry->key, key);
        if (diff < 0) {
            if (entry->below) {
                return hashmap_remove_entry(map, &entry->below, key);
            } else {
                return ERROR(ENOTCONN);
            }
        } else if (diff == 0) {
            if (entry->above) {
                *branch = entry->above;
                if (entry->below) {
                    hashmap_entry_t *lowest = entry->above;
                    while (lowest->below) {
                        lowest = lowest->below;
                    }
                    lowest->below = entry->below;
                }
            } else if (entry->below) {
                *branch = entry->below;
            } else {
                *branch = NULL;
            }
            map->free_key(entry->key);
            free(entry);
            --map->size;
            return 0;
        } else if (entry->above) {
            return hashmap_remove_entry(map, &entry->above, key);
        } else {
            return ERROR(ENOTCONN);
        }
    } else {
        return ERROR(EINVAL);
    }
}

void *hashmap_get(hashmap_t *map, void *key) {
    if (map) {
        if (map->root) {
            return hashmap_get_entry(map, map->root, key);
        } else {
            ERROR(ENOTCONN);
            return NULL;
        }
    } else {
        ERROR(EINVAL);
        return NULL;
    }
}

void *hashmap_get_entry(hashmap_t *map, hashmap_entry_t *branch, void *key) {
    if (map && branch) {
        int diff = map->compare(branch->key, key);
        if (diff < 0) {
            if (branch->below) {
                return hashmap_get_entry(map, branch->below, key);
            } else {
                ERROR(ENOTCONN);
                return NULL;
            }
        } else if (diff == 0) {
            return branch->value;
        } else if (branch->above) {
            return hashmap_get_entry(map, branch->above, key);
        } else {
            ERROR(ENOTCONN);
            return NULL;
        }
    } else {
        ERROR(EINVAL);
        return NULL;
    }
}
