#ifndef LIBCRAFT_NET_SERVER_SERVER_H
#define LIBCRAFT_NET_SERVER_SERVER_H
#include <stddef.h>
#include <libcraft/threading/loop.h>
#include <libcraft/ids.h>

typedef craft_id_t craft_server_t;
typedef craft_id_t craft_server_client_t;

craft_server_t craft_server_create();

void craft_server_free(craft_server_t server);

craft_loop_t craft_server_bind(craft_server_t server, int port, size_t backlog, int (*client_connected)(craft_server_client_t client));

int craft_server_read_from(craft_server_client_t client, void *buffer, size_t length);

int craft_server_send_to(craft_server_client_t client, void *buffer, size_t length);

#endif
