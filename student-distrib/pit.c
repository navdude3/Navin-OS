#include "pit.h"
#include "process.h"


/* 
 * pit_init
 *   DESCRIPTION: initializes our PIT for scheduling  
 *   INPUTS: none
 *   OUTPUTS: none, but PIT devices is enabled and PIT frequency is set and 
 *   RETURN VALUE: none
*/
void pit_init() {
    outb(MODE, MODE_REG);                       /* Sending mode to command register */
    outb((FREQ & 0xFF), CHANNEL);               /* Sends bottom 8 bits of reload value */
    outb((FREQ >> 8), CHANNEL);                 /* Sends top 8 bits of reload value */
    enable_irq(PIT_IRQ);                        /* End of interrupt on PIT*/
    return;
}

   
/* 
 * pit_link_handler
 *   DESCRIPTION: main function for schedular to allow switching between process among 3 terminals  
 *   INPUTS: s_frame - registers to be saved during interrupt
 *   OUTPUTS: none, but registers saved and updated to switch to new processes
 *   RETURN VALUE: none
*/
void pit_link_handler(pt_regs_int_t s_frame) { /* aka scheduler */
    cli();
    pcb_t* current_pcb = get_cur_proc();

    if(current_pcb == NULL) {
        send_eoi(PIT_IRQ);
        sti();
        return;  
    }

    /* Saving current esp and ebp*/
    asm volatile(
        "movl %%esp, %0       \n"
        "movl %%ebp, %1       \n"
        : "=r" (current_pcb->sched_esp), "=r" (current_pcb->sched_ebp)
    );
    
    int32_t temp = (current_pcb->term_id + 1) % 3;                  /* Updating terminal */

    /* Shell has not been launched in that terminal */
    if(process_array[temp] == -1) {                                 /* Launches 3 shells upon boot */
        send_eoi(PIT_IRQ);
        sys_execute((uint8_t*)"shell");    
    }

    /* Gets PCB of process running on next terminal */
    pcb_t* next_pcb = get_pcb((int32_t)(process_array[temp]));

    /* Save tss info for next process */
    tss.ss0 = KERNEL_DS;
    tss.esp0 = USER_MEMORY_BASE - (KERNEL_AREA_SIZE * next_pcb->pid);
    
    /* Set up user page */
    setup_user_page(next_pcb->pid);

    set_cur_proc(next_pcb);                                         /* Updates current process */

    send_eoi(PIT_IRQ);

    /* Switch to next processes esp and ebp*/
    asm volatile(
        "movl %0, %%esp       \n"
        "movl %1, %%ebp       \n"
        :
        : "r" (next_pcb->sched_esp), "r" (next_pcb->sched_ebp)
        : "esp" , "ebp"
    );
    sti();
    return;
}
