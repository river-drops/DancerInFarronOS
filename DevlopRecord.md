# DancerInFarronOS开发记录

[toc]







## Day4.25 物理内存管理

今日完成相关代码的注释部分

完成中断部分

学习有关物理内存管理的部分



### 页表项

我们的”词典“（页表）存储在内存里，由若干个格式固定的”词条“也就是**页表项（PTE, Page Table Entry）**组成。

一个页表项是用来描述一个虚拟页号如何映射到物理页号的。如果一个虚拟页号通过**某种手段**找到了一个页表项，并通过读取上面的物理页号完成映射，我们称这个虚拟页号**通过该页表项**完成映射。

Sv39的一个页表项占据8字节，结构是这样的：

| 63-54      | 53-28  | 27-19  | 18-10  | 9-8  | 7    | 6    | 5    | 4    | 3    | 2    | 1    | 0    |
| ---------- | ------ | ------ | ------ | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| *Reserved* | PPN[2] | PPN[1] | PPN[0] | RSW  | D    | A    | G    | U    | X    | W    | R    | V    |
| 10         | 26     | 9      | 9      | 2    | 1    | 1    | 1    | 1    | 1    | 1    | 1    | 1    |

---

我们可以看到 Sv39 里面的一个页表项大小为64位 8 字节。其中第 53−10 共 44 位为一个物理页号，表示这个虚拟页号映射到的物理页号。后面的第 9−0位则描述映射的状态信息。

- RSW两位留给 S Mode 的应用程序，我们可以用来进行拓展。

- D，即 Dirty ，如果 D=1表示自从上次 D被清零后，有虚拟地址通过这个页表项进行写入。

- A，即 Accessed，如果 A=1表示自从上次 A 被清零后，有虚拟地址通过这个页表项进行读、或者写、或者取指。

- G，即Global，如果G=1表示这个页表项是”全局"的，也就是所有的地址空间（所有的页表）都包含这一项

- U(user)为 111 表示用户态 (U Mode)的程序 可以通过该页表项进行映射。在用户态运行时也只能够通过 U=1的页表项进行虚实地址映射。

  注意，S Mode 不一定可以通过 U=1的页表项进行映射。我们需要将 S Mode 的状态寄存器 `sstatus` 上的 **SUM** 位手动设置为 111 才可以做到这一点（通常情况不会把它置1）。否则通过 U=1的页表项进行映射也会报出异常。另外，不论`sstatus`的**SUM**位如何取值，S Mode都不允许执行 U=1的页面里包含的指令，这是出于安全的考虑。

- R,W,X为许可位，分别表示是否可读 (Readable)，可写 (Writable)，可执行 (Executable)。

- V表示这个页表项是否合法。如果为 000 表示不合法，此时页表项其他位的值都会被忽略。

以 W这一位为例，如果 W=0 表示不可写，那么如果一条 store 的指令，它通过这个页表项完成了虚拟页号到物理页号的映射，找到了物理地址。但是仍然会报出异常，是因为这个页表项规定如果物理地址是通过它映射得到的，那么不准写入！R,X 也是同样的道理。

根据 R,W,X取值的不同，我们可以分成下面几种类型：

| X    | W    | R    | Meaning                         |
| ---- | ---- | ---- | ------------------------------- |
| 0    | 0    | 0    | 指向下一级页表的指针            |
| 0    | 0    | 1    | 这一页只读                      |
| 0    | 1    | 0    | *保留(reserved for future use)* |
| 0    | 1    | 1    | 这一页可读可写（不可执行）      |
| 1    | 0    | 0    | 这一页可读可执行（不可写）      |
| 1    | 0    | 1    | 这一页可读可执行                |
| 1    | 1    | 0    | *保留(reserved for future use)* |
| 1    | 1    | 1    | 这一页可读可写可执行            |





### 页表基址

在翻译的过程中，我们首先需要知道树状页表的根节点的物理地址（思考：为啥不是“根节点的虚拟地址”？）。

这一般保存在一个特殊寄存器里。对于RISCV架构，是一个叫做`satp`（Supervisor Address Translation and Protection Register）的CSR。实际上，`satp`里面存的不是最高级页表的起始物理地址，而是它所在的物理页号。除了物理页号，`satp`还包含其他信息

| 63-60 | 59-44 | 43-0 |
| ----- | ----- | ---- |
| MODE  | ASID  | PPN  |
| 4     | 16    | 44   |

MODE表示当前页表的模式

- 0000表示不使用页表，直接使用物理地址，在简单的嵌入式系统里用着很方便。
- 0100表示Sv39页表，也就是我们使用的，虚拟内存空间高达512GiB。
- 0101表示Sv48页表，它和Sv39兼容，可以猜猜它有几层。虚拟内存空间高达256TiB。
- 其他编码保留备用。

