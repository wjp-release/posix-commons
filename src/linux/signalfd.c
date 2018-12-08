#include "signalfd.h"

#include <signal.h>
#include <sys/signalfd.h>

static void posixc_block_all_signals(){
    sigset_t all;
    sigfillset(&all);
    sigprocmask(SIG_SETMASK,&all,NULL);
}

int posixc_signalfd_open(int* signals, int n){
    posixc_block_all_signals();
    sigset_t tmp;
    sigemptyset(&tmp);
    for(int i=0; i <n ; i++){
        sigaddset(&tmp, signals[i]);
    } 
    return signalfd(-1, &tmp, 0);
}

bool posixc_signalfd_read(int signalfd, int* signo){
    struct signalfd_siginfo info;
    ssize_t nr = read(signalfd, &info, sizeof(info));
    if (nr != sizeof(info)) {
        return false;
    }
    signo=info.ssi_signo;
    return true;
}

