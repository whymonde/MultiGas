/*
	���ļ�Ϊ����Ŀ����Ļ��غ���
	���庯������
*/

#include "screen.h"

Screen_DataStructure ScreenData;
/*
	��������void Screen_Main(void)
	������	heat.c������
*/
void Screen_Main(void)
{
	
}
//---
/*
	��������void Screen_Timer(void)
	������screen.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void Screen_Timer(void)
{
	
}
//---
/*
	��������void Screen_Init(void)
	������	screen.c�ļ���ʼ������
*/
void Screen_Init(void)
{
	
}
//---
/*
	��������Screen_DataStructure* Screen_GetData(void)
	��������ȡ����screen.c�ļ�ȫ�ֱ����ṹ�����
*/
Screen_DataStructure* Screen_GetData(void)
{
	return &ScreenData;
}//-------------------------���ݶ�ȡ���洢----------------------------
//---
/*
	��������void Screen_ReadData(u8 address,u8 *p,u8 len)
	������	��ȡ����
*/
void Screen_ReadData(u8 address,u8 *p,u8 len)
{
//	W25Xxx_ReadData(p,AD7689_AdrStoreBase+address,len);
}
//---
/*
	��������void Screen_StoreData(u8 address,u8 *p,u8 len)
	�������ú���Ϊ�洢Screen.c���ò�������
	˵������ȡ����
				��������
				�޸�����
				��������
*/
void Screen_StoreData(u8 address,u8 *p,u8 len)
{
//	u8 buffer[AD7689_AdrStoreEND],cnt;
//	W25Xxx_ReadData(buffer,AD7689_AdrStoreBase,AD7689_AdrStoreEND);//���������ò���
//	W25Xxx_EraseSector(AD7689_AdrStoreBase);//��������
//	for(cnt=0;cnt<len;cnt++)
//	{
//		buffer[address++] = *p;
//		p++;
//	}
//	W25Xxx_PageWrite(buffer,AD7689_AdrStoreBase,AD7689_AdrStoreEND);//��������
}
//---
/*
	��������void Screen_FactoryReset(void)
	�������ú���Screen.c�洢���ݻָ���������
*/
void Screen_FactoryReset(void)
{
	//AD4ͨ��
//	i = 4;
//	AD7689_StoreData(AD7689_AdrStoreAD4Map,&i,1);//�豸ӳ��
//	two.U16Data = 100;
//	AD7689_StoreData(AD7689_AdrStoreAD4Orgin,two.U8Data,2);//ԭ��ֵ
//	two.U16Data = 60000;
//	AD7689_StoreData(AD7689_AdrStoreAD4Endpoint,two.U8Data,2);//�˵�ֵ
//	four.FloatData = 50000.0;
//	AD7689_StoreData(AD7689_AdrStoreAD4Refer,four.U8Data,4);//����ֵ
	//��ʼ����־λ
//	two.U8Data[0] = AD7689_FACTORY;
//	AD7689_StoreData(AD7689_AdrStoreFactory,two.U8Data,1);//�ָ��������ñ�־
	printf("Screen������ʼ��\r\n");
}
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Screen_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪScreen.c�ļ���ASCII�����ļ�
*/
void Screen_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:
		{
			break;
		}
		case 2:
		{
			break;
		}
	}
}
//---
/*
	�������� 	void Screen_DebugRTU((u8 *p)
	������		�ú���ΪScreen.c�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*//*
void Screen_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//�鿴���ĸ�������
	{
		case 0x01:
		{
			break;
		}
		case 0x02:
		{
			break;
		}
	}
}*/
//---





































