#include "pit.h"

void pit_init() {
    outb(MODE, MODE_REG);                       /* Sending mode to command register */
    outb(FREQ, CHANNEL);                        /* Sends bottom 8 bits of reload value */
    outb((FREQ >> 8), CHANNEL);                 /* Sends top 8 bits of reload value */
    send_eoi(PIT_IRQ);                          /* End of interrupt on PIT*/
    return;
}

void pit_handler() { /* aka scheduler */
    pcb_t* current_pcb = get_curr_pcb();

    /* Saving current esp and ebp*/
    asm volatile(
        "movl %%esp, %0       \n"
        "movl %%ebp, %1       \n"
        : "=r" (current_pcb->saved_esp), "=r" (current_pcb->saved_ebp)
    );

    cur_term_id = (cur_term_id + 1) % 3;
    pcb_t* next_pcb = get_pcb(process_array[cur_term_id])

    /* Save tss info for next process */
    tss.ss0 = KERNEL_DS;
    tss.esp0 = USER_MEMORY_BASE - (KERNEL_AREA_SIZE * next_pcb->pid);

    /* Set up user page */
    setup_user_page(next_pcb->pid);

    send_eoi(PIT_IRQ);
    /* Switch to next processes esp and ebp*/
    asm volatile(
        "movl %0, %%esp       \n"
        "movl %1, %%ebp       \n"
        :
        : "r" (next_pcb->saved_esp), "r" (next_pcb->saved_ebp)
        : "esp" , "ebp"
    );
    return;
}
