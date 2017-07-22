#ifndef LIBCRAFT_COLLECTIONS_LINKEDLIST_H
#define LIBCRAFT_COLLECTIONS_LINKEDLIST_H

struct linkedlist;
typedef struct linkedlist linkedlist_t;

linkedlist_t *linkedlist_create(void (*element_free)(void *data));

void linkedlist_destroy(linkedlist_t *list);

int linkedlist_count(linkedlist_t *list);

int linkedlist_append(linkedlist_t *list, void *data);

int linkedlist_prepend(linkedlist_t *list, void *data);

int linkedlist_insert(linkedlist_t *list, void *data, int index);

int linkedlist_remove(linkedlist_t *list, int index);

void *linkedlist_get(linkedlist_t *list, int index);

int linkedlist_iterate_start(linkedlist_t *list);

void *linkedlist_iterate(linkedlist_t *list);

#endif
