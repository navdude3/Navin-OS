#include "vfs.h"
#include "lib.h"
#include "mp3fs.h"
#include "terminal.h"

/* 
 * init_vfs
 *   DESCRIPTION: Initializes the virtual filesystem and stdoutput
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if successful
 *   SIDE EFFECTS: Initializes stdout to fd entry 1
 */
int32_t init_vfs(){
    /* terminal driver assigned to fd 1*/
    fd_entry_t* stdout_fd = &fd_array[1];
    stdout_fd->file_position = 0;
    stdout_fd->flags.present = 1;
    stdout_fd->j_tbl = &terminal_ops;
    stdout_fd->j_tbl->open((uint8_t*)"garbage"); // initialize terminal driver
    return 0;
}

/* 
 * get_free_fd_entry_idx
 *   DESCRIPTION: Finds first available fd entry in fd array
 *   INPUTS: none
 *   OUTPUTS: index to available entry
 *   RETURN VALUE: none
 *   SIDE EFFECTS: marks entry as present (might need to implement race condition handling), must be freed with close()
 */
int32_t      get_free_fd_entry_idx(){
    int idx;
    for (idx = 2; idx < FD_ARRAY_SIZE; ++idx){
        if(fd_array[idx].flags.present == 0){
            fd_array[idx].flags.present = 1;
            return idx;
        }
    }
    return -1;
}

/* 
 * free_fd_entry
 *   DESCRIPTION: Frees corresponding fd entry
 *   INPUTS: idx- fd entry to free
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: marks entry as not present (might need to implement race condition handling), does not clear contents (for now)
 */
void free_fd_entry(uint32_t idx){
    fd_array[idx].flags.present = 0;
}

/* 
 * open
 *   DESCRIPTION: VFS filesystem open call that looks up file entry and assigns a filedescriptor (FD) entry to the global fd array
 *   INPUTS: fname- filename to open
 *   OUTPUTS: none
 *   RETURN VALUE: index to file descriptor entry in fd array, -1 if error occured
 *   SIDE EFFECTS: calls file type's open() function, marks entry as present (might need to implement race condition handling)
 */
int32_t open              (const uint8_t* fname){
    dentry_t f_dentry;
    fd_entry_t* entry;
    
    if(read_dentry_by_name(fname, &f_dentry) < 0){
        return -1;
    } 
    uint32_t fd = get_free_fd_entry_idx();
    if (fd < 0) return fd;
    entry = &fd_array[fd];

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
    entry->j_tbl->open(fname);
    
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
int32_t close             (uint32_t fd){
    if(fd < 2) return -1; // cannot close stdin/out
    if(fd_array[fd].j_tbl->close(fd) < 0) return -1;
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
int32_t read              (uint32_t fd, uint8_t* buf, uint32_t length){
    return fd_array[fd].j_tbl->read(fd, buf, length);
}

/* 
 * write
 *   DESCRIPTION: VFS filesystem write call that calls file type's write function and writes from buffer to file
 *   INPUTS: fd- fd index to write to; buf- buffer to read data from, length- amount of bytes to write
 *   OUTPUTS: writes length bytes into file
 *   RETURN VALUE: return value of file type's write function
 */
int32_t write              (uint32_t fd, uint8_t* buf, uint32_t length){
    return fd_array[fd].j_tbl->write(fd, buf, length);
}
