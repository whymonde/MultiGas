/*
本文件为红润OHRD23系列三相液晶智能电量变送器相关函数
	具体函数如下
*/

#include "OHRD23.h"

Ohrd23_DataStructure Ohrd23Data;
/*
	函数名：void Ohrd23_Main(void)
	描述：	OHRD23.c主函数
*/
void Ohrd23_Main(void)
{
	if(Ohrd23Data.RunMode != 0)
	switch(Ohrd23Data.RunMode)
	{
		case 201:
		{
			Ohrd23Data.Flag |= B0;
			Ohrd23Data.Conf.ADR = 1;
			Ohrd23Data.Conf.BR = 4;
			Ohrd23Data.RunMode = 202;
			break;
		}
		case 202:
		{
			Ohrd23_ReadConf();
			printf("BR=%u,ADR=%u\r\n",Ohrd23Data.Conf.BR,Ohrd23Data.Conf.ADR);
			Ohrd23Data.RunMode = 203;
			break;
		}
		case 203:
		{
			if(Ohrd23Data.CollectMode == 0)
			{
				if((Ohrd23Data.Flag & B0) ==0)
				{
					printf("搜索完毕，找到OHRD23\r\n");
					Ohrd23Data.RunMode = 0;
				}
				else
				{
					Ohrd23Data.Conf.ADR++;
					if(Ohrd23Data.Conf.ADR < 254)
					{
						Ohrd23Data.RunMode = 202;
					}
					else
					{
						Ohrd23Data.Conf.ADR = 1;
						Ohrd23Data.Conf.BR--;
						if(Ohrd23Data.Conf.BR==0)
						{
							Ohrd23Data.RunMode = 204;
						}
						else
						{
							Ohrd23Data.RunMode = 202;
						}
					}
				}
			}
			break;
		}
		case 204:
		{
			Ohrd23_ReadConf();
			printf("BR=%u,ADR=%u\r\n",Ohrd23Data.Conf.BR,Ohrd23Data.Conf.ADR);
			Ohrd23Data.RunMode = 205;
			break;
		}
		case 205:
		{
			if(Ohrd23Data.CollectMode == 0)
			{
				if((Ohrd23Data.Flag & B0) ==0)
				{
					printf("搜索完毕，找到OHRD23\r\n");
					Ohrd23Data.RunMode = 0;
				}
				else
				{
					Ohrd23Data.Conf.ADR++;
					if(Ohrd23Data.Conf.ADR < 254)
					{
						Ohrd23Data.RunMode = 0;
						printf("搜索完毕，没有找到OHRD23\r\n");
					}
					else
					{
						Ohrd23Data.RunMode = 204;
					}
				}
			}
			break;
		}
	}
	
	//---
	if(Ohrd23Data.CollectMode != 0)
	{//当前有数据通讯
		if(Ohrd23Data.Timer == 0)
		{//超时未接收到数据
//			printf("超时未接收到数据\r\n");
			Ohrd23Data.CollectMode = 0;
			Ohrd23Data.ErrorCnt++;
			Ohrd23Data.Flag |= B1;//通讯故障
			if(Ohrd23Data.ErrorCnt == 10)
			{//连续10次没有收到回复则认为故障
				Ohrd23Data.ErrorCnt = 0;
				Ohrd23Data.Flag |= B0;//通讯故障
			}
		}
	}
}
//---
/*
	函数名：void Ohrd23_Collect(void)
	
*/
void Ohrd23_Collect(void)
{
	switch(Ohrd23Data.RunMode)
	{
		//采集基本电量数据
		case 1:
		{//采集基本电量参数
			Ohrd23_ReadBase();
			Ohrd23Data.Timer = 200;
			Ohrd23Data.RunMode = 2;
			break;
		}
		case 2:
		{
			if(Ohrd23Data.CollectMode == 0)
			{
				Ohrd23Data.RunMode = 1;
			}
			else if(Ohrd23Data.Timer == 0)
			{
				Ohrd23Data.RunMode = 1;
			}
			break;
		}
		//采集电能数据
		case 11:
		{
			Ohrd23_ReadEnergy();
			Ohrd23Data.Timer = 200;
			Ohrd23Data.RunMode = 12;
			break;
		}
		case 12:
		{
			if(Ohrd23Data.CollectMode == 0)
			{
//				printf("OK");
				Ohrd23Data.RunMode = 11;
			}
			else if(Ohrd23Data.Timer == 0)
			{
//				printf("BUG");
				Ohrd23Data.RunMode = 11;
			}
			break;
		}
	}
}
//---
/*
	函数名：void Ohrd23_Init(void)
	描述：	OHRD23.c文件初始化函数
*/
void Ohrd23_Init(void)
{
	u8* pdata = (u8*)&Ohrd23Data.Conf;
	Ohrd23_ReadStoreData(OHRD23_AdrConfBase,pdata,OHRD23_AdrStoreEnd);//读取配置参数
	Ohrd23Data.CollectCommand = 0;
	Ohrd23Data.Conf.ADR = 4;
	Ohrd23Data.Conf.BR = 4;
}
//---
/*
	函数名：void Ohrd23_Timer(void)
	描述：OHRD23.c文件定时器函数，由系统1ms定时器调用
*/
void Ohrd23_Timer(void)
{
	if(Ohrd23Data.Timer != 0)	Ohrd23Data.Timer--;
}
//---
/*
	函数名：Ohrd23_DataStructure* Ohrd23_GetOhrd23Data(void)
	描述：获取返回OHRD23.c文件全局变量结构体
*/
Ohrd23_DataStructure* Ohrd23_GetOhrd23Data(void)
{
	return &Ohrd23Data;
}
//---
/*
函数名：void Ohrd23_ReadBase(void)
描述：读基本电量数据
说明：首地址OHRD23_AdrBase，数据长度52
*/
void Ohrd23_ReadBase(void)
{
	Ohrd23_ReadData(OHRD23_AdrBase,52);
	Ohrd23Data.CollectMode = 0x01;
}
//---
/*
函数名：void Ohrd23_ReadBase(void)
描述：读所有电能数据
说明：首地址OHRD23_AdrEnergy，数据长度12
*/
void Ohrd23_ReadEnergy(void)
{
	Ohrd23_ReadData(OHRD23_AdrEnergy,12);
	Ohrd23Data.CollectMode = 0x02;
}
//---
/*
函数名：void Ohrd23_ReadConf(void)
描述：读基配置参数
说明：首地址OHRD23_AdrConf，数据长度5
*/
void Ohrd23_ReadConf(void)
{
	Ohrd23_ReadData(OHRD23_AdrConf,5);
	Ohrd23Data.CollectMode = 0x03;
}
//---
/*
函数名：void Ohrd23_SetVT(s16 rate)
描述：	设置电压倍率，参数输入范围1-1200
*/
void Ohrd23_SetVT(s16 rate)
{
 	TwoBytes buffer;
	buffer.S16Data = rate;
	Ohrd23_WriteOneWord(OHRD23_AdrConfVT,buffer.U16Data);
	Ohrd23Data.CollectMode = 0x81;
}
//---
/*
函数名：void Ohrd23_SetIT(s16 rate)
描述：	设置电流倍率，参数输入范围1-1200
*/
void Ohrd23_SetIT(s16 rate)
{
 	TwoBytes buffer;
	buffer.S16Data = rate;
	Ohrd23_WriteOneWord(OHRD23_AdrConfIT,buffer.U16Data);
	Ohrd23Data.CollectMode = 0x82;
}
//---
/*
函数名：void Ohrd23_SetLM(u8 mode)
描述：	设置接线模式，参数输入范围0=三相四线，1=三相三线，2=3V3A
*/
void Ohrd23_SetLM(u8 mode)
{
	Ohrd23_WriteOneWord(OHRD23_AdrConfLM,mode);
	Ohrd23Data.CollectMode = 0x83;
}
//---
/*
函数名：void Ohrd23_SetAddress(u8 address)
描述：	设置地址，参数输入范围1-253
*/
void Ohrd23_SetAddress(u8 address)
{
	Ohrd23_WriteOneWord(OHRD23_AdrConfADR,address);
	Ohrd23Data.CollectMode = 0x84;
}
//---
/*
函数名：void Ohrd23_SetAddress(u8 address)
描述：	设置地址，参数输入范围0-4分别对应1200~19200
*/
void Ohrd23_SetBaudRate(u8 baudrate)
{
	Ohrd23_WriteOneWord(OHRD23_AdrConfBR,baudrate);
	Ohrd23Data.CollectMode = 0x85;
}
//---
/*
	函数名：void Ohrd23_SetConfiguration(OHRD23_ConfigurationStructure *p)
	描述：设置OHRD23的配置参数
*/
void Ohrd23_SetConfiguration(OHRD23_ConfigurationStructure *p)
{
	TwoBytes change;
	u16 buffer[5];
	u8 cnt = 0;
	change.S16Data = p->VT;
	buffer[cnt++] = change.U16Data;
	change.S16Data = p->IT;
	buffer[cnt++] = change.U16Data;
	change.S16Data = p->LM;
	buffer[cnt++] = change.U16Data;
	change.S16Data = p->ADR;
	buffer[cnt++] = change.U16Data;
	change.S16Data = p->BR;
	buffer[cnt++] = change.U16Data;
	Ohrd23_WriteMultiWord(OHRD23_AdrConf,buffer,5);
	Ohrd23Data.CollectMode = 0x86;
}
//---
/*
函数名：void Ohrd23_ReadData(u16 address,u8 amount)
描述：发送读取数据命令
输入：u16 address读取数据的起始地址
			u8 amount读取数据的个数，以字为单位
*/
void Ohrd23_ReadData(u16 address,u16 amount)
{
	u8 buffer[8],cnt=0;
	TwoBytes change;
	buffer[cnt++] = Ohrd23Data.Conf.ADR;
	buffer[cnt++] = 0x03;//读取命令
	change.U16Data = address;
//	printf("通讯地址为=%d\r\n",address);
	buffer[cnt++] = change.U8Data[1];//起始地址0x0100，2字节
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = amount;
	buffer[cnt++] = change.U8Data[1];//数据长度0x0034(52个字数据)，2字节
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = CRC16(buffer,cnt);
	buffer[cnt++] = change.U8Data[1];//CRC高字节，2个字节
	buffer[cnt++] = change.U8Data[0];
	Ohrd23_UsartModeSet();
	for(change.U16Data=0;change.U16Data<cnt;change.U16Data++)
	{
		Ohrd23_SendBufferData(Ohrd23_UsartPort,Ohrd23_UsartBuffer(),buffer[change.U16Data]);//发送数据
	}
	Ohrd23Data.Timer = 200;//启动计时
}
//---
/*
	函数名：void Ohrd23_WriteOneWord(u16 address,u16 data)
	描述：	该函数为向目标寄存器写入一个字内容
*/
void Ohrd23_WriteOneWord(u16 address,u16 data)
{
	u8 buffer[8],cnt=0;
	TwoBytes change;
	buffer[cnt++] = (u8)Ohrd23Data.Conf.ADR;
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
	Ohrd23_UsartModeSet();
	for(change.U16Data=0;change.U16Data<cnt;change.U16Data++)
	{
		Ohrd23_SendBufferData(Ohrd23_UsartPort,Ohrd23_UsartBuffer(),buffer[change.U16Data]);//发送数据
	}
	Ohrd23Data.Timer = 200;//启动计时
}
//---
/*
	函数名：void Ohrd23_WriteMultiWord(u16 address,u16 *p,u16 len)
	描述：该函数为向目标寄存器写入写多个字
*/
void Ohrd23_WriteMultiWord(u16 address,u16 *p,u16 len)
{
	u8 buffer[200],cnt=0,i=0;
	TwoBytes change;
	buffer[cnt++] = (u8)Ohrd23Data.Conf.ADR;
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
	Ohrd23_UsartModeSet();
	for(change.U16Data=0;change.U16Data<cnt;change.U16Data++)
	{
		Ohrd23_SendBufferData(Ohrd23_UsartPort,Ohrd23_UsartBuffer(),buffer[change.U16Data]);//发送数据
	}
	Ohrd23Data.Timer = 200;//启动计时
}
//---
/*
函数名：void Ohrd23_BaseChange(u8 *p,OHRD23_BaseStructure* Base)
描述：	该函数把一帧104个字节（共26个基本电量数据）的基本电量数据转换为Ohrd23基本电量数据
				包括相电压，线电压，相电流，有功功率，无功功率，视在功率，功率因数，频率
输入:		u8 *p基本电量参数的首地址
				OHRD23_BaseStructure* Base存储OHRD23基本电量数据首地址	
说明：	OHRD23的数据是大端模式
				STM32的数据是小端模式
				所以数据转换过程为高对低，低对高
*/
void Ohrd23_BaseChange(u8 *p,OHRD23_BaseStructure* Base)
{
	u8 i;
	FourBytes four;
	s32 *pS32 = &(Base->VA);
	float *pFlaot = &(Base->PAY);
	for(i=0;i<9;i++)
	{//计算相电压，线电压，相电流
		four.U8Data[0] = *(p+3);
		four.U8Data[1] = *(p+2);
		four.U8Data[2] = *(p+1);
		four.U8Data[3] = *(p+0);
		*pS32 = four.S32Data;
		pS32++;
		p = p+4;
	}
	for(i=0;i<12;i++)
	{//计算有功功率，无功功率，视在功率
		four.U8Data[0] = *(p+3);
		four.U8Data[1] = *(p+2);
		four.U8Data[2] = *(p+1);
		four.U8Data[3] = *(p+0);
		*pFlaot = four.FloatData;
		pFlaot++;
		p = p+4;
	}
	pS32 = &(Base->FA);
	for(i=0;i<5;i++)
	{//计算相电压，线电压，相电流
		four.U8Data[0] = *(p+3);
		four.U8Data[1] = *(p+2);
		four.U8Data[2] = *(p+1);
		four.U8Data[3] = *(p+0);
		*pS32 = four.S32Data;
		pS32++;
		p = p+4;
	}
	/*
	printf("有符号长整型，等于%d，实际值%d÷100=%f\r\n",Base->VA,Base->VA,Base->VA/100.0);
	printf("有符号长整型，等于%d，实际值%d÷100=%f\r\n",Base->VB,Base->VB,Base->VB/100.0);
	printf("有符号长整型，等于%d，实际值%d÷100=%f\r\n",Base->VC,Base->VC,Base->VC/100.0);
	printf("有符号长整型，等于%d，实际值%d÷100=%f\r\n",Base->VAB,Base->VAB,Base->VAB/100.0);
	printf("有符号长整型，等于%d，实际值%d÷100=%f\r\n",Base->VBC,Base->VBC,Base->VBC/100.0);
	printf("有符号长整型，等于%d，实际值%d÷100=%f\r\n",Base->VCA,Base->VCA,Base->VCA/100.0);
	printf("有符号长整型，等于%d，实际值%d÷1000=%f\r\n",Base->IA,Base->IA,Base->IA/1000.0);
	printf("有符号长整型，等于%d，实际值%d÷1000=%f\r\n",Base->IB,Base->IB,Base->IB/1000.0);
	printf("有符号长整型，等于%d，实际值%d÷1000=%f\r\n",Base->IC,Base->IC,Base->IC/1000.0);
	printf("浮点型，等于%f，实际值%f÷100=%f\r\n",Base->PAY,Base->PAY,Base->PAY/10.0);
	printf("浮点型，等于%f，实际值%f÷100=%f\r\n",Base->PBY,Base->PBY,Base->PBY/10.0);
	printf("浮点型，等于%f，实际值%f÷100=%f\r\n",Base->PCY,Base->PCY,Base->PCY/10.0);
	printf("浮点型，等于%f，实际值%f÷100=%f\r\n",Base->PZY,Base->PZY,Base->PZY/10.0);
	printf("浮点型，等于%f，实际值%f÷100=%f\r\n",Base->PAW,Base->PAW,Base->PAW/10.0);
	printf("浮点型，等于%f，实际值%f÷100=%f\r\n",Base->PBW,Base->PBW,Base->PBW/10.0);
	printf("浮点型，等于%f，实际值%f÷100=%f\r\n",Base->PCW,Base->PCW,Base->PCW/10.0);
	printf("浮点型，等于%f，实际值%f÷100=%f\r\n",Base->PZW,Base->PZW,Base->PZW/10.0);
	printf("浮点型，等于%f，实际值%f÷100=%f\r\n",Base->PAS,Base->PAS,Base->PAS/10.0);
	printf("浮点型，等于%f，实际值%f÷100=%f\r\n",Base->PBS,Base->PBS,Base->PBS/10.0);
	printf("浮点型，等于%f，实际值%f÷100=%f\r\n",Base->PCS,Base->PCS,Base->PCS/10.0);
	printf("浮点型，等于%f，实际值%f÷100=%f\r\n",Base->PZS,Base->PZS,Base->PZS/10.0);
	printf("有符号长整型，等于%d，实际值%d÷100=%f\r\n",Base->FA,Base->FA,Base->FA/1000.0);
	printf("有符号长整型，等于%d，实际值%d÷100=%f\r\n",Base->FB,Base->FB,Base->FB/1000.0);
	printf("有符号长整型，等于%d，实际值%d÷100=%f\r\n",Base->FC,Base->FC,Base->FC/1000.0);
	printf("有符号长整型，等于%d，实际值%d÷100=%f\r\n",Base->FZ,Base->FZ,Base->FZ/1000.0);
	printf("有符号长整型，等于%d，实际值%d÷100=%f\r\n",Base->Frequency,Base->Frequency,Base->Frequency/1000.0);
	*/
	
//	printf("A相相电压=%d\r\n",Base->VA);
//	printf("B相相电压=%d\r\n",Base->VB);
//	printf("C相相电压=%d\r\n",Base->VC);
//	printf("A-B线电压=%d\r\n",Base->VAB);
//	printf("B-C线电压=%d\r\n",Base->VBC);
//	printf("C-A线电压=%d\r\n",Base->VCA);
//	printf("A相电流=%d\r\n",Base->IA);
//	printf("B相电流=%d\r\n",Base->IB);
//	printf("C相电流=%d\r\n",Base->IC);
//	printf("A相有功功率=%f\r\n",Base->PAY);
//	printf("B相有功功率=%f\r\n",Base->PBY);
//	printf("C相有功功率=%f\r\n",Base->PCY);
//	printf("总相有功功率=%f\r\n",Base->PZY);
//	printf("A相无功功率=%f\r\n",Base->PAW);
//	printf("B相无功功率=%f\r\n",Base->PBW);
//	printf("C相无功功率=%f\r\n",Base->PCW);
//	printf("总相无功功率=%f\r\n",Base->PZW);
//	printf("A相视功功率=%f\r\n",Base->PAS);
//	printf("B相视功功率=%f\r\n",Base->PBS);
//	printf("C相视功功率=%f\r\n",Base->PCS);
//	printf("总相视功功率=%f\r\n",Base->PZS);
//	printf("A相功率因数=%d\r\n",Base->FA);
//	printf("B相功率因数=%d\r\n",Base->FB);
//	printf("C相功率因数=%d\r\n",Base->FC);
//	printf("总相功率因数=%d\r\n",Base->FZ);
//	printf("频率=%d\r\n",Base->Frequency);
}
//---
/*
函数名：void Ohrd23_EnergyChange(u8 *p,OHRD23_EnergyStructure* Energy)
描述：	该函数把一帧24个字节（共6个电能参数）的电能数据转换为Ohrd23的电能数据
输入:		u8 *p基本电量参数的首地址
				OHRD23_EnergyStructure* Energy存储OHRD23电能数据首地址	
说明：	OHRD23的数据是大端模式
				STM32的数据是小端模式
				所以数据转换过程为高对低，低对高	
*/
void Ohrd23_EnergyChange(u8 *p,OHRD23_EnergyStructure* Energy)
{
	u8 i;
	FourBytes buffer;
	s32 *pS32 = &(Energy->WHP);
	u32 *pU32 = &(Energy->SWHP);
	for(i=0;i<4;i++)
	{//计算正向有功电能，反向有功电能，正向无功电能，反向无功电能
		buffer.U8Data[0] = *(p+3);
		buffer.U8Data[1] = *(p+2);
		buffer.U8Data[2] = *(p+1);
		buffer.U8Data[3] = *(p+0);
		*pS32 = buffer.S32Data;
		pS32++;
		p = p+4;
	}
	for(i=0;i<2;i++)
	{//计算总有功电能，总无功电能
		buffer.U8Data[0] = *(p+3);
		buffer.U8Data[1] = *(p+2);
		buffer.U8Data[2] = *(p+1);
		buffer.U8Data[3] = *(p+0);
		*pU32 = buffer.U32Data;
		pU32++;
		p = p+4;
	}
	
//	printf("正向有功电能=%f\r\n",Energy->WHP/100.0);
//	printf("反向有功电能=%f\r\n",Energy->WHN/100.0);
//	printf("正向无功电能=%f\r\n",Energy->VARHP/100.0);
//	printf("反向无功电能=%f\r\n",Energy->VARHN/100.0);
//	printf("总有功电能=%f\r\n",Energy->SWHP/100.0);
//	printf("总无功电能=%f\r\n",Energy->SWHN/100.0);
	
	
	
	
	
}
//---
/*
	函数名：void Ohrd23_ConfigurationChange(u8 *p,OHRD23_ConfigurationStructure *conf)
	描述：
*/
void Ohrd23_ConfigurationChange(u8 *p,OHRD23_ConfigurationStructure *conf)
{
	u8 i;
	TwoBytes buffer;
	s16 *pS16 = &conf->VT;
	for(i=0;i<5;i++)
	{
		buffer.U8Data[0] = *(p+1);
		buffer.U8Data[1] = *(p);
		*pS16 = buffer.S16Data;
		pS16++;
		p = p+2;
	}
//	printf("电压倍率=%d\r\n",conf->VT);
//	printf("电流倍率=%d\r\n",conf->IT);
//	printf("接线模式=%d\r\n",conf->LM);
//	printf("通讯地址=%d\r\n",conf->ADR);
//	printf("通讯波特率=%d\r\n",conf->BR);
}
//---
/*
	函数名：void Ohrd23_SearchOhrd23(void)
	描述：该函数为查找总线上的Ohrd23电量仪
	说明：需要在主函数的过程中调用该函数，调用该函数后，该函数将直接占有系统，知道查找完毕
*/
void Ohrd23_SearchOhrd23(void)
{
	u8 i,j;
	Ohrd23Data.Flag |= B0;//通讯故障
	for(i=5;i>0;i--)
	{
		printf("\r\n%u\r\n",i-1);
//		Ohrd23Data.Conf.BR = i;
		for(j=1;j<253;j++)
		{
//			Ohrd23Data.Conf.ADR = j;
//			Ohrd23_ReadConf();
//			printf("A ");
//			Ohrd23Data.CollectMode = 1;
//			Ohrd23Data.Timer = 1000;//启动计时
//			while(Ohrd23Data.CollectMode != 0);
			Ohrd23Data.Timer = 10;//启动计时
			Ohrd23Data.CollectMode = 1;
			printf("%u ",j);
			while(Ohrd23Data.CollectMode != 0);
//			if(Ohrd23Data.Flag & B0) break;
		}
//		if(Ohrd23Data.Flag & B0) break;
	}
}
//---
/*
	void Ohrd23_UsartModeSet(void)
	描述：为Ohrd23配置串口通讯模式
*/
void Ohrd23_UsartModeSet(void)
{
	u32 BaudRate=(u32)(1200*pow(2,Ohrd23Data.Conf.BR));
//	printf("通讯波特率=%d\r\n",BaudRate);
	Ohrd23_UartModeSet(Ohrd23_UsartPort,BaudRate,'n','1');
}
//---
/*
	函数名：void Ohrd23_Operation(u16 mode,u8 *p,u8 len)
	描述：该函数为Ohrd23.c的数据操作
*/
void Ohrd23_Operation(u16 mode,u8 *p,u8 len)
{
	switch(mode)
	{
		//读取
		case 1:
		{//读取所有配置参数
			break;
		}
		case 2:
		{//读电压倍率
			Ohrd23_ReadStoreData(OHRD23_AdrStoreVT,p,len);
			break;
		}
		case 3:
		{//读电流倍率
			Ohrd23_ReadStoreData(OHRD23_AdrStoreIT,p,len);
			break;
		}
		case 4:
		{//读接线方式
			Ohrd23_ReadStoreData(OHRD23_AdrStoreLM,p,len);
			break;
		}
		case 5:
		{//读通讯地址
			Ohrd23_ReadStoreData(OHRD23_AdrStoreADR,p,len);
			break;
		}
		case 6:
		{//读通讯波特率
			Ohrd23_ReadStoreData(OHRD23_AdrStoreBR,p,len);
			break;
		}
		//修改
		case 101:
		{//修改所有配置参数
			break;
		}
		case 102:
		{//修改电压倍率
			Ohrd23_StoreData(OHRD23_AdrStoreVT,p,len);
			break;
		}
		case 103:
		{//修改电流倍率
			Ohrd23_StoreData(OHRD23_AdrStoreIT,p,len);
			break;
		}
		case 104:
		{//修改接线方式
			Ohrd23_StoreData(OHRD23_AdrStoreLM,p,len);
			break;
		}
		case 105:
		{//修改通讯地址
			Ohrd23_StoreData(OHRD23_AdrStoreADR,p,len);
			break;
		}
		case 106:
		{//修改通讯波特率
			Ohrd23_StoreData(OHRD23_AdrStoreBR,p,len);
			break;
		}
	}
}
//-------------------------数据读取、存储----------------------------
//---
/*
	函数名：void Ohrd23_ReadStoreData(u8 address,u8 *p,u8 len)
	描述：	读取数据
*/
void Ohrd23_ReadStoreData(u8 address,u8 *p,u8 len)
{
	W25Xxx_ReadData(p,OHRD23_AdrStoreBase+address,len);
}
//---
/*
	函数名：void Ohrd23_StoreData(u8 address,u8 *p,u8 len)
	描述：该函数为存储OHRD23配置参数函数
	说明：读取数据
				擦除数据
				修改数据
				保存数据
*/
void Ohrd23_StoreData(u8 address,u8 *p,u8 len)
{
	u8 buffer[OHRD23_AdrStoreEnd],cnt;
	W25Xxx_ReadData(buffer,OHRD23_AdrStoreBase,OHRD23_AdrStoreEnd);//读所有配置参数
	W25Xxx_EraseSector(OHRD23_AdrStoreBase);//擦除数据
	for(cnt=0;cnt<len;cnt++)
	{
		buffer[address++] = *p;
		p++;
	}
	W25Xxx_PageWrite(buffer,OHRD23_AdrStoreBase,OHRD23_AdrStoreEnd);//保存数据
}
/*
	函数名：void Ohrd23_RestoreFactorySetting(void)
	描述：恢复出厂设置，包括电压倍率，电流倍率，接线方式，通讯地址，通讯波特率
*/
void Ohrd23_RestoreFactorySetting(void)
{
	u8 buffer[OHRD23_AdrStoreEnd],cnt=0;
	TwoBytes change;
	W25Xxx_EraseSector(OHRD23_AdrStoreBase);//擦除数据
	Ohrd23Data.Conf.VT = 1;
	Ohrd23Data.Conf.IT = 40;
	Ohrd23Data.Conf.LM = 0;
	Ohrd23Data.Conf.ADR = 0x01;
	Ohrd23Data.Conf.BR = 3;
	change.S16Data = Ohrd23Data.Conf.VT;
	buffer[cnt++] = change.U8Data[0];
	buffer[cnt++] = change.U8Data[1];
	change.S16Data = Ohrd23Data.Conf.IT;
	buffer[cnt++] = change.U8Data[0];
	buffer[cnt++] = change.U8Data[1];
	change.S16Data = Ohrd23Data.Conf.LM;
	buffer[cnt++] = change.U8Data[0];
	buffer[cnt++] = change.U8Data[1];
	change.S16Data = Ohrd23Data.Conf.ADR;
	buffer[cnt++] = change.U8Data[0];
	buffer[cnt++] = change.U8Data[1];
	change.S16Data = Ohrd23Data.Conf.BR;
	buffer[cnt++] = change.U8Data[0];
	buffer[cnt++] = change.U8Data[1];
	W25Xxx_PageWrite(buffer,OHRD23_AdrStoreBase,cnt);//保存数据
}
//---
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Ohrd23_DebugASCII(u8 *p,u8 len)
	描述：		该函数为OHRD23文件的ASCII调试文件
