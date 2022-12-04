#include "keyboard.h"

/* array that holds the character corresponding to each scancode */
static char let_num [OPTION_SIZE] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '-', '=', '[', ']', ';', '`', ',', '.', '/', ' ', '\\', '\'', '\t', '\b', '\n'};
static char shift_let_num [OPTION_SIZE] = {')', '!', '@', '#', '$', '%', '^', '&', '*', '(', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_', '+', '{', '}', ':', '~', '<', '>', '/', ' ', '|', '"', '\t', '\b','\n'};
static char cap_let_num [OPTION_SIZE] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '-', '=', '[', ']', ';', '`', ',', '.', '/', ' ', '\\', '\'', '\t', '\b', '\n'};

/* scancode info from https://wiki.osdev.org/PS2_Keyboard && https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html */
/* array that holds the keyboard scancodes for 0-9 and a-z */
static char scancode_pressed[OPTION_SIZE] = {0x0B, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x1E, 0x30, 0x2E, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 0x26, 0x32, 0x31, 0x18, 0x19, 0x10, 0x13, 0x1F, 0x14, 0x16, 0x2F, 0x11, 0x2D, 0x15, 0x2C, 0x0C, 0x0D, 0x1A, 0x1B, 0x27, 0x29, 0x33, 0x34, 0x35, 0x39, 0x2B, 0x28, 0x0F, 0x0E, 0x1C};

/* Static varibles to keep track of shifting, caps lock, and ctrl buttons */
static int lshift = 0;
static int rshift = 0;
static int caps_en = 0;
static int shift_en = 0;
static int capital_letters = 0;
static int caps_count = 0;
static int ctrl = 0;

/* 
 * keyboard_init 
 *   DESCRIPTION: Enables interrupts on IRQ 1 for the keyboard 
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
*/
void keyboard_init() {
    enable_irq(1);              // enables respective irq for the keyboard
}

/* 
 * shift_check
 *   DESCRIPTION: checker to see if shift should be enabled
 *   INPUTS: key
 *   OUTPUTS: lshift, rshift, ctrl, or caps set to 1
 *   RETURN VALUE: void
*/
void shift_check(int key) {
    if(key == LSHIFT_PRESS){
        lshift = 1;                                         // update lshift if left shift is pressed
    }
    if(key == RSHIFT_PRESS){
        rshift = 1;                                         // update rshift if right shift is pressed
    }
    if(key == LSHIFT_RELEASE){
        lshift = 0;                                         // update lshift if left shift is released
    }
    if(key == RSHIFT_RELEASE){
        rshift = 0;                                         // update rshift if right shift is released
    }
    if(key == CAPS_PRESSED){
        if(++caps_count == 1) caps_en = 1;                  // update caps_en if is pressed
    }
    if(key == CAPS_RELEASED){
        if(caps_count == 2)  {
            caps_en = 0;
            caps_count = 0;                                 // if caps pressed twice, reset caps_en and cap_count
        }
    }
    if(key == LCTRL_PRESS || key == RCTRL_PRESS){
        ctrl = 1;                                           // update ctrl if either of them pressed
    }
    if(key == LCTRL_RELEASE || key == RCTRL_RELEASE) {
        ctrl = 0;                                           // update ctrl if either of them released
    }
}


/* 
 * terminal_check
 *   DESCRIPTION: checker to see which terminal should be switched
 *   INPUTS: key
 *   OUTPUTS: none
 *   RETURN VALUE: terminal to switch to 0 indexed or -1 if not switching
*/

int terminal_check(int key){    
        if(key == F1_PRESS){
            return 0;
        }
        else if(key == F2_PRESS){
            return 1;
        }
        else if(key == F3_PRESS){
            return 2;
        }
        
    return -1;    
}




/* 
 * keyboard_link_handler
 *   DESCRIPTION: print the value to screen based on the scancode, send an EOI to master pic irq 1 
 *   INPUTS: none
 *   OUTPUTS: print keyboard character to screen
 *   RETURN VALUE: void
*/
void keyboard_link_handler(){
    int key = inb(0x60);                                                                                           // 0x60 is the port for keyboard
    int i;
    char output = -1;  
    int terminal_switch;

    shift_check(key);                       
    shift_en = lshift | rshift;                                                                                     /* Shift enabled if either shift key is pressed */
    capital_letters = shift_en ^ caps_en;                                                                           /* XORed to negate */
    terminal_switch = terminal_check(key);
    if(terminal_switch != -1){
        send_eoi(1); 
        switch_terms(terminal_switch);                                                                              /* Switch terminals */
        return;
    }
    for(i = 0; i < OPTION_SIZE; i++) {
        if(key == scancode_pressed[i]) {                                                                           // check if the key is in scancode_pressed
            if(capital_letters == 0) output = let_num[i]; 
            else if(caps_en == 1) output = cap_let_num[i];
            else output = shift_let_num[i];                                                                        // check if the scancode should be from capital, lowercase or shifted letters 
            if (caps_en == 1 && shift_en == 1 && ((i < SHIFT_MIN)|| (i>=SHIFT_MAX))) output = shift_let_num[i];
            if(ctrl == 1 && key ==  L){                                                                           //control l checking, send specific escape character if so
                output = '\f';
            }
        }
    }
    send_eoi(1);                                                                                                   //send eoi signal
    if(output != -1){                                                                                              //if not invalid 
        if(fill_buffer(output) == -1){
            return; // could not write to buffer, so do not print to screen
        };
        
        putc(output); 
    }                                 
}
