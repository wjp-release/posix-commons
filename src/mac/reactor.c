#include "reactor.h"

#include <fcntl.h>
#include <sys/event.h>
#include <sys/socket.h>

struct posixc_reactor{
	pthread_mutex_t  mtx;
	pthread_t        thread;   
    int              kq;   
	list_head        gc_events; 
};

