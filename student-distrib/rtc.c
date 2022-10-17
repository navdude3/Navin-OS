#include "rtc.h"
// DONE

/* info from wiki.osdev.org/RTC */
/* 
 * rtc_init
 *  DESCRIPTION: Turns on IRQ 8 with default 1024 Hz Rate
 *  INPUTS: none
 *  OUTPUTS: none
 *  RETURN VALUE: none
 *  SIDE EFFECTS: Enables interrupts on IRQ 8
 */
void rtc_init(){
    disable_irq(RTC_IRQ);                            // Disable interrupts on IRQ 8
    outb(REG_B_DATA, REG_PORT);                      // Disable NMI 
    char prev = inb(RW_PORT);                        // Read the current val of register B
    outb(REG_B_DATA, REG_PORT);                      // Reset index
    outb(prev | 0x40, RW_PORT);                      // Turn on bit 6 of reg B by OR-ing previous with 0x01000000   
    enable_irq(RTC_IRQ);                             // Enable interrupts on IRQ 8
}

