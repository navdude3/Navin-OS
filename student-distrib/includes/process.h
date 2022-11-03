//#ifndef PROCESS_H
#define PROCESS_H

#define KERNEL_AREA_SIZE 0x2000
#define PROC_FD_ARRAY_SIZE 8
#define MAX_PROCESS        6
#include "vfs.h"



// typedef struct __attribute__((packed)) proc_ctrl_blk{
//         struct proc_ctrl_blk* parent_ctrl_blk;
//         /* array of indexes (indices?) to fd's in global fd array (see vfs.h) 
//         * For example, if proc_fd_map[5] = 20, the actual file descriptor is in fd_array[20]
//         */ 
//         int proc_fd_map [PROC_FD_ARRAY_SIZE]; 
// } proc_ctrl_blk_t;

// static proc_ctrl_blk_t* active_proc;

// typedef struct proc_kernel_blk{
//     union 
//     {
//         char reserved[KERNEL_AREA_SIZE];
//         struct{
//             proc_ctrl_blk_t ctrl_blk;
//             char stack[KERNEL_AREA_SIZE - sizeof(proc_ctrl_blk_t)];
//         }__attribute__((packed));
//     };
// } proc_kernel_blk_t;


static int pid_array[MAX_PROCESS]; //account for stdin and stdout

typedef struct{
    int proc_fd_map[PROC_FD_ARRAY_SIZE];
    uint32_t pid;
    uint32_t parent_pid;
    uint32_t fd;
    uint32_t saved_esp;
    uint32_t saved_ebp;
    uint32_t active;
} pcb_t;
