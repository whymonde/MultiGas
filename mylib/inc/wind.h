/*
	wind库函数
*/

#ifndef __WIND_H
#define __WIND_H

#include "stm32f10x.h"


#define WIND_DefaultAddress			0x02		//风速传感器默认地址

//运行模式
#define YCFS260_ComModeNone						0				//什么模式也米有运行
#define YCFS260_ComModeReadWind				0x10		//运行在读取风速模式
#define YCFS260_ComModeReadAddress		0x11		//运行在读取地址模式
#define YCFS260_ComModeSetAddress			0x12		//运行在修改地址模式
#define YCFS260_ComModeSetBuadRate		0x13		//运行在修改波特率模式
//常量宏定义
#define WIND_CmdBaudRate2400				0x01		//设置风速传感器波特率为2400命令
#define WIND_CmdBaudRate4800				0x02		//设置风速传感器波特率为4800命令
#define WIND_CmdBaudRate9600				0x03		//设置风速传感器波特率为9600命令

//---------------------------------------------------------------
//硬件端口宏定义
#define YCFS260_SendBufferData	 	USARTx_StoreBufferData			//RTU数据发送函数
//采集端口(RS485)
#define YCFS260_UsartPort					USART3											//对应串口输出端口
#define YCFS260_UsartBuffer				Get_USART3Buffer()					//对应串口结构体
#define YCFS260_SetComNoneVerify	USART_SetNoneVerify				//设置端口号为无校验模式
#define YCFS260_UsartPort					USART3											//对应串口输出端口
#define YCFS260_UartModeSet				USART_ModeSet


//存储地址
//温度参数存储地址
#define YCFS260_AdrStoreBase				W25XXX_AdrYCFS260Base
#define YCFS260_AdrStoreTXDZ				0//通讯地址，1个字节
#define YCFS260_AdrStoreTXBTL				YCFS260_AdrStoreTXDZ+1//通讯波特率，2个字节
#define YCFS260_AdrStoreEND					YCFS260_AdrStoreTXBTL+2				



/*
	风速传感器数据结构体
*/
typedef struct
{
	u8 RunMode;				//当前运行模式
	u8 ComMode;				//通讯模式
	u16 Timer;				//定时器
	u8 Address;				//接收及处理计时器
	u16 BaudRate;			//波特率
	u16 Speed;				//当前风速值
}Wind_DataStructure;


//库函数
void YCFS260_Communicate(void);
void YCFS260_Timer(void);

void Wind_Init(void);
void Wind_DataInit(void);
Wind_DataStructure* Wind_GetWindData(void);
void YCFS260_CollectWindSpeed(void);
void Wind_ReadCommAddress(void);
void Wind_SetCommAddress(u8 address);
void Wind_SetCommBaudRate(u8 baudrate);


void YCFS260_Operation(u16 mode,u8 *p,u8 len);
void YCFS260_ReadStoreData(u8 address,u8 *p,u8 len);
void YCFS260_StoreData(u8 address,u8 *p,u8 len);

void YCFS260_UsartModeSet(void);

//调试函数
void Wind_DebugASCII(u8 *p,u8 len);
void YCFS260_DebugRTU(u8 *p,u8 len);

#endif














