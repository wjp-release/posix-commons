#include "runtime.h"

int posixc_nr_cpus(){
    return sysconf(_SC_NPROCESSORS_ONLN);
}

int posixc_hostname_length_max(){
    return sysconf(_SC_HOST_NAME_MAX);
}

int posixc_nr_clock_ticks_per_sec(){
    return sysconf(_SC_CLK_TCK);
}

int posixc_nr_files_opened_max(){
    return sysconf(_SC_OPEN_MAX);
}

int posixc_page_size(){
    return sysconf(_SC_PAGESIZE);
}

int posixc_nr_pages(){
    return sysconf(_SC_PHYS_PAGES);
}

int posixc_nr_pages_available(){
    return sysconf(_SC_AVPHYS_PAGES);
}