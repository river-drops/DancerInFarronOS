#include "defs.h"
#include <stdarg.h>
static char digits[] = "0123456789abcdef";

//打印整型数,base=基本进制,sign=是否有符号
static void
printint(int xx, int base, int sign) {
    char buf[16];
    int i;
    uint x;

    if (sign && (sign = xx < 0))
        x = -xx;
    else
        x = xx;

    i = 0;
    do {
        buf[i++] = digits[x % base];
    } while ((x /= base) != 0);

    if (sign)
        buf[i++] = '-';

    while (--i >= 0)
        consputc(buf[i]);
}


//打印地址
static void
printptr(uint64 x) {
    int i;
    consputc('0');
    consputc('x');
    for (i = 0; i < (sizeof(uint64) * 2); i++, x <<= 4)
        consputc(digits[x >> (sizeof(uint64) * 8 - 4)]);
}

// Print to the console. only understands %d, %x, %p, %s.
void printf(char *fmt, ...) {
    va_list ap;
    int i, c;
    char *s;

    if (fmt == 0)
        panic("null fmt");

    va_start(ap, fmt);
    for (i = 0; (c = fmt[i] & 0xff) != 0; i++) {
        if (c != '%') {
            consputc(c);
            continue;
        }
        c = fmt[++i] & 0xff;
        if (c == 0)
            break;
        switch (c) {
            case 'd':
                printint(va_arg(ap, int), 10, 1);
                break;
            case 'x':
                printint(va_arg(ap, int), 16, 1);
                break;
            case 'p':
                printptr(va_arg(ap, uint64));
                break;
            case 's':
                if ((s = va_arg(ap, char *)) == 0)
                    s = "(null)";
                for (; *s; s++)
                    consputc(*s);
                break;
            case '%':
                consputc('%');
                break;
            default:
                // Print unknown % sequence to draw attention.
                consputc('%');
                consputc(c);
                break;
        }
    }
}

void ErrorStr(char *s) {
    printf("\x1b[31m%s\x1b[0m\n",s);
}
void WarnStr(char *s) {
    printf("\x1b[93m%s\x1b[0m\n",s);
}
// info
//     printf("\x1b[34m%s\x1b[0m",s);
void InfoStr(char *s) {
    printf("\x1b[34m%s\x1b[0m\n",s);
}
void InfoData(char *name,char * start,char * end){
    printf("\x1b[34m%s load range : [%p, %p]\x1b[0m\n", name, start, end);
}
void DebugStr(char *s) {
    printf("\x1b[32m%s\x1b[0m\n",s);
}
void TraceStr(char *s) {
    printf("\x1b[90m%s\x1b[0m\n",s);
}


void
panic(char *s)
{
    printf("panic: ");
    printf(s);
    printf("\n");
    loop();
}