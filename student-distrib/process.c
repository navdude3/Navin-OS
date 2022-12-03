#include "process.h"
#include "loader.h"
#include "terminal.h"

static pcb_t* cur_process;                     /* Current active process */     
/* 
 * get_cur_proc
 *   DESCRIPTION: Returns current process number
 *   INPUTS: None
 *   OUTPUTS: None
 *   RETURN VALUE: Current Process 
*/
pcb_t* get_cur_proc() {
    return cur_process;    
}

/* 
 * set_cur_proc
 *   DESCRIPTION: sets the cur_process based on the new process to switch to
 *   INPUTS: new_process - the new PCB to set the process to
 *   OUTPUTS: cur_process is set to the new process
 *   RETURN VALUE: None 
*/
void set_cur_proc(pcb_t* new_proc){
    cur_process = new_proc;
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
