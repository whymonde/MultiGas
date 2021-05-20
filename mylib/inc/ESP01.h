/*
	screen库函数
*/

#ifndef __ESP01_H
#define __ESP01_H

#include "stm32f10x.h"


//宏定义
//硬件端口宏定义
#define ESP01_SendBufferData	 		USARTx_StoreBufferData			//RTU数据发送函数
//采集端口(RS485)
#define ESP01_UsartPort						USART2											//对应串口输出端口
#define ESP01_UsartBuffer()				Get_USART2Buffer()					//对应串口结构体
#define ESP01_SetComNoneVerify		USART_SetNoneVerify				//设置端口号为无校验模式
#define ESP01_UartModeSet					USART_ModeSet



/*
	串口收发结构体
*/
typedef struct
{
	u8 Flag;						//标志位，B0=0启动通讯，B0=1接收到模块数据；B7=1模块连接上服务器
	u8 RunMode;					//运行模式
	u8 WorkMode;				//运行模式
	u8 *RxBf;
	u8 RxLen;
	u8 ErrorCnt;				//错误统计
	u16 Timer;					//定时器
}ESP01_DataStructure;





//----------------------------------------------------
void ESP01_Main(void);
void ESP01_Work(ESP01_DataStructure *p);
void ESP01_Init(void);
void ESP01_Timer(void);
ESP01_DataStructure* ESP01_GetData(void);

void ESP01_SendData(u8 *p,u8 len,ESP01_DataStructure *ESP);

void ESP01_DebugASCII(u8 *p,u8 len);
void ESP01_DebugRTU(u8 *p,u8 len);
void ESP01_Receive(u8 *p,u8 len);





#endif










