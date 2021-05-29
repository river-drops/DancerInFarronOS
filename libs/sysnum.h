#ifndef __SYSNUM_H
#define __SYSNUM_H

// System call numbers
#define SYS_fork         220  //clone


#define SYS_exit         93


#define SYS_wait         260

#define SYS_pipe         59


#define SYS_read         63


#define SYS_kill         6
#define SYS_exec         7

#define SYS_fstat        80


#define SYS_chdir        49


#define SYS_dup         23


#define SYS_getpid      172

#define SYS_sbrk        214

#define SYS_sleep       13

#define SYS_uptime      169


#define SYS_open        56



#define SYS_write       64


//---
#define SYS_remove      21
//---
#define SYS_trace       18
#define SYS_sysinfo     19

#define SYS_mkdir       34


#define SYS_close       57


#define SYS_test_proc   22
//---
#define SYS_dev         25
//---
#define SYS_readdir     24


#define SYS_getcwd      17


#define SYS_rename      26

#endif