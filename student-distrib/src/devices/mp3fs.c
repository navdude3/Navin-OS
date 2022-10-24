#include "mp3fs.h"
#include "lib.h"

#define DIR_READ_DELIM ','

fd_ops_t file_ops = (fd_ops_t){
    f_open,
    f_close,
    f_read,
    f_write
};
fd_ops_t dir_ops = (fd_ops_t){
    d_open,
    d_close,
    d_read,
    d_write
};

void mp3fs_init (module_t* mod){
    uint32_t* ptr = (uint32_t*) mod->mod_start;
    boot_blk = (boot_block_t*) ptr;
    ptr += BLK_PTR_INC;
    init_inode = (inode_t*) ptr;
    ptr += (BLK_PTR_INC*boot_blk->num_inodes);
    init_dblock = (dblock_t*) ptr;
    fs_end_pt = (uint32_t*) mod->mod_end;

    
    return;
}

int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
    if(index > boot_blk->num_dirs) return -1;

    *dentry = (boot_blk->d_entries[index]);
    return 0;
}

int32_t read_dentry_by_name (const uint8_t* fname, dentry_t* dentry){
    uint32_t idx;

    for (idx = 0; idx < boot_blk->num_dirs; ++idx){

        if(!strncmp((int8_t*)boot_blk->d_entries[idx].file_name, (int8_t*) fname, 32)){
            read_dentry_by_index(idx, dentry);
            return 0;
        }
    }
    return -1;
}

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    inode_t inode_entry = init_inode[inode];
    uint32_t dblk_idx,dblk_off, cnt, file_size;
    file_size = inode_entry.file_size;
    if(offset >= file_size) return 0;
    dblk_idx = (offset/BLK_SIZE);
    dblk_off = offset%BLK_SIZE;
    dblock_t* blk = &(init_dblock[inode_entry.dblock_idxs[dblk_idx]]);
    if(inode_entry.dblock_idxs[dblk_idx] >= boot_blk->num_dblocks) return -1; //bad block
    cnt = 0;

    while(cnt < length && offset < file_size){
        if (dblk_off == BLK_SIZE){
            ++dblk_idx;
            dblk_off = 0;
            blk = &(init_dblock[inode_entry.dblock_idxs[dblk_idx]]);
            if((uint32_t*) blk >= fs_end_pt) return -1;    
        }
        buf[cnt] = blk->data[dblk_off];
        ++cnt;
        ++offset; ++dblk_off;
    }

    return cnt;
}


int32_t f_open              (const uint8_t* fname){
    return 0;
}

int32_t f_close             (uint32_t fd){
    return 0;
}

int32_t f_read              (uint32_t fd, uint8_t* buf, uint32_t length){
    uint32_t bytes_read = read_data(fd_array[fd].inode_idx, fd_array[fd].file_position, buf, length);
    fd_array[fd].file_position += bytes_read;
    return bytes_read;
}
int32_t f_write             (uint32_t fd, uint8_t* buf, uint32_t length){
    return -1;
}

int32_t d_open              (const uint8_t* fname){
    return 0;
}
int32_t d_close             (uint32_t fd){
    return 0;
}


int32_t d_read(uint32_t fd, uint8_t *buf, uint32_t length){
    uint32_t* d_offset = &fd_array[fd].file_position;
    
    if (*d_offset >= boot_blk->num_dirs) return 0;
    if (length < sizeof(dentry_t)) return -1;
    memcpy(buf, &boot_blk->d_entries[*d_offset], sizeof(dentry_t));
    (*d_offset)++;
    return sizeof(dentry_t);
}

int32_t d_write             (uint32_t fd, uint8_t* buf, uint32_t length){
    return -1;
}
