/*
	本文件为该项目的屏幕相关函数
	具体函数如下
*/

#include "TestPower.h"

TestPower_DataStructure TestPowerData;
/*
	函数名：void TestPower_Main(void)
	描述：	heat.c主函数
*/
void TestPower_Main(void)
{
	
}
//---
/*
	函数名：void Screen_Timer(void)
	描述：screen.c文件定时器函数，由系统1ms定时器调用
*/
void TestPower_Timer(void)
{
    if(TestPowerData.Timer>0) TestPowerData.Timer--;
	
	//以下为10MS检测一次power 状态模型
	if(TestPowerData.Timer10ms>0)
	{
		TestPowerData.Timer10ms--;
	}
	else  //
	{
		TestPowerData.Timer10ms=10;   //定义10MS
		
		//以下是检测外部电源
		if(TPOWER_LED)  //检测电平
			TestPowerData.StaCnt++;
		//以下是检测充电器是否插上
		   if(TCHANG_LED)  //检测电平
			TestPowerData.ChangCnt++;
		
		TestPowerData.Cnt++;
		if(TestPowerData.Cnt>=100)  //1秒时间到,1s判断一次
		{   TestPowerData.Cnt=0;
			//检测有多小个高电平外部电源
			if(TestPowerData.StaCnt==100)  //有10个高电平就没电
			{
				TestPowerData.flag = 0;  //无电
			}
			else if(TestPowerData.StaCnt==0)
			{
				TestPowerData.flag = 1;//有电
			}
			TestPowerData.StaCnt=0;
			
			//检测有多小个高电平外部电源
			if(TestPowerData.ChangCnt==100)  //有10个高电平就没电
			{
				TestPowerData.Changflg = 0;  //无ch充电器
			}
			else if(TestPowerData.ChangCnt==0)
			{
				TestPowerData.Changflg = 1;//有充电器
			}
			TestPowerData.ChangCnt=0;
			
			
			
		}
		
		//以下是检测充电器是否插上
		
		
	}	
}
//---
/*
	函数名：void Screen_Init(void)
	描述：	screen.c文件初始化函数
*/
void TestPower_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TPOWER_APP, ENABLE);
    RCC_AHB1PeriphClockCmd(TCHANG_APP, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = TPOWER_PIN;              //Pb9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //普通输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;         //上拉无法读取任意时间，下拉可以
	GPIO_Init(TPOWER_PORT, &GPIO_InitStructure);                 //初始化

	GPIO_InitStructure.GPIO_Pin = TCHANG_PIN;              //PB8
	GPIO_Init(TCHANG_PORT, &GPIO_InitStructure);                 //初始化
	
	TestPowerData.flag=0;
    TestPowerData.Timer10ms=10;
    TestPowerData.Cnt=0;
    TestPowerData.StaCnt=0;
	
	TestPowerData.Changflg=0;
	TestPowerData.ChangCnt=0;
	
}
//---
/*
	函数名：Screen_DataStructure* Screen_GetData(void)
	描述：获取返回screen.c文件全局变量结构体变量
*/
TestPower_DataStructure* TestPower_GetData(void)
{
	return &TestPowerData;
}
//-------------------------调试函数----------------------------
//---
/*
	函数名： void TestPower_DebugASCII(u8 *p,u8 len)
	描述：		该函数为TestPower.c文件的ASCII调试文件
*/
void TestPower_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:
		{
			break;
		}
		case 2:
		{
			break;
		}
	}
}
//---
/*
	函数名： 	void TestPower_DebugRTU((u8 *p)
	描述：		该函数为TestPower.c文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*//*
void TestPower_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//查看是哪个功能码
	{
		case 0x01:
		{
			break;
		}
		case 0x02:
		{
			break;
		}
	}
}*/
//---





































