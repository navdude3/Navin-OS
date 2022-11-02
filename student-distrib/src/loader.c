#include "loader.h"
int32_t load_program(void *addr, int fd) {
    dentry_t entry;
    uint32_t buf[4];
    if(addr == NULL || fd == NULL) return -1;                                           /* Invalid arguments */
    if(read_dentry_by_name(fd, &entry) == -1) return -1;                                /* Entry does not exist in directory */
    read_data(entry.inode_idx, 0, buf, 4);                                              /* Writes first four bytes of data to buf */
    if(*buf != 0x464c457f) return -1;                                                   /* Not an executable */
    /* At this point, we have verified that the file exists and is a valid executable. Can now copy the program into address */
    while (0 != read_data(entry.inode_idx, 0, addr, 4)) continue;                       /* Not sure abt length -- intention is to read whole program byte by byte */ 
    /* How is switch to user done? */
    int32_t entry_point = (uint32_t*) (addr+24);                                        /* Return entry point at bit 24 */
    return addr;                                           
}


int32_t setup_user_page(void *addr) {

}       


int32_t setup_pcb(void *addr) {

}


int32_t sys_execute(const uint8_t* command) {
    uint8_t fname[32];                              // max filesize name
    uint8_t args[32];                               // store args here
    int i;
    int fname_indexer, args_indexer;
    int file_flag, args_flag;
    uint32_t user_addr = (num_active_procs * 0x400000) + USER_MEMORY_BASE;

    if(command == NULL) return -1;
    if(command[0] == '\0') return -1;                // file non existent

    /* 1. Parse args and name */
    file_flag = 0;
    args_flag = 0;
    for(i = 0; i < 128; i++){
        if(command[i] != ' ' && file_flag == 0){
            for(fname_indexer = i; fname_indexer < 128; fname_indexer++){
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
            for(args_indexer = i; args_indexer < 128; args_indexer++){
                if(command[args_indexer] == ' '){
                    args_flag = 1;
                    break;
                }
                args[args_indexer] = command[args_indexer];
            }
        }
    }


    //make a test case here to see if these can print name and args


    /* 2. Executable check */
    dentry_t dentry;   
    uint32_t buf[4];      
    uint32_t start;              
      
    if(read_dentry_by_name(fname, &dentry) == -1) return -1;            /* Does file exist */
    if(read_data(dentry.inode_idx, 0, buf, 4) == -1) return -1;         /* Error occurred while writing */
    if(*buf != 0x464c457f) return -1;                                   /* Checks if it is an executable */
    /* Valid Executable */
    read_data(dentry.inode_idx, 24, buf, 4);                            /* Read the four bytes from 24-27 that contain virtual address of first instruction to be executed */
    start = (uint32_t*)buf;                                             /* ?? is this the right accessing */


    /* 3. Set up program paging */
    setup_user_page((void*)user_addr);
    flush_tlb();

    /* 4. User Level Program Loader */
    inode_t inode = init_inode[dentry.inode_idx];
    uint32_t size = inode.file_size;
    read_data(dentry.inode_idx, 0, (uint8_t*)0x08048000, size);         /* Copying entire file to memory */

    /* 5. Create PCB */
    // give pcb memory
    // set active
    // set file descriptor 

    /* 6. Create it’s own context switch stack */
    num_active_procs++;                           
    //return;
}


int32_t sys_halt(uint8_t status) {

}
