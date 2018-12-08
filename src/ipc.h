#pragma once
#include "internal.h"

int posixc_ipc_dgram_socket(const char* usockpath);
bool posixc_ipc_usockpath_already_used(const char* usockpath);
int posixc_ipc_nonblock_stream_socket(const char* usockpath);

// Obtains client's user ID from the usockpath that it must bind before calling us.
bool posixc_ipc_getuid(const char* usockpath, uid_t* uid, int age_of_usockpath_sec);