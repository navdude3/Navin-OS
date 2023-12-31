#include "loader.h"
#include "paging.h"
#include "process.h"
#include "mp3fs.h"
#include "vfs.h"
#include "terminal.h"

static int num_active_procs;                                                        /* Reads bytes from an executable file into this address */
static int pid_array[6];                                                            /* Keeps track of which processes are open */
int32_t parse_fname_args(const uint8_t* input, uint8_t* fname, uint8_t* args);
int32_t check_exec(dentry_t* file_dentry);
int32_t setup_fd_array(pcb_t* proc);


/* 
 * setup_user_page
 *   DESCRIPTION: sets up 4MB page for user program
 *   INPUTS: pid - which program 
 *   OUTPUTS: None
 *   RETURN VALUE: void
*/
void setup_user_page(int pid){
    pde_desc_t user_page;

    int start_mem_index = PROGRAM_VMEM_BASE >> OFFSET;                              /* Page Directory Bits = 10 MSB */
    
    uint32_t user_addr = (pid * PROGRAM_SIZE) + USER_MEMORY_BASE;

    user_page.pde_p = 1;                                                            /* Mark as preset */
    user_page.pde_rw = 1;                                                           /* Enable Read/Write */
    user_page.pde_us = 1;                                                           /* Enable user mode */
    user_page.pde_pwt = 0;
    user_page.pde_pcd = 0;
    user_page.pde_a = 0;
    user_page.pde_d = 0;
    user_page.pde_ps = 1;                                                             /* Set to 4MB pages */
    user_page.pde_g = 0;
    user_page.pde_avail = 0;
    user_page.pde_pat = 0;
    user_page.pde_reserved = 0;
    user_page.pde_pba = (user_addr >> OFFSET);
    
    set_pdentry(start_mem_index, user_page);
}       
 

