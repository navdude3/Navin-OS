#ifndef VFS_H
#define VFS_H
#include "types.h"


#define FD_ARRAY_SIZE 64


typedef struct fd_ops {
    int32_t (*open)     (const uint8_t* fname);
    int32_t (*close)    (uint32_t fd);
    int32_t (*read)     (uint32_t fd, uint8_t* buf, uint32_t length);
    int32_t (*write)    (uint32_t fd, uint8_t* buf, uint32_t length);
} fd_ops_t;


typedef struct fd_flags{
    uint32_t    present    : 1; // : is called bitfield
    uint32_t    type       : 2; //
    char        reserved[29];

}fd_flags_t;

typedef struct fd_entry{
    fd_ops_t*   j_tbl;
    uint32_t    inode_idx;
    uint32_t    file_position;
    fd_flags_t  flags;
} fd_entry_t;

int32_t init_vfs();
/* Main file descriptor functions */

int32_t open              (const uint8_t* fname);
int32_t close             (uint32_t fd);
int32_t read              (uint32_t fd, uint8_t* buf, uint32_t length);
int32_t write             (uint32_t fd, uint8_t* buf, uint32_t length);

/* Utility functions for file descriptor table*/
int32_t      get_free_fd_entry_idx();
void    free_fd_entry(uint32_t idx);

fd_entry_t fd_array [FD_ARRAY_SIZE];

#endif // VFS_H
