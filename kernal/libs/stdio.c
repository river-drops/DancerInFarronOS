#include <console.h>
#include <definations.h>
#include <stdio.h>


// c 输出到 stdout
//it will increace the value of counter pointed by @cnt.
static void cputch(int c,int *cnt){
    cons_putc(c);
    (*cnt)++;
}


//vcprintf - format a string and writes it to stdout

//The return value is the number of characters which would bewritten to stdout.

//Call this function if you are already dealing with a va_list.
//you probably want cprintf() instead.
int vcprintf(const char *fmt, va_list ap){
    int cnt=0;
    vprintfmt((void*)cputch, &cnt, fmt, ap);
    return cnt;
}


//cprintf - formats a string and writes it to stdout
int cprintf(const char *fmt, ...){
    va_list ap;
    int cnt;
    va_start(ap,fmt);
    cnt = vcprintf(fmt,ap);
    va_end(ap);
    return cnt;
}


// cputchar - writes a single character to stdout 
void cputchar(int c) { cons_putc(c); }



//cputs- writes the string pointed by @str to stdout and
//appends a newline character.
int cputs(const char *str) {
    int cnt = 0;
    char c;
    while ((c = *str++) != '\0') {
        cputch(c, &cnt);
    }
    cputch('\n', &cnt);
    return cnt;
}


//getchar
int getchar(void){
    int c;
    while((c=cons_getc())==0);
    return c;
}
