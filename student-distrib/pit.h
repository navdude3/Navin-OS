#include "lib.h"
#include "types.h"
#include "x86_desc.h"
#include "loader.h"
#include "terminal.h"
#include "i8259.h"

/* From https://wiki.osdev.org/Programmable_Interval_Timer & http://www.osdever.net/bkerndev/Docs/pit.htm */

#define CHANNEL     0x40        /* Connected directly to IRQ0, can be used to generate infinite series of timer ticks at set frequency */
#define PIT_IRQ      0          /* PIT IRQ number on PIC */
#define MODE_REG    0x43        /* Mode/Command register */
#define MODE        0x34        /* 0011 0100: 00 - channel 0, 11 - low/high bytes , 010 - mode 2 (rate generator) */   
#define FREQ        11932       /* Frequency for rate generator (1193182)/100 to interrupt every 10ms */

void pit_init();
void pit_link_handler();
