#include "lib.h"
#include "i8259.h"


#define REG_B_DATA  0x8B;
#define REG_A_DATA  0x8A;
#define RW_PORT     0x71;
#define REG_PORT    0x70;
extern void rtc_init();