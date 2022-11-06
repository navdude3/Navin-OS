#include "loader.h"
#include "paging.h"
#include "process.h"
#include "mp3fs.h"
#include "vfs.h"

static int num_active_procs;                                                        /* Reads bytes from an executable file into this address */
static int pid_array[6];
int32_t parse_fname_args(const uint8_t* input, uint8_t* fname, uint8_t* args);

void setup_user_page(int pid){
    pde_desc_t user_page;

    int start_mem_index = PROGRAM_VMEM_BASE >> 22;                                  /* Page Directory Bits = 10 MSB */
    

    uint32_t user_addr = (pid * 0x400000) + USER_MEMORY_BASE;

    user_page.pde_p = 1;                                                            /* Mark as preset */
    user_page.pde_rw = 1;                                                           /* Enable Read/Write */
    user_page.pde_us = 1;                                                           /* Enable user mode */
    user_page.pde_pwt = 0;
    user_page.pde_pcd = 0;
    user_page.pde_a = 0;
    user_page.pde_d = 0;
    user_page.pde_ps = 1;                                                           /* Set to 4MB pages */
    user_page.pde_g = 0;
    user_page.pde_avail = 0;
    user_page.pde_pat = 0;
    user_page.pde_reserved = 0;
    user_page.pde_pba = (user_addr >> 22);
    
    set_pdentry(start_mem_index, user_page);
}       
 

int32_t sys_execute(const uint8_t* command) {
    uint8_t fname[32];                                                              // max filesize name
    uint8_t args[32];                                                               // store args here
    int i,j;
    uint32_t esp;
    uint32_t ebp;
    int active_processes = 0;
    uint8_t eip_buffer[4];
    uint32_t entry;
    int8_t exe_check_buf[4];
    int8_t* exe_check_str = "\x7f""ELF";
    dentry_t dentry;   
    inode_t* inode;
    uint32_t size;
    uint32_t new_pid;


    asm volatile(
        "movl %%ebp, %0" : "=r"(ebp) 
    );                                                                                            //immediately get ebp, get esp just before IRET 
    

    if(command == NULL) return -1;
    if(command[0] == '\0') return -1;                                                              // file non existent


   for(i = 0; i < MAX_PROCESS; i++){                                                               // check which pids are open 
        if(pid_array[i] == 0){
            new_pid = i;
            pid_array[i] = 1;
            active_processes++;
            break;
        }
        if(pid_array[i] == 1){
            active_processes++;
        }
        if(i == 5 && pid_array[5] == 1){                                                            //every pid is used
            return -1;
        }
   }

   for(i = 0; i < 4; i++) eip_buffer[i] = 0;
   for(j = 0; j < 32; j++) fname[j] = NULL;
   
   num_active_procs = active_processes;                                                             //always check to see how many active processes we have


    /* 1. Parse args and name */
    parse_fname_args(command, fname, args);
    

    /* 2. Executable check */
    read_dentry_by_name(fname, &dentry);
    read_data(dentry.inode_idx, 0, exe_check_buf, 4);                                                          /* Writes first four bytes of data to buf */                                                  
    if(strncmp(exe_check_buf, exe_check_str, 4) != 0){
        pid_array[new_pid] = 0;
        return -1; 
    }


    /* 3. Set up program paging */
    setup_user_page(new_pid);                  /* Sets up page and flushes TLB */


    //this might be needed for some more null checking later on?                                        
    // if(read_dentry_by_name(fd, &entry) == -1) return -1;  
                   
    
    /* 4. User Level Program Loader */                                                     
    /* At this point, we have verified that the file exists and is a valid executable. Can now copy the program into address */
    read_data(dentry.inode_idx, 24, eip_buffer, 4);                               /* Read the four bytes from 24-27 that contain virtual address of first instruction to be executed */ 
    entry = *((uint32_t*)eip_buffer);                                             /* Return entry point at bit 24 */
	

    inode = &init_inode[dentry.inode_idx];
    size = inode->file_size;
    read_data(dentry.inode_idx, 0, (uint8_t*)PROGRAM_VMEM_START, size);                     /* Copying entire file to memory */

    /* 5. Create PCB */
    pcb_t* new_process = (pcb_t *) (USER_MEMORY_BASE - ((new_pid + 1) * PCB_SIZE));        //+1 because curr_pid goes from 0 to 5


    new_process->pid = new_pid;

    if(new_pid == 0){
        new_process->parent_pid = -1;
    }else{
        new_process->parent_pid = cur_process->pid;
    }
    init_fd_array(new_process->fd_array);

    /* 6. Create it’s own context switch stack */

    tss.ss0 = KERNEL_DS;
    tss.esp0 = USER_MEMORY_BASE - (KERNEL_AREA_SIZE * new_pid);
    //tss.esp = PROGRAM_VMEM_STACK;

    /* Save current ESP and EBP before context switch*/
    asm volatile(
        "movl %%esp, %0" : "=r"(esp) 
    );   

    if(cur_process){
        cur_process->saved_ebp = ebp;
        cur_process->saved_esp = esp;
    }
    
   
    cur_process = new_process;

    /* Context Switch */

    asm volatile(
        "pushl %0                           \n"
        "pushl %1                           \n"

        "pushfl                             \n"
        "orl $0x200, (%%esp)                \n"
        
        "pushl %2                           \n"
        "pushl %3                           \n"
        "iret                               \n"
        : 
        : "r"(USER_DS), "r"(PROGRAM_VMEM_STACK), "r" (USER_CS), "r" (entry)
        );
    asm volatile ("BACK_TO_EXECUTE:");

    return 0;
}


