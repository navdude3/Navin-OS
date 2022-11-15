#include "mp3fs.h"
#include "lib.h"
#include "process.h"

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

/* 
 * mp3fs_init
 *   DESCRIPTION: Initializes the filesystem
 *   INPUTS: module pointer pointing to the start of the filesys_img module
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: Assigns boot block, inode, and data block pointers
*/
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

/* 
 * read_dentry_by_index
 *   DESCRIPTION: Finds directory entry corresponding to index and writes to denty_t pointer
 *   INPUTS: index- Index in directory entries to lookup; dentry- dentry pointer to pass the directory entry block to
 *   OUTPUTS: Writes to dentry
 *   RETURN VALUE: 0 if successful, -1 if index is out of range
*/
int32_t read_dentry_by_index (uint32_t index, dentry_t* dentry){
    if(index > boot_blk->num_dirs) return -1;

    *dentry = (boot_blk->d_entries[index]);
    return 0;
}

/* 
 * read_dentry_by_name
 *   DESCRIPTION: Finds directory entry corresponding to filename provided and writes to denty_t pointer
 *   INPUTS: fname- filename in directory entries to lookup; dentry- dentry pointer to pass the directory entry block to
 *   OUTPUTS: Writes to dentry
 *   RETURN VALUE: 0 if successful, -1 if corresponding filename not found
*/
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
/* 
 * read_data
 *   DESCRIPTION: Reads data corresponding to inode+offset and writes into provided buffer
 *   INPUTS: inode- inode entry of file to read from; offset- offset in bytes from start of file; buf- pointer to buffer to write file data to; length- number of bytes to write to buf
 *   OUTPUTS: Writes "length" bytes or EOF to buf, whichever is shorter
 *   RETURN VALUE: number of bytes written to buffer, 0 if EOF is reacehd, -1 if any error in reading file data (buffer might still have been written into at this point)
*/
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length){
    inode_t* inode_entry = &init_inode[inode];
    uint32_t dblk_idx,dblk_off, cnt, file_size;
    file_size = inode_entry->file_size;
    if(offset >= file_size) return 0;
    dblk_idx = (offset/BLK_SIZE);
    dblk_off = offset%BLK_SIZE;
    dblock_t* blk = &(init_dblock[inode_entry->dblock_idxs[dblk_idx]]);
    if(inode_entry->dblock_idxs[dblk_idx] >= boot_blk->num_dblocks) return -1; //bad block
    cnt = 0;

    while(cnt < length && offset < file_size){
        if (dblk_off == BLK_SIZE){
            ++dblk_idx;
            dblk_off = 0;
            blk = &(init_dblock[inode_entry->dblock_idxs[dblk_idx]]);
            if((uint32_t*) blk >= fs_end_pt) return -1;    
        }
        buf[cnt] = blk->data[dblk_off];
        ++cnt;
        ++offset; ++dblk_off;
    }

    return cnt;
}

/* 
 * f_open
 *   DESCRIPTION: initializes variables needed for a system open call on a regular file
 *   INPUTS: fname- filename that is to be opened. Note that the system open call has a wrapper that handles the actual file lookup, so we don't use fname here. Strictly for adhering to abstraction interface
 *   RETURN VALUE: 0
*/
int32_t f_open              (fd_entry_t* fd_entry){
    fd_entry->file_position = 0;
    return 0;
}

/* 
 * f_close
 *   DESCRIPTION: Clears variables used for regular file interfacing
 *   INPUTS: fd- corresponding file descriptor entry to close. Note that system close call wrapper handles the fd entry cleanup, so nothing is done here. Strictly for adhering to abstraction interface.
 *   RETURN VALUE: 0
*/
int32_t f_close (fd_entry_t* fd_entry){
    return 0;
}

/* 
 * f_read
 *   DESCRIPTION: Reads file contents corresponding to file descriptor into buffer
 *   INPUTS: fd- file descriptor entry corresponding to file to read from; buf- pointer to byte buffer to write to; length- number of bytes to read into (should match the size of buffer)
 *   OUTPUTS: Writes length bytes or until EOF into buf, whichever is shorter
 *   RETURN VALUE: number of bytes written to buffer, 0 if EOF is reacehd, -1 if any error in reading file data (buffer might still have been written into at this point)
*/
int32_t f_read (fd_entry_t* fd_entry, uint8_t* buf, uint32_t length){
    uint32_t bytes_read = read_data(fd_entry->inode_idx, fd_entry->file_position, buf, length);
    fd_entry->file_position += bytes_read;
    return bytes_read;
}

/* 
 * f_write
 *   DESCRIPTION: Does not do anything, file system is read-only
 *   RETURN VALUE: -1 because write is an error case
*/
int32_t f_write (fd_entry_t* fd_entry, uint8_t* buf, uint32_t length){
    return -1;
}

/* 
 * d_open
 *   DESCRIPTION: initializes variables needed for a system open call on a directory file type
 *   INPUTS: fname- filename that is to be opened. Note that the system open call has a wrapper that handles the actual file lookup, so we don't use fname here. Strictly for adhering to abstraction interface
 *   RETURN VALUE: 0
*/
int32_t d_open (fd_entry_t* fd_entry){
    fd_entry->file_position = 0;
    return 0;
}

/* 
 * d_close
 *   DESCRIPTION: Clears variables used for regular file interfacing
 *   INPUTS: fd- corresponding file descriptor entry to close. Note that system close call wrapper handles the fd entry cleanup, so nothing is done here. Strictly for adhering to abstraction interface.
 *   RETURN VALUE: 0
*/
int32_t d_close (fd_entry_t* fd_entry){
    return 0;
}

/* 
 * d_read
 *   DESCRIPTION: Reads one directory entry filename into buffer
 *   INPUTS: fd- file descriptor entry corresponding to directory to read from; buf- pointer to byte buffer to write directory entry to; length- number of bytes to read into (should match the size of dentry_t)
 *   OUTPUTS: Writes exactly one directory entry filename into buffer
 *   RETURN VALUE: returns 0 if reached end of directory entries, otherwise number of bytes written to buf
*/
int32_t d_read(fd_entry_t* fd_entry, uint8_t *buf, uint32_t length){
    uint32_t* d_offset = (uint32_t*) &(fd_entry->file_position);
    if (*d_offset >= boot_blk->num_dirs) return 0;
    // if (length < sizeof(dentry_t)) return -1;
    if (length > FNAME_LIMIT) length = FNAME_LIMIT;
    strncpy((int8_t *)buf, (int8_t *) &boot_blk->d_entries[*d_offset], length);
    (*d_offset)++;
    return length;
}

/* 
 * d_write
 *   DESCRIPTION: Does not do anything, file system is read-only
 *   RETURN VALUE: -1 because write is an error case
*/
int32_t d_write             (fd_entry_t* fd_entry, uint8_t* buf, uint32_t length){
    return -1;
}
