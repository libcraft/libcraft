#ifndef LIBCRAFT_THREADING_LOOP_H
#define LIBCRAFT_THREADING_LOOP_H
#include <libcraft/ids.h>

typedef craft_id_t craft_loop_t;

craft_loop_t craft_start_loop(void (*task)(void *arg, int i), void *arg);

int craft_end_loop(craft_loop_t loop);

#endif
