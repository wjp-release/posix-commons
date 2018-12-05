#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>

#include "event.h"
#include "reactor.h"

void posixc_reactor_destroy(posixc_reactor* reactor){
	posixc_reactor_gc(reactor);
	reactor->closing=true;
	pthread_join(&reactor->thread, NULL);
	close(reactor->id);
	pthread_mutex_destroy(&reactor->mtx);
	free(reactor);
}

posixc_reactor* posixc_reactor_create(){
	int fd=posixc_reactor_plat_create();
	if(fd<0)return NULL;
	posixc_reactor* reactor=(posixc_reactor*)malloc(sizeof(posixc_reactor));
	reactor->id=fd;
	reactor->closing=false;
	INIT_LIST_HEAD(&reactor->gc_events);
	pthread_mutex_init(&reactor->mtx);
	int rv=pthread_create(&reactor->thread, NULL, posixc_reactor_plat_threadfn, reactor);
	if(rv!=0)return NULL;
	return reactor;
}

void posixc_reactor_gc(posixc_reactor*r){
    //todo
}