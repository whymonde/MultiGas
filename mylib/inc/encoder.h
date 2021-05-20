/*
	Electric�⺯��
*/

#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"

//
#define ENCODER_SlewingAddress			0x0A		//��ת������Ĭ�ϵ�ַ
#define ENCODER_TrolleyingAddress		0x0B		//���������Ĭ�ϵ�ַ
#define ENCODER_HoistingAddress			0x0C		//�����߶ȱ�����Ĭ�ϵ�ַ



//---------------------------------------------------------------
//Ӳ���˿ں궨��
#define Encoder_SendBufferData	 			USARTx_StoreBufferData			//RTU���ݷ��ͺ���
//A1�˿�(RS485)
#define Encoder_UsartPort							USART3											//��Ӧ��������˿�
#define Encoder_UsartBuffer						Get_USART3Buffer()					//��Ӧ���ڽṹ��
#define Encoder_SetComNoneVerify			USART_SetNoneVerify				//���ö˿ں�Ϊ��У��ģʽ

//GPIO�ں궨��
//Ӳ���궨��
#define ENCODER_Slewing_RCC         	RCC_APB2Periph_GPIOA		//��ת���ƣ�0=�������ƣ�1=�رտ���
#define ENCODER_Slewing_PORT        	GPIOA    
#define ENCODER_Slewing_PIN         	GPIO_Pin_10
#define ENCODER_Trolleying_RCC        RCC_APB2Periph_GPIOB		//������ƣ�0=�������ƣ�1=�رտ���
#define ENCODER_Trolleying_PORT       GPIOA    
#define ENCODER_Trolleying_PIN        GPIO_Pin_9
#define ENCODER_Hoisting_RCC         	RCC_APB2Periph_GPIOA		//���߿��ƣ�0=�������ƣ�1=�رտ���
#define ENCODER_Hoisting_PORT        	GPIOA    
#define ENCODER_Hoisting_PIN         	GPIO_Pin_11

//����궨��
#define ENCODER_SlewingSET()					GPIO_ResetBits(ENCODER_Slewing_PORT, ENCODER_Slewing_PIN)	//�����ã����24V
#define ENCODER_SlewingRESET()				GPIO_SetBits(ENCODER_Slewing_PORT, ENCODER_Slewing_PIN)		//�ر����ã����0V		
#define ENCODER_TrolleyingSET()				GPIO_ResetBits(ENCODER_Trolleying_PORT, ENCODER_Trolleying_PIN)//�����ã����24V
#define ENCODER_TrolleyingRESET()			GPIO_SetBits(ENCODER_Trolleying_PORT, ENCODER_Trolleying_PIN)	 //�ر����ã����0V
#define ENCODER_HoistingSET()					GPIO_ResetBits(ENCODER_Hoisting_PORT, ENCODER_Hoisting_PIN)//�����ã����24V
#define ENCODER_HoistingRESET()				GPIO_SetBits(ENCODER_Hoisting_PORT, ENCODER_Hoisting_PIN)	 //�ر����ã����0V


//����ģʽ
#define ENCODER_RunModeNone								0				//ʲôģʽҲ��������
#define ENCODER_SlewingRunModeBasic				0x10		//Slewing����ģʽ��ֵ
#define ENCODER_TrolleyingRunModeBasic		0x30		//Trolleying����ģʽ��ֵ
#define ENCODER_HoistingRunModeBasic			0x60		//Hoisting����ģʽ��ֵ

//�����궨�壬����������������
#define ENCODER_CmdReadData							0x03		//���ر���������ֵ
#define ENCODER_CmdSetAddress						0xA0		//���ñ�������ַ����
#define ENCODER_CmdBaudRate4800					0xB0		//���ò�����Ϊ4800
#define ENCODER_CmdBaudRate9600					0xB1		//���ò�����Ϊ9600
#define ENCODER_CmdBaudRate19200				0xB2		//���ò�����Ϊ19200
#define ENCODER_CmdBaudRate57600				0xB3		//���ò�����Ϊ57600
#define ENCODER_CmdBaudRate115200				0xB4		//���ò�����Ϊ115200
#define ENCODER_CmdReturnSet						0xC7		//��ȡ�趨ֵ�����ص�ַ�������ʡ��������
#define ENCODER_CmdReturnValue					0xC8		//��ȡ��������ǰ��Ȧ�뵥Ȧ����
#define ENCODER_CmdRetrunBit						0xC9		//��ȡ��������Ȧ�뵥Ȧλ��
#define ENCODER_CmdSet2047Circle				0xCA		//���ñ�������ǰȦ��Ϊ2047Ȧ
#define ENCODER_CmdSetCircle						0xD0		//���ñ�����Ȧ��
#define ENCODER_CmdClockwiseAdd					0xF1		//˳ʱ��Ӽ�������ʱ�������
#define ENCODER_CmdAnticlockwiseAdd			0xF2		//��ʱ��Ӽ�����˳ʱ�������


/*
	�������ṹ��
*/
typedef struct
{
	u8 Address;				//���ռ������ʱ��
	u16 ValCircle;		//Ȧ����
	u16 ValAngle;			//��Ȧ�Ƕ�
}Encoder_EncoderStructure;
//Encoder.c���ݽṹ��
typedef struct
{
	u8 Flag;		//��ر�־λ,B0=1������1�������䣬B1=1������2�������䣬B2=1������3��������
	u8 RunMode;	//����ģʽ�Ĵ���
	u8 CollectMode;
	u16 Timer;
	Encoder_EncoderStructure Slewing;		//��ת
	Encoder_EncoderStructure Trolleying;//���
	Encoder_EncoderStructure Hoisting;//����
}Encoder_DataStructure;



//------------------------------------
void Encoder_Init(void);
void Encoder_Timer(void);
void Encoder_GpioInit(void);
void Encoder_EncoderDataInit(void);
//��ȡ��ַ
Encoder_DataStructure* Encoder_GetData(void);
Encoder_EncoderStructure* Encoder_GetSlewingData(void);
Encoder_EncoderStructure* Encoder_GetTrolleyingData(void);
Encoder_EncoderStructure* Encoder_GetHoistinggData(void);
//���ñ�����
void Encoder_CollectValue(Encoder_EncoderStructure *p);
void Encoder_SetAddress(Encoder_EncoderStructure *p,u8 adr);
void Encoder_SetCycle(Encoder_EncoderStructure *p,u8 cycle);
void Encoder_SetCommand(Encoder_EncoderStructure *p,u8 cmd);
u8 Encoder_SetRunMode(u8 cmd);

//���Ժ���
void Encoder_DebugASCII(u8 *p,u8 len);
void Encoder_DebugRTU(u8 *p,u8 len);

#endif











