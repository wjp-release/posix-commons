#pragma once

#include "internal.h"

int posixc_reactor_create(posixc_reactor** r);
void posixc_reactor_destroy(posixc_reactor* r);
