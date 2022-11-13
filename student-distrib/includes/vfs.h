#ifndef VFS_H
#define VFS_H
#include "types.h"


#define FD_ARRAY_SIZE 8


typedef struct fd_ops {
    int32_t (*open)     (uint32_t fd);
    int32_t (*close)    (uint32_t fd);
    int32_t (*read)     (uint32_t fd, uint8_t* buf, uint32_t length);
    int32_t (*write)    (uint32_t fd, uint8_t* buf, uint32_t length);
} fd_ops_t;


typedef struct fd_flags{
    uint32_t    present    : 1; // : is called bitfield
    uint32_t    type       : 2; 
    char        reserved[29];

}fd_flags_t;

typedef struct fd_entry{
    fd_ops_t*   j_tbl;
    // union{
    //     struct{
    //         uint32_t    inode_idx;
    //         uint32_t    file_position;
    //     }; // file type = 2 (files)
    //     uint32_t rtc_freq; // file type = 0 (rtc)
    // }
    uint32_t    inode_idx;
    uint32_t    file_position;
    struct{
        uint32_t    freq;
    }rtc;
    
    fd_flags_t  flags;
} fd_entry_t;

int32_t init_vfs();
int32_t init_fd_array(fd_entry_t* fd_array);
/* Main file descriptor functions */

/* Utility functions for file descriptor table*/
int32_t      get_free_fd_entry_idx();
void    free_fd_entry(uint32_t idx);
void    refresh_cur_fd_array();


#endif // VFS_H
