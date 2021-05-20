/*
	本文件为calculate.c库文件
	本文件包括以下函数
	u16 charTOu16(u8 *p,u8 len)	//将字符串转换成16位的无符号整形数据
	
*/
#ifndef __CALCULATE_H
#define __CALCULATE_H
#include "stm32f4xx.h"



unsigned short CRC16(unsigned char *puchMsg,unsigned short usDataLen);	//标准CRC算法
u8 AnalysisCRC(u8 *pstr);//比较数据与CRC是否相等
unsigned int Calculate_CRC16(unsigned char const * pucY, unsigned char ucX);//标准CRC算法，不带字节表
u16 Calculate_CharToU16(u8 *p,u8 len);	//将字符串转换成16位的无符号整形数据
u32 Calculate_CharToU32(u8 *p,u8 len);	//将字符串转换成32位的无符号整形数据
//u16 Calculate_HexStrToU16(u8 *p,u8 len);//该函数为将十六进制字符串转换为无符号16进制数
u8 BeforeToBafter(u8 buffer);	//该函数用于将一个8位的无符号数据的每一位前后对换，b0-b7,b1-b6·····
float Calculate_CharToFloat(u8 *p,u8 len);//该函数用于将一串数字字符转换为浮点型数据
void Calculate_TimeToFrequency(u32 *buffer);//将周期换算成频率
void Calculate_FourBigEndToSmallEnd(FourBytes *p);//四字节空间的大小端转换
void Pid_Calculate(PID_DataStructure* p);
void Calculate_DebugASCII(u8 *p,u8 len);	//该函数为Calculate.c文件的调试函数，由系统调试函数调用
u32  JiCai_Own(u8 year,u8 month,u8 day,u8 hour,u8 min,u8 sec);  //计算掉电到上电时间差
float  Calc_O2_K(float biaodinzhi,u32 ad);   //计算氧气的常数K;
float  Calc_O2(float K,u32 ad);   //计算氧气浓度;
float  Calc_SO2_K(float biaodinzhi,u32 ad);   //计算氧气的常数K;
float  Calc_SO2(float K,u32 ad);   //计算氧气浓度;

#endif

