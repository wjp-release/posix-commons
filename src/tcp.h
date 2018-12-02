#pragma once

#include "internal.h"

int posixc_tcpipv4_socket(){
    return socket(AF_INET, SOCK_STREAM, 0);
}

void posixc_set_remote_addr(){
    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(server);
    sin.sin_port = htons(port);

    if (sin.sin_addr.s_addr == INADDR_NONE) {
        return;
        printf("invalid remote IP %s\n", server);
        exit(-1);
    }

}
