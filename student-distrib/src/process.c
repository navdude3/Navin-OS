#include "process.h"

init_proc_fd_array(proc_ctrl_blk_t* ctrl_blk){
    ctrl_blk->proc_fd_map[0] = 0; // stdin
    ctrl_blk->proc_fd_map[1] = 1; // stdout
}