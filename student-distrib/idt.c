#include "idt.h"

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
//     } __attribute__ ((packed));
// } idt_desc_t;

void idt_init(){
    int i;
    
    for(i = 0; i < 20; i++){

        if(i == 1 || i == 15){
            idt[i].present = 0;
        }
        else{
            idt[i].present = 1;
        }
    }
    lidt(idt_desc_ptr);

    SET_IDT_ENTRY(idt[0], divide_error);
    //SET_IDT_ENTRY(idt[1], RESERVED_1);
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
}


void handle_exception(int id) {
    printf("Exception found: \n");
    char c = interrupt_names[id];
    printf("%c", c);
    while(1) {
        continue;
    }
}