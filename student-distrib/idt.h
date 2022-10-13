#include "x86_desc.h"
#include "handle.h"

extern void idt_init();
extern void handle_exception(int id);


const char interrupt_names[20];
interrupt_names[0] = "divide_error";
interrupt_names[1] = "RESERVED_1";
interrupt_names[2] = "nmi_int";
interrupt_names[3] = "breakpoint";
interrupt_names[4] = "overflow";
interrupt_names[5] = "bound_range";
interrupt_names[6] = "invalid_opcode";
interrupt_names[7] = "device_not_available";
interrupt_names[8] = "double_fault";
interrupt_names[9] = "segment_overrun";
interrupt_names[10] = "invalid_tss";
interrupt_names[11] = "seg_not_present"; 
interrupt_names[12] = "seg_fault";
interrupt_names[13] = "general_protection";
interrupt_names[14] = "page_fault";
interrupt_names[15] = "RESERVED_15";
interrupt_names[16] = "floating_point_error";
interrupt_names[17] = "alignment_check";
interrupt_names[18] = "machine_check";
interrupt_names[19] = "simd_floating_point_exception";
