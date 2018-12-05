#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/epoll.h>

#include "event.h"
#include "reactor.h"

bool posixc_event_plat_create(posixc_event*e){
    struct epoll_event ee;
    ee.events=0;
    ee.data.ptr=e;
    return epoll_ctl(e->reactor->id, EPOLL_CTL_ADD, e->fd, &ee)==0;
}

bool posixc_event_plat_destroy(posixc_event*e){
    struct epoll_event ee;
    return epoll_ctl(e->reactor->id, EPOLL_CTL_DEL, e->fd, &ee)==0;
}

bool posixc_event_plat_submit(posixc_event*e){
    struct epoll_event ee;
    ee.events=e->evmask|EPOLLONESHOT|EPOLLERR;
    ee.data.ptr=e;
    return epoll_ctl(e->reactor->id, EPOLL_CTL_MOD, e->fd, &ee)==0;
}