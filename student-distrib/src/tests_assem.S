
.globl hellothere_test

hellothere_test:
    movl $2, %eax
    int $0x80 
    andl $0x000000F, %eax
    addl $48, %eax                  # convert number to its ASCII
    pushl %eax
    call putc
    addl $4, %esp
    ret
