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

int posixc_delete_file(const char* fname){
    return unlink(fname);
}

void posixc_rename_file(const char* src, const char* target){
    return rename(src, target);
}   

bool posixc_dir_exists(const char* dir){
    DIR* tmp=opendir(dir)
    if(tmp){
        closedir(tmp);
        return true;
    } 
    return false;
}

#define tmp_cap 1024

void posixc_recursive_mkdir(const char *dir) {
    char tmp[tmp_cap];
    snprintf(tmp, sizeof(tmp),"%s",dir);
    size_t len = strlen(tmp);
    assert(len<=tmp_cap);
    if(tmp[len-1] == '/') tmp[len-1] = 0;
    for(char* p=tmp+1; *p; p++){
        if(*p == '/') {
            *p = 0;
            mkdir(tmp, S_IRWXU|S_IRWXG|S_IROTH);
            *p = '/';
        }
    }
    mkdir(tmp, S_IRWXU|S_IRWXG|S_IROTH);
}

void posixc_prepare_dirs_for(const char* filename){
    char tmp[tmp_cap];
    snprintf(tmp, sizeof(tmp),"%s",dir);
    size_t len = strlen(tmp);
    assert(len<=tmp_cap);
    char*t = tmp+len-1; 
    while( *t != '/') t--;
    *t = 0;
    if(!posixc_dir_exists(tmp)){
        for(char* p=tmp+1; *p; p++){
            if(*p == '/') {
                *p = 0;
                mkdir(tmp, S_IRWXU|S_IRWXG|S_IROTH);
                *p = '/';
            }
        }
        mkdir(tmp, S_IRWXU|S_IRWXG|S_IROTH);
    }
}

FILE* posixc_fopen(const char* filename, const char *mode){
    posixc_prepare_dirs_for(filename);
    return fopen(filename, mode);
}