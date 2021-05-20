/*
	���ļ�Ϊ����Ŀ�ĳ��ش�������غ���
	���庯������
*/

#include "THBSS01.h"

Thbss01_DataStructure Thbss01Data;
/*
	��������void Thbss01_Main(void)
	������	��Ļ��ʾ������
*/
void Thbss01_Main(void)
{
	switch(Thbss01Data.RunMode)
	{
		case 1:
		{
			if(Thbss01Data.CollectMode == 0)
			{
				Thbss01_Operation(2,0,0);
				Thbss01Data.Timer = 200;
				Thbss01Data.RunMode = 2;
			}
			break;
		}
		case 2:
		{
			if((Thbss01Data.CollectMode == 0) || (Thbss01Data.Timer == 0))
			{
				Thbss01Data.Timer = 100;
				Thbss01Data.RunMode = 3;
			}
			break;
		}
		case 3:
		{
			if(Thbss01Data.Timer == 0)
			{
				Thbss01Data.RunMode = 0;
			}
		}
	}
}
//---
/*
	��������void Thbss01_Init(void)
	������	Thbss01.c�ļ���ʼ������
*/
void Thbss01_Init(void)
{
	Thbss01_DataInit();
}
//---
/*
	��������void Thbss01_DataInit(void)
*/
void Thbss01_DataInit(void)
{
	TwoBytes two;
	Thbss01_Operation(202,two.U8Data,1);
	Thbss01Data.Conf.ADR = two.U8Data[0];
	printf("���ش�����ͨѶ��ַ0x%X\r\n",Thbss01Data.Conf.ADR);
	Thbss01_Operation(203,two.U8Data,1);
	Thbss01Data.Conf.BR = two.U16Data;
	printf("���ش�����ͨѶ������%u\r\n",Thbss01Data.Conf.BR);
}
//---
/*
	��������void Thbss01_Timer(void)
	������Thbss01.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void Thbss01_Timer(void)
{
	if(Thbss01Data.Timer != 0)Thbss01Data.Timer--;
}
//---
/*
	��������Thbss01_DataStructure* Thbss01_GetThbss01Data(void)
	��������ȡ����Thbss01.c�ļ�ȫ�ֱ����ṹ��
*/
Thbss01_DataStructure* Thbss01_GetThbss01Data(void)
{
	return &Thbss01Data;
}
//---
/*
	��������void Thbss01_Operation(u8 mode,u8 *p,u8 len)
	˵����Thbss01�����ݲ���
*/
void Thbss01_Operation(u16 mode,u8 *p,u8 len)
{
	switch(mode)
	{
		case 1:
		{//���Ͷ�ȡTHBSS01������������
			Thbss01_ReadData(THBSS01_AdrBase,30);
			Thbss01Data.CollectMode = 0x01;
			break;
		}
		case 2:
		{//���Ͷ�ȡTHBSS01����ֵ����
			Thbss01_ReadData(THBSS01_AdrWeight,2);
			Thbss01Data.CollectMode = 0x02;
			break;
		}
		case 101:
		{//����д���ֵ����
			Thbss01_WriteOneWord(THBSS01_AdrCalibration,1);
			Thbss01Data.CollectMode = 0x81;
			break;
		}
		case 102:
		{//����д����ֵ����
			Thbss01_WriteOneWord(THBSS01_AdrCalibration,2);
			Thbss01Data.CollectMode = 0x82;
			break;
		}
		case 103:
		{//������������ֵ
			FourBytes four;
			four.U8Data[1] = p[0];
			four.U8Data[0] = p[1];
			four.U8Data[3] = p[2];
			four.U8Data[2] = p[3];
			Thbss01_WriteMultiWord(THBSS01_AdrFaMa,four.U16Data,2);
			Thbss01Data.CollectMode = 0x83;
			break;
		}
		case 104:
		{//ȥƤ����
			Thbss01_WriteOneWord(THBSS01_AdrShelling,1);
			Thbss01Data.CollectMode = 0x84;
			break;
		}
		case 105:
		{//ȡ��ȥƤ����
			Thbss01_WriteOneWord(THBSS01_AdrShelling,2);
			Thbss01Data.CollectMode = 0x85;
			break;
		}
		case 121:
		{//����ͨѶ��ַ��
			*p = *p - THBSS01_AddressSkewing;//��ȥƫ��ֵ
			Thbss01_WriteOneWord(THBSS01_AdrBaudRate,*p);
			Thbss01Data.CollectMode = 0xA1;
			break;
		}
		case 122:
		{//����ͨѶ������
			TwoBytes BR;
			BR.U8Data[0] = *p;
			BR.U8Data[1] = *(p+1);
			Thbss01_WriteOneWord(THBSS01_AdrBaudRate,BR.U16Data);
			Thbss01Data.CollectMode = 0xA2;
			break;
		}		
		case 201:
		{//��ȡThbss01�����ò���
			u8 buffer[THBSS01_AdrStoreEnd];
			TwoBytes a;
			Thbss01_ReadStoreData(THBSS01_AdrConfBase,buffer,THBSS01_AdrStoreEnd);
			a.U8Data[0] = buffer[THBSS01_AdrStoreBR];
			a.U8Data[1] = buffer[THBSS01_AdrStoreBR+1];
			printf("�洢��EEPROM��THBSS01ͨѶ��ַΪ��%u\r\n",buffer[THBSS01_AdrConfBase]);
			printf("�洢��EEPROM��THBSS01ͨѶ������Ϊ��%u\r\n",a.U16Data);
			break;
		}
		case 202:
		{//��ȡ�洢��EEPROM��Thbss01��ͨѶ��ַ
			Thbss01_ReadStoreData(THBSS01_AdrStoreADR,p,1);
//			printf("�洢��EEPROM��THBSS01ͨѶ��ַΪ��%u\r\n",address);
			break;
		}
		case 203:
		{//��ȡ�洢��EEPROM��Thbss01��ͨѶ������
			Thbss01_ReadStoreData(THBSS01_AdrStoreBR,p,2);
//			printf("�洢��EEPROM��THBSS01ͨѶ������Ϊ��%u\r\n",a.U16Data);
			break;
		}
		case 301:
		{//�޸�EEPROM��Thbss01�洢����
			Thbss01_StoreData(THBSS01_AdrConfBase,p,len);
			Thbss01_DataInit();
			break;
		}
		case 302:
		{//�޸�EEPROM��Thbss01�洢ͨѶ��ַ
			Thbss01_StoreData(THBSS01_AdrStoreADR,p,1);
			Thbss01_DataInit();
			break;
		}
		case 303:
		{//�޸�EEPROM��Thbss01�洢ͨѶ������
			Thbss01_StoreData(THBSS01_AdrStoreBR,p,2);
			Thbss01_DataInit();
			break;
		}
	}
}
//---
/*
	��������void Thbss01_ReadData(u16 address,u8 amount)
	���������Ͷ�ȡ��������
	���룺u16 address��ȡ���ݵ���ʼ��ַ
				u8 amount��ȡ���ݵĸ���������Ϊ��λ
*/
void Thbss01_ReadData(u16 address,u16 amount)
{
	u8 buffer[8],cnt=0;
	TwoBytes change;
	buffer[cnt++] = Thbss01Data.Conf.ADR;
	buffer[cnt++] = 0x03;//��ȡ����
	change.U16Data = address;
	buffer[cnt++] = change.U8Data[1];//��ʼ��ַ0x0100��2�ֽ�
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = amount;
	buffer[cnt++] = change.U8Data[1];//���ݳ���0x0034(52��������)��2�ֽ�
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = CRC16(buffer,cnt);
	buffer[cnt++] = change.U8Data[1];//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = change.U8Data[0];
	Thbss01_UsartModeSet();
	for(change.U16Data=0;change.U16Data<cnt;change.U16Data++)
	{
		THBSS01_SendBufferData(THBSS01_UsartPort,THBSS01_UsartBuffer(),buffer[change.U16Data]);//��������
	}
	Thbss01Data.Timer = 200;//������ʱ
}
//---
/*
	��������void Thbss01_WriteOneWord(u16 address,u16 data)
	������	�ú���Ϊ��Ŀ��Ĵ���д��һ��������
*/
void Thbss01_WriteOneWord(u16 address,u16 data)
{
	u8 buffer[8],cnt=0;
	TwoBytes change;
	buffer[cnt++] = (u8)Thbss01Data.Conf.ADR;
	buffer[cnt++] = 0x06;					//д��һ��������
	change.U16Data = address;
	buffer[cnt++] = change.U8Data[1];//��ַ0x0100��2�ֽ�
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = data;
	buffer[cnt++] = change.U8Data[1];//д������ݣ�2�ֽ�
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = CRC16(buffer,cnt);
	buffer[cnt++] = change.U8Data[1];//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = change.U8Data[0];
	Thbss01_UsartModeSet();
	for(change.U16Data=0;change.U16Data<cnt;change.U16Data++)
	{
		Ohrd23_SendBufferData(Ohrd23_UsartPort,Ohrd23_UsartBuffer(),buffer[change.U16Data]);//��������
	}
	Thbss01Data.Timer = 200;//������ʱ
}
//---
/*
	��������void Thbss01_WriteMultiWord(u16 address,u16 *p,u16 len)
	�������ú���Ϊ��Ŀ��Ĵ���д��д�����
*/
void Thbss01_WriteMultiWord(u16 address,u16 *p,u16 len)
{
	u8 buffer[200],cnt=0,i=0;
	TwoBytes change;
	buffer[cnt++] = (u8)Thbss01Data.Conf.ADR;
	buffer[cnt++] = 0x10;//д����������
	change.U16Data = address;
	buffer[cnt++] = change.U8Data[1];//��ַ
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = len;
	buffer[cnt++] = change.U8Data[1];//�����ֳ���
	buffer[cnt++] = change.U8Data[0];
	buffer[cnt++] = len*2;					//�ֽ���
	for(i=0;i<len;i++)
	{
		change.U16Data = *p;
		buffer[cnt++] = change.U8Data[1];//��ַ
		buffer[cnt++] = change.U8Data[0];
		p++;
	}
	change.U16Data = CRC16(buffer,cnt);
	buffer[cnt++] = change.U8Data[1];//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = change.U8Data[0];
	Thbss01_UsartModeSet();
	for(change.U16Data=0;change.U16Data<cnt;change.U16Data++)
	{
		THBSS01_SendBufferData(THBSS01_UsartPort,THBSS01_UsartBuffer(),buffer[change.U16Data]);//��������
	}
	Thbss01Data.Timer = 200;//������ʱ
}
//---
//-------------------------���ݶ�ȡ���洢----------------------------
//---
/*
	��������void Thbss01_ReadStoreData(u8 address,u8 *p,u8 len)
	������	��ȡ����
*/
void Thbss01_ReadStoreData(u8 address,u8 *p,u8 len)
{
	W25Xxx_ReadData(p,THBSS01_AdrStoreBase+address,len);
}
//---
/*
	��������void Thbss01_StoreData(u8 address,u8 *p,u8 len)
	�������ú���Ϊ�洢OHRD23���ò�������
	˵������ȡ����
				��������
				�޸�����
				��������
*/
void Thbss01_StoreData(u8 address,u8 *p,u8 len)
{
	u8 buffer[THBSS01_AdrStoreEnd],cnt;
	W25Xxx_ReadData(buffer,THBSS01_AdrStoreBase,THBSS01_AdrStoreEnd);//���������ò���
	W25Xxx_EraseSector(THBSS01_AdrStoreBase);//��������
	for(cnt=0;cnt<len;cnt++)
	{
		buffer[address++] = *p;
		p++;
	}
	W25Xxx_PageWrite(buffer,THBSS01_AdrStoreBase,THBSS01_AdrStoreEnd);//��������
}
//---
//-----------------Ӳ�����-----------------
//---
/*
	void Thbss01_UsartModeSet(void)
	������ΪThbss01���ô���ͨѶģʽ
*/
void Thbss01_UsartModeSet(void)
{
	u32 BaudRate=Thbss01Data.Conf.BR;
	THBSS01_UartModeSet(THBSS01_UsartPort,BaudRate,'n','1');
}
//---
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Thbss01_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪThbss01�ļ���ASCII�����ļ�
:000|85|001
*/
void Thbss01_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:
		{//��Thbss01��������
			printf("��Thbss01��������\r\n");
			Thbss01_Operation(1,p,0);
			break;
		}
		case 2:
		{
			printf("��Thbss01��ǰ����ֵ\r\n");
			Thbss01_Operation(2,p,0);
			break;
		}
		case 101:
		{
			printf("����д���ֵ����\r\n");
			Thbss01_Operation(101,p,0);
			break;
		}
		case 102:
		{
			printf("����д����ֵ����\r\n");
			Thbss01_Operation(102,p,0);
			break;
		}
		case 103:
		{//��������ֵ����  :000|86|103|123456
			FourBytes buffer;
			buffer.S32Data = Calculate_CharToU16(p+4,6);//��������
			buffer.S32Data = 99500;
			printf("����ֵ����Ϊ%fkg\r\n",buffer.S32Data/100.0);
			Thbss01_Operation(103,buffer.U8Data,4);
			break;
		}
		case 104:
		{//����ȥƤ��������
			printf("����ȥƤ��������\r\n");
			Thbss01_Operation(104,p,0);
			break;
		}
		case 105:
		{//����ȡ��ȥƤ��������
			printf("����ȡ��ȥƤ��������\r\n");
			Thbss01_Operation(105,p,0);
			break;
		}
		case 121:
		{//���õ�ַ
			u8 adr = Calculate_CharToU16(p+4,3);//��������
			if(adr > THBSS01_AddressSkewing)
			{
				printf("���õ�ֵַΪ%u\r\n",adr);
				adr = adr-THBSS01_AddressSkewing;
				Thbss01_Operation(121,&adr,1);
			}
			else
			{
				printf("�������\r\n");
				printf("��ַ�����÷�ΧΪ%u~253\r\n",THBSS01_AddressSkewing);
			}
			break;
		}
		case 122:
		{//���ô洢������
			TwoBytes a;
			a.U16Data = Calculate_CharToU16(p+4,5);//��ַ
			if(a.U16Data == 4800 || a.U16Data == 9600 || a.U16Data == 19200 || a.U16Data == 38400)
			{
				printf("���ò�����ֵΪ%u\r\n",a.U16Data);
				Thbss01_Operation(122,a.U8Data,2);
			}
			else
			{
				printf("�������\r\n");
				printf("ͨѶ�����ʿ������÷�ΧΪ4800��9600��19200��38400\r\n");
			}
			break;
		}
		case 201:
		{//��ȡ�洢����
			Thbss01_Operation(201,p,0);
			break;
		}
		case 202:
		{//��ȡ�洢Thbss01�����õ�ַ
			Thbss01_Operation(202,p,0);
			break;
		}
		case 203:
		{//��ȡ�洢Thbss01�����ò�����
			Thbss01_Operation(203,p,0);
			break;
		}
		case 301:
		{//�޸Ĵ洢���� :000|86|301|001|38400
			u8 buffer[3];
			TwoBytes a;
			buffer[0] = Calculate_CharToU16(p+4,3);//��ַ
			a.U16Data = Calculate_CharToU16(p+8,5);//������
			if((buffer[0]> 0 && buffer[0]<253) && \
				(a.U16Data == 4800 || a.U16Data == 9600 || a.U16Data == 19200 || a.U16Data == 38400))
			{
				buffer[1] = a.U8Data[0];
				buffer[2] = a.U8Data[1];
				Thbss01_Operation(301,buffer,3);
			}
			else
			{
				printf("�������\r\n");
				printf("��ַ�������÷�ΧΪ1~253��");
				printf("ͨѶ�����ʿ������÷�ΧΪ4800��9600��19200��38400\r\n");
			}
			break;
		}
		case 302:
		{//���ô洢��ַ
			u8 a = Calculate_CharToU16(p+4,3);//��ַ
			if((a> 0) && (a < 250))
			{
				Thbss01_Operation(302,&a,1);
				printf("���õ�ַΪ%u\r\n",a);
			}
			else
			{
				printf("�������\r\n");
				printf("��ַ�������÷�ΧΪ1~253\r\n");
			}
			break;
		}
		case 303:
		{//���ô洢������
			TwoBytes a;
			a.U16Data = Calculate_CharToU16(p+4,5);//��ַ
			if(a.U16Data == 4800 || a.U16Data == 9600 || a.U16Data == 19200 || a.U16Data == 38400)
			{
				Thbss01_Operation(303,a.U8Data,2);
				printf("���ò�����Ϊ%u\r\n",a.U16Data);
			}
			else
			{
				printf("�������\r\n");
				printf("ͨѶ�����ʿ������÷�ΧΪ4800��9600��19200��38400\r\n");
			}
			break;
		}
	}
}
//---
/*
	�������� 	void Thbss01_DebugRTU((u8 *p)
	������		�ú���ΪThbss01�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*/
