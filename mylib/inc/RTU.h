/*
		���ļ�������2017��8��3��
		���ļ�Ϊ"RTU.c"�ļ��Ŀ��ļ�
*/

#ifndef __RTU_H
#define __RTU_H

//������ͷ�ļ�
#include "stm32f4xx.h"

//Ӳ���˿ں궨��
#define RTU_SendBufferData	 			USARTx_StoreBufferData			//RTU���ݷ��ͺ���
//���Զ˿�
#define RTU_UsartComputerPort			USART2											//��Ӧ��������˿�
#define RTU_UsartComputerBuffer		Get_USART2Buffer()					//��Ӧ���ڽṹ��
//�����Ӧ����Ӳ���˿�
#define RTU_UsartSlavePort				USART1												//��Ӧ��������˿�
#define RTU_UsartSlaveBuffer			Get_USART1Buffer()						//��Ӧ���ڽṹ��

typedef struct
{
//	u8 Error;
//	u8 ErrorCnt;
	u8 RunMode;
	u8 NewMode;
	u16 Timer;					//��ʱ��
}RTU_DataStructure;




void RTU_main(void);
//����
void RTU_CommunicateReceive(u8 *p,u8 len);//���ղ��������Դ��ڵ�RTU����

void RTU_Timer(void);
void RTU_Init(void);
//void RTU_ReceiveSlave(u8 *p,u8 len);
//void RTU_ReadSlaveBase(u8 address);

//����











#endif