/* 
 * sys_execute
 *   DESCRIPTION: Loads program, allocated the space for it, and executes if successful 
 *   INPUTS: const uint8_t* command - command to be executed 
 *   OUTPUTS: 0 if successful, -1 if not
 *   SIDE EFFECT: Executes user program to the screen 
*/
int32_t sys_execute(const uint8_t* command) {
    cli();
    uint8_t fname[MAXSIZE];                                                              /* max filesize name */ 
    uint8_t args[MAX_ARG_SIZE];                                                          // store args here
    int i,j,k;
    uint32_t esp;
    uint32_t ebp;
    int active_processes = 0;
    uint8_t eip_buffer[SIZE_OF_ADDR];                                                   /* Holds entry point to function (Bytes 24-27 of command)*/                                              
    uint32_t entry;
    dentry_t dentry;   
    inode_t* inode;
    uint32_t size;
    uint32_t new_pid;
    int arg_size_count;

    pcb_t* cur_process = get_cur_proc();
    asm volatile(
        "movl %%ebp, %0" : "=r"(ebp) 
    );                                                                                   /* Immediately get ebp, get esp just before IRET */
    

    if(command == NULL || command[0] == '\0') {                                          /* File or command non-existent */
        sti();
        return -1;
    }

   for(i = 0; i < MAX_PROCESS; i++){                                                     /* check which pids are open */ 
        if(pid_array[i] == 0){                                                           /* If not in use, assign new process to this pid*/
            new_pid = i;
            active_processes++;                                                          /* Increment nyumber of active processes */
            break;
        }
        if(pid_array[i] == 1){
            active_processes++;
        }
        if(active_processes == MAX_PROCESS){                                              /* Every PID is used */
            sti();
            return MAX_NUM;
        }
   }

   for(i = 0; i < SIZE_OF_ADDR; i++) eip_buffer[i] = 0;                                   /* Clearing fname and eip_buffer before populating */
   for(j = 0; j < MAXSIZE; j++) fname[j] = NULL;
   for(k = 0; k < MAX_ARG_SIZE; k++) args[k] = NULL;
   
   
   num_active_procs = active_processes;                                                  


    /* 1. Parse args and name */
    arg_size_count = parse_fname_args(command, fname, args);

    /* 2. Executable check */
    if(read_dentry_by_name(fname, &dentry) != 0) {
        sti();
        return -1;                               /* If failed to read, return -1 */
    }
    
    if(0 != check_exec(&dentry)){
        sti();
        return -1;
    }

    /* 3. Set up program paging and flushes TLB */
    setup_user_page(new_pid);               
                   
    /* 4. User Level Program Loader */                                                     
    /* At this point, we have verified that the file exists and is a valid executable. Can now copy the program into address */
    read_data(dentry.inode_idx, 24, eip_buffer, SIZE_OF_ADDR);                              /* Read the four bytes from 24-27 that contain virtual address of first instruction to be executed */ 
    entry = *((uint32_t*)eip_buffer);                                                       /* Return entry point at bit 24 */
	

    inode = &init_inode[dentry.inode_idx];
    size = inode->file_size;
    read_data(dentry.inode_idx, 0, (uint8_t*)PROGRAM_VMEM_START, size);                     /* Copying entire file to memory */

    /* 5. Create PCB */
    pcb_t* new_process = (pcb_t *) (USER_MEMORY_BASE - ((new_pid + 1) * PCB_SIZE));         //+1 because pcb resides on top of 8kb block


    new_process->pid = new_pid;

    if(cur_process == NULL || new_pid < 3){         /* Base program */
        new_process->parent_pid = -1;              
        new_process->term_id = new_pid;
    }                                         
    else{ /* Setting parent process info */
        new_process->parent_pid = process_array[cur_term_id];    
        new_process->term_id = cur_term_id;
    }                                     
    setup_fd_array(new_process);

    /* 6. Create it’s own context switch stack */
    tss.ss0 = KERNEL_DS;
    tss.esp0 = USER_MEMORY_BASE - (KERNEL_AREA_SIZE * new_pid);                     /* Points to bottom of process' block */

    /* Save current ESP and EBP before context switch*/
    asm volatile(
        "movl %%esp, %0" : "=r"(esp) 
    );   

    if(new_process->parent_pid != -1){
        cur_process->saved_ebp = ebp;                                                       /* Saving EBP/ESP */
        cur_process->saved_esp = esp;
    }
    
    for(i = 0; i < MAX_ARG_SIZE; i++) new_process->args[i] = args[i];
    new_process->arg_size = arg_size_count;
    
    set_cur_proc(new_process);
    pid_array[new_pid] = 1;
    process_array[(uint32_t)new_process->term_id] = new_pid;           

    sti();
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

/* 
 * sys_halt
 *   DESCRIPTION: Halts current program. If program is base (shell), it reboots after halting
 *   INPUTS: const uint8_t* status - status of program that finished execution 
 *   OUTPUTS: 0 if successful, -1 if not
 *   SIDE EFFECT: Halts program
*/
int32_t sys_halt(uint8_t status) {
    cli();
    pcb_t* cur_process = get_cur_proc();
    if(cur_process == NULL) {return -1;}

    uint32_t esp, ebp;
    uint32_t statusval = (uint32_t) status;

    /* Close any open FDs */ 
    int i;
    for(i = 0; i < 8; i++) 
        if(cur_process->fd_array[i].flags.present == 1) sys_close(i);
    
    
    /* If parent pid = -1, this is the base program */
    if(cur_process->parent_pid == -1) {
        pid_array[cur_process->pid] = 0;                        /* Sets pid to 0 */
        cur_process = NULL;
        sti();
	    sys_execute((uint8_t*) "shell");                        /* Reboots shell */
        return 0;
    }

    /* Teardown vidmap entry */
    pte_desc_t vidmap;
    vidmap.val = 0x0;
    set_ptentry(usr_vidmap_table_desc.addr, cur_process->pid, vidmap);
    
    /* Getting parent process info */
    pid_array[cur_process->pid] = 0;
    process_array[(uint32_t) cur_process->term_id] = cur_process->parent_pid;
    pcb_t* parent_process = get_pcb(cur_process->parent_pid);

    /* Restore Parent Data */
    esp = parent_process->saved_esp;
    ebp = parent_process->saved_ebp;
    tss.esp0 = esp;

    /* Restore Paging */
    setup_user_page(parent_process->pid);
    set_cur_proc(parent_process);
    
    sti();

    /* Jump to Execute Return */
    asm volatile(
            "mov %0, %%esp;           \n\
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

/* 
 * parse_fname_args
 *   DESCRIPTION: Parses command for names and argument 
 *   INPUTS: const uint8_t* input, uint8_t* fname, uint8_t* args
 *   OUTPUTS: None
 *   RETURNS: Returns length of arguments 
*/
int32_t parse_fname_args(const uint8_t* input, uint8_t* fname, uint8_t* args){
    int i,j;
    int flagger = 0;

    /* Parsing for command */
    for(i = 0; i < MAXSIZE; i++){
        if(input[i] == '\n') return 0;
        if(input[i] == ' '){
            fname[i] = '\0';                // terminate fname string
            ++i;
            break;
        } 
        fname[i] = input[i];                /* Copy over name */
        if(input[i] == '\0'){
            args = 0;
            return -1;
        } 

    }
    int leading_ws_flag = 0;
    int arg_idx = 0;

    /* Parsing for arguments */
    for(j = 0; j+i < MAX_ARG_SIZE; ++j){    /* Checks rest of string (after command) for arguments*/
        if(leading_ws_flag == 0 && input[j+i] == ' ') continue;
        else{
            if(input[j+i] == '\n' 
            || input[j+i] == ' ' 
            || input[j+i] == '\0'){
                args[arg_idx] = '\0';
                break;
            }
            args[arg_idx] = input[j + i];
            leading_ws_flag = 1;
            ++arg_idx;
        }
        
    }

    /* Iterating through string to check for null terminator or space */
    for(i = 0; i < MAX_ARG_SIZE; i++){
        if(args[i] == (uint32_t)' ' || args[i] == (uint32_t)'\0' || args[i] == (uint32_t)""){
            continue;
        }
        else{
            flagger = 1;
            break;
        }
    }
    /* End of strinig reached with no arguments */
    if(i == MAX_ARG_SIZE && flagger == 0){
        args = 0;
        return -1;
    }
    

    return arg_idx;
}

/* 
 * check_exec
 *   DESCRIPTION: checks if the file is executable or not
 *   INPUTS: dentry_t* file_dentry
 *   OUTPUTS: checker for if the file can be executed
 *   RETURNS: Returns comparison between exe_check_buf and exe_check_str to see if executable
*/
int32_t check_exec(dentry_t* file_dentry){
    uint8_t exe_check_buf[EXEC_IDENT_SIZE];                                             /* Holds first four bytes of command to check if executable */
    int8_t* exe_check_str = "\x7f""ELF";

    read_data(file_dentry->inode_idx, 0, exe_check_buf, 4);                                     /* Writes first four bytes of data to buf */                  
    return strncmp((int8_t *)exe_check_buf, exe_check_str, 4);
}

/* 
 * setup_fd_array
 *   DESCRIPTION: sets up the file descriptor fur the processwe are at 
 *   INPUTS: pcb_t* proc
 *   OUTPUTS: fd_entry is set for the current PCB
 *   RETURNS: Returns 0 
*/
int32_t setup_fd_array(pcb_t* proc){
    int i;

    for (i = 0; i < 2; ++i){
        fd_entry_t* stdout_fd = &proc->fd_array[i];         // i = 0 initializes stdin , i = 1 initializes stdout
        stdout_fd->file_position = 0;
        stdout_fd->flags.present = 1;
        stdout_fd->j_tbl = &terminal_ops;                   // both stdin and stdout use terminal driver
        stdout_fd->term_id = proc->term_id;
        stdout_fd->j_tbl->open(stdout_fd);                  // initialize terminal driver
    }
    return 0;
}
