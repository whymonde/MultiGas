/*
	MCH58库函数
*/

#ifndef __MCH58_H
#define __MCH58_H

#include "stm32f10x.h"

//硬件端口宏定义
#define MCH58_SendBufferData	 			USARTx_StoreBufferData			//RTU数据发送函数
//采集端口(RS485)
#define MCH58_UsartPort							USART3											//对应串口输出端口
#define MCH58_UsartBuffer()					Get_USART3Buffer()					//对应串口结构体
#define MCH58_UartModeSet						USART_ModeSet
#define MCH58_SetComNoneVerify			USART_SetNoneVerify				//设置端口号为无校验模式


//运行模式
#define MCH58_RunModeNone								0				//什么模式也米有运行
#define MCH58_Con1RunModeBasic				0x10		//Slewing运行模式基值
#define MCH58_Con2RunModeBasic				0x30		//Trolleying运行模式基值
#define MCH58_Con3RunModeBasic				0x60		//Hoisting运行模式基值

//常量宏定义，编码器的设置命令
#define MCH58_CmdReadData							0x03		//返回编码器计数值
#define MCH58_CmdSetAddress						0xA0		//设置编码器地址命令
#define MCH58_CmdBaudRate4800					0xB0		//设置波特率为4800
#define MCH58_CmdBaudRate9600					0xB1		//设置波特率为9600
#define MCH58_CmdBaudRate19200				0xB2		//设置波特率为19200
#define MCH58_CmdBaudRate57600				0xB3		//设置波特率为57600
#define MCH58_CmdBaudRate115200				0xB4		//设置波特率为115200
#define MCH58_CmdReturnSet						0xC7		//读取设定值，返回地址、波特率、方向代码
#define MCH58_CmdReturnValue					0xC8		//读取编码器当前多圈与单圈数据
#define MCH58_CmdRetrunBit						0xC9		//读取编码器多圈与单圈位数
#define MCH58_CmdSet2047Circle				0xCA		//设置编码器当前圈数为2047圈
#define MCH58_CmdSetCircle						0xD0		//设置编码器圈数
#define MCH58_CmdClockwiseAdd					0xF1		//顺时针加计数，逆时针减计数
#define MCH58_CmdAnticlockwiseAdd			0xF2		//逆时针加计数，顺时针减计数


//存储地址
//塔机参数存储地址
#define MCH58_AdrStoreBase					W25XXX_AdrMCH58Base
#define MCH58_AdrStoreBM1Base				0
#define MCH58_AdrStoreBM1ZeroH			MCH58_AdrStoreBM1Base				//编码器1零点多圈值
#define MCH58_AdrStoreBM1ZeroL			MCH58_AdrStoreBM1ZeroH+2		//编码器1零点单圈值
#define MCH58_AdrStoreBM1CD					MCH58_AdrStoreBM1ZeroL+2		//编码器1计数方向
#define MCH58_AdrStoreBM1YSSB				MCH58_AdrStoreBM1CD+1				//编码器1采集映射设备
#define MCH58_AdrStoreBM1ADR				MCH58_AdrStoreBM1YSSB+1			//编码器1设备地址
#define MCH58_AdrStoreBM1BD					MCH58_AdrStoreBM1ADR+1			//编码器1波特率
#define MCH58_AdrStoreBM1END				MCH58_AdrStoreBM1BD+2				//编码器1末端

#define MCH58_AdrStoreBM2Base				MCH58_AdrStoreBM1END
#define MCH58_AdrStoreBM2ZeroH			MCH58_AdrStoreBM2Base				//编码器2零点多圈值
#define MCH58_AdrStoreBM2ZeroL			MCH58_AdrStoreBM2ZeroH+2		//编码器2零点单圈值
#define MCH58_AdrStoreBM2CD					MCH58_AdrStoreBM2ZeroL+2		//编码器2计数方向
#define MCH58_AdrStoreBM2YSSB				MCH58_AdrStoreBM2CD+1				//编码器2采集映射设备
#define MCH58_AdrStoreBM2ADR				MCH58_AdrStoreBM2YSSB+1			//编码器2设备地址
#define MCH58_AdrStoreBM2BD					MCH58_AdrStoreBM2ADR+1			//编码器2波特率
#define MCH58_AdrStoreBM2END				MCH58_AdrStoreBM2BD+2				//编码器2末端

