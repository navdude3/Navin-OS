#include "lib.h"

#define BUFFER 128

extern int32_t terminal_write(uint32_t fd, void* user_buffer, int32_t bytes);
extern int32_t terminal_read(uint32_t fd, void* user_buffer, int32_t bytes);
extern int32_t terminal_close(uint32_t fd);
extern int32_t terminal_open(const uint8_t* fname);

extern void fill_buffer(char input_char);

