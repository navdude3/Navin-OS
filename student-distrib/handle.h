//first 20 are IDT initialized
extern void divide_error();
extern void RESERVED_1();
extern void nmi_int();
extern void breakpoint();
extern void overflow();
extern void bound_range();
extern void invalid_opcode();
extern void device_not_available();
extern void double_fault();
extern void segment_overrun();
extern void invalid_tss();
extern void seg_not_present(); 
extern void seg_fault();
extern void general_protection();
extern void page_fault();
extern void RESERVED_15();
extern void floating_point_error();
extern void alignment_check();
extern void machine_check();
extern void simd_floating_point_exception();


//handlers for specific interactions
extern void keyboard_link_handler();
