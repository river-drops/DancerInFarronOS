#include <kdebug.h>
#include <kmonitor.h>
#include <stdio.h>
#include <string.h>
#include <trap.h>

/* *
 * Simple command-line kernel monitor useful for controlling the
 * kernel and exploring the system interactively.
 * */
//命令信息，在下面的commands进行初始化
struct command {
    //命令
    const char *name;
    //描述
    const char *desc;
    // return -1 to force monitor to exit
    int (*func)(int argc, char **argv, struct trapframe *tf);
};

static struct command commands[] = {
    {"help", "Display this list of commands.", mon_help},
    {"kerninfo", "Display information about the kernel.", mon_kerninfo},
    {"backtrace", "Print backtrace of stack frame.", mon_backtrace},
};


//定义了命令的条数
#define NCOMMANDS (sizeof(commands) / sizeof(struct command))

/***** Kernel monitor command interpreter *****/

//定义了最大的参数个数
#define MAXARGS 16
#define WHITESPACE " \t\n\r"

/* parse - parse the command buffer into whitespace-separated arguments */
//用来提取参数，并返回参数个数
//buf指输入的命令行command
//argv用来保存分割的每一个字符串的token
static int parse(char *buf, char **argv) {
    int argc = 0;
    while (1) {
        // find global whitespace
        // strchr - locates first occurrence of character in string
        while (*buf != '\0' && strchr(WHITESPACE, *buf) != NULL) {
            *buf++ = '\0';
        }
        if (*buf == '\0') {
            break;
        }

        // save and scan past next arg
        if (argc == MAXARGS - 1) {
            cprintf("Too many arguments (max %d).\n", MAXARGS);
        }
        argv[argc++] = buf;
        //strchr返回一个指针
        //指向的第二个参数字符在第一个字符数组中出现的位置指针
        while (*buf != '\0' && strchr(WHITESPACE, *buf) == NULL) {
            buf++;
        }
    }
    return argc;
}

/* *
 * runcmd - parse the input string, split it into separated arguments
 * and then lookup and invoke some related commands/
 * */
static int runcmd(char *buf, struct trapframe *tf) {
    char *argv[MAXARGS];
    int argc = parse(buf, argv);
    if (argc == 0) {
        return 0;
    }
    int i;
    for (i = 0; i < NCOMMANDS; i++) {
        if (strcmp(commands[i].name, argv[0]) == 0) {
            return commands[i].func(argc - 1, argv + 1, tf);
        }
    }
    cprintf("Unknown command '%s'\n", argv[0]);
    return 0;
}

/**
 * @brief      Implementations of basic kernel monitor commands
 *
 * @param      tf    trapframe
 */
void kmonitor(struct trapframe *tf) {
    cprintf("Welcome to the kernel debug monitor!!\n");
    cprintf("Type 'help' for a list of commands.\n");

    if (tf != NULL) {
        print_trapframe(tf);
    }

    char *buf;
    while (1) {
        if ((buf = readline("K> ")) != NULL) {
            if (runcmd(buf, tf) < 0) {
                break;
            }
        }
    }
}

/* mon_help - print the information about mon_* functions */
int mon_help(int argc, char **argv, struct trapframe *tf) {
    int i;
    for (i = 0; i < NCOMMANDS; i++) {
        cprintf("%s - %s\n", commands[i].name, commands[i].desc);
    }
    return 0;
}

/* *
 * mon_kerninfo - call print_kerninfo in kern/debug/kdebug.c to
 * print the memory occupancy in kernel.
 * */
int mon_kerninfo(int argc, char **argv, struct trapframe *tf) {
    print_kerninfo();
    return 0;
}

/* *
 * mon_backtrace - call print_stackframe in kern/debug/kdebug.c to
 * print a backtrace of the stack.
 * */
int mon_backtrace(int argc, char **argv, struct trapframe *tf) {
    print_stackframe();
    return 0;
}
