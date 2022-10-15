#include "handle.h"
#include "lib.h"

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
}