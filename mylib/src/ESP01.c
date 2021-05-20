/*
	本文件为该项目的屏幕相关函数
	具体函数如下
*/

#include "ESP01.h"

ESP01_DataStructure ESP01Data;
/*
	函数名：void ESP01_Main(void)
	描述：	屏幕显示主函数
*/
void ESP01_Main(void)
{
	ESP01_Work(&ESP01Data);
}
//---
/*
	函数名：void ESP01_Work(ESP01_DataStructure *p)
	描述：该函数为ESP01工作函数
*/
void ESP01_Work(ESP01_DataStructure *p)
{
	switch(p->WorkMode)
	{
		case 1:
		{//延时等等模块启动
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
		{//发送模块测试指令以查看模块是否准备好
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
		//查看模块连接网络信息信息
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
					{//ESP8266 Station已连接AP，获得IP地址
						p->WorkMode = 51;
						printf("ESP8266 Station已连接AP，获得IP地址\r\n");
						break;
					}
					case '3':
					{//ESP8266 Station已建立TCP或UDP传输
						p->WorkMode = 61;
						printf("ESP8266 Station已建立TCP或UDP传输\r\n");
						break;
					}
					case '4':
					{//ESP8266 Station断开网络连接
						p->WorkMode = 51;
						printf("ESP8266 Station断开网络连接\r\n");
						break;
					}
					case '5':
					{//ESP8266 Station未连接AP
						p->WorkMode = 31;
						printf("ESP8266 Station未连接AP\r\n");
						break;
					}
				}
			}
			else if(p->Timer == 0)
			{
				p->ErrorCnt++;
				p->WorkMode = 21;
				printf("超时\r\n");
			}
			break;
		}
		//连接WiFi
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
				printf("收到回复：Timer=%u\r\n",p->Timer);
				p->WorkMode = 0;
			}
			else if(p->Timer == 0)
			{
				printf("超时\r\n");
				p->WorkMode = 0;
			}
			break;
		}
		case 51:
		{//已经连接WIFI，连接服务器
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
				printf("收到回复\r\n");
				p->WorkMode = 0;
			}
			else if(p->Timer == 0)
			{
				printf("超时\r\n");
				p->WorkMode = 0;
			}
			break;
		}
	}
}	
//---
/*
	函数名：void ESP01_Init(void)
	描述：	screen.c文件初始化函数
*/
void ESP01_Init(void)
{
	ESP01Data.WorkMode = 1;
	ESP01Data.Flag = 0;
	ESP01Data.Flag |= B0;//当前没有启动通讯
}
//---
/*
	函数名：void ESP01_Timer(void)
	描述：screen.c文件定时器函数，由系统1ms定时器调用
*/
void ESP01_Timer(void)
{
	if(ESP01Data.Timer != 0)ESP01Data.Timer--;
}
//---
/*
	函数名：ESP01_DataStructure* ESP01_GetData(void)
	描述：获取返回screen.c文件全局变量结构体
*/
ESP01_DataStructure* ESP01_GetESP01Data(void)
{
	return &ESP01Data;
}
//---
/*
	函数名：void ESP01_SendData(u8 *p,u8 len,ESP01_DataStructure *ESP)
描述：	向ESP01模块发送数据（可以命令或数字）
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
//-------------------------调试函数----------------------------
//---
/*
	函数名： void ESP01_DebugASCII(u8 *p,u8 len)
	描述：		该函数为ESP01文件的ASCII调试文件
*/
void ESP01_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
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
	函数名： 	void ESP01_DebugRTU((u8 *p)
	描述：		该函数为ESP01文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*/
void ESP01_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//查看是哪个功能码
	{
		case 0x01:
		{
			break;
		}
	}
}

//---
/*
	函数名：void ESP01_Receive(u8 *p,u8 len)
	描述：处理来讲ESP01模块接收到的回应
*/
void ESP01_Receive(u8 *p,u8 len)
{
	ESP01_DataStructure *ESP = &ESP01Data;
	if(ESP->Flag & B0)
	{
		if(memcmp((char*)p,"\r\n+IPD",6) == 0)
		{//接收到服务器的数据
//			printf("接收到服务器的数据\r\n");
		}
		else if (strcoll((char*)p,"CLOSED\r\n") == 0)
		{
			printf("网络被断开\r\n");
		}
		else 
		{
			printf("收到模块数据\r\n");
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





































