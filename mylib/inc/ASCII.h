/*
		���ļ�������2017��8��3��
		���ļ�Ϊ"ASCII.c"�ļ��Ŀ��ļ�
*/

#ifndef __ASCII_H
#define __ASCII_H

//������ͷ�ļ�
#include "stm32f4xx.h"

//Ӳ���˿ں궨��
#define ASCII_SendBufferData				USARTx_StoreBufferData			//RTU���ݷ��ͺ���
//���Զ˿�
#define ASCII_UsartCompuerPort			USART1											//��Ӧ��������˿�
#define ASCII_UsartCompuerBuffer		Get_USART1Buffer()					//��Ӧ���ڽṹ��
//�����Ӧ����Ӳ���˿�
#define ASCII_UsartSlavePort				UART7												//��Ӧ��������˿�
#define ASCII_UsartSlaveBuffer			Get_USART1Buffer()						//��Ӧ���ڽṹ��






//����
void ASCII_CommunicateReceive(u8 *p,u8 len);//���Ժ���






#endif
