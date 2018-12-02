#pragma once

#include "internal.h"

int posixc_tcp_socketfd(); //get tcp ipv4 socket fd
void posixc_tcp_set_addr(struct sockaddr_in* sin, const char* host, int port);
bool posixc_tcp_is_addr_valid(struct sockaddr_in* sin);
int posixc_tcp_connect(int fd, struct sockaddr_in* sin);
int posixc_tcp_bind(int fd, struct sockaddr_in* sin);
int posixc_tcp_listen(int fd);
int posixc_tcp_accept_fd(int fd, struct sockaddr_in* cin); //get accepted fd
posixc_slice posixc_tcp_get_ip(struct sockaddr_in* cin);
int posixc_tcp_get_port(struct sockaddr_in* cin); 
