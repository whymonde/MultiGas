/*
	本文件为该项目的称重传感器相关函数
	具体函数如下
*/

#include "THBSS01.h"

Thbss01_DataStructure Thbss01Data;
/*
	函数名：void Thbss01_Main(void)
	描述：	屏幕显示主函数
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
	函数名：void Thbss01_Init(void)
	描述：	Thbss01.c文件初始化函数
*/
void Thbss01_Init(void)
{
	Thbss01_DataInit();
}
//---
/*
	函数名：void Thbss01_DataInit(void)
*/
void Thbss01_DataInit(void)
{
	TwoBytes two;
	Thbss01_Operation(202,two.U8Data,1);
	Thbss01Data.Conf.ADR = two.U8Data[0];
	printf("称重传感器通讯地址0x%X\r\n",Thbss01Data.Conf.ADR);
	Thbss01_Operation(203,two.U8Data,1);
	Thbss01Data.Conf.BR = two.U16Data;
	printf("称重传感器通讯波特率%u\r\n",Thbss01Data.Conf.BR);
}
//---
/*
	函数名：void Thbss01_Timer(void)
	描述：Thbss01.c文件定时器函数，由系统1ms定时器调用
*/
void Thbss01_Timer(void)
{
	if(Thbss01Data.Timer != 0)Thbss01Data.Timer--;
}
//---
/*
	函数名：Thbss01_DataStructure* Thbss01_GetThbss01Data(void)
	描述：获取返回Thbss01.c文件全局变量结构体
*/
Thbss01_DataStructure* Thbss01_GetThbss01Data(void)
{
	return &Thbss01Data;
}
//---
/*
	函数名：void Thbss01_Operation(u8 mode,u8 *p,u8 len)
	说明：Thbss01的数据操作
*/
void Thbss01_Operation(u16 mode,u8 *p,u8 len)
{
	switch(mode)
	{
		case 1:
		{//发送读取THBSS01所有数据命令
			Thbss01_ReadData(THBSS01_AdrBase,30);
			Thbss01Data.CollectMode = 0x01;
			break;
		}
		case 2:
		{//发送读取THBSS01重量值命令
			Thbss01_ReadData(THBSS01_AdrWeight,2);
			Thbss01Data.CollectMode = 0x02;
			break;
		}
		case 101:
		{//发送写零点值命令
			Thbss01_WriteOneWord(THBSS01_AdrCalibration,1);
			Thbss01Data.CollectMode = 0x81;
			break;
		}
		case 102:
		{//发送写砝码值命令
			Thbss01_WriteOneWord(THBSS01_AdrCalibration,2);
			Thbss01Data.CollectMode = 0x82;
			break;
		}
		case 103:
		{//发送砝码重量值
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
		{//去皮操作
			Thbss01_WriteOneWord(THBSS01_AdrShelling,1);
			Thbss01Data.CollectMode = 0x84;
			break;
		}
		case 105:
		{//取消去皮操作
			Thbss01_WriteOneWord(THBSS01_AdrShelling,2);
			Thbss01Data.CollectMode = 0x85;
			break;
		}
		case 121:
		{//设置通讯地址，
			*p = *p - THBSS01_AddressSkewing;//减去偏移值
			Thbss01_WriteOneWord(THBSS01_AdrBaudRate,*p);
			Thbss01Data.CollectMode = 0xA1;
			break;
		}
		case 122:
		{//设置通讯波特率
			TwoBytes BR;
			BR.U8Data[0] = *p;
			BR.U8Data[1] = *(p+1);
			Thbss01_WriteOneWord(THBSS01_AdrBaudRate,BR.U16Data);
			Thbss01Data.CollectMode = 0xA2;
			break;
		}		
		case 201:
		{//读取Thbss01的配置参数
			u8 buffer[THBSS01_AdrStoreEnd];
			TwoBytes a;
			Thbss01_ReadStoreData(THBSS01_AdrConfBase,buffer,THBSS01_AdrStoreEnd);
			a.U8Data[0] = buffer[THBSS01_AdrStoreBR];
			a.U8Data[1] = buffer[THBSS01_AdrStoreBR+1];
			printf("存储在EEPROM的THBSS01通讯地址为：%u\r\n",buffer[THBSS01_AdrConfBase]);
			printf("存储在EEPROM的THBSS01通讯波特率为：%u\r\n",a.U16Data);
			break;
		}
		case 202:
		{//读取存储在EEPROM的Thbss01的通讯地址
			Thbss01_ReadStoreData(THBSS01_AdrStoreADR,p,1);
//			printf("存储在EEPROM的THBSS01通讯地址为：%u\r\n",address);
			break;
		}
		case 203:
		{//读取存储在EEPROM的Thbss01的通讯波特率
			Thbss01_ReadStoreData(THBSS01_AdrStoreBR,p,2);
//			printf("存储在EEPROM的THBSS01通讯波特率为：%u\r\n",a.U16Data);
			break;
		}
		case 301:
		{//修改EEPROM的Thbss01存储配置
			Thbss01_StoreData(THBSS01_AdrConfBase,p,len);
			Thbss01_DataInit();
			break;
		}
		case 302:
		{//修改EEPROM的Thbss01存储通讯地址
			Thbss01_StoreData(THBSS01_AdrStoreADR,p,1);
			Thbss01_DataInit();
			break;
		}
		case 303:
		{//修改EEPROM的Thbss01存储通讯波特率
			Thbss01_StoreData(THBSS01_AdrStoreBR,p,2);
			Thbss01_DataInit();
			break;
		}
	}
}
//---
/*
	函数名：void Thbss01_ReadData(u16 address,u8 amount)
	描述：发送读取数据命令
	输入：u16 address读取数据的起始地址
				u8 amount读取数据的个数，以字为单位
*/
void Thbss01_ReadData(u16 address,u16 amount)
{
	u8 buffer[8],cnt=0;
	TwoBytes change;
	buffer[cnt++] = Thbss01Data.Conf.ADR;
	buffer[cnt++] = 0x03;//读取命令
	change.U16Data = address;
	buffer[cnt++] = change.U8Data[1];//起始地址0x0100，2字节
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = amount;
	buffer[cnt++] = change.U8Data[1];//数据长度0x0034(52个字数据)，2字节
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = CRC16(buffer,cnt);
	buffer[cnt++] = change.U8Data[1];//CRC高字节，2个字节
	buffer[cnt++] = change.U8Data[0];
	Thbss01_UsartModeSet();
	for(change.U16Data=0;change.U16Data<cnt;change.U16Data++)
	{
		THBSS01_SendBufferData(THBSS01_UsartPort,THBSS01_UsartBuffer(),buffer[change.U16Data]);//发送数据
	}
	Thbss01Data.Timer = 200;//启动计时
}
//---
/*
	函数名：void Thbss01_WriteOneWord(u16 address,u16 data)
	描述：	该函数为向目标寄存器写入一个字内容
*/
void Thbss01_WriteOneWord(u16 address,u16 data)
{
	u8 buffer[8],cnt=0;
	TwoBytes change;
	buffer[cnt++] = (u8)Thbss01Data.Conf.ADR;
	buffer[cnt++] = 0x06;					//写入一个字命令
	change.U16Data = address;
	buffer[cnt++] = change.U8Data[1];//地址0x0100，2字节
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = data;
	buffer[cnt++] = change.U8Data[1];//写入的数据，2字节
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = CRC16(buffer,cnt);
	buffer[cnt++] = change.U8Data[1];//CRC高字节，2个字节
	buffer[cnt++] = change.U8Data[0];
	Thbss01_UsartModeSet();
	for(change.U16Data=0;change.U16Data<cnt;change.U16Data++)
	{
		Ohrd23_SendBufferData(Ohrd23_UsartPort,Ohrd23_UsartBuffer(),buffer[change.U16Data]);//发送数据
	}
	Thbss01Data.Timer = 200;//启动计时
}
//---
/*
	函数名：void Thbss01_WriteMultiWord(u16 address,u16 *p,u16 len)
	描述：该函数为向目标寄存器写入写多个字
*/
void Thbss01_WriteMultiWord(u16 address,u16 *p,u16 len)
{
	u8 buffer[200],cnt=0,i=0;
	TwoBytes change;
	buffer[cnt++] = (u8)Thbss01Data.Conf.ADR;
	buffer[cnt++] = 0x10;//写入多个字命令
	change.U16Data = address;
	buffer[cnt++] = change.U8Data[1];//地址
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = len;
	buffer[cnt++] = change.U8Data[1];//数据字长度
	buffer[cnt++] = change.U8Data[0];
	buffer[cnt++] = len*2;					//字节数
	for(i=0;i<len;i++)
	{
		change.U16Data = *p;
		buffer[cnt++] = change.U8Data[1];//地址
		buffer[cnt++] = change.U8Data[0];
		p++;
	}
	change.U16Data = CRC16(buffer,cnt);
	buffer[cnt++] = change.U8Data[1];//CRC高字节，2个字节
	buffer[cnt++] = change.U8Data[0];
	Thbss01_UsartModeSet();
	for(change.U16Data=0;change.U16Data<cnt;change.U16Data++)
	{
		THBSS01_SendBufferData(THBSS01_UsartPort,THBSS01_UsartBuffer(),buffer[change.U16Data]);//发送数据
	}
	Thbss01Data.Timer = 200;//启动计时
}
//---
//-------------------------数据读取、存储----------------------------
//---
/*
	函数名：void Thbss01_ReadStoreData(u8 address,u8 *p,u8 len)
	描述：	读取数据
*/
void Thbss01_ReadStoreData(u8 address,u8 *p,u8 len)
{
	W25Xxx_ReadData(p,THBSS01_AdrStoreBase+address,len);
}
//---
/*
	函数名：void Thbss01_StoreData(u8 address,u8 *p,u8 len)
	描述：该函数为存储OHRD23配置参数函数
	说明：读取数据
				擦除数据
				修改数据
				保存数据
*/
void Thbss01_StoreData(u8 address,u8 *p,u8 len)
{
	u8 buffer[THBSS01_AdrStoreEnd],cnt;
	W25Xxx_ReadData(buffer,THBSS01_AdrStoreBase,THBSS01_AdrStoreEnd);//读所有配置参数
	W25Xxx_EraseSector(THBSS01_AdrStoreBase);//擦除数据
	for(cnt=0;cnt<len;cnt++)
	{
		buffer[address++] = *p;
		p++;
	}
	W25Xxx_PageWrite(buffer,THBSS01_AdrStoreBase,THBSS01_AdrStoreEnd);//保存数据
}
//---
//-----------------硬件相关-----------------
//---
/*
	void Thbss01_UsartModeSet(void)
	描述：为Thbss01配置串口通讯模式
*/
void Thbss01_UsartModeSet(void)
{
	u32 BaudRate=Thbss01Data.Conf.BR;
	THBSS01_UartModeSet(THBSS01_UsartPort,BaudRate,'n','1');
}
//---
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Thbss01_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Thbss01文件的ASCII调试文件
:000|85|001
*/
void Thbss01_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:
		{//读Thbss01所有数据
			printf("读Thbss01所有数据\r\n");
			Thbss01_Operation(1,p,0);
			break;
		}
		case 2:
		{
			printf("读Thbss01当前重量值\r\n");
			Thbss01_Operation(2,p,0);
			break;
		}
		case 101:
		{
			printf("发送写零点值命令\r\n");
			Thbss01_Operation(101,p,0);
			break;
		}
		case 102:
		{
			printf("发送写砝码值命令\r\n");
			Thbss01_Operation(102,p,0);
			break;
		}
		case 103:
		{//设置砝码值重量  :000|86|103|123456
			FourBytes buffer;
			buffer.S32Data = Calculate_CharToU16(p+4,6);//计算命令
			buffer.S32Data = 99500;
			printf("砝码值重量为%fkg\r\n",buffer.S32Data/100.0);
			Thbss01_Operation(103,buffer.U8Data,4);
			break;
		}
		case 104:
		{//发送去皮操作命令
			printf("发送去皮操作命令\r\n");
			Thbss01_Operation(104,p,0);
			break;
		}
		case 105:
		{//发送取消去皮操作命令
			printf("发送取消去皮操作命令\r\n");
			Thbss01_Operation(105,p,0);
			break;
		}
		case 121:
		{//设置地址
			u8 adr = Calculate_CharToU16(p+4,3);//计算命令
			if(adr > THBSS01_AddressSkewing)
			{
				printf("设置地址值为%u\r\n",adr);
				adr = adr-THBSS01_AddressSkewing;
				Thbss01_Operation(121,&adr,1);
			}
			else
			{
				printf("输入错误\r\n");
				printf("地址可配置范围为%u~253\r\n",THBSS01_AddressSkewing);
			}
			break;
		}
		case 122:
		{//设置存储波特率
			TwoBytes a;
			a.U16Data = Calculate_CharToU16(p+4,5);//地址
			if(a.U16Data == 4800 || a.U16Data == 9600 || a.U16Data == 19200 || a.U16Data == 38400)
			{
				printf("设置波特率值为%u\r\n",a.U16Data);
				Thbss01_Operation(122,a.U8Data,2);
			}
			else
			{
				printf("输入错误\r\n");
				printf("通讯波特率可以配置范围为4800或9600或19200或38400\r\n");
			}
			break;
		}
		case 201:
		{//读取存储配置
			Thbss01_Operation(201,p,0);
			break;
		}
		case 202:
		{//读取存储Thbss01的配置地址
			Thbss01_Operation(202,p,0);
			break;
		}
		case 203:
		{//读取存储Thbss01的配置波特率
			Thbss01_Operation(203,p,0);
			break;
		}
		case 301:
		{//修改存储配置 :000|86|301|001|38400
			u8 buffer[3];
			TwoBytes a;
			buffer[0] = Calculate_CharToU16(p+4,3);//地址
			a.U16Data = Calculate_CharToU16(p+8,5);//波特率
			if((buffer[0]> 0 && buffer[0]<253) && \
				(a.U16Data == 4800 || a.U16Data == 9600 || a.U16Data == 19200 || a.U16Data == 38400))
			{
				buffer[1] = a.U8Data[0];
				buffer[2] = a.U8Data[1];
				Thbss01_Operation(301,buffer,3);
			}
			else
			{
				printf("输入错误\r\n");
				printf("地址可以配置范围为1~253，");
				printf("通讯波特率可以配置范围为4800或9600或19200或38400\r\n");
			}
			break;
		}
		case 302:
		{//设置存储地址
			u8 a = Calculate_CharToU16(p+4,3);//地址
			if((a> 0) && (a < 250))
			{
				Thbss01_Operation(302,&a,1);
				printf("设置地址为%u\r\n",a);
			}
			else
			{
				printf("输入错误\r\n");
				printf("地址可以配置范围为1~253\r\n");
			}
			break;
		}
		case 303:
		{//设置存储波特率
			TwoBytes a;
			a.U16Data = Calculate_CharToU16(p+4,5);//地址
			if(a.U16Data == 4800 || a.U16Data == 9600 || a.U16Data == 19200 || a.U16Data == 38400)
			{
				Thbss01_Operation(303,a.U8Data,2);
				printf("设置波特率为%u\r\n",a.U16Data);
			}
			else
			{
				printf("输入错误\r\n");
				printf("通讯波特率可以配置范围为4800或9600或19200或38400\r\n");
			}
			break;
		}
	}
}
//---
/*
	函数名： 	void Thbss01_DebugRTU((u8 *p)
	描述：		该函数为Thbss01文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*/
