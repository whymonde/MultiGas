/*
	���ļ�Ϊ����Ŀ����Ļ��غ���
	���庯������
*/

#include "SPEAK.h"

Speak_DataStructure SpeakData;
/*
	��������void Speak_Main(void)
	������	SPEAK.c������
*/
void Speak_Main(void)
{
	switch(SpeakData.RunMode)
	{ 
		case 0:
		{
			break;
		}
		case 1:  //����������
		{
			if(SpeakData.Timer==0)
			{
				SpeakData.RunMode=0;
				//�ط�����
				SPEAK_OUT(0);
			}
			break;
		}
		
	}
}
//---
/*
	��������void Speak_Timer(void)
	������speak.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void Speak_Timer(void)
{
	if(SpeakData.Timer>0)
	{
		SpeakData.Timer--;
	}
}
/*
	��������void Speak_Buzzer(void)
  ������timer ����ʱ�䣬��1MSΪ��λ
	������speak.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void Speak_Buzzer(u16 timer)
 { 
	 SPEAK_OUT(1);
	 SpeakData.Timer=timer;
	 SpeakData.RunMode=1;  //����������
 }
//---
/*
	��������void Screen_Init(void)
	������	screen.c�ļ���ʼ������
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
	��������Speak_DataStructure* Screen_GetData(void)
	��������ȡ����speak.c�ļ�ȫ�ֱ����ṹ�����
*/
Speak_DataStructure* Speak_GetData(void)
{
	return &SpeakData;
}//-------------------------���ݶ�ȡ���洢----------------------------
//---
/*
	��������void Speak_ReadData(u8 address,u8 *p,u8 len)
	������	��ȡ����
*/
void Speak_ReadData(u8 address,u8 *p,u8 len)
{

}
//---
/*
	��������void Speak_StoreData(u8 address,u8 *p,u8 len)
	�������ú���Ϊ�洢Speak.c���ò�������
	˵������ȡ����
				��������
				�޸�����
				��������
*/
void Speak_StoreData(u8 address,u8 *p,u8 len)
{

}
//---
/*
	��������void Screen_FactoryReset(void)
	�������ú���Screen.c�洢���ݻָ���������
*/
void Speak_FactoryReset(void)
{

}
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Speak_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪScreen.c�ļ���ASCII�����ļ�
*/
void Speak_DebugASCII(u8 *p,u8 len)
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
/*
	�������� 	void Speak_DebugRTU((u8 *p)
	������		�ú���ΪSpeak.c�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*//*
void Speak_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//�鿴���ĸ�������
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
