/*
	screen�⺯��
*/

#ifndef __ESP01_H
#define __ESP01_H

#include "stm32f10x.h"


//�궨��
//Ӳ���˿ں궨��
#define ESP01_SendBufferData	 		USARTx_StoreBufferData			//RTU���ݷ��ͺ���
//�ɼ��˿�(RS485)
#define ESP01_UsartPort						USART2											//��Ӧ��������˿�
#define ESP01_UsartBuffer()				Get_USART2Buffer()					//��Ӧ���ڽṹ��
#define ESP01_SetComNoneVerify		USART_SetNoneVerify				//���ö˿ں�Ϊ��У��ģʽ
#define ESP01_UartModeSet					USART_ModeSet



/*
	�����շ��ṹ��
*/
typedef struct
{
	u8 Flag;						//��־λ��B0=0����ͨѶ��B0=1���յ�ģ�����ݣ�B7=1ģ�������Ϸ�����
	u8 RunMode;					//����ģʽ
	u8 WorkMode;				//����ģʽ
	u8 *RxBf;
	u8 RxLen;
	u8 ErrorCnt;				//����ͳ��
	u16 Timer;					//��ʱ��
}ESP01_DataStructure;





//----------------------------------------------------
void ESP01_Main(void);
void ESP01_Work(ESP01_DataStructure *p);
void ESP01_Init(void);
void ESP01_Timer(void);
ESP01_DataStructure* ESP01_GetData(void);

void ESP01_SendData(u8 *p,u8 len,ESP01_DataStructure *ESP);

void ESP01_DebugASCII(u8 *p,u8 len);
void ESP01_DebugRTU(u8 *p,u8 len);
void ESP01_Receive(u8 *p,u8 len);





#endif










