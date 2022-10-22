#include "vfs.h"
#include "lib.h"
#include "mp3fs.h"

uint32_t      get_free_fd_entry_idx(){
    int idx;
    for (idx = 2; idx < FD_ARRAY_SIZE; ++idx){
        if(fd_array[idx].flags.present == 0){
            fd_array[idx].flags.present = 1;
            return idx;
        }
    }
    return -1;
}

void free_fd_entry(uint32_t idx){
    fd_array[idx].flags.present = 0;
}

int32_t open              (const uint8_t* fname){
    dentry_t f_dentry;
    fd_entry_t* entry;
    uint32_t fd = get_free_fd_entry_idx();
    if (fd < 0) return fd;
    entry = &fd_array[fd];
    if(read_dentry_by_name(fname, &f_dentry) < 0) return -1;

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
    
    return fd;
}

int32_t close             (uint32_t fd){
    if(fd_array[fd].j_tbl->close(fd) < 0) return -1;
    free_fd_entry(fd);
    return 0;
}

int32_t read              (uint32_t fd, uint8_t* buf, uint32_t length){
    return fd_array[fd].j_tbl->read(fd, buf, length);
}

int32_t write              (uint32_t fd, uint8_t* buf, uint32_t length){
    return fd_array[fd].j_tbl->write(fd, buf, length);
}