int32_t sys_halt(uint8_t status) {
    if(cur_process == NULL) {return -1;}

   /* Restore Parent Data */
    uint32_t statusval = (uint32_t) status;
    

    /* Close any relevant FDs */ 
    int i;
    for(i = 0; i < 8; i++){
        if(cur_process->fd_array[i].flags.present == 1) sys_close(i);
    }
    
    if(cur_process->parent_pid == -1) {
        pid_array[cur_process->pid] = 0;
        cur_process = NULL;
	    sys_execute((uint8_t*) "shell");
        return 0;
    }


    pid_array[cur_process->pid] = 0;
    pcb_t* parent_process = get_pcb(cur_process->parent_pid);

    uint32_t esp, ebp;
    esp = parent_process->saved_esp;
    ebp = parent_process->saved_ebp;

    /* Restore Paging */
    setup_user_page(parent_process->pid);
    cur_process = parent_process;
    

    /* Jump to Execute Return */
    asm volatile("                   \n\  
            mov %0, %%esp;           \n\
            mov %1, %%ebp;           \n\
            mov %2, %%eax;           \n\
            jmp BACK_TO_EXECUTE      \n\
            "
            :
            : "r"(esp), "r" (ebp), "r"(statusval)
            : "eax"
    );
    return 0;
}

int32_t parse_fname_args(const uint8_t* input, uint8_t* fname, uint8_t* args){
    int i,j;
    int fname_indexer, args_indexer;
    int file_flag, args_flag;

    file_flag = 0;
    args_flag = 0;
    for(i = 0; i < 128; i++){
        if(input[i] == "\n") break;
        if(input[i] != ' ' && file_flag == 0){
            for(fname_indexer = i; fname_indexer < 128; fname_indexer++){                             //put file name into fname
                if(input[fname_indexer] == ' ' || input[fname_indexer] == '\0'){
                    file_flag = 1;
                    break;
                }
                fname[fname_indexer] = input[fname_indexer];
            }
        }
    }

    for(i = fname_indexer; i < 128; i++){
        if(input[i] != ' ' && args_flag == 0){
            for(args_indexer = i; args_indexer < 128; args_indexer++){                                     //put arguments into args
                if(input[args_indexer] == ' '|| input[args_indexer] == '\0'){                          //NOT NEEDED UNTIL AFTER 3.3
                    args_flag = 1;
                    break;
                }
                args[args_indexer] = input[args_indexer];
            }
        }
    }
    return 0;
}
