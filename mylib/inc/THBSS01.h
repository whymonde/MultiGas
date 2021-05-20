/*
	Thbss01�⺯��
*/

#ifndef __THBSS01_H
#define __THBSS01_H

#include "stm32f10x.h"

//�����궨��
#define THBSS01_AddressSkewing				0x03		//THSS01��ַƫ��ֵ

//THBSS01�ڲ����ò�����ַ
#define THBSS01_AdrBase								0x0000	//��ַ
#define THBSS01_AdrWeight							THBSS01_AdrBase	//ʵʱ������ַ
#define THBSS01_AdrFaMa								0x0008	//���������Ĵ�����ַ
#define THBSS01_AdrAddress						0x000A	//У׼�Ĵ�����ַ
#define THBSS01_AdrBaudRate						0x000B	//У׼�Ĵ�����ַ
#define THBSS01_AdrShelling						0x0011	//ȥƤ�Ĵ�����ַ
#define THBSS01_AdrCalibration				0x0012	//У׼�Ĵ�����ַ


//�洢��ַ
#define THBSS01_AdrStoreBase					W25XXX_AdrTHBSS01Base
#define THBSS01_AdrConfBase						0
#define THBSS01_AdrStoreADR						THBSS01_AdrConfBase
#define THBSS01_AdrStoreBR						THBSS01_AdrStoreADR+1
#define THBSS01_AdrStoreEnd						THBSS01_AdrStoreBR+2




//Ӳ���궨��
//---------------------------------------------------------------
//Ӳ���˿ں궨��
#define THBSS01_SendBufferData	 			USARTx_StoreBufferData			//RTU���ݷ��ͺ���
//�ɼ��˿�(RS485)
#define THBSS01_UsartPort							USART3											//��Ӧ��������˿�
#define THBSS01_UsartBuffer()					Get_USART3Buffer()					//��Ӧ���ڽṹ��
#define THBSS01_UartModeSet						USART_ModeSet



//THBSS01���ش��������ò������ݽṹ��
typedef struct
{
	u8 ADR;//ͨѶ��ַ
	u16 BR;//ͨѶ������
}THBSS01_ConfStructure;

/*
	Thbss01���ݽṹ��
*/
typedef struct
{
	u8 Flag;						//��־λ
	u8 RunMode;					//����ģʽ
	u8 CollectMode;			//�ɼ�ģʽ
//	u16 OpMode;					//����ģʽ
	u16 Timer;					//��ʱ��
	s32 Hosting;				//����ֵ
	THBSS01_ConfStructure Conf;
}Thbss01_DataStructure;





//----------------------------------------------------
void Thbss01_Main(void);
void Thbss01_Init(void);
void Thbss01_DataInit(void);
void Thbss01_Timer(void);

Thbss01_DataStructure* Thbss01_GetThbss01Data(void);

//���ݲ���
void Thbss01_Operation(u16 mode,u8 *p,u8 len);
//ͨѶ���
void Thbss01_ReadData(u16 address,u16 amount);
void Thbss01_WriteOneWord(u16 address,u16 data);
void Thbss01_WriteMultiWord(u16 address,u16 *p,u16 len);
//�洢���
void Thbss01_ReadStoreData(u8 address,u8 *p,u8 len);
void Thbss01_StoreData(u8 address,u8 *p,u8 len);

void Thbss01_UsartModeSet(void);

void Thbss01_DebugASCII(u8 *p,u8 len);
void Thbss01_DebugRTU(u8 *p,u8 len);
void Thbss01_Receive(u8 *p,u8 len);





#endif