:000|85|001
*/
void Ohrd23_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:
		{//读基本电量数据
			Ohrd23_ReadBase();
			printf("读基本电量数据\r\n");
			break;
		}
		case 2:
		{//读电能数据
			Ohrd23_ReadEnergy();
			printf("读电能数据\r\n");
			break;
		}
		case 3:
		{//读配置数据
			Ohrd23_ReadConf();
			printf("读配置参数\r\n");
			break;
		}
		//---
		case 101://:000|85|101|0001
		{//设置电压倍率
			u16 rate = Calculate_CharToU16(p+4,4);//计算命令
			if(rate<=1200)
			{
				Ohrd23_SetVT(rate);
				printf("设置电压倍率为%u\r\n",rate);
			}
			else
			{
				printf("输入错误，设置电压倍率范围为0~1200\r\n");
			}
			break;
		}
		case 102://:000|85|102|0040
		{//设置电流倍率
			u16 rate = Calculate_CharToU16(p+4,4);//计算命令
			if(rate<=1200)
			{
				Ohrd23_SetIT(rate);
				printf("设置电流倍率为%u\r\n",rate);
			}
			else
			{
				printf("输入错误，设置电流倍率范围为0~1200\r\n");
			}
			break;
		}
		case 103:
		{//设置接线模式
			if(*(p+4) == '0')
			{
				Ohrd23_SetLM(0);
				printf("接线模式可设置为0=三相四线\r\n");
			}
			else if(*(p+4) == '1')
			{
				Ohrd23_SetLM(1);
				printf("接线模式可设置为1=三相三线\r\n");
			}
			else if(*(p+4) == '2')
			{
				Ohrd23_SetLM(2);
				printf("接线模式2=3V3A\r\n");
			}
			else
			{
				printf("输入错误，接线模式可设置为0=三相四线，1=三相三线，2=3V3A\r\n");
			}
			break;
		}
		case 104:
		{//设置地址
			u8 address = Calculate_CharToU16(p+4,3);//计算命令
			if((address>0) && (address<253))
			{
				Ohrd23_SetAddress(address);
				printf("通讯地址设置为%u\r\n",address);
			}
			else
			{
				printf("输入错误，通讯地址设置范围为1~253\r\n");
			}
			break;
		}
		case 105:
		{//设置波特率
			u8 baudrate = Calculate_CharToU16(p+4,1);//计算命令
			if(baudrate<5)
			{
				Ohrd23_SetBaudRate(baudrate);
				printf("通讯波特率设置为%u\r\n",(u16)(1200*pow(2,baudrate)));
			}
			else
			{
				printf("输入错误，通讯波特率设置范围为0-4\r\n");
			}
			break;
		}
		case 106:
		{//设置配置参数 :000|85|106|1000|1000|0|004|3
			OHRD23_ConfigurationStructure buffer;
			buffer.VT = Calculate_CharToU16(p+4,4);
			buffer.IT = Calculate_CharToU16(p+9,4);
			buffer.LM = Calculate_CharToU16(p+14,1);
			buffer.ADR = Calculate_CharToU16(p+16,3);
			buffer.BR = Calculate_CharToU16(p+20,1);
			if(buffer.VT<1201 && buffer.IT<1201 && buffer.LM<3 && buffer.ADR>1 && buffer.ADR<253 && buffer.BR<5)
			{
				Ohrd23_SetConfiguration(&buffer);
				Ohrd23Data.Conf = buffer;
				printf("设置电压倍率为%d,",buffer.VT);
				printf("电流倍率为%d,",buffer.IT);
				printf("接线方式为%d,",buffer.LM);
				printf("通讯地址为%d,",buffer.ADR);
				printf("通讯波特率为%d\r\n",buffer.BR);
			}
			else
			{
				printf("输入错误\r\n");
				printf("电压倍率范围为0~1200,");
				printf("电流倍率范围为0~1200,");
				printf("接线方式范围为0~2,");
				printf("通讯地址范围为1~253,");
				printf("通讯波特率范围为0~4\r\n");
			}
			break;
		}
		case 201:
		{//读取存储在EEPROM的配置参数
//			OHRD23_ConfigurationStructure buffer;
//			u8 *pdata = (u8*)&buffer;	
//			Ohrd23_ReadStoreData(OHRD23_AdrConfBase,pdata,OHRD23_AdrStoreEnd);
//			printf("电压倍率为%d,",buffer.VT);
//			printf("电流倍率为%d,",buffer.IT);
//			printf("接线方式为%d,",buffer.LM);
//			printf("通讯地址为%d,",buffer.ADR);
//			printf("通讯波特率为%d\r\n",buffer.BR);
			break;
		}
		case 202:
		{//读取存储在EEPROM的电压倍率
			TwoBytes two;
			Ohrd23_Operation(2,two.U8Data,2);
			printf("存在在EEPROM的电压倍率为%u\r\n",two.U16Data);
			break;
		}
		case 203:
		{//读取存储在EEPROM的电流倍率
			TwoBytes two;
			Ohrd23_Operation(3,two.U8Data,2);
			printf("存在在EEPROM的电流倍率为%d\r\n",two.U16Data);
			break;
		}
		case 204:
		{//读取存储在EEPROM的接线方式
			u8 bf;
			Ohrd23_Operation(4,&bf,1);
			printf("存在在EEPROM的接线方式为%d\r\n",bf);
			break;
		}
		case 205:
		{//读取存储在EEPROM的通讯地址
			u8 bf;
			Ohrd23_Operation(5,&bf,1);
			printf("存在在EEPROM的通讯地址为%d\r\n",bf);
			break;
		}
		case 206:
		{//读取存储在EEPROM的通讯波特率
			TwoBytes two;
			Ohrd23_Operation(6,two.U8Data,2);
			printf("存在在EEPROM的通讯波特率为%d\r\n",two.U16Data);
			break;
		}
		case 301:
		{//设置存储在EEPROM的配置参数
			OHRD23_ConfigurationStructure buffer;
			buffer.VT = Calculate_CharToU16(p+4,4);
			buffer.IT = Calculate_CharToU16(p+9,4);
			buffer.LM = Calculate_CharToU16(p+14,1);
			buffer.ADR = Calculate_CharToU16(p+16,3);
			buffer.BR = Calculate_CharToU16(p+20,1);
			if(buffer.VT<1201 && buffer.IT<1201 && buffer.LM<3 && buffer.ADR>1 && buffer.ADR<253 && buffer.BR<5)
			{
				u8 *pdata = (u8*)&buffer;
				Ohrd23_StoreData(OHRD23_AdrConfBase,pdata,OHRD23_AdrStoreEnd);//存储数据	
				printf("设置电压倍率为%d,",buffer.VT);
				printf("电流倍率为%d,",buffer.IT);
				printf("接线方式为%d,",buffer.LM);
				printf("通讯地址为%d,",buffer.ADR);
				printf("通讯波特率为%d\r\n",buffer.BR);
			}
			else
			{
				printf("输入错误\r\n");
				printf("电压倍率范围为0~1200,");
				printf("电流倍率范围为0~1200,");
				printf("接线方式范围为0~2,");
				printf("通讯地址范围为1~253,");
				printf("通讯波特率范围为0~4\r\n");
			}
			break;
		}
		case 302:
		{//设置存储在EEPROM的电压倍率
			TwoBytes two;
			two.U16Data = Calculate_CharToU16(p+4,4);//计算命令
			if(two.U16Data<=1200)
			{
				Ohrd23_Operation(102,two.U8Data,2);
				printf("设置电压倍率为%u\r\n",two.U16Data);
			}
			else
			{
				printf("输入错误，设置电压倍率范围为0~1200\r\n");
			}
			break;
		}
		case 303:
		{//设置存储在EEPROM的电流倍率
			TwoBytes two;
			two.U16Data = Calculate_CharToU16(p+4,4);//计算命令
			if(two.U16Data<=1200)
			{
				Ohrd23_Operation(103,two.U8Data,2);
				printf("设置电流倍率为%u\r\n",two.U16Data);
			}
			else
			{
				printf("输入错误，设置电流倍率范围为0~1200\r\n");
			}
			break;
		}
		case 304:
		{//设置存储在EEPROM的接线方式
			u8 bf = Calculate_CharToU16(p+4,1);//计算命令
			if(bf<3)
			{
				Ohrd23_Operation(104,&bf,1);
				printf("设置接线模式为%u\r\n",bf);
			}
			else
			{
				printf("输入错误，接线模式可设置为0=三相四线，1=三相三线，2=3V3A\r\n");
			}
			break;
		}
		case 305:
		{//设置存储在EEPROM的通讯地址
			u8 bf = Calculate_CharToU16(p+4,3);//计算命令
			if((bf>0) && (bf<253))
			{
				Ohrd23_Operation(105,&bf,1);
				printf("通讯地址设置为%u\r\n",bf);
			}
			else
			{
				printf("输入错误，通讯地址设置范围为1~253\r\n");
			}
			break;
		}
		case 306:
		{//设置存储在EEPROM的波特率
			TwoBytes two;
			two.U16Data = Calculate_CharToU16(p+4,5);//计算命令
			if((two.U16Data==1200) || (two.U16Data==2400) || (two.U16Data==4800) || (two.U16Data==9600) || (two.U16Data==19200))
			{
				Ohrd23_Operation(106,two.U8Data,2);
				printf("通讯波特率设置为%u\r\n",two.U16Data);
			}
			else
			{
				printf("输入错误，通讯波特率设置范围为1200,2400,4800,9600,19200\r\n");
			}
			break;
		}
		case 500:
		{
			Ohrd23Data.RunMode = 201;
			break;
		}
		case 900:
		{
			printf("搜索OHRD23\r\n");
			Ohrd23_SearchOhrd23();
			break;
		}
		default:
		{
			printf("输入OHRD23命令错误\r\n");
			break;
		}
	}
}
//---
/*
	函数名： 	void Ohrd23_DebugRTU((u8 *p)
	描述：		该函数为OHRD23文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*/
