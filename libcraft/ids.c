#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <libcraft/collections/hashmap.h>
#include <libcraft/ids.h>
#include <libcraft/log.h>

#define HASHMAP_EFFICIENCY_TOLERANCE 0.7f

hashmap_t *objects;
#ifndef HAVE_UINT_128
craft_id_t CRAFT_ID_NULL;
#endif

int compare_ids(void *a, void *b);

int craft_init_obj_db() {
    objects = hashmap_create(compare_ids, free);
#ifndef HAVE_UINT_128
    CRAFT_ID_NULL.first = 0;
    CRAFT_ID_NULL.second = 0;
#ifndef HAVE_UINT_64
    CRAFT_ID_NULL.third = 0;
    CRAFT_ID_NULL.fourth = 0;
#endif
#endif
    if (objects == NULL) {
        return ~ENOBUFS;
    } else {
        return 0;
    }
}

void craft_free_obj_db() {
    hashmap_destroy(objects);
    objects = NULL;
}

void *craft_get_ref(craft_id_t id) {
    if (id != CRAFT_ID_NULL) {
        return hashmap_get(objects, &id);
    } else {
        ERROR(EINVAL);
        return NULL;
    }
}

void *craft_alloc_ref(size_t length, craft_id_t *id, bool random_id) {
    if (length > 0 && id) {
        if (random_id) {
            do {
#if RAND_MAX < 256
#error rand() must return a least one byte of randomness
#endif
                union {
                    craft_id_t id;
                    char raw[sizeof(craft_id_t)];
                } buffer;
                int i;
                for (i = sizeof(craft_id_t) - 1; i >= 0; --i) {
                    buffer.raw[i] = (char) (rand() & 0xFF);
                }
                *id = buffer.id;
            } while (hashmap_get(objects, id));
        }
        void *key = malloc(sizeof(craft_id_t));
        if (key) {
            memcpy(key, id, sizeof(craft_id_t));
            void *data = malloc(length);
            if (data) {
                if (hashmap_insert(objects, key, data) == 0) {
                    if (hashmap_get_usage(objects) < HASHMAP_EFFICIENCY_TOLERANCE) {
                        hashmap_optimize(objects);
                    }
                    return data;
                } else {
                    int tmp = errno;
                    free(data);
                    free(key);
                    errno = tmp;
                    return NULL;
                }
            } else {
                free(key);
                ERROR(ENOBUFS);
                return NULL;
            }
        } else {
            ERROR(ENOBUFS);
            return NULL;
        }
    } else {
        ERROR(EINVAL);
        return NULL;
    }
}

void craft_free_ref(craft_id_t id) {
    void *ref = hashmap_get(objects, &id);
    hashmap_remove(objects, &id);
    free(ref);
}

int compare_ids(void *_a, void *_b) {
    if (_a && _b) {
        craft_id_t a = *(craft_id_t *) _a;
        craft_id_t b = *(craft_id_t *) _b;
#ifdef HAVE_UINT_128
        if (a < b) {
            return -1;
        } else if (a == b) {
            return 0;
        } else {
            return 1;
        }
#else
        if (a.first < b.first) {
            return -1;
        } else if (a.first > b.first) {
            return 1;
        } else if (a.second < b.second) {
            return -1;
        } else if (a.second > b.second) {
            return 1;
#ifdef HAVE_UINT_64
        } else {
            return 0;
        }
#else
        } else if (a.third < b.third) {
            return -1;
        } else if (a.third > b.third) {
            return 1;
        } else if (a.fourth < b.fourth) {
            return -1;
        } else if (a.fourth > b.fourth) {
            return 1;
        } else {
            return 0;
        }
#endif
#endif
    } else {
        ERROR(EINVAL);
        return 0;
    }
}
