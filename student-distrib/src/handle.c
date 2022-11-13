#include "handle.h"
#include "lib.h"
#include "i8259.h"
#include "rtc.h"
#include "syscall_functions.h"

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
    printf("divide error :(\n");
    sys_halt(-1);
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
    printf("reserved 1 :(\n");
    sys_halt(-1);
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
    printf("nmi error :(\n");
    sys_halt(-1);
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
    printf("breakpoint error:(\n");
    sys_halt(-1);
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
    printf("overflow error:(\n");
    sys_halt(-1);
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
    printf("bound_range error :(\n");
    sys_halt(-1);
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
    printf("invalid_opcode error :(\n");
    sys_halt(-1);
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
    printf("device_not_available error :(\n");
    sys_halt(-1);
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
    printf("double_fault error :(\n");
    sys_halt(-1);
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
    printf("segment_overrun error :(\n");
    sys_halt(-1);
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
    printf("invalid_tss error :(\n");
    sys_halt(-1);
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
    printf("seg_not_present error :(\n");
    sys_halt(-1);
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
    printf("seg_fault error :(\n");
    sys_halt(-1);
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
    printf("general_protection error :(\n");
    sys_halt(-1);
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
    printf("page_fault error :(\n");
    sys_halt(-1);
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
    printf("floating_point_error error :(\n");
    sys_halt(-1);
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
    printf("alignment_check error :(\n");
    sys_halt(-1);
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
    printf("machine_check error :(\n");
    sys_halt(-1);
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
    printf("simd_floating_point_exception error :(\n");
    sys_halt(-1);
}
