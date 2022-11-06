#include "lib.h"
#include "types.h"

/* Function headers for all system calls */
extern int32_t sys_halt(uint8_t status);
extern int32_t sys_execute(const uint8_t* command);
extern int32_t sys_read(uint32_t fd, uint8_t* buf, uint32_t length);
extern int32_t sys_write(uint32_t fd, uint8_t* buf, uint32_t length);
extern int32_t sys_open(const uint8_t* filename);
extern int32_t sys_close(uint32_t fd);
extern int32_t sys_getargs(uint8_t* buf, uint32_t nbytes);
extern int32_t sys_vidmap(uint8_t** screen_start);
extern int32_t sys_set_handler(uint32_t signum, void* handler_address);
extern int32_t sys_sigreturn(void);
