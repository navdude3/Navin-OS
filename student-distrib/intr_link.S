#define ASM     1

.global keyboard_linkage, rtc_linkage, pit_linkage
/* Assembly linkage for keyboard and RTC */

keyboard_linkage:
    pushal                              /* Pushes all registers */
    pushfl                              /* Pushes all flags */
    call keyboard_link_handler
    popfl
    popal
    iret


rtc_linkage:
    pushal                              /* Pushes all registers */
    pushfl                              /* Pushes all flags */
    call rtc_link_handler
    popfl
    popal
    iret

pit_linkage:
    pushal                              /* Pushes all registers */
    pushfl                              /* Pushes all flags */
    call pit_handler
    popfl
    popal
    iret
