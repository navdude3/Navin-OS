#include "handle.h"
#include "lib.h"

void divide_error(void){
    printf("divide error :(");
    while(1){
        continue;
    }
}

void nmi_int(void){
    printf("nmi error :(");
    while(1){
        continue;
    }
}

void breakpoint(void){
    printf("breakpoint error:(");
    while(1){
        continue;
    }
}

void overflow(void){
    printf("overflow error:(");
    while(1){
        continue;
    }
}

void bound_range(void){
    printf("bound_range error :(");
    while(1){
        continue;
    }
}

void invalid_opcode(void){
    printf("invalid_opcode error :(");
    while(1){
        continue;
    }
}

void device_not_available(void){
    printf("device_not_available error :(");
    while(1){
        continue;
    }
}

void double_fault(void){
    printf("double_fault error :(");
    while(1){
        continue;
    }
}

void segment_overrun(void){
    printf("segment_overrun error :(");
    while(1){
        continue;
    }
}

void invalid_tss(void){
    printf("invalid_tss error :(");
    while(1){
        continue;
    }
}

void seg_not_present(void){
    printf("seg_not_present error :(");
    while(1){
        continue;
    }
}

void seg_fault(void){
    printf("seg_fault error :(");
    while(1){
        continue;
    }
}

void general_protection(void){
    printf("general_protection error :(");
    while(1){
        continue;
    }
}
void page_fault(void){
    printf("page_fault error :(");
    while(1){
        continue;
    }
}

void floating_point_error(void){
    printf("floating_point_error error :(");
    while(1){
        continue;
    }
}

void alignment_check(void){
    printf("alignment_check error :(");
    while(1){
        continue;
    }
}

void machine_check(void){
    printf("machine_check error :(");
    while(1){
        continue;
    }
}
void simd_floating_point_exception(void){
    printf("simd_floating_point_exception error :(");
    while(1){
        continue;
    }
}


void keyboard_link_handler(){
    int key = inb(0x60);
}