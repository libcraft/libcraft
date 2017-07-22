#include <libcraft/net/server/server.h>
#include <libcraft/threading/loop.h>
#include <libcraft/ids.h>

int client_connected(craft_server_client_t client) {

}

int main(int argc, const char **argv) {
    craft_server_t server =  craft_server_create();
    craft_loop_t loop = craft_server_bind(server, 1234, 5, client_connected);
    return 0;
}
