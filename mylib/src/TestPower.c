/*
	���ļ�Ϊ����Ŀ����Ļ��غ���
	���庯������
*/

#include "TestPower.h"

TestPower_DataStructure TestPowerData;
/*
	��������void TestPower_Main(void)
	������	heat.c������
*/
void TestPower_Main(void)
{
	
}
//---
/*
	��������void Screen_Timer(void)
	������screen.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void TestPower_Timer(void)
{
    if(TestPowerData.Timer>0) TestPowerData.Timer--;
	
	//����Ϊ10MS���һ��power ״̬ģ��
	if(TestPowerData.Timer10ms>0)
	{
		TestPowerData.Timer10ms--;
	}
	else  //
	{
		TestPowerData.Timer10ms=10;   //����10MS
		
		//�����Ǽ���ⲿ��Դ
		if(TPOWER_LED)  //����ƽ
			TestPowerData.StaCnt++;
		//�����Ǽ�������Ƿ����
		   if(TCHANG_LED)  //����ƽ
			TestPowerData.ChangCnt++;
		
		TestPowerData.Cnt++;
		if(TestPowerData.Cnt>=100)  //1��ʱ�䵽,1s�ж�һ��
		{   TestPowerData.Cnt=0;
			//����ж�С���ߵ�ƽ�ⲿ��Դ
			if(TestPowerData.StaCnt==100)  //��10���ߵ�ƽ��û��
			{
				TestPowerData.flag = 0;  //�޵�
			}
			else if(TestPowerData.StaCnt==0)
			{
				TestPowerData.flag = 1;//�е�
			}
			TestPowerData.StaCnt=0;
			
			//����ж�С���ߵ�ƽ�ⲿ��Դ
			if(TestPowerData.ChangCnt==100)  //��10���ߵ�ƽ��û��
			{
				TestPowerData.Changflg = 0;  //��ch�����
			}
			else if(TestPowerData.ChangCnt==0)
			{
				TestPowerData.Changflg = 1;//�г����
			}
			TestPowerData.ChangCnt=0;
			
			
			
		}
		
		//�����Ǽ�������Ƿ����
		
		
	}	
}
//---
/*
	��������void Screen_Init(void)
	������	screen.c�ļ���ʼ������
*/
void TestPower_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(TPOWER_APP, ENABLE);
    RCC_AHB1PeriphClockCmd(TCHANG_APP, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = TPOWER_PIN;              //Pb9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //��ͨ����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;         //�����޷���ȡ����ʱ�䣬��������
	GPIO_Init(TPOWER_PORT, &GPIO_InitStructure);                 //��ʼ��

	GPIO_InitStructure.GPIO_Pin = TCHANG_PIN;              //PB8
	GPIO_Init(TCHANG_PORT, &GPIO_InitStructure);                 //��ʼ��
	
	TestPowerData.flag=0;
    TestPowerData.Timer10ms=10;
    TestPowerData.Cnt=0;
    TestPowerData.StaCnt=0;
	
	TestPowerData.Changflg=0;
	TestPowerData.ChangCnt=0;
	
}
//---
/*
	��������Screen_DataStructure* Screen_GetData(void)
	��������ȡ����screen.c�ļ�ȫ�ֱ����ṹ�����
*/
TestPower_DataStructure* TestPower_GetData(void)
{
	return &TestPowerData;
}
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void TestPower_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪTestPower.c�ļ���ASCII�����ļ�
*/
void TestPower_DebugASCII(u8 *p,u8 len)
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
	�������� 	void TestPower_DebugRTU((u8 *p)
	������		�ú���ΪTestPower.c�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*//*
void TestPower_DebugRTU(u8 *p,u8 len)
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





































