#define ASM     1

.global keyboard_linkage, rtc_linkage, pit_linkage
/* Assembly linkage for keyboard and RTC */

/* 
 * keyboard_linkage
 *   DESCRIPTION: assembly linkage for keyboard_link_handler
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
keyboard_linkage:
    pushal                              /* Pushes all registers */
    pushfl                              /* Pushes all flags */
    call keyboard_link_handler
    popfl
    popal
    iret

/* 
 * rtc_linkage
 *   DESCRIPTION: assembly linkage for rtc_link_handler
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
rtc_linkage:
    pushal                              /* Pushes all registers */
    pushfl                              /* Pushes all flags */
    call rtc_link_handler
    popfl
    popal
    iret

/* 
 * pit_linkage
 *   DESCRIPTION: assembly linkage for pit_link_handler
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 */
pit_linkage:
    pushal                              /* Pushes all registers */
    pushfl
    call pit_link_handler
    popfl
    popal
    iret
