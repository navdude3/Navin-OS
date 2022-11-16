#include "lib.h"
#include "i8259.h"
#include "vfs.h"

/* Ports that RTC utilizes for registers A/B and to read/write */
#define REG_A_DATA  0x8A
#define REG_B_DATA  0x8B
#define RW_PORT     0x71
#define REG_PORT    0x70
/* RTC is on IRQ 8 -> IRQ 0 on secondary PIC */
#define RTC_IRQ     8

#define MAX_RATE    15
#define MIN_RATE    2
#define MAX_FREQ    1024
#define MIN_FREQ    2

//used to measure tics since start of cycle
volatile int rtc_count;

/* RTC Fucntions */
void    rtc_init();
int32_t rtc_open(fd_entry_t* fd_entry);
int32_t rtc_close(fd_entry_t* fd_entry);
int32_t rtc_read(fd_entry_t* fd_entry, uint8_t* buf, uint32_t length);
int32_t rtc_write(fd_entry_t* fd_entry, uint8_t* buf, uint32_t length);
void rtc_link_handler();
void set_rate(uint32_t freq);

extern fd_ops_t rtc_ops;
