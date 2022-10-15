#include "rtc.h"

// turns on IRQ with default 1024 Hz rate
// info from wiki.osdev.org/RTC

void rtc_init() {
    disable_irq(7);                             // disable interrupts
    // outb(data, port)
    outb(REG_B_DATA, REG_PORT);                // select reg B and disable NMI
    char prev = inb(RW_PORT);                  // read the current val of register B
    outb(REG_B_DATA, REG_PORT);                // reset index
    outb(prev | 0x40, RW_PORT);                // turn on bit 6 of reg B    
    enable_irq(7);                             // enable interrupts on IRQ 8
}


