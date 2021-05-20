/*
	���ļ�Ϊ����Ŀ�ķ��ٴ�������غ����ļ�
	���庯������
*/

#include "wind.h"


Wind_DataStructure WindData;//���ٴ������ṹ�����


//---
/*
	��������void YCFS260_Communicate(void)
	������YCFS260������
*/
void YCFS260_Communicate(void)
{
	switch(WindData.RunMode)
	{
		case 1:
		{//���Ͷ�ȡ��������
			YCFS260_CollectWindSpeed();
			WindData.Timer = 200;
			WindData.RunMode = 2;
			break;
		}
		case 2:
		{
			if(WindData.ComMode == 0)
			{//���յ�����
				WindData.RunMode = 1;
			}
			else if(WindData.Timer == 0)
			{//��ʱ���գ����ٴ���������
				WindData.RunMode = 1;
				WindData.Speed = 0;
			}
			break;
		}
	}
}
//---
/*
	��������void YCFS260_Timer(void)
	������	�ú���Ϊcollect.c�ļ���ʱ��������1ms����ϵͳ1ms��ʱ������
*/
void YCFS260_Timer(void)
{
	if(WindData.Timer != 0)WindData.Timer--;
}
//---
/*
	��������void Wind_Init(void)
	�������ú���Ϊwind.c�ļ���ʼ������
*/
void Wind_Init(void)
{
	Wind_DataInit();
}
//---
/*
	��������void Wind_DataInit(void)
	�������ú���ΪWindData��ʼ������
*/
void Wind_DataInit(void)
{
	TwoBytes two;
	YCFS260_Operation(1,two.U8Data,1);//����ַ
	WindData.Address = two.U8Data[0];
	YCFS260_Operation(2,two.U8Data,2);//��������
	WindData.BaudRate = two.U16Data;
	WindData.RunMode = 0;
	WindData.Speed = 0;
}
//---
/*
	��������Wind_DataStructure* Wind_GetWindData(void)
	������	�ú���Ϊ��ȡWindData������ַ
*/
Wind_DataStructure* Wind_GetWindData(void)
{
	return &WindData;
}
//---
/*
	��������void YCFS260_CollectWindSpeed(void)
	������	�ú���Ϊ���Ͳɼ����ٴ���������
	02 03 00 00 00 01 84 39
*/
void YCFS260_CollectWindSpeed(void)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = WindData.Address;
	buffer[cnt++] = 0x03;//��ȡ����
	buffer[cnt++] = 0x00;//��ʼ��ַ0x0000��2�ֽ�
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x00;//���ݳ���1���֣�2�ֽ�
	buffer[cnt++] = 0x01;
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	YCFS260_UsartModeSet();
	for(crc=0;crc<cnt;crc++)
	{
		YCFS260_SendBufferData(YCFS260_UsartPort,YCFS260_UsartBuffer,buffer[crc]);//��������
	}
	WindData.ComMode = YCFS260_ComModeReadWind;
}
//---
/*
	��������void Wind_ReadCommAddress(void)
	������	��ȡ����������ͨѶ��ַ
					FD FD FD 00 00 E9 88
*/
void Wind_ReadCommAddress(void)
{
		u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = 0xFD;
	buffer[cnt++] = 0xFD;//��ȡ����
	buffer[cnt++] = 0xFD;//��ʼ��ַ0x0000��2�ֽ�
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x00;//���ݳ���1���֣�2�ֽ�
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	YCFS260_UsartModeSet();
	for(crc=0;crc<cnt;crc++)
	{
		YCFS260_SendBufferData(YCFS260_UsartPort,YCFS260_UsartBuffer,buffer[crc]);//��������
	}
	WindData.RunMode = YCFS260_ComModeReadAddress;
}
//---
/*
	��������void Wind_SetCommAddress(void)
	������	���÷���������ͨѶ��ַ
					FD FD FD 00 02 68 49
*/
void Wind_SetCommAddress(u8 address)
{
		u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = 0xFD;
	buffer[cnt++] = 0xFD;//��ȡ����
	buffer[cnt++] = 0xFD;//��ʼ��ַ0x0000��2�ֽ�
	buffer[cnt++] = 0x00;
	buffer[cnt++] = address;//��ַ
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	YCFS260_UsartModeSet();
	for(crc=0;crc<cnt;crc++)
	{
		YCFS260_SendBufferData(YCFS260_UsartPort,YCFS260_UsartBuffer,buffer[crc]);//��������
	}
	WindData.RunMode = YCFS260_ComModeSetAddress;
}
//---
/*
	��������void Wind_SetCommBaudRate(u8 baudrate)
	������	���÷���������������
					FD FD FD 03 00 E9 78����4��λ���������ݣ���5��λ��ַ���ݣ�
*/
void Wind_SetCommBaudRate(u8 baudrate)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = 0xFD;
	buffer[cnt++] = 0xFD;//��ȡ����
	buffer[cnt++] = 0xFD;//��ʼ��ַ0x0000��2�ֽ�
	buffer[cnt++] = baudrate;//������
	buffer[cnt++] = 0x00;//calibration
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	YCFS260_UsartModeSet();
	for(crc=0;crc<cnt;crc++)
	{
		YCFS260_SendBufferData(YCFS260_UsartPort,YCFS260_UsartBuffer,buffer[crc]);//��������
	}
	WindData.RunMode = YCFS260_ComModeSetBuadRate;
}
//---
/*
	void YCFS260_UsartModeSet(void)
	������ΪYCFS260���ô���ͨѶģʽ
*/
void YCFS260_UsartModeSet(void)
{
	u32 BaudRate=(u32)WindData.BaudRate;
	YCFS260_UartModeSet(YCFS260_UsartPort,BaudRate,'n','1');
}
//---
/*
	��������void YCFS260_Operation(u16 mode,u8 *p,u8 len)
	�������ú���ΪYCFS260.c�����ݲ���
*/
void YCFS260_Operation(u16 mode,u8 *p,u8 len)
{
	switch(mode)
	{
		case 1:
		{//������ͨѶ��ַ
			YCFS260_ReadStoreData(YCFS260_AdrStoreTXDZ,p,len);
			break;
		}
		case 2:
		{//������ͨѶ������
			YCFS260_ReadStoreData(YCFS260_AdrStoreTXBTL,p,len);
			break;
		}
		case 101:
		{//�޸�ͨѶ��ַ
			YCFS260_StoreData(YCFS260_AdrStoreTXDZ,p,len);
			break;
		}
		case 102:
		{//�޸�ͨѶ������
			YCFS260_StoreData(YCFS260_AdrStoreTXBTL,p,len);
			break;
		}
		case 201:
		{//���Ͷ�ȡ��������
				
			break;
		}
	}
}
//---
//-------------------------���ݶ�ȡ���洢----------------------------
//---
/*
	��������void YCFS260_ReadStoreData(u8 address,u8 *p,u8 len)
	������	��ȡ����
*/
void YCFS260_ReadStoreData(u8 address,u8 *p,u8 len)
{
	W25Xxx_ReadData(p,YCFS260_AdrStoreBase+address,len);
}
//---
/*
	��������void YCFS260_StoreData(u8 address,u8 *p,u8 len)
	�������ú���Ϊ�洢YCFS260���ò�������
	˵������ȡ����
				��������
				�޸�����
				��������
*/
void YCFS260_StoreData(u8 address,u8 *p,u8 len)
{
	u8 buffer[YCFS260_AdrStoreEND],cnt;
	W25Xxx_ReadData(buffer,YCFS260_AdrStoreBase,YCFS260_AdrStoreEND);//���������ò���
	W25Xxx_EraseSector(YCFS260_AdrStoreBase);//��������
	for(cnt=0;cnt<len;cnt++)
	{
		buffer[address++] = *p;
		p++;
	}
	W25Xxx_PageWrite(buffer,YCFS260_AdrStoreBase,YCFS260_AdrStoreEND);//��������
}
//---
//-------------------------------------���Ժ���----------------------------------------
//---
/*
	�������� void Wind_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪWind.c�ļ���ASCII�����ļ�
	:000|58|XXX������������
*/
void Wind_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1://
		{//:000|58|001
			YCFS260_SetComNoneVerify();//��У��
			YCFS260_CollectWindSpeed();			//��������
			printf("���Ͳɼ���������!\r\n");
			break;
		}
		case 2:
		{//:000|58|002
			YCFS260_SetComNoneVerify();//��У��
			Wind_ReadCommAddress();//��ȡ��ַ
			break;
		}
		case 3:
		{//:000|58|003|002
			u8 buffer = Calculate_CharToU16(p+4,3);//��������
			YCFS260_SetComNoneVerify();//��У��
			Wind_SetCommAddress(buffer);//���õ�ַ
			break;
		}
		case 4:
		{//:000|58|004|3	(1Ϊ2400,2Ϊ4800,3Ϊ9600)
			u8 buffer = Calculate_CharToU16(p+4,1);//��������
			YCFS260_SetComNoneVerify();//��У��
			Wind_SetCommAddress(buffer);//���õ�ַ
			break;
		}
		//��ȡ
		case 101:
		{//��ͨѶ��ַ
			u8 bf;
			YCFS260_Operation(1,&bf,1);
			printf("���ٴ�����ͨѶ��ַΪ��0x%X\r\n",bf);
			break;
		}
		case 102:
		{//��ͨѶ������
			TwoBytes two;
			YCFS260_Operation(2,two.U8Data,2);
			printf("���ٴ�����ͨѶ������Ϊ��%u\r\n",two.U16Data);
			break;
		}
		//�޸�
		case 201:
		{//�޸�ͨѶ��ַ
			u8 bf = Calculate_CharToU16(p+4,3);//��������
			YCFS260_Operation(101,&bf,1);
			printf("���÷��ٴ�����ͨѶ��ַΪ��0x%X\r\n",bf);
			break;
		}
		case 202:
		{//�޸�ͨѶ������
			TwoBytes two;
			two.U16Data = Calculate_CharToU16(p+4,5);//��������
			YCFS260_Operation(102,two.U8Data,2);
			printf("���÷��ٴ�����ͨѶ������Ϊ��%u\r\n",two.U16Data);
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
	�������� void YCFS260_DebugRTU((u8 *p)
	������	�ú���Ϊwind.c�ļ���RTU�����ļ�
	˵����	���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*/
void YCFS260_DebugRTU(u8 *p,u8 len)
{
	switch(WindData.ComMode)
	{
		case YCFS260_ComModeReadWind:
		{//�����ڶ�ȡ����ģʽ
			WindData.Speed = *(p+3)<<8 | *(p+4);//�������
			Server_GetTowerCraneData()->WindSpeed = WindData.Speed;
//			printf("��ǰ����Ϊ%f\r\n",((float)Server_GetTowerCraneData()->WindSpeed/10));
			break;
		}
		case YCFS260_ComModeReadAddress:
		{//�����ڶ�ȡ��ַģʽ
			printf("��ǰ���ٴ�������ַΪ0x%x\r\n",*(p+4));
			break;
		}
		case YCFS260_ComModeSetAddress:
		{//
			printf("��ǰ���õĵ�ַΪ0x%x\r\n",*(p+4));
			break;
		}
		case YCFS260_ComModeSetBuadRate:
		{
			printf("��ǰ���õĲ�����Ϊ%u\r\n",*(p+3));
			break;
		}
	}
	WindData.ComMode = YCFS260_ComModeNone;
}
//---












