/*
	encoder�⺯��
*/

#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"





//Encoder.c���ݽṹ��
typedef struct
{
	u8 Flag;		//��ر�־λ,B0=1������1�������䣬B1=1������2�������䣬B2=1������3��������
	u8 RunMode;	//����ģʽ�Ĵ���
	u8 Address;			//��ַ
	u16 Timer;				//��ʱ��
	u16 ValCircle;		//Ȧ����
	u16 ValAngle;			//��Ȧ�Ƕ�
	u16 ZeroValCircle;		//Ȧ����
	u16 ZeroValAngle;			//��Ȧ�Ƕ�
}Encoder_DataStructure;
















#endif


