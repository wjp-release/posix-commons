#pragma once
#include "internal.h"

#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>

int posixc_event_create(posixc_event** e, posixc_reactor* r int fd, posixc_event_cb cb, void* arg);
void posixc_event_close(posixc_event* e);
int posixc_event_destroy(posixc_event*e);
int posixc_event_submit(posixc_event* e, int fd);
int posixc_event_fd(posixc_event* e);
void posixc_event_bind(posixc_event* e, posixc_event_cb cb, void* arg);

static inline void posixc_preparefd(int fd){
    fcntl(fd, F_SETFD, FD_CLOEXEC);
	fcntl(fd, F_SETFL, O_NONBLOCK);
    int one = 1;
	setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof(one)); //no sigpipe
}