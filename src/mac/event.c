#include "event.h"

#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <pthread.h>
#include <poll.h>

struct posixc_event{
    posixc_reactor* reactor;
    int             fd;
    posixc_event_cb cb;
    void*           arg;
    int             evmask;
    bool            closing; 
    pthread_mutex_t mtx;
    list_head       node;
};

#define max_events 64

#define ev_newly_add (EV_ADD | EV_DISABLE | EV_CLEAR)

static void register_write(struct kevent* ev, posixc_event* e){
    EV_SET(ev, (uintptr_t)(e->fd), EVFILT_WRITE, ev_newly_add, 0, 0, (uint64_t)e);
}

static void register_read(struct kevent* ev, posixc_event* e){
    EV_SET(ev, (uintptr_t)(e->fd), EVFILT_READ, ev_newly_add, 0, 0, (uint64_t)e);
}

static void del_write(struct kevent* ev, posixc_event* e){
    EV_SET(ev, (uintptr_t)(e->fd), EVFILT_WRITE, EV_DELETE, 0, 0, (uint64_t)e);
}

static void del_read(struct kevent* ev, posixc_event* e){
    EV_SET(ev, (uintptr_t)(e->fd), EVFILT_READ, EV_DELETE, 0, 0, (uint64_t)e);
}

#define ev_submit (EV_ENABLE | EV_DISPATCH | EV_CLEAR)

static void submit_read(struct kevent* ev, posixc_event* e){
	EV_SET(ev, (uintptr_t)(e->fd), EVFILT_READ, ev_submit, 0, 0, (uint64_t)e);
}

static void submit_write(struct kevent* ev, posixc_event* e){
	EV_SET(ev, (uintptr_t)(e->fd), EVFILT_WRITE, ev_submit, 0, 0, (uint64_t)e);
}

static void posixc_event_init(posixc_event*event, posixc_reactor* reactor int fd, posixc_event_cb cb, void* arg){
    posixc_preparefd(fd);
    event->reactor=reactor;
    event->fd=fd;
    event->cb=cb;
    event->arg=arg;
    event->evmask=0;
    event->closing=false;
    pthread_mutex_init(&event->mtx);
    INIT_LIST_HEAD(&event->node);
}

static bool kevent_set(int reactor_id, struct kevent* ev, int nr_ev){
    return kevent(reactor_id, ev, nr_ev, NULL, 0, NULL)==0;
}

posixc_event* posixc_event_create(posixc_reactor* reactor int fd, posixc_event_cb cb, void* arg){
    posixc_event* event = (posixc_event*)malloc(sizeof(posixc_event));
    posixc_event_init(event,reactor,fd,cb,arg);
    struct kevent ev[2];
    register_read(&ev[0], event);
    register_write(&ev[1], event); 
    if(kevent_set(posixc_reactor_getid(reactor), ev, 2)){
        return event;
    }else{
		free(event);
		return NULL;
    }
}

void posixc_event_destroy(posixc_event*e){
	posixc_reactor *reactor = e->reactor;
    // Any user thread that holds a reference to the posixc_event could stop it from working.
	if (!e->closing) {  
        e->closing = true;
		struct kevent ev[2];
        del_read(&ev[0], e);
        del_write(&ev[1], e);
		shutdown(e->fd, SHUT_RDWR);
        kevent_set(posixc_reactor_getid(reactor),ev,2);
        //@todo: add e into reap list
	}
    // Final destruction can only be performed by its owning reactor thread. This restriction ensures that the reactor won't process already destroyed posixc_events.
    if(pthread_self()==posixc_reactor_getpthread(reactor)){
        close(e->fd);
        pthread_mutex_destroy(&e->mtx);
        free(e);
    }
}

bool posixc_event_submit(posixc_event* e, int evmask){
    if (e->closing) return false; 
	nni_mtx_lock(&e->mtx);
	e->evmask |= evmask;
	nni_mtx_unlock(&e->mtx);
    struct kevent ev[2];
	if(e->evmask & POSIXC_EVENT_IN)  submit_read(&ev[0], e);
	if(e->evmask & POSIXC_EVENT_OUT) submit_write(&ev[0], e);
	while(!kevent_set(posixc_reactor_getid(e->reactor),ev,2)){
		if (errno == EINTR) continue;
		return false;
	}
	return true;
}

int posixc_event_getfd(posixc_event* e){
    return e->fd;
}
