/*
	���ļ�Ϊ����Ŀ�İ�����غ���
	���庯������
*/

#include "button.h"

//#define BUTTON_R					42//��
//#define BUTTON_ESC		            43//��
//#define BUTTON_L					44//��
//#define BUTTON_SW					45//��

Button_DataStructure ButtonData;
/*
	��������void Button_Main(void)
	������	button.c������
*/
void Button_Main(void)
{
}
//---
/*
	��������void Button_Timer(void)
	������button.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void Button_Timer(void)
{
//	if(ButtonData.Timer != 0) ButtonData.Timer--;
	ButtonData.Timer--;
	
	if(ButtonData.Timer == 0)
	{ 
		ButtonData.Timer = 40;
		u8 Ydata;
		switch(ButtonData.RunMode)
		{
			case 1: //��������
			{
				X_ALLDOWM;
				ButtonData.KeyData=0;
				ButtonData.XData=0; 
				ButtonData.XData=0;
				Ydata=GET_YDATA;
				if(Ydata!=0xff)//�а���
				{
				   ButtonData.RunMode=2;//������һ������ģʽ ȡ������ֵ
				}
			//	printf("YֵYData=0X%2x\r\n", Ydata);
				break;
			}
			case 2:
			{  //ȡ��������ֵ
				Ydata=GET_YDATA;
				if(Ydata==0xff)  //�ް���  ȥ������
				{
					ButtonData.RunMode=1;
				}else
				{
					//Y1~Y7 code 0xfe 0xfd 0xfc 0xf7 0xef 0xdf 0xcf
					ButtonData.YData=Ydata;
					ButtonData.RunMode=3;
					ButtonData.Timer = 1; //��ʱ10ms�������ֵ
				}
				break;
			}
			case 3:
			{//������ֵ
				X1_UP;	
				Ydata=GET_YDATA;				
				if(Ydata==0xff)
				{
					ButtonData.XData=0x01;
					ButtonData.RunMode=4;
					break;
				}
				X2_UP;	 
				Ydata=GET_YDATA;
				if(Ydata==0xff)
				{
					ButtonData.XData=0x02;
					ButtonData.RunMode=4;
					break;
				}
				X3_UP;	 
				Ydata=GET_YDATA;
				if(Ydata==0xff)
				{
					ButtonData.XData=0x03;
					ButtonData.RunMode=4;
					break;
				}
				X4_UP;	 
				Ydata=GET_YDATA;
				if(Ydata==0xff)
				{
					ButtonData.XData=0x04;
					ButtonData.RunMode=4;
					break;
				}
				break;
			}
			case 4:
			{//���㰴��ֵ		
				int i,data;
				data=ButtonData.YData;
				for(i=0;i<7;i++)
				{
					if((data&0x01)==0x00)
					{
						break;
					}
					data>>=1;				 
				}
				ButtonData.KeyData=(ButtonData.XData)*0x10+i;
				ButtonData.RunMode=5;
			}
			case 5: 
			{ //��������
				printf("����ֵXData=0X%2x, YData=0X%2x", ButtonData.KeyData,ButtonData.YData);
				printf("����ֵKeyData=0X%2x", ButtonData.KeyData);
				ButtonData.RunMode=1;
				ButtonData.Timer=200;
				
				if(Screen_GetData()->ScreenButtonLock==2)  //������
				ButtonData.KeyData=0;
				
				Speak_Buzzer(100);
				break;
			}
			default:
			{
				ButtonData.RunMode = 1;
				break;
			}
		}
	}
}
//---
/*
	��������void Button_Init(void)
	������	button.c�ļ���ʼ������
*/
void Button_Init(void)
{
	Button_GpioInit();
	Button_DataInit();
}
//---
/*
	��������void Button_DataInit(void)
	������	�ú���Ϊbutton.c�ļ����ݳ�ʼ��
*/
void Button_DataInit(void)
{
	ButtonData.KeyData = 0;
	ButtonData.Timer = 20;
    ButtonData.RunMode=1;
	ButtonData.XData=0;
	ButtonData.YData=0;
}
//---
/*
	��������void Button_GpioInit(void)
	�����������˿ڳ�ʼ��
*/
void Button_GpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE, ENABLE); 
	//Y ���ó���������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_13;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13;		
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_7);
	GPIO_SetBits(GPIOE,GPIO_Pin_9);
	//X ���ó����
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_OUT;
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
//---
/*
	��������Button_DataStructure* Button_GetData(void)
	��������ȡ����button.c�ļ�ȫ�ֱ����ṹ�����
*/
Button_DataStructure* Button_GetData(void)
{
	return &ButtonData;
}
//---
//-------------------------���ݶ�ȡ���洢----------------------------
//---
/*
	��������void Button_ReadData(u8 address,u8 *p,u8 len)
	������	��ȡ����
*/
void Button_ReadData(u8 address,u8 *p,u8 len)
{
}
//---
/*
	��������void Button_StoreData(u8 address,u8 *p,u8 len)
	�������ú���Ϊ�洢button.c���ò�������
	˵������ȡ����
				��������
				�޸�����
				��������
*/
void Button_StoreData(u8 address,u8 *p,u8 len)
{
}
//---
/*
	��������void Button_FactoryReset(void)
	�������ú���Button.c�洢���ݻָ���������
*/
void Button_FactoryReset(void)
{
	
}
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Button_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪButton.c�ļ���ASCII�����ļ�
	���	 :000|06|XXX|XXX
*/
void Button_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 101:
		{//����ButtonData.KeyDataֵ
//			ButtonData.KeyData = Calculate_CharToU16(p+4,3);
			Screen_GetData()->Key.KeyData = Calculate_CharToU16(p+4,3);
			printf("���õ���KeyDataֵΪ��%u\r\n",Screen_GetData()->Key.KeyData);
			
			break;
		}
		case 102:
		{
			break;
		}
	}
}
//---
/*
	�������� 	void Button_DebugRTU((u8 *p)
	������		�ú���ΪButton.c�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*//*
void Button_DebugRTU(u8 *p,u8 len)
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





































