/*
	MCH58�⺯��
*/

#ifndef __MCH58_H
#define __MCH58_H

#include "stm32f10x.h"

//Ӳ���˿ں궨��
#define MCH58_SendBufferData	 			USARTx_StoreBufferData			//RTU���ݷ��ͺ���
//�ɼ��˿�(RS485)
#define MCH58_UsartPort							USART3											//��Ӧ��������˿�
#define MCH58_UsartBuffer()					Get_USART3Buffer()					//��Ӧ���ڽṹ��
#define MCH58_UartModeSet						USART_ModeSet
#define MCH58_SetComNoneVerify			USART_SetNoneVerify				//���ö˿ں�Ϊ��У��ģʽ


//����ģʽ
#define MCH58_RunModeNone								0				//ʲôģʽҲ��������
#define MCH58_Con1RunModeBasic				0x10		//Slewing����ģʽ��ֵ
#define MCH58_Con2RunModeBasic				0x30		//Trolleying����ģʽ��ֵ
#define MCH58_Con3RunModeBasic				0x60		//Hoisting����ģʽ��ֵ

//�����궨�壬����������������
#define MCH58_CmdReadData							0x03		//���ر���������ֵ
#define MCH58_CmdSetAddress						0xA0		//���ñ�������ַ����
#define MCH58_CmdBaudRate4800					0xB0		//���ò�����Ϊ4800
#define MCH58_CmdBaudRate9600					0xB1		//���ò�����Ϊ9600
#define MCH58_CmdBaudRate19200				0xB2		//���ò�����Ϊ19200
#define MCH58_CmdBaudRate57600				0xB3		//���ò�����Ϊ57600
#define MCH58_CmdBaudRate115200				0xB4		//���ò�����Ϊ115200
#define MCH58_CmdReturnSet						0xC7		//��ȡ�趨ֵ�����ص�ַ�������ʡ��������
#define MCH58_CmdReturnValue					0xC8		//��ȡ��������ǰ��Ȧ�뵥Ȧ����
#define MCH58_CmdRetrunBit						0xC9		//��ȡ��������Ȧ�뵥Ȧλ��
#define MCH58_CmdSet2047Circle				0xCA		//���ñ�������ǰȦ��Ϊ2047Ȧ
#define MCH58_CmdSetCircle						0xD0		//���ñ�����Ȧ��
#define MCH58_CmdClockwiseAdd					0xF1		//˳ʱ��Ӽ�������ʱ�������
#define MCH58_CmdAnticlockwiseAdd			0xF2		//��ʱ��Ӽ�����˳ʱ�������


//�洢��ַ
//���������洢��ַ
#define MCH58_AdrStoreBase					W25XXX_AdrMCH58Base
#define MCH58_AdrStoreBM1Base				0
#define MCH58_AdrStoreBM1ZeroH			MCH58_AdrStoreBM1Base				//������1����Ȧֵ
#define MCH58_AdrStoreBM1ZeroL			MCH58_AdrStoreBM1ZeroH+2		//������1��㵥Ȧֵ
#define MCH58_AdrStoreBM1CD					MCH58_AdrStoreBM1ZeroL+2		//������1��������
#define MCH58_AdrStoreBM1YSSB				MCH58_AdrStoreBM1CD+1				//������1�ɼ�ӳ���豸
#define MCH58_AdrStoreBM1ADR				MCH58_AdrStoreBM1YSSB+1			//������1�豸��ַ
#define MCH58_AdrStoreBM1BD					MCH58_AdrStoreBM1ADR+1			//������1������
#define MCH58_AdrStoreBM1END				MCH58_AdrStoreBM1BD+2				//������1ĩ��

#define MCH58_AdrStoreBM2Base				MCH58_AdrStoreBM1END
#define MCH58_AdrStoreBM2ZeroH			MCH58_AdrStoreBM2Base				//������2����Ȧֵ
#define MCH58_AdrStoreBM2ZeroL			MCH58_AdrStoreBM2ZeroH+2		//������2��㵥Ȧֵ
#define MCH58_AdrStoreBM2CD					MCH58_AdrStoreBM2ZeroL+2		//������2��������
#define MCH58_AdrStoreBM2YSSB				MCH58_AdrStoreBM2CD+1				//������2�ɼ�ӳ���豸
#define MCH58_AdrStoreBM2ADR				MCH58_AdrStoreBM2YSSB+1			//������2�豸��ַ
#define MCH58_AdrStoreBM2BD					MCH58_AdrStoreBM2ADR+1			//������2������
#define MCH58_AdrStoreBM2END				MCH58_AdrStoreBM2BD+2				//������2ĩ��

