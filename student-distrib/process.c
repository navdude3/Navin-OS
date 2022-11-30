#include "process.h"
#include "loader.h"
#include "terminal.h"

/* 
 * get_curr_pcb
 *   DESCRIPTION: Returns current process number
 *   INPUTS: None
 *   OUTPUTS: None
 *   RETURN VALUE: Current Process 
*/
pcb_t* get_curr_pcb() {
    return cur_process;    
}

/* 
 * get_pcb
 *   DESCRIPTION: Returns pointer to PCB for a specified PID
 *   INPUTS: pid number to allocate space for
 *   OUTPUTS: None
 *   RETURN VALUE: Pointer to PCB
*/
pcb_t* get_pcb(int32_t pid){
    if(pid < 0 || pid > 6) return NULL;
    return (pcb_t*) (USER_MEMORY_BASE - ((pid + 1) * PCB_SIZE));
}
