#include "event.h"

#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <pthread.h>

struct posixc_event{
    posixc_reactor* reactor;
    int             fd;
    posixc_event_cb cb;
    void*           arg;
    int             events;
    bool            closing;
    bool            closed;
    pthread_cond_t  cv;
    pthread_mutex_t mtx;
};

#define max_events 64
#define flags (EV_ADD | EV_DISABLE | EV_CLEAR)

static void register_write(struct kevent* ev,uintptr_t fd, uint64_t data){
    EV_SET(ev, fd, EVFILT_WRITE, flags, 0, 0, data);
}

static void register_read(struct kevent* ev,uintptr_t fd, uint64_t data){
    EV_SET(ev, fd, EVFILT_READ, flags, 0, 0, data);
}

static void posixc_event_init(posixc_event*event, posixc_reactor* reactor int fd, posixc_event_cb cb, void* arg){
    posixc_preparefd(fd);
    event->reactor=reactor;
    event->fd=fd;
    event->cb=cb;
    event->arg=arg;
    event->events=0;
    event->closing=false;
    event->closed=false;
    pthread_mutex_init(&event->mtx);
    pthread_cond_init(&event->cv);
}

int posixc_event_create(posixc_event** e, posixc_reactor* reactor int fd, posixc_event_cb cb, void* arg){
    *e = (posixc_event*)malloc(sizeof(posixc_event));
    posixc_event_init(*e,reactor,fd,cb,arg);
    struct kevent ev[2];
    register_read(&ev[0]);
    register_write(&ev[1]); // epoll ctl add
	if (kevent(pq->kq, ev, 2, NULL, 0, NULL) != 0) {
		free(event);
		return -1;
	}
    return 0;
}

