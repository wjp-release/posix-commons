#include "signal.h"

void posixc_block_all_signals(){
    sigset_t all;
    sigfillset(&all);
    sigprocmask(SIG_SETMASK,&all,NULL);
}