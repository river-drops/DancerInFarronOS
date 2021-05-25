#include "../libs/types.h"
#include "../libs/param.h"
#include "../libs/proc.h"
#include "../libs/intr.h"
#include "../libs/printf.h"

// push_off/pop_off are like intr_off()/intr_on() except that they are matched:
// it takes two pop_off()s to undo two push_off()s.  Also, if interrupts
// are initially off, then push_off, pop_off leaves them off.

// 为了避免死锁，xv6中规定若一个进程持有spinlock，则必须要禁用中断（若否，比如一个进程拿
// 到spinlock之后一个中断过来，然后该跳转到中断处理程序，如果该中断处理程序也要获得这把锁，
// 该进程就死锁了。当然这其中的原因在于同一个进程连续申请同一把锁），因此调用push_off和
// pop_off来禁用和开启中断。

void
push_off(void)
{
  int old = intr_get();

  intr_off();
  //printf("\e[32mpush_off()\e[0m: cpuid(): %d\n", cpuid());
  if(mycpu()->noff == 0)
    mycpu()->intena = old;
  mycpu()->noff += 1;
}

void
pop_off(void)
{
  struct cpu *c = mycpu();

  //printf("\e[31mpop_off()\e[0m: cpuid(): %d\n", cpuid());
  if(intr_get())
    panic("pop_off - interruptible");
  if(c->noff < 1) {
    //printf("c->noff = %d\n", c->noff);
    panic("pop_off");
  }
  //printf("c->noff: %d\n", c->noff);
  //printf("c: %x\n", c);
  c->noff -= 1;
  if(c->noff == 0 && c->intena)
    intr_on();
}
