#pragma once

#include "internal.h"

int posixc_signalfd_open(int* signals, int n);
bool posixc_signalfd_read(int signalfd, int* signo);