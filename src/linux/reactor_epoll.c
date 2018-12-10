#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <errno.h>
#include "reactor.h"
#include "event.h"

#define max_events 64
#define loops_per_gc 128

static void consume_epollev(const struct epoll_event* ev){
	posixc_event*e = ev->data.ptr;
	int evmask_to_consume = ev->events & (EPOLLIN|EPOLLOUT|EPOLLERR);
	posixc_event_cb cb;
	void* arg;
	posixc_event_consume(e, evmask_to_consume, &cb, &arg);
	if(cb!=NULL) cb(e,evmask_to_consume, arg);
}

void* posixc_reactor_plat_threadfn(void*arg){
	posixc_reactor* reactor=arg;
	for(unsigned i=0; !reactor->closing;i++){
		struct epoll_event events[max_events];
		int n = epoll_wait(reactor->id,events,max_events,-1);
		if(n<0 && errno==EBADF) return NULL;
		for(int j=0; j<n; j++){
			const struct epoll_event* ev=&events[i];
			consume_epollev(ev);
		}
		if(i%loops_per_gc == 0) posixc_reactor_gc(reactor);
	}
	return NULL;
}

int posixc_reactor_plat_create(){
	return epoll_create1(EPOLL_CLOEXEC);
}