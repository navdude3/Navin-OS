#include "pit.h"
#include "process.h"


// static int32_t term_id_parser; // no longer used


void pit_init() {
    outb(MODE, MODE_REG);                       /* Sending mode to command register */
    outb((FREQ & 0xFF), CHANNEL);               /* Sends bottom 8 bits of reload value */
    outb((FREQ >> 8), CHANNEL);                 /* Sends top 8 bits of reload value */
    // term_id_parser = -1;                         /* Initialize variable */
    // sched_flag = -1;
    enable_irq(PIT_IRQ);                        /* End of interrupt on PIT*/
    return;
}

   

void pit_link_handler(pt_regs_int_t s_frame) { /* aka scheduler */
    cli();
    // pcb_t* current_pcb = get_pcb((int32_t)(process_array[term_id_parser]));
    pcb_t* current_pcb = get_cur_proc();

    // if(cur_process != current_pcb){
    //     send_eoi(PIT_IRQ);
    //     sti();
    //     return; 
    // }
    // if(sched_flag != -1){
    //     term_id_parser = sched_flag;
    //     sched_flag = -1;
    // }
    
 
    if(current_pcb == NULL) {
        send_eoi(PIT_IRQ);
        sti();
        return;  
    }

    /* Saving current esp and ebp*/
    asm volatile(
        "movl %%esp, %0       \n"
        "movl %%ebp, %1       \n"
        : "=r" (current_pcb->saved_esp), "=r" (current_pcb->saved_ebp)
    );
    current_pcb->saved_regs = s_frame;
    int32_t temp = (current_pcb->term_id + 1) % 3;

    if(process_array[temp] == -1) {                                 /* Launches 3 shells upon boot */
        switch_terms(temp);
        clear();
        send_eoi(PIT_IRQ);
        // sti();
        sys_execute((uint8_t*)"shell");    
    }


    pcb_t* next_pcb = get_pcb((int32_t)(process_array[temp]));

    /* Save tss info for next process */
    tss.ss0 = KERNEL_DS;
    tss.esp0 = USER_MEMORY_BASE - (KERNEL_AREA_SIZE * next_pcb->pid);
    
    /* Set up user page */
    setup_user_page(next_pcb->pid);

    memcpy(&s_frame, &(next_pcb->saved_regs), sizeof(pt_regs_int_t));
    // cur_process = next_pcb;
    set_cur_proc(next_pcb);

    send_eoi(PIT_IRQ);

    /* Switch to next processes esp and ebp*/
    asm volatile(
        "movl %0, %%esp       \n"
        "movl %1, %%ebp       \n"
        :
        : "r" (next_pcb->saved_esp), "r" (next_pcb->saved_ebp)
        : "esp" , "ebp"
    );
    sti();
    return;
}
