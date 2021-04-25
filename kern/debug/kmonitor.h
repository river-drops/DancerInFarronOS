#ifndef __KERN_DEBUG_MONITOR_H__
#define __KERN_DEBUG_MONITOR_H__

#include <trap.h>

void kmonitor(struct trapframe *tf);

//打印可以使用那些命令和命令的描述
int mon_help(int argc, char **argv, struct trapframe *tf);

//
int mon_kerninfo(int argc, char **argv, struct trapframe *tf);
int mon_backtrace(int argc, char **argv, struct trapframe *tf);

#endif /* !__KERN_DEBUG_MONITOR_H__ */

