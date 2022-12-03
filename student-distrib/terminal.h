#ifndef TERMINAL_H
#define TERMINAL_H
#include "vfs.h"
#include "paging.h"

/* Size of text buffer */
#define BUFFER      128
#define FOUR_KB     4096
#define MAX_TERMS   3
#define USR_PRESENT_RW 0x7

typedef struct term{
    char vid_page[FOUR_KB]; // Video memory is 80*25*2 = 4000. This allows us to save pages when aligning
    int  curr_size;
    char term_buffer[BUFFER];
    int scr_x; int scr_y;
    
}__attribute__((aligned (4096))) term_t;

int cur_term_id;
term_t terminals[MAX_TERMS];
term_t* get_cur_term();
term_t* get_term(int8_t term_id);
extern void init_terms();
extern void switch_terms(int8_t new_term_id);
/* Functions for the terminal driver */
extern int32_t terminal_write(fd_entry_t* fd_entry, uint8_t* user_buffer, uint32_t bytes);
extern int32_t terminal_read(fd_entry_t* fd_entry, uint8_t* user_buffer, uint32_t bytes);
extern int32_t terminal_close(fd_entry_t* fd_entry);
extern int32_t terminal_open(fd_entry_t* fd_entry);

extern int32_t fill_buffer(char input_char);

/* FD related declarations*/
extern fd_ops_t terminal_ops;
#endif