ASID （Address Space Identifier 地址空间标识符）域是可选的，它可以用来降低上下文切换的开销。

PPN字段保存了根页表的物理地址，它以 4 KiB的页面大小为单位。通常 M模式的程序在第一次进入 S模式之前会把零写入 satp以禁用分页，然后 S模式的程序在初始化页表以后会再次进行satp寄存器的写操作。

OS 可以在内存中为不同的应用分别建立不同虚实映射的页表，并通过修改寄存器 `satp` 的值指向不同的页表，从而可以修改 CPU 虚实地址映射关系及内存保护的行为。



---







## Day4.24 中断相关

划水一天

问题

1. 描述处理中断异常的流程
2. 对于任何中断都需要保存所有的寄存器吗？为什么？
3. 触发，捕获，处理异常





## Day4.23 

今日target

- [ ] 为小内核编写适合的makefile文件

---

有关函数调用的知识

指令集必须给用于函数调用的跳转指令一些额外的能力，而不只是单纯的跳转。在 RISC-V 架构上，有两条指令即符合这样的特征：

![image-20210423165843444](C:\Users\96233\AppData\Roaming\Typora\typora-user-images\image-20210423165843444.png)

从中可以看出，这两条指令除了设置 pc 寄存器完成跳转功能之外，还将当前跳转指令的下一条指令地址保存在 rd 寄存器中。 （这里假设所有指令的长度均为 4 字节，在不使用 C 标准指令集拓展的情况下成立） 在 RISC-V 架构中， 通常使用 ra(x1) 寄存器作为其中的 rd ，因此在函数返回的时候，只需跳转回 ra 所保存的地址即可。事实上在函数返回的时候我们常常使用一条 **伪指令** (Pseudo Instruction) 跳转回调用之前的位置： `ret` 。它会被汇编器翻译为 `jalr x0, 0(x1)`，含义为跳转到寄存器 ra 保存的物理地址，由于 x0 是一个恒为 0 的寄存器，在 rd 中保存这一步被省略。

总结一下，在进行函数调用的时候，我们通过 jalr 指令 保存返回地址并实现跳转；而在函数即将返回的时候，则通过 ret 指令跳转之前的下一条指令继续执行。这两条指令实现了函数调用流程的核心机制。

![image-20210423165935562](C:\Users\96233\AppData\Roaming\Typora\typora-user-images\image-20210423165935562.png)

---

一直在看代码没啥时间写md文档:cry:

今天的完成内容

* 中断向量表的理解构建
* makefile文件的全注释
* 定时器中断的设置

lab1实验五代码

```c
/* *
 * print_stackframe - print a list of the saved eip values from the nested
 * 'call'
 * instructions that led to the current point of execution
 *
 * The x86 stack pointer, namely esp, points to the lowest location on the stack
 * that is currently in use. Everything below that location in stack is free.
 * Pushing
 * a value onto the stack will invole decreasing the stack pointer and then
 * writing
 * the value to the place that stack pointer pointes to. And popping a value do
 * the
 * opposite.
 *
 * The ebp (base pointer) register, in contrast, is associated with the stack
 * primarily by software convention. On entry to a C function, the function's
 * prologue code normally saves the previous function's base pointer by pushing
 * it onto the stack, and then copies the current esp value into ebp for the
 * duration
 * of the function. If all the functions in a program obey this convention,
 * then at any given point during the program's execution, it is possible to
 * trace
 * back through the stack by following the chain of saved ebp pointers and
 * determining
 * exactly what nested sequence of function calls caused this particular point
 * in the
 * program to be reached. This capability can be particularly useful, for
 * example,
 * when a particular function causes an assert failure or panic because bad
 * arguments
 * were passed to it, but you aren't sure who passed the bad arguments. A stack
 * backtrace lets you find the offending function.
 *
 * The inline function read_ebp() can tell us the value of current ebp. And the
 * non-inline function read_eip() is useful, it can read the value of current
 * eip,
 * since while calling this function, read_eip() can read the caller's eip from
 * stack easily.
 *
 * In print_debuginfo(), the function debuginfo_eip() can get enough information
 * about
 * calling-chain. Finally print_stackframe() will trace and print them for
 * debugging.
 *
 * Note that, the length of ebp-chain is limited. In boot/bootasm.S, before
 * jumping
 * to the kernel entry, the value of ebp has been set to zero, that's the
 * boundary.
 * */
void print_stackframe(void) {
    /* LAB1 YOUR CODE : STEP 1 */
    /* (1) call read_ebp() to get the value of ebp. the type is (uint32_t);
     * (2) call read_eip() to get the value of eip. the type is (uint32_t);
     * (3) from 0 .. STACKFRAME_DEPTH
     *    (3.1) printf value of ebp, eip
     *    (3.2) (uint32_t)calling arguments [0..4] = the contents in address
     * (unit32_t)ebp +2 [0..4]
     *    (3.3) cprintf("\n");
     *    (3.4) call print_debuginfo(eip-1) to print the C calling function name
     * and line number, etc.
     *    (3.5) popup a calling stackframe
     *           NOTICE: the calling funciton's return addr eip  = ss:[ebp+4]
     *                   the calling funciton's ebp = ss:[ebp]
     */

    // +| 栈底方向 | 高位地址

    // | ... |

    // | ... |

    // | 参数3 |

    // | 参数2 |

    // | 参数1 |

    // | 返回地址 |

    // | 上一层[ebp] |<-------- [ebp]

    // | 局部变量 | 低位地址


    uint32_t ebp = read_ebp();
    uint32_t eip = read_eip();
    uint32_t *arguments;
    int i,j;
    for(i=0;ebp!=0&&i< 19 +1;++i){
        cprintf("ebp:0x%08x eip:0x%08x \nargs:", ebp, eip);
        arguments=(uint32_t *)ebp +2;
        for(j=0;j<4;++j){
            cprintf("0x%08x ",arguments[j]);
        }
        cprintf("\n\n");
        print_debuginfo(eip-1);
        eip=((uint32_t *)ebp)[1];
        ebp=((uint32_t *)ebp)[0];
    }
    panic("Not Implemented!");
}
```



