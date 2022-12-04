#include "idt.h"

/* 
 * init_idt
 *   DESCRIPTION: initializes the entries of the IDT, sets the first 20 exceptions as trap gates, sets the handler address to the proper handler
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
*/
void idt_init(){
    int i;
    /* First 20 entries are the exceptions */
    for(i = 0; i < 20; i++){

        if(i == 15){
            idt[i].present = 0;
        }
        else{
            /* Reserved set to 01110 to enable trap gate - INTEL pg 156 */
            idt[i].present = 1;
            idt[i].reserved4 = 0;
            idt[i].reserved3 = 1;
            idt[i].reserved2 = 1;
            idt[i].reserved1 = 1;
            idt[i].reserved0 = 0;
            idt[i].size = 1;
            idt[i].dpl = 0;                     /* Kernel has descriptor level priviledge */
            idt[i].seg_selector = KERNEL_CS;    /* Enables kernel code segment */
        }
    }
    for(i = 20; i < NUM_VEC; i++){
            /* Reserved set to 00110 to enable interrupt gate - INTEL pg 156 */
            idt[i].present = 1;
            idt[i].reserved4 = 0;
            idt[i].reserved3 = 0;
            idt[i].reserved2 = 1;
            idt[i].reserved1 = 1;
            idt[i].reserved0 = 0;
            idt[i].size = 1;
            idt[i].seg_selector = KERNEL_CS;    /* Enables kernel code segment */
            idt[i].dpl = 0;                     /* Kernel has descriptor level priviledge */
    }
    idt[32].reserved3 = 1;                      /* Enabling trap gate for PIT */
    idt[33].reserved3 = 1;                      /* Enabling trap gate for keyboard */
    idt[128].dpl = 3;                           /* Setting DPL for SYS CALLS */

    //x21 is keyboard port
    /* Setting the corresponding IDT entry with the appropriate exception */
    SET_IDT_ENTRY(idt[0], divide_error);
    SET_IDT_ENTRY(idt[1], RESERVED_1);
    SET_IDT_ENTRY(idt[2], nmi_int);
    SET_IDT_ENTRY(idt[3], breakpoint);
    SET_IDT_ENTRY(idt[4], overflow);
    SET_IDT_ENTRY(idt[5], bound_range);
    SET_IDT_ENTRY(idt[6], invalid_opcode);
    SET_IDT_ENTRY(idt[7], device_not_available);
    SET_IDT_ENTRY(idt[8], double_fault);
    SET_IDT_ENTRY(idt[9], segment_overrun);
    SET_IDT_ENTRY(idt[10], invalid_tss);
    SET_IDT_ENTRY(idt[11], seg_not_present );
    SET_IDT_ENTRY(idt[12], seg_fault);
    SET_IDT_ENTRY(idt[13], general_protection);
    SET_IDT_ENTRY(idt[14], page_fault);
    SET_IDT_ENTRY(idt[16], floating_point_error);
    SET_IDT_ENTRY(idt[17], alignment_check);
    SET_IDT_ENTRY(idt[18], machine_check);
    SET_IDT_ENTRY(idt[19], simd_floating_point_exception);


    /* Setting the corresponding IDT entry with the linkage for devices */
    SET_IDT_ENTRY(idt[32], pit_linkage);
    SET_IDT_ENTRY(idt[33], keyboard_linkage);
    SET_IDT_ENTRY(idt[40], rtc_linkage);
    lidt(idt_desc_ptr);

    SET_IDT_ENTRY(idt[0x80], syscall_linkage);    //128 in decimal

}

