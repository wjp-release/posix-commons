#include <sys/socket.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/un.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "ipc.h"
#include "fileio.h"
// Unix Domain Socket

// The struct sockaddr_un has additional data member sun_len on Mac and FreeBSD, compared to Linux and Solaris. It doesn't matter unless usockpath is unreasonably long.
static void set_sockaddr_un(struct sockaddr_un* addr, const char* usockpath){
    memset(addr, 0, sizeof(*addr));
    addr->sun_family = AF_UNIX;
    strncpy(addr->sun_path, usockpath, sizeof(addr->sun_path)-1);
    //Bind will fail if usockpath already exists. Caller should make sure this usockpath is not in use by calling posixc_ipc_usockpath_already_used first.
    unlink(usockpath); 
}

// Note that: this function returns a dgram socket fd that's already capable of receiving datagrams from peers. It can't distinguish senders without reading datagrams though. 
int posixc_ipc_dgram_socket(const char* usockpath){
    int fd = socket(AF_UNIX, SOCK_DGRAM, 0);
    if(fd==-1) return -1;
    struct sockaddr_un addr;
    set_sockaddr_un(&addr, usockpath); 
    int rv=bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(rv==-1)return -1;
    //Damn, I am good! No need for listen/accept!
    return fd;
}

// This function has no access to all usockpaths created by the application. Therefore it just create a temporary probe that attempts to connect this unix domain socket. It could generate false positives.
bool posixc_ipc_usockpath_already_used(const char* usockpath){
    int fd=socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, usockpath, sizeof(addr.sun_path)-1);
    fcntl(fd, F_SETFL, O_NONBLOCK);
    if (connect(fd, (void *) &addr, sizeof(addr)) < 0) {
        if (errno == ECONNREFUSED) { 
            return false; // Definitely not used.
        }
    }
    return true; // Almost certainly in use.
}

// This socket has binded and listened. The next step is to put it into a reactor and set up a do-accept event handler. Or you can simply call accept on blocking mode.
int posixc_ipc_nonblock_stream_socket(const char* usockpath){
    int fd=socket(AF_UNIX, SOCK_STREAM, 0);
    if(fd==-1) return -1;
    struct sockaddr_un addr;
    set_sockaddr_un(&addr, usockpath);
    int rv = bind(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(rv==-1) return -1;
    rv = listen(fd, 128);
    if(rv!=0){ 
        unlink(usockpath);    
        return -1;
    }
    return fd;
}

// usockpath cannot be older than age_of_usockpath_sec seconds
bool posixc_ipc_getuid(const char* usockpath, uid_t* uid, int age_of_usockpath_sec){
    struct stat statbuf;
    if(stat(usockpath, &statbuf)!=0) return false;
    #ifdef S_ISSOCK
    //not a socket
    if(S_ISSOCK(statbuf.st_mode)==0) return false;
    #endif
    //not rwx
    if(posixc_is_user_rwx(&statbuf) && !posixc_is_stale(&statbuf, age_of_usockpath_sec)){
        *uid=statbuf.st_uid;
        return true;
    }
    return false;
}