int32_t load_program(void *addr, int fd) {

}


int32_t setup_user_page(void *addr) {

}


int32_t check_executable(int fd) {

}               


int32_t setup_pcb(void *addr) {

}


int32_t sys_execute(const uint8_t* command) {
    if(command == NULL) return -1;
    if(command[0] == ' ') return -1;                // file non existent 
    /* Parse args */

    /* Executable check */
    /* Set up program paging */
    /* User-level Program Loader */
    /* Create PCB */
    /* Context Switch */
    /* Create it’s own context switch stack */
    /* IRET */
}


int32_t sys_halt(uint8_t status) {

}


/* Use file operations jump table to call the corresponding read or write function */
int32_t read (int32_t fd, void* buf, int32_t nbytes) {
    if(fd < 1 || fd > 10 || buf == NULL) return -1;                     /* Sys calls go from 1-10 */
    // check if process is currently active? PCB 
    // if(smthnsmthn.flag == 1) return -1

    /* what else is this supposed to do ???? */

}


/* Use file operations jump table to call the corresponding read or write function */
int32_t write (int32_t fd, void* buf, int32_t nbytes) {
    if(fd < 1 || fd > 10 || buf == NULL) return -1;  
    // check if process is currently active? PCB 
    // if(smthnsmthn.flag == 1) return -1

    // write to correct file    
}


/*Close the file descriptor passed in (set it to be available)
  Check for invalid descriptors*/
  int32_t close(int32_t fd) {
    if(fd < 2 || fd > 10) return -1;            /* Checking for invalid descriptor */
    /* closing descriptor ? smthn with the PCB */
  }


/* Find the file in the file system and assign an unused file descriptor
   File descriptors need to be set up according to the file type */
 int32_t open (const uint8_t* filename) {
    if(filename == NULL) return -1;
    // check nonexistent file name and 

    int i;
    for(i = 0; i < 8; i++) {
        // check if flag of a process is unused
        // assign to this process
    }
    if(i == 8) return -1;           // process cannot currently execute

    // call read_dentry_by_name? to see if file actually exists

    // then set up file descriptors

    // then idk
 }