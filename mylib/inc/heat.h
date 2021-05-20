/*
	Heat.c库函数
*/

#ifndef __HEAT_H
#define __HEAT_H

#include "stm32f4xx.h"


//输出
#define HEAT_OUT(a)\
					if (a==0)\
						GPIO_ResetBits(HEAT_PORT,HEAT_PIN);\
					else if (a==1)\
						GPIO_SetBits(HEAT_PORT,HEAT_PIN);\
					else if(a==2)\
						HEAT_PORT->ODR ^= HEAT_PIN;
					

//结构体
typedef struct
{
	u8 RunMode;					//运行模式
	u16 Timer;					//定时器
	u8 Mode;						//模式
	u16 Pwm;
	PID_DataStructure PidData;//PID数据结构体
}Heat_DataStructure;





//----------------------------------------------------
void Heat_Main(void);
void Heat_Timer(void);
void Heat_Init(void);
void Heat_DataInit(void);
void Heat_GpioInit(void);
void Heat_TimerInit(void);
Heat_DataStructure* Heat_GetData(void);
void Heat_OnOff(u8 sw,u16 pwm);

void Heat_ReadData(u8 address,u8 *p,u8 len);
void Heat_StoreData(u8 address,u8 *p,u8 len);
void Heat_DebugASCII(u8 *p,u8 len);


void Heat_DebugASCII(u8 *p,u8 len);
//void Heat_DebugRTU(u8 *p,u8 len);






#endif










