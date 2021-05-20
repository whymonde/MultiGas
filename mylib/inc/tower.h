/*
	tower�⺯��
*/

#ifndef __TOWER_H
#define __TOWER_H

#include "stm32f10x.h"


#define TOWER_PI 				3.1415926
#define TOWER_EnDiv 		4096




//�洢��ַ
//���������洢��ַ
#define TOWER_AdrStoreBase					W25XXX_AdrTowerBase
#define TOWER_AdrStoreTJBase				0
#define TOWER_AdrStoreTJBH					TOWER_AdrStoreTJBase			//�������
#define TOWER_AdrStoreTJXH					TOWER_AdrStoreTJBH+4			//�����ͺ�
#define TOWER_AdrStoreTJZZS					TOWER_AdrStoreTJXH+21			//����������
#define TOWER_AdrStoreTJBC					TOWER_AdrStoreTJZZS+21		//�����۳�
#define TOWER_AdrStoreTJGD					TOWER_AdrStoreTJBC+2			//�����߶�
#define TOWER_AdrStoreTJEDZZ				TOWER_AdrStoreTJGD+2			//�����
#define TOWER_AdrStoreTJSDZZ				TOWER_AdrStoreTJEDZZ+4			//�׶�����
#define TOWER_AdrStoreTJMDZZ				TOWER_AdrStoreTJSDZZ+4			//ĩ������
#define TOWER_AdrStoreTJCCRQ				TOWER_AdrStoreTJMDZZ+4			//��������ʱ��������
#define TOWER_AdrStoreTJSBGG				TOWER_AdrStoreTJCCRQ+4			//�豸���
#define TOWER_AdrStoreTJSBLX				TOWER_AdrStoreTJSBGG+10			//�豸����
#define TOWER_AdrStoreTJEnd					TOWER_AdrStoreTJSBLX+10			//��������ĩ��

//��ת�����洢��ַ
#define TOWER_AdrStoreHZBase				TOWER_AdrStoreTJEnd					//��ת�����洢��ַ
#define TOWER_AdrStoreHZDCLS				TOWER_AdrStoreHZBase				//��ת�������
#define TOWER_AdrStoreHZXCLS				TOWER_AdrStoreHZDCLS+2			//��תС������
#define TOWER_AdrStoreHZEnd					TOWER_AdrStoreHZXCLS+2			//��ת����ĩ��
//��������洢��ַ
#define TOWER_AdrStoreBFBase				TOWER_AdrStoreHZEnd//70//TOWER_AdrStoreHZEnd					//��������洢��ַ
#define TOWER_AdrStoreBFDCLS				TOWER_AdrStoreBFBase+0			//����������
#define TOWER_AdrStoreBFXCLS				TOWER_AdrStoreBFDCLS+2			//���С������
#define TOWER_AdrStoreBFJTDBZJ			TOWER_AdrStoreBFXCLS+2			//�����Ͳ�ײ�ֱ��
#define TOWER_AdrStoreBFJTYXCD			TOWER_AdrStoreBFJTDBZJ+2		//�����Ͳ��Ч����
#define TOWER_AdrStoreBFLJ					TOWER_AdrStoreBFJTYXCD+2		//�����Ͳ�ݾ�
#define TOWER_AdrStoreBFGSSZJ				TOWER_AdrStoreBFLJ+2				//�����˿��ֱ��
#define TOWER_AdrStoreBFGSSCS				TOWER_AdrStoreBFGSSZJ+2			//�����˿������
#define TOWER_AdrStoreBFYXZS				TOWER_AdrStoreBFGSSCS+1			//�����˿����Чת��
#define TOWER_AdrStoreBFEnd					TOWER_AdrStoreBFYXZS+2			//�������ĩ��
//���߲����洢��ַ
#define TOWER_AdrStoreDGBase				TOWER_AdrStoreBFEnd					//���߲����洢��ַ
#define TOWER_AdrStoreDGDCLS				TOWER_AdrStoreDGBase+0			//���ߴ������
#define TOWER_AdrStoreDGXCLS				TOWER_AdrStoreDGDCLS+2			//����С������
#define TOWER_AdrStoreDGJTDBZJ			TOWER_AdrStoreDGXCLS+2			//���߾�Ͳ�ײ�ֱ��
#define TOWER_AdrStoreDGJTYXCD			TOWER_AdrStoreDGJTDBZJ+2		//���߾�Ͳ��Ч����
#define TOWER_AdrStoreDGLJ					TOWER_AdrStoreDGJTYXCD+2		//���߾�Ͳ�ݾ�
#define TOWER_AdrStoreDGGSSZJ				TOWER_AdrStoreDGLJ+2				//���߸�˿��ֱ��
#define TOWER_AdrStoreDGGSSCS				TOWER_AdrStoreDGGSSZJ+2			//���߸�˿������
#define TOWER_AdrStoreDGYXZS				TOWER_AdrStoreDGGSSCS+1			//���߸�˿����Чת��
#define TOWER_AdrStoreDGGSSBL				TOWER_AdrStoreDGYXZS+2			//���߸�˿������
#define TOWER_AdrStoreDGGSSRL				TOWER_AdrStoreDGGSSBL+1			//���߸�˿������
#define TOWER_AdrStoreDGEnd					TOWER_AdrStoreDGGSSRL+2			//���߲���ĩ��

