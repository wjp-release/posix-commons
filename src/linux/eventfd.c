#include "eventfd.h"
#include <sys/eventfd.h>
#include <unistd.h>
#include <fcntl.h>

int posixc_efd_open()
{
    int fd=eventfd(0, EFD_CLOEXEC);
    fcntl(fd, F_SETFD, FD_CLOEXEC);
    fcntl(fd, F_SETFL, O_NONBLOCK);
    return fd;
}

void posixc_efd_send(int efd)
{
    uint64_t one=1;
    write(efd,&one,sizeof(one));
}

void posixc_efd_recv(int efd)
{
    uint64_t duh;
    read(efd, &duh, sizeof(duh));
}

