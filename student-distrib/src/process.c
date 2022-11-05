#include "process.h"
#include "loader.h"

pcb_t* get_curr_pcb() {
    get_pcb(curr_pid);    
}

pcb_t* get_pcb(uint32_t pid){
    if(pid < 0 || pid > 6) return NULL;
    return (pcb_t*) (USER_MEMORY_BASE - ((pid + 1) * PCB_SIZE));
}