#include "x86_desc.h"
#include "handle.h"
#include "intr_link.h"
#include "keyboard.h"
#include "rtc.h"
#include "syscall_handler.h"
#include "syscall_functions.h"


/* Function that sets the IDT entries */
extern void idt_init();
