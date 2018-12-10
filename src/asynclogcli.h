#pragma once

#include "internal.h"

struct posixc_asynclogcli{
    int lowest_log_level;
    int send_fd; 
};

void posixc_asynclogcli_init(posixc_asynclogcli*cli, const char* usockpath, int lowest);

void posixc_asynclogcli_log(posixc_asynclogcli*cli, const char* what, int len, int level);