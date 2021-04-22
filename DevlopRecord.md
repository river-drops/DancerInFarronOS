# DancerInFarronOS开发记录

[toc]







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

