#include "vfs.h"

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