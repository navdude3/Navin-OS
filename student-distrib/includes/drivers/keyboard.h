#include "lib.h"
#include "i8259.h"
#include "terminal.h"
#define OPTION_SIZE         52         // Number of keyboard options
#define CAPS_PRESSED       0x3A        // Caps lock pressed
#define CAPS_RELEASED      0xBA        // Caps lock released
#define LSHIFT_PRESS       0x2A        // Left shift pressed
#define RSHIFT_PRESS       0x36        // Right shift pressed  
#define LSHIFT_RELEASE     0xAA        // Left shift released      
#define RSHIFT_RELEASE     0xB6        // Right shift released 
#define LCTRL_PRESS        0x1D
#define RCTRL_PRESS        0xE0
#define LCTRL_RELEASE      0x9D
#define RCTRL_RELEASE      0x9D //check these values, ps2 might be broken w rctrl?


/* initialization function */
extern void keyboard_init();
/* Keyboard handler function */
extern void keyboard_link_handler();
void shift_check();
