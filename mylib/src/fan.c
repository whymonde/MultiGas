/*
	���ļ�Ϊ����Ŀ�ķ�����غ���
	���庯������
*/

#include "fan.h"

Fan_DataStructure FanData;
/*
	��������void Fan_Main(void)
	������	��Ļ��ʾ������
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
	��������void Fan_Timer(void)
	������Fan.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void Fan_Timer(void)
{
	
}
//---
/*
	��������void Fan_Init(void)
	������	Fan.c�ļ���ʼ������
*/
void Fan_Init(void)
{
	Fan_GpioInit();
	Fan_DataInit();
}
//---
/*
	��������void Fan_DataInit(void)
	������	�ú���ΪFan.c�ļ����ݳ�ʼ��
*/
void Fan_DataInit(void)
{
	FanData.RunMode = 1;
}
//---
/*
	��������void Fan_GpioInit(void)
	������	���ȹܽ�GPIO��ʼ��
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
	��������Fan_DataStructure* Fan_GetData(void)
	��������ȡ����Fan.c�ļ�ȫ�ֱ����ṹ��
*/
Fan_DataStructure* Fan_GetData(void)
{
	return &FanData;
}
//---
/*
	��������Fan_Open(void) //
	�������򿪷���
*/
void Fan_Open(void) //�򿪷���
  {
	  FanData.RunMode = 1;
  }
  //---
/*
	��������Fan_Close(void) //
	�������رշ���
*/
void Fan_Close(void) //�رշ���
  {
	 FanData.RunMode = 2; 
  }

//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Fan_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪFan�ļ���ASCII�����ļ�
*/
void Fan_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
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






































