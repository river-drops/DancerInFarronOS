#include<mmu.h>
#include<memlayout.h>

    # "ax"表示该节区可分配并且可执行,progbits是type
    .section .text,"ax",%progbits
    
    .globl kern_entry
kern_entry:

    # load adress
    la sp, bootstacktop

    # tail == riscv伪指令，作用相当于调用函数（跳转）
    tail kern_init

.section .data
    # .align 2^12
    .align PGSHIFT
    .global bootstack
bootstack:
    .space KSTACKSIZE
    .global bootstacktop
bootstacktop: