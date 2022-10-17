/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask = 0xFF; /* IRQs 0-7  */
uint8_t slave_mask = 0xFF;  /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void i8259_init(void) {

    int a;

    outb(master_mask, MASTER_8259_DATA);
    outb(slave_mask, SLAVE_8259_DATA);

    outb(ICW1, MASTER_8259_PORT);
    outb(ICW1, SLAVE_8259_PORT);

    outb(ICW2_MASTER, MASTER_8259_DATA);
    outb(ICW2_SLAVE, SLAVE_8259_DATA);


    outb(ICW3_MASTER, MASTER_8259_DATA);
    outb(ICW3_SLAVE, SLAVE_8259_DATA);

    outb(ICW4, MASTER_8259_DATA);
    outb(ICW4, SLAVE_8259_DATA);


    outb(master_mask, MASTER_8259_DATA);
    outb(slave_mask, SLAVE_8259_DATA);
    // d = inb(SLAVE_8259_DATA);

    // udelay(100);
    // clear();
    // printf("%x", c);
    // clear();

    //outb(a, MASTER_8259_DATA);
    //outb(b, SLAVE_8259_DATA);
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    uint16_t port;
    uint8_t value;
 
    if(irq_num < 8) {
        port = MASTER_8259_DATA;
    } else {
        port = SLAVE_8259_DATA;
        irq_num -= 8;
    }

    value = inb(port) & ~(1 << irq_num);
    clear();
   // printf("%x", value);
    outb(value, port);
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    uint16_t port;
    uint8_t value;
 
    if(irq_num < 8) {
        port = MASTER_8259_DATA;
    } else {
        port = SLAVE_8259_DATA;
        irq_num -= 8;
    }
    
    value = inb(port) | (1 << irq_num);
    outb(value, port);
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {

    if(irq_num >= 8){
        outb((EOI | irq_num), SLAVE_8259_PORT);
        outb((EOI | 0x2), MASTER_8259_PORT); //clear on primary as well
    } else {
        outb((EOI | irq_num), MASTER_8259_PORT);
    }
	
}


void set_all_irq(){
    int i;
    for(i = 0; i < 8; i++){
        enable_irq(i);
    }
}

void disable_all_irq(){
    int i;
    for(i = 0; i < 8; i++){
        disable_irq(i);
    }
}
