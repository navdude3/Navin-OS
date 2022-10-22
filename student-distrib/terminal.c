#include "terminal.h"

static char term_buffer[BUFFER];

static int curr_size = 0;

volatile static int read_flag = 0; //volatile?


void fill_buffer(char input_char){
    if(input_char == '\n' || input_char == '\e'){
        term_buffer[curr_size] = '\n';           //set to 128 and always just check this location?
        read_flag = 1;          //clear buffer in read_terminal not here in case not ready
    }
    else if(input_char == '\b'){
        if(curr_size != 0){
            if(term_buffer[curr_size] != '\n'){   //cant cancel a read w backspace, incase terminal read is slow
                term_buffer[curr_size - 1] = ' ';
                curr_size--;
            } 
        }
    }
    else if(curr_size < BUFFER - 1){
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

//need to communicate with outside buffer
int32_t terminal_write(uint32_t fd, void* user_buffer, int32_t bytes){  //doesnt really need locking since only end of buffer can be modified
    int i;
    if(bytes <= 0) return 0;                  /* If nothing to be written, return immediately */
    else if(bytes > 128){
        bytes = 128;
    }
    for(i = 0; i < bytes; i++){
        if(((char*)user_buffer)[i] != '\0'){ // empty in C
            putc(((char*)user_buffer)[i]);
        }
    } 
    return bytes;
}

int32_t terminal_read(uint32_t fd, void* user_buffer, int32_t bytes){
    int i;
    int j;
    int contains_nl = 0; 

    while(read_flag == 0);

    cli(); 
    if(bytes <= 0) {                         /* If nothing to be read, return immediately */
        sti();
        return 0;
    }
    else if(bytes > 128){
        bytes = 128;
    }
    for(i = 0; i < bytes; i++){
        ((char*)user_buffer)[i] = term_buffer[i];
        if(term_buffer[i] == '\n'){
             ((char*)user_buffer)[i] = term_buffer[i];
            term_buffer[i] = ' ';
            break;
        }
        else{
            ((char*)user_buffer)[i] = term_buffer[i];
            term_buffer[i] = ' '; 
        }
    }

    for(j = i; j < 128; j++){
        ((char*)user_buffer)[j] = ' ';
    }

    //check for newline and add one at end otherwise
    for(i = 0; i < 128; i++){
        if(((char*)user_buffer)[i] == '\n'){
            contains_nl = 1;
            break;
        }
    }
    if(contains_nl == 0){
        ((char*)user_buffer)[128] = '\n';
    }

    curr_size = 0;
    read_flag = 0;
    sti();
    
    return bytes; 
}

int32_t terminal_close(uint32_t fd) {
    return 0;
}

int32_t terminal_open(const uint8_t fname) {
    int i;
    clear();
    for (i = 0; i < BUFFER; i++) term_buffer[i] = ' ';
    return 0;
}

