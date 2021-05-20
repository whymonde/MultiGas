/*
	encoder库函数
*/

#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"





//Encoder.c数据结构体
typedef struct
{
	u8 Flag;		//相关标志位,B0=1编码器1启动传输，B1=1编码器2启动传输，B2=1编码器3启动传输
	u8 RunMode;	//运行模式寄存器
	u8 Address;			//地址
	u16 Timer;				//定时器
	u16 ValCircle;		//圈计数
	u16 ValAngle;			//单圈角度
	u16 ZeroValCircle;		//圈计数
	u16 ZeroValAngle;			//单圈角度
}Encoder_DataStructure;
















#endif


