#include "vfs.h"

FD      get_free_fd_idx(){
    int idx;
    for (idx = 2; idx < FD_ARRAY_SIZE; ++idx){
        if(fd_array[idx].flags.present == 0){
            fd_array[idx].flags.present = 1;
            return idx;
        }
    }
    return -1;
}

void free_fd_entry(FD idx){
    fd_array[idx].flags.present = 0;
}