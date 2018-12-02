#include "epoller.h"
#include <fcntl.h>          // fcntl, posix_fadvise
#include <sys/stat.h>       // stat, S_ISREG
#include <unistd.h>         // read, open, write, close, lseek, pread
#include <sys/epoll.h>

#define max_nr_epoll_events 32

struct epoller{
    int                     fd;
    struct epoll_event      events[max_nr_epoll_events];
};

int posixc_epoller_init(posixc_epoller* epoller){
    epoller->fd = epoll_create1(EPOLL_CLOEXEC);
    if(epoller->fd<0) return -1;
    return 0;
}

int posixc_epoller_destroy(posixc_epoller* epoller){
    return close(epoller->fd);
}

int posixc_epoller_add(posixc_epoller* epoller, int fd, void* ev_data){
    fcntl(fd, F_SETFD, FD_CLOEXEC);
    fcntl(fd, F_SETFL, O_NONBLOCK);
    struct epoll_event ee;
    ee.events=0;
    ee.data.ptr=ev_data;
    return epoll_ctl(epoller->fd, EPOLL_CTL_ADD, fd, &ee);
}

int posixc_epoller_mod(posixc_epoller* epoller, int fd, int events_new, void* ev_data){
    struct epoll_event ee;
    ee.events   = events_new | EPOLLONESHOT | EPOLLERR;
    ee.data.ptr = ev_data; 
    return epoll_ctl(epoller->fd, EPOLL_CTL_MOD, fd, &ee); 
}

int posixc_epoller_del(posixc_epoller* epoller, int fd){
    struct epoll_event ee; //unused
    epoll_ctl(epoller->fd, EPOLL_CTL_DEL, fd, &ee);
}

int posixc_epoller_wait(posixc_epoller* epoller){
    return epoll_wait(epoller->fd, events, max_nr_epoll_events, -1);
}



