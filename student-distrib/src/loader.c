#include "loader.h"
#include "paging.h"
#include "process.h"
#include "mp3fs.h"

// int32_t load_program() {
//     dentry_t entry;
//     uint32_t buf[4];
//     int32_t start;
//     // if(addr == NULL || fd == NULL) return -1;                                           /* Invalid arguments */
//     // if(read_dentry_by_name(fd, &entry) == -1) return -1;                                /* Entry does not exist in directory */
//     read_data(entry.inode_idx, 0, buf, 4);                                              /* Writes first four bytes of data to buf */
//     if(*buf != 0x464c457f) return -1;                                                   /* Not an executable */
//     /* At this point, we have verified that the file exists and is a valid executable. Can now copy the program into address */
//    //while (0 != read_data(entry.inode_idx, 0, addr, 4)) continue;                       /* Not sure abt length -- intention is to read whole program byte by byte */ 
//     /* How is switch to user done? */
//     read_data(entry.inode_idx, 24, buf, 4);                                         /* Read the four bytes from 24-27 that contain virtual address of first instruction to be executed */
//     start = (*(uint32_t*)buf);                                                       /* Return entry point at bit 24 */
//     return start;                                           
// }


void setup_user_page(int pid){
    pde_desc_t user_page;

    int start_mem_index = PROGRAM_VMEM_BASE >> 22;
    set_pdentry(start_mem_index, user_page);

    uint32_t user_addr = (pid * 0x400000) + USER_MEMORY_BASE;

    user_page.pde_p = 1;
    user_page.pde_rw = 1;
    user_page.pde_us = 1;
    user_page.pde_pwt = 0;
    user_page.pde_pcd = 0;
    user_page.pde_a = 0;
    user_page.pde_d = 0;
    user_page.pde_ps = 1;
    user_page.pde_g = 0;
    user_page.pde_avail = 0;
    user_page.pde_pat = 0;
    user_page.pde_reserved = 0;
    user_page.pde_pba = user_addr;
    
    flush_tlb();
}       



//ravyu code

// int32_t setup_pcb(void *addr) {
//     uint32_t kernel_area_base_addr = (uint32_t) KERNEL_AREA_BASE - ((num_active_procs + 1)* KERNEL_AREA_SIZE);
//     proc_ctrl_blk_t pblk;

//     pblk.parent_ctrl_blk = (proc_ctrl_blk_t*) kernel_area_base_addr + KERNEL_AREA_SIZE; // assumes parent process is right below in stack
//     init_proc_fd_array(&pblk);
//     *(proc_ctrl_blk_t*) kernel_area_base_addr = pblk; // write initialized pblk to top of kernel process area
//     return 0;
// }


// int32_t setup_pcb(void *addr){
    




// }



int32_t sys_execute(const uint8_t* command) {
    uint8_t fname[32];                              // max filesize name
    uint8_t args[32];                               // store args here
    int i;
    int fname_indexer, args_indexer;
    int file_flag, args_flag;
    uint32_t esp, ebp;
    uint32_t curr_pid;
    int active_processes;
    uint8_t eip_buffer[4];
    uint32_t entry;
    uint8_t exe_check[4];
    dentry_t dentry;   
    inode_t inode;
    uint32_t size;

    asm volatile(                                    //immediately get esp and ebp
        "movl %%esp, %0" : "=r"(esp)
    );
    asm volatile(
        "movl %%ebp, %0" : "=r"(esp)
    );

    if(command == NULL) return -1;
    if(command[0] == '\0') return -1;                // file non existent


   for(i = 0; i < MAX_PROCESS; i++){                    //check which pids are open 
        if(pid_array[i] == 0){
            curr_pid = i;
            pid_array[i] = 1;
            active_processes++;
            break;
        }
        if(pid_array[i] == 1){
            active_processes++;
        }
        if(i == 5 && pid_array[5] == 1){           //every pid is used
            return -1;
        }
   }
   num_active_procs = active_processes; //always check to see how many active processes we have

   

    /* 1. Parse args and name */
    file_flag = 0;
    args_flag = 0;
    for(i = 0; i < 128; i++){
        if(command[i] != ' ' && file_flag == 0){
            for(fname_indexer = i; fname_indexer < 128; fname_indexer++){       //put file name into fname
                if(command[fname_indexer] == ' '){
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
                if(command[args_indexer] == ' '){                               //NOT NEEDED UNTIL AFTER 3.3
                    args_flag = 1;
                    break;
                }
                args[args_indexer] = command[args_indexer];
            }
        }
    }



    /* 2. Executable check */
     
    read_data(dentry.inode_idx, 0, exe_check, 4);        /* Writes first four bytes of data to buf */
    //if(*exe_check != 0x464c457f){                         changed because was getting warning                                                    
    if(exe_check[3] != 0x46 || exe_check[2] != 0x4C || exe_check[1] != 0x45 || exe_check[0] != 0x7f){      /* Not an executable */
        pid_array[curr_pid] = 0;
        return -1;        
    }           


    /* 3. Set up program paging */
    setup_user_page(curr_pid); // flushes TLB aswell



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
    read_data(dentry.inode_idx, 0, (uint8_t*)0x08048000, size);         /* Copying entire file to memory */

    /* 5. Create PCB */
    pcb_t* pcb_startmem = (pcb_t *) USER_MEMORY_BASE - ((curr_pid + 1) * PCB_SIZE);  //+1 because curr_pid goes from 0 to 5

    pcb_startmem->pid = curr_pid;
   // uint32_t parent_pid; //grab the terminals pid

    pcb_startmem->saved_esp = esp;   //use the esp and epb got from beginning of execute
    pcb_startmem->saved_ebp = ebp;

    /* 6. Create it’s own context switch stack */
    


    tss.ss0 = KERNEL_DS;
    tss.esp0 = USER_MEMORY_BASE - (KERNEL_AREA_SIZE * curr_pid);



    //ring3 implementation below, need to rewrite so correct args are passed

    //this is basically the context switch, we need to get back to user mode :)



    // mov ax, (4 * 8) | 3 ; ring 3 data with bottom 2 bits set for ring 3
	// mov ds, ax
	// mov es, ax 
	// mov fs, ax 
	// mov gs, ax ; SS is handled by iret
 
	// ; set up the stack frame iret expects
	// mov eax, esp
	// push (4 * 8) | 3 ; data selector
	// push eax ; current esp
	// pushf ; eflags
	// push (3 * 8) | 3 ; code selector (ring 3 code with bottom 2 bits set for ring 3)
	// push test_user_function ; instruction address to return to
	// iret


    return 0;
}


int32_t sys_halt(uint8_t status) {
    /* Restore Parent Data */
    //load user program with old pid? need PCB

    /* Restore Parent Paging */ //pcb needs to be done for this to retrive parent block


    /* Close any relevant FDs */
    int i;
    for(i = 0; i<8; i++) {
        //if flag is in use (PCB)
            close(i);
    }

    /* Jump to execute return */
    // asm volatile (
	//     "movl smthn into smthn"
    //     "movl smthn into smthn"
    //     "jmp to smthn"
	//     : "r" //to old kernal ebp and esp prob
	//     : "%eax"
	// );

	return 0;

}
