#include "terminal.h"

//make the terminal buffer
static char term_buffer[BUFFER];

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

/* 
 * fill_buffer
 *   DESCRIPTION: takes an input from the keyboard and fills the terminal buffer  
 *   INPUTS: input_char - takes a single charachter and put it in the terminal buffer
 *   OUTPUTS: update curr_size of terminal buffer
 *   RETURN VALUE: none
*/
void fill_buffer(char input_char){
    if(input_char == '\n' || input_char == '\e'){
        term_buffer[curr_size] = '\n';           //set to 128 and always just check this location?
        read_flag = 1;          //clear buffer in read_terminal not here in case not ready
    }
    else if(input_char == '\b'){
        if(curr_size != 0){
            if(term_buffer[curr_size] != '\n'){   //cant cancel a read w backspace, incase terminal read is slow
                term_buffer[curr_size - 1] = ' ';
                curr_size--;                        //handle backspace sizing
            } 
        }
    }
    else if(curr_size < BUFFER - 1){                //otherwise fill in the buffer with requested data
        term_buffer[curr_size] = input_char;
        curr_size++;
    }
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
int32_t terminal_write(uint32_t fd, uint8_t* user_buffer, uint32_t bytes){  //doesnt really need locking since only end of buffer can be modified
    int i;
    if(bytes <= 0) return 0;                  /* If nothing to be written, return immediately */
    else if(bytes > 128){                       //overflow checking
        bytes = 128;        
    }
    for(i = 0; i < bytes; i++){
        if(((char*)user_buffer)[i] != '\0'){ // empty in C
            putc(((char*)user_buffer)[i]);   //place the specific charachter to screen
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
int32_t terminal_read(uint32_t fd, uint8_t* user_buffer, uint32_t bytes){
    int i;
    int j;
    int contains_nl = 0;        //newline checker, starts at 0

    while(read_flag == 0);

    cli();                      //allow interrupts
    if(bytes <= 0) {                         /* If nothing to be read, return immediately */
        sti();
        return 0;
    }
    else if(bytes > 128){           //overflow checking
        bytes = 128;
    }
    for(i = 0; i < bytes; i++){
        ((char*)user_buffer)[i] = term_buffer[i];       
        if(term_buffer[i] == '\n'){                         //look for newline to end buffer
             ((char*)user_buffer)[i] = term_buffer[i];
            term_buffer[i] = ' ';
            break;
        }
        else{
            ((char*)user_buffer)[i] = term_buffer[i];
            term_buffer[i] = ' '; 
        }
    }

    for(j = i; j < 128; j++){                       //fill rest of buffer with empty spaces
        ((char*)user_buffer)[j] = ' ';
    }

    //check for newline and add one at end otherwise
    for(i = 0; i < 128; i++){
        if(((char*)user_buffer)[i] == '\n'){
            contains_nl = 1;                    //newline detected
            break;
        }
    }
    if(contains_nl == 0){
        ((char*)user_buffer)[128] = '\n';       //if no newline detected, add one at the end of the buffer
    }

    curr_size = 0;
    read_flag = 0;
    sti();                                  //disable interrupts
    
    return bytes; 
}

/* 
 * terminal_close
 *   DESCRIPTION: close the terminal  
 *   INPUTS: fd - the file descriptor 
 *   OUTPUTS: none
 *   RETURN VALUE: return 0
*/
int32_t terminal_close(uint32_t fd) {
    return 0;
}


/* 
 * terminal_open
 *   DESCRIPTION: opens the terminal and cleans the buffer 
 *   INPUTS: fname - the specific terminal to open
 *   OUTPUTS: none
 *   RETURN VALUE: return 0
*/
int32_t terminal_open(const uint8_t* fname) {
    int i;
    clear();
    for (i = 0; i < BUFFER; i++) term_buffer[i] = ' ';              //clean the buffer
    return 0;
}

