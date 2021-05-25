#   DancerInFarron队伍——OS文档总结 
> 说明：本项目是对华中科技大学同学所开发的xv6-k210进行的部分注释版，对于比赛来说由于时间和精力有限，小组开发的OS并没有完成文件系统，因此无法跑测试用例，故临时打算对xv6-k210项目进行注释，并写此文档(未完成)，虽然没有完成比赛，但是还是希望通过此文档对我们这几个月来的学习成果进行一些总结。




## 1 环境搭建和运行展示

我们在进行项目设计的过程中采用的是Ubuntu18.04操作系统，使用的VMWARE 15 pro安装并进行相关设计。
需要的依赖环境有RUST开发环境配置（因为项目用到了RUSTSBI），QEMU模拟器和RISCV工具链的配置。
将项目git下来后操作步骤如下：
在qemu平台上：
```
make fs
make run platform=qemu
```


## 2 xv6-k210的项目结构
代码树展示
```
DANCERINFARRONOS
│  Makefile
│  OS文档.md
│  README.md
├─.vscode
├─bootloader
│  └─SBI
│      │  sbi-k210
│      │  sbi-qemu
│      │
│      ├─rustsbi-k210
│      │
│      └─rustsbi-qemu
│
├─kernel（内核代码部分）
│      bio.c    (高速缓存部分)
│      console.c(管理控制台部分)
│      disk.c   (选择硬盘分配方式是按qemu还是k210)
│      dmac.c   (K210)
│      entry_k210.S (K210的内核执行环境的汇编代码)
│      entry_qemu.S (qemu的内核执行环境的汇编代码)
│      exec.c   (内存管理部分，将程序段加载到虚拟地址页表等操作)
│      fat32.c  (fat32文件系统)
│      file.c   (文件的相关操作)
│      fpioa.c  (K210所需要的部分)
│      gpiohs.c (K210所需要的部分)
│      intr.c   (禁用和开启中断)
│      kalloc.c (物理内存分配)
│      kernelvec.S  (保存&&恢复全部寄存器，加载kerneltrap等，主要用于中断)
│      logo.c   (logo标识)
│      main.c   (进行整个OS的初始化进行)
│      pipe.c   (用于进程间通信)
│      plic.c   (Platform-Level Interrupt Controller，平台级中断控制器)
│      printf.c (打印函数相关)
│      proc.c   (进程实现)
│      sdcard.c (K210所需要的部分)
│      sleeplock.c  (sleep调用的专门实现)
│      spi.c    (K210所需要的部分）
│      spinlock.c   (互斥原语的实现)
│      string.c (String相关函数的实现)
│      swtch.S  (上下文交换，主要与各态之间的切换)
│      syscall.c    (系统调用的接口)
│      sysctl.c (K210所需要的部分)
│      sysfile.c    (有关文件部分系统调用的接口的具体实现)
│      sysproc.c    (系统调用程序部分的具体实现)
│      timer.c  (计时器的实现)
│      trampoline.S (用户态到内核态的“跳板”)
│      trap.c   (中断处理)
│      uart.c   ()
│      utils.c  (K210所需要的部分)
│      virtio_disk.c(qemu)
│      vm.c     (虚拟内存部分)
│
├─libs（存放引用的头文件）
│
├─linker（链接文件）
│      k210.ld
│      qemu.ld
│
├─pic（md文件引用的图片）
|
├─test_example（测试程序）
|
├─tools（烧写至k210的工具文件）
│      flash-list.json
│      kflash.py
│
└─user(存放写的用户程序)
```




## 3 做出的部分修改


## 4 操作系统概述


## 5 进程与中断
### 5.1 进程与中断原理

### 5.2 相关代码文件及解释


## 6 内存管理
### 6.1 内存管理原理

### 6.2 相关代码文件及解释


## 7 文件系统
### 7.1 FAT32文件系统原理

### 7.2 相关代码文件及解释


## 8 大赛总结