#include "loader.h"
#include "paging.h"
#include "process.h"
#include "mp3fs.h"
#include "vfs.h"

static int num_active_procs;                                 /* Reads bytes from an executable file into this address */
static int pid_array[6];

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
    user_page.pde_pba = user_addr;
    
    set_pdentry(start_mem_index, user_page);
}       
 

int32_t sys_execute(const uint8_t* command) {
    uint8_t fname[32];                              // max filesize name
    uint8_t args[32];                               // store args here
    int i;
    int fname_indexer, args_indexer;
    int file_flag, args_flag;
    uint32_t esp;
    uint32_t ebp;
    uint32_t curr_pid;
    int active_processes = 0;
    uint8_t eip_buffer[4];
    uint32_t entry;
    uint8_t exe_check[4];
    dentry_t dentry;   
    inode_t inode;
    uint32_t size;
    uint32_t stack_top;


    // stack_top = (uint32_t) &command;
    // esp = stack_top + 14 * 4; //esp is 14 down on stack * 4 bytes
    // ebp = stack_top + 8 * 4;

    asm volatile(                                    //immediately get esp and ebp
        "movl %%esp, %0" : "=r"(esp)
    );
    asm volatile(
        "movl %%ebp, %0" : "=r"(ebp)
    );
    

    if(command == NULL) return -1;
    if(command[0] == '\0') return -1;                                           // file non existent


   for(i = 0; i < MAX_PROCESS; i++){                                            // check which pids are open 
        if(pid_array[i] == 0){
            curr_pid = i;
            pid_array[i] = 1;
            active_processes++;
            break;
        }
        if(pid_array[i] == 1){
            active_processes++;
        }
        if(i == 5 && pid_array[5] == 1){                                        //every pid is used
            return -1;
        }
   }
   
   num_active_procs = active_processes;                                         //always check to see how many active processes we have


    /* 1. Parse args and name */
    file_flag = 0;
    args_flag = 0;
    for(i = 0; i < 128; i++){
        if(command[i] != ' ' && file_flag == 0){
            for(fname_indexer = i; fname_indexer < 128; fname_indexer++){       //put file name into fname
                if(command[fname_indexer] == ' ' || command[fname_indexer] == '\0'){
                    file_flag = 1;
                    break;
                }
                fname[fname_indexer] = command[fname_indexer];
            }
        }
    }

    for(i = fname_indexer; i < 128; i++){
        if(command[i] != ' ' && args_flag == 0){
            for(args_indexer = i; args_indexer < 128; args_indexer++){          //put arguments into args
                if(command[args_indexer] == ' '|| command[fname_indexer] == '\0'){                               //NOT NEEDED UNTIL AFTER 3.3
                    args_flag = 1;
                    break;
                }
                args[args_indexer] = command[args_indexer];
            }
        }
    }



    /* 2. Executable check */
    read_dentry_by_name(fname, &dentry);
     
    read_data(dentry.inode_idx, 0, exe_check, 4);          /* Writes first four bytes of data to buf */
    //if(*exe_check != 0x464c457f){                         changed because was getting warning                                                    
    if(exe_check[3] != 0x46 || exe_check[2] != 0x4C || exe_check[1] != 0x45 || exe_check[0] != 0x7f){      /* Not an executable */
        pid_array[curr_pid] = 0;
        return -1;        
    }           


    /* 3. Set up program paging */
    setup_user_page(curr_pid);                  /* Sets up page and flushes TLB */


    //this might be needed for some more null checking later on?
    // if(addr == NULL || fd == NULL) return -1;                                          
    // if(read_dentry_by_name(fd, &entry) == -1) return -1;  
    
                                
    
    /* 4. User Level Program Loader */                                     
                                       
    /* At this point, we have verified that the file exists and is a valid executable. Can now copy the program into address */
    read_data(dentry.inode_idx, 24, eip_buffer, 4);                                                                                   /* Return entry point at bit 24 */
    
    for (i = 0; i < 4; i++) {                                         /* Read the four bytes from 24-27 that contain virtual address of first instruction to be executed */      
		entry |= (eip_buffer[i] << (i * 8));                         /* Reversing data so entry contains bytes 27-24*/
	}

    inode = init_inode[dentry.inode_idx];
    size = inode.file_size;
    read_data(dentry.inode_idx, 0, (uint8_t*)PROGRAM_VMEM_START, size);         /* Copying entire file to memory */

    /* 5. Create PCB */
    pcb_t* pcb_startmem = (pcb_t *) USER_MEMORY_BASE - ((curr_pid + 1) * PCB_SIZE);  //+1 because curr_pid goes from 0 to 5

    pcb_startmem->pid = curr_pid;
    pcb_startmem->parent_pid = cur_process->pid;

    pcb_startmem->saved_esp = esp;   //use the esp and epb got from beginning of execute
    pcb_startmem->saved_ebp = ebp;
    init_fd_array(pcb_startmem->fd_array);

    /* 6. Create it’s own context switch stack */



    tss.ss0 = KERNEL_DS;
    tss.esp0 = USER_MEMORY_BASE - (KERNEL_AREA_SIZE * curr_pid);
    tss.esp = PROGRAM_VMEM_STACK;



    /* Context Switch */

    asm volatile ("                                                             \n\
        push %0                                                                 \n\
        push %1                                                                 \n\
        pushfl                                                                  \n\
        push %2                                                                 \n\
        push %3                                                                 \n\
        iret                                                                    \n\
        "
        :   \
        :"r" (USER_DS), "r" (PROGRAM_VMEM_STACK), "r" (USER_CS), "r" (entry) \
    );


    return 0;
}


int32_t sys_halt(uint8_t status) {
    /* Restore Parent Data */
    //load user program with old pid? need PCB


    /* Restore Parent Paging */ //pcb needs to be done for this to retrive parent block]
    //setup_user_page(curr_pid);

    // pcb_t* pcb_parent_pid = cur_process->parent_pid;
    // pcb_t* pcb_current_pid = cur_process->pid;
    /* Close any relevant FDs */
    int i;
    for(i = 0; i<8; i++) {
       // if(pcb_current_pid->fd_array[i].flags == 1)
        close(i);
    }
    // pid_array[pcb_current_pid] = 0;



    // cur_process = (pcb_t *) USER_MEMORY_BASE - ((pcb_parent_pid + 1) * PCB_SIZE);


    // set_pdentry()
    // flush_tlb();

    /* Jump to execute return */
    // asm volatile (
	//     "movl smthn into %esp"
    //     "movl smthn into %ebp"
    //     "jmp to smthn"
	//     : "r" //to old kernal ebp and esp prob
	//     : "%eax"
	// );

	return 0;

}
