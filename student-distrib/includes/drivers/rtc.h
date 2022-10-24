#include "lib.h"
#include "i8259.h"

// DONE
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
/* Initializes the RTC */
void    rtc_init();
int32_t rtc_open(const uint8_t* filename);
int32_t rtc_close(int fd);
int32_t rtc_read(int fd, void* buf, int nbytes);
int32_t rtc_write(int fd, const void* buf, int nbytes);
void rtc_link_handler();
void set_rate(int freq);


/* Once virtualized, must look like this */
// extern int32_t open (const uint8_t* filename);
// extern int32_t close (int32_t fd);
// extern int32_t read (int32_t fd, void* buf, int32_t nbytes);
// extern int32_t write (int32_t fd, const void* buf, int32_t nbytes);
