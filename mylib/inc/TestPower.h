/*
	TestPower.c库函数
*/

#ifndef __TestPower_H
#define __TestPower_H

#include "stm32f4xx_conf.h"

#define TPOWER_LED    PBin(9)
#define TCHANG_LED    PBin(8)

//结构体
typedef struct
{       u8 flag;                                        //电源插电标志
	    u8 RunMode;					//运行模式
	    u16 Timer;					//定时器
        u16 Timer10ms;                                  //10ms定时器
        u16 Cnt;                                        //计数器
        u16 StaCnt;                                     //状态计数器
	 
	    u8 Changflg;                                    //有没有插充电器标志    
	    u16 ChangCnt;                                     //充电器状态计数器
	
	
}TestPower_DataStructure;





//----------------------------------------------------
void TestPower_Main(void);
void TestPower_Timer(void);
void TestPower_Init(void);
TestPower_DataStructure* TestPower_GetData(void);

void TestPower_DebugASCII(u8 *p,u8 len);


void TestPower_DebugASCII(u8 *p,u8 len);
#endif










