#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <poll.h>

#include "reactor.h"
#include "event.h"

#define max_events 64
#define loops_per_gc 128

static void consume_ev(const struct kevent* ev){
    int evmask_to_consume=0;
    switch(ev->filter){
    case EVFILT_READ:
        evmask_to_consume=POSIXC_EVENT_IN;
        break;
    case EVFILT_WRITE:
        evmask_to_consume=POSIXC_EVENT_OUT;
        break;
	case EVFILT_SIGNAL:
		evmask_to_consume=POSIXC_EVENT_SIG;
		break;
	case EVFILT_TIMER:
		evmask_to_consume=0;//not consume timer events as they are periodic
		break;
	case EVFILT_USER:
		evmask_to_consume=POSIXC_EVENT_USER;
		break;
    default:
        return;
    }
	posixc_event*e = (posixc_event*)ev->udata;
    if(ev->flags&EV_ERROR) evmask_to_consume |= POSIXC_EVENT_ERR;
	posixc_event_cb cb;
	void* arg;
	posixc_event_consume(e, evmask_to_consume, &cb, &arg);
	if(cb!=NULL) cb(e,evmask_to_consume, arg);
}

void* posixc_reactor_plat_threadfn(void*arg){
	posixc_reactor* reactor=arg;
	for(unsigned i=0; !reactor->closing;i++){
		struct kevent events[max_events];
		int n = kevent(reactor->id,NULL,0,events,max_events,NULL);
		if(n<0 && errno==EBADF) return NULL;
		for(int j=0; j<n; j++){
			const struct kevent* ev=&events[j];
			consume_ev(ev);
		}
		if(i%loops_per_gc == 0) posixc_reactor_gc(reactor);
	}
	return NULL;
}

int posixc_reactor_plat_create(){
	return kqueue();
}