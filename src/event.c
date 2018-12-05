#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <pthread.h>

#include "event.h"
#include "reactor.h"

void posixc_preparefd(int fd){
    fcntl(fd, F_SETFD, FD_CLOEXEC);
	fcntl(fd, F_SETFL, O_NONBLOCK);
    int one = 1;
	setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof(one)); 
}

static void posixc_event_init(posixc_event*event, posixc_reactor* reactor, int fd, posixc_event_cb cb, void* arg){
    posixc_preparefd(fd);
    event->reactor=reactor;
    event->fd=fd;
    event->cb=cb;
    event->arg=arg;
    event->evmask=0;
    event->closing=false;
    pthread_mutex_init(&event->mtx, NULL);
    INIT_LIST_HEAD(&event->node);
}

static void posixc_event_release(posixc_event*event){
    close(event->fd);
    pthread_mutex_destroy(&event->mtx);
    free(event);
}

posixc_event* posixc_event_create(posixc_reactor* reactor, int fd, posixc_event_cb cb, void* arg){
    posixc_event* event=(posixc_event*)malloc(sizeof(posixc_event));
    if(event==NULL) return NULL;
    posixc_event_init(event,reactor,fd,cb,arg);
    if(posixc_event_plat_create(event)){
         return event;
    }else{
		posixc_event_release(event);
		return NULL;
    }
}

void posixc_event_destroy(posixc_event*e){
    // Any user thread that holds a reference to the posixc_event could stop it from working.
	if (!e->closing) {  
        e->closing = true;
        posixc_event_plat_destroy(e);
		shutdown(e->fd, SHUT_RDWR);
        //@todo: add e into gc list
	}
    // Final destruction can only be performed by its owning reactor thread. This restriction ensures that the reactor won't process already destroyed posixc_events.
    if(pthread_self()==e->reactor->thread){
        posixc_event_release(e);
    }
}

bool posixc_event_submit(posixc_event* e, int evmask){
    if (e->closing) return false; 
	pthread_mutex_lock(&e->mtx);
	e->evmask |= evmask;
    bool success=posixc_event_plat_submit(e);
	pthread_mutex_unlock(&e->mtx);
    return success;
}

void posixc_event_consume(posixc_event*e, int evmask_to_consume, posixc_event_cb* cb, void** arg){
    pthread_mutex_lock(&e->mtx);
    e->evmask &= ~ evmask_to_consume;
    *cb=e->cb;
    *arg=e->arg;
    pthread_mutex_unlock(&e->mtx);
}