#define MCH58_AdrStoreBM3Base				MCH58_AdrStoreBM2END
#define MCH58_AdrStoreBM3ZeroH			MCH58_AdrStoreBM3Base				//编码器3零点多圈值
#define MCH58_AdrStoreBM3ZeroL			MCH58_AdrStoreBM3ZeroH+2		//编码器3零点单圈值
#define MCH58_AdrStoreBM3CD					MCH58_AdrStoreBM3ZeroL+2		//编码器3计数方向
#define MCH58_AdrStoreBM3YSSB				MCH58_AdrStoreBM3CD+1				//编码器3采集映射设备
#define MCH58_AdrStoreBM3ADR				MCH58_AdrStoreBM3YSSB+1			//编码器3设备地址
#define MCH58_AdrStoreBM3BD					MCH58_AdrStoreBM3ADR+1			//编码器3波特率
#define MCH58_AdrStoreBM3END				MCH58_AdrStoreBM3BD+2				//编码器3末端


#define MCH58_AdrStoreEnd						100			//

#define MCH58_PWR_ON()				GPIO_ResetBits(PwrV24_PWR_PORT, PwrV24_PWR_PIN)	//打开
#define MCH58_PWR_OFF()				GPIO_SetBits(PwrV24_PWR_PORT, PwrV24_PWR_PIN)		//关闭
#define MCH58_Con1_ON()				GPIO_ResetBits(MCH58_Con1_PORT, MCH58_Con1_PIN)	//打开
#define MCH58_Con1_OFF()			GPIO_SetBits(MCH58_Con1_PORT, MCH58_Con1_PIN)		//关闭
#define MCH58_Con2_ON()				GPIO_ResetBits(MCH58_Con2_PORT, MCH58_Con2_PIN)	//打开
#define MCH58_Con2_OFF()			GPIO_SetBits(MCH58_Con2_PORT, MCH58_Con2_PIN)		//关闭
#define MCH58_Con3_ON()				GPIO_ResetBits(MCH58_Con3_PORT, MCH58_Con3_PIN)	//打开
#define MCH58_Con3_OFF()			GPIO_SetBits(MCH58_Con3_PORT, MCH58_Con3_PIN)		//关闭

/*
	MCH58文件结构体
*/
typedef struct
{
	u8 Address;				//地址
	u16 BaudRate;			//波特率
	u8 Map;						//映射
	u32 CodedValue;		//编码值
	u32 ZeroValue;		//零点值
}MCH58_Structure;

/*
	MCH58文件结构体
*/
typedef struct
{
	u8 Flag;						//标志位
	u8 RunMode;					//运行模式
	u8 CollectMode;
	u16 Timer;					//定时器
	MCH58_Structure Mch58[3];
}MCH58_DataStructure;





//----------------------------------------------------
void MCH58_Main(void);
void MCH58_Init(void);
void MCH58_DataInit(void);
void MCH58_GpioInit(void);
void MCH58_Timer(void);
MCH58_DataStructure* MCH58_GetMCH58Data(void);
void MCH58_CollectValue(MCH58_Structure *p);
u8 MCH58_SetCollectMode(u8 cmd);

//数据操作
void MCH58_Operation(u16 mode,u8 *p,u8 len);

void MCH58_UsartModeSet(MCH58_Structure *p);
//存储函数
void MCH58_ReadStoreData(u8 address,u8 *p,u8 len);
void MCH58_StoreData(u8 address,u8 *p,u8 len);

//调试函数
void MCH58_DebugASCII(u8 *p,u8 len);
void MCH58_DebugRTU(u8 *p,u8 len);
void MCH58_Receive(u8 *p,u8 len);





#endif










