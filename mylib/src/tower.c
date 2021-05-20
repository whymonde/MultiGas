/*
���ļ�Ϊ�����ļ���������������ص�����
*/
#include "tower.h"

TOWER_DataStructure TowerData;//Tower�ṹ�����
/*
	��������void Tower_Init(void)
	˵��Tower.c��ʼ���ļ�
*/
void Tower_Init(void)
{
	Tower_FactoryReset();
	Tower_MachineInit();
	Tower_BaseInit();
	Tower_SlewingInit();
	Tower_TrolleyingInit();
	Tower_HoistingInit();
}
//---
/*
	��������void Tower_MachineInit(void)
*/
void Tower_MachineInit(void)
{
	
}
//---
/*
	��������void Tower_BaseInit(void)
	������	Slewing��س�ʼ��
*/
void Tower_BaseInit(void)
{
	TwoBytes two;
	FourBytes four;
	//�����۳�
	Tower_Operation(5,two.U8Data,2);
	TowerData.Base.Length = two.U16Data * 100;//ת��Ϊmm
	//�����߶�
	Tower_Operation(6,two.U8Data,2);
	TowerData.Base.Height = two.U16Data * 100;//ת��Ϊmm
	//����������
//	Tower_Operation(4,ScreenData.Buffer,20);
	TowerData.Base.Manufacturer = 22861;
	//�����ͺ�
	TowerData.Base.Version = 553;
	//�������
	Tower_Operation(2,four.U8Data,4);//�������������
	TowerData.Base.SerialNumber = four.U32Data;
	//�����
	Tower_Operation(7,four.U8Data,4);
	TowerData.Base.RatedLoad = four.FloatData;
	//�׶�����
	Tower_Operation(8,four.U8Data,4);
	TowerData.Base.HeadLoad = four.FloatData;
	//ĩ������
	Tower_Operation(9,four.U8Data,4);
	TowerData.Base.EndLoad = four.FloatData;
	
	printf("�������%u\r\n",TowerData.Base.SerialNumber);
	printf("���������̴���%u\r\n",TowerData.Base.Manufacturer);
	printf("�����ͺŴ���%u\r\n",TowerData.Base.Version);
	printf("�����۳�%.2fmm\r\n",TowerData.Base.Length);
	printf("�����߶�%.2fmm\r\n",TowerData.Base.Height);
	printf("�����%.3fKg\r\n",TowerData.Base.RatedLoad);
	printf("�׶�����%.3fKg\r\n",TowerData.Base.HeadLoad);
	printf("ĩ������%.3fKg\r\n\r\n",TowerData.Base.EndLoad);
}
//---
/*
	��������void Tower_SlewingInit(void)
	������	Slewing��س�ʼ��
*/
void Tower_SlewingInit(void)
{
	TwoBytes two;
	//�����
	Tower_Operation(22,two.U8Data,2);//������
	TowerData.Slewing.BigGear = two.U16Data;//�����
	//С����
	Tower_Operation(23,two.U8Data,2);//������
	TowerData.Slewing.LittleGear = two.U16Data;//С����
	printf("��ת�������%u\r\n",TowerData.Slewing.BigGear);
	printf("��תС������%u\r\n\r\n",TowerData.Slewing.LittleGear);
}
//---
/*
	��������void Tower_TrolleyingInit(void)
	������	Trolleying��س�ʼ��
*/
void Tower_TrolleyingInit(void)
{
	TwoBytes two;
	//�����
	Tower_Operation(42,two.U8Data,2);
	TowerData.Trolleying.BigGear = two.U16Data;
	//С����
	Tower_Operation(43,two.U8Data,2);
	TowerData.Trolleying.LittleGear = two.U16Data;
	//��˿������
	Tower_Operation(48,two.U8Data,1);
	TowerData.Trolleying.RopeLayer = two.U8Data[0];
	//��˿��ֱ��
	Tower_Operation(47,two.U8Data,2);
	TowerData.Trolleying.RopeDiameter = (float)(two.U16Data/10.0);
	//��Ͳֱ��
	Tower_Operation(44,two.U8Data,2);
	TowerData.Trolleying.CoilingDiameter = (float)(two.U16Data/10.0);
	
	printf("����������%u\r\n",TowerData.Trolleying.BigGear);
	printf("���С������%u\r\n",TowerData.Trolleying.LittleGear);
	printf("�����˿������%u\r\n",TowerData.Trolleying.RopeLayer);
	printf("�����˿��ֱ��%.2fmm\r\n",TowerData.Trolleying.RopeDiameter);
	printf("�����Ͳֱ��%.2fmm\r\n\r\n",TowerData.Trolleying.CoilingDiameter);
}
//---
/*
	��������void Tower_HoistingInit(void)
	������	Hoisting��س�ʼ��
*/
void Tower_HoistingInit(void)
{
	TwoBytes two;
	//�����
	Tower_Operation(62,two.U8Data,2);
	TowerData.Hoisting.BigGear = two.U16Data;
	//С����
	Tower_Operation(63,two.U8Data,2);
	TowerData.Hoisting.LittleGear = two.U16Data;
	//��˿������
	Tower_Operation(68,two.U8Data,1);
	TowerData.Hoisting.RopeLayer = two.U8Data[0];
	//��˿������
	Tower_Operation(70,two.U8Data,1);
	TowerData.Hoisting.RopeRate = two.U8Data[0];
	//��˿��ֱ��
	Tower_Operation(67,two.U8Data,2);
	TowerData.Hoisting.RopeDiameter = (float)(two.U16Data/10.0);
	//��Ͳֱ��
	Tower_Operation(64,two.U8Data,2);
	TowerData.Hoisting.CoilingDiameter  = (float)(two.U16Data/10.0);
	
	printf("���ߴ������%u\r\n",TowerData.Hoisting.BigGear);
	printf("����С������%u\r\n",TowerData.Hoisting.LittleGear);
	printf("���߸�˿������%u\r\n",TowerData.Hoisting.RopeLayer);
	printf("���߸�˿������%u\r\n",TowerData.Hoisting.RopeRate);
	printf("���߸�˿��ֱ��%.2fmm\r\n",TowerData.Hoisting.RopeDiameter);
	printf("���߾�Ͳֱ��%.2fmm\r\n\r\n",TowerData.Hoisting.CoilingDiameter);
}
//---
/*
	��������TOWER_BaseStructure* Tower_GetBaseData(void)
	�������ú������ڻ�ȡ�������������ṹ���ַ
*/
TOWER_BaseStructure* Tower_GetBaseData(void)
{
	return &TowerData.Base;
}
//---
/*
	��������TOWER_SlewingStructure* Tower_GetSlewingData(void)
	������	�ú������ڻ�ȡSlewingData�ṹ�����
*/
TOWER_SlewingStructure* Tower_GetSlewingData(void)
{
	return &TowerData.Slewing;
}
//---
/*
	��������TOWER_SlewingStructure* Tower_GetTrolleyingData(void)
	������	�ú������ڻ�ȡTrolleyingData�ṹ�����
*/
TOWER_TrolleyingStructure* Tower_GetTrolleyingData(void)
{
	return &TowerData.Trolleying;
}
//---
/*
	��������TOWER_SlewingStructure* Tower_GetHoistingData(void)
	������	�ú������ڻ�ȡHoistingData�ṹ�����
*/
TOWER_HoistingStructure* Tower_GetHoistingData(void)
{
	return &TowerData.Hoisting;
}
//---
/*
	��������void Tower_Operation(u16 mode,u8 *p,u8 len)
	�������ú���ΪTower.c�����ݲ���
*/
void Tower_Operation(u16 mode,u8 *p,u8 len)
{
	switch(mode)
	{
		case 1:
		{//��������������
			Tower_ReadStoreData(TOWER_AdrStoreTJBase,p,len);
			break;
		}
		case 2:
		{//���������
			Tower_ReadStoreData(TOWER_AdrStoreTJBH,p,len);
			break;
		}
		case 3:
		{//�������ͺ�
			Tower_ReadStoreData(TOWER_AdrStoreTJXH,p,len);
			break;
		}
		case 4:
		{//������������
			Tower_ReadStoreData(TOWER_AdrStoreTJZZS,p,len);
			break;
		}
		case 5:
		{//�������۳�
			Tower_ReadStoreData(TOWER_AdrStoreTJBC,p,len);
			break;
		}
		case 6:
		{//�������߶�
			Tower_ReadStoreData(TOWER_AdrStoreTJGD,p,len);
			break;
		}
		case 7:
		{//�����������
			Tower_ReadStoreData(TOWER_AdrStoreTJEDZZ,p,len);
			break;
		}
		case 8:
		{//�������׶�����
			Tower_ReadStoreData(TOWER_AdrStoreTJSDZZ,p,len);
			break;
		}
		case 9:
		{//������ĩ������
			Tower_ReadStoreData(TOWER_AdrStoreTJMDZZ,p,len);
			break;
		}
		case 10:
		{//����������ʱ������
			Tower_ReadStoreData(TOWER_AdrStoreTJCCRQ,p,len);
			break;
		}
		case 21:
		{//�����л�ת����
			Tower_ReadStoreData(TOWER_AdrStoreHZBase,p,len);
			break;
		}
		case 22:
		{//����ת�������
			Tower_ReadStoreData(TOWER_AdrStoreHZDCLS,p,len);
			break;
		}
		case 23:
		{//����תС������
			Tower_ReadStoreData(TOWER_AdrStoreHZXCLS,p,len);
			break;
		}
		case 41:
		{//�����б������
			Tower_ReadStoreData(TOWER_AdrStoreBFBase,p,len);
			break;
		}
		case 42:
		{//������������
			Tower_ReadStoreData(TOWER_AdrStoreBFDCLS,p,len);
			break;
		}
		case 43:
		{//�����С������
			Tower_ReadStoreData(TOWER_AdrStoreBFXCLS,p,len);
			break;
		}
		case 44:
		{//�������Ͳ�ײ�ֱ��
			Tower_ReadStoreData(TOWER_AdrStoreBFJTDBZJ,p,len);
			break;
		}
		case 45:
		{//�������Ͳ��Ч����
			Tower_ReadStoreData(TOWER_AdrStoreBFJTYXCD,p,len);
			break;
		}
		case 46:
		{//�������Ͳ�ݾ�
			Tower_ReadStoreData(TOWER_AdrStoreBFLJ,p,len);
			break;
		}
		case 47:
		{//�������˿��ֱ��
			Tower_ReadStoreData(TOWER_AdrStoreBFGSSZJ,p,len);
			break;
		}
		case 48:
		{//�������˿������
			Tower_ReadStoreData(TOWER_AdrStoreBFGSSCS,p,len);
			break;
		}
		case 49:
		{//�������˿����Чת��
			Tower_ReadStoreData(TOWER_AdrStoreBFYXZS,p,len);
			break;
		}
		case 61:
		{//�����е��߲���
			Tower_ReadStoreData(TOWER_AdrStoreDGBase,p,len);
			break;
		}
		case 62:
		{//�����ߴ������
			Tower_ReadStoreData(TOWER_AdrStoreDGDCLS,p,len);
			break;
		}
		case 63:
		{//������С������
			Tower_ReadStoreData(TOWER_AdrStoreDGXCLS,p,len);
			break;
		}
		case 64:
		{//�����߾�Ͳ�ײ�ֱ��
			Tower_ReadStoreData(TOWER_AdrStoreDGJTDBZJ,p,len);
			break;
		}
		case 65:
		{//�����߾�Ͳ��Ч����
			Tower_ReadStoreData(TOWER_AdrStoreDGJTYXCD,p,len);
			break;
		}
		case 66:
		{//�����߾�Ͳ�ݾ�
			Tower_ReadStoreData(TOWER_AdrStoreDGLJ,p,len);
			break;
		}
		case 67:
		{//�����߸�˿��ֱ��
			Tower_ReadStoreData(TOWER_AdrStoreDGGSSZJ,p,len);
			break;
		}
		case 68:
		{//�����߸�˿������
			Tower_ReadStoreData(TOWER_AdrStoreDGGSSCS,p,len);
			break;
		}
		case 69:
		{//�����߸�˿����Чת��
			Tower_ReadStoreData(TOWER_AdrStoreDGYXZS,p,len);
			break;
		}
		case 70:
		{//�����߸�˿������
			Tower_ReadStoreData(TOWER_AdrStoreDGGSSBL,p,len);
			break;
		}
		case 71:
		{//�����߸�˿������
			Tower_ReadStoreData(TOWER_AdrStoreDGGSSRL,p,len);
			break;
		}
		case 201:
		{//д������������
			Tower_StoreData(TOWER_AdrStoreTJBase,p,len);
			break;
		}
		case 202:
		{//�޸��������
			Tower_StoreData(TOWER_AdrStoreTJBH,p,len);
			break;
		}
		case 203:
		{//�޸������ͺ�
			Tower_StoreData(TOWER_AdrStoreTJXH,p,len);
			break;
		}
		case 204:
		{//�޸�����������
			Tower_StoreData(TOWER_AdrStoreTJZZS,p,len);
			break;
		}
		case 205:
		{//�޸������۳�
			Tower_StoreData(TOWER_AdrStoreTJBC,p,len);
			break;
		}
		case 206:
		{//�޸������߶�
			Tower_StoreData(TOWER_AdrStoreTJGD,p,len);
			break;
		}
		case 207:
		{//�޸����������
			Tower_StoreData(TOWER_AdrStoreTJEDZZ,p,len);
			break;
		}
		case 208:
		{//�޸������׶�����
			Tower_StoreData(TOWER_AdrStoreTJSDZZ,p,len);
			break;
		}
		case 209:
		{//�޸�����ĩ������
			Tower_StoreData(TOWER_AdrStoreTJMDZZ,p,len);
			break;
		}
		case 210:
		{//�޸�������������
			Tower_StoreData(TOWER_AdrStoreTJCCRQ,p,len);
			break;
		}
		case 221:
		{//�޸�����������ת����
			Tower_StoreData(TOWER_AdrStoreHZBase,p,len);
			break; 
		}
		case 222:
		{//�޸�������ת�������
			Tower_StoreData(TOWER_AdrStoreHZDCLS,p,len);
			break;
		}
		case 223:
		{//�޸�������תС������
			Tower_StoreData(TOWER_AdrStoreHZXCLS,p,len);
			break;
		}
		case 241:
		{//�޸����������������
			Tower_StoreData(TOWER_AdrStoreBFBase,p,len);
			break;
		}
		case 242:
		{//�޸���������������
			Tower_StoreData(TOWER_AdrStoreBFDCLS,p,len);
			break;
		}
		case 243:
		{//�޸��������С������
			Tower_StoreData(TOWER_AdrStoreBFXCLS,p,len);
			break;
		}
		case 244:
		{//�޸����������Ͳ�ײ�ֱ��
			Tower_StoreData(TOWER_AdrStoreBFJTDBZJ,p,len);
			break;
		}
		case 245:
		{//�޸����������Ͳ��Ч����
			Tower_StoreData(TOWER_AdrStoreBFJTYXCD,p,len);
			break;
		}
		case 246:
		{//�޸����������Ͳ�ݾ�
			Tower_StoreData(TOWER_AdrStoreBFLJ,p,len);
			break;
		}
		case 247:
		{//�޸����������˿��ֱ��
			Tower_StoreData(TOWER_AdrStoreBFGSSZJ,p,len);
			break;
		}
		case 248:
		{//�޸����������˿������
			Tower_StoreData(TOWER_AdrStoreBFGSSCS,p,len);
			break;
		}
		case 249:
		{//�޸����������˿����Чת��
			Tower_StoreData(TOWER_AdrStoreBFYXZS,p,len);
			break;
		}
		case 261:
		{//�޸������������߲���
			Tower_StoreData(TOWER_AdrStoreDGBase,p,len);
			break;
		}
		case 262:
		{//�޸��������ߴ������
			Tower_StoreData(TOWER_AdrStoreDGDCLS,p,len);
			break;
		}
		case 263:
		{//�޸���������С������
			Tower_StoreData(TOWER_AdrStoreDGXCLS,p,len);
			break;
		}
		case 264:
		{//�޸��������߾�Ͳ�ײ�ֱ��
			Tower_StoreData(TOWER_AdrStoreDGJTDBZJ,p,len);
			break;
		}
		case 265:
		{//�޸��������߾�Ͳ��Ч����
			Tower_StoreData(TOWER_AdrStoreDGJTYXCD,p,len);
			break;
		}
		case 266:
		{//�޸��������߾�Ͳ�ݾ�
			Tower_StoreData(TOWER_AdrStoreDGLJ,p,len);
			break;
		}
		case 267:
		{//�޸��������߸�˿��ֱ��
			Tower_StoreData(TOWER_AdrStoreDGGSSZJ,p,len);
			break;
		}
		case 268:
		{//�޸��������߸�˿������
			Tower_StoreData(TOWER_AdrStoreDGGSSCS,p,1);
			break;
		}
		case 269:
		{//�޸��������߸�˿����Чת��
			Tower_StoreData(TOWER_AdrStoreDGYXZS,p,2);
			break;
		}
		case 270:
		{//�޸��������߸�˿������
			Tower_StoreData(TOWER_AdrStoreDGGSSBL,p,1);
			break;
		}
		case 271:
		{//�޸��������߸�˿����
			Tower_StoreData(TOWER_AdrStoreDGGSSRL,p,2);
			break;
		}
		case 501:
		{//��ӡ������������
			FourBytes change4;
			TwoBytes change2;
			u8 *pdata = p+TOWER_AdrStoreTJBH;
			u8 i;
			change4.U8Data[0] = pdata[0];
			change4.U8Data[1] = pdata[1];
			change4.U8Data[2] = pdata[2];
			change4.U8Data[3] = pdata[3];
			printf("�������Ϊ��%u\r\n",change4.U32Data);
			pdata = p+TOWER_AdrStoreTJXH;
			printf("�����ͺ�Ϊ��");
			i=0;
			do
			{
				printf("%c",pdata[i]);
				i++;
				if(i>20) break;
			}while(pdata[i] != 0);
			printf("\r\n");
			pdata = p+TOWER_AdrStoreTJZZS;
			printf("������Ϊ��");
			i=0;
			do
			{
				printf("%c",pdata[i]);
				i++;
				if(i>20) break;
			}while(pdata[i] != 0);
			printf("\r\n");
			pdata = p+TOWER_AdrStoreTJCCRQ;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("�豸��������Ϊ��%u��",change2.U16Data);
			change2.U8Data[0] = pdata[2];
			change2.U8Data[1] = pdata[3];
			printf("%u��",change2.U8Data[0]);
			printf("%u��\r\n",change2.U8Data[1]);
			
			pdata = p+TOWER_AdrStoreTJBC;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("�����۳�Ϊ��%fm\r\n",change2.U16Data/10.0);
			pdata = p+TOWER_AdrStoreTJGD;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("�����߶�Ϊ��%fm\r\n",change2.U16Data/10.0);
			pdata = p+TOWER_AdrStoreTJEDZZ;
			change4.U8Data[0] = pdata[0];
			change4.U8Data[1] = pdata[1];
			change4.U8Data[2] = pdata[2];
			change4.U8Data[3] = pdata[3];
			printf("���������Ϊ��%fKg\r\n",change4.U32Data/100.0);
			pdata = p+TOWER_AdrStoreTJSDZZ;
			change4.U8Data[0] = pdata[0];
			change4.U8Data[1] = pdata[1];
			change4.U8Data[2] = pdata[2];
			change4.U8Data[3] = pdata[3];
			printf("�����׶�����Ϊ��%fKg\r\n",change4.U32Data/100.0);
			pdata = p+TOWER_AdrStoreTJMDZZ;
			change4.U8Data[0] = pdata[0];
			change4.U8Data[1] = pdata[1];
			change4.U8Data[2] = pdata[2];
			change4.U8Data[3] = pdata[3];
			printf("����ĩ������Ϊ��%fKg\r\n",change4.U32Data/100.0);
			break;
		}
		case 502:
		{//��ӡ�������
			FourBytes change;
			change.U8Data[0] = p[0];
			change.U8Data[1] = p[1];
			change.U8Data[2] = p[2];
			change.U8Data[3] = p[3];
			printf("�������Ϊ��%u\r\n",change.U32Data);
			break;
		}
		case 503:
		{//�������ͺ�
			u8 i=0;
			printf("�����ͺ�Ϊ��");
			do
			{
				printf("%c",p[i]);
				i++;
				if(i>20) break;
			}while(p[i] != 0);
			printf("\r\n");
			break;
		}
		case 504:
		{//��ӡ����������
			u8 i=0;
			printf("������Ϊ��");
			do
			{
				printf("%c",p[i]);
				i++;
				if(i>20) break;
			}while(p[i] != 0);
			printf("\r\n");
			break;
		}
		case 505:
		{//��ӡ�����۳�
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("�����۳�Ϊ��%fm\r\n",change2.U16Data/100.0);
			break;
		}
		case 506:
		{//��ӡ�����߶�
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("�����߶�Ϊ��%fm\r\n",change2.U16Data/100.0);
			break;
		}
		case 507:
		{//��ӡ�����
			FourBytes change4;
			change4.U8Data[0] = p[0];
			change4.U8Data[1] = p[1];
			change4.U8Data[2] = p[2];
			change4.U8Data[3] = p[3];
			printf("�����Ϊ��%fKG\r\n",change4.U32Data/100.0);
			break;
		}
		case 508:
		{//��ӡ�׶�����
			FourBytes change4;
			change4.U8Data[0] = p[0];
			change4.U8Data[1] = p[1];
			change4.U8Data[2] = p[2];
			change4.U8Data[3] = p[3];
			printf("�׶�����Ϊ��%fKG\r\n",change4.U32Data/100.0);
			break;
		}
		case 509:
		{//��ӡ�׶�����
			FourBytes change4;
			change4.U8Data[0] = p[0];
			change4.U8Data[1] = p[1];
			change4.U8Data[2] = p[2];
			change4.U8Data[3] = p[3];
			printf("����ĩ������Ϊ��%fKG\r\n",change4.U32Data/100.0);
			break;
		}
		case 510:
		{//��ӡ����������
			TwoBytes change;
			change.U8Data[0] = p[0];
			change.U8Data[1] = p[1];
			printf("�豸��������Ϊ��%u��",change.U16Data);
			change.U8Data[0] = p[2];
			change.U8Data[1] = p[3];
			printf("%u��",change.U8Data[0]);
			printf("%u��\r\n",change.U8Data[1]);
			break;
		}
		case 521:
		{//��ӡ���л�ת����
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("��ת�������Ϊ��%u\r\n",change2.U16Data);
			change2.U8Data[0] = p[2];
			change2.U8Data[1] = p[3];
			printf("��תС������Ϊ��%u\r\n",change2.U16Data);
			break;
		}
		case 522:
		{//��ӡ��ת�������
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("��ת�������Ϊ��%u\r\n",change2.U16Data);
			break;
		}
		case 523:
		{//��ӡ��תС������
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("��תС������Ϊ��%u\r\n",change2.U16Data);
			break;
		}
		case 541:
		{//��ӡ���б������
			TwoBytes change2;
			u8 *pdata;
			u8 bf = TOWER_AdrStoreBFBase;
			pdata = p+TOWER_AdrStoreBFDCLS-bf;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("����������Ϊ��%u\r\n",change2.U16Data);
			pdata = p+TOWER_AdrStoreBFXCLS-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("���С������Ϊ��%u\r\n",change2.U16Data);
			pdata = p+TOWER_AdrStoreBFJTDBZJ-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("�����Ͳ�ײ�ֱ����%fmm\r\n",change2.U16Data/10.0);
			pdata = p+TOWER_AdrStoreBFJTYXCD-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("�����Ͳ��Ч���ȣ�%fmm\r\n",change2.U16Data/10.0);
			pdata = p+TOWER_AdrStoreBFLJ-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("�����Ͳ�ݾࣺ%fmm\r\n",change2.U16Data/100.0);
			pdata = p+TOWER_AdrStoreBFGSSZJ-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("�����˿��ֱ����%fmm\r\n",change2.U16Data/100.0);
			pdata = p+TOWER_AdrStoreBFGSSCS-bf;
			printf("�����˿��������%u\r\n",*pdata);
			pdata = p+TOWER_AdrStoreBFYXZS-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("�����˿����Чת����%f\r\n",change2.U16Data/100.0);
			break;
		}
		case 542:
		{//��ӡ����������
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("����������Ϊ��%u\r\n",change2.U16Data);
			break;
		}
		case 543:
		{//��ӡ���С������
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("���С������Ϊ��%u\r\n",change2.U16Data);
			break;
		}
		case 544:
		{//��ӡ���б������
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("�����Ͳ�ײ�ֱ����%fmm\r\n",change2.U16Data/10.0);
			break;
		}
		case 545:
		{//��ӡ���б������
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("�����Ͳ��Ч���ȣ�%fmm\r\n",change2.U16Data/10.0);
			break;
		}
		case 546:
		{//��ӡ���б������
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("�����Ͳ�ݾࣺ%fmm\r\n",change2.U16Data/100.0);
			break;
		}
		case 547:
		{//��ӡ���б������
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("�����˿��ֱ����%fmm\r\n",change2.U16Data/100.0);
			break;
		}
		case 548:
		{//��ӡ���б������
			printf("�����˿��������%u\r\n",*p);
			break;
		}
		case 549:
		{//��ӡ���б������
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("�����˿����Чת����%f\r\n",change2.U16Data/100.0);
			break;
		}
		case 561:
		{//��ӡ�����������߲���
			TwoBytes change2;
			u8 *pdata;
			u8 bf = TOWER_AdrStoreDGBase;
			pdata = p+TOWER_AdrStoreDGDCLS-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("���ߴ������Ϊ��%u\r\n",change2.U16Data);
			pdata = p+TOWER_AdrStoreDGXCLS-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("����С������Ϊ��%u\r\n",change2.U16Data);
			pdata = p+TOWER_AdrStoreDGJTDBZJ-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("���߾�Ͳ�ײ�ֱ����%fmm\r\n",change2.U16Data/10.0);
			pdata = p+TOWER_AdrStoreDGJTYXCD-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("���߾�Ͳ��Ч���ȣ�%fmm\r\n",change2.U16Data/10.0);
			pdata = p+TOWER_AdrStoreDGLJ-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("���߾�Ͳ�ݾࣺ%fmm\r\n",change2.U16Data/100.0);
			pdata = p+TOWER_AdrStoreDGGSSZJ-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("���߸�˿��ֱ����%fmm\r\n",change2.U16Data/100.0);
			pdata = p+TOWER_AdrStoreDGGSSCS-bf;
			printf("���߸�˿��������%u\r\n",*pdata);
			pdata = p+TOWER_AdrStoreDGYXZS-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("���߸�˿����Чת����%f\r\n",change2.U16Data/10.0);
			pdata = p+TOWER_AdrStoreDGGSSBL-bf;
			printf("���߸�˿�����ʣ�%u\r\n",*pdata);
			pdata = p+TOWER_AdrStoreDGGSSRL-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("���߸�˿��������%f\r\n",change2.U16Data/10.0);
			break;
		}
		case 562:
		{//��ӡ���ߴ������
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("���ߴ������Ϊ��%u\r\n",change2.U16Data);
			break;
		}
		case 563:
		{//��ӡ����С������
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("����С������Ϊ��%u\r\n",change2.U16Data);
			break;
		}
		case 564:
		{//��ӡ���߾�Ͳ�ײ�ֱ��
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("���߾�Ͳ�ײ�ֱ����%fmm\r\n",change2.U16Data/10.0);
			break;
		}
		case 565:
		{//��ӡ���߾�Ͳ��Ч����
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("���߾�Ͳ��Ч���ȣ�%fmm\r\n",change2.U16Data/10.0);
			break;
		}
		case 566:
		{//��ӡ���߾�Ͳ�ݾ�
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("���߾�Ͳ�ݾࣺ%fmm\r\n",change2.U16Data/100.0);
			break;
		}
		case 567:
		{//��ӡ���߸�˿��ֱ��
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("���߸�˿��ֱ����%fmm\r\n",change2.U16Data/100.0);
			break;
		}
		case 568:
		{//��ӡ���߸�˿������
			printf("���߸�˿��������%u\r\n",*p);
			break;
		}
		case 569:
		{//��ӡ���߸�˿����Чת��
			TwoBytes change;
			change.U8Data[0] = p[0];
			change.U8Data[1] = p[1];
			printf("���߸�˿����Чת����%f\r\n",change.U16Data/10.0);
			break;
		}
		case 570:
		{//��ӡ���߸�˿������
			printf("���߸�˿�����ʣ�%u\r\n",*p);
			break;
		}
		case 571:
		{//��ӡ���߸�˿������
			TwoBytes change;
			change.U8Data[0] = p[0];
			change.U8Data[1] = p[1];
			printf("���߸�˿��������%f\r\n",change.U16Data/10.0);
			break;
		}
	}
}
//---
/*
	��������void Tower_CalSlewing(s32 buffer)
	������	����������ת�Ƕȣ�����Ƕ���0~360��֮��			
*/
void Tower_CalSlewing(s32 buffer)
{
	TowerData.Slewing.Rotation = (float)buffer/TOWER_EnDiv * \
		(360.0/(TowerData.Slewing.BigGear / TowerData.Slewing.LittleGear));
	while(TowerData.Slewing.Rotation > 360)
		TowerData.Slewing.Rotation -=360;
	while(TowerData.Slewing.Rotation < 0)
		TowerData.Slewing.Rotation +=360;
}
//---
/*
	��������float Tower_CalTrolleying(u32 buffer)
	������	�����������λ��			
*/
void Tower_CalTrolleying(s32 buffer)
{
	TowerData.Trolleying.Range = (float)buffer/TOWER_EnDiv * \
		PI*(TowerData.Trolleying.CoilingDiameter +\
		TowerData.Trolleying.RopeDiameter*TowerData.Trolleying.RopeLayer)/ \
		((float)TowerData.Trolleying.BigGear/TowerData.Trolleying.LittleGear);
	Tower_CalMomental();//��������
}
//---
/*
	��������float Tower_CalHoisting(u32 buffer)
	������	�����������λ��			
*/
void Tower_CalHoisting(s32 buffer)
{
	TowerData.Hoisting.HookHeight = (float)buffer/TOWER_EnDiv * \
		PI*(TowerData.Hoisting.CoilingDiameter +\
		TowerData.Hoisting.RopeDiameter*TowerData.Hoisting.RopeLayer)/ \
		((float)TowerData.Hoisting.BigGear/TowerData.Hoisting.LittleGear);
}
//---
/*
	��������void Tower_CalMomental(void)
	��������������
*/
void Tower_CalMomental(void)
{
	TowerData.Base.Momental = (Thbss01_GetThbss01Data()->Hosting/1000.0)\
	/(TowerData.Base.HeadLoad-(TowerData.Base.HeadLoad-TowerData.Base.EndLoad)\
	*TowerData.Trolleying.Range/TowerData.Base.Length);
}
//-------------------------���ݶ�ȡ���洢----------------------------
//---
/*
	��������void Tower_ReadStoreData(u8 address,u8 *p,u8 len)
	������	��ȡ����
*/
void Tower_ReadStoreData(u8 address,u8 *p,u8 len)
{
	W25Xxx_ReadData(p,TOWER_AdrStoreBase+address,len);
}
//---
/*
	��������void Tower_StoreData(u8 address,u8 *p,u8 len)
	�������ú���Ϊ�洢OHRD23���ò�������
	˵������ȡ����
				��������
				�޸�����
				��������
*/
void Tower_StoreData(u8 address,u8 *p,u8 len)
{
	u8 buffer[TOWER_AdrStoreEnd],cnt;
	W25Xxx_ReadData(buffer,TOWER_AdrStoreBase,TOWER_AdrStoreEnd);//���������ò���
	W25Xxx_EraseSector(TOWER_AdrStoreBase);//��������
	for(cnt=0;cnt<len;cnt++)
	{
		buffer[address++] = *p;
		p++;
	}
	W25Xxx_PageWrite(buffer,TOWER_AdrStoreBase,TOWER_AdrStoreEnd);//��������
	
	//���³�ʼ������
	Tower_BaseInit();
	Tower_SlewingInit();
	Tower_TrolleyingInit();
	Tower_HoistingInit();
}
//---
/*
	��������void Tower_FactoryReset(void)
	�������ú���Tower.c�洢���ݻָ���������
*/
void Tower_FactoryReset(void)
{
	u8 bf[21];
	FourBytes four;
	TwoBytes two;
	u8 i;
	Tower_ReadStoreData(TOWER_AdrStoreInit,&i,1);
	if(i == 0x55) return;
	i = 0x55;
	Tower_StoreData(TOWER_AdrStoreTJBH,four.U8Data,4);

	four.U32Data = 1234567890;
	Tower_StoreData(TOWER_AdrStoreTJBH,four.U8Data,4);//�������
	for(i=0;i<21;i++)
		bf[i] = 0;
	sprintf((char*)bf,"SST553");
	Tower_StoreData(TOWER_AdrStoreTJXH,bf,21);//�����ͺ�
	for(i=0;i<21;i++)
		bf[i] = 0;
	sprintf((char*)bf,"�����Ƽ�");
	Tower_StoreData(TOWER_AdrStoreTJZZS,bf,21);//����������
	
	two.U16Data = 1000;
	Tower_StoreData(TOWER_AdrStoreTJGD,two.U8Data,2);//�����߶�
	
	four.U16Data[0] = 2018;
	four.U8Data[2] = 11;
	four.U8Data[3] = 1;
	Tower_StoreData(TOWER_AdrStoreTJCCRQ,four.U8Data,21);//��������
	
	for(i=0;i<21;i++)
		bf[i] = 0;
	sprintf((char*)bf,"����");
	Tower_StoreData(TOWER_AdrStoreTJSBGG,bf,10);//�������
	for(i=0;i<21;i++)
		bf[i] = 0;
	sprintf((char*)bf,"����");
	Tower_StoreData(TOWER_AdrStoreTJSBLX,bf,10);//��������
	
//	u16 bf[3]={1111,120,55555};
//	Flashisp_StoreData(FLASHIS_AdrStorePassword,bf,3);
//	Flashisp_ReadStoreData(FLASHIS_AdrStorePassword,bf,3);
//	printf("�ָ���������\r\n");
//	printf("ά������Ϊ��%u\r\n",bf[0]);
//	printf("�趨�¶�Ϊ��%u\r\n",bf[1]);
}
//---

