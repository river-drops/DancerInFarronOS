#include "../libs/types.h"
#include "../libs/sysinfo.h"
#include "user.h"
#include "../libs/stat.h"
#include "../libs/file.h"
#include "../libs/fcntl.h"

#define syscall_nums 31


int main(){
    close(0);
    close(1);
    close(2);
    dev(O_RDWR, CONSOLE, 0);
    dev(O_RDWR, CONSOLE, 0);
    dev(O_RDWR, CONSOLE, 0);
    dup(0);  // stdout
    dup(0);  // stderr

    char* syscall_name[]={
        "brk",
        "chdir",
        //"clone",
        "dup",
        "dup2",
        "execve",
        "exit",
        "fork",
        "fstat",
        "getcwd",
        "getdents",
        "getpid",
        "getppid",
        "gettimeofday",
        "mkdir_",
        "mmap",
        "mount",
        "munmap",
        "open",
        "openat",
        "pipe",
        "read",
        "sleep",
        "test_echo",
        "times",
        "umount",
        "uname",
        "unlinke",
        "wait",
        "waitpid",
        "write",
        "yield"
    };


    char* argv[]={"",0};
    //char* argv1[]={"sh",0};
    int i=0;
    for(i=0;i<syscall_nums;++i){
        argv[0]=syscall_name[i];
        int pid=fork();
        if(pid==0){
            exec(argv[0],argv);
            printf("init: exec %s failed\n",argv[0]);
            exit(-1);
        }else{
            sleep(10);
        }
    }
    exit(0);
    // int pid=fork();
    // if(pid==0){
    //     exec(argv1[0],argv1);
    //     printf("init: exec sh failed\n");
    //     exit(-1);
    // }
    //while(1);

}