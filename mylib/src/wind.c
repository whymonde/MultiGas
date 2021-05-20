/*
	本文件为该项目的风速传感器相关函数文件
	具体函数如下
*/

#include "wind.h"


Wind_DataStructure WindData;//风速传感器结构体变量


//---
/*
	函数名：void YCFS260_Communicate(void)
	描述：YCFS260主函数
*/
void YCFS260_Communicate(void)
{
	switch(WindData.RunMode)
	{
		case 1:
		{//发送读取风速命令
			YCFS260_CollectWindSpeed();
			WindData.Timer = 200;
			WindData.RunMode = 2;
			break;
		}
		case 2:
		{
			if(WindData.ComMode == 0)
			{//接收到风速
				WindData.RunMode = 1;
			}
			else if(WindData.Timer == 0)
			{//超时接收，风速传感器故障
				WindData.RunMode = 1;
				WindData.Speed = 0;
			}
			break;
		}
	}
}
//---
/*
	函数名：void YCFS260_Timer(void)
	描述：	该函数为collect.c文件定时器函数，1ms，由系统1ms定时器调用
*/
void YCFS260_Timer(void)
{
	if(WindData.Timer != 0)WindData.Timer--;
}
//---
/*
	函数名：void Wind_Init(void)
	描述：该函数为wind.c文件初始化函数
*/
void Wind_Init(void)
{
	Wind_DataInit();
}
//---
/*
	函数名：void Wind_DataInit(void)
	描述：该函数为WindData初始化函数
*/
void Wind_DataInit(void)
{
	TwoBytes two;
	YCFS260_Operation(1,two.U8Data,1);//读地址
	WindData.Address = two.U8Data[0];
	YCFS260_Operation(2,two.U8Data,2);//读波特率
	WindData.BaudRate = two.U16Data;
	WindData.RunMode = 0;
	WindData.Speed = 0;
}
//---
/*
	函数名：Wind_DataStructure* Wind_GetWindData(void)
	描述：	该函数为获取WindData变量地址
*/
Wind_DataStructure* Wind_GetWindData(void)
{
	return &WindData;
}
//---
/*
	函数名：void YCFS260_CollectWindSpeed(void)
	描述：	该函数为发送采集风速传感器命令
	02 03 00 00 00 01 84 39
*/
void YCFS260_CollectWindSpeed(void)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = WindData.Address;
	buffer[cnt++] = 0x03;//读取命令
	buffer[cnt++] = 0x00;//起始地址0x0000，2字节
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x00;//数据长度1个字，2字节
	buffer[cnt++] = 0x01;
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	YCFS260_UsartModeSet();
	for(crc=0;crc<cnt;crc++)
	{
		YCFS260_SendBufferData(YCFS260_UsartPort,YCFS260_UsartBuffer,buffer[crc]);//发送数据
	}
	WindData.ComMode = YCFS260_ComModeReadWind;
}
//---
/*
	函数名：void Wind_ReadCommAddress(void)
	描述：	读取风力传感器通讯地址
					FD FD FD 00 00 E9 88
*/
void Wind_ReadCommAddress(void)
{
		u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = 0xFD;
	buffer[cnt++] = 0xFD;//读取命令
	buffer[cnt++] = 0xFD;//起始地址0x0000，2字节
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x00;//数据长度1个字，2字节
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	YCFS260_UsartModeSet();
	for(crc=0;crc<cnt;crc++)
	{
		YCFS260_SendBufferData(YCFS260_UsartPort,YCFS260_UsartBuffer,buffer[crc]);//发送数据
	}
	WindData.RunMode = YCFS260_ComModeReadAddress;
}
//---
/*
	函数名：void Wind_SetCommAddress(void)
	描述：	设置风力传感器通讯地址
					FD FD FD 00 02 68 49
*/
void Wind_SetCommAddress(u8 address)
{
		u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = 0xFD;
	buffer[cnt++] = 0xFD;//读取命令
	buffer[cnt++] = 0xFD;//起始地址0x0000，2字节
	buffer[cnt++] = 0x00;
	buffer[cnt++] = address;//地址
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	YCFS260_UsartModeSet();
	for(crc=0;crc<cnt;crc++)
	{
		YCFS260_SendBufferData(YCFS260_UsartPort,YCFS260_UsartBuffer,buffer[crc]);//发送数据
	}
	WindData.RunMode = YCFS260_ComModeSetAddress;
}
//---
/*
	函数名：void Wind_SetCommBaudRate(u8 baudrate)
	描述：	设置风力传感器波特率
					FD FD FD 03 00 E9 78（低4个位波特率数据，第5个位地址数据）
*/
void Wind_SetCommBaudRate(u8 baudrate)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = 0xFD;
	buffer[cnt++] = 0xFD;//读取命令
	buffer[cnt++] = 0xFD;//起始地址0x0000，2字节
	buffer[cnt++] = baudrate;//波特率
	buffer[cnt++] = 0x00;//calibration
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	YCFS260_UsartModeSet();
	for(crc=0;crc<cnt;crc++)
	{
		YCFS260_SendBufferData(YCFS260_UsartPort,YCFS260_UsartBuffer,buffer[crc]);//发送数据
	}
	WindData.RunMode = YCFS260_ComModeSetBuadRate;
}
//---
/*
	void YCFS260_UsartModeSet(void)
	描述：为YCFS260配置串口通讯模式
*/
void YCFS260_UsartModeSet(void)
{
	u32 BaudRate=(u32)WindData.BaudRate;
	YCFS260_UartModeSet(YCFS260_UsartPort,BaudRate,'n','1');
}
//---
/*
	函数名：void YCFS260_Operation(u16 mode,u8 *p,u8 len)
	描述：该函数为YCFS260.c的数据操作
*/
void YCFS260_Operation(u16 mode,u8 *p,u8 len)
{
	switch(mode)
	{
		case 1:
		{//读风速通讯地址
			YCFS260_ReadStoreData(YCFS260_AdrStoreTXDZ,p,len);
			break;
		}
		case 2:
		{//读风速通讯波特率
			YCFS260_ReadStoreData(YCFS260_AdrStoreTXBTL,p,len);
			break;
		}
		case 101:
		{//修改通讯地址
			YCFS260_StoreData(YCFS260_AdrStoreTXDZ,p,len);
			break;
		}
		case 102:
		{//修改通讯波特率
			YCFS260_StoreData(YCFS260_AdrStoreTXBTL,p,len);
			break;
		}
		case 201:
		{//发送读取风速命令
				
			break;
		}
	}
}
//---
//-------------------------数据读取、存储----------------------------
//---
/*
	函数名：void YCFS260_ReadStoreData(u8 address,u8 *p,u8 len)
	描述：	读取数据
*/
void YCFS260_ReadStoreData(u8 address,u8 *p,u8 len)
{
	W25Xxx_ReadData(p,YCFS260_AdrStoreBase+address,len);
}
//---
/*
	函数名：void YCFS260_StoreData(u8 address,u8 *p,u8 len)
	描述：该函数为存储YCFS260配置参数函数
	说明：读取数据
				擦除数据
				修改数据
				保存数据
*/
void YCFS260_StoreData(u8 address,u8 *p,u8 len)
{
	u8 buffer[YCFS260_AdrStoreEND],cnt;
	W25Xxx_ReadData(buffer,YCFS260_AdrStoreBase,YCFS260_AdrStoreEND);//读所有配置参数
	W25Xxx_EraseSector(YCFS260_AdrStoreBase);//擦除数据
	for(cnt=0;cnt<len;cnt++)
	{
		buffer[address++] = *p;
		p++;
	}
	W25Xxx_PageWrite(buffer,YCFS260_AdrStoreBase,YCFS260_AdrStoreEND);//保存数据
}
//---
//-------------------------------------调试函数----------------------------------------
//---
/*
	函数名： void Wind_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Wind.c文件的ASCII调试文件
	:000|58|XXX······
*/
void Wind_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1://
		{//:000|58|001
			YCFS260_SetComNoneVerify();//无校验
			YCFS260_CollectWindSpeed();			//发送命令
			printf("发送采集风速命令!\r\n");
			break;
		}
		case 2:
		{//:000|58|002
			YCFS260_SetComNoneVerify();//无校验
			Wind_ReadCommAddress();//读取地址
			break;
		}
		case 3:
		{//:000|58|003|002
			u8 buffer = Calculate_CharToU16(p+4,3);//计算命令
			YCFS260_SetComNoneVerify();//无校验
			Wind_SetCommAddress(buffer);//设置地址
			break;
		}
		case 4:
		{//:000|58|004|3	(1为2400,2为4800,3为9600)
			u8 buffer = Calculate_CharToU16(p+4,1);//计算命令
			YCFS260_SetComNoneVerify();//无校验
			Wind_SetCommAddress(buffer);//设置地址
			break;
		}
		//读取
		case 101:
		{//读通讯地址
			u8 bf;
			YCFS260_Operation(1,&bf,1);
			printf("风速传感器通讯地址为：0x%X\r\n",bf);
			break;
		}
		case 102:
		{//读通讯波特率
			TwoBytes two;
			YCFS260_Operation(2,two.U8Data,2);
			printf("风速传感器通讯波特率为：%u\r\n",two.U16Data);
			break;
		}
		//修改
		case 201:
		{//修改通讯地址
			u8 bf = Calculate_CharToU16(p+4,3);//计算命令
			YCFS260_Operation(101,&bf,1);
			printf("设置风速传感器通讯地址为：0x%X\r\n",bf);
			break;
		}
		case 202:
		{//修改通讯波特率
			TwoBytes two;
			two.U16Data = Calculate_CharToU16(p+4,5);//计算命令
			YCFS260_Operation(102,two.U8Data,2);
			printf("设置风速传感器通讯波特率为：%u\r\n",two.U16Data);
			break;
		}
		default:
		{
			printf("Wind.c接收到无效调试指令%u!\r\n",cmd);
			break;
		}
	}
}
//---
/*
	函数名： void YCFS260_DebugRTU((u8 *p)
	描述：	该函数为wind.c文件的RTU调试文件
	说明：	调用该函数之前需要先进行CRC校验
*/
void YCFS260_DebugRTU(u8 *p,u8 len)
{
	switch(WindData.ComMode)
	{
		case YCFS260_ComModeReadWind:
		{//运行在读取风速模式
			WindData.Speed = *(p+3)<<8 | *(p+4);//计算风速
			Server_GetTowerCraneData()->WindSpeed = WindData.Speed;
//			printf("当前风速为%f\r\n",((float)Server_GetTowerCraneData()->WindSpeed/10));
			break;
		}
		case YCFS260_ComModeReadAddress:
		{//运行在读取地址模式
			printf("当前风速传感器地址为0x%x\r\n",*(p+4));
			break;
		}
		case YCFS260_ComModeSetAddress:
		{//
			printf("当前设置的地址为0x%x\r\n",*(p+4));
			break;
		}
		case YCFS260_ComModeSetBuadRate:
		{
			printf("当前设置的波特率为%u\r\n",*(p+3));
			break;
		}
	}
	WindData.ComMode = YCFS260_ComModeNone;
}
//---












