#include "keyboard.h"

/* array that holds the character corresponding to each scancode */
static char let_num [OPTION_SIZE] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 
'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '-', '=', '[', ']', ':', '`', ',', '.', '/', ' ', '\\', '\''};

/* scancode info from https://wiki.osdev.org/PS2_Keyboard && https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html */
/* array that holds the keyboard scancodes for 0-9 and a-z */
static char scancode_pressed[OPTION_SIZE] = {0x0B, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x1E, 0x30, 0x2E, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 
0x26, 0x32, 0x31, 0x18, 0x19, 0x10, 0x13, 0x1F, 0x14, 0x16, 0x2F, 0x11, 0x2D, 0x15, 0x2C, 0x0C, 0x0D, 0x1A, 0x1B, 0x27, 0x29, 0x33, 0x34, 0x35, 0x39, 0x2B, 0x28};

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


/* 
 * keyboard_link_handler
 *   DESCRIPTION: print the value to screen based on the scancode, send an EOI to master pic irq 1 
 *   INPUTS: none
 *   OUTPUTS: print keyboard character to screen
 *   RETURN VALUE: void
*/
void keyboard_link_handler(){
    int key = inb(0x60);                            //port for keyboard
    int i;
    char output = 64;                               //initial value to check if lowercase or number
    for(i = 0; i < OPTION_SIZE; i++) {
        if(key == scancode_pressed[i]) {
            output = let_num[i];
        }
    }
    send_eoi(1);                                    //send EOI to primary pic 1
    if(output != 64) putc(output);                  //if the value was detected as a valid key print it
    
    else return;
}
