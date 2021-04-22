#include <definations.h>
#include <riscv.h>
#include <errors.h>
#include <stdio.h>
#include <string.h>

//打印格式化的函数相关



static const char * const error_string[MAXERROR + 1]={
    [0]                     NULL,
    [E_UNSPECIFIED]            "unspecified error",
    [E_BAD_PROC]            "bad process",
    [E_INVAL]                "invalid parameter",
    [E_NO_MEM]                "out of memory",
    [E_NO_FREE_PROC]        "out of processes",
    [E_FAULT]                "segmentation fault",
}