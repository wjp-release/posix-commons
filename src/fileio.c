#include "fileio.h"
#include <fcntl.h>          // fcntl, posix_fadvise
#include <dirent.h>         // readdir, opendir, closedir
#include <sys/resource.h>   // getrlimit 
#include <sys/stat.h>       // stat, S_ISREG
#include <unistd.h>         // read, open, write, close, lseek, pread

int posixc_fd_limit()
{
    struct rlimit rlim;
    if(getrlimit(RLIMIT_NOFILE, &rlim)==-1){
        return -1;
    }else if(rlim.rlim_cur==RLIM_INFINITY){
        return INT_MAX;
    }
    else{
        return rlim.rlim_cur;
    }
}

int64_t posixc_file_size(const char* fname)
{
    struct stat sbuf;
    if (stat(fname, &sbuf) != 0) {
        return -1;
    } else {
        return sbuf.st_size;
    }
}

bool posixc_is_regular_file(const char* fname)
{
    struct stat sbuf;
    if (stat(fname, &sbuf) != 0) {
        return false;
    } else {
        return S_ISREG(sbuf.st_mode);
    }
}

int posixc_truncate_file(const char* fname, int64_t bytes){
    return truncate(fname, (off_t)(bytes));
}

int posixc_create_dir(const char* dir){
    return mkdir(dir, 0755);
}

int posixc_delete_dir(const char* dir){
    return rmdir(dir);
}

bool posixc_file_exists(const char* fname){
    return access(fname, F_OK);
}

int posix_delete_file(const char* fname){
    return unlink(fname);
}

void posix_rename_file(const char* src, const char* target){
    return rename(src, target);
}   

