// Timer Interrupt handler


#include "../libs/types.h"
#include "../libs/param.h"
#include "../libs/riscv.h"
#include "../libs/sbi.h"
#include "../libs/spinlock.h"
#include "../libs/timer.h"
#include "../libs/printf.h"
#include "../libs/proc.h"

struct spinlock tickslock;
uint ticks;

void timerinit() {
    initlock(&tickslock, "time");
    #ifdef DEBUG
    printf("timerinit\n");
    #endif
}

void
set_next_timeout() {
    // There is a very strange bug,
    // if comment the `printf` line below
    // the timer will not work.

    // this bug seems to disappear automatically
    // printf("");
    sbi_set_timer(r_time() + INTERVAL);
}

void timer_tick() {
    acquire(&tickslock);
    ticks++;
    wakeup(&ticks);
    release(&tickslock);
    set_next_timeout();
}
