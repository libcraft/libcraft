#ifdef HAVE_LIBPTHREAD
#include <errno.h>
#include <stddef.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <libcraft/net/server/server.h>
#include <libcraft/ids.h>
#include <libcraft/log.h>

struct server_info {
    int sockfd;
    int (*client_connected)(craft_server_client_t client);
};
typedef struct server_info server_info_t;

struct client_info {
    int sockfd;
};
typedef struct client_info client_info_t;

void server_accept_loop(void *arg, int i);

craft_server_t craft_server_create() {
    craft_server_t id;
    server_info_t *server = (server_info_t *) craft_alloc_ref(sizeof(server_info_t), &id, true);
    if (server) {
        server->sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (server->sockfd < 0) {
            ERROR(errno);
            return CRAFT_ID_NULL;
        } else {
            return id;
        }
    } else {
        ERROR(ENOBUFS);
        return CRAFT_ID_NULL;
    }
}

void craft_server_free(craft_server_t id) {
    if (id != CRAFT_ID_NULL) {
        server_info_t *server = (server_info_t *) craft_get_ref(id);
        if (server) {
            close(server->sockfd);
            craft_free_ref(id);
        }
    } else {
        ERROR(EINVAL);
    }
}

craft_loop_t craft_server_bind(craft_server_t id, int port, size_t backlog, int (*client_connected)(craft_server_client_t client)) {
    if (id != CRAFT_ID_NULL && port > 0 && port < 65536 && backlog > 0 && client_connected) {
        server_info_t *server = (server_info_t *) craft_get_ref(id);
        if (server) {
            struct sockaddr_in addr;
            bzero(&addr, sizeof(addr));
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = INADDR_ANY;
            addr.sin_port = htons(port);
            if (bind(server->sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
                ERROR(errno);
                return CRAFT_ID_NULL;
            } else {
                if (listen(server->sockfd, backlog) < 0) {
                    ERROR(errno);
                    return CRAFT_ID_NULL;
                } else {
                    server->client_connected = client_connected;
                    return craft_start_loop(server_accept_loop, server);
                }
            }
        } else {
            ERROR(EINVAL);
            return CRAFT_ID_NULL;
        }
    } else {
        ERROR(EINVAL);
        return CRAFT_ID_NULL;
    }
}

int craft_server_read_from(craft_server_client_t id, void *buffer, size_t length) {
    if (id != CRAFT_ID_NULL && buffer && length > 0) {
        client_info_t *client = (client_info_t *) craft_get_ref(id);
        return read(client->sockfd, buffer, length);
    } else {
        return ERROR(EINVAL);
    }
}

int craft_server_send_to(craft_server_client_t id, void *buffer, size_t length) {
    if (id != CRAFT_ID_NULL && buffer && length > 0) {
        client_info_t *client = (client_info_t *) craft_get_ref(id);
        return write(client->sockfd, buffer, length);
    } else {
        return ERROR(EINVAL);
    }
}

void server_accept_loop(void *arg, int i) {
    server_info_t *server = (server_info_t *) arg;
    struct sockaddr_in addr;
    int length = sizeof(addr);
    int sockfd = accept(server->sockfd, (struct sockaddr *) addr, &length);
    if (sockfd < 0) {
        ERROR(errno);
    }
    craft_server_client_t id;
    client_info_t *client = (client_info_t *) craft_alloc_ref(sizeof(client_info_t), &id, true);
    if (client) {
        client->sockfd = sockfd;
        if (server->client_connected(id) < 0) {
            log_craft_fmt("Failed to initialize client: %s", strerror(errno));
            craft_free_ref(id);
            close(sockfd);
        }
    } else {
        log_crash_fmt("Unable to allocate space for accepted client data: %s", strerror(errno));
        close(sockfd);
    }
}

#endif
