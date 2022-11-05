#include "syscall_functions.h"
#include "vfs.h"
#include "mp3fs.h"
#include "terminal.h"
#include "process.h"

// int32_t sys_halt(uint8_t status){

//     return 10;
// }

// int32_t sys_execute(const uint8_t* command){

//     return 0;
// }

/* 
 * open
 *   DESCRIPTION: VFS filesystem open call that looks up file entry and assigns a filedescriptor (FD) entry to the global fd array
 *   INPUTS: filename- filename to open
 *   OUTPUTS: none
 *   RETURN VALUE: index to file descriptor entry in fd array, -1 if error occured
 *   SIDE EFFECTS: calls file type's open() function, marks entry as present (might need to implement race condition handling)
 */
int32_t sys_open (const uint8_t* filename) {
    dentry_t f_dentry;
    fd_entry_t* entry;
    pcb_t* cur_process = get_curr_pcb();

    if(filename == NULL) return -1;
    
    if(read_dentry_by_name(filename, &f_dentry) < 0){
        return -1;
    } 
    uint32_t fd = get_free_fd_entry_idx();
    if (fd < 0) return fd;
    entry = &cur_process->fd_array[fd];

    switch (f_dentry.file_type){
        // ADD CASE 0 ONCE FD OPERATIONS IMPLEMENTED FOR RTC
        case 1 :
            entry->j_tbl = &dir_ops;
            break;
        case 2 :
            entry->j_tbl = &file_ops;
            entry->inode_idx = f_dentry.inode_idx;
            break;
    }
    entry->flags.type = f_dentry.file_type;
    entry->file_position = 0;
    entry->j_tbl->open(filename);
    
    return fd;
}

/* 
 * close
 *   DESCRIPTION: VFS filesystem close call that makes fd entry available in fd array
 *   INPUTS: fd- fd index to close
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if successful, -1 if error
 *   SIDE EFFECTS: calls file type's close() function, marks entry as not present (might need to implement race condition handling)
 */
int32_t sys_close(uint32_t fd){
    if(fd < 2 || fd > 10) return -1;   // cannot close stdin/out
    pcb_t* cur_process = get_curr_pcb();
    if(cur_process->fd_array[fd].flags.present == 0) return -1;
    if(cur_process->fd_array[fd].j_tbl->close(fd) < 0) return -1;
    free_fd_entry(fd);
    return 0;
}

/* 
 * read
 *   DESCRIPTION: VFS filesystem read call that calls file type's read function and writes to provided buffer
 *   INPUTS: fd- fd index to read from; buf- buffer to read data into, length- amount of bytes to read
 *   OUTPUTS: writes length bytes or until EOF into buf, whichever is shorter
 *   RETURN VALUE: return value of file type's read function
 */
int32_t sys_read(uint32_t fd, uint8_t* buf, uint32_t length){
    if(fd < 1 || fd > 10 || buf == NULL) return -1;
    pcb_t* cur_process = get_curr_pcb();
    return cur_process->fd_array[fd].j_tbl->read(fd, buf, length);
}

/* 
 * write
 *   DESCRIPTION: VFS filesystem write call that calls file type's write function and writes from buffer to file
 *   INPUTS: fd- fd index to write to; buf- buffer to read data from, length- amount of bytes to write
 *   OUTPUTS: writes length bytes into file
 *   RETURN VALUE: return value of file type's write function
 */
int32_t sys_write (uint32_t fd, uint8_t* buf, uint32_t length){
    if(fd < 1 || fd > 10 || buf == NULL) return -1;  
    pcb_t* cur_process = get_curr_pcb();
    return cur_process->fd_array[fd].j_tbl->write(fd, buf, length);
}


int32_t sys_getargs(uint8_t* buf, uint32_t nbytes) {return 0;}

int32_t sys_vidmap(uint8_t** screen_start) {return 0;}

int32_t sys_set_handler(uint32_t signum, void* handler_address) {return 0;}

int32_t sys_sigreturn(void) {return 0;}
