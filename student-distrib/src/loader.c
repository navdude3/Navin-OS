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
    if(command == NULL) return -1;
    if(command[0] == ' ') return -1;                // file non existent 
    /* 1. Parse args */

    /* 2. Executable check */
    dentry_t dentry;   
    uint32_t buf[4];      
    uint32_t start;              
    /* SHOULD COMMAND BE THE FIRST ARG HERE ???? */     
    if(read_dentry_by_name(command, &dentry) == -1) return -1;          /* Does file exist */
    if(read_data(dentry.inode_idx, 0, buf, 4) == -1) return -1;         /* Error occurred while writing */
    if(*buf != 0x464c457f) return -1;                                   /* Checks if it is an executable */
    /* Valid Executable */
    read_data(dentry.inode_idx, 24, buf, 4);                            /* Read the four bytes from 24-27 that contain virtual address of first instruction to be executed */
    start = (uint32_t*)buf;                                             /* ?? is this the right accessing */


    /* 3. Set up program paging */
    // no idea
    // flush tlb prob

    /* 4. User Level Program Loader */

    /* 5. Create PCB */
    // give pcb memory
    // set active
    // set file descriptor 

    /* 6. Create it’s own context switch stack */

    /* IRET */
}


int32_t sys_halt(uint8_t status) {

}
