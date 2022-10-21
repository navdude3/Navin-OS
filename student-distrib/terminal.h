#include "lib.h"

#define BUFFER 128

// extern void terminal_open();
// extern void terminal_close();
extern void terminal_write(void* user_buffer);
extern void terminal_read(void* user_buffer);
extern void fill_buffer(char input_char);
