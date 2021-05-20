/*
		该文件建立于2017年8月3日
		该文件为"ASCII.c"文件的库文件
*/

#ifndef __ASCII_H
#define __ASCII_H

//包含的头文件
#include "stm32f4xx.h"

//硬件端口宏定义
#define ASCII_SendBufferData				USARTx_StoreBufferData			//RTU数据发送函数
//电脑端口
#define ASCII_UsartCompuerPort			USART1											//对应串口输出端口
#define ASCII_UsartCompuerBuffer		Get_USART1Buffer()					//对应串口结构体
//主板对应串口硬件端口
#define ASCII_UsartSlavePort				UART7												//对应串口输出端口
#define ASCII_UsartSlaveBuffer			Get_USART1Buffer()						//对应串口结构体






//函数
void ASCII_CommunicateReceive(u8 *p,u8 len);//调试函数






#endif
