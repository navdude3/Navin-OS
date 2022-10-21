#include "terminal.h"

static char term_buffer[BUFFER];

static int curr_size = 0;

static int read_flag = 0; //volatile?


void fill_buffer(char input_char){
    if(input_char == '\n'){
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
//need to communicate with outside buffer
void terminal_write(void* user_buffer){  //doesnt really need locking since only end of buffer can be modified
    int i;
    for(i = 0; i < curr_size; i++){
        if(((char*)user_buffer)[i] != '\0'){ // empty in C
            putc(((char*)user_buffer)[i]);
        }
    }
}

void terminal_read(void* user_buffer){
    cli(); // dont mess with buffer ptr
    int i;
    while(read_flag == 0){
        //spin here
    }

    for(i = 0; i < 128; i++){
        ((char*)user_buffer)[i] = term_buffer[i];
        if(term_buffer[i] == '\n'){
            term_buffer[i] = ' ';
            break;
        }
        else{
            ((char*)user_buffer)[i] = term_buffer[i];
            term_buffer[i] = ' '; 
        }
    }
    curr_size = 0;
    sti();
    
    read_flag = 0;
}
