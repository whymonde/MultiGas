/*
	button.c�⺯��
*/

#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f4xx.h"

//����ֵ�궨��
#define BUTTON_NONE						0//��
#define BUTTON_F1						0x10//F1
#define BUTTON_F2						0x11//F2
#define BUTTON_1						0x12//1
#define BUTTON_2						0x13//2
#define BUTTON_3						0x14//3
#define BUTTON_DELETE					0x15//ɾ��

#define BUTTON_F3						0x20//F3
#define BUTTON_F4						0x21//F4
#define BUTTON_4						0x22//4
#define BUTTON_5						0x23//5
#define BUTTON_6						0x24//6
#define BUTTON_UP						0x25//��
#define BUTTON_RIGHT					0x26//��

#define BUTTON_F5						0x30//F5
#define BUTTON_F6						0x31//F6
#define BUTTON_7						0x32//7
#define BUTTON_8		                0x33//8
#define BUTTON_9						0x34//9
#define BUTTON_LEFT		                0x35//��
#define BUTTON_DOWN						0x36//��

#define BUTTON_EN_CN					0x40//��Ӣ
#define BUTTON_MENU						0x41//�˵�
#define BUTTON_POINT					0x42//��
#define BUTTON_0   	                    0x43//0
#define BUTTON_PLUS_MINUS				0x44//��
#define BUTTON_BACK					    0x45//�˳�
#define BUTTON_OK						0x46//ȷ��


/*

#define BUTTON_1						0x10//1
#define BUTTON_2						0x11//2
#define BUTTON_3						0x12//3
#define BUTTON_F1						0x13//F1
#define BUTTON_F2						0x14//F2
#define BUTTON_F3						0x15//F3
#define BUTTON_4						0x20//4
#define BUTTON_5						0x21//5
#define BUTTON_6						0x22//6
#define BUTTON_F4						0x23//F4
#define BUTTON_F5						0x24//F5
#define BUTTON_F6						0x25//F6
#define BUTTON_MENU						0x26//�˵�
#define BUTTON_7						0x30//7
#define BUTTON_8						0x31//8
#define BUTTON_9						0x32//9
#define BUTTON_PLUS_MINUS		        0x33//��
#define BUTTON_UP						0x34//��
#define BUTTON_DELETE					0x35//ɾ��
#define BUTTON_BACK						0x36//�˳�
#define BUTTON_EN_CN					0x40//��Ӣ
#define BUTTON_0						0x41//0
#define BUTTON_POINT					0x42//��
#define BUTTON_LEFT		                0x43//��
#define BUTTON_DOWN						0x44//��
#define BUTTON_RIGHT					0x45//��
#define BUTTON_OK						0x46//ȷ��

*/

//Y1						
#define ButtonY1    					GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
//Y2			
#define ButtonY2    					GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)
//Y3				
#define ButtonY3    					GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)
//Y4			
#define ButtonY4   					    GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)
//Y5			
#define ButtonY5    					GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)
//Y6			
#define ButtonY6   					    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
//Y7				
#define ButtonY7    					GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)


#define ButtonX1(a)\
					if (a==0)\
						GPIO_ResetBits(GPIOC,GPIO_Pin_14);\
					else if (a==1)\
						GPIO_SetBits(GPIOC,GPIO_Pin_14);\
					else if(a==2)\
						GPIOC->ODR ^= GPIO_Pin_13;
#define ButtonX2(a)\
					if (a==0)\
						GPIO_ResetBits(GPIOC,GPIO_Pin_15);\
					else if (a==1)\
						GPIO_SetBits(GPIOC,GPIO_Pin_15);\
					else if(a==2)\
						GPIOC->ODR ^= GPIO_Pin_15;
#define ButtonX3(a)\
					if (a==0)\
						GPIO_ResetBits(GPIOA,GPIO_Pin_1);\
					else if (a==1)\
						GPIO_SetBits(GPIOA,GPIO_Pin_1);\
					else if(a==2)\
						GPIOA->ODR ^= GPIO_Pin_1;
#define ButtonX4(a)\
					if (a==0)\
						GPIO_ResetBits(GPIOA,GPIO_Pin_0);\
					else if (a==1)\
						GPIO_SetBits(GPIOA,GPIO_Pin_0);\
					else if(a==2)\
						GPIOA->ODR ^= GPIO_Pin_0;

#define  X_ALLDOWM ButtonX1(0);ButtonX2(0);ButtonX3(0);ButtonX4(0);
#define  X1_UP     ButtonX1(1);ButtonX2(0);ButtonX3(0);ButtonX4(0);
#define  X2_UP     ButtonX1(0);ButtonX2(1);ButtonX3(0);ButtonX4(0);
#define  X3_UP     ButtonX1(0);ButtonX2(0);ButtonX3(1);ButtonX4(0);
#define  X4_UP     ButtonX1(0);ButtonX2(0);ButtonX3(0);ButtonX4(1);

#define  GET_YDATA (ButtonY1<<0)|(ButtonY2<<1)|(ButtonY3<<2)|(ButtonY4<<3)|(ButtonY5<<4)|(ButtonY6<<5)|(ButtonY7<<6)|(1<<7)
			 					
//button.c�ṹ��
typedef struct
{
	u8 KeyData;					//����ֵ
	u16 Timer;					//��ʱ��
	u8 YData;           //��������
	u8 XData;           //��������
	u8 RunMode;         //��ť����׶� 1�ް���״̬ 2�а�������ȷ����  3�а�������ȷ������ 4���㰴��ֵ
}Button_DataStructure;





//----------------------------------------------------
void Button_Main(void);
void Button_Timer(void);
void Button_Init(void);
void Button_DataInit(void);
void Button_GpioInit(void);
Button_DataStructure* Button_GetData(void);

void Buttom_MainPage(void);

void Button_ReadData(u8 address,u8 *p,u8 len);
void Button_StoreData(u8 address,u8 *p,u8 len);
void Button_DebugASCII(u8 *p,u8 len);
/*
	��������void Buttom_MainPage(void)
	������	�ú���Ϊ�����水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Buttom_UpdataProgramPage(void);
/*
	��������void Buttom_MainPage(void)
	������	�ú���Ϊ�����水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Buttom_PowerOffPage(void);

void Button_DebugASCII(u8 *p,u8 len);
//void Button_DebugRTU(u8 *p,u8 len);






#endif










