#include "event.h"

#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <pthread.h>
#include <poll.h>

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

static bool kevent_set(int reactor_id, struct kevent* ev, int nr_ev){
    return kevent(reactor_id, ev, nr_ev, NULL, 0, NULL)==0;
}

bool posixc_event_plat_create(posixc_event*event){
    struct kevent ev[2];
    register_read(&ev[0], event);
    register_write(&ev[1], event); 
    return kevent_set(event->reactor->id, ev, 2);
}

bool posixc_event_plat_destroy(posixc_event*e){
    struct kevent ev[2];
    del_read(&ev[0], e);
    del_write(&ev[1], e);
    return kevent_set(e->reactor->id,ev,2);
}

bool posixc_event_plat_submit(posixc_event*e){
    struct kevent ev[2];
	if(e->evmask & POSIXC_EVENT_IN)  submit_read(&ev[0], e);
	if(e->evmask & POSIXC_EVENT_OUT) submit_write(&ev[0], e);
	return kevent_set(e->reactor->id,ev,2);
}