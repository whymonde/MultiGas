/*
	本文件为该项目的屏幕相关函数
	具体函数如下
*/

#include "SPEAK.h"

Speak_DataStructure SpeakData;
/*
	函数名：void Speak_Main(void)
	描述：	SPEAK.c主函数
*/
void Speak_Main(void)
{
	switch(SpeakData.RunMode)
	{ 
		case 0:
		{
			break;
		}
		case 1:  //启动蜂鸣器
		{
			if(SpeakData.Timer==0)
			{
				SpeakData.RunMode=0;
				//关蜂鸣器
				SPEAK_OUT(0);
			}
			break;
		}
		
	}
}
//---
/*
	函数名：void Speak_Timer(void)
	描述：speak.c文件定时器函数，由系统1ms定时器调用
*/
void Speak_Timer(void)
{
	if(SpeakData.Timer>0)
	{
		SpeakData.Timer--;
	}
}
/*
	函数名：void Speak_Buzzer(void)
  参数：timer 鸣叫时间，以1MS为单位
	描述：speak.c文件定时器函数，由系统1ms定时器调用
*/
void Speak_Buzzer(u16 timer)
 { 
	 SPEAK_OUT(1);
	 SpeakData.Timer=timer;
	 SpeakData.RunMode=1;  //启动蜂鸣器
 }
//---
/*
	函数名：void Screen_Init(void)
	描述：	screen.c文件初始化函数
*/
void Speak_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(SPEAK_APP, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = SPEAK_PIN;	 //***********
  GPIO_Init(SPEAK_PORT, &GPIO_InitStructure);	
  SPEAK_OUT(0);

}
//---
/*
	函数名：Speak_DataStructure* Screen_GetData(void)
	描述：获取返回speak.c文件全局变量结构体变量
*/
Speak_DataStructure* Speak_GetData(void)
{
	return &SpeakData;
}//-------------------------数据读取、存储----------------------------
//---
/*
	函数名：void Speak_ReadData(u8 address,u8 *p,u8 len)
	描述：	读取数据
*/
void Speak_ReadData(u8 address,u8 *p,u8 len)
{

}
//---
/*
	函数名：void Speak_StoreData(u8 address,u8 *p,u8 len)
	描述：该函数为存储Speak.c配置参数函数
	说明：读取数据
				擦除数据
				修改数据
				保存数据
*/
void Speak_StoreData(u8 address,u8 *p,u8 len)
{

}
//---
/*
	函数名：void Screen_FactoryReset(void)
	描述：该函数Screen.c存储数据恢复出厂设置
*/
void Speak_FactoryReset(void)
{

}
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Speak_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Screen.c文件的ASCII调试文件
*/
void Speak_DebugASCII(u8 *p,u8 len)
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
	函数名： 	void Speak_DebugRTU((u8 *p)
	描述：		该函数为Speak.c文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*//*
void Speak_DebugRTU(u8 *p,u8 len)
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
