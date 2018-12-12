#include "posixc.h"

int main()
{
    printf("PosixC Asynchronous Logging Client\n");
    posixc_asynclogcli client;
    posixc_asynclogcli_init(&client, usockpath_log, LOG_LEVEL_DEBUG);
    posixc_asynclogcli_logstr(&client, "wwwwww", LOG_LEVEL_INFO);
    posixc_asynclogcli_logstr(&client, "hello world!", LOG_LEVEL_ERR);

    return 0;
}