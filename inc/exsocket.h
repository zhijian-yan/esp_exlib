#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "../exlib_config.h"

#if INCLUDE_EXLIB_SOCKET

#include "lwip/sockets.h"

#define EXSOCKET_TCP_LISTEN_BKAGLOG 10 // esp32 max value:10

int exsocket_tcp_server_init(uint16_t port);
int exsocket_tcp_client_init(const char *ip, uint16_t port);
int exsocket_udp_server_init(uint16_t port);
int exsocket_udp_client_init(void);
int exsocket_accept(int sock);
int exsocket_udp_enable_broadcast(int sock);
int exsocket_udp_multicast_add(int sock, const char *ip);
int exsocket_udp_multicast_drop(int sock, const char *ip);
int exsocket_tcp_set_keepalive(int sock);
int exsocket_tcp_set_nodelay(int sock);
int exsocket_set_sockaddr(const char *ip, uint16_t port,
                          struct sockaddr_in *sockaddr);
int exsocket_set_nonblock(int sock);
int exsocket_extract_ipv4(const char *ip, int *ipv4);
int exsocket_send(int sock, const void *data, unsigned int size);
int exsocket_recv(int sock, void *data, unsigned int size);
int exsocket_sendto(int sock, const void *data, unsigned int size,
                    struct sockaddr_in *to);
int exsocket_recvfrom(int sock, void *data, unsigned int size,
                      struct sockaddr_in *to);

#endif

#ifdef __cplusplus
}
#endif
