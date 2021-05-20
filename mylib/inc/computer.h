/*
	computer库函数
*/

#ifndef __COMPUTER_H
#define __COMPUTER_H

#include "stm32f10x.h"

#define COMPUTER_ADDREDD				0XF1		//本机与上位机通讯地址


//Computer.c数据结构体
typedef struct
{
	u8 Flag;		//相关标志位,B0=1编码器1启动传输，B1=1编码器2启动传输，B2=1编码器3启动传输
	u8 RunMode;	//运行模式寄存器
	u16 Timer;
//	Encoder_EncoderStructure Slewing;		//回转
//	Encoder_EncoderStructure Trolleying;//变幅
//	Encoder_EncoderStructure Hoisting;//吊高
}Computer_DataStructure;




void Computer_main(void);
void Computer_Init(void);
void Computer_Timer(void);

void Computer_DebugRTU(u8 *p,u8 len);
void Computer_Send(u8 *p,u8 len);







#endif



