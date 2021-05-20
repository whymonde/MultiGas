/*
	Fan库函数
*/

#ifndef __FAN_H
#define __FAN_H

#include "stm32f4xx.h"

//输出
#define FAN_OUT(a)\
					if (a==0)\
						GPIO_ResetBits(FAN_PORT,FAN_PIN);\
					else if (a==1)\
						GPIO_SetBits(FAN_PORT,FAN_PIN);\
					else if(a==2)\
						FAN_PORT->ODR ^= FAN_PIN;


//结构体
typedef struct
{
	u8 RunMode;					//运行模式
	u16 Timer;					//定时器
}Fan_DataStructure;





//----------------------------------------------------
void Fan_Main(void);
void Fan_Timer(void);
void Fan_Init(void);
void Fan_DataInit(void);
void Fan_GpioInit(void);
Fan_DataStructure* Fan_GetData(void);

void Fan_Open(void);  //对外接口
void Fan_Close(void);
void Fan_DebugASCII(u8 *p,u8 len);


void Fan_DebugASCII(u8 *p,u8 len);
//void Fan_DebugRTU(u8 *p,u8 len);






#endif










