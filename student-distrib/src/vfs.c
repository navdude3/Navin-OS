#include "vfs.h"
#include "lib.h"
#include "mp3fs.h"
#include "terminal.h"
#include "process.h"

/* 
 * init_vfs
 *   DESCRIPTION: Initializes the global 
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if successful
 *   SIDE EFFECTS: Initializes stdout to fd entry 1
 */
int32_t init_vfs(){
    init_fd_array(global_fd_array);
    return 0;
}

int32_t init_fd_array(fd_entry_t* fd_array){
    /* terminal driver assigned to fd 1*/
    int i;

    for (i = 0; i < 2; ++i){
        fd_entry_t* stdout_fd = &fd_array[i]; // i = 0 initializes stdin , i = 1 initializes stdout
        stdout_fd->file_position = 0;
        stdout_fd->flags.present = 1;
        stdout_fd->j_tbl = &terminal_ops; // both stdin and stdout use terminal driver
        stdout_fd->j_tbl->open((uint8_t*)"garbage"); // initialize terminal driver
    }
    
    return 0;
}

/* 
 * get_free_fd_entry_idx
 *   DESCRIPTION: Finds first available fd entry in global fd array
 *   INPUTS: none
 *   OUTPUTS: index to available entry
 *   RETURN VALUE: none
 *   SIDE EFFECTS: marks entry as present (might need to implement race condition handling), must be freed with close()
 */
int32_t get_free_fd_entry_idx(){
    int idx;
    pcb_t* cur_process = get_curr_pcb();

    for (idx = 2; idx < FD_ARRAY_SIZE; ++idx){
        if(cur_process->fd_array[idx].flags.present == 0){
            cur_process->fd_array[idx].flags.present = 1;
            return idx;
        }
    }
    return -1;
}

/* 
 * free_fd_entry
 *   DESCRIPTION: Frees corresponding fd entry
 *   INPUTS: idx- fd entry to free
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: marks entry as not present (might need to implement race condition handling), does not clear contents (for now)
 */
void free_fd_entry(uint32_t idx){
    pcb_t* cur_process = get_curr_pcb();
    cur_process->fd_array[idx].flags.present = 0;
}

