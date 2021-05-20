/*
	���ļ�Ϊ����Ŀ����Ļ��غ���
	���庯������
*/

#include "ESP01.h"

ESP01_DataStructure ESP01Data;
/*
	��������void ESP01_Main(void)
	������	��Ļ��ʾ������
*/
void ESP01_Main(void)
{
	ESP01_Work(&ESP01Data);
}
//---
/*
	��������void ESP01_Work(ESP01_DataStructure *p)
	�������ú���ΪESP01��������
*/
void ESP01_Work(ESP01_DataStructure *p)
{
	switch(p->WorkMode)
	{
		case 1:
		{//��ʱ�ȵ�ģ������
			p->Timer = 1000;
			p->WorkMode = 2;
			break;
		}
		case 2:
		{
			if(p->Timer == 0)
				p->WorkMode = 11;
			break;
		}
		case 11:
		{//����ģ�����ָ���Բ鿴ģ���Ƿ�׼����
			ESP01_SendData("AT\r\n",0,p);
			p->Timer = 2000;
			p->WorkMode = 12;
			break;
		}
		case 12:
		{
			if(p->Flag & B0) p->WorkMode = 21;
			else if(p->Timer == 0) p->WorkMode = 11;
			break;
		}
		//�鿴ģ������������Ϣ��Ϣ
		case 21:
		{
			ESP01_SendData("AT+CIPSTATUS\r\n",0,p);
			p->Timer = 2000;
			p->WorkMode = 22;
			break;
		}
		case 22:
		{
			if(p->Flag & B0)
			{
				switch(p->RxBf[22])
				{
					case '2':
					{//ESP8266 Station������AP�����IP��ַ
						p->WorkMode = 51;
						printf("ESP8266 Station������AP�����IP��ַ\r\n");
						break;
					}
					case '3':
					{//ESP8266 Station�ѽ���TCP��UDP����
						p->WorkMode = 61;
						printf("ESP8266 Station�ѽ���TCP��UDP����\r\n");
						break;
					}
					case '4':
					{//ESP8266 Station�Ͽ���������
						p->WorkMode = 51;
						printf("ESP8266 Station�Ͽ���������\r\n");
						break;
					}
					case '5':
					{//ESP8266 Stationδ����AP
						p->WorkMode = 31;
						printf("ESP8266 Stationδ����AP\r\n");
						break;
					}
				}
			}
			else if(p->Timer == 0)
			{
				p->ErrorCnt++;
				p->WorkMode = 21;
				printf("��ʱ\r\n");
			}
			break;
		}
		//����WiFi
		case 31:
		{
			ESP01_SendData("AT+CWJAP_DEF=\"MyWiFi\",\"abc123ABC\"\r\n",0,p);
			p->Timer = 6000;
			p->WorkMode = 32;
			break;
		}
		case 32:
		{
			if(p->Flag & B0)
			{
				printf("�յ��ظ���Timer=%u\r\n",p->Timer);
				p->WorkMode = 0;
			}
			else if(p->Timer == 0)
			{
				printf("��ʱ\r\n");
				p->WorkMode = 0;
			}
			break;
		}
		case 51:
		{//�Ѿ�����WIFI�����ӷ�����
			//AT+CIPSTART="TCP","192.168.3.116",8080
			ESP01_SendData("AT+CIPSTART=\"TCP\",\"192.168.199.117\",12345\r\n",0,p);
			p->Timer = 6000;
			p->WorkMode = 52;
			break;
		}
		case 52:
		{
			if(p->Flag & B0)
			{
				printf("�յ��ظ�\r\n");
				p->WorkMode = 0;
			}
			else if(p->Timer == 0)
			{
				printf("��ʱ\r\n");
				p->WorkMode = 0;
			}
			break;
		}
	}
}	
//---
/*
	��������void ESP01_Init(void)
	������	screen.c�ļ���ʼ������
*/
void ESP01_Init(void)
{
	ESP01Data.WorkMode = 1;
	ESP01Data.Flag = 0;
	ESP01Data.Flag |= B0;//��ǰû������ͨѶ
}
//---
/*
	��������void ESP01_Timer(void)
	������screen.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void ESP01_Timer(void)
{
	if(ESP01Data.Timer != 0)ESP01Data.Timer--;
}
//---
/*
	��������ESP01_DataStructure* ESP01_GetData(void)
	��������ȡ����screen.c�ļ�ȫ�ֱ����ṹ��
*/
ESP01_DataStructure* ESP01_GetESP01Data(void)
{
	return &ESP01Data;
}
//---
/*
	��������void ESP01_SendData(u8 *p,u8 len,ESP01_DataStructure *ESP)
������	��ESP01ģ�鷢�����ݣ�������������֣�
*/
//---
void ESP01_SendData(u8 *p,u8 len,ESP01_DataStructure *ESP)
{
	if(len != 0)
	{
	}
	else
	{
		while((*p>0x00))
		{
			ESP01_SendBufferData(ESP01_UsartPort,ESP01_UsartBuffer(),*p);
			p++;
		}
	}
	ESP->Flag &= ~b0;
}
//---
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void ESP01_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪESP01�ļ���ASCII�����ļ�
*/
void ESP01_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:
		{
			
			break;
		}
	}
}
//---
/*
	�������� 	void ESP01_DebugRTU((u8 *p)
	������		�ú���ΪESP01�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*/
void ESP01_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//�鿴���ĸ�������
	{
		case 0x01:
		{
			break;
		}
	}
}

//---
/*
	��������void ESP01_Receive(u8 *p,u8 len)
	��������������ESP01ģ����յ��Ļ�Ӧ
*/
void ESP01_Receive(u8 *p,u8 len)
{
	ESP01_DataStructure *ESP = &ESP01Data;
	if(ESP->Flag & B0)
	{
		if(memcmp((char*)p,"\r\n+IPD",6) == 0)
		{//���յ�������������
//			printf("���յ�������������\r\n");
		}
		else if (strcoll((char*)p,"CLOSED\r\n") == 0)
		{
			printf("���类�Ͽ�\r\n");
		}
		else 
		{
			printf("�յ�ģ������\r\n");
		}
	}
	else 
	{
		ESP->Flag |= B0;
		ESP->RxLen = len;
		ESP->RxBf = p;
		ESP01_Work(ESP);
	}
}
//---





