#define TOWER_AdrStoreInit					TOWER_AdrStoreDGEnd				//��ʼ������״̬
#define TOWER_AdrStoreEnd						TOWER_AdrStoreInit+1			//


#define TOWER_AdrStoreADR						THBSS01_AdrConfBase
#define TOWER_AdrStoreBR						THBSS01_AdrStoreADR+1
//#define TOWER_AdrStoreEnd						THBSS01_AdrStoreBR+2


//�������ݻ�����
//��ת�ṹ��
typedef struct
{
	u16 Manufacturer;		//����������
	u16 Version;				//�����ͺ�
	u32 SerialNumber;		//�������
	float Length;				//�����۳�����λmm0
	float Height;				//�����߶ȣ���λmm
	float RatedLoad;		//����أ���λKg
	float HeadLoad;			//�׶����أ���λKg
	float EndLoad;			//ĩ�����أ���λKg
	float Momental;			//����
	u16 UpHour;					//����ʱ�䣺Сʱ�������Կ����󾭹���ʱ���Сʱ���֣���ʱ�����ڹػ������㣩
	u16 UpMinutes;			//����ʱ�䣺����
	u16 UpSec;					//����ʱ�䣺��
	u16 WorkingHour;		//����ʱ�䣺Сʱ�������Կ�������Ч������ʱ���Сʱ���֣���ʱ�����ڹػ������㣩
	u16 WorkingMinutes;	//����ʱ�䣺����
	u16 WorkingSec;			//����ʱ�䣺��
}TOWER_BaseStructure;

//��ת�ṹ��
typedef struct
{
	u16 BigGear;//�����
	u16 LittleGear;//С����
	float Rotation;//�Ƕȣ���λ��
}TOWER_SlewingStructure;

//����ṹ��
typedef struct
{
	u16 BigGear;//�����
	u16 LittleGear;//С����
	u8 RopeLayer;//��˿������
	float CoilingDiameter;//��Ͳֱ��,��λmm
	float RopeDiameter;//��˿��ֱ��,��λmm
	float Range;//���,��λmm
}TOWER_TrolleyingStructure;

//���߽ṹ��
typedef struct
{
	u16 BigGear;//�����
	u16 LittleGear;//С����
	u8 RopeLayer;//��˿������
//	u8 LayerCnt;//��˿������
	u8 RopeRate;//��˿������
	float CoilingDiameter;//��Ͳֱ��
	float RopeDiameter;//��˿��ֱ��
	float HookHeight;//���ߣ���λmm
}TOWER_HoistingStructure;

//���߽ṹ��
typedef struct
{
	TOWER_BaseStructure Base;
	TOWER_SlewingStructure Slewing;
	TOWER_TrolleyingStructure Trolleying;
	TOWER_HoistingStructure Hoisting;
}TOWER_DataStructure;



//�⺯��
void Tower_Init(void);
void Tower_MachineInit(void);
void Tower_BaseInit(void);
void Tower_SlewingInit(void);
void Tower_TrolleyingInit(void);
void Tower_HoistingInit(void);
TOWER_BaseStructure* Tower_GetBaseData(void);
TOWER_SlewingStructure* Tower_GetSlewingData(void);
TOWER_TrolleyingStructure* Tower_GetTrolleyingData(void);
TOWER_HoistingStructure* Tower_GetHoistingData(void);

void Tower_Operation(u16 mode,u8 *p,u8 len);


void Tower_CalSlewing(s32 buffer);
void Tower_CalTrolleying(s32 buffer);
void Tower_CalHoisting(s32 buffer);
void Tower_CalMomental(void);

//�洢���
void Tower_ReadStoreData(u8 address,u8 *p,u8 len);
void Tower_StoreData(u8 address,u8 *p,u8 len);
void Tower_FactoryReset(void);
//���Ժ���
void Tower_DebugASCII(u8 *p,u8 len);
void Tower_DebugRTU(u8 *p,u8 len);
















#endif
