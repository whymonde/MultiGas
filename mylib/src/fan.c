/*
	本文件为该项目的风扇相关函数
	具体函数如下
*/

#include "fan.h"

Fan_DataStructure FanData;
/*
	函数名：void Fan_Main(void)
	描述：	屏幕显示主函数
*/
void Fan_Main(void)
{
	switch(FanData.RunMode)
	{
		case 0:
			break;
		case 1:
		{
			FAN_OUT(1);
			FanData.RunMode=0;
			break;
		}
		case 2:
		{
			FAN_OUT(0);
			FanData.RunMode=0;
			break;
		}
		default:
		{
			FanData.RunMode = 1;
			break;
		}
	}
}
//---
/*
	函数名：void Fan_Timer(void)
	描述：Fan.c文件定时器函数，由系统1ms定时器调用
*/
void Fan_Timer(void)
{
	
}
//---
/*
	函数名：void Fan_Init(void)
	描述：	Fan.c文件初始化函数
*/
void Fan_Init(void)
{
	Fan_GpioInit();
	Fan_DataInit();
}
//---
/*
	函数名：void Fan_DataInit(void)
	描述：	该函数为Fan.c文件数据初始化
*/
void Fan_DataInit(void)
{
	FanData.RunMode = 1;
}
//---
/*
	函数名：void Fan_GpioInit(void)
	描述：	风扇管脚GPIO初始化
*/
void Fan_GpioInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(FAN_APP, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = FAN_PIN;	
  GPIO_Init(FAN_PORT, &GPIO_InitStructure);
  FAN_OUT(1);

}
//---
/*
	函数名：Fan_DataStructure* Fan_GetData(void)
	描述：获取返回Fan.c文件全局变量结构体
*/
Fan_DataStructure* Fan_GetData(void)
{
	return &FanData;
}
//---
/*
	函数名：Fan_Open(void) //
	描述：打开风扇
*/
void Fan_Open(void) //打开风扇
  {
	  FanData.RunMode = 1;
  }
  //---
/*
	函数名：Fan_Close(void) //
	描述：关闭风扇
*/
void Fan_Close(void) //关闭风扇
  {
	 FanData.RunMode = 2; 
  }

//-------------------------调试函数----------------------------
//---
/*
	函数名： void Fan_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Fan文件的ASCII调试文件
*/
void Fan_DebugASCII(u8 *p,u8 len)
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






































