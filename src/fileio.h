#pragma once

#include "internal.h"

int     posixc_fd_limit();
int64_t posixc_file_size(const char* fname);
bool    posixc_is_regular_file(const char* fname);
int     posixc_truncate_file(const char* fname, int64_t bytes);
int     posixc_create_dir(const char* dir);
int     posixc_delete_dir(const char* dir);
bool    posixc_file_exists(const char* fname);
int     posixc_delete_file(const char* fname);
int     posixc_rename_file(const char* src, const char* target);
void    posixc_recursive_mkdir(const char *dir);
void    posixc_prepare_dirs_for(const char* filename); // creates dirs if they don't exist
FILE*   posixc_fopen(const char* filename, const char *mode); 

