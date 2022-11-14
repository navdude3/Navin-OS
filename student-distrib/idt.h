#include "x86_desc.h"
#include "handle.h"
#include "intr_link.h"
#include "keyboard.h"
#include "rtc.h"
#include "syscall_handler.h"
#include "syscall_functions.h"

/* Start of PIC Entries */
#define START_PIC_VEC 0x20  

/* Function that sets the IDT entries */
extern void idt_init();
