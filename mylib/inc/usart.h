/*
	usart�⺯��
*/

#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"
//#include <stdio.h>

//IO�ں궨��
//USART1
#define USART1_TX_GPIO	GPIOA
#define USART1_TX_Pin		GPIO_Pin_9
#define USART1_RX_GPIO	GPIOA
#define USART1_RX_Pin		GPIO_Pin_10
//USART2
#define USART2_TX_GPIO	GPIOD	
#define USART2_TX_Pin		GPIO_Pin_5
#define USART2_RX_GPIO	GPIOD
#define USART2_RX_Pin		GPIO_Pin_6
//USART3
#define USART3_TX_GPIO	GPIOD
#define USART3_TX_Pin		GPIO_Pin_8
#define USART3_RX_GPIO	GPIOD
#define USART3_RX_Pin		GPIO_Pin_9
//UART4
#define USART4_TX_GPIO	GPIOA	
#define USART4_TX_Pin		GPIO_Pin_0
#define USART4_RX_GPIO	GPIOA
#define USART4_RX_Pin		GPIO_Pin_1
//UART5
#define USART5_TX_GPIO	GPIOC
#define USART5_TX_Pin		GPIO_Pin_12
#define USART5_RX_GPIO	GPIOD
#define USART5_RX_Pin		GPIO_Pin_2
//USART6
#define USART6_TX_GPIO	GPIOC
#define USART6_TX_Pin		GPIO_Pin_6
#define USART6_RX_GPIO	GPIOC
#define USART6_RX_Pin		GPIO_Pin_7
//UART7
#define USART7_TX_GPIO	GPIOE
#define USART7_TX_Pin		GPIO_Pin_7
#define USART7_RX_GPIO	GPIOE
#define USART7_RX_Pin		GPIO_Pin_8
//UART8
#define USART8_TX_GPIO	GPIOE		
#define USART8_TX_Pin		GPIO_Pin_1
#define USART8_RX_GPIO	GPIOE	
#define USART8_RX_Pin		GPIO_Pin_0


#define RXBF_LEN							100			//���ջ�������Ϊ100���ֽ�
#define TXBF_LEN							1024			//���ͻ�������Ϊ250(���ܴ���255��)���ֽ�
#define STX										':'			//���տ�ʼ
#define RXCR									'\r'		//�س�
#define RXLF									'\n'		//����
#define RECEIVING							0X01		//���ڽ���
#define RECEIVE_SUCCESS				0X02		//���ճɹ�
#define RECEIVE_WRONG					0X04		//���ܴ���
#define RECEIVE_OVER					0X08		//�������
#define RECEIVE_TIME					0X10		//���ն�ʱ����ʱ��

//RS485ͨѶ�ӿڶ���
//#define RS485DIR_OPEN																//RS485DIR��
//#define RS485DIR_APP					RCC_AHB1Periph_GPIOA	//RS485DIR�ܽŵ�ʱ����
//#define RS485DIR_PORT					GPIOA									//RS485DIR�ܽŶ˿�
//#define RS485DIR_PIN					GPIO_Pin_2						//RS485DIR�ܽ�
//#define RS485DIR_SEND					GPIO_SetBits(RS485DIR_PORT, RS485DIR_PIN)		//RS485ͨѶ������
//#define RS485DIR_RECEIVE			GPIO_ResetBits(RS485DIR_PORT, RS485DIR_PIN)		//RS485ͨѶ�������
//RS485Ӳ���˿ڶ��壨���������˿�ӳ�䣩
#define RS485_Port						USART3										//RS485�˿�
#define RS485_Buffer					Get_USART3Buffer()				//RS485���ڽṹ��ָ��
#define RS485_SendData				USARTx_StoreBufferData		//RS485����һ���ֽ�����

#define MAX485CTR_OUT(a)\
					if (a==0)\
						GPIO_ResetBits(MAX485CTR_PORT,MAX485CTR_PIN);\
					else if (a==1)\
						GPIO_SetBits(MAX485CTR_PORT,MAX485CTR_PIN);\
					else if(a==2)\
						MAX485CTR_PORT->ODR ^= MAX485CTR_PIN;
