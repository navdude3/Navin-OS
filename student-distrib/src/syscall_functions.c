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
    entry->flags.type = f_dentry.file_type;
    
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
    if(fd == 1 || fd > 10 || buf == NULL) return -1;
    pcb_t* cur_process = get_curr_pcb();
    if(cur_process->fd_array[fd].flags.present == 0) return -1;
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
    if(cur_process->fd_array[fd].flags.present == 0) return -1;
    return cur_process->fd_array[fd].j_tbl->write(fd, buf, length);
}


/* 
 * sys_getargs
 *   DESCRIPTION: Obtains the arguments for a command
 *   INPUTS: uint8_t* buf - buffer to read into, uint32_t nbytes - number of bytes to copy
 *   OUTPUTS: None
 *   RETURN VALUE: 0 always 
 */
int32_t sys_getargs(uint8_t* buf, uint32_t nbytes) {
   if((buf == NULL) || (get_curr_pcb()->arg_size > nbytes)) return -1;          /* If buffer is null or size is larger than number of bytes, return -1*/
   pcb_t* current = get_curr_pcb();
   int i;
   for(i = 0; i < 128; i++) buf[i] = '\0';                                      /* Fills buffer will null terminators*/
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
    /* Fail if address is outside current process address space*/
    pte_desc_t vidmap_entry;
    uint32_t user_addr_base = PROGRAM_VMEM_BASE;
    if (    (uint32_t) screen_start < user_addr_base
        ||  (uint32_t) screen_start >= user_addr_base + PROGRAM_SIZE){
            return -1;
        }
    
    int32_t term_id = get_curr_pcb()->term_id;
    
    if (cur_term_id == term_id){
        vidmap_entry.val = (0xB8000 | 0x7); // 0x7 to set video memory as present for user
    } else{
        uint32_t term_addr = (uint32_t) get_term(term_id);
        vidmap_entry.val =   (term_addr | 0x7);
    }
    
    set_ptentry(usr_vidmap_table_desc.addr, term_id, vidmap_entry);
    
    *screen_start =  (uint8_t *) VIDMAP_TABLE_BASE + (term_id << 12);
    return 0;
}

/* 
 * sys_set_handler
 *   DESCRIPTION: Not implemented for this CP
 *   INPUTS: uint32_t signum, void* handler_address
 *   OUTPUTS: None for now
 *   RETURN VALUE: 0 always for now
 */
int32_t sys_set_handler(uint32_t signum, void* handler_address) {return -1;}

/* 
 * sys_sigreturn
 *   DESCRIPTION: Not implemented for this CP
 *   INPUTS: None
 *   OUTPUTS: None for now
 *   RETURN VALUE: 0 always for now
 */
int32_t sys_sigreturn(void) {return -1;}

