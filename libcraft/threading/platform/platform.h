#ifdef LIBCRAFT_SOURCE
#ifndef LIBCRAFT_THREADING_PLATFORM_PLATFORM_H
#define LIBCRAFT_THREADING_PLATFORM_PLATFORM_H

int craft_thread_plat_init();

int craft_thread_plat_free();

int craft_thread_plat_create(void (*task)(void *arg), void *arg);

void craft_thread_plat_sleep(int usec);

#endif
#endif
