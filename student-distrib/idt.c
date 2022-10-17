#include "idt.h"

void idt_init(){
    int i;
    for(i = 0; i < 20; i++){

        if(i == 15){
            idt[i].present = 0;
        }
        else{
            idt[i].present = 1;
            idt[i].reserved4 = 0;
            idt[i].reserved3 = 1;
            idt[i].reserved2 = 1;
            idt[i].reserved1 = 1;
            idt[i].reserved0 = 0;
            idt[i].size = 1;
            idt[i].dpl = 0;
            idt[i].seg_selector = KERNEL_CS;
        }
    }
    for(i = 20; i < 47; i++){
            idt[i].present = 1;
            idt[i].reserved4 = 0;
            idt[i].reserved3 = 0;
            idt[i].reserved2 = 1;
            idt[i].reserved1 = 1;
            idt[i].reserved0 = 0;
            idt[i].size = 1;
            idt[i].seg_selector = KERNEL_CS;
            idt[i].dpl = 0;
    }
    for(i = 47; i < NUM_VEC; i++){
        idt[i].present = 1;
        idt[i].reserved4 = 0;
        idt[i].reserved3 = 0;
        idt[i].reserved2 = 1;
        idt[i].reserved1 = 1;
        idt[i].reserved0 = 0;
        idt[i].size = 1;
        idt[i].seg_selector = KERNEL_CS;
        idt[i].dpl = 0;     
    }
    idt[33].reserved3 = 1;




// typedef union idt_desc_t {
//     uint32_t val[2];
//     struct {
//         uint16_t offset_15_00;
//         uint16_t seg_selector;
//         uint8_t  reserved4;
//         uint32_t reserved3 : 1;
//         uint32_t reserved2 : 1;
//         uint32_t reserved1 : 1;
//         uint32_t size      : 1;
//         uint32_t reserved0 : 1;
//         uint32_t dpl       : 2;
//         uint32_t present   : 1;
//         uint16_t offset_31_16;

    //x21 is keyboard port
    
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
    //SET_IDT_ENTRY(idt[15], RESERVED_15);
    SET_IDT_ENTRY(idt[16], floating_point_error);
    SET_IDT_ENTRY(idt[17], alignment_check);
    SET_IDT_ENTRY(idt[18], machine_check);
    SET_IDT_ENTRY(idt[19], simd_floating_point_exception);



    SET_IDT_ENTRY(idt[33], keyboard_linkage);
    SET_IDT_ENTRY(idt[40], rtc_linkage);




    lidt(idt_desc_ptr);
}

