#ifndef PROCESS_H
#define PROCESS_H

#define KERNEL_AREA_SIZE 0x2000
#define PROC_FD_ARRAY_SIZE 8
#include "vfs.h"



typedef struct __attribute__((packed)) proc_ctrl_blk{
        struct proc_ctrl_blk* parent_ctrl_blk;
        fd_entry_t* proc_fd_array [PROC_FD_ARRAY_SIZE]; // array of pointers pointing to fd's in global array (see vfs.h)
} proc_ctrl_blk_t;

static proc_ctrl_blk_t* active_proc;

typedef struct proc_kernel_blk{
    union 
    {
        char reserved[KERNEL_AREA_SIZE];
        struct{
            proc_ctrl_blk_t ctrl_blk;
            char stack[KERNEL_AREA_SIZE - sizeof(proc_ctrl_blk_t)];
        }__attribute__((packed));
    };
} proc_kernel_blk_t;

int init_proc_fd_array(proc_ctrl_blk_t* ctrl_blk);

#endif