/*
	�����շ��ṹ��
*/
typedef struct
{
	u8 RxBuffer[RXBF_LEN];	//���ջ�����
	
	u8 ProgressBuffer1[RXBF_LEN];	//���ջ�����
	u8 ProgressBuffer2[RXBF_LEN];	//���ջ�����
	u8 BufFlg1;           //
	u8 BufFlg2;
	u8 i;
	u8 RxCount;					//���ճ���
	u8 Timer;						//���ռ������ʱ��
	u8 TxBuffer[TXBF_LEN];	//���ͻ�����
	u8 RxFlag;					//���ձ�־λ��b1(���ڽ���),b2(�������),b3(�������),b8=1(RTUģʽ����)
	u16 TxLen;						//���ͳ���
	u16 TxSendPoint;			//���ͼ�ָ��
	u16 TxStorePoint;		//�洢ָ��
	u8 TxFlag;					//���ͱ�־λ��b8=1(RTUģʽ����)	
}USART_DataStructure;

#define UART4_REC_LEN  	 100  	//�����������ֽ��� 100
	  	
typedef struct
{ u8 EnReceveFlg;       //������ձ�־
	u8 ReceveOkFlg;       //������ϱ�־ ����
	u8 ReceveMax;         //�����ֽ���
	u8 ReceveCnt;         //���ռ�����
	u8 ReceveMode;        //AT /͸��
	u8 USART_RX_BUF[UART4_REC_LEN];//���ջ�����  ����
}USART_Com4Structure;			

//ȫ�����
void USART_InitAll(void);		//��ʼ�������ļ�USART
void USART_main(void);			//�ú���Ϊusart.c�ļ�������������ϵͳ����������
void USART_Timer(void);			//�����ļ���ʱ��

void USARTx_InitUsart(USART_TypeDef* USARTx,u32 BaudRate);//��ʼ������1~8
void USARTx_StoreBufferData(USART_TypeDef* USARTx,USART_DataStructure *p,unsigned char b);//����Ҫ��ӡ�����ݴ����ӡ������
void USARTx_Send(USART_TypeDef* USARTx,USART_DataStructure *p);//�ú���Ϊ���ʹ�ӡ��������һ���ֽ����ݣ��ɷ����жϳ����ã�ÿ����һ�η���һ���ֽ�����
void USARTx_Receive(USART_TypeDef* USARTx,USART_DataStructure *p,unsigned char b);//���ڽ���һ�����ݴ���
void USARTx_Timer(USART_TypeDef* USARTx,USART_DataStructure *p);//�ú���Ϊ����x��ʱ�������ڴ���x��ʱʹ�ã��������ճ�ʱ��ʱ������ʱ��ʱ
void USARTx_DW(USART_TypeDef* USARTx,USART_DataStructure *p);//����x�������ݴ���������������ѭ������
void Usart3_pro(void);//����3�������ݴ���������������ѭ������


USART_DataStructure *Get_USART1Buffer(void);	//����1�ṹ���ַ��ȡ
USART_DataStructure *Get_USART2Buffer(void);	//����2�ṹ���ַ��ȡ
USART_DataStructure *Get_USART3Buffer(void);//��ȡ����3�ṹ������
USART_DataStructure *Get_UART4Buffer(void);	//����1�ṹ���ַ��ȡ
//USART_DataStructure *Get_UART5Buffer(void);	//����5�ṹ���ַ��ȡ
USART_DataStructure *Get_USART6Buffer(void);//��ȡ����7�ṹ������
//USART_DataStructure *Get_UART7Buffer(void);//��ȡ����7�ṹ������
//USART_DataStructure *Get_UART8Buffer(void);//��ȡ����7�ṹ������
void USARTx_Printf(USART_TypeDef* USARTx,u8 mode);//�����������

void USART1_Configuration(void);
void USART2_Configuration(void);
void USART3_Configuration(void);
void UART4_Configuration(void);
void UART5_Configuration(void);
void USART6_Configuration(void);

void USART_SetEvenVerify(void);
void USART_SetNoneVerify(void);
void USART_ModeSet(USART_TypeDef* USARTx,u32 BaudRate,u8 Parity,u8 StopBits);
void USART1_SendByte(u8 TxData);


void USART_COM4_Enable(void);
void USART_COM4_Disable(void);
void USART_COM4_Receve_com(u8 ReceveMode,u8 ReceveMax);

void USART_COM5_Enable(void);
void USART_COM5_Disable(void);
void USART_COM5_Receve_com(u8 ReceveMode,u8 ReceveMax);

USART_Com4Structure *USART_COM4_GetData(void);

USART_Com4Structure *USART_COM5_GetData(void);

#endif














