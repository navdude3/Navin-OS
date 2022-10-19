#include "keyboard.h"

/* array that holds the character corresponding to each scancode */
static char let_num [OPTION_SIZE] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '-', '=', '[', ']', ';', '`', ',', '.', '/', ' ', '\\', '\'', '\t', '\b', '\e'};
static char shift_let_num [OPTION_SIZE] = {')', '!', '@', '#', '$', '%', '^', '&', '*', '(', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_', '+', '{', '}', ':', '~', '<', '>', '/', ' ', '|', '"', '\t', '\b','\e'};
static char cap_let_num [OPTION_SIZE] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '-', '=', '[', ']', ';', '`', ',', '.', '/', ' ', '\\', '\'', '\t', '\b', '\e'};

/* scancode info from https://wiki.osdev.org/PS2_Keyboard && https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html */
/* array that holds the keyboard scancodes for 0-9 and a-z */
static char scancode_pressed[OPTION_SIZE] = {0x0B, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x1E, 0x30, 0x2E, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 0x26, 0x32, 0x31, 0x18, 0x19, 0x10, 0x13, 0x1F, 0x14, 0x16, 0x2F, 0x11, 0x2D, 0x15, 0x2C, 0x0C, 0x0D, 0x1A, 0x1B, 0x27, 0x29, 0x33, 0x34, 0x35, 0x39, 0x2B, 0x28, 0x0F, 0x0E, 0x1C};

static int lshift = 0;
static int rshift = 0;
static int caps_en = 0;
static int shift_en = 0;
static int capital_letters = 0;
static int caps_count = 0;
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


void shift_check(int key) {
    if(key == LSHIFT_PRESS){
        lshift = 1;
    }
    if(key == RSHIFT_PRESS){
        rshift = 1;
    }
    if(key == LSHIFT_RELEASE){
        lshift = 0;
    }
    if(key == RSHIFT_RELEASE){
        rshift = 0;
    }
    if(key == CAPS_PRESSED){
        if(++caps_count == 1) caps_en = 1; 
    }
    if(key == CAPS_RELEASED){
        if(caps_count == 2)  {
            caps_en = 0;
            caps_count = 0;
        }
    }
}


void keyboard_link_handler(){
    int key = inb(0x60);                            // port for keyboard
    int i;
    char output = -1;  
    shift_check(key);                       
    shift_en = lshift | rshift; 
    capital_letters = shift_en ^ caps_en;           // xor so they negate
   
    for(i = 0; i < OPTION_SIZE; i++) {
        if(key == scancode_pressed[i]) {
            if(capital_letters == 0) output = let_num[i];
            else if(caps_en == 1) output = cap_let_num[i];
            else output = shift_let_num[i];
            if (caps_en == 1 && shift_en == 1 && ((i <= 10)|| (i>=36))) output = shift_let_num[i];
        }
    }
    send_eoi(1);      
    if(output != -1) putc(output);                                 
}

