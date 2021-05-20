/*
		该文件建立于2017年8月3日
		该文件为"RTU.c"文件的库文件
*/

#ifndef __RTU_H
#define __RTU_H

//包含的头文件
#include "stm32f4xx.h"

//硬件端口宏定义
#define RTU_SendBufferData	 			USARTx_StoreBufferData			//RTU数据发送函数
//电脑端口
#define RTU_UsartComputerPort			USART2											//对应串口输出端口
#define RTU_UsartComputerBuffer		Get_USART2Buffer()					//对应串口结构体
//主板对应串口硬件端口
#define RTU_UsartSlavePort				USART1												//对应串口输出端口
#define RTU_UsartSlaveBuffer			Get_USART1Buffer()						//对应串口结构体

typedef struct
{
//	u8 Error;
//	u8 ErrorCnt;
	u8 RunMode;
	u8 NewMode;
	u16 Timer;					//定时器
}RTU_DataStructure;




void RTU_main(void);
//接收
void RTU_CommunicateReceive(u8 *p,u8 len);//接收并处理来自串口的RTU数据

void RTU_Timer(void);
void RTU_Init(void);
//void RTU_ReceiveSlave(u8 *p,u8 len);
//void RTU_ReadSlaveBase(u8 address);

//发送











#endif














