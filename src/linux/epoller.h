#pragma once

#include "internal.h"

// posixc_epoller is configured to use "oneshot mode", as it is supposed to be handled only once
// in an event-driven system. 

int posixc_epoller_init(posixc_epoller* epoller);
int posixc_epoller_destroy(posixc_epoller* epoller);
// Note: void* ev_data is stored in event.data.ptr
int posixc_epoller_add(posixc_epoller* epoller, int fd, void* ev_data);
int posixc_epoller_mod(posixc_epoller* epoller, int fd, int events_new, void* ev_data);
int posixc_epoller_del(posixc_epoller* epoller, int fd);
int posixc_epoller_wait(posixc_epoller* epoller);

