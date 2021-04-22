#include<mmu.h>
#include<memlayout.h>


    .section .text,"ax",&progits

    .globl kernal_entry
kernal_entry:

    # load address
    la sp,bootstacktop

    #tail == riscv伪指令，作用相当于调用函数（跳转）
    tail kernal_init

.section .data
    # .align 2^12
    .align PAGESHIFT
    .global bootstack
bootstack:
    .space KSTACKSIZE
    .global bootstacktop
bootstacktop: