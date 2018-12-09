#include "event.h"
#include "reactor.h"

#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <pthread.h>
#include <poll.h>

#define max_events 64

#define ev_newly_add (EV_ADD | EV_DISABLE | EV_CLEAR)

static void register_write(struct kevent* ev, posixc_event* e){
    EV_SET(ev, (uintptr_t)(e->fd), EVFILT_WRITE, ev_newly_add, 0, 0, e);
}

static void register_read(struct kevent* ev, posixc_event* e){
    EV_SET(ev, (uintptr_t)(e->fd), EVFILT_READ, ev_newly_add, 0, 0, e);
}

static void register_timer(struct kevent* ev, posixc_event* e){
    EV_SET(ev, (uintptr_t)(e->fd), EVFILT_TIMER, ev_newly_add, 0, (intptr_t)e->data_2, e);
}

static void register_user(struct kevent* ev, posixc_event* e){
    EV_SET(ev, (uintptr_t)(e->fd), EVFILT_USER, ev_newly_add, 0, 0, e);
}

static void register_sig(struct kevent* ev, posixc_event* e){
    EV_SET(ev, (uintptr_t)(e->fd), EVFILT_SIGNAL, ev_newly_add, 0, 0, e);
}

static void del_write(struct kevent* ev, posixc_event* e){
    EV_SET(ev, (uintptr_t)(e->fd), EVFILT_WRITE, EV_DELETE, 0, 0, e);
}

static void del_read(struct kevent* ev, posixc_event* e){
    EV_SET(ev, (uintptr_t)(e->fd), EVFILT_READ, EV_DELETE, 0, 0, e);
}

static void del_timer(struct kevent* ev, posixc_event* e){
    EV_SET(ev, (uintptr_t)(e->fd), EVFILT_TIMER, EV_DELETE, 0, (intptr_t)e->data_2, e);
}

static void del_sig(struct kevent* ev, posixc_event* e){
    EV_SET(ev, (uintptr_t)(e->fd), EVFILT_SIGNAL, EV_DELETE, 0, 0, e);
}

static void del_user(struct kevent* ev, posixc_event* e){
    EV_SET(ev, (uintptr_t)(e->fd), EVFILT_USER, EV_DELETE, 0, 0, e);
}

#define ev_submit (EV_ENABLE | EV_DISPATCH | EV_CLEAR)

static void submit_read(struct kevent* ev, posixc_event* e){
	EV_SET(ev, (uintptr_t)(e->fd), EVFILT_READ, ev_submit, 0, 0, e);
}

static void submit_write(struct kevent* ev, posixc_event* e){
	EV_SET(ev, (uintptr_t)(e->fd), EVFILT_WRITE, ev_submit, 0, 0, e);
}

static void submit_sig(struct kevent* ev, posixc_event* e){
	EV_SET(ev, (uintptr_t)(e->fd), EVFILT_SIGNAL, ev_submit, 0, 0, e); //fd is unused
}

static void submit_timer(struct kevent* ev, posixc_event* e){
    uint32_t flag = EV_ENABLE|EV_CLEAR;
    if(e->data_1==1){
        flag|=EV_DISPATCH;
    }
    EV_SET(ev, (uintptr_t)(e->fd), EVFILT_TIMER, flag, 0, (intptr_t)e->data_2, e);
}

static void submit_user(struct kevent* ev, posixc_event* e){
	EV_SET(ev, (uintptr_t)(e->fd), EVFILT_USER, ev_submit, 0, 0, e); //fd is unused
}

static bool kevent_set(int reactor_id, struct kevent* ev, int nr_ev){
    return kevent(reactor_id, ev, nr_ev, NULL, 0, NULL)==0;
}

bool posixc_event_plat_create(posixc_event*e){
    struct kevent ev[2];
    int i=0;
    switch(e->type){
    case SOCK_EV:
        register_read(&ev[i++], e);
        register_write(&ev[i++], e); 
        break;
    case SIG_EV:
        register_sig(&ev[i++], e); 
        break;
    case USER_EV:
        register_user(&ev[i++], e); 
        break;
    case TIMER_EV:
        register_timer(&ev[i++], e); 
        break;
    default:
        return false;
    }
    return kevent_set(e->reactor->id, ev, i);
}

bool posixc_event_plat_destroy(posixc_event*e){
    struct kevent ev[2];
    int i=0;
    switch(e->type){
    case SOCK_EV:
        del_read(&ev[i++], e);
        del_write(&ev[i++], e);
        break;
    case SIG_EV:
        del_sig(&ev[i++],e);
        break;
    case USER_EV:
        del_user(&ev[i++],e);
        break;
    case TIMER_EV:
        del_timer(&ev[i++],e);
        break;
    default:
        return false;
    }
    return kevent_set(e->reactor->id,ev,i);
}

bool posixc_event_plat_submit(posixc_event*e){
    struct kevent ev[2];
    int i=0;
    switch(e->type){
    case SOCK_EV:
        if(e->evmask & POSIXC_EVENT_IN) submit_read(&ev[i++], e);
	    if(e->evmask & POSIXC_EVENT_OUT) submit_write(&ev[i++], e);
        break;
    case SIG_EV:
        if(e->evmask & POSIXC_EVENT_SIG) submit_sig(&ev[i++],e);
        break;
    case USER_EV:
        if(e->evmask & POSIXC_EVENT_USER) submit_user(&ev[i++],e);
        break;
    case TIMER_EV:
        if(e->evmask & POSIXC_EVENT_TIMER) submit_timer(&ev[i++],e);
        break;
    default:
        return false;
    }
    if(i==0) return false;
	return kevent_set(e->reactor->id,ev,i);
}