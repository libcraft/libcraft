#include <errno.h>
#include <stdlib.h>
#include <libcraft/collections/linkedlist.h>
#include <libcraft/threading/platform/platform.h>
#include <libcraft/threading/task.h>
#include <libcraft/log.h>

#define THREAD_BUSY 1

struct thread_info {
    void *(*task)(void *arg);
    void (*callback)(void *result);
    void *arg;
    int state;
};
typedef struct thread_info thread_info_t;

linkedlist_t *available;

void thread_main(void *arg);

int craft_threads_init() {
    available = linkedlist_create(free);
    if (available) {
        return craft_thread_plat_init();
    } else {
        return ERROR(ENOBUFS);
    }
}

void craft_threads_free() {
    linkedlist_destroy(available);
    available = NULL;
    craft_thread_plat_free();
}

int craft_start_task(void *(*task)(void *arg), void (*callback)(void *result), void *arg) {
    if (task && callback) {
        thread_info_t *thread = (thread_info_t *) linkedlist_get(available, 0);
        if (thread) {
            linkedlist_remove(available, 0);
            thread->task = task;
            thread->callback = callback;
            thread->arg = arg;
            thread->state = THREAD_BUSY;
            return 0;
        } else {
            thread = (thread_info_t *) malloc(sizeof(thread_info_t));
            if (thread) {
                thread->task = task;
                thread->callback = callback;
                thread->arg = arg;
                thread->state = THREAD_BUSY;
                craft_thread_plat_create(thread_main, thread);
            } else {
                return ERROR(ENOBUFS);
            }
        }
    } else {
        return ERROR(EINVAL);
    }
}

void thread_main(void *arg) {
    thread_info_t *thread = (thread_info_t *) arg;
    while (available) {
        if (thread->state == THREAD_BUSY) {
            thread->callback(thread->task(thread->arg));
            thread->state = 0;
            linkedlist_append(available, thread);
        } else {
            craft_thread_plat_sleep(100);
        }
    }
}

void craft_null_callback(void *result) {
}
