#pragma once
#include "internal.h"

#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>

#define POSIXC_EVENT_IN 0x0001
#define POSIXC_EVENT_OUT 0x0004

posixc_event* posixc_event_create(posixc_reactor* r int fd, posixc_event_cb cb, void* arg);

void posixc_event_destroy(posixc_event*e);

bool posixc_event_submit(posixc_event* e, int evmask);

int posixc_event_getfd(posixc_event* e);

// nonblocking, close-on-exec, no sigpipe
static inline void posixc_preparefd(int fd){
    fcntl(fd, F_SETFD, FD_CLOEXEC);
	fcntl(fd, F_SETFL, O_NONBLOCK);
    int one = 1;
	setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof(one)); 
}