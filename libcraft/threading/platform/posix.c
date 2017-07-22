#ifdef HAVE_LIBPTHREAD
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <libcraft/collections/linkedlist.h>
#include <libcraft/threading/platform/platform.h>
#include <libcraft/log.h>

struct thread_create_params {
    void (*task)(void *arg);
    void *arg;
    pthread_t thread;
    int index;
};
typedef struct thread_create_params thread_create_params_t;

linkedlist_t *threads;

void *create_thread_callback(void *arg);

int craft_thread_plat_init() {
    threads = linkedlist_create(free);
    if (threads) {
        return 0;
    } else {
        return ERROR(ENOBUFS);
    }
}

int craft_thread_plat_free() {
    int ret = linkedlist_iterate_start(threads);
    if (ret == 0) {
        thread_create_params_t *params;
        while (params = (thread_create_params_t *) linkedlist_iterate(threads)) {
            pthread_join(params->thread, NULL);
        }
        linkedlist_destroy(threads);
        return 0;
    } else {
        return ret;
    }
}

int craft_thread_plat_create(void (*task)(void *arg), void *arg) {
    if (task) {
        thread_create_params_t *params = (thread_create_params_t *) malloc(sizeof(thread_create_params_t));
        if (params) {
            int ret = pthread_create(&params->thread, NULL, create_thread_callback, params);
            if (ret == 0) {
                linkedlist_append(threads, params);
                params->index = linkedlist_count(threads) - 1;
            }
            return ret;
        } else {
            return ERROR(ENOBUFS);
        }
    } else {
        return ERROR(EINVAL);
    }
}

void *create_thread_callback(void *arg) {
    thread_create_params_t *params = (thread_create_params_t *) arg;
    params->task(params->arg);
    linkedlist_remove(threads, params->index);
    return NULL;
}

void craft_thread_plat_sleep(int usec) {
    if (usec > 0) {
        usleep(usec);
    } else {
        ERROR(EINVAL);
    }
}

#endif
