#include "handle.h"
#include "lib.h"
#include "i8259.h"
#include "rtc.h"

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
