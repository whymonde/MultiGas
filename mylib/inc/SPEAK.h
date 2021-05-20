/*
	screen.c库函数
*/

#ifndef __SPEAK_H
#define __SPEAK_H

#include "stm32f4xx.h"
//输出
#define SPEAK_OUT(a)\
					if (a==0)\
						GPIO_ResetBits(SPEAK_PORT,SPEAK_PIN);\
					else if (a==1)\
						GPIO_SetBits(SPEAK_PORT,SPEAK_PIN);\
				
//结构体
typedef struct
{
	u8 RunMode;					//运行模式
	u16 Timer;					//定时器
}Speak_DataStructure;

//----------------------------------------------------
void Speak_Main(void);
void Speak_Timer(void);
void Speak_Init(void);
void Speak_Buzzer(u16 timer);
Speak_DataStructure* Speak_GetData(void);

void Speak_ReadData(u8 address,u8 *p,u8 len);
void Speak_StoreData(u8 address,u8 *p,u8 len);
void Speak_DebugASCII(u8 *p,u8 len);


void Speak_DebugASCII(u8 *p,u8 len);
//void Speak_DebugRTU(u8 *p,u8 len);


#endif










