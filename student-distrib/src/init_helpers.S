
.text
.globl set_paging_params
# Set paging related registers (touches cr0, cr3, and cr4 register). Value for cr3 is passed in as argument (ebp+8) to function.
set_paging_params:
    push %ebp
    movl %esp, %ebp
    mov %cr4, %eax
    or $0x10, %eax
    mov %eax, %cr4
    mov 8(%ebp), %eax
    mov %eax, %cr3
    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0
    leave
    ret


