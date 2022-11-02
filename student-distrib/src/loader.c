#include "loader.h"
#include "paging.h"

int32_t load_program(void *addr, int fd) {

}


int32_t setup_user_page(int process_number) {
    
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
