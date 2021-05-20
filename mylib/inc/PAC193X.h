/*
	PAC193X.c�⺯��
*/

#ifndef __PAC193X_H
#define __PAC193X_H

#include "stm32f10x.h"

//�����궨��
#define PAC193X_ADDRESS      0x20    
//PAC1934����궨��
//�洢��ַ��غ궨��
#define PAC193X_AdrStoreBase					W25XXX_AdrPAC193XmBase
#define PAC193X_AdrStoreStart					0	
#define PAC193X_AdrStoreAD1Orgin			PAC193X_AdrStoreStart			//AD1ԭ��ֵ
#define PAC193X_AdrStoreAD1Endpoint		PAC193X_AdrStoreAD1Orgin+2//AD1�˵�ֵ
#define PAC193X_AdrStoreAD1Refer			PAC193X_AdrStoreAD1Endpoint+2//AD1����ֵ
#define PAC193X_AdrStoreAD1Map				PAC193X_AdrStoreAD1Refer+2//AD1�ɼ��豸ӳ��
#define PAC193X_AdrStoreAD2Orgin			PAC193X_AdrStoreAD1Map+1	//AD2ԭ��ֵ
#define PAC193X_AdrStoreAD2Endpoint		PAC193X_AdrStoreAD2Orgin+2//AD2�˵�ֵ
#define PAC193X_AdrStoreAD2Refer			PAC193X_AdrStoreAD2Endpoint+2//AD2����ֵ
#define PAC193X_AdrStoreAD2Map				PAC193X_AdrStoreAD2Refer+2//AD2�ɼ��豸ӳ��
#define PAC193X_AdrStoreAD3Orgin			PAC193X_AdrStoreAD2Map+1	//AD3ԭ��ֵ
#define PAC193X_AdrStoreAD3Endpoint		PAC193X_AdrStoreAD3Orgin+2//AD3�˵�ֵ
#define PAC193X_AdrStoreAD3Refer			PAC193X_AdrStoreAD3Endpoint+2//AD3����ֵ
#define PAC193X_AdrStoreAD3Map				PAC193X_AdrStoreAD3Refer+2//AD1�ɼ��豸ӳ��
#define PAC193X_AdrStoreEND						PAC193X_AdrStoreAD3Map+1	//PAC193X�洢ĩ��



//IO�ں궨��
#define PAC193X_SCL(a)\
					if (a==1)\
						GPIO_SetBits(PAC193X_SCL_PORT,PAC193X_SCL_PIN);\
					else if (a==0)\
						GPIO_ResetBits(PAC193X_SCL_PORT,PAC193X_SCL_PIN);\
					else if(a==2)\
						PAC193X_SCL_PORT->ODR ^= PAC193X_SCL_PIN;	
#define PAC193X_SDA(a)\
					if (a==1)\
						GPIO_SetBits(PAC193X_SDA_PORT,PAC193X_SDA_PIN);\
					else if (a==0)\
						GPIO_ResetBits(PAC193X_SDA_PORT,PAC193X_SDA_PIN);\
					else if(a==2)\
						PAC193X_SDA_PORT->ODR ^= PAC193X_SDA_PIN;	
#define PAC193X_PWRDN(a)\
					if (a==1)\
						GPIO_ResetBits(PAC193X_PWRDN_PORT,PAC193X_PWRDN_PIN);\
					else if (a==0)\
						GPIO_SetBits(PAC193X_PWRDN_PORT,PAC193X_PWRDN_PIN);\
					else if(a==2)\
						PAC193X_PWRDN_PORT->ODR ^= PAC193X_PWRDN_PIN;	
#define PAC193X_SLAL(a)\
					if (a==1)\
						GPIO_ResetBits(PAC193X_SLAL_PORT,PAC193X_SLAL_PIN);\
					else if (a==0)\
						GPIO_SetBits(PAC193X_SLAL_PORT,PAC193X_SLAL_PIN);\
					else if(a==2)\
						PAC193X_SLAL_PORT->ODR ^= PAC193X_SLAL_PIN;
#define PAC193X_SCLIN() 				GPIO_ReadInputDataBit(PAC193X_SCLIN_PORT, PAC193X_SCLIN_PIN)
#define PAC193X_SDAIN() 				GPIO_ReadInputDataBit(PAC193X_SDAIN_PORT, PAC193X_SDAIN_PIN)

typedef struct
{
	u8 Map;
	u16 Orgin;			//ԭ��ֵ
	u16 ADValue;	
	float Rate;			//���ʣ�(�˵�ֵ-ԭ��ֵ)/����ֵ��
	float MapValue;
}PAC193X_TowerStructure;					
/*
	�����շ��ṹ��
*/
typedef struct
{
//	u8 RunMode;					//����ģʽ
	u16 Timer;					//��ʱ��
	u8 CollectTimer;		//��ʱ��
	u8 WorkMode;				
	u8 OldWorkMode;	
	u8 NewWorkMode;
	u8 Command;					
	u8 CntByte;
	u8 CntBit;
	u8 Address;					//��ַ
	u8 RTxBuffer;
	u8 VBuffer[8];
	u16 Channel[4];
	float Voltage[4];
	PAC193X_TowerStructure Facility[3];
}PAC193X_DataStructure;





//----------------------------------------------------
void PAC193X_Main(void);
void PAC193X_Work(void);
void PAC193X_Operation(u16 mode,u8 *p,u8 len);
void PAC193X_Timer(void);
void PAC193X_Init(void);
void PAC193X_InitData(void);
void PAC193X_CalMapValue(PAC193X_TowerStructure *p);
void PAC193X_InitGPIO(void);
void PAC193X_TimerInit(void);
void PAC193X_DelayTime(u16 tim);
PAC193X_DataStructure* PAC193X_GetData(void);

void PAC193X_ReadStoreData(u8 address,u8 *p,u8 len);
void PAC193X_StoreData(u8 address,u8 *p,u8 len);

void PAC193X_DebugASCII(u8 *p,u8 len);
void PAC193X_DebugRTU(u8 *p,u8 len);






#endif










