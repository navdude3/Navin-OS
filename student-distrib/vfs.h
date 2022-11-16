#ifndef VFS_H
#define VFS_H
#include "types.h"


#define FD_ARRAY_SIZE 8
typedef struct fd_ops fd_ops_t;

typedef struct fd_flags{
    uint32_t    present    : 1; // : is called bitfield
    uint32_t    type       : 2; 
    char        reserved[29];

}fd_flags_t;

typedef struct fd_entry{
    fd_ops_t*   j_tbl;
    union{
        uint32_t    inode_idx; // file type = 2 (files)
        uint32_t    rtc_freq; // file type = 0 (rtc)
        uint32_t    term_id;  // for stdio files
    };
    uint32_t    file_position;
    
    
    fd_flags_t  flags;
} fd_entry_t;

struct fd_ops {
    int32_t (*open)     (fd_entry_t* fd_entry);
    int32_t (*close)    (fd_entry_t* fd_entry);
    int32_t (*read)     (fd_entry_t* fd_entry, uint8_t* buf, uint32_t length);
    int32_t (*write)    (fd_entry_t* fd_entry, uint8_t* buf, uint32_t length);
};


int32_t init_vfs();

/* Main file descriptor functions */

/* Utility functions for file descriptor table*/
int32_t      get_free_fd_entry_idx();
void    free_fd_entry(fd_entry_t* fd_entry);
void    refresh_cur_fd_array();


#endif // VFS_H
