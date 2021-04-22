#include<stdio.h>
#include<string.h>

int kernel_init(void) __attribute__((noreturn));


int kernel_init(void){
    extern char edata[],end[];

    memset(edata,0,end-edata);
    const char *message = "DancerInFarronOS is loading ...\n";
    
    cprintf("%s\n\n",message);


    while (1);
    
}
