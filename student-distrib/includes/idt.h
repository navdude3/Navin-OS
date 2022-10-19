#include "x86_desc.h"
#include "handle.h"
#include "intr_link.h""

#define START_PIC_VEC 0x20 // start of PIC entries
/* Function that sets the IDT entries */
extern void idt_init();
