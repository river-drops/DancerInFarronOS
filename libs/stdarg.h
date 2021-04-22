#ifndef __LIBS_STDARG_H__
#define __LIBS_STDARG_H__

//本头文件用作函数传参作用

/* compiler provides size of save area */
// va_list是一个字符指针，可以理解为指向当前参数的一个指针，取参必须通过这个指针进行。
// 在调用参数表之前，应该定义一个 va_list类型的变量，以供后用(假设这个 va_list 类型变量被定义为ap)；
typedef __builtin_va_list va_list;



//然后对 ap进行初始化，让它指向可变参数表里面的第一个参数。
//这是通过 va_start 来实现的，其第一个参数是 ap本身，第二个参数是在变参表前面紧挨着的一个变量；
#define va_start(ap, last)              (__builtin_va_start(ap, last))

//然后是获取参数，调用 va_arg。它的第一个参数是ap，第二个参数是要获取的参数的指定类型\
//并返回这个指定类型的值,同时把 ap 的位置指向变参表的下一个变量位置
#define va_arg(ap, type)                (__builtin_va_arg(ap, type))


//获取所有的参数之后，我们有必要将这个 ap指针关掉，以免发生危险，方法是调用 va_end
//它是将输入的参数 ap 置为NULL，应该养成获取完参数表之后关闭指针的习惯。
#define va_end(ap)                      /*nothing*/

#endif /* !__LIBS_STDARG_H__ */

