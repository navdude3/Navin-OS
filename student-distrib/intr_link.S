#define ASM     1

.global keyboard_linkage, rtc_linkage

keyboard_linkage:
    pushal
    pushfl
    call keyboard_link_handler
    popfl
    popal
    iret


rtc_linkage:
    pushal
    pushfl
    call rtc_link_handler
    popfl
    popal
    iret




