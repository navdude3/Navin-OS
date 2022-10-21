#ifndef VFS_H
#define VFS_H
#include "types.h"

#define FD_ARRAY_SIZE 64
typedef uint32_t FD;

typedef struct fd_ops {
    int32_t (*open)     (const uint8_t* fname);
    int32_t (*close)    (FD fd);
    int32_t (*read)     (FD fd, uint8_t* buf, uint32_t length);
    int32_t (*write)    (FD fd, uint8_t* buf, uint32_t length);
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

FD      get_free_fd_idx();
void    free_fd_entry(FD idx);

fd_entry_t fd_array [FD_ARRAY_SIZE];

#endif // VFS_H
