#include "handle.h"
#include "lib.h"
#include "i8259.h"

static char let_num [OPTION_SIZE] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
static char scancode_pressed[OPTION_SIZE] = {0x0B, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x1E, 0x30, 0x2E, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 0x26, 0x32, 0x31, 0x18, 0x19, 0x10, 0x13, 0x1F, 0x14, 0x16, 0x2F, 0x11, 0x2D, 0x15, 0x2C};

static int rtc_alt = 0;
static int rtc_count = 0;

void divide_error(void){
    clear();
    printf(" divide error :(");
    while(1){
        
    }
}

void RESERVED_1(void){
    clear();
    printf(" reserved 1 :(");
    while(1){
        continue;
    }
}

void nmi_int(void){
    clear();
    printf(" nmi error :(");
    while(1){
        continue;
    }
}

void breakpoint(void){
    clear();
    printf(" breakpoint error:(");
    while(1){
        continue;
    }
}

void overflow(void){
    clear();
    printf(" overflow error:(");
    while(1){
        continue;
    }
}

void bound_range(void){
    clear();
    printf(" bound_range error :(");
    while(1){
        continue;
    }
}

void invalid_opcode(void){
    clear();
    printf(" invalid_opcode error :(");
    while(1){
        continue;
    }
}

void device_not_available(void){
    clear();
    printf(" device_not_available error :(");
    while(1){
        continue;
    }
}

void double_fault(void){
    clear();
    printf(" double_fault error :(");
    while(1){
        continue;
    }
}

void segment_overrun(void){
    clear();
    printf(" segment_overrun error :(");
    while(1){
        continue;
    }
}

void invalid_tss(void){
    clear();
    printf(" invalid_tss error :(");
    while(1){
        continue;
    }
}

void seg_not_present(void){
    clear();
    printf(" seg_not_present error :(");
    while(1){
        continue;
    }
}

void seg_fault(void){
    clear();
    printf(" seg_fault error :(");
    while(1){
        continue;
    }
}

void general_protection(void){
    clear();
    printf(" general_protection error :(");
    while(1){
        continue;
    }
}
void page_fault(void){
    clear();
    printf(" page_fault error :(");
    while(1){
        continue;
    }
}

void floating_point_error(void){
    clear();
    printf(" floating_point_error error :(");
    while(1){
        continue;
    }
}

void alignment_check(void){
    clear();
    printf(" alignment_check error :(");
    while(1){
        continue;
    }
}

void machine_check(void){
    clear();
    printf(" machine_check error :(");
    while(1){
        continue;
    }
}
void simd_floating_point_exception(void){
    clear();
    printf(" simd_floating_point_exception error :(");
    while(1){
        continue;
    }
}


void keyboard_link_handler(){
    int key = inb(0x60);
    int i;
    char output = 64;
    for(i = 0; i < OPTION_SIZE; i++) {
        if(key == scancode_pressed[i]) {
            output = let_num[i];
        }
    }
    send_eoi(1);
    if(output != 64) putc(output);
    
    else return;
}


void rtc_link_handler(){
    cli();

    rtc_count = rtc_count + 1;
    if(rtc_count == 1024 && rtc_alt == 0){
        printf("1");
        rtc_count = 0;
        rtc_alt = 1;
    }
    else if(rtc_count == 1024 && rtc_alt == 1){
        printf("2");
        rtc_count = 0;
        rtc_alt = 0;
    }
    
    outb(0x8C, 0x70);	// select register C
    inb(0x71);		    // just throw away contents
    send_eoi(8);
    sti();
}

