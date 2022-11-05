#include "process.h"
#include "loader.h"

pcb_t* get_curr_pid() {
    if(curr_pid < 0 || curr_pid > 6) return NULL;
    return (pcb_t*) (USER_MEMORY_BASE - ((curr_pid + 1) * PCB_SIZE));
}