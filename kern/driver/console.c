#include<sbi.h>
#include<console.h>


//从键盘读取输入数据
void kbd_intr(void){

}

//从serial port读取输入数据
void serial_intr(void) {

}

//初始化控制台设备
void cons_init(void) {

}

//向控制台设备打印一个单个字符
void cons_putc(int c) { 
    sbi_console_putchar((unsigned char)c); 
}

//从控制台获取一个输入字符
int cons_getc(void) {
    int c = 0;
    c = sbi_console_getchar();
    return c;
}
