/*
	Thbss01库函数
*/

#ifndef __THBSS01_H
#define __THBSS01_H

#include "stm32f10x.h"

//常量宏定义
#define THBSS01_AddressSkewing				0x03		//THSS01地址偏移值

//THBSS01内部配置参数地址
#define THBSS01_AdrBase								0x0000	//基址
#define THBSS01_AdrWeight							THBSS01_AdrBase	//实时重量地址
#define THBSS01_AdrFaMa								0x0008	//砝码重量寄存器地址
#define THBSS01_AdrAddress						0x000A	//校准寄存器地址
#define THBSS01_AdrBaudRate						0x000B	//校准寄存器地址
#define THBSS01_AdrShelling						0x0011	//去皮寄存器地址
#define THBSS01_AdrCalibration				0x0012	//校准寄存器地址


//存储地址
#define THBSS01_AdrStoreBase					W25XXX_AdrTHBSS01Base
#define THBSS01_AdrConfBase						0
#define THBSS01_AdrStoreADR						THBSS01_AdrConfBase
#define THBSS01_AdrStoreBR						THBSS01_AdrStoreADR+1
#define THBSS01_AdrStoreEnd						THBSS01_AdrStoreBR+2




//硬件宏定义
//---------------------------------------------------------------
//硬件端口宏定义
#define THBSS01_SendBufferData	 			USARTx_StoreBufferData			//RTU数据发送函数
//采集端口(RS485)
#define THBSS01_UsartPort							USART3											//对应串口输出端口
#define THBSS01_UsartBuffer()					Get_USART3Buffer()					//对应串口结构体
#define THBSS01_UartModeSet						USART_ModeSet



//THBSS01称重传感器配置参数数据结构体
typedef struct
{
	u8 ADR;//通讯地址
	u16 BR;//通讯波特率
}THBSS01_ConfStructure;

/*
	Thbss01数据结构体
*/
typedef struct
{
	u8 Flag;						//标志位
	u8 RunMode;					//运行模式
	u8 CollectMode;			//采集模式
//	u16 OpMode;					//运行模式
	u16 Timer;					//定时器
	s32 Hosting;				//重量值
	THBSS01_ConfStructure Conf;
}Thbss01_DataStructure;





//----------------------------------------------------
void Thbss01_Main(void);
void Thbss01_Init(void);
void Thbss01_DataInit(void);
void Thbss01_Timer(void);

Thbss01_DataStructure* Thbss01_GetThbss01Data(void);

//数据操作
void Thbss01_Operation(u16 mode,u8 *p,u8 len);
//通讯相关
void Thbss01_ReadData(u16 address,u16 amount);
void Thbss01_WriteOneWord(u16 address,u16 data);
void Thbss01_WriteMultiWord(u16 address,u16 *p,u16 len);
//存储相关
void Thbss01_ReadStoreData(u8 address,u8 *p,u8 len);
void Thbss01_StoreData(u8 address,u8 *p,u8 len);

void Thbss01_UsartModeSet(void);

void Thbss01_DebugASCII(u8 *p,u8 len);
void Thbss01_DebugRTU(u8 *p,u8 len);
void Thbss01_Receive(u8 *p,u8 len);





#endif










