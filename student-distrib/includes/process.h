#define KERNEL_STACK_SIZE 8192
#define PROC_FD_ARRAY_SIZE 8
#include "vfs.h"

typedef struct __attribute__((packed)) proc_ctrl_blk{
        fd_entry_t fd_array [PROC_FD_ARRAY_SIZE];
} proc_ctrl_blk_t;

typedef struct proc_kernel_blk{
    union 
    {
        char reserved[KERNEL_STACK_SIZE];
        struct{
            proc_ctrl_blk_t ctrl_blk;
            char stack[KERNEL_STACK_SIZE - sizeof(proc_ctrl_blk_t)];
        }__attribute__((packed));
    };
} proc_kernel_blk_t;