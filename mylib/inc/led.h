/*
	LED¿âº¯Êý
*/

#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"


#define RELAY11_OPEN					GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define RELAY11_CLOSE					GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define RELAY12_OPEN					GPIO_ResetBits(GPIOA, GPIO_Pin_12)
#define RELAY12_CLOSE					GPIO_SetBits(GPIOA, GPIO_Pin_12)
#define OUT24V1_OPEN					GPIO_ResetBits(GPIOA, GPIO_Pin_10)
#define OUT24V1_CLOSE					GPIO_SetBits(GPIOA, GPIO_Pin_10)
#define OUT24V2_OPEN					GPIO_ResetBits(GPIOA, GPIO_Pin_9)
#define OUT24V2_CLOSE					GPIO_SetBits(GPIOA, GPIO_Pin_9)
#define OUT24V3_OPEN					GPIO_ResetBits(GPIOA, GPIO_Pin_11)
#define OUT24V3_CLOSE					GPIO_SetBits(GPIOA, GPIO_Pin_11)
#define DS18B20PWR_OPEN				GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define DS18B20PWR_CLOSE			GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define DS18B20TX_OPEN				GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define DS18B20TX_CLOSE				GPIO_ResetBits(GPIOB, GPIO_Pin_0)



#define LED2(a)\
					if (a==1)\
						GPIO_SetBits(GPIO_LED2_PORT,GPIO_LED2_PIN);\
					else if (a==0)\
						GPIO_ResetBits(GPIO_LED2_PORT,GPIO_LED2_PIN);\
					else if(a==2)\
						GPIO_LED2_PORT->ODR ^= GPIO_LED2_PIN;	
#define LED3(a)\
					if (a==1)\
						GPIO_SetBits(GPIO_LED3_PORT,GPIO_LED3_PIN);\
					else if (a==0)\
						GPIO_ResetBits(GPIO_LED3_PORT,GPIO_LED3_PIN);\
					else if(a==2)\
						GPIO_LED3_PORT->ODR ^= GPIO_LED3_PIN;	
					


void LED_Init(void);
void LED_Timer(void);

void LED_DebugASCII(u8 *p,u8 len);
void LED_DebugRTU(u8 *p,u8 len);



#endif




