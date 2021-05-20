/*
	prinrf库函数
*/

#ifndef __PRINTF_H
#define __PRINTF_H
#define OPEN_PRINTF		//是否启用打印功能，若启用的宏定义OPEN_PRINTF,否则不定义OPEN_PRINTF=4G功能

#include "stm32f4xx.h"


#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)		//与标准库函数Printf相关
typedef struct 
	{
    u8 Printf_Mode;   //打印共用参数
  }Printf_DataStructure;

Printf_DataStructure *Printf_GetData(void);
void Printf_Init(void);

#endif

