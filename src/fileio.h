#pragma once

#include "internal.h"

extern int posixc_fd_limit();
extern int64_t posixc_file_size(const char* fname);
extern bool posixc_is_regular_file(const char* fname);
extern int posixc_truncate_file(const char* fname, int64_t bytes);
extern int posixc_create_dir(const char* dir);
extern int posixc_delete_dir(const char* dir);
extern bool posixc_file_exists(const char* fname);
extern int posix_delete_file(const char* fname);
extern void posix_rename_file(const char* src, const char* target);
