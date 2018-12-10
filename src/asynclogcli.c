#include "asynclogcli.h"
#include "ipc.h"

void posixc_asynclogcli_init(posixc_asynclogcli*cli, const char* usockpath, int lowest){
    cli->send_fd=posixc_ipc_connect_dgram(usockpath);
    cli->lowest_log_level=lowest;
}


void posixc_asynclogcli_log(posixc_asynclogcli*cli, const char* what, int len, int level){
    if(level<cli->lowest_log_level) return;

}