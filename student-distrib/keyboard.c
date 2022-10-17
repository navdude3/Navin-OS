#include "keyboard.h"

// DONE
/* 
 * keyboard_init
 *   DESCRIPTION: Enables interrupts on IRQ 1 for the keyboard 
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
*/
void keyboard_init() {
    enable_irq(1);
}
