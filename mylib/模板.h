/*
	screen.c库函数
*/

#ifndef __SCREEN_H
#define __SCREEN_H

#include "stm32f10x.h"

//结构体
typedef struct
{
	u8 RunMode;					//运行模式
	u16 Timer;					//定时器
}Screen_DataStructure;





//----------------------------------------------------
void Screen_Main(void);
void Screen_Timer(void);
void Screen_Init(void);
Screen_DataStructure* Screen_GetData(void);

void Screen_ReadData(u8 address,u8 *p,u8 len);
void Screen_StoreData(u8 address,u8 *p,u8 len);
void Screen_DebugASCII(u8 *p,u8 len);


void Screen_DebugASCII(u8 *p,u8 len);
//void Screen_DebugRTU(u8 *p,u8 len);






#endif