#define MCH58_AdrStoreBM3Base				MCH58_AdrStoreBM2END
#define MCH58_AdrStoreBM3ZeroH			MCH58_AdrStoreBM3Base				//������3����Ȧֵ
#define MCH58_AdrStoreBM3ZeroL			MCH58_AdrStoreBM3ZeroH+2		//������3��㵥Ȧֵ
#define MCH58_AdrStoreBM3CD					MCH58_AdrStoreBM3ZeroL+2		//������3��������
#define MCH58_AdrStoreBM3YSSB				MCH58_AdrStoreBM3CD+1				//������3�ɼ�ӳ���豸
#define MCH58_AdrStoreBM3ADR				MCH58_AdrStoreBM3YSSB+1			//������3�豸��ַ
#define MCH58_AdrStoreBM3BD					MCH58_AdrStoreBM3ADR+1			//������3������
#define MCH58_AdrStoreBM3END				MCH58_AdrStoreBM3BD+2				//������3ĩ��


#define MCH58_AdrStoreEnd						100			//

#define MCH58_PWR_ON()				GPIO_ResetBits(PwrV24_PWR_PORT, PwrV24_PWR_PIN)	//��
#define MCH58_PWR_OFF()				GPIO_SetBits(PwrV24_PWR_PORT, PwrV24_PWR_PIN)		//�ر�
#define MCH58_Con1_ON()				GPIO_ResetBits(MCH58_Con1_PORT, MCH58_Con1_PIN)	//��
#define MCH58_Con1_OFF()			GPIO_SetBits(MCH58_Con1_PORT, MCH58_Con1_PIN)		//�ر�
#define MCH58_Con2_ON()				GPIO_ResetBits(MCH58_Con2_PORT, MCH58_Con2_PIN)	//��
#define MCH58_Con2_OFF()			GPIO_SetBits(MCH58_Con2_PORT, MCH58_Con2_PIN)		//�ر�
#define MCH58_Con3_ON()				GPIO_ResetBits(MCH58_Con3_PORT, MCH58_Con3_PIN)	//��
#define MCH58_Con3_OFF()			GPIO_SetBits(MCH58_Con3_PORT, MCH58_Con3_PIN)		//�ر�

/*
	MCH58�ļ��ṹ��
*/
typedef struct
{
	u8 Address;				//��ַ
	u16 BaudRate;			//������
	u8 Map;						//ӳ��
	u32 CodedValue;		//����ֵ
	u32 ZeroValue;		//���ֵ
}MCH58_Structure;

/*
	MCH58�ļ��ṹ��
*/
typedef struct
{
	u8 Flag;						//��־λ
	u8 RunMode;					//����ģʽ
	u8 CollectMode;
	u16 Timer;					//��ʱ��
	MCH58_Structure Mch58[3];
}MCH58_DataStructure;





//----------------------------------------------------
void MCH58_Main(void);
void MCH58_Init(void);
void MCH58_DataInit(void);
void MCH58_GpioInit(void);
void MCH58_Timer(void);
MCH58_DataStructure* MCH58_GetMCH58Data(void);
void MCH58_CollectValue(MCH58_Structure *p);
u8 MCH58_SetCollectMode(u8 cmd);

//���ݲ���
void MCH58_Operation(u16 mode,u8 *p,u8 len);

void MCH58_UsartModeSet(MCH58_Structure *p);
//�洢����
void MCH58_ReadStoreData(u8 address,u8 *p,u8 len);
void MCH58_StoreData(u8 address,u8 *p,u8 len);

//���Ժ���
void MCH58_DebugASCII(u8 *p,u8 len);
void MCH58_DebugRTU(u8 *p,u8 len);
void MCH58_Receive(u8 *p,u8 len);





#endif










