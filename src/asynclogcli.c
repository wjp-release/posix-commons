#include "asynclogcli.h"
#include "ipc.h"
#include "common/timestr.h"

void posixc_asynclogcli_init(posixc_asynclogcli*cli, const char* usockpath, int lowest){
    cli->send_fd=posixc_ipc_connect_dgram(usockpath);
    cli->lowest_log_level=lowest;
    if(cli->send_fd<0) fprintf(stderr,"dgram sock not connected.\n");
}


void posixc_asynclogcli_log(posixc_asynclogcli*cli, const char* what, int len, int level){
    if(level<cli->lowest_log_level) return;
    const char* tmp=NULL;
    switch(level){
    case LOG_LEVEL_DEBUG:
        tmp=" Debug: ";
        break;
    case LOG_LEVEL_INFO:
        tmp=" Info: ";
        break;
    case LOG_LEVEL_WARN:
        tmp=" Warn: ";
        break;
    case LOG_LEVEL_ERR:
        tmp=" Err: ";
        break;
    case LOG_LEVEL_FATAL:
        tmp=" Fatal: ";
        break;
    default:
        return;
    }
    char buf[4096];
    int pos=posixc_default_timestr(buf);
    strcpy(buf+pos,tmp);
    pos+=strlen(tmp);
    if(pos+len>4095){
        len=4095-pos; //discard overflowing bytes
    }
    memcpy(buf+pos,what,len);
    pos+=len;
    buf[pos]='\n';
    posixc_ipc_send_dgram(cli->send_fd,buf,pos+1);
}

void posixc_asynclogcli_logstr(posixc_asynclogcli*cli, const char* what, int level){
    posixc_asynclogcli_log(cli, what, strlen(what), level);
}


