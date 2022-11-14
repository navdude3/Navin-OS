#include "lib.h"
#include "vfs.h"

/* Size of text buffer */
#define BUFFER 128

/* Functions for the terminal driver */
extern int32_t terminal_write(uint32_t fd, uint8_t* user_buffer, uint32_t bytes);
extern int32_t terminal_read(uint32_t fd, uint8_t* user_buffer, uint32_t bytes);
extern int32_t terminal_close(uint32_t fd);
extern int32_t terminal_open(const uint8_t* fname);

extern int32_t fill_buffer(char input_char);

/* FD related declarations*/
extern fd_ops_t terminal_ops;

