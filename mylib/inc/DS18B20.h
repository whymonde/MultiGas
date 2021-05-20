/*
	DS18B20�⺯��
*/

#ifndef __DS18B20_H
#define __DS18B20_H

#include "stm32f4xx.h"


#define DS18B20_ColFAULT						1000000	//�ɼ����ݹ���

//���
#define DS18B20_TX(a)\
					if (a==1)\
						GPIO_SetBits(DS18B20_DATA_PORT,DS18B20_DATA_PIN);\
					else if (a==0)\
						GPIO_ResetBits(DS18B20_DATA_PORT,DS18B20_DATA_PIN);\
					else if(a==2)\
						DS18B20_DATA_PORT->ODR ^= DS18B20_DATA_PIN;
#define DS18B20_RX() 				GPIO_ReadInputDataBit(DS18B20_DATA_PORT, DS18B20_DATA_PIN)
//#define DS18B20_IO_IN()  		DS18B20_SetGPIO_In()//{DS18B20_DATA_PORT->CRL&=0XFFFFFFF0;DS18B20_DATA_PORT->CRL|=8<<0;}
//#define DS18B20_IO_OUT() 		DS18B20_SetGPIO_Out()//{DS18B20_DATA_PORT->CRL&=0XFFFFFFF0;DS18B20_DATA_PORT->CRL|=3<<0;}

#define DS18B20_IO_IN()  		{DS18B20_DATA_PORT->MODER&=~(3<<(1*2));DS18B20_DATA_PORT->MODER|=0<<1*2;}	//PG9����ģʽ
#define DS18B20_IO_OUT() 		{DS18B20_DATA_PORT->MODER&=~(3<<(1*2));DS18B20_DATA_PORT->MODER|=1<<1*2;} 	//PG9���ģʽ

/*
	�ṹ��
*/
typedef struct
{
	u8 RunMode;					//����ģʽ
	u8 NewMode;					//��һ������ģʽ
	u8 WorkMode;				//����ģʽ
	u8 CntOne;					//������1
	u8 CntTwo;					//������2
	u16 Timer;					//��ʱ��
	u8 Buffer;					//
	u8 Error;						//����
	u8 ErrorCnt;				//����ͳ��
	u8 Data[9];					//������
	float Tem;					//�¶�ֵ
	u32 Cnt;						//������
	u32 CntError;				//������
}DS18B20_DataStructure;





//----------------------------------------------------
void DS18B20_Main(void);
void DS18B20_Work(void);
void DS18B20_Init(void);
void DS18B20_GpioInit(void);
void DS18B20_SetGPIO_In(void);
void DS18B20_SetGPIO_Out(void);
void DS18B20_TimerInit(void);
void DS18B20_InitData(void);
void DS18B20_Timer(void);
void DS18B20_DelayTime(u16 tim);


void DS18B20_Reset(void);
void DS18B20_SendData(u8 buffer);
void DS18B20_ReceiveData(void);
u8 DS18B20_CRC8(u8 *p, u16 len);
float DS18B20_CalculateTem(u8 *p);

DS18B20_DataStructure* DS18B20_GetData(void);

void DS18B20_DebugASCII(u8 *p,u8 len);
void DS18B20_DebugRTU(u8 *p,u8 len);

#endif










