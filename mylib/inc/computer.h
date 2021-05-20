/*
	computer�⺯��
*/

#ifndef __COMPUTER_H
#define __COMPUTER_H

#include "stm32f10x.h"

#define COMPUTER_ADDREDD				0XF1		//��������λ��ͨѶ��ַ


//Computer.c���ݽṹ��
typedef struct
{
	u8 Flag;		//��ر�־λ,B0=1������1�������䣬B1=1������2�������䣬B2=1������3��������
	u8 RunMode;	//����ģʽ�Ĵ���
	u16 Timer;
//	Encoder_EncoderStructure Slewing;		//��ת
//	Encoder_EncoderStructure Trolleying;//���
//	Encoder_EncoderStructure Hoisting;//����
}Computer_DataStructure;




void Computer_main(void);
void Computer_Init(void);
void Computer_Timer(void);

void Computer_DebugRTU(u8 *p,u8 len);
void Computer_Send(u8 *p,u8 len);







#endif



