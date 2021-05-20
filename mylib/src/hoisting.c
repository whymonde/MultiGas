/*
	���ļ�Ϊ����Ŀ�ĵ��ش�������غ����ļ�
	���庯������
*/

#include "hoisting.h"


Hoisting_DataStructure HoistingData;//Hoisting�ṹ�����
/*
	��������void Hoisting_Init(void)
	����:		�ú���Ϊhoisting.c�ļ���ʼ������
*/
void Hoisting_Init(void)
{
	Hoisting_DataInit();
}
//---
/*
	��������void Hoisting_DataInit(void)
	������	�ú���Ϊ��
*/
void Hoisting_DataInit(void)
{
//	HoistingData.Flag = 0;
	HoistingData.Address = HOISTING_DefaultAddress;
	HoistingData.Weight = 0;
}
//---
/*
	��������Hoisting_DataStructure* Hoisting_GetData(void)
	������	�ú���Ϊ��ȡHoistingData������ַ
*/
Hoisting_DataStructure* Hoisting_GetData(void)
{
	return &HoistingData;
}
//---
/*
	��������void Hoisting_CollectData(void)
	�������ú���Ϊ���Ͳɼ����ش���������
	03 03 00 00 00 02 C5 E9
*/
void Hoisting_CollectData(void)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = HoistingData.Address;
	buffer[cnt++] = 0x03;//��ȡ����
	buffer[cnt++] = 0x00;//��ʼ��ַ0x0000��2�ֽ�
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x00;//���ݳ���1���֣�2�ֽ�
	buffer[cnt++] = 0x02;
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	for(crc=0;crc<cnt;crc++)
	{
		Hoistin_SendBufferData(Hoisting_UsartPort,Hoisting_UsartBuffer,buffer[crc]);//��������
	}
	HoistingData.RunMode = HOISTING_RunModeColHoisting;
}
//---
/*
	��������void Hoisting_ReadAllData(void)
	������	�ú���Ϊ��ȡ���ش������ڲ��Ĵ������е�32���ֵ�ֵ
*/
void Hoisting_ReadAllData(void)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = HoistingData.Address;
	buffer[cnt++] = 0x03;//��ȡ����
	buffer[cnt++] = 0x00;//��ʼ��ַ0x0000��2�ֽ�
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x00;//���ݳ���1���֣�2�ֽ�
	buffer[cnt++] = 0x20;
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	for(crc=0;crc<cnt;crc++)
	{
		Hoistin_SendBufferData(Hoisting_UsartPort,Hoisting_UsartBuffer,buffer[crc]);//��������
	}
	HoistingData.RunMode = HOISTING_RunModeReadAllData;
}
//---
//-------------------------------------���Ժ���----------------------------------------
//---
/*
	�������� void Hoisting_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪHoisting.c�ļ���ASCII�����ļ�
	:000|59|XXX������������
*/
void Hoisting_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1://
		{//:000|59|001
			Hoisting_SetComNoneVerify();//��У��
			Hoisting_CollectData();			//��������
			printf("���Ͳɼ����ش���������!\r\n");
			break;
		}
		default:
		{
			printf("Wind.c���յ���Ч����ָ��%u!\r\n",cmd);
			break;
		}
	}
}
//---
/*
	�������� void Hoisting_DebugRTU((u8 *p)
	������	�ú���ΪHoisting.c�ļ���RTU�����ļ�
	˵����	���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*/
void Hoisting_DebugRTU(u8 *p,u8 len)
{
	switch(HoistingData.RunMode)//�鿴���ĸ�������
	{
		case HOISTING_RunModeColHoisting:
		{//��ȡ����
			u32 weight = *(p+3)<<24 | *(p+4)<<16 | *(p+5)<<8 | *(p+6);//��ȡ����
			Server_GetTowerCraneData()->Hoisting = (u16)((float)weight/100000000.0);
//			printf("��ǰ����Ϊ%u\r\n",Server_GetTowerCraneData()->Hoisting);
			break;
		}
		case HOISTING_RunModeReadAllData:
		{//��ȡ����
			u32 weight = *(p+3)<<24 | *(p+4)<<16 | *(p+5)<<8 | *(p+6);//��ȡ����
			Server_GetTowerCraneData()->Hoisting = (u16)((float)weight/100000000.0);
//			printf("��ǰ����Ϊ%u\r\n",weight);
			break;
		}
	}
	HoistingData.RunMode = HOISTING_RunModeNone;
}
//---























