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
// Premake parent directories 
FILE*   posixc_fopen(const char* filename, const char *mode); 
bool    posixc_stat(struct stat* statbuf, const char* filename);
bool    posixc_is_stale(struct stat* statbuf, int age_seconds);
bool    posixc_is_user_rwx(struct stat* statbuf);
// Retries if interrupted
bool    posixc_read(int fd, char* buf, ssize_t* read_size, size_t n);
bool    posixc_skip(int fd, size_t n);
int     posixc_readonly_open(const char* filename);
// Premake parent directories 
int     posixc_writable_open(const char* filename);
// Premake parent directories 
int     posixc_appendable_open(const char* filename);
// Note that random access performance is sub-ideal.
bool    posixc_pread(int fd, char* buf, ssize_t* read_size, size_t n, uint64_t offset);
bool    posixc_unbuffered_write(int fd, const char* data, size_t size);
void    posixc_buffered_writable_init(posixc_buffered_writable* bw, int fd);
void    posixc_buffered_writable_init_from(posixc_buffered_writable* bw, const char* filename);

bool    posixc_buffered_write(posixc_buffered_writable* bw, const char* data, size_t size);
bool    posixc_flush(posixc_buffered_writable* bw);

struct posixc_buffered_writable
{
    char buf[buffered_writable_size];
    size_t pos;
    int fd;
};

