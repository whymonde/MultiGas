/*
	���ļ�Ϊ����Ŀ����غ���
	���庯������

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
	��������void LED_Timer(void)
	������LED��ˮ�ƿ���
*/
u16 LED_Tim=0;
void LED_Timer(void)//1ms��ʱ������
{
}
//---

//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Project_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪPorject�ļ���ASCII�����ļ�
						:000|80|001|xxx������������
*/
void LED_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		default :
		{
			printf("LED.c���յ���Ч����ָ��%u!\r\n",cmd);
			break;
		}
	}
}
//---
/*
	�������� void Project_DebugRTU((u8 *p)
	������		�ú���ΪProject�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*/
void LED_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//�鿴���ĸ�������
	{
	}
}
//---










