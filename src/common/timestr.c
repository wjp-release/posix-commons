#include "timestr.h"

int posixc_timestr(char*buffer, const char* pattern,  int len){
    time_t rawtime;
    time(&rawtime);
    struct tm* timeinfo = localtime(&rawtime);
    return strftime(buffer,len, pattern, timeinfo);
}

int posixc_default_timestr(char*buffer){
    time_t rawtime;
    time(&rawtime);
    struct tm* timeinfo = localtime(&rawtime);
    return strftime(buffer,80,"%d-%m-%Y %H:%M:%S",timeinfo);
}

int posixc_timestr_logfile(char*buffer){
    time_t rawtime;
    time(&rawtime);
    struct tm* timeinfo = localtime(&rawtime);
    return strftime(buffer,80,"%d-%m-%Y_%H:%M:%S.log",timeinfo);
}