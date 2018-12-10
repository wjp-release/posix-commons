#include "posixc.h"

int main()
{
    printf("PosixC Asynchronous Logging Server\n");
    posixc_asynclogsvr server;
    posixc_asynclogsvr_init(&server, "/tmp", 1024, 3000);
    
    while(true);

    return 0;
}