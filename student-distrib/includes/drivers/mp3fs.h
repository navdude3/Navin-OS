#ifndef MP3FS_H
#define MP3FS_H
#include "types.h"
#include "multiboot.h"
#include "vfs.h"


#define BLK_SIZE        4096
#define BLK_PTR_INC     (BLK_SIZE/4)
#define DIR_FILE_TYPE   1
#define REG_FILE_TYPE   2
#define FNAME_LIMIT     32

typedef struct __attribute__((packed)) dblock {
    char data[BLK_SIZE];
} dblock_t;


typedef struct __attribute__((packed)) inode {
    uint32_t file_size; // in bytes
    uint32_t dblock_idxs[BLK_PTR_INC - 1];
} inode_t;

typedef struct __attribute__((packed)) dentry
{
    char file_name[32];
    uint32_t file_type;
    uint32_t inode_idx;
    char reserved[24];
} dentry_t;

typedef struct __attribute__((packed)) boot_block {
    uint32_t num_dirs;
    uint32_t num_inodes;
    uint32_t num_dblocks;
    char reserved[52];
    dentry_t d_entries[63];
} boot_block_t;

boot_block_t*    boot_blk;
inode_t*         init_inode;
dblock_t*        init_dblock;
uint32_t*        fs_end_pt;

/*file related code*/
dentry_t         f_dentry;
uint32_t         f_offset;



int32_t f_open              (const uint8_t* fname);
int32_t f_close             (uint32_t fd);
int32_t f_read              (uint32_t fd, uint8_t* buf, uint32_t length);
int32_t f_write             (uint32_t fd, uint8_t* buf, uint32_t length); //readonly fs

/*directory related code*/
uint32_t         d_offset;



int32_t d_open              (const uint8_t* fname);
int32_t d_close             (uint32_t fd);
int32_t d_read              (uint32_t fd, uint8_t* buf, uint32_t length);
int32_t d_write             (uint32_t fd, uint8_t* buf, uint32_t length); //readonly fs
/* Module functions*/

void    mp3fs_init          (module_t* mod);
int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry);
int32_t read_dentry_by_index(uint32_t index, dentry_t* dentry);
int32_t read_data           (uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

/* vfs related declarations */
extern fd_ops_t file_ops;

extern fd_ops_t dir_ops;
#endif // MP3FS_H