void Thbss01_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//查看是哪个功能码
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
	函数名：void Thbss01_Receive(u8 *p,u8 len)
	描述：处理来自接收到Thbss01的一帧数据
	说明：进入该函数之前，需要先进行CRC校验
*/
void Thbss01_Receive(u8 *p,u8 len)
{
	switch(Thbss01Data.CollectMode)//查看是哪个功能码
	{
		case 0x01:
		{//读取所有数据
			break;
		}
		case 0x02:
		{//读取重量数据
			FourBytes four;
//			TwoBytes change2;
			four.U8Data[0] = p[4];
			four.U8Data[1] = p[3];
			four.U8Data[2] = p[6];
			four.U8Data[3] = p[5];
			 
			Thbss01Data.Hosting = four.S32Data;
			Tower_CalMomental();//计算力矩
//			printf("当前吊重为%dg\r\n",four.S32Data);
			break;
		}
		case 0x81:
		{
			printf("写零点值成功\r\n");
			break;
		}
		case 0x82:
		{
			printf("写砝码值成功\r\n");
			break;
		}
		case 0x83:
		{
			printf("写砝码重量成功\r\n");
			break;
		}
		case 0x84:
		{
			printf("去皮成功\r\n");
			break;
		}
		case 0x85:
		{
			printf("取消去皮成功\r\n");
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





































