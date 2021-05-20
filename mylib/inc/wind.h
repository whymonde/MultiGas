/*
	wind�⺯��
*/

#ifndef __WIND_H
#define __WIND_H

#include "stm32f10x.h"


#define WIND_DefaultAddress			0x02		//���ٴ�����Ĭ�ϵ�ַ

//����ģʽ
#define YCFS260_ComModeNone						0				//ʲôģʽҲ��������
#define YCFS260_ComModeReadWind				0x10		//�����ڶ�ȡ����ģʽ
#define YCFS260_ComModeReadAddress		0x11		//�����ڶ�ȡ��ַģʽ
#define YCFS260_ComModeSetAddress			0x12		//�������޸ĵ�ַģʽ
#define YCFS260_ComModeSetBuadRate		0x13		//�������޸Ĳ�����ģʽ
//�����궨��
#define WIND_CmdBaudRate2400				0x01		//���÷��ٴ�����������Ϊ2400����
#define WIND_CmdBaudRate4800				0x02		//���÷��ٴ�����������Ϊ4800����
#define WIND_CmdBaudRate9600				0x03		//���÷��ٴ�����������Ϊ9600����

//---------------------------------------------------------------
//Ӳ���˿ں궨��
#define YCFS260_SendBufferData	 	USARTx_StoreBufferData			//RTU���ݷ��ͺ���
//�ɼ��˿�(RS485)
#define YCFS260_UsartPort					USART3											//��Ӧ��������˿�
#define YCFS260_UsartBuffer				Get_USART3Buffer()					//��Ӧ���ڽṹ��
#define YCFS260_SetComNoneVerify	USART_SetNoneVerify				//���ö˿ں�Ϊ��У��ģʽ
#define YCFS260_UsartPort					USART3											//��Ӧ��������˿�
#define YCFS260_UartModeSet				USART_ModeSet


//�洢��ַ
//�¶Ȳ����洢��ַ
#define YCFS260_AdrStoreBase				W25XXX_AdrYCFS260Base
#define YCFS260_AdrStoreTXDZ				0//ͨѶ��ַ��1���ֽ�
#define YCFS260_AdrStoreTXBTL				YCFS260_AdrStoreTXDZ+1//ͨѶ�����ʣ�2���ֽ�
#define YCFS260_AdrStoreEND					YCFS260_AdrStoreTXBTL+2				



/*
	���ٴ��������ݽṹ��
*/
typedef struct
{
	u8 RunMode;				//��ǰ����ģʽ
	u8 ComMode;				//ͨѶģʽ
	u16 Timer;				//��ʱ��
	u8 Address;				//���ռ������ʱ��
	u16 BaudRate;			//������
	u16 Speed;				//��ǰ����ֵ
}Wind_DataStructure;


//�⺯��
void YCFS260_Communicate(void);
void YCFS260_Timer(void);

void Wind_Init(void);
void Wind_DataInit(void);
Wind_DataStructure* Wind_GetWindData(void);
void YCFS260_CollectWindSpeed(void);
void Wind_ReadCommAddress(void);
void Wind_SetCommAddress(u8 address);
void Wind_SetCommBaudRate(u8 baudrate);


void YCFS260_Operation(u16 mode,u8 *p,u8 len);
void YCFS260_ReadStoreData(u8 address,u8 *p,u8 len);
void YCFS260_StoreData(u8 address,u8 *p,u8 len);

void YCFS260_UsartModeSet(void);

//���Ժ���
void Wind_DebugASCII(u8 *p,u8 len);
void YCFS260_DebugRTU(u8 *p,u8 len);

#endif