//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Tower_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪTower�ļ���ASCII�����ļ�
*/
void Tower_DebugASCII(u8 *p,u8 len)
{//:000|63|100
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 201:
		{//д������������	:000|63|201|1234567890|
			if(len>TOWER_AdrStoreEnd)
			{
				len = 0;
			}
			break;
		}
		case 202:
		{//�޸��������		:000|63|202|1234567890
			FourBytes buffer;
			buffer.U32Data = Calculate_CharToU32(p+4,10);//��������
			printf("���������Ϊ��%u\r\n",buffer.U32Data);
			Tower_Operation(202,buffer.U8Data,4);//�޸�����
			Tower_Operation(2,buffer.U8Data,4);//������
			Tower_Operation(502,buffer.U8Data,4);//��ʾ����
			break;
		}
		case 203:
		{//�޸������ͺ�
			u8 i,buffer[21];
			for(i=0;i<21;i++)
			{
				buffer[i]= 0;
			}
			i = 0;
			do
			{
				buffer[i] = p[4+i];
				i++;
				if(i>20) break;
				if(p[4+i] == '|') break;
			}while(p[4+i] != '\r');
			buffer[20] = i;
			printf("����������ͺ�Ϊ��");
			for(i=0;i<buffer[20];i++)
			{
				printf("%c",buffer[i]);
			}
			printf("\r\n");
			buffer[20] = 0;
			Tower_Operation(203,buffer,21);//�޸�����
			Tower_Operation(3,buffer,21);//������
			Tower_Operation(503,buffer,21);//��ʾ����
			break;
		}
		case 204:
		{//�޸�����������
			u8 i,buffer[21];
			for(i=0;i<21;i++)
			{
				buffer[i]= 0;
			}
			i = 0;
			do
			{
				buffer[i] = p[4+i];
				i++;
				if(i>20) break;
				if(p[4+i] == '|') break;
			}while(p[4+i] != '\r');
			buffer[20] = i;
			printf("���������������Ϊ��");
			for(i=0;i<buffer[20];i++)
			{
				printf("%c",buffer[i]);
			}
			printf("\r\n");
			buffer[20] = 0;
			Tower_Operation(204,buffer,21);//�޸�����
			Tower_Operation(4,buffer,21);//������
			Tower_Operation(504,buffer,21);//��ʾ����
			break;
		}
		case 205:
		{//�޸������۳�
			TwoBytes buffer;
			float a = Calculate_CharToFloat(p+4,5);//��������
			if(a<655.35)
			{
				printf("����ֵΪ��%fm\r\n",a);
				buffer.U16Data = (u16)(a*100);
				Tower_Operation(205,buffer.U8Data,2);//�޸�����
				Tower_Operation(5,buffer.U8Data,2);//������
				Tower_Operation(505,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("�����۳������÷�ΧΪ0~655.35m\r\n");
			}
			break;
		}
		case 206:
		{//�޸������߶�
			TwoBytes buffer;
			float a = Calculate_CharToFloat(p+4,5);//��������
			if(a<655.35)
			{
				printf("����ֵΪ��%fm\r\n",a);
				buffer.U16Data = (u16)(a*100);
				Tower_Operation(206,buffer.U8Data,2);//�޸�����
				Tower_Operation(6,buffer.U8Data,2);//������
				Tower_Operation(506,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("�����߶ȿ����÷�ΧΪ0~655.35m\r\n");
			}
			break;
		}
		case 207:
		{//�޸Ķ����
			FourBytes buffer;
			float a = Calculate_CharToFloat(p+4,8);//��������
			if(a<99999.99)
			{
				printf("����ֵΪ��%fKG\r\n",a);
				buffer.U32Data = (u32)(a*100);
				Tower_Operation(207,buffer.U8Data,4);//�޸�����
				Tower_Operation(7,buffer.U8Data,4);//������
				Tower_Operation(507,buffer.U8Data,4);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("��������ؿ����÷�ΧΪ0~99999.99KG\r\n");
			}
			break;
		}
		case 208:
		{//�޸��׶�����
			FourBytes buffer;
			float a = Calculate_CharToFloat(p+4,8);//��������
			if(a<99999.99)
			{
				printf("����ֵΪ��%fKG\r\n",a);
				buffer.U32Data = (u32)(a*100);
				Tower_Operation(208,buffer.U8Data,4);//�޸�����
				Tower_Operation(8,buffer.U8Data,4);//������
				Tower_Operation(508,buffer.U8Data,4);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("�����׶����ؿ����÷�ΧΪ0~99999.99KG\r\n");
			}
			break;
		}
		case 209:
		{//�޸�ĩ������
			FourBytes buffer;
			float a = Calculate_CharToFloat(p+4,8);//��������
			if(a<99999.99)
			{
				printf("����ֵΪ��%fKG\r\n",a);
				buffer.U32Data = (u32)(a*100);
				Tower_Operation(209,buffer.U8Data,4);//�޸�����
				Tower_Operation(9,buffer.U8Data,4);//������
				Tower_Operation(509,buffer.U8Data,4);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("����ĩ�����ؿ����÷�ΧΪ0~99999.99KG\r\n");
			}
			break;
		}
		case 210:
		{//�޸�������������
			u16 year = Calculate_CharToU16(p+4,4);//��������
			u8 month	= Calculate_CharToU16(p+9,2);//��������
			u8 day	= Calculate_CharToU16(p+12,2);//��������
			if(year >1899 && year <2100 && month >0 && month <13 && day>0 && day < 32)
			{
				u8 buffer[4];
				TwoBytes a;
				a.U16Data = year;
				buffer[0] = a.U8Data[0];
				buffer[1] = a.U8Data[1];
				buffer[2] = month;
				buffer[3] = day;
				printf("������ֵΪ��%u��\r\n",year);
				printf("������ֵΪ��%u��\r\n",month);
				printf("������ֵΪ��%u��\r\n",day);
				Tower_Operation(210,buffer,4);//�޸�����
				Tower_Operation(10,buffer,4);//������
				Tower_Operation(510,buffer,4);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("����������뷶ΧΪ1900~2099��\r\n");
				printf("�²��������뷶ΧΪ1~12��\r\n");
				printf("�ղ��������뷶ΧΪ1~31��\r\n");
			}
			break;
		}
		
		case 221:
		{//�޸����л�ת����
			break;
		}
		case 222:
		{//�޸Ļ�ת����ֲ���
			TwoBytes buffer;
			buffer.U16Data = Calculate_CharToU16(p+4,5);//��������
			if(buffer.U16Data<60000)
			{
				printf("����ֵΪ��%u\r\n",buffer.U16Data);
				Tower_Operation(222,buffer.U8Data,2);//�޸�����
				Tower_Operation(22,buffer.U8Data,2);//������
				Tower_Operation(522,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("��ת����ֲ��������÷�ΧΪ0~60000\r\n");
			}
			break;
		}
		case 223:
		{//�޸Ļ�תС���ֲ���
			TwoBytes buffer;
			buffer.U16Data = Calculate_CharToU16(p+4,5);//��������
			if(buffer.U16Data<60000)
			{
				printf("����ֵΪ��%u\r\n",buffer.U16Data);
				Tower_Operation(223,buffer.U8Data,2);//�޸�����
				Tower_Operation(23,buffer.U8Data,2);//������
				Tower_Operation(523,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("��תС���ֲ��������÷�ΧΪ0~60000\r\n");
			}
			break;
		}
		case 241:
		{//�޸����б������
			break;
		}
		case 242:
		{//�޸ı���������
			TwoBytes buffer;
			buffer.U16Data = Calculate_CharToU16(p+4,5);
			if(buffer.U16Data<60000)
			{
				printf("����ֵΪ��%u\r\n",buffer.U16Data);
				Tower_Operation(242,buffer.U8Data,2);//�޸�����
				Tower_Operation(42,buffer.U8Data,2);//������
				Tower_Operation(542,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("�������������������÷�ΧΪ0~60000\r\n");
			}
			break;
		}
		case 243:
		{//�޸ı��С������
			TwoBytes buffer;
			buffer.U16Data = Calculate_CharToU16(p+4,5);
			if(buffer.U16Data<60000)
			{
				printf("����ֵΪ��%u\r\n",buffer.U16Data);
				Tower_Operation(243,buffer.U8Data,2);//�޸�����
				Tower_Operation(43,buffer.U8Data,2);//������
				Tower_Operation(543,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("�������������������÷�ΧΪ0~60000\r\n");
			}
			break;
		}
		case 244:
		{//�޸ı����Ͳ�ײ�ֱ��
			float a = Calculate_CharToFloat(p+4,6);
			if(a<6553.5)
			{
				TwoBytes buffer;
				printf("����ֵΪ��%f\r\n",a);
				buffer.U16Data = (u16)(a*10);
				Tower_Operation(244,buffer.U8Data,2);//�޸�����
				Tower_Operation(44,buffer.U8Data,2);//������
				Tower_Operation(544,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("�����Ͳ�ײ�ֱ�����������÷�ΧΪ0~6553.5mm\r\n");
			}
			break;
		}
		case 245:
		{//�޸ı����Ͳ��Ч����
			float a = Calculate_CharToFloat(p+4,6);
			if(a<6553.5)
			{
				TwoBytes buffer;
				printf("����ֵΪ��%f\r\n",a);
				buffer.U16Data = (u16)(a*10);
				Tower_Operation(245,buffer.U8Data,2);//�޸�����
				Tower_Operation(45,buffer.U8Data,2);//������
				Tower_Operation(545,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("�����Ͳ��Ч���Ȳ��������÷�ΧΪ0~6553.5mm\r\n");
			}
			break;
		}
		case 246:
		{//�޸ı����Ͳ�ݾ�
			float a = Calculate_CharToFloat(p+4,6);
			if(a<655.35)
			{
				TwoBytes buffer;
				printf("����ֵΪ��%f\r\n",a);
				buffer.U16Data = (u16)(a*100);
				Tower_Operation(246,buffer.U8Data,2);//�޸�����
				Tower_Operation(46,buffer.U8Data,2);//������
				Tower_Operation(546,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("�����Ͳ�ݾ���������÷�ΧΪ0~655.35mm\r\n");
			}
			break;
		}
		case 247:
		{//�޸ı����˿��ֱ��
			float a = Calculate_CharToFloat(p+4,6);
			if(a<655.35)
			{
				TwoBytes buffer;
				printf("����ֵΪ��%f\r\n",a);
				buffer.U16Data = (u16)(a*100);
				Tower_Operation(247,buffer.U8Data,2);//�޸�����
				Tower_Operation(47,buffer.U8Data,2);//������
				Tower_Operation(547,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("�����˿��ֱ�����������÷�ΧΪ0~655.35mm\r\n");
			}
			break;
		}
		case 248:
		{//�޸ı����˿������
			u8 a = Calculate_CharToFloat(p+4,3);
			if(a<250)
			{
				printf("����ֵΪ��%u\r\n",a);
				Tower_Operation(248,&a,1);//�޸�����
				Tower_Operation(48,&a,1);//������
				Tower_Operation(548,&a,1);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("�����˿���������������÷�ΧΪ0~250\r\n");
			}
			break;
		}
		case 249:
		{//�޸ı����˿����Чת��
			float a = Calculate_CharToFloat(p+4,6);
			if(a<655.35)
			{
				TwoBytes buffer;
				printf("����ֵΪ��%f\r\n",a);
				buffer.U16Data = a * 100;
				Tower_Operation(249,buffer.U8Data,2);//�޸�����
				Tower_Operation(49,buffer.U8Data,2);//������
				Tower_Operation(549,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("�����˿����Чת�����������÷�ΧΪ0~655.35\r\n");
			}
			break;
		}
		case 261:
		{//�޸����е��߲���
			break;
		}
		case 262:
		{//�޸ĵ��ߴ������
			TwoBytes buffer;
			buffer.U16Data = Calculate_CharToU16(p+4,5);
			if(buffer.U16Data<60000)
			{
				printf("����ֵΪ��%u\r\n",buffer.U16Data);
				Tower_Operation(262,buffer.U8Data,2);//�޸�����
				Tower_Operation(62,buffer.U8Data,2);//������
				Tower_Operation(562,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("���ߴ���������������÷�ΧΪ0~60000\r\n");
			}
			break;
		}
		case 263:
		{//�޸ĵ���С������
			TwoBytes buffer;
			buffer.U16Data = Calculate_CharToU16(p+4,5);
			if(buffer.U16Data<60000)
			{
				printf("����ֵΪ��%u\r\n",buffer.U16Data);
				Tower_Operation(263,buffer.U8Data,2);//�޸�����
				Tower_Operation(63,buffer.U8Data,2);//������
				Tower_Operation(563,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("����С���������������÷�ΧΪ0~60000\r\n");
			}
			break;
		}
		case 264:
		{//�޸ĵ��߾�Ͳ�ײ�ֱ��
			float a = Calculate_CharToFloat(p+4,6);
			if(a<6553.5)
			{
				TwoBytes buffer;
				printf("����ֵΪ��%f\r\n",a);
				buffer.U16Data = (u16)(a*10);
				Tower_Operation(264,buffer.U8Data,2);//�޸�����
				Tower_Operation(64,buffer.U8Data,2);//������
				Tower_Operation(564,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("���߾�Ͳ�ײ�ֱ�����������÷�ΧΪ0~6553.5mm\r\n");
			}
			break;
		}
		case 265:
		{//�޸ĵ��߾�Ͳ��Ч����
			float a = Calculate_CharToFloat(p+4,6);
			if(a<6553.5)
			{
				TwoBytes buffer;
				printf("����ֵΪ��%f\r\n",a);
				buffer.U16Data = (u16)(a*10);
				Tower_Operation(265,buffer.U8Data,2);//�޸�����
				Tower_Operation(65,buffer.U8Data,2);//������
				Tower_Operation(565,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("���߾�Ͳ��Ч���Ȳ��������÷�ΧΪ0~6553.5mm\r\n");
			}
			break;
		}
		case 266:
		{//�޸ĵ��߾�Ͳ�ݾ�
			float a = Calculate_CharToFloat(p+4,6);
			if(a<655.35)
			{
				TwoBytes buffer;
				printf("����ֵΪ��%f\r\n",a);
				buffer.U16Data = (u16)(a*100);
				Tower_Operation(266,buffer.U8Data,2);//�޸�����
				Tower_Operation(66,buffer.U8Data,2);//������
				Tower_Operation(566,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("���߾�Ͳ�ݾ���������÷�ΧΪ0~655.35mm\r\n");
			}
			break;
		}
		case 267:
		{//�޸ĵ��߸�˿��ֱ��
			float a = Calculate_CharToFloat(p+4,6);
			if(a<655.35)
			{
				TwoBytes buffer;
				printf("����ֵΪ��%f\r\n",a);
				buffer.U16Data = (u16)(a*100);
				Tower_Operation(267,buffer.U8Data,2);//�޸�����
				Tower_Operation(67,buffer.U8Data,2);//������
				Tower_Operation(567,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("���߸�˿��ֱ�����������÷�ΧΪ0~655.35mm\r\n");
			}
			break;
		}
		case 268:
		{//�޸ĵ��߸�˿������
			u8 a = Calculate_CharToU16(p+4,3);
			if(a<250)
			{
				printf("����ֵΪ��%u\r\n",a);
				Tower_Operation(268,&a,1);//�޸�����
				Tower_Operation(68,&a,1);//������
				Tower_Operation(568,&a,1);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("���߸�˿���������������÷�ΧΪ0~250\r\n");
			}
			break;
		}
		case 269:
		{//�޸ĵ��߸�˿����Чת��
			float a = Calculate_CharToFloat(p+4,6);
			if(a<6553.5)
			{
				TwoBytes buffer;
				printf("����ֵΪ��%f\r\n",a);
				buffer.U16Data = a * 10;
				Tower_Operation(269,buffer.U8Data,2);//�޸�����
				Tower_Operation(69,buffer.U8Data,2);//������
				Tower_Operation(569,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("���߸�˿����Чת�����������÷�ΧΪ0~6553.5\r\n");
			}
			break;
		}
		case 270:
		{//�޸ĵ��߸�˿������
			u8 a = Calculate_CharToU16(p+4,3);
			if(a<250)
			{
				printf("����ֵΪ��%u\r\n",a);
				Tower_Operation(270,&a,1);//�޸�����
				Tower_Operation(70,&a,1);//������
				Tower_Operation(570,&a,1);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("���߸�˿�����ʲ��������÷�ΧΪ0~250\r\n");
			}
			break;
		}
		case 271:
		{//�޸ĵ��߸�˿������
			float a = Calculate_CharToFloat(p+4,6);
			if(a<6553.5)
			{
				TwoBytes buffer;
				buffer.U16Data = a * 10;
				printf("����ֵΪ��%f\r\n",a);
				Tower_Operation(271,buffer.U8Data,2);//�޸�����
				Tower_Operation(71,buffer.U8Data,2);//������
				Tower_Operation(571,buffer.U8Data,2);//��ʾ����
			}
			else
			{
				printf("�������\r\n");
				printf("���߸�˿���������������÷�ΧΪ0~6553.5\r\n");
			}
			break;
		}
		
		case 501:
		{//��ʾ������������
			u8 buffer[TOWER_AdrStoreEnd];
			Tower_Operation(1,buffer,TOWER_AdrStoreEnd);//������
			Tower_Operation(501,buffer,TOWER_AdrStoreEnd);//��ʾ����
			break;
		}
		case 502:
		{//��ʾ�������
			u8 buffer[4];
			Tower_Operation(2,buffer,4);//������
			Tower_Operation(502,buffer,4);//��ʾ����
			break;
		}
		case 503:
		{//��ʾ�����ͺ�
			u8 buffer[21];
			Tower_Operation(3,buffer,21);//������
			Tower_Operation(503,buffer,21);//��ʾ����
			break;
		}
		case 504:
		{//��ʾ����������
			u8 buffer[21];
			Tower_Operation(4,buffer,21);//������
			Tower_Operation(504,buffer,21);//��ʾ����
			break;
		}
		case 505:
		{//��ʾ�����۳�
			u8 buffer[2];
			Tower_Operation(5,buffer,2);//������
			Tower_Operation(505,buffer,2);//��ʾ����
			break;
		}
		case 506:
		{//��ʾ�����߶�
			u8 buffer[2];
			Tower_Operation(6,buffer,2);//������
			Tower_Operation(506,buffer,2);//��ʾ����
			break;
		}
		case 507:
		{//��ʾ�����
			u8 buffer[4];
			Tower_Operation(7,buffer,4);//������
			Tower_Operation(507,buffer,4);//��ʾ����
			break;
		}
		case 508:
		{//��ʾ�׶�����
			u8 buffer[4];
			Tower_Operation(8,buffer,4);//������
			Tower_Operation(508,buffer,4);//��ʾ����
			break;
		}
		case 509:
		{//��ʾ�׶�����
			u8 buffer[4];
			Tower_Operation(9,buffer,4);//������
			Tower_Operation(509,buffer,4);//��ʾ����
			break;
		}
		case 510:
		{//��ʾ��������
			u8 buffer[4];
			Tower_Operation(10,buffer,4);//������
			Tower_Operation(510,buffer,4);//��ʾ����
			break;
		}
		case 521:
		{//��ʾ���л�ת����
			u8 buffer[TOWER_AdrStoreHZEnd-TOWER_AdrStoreHZBase];
			Tower_Operation(21,buffer,TOWER_AdrStoreHZEnd-TOWER_AdrStoreHZBase);//������
			Tower_Operation(521,buffer,TOWER_AdrStoreHZEnd-TOWER_AdrStoreHZBase);//��ʾ����
			break;
		}
		case 522:
		{//��ʾ��ת�������
			u8 buffer[2];
			Tower_Operation(22,buffer,2);//������
			Tower_Operation(522,buffer,2);//��ʾ����
			break;
		}
		case 523:
		{//��ʾ��תС������
			u8 buffer[2];
			Tower_Operation(23,buffer,2);//������
			Tower_Operation(523,buffer,2);//��ʾ����
			break;
		}
		case 541:
		{//��ʾ���б������
			u8 buffer[TOWER_AdrStoreBFEnd-TOWER_AdrStoreBFBase];
			Tower_Operation(41,buffer,TOWER_AdrStoreBFEnd-TOWER_AdrStoreBFBase);//������
			Tower_Operation(541,buffer,TOWER_AdrStoreBFEnd-TOWER_AdrStoreBFBase);//��ʾ����
			break;
		}
		case 542:
		{//��ʾ����������
			u8 buffer[2];
			Tower_Operation(42,buffer,2);//������
			Tower_Operation(542,buffer,2);//��ʾ����
			break;
		}
		case 543:
		{//��ʾ���С������
			u8 buffer[2];
			Tower_Operation(43,buffer,2);//������
			Tower_Operation(543,buffer,2);//��ʾ����
			break;
		}
		case 544:
		{//��ʾ�����Ͳ�ײ�ֱ��
			u8 buffer[2];
			Tower_Operation(44,buffer,2);//������
			Tower_Operation(544,buffer,2);//��ʾ����
			break;
		}
		case 545:
		{//��ʾ�����Ͳ��Ч����
			u8 buffer[2];
			Tower_Operation(45,buffer,2);//������
			Tower_Operation(545,buffer,2);//��ʾ����
			break;
		}
		case 546:
		{//��ʾ�����Ͳ�ݾ�
			u8 buffer[2];
			Tower_Operation(46,buffer,2);//������
			Tower_Operation(546,buffer,2);//��ʾ����
			break;
		}
		case 547:
		{//��ʾ�����˿��ֱ��
			u8 buffer[2];
			Tower_Operation(47,buffer,2);//������
			Tower_Operation(547,buffer,2);//��ʾ����
			break;
		}
		case 548:
		{//��ʾ�����˿������
			u8 buffer;
			Tower_Operation(48,&buffer,1);//������
			Tower_Operation(548,&buffer,1);//��ʾ����
			break;
		}
		case 549:
		{//��ʾ�����˿����Чת��
			u8 buffer[2];
			Tower_Operation(49,buffer,2);//������
			Tower_Operation(549,buffer,2);//��ʾ����
			break;
		}
		case 561:
		{//��ʾ���е��߲���
			u8 buffer[TOWER_AdrStoreDGEnd - TOWER_AdrStoreDGBase];
			Tower_Operation(61,buffer,TOWER_AdrStoreDGEnd - TOWER_AdrStoreDGBase);//������
			Tower_Operation(561,buffer,TOWER_AdrStoreDGEnd - TOWER_AdrStoreDGBase);//��ʾ����
			break;
		}
		case 562:
		{//��ʾ���ߴ������
			u8 buffer[2];
			Tower_Operation(62,buffer,2);//������
			Tower_Operation(562,buffer,2);//��ʾ����
			break;
		}
		case 563:
		{//��ʾ����С������
			u8 buffer[2];
			Tower_Operation(63,buffer,2);//������
			Tower_Operation(563,buffer,2);//��ʾ����
			break;
		}
		case 564:
		{//��ʾ���߾�Ͳ�ײ�ֱ��
			u8 buffer[2];
			Tower_Operation(64,buffer,2);//������
			Tower_Operation(564,buffer,2);//��ʾ����
			break;
		}
		case 565:
		{//��ʾ���߾�Ͳ��Ч����
			u8 buffer[2];
			Tower_Operation(65,buffer,2);//������
			Tower_Operation(565,buffer,2);//��ʾ����
			break;
		}
		case 566:
		{//��ʾ���߾�Ͳ�ݾ�
			u8 buffer[2];
			Tower_Operation(66,buffer,2);//������
			Tower_Operation(566,buffer,2);//��ʾ����
			break;
		}
		case 567:
		{//��ʾ���߸�˿��ֱ��
			u8 buffer[2];
			Tower_Operation(67,buffer,2);//������
			Tower_Operation(567,buffer,2);//��ʾ����
			break;
		}
		case 568:
		{//��ʾ���߸�˿������
			u8 buffer;
			Tower_Operation(68,&buffer,1);//������
			Tower_Operation(568,&buffer,1);//��ʾ����
			break;
		}
		case 569:
		{//��ʾ���߸�˿����Чת��
			u8 buffer[2];
			Tower_Operation(69,buffer,2);//������
			Tower_Operation(569,buffer,2);//��ʾ����
			break;
		}
		case 570:
		{//��ʾ���߸�˿������
			u8 buffer;
			Tower_Operation(70,&buffer,1);//������
			Tower_Operation(570,&buffer,1);//��ʾ����
			break;
		}
		case 571:
		{//��ʾ���߸�˿������
			u8 buffer;
			Tower_Operation(71,&buffer,2);//������
			Tower_Operation(571,&buffer,2);//��ʾ����
			break;
		}
	}
}
//---
/*
	�������� void Tower_DebugRTU((u8 *p)
	������		�ú���ΪTower�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*/
void Tower_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//�鿴���ĸ�������
	{
		//-----------------------------------------
		case 0x01:
		{
			break;
		}
	}  
}
//---





















