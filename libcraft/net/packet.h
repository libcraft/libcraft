#ifndef LIBCRAFT_NET_PACKET_H
#define LIBCRAFT_NET_PACKET_H
#include <stddef.h>
#include <libcraft/net/server/server.h>

struct craft_packet {
    size_t packet_length;
    size_t packet_data_length;
    size_t packet_id;
    void *packet_data;
};
typedef struct craft_packet craft_packet_t;

int craft_server_packet_send(craft_server_client_t client, craft_packet_t packet);

int craft_server_packet_read(craft_server_client_t client, craft_packet_t *packet, bool compressable);

#endif
