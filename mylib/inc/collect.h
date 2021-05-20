/*
	collect库函数
*/

#ifndef __COLLECT_H
#define __COLLECT_H

#include "stm32f10x.h"


/*
	串口收发结构体
*/
typedef struct
{
	u8 RunMode;					//运行模式
	u16 Timer;						//定时器1
	u16 TimerTwo;					//定时器2
}Collect_DataStructure;



//库函数
void Collect_main(void);
void Collect_Init(void);
void Collect_Timer(void);
Collect_DataStructure* Collect_GetCollectData(void);
void Collect_CollectDataInit(void);


void Collect_DebugASCII(u8 *p,u8 len);
void Collect_DebugRTU(u8 *p,u8 len);

#endif