---









## Day4.22

今日target

- [ ] ucore实验123，改编为自己的内核并在qemu上跑起来
- [ ] makefile文件的相应编写

---

资源一览

* makefile
  * https://seisman.github.io/how-to-write-makefile/overview.html

---

### 实验环境的搭建

* qemu前两天装完了

* 安装预编译工具链 riscv64-unknown-elf-gcc，需要添加环境变量

  * 把解压后的文件的bin目录添加到/etc/profile中，并运行source /etc/profile

  * ```shell
    添加的path
    export PATH="/home/xin/work/riscv64-unknown-elf-gcc-8.3.0-2020.04.0-x86_64-linux-ubuntu14/bin"
    ```

  * ![image-20210422154809329](C:\Users\96233\AppData\Roaming\Typora\typora-user-images\image-20210422154809329.png)

  * 把上述图片中的添加部分换成代码块中的路径即可

---

### lab0.5

* 复习一下risc-v的汇编

  * https://lgl88911.gitee.io/2021/02/28/RISC-V%E6%B1%87%E7%BC%96%E5%BF%AB%E9%80%9F%E5%85%A5%E9%97%A8/

* 关于C语言中define相关用法

  * define后面只跟一个“参数”

    定义宏，并在预处理过程中将其替换为空字符串（即删除）。
    这样做主要是为了标记某些内容，使程序阅读者能够清楚标记表明的意义，同时又不影响被编译的源代码。
    另外，通常这些标记能被条件编译的预处理命令#ifdef、#ifndef检测到。一个重要的用法是多文件编译中防止头文件被重复包含，如：
    header.h:
    \#ifndef _HEADER_
    \#define _HEADER_
    /*something*/
    \#endif
    a.c
    \#include"header.h"
    /*something*/
    main.c
    \#include"header.h"
    /*something*/

    这样无论哪个.c文件先被编译，包含header.h以后，之后进入#include后header.h中的_HEADER_已定义，那么#ifndef _HEADER_和#endif之间的代码不会被重复编译，header.h的目标代码只存在于其中一个源文件的目标代码文件中，链接器链接两个源文件编译的目标文件时不会产生因为重复编译导致的重复定义的链接错误。

    从红字部分我们就可以理解了

    具体意思是这样的，就是在预编译时期，我们的宏要做宏替换，包含的头文件要替换为头文件里面的代码，但是注意，当你写一个大的C语言文问题件，你可能会写成为几个小的C语言文件

     分别编译，链接后又成为一个主体。问题是在编写小的C语言文件时，你可能会重复的包含了头文件。比如你在a.c中包含了头文件zhangfei.h在头文件b.c中也包含头文件zhangfei.h

    但其实问题就来了，因为在编译后链接到一个文件时就有了重复的东西就是那个头文件。当然如果你一开始就写为一个c文件就不会有这个问题。但是工程大了不会只写一个文件。我们如何解决那

    就是自己编写一个文件，在里面我们用预编译的一些语法手段。让一个头文件只编译一次。具体语法实现就是上面写的代码。

    宏定义编译前会被编译器进行替换，像你所说的只有一个标识符的情况，如果在代码里使用了这个标识符，会被替换为空，也就是相当于没加。
    用来做编译开关的话
    \#define DEBUG
    \#ifdefine DEBUG
    your code(输出调试信息等等)
    \#endif

    DEBUG被定义了的话your code部分会被编译，如果DEBUG未定义，编译器则忽略your code部分。

---

### lab0.5源码阅读

相关注释写在对应文件中

