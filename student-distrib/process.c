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


void vid_remap(pcb_t* next_pcb) {
    pte_desc_t vidmap_entry;
    uint32_t term_id = get_curr_pcb()->term_id;
    uint32_t term_addr = (uint32_t) get_term(next_pcb->pid);

    /* If viewing terminal = terminal process is running on, map to physical memory */
    if (cur_term_id == term_id) 
        vidmap_entry.val =  (0xB8000| 0x7);   
    else /* map to a separate physical memory */
        vidmap_entry.val = term_addr | 0x7;
    /* Set page entry for that address */
    set_ptentry(usr_vidmap_table_desc.addr, term_id, vidmap_entry);
}