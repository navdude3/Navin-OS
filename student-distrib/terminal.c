#include "terminal.h"
#include "loader.h"
#include "process.h"
#include "lib.h"
//make the terminal buffer
// static char term_buffer[BUFFER];

//size of the buffer
static int curr_size = 0;

//acts as a lock for enter key
volatile static int read_flag = 0; 

fd_ops_t terminal_ops = (fd_ops_t){
    terminal_open,
    terminal_close,
    terminal_read,
    terminal_write
};

term_t* get_term(int8_t term_id){
    return &terminals[(unsigned int)term_id];
}
term_t* get_cur_term(){
    return get_term(cur_term_id);
}

void init_terms(){
    int i;
    int j;
    pte_desc_t vidmap_init_entry;
    //ask TA if we need to call shell on boot or on switch
    for(i = 0; i < 3; i++){
        cur_term_id = i;
        terminals[i].scr_x = 0;
        terminals[i].scr_y = 0;
        terminals[i].curr_size = 0;
        process_array[i] = -1;
    }
    cur_term_id = 0;
    vidmap_init_entry.val = (0xB8000 | 0x7);
    set_ptentry(usr_vidmap_table_desc.addr, cur_term_id, vidmap_init_entry);

    return;
}

void switch_terms(int8_t new_term_id){
    if(new_term_id == cur_term_id) return;

    uint32_t vid_mem = 0xB8000;
    uint32_t* usr_vidmap_table_base = (uint32_t *) usr_vidmap_table_desc.addr;
    term_t* new_term = get_term(new_term_id);
    term_t* cur_term = get_cur_term();

    /* Save current cursor and restore new cursor */
    cur_term->scr_x = get_term_x();
    cur_term->scr_y = get_term_y();
    update_term_xy(new_term->scr_x, new_term->scr_y);

    /* Save and restore term_buffer and curr_size */
    cur_term->curr_size = curr_size;
    curr_size = new_term->curr_size;
    // int i;
    // for(i = 0; i < BUFFER; ++i){
    //     cur_term->term_buffer[i] = term_buffer[i];
    //     term_buffer[i] = new_term->term_buffer[i];
    // }

    /* Save current screen and restore new screen*/
    usr_vidmap_table_base[cur_term_id] = ((uint32_t) cur_term | 0x7);
    flush_tlb();
    memcpy(cur_term->vid_page, (void*) vid_mem, FOUR_KB);

    usr_vidmap_table_base[(unsigned int)new_term_id] = (vid_mem | 0x7);
    flush_tlb();
    memcpy((void*) vid_mem, new_term->vid_page, FOUR_KB);
    
    cur_process = get_pcb((int32_t)(process_array[new_term_id]));

    cur_term_id = new_term_id;
   
    
    // if(process_array[cur_term_id] == -1){
    //     clear();
    //     cli();
    //     sys_execute((uint8_t*)"shell");
    // }
}

/* 
 * fill_buffer
 *   DESCRIPTION: takes an input from the keyboard and fills the terminal buffer  
 *   INPUTS: input_char - takes a single charachter and put it in the terminal buffer
 *   OUTPUTS: update curr_size of terminal buffer
 *   RETURN VALUE: 0 if character was successfully filled into buffer, -1 otherwise
*/
int32_t fill_buffer(char input_char){
    term_t* cur_term = get_cur_term();
    char* term_buffer = cur_term->term_buffer;
    if(input_char == '\n' || input_char == '\e'){
        term_buffer[cur_term->curr_size] = '\n';                          //set to 128 and always just check this location?
        read_flag = 1;                                          //clear buffer in read_terminal not here in case not ready
        //cur_process = get_pcb(process_array[cur_term_id]);
    }
    else if(input_char == '\b'){
        if(cur_term->curr_size != 0){
            if(term_buffer[cur_term->curr_size] != '\n'){                 //cant cancel a read w backspace, incase terminal read is slow
                term_buffer[cur_term->curr_size - 1] = ' ';
                curr_size--;                                    //handle backspace sizing
            } 
        } else{
            return -1; // prevent backspace from flowing into already written parts
        }
    } else if (input_char == '\f'){
        cur_term->curr_size = 0;
    }
    else if(cur_term->curr_size < BUFFER - 1){                            //otherwise fill in the buffer with requested data
        term_buffer[cur_term->curr_size] = input_char;
        cur_term->curr_size++;
    }
    else { 
        return -1;                                              // could not insert character
    }
    return 0;
}