* git有关错误
  * 需要配置proxy，端口对应cordCloud给出的端口







使用vscode编写代码包含头文件的时候遇到如下错误

> #include errors detected. Please update your includePath. Squiggles are disabled for this translation unit (/home/xin/work/DancerInFarronOS/kernal/libs/string.h).C/C++(1696)

* https://blog.csdn.net/weixin_44718794/article/details/106751513
* 此方法成功解决

---

### 今日完成的工作

- [x] 熟悉了git的使用
- [x] 写完了os的defination部分
- [x] 写完了error错误码部分
- [x] 完成了一小部分string的编写，实现的函数如下
  * strlen
  * strnlen
  * memset
  * memcpy
  * memcmp



---










## Day4.21

* 第二天，收到板子了，打算跑清华rcore
  * 收到的U盘插到电脑上虚拟机不识别，需要把usb设置改为3.0
  * ![image-20210421154004405](C:\Users\96233\AppData\Roaming\Typora\typora-user-images\image-20210421154004405.png)
* 解决u
* buntu18 github打不开的问题
  * https://blog.csdn.net/u012856866/article/details/107614765?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_baidulandingword-1&spm=1001.2101.3001.4242
* ubuntu翻墙！！！
  * https://docs.cordcloud.biz/shi-yong-fang-fa/untitled/linux-ruan-jian-she-zhi/clashy-for-linux
  * https://help.hitun.io/zh/article/linux-clashy-159ofij/   
  * 先第一个再第二个
  * 用cordcloud购买一个服务器，再利用clashy进行服务器的配置
* 初步确定用南开实验课的ucore进行改进
  * https://nankai.gitbook.io/ucore-os-on-risc-v64/



---





## Day4.20

* 第一天开始，由于之前用的是ubuntu16，比赛要求的18版本，新创建了一个ubuntu18.04版本的虚拟机，并做了相关的配置
* 之前看过Oranges操作系统和30天自制操作系统，个人感觉30天自制更加易懂，读起来挺舒服的
* 找了一部分参考资料，感觉比较有用的如下链接

  * 手把手教你设计risc-v处理器（书）
  * https://rcore-os.github.io/rCore-Tutorial-Book-v3/index.html

    * 这个资料是展示如何 **从零开始** 用 **Rust** 语言写一个基于 **RISC-V** 架构的 **类 Unix 内核**，支持模拟器qemu
  * https://chyyuu.gitbooks.io/simple_os_book/content/
    * ucore OS + RISC-V 清华版本编辑
  * https://nankai.gitbook.io/ucore-os-on-risc-v64/lab0/shi-yan-mu-de
    * uCore + RISC-V实验指导，各类介绍，挺全
    * 有各类工具介绍，调试器介绍，各类命令介绍，git介绍
    * ==环境搭建指导，调试工具安装指导==
  * https://gitee.com/syivester/pke-doc/blob/master/chapter1.md

    * 介绍了一下RISC-V体系结构等相关知识
  * https://nju-projectn.github.io/ics-pa-gitbook/ics2020/

    * 南京大学操作系统相关实验
    * 基本全英文描述，可以练练计算机相关英语
  * http://crva.ict.ac.cn/documents/RISC-V-Reader-Chinese-v2p1.pdf
    * 包云岗老师翻译的一本RISC-V的手册
  * https://risc-v-getting-started-guide.readthedocs.io/en/latest/linux-qemu.html
    * 跑一个risv-v的linux系统在qemu上，可以测试一下
* 开发用到的工具
  * qemu（硬件模拟器）
  * 代码编辑器vscode



今天的实际工作，跑了一下https://risc-v-getting-started-guide.readthedocs.io/en/latest/linux-qemu.html

* 其实并没有跑，linux包太大了没下完，只安装了qemu

### QEMU的安装（全新ubuntu18.04虚拟机）

1. 首先新建一个working directory，在执行以下命令

```shell
//安装编译所需要的依赖包
sudo apt install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev \
              gawk build-essential bison flex texinfo gperf libtool patchutils bc \
              zlib1g-dev libexpat-dev pkg-config  libglib2.0-dev libpixman-1-dev git tmux python3



git clone https://github.com/qemu/qemu

cd qemu
git checkout v5.0.0
./configure --target-list=riscv64-softmmu
make -j $(nproc)
sudo make install
```

2.在执行./configure --target-list=riscv64-softmmu会报错，缺少相关文件支持，输入以下命令

```shell
sudo apt install build-essential

sudo apt-get install pkg-config

sudo apt-get install libglib2.0-dev

sudo apt-get install libpixman-1-dev
```

安装完成后继续1的操作即可

3.突然发现清华的rcore有一部分实验配置环境的内容

* https://rcore-os.github.io/rCore-Tutorial-Book-v3/chapter0/5setup-devel-env.html
* 跟着配置环境就好了

