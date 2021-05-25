// Copyright (c) 2006-2019 Frans Kaashoek, Robert Morris, Russ Cox,
//                         Massachusetts Institute of Technology

#include "../libs/types.h"
#include "../libs/param.h"
#include "../libs/memlayout.h"
#include "../libs/riscv.h"
#include "../libs/sbi.h"
#include "../libs/console.h"
#include "../libs/printf.h"
#include "../libs/kalloc.h"
#include "../libs/timer.h"
#include "../libs/trap.h"
#include "../libs/proc.h"
#include "../libs/plic.h"
#include "../libs/vm.h"
#include "../libs/disk.h"
#include "../libs/buf.h"
#ifndef QEMU
#include "../libs/sdcard.h"
#include "../libs/fpioa.h"
#include "../libs/dmac.h"
#endif

static inline void inithartid(unsigned long hartid) {
  asm volatile("mv tp, %0" : : "r" (hartid & 0x1));
}

volatile static int started = 0;

void
main(unsigned long hartid, unsigned long dtb_pa)
{
  inithartid(hartid);
  
  if (hartid == 0) {
    consoleinit();
    printfinit();   // init a lock for printf 
    print_logo();
    #ifdef DEBUG
    printf("hart %d enter main()...\n", hartid);
    #endif
    kinit();         // physical page allocator
    kvminit();       // create kernel page table
    kvminithart();   // turn on paging
    timerinit();     // init a lock for timer
    trapinithart();  // install kernel trap vector, including interrupt handler
    procinit();
    plicinit();
    plicinithart();
    #ifndef QEMU
    fpioa_pin_init();
    dmac_init();
    #endif 
    disk_init();
    binit();         // buffer cache
    fileinit();      // file table
    userinit();      // first user process
    printf("hart 0 init done\n");
    
    for(int i = 1; i < NCPU; i++) {
      unsigned long mask = 1 << i;
      sbi_send_ipi(&mask);
    }
    __sync_synchronize();
    started = 1;
  }
  else
  {
    // hart 1
    while (started == 0)
      ;
    __sync_synchronize();
    #ifdef DEBUG
    printf("hart %d enter main()...\n", hartid);
    #endif
    kvminithart();
    trapinithart();
    plicinithart();  // ask PLIC for device interrupts
    printf("hart 1 init done\n");
  }
  scheduler();
}
