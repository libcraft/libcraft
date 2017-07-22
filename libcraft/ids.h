#ifndef LIBCRAFT_IDS_H
#define LIBCRAFT_IDS_H
#include <stddef.h>
#include <stdbool.h>

#ifdef HAVE_UINT_128
typedef unsigned __int128 craft_id_t;

#define CRAFT_ID_NULL 0
#else
#include <stdint.h>

struct craft_id {
#ifdef HAVE_UINT_64
    uint64_t first;
    uint64_t second;
#else
    uint32_t first;
    uint32_t second;
    uint32_t third;
    uint32_t fourth;
#endif
};

typedef struct craft_id craft_id_t;

extern craft_id_t CRAFT_ID_NULL;
#endif

int craft_init_obj_db();

void craft_free_obj_db();

#ifdef LIBCRAFT_SOURCE
void *craft_get_ref(craft_id_t id);

void *craft_alloc_ref(size_t length, craft_id_t *id, bool random_id);

void craft_free_ref(craft_id_t id);
#endif

#endif
