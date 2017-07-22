#ifndef LIBCRAFT_THREADING_TASK_H
#define LIBCRAFT_THREADING_TASK_H

int craft_threads_init();

void craft_threads_free();

int craft_start_task(void *(*task)(void *arg), void (*callback)(void *result), void *arg);

void craft_null_callback(void *result);

#endif
