//#ifndef PROCESS_H
#define PROCESS_H

#define KERNEL_AREA_SIZE 0x2000
#define PROC_FD_ARRAY_SIZE 8
#define MAX_PROCESS        3
#include "vfs.h"

typedef struct{
    fd_entry_t fd_array [PROC_FD_ARRAY_SIZE];
    int32_t pid;
    int32_t parent_pid;
    uint32_t saved_esp;
    uint32_t saved_ebp;
    char args[128];
    uint32_t arg_size;
} pcb_t;

pcb_t* cur_process;                     /* Current active process */             
extern pcb_t* get_curr_pcb();           /* Returns pointer to current PCB */
extern pcb_t* get_pcb(uint32_t pid);    /* Returns pointer for specified PCB */
