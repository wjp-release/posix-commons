#pragma once

#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "tcp.h"
#include "common/slice.h"

int posixc_tcp_socketfd(){
    return socket(AF_INET, SOCK_STREAM, 0);
}

void posixc_tcp_set_addr(struct sockaddr_in* sin, const char* host, int port){
    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr = inet_addr(host);
    sin->sin_port = htons(port);
}

void posixc_tcp_set_addr_any(struct sockaddr_in* sin, int port){
    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr = htonl(INADDR_ANY);
    sin->sin_port = htons(port);
}

bool posixc_tcp_is_addr_valid(struct sockaddr_in* sin){
    return sin->sin_addr.s_addr != INADDR_NONE;
}

int posixc_tcp_connect(int fd, struct sockaddr_in* sin){
    return connect(fd, sin, sizeof(*sin));
}

int posixc_tcp_bind(int fd, struct sockaddr_in* sin){
    return bind(fd, sin, sizeof(*sin));
}

int posixc_tcp_listen(int fd){
    return listen(fd,1);
}

int posixc_tcp_accept_fd(int fd, struct sockaddr_in* cin)
    socklen_t cin_sz = sizeof(*cin);
    return accept(fd, cin, &cin_sz);
}

posixc_slice posixc_tcp_get_ip(struct sockaddr_in* cin){
    posixc_slice s;
    posixc_slice_init_from(&s, inet_ntoa(cin->sin_addr))
    return s;
}

int posixc_tcp_get_port(struct sockaddr_in* cin){
    return (int)ntohs(cin->sin_port));
}

