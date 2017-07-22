#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <libcraft/collections/linkedlist.h>
#include <libcraft/log.h>

struct linkedlist_entry;
typedef struct linkedlist_entry linkedlist_entry_t;
struct linkedlist_entry {
    void *data;
    linkedlist_entry_t *next;
    linkedlist_entry_t *prev;
};

struct linkedlist {
    int size;
    linkedlist_entry_t *start;
    linkedlist_entry_t *iterator;
    linkedlist_entry_t *end;
    void (*element_free)(void *data);
};

linkedlist_t *linkedlist_create(void (*element_free)(void *data)) {
    if (element_free) {
        linkedlist_t *list = (linkedlist_t *) malloc(sizeof(linkedlist_t));
        if (list) {
            list->size = 0;
            list->start = NULL;
            list->end = NULL;
            list->element_free = element_free;
        }
        return list;
    } else {
        ERROR(EINVAL);
        return NULL;
    }
}

void linkedlist_destroy(linkedlist_t *list) {
    if (list) {
        linkedlist_entry_t *entry = list->start;
        while (entry) {
            list->element_free(entry->data);
            entry = entry->next;
        }
        free(list);
    } else {
        ERROR(EINVAL);
    }
}

int linkedlist_count(linkedlist_t *list) {
    if (list) {
        return list->size;
    } else {
        return ERROR(EINVAL);
    }
}

int linkedlist_append(linkedlist_t *list, void *data) {
    if (list) {
        linkedlist_entry_t *entry = (linkedlist_entry_t *) malloc(sizeof(linkedlist_entry_t));
        if (entry) {
            entry->data = data;
            list->end->next = entry;
            entry->prev = list->end;
            list->end = entry;
                ++list->size;
            return 0;
        } else {
            return ERROR(ENOBUFS);
        }
    } else {
        return ERROR(EINVAL);
    }
}

int linkedlist_prepend(linkedlist_t *list, void *data) {
    if (list) {
        linkedlist_entry_t *entry = (linkedlist_entry_t *) malloc(sizeof(linkedlist_entry_t));
        if (entry) {
            entry->data = data;
            list->start->prev = entry;
            entry->next = list->start;
            list->start = entry;
                ++list->size;
            return 0;
        } else {
            return ERROR(ENOBUFS);
        }
    } else {
        return ERROR(EINVAL);
    }
}

int linkedlist_insert(linkedlist_t *list, void *data, int index) {
    if (list && index > 0) {
        linkedlist_entry_t *entry = list->start;
        int i;
        for (i = index; i > 0; --i) {
            if (entry) {
                entry = entry->next;
            } else {
                return ERROR(EINVAL);
            }
        }
        if (entry) {
            linkedlist_entry_t *new = (linkedlist_entry_t *) malloc(sizeof(linkedlist_entry_t));
            if (new) {
                new->data = data;
                new->next = entry->next;
                new->prev = entry;
                entry->next = new;
                new->next->prev = new;
                ++list->size;
                return 0;
            } else {
                return ERROR(ENOBUFS);
            }
        } else {
            return ERROR(EINVAL);
        }
    } else {
        return ERROR(EINVAL);
    }
}

int linkedlist_remove(linkedlist_t *list, int index) {
    if (list && index > 0) {
        linkedlist_entry_t *entry = list->start;
        int i;
        for (i = index; i > 0; --i) {
            if (entry) {
                entry = entry->next;
            } else {
                return ERROR(EINVAL);
            }
        }
        if (entry) {
            entry->prev->next = entry->next;
            entry->next->prev = entry->prev;
            --list->size;
            list->element_free(entry->data);
            free(entry);
            return 0;
        } else {
            return ERROR(EINVAL);
        }
    } else {
        return ERROR(EINVAL);
    }
}

void *linkedlist_get(linkedlist_t *list, int index) {
    if (list && index > 0) {
        linkedlist_entry_t *entry = list->start;
        int i;
        for (i = index; i > 0; --i) {
            if (entry) {
                entry = entry->next;
            } else {
                ERROR(EINVAL);
                return NULL;
            }
        }
        if (entry) {
            return entry->data;
        } else {
            ERROR(EINVAL);
            return NULL;
        }
    } else {
        ERROR(EINVAL);
        return NULL;
    }
}

int linkedlist_iterate_start(linkedlist_t *list) {
    if (list) {
        list->iterator = list->start;
        return 0;
    } else {
        return ERROR(EINVAL);
    }
}

void *linkedlist_iterate(linkedlist_t *list) {
    if (list) {
        void *data = NULL;
        if (list->iterator) {
            data = list->iterator->data;
            list->iterator = list->iterator->next;
        }
        return data;
    } else {
        ERROR(EINVAL);
        return NULL;
    }
}
