/*
	Electric库函数
*/

#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"

//
#define ENCODER_SlewingAddress			0x0A		//回转编码器默认地址
#define ENCODER_TrolleyingAddress		0x0B		//变幅编码器默认地址
#define ENCODER_HoistingAddress			0x0C		//吊钩高度编码器默认地址



//---------------------------------------------------------------
//硬件端口宏定义
#define Encoder_SendBufferData	 			USARTx_StoreBufferData			//RTU数据发送函数
//A1端口(RS485)
#define Encoder_UsartPort							USART3											//对应串口输出端口
#define Encoder_UsartBuffer						Get_USART3Buffer()					//对应串口结构体
#define Encoder_SetComNoneVerify			USART_SetNoneVerify				//设置端口号为无校验模式

//GPIO口宏定义
//硬件宏定义
#define ENCODER_Slewing_RCC         	RCC_APB2Periph_GPIOA		//回转控制，0=启动控制，1=关闭控制
#define ENCODER_Slewing_PORT        	GPIOA    
#define ENCODER_Slewing_PIN         	GPIO_Pin_10
#define ENCODER_Trolleying_RCC        RCC_APB2Periph_GPIOB		//变幅控制，0=启动控制，1=关闭控制
#define ENCODER_Trolleying_PORT       GPIOA    
#define ENCODER_Trolleying_PIN        GPIO_Pin_9
#define ENCODER_Hoisting_RCC         	RCC_APB2Periph_GPIOA		//吊高控制，0=启动控制，1=关闭控制
#define ENCODER_Hoisting_PORT        	GPIOA    
#define ENCODER_Hoisting_PIN         	GPIO_Pin_11

//输出宏定义
#define ENCODER_SlewingSET()					GPIO_ResetBits(ENCODER_Slewing_PORT, ENCODER_Slewing_PIN)	//打开设置，输出24V
#define ENCODER_SlewingRESET()				GPIO_SetBits(ENCODER_Slewing_PORT, ENCODER_Slewing_PIN)		//关闭设置，输出0V		
#define ENCODER_TrolleyingSET()				GPIO_ResetBits(ENCODER_Trolleying_PORT, ENCODER_Trolleying_PIN)//打开设置，输出24V
#define ENCODER_TrolleyingRESET()			GPIO_SetBits(ENCODER_Trolleying_PORT, ENCODER_Trolleying_PIN)	 //关闭设置，输出0V
#define ENCODER_HoistingSET()					GPIO_ResetBits(ENCODER_Hoisting_PORT, ENCODER_Hoisting_PIN)//打开设置，输出24V
#define ENCODER_HoistingRESET()				GPIO_SetBits(ENCODER_Hoisting_PORT, ENCODER_Hoisting_PIN)	 //关闭设置，输出0V


//运行模式
#define ENCODER_RunModeNone								0				//什么模式也米有运行
#define ENCODER_SlewingRunModeBasic				0x10		//Slewing运行模式基值
#define ENCODER_TrolleyingRunModeBasic		0x30		//Trolleying运行模式基值
#define ENCODER_HoistingRunModeBasic			0x60		//Hoisting运行模式基值

//常量宏定义，编码器的设置命令
#define ENCODER_CmdReadData							0x03		//返回编码器计数值
#define ENCODER_CmdSetAddress						0xA0		//设置编码器地址命令
#define ENCODER_CmdBaudRate4800					0xB0		//设置波特率为4800
#define ENCODER_CmdBaudRate9600					0xB1		//设置波特率为9600
#define ENCODER_CmdBaudRate19200				0xB2		//设置波特率为19200
#define ENCODER_CmdBaudRate57600				0xB3		//设置波特率为57600
#define ENCODER_CmdBaudRate115200				0xB4		//设置波特率为115200
#define ENCODER_CmdReturnSet						0xC7		//读取设定值，返回地址、波特率、方向代码
#define ENCODER_CmdReturnValue					0xC8		//读取编码器当前多圈与单圈数据
#define ENCODER_CmdRetrunBit						0xC9		//读取编码器多圈与单圈位数
#define ENCODER_CmdSet2047Circle				0xCA		//设置编码器当前圈数为2047圈
#define ENCODER_CmdSetCircle						0xD0		//设置编码器圈数
#define ENCODER_CmdClockwiseAdd					0xF1		//顺时针加计数，逆时针减计数
#define ENCODER_CmdAnticlockwiseAdd			0xF2		//逆时针加计数，顺时针减计数


/*
	编码器结构体
*/
typedef struct
{
	u8 Address;				//接收及处理计时器
	u16 ValCircle;		//圈计数
	u16 ValAngle;			//单圈角度
}Encoder_EncoderStructure;
//Encoder.c数据结构体
typedef struct
{
	u8 Flag;		//相关标志位,B0=1编码器1启动传输，B1=1编码器2启动传输，B2=1编码器3启动传输
	u8 RunMode;	//运行模式寄存器
	u8 CollectMode;
	u16 Timer;
	Encoder_EncoderStructure Slewing;		//回转
	Encoder_EncoderStructure Trolleying;//变幅
	Encoder_EncoderStructure Hoisting;//吊高
}Encoder_DataStructure;



//------------------------------------
void Encoder_Init(void);
void Encoder_Timer(void);
void Encoder_GpioInit(void);
void Encoder_EncoderDataInit(void);
//获取地址
Encoder_DataStructure* Encoder_GetData(void);
Encoder_EncoderStructure* Encoder_GetSlewingData(void);
Encoder_EncoderStructure* Encoder_GetTrolleyingData(void);
Encoder_EncoderStructure* Encoder_GetHoistinggData(void);
//设置编码器
void Encoder_CollectValue(Encoder_EncoderStructure *p);
void Encoder_SetAddress(Encoder_EncoderStructure *p,u8 adr);
void Encoder_SetCycle(Encoder_EncoderStructure *p,u8 cycle);
void Encoder_SetCommand(Encoder_EncoderStructure *p,u8 cmd);
u8 Encoder_SetRunMode(u8 cmd);

//调试函数
void Encoder_DebugASCII(u8 *p,u8 len);
void Encoder_DebugRTU(u8 *p,u8 len);

#endif











