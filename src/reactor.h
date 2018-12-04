#pragma once

#include "internal.h"

// kqueue identifier or epoll fd.
int posixc_reactor_getid(posixc_reactor* r);

pthread_t posixc_reactor_getpthread(posixc_reactor* r);

posixc_reactor* posixc_reactor_create();

void posixc_reactor_destroy(posixc_reactor* r);
