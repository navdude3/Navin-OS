#include "mp3fs.h"
#include "lib.h"

#define DIR_READ_DELIM ','

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

int32_t read_dentry_of_type(const uint8_t* fname, dentry_t* dentry, uint32_t type){
    if(read_dentry_by_name(fname, dentry) < 0){
        return -1;
    } else if (dentry->file_type != type){
        return -1;
    }
    return 0;
}

int32_t f_open              (const uint8_t* fname){
    f_offset = 0;
    return read_dentry_of_type(fname, &f_dentry, REG_FILE_TYPE);
}

int32_t f_close             (void){
    f_offset = 0;
    return 0;
}

int32_t f_read              (uint8_t* buf, uint32_t length){
    uint32_t bytes_read = read_data(f_dentry.inode_idx, f_offset, buf, length);
    f_offset += bytes_read;
    return bytes_read;
}

int32_t d_open              (const uint8_t* fname){
    d_offset = 0;
    return 0;
}
int32_t d_close             (void){
    return 0;
}

// file name,type,size (comma separated)
int32_t d_read(uint8_t *buf, uint32_t length){
    if (d_offset >= boot_blk->num_dirs) return 0;
    

    uint8_t buf_ptr = 0;
    char* file_name = boot_blk->d_entries[d_offset].file_name;
    uint32_t file_type = boot_blk->d_entries[d_offset].file_type;
    uint32_t file_size = 0;
    if (file_type == 2){
        file_size = init_inode[boot_blk->d_entries[d_offset].inode_idx].file_size;
    }
    char file_type_str[10];
    itoa(file_type,file_type_str,10);

    char file_size_str[10];
    itoa(file_size,file_size_str,10);
    // Length check
    uint32_t fname_len = strlen(file_name);
    if(fname_len > FNAME_LIMIT){
        fname_len = FNAME_LIMIT; //ceiling
    }
    uint32_t ftype_len = strlen(file_type_str);
    uint32_t fsize_len = strlen(file_size_str);
    if(fname_len + ftype_len + fsize_len > length) return -1; // overflow protection

    ++d_offset;
       
    strncpy((int8_t *)buf+buf_ptr, file_name,fname_len);
    buf_ptr += fname_len;

    buf[buf_ptr] = DIR_READ_DELIM;
    buf_ptr++;
    
    strncpy((int8_t *)buf+buf_ptr, file_type_str,ftype_len);
    buf_ptr += ftype_len;

    buf[buf_ptr] = DIR_READ_DELIM;
    buf_ptr++;

    strncpy((int8_t *)buf+buf_ptr, file_size_str,fsize_len);
    buf_ptr += fsize_len;
    
    return buf_ptr;
}
