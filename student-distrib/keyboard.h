#include "lib.h"
#include "i8259.h"
#define OPTION_SIZE 36  //number of keyboard options for checkpoint 1


/* initialization function */
extern void keyboard_init();
/* Keyboard handler function */
extern void keyboard_link_handler();