void Thbss01_DebugRTU(u8 *p,u8 len)
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
}
//---
/*
	��������void Thbss01_Receive(u8 *p,u8 len)
	�������������Խ��յ�Thbss01��һ֡����
	˵��������ú���֮ǰ����Ҫ�Ƚ���CRCУ��
*/
void Thbss01_Receive(u8 *p,u8 len)
{
	switch(Thbss01Data.CollectMode)//�鿴���ĸ�������
	{
		case 0x01:
		{//��ȡ��������
			break;
		}
		case 0x02:
		{//��ȡ��������
			FourBytes four;
//			TwoBytes change2;
			four.U8Data[0] = p[4];
			four.U8Data[1] = p[3];
			four.U8Data[2] = p[6];
			four.U8Data[3] = p[5];
			 
			Thbss01Data.Hosting = four.S32Data;
			Tower_CalMomental();//��������
//			printf("��ǰ����Ϊ%dg\r\n",four.S32Data);
			break;
		}
		case 0x81:
		{
			printf("д���ֵ�ɹ�\r\n");
			break;
		}
		case 0x82:
		{
			printf("д����ֵ�ɹ�\r\n");
			break;
		}
		case 0x83:
		{
			printf("д���������ɹ�\r\n");
			break;
		}
		case 0x84:
		{
			printf("ȥƤ�ɹ�\r\n");
			break;
		}
		case 0x85:
		{
			printf("ȡ��ȥƤ�ɹ�\r\n");
			break;
		}
		case 0xA1:
		{
			break;
		}
		case 0xA2:
		{
			break;
		}
	}
	Thbss01Data.CollectMode = 0;
}
	
//---





