/*
( ͡° ͜ʖ ͡°)
¯\_(ツ)_/¯ 
(ಠ ͜ʖಠ)
ヽ༼ຈل͜ຈ༽ﾉ 
*/

/* 
 * terminal_write
 *   DESCRIPTION: writes from the user buffer to the screen with putc 
 *   INPUTS: fd - the file descriptor, user_buffer - buffer thats passed in to go to screen, bytes - the number of bytes to write
 *   OUTPUTS: modifies the screen with putc
 *   RETURN VALUE: number of bytes wrote
*/
int32_t terminal_write(fd_entry_t* fd_entry, uint8_t* user_buffer, uint32_t bytes){  //doesnt really need locking since only end of buffer can be modified
    int i;
    if(bytes <= 0) return 0;                                    /* If nothing to be written, return immediately */
    // else if(bytes > BUFFER){                                    //overflow checking
    //     bytes = BUFFER;        
    // }
    for(i = 0; i < bytes; i++){
        if(((char*)user_buffer)[i] != '\0'){                    // empty in C
            //terminal_putc(((char*)user_buffer)[i], cur_term_id);                      //place the specific charachter to screen
            putc(((char*)user_buffer)[i]);
        }
    } 
    return bytes;
}

/* 
 * terminal_read
 *   DESCRIPTION: read data from keyboard and put it into the user buffer
 *   INPUTS: fd - the file descriptor, user_buffer - the user buffer to modify, the number of bytes to read
 *   OUTPUTS: modify the user buffer
 *   RETURN VALUE: number of bytes read
*/
int32_t terminal_read(fd_entry_t* fd_entry, uint8_t* user_buffer, uint32_t bytes){
    int i;
    int j;
    int contains_nl = 0;                                        //newline checker, starts at 0

    sti();
    while(read_flag == 0);

    cli();                                                      // disable interrupts 
    term_t* term = get_term(get_curr_pcb()->term_id);
    
    if(bytes <= 0) {                                            /* If nothing to be read, return immediately */
        sti();
        return 0;
    }
    // else if(bytes > BUFFER){                                       //overflow checking
    //     bytes = BUFFER;
    // }

    if (bytes > term->curr_size){
        bytes = term->curr_size; // do not read beyond end pointer of termbuffer
    }
    for(i = 0; i < bytes; i++){
        ((char*)user_buffer)[i] = term->term_buffer[i];       
        if(term->term_buffer[i] == '\n'){                             //look for newline to end buffer
             ((char*)user_buffer)[i] = term->term_buffer[i];
            term->term_buffer[i] = ' ';
            break;
        }
        else{
            ((char*)user_buffer)[i] = term->term_buffer[i];
            term->term_buffer[i] = ' '; 
        }
    }

    for(j = i; j < BUFFER; j++){                                   //fill rest of buffer with empty spaces
        ((char*)user_buffer)[j] = '\0';
    }

    //check for newline and add one at end otherwise
    for(i = 0; i < BUFFER; i++){
        if(((char*)user_buffer)[i] == '\n'){
            contains_nl = 1;                                    //newline detected
            break;
        }
    }
    if(contains_nl == 0){
        ((char*)user_buffer)[BUFFER] = '\n';                       //if no newline detected, add one at the end of the buffer
    }

    term->curr_size = 0;
    read_flag = 0;
    sti();                                                      //enable interrupts 
    
    return bytes; 
}

/* 
 * terminal_close
 *   DESCRIPTION: close the terminal  
 *   INPUTS: fd - the file descriptor 
 *   OUTPUTS: none
 *   RETURN VALUE: return 0
*/
int32_t terminal_close(fd_entry_t* fd_entry) {
    return 0;
}


/* 
 * terminal_open
 *   DESCRIPTION: opens the terminal and cleans the buffer 
 *   INPUTS: fname - the specific terminal to open
 *   OUTPUTS: none
 *   RETURN VALUE: return 0
*/
int32_t terminal_open(fd_entry_t* fd_entry) {
    int i;
    char* proc_term_buffer = get_term(fd_entry->term_id)->term_buffer;
    for(i = 0; i < BUFFER; i++) proc_term_buffer[i] = ' ';              //clean the buffer
    return 0;
}
