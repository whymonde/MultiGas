/*
	本文件为该项目的吊重传感器相关函数文件
	具体函数如下
*/

#include "hoisting.h"


Hoisting_DataStructure HoistingData;//Hoisting结构体变量
/*
	函数名：void Hoisting_Init(void)
	描述:		该函数为hoisting.c文件初始化函数
*/
void Hoisting_Init(void)
{
	Hoisting_DataInit();
}
//---
/*
	函数名：void Hoisting_DataInit(void)
	描述：	该函数为对
*/
void Hoisting_DataInit(void)
{
//	HoistingData.Flag = 0;
	HoistingData.Address = HOISTING_DefaultAddress;
	HoistingData.Weight = 0;
}
//---
/*
	函数名：Hoisting_DataStructure* Hoisting_GetData(void)
	描述：	该函数为获取HoistingData变量地址
*/
Hoisting_DataStructure* Hoisting_GetData(void)
{
	return &HoistingData;
}
//---
/*
	函数名：void Hoisting_CollectData(void)
	描述：该函数为发送采集称重传感器命令
	03 03 00 00 00 02 C5 E9
*/
void Hoisting_CollectData(void)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = HoistingData.Address;
	buffer[cnt++] = 0x03;//读取命令
	buffer[cnt++] = 0x00;//起始地址0x0000，2字节
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x00;//数据长度1个字，2字节
	buffer[cnt++] = 0x02;
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	for(crc=0;crc<cnt;crc++)
	{
		Hoistin_SendBufferData(Hoisting_UsartPort,Hoisting_UsartBuffer,buffer[crc]);//发送数据
	}
	HoistingData.RunMode = HOISTING_RunModeColHoisting;
}
//---
/*
	函数名：void Hoisting_ReadAllData(void)
	描述：	该函数为读取称重传感器内部寄存器所有的32个字的值
*/
void Hoisting_ReadAllData(void)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = HoistingData.Address;
	buffer[cnt++] = 0x03;//读取命令
	buffer[cnt++] = 0x00;//起始地址0x0000，2字节
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x00;//数据长度1个字，2字节
	buffer[cnt++] = 0x20;
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	for(crc=0;crc<cnt;crc++)
	{
		Hoistin_SendBufferData(Hoisting_UsartPort,Hoisting_UsartBuffer,buffer[crc]);//发送数据
	}
	HoistingData.RunMode = HOISTING_RunModeReadAllData;
}
//---
//-------------------------------------调试函数----------------------------------------
//---
/*
	函数名： void Hoisting_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Hoisting.c文件的ASCII调试文件
	:000|59|XXX······
*/
void Hoisting_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1://
		{//:000|59|001
			Hoisting_SetComNoneVerify();//无校验
			Hoisting_CollectData();			//发送命令
			printf("发送采集称重传感器命令!\r\n");
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
	函数名： void Hoisting_DebugRTU((u8 *p)
	描述：	该函数为Hoisting.c文件的RTU调试文件
	说明：	调用该函数之前需要先进行CRC校验
*/
void Hoisting_DebugRTU(u8 *p,u8 len)
{
	switch(HoistingData.RunMode)//查看是哪个功能码
	{
		case HOISTING_RunModeColHoisting:
		{//读取重量
			u32 weight = *(p+3)<<24 | *(p+4)<<16 | *(p+5)<<8 | *(p+6);//获取重量
			Server_GetTowerCraneData()->Hoisting = (u16)((float)weight/100000000.0);
//			printf("当前吊重为%u\r\n",Server_GetTowerCraneData()->Hoisting);
			break;
		}
		case HOISTING_RunModeReadAllData:
		{//读取重量
			u32 weight = *(p+3)<<24 | *(p+4)<<16 | *(p+5)<<8 | *(p+6);//获取重量
			Server_GetTowerCraneData()->Hoisting = (u16)((float)weight/100000000.0);
//			printf("当前吊重为%u\r\n",weight);
			break;
		}
	}
	HoistingData.RunMode = HOISTING_RunModeNone;
}
//---























