#define ASM     1
.globl syscall_linkage

error_seen:    .long 0x0  

jumptable:
    .long 0x0                   # this is a placeholder so a sys call w eax 1 can call halt 
    .long sys_halt              # put c function here 
    .long sys_execute
    .long sys_read
    .long sys_write
    .long sys_open
    .long sys_close
    .long sys_getargs
    .long sys_vidmap
    .long sys_set_handler
    .long sys_sigreturn



syscall_linkage:
    cmpl $10, %eax
    ja oob_error
    cmpl $1, %eax
    jb oob_error

    pushal
    pushfl    

    pushl %edx
    pushl %ecx
    pushl %ebx


    call *jumptable(, %eax, 4)      # use call to get back here eax has error value at this point 
    popl %ebx
    popl %ecx
    popl %edx

    movl %eax, error_seen
    

    popfl
    popal

    movl error_seen, %eax
   
oob_error:
    iret
