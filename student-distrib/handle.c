#include "handle.h"
#include "lib.h"
#include "i8259.h"
#include "rtc.h"

//done

/* array that holds the character corresponding to each scancode */
static char let_num [OPTION_SIZE] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
/* scancode info from https://wiki.osdev.org/PS2_Keyboard && https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html */
/* array that holds the keyboard scancodes for 0-9 and a-z */
static char scancode_pressed[OPTION_SIZE] = {0x0B, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x1E, 0x30, 0x2E, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 0x26, 0x32, 0x31, 0x18, 0x19, 0x10, 0x13, 0x1F, 0x14, 0x16, 0x2F, 0x11, 0x2D, 0x15, 0x2C};





/* 
 * divide_error
 *   DESCRIPTION: blue screen of death on division by 0
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void divide_error(void){
    cli();
    clear();
    printf(" divide error :(");
    while(1){
        
    }
}


/* 
 * RESERVED_1
 *   DESCRIPTION: blue screen of death on the intel reserved 1 
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void RESERVED_1(void){
    cli();
    clear();
    printf(" reserved 1 :(");
    while(1){
        continue;
    }
}

/* 
 * nmi_int
 *   DESCRIPTION: blue screen of death on nmi interrupt
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void nmi_int(void){
    cli();
    clear();
    printf(" nmi error :(");
    while(1){
        continue;
    }
}

/* 
 * breakpoint
 *   DESCRIPTION: blue screen of death on breakpoint exception
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void breakpoint(void){
    cli();
    clear();
    printf(" breakpoint error:(");
    while(1){
        continue;
    }
}

/* 
 * overflow
 *   DESCRIPTION: blue screen of death on overflow error
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void overflow(void){
    cli();
    clear();
    printf(" overflow error:(");
    while(1){
        continue;
    }
}

/* 
 * bound_range
 *   DESCRIPTION: blue screen of death on bound range error
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void bound_range(void){
    cli();
    clear();
    printf(" bound_range error :(");
    while(1){
        continue;
    }
}


/* 
 * invalid_opcode
 *   DESCRIPTION: blue screen of death on an invalid opcode
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void invalid_opcode(void){
    cli();
    clear();
    printf(" invalid_opcode error :(");
    while(1){
        continue;
    }
}

/* 
 * device_not_available
 *   DESCRIPTION: blue screen of death when device is not available
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void device_not_available(void){
    cli();
    clear();
    printf(" device_not_available error :(");
    while(1){
        continue;
    }
}

/* 
 * double_fault
 *   DESCRIPTION: blue screen of death on double fault error
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void double_fault(void){
    cli();
    clear();
    printf(" double_fault error :(");
    while(1){
        continue;
    }
}


/* 
 * segment_overrun
 *   DESCRIPTION: blue screen of death on segment overrun
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void segment_overrun(void){
    cli();
    clear();
    printf(" segment_overrun error :(");
    while(1){
        continue;
    }
}


/* 
 * invalid_tss
 *   DESCRIPTION: blue screen of death on invalid tss
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void invalid_tss(void){
    cli();
    clear();
    printf(" invalid_tss error :(");
    while(1){
        continue;
    }
}

/* 
 * seg_not_present
 *   DESCRIPTION: blue screen of death on non present segment
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void seg_not_present(void){
    cli();
    clear();
    printf(" seg_not_present error :(");
    while(1){
        continue;
    }
}


/* 
 * seg_fault
 *   DESCRIPTION: blue screen of death on a segmentation fault
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void seg_fault(void){
    cli();
    clear();
    printf(" seg_fault error :(");
    while(1){
        continue;
    }
}

/* 
 * general_protection
 *   DESCRIPTION: blue screen of death on general protection error
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void general_protection(void){
    cli();
    clear();
    printf(" general_protection error :(");
    while(1){
        continue;
    }
}

/* 
 * page_fault
 *   DESCRIPTION: blue screen of death on page fault error
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void page_fault(void){
    cli();
    clear();
    printf(" page_fault error :(");
    while(1){
        continue;
    }
}


/* 
 * floating_point_error
 *   DESCRIPTION: blue screen of death on a floating point error
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void floating_point_error(void){
    cli();
    clear();
    printf(" floating_point_error error :(");
    while(1){
        continue;
    }
}


/* 
 * alignment_check
 *   DESCRIPTION: blue screen of death on an alignment check error
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void alignment_check(void){
    cli();
    clear();
    printf(" alignment_check error :(");
    while(1){
        continue;
    }
}


/* 
 * machine_check
 *   DESCRIPTION: blue screen of death on machine check error
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void machine_check(void){
    cli();
    clear();
    printf(" machine_check error :(");
    while(1){
        continue;
    }
}

/* 
 * simd_floating_point_exception
 *   DESCRIPTION: blue screen of death on floating_point_exception
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: never returns, infinite loop
*/
void simd_floating_point_exception(void){
    cli();
    clear();
    printf(" simd_floating_point_exception error :(");
    while(1){
        continue;
    }
}



/* 
 * keyboard_link_handler
 *   DESCRIPTION: print the value to screen based on the scancode, send an EOI to master pic irq 1 
 *   INPUTS: none
 *   OUTPUTS: print keyboard character to screen
 *   RETURN VALUE: void
*/
void keyboard_link_handler(){
    int key = inb(0x60);             //port for keyboard
    int i;
    char output = 64;               //initial value to check if lowercase or number
    for(i = 0; i < OPTION_SIZE; i++) {
        if(key == scancode_pressed[i]) {
            output = let_num[i];
        }
    }
    send_eoi(1);                    //send EOI to primary pic 1
    if(output != 64) putc(output);      //if the value was detected as a valid key print it
    
    else return;
}

/* 
 * rtc_link_handler
 *   DESCRIPTION: prints either a 1 or a 2 to the screen every time the interrupt is called every 1024 times
 *   INPUTS: none
 *   OUTPUTS: print a 1 or a 2 to the screen
 *   RETURN VALUE: void
*/
void rtc_link_handler(){
    cli();
    //used as an alternating flag to show which value should be displayed next
    static int rtc_alt = 0;
    // test_interrupts();
    rtc_count = rtc_count + 1;              //count to static variable every interrupt call
    if(rtc_count % 1024 == 0 && rtc_alt == 0){  //cond to print 1
        printf("1");
        
        rtc_alt = 1;
    }
    else if(rtc_count % 1024 == 0 && rtc_alt == 1){ //cond to print 2
        printf("2");

        rtc_alt = 0;
    }
    
    outb(0x8C, 0x70);	// choose register C
    inb(0x71);		    // remove contents
    send_eoi(8);        //send eoi to irq 8, (slave pic irq 0)
    sti();
}

