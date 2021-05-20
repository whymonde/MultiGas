/*
	FM24CL64.c库函数
*/

#ifndef __FM24CL64_H
#define __FM24CL64_H

#include "stm32f4xx.h"

//
#define FM24CL64_CMD_WRITE 			0xA0		//写命令
#define FM24CL64_CMD_READ				0xA1		//读命令


//结构体
typedef struct
{
	u8 RunMode;					//运行模式
	u16 Timer;					//定时器
}FM24CL64_DataStructure;





//----------------------------------------------------
void FM24CL64_Main(void);
void FM24CL64_Timer(void);
void FM24CL64_Init(void);

FM24CL64_DataStructure* FM24CL64_GetData(void);

void FM24CL64_WriteData(void);
void FM24CL64_ReadData(void);


void FM24CL64_TestReadData(void);
void FM24CL64_TestWriteData(void);

void FM24CL64_DebugASCII(u8 *p,u8 len);
//void FM24CL64_DebugRTU(u8 *p,u8 len);


#endif










