/*
	hoisting�⺯��
*/

#ifndef __HOISTING_H
#define __HOISTING_H

#include "stm32f10x.h"



#define HOISTING_DefaultAddress			0x03		//���ش�����Ĭ�ϵ�ַ


//����ģʽ
#define HOISTING_RunModeNone						0				//ʲôģʽҲ��������
#define HOISTING_RunModeColHoisting			0x10		//�ɼ�����ģʽ
#define HOISTING_RunModeReadAllData			0x11		//��ȡ�ڲ����мĴ���ֵģʽ



//---------------------------------------------------------------
//Ӳ���˿ں궨��
#define Hoistin_SendBufferData	 			USARTx_StoreBufferData			//RTU���ݷ��ͺ���
//�ɼ��˿�(RS485)
#define Hoisting_UsartPort						USART3											//��Ӧ��������˿�
#define Hoisting_UsartBuffer					Get_USART3Buffer()					//��Ӧ���ڽṹ��
#define Hoisting_SetComNoneVerify			USART_SetNoneVerify				//���ö˿ں�Ϊ��У��ģʽ



/*
	���ٴ��������ݽṹ��
*/
typedef struct
{
//	u8 Flag;					//��ر�־λ
	u8 Address;				//���ռ������ʱ��
	u8 RunMode;				
	u32 Weight;				//��ǰ���ؼĴ���ֵ
}Hoisting_DataStructure;




//�⺯��
void Hoisting_Init(void);
void Hoisting_DataInit(void);
Hoisting_DataStructure* Hoisting_GetData(void);
void Hoisting_CollectData(void);
void Hoisting_ReadAllData(void);
void Hoisting_DebugASCII(u8 *p,u8 len);
void Hoisting_DebugRTU(u8 *p,u8 len);



#endif













