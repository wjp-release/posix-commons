#include "eventfd.h"
#include <sys/eventfd.h>
#include <unistd.h>
#include <fcntl.h>

int posixc_efd_open()
{
    int fd;
    qerror_if(fd=eventfd(0, EFD_CLOEXEC)<0, "cannot create eventfd");
    fcntl(fd, F_SETFD, FD_CLOEXEC);
    fcntl(fd, F_SETFL, O_NONBLOCK);
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

