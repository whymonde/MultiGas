/*
	OHRD23�⺯��
*/

#ifndef __OHRD23_H
#define __OHRD23_H

#include "stm32f10x.h"

//������
#define OHRD23_CmdReadWords						0x03		//��ȡ�Ĵ�������������
#define OHRD23_CmdWriteOneWord				0x06		//��Ŀ��Ĵ���д��һ��������
#define OHRD23_CmdWriteWords					0x10		//��Ŀ��Ĵ���д����������

//��ַ
//��������������ַ
#define OHRD23_AdrBase								0x0100	//��������������ַ
//���ܲ�����ַ
#define OHRD23_AdrEnergy							0x0600	//���ܲ�����ַ
//���ò�����ַ
#define OHRD23_AdrConf								0x0903	//���ò�����ַ
#define OHRD23_AdrConfVT							0x0903	//��ѹ����
#define OHRD23_AdrConfIT							0x0904	//��������
#define OHRD23_AdrConfLM							0x0905	//���߷�ʽ
#define OHRD23_AdrConfADR							0x0906	//ͨѶ��ַ
#define OHRD23_AdrConfBR							0x0907	//ͨѶ������
//�洢��ַ
#define OHRD23_AdrStoreBase						W25XXX_AdrOHRD23Base
#define OHRD23_AdrConfBase						0
#define OHRD23_AdrStoreVT							OHRD23_AdrConfBase
#define OHRD23_AdrStoreIT							OHRD23_AdrStoreVT+2
#define OHRD23_AdrStoreLM							OHRD23_AdrStoreIT+2
#define OHRD23_AdrStoreADR						OHRD23_AdrStoreLM+1
#define OHRD23_AdrStoreBR							OHRD23_AdrStoreADR+1
#define OHRD23_AdrStoreEnd						OHRD23_AdrStoreBR+2


//����ģʽ

//Ӳ���궨��
//---------------------------------------------------------------
//Ӳ���˿ں궨��
#define Ohrd23_SendBufferData	 				USARTx_StoreBufferData			//RTU���ݷ��ͺ���
//�ɼ��˿�(RS485)
#define Ohrd23_UsartPort							USART3											//��Ӧ��������˿�
#define Ohrd23_UsartBuffer()					Get_USART3Buffer()					//��Ӧ���ڽṹ��
#define Ohrd23_UartModeSet						USART_ModeSet
#define Ohrd23_SetComNoneVerify				USART_SetNoneVerify				//���ö˿ں�Ϊ��У��ģʽ

//�洢��ַ
//���������洢��ַ
//#define Ohrd23_AdrStoreBase					W25XXX_AdrOHRD23Base
//#define Ohrd23_AdrStoreDYBL					0		//��ѹ����
//#define Ohrd23_AdrStoreDLBL					Ohrd23_AdrStoreDYBL+2//��������
//#define Ohrd23_AdrStoreJXFS					Ohrd23_AdrStoreDLBL+2//���߷�ʽ
//#define Ohrd23_AdrStoreTXDZ					Ohrd23_AdrStoreJXFS+1//ͨѶ��ַ
//#define Ohrd23_AdrStoreTXBTL				Ohrd23_AdrStoreTXDZ+1//ͨѶ������
//#define Ohrd23_AdrStoreEND					Ohrd23_AdrStoreTXBTL+2//��������ĩ��




