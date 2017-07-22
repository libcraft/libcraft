#include <errno.h>
#include <stddef.h>
#include <libcraft/threading/loop.h>
#include <libcraft/threading/task.h>
#include <libcraft/ids.h>
#include <libcraft/log.h>

struct loop_thread {
    void (*task)(void *arg, int i);
    void *arg;
    bool run;
};
typedef struct loop_thread loop_thread_t;

void *loop_main(void *arg);

craft_loop_t craft_start_loop(void (*task)(void *arg, int i), void *arg) {
    if (task) {
        craft_loop_t loop;
        loop_thread_t *thread = (loop_thread_t *) craft_alloc_ref(sizeof(loop_thread_t), &loop, true);
        if (thread) {
            thread->task = task;
            thread->arg = arg;
            thread->run = true;
            if (craft_start_task(loop_main, craft_null_callback, thread) == 0) {
                return loop;
            } else {
                int tmp = errno;
                craft_free_ref(loop);
                errno = tmp;
                return CRAFT_ID_NULL;
            }
        } else {
            return CRAFT_ID_NULL;
        }
    } else {
        ERROR(EINVAL);
        return CRAFT_ID_NULL;
    }
}

int craft_end_loop(craft_loop_t loop) {
    if (loop != CRAFT_ID_NULL) {
        loop_thread_t *thread = (loop_thread_t *) craft_get_ref(loop);
        if (thread) {
            thread->run = false;
        } else {
            return ERROR(EINVAL);
        }
    } else {
        return ERROR(EINVAL);
    }
}

void *loop_main(void *arg) {
    loop_thread_t *thread = (loop_thread_t *) arg;
    int i;
    for (i = 0; thread->run; ++i) {
        thread->task(thread->arg, i);
    }
    return NULL;
}
