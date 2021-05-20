/*
	Electric库函数
*/

#ifndef __ELECTRIC_H
#define __ELECTRIC_H

#include "stm32f10x.h"


#define ELECTRIC_DefaultAddress			0x01		//风速传感器默认地址
//通讯命令
#define ELECTRIC_ReadWords					0x03		//读取寄存器数个字命令
#define ELECTRIC_WriteOneWord				0x06		//向目标寄存器写入一个字命令

//OHR-D23寄存器地址
#define ELECTRIC_AddBasicData				0x0100		//基本电量寄存器首地址
#define ELECTRIC_AddEnergyData			0x0600		//电能数据收地址
#define ELECTRIC_AddParameterData		0x0903		//参数首地址
#define ELECTRIC_AddVoltageRatio		0x0903		//电压倍率地址
#define ELECTRIC_AddCurrentRatio		0x0904		//电流倍率地址
#define ELECTRIC_AddWiringWay				0x0905		//接线方式地址
#define ELECTRIC_AddCommAddress			0x0906		//通讯地址地址
#define ELECTRIC_AddBaudRate				0x0907		//波特率地址

#define ELECTRIC_LenBasicData				0x0034	//基本电量寄存器连续个数，26个
#define ELECTRIC_LenEnergyData			0x000C	//电能数据地址,6个
#define ELECTRIC_LenParameterData		0x0005		//参数首地址

//运行模式
#define ELECTRIC_RunModeNone						0				//什么模式也米有运行
#define ELECTRIC_RunModeBasicData				0x10		//读取基本电能参数模式
#define ELECTRIC_RunModeEnergyData			0x11		//读取电能参数模式
#define ELECTRIC_RunModeParameterData		0x12		//读取配置参数模式
#define ELECTRIC_RunModeVoltageRatio		0x20		//写入电压倍率参数模式
#define ELECTRIC_RunModeCurrentRatio		0x21		//写入电流倍率参数模式
#define ELECTRIC_RunModeWiringWay				0x22		//写入接线方式参数模式
#define ELECTRIC_RunModeCommAddress			0x23		//写入通讯地址参数模式
#define ELECTRIC_RunModeBaudRate				0x24		//写入通讯波特率参数模式	


//---------------------------------------------------------------
//硬件端口宏定义
#define Electric_SendBufferData	 			USARTx_StoreBufferData			//RTU数据发送函数
//采集端口(RS485)
#define Electric_UsartPort						USART3											//对应串口输出端口
#define Electric_UsartBuffer					Get_USART3Buffer()					//对应串口结构体
#define Electric_SetComNoneVerify			USART_SetNoneVerify				//设置端口号为无校验模式



/*
	串口收发结构体
*/
typedef struct
{
//	u8 Flag;						//相关标志位
	u8 Address;					//接收长度
	u8 RunMode;					//运行模式，用于判断当前读取是哪些数据，或是写入哪些数据
}Electric_DataStructure;


//库函数
void Electric_Init(void);
void Electric_DataInit(void);
Electric_DataStructure* Electric_GetData(void);
void Electric_CollectBasicData(void);
void Electric_SendReadCmd(u16 address,u16 len);
void Electric_WriteOneWord(u16 address,u16 data);
void Electric_DebugASCII(u8 *p,u8 len);
void Electric_DebugRTU(u8 *p,u8 len);

#endif
