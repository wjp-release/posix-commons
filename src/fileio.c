#include "fileio.h"
#include <fcntl.h>          // fcntl, posix_fadvise
#include <dirent.h>         // readdir, opendir, closedir
#include <sys/resource.h>   // getrlimit 
#include <sys/stat.h>       // stat, S_ISREG
#include <unistd.h>         // read, open, write, close, lseek, pread
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "common/math.h"

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

int posixc_rename_file(const char* src, const char* target){
    return rename(src, target);
}   

bool posixc_dir_exists(const char* dir){
    DIR* tmp=opendir(dir);
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
    size_t len = (size_t) strlen(tmp);
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
    snprintf(tmp, sizeof(tmp),"%s",filename);
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

bool posixc_stat(struct stat* statbuf, const char* filename){
    return stat(filename, &statbuf)==0;
}

bool posixc_is_stale(struct stat* statbuf, int age_seconds){
    time_t staletime=time(0)-age_seconds;
    if(statbuf->st_atime<staletime||statbuf->st_ctime<staletime||statbuf->st_mtime<staletime){
        return false;
    }
    return true;
}

bool posixc_is_user_rwx(struct stat* statbuf){
    if(statbuf->st_mode & (S_IRWXG | S_IRWXO) || (statbuf->st_mode & S_IRWXU) != S_IRWXU){
        return false;
    }
    return true;
}

bool posixc_read(int fd, char* buf, ssize_t* read_size, size_t n){
    while (true) {
        *read_size = read(fd, buf, n);
        if (*read_size < 0) {
            if (errno == EINTR) continue; 
            return false;
        }
        break;
    }
    return true;
}

bool posixc_skip(int fd, size_t n){
    return lseek(fd_, n, SEEK_CUR) != (off_t) -1;
}

int posixc_readonly_open(const char* filename){
    return open(filename, O_RDONLY);
}

int posixc_writable_open(const char* filename){
    posixc_prepare_dirs_for(filename);
    return open(filename, O_TRUNC | O_WRONLY | O_CREAT, 0644);
}

int posixc_appendable_open(const char* filename){
    posixc_prepare_dirs_for(filename);
    return open(filename, O_APPEND | O_WRONLY | O_CREAT, 0644);
}

bool posixc_pread(int fd, char* buf, ssize_t* read_size, size_t n, uint64_t offset){
    *read_size = ::pread(fd, buf, n, (off_t)(offset));
    return *read_size>=0
}

bool posixc_unbuffered_write(int fd, const char* data, size_t size){
    while (size > 0) {
        ssize_t bytes_written=write(fd_, data, size);
        if (bytes_written < 0) {
            if (errno == EINTR) continue;  
            return false;
        }
        data += bytes_written;
        size -= bytes_written;
    }
    return true;
}

bool posixc_flush(posixc_buffered_writable* bw){
    int size=bw->pos;
    bw->pos=0;
    return posixc_unbuffered_write(bw->fd, bw->buf, size);
}
void posixc_buffered_writable_init(posixc_buffered_writable* bw, int fd){
    bw->pos=0;
    bw->fd=fd;
}

bool posixc_buffered_write(posixc_buffered_writable* bw, const char* data, size_t size){
    size_t n = posixc_min(size, buffered_writable_size - bw->pos);
    memcpy(bw->buf + bw->pos, data, n);
    data += n;
    size -= n;
    bw->pos += n;
    if (size == 0) return true;
    if(posixc_flush(bw)){
        if (size < buffered_writable_size) {
            memcpy(bw->buf, data, size);
            bw->pos = size;
            return true;
        }
        return posixc_unbuffered_write(bw->fd, data, size);
    }
    return false;
}

bool posixc_sync(int fd){
    return fdatasync(fd)==0;
}

int posixc_fd_limit(){
    struct rlimit rlim;
    if(getrlimit(RLIMIT_NOFILE, &rlim)==-1){
        return -1;
    }
    if(rlim.rlim_cur == RLIM_INFINITY) return INT_MAX;
    return rlim.rlim_cur;
}