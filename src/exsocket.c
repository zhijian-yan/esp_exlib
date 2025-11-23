#include "exsocket.h"

#if INCLUDE_EXLIB_SOCKET

typedef enum { mode_server, mode_client } exsocket_mode_t;

static int exsoket_bind(int sock, uint16_t port) {
    struct sockaddr_in sa = {0};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = INADDR_ANY;
    if (bind(sock, (const struct sockaddr *)&sa, sizeof(struct sockaddr_in)) <
        0) {
        return -1;
    }
    return 0;
}

static int exsocket_tcp_init(const char *ip, uint16_t port,
                             exsocket_mode_t mode) {
    int ret = 0;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        ret = -1;
        goto err;
    }
    if (mode == mode_server) {
        int opt = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            ret = -2;
            goto err;
        }
        if (exsoket_bind(sock, port) < 0) {
            ret = -3;
            goto err;
        }
        if (listen(sock, EXSOCKET_TCP_LISTEN_BKAGLOG) < 0) {
            ret = -4;
            goto err;
        }
    } else if (mode == mode_client) {
        struct sockaddr_in sa = {
            .sin_family = AF_INET,
            .sin_port = htons(port),
        };
        if (inet_pton(AF_INET, ip, &sa.sin_addr) <= 0) {
            ret = -5;
            goto err;
        }
        if (connect(sock, (const struct sockaddr *)&sa,
                    sizeof(struct sockaddr_in)) < 0) {
            ret = -6;
            goto err;
        }
    }
    return sock;
err:
    if (ret != -1) {
        close(sock);
    }
    return ret;
}

static int exsocket_udp_init(uint16_t port, exsocket_mode_t mode) {
    int ret = 0;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        ret = -1;
        goto err;
    }
    if (mode == mode_server) {
        int opt = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            ret = -2;
            goto err;
        }
        if (exsoket_bind(sock, port) < 0) {
            ret = -3;
            goto err;
        }
    }
    return sock;
err:
    if (ret != -1) {
        close(sock);
    }
    return ret;
}

int exsocket_tcp_server_init(uint16_t port) {
    return exsocket_tcp_init(NULL, port, mode_server);
}

int exsocket_tcp_client_init(const char *ip, uint16_t port) {
    return exsocket_tcp_init(ip, port, mode_client);
}

int exsocket_udp_server_init(uint16_t port) {
    return exsocket_udp_init(port, mode_server);
}

int exsocket_udp_client_init(void) { return exsocket_udp_init(0, mode_client); }

int exsocket_accept(int sock) {
    int client_fd = accept(sock, NULL, NULL);
    if (client_fd < 0) {
        return -1;
    }
    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        return -2;
    }
    return client_fd;
}

int exsocket_udp_enable_broadcast(int sock) {
    int opt = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)) < 0) {
        return -1;
    }
    return 0;
}

int exsocket_udp_multicast_add(int sock, const char *ip) {
    struct ip_mreq multicast_request;
    multicast_request.imr_interface.s_addr = htonl(INADDR_ANY);
    multicast_request.imr_multiaddr.s_addr = inet_addr(ip);
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicast_request,
                   sizeof(multicast_request)) < 0) {
        return -1;
    }
    return 0;
}

int exsocket_udp_multicast_drop(int sock, const char *ip) {
    struct ip_mreq multicast_request;
    multicast_request.imr_interface.s_addr = htonl(INADDR_ANY);
    multicast_request.imr_multiaddr.s_addr = inet_addr(ip);
    if (setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, &multicast_request,
                   sizeof(multicast_request)) < 0) {
        return -1;
    }
    return 0;
}

int exsocket_tcp_set_keepalive(int sock) {
    int alive = 1;    // 开启keepalive属性
    int idle = 1;     // 如果连接在1秒内没有任何数据往来,则进行探测
    int interval = 1; // 探测时发包的时间间隔为1秒
    int count =
        3; // 探测尝试的次数.如果第1次探测包就收到响应了,则后几次的不再发
    if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &alive, sizeof(int)) < 0) {
        return -1;
    }
    if (setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(int)) < 0) {
        return -2;
    }
    if (setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(int)) <
        0) {
        return -3;
    }
    if (setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &count, sizeof(int)) < 0) {
        return -4;
    }
    return 0;
}

int exsocket_tcp_set_nodelay(int sock) {
    int nodelay = 1;
    if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(int)) < 0) {
        return -1;
    }
    return 0;
}

int exsocket_set_nonblock(int sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags < 0) {
        return -1;
    }
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0) {
        return -2;
    }
    return 0;
}

int exsocket_set_sockaddr(const char *ip, uint16_t port,
                          struct sockaddr_in *sockaddr) {
    if (!ip || !sockaddr) {
        return -1;
    }
    memset(sockaddr, 0, sizeof(struct sockaddr_in));
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = htons(port);
    if (inet_pton(AF_INET, ip, &sockaddr->sin_addr) <= 0) {
        return -2;
    }
    return 0;
}

int exsocket_extract_ipv4(const char *ip, int *ipv4) {
    struct sockaddr_in sa;
    if (inet_pton(AF_INET, ip, &(sa.sin_addr)) < 0) {
        return -1;
    }
    if (!ipv4) {
        return -2;
    }
    char temp[16];
    strncpy(temp, ip, 15);
    temp[15] = '\0';
    char *token;
    token = strtok(temp, ".");
    for (int i = 0; i < 4; ++i) {
        sscanf(token, "%d", &ipv4[i]);
        token = strtok(NULL, ".");
    }
    return 0;
}

int exsocket_send(int sock, const void *data, unsigned int size) {
    int to_send = size;
    while (to_send > 0) {
        int sent = send(sock, data + (size - to_send), to_send, 0);
        if (sent < 0 && errno != EINPROGRESS && errno != EAGAIN &&
            errno != EWOULDBLOCK) {
            return -1;
        }
        to_send -= sent;
    }
    return size;
}

int exsocket_recv(int sock, void *data, unsigned int size) {
    int recieved = recv(sock, data, size, 0);
    if (recieved < 0) {
        if (errno == EINPROGRESS || errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0; // 这并不是一个错误
        }
        if (errno == ENOTCONN) {
            return -1; // disconnect
        }
        return -2;
    }
    return recieved;
}

int exsocket_sendto(int sock, const void *data, unsigned int size,
                    struct sockaddr_in *to) {
    int to_send = size;
    while (to_send > 0) {
        int sent = sendto(sock, data, size, 0, (const struct sockaddr *)to,
                          sizeof(struct sockaddr_in));
        if (sent < 0) {
            return -1;
        }
        to_send -= sent;
    }
    return size;
}

int exsocket_recvfrom(int sock, void *data, unsigned int size,
                      struct sockaddr_in *to) {
    int recieved = recvfrom(sock, data, size, 0, (struct sockaddr *)to, NULL);
    if (recieved < 0) {
        if (errno == EINPROGRESS || errno == EAGAIN || errno == EWOULDBLOCK) {
            return 0; // 这并不是一个错误
        } else {
            return -1;
        }
    }
    return recieved;
}

#endif
