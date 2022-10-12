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
}