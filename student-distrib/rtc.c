#include "rtc.h"
#include "process.h"

/* info from wiki.osdev.org/RTC */
 static int flag[3];
//static int rtc_int_count = 512;
static int freqs[MAX_PROCESS + 1]; // 0-5 for processes, last one for scheduler

fd_ops_t rtc_ops = (fd_ops_t){
    rtc_open,
    rtc_close,
    rtc_read,
    rtc_write
};

/* 
 * rtc_init
 *  DESCRIPTION: Turns on IRQ 8 with default 1024 Hz Rate
 *  INPUTS: none
 *  OUTPUTS: none
 *  RETURN VALUE: none
 *  SIDE EFFECTS: Enables interrupts on IRQ 8
 */
void rtc_init(){
    disable_irq(RTC_IRQ);                            // Disable interrupts on IRQ 8
    outb(REG_B_DATA, REG_PORT);                      // Disable NMI 
    char prev = inb(RW_PORT);                        // Read the current val of register B
    outb(REG_B_DATA, REG_PORT);                      // Reset index
    outb(prev | 0x40, RW_PORT);                      // Turn on bit 6 of reg B by OR-ing previous with 0x01000000  

    outb(REG_A_DATA, REG_PORT);                      // Disable NMI 
    prev = inb(RW_PORT);                             // Read the current val of register A
    outb(REG_A_DATA, REG_PORT);                      // Reset index
    outb((prev & 0xF0) | 0xF, RW_PORT);   
    
    enable_irq(RTC_IRQ);                             // Enable interrupts on IRQ 8
    
    int i;
    for(i = 0; i < 7; ++i) freqs[i] = MAX_FREQ/MIN_FREQ;
    
    set_rate(MAX_FREQ);  
}

/* 
 * rtc_open
 *  DESCRIPTION: initializes the RTC device and its frequency
 *  INPUTS: 8 bit file name 
 *  OUTPUTS: 0
 *  RETURN VALUE: 0
 *  SIDE EFFECTS: none
 */
int32_t rtc_open(fd_entry_t* fd_entry){
    fd_entry->rtc_freq = MAX_FREQ / MIN_FREQ;                      
    return 0;
}

/* 
 * rtc_close
 *  DESCRIPTION: closes RTC device
 *  INPUTS: fd file descriptor number
 *  OUTPUTS: none
 *  RETURN VALUE: 0
 *  SIDE EFFECTS: none
 */
int32_t rtc_close(fd_entry_t* fd_entry){
    return 0;                                       // function does nothing but return 0
}

/* 
 * rtc_read
 *  DESCRIPTION: blocks until next interrupt 
 *  INPUTS: int fd, void* buf, int nbytes)
 *  OUTPUTS: none
 *  RETURN VALUE: 0
 *  SIDE EFFECTS: Enables interrupts on IRQ 8
 */
int32_t rtc_read(fd_entry_t* fd_entry, uint8_t* buf, uint32_t length){
    flag[cur_term_id] = 1;
    int32_t interval = MAX_FREQ / fd_entry->rtc_freq;
    sti();
    while(flag[cur_term_id]);               // infinite loop until rtc_count is a multiple of desired interval
    return 0;
}

/* 
 * rtc_write
 *  DESCRIPTION: writes data to the RTC device
 *  INPUTS: file descriptor number, buf number, and number of bytes
 *  OUTPUTS: rtc int count updated with new frequency
 *  RETURN VALUE: -1 or 0
 *  SIDE EFFECTS: frequency must be power of 2 and between 2 and 1024
 */
int32_t rtc_write(fd_entry_t* fd_entry, uint8_t* buf, uint32_t length){
    if((buf == NULL ) || (length != sizeof(uint32_t))) return -1;
    
    int freq = *(int *) buf;                // set the frequency based on the buffer number
    int temp = freq;

    if(freq < MIN_FREQ || freq > MAX_FREQ) return -1;
    
    while(temp != 1) {
        if(temp % MIN_FREQ == 1) return -1;
        temp = temp / MIN_FREQ;             // check to make sure function is a power of 2
    }                                
    
    // rtc_int_count = MAX_FREQ / freq;        // update the interrupt count
    // freqs[cur_process->pid] = freq;

    fd_entry->rtc_freq = freq;
    set_rate(fd_entry->rtc_freq);                         // set the rtc_rate to the new frequency
    
    return 0;
}

/* 
 * set_rate
 *  DESCRIPTION: sets the RTC rate based on the passed in frequency
 *  INPUTS: frequency
 *  OUTPUTS: rtc_rate written to RW port
 *  RETURN VALUE: none
 *  SIDE EFFECTS: rtc_rate updated
 */
void set_rate(uint32_t freq){
    int rtc_rate;

    switch (freq){
        case 2:     rtc_rate = 0x0F;
                    break;
        case 4:     rtc_rate = 0x0E;
                    break;
        case 8:     rtc_rate = 0x0D;
                    break;
        case 16:    rtc_rate = 0x0C;
                    break;
        case 32:    rtc_rate = 0x0B;
                    break;
        case 64:    rtc_rate = 0x0A;
                    break;
        case 128:   rtc_rate = 0x09;
                    break;
        case 256:   rtc_rate = 0x08;
                    break;
        case 512:   rtc_rate = 0x07;
                    break;
        case 1024:  rtc_rate = 0x06; 
                    break;  
        default:    return;      // switch case to set rate based on frequency (frequency =  32768 >> (rate-1))
    }

    outb(REG_A_DATA, REG_PORT);                      // Set index to register A
    char prev = inb(RW_PORT);                        // Read the current val of register A
    outb(REG_A_DATA, REG_PORT);                      // Reset index
    outb((prev & 0xF0) | rtc_rate, RW_PORT);         // Write our rate to bottom four bits
}


/* 
 * rtc_link_handler
 *   DESCRIPTION: prints either a 1 or a 2 to the screen every time the interrupt is called every 1024 times
 *   INPUTS: none
 *   OUTPUTS: print a 1 or a 2 to the screen
 *   RETURN VALUE: void
*/
void rtc_link_handler(){
    cli();
    ++rtc_count;              //count to static variable every interrupt call
    flag[cur_term_id] = 0;
    outb(0x8C, 0x70);	                    // choose register C
    inb(0x71);		                        // remove contents
    send_eoi(8);                            // send eoi to irq 8, (slave pic irq 0)
    sti();
}
