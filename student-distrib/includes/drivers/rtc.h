#include "lib.h"
#include "i8259.h"

/* Ports that RTC utilizes for registers A/B and to read/write */
#define REG_A_DATA  0x8A
#define REG_B_DATA  0x8B
#define RW_PORT     0x71
#define REG_PORT    0x70
/* RTC is on IRQ 8 -> IRQ 0 on secondary PIC */
#define RTC_IRQ     8


/* Used to measure tics since start of cycle */
volatile int rtc_count;
/* Initializes the RTC */
extern void rtc_init();
