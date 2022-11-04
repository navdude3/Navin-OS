#include "types.h"
#include "mp3fs.h"
#include "syscall_functions.h"

#define USER_MEMORY_BASE    0x800000
#define KERNEL_AREA_BASE    (USER_MEMORY_BASE) // grows upwards
#define PROGRAM_VMEM_BASE   0x8000000
#define PROGRAM_VMEM_STACK  PROGRAM_VMEM_BASE + 0x400000
#define PCB_SIZE            0x2000
#define PROGRAM_VMEM_START  PROGRAM_VMEM_BASE+ 0x48000



extern void setup_user_page(int pid);                        /* Addr is the bounds of user page we want to define */
extern int32_t setup_pcb(void *addr);
extern int32_t sys_execute(const uint8_t* command);
extern int32_t sys_halt(uint8_t status);


