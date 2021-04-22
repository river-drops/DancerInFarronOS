#ifndef _ASM_RISCV_SBI_H
#define _ASM_RISCV_SBI_H


//内存块信息
typedef struct {
  unsigned long base;
  unsigned long size;
  unsigned long node_id;
} memory_block_info;



unsigned long sbi_query_memory(unsigned long id, memory_block_info *p);

//设置定时器
void sbi_set_timer(unsigned long long stime_value);
void sbi_send_ipi(unsigned long hart_id);
unsigned long sbi_clear_ipi(void);



void sbi_shutdown(void);

//打印一个字符
void sbi_console_putchar(unsigned char ch);
//获取一个字符
int sbi_console_getchar(void);

#endif