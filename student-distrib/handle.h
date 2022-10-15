#define OPTION_SIZE 36

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

// from https://wiki.osdev.org/PS2_Keyboard && https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html 
static char let_num [OPTION_SIZE] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
static char scancode_pressed[OPTION_SIZE] = {0x0B, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x1E, 0x30, 0x2E, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24, 0x25, 0x26, 0x32, 0x31, 0x18, 0x19, 0x10, 0x13, 0x1F, 0x14, 0x16, 0x2F, 0x11, 0x2D, 0x15, 0x2C};
//static char scancode_released[OPTION_SIZE] = [0x8B, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x9E, 0xB0, 0xAE, 0xA0, 0x92, 0xA1, 0xA2, 0xA3, 0x97, 0xA4, 0xA5, 0xA6, 0xB2, 0xB1, 0x98, 0x99, 0x90, 0x93, 0x9F, 0x94, 0x96, 0xAF, 0x91, 0xAD, 0x95, 0xAC];