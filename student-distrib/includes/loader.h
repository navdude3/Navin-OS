#include "types.h"

int32_t load_program(void *addr, int fd);                   /* Reads bytes from an executable file into this address */
int32_t setup_user_page(void *addr);                        /* Addr is the bounds of user page we want to define */
int32_t check_executable(int fd);                           /* Checks that file is a valid executable */
int32_t setup_pcb(void *addr);
int32_t sys_execute(const uint8_t* command);
int32_t sys_halt(uint8_t status);