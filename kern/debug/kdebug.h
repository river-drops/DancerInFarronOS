#ifndef __KERN_DEBUG_KDEBUG_H__
#define __KERN_DEBUG_KDEBUG_H__

#include <defs.h>

//打印内核地址信息
void print_kerninfo(void);
//打印栈结构
void print_stackframe(void);
void print_debuginfo(uintptr_t eip);

#endif /* !__KERN_DEBUG_KDEBUG_H__ */

