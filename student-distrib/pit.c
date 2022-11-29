#include "pit.h"
#include "process.h"


static int32_t term_id_parser;


void pit_init() {
    outb(MODE, MODE_REG);                       /* Sending mode to command register */
    outb((FREQ & 0xFF), CHANNEL);               /* Sends bottom 8 bits of reload value */
    outb((FREQ >> 8), CHANNEL);                 /* Sends top 8 bits of reload value */
    term_id_parser = -1;                         /* Initialize variable */
    enable_irq(PIT_IRQ);                        /* End of interrupt on PIT*/
    return;
}

void pit_link_handler() { /* aka scheduler */
    pcb_t* current_pcb = get_curr_pcb();

    /* Saving current esp and ebp*/
    asm volatile(
        "movl %%esp, %0       \n"
        "movl %%ebp, %1       \n"
        : "=r" (current_pcb->saved_esp), "=r" (current_pcb->saved_ebp)
    );
    current_pcb->saved_esp0 = tss.esp0;

    term_id_parser = (term_id_parser + 1) % 3;

    if(process_array[term_id_parser] == -1) {
        send_eoi(PIT_IRQ);
        return;       
    }
    pcb_t* next_pcb = get_pcb((int32_t)(process_array[term_id_parser]));

  
    /* Save tss info for next process */
    tss.ss0 = KERNEL_DS;
    //tss.esp0 = next_pcb->saved_esp0; //USER_MEMORY_BASE - (KERNEL_AREA_SIZE * next_pcb->pid);
    
    // /* Set up user page */
    //setup_user_page(next_pcb->pid);
    send_eoi(PIT_IRQ);

    // /* Switch to next processes esp and ebp*/
    // asm volatile(
    //     "movl %0, %%esp       \n"
    //     "movl %1, %%ebp       \n"
    //     :
    //     : "r" (next_pcb->saved_esp), "r" (next_pcb->saved_ebp)
    //     : "esp" , "ebp"
    // );

    return;
}
