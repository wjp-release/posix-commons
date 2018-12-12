#pragma once

//Use this header instead of internal headers, so that c++ targets can directly link c lib
#ifdef __cplusplus 
extern "C" {    
#endif

#include "internal.h"

#include "common/boyermoore.h"
#include "common/kmp.h"
#include "common/utils.h"
#include "common/math.h"
#include "common/slice.h"
#include "common/list.h"
#include "common/timestr.h"

#include "fileio.h"
#include "ipc.h"
#include "log.h"
#include "runtime.h"
#include "tcp.h"
#include "asynclogcli.h"
#include "asynclogsvr.h"
#include "event.h"
#include "reactor.h"

#ifdef __linux__
#include "linux/eventfd.h"
#include "linux/signalfd.h"
#endif

#ifdef __APPLE__
#endif

#ifdef __cplusplus
}
#endif
