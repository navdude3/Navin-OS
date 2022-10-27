#include "types.h"

extern int32_t load_program(void *addr, int fd);                   /* Reads bytes from an executable file into this address */
extern int32_t setup_user_page(void *addr);                        /* Addr is the bounds of user page we want to define */
extern int32_t check_executable(int fd);                           /* Checks that file is a valid executable */
extern int32_t setup_pcb(void *addr);
extern int32_t sys_execute(const uint8_t* command);
extern int32_t sys_halt(uint8_t status);
extern int32_t read (int32_t fd, void* buf, int32_t nbytes);
extern int32_t write (int32_t fd, void* buf, int32_t nbytes);
extern int32_t close(int32_t fd);
extern int32_t open (const uint8_t* filename);