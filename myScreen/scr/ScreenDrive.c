#include "ScreenDrive.h"
#include "Screenlogic.h"  
	 
extern Screen_DataStructure ScreenData;
//---
/*
	��������void Screen_ShowPage(u16 page)
	������	��ʾָ��ҳ��5A A5 06 80 03 0005 CRCH CRCL
*/
void Screen_ShowPage(u16 page)
{
	u8 bf[10];
	u8 cnt=0;
	u8 i;
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	ScreenData.OldPage = ScreenData.Page;
	ScreenData.Page = page;
	bf[cnt++] = 0x80;
	bf[cnt++] = 0x03;
	bf[cnt++] = page>>8;
	bf[cnt++] = page;
	#ifdef SCREE_CRC
	crc = CRC16(bf,cnt);
	bf[cnt++] = crc>>8;
	bf[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,bf[i]);
	}
	//---------------
//	ScreenData.Key.OldOpt = 0;//���½�������¶Խ�
	//---------------
}
//---
/*
	��������void Screen_RefreshDataMulti(u16 address,u8 *p,u8 len)
	����������Ļ�����洢�����Ͷ������
	���룺address ��ַ
				p���ݵ�ַ
				len ���ݳ��ȣ����ȴ�СΪС�ڵ���195���ַ�
*/
void Screen_RefreshDataMulti(u16 address,u8 *p,u8 len)
{
	u8 buffer[80];
	u8 cnt=0;
	u8 i;
	StrinGalignment(p,len);
	len=strlen((const char* )p);
	
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x82;
	buffer[cnt++] = address>>8;
	buffer[cnt++] = address;
	for(i=0;i<len;i++)
	{
		buffer[cnt++] = *p;
		p++;
	}
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
	Screen_ResetDataBuffer();
}
//---
/*
	��������void Screen_RefreshVarData(u16 address,u16 data)
	����������Ļ�����洢������λͼѡ��
	���룺address ��ַ
				data����
			
*/
void Screen_RefreshVarData(u16 address,u16 data)
{
	u8 buffer[20];
	u8 cnt=0;
	u8 i;

	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x82;
	buffer[cnt++] = address>>8;
	buffer[cnt++] = address;
	buffer[cnt++] = data>>8;
	buffer[cnt++] = data;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
}
//---
/*
	��������void Screen_Reset(void)
	��������λ��Ļ5A A5 04 80 EE 5A A5
*/
void Screen_Reset(void)
{
	u8 buffer[10];
	u8 cnt=0;
	u8 i;
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x80;
	buffer[cnt++] = 0xEE;
	buffer[cnt++] = 0x5A;
	buffer[cnt++] = 0xA5;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	#ifdef SCREE_CRC
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x06);
	#else
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,4);
	#endif
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
}
//---
/*
	��������void Screen_ResetDataBuffer(void)
	��������λScreenData.Bfffer����
*/
void Screen_ResetDataBuffer(void)
{
	for(ScreenData.Cnt=0;ScreenData.Cnt<SCREEN_BFLEN;ScreenData.Cnt++)
		ScreenData.Buffer[ScreenData.Cnt]=0;
	ScreenData.Cnt = 0;
	ScreenData.Con = 0;
}
//---
/*
	��������void Screen_ResetKeyDataBuffer(void)
	��������λScreenData.Bfffer����
*/
void Screen_ResetKeyDataBuffer(void)
{
	u8 i;
	for(i=0; i<30;i++) ScreenData.Key.Buffer[i] = 0;
}
//---
/*
	��������void Screen_WriteRegister(u8 address,u8 len,u8 *p)
	������	����Ļ�Ĵ���д����
	���룺	address��д��Ĵ�����ʼ��ַ
					len��д�����ݳ��ȣ�����С�ڵ���195
					*p����д������ָ���׵�ַ
*/
void Screen_WriteRegister(u8 address,u8 len,u8 *p)
{
	u8 buffer[60]; //�޸���20200310 �޸�200
	u8 cnt=0;
	u8 i;
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x80;
	buffer[cnt++] = address;
	for(i=0;i<len;i++)
	{
		buffer[cnt++] = p[i];
	}
	buffer[cnt++] = len;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
}
//---
/*
	��������void Screen_WriteRegisterOneData(u8 address,u8 data)
	������	����Ļ�Ĵ���дһ���ֽ�����
	���룺	address��д��Ĵ�����ʼ��ַ
					data����д�������
*/
void Screen_WriteRegisterOneData(u8 address,u8 data)
{
	u8 buffer[20];
	u8 cnt=0;
	u8 i;
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x80;
	buffer[cnt++] = address;
	buffer[cnt++] = data;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
	
	if(address==0x4f)   //�ر��������˵�ʱ
	{
		Screen_GetData()->Scan_Stimer=100;  //100 ms��ʼ����
 		Screen_GetData()->Scan_status=Screen_GetData()->Scan_status|0x02;  //�������
		Screen_GetData()->Scan_status=Screen_GetData()->Scan_status|0x01;  
	}
}
//---
/*
	��������void Screen_ReadRegister(u8 address,u8 len)
������	����Ļ���Ͷ�ȡ�Ĵ�����������
	���룺	address�����Ĵ�����ַ
					len�������ݳ���
*/
void Screen_ReadRegister(u8 address,u8 len)
{
	u8 buffer[20];
	u8 cnt=0;
	u8 i;
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x81;
	buffer[cnt++] = address;
	buffer[cnt++] = len;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
}
//---
/*
	��������void Screen_PastePicture(u16 address,u16 pid,u32 ctlc,u32 clrc,u32 ptlc)
	������	�ú���Ϊ��pidҳ��ͼƬ����Ϊ���Ͻ�ctlc�����½�clrc�������Ƶ���ǰҳ������Ͻ�Ϊptlcλ��
					��Ӧ������Ļ�ġ�����ͼ����ʾ���ؼ��ġ�ͼƬ������С����"ָ��
	���룺	address������ͼ����ʾ����������ַ
					pid��������ͼƬҳ��ID(Picture ID)
					ctlc��������ͼƬҳ�����Ͻ�XY���꣬����ΪX���꣬����ΪY����(copy top left corner)
					clrc��������ͼƬҳ�����½�XY���꣬����ΪX���꣬����ΪY����(copy lower right corner)
					ptlc�������ǰҳ�����Ͻ�XY���꣬����ΪX���꣬����ΪY����(paste top left corner)
*/
void Screen_PastePicture(u16 address,u16 pid,u32 ctlc,u32 clrc,u32 ptlc)
{
	u8 buffer[30];
	u8 cnt=0;
	u8 i;
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x82;
	buffer[cnt++] = address>>8;
	buffer[cnt++] = address;
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x06;
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x01;
	buffer[cnt++] = pid>>8;
	buffer[cnt++] = pid;
	buffer[cnt++] = ctlc>>24;
	buffer[cnt++] = ctlc>>16;
	buffer[cnt++] = ctlc>>8;
	buffer[cnt++] = ctlc;
	buffer[cnt++] = clrc>>24;
	buffer[cnt++] = clrc>>16;
	buffer[cnt++] = clrc>>8;
	buffer[cnt++] = clrc;
	buffer[cnt++] = ptlc>>24;
	buffer[cnt++] = ptlc>>16;
	buffer[cnt++] = ptlc>>8;
	buffer[cnt++] = ptlc;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
}
//---
/*
	��������void Screen_RectangleFilling(u16 address,u32 ctlc,u32 clrc,u16 colour)
	������	�ú���Ϊ��address��ַ������ͼ�α������ؼ����о������
	���룺	address������ͼ����ʾ����������ַ
					ctlc�����λ�õ����Ͻ�XY���꣬����ΪX���꣬����ΪY����(copy top left corner)
					clrc�����λ�õ����½�XY���꣬����ΪX���꣬����ΪY����(copy lower right corner)
					colour��������ɫ
*/
void Screen_RectangleFilling(u16 address,u32 ctlc,u32 clrc,u16 colour)
{
	u8 buffer[30];
	u8 cnt=0;
	u8 i;
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x82;
	buffer[cnt++] = address>>8;
	buffer[cnt++] = address;
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x04;//�������������
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x01;//��Ŀ����
	buffer[cnt++] = ctlc>>24;
	buffer[cnt++] = ctlc>>16;
	buffer[cnt++] = ctlc>>8;
	buffer[cnt++] = ctlc;
	buffer[cnt++] = clrc>>24;
	buffer[cnt++] = clrc>>16;
	buffer[cnt++] = clrc>>8;
	buffer[cnt++] = clrc;
	buffer[cnt++] = colour>>8;
	buffer[cnt++] = colour;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
}
//---
/*
	��������void Screen_SetClock(u16 year,u8 month,u8 day,u8 hour,u8 minute,u8 sec)
	������	�ú���Ϊ����ʱ��
	���룺	������ʱ����
					�꣺2000~2099
					�£�1~12
					�գ�1~31
					ʱ��0~23
					�֣�0~59
					�룺0~59
	˵����	����ת������Ļ��ʱ���ʽ
	������ʽ��	5A A5 0A 80 1F 5A 16 01 25 00 17 08 20��2016��1��25��17:08:20��
	˵����	
	���룺	struct tm *sTime ������ʱ��ָ��
*/
void Screen_SetClock(struct tm *sTime)//u16 year,u8 month,u8 day,u8 hour,u8 minute,u8 sec)
{
	u8 buffer[20];
	u8 cnt=0;
	u8 i;
	u8 bf = sTime->tm_year - SCREEN_YearScreen;	//(year - SCREEN_YearBase) - SCREEN_YearScreen;//ת��Ϊ������Ļ��
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x80;
	buffer[cnt++] = 0x1F;
	buffer[cnt++] = 0x5A;
	buffer[cnt++] = (bf/10)*0x10+bf%10;
	buffer[cnt++] = ((sTime->tm_mon + 1) / 10) * 0x10 + (sTime->tm_mon + 1) % 10;
	buffer[cnt++] = (sTime->tm_mday / 10) * 0x10 + sTime->tm_mday % 10;
	buffer[cnt++] = 0;
	buffer[cnt++] = (sTime->tm_hour / 10) * 0x10 + sTime->tm_hour % 10;
	buffer[cnt++] = (sTime->tm_min / 10) * 0x10 + sTime->tm_min % 10;
	buffer[cnt++] = (sTime->tm_sec /10) * 0x10 + sTime->tm_sec % 10;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
}


