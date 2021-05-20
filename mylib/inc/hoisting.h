/*
	hoisting库函数
*/

#ifndef __HOISTING_H
#define __HOISTING_H

#include "stm32f10x.h"



#define HOISTING_DefaultAddress			0x03		//称重传感器默认地址


//运行模式
#define HOISTING_RunModeNone						0				//什么模式也米有运行
#define HOISTING_RunModeColHoisting			0x10		//采集吊重模式
#define HOISTING_RunModeReadAllData			0x11		//读取内部所有寄存器值模式



//---------------------------------------------------------------
//硬件端口宏定义
#define Hoistin_SendBufferData	 			USARTx_StoreBufferData			//RTU数据发送函数
//采集端口(RS485)
#define Hoisting_UsartPort						USART3											//对应串口输出端口
#define Hoisting_UsartBuffer					Get_USART3Buffer()					//对应串口结构体
#define Hoisting_SetComNoneVerify			USART_SetNoneVerify				//设置端口号为无校验模式



/*
	风速传感器数据结构体
*/
typedef struct
{
//	u8 Flag;					//相关标志位
	u8 Address;				//接收及处理计时器
	u8 RunMode;				
	u32 Weight;				//当前吊重寄存器值
}Hoisting_DataStructure;




//库函数
void Hoisting_Init(void);
void Hoisting_DataInit(void);
Hoisting_DataStructure* Hoisting_GetData(void);
void Hoisting_CollectData(void);
void Hoisting_ReadAllData(void);
void Hoisting_DebugASCII(u8 *p,u8 len);
void Hoisting_DebugRTU(u8 *p,u8 len);



#endif













