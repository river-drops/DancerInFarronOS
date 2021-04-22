#ifndef __LIBS_DEFS_H__

//define follow no args means  compile as no
//because when you getinto this branch __LIBS_DEFS_H__ has defined
#define __LIBS_DEFS_H__

#ifndef NULL
#define NULL ((void *)0)
#endif

//force to inline function
//like
//__attribute__((always_inline)) void a(){
//     print("a"); 
// }
// void b()
// {
//   a();
// }
// after transform become
//  void b()
// ｛
//       print("a"); 
// ｝
#define __always_inline inline __attribute__((always_inline))

//reverse to upone
#define __noinline __attribute__((noinline))
//call somthiong like exit(int) to return
#define __noreturn __attribute__((noreturn))


//means bool => int
//like bool i  == int i
//contrary to define
typedef int bool;

//define the exact number of digital
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long long int64_t;
typedef unsigned long long uint64_t;


//define the border
#define INT8_MIN    (-0x7f - 1)
#define INT16_MIN   (-0x7fff - 1)
#define INT32_MIN   (-0x7fffffff - 1)
#define INT64_MIN   (-0x7fffffffffffffff - 1)

#define INT8_MAX    (0x7f)
#define INT16_MAX   (0x7fff)
#define INT32_MAX   (0x7fffffff)
#define INT64_MAX   (0x7fffffffffffffff)

#define UINT8_MAX   (0xff)
#define UINT16_MAX  (0xffff)
#define UINT32_MAX  (0xffffffff)
#define UINT64_MAX  (0xffffffffffffffff)


//define pointer 
//pointers and addresses are 32 bits long
//uintptr_t to represent the numerical values of addresses
typedef int64_t intptr_t;
typedef uint64_t uintptr_t;

//size_t is used for memory object sizes
typedef uintptr_t size_t;

//used for page numbers 
typedef size_t ppn_t;

//xiangxia shejin n zhengshu
#define ROUNDDOWN(a, n) ({                                          \
            size_t __a = (size_t)(a);                               \
            (typeof(a))(__a - __a % (n));                           \
        })

//xiangshang shejin n zhengshu
#define ROUNDUP(a, n) ({                                            \
            size_t __n = (size_t)(n);                               \
            (typeof(a))(ROUNDDOWN((size_t)(a) + __n - 1, __n));     \
        })


//Return the offset of 'member' relative to the beginning of a struct type
//explanation https://blog.csdn.net/eleanoryss/article/details/70053525
#define offsetof(type, member)                                      \
    ((size_t)(&((type *)0)->member))


/* *
 * to_struct - get the struct from a ptr
 * @ptr:    a struct pointer of member
 * @type:   the type of the struct this is embedded in
 * @member: the name of the member within the struct
 * */
#define to_struct(ptr, type, member)                               \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#endif /* !__LIBS_DEFS_H__ */