/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask = 0xFF; /* IRQs 0-7  */
uint8_t slave_mask = 0xFF;  /* IRQs 8-15 */
static spinlock_t i8259_lock = SPIN_LOCK_UNLOCKED; 

/* Initialize the 8259 PIC */
void i8259_init(void) {
    unsigned long flags;

    spin_lock_irqsave(&i8259_lock, flags);

    outb(0xFF, 0x21);
    outb(0xFF, 0xA1);

    outb_p(0x11, 0x20);
    outb_p(0x20, 0x21);
    outb_p(0x04, 0x21);
    outb_p(0x01, 0x21);

    outb_p(0x11, 0xA0);
    outb_p(0x20 + 8, 0xA1);
    outb_p(0x02, 0xA1);
    outb_p(0x01, 0xA1);

    udelay(100);

    outb(0xff, 0x21);
    outb(0xff, 0xA1);

    spin_unlock_irqrestore(&i8259_lock, flags);
}

/* Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    
    if (irq_num < 8){
        inb(0x21);
        master_mask = master_mask & ~(1 << irq_num);
        outb(master_mask, 0x21);
    }
    else{
        inb(0xA1)
        slave_mask = slave_mask & ~(1 << (irq_num - 8));
        outb(slave_mask, 0xA1);
    }
}

/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
    if (irq_num < 8){
        master_mask = master_mask | (1 << irq_num);
        outb(master_mask, 0x21);
    }
    else{
        slave_mask = slave_mask | (1 << (irq_num - 8));
        outb(slave_mask, 0xA1);
    }
}

/* Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
    
}