//OHRD23�����ǻ��������������ݽṹ��
typedef struct
{
	s32 VA;//A�����ѹ
	s32 VB;//B�����ѹ
	s32 VC;//C�����ѹ
	s32 VAB;//A-B�ߵ�ѹ
	s32 VBC;//B-C�ߵ�ѹ
	s32 VCA;//C-A�ߵ�ѹ
	s32 IA;//A�����
	s32 IB;//B�����
	s32 IC;//C�����	
	float PAY;//A���й�����
	float PBY;//B���й�����
	float PCY;//C���й�����
	float PZY;//�����й�����
	float PAW;//A���޹�����
	float PBW;//B���޹�����
	float PCW;//C���޹�����
	float PZW;//�����޹�����
	float PAS;//A�����ڹ���
	float PBS;//B�����ڹ���
	float PCS;//C�����ڹ���
	float PZS;//�������ڹ���
	s32 FA;//A�๦������
	s32 FB;	//B�๦������
	s32 FC;	//C�๦������
	s32 FZ;	//C�๦������
	s32 Frequency;//Ƶ��
}OHRD23_BaseStructure;
//OHRD23�����ǵ��ܲ������ݽṹ��
typedef struct
{
	s32 WHP;//�����й�����
	s32 WHN;//�����й�����
	s32 VARHP;//�����޹�����
	s32 VARHN;//�����޹�����
	u32 SWHP;//���й�����
	u32 SWHN;//���޹�����
}OHRD23_EnergyStructure;
//OHRD23���������ò������ݽṹ��
typedef struct
{
	s16 VT;//��ѹ����
	s16 IT;//��������
	s16 LM;//����ģʽ
	s16 ADR;//ͨѶ��ַ
	s16 BR;//ͨѶ������
}OHRD23_ConfigurationStructure;

/*
	OHRD23���ݽṹ��
*/
typedef struct
{
	u8 Flag;		//B0=1������OHRD23ͨѶ����,B1=1��ʱδ���յ�����
	u8 RunMode;	//����ģʽ��RunMode=ͨѶ�������
	u8 CollectMode;	//�ɼ�ģʽ
	u8 CollectCommand;//�ɼ�����
	u8 ErrorCnt;	//ͨѶ�����ۼ�
	u16 Timer;	//��ʱ��
	OHRD23_BaseStructure Base;
	OHRD23_EnergyStructure Energy;
	OHRD23_ConfigurationStructure Conf;
}Ohrd23_DataStructure;
//---




//----------------------------------------------------
void Ohrd23_Main(void);
void Ohrd23_Collect(void);
void Ohrd23_Init(void);
void Ohrd23_Timer(void);
Ohrd23_DataStructure* Ohrd23_GetOhrd23Data(void);
//��ȡ����
void Ohrd23_ReadBase(void);
void Ohrd23_ReadEnergy(void);
void Ohrd23_ReadConf(void);
//��������
void Ohrd23_SetVT(s16 rate);
void Ohrd23_SetIT(s16 rate);
void Ohrd23_SetLM(u8 mode);
void Ohrd23_SetAddress(u8 address);
void Ohrd23_SetBaudRate(u8 baudrate);
void Ohrd23_SetConfiguration(OHRD23_ConfigurationStructure *p);
//ͨѶ
void Ohrd23_ReadData(u16 address,u16 amount);
void Ohrd23_WriteOneWord(u16 address,u16 data);
void Ohrd23_WriteMultiWord(u16 address,u16 *p,u16 len);
//���ݻ���
void Ohrd23_BaseChange(u8 *p,OHRD23_BaseStructure* Base);
void Ohrd23_EnergyChange(u8 *p,OHRD23_EnergyStructure* Energy);
void Ohrd23_ConfigurationChange(u8 *p,OHRD23_ConfigurationStructure *conf);
//Ӳ������
void Ohrd23_SearchOhrd23(void);
void Ohrd23_UsartModeSet(void);

void Ohrd23_Operation(u16 mode,u8 *p,u8 len);
//�洢���
void Ohrd23_ReadStoreData(u8 address,u8 *p,u8 len);
void Ohrd23_StoreData(u8 address,u8 *p,u8 len);
void Ohrd23_RestoreFactorySetting(void);
//ͨѶ���
void Ohrd23_DebugASCII(u8 *p,u8 len);
void Ohrd23_DebugRTU(u8 *p,u8 len);
void Ohrd23_Receive(u8 *p,u8 len);





#endif










