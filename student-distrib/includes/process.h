//#ifndef PROCESS_H
#define PROCESS_H

#define KERNEL_AREA_SIZE 0x2000
#define PROC_FD_ARRAY_SIZE 8
#define MAX_PROCESS        6
#include "vfs.h"

typedef struct{
    fd_entry_t fd_array [PROC_FD_ARRAY_SIZE];
    uint32_t pid;
    uint32_t parent_pid;
    uint32_t saved_esp;
    uint32_t saved_ebp;
    // uint32_t active;
} pcb_t;

pcb_t* cur_process_g;              // currently active process
uint32_t curr_pid;
extern pcb_t* get_curr_pcb();
extern pcb_t* get_pcb(uint32_t pid);