void Ohrd23_DebugRTU(u8 *p,u8 len)
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
	函数名：void Ohrd23_Receive(u8 *p,u8 len)
	描述：处理来自接收到Ohrd23的一帧数据
	说明：进入该函数之前，需要先进行CRC校验
*/
void Ohrd23_Receive(u8 *p,u8 len)
{
	switch(Ohrd23Data.CollectMode)//查看是哪个功能码
	{
		case 0x01:
		{//读基本电量参数
			if(*(p+1) == 0x03)
			{//读取成功
				Ohrd23_BaseChange(p+3,&Ohrd23Data.Base);//数据处理
			}
			else if(*(p+1) == 0x83)
			{//读取错误
				printf("读取基本数据错误\r\n");
			}
			break;
		}
		case 0x02:
		{//读电能参数
			if(*(p+1) == 0x03)
			{//读取成功
				Ohrd23_EnergyChange(p+3,&Ohrd23Data.Energy);//数据处理
			}
			else if(*(p+1) == 0x83)
			{//读取错误
				printf("读取电能数据错误\r\n");
			}
			break;
		}
		case 0x03:
		{//读配置参数
			if(*(p+1) == 0x03)
			{//读取成功
				Ohrd23_ConfigurationChange(p+3,&Ohrd23Data.Conf);//数据处理
			}
			else if(*(p+1) == 0x83)
			{//读取错误
				printf("读取电能数据错误\r\n");
			}
			break;
		}
		case 0x81:
		{//设置电压倍率
			if(*(p+1) == 0x06)
			{//设置成功
				TwoBytes buffer;
				buffer.U8Data[0] = *(p+5);
				buffer.U8Data[1] = *(p+4);
//				u8 *pdata = (u8*)&(Ohrd23Data.Conf.VT);
				Ohrd23Data.Conf.VT = buffer.S16Data;
				Ohrd23_StoreData(OHRD23_AdrStoreVT,buffer.U8Data,2);//存储数据
				printf("设置电压倍率为%d\r\n",Ohrd23Data.Conf.VT);
			}
			else if(*(p+1) == 0x86)
			{//设置失败
				printf("电压倍率失败\r\n");
			}
			break;
		}
		case 0x82:
		{//设置电流倍率
			if(*(p+1) == 0x06)
			{//设置成功
				TwoBytes buffer;
				buffer.U8Data[0] = *(p+5);
				buffer.U8Data[1] = *(p+4);
//				u8 *pdata = (u8*)&Ohrd23Data.Conf.IT;
				Ohrd23Data.Conf.IT = buffer.S16Data;
				Ohrd23_StoreData(OHRD23_AdrStoreIT,buffer.U8Data,2);//存储数据
				printf("设置电流倍率为%d\r\n",Ohrd23Data.Conf.IT);
			}
			else if(*(p+1) == 0x86)
			{//设置失败
				printf("设置电流倍率失败\r\n");
			}
			break;
		}
		case 0x83:
		{//设置接线模式
			if(*(p+1) == 0x06)
			{//设置成功
				TwoBytes buffer;
				buffer.U8Data[0] = *(p+5);
				buffer.U8Data[1] = *(p+4);
				Ohrd23Data.Conf.LM = buffer.S16Data;
//				u8 *pdata = (u8*)&Ohrd23Data.Conf.LM;
				Ohrd23_StoreData(OHRD23_AdrStoreLM,buffer.U8Data,2);//存储数据
				printf("设置接线模式为%d\r\n",Ohrd23Data.Conf.LM);
			}
			else if(*(p+1) == 0x86)
			{//设置失败
				printf("设置接线模式失败\r\n");
			}
			break;
		}
		case 0x84:
		{//设置地址
			if(*(p+1) == 0x06)
			{//设置成功
				TwoBytes buffer;
				buffer.U8Data[0] = *(p+5);
				buffer.U8Data[1] = *(p+4);
//				u8 *pdata = (u8*)&Ohrd23Data.Conf.ADR;
				Ohrd23Data.Conf.ADR = buffer.S16Data;
				Ohrd23_StoreData(OHRD23_AdrStoreADR,buffer.U8Data,2);//
				printf("设置新地址为%d\r\n",Ohrd23Data.Conf.ADR);
			}
			else if(*(p+1) == 0x86)
			{//设置失败
				printf("地址设置失败\r\n");
			}
			break;
		}
		case 0x85:
		{//设置波特率
			if(*(p+1) == 0x06)
			{//设置成功
				TwoBytes buffer;
				buffer.U8Data[0] = *(p+5);
				buffer.U8Data[1] = *(p+4);
//				u8 *pdata = (u8*)&Ohrd23Data.Conf.BR;
				Ohrd23Data.Conf.BR = buffer.S16Data;
				Ohrd23_StoreData(OHRD23_AdrStoreBR,buffer.U8Data,2);//
				printf("设置波特率为%d\r\n",(u16)(1200*pow(2,Ohrd23Data.Conf.BR)));
			}
			else if(*(p+1) == 0x86)
			{//设置失败
				printf("地址设置失败\r\n");
			}
			break;
		}
		case 0x86:
		{//所有配置参数
			if(*(p+1) == 0x10)
			{//设置成功
				u8 *pdata = (u8*)&Ohrd23Data.Conf.VT;
				Ohrd23_StoreData(OHRD23_AdrConfBase,pdata,OHRD23_AdrStoreEnd);//存储数据
				printf("设置成功\r\n");
			}
			else if(*(p+1) == 0x90)
			{//设置失败，读取存储的数据
//				u8 *pdata = (u8*)&Ohrd23Data.Conf.VT;
				Ohrd23_ReadStoreData(OHRD23_AdrConfBase,(u8*)&Ohrd23Data.Conf.VT,OHRD23_AdrStoreEnd);	
				printf("设置失败\r\n");
			}
			break;
		}
	}
	Ohrd23Data.CollectMode = 0;
	Ohrd23Data.Flag &= ~(B0|B1);
	Ohrd23Data.ErrorCnt = 0;
}
//---





































