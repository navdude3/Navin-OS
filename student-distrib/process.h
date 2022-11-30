#ifndef PROCESS_H
#define PROCESS_H

#define KERNEL_AREA_SIZE 0x2000
#define PROC_FD_ARRAY_SIZE 8
#define MAX_PROCESS        6
#include "vfs.h"
typedef struct __attribute__((packed)) pt_regs{
    
    uint32_t edi;
    uint32_t esi;
    uint32_t kebp;
    uint32_t kesp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
} pt_regs_t;

typedef struct __attribute__((packed)) pt_regs_fl{
    uint32_t fl;
    pt_regs_t regs;
} pt_regs_fl_t;

typedef struct __attribute__((packed)) pt_regs_int{
    
    pt_regs_t regs;
    uint32_t ueip;
    uint32_t cs;
    uint32_t fl;
    uint32_t uesp;      // MIGHT BE GARBAGE
    uint32_t ss;        // might also be garbage!
    
}pt_regs_int_t;


typedef struct{
    fd_entry_t fd_array [PROC_FD_ARRAY_SIZE];
    int32_t pid;
    int32_t parent_pid;
    uint32_t saved_esp;
    uint32_t saved_ebp;
    pt_regs_int_t saved_regs;
    char args[128];
    uint32_t arg_size;
    int8_t term_id;
} pcb_t;

pcb_t* cur_process;                     /* Current active process */             
extern pcb_t* get_curr_pcb();           /* Returns pointer to current PCB */
extern pcb_t* get_pcb(int32_t pid);    /* Returns pointer for specified PCB */

#endif