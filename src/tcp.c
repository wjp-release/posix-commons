#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "tcp.h"
#include "common/slice.h"

/*

struct sockaddr {
  unsigned short sa_family; // address family, AF_xxx
  char sa_data[14]; // 14 bytes of protocol address
};
struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};
struct sockaddr_in {
  short int sin_family; // Address family, AF_INET
  unsigned short int sin_port; // Port number
  struct in_addr sin_addr; // Internet address
  unsigned char sin_zero[8]; // Same size as struct sockaddr
};
sizeof sockaddr === sizeof sockaddr_in === 16

struct sockaddr_storage {
	__uint8_t ss_len;		
	sa_family_t ss_family;	
	char __ss_pad1[_SS_PAD1SIZE]; 
	__int64_t __ss_align;	
	char __ss_pad2[_SS_PAD2SIZE]; 
};

struct sockaddr is just an opaque struct that has room enough to hold any address of the supported address families, be it AF_INET for IPv4, AF_INET6 for IPv6, AF_UNIX for UNIX domain sockets, etc.

To access individual fields making up the address of a specific family, one has to downcast it into the corresponding C struct, which means struct sockaddr_in for AF_INET, struct sockaddr_in6 for AF_INET6, struct sockaddr_un for AF_UNIX and so on.

*/

int posixc_tcp_socketfd(){
    return socket(AF_INET, SOCK_STREAM, 0);
}

void posixc_tcp_set_addr(struct sockaddr_in* sin, const char* host, int port){
    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr = inet_addr(host);
    sin->sin_port = htons(port);
}

void posixc_tcp_set_addr_any(struct sockaddr_in* sin, int port){
    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr = htonl(INADDR_ANY);
    sin->sin_port = htons(port);
}

bool posixc_tcp_is_addr_valid(struct sockaddr_in* sin){
    return sin->sin_addr.s_addr != INADDR_NONE;
}

int posixc_tcp_connect(int fd, struct sockaddr_in* sin){
    return connect(fd, (struct sockaddr*)sin, sizeof(*sin));
}

int posixc_tcp_bind(int fd, struct sockaddr_in* sin){
    return bind(fd, (struct sockaddr*)sin, sizeof(*sin));
}

int posixc_tcp_listen(int fd){
    return listen(fd,1);
}

int posixc_tcp_accept_fd(int fd, struct sockaddr_in* cin){
    socklen_t cin_sz = sizeof(*cin);
    return accept(fd, (struct sockaddr*)cin, &cin_sz);
}

void posixc_tcp_get_ip(posixc_slice* s, struct sockaddr_in* cin){
    posixc_slice_init_from(s, inet_ntoa(cin->sin_addr));
}

int posixc_tcp_get_port(struct sockaddr_in* cin){
    return (int)ntohs(cin->sin_port);
}

