#include "syscall_functions.h"
#include "vfs.h"
#include "mp3fs.h"
#include "terminal.h"
#include "process.h"
#include "rtc.h"
#include "paging.h"
#include "loader.h"

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
    pcb_t* cur_process = get_cur_proc();

    if(filename == NULL) return -1;                                             /* Checking for valid file */
    if(read_dentry_by_name(filename, &f_dentry) < 0) return -1;
    
    uint32_t fd = get_free_fd_entry_idx();
    if (fd < 0) return fd;
    entry = &cur_process->fd_array[fd];                                         /* Getting entry point */

    switch (f_dentry.file_type){                                                /* Determining which type of file, rtc, terminal, or file */
        case 0 :
            entry->j_tbl = &rtc_ops;
            break;
        case 1 :
            entry->j_tbl = &dir_ops;
            break;
        case 2 :
            entry->j_tbl = &file_ops;
            entry->inode_idx = f_dentry.inode_idx;
            break;
    }
    entry->flags.type = f_dentry.file_type;                                     /* Storing type of file that is being opened */
    entry->j_tbl->open(entry);
    
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
    if(fd < 2 || fd > 10) return -1;                    // cannot close stdin/out -> fd 0/1
    pcb_t* cur_process = get_cur_proc();
    fd_entry_t* fd_e = &cur_process->fd_array[fd];
    if(fd_e->flags.present == 0) return -1;             /* Not open */
    if(fd_e->j_tbl->close(fd_e) < 0) return -1;
    free_fd_entry(fd_e);
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
    if(fd == 1 || fd > 10 || buf == NULL) return -1;                /* Invalid fd number or empty buffer */
    pcb_t* cur_process = get_cur_proc();
    fd_entry_t* fd_e = &cur_process->fd_array[fd];
    if(fd_e->flags.present == 0) return -1;                         /* Entry not present */
    return fd_e->j_tbl->read(fd_e, buf, length);
}

/* 
 * write
 *   DESCRIPTION: VFS filesystem write call that calls file type's write function and writes from buffer to file
 *   INPUTS: fd- fd index to write to; buf- buffer to read data from, length- amount of bytes to write
 *   OUTPUTS: writes length bytes into file
 *   RETURN VALUE: return value of file type's write function
 */
int32_t sys_write (uint32_t fd, uint8_t* buf, uint32_t length){
    if(fd < 1 || fd > 10 || buf == NULL) return -1;                 /* Invalid fd number or empty buffer */
    pcb_t* cur_process = get_cur_proc();
    fd_entry_t* fd_e = &cur_process->fd_array[fd];
    if(fd_e->flags.present == 0) return -1;                         /* Entry not present */
    return fd_e->j_tbl->write(fd_e, buf, length);
}


/* 
 * sys_getargs
 *   DESCRIPTION: Obtains the arguments for a command
 *   INPUTS: uint8_t* buf - buffer to read into, uint32_t nbytes - number of bytes to copy
 *   OUTPUTS: None
 *   RETURN VALUE: 0 always 
 */
int32_t sys_getargs(uint8_t* buf, uint32_t nbytes) {
   if((buf == NULL) || (get_cur_proc()->arg_size > nbytes)) return -1;          /* If buffer is null or size is larger than number of bytes, return -1*/
   pcb_t* current = get_cur_proc();
   int i;
   for(i = 0; i < nbytes; i++) buf[i] = '\0';                                   /* Fills buffer will null terminators */
   for(i = 0; i < current->arg_size; i++) buf[i] = current->args[i];            /* Places arguments at the beginning of buffer */
   return 0;
}

/* 
 * sys_vidmap
 *   DESCRIPTION: Setup page entry for user video memory 
 *   INPUTS: uint8_t** screen_start - pointer to pass user video memory location to
 *   SIDE EFFECTS: *screen_start modified, video memory page table entry pertaining to process modified
 *   RETURN VALUE: 0 if successful, -1 if screen_start is not within user memory space 
 */
int32_t sys_vidmap(uint8_t** screen_start) {
    uint32_t user_addr_base = PROGRAM_VMEM_BASE;
    /* Fail if address is outside current process address space*/
    if ((uint32_t) screen_start < user_addr_base || (uint32_t) screen_start >= user_addr_base + PROGRAM_SIZE) return -1;

    uint32_t term_id = get_cur_proc()->term_id;
    *screen_start = (uint8_t*)(VIDMAP_TABLE_BASE + term_id*FOURKB);
    return 0;
}

/* 
 * sys_set_handler
 *   DESCRIPTION: Not implemented
 *   INPUTS: None
 *   OUTPUTS: None
 *   RETURN VALUE: -1 always
 */
int32_t sys_set_handler(uint32_t signum, void* handler_address) {return -1;}

/* 
 * sys_sigreturn
 *   DESCRIPTION: Not implemented
 *   INPUTS: None
 *   OUTPUTS: None
 *   RETURN VALUE: -1 always
 */
int32_t sys_sigreturn(void) {return -1;}

