#include "types.h"
#include "mp3fs.h"
#include "syscall_functions.h"
#include "x86_desc.h"

#define USER_MEMORY_BASE    0x800000
#define KERNEL_AREA_BASE    (USER_MEMORY_BASE)               /* Stack grows upward */
#define PROGRAM_SIZE        0x400000
#define PROGRAM_VMEM_BASE   0x08000000                       /* Start of virtual memory for program*/
#define PROGRAM_VMEM_STACK  PROGRAM_VMEM_BASE + PROGRAM_SIZE     /* Start of program stack */
#define PCB_SIZE            0x2000                           /* 8KB space must be allocated for each PCB*/
#define PROGRAM_VMEM_START  PROGRAM_VMEM_BASE+ 0x48000       /* Start of program data in memory */
#define MAXSIZE             32                               /* Max file size */
#define EXEC_IDENT_SIZE     4
#define SIZE_OF_ADDR        4


extern void setup_user_page(int pid);                       
extern int32_t setup_pcb(void *addr);                        /* Addr is the bounds of user page we want to define */
extern int32_t sys_execute(const uint8_t* command);          /* Loads and executes a new program */
extern int32_t sys_halt(uint8_t status);                     /* Halts the current program */
