/*
	本文件为该项目的相关函数
	具体函数如下

*/

#include "led.h"


void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(PRO_LED1_RCC_GPIO, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = PRO_LED1_GPIO_PIN;
  GPIO_Init(PRO_LED1_GPIO_PORT, &GPIO_InitStructure);
	PRO_LED1(0)
}

/*
	函数名：void LED_Timer(void)
	描述：LED流水灯控制
*/
u16 LED_Tim=0;
void LED_Timer(void)//1ms定时器调用
{
}
//---

//-------------------------调试函数----------------------------
//---
/*
	函数名： void Project_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Porject文件的ASCII调试文件
						:000|80|001|xxx······
*/
void LED_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		default :
		{
			printf("LED.c接收到无效调试指令%u!\r\n",cmd);
			break;
		}
	}
}
//---
/*
	函数名： void Project_DebugRTU((u8 *p)
	描述：		该函数为Project文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*/
void LED_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//查看是哪个功能码
	{
	}
}
//---










