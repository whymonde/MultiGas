/*
	本文件为该项目的相关函数
	具体函数如下

*/

#include "server.h"


Sever_TowerCraneStructure TowerCraneData;//塔机发送数据结构体
/*
	函数名：void Server_Init(void);
	描述：		该函数为server.c初始化函数
*/
void Server_Init(void)
{
	Server_PwrIoInt();
	Server_InitTowerCraneData();
}
//---
/*
	函数名：void Server_PwrIoInt(void)
	描述：数据传输模块A1电源IO口初始化
	说明：使用到PA8端口，0=打开电源，1=关闭电源
*/
void Server_PwrIoInt(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(SERVERPWR_APP, ENABLE);  //RCC_APB2Periph_AFIO
  GPIO_InitStructure.GPIO_Pin = SERVERPWR_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SERVERPWR_PORT, &GPIO_InitStructure);
	SERVERPWR_OPEN;//打开A1电源
}
//---
/*
	函数名： Sever_TowerCraneStructure Server_GetTowerCraneData(void)
	描述：该函数为用于获取Server.c结构体变量TowerCraneData的地址
*/
Sever_TowerCraneStructure *Server_GetTowerCraneData(void)
{
	return &TowerCraneData;
}
//---
/*
	函数名：void Server_InitTowerCraneData(void)
	描述：对TowerCraneData结构体变量进行初始化
*/
void Server_InitTowerCraneData(void)
{
	TowerCraneData.Address = 0x08;//A1通讯模块地址
	TowerCraneData.Address = 0x0C;//A1通讯模块地址
	TowerCraneData.Flag = 0;
	TowerCraneData.Flag |= B0;//系统开机
	TowerCraneData.RunMode = SERVER_RunModeNone;
	//数据初始化
	TowerCraneData.LocalNumber = 180600011;//A1模块序列号
	TowerCraneData.Manufacturer = 22861;//塔机制造商
	TowerCraneData.Version = 553;//塔机型号
	TowerCraneData.SerialNumber = 180600011;//塔机编号
	TowerCraneData.Height = 100;//塔机高度，单位‘米’
	TowerCraneData.Length = 60;//塔机臂长，单位‘米’
	TowerCraneData.RatedLoad = 6;//塔机额定载重，单位‘吨’
	TowerCraneData.OutdoorsTem = 286;//户外温度，单位0.1摄氏度
	TowerCraneData.WindSpeed  = 0;//风速，单位0.1米
	TowerCraneData.UpHour = 0;//开机时间：小时
	TowerCraneData.UpMinutes = 0;//开机时间：分钟
	TowerCraneData.UpSec = 0;//开机时间：秒
	TowerCraneData.WorkingHour = 0;//工作时间：小时
	TowerCraneData.WorkingMinutes = 0;//工作时间：分钟
	TowerCraneData.WorkingSec = 0;//工作时间：秒
	TowerCraneData.Rotation = 100;//塔机回转数据，单位度，范围0~359
	TowerCraneData.Range = 3000;//塔机变幅，单位0.01米
	TowerCraneData.Raising = 5000;//塔机起升，单位0.01米
	TowerCraneData.HookHeight = 5000;//吊钩高度，单位0.01米
	TowerCraneData.Hoisting = 258;//当前吊重，单位0.01吨
	TowerCraneData.CurrentPower = 150;//当前功率，单位0.1千瓦
	TowerCraneData.Momental = 80;//当前力矩，当前塔机起升力矩到达最大力矩的百分比，取值0~100
	TowerCraneData.TaErrorCode = 11111;//当前塔机错误代码
	TowerCraneData.BianErrorCode = 22222;//变频器错误代码
	TowerCraneData.PlcInState = 0xAAAAAAAA;//当前PLC输入状态
	TowerCraneData.PlcOutState = 0x55555555;//当前PLC输出状态
}
//---
/*
	函数名：void Server_Timer(void)
	描述：	该函数为server.c文件的时钟函数，时钟单位为1ms，由系统1ms定时器中断调用
*/
void Server_Timer(void)
{
	TowerCraneData.Timer++;
	if(TowerCraneData.Timer == 1000)
	{//1S计时
		TowerCraneData.Timer = 0;
		if(TowerCraneData.Flag & b0)
		{//系统开机
			TowerCraneData.UpSec++;//开机时间秒
			if(TowerCraneData.UpSec == 60)
			{//分
				TowerCraneData.UpSec = 0;
				TowerCraneData.UpMinutes++;
				if(TowerCraneData.UpMinutes == 60)
				{//时
					TowerCraneData.UpMinutes = 0;
					TowerCraneData.UpHour++;
				}
			}
		}
		if(TowerCraneData.Flag & b1)
		{//系统工作
			TowerCraneData.WorkingSec++;//开机时间秒
			if(TowerCraneData.WorkingSec == 60)
			{//分
				TowerCraneData.WorkingSec = 0;
				TowerCraneData.WorkingMinutes++;
				if(TowerCraneData.WorkingMinutes == 60)
				{//时
					TowerCraneData.WorkingMinutes = 0;
					TowerCraneData.WorkingHour++;
				}
			}
		}
	}
}
//---
/*
	函数名：void Server_Writ30to44(void)
	描述：	该函数为向A1模块写30~44地址寄存器
	08 10 00 1E 00 0F 1E 00 03 00 00 00 00 00 
	00 00 00 00 00 00 00 00 00 00 00 00 00 00 
	00 00 00 00 00 00 00 00 00 5F 12
*/
void Server_Writ30to44(void)
{
	u8 buffer[40];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = TowerCraneData.Address;//地址
	buffer[cnt++] = 0x10;//写命令
	buffer[cnt++] = 0x00;//起始地址0x001E(30)，2个字节
	buffer[cnt++] = 0x1E;
	buffer[cnt++] = 0x00;//字数据长度0x000F(15),2个字节
	buffer[cnt++] = 0x0F;
	buffer[cnt++] = 0x1E;//字节数据长度0x1E(30),1个字节
	//30（单字）
	buffer[cnt++] = TowerCraneData.Hoisting>>8;//当前起吊重量高字节，2个字节
	buffer[cnt++] = TowerCraneData.Hoisting;//当前起吊重量低字节，2个字节
	//31（单字）
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
//	
//	buffer[cnt++] = TowerCraneData.Version>>8;//塔机型号高字节，2个字节
//	buffer[cnt++] = TowerCraneData.Version;//塔机型号低字节，2个字节
	//32~33（双字）
	buffer[cnt++] = TowerCraneData.SerialNumber>>8;//塔机编号高字节，4个字节
	buffer[cnt++] = TowerCraneData.SerialNumber;//塔机编号低字节，4个字节
	buffer[cnt++] = TowerCraneData.SerialNumber>>24;//塔机编号24~32字节，4个字节
	buffer[cnt++] = TowerCraneData.SerialNumber>>16;//塔机编号低字节，4个字节
	//34（单字）
	buffer[cnt++] = TowerCraneData.CurrentPower>>8;//当前功率高字节，2个字节
	buffer[cnt++] = TowerCraneData.CurrentPower;//当前功率低字节，2个字节
	//35（单字）
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//36（单字）
	buffer[cnt++] = TowerCraneData.Height>>8;//塔机高度高字节，2个字节
	buffer[cnt++] = TowerCraneData.Height;//塔机高度低字节，2个字节
	//37（单字）
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//38（单字）
	buffer[cnt++] = TowerCraneData.Length>>8;//塔臂长度高字节，2个字节
	buffer[cnt++] = TowerCraneData.Length;//塔臂长度低字节，2个字节
	//39（单字）
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//40（单字）
	buffer[cnt++] = TowerCraneData.RatedLoad>>8;//额定载重高字节，2个字节
	buffer[cnt++] = TowerCraneData.RatedLoad;//额定载重低字节，2个字节
	//41（单字）
	buffer[cnt++] = TowerCraneData.OutdoorsTem>>8;//户外温度高字节，2个字节
	buffer[cnt++] = TowerCraneData.OutdoorsTem;//户外温度低字节，2个字节
	//42（单字）
	buffer[cnt++] = TowerCraneData.WindSpeed>>8;//风速高字节，2个字节
	buffer[cnt++] = TowerCraneData.WindSpeed;//风速低字节，2个字节
	//43（单字）
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//44（单字）
	buffer[cnt++] = TowerCraneData.UpHour>>8;//开机时间：小时高字节，2个字节
	buffer[cnt++] = TowerCraneData.UpHour;//开机时间：小时低字节，2个字节
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	USART_SetEvenVerify();
	for(crc=0;crc<cnt;crc++)
	{
		Server_SendBufferData(Server_UsartServerPort,Server_UsartServerBuffer,buffer[crc]);//发送数据
	}
	TowerCraneData.RunMode = SERVER_RunModeWrit30to44;
}
//---
/*
	函数名：void Server_Writ30to44(void)
	描述：	该函数为向A1模块写45~59地址寄存器
	08 10 00 2D 00 0F 1E 00 00 00 00 00 00 00
	00 00 00 02 BF 00 01 00 00 04 00 00 00 00
	00 00 00 00 00 00 00 00 00 09 EE
*/
void Server_Writ45to59(void)
{
	u8 buffer[40];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = TowerCraneData.Address;//地址
	buffer[cnt++] = 0x10;//写命令
	buffer[cnt++] = 0x00;//起始地址0x001E(30)，2个字节
	buffer[cnt++] = 0x2D;
	buffer[cnt++] = 0x00;//字数据长度0x000F(15),2个字节
	buffer[cnt++] = 0x0F;
	buffer[cnt++] = 0x1E;//字节数据长度0x1E(30),1个字节
	//45（单字）
	buffer[cnt++] = 0;	//无
	buffer[cnt++] = 0;
	//46~47（双字）
	buffer[cnt++] = 0;	//无
	buffer[cnt++] = 0;
	buffer[cnt++] = 0;
	buffer[cnt++] = 0;
	//48（单字）
	buffer[cnt++] = 0;	//无
	buffer[cnt++] = 0;
	//49（单字）
	buffer[cnt++] = 0;	//无
	buffer[cnt++] = 0;
	//50（单字）
	buffer[cnt++] = TowerCraneData.UpMinutes>>8;//开机时间：分钟高字节，2个字节
	buffer[cnt++] = TowerCraneData.UpMinutes;//开机时间：分钟低字节，2个字节
	//51（单字）
	buffer[cnt++] = TowerCraneData.UpSec>>8;//开机时间：秒高字节，2个字节
	buffer[cnt++] = TowerCraneData.UpSec;//开机时间：秒低字节，2个字节
	//52（单字）
	buffer[cnt++] = 0;	//无
	buffer[cnt++] = 0;
	//53（单字）
	buffer[cnt++] = TowerCraneData.Momental>>8;//力矩高字节，2个字节
	buffer[cnt++] = TowerCraneData.Momental;//力矩低字节，2个字节
	//54（单字）
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//55（单字）
	buffer[cnt++] = TowerCraneData.TaErrorCode>>8;//塔机错误代码高字节，2个字节
	buffer[cnt++] = TowerCraneData.TaErrorCode;//塔机错误代码低字节，2个字节
	//56（单字）
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//57（单字）
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//58（单字）
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//59（单字）
	buffer[cnt++] = TowerCraneData.BianErrorCode>>8;//变频器错误代码，2个字节
	buffer[cnt++] = TowerCraneData.BianErrorCode;//变频器错误代码，2个字节
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	USART_SetEvenVerify();
	for(crc=0;crc<cnt;crc++)
	{
		Server_SendBufferData(Server_UsartServerPort,Server_UsartServerBuffer,buffer[crc]);//发送数据
	}
	TowerCraneData.RunMode = SERVER_RunModeWrit45to59;
}
//---
/*
	函数名：void Server_Writ30to44(void)
	描述：	该函数为向A1模块写45~59地址寄存器
	08 10 00 3C 00 0F 1E 00 00 00 00 00 00 00
	00 00 00 00 00 00 00 00 00 00 00 00 00 00
	00 00 00 00 00 00 00 00 00 13 EA
*/
void Server_Writ60to74(void)
{
	u8 buffer[40];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = TowerCraneData.Address;//地址
	buffer[cnt++] = 0x10;//写命令
	buffer[cnt++] = 0x00;//起始地址0x001E(30)，2个字节
	buffer[cnt++] = 0x3C;
	buffer[cnt++] = 0x00;//字数据长度0x000F(15),2个字节
	buffer[cnt++] = 0x0F;
	buffer[cnt++] = 0x1E;//字节数据长度0x1E(30),1个字节
	//60(单字）
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//61（单字）
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//62（单字）
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//63（单字）
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//64（单字）
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//65（单字）
	buffer[cnt++] = TowerCraneData.Rotation>>8;//塔机回转角高字节，2个字节
	buffer[cnt++] = TowerCraneData.Rotation;//塔机回转角低字节，2个字节
	//66（单字）
	buffer[cnt++] = TowerCraneData.Range>>8;//塔机变幅高字节，2个字节
	buffer[cnt++] = TowerCraneData.Range;//塔机变幅低字节，2个字节
	//67~68（双字）
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//69（单字）
	buffer[cnt++] = TowerCraneData.Raising>>8;//塔机起升高字节，2个字节
	buffer[cnt++] = TowerCraneData.Raising;//塔机起升低字节，2个字节
	//70（单字）
	buffer[cnt++] = TowerCraneData.HookHeight>>8;//塔机吊钩高度高字节，2个字节
	buffer[cnt++] = TowerCraneData.HookHeight;//塔机吊钩高度低字节，2个字节
	//71~72（双字）
	buffer[cnt++] = TowerCraneData.PlcInState>>8;//PLC输入状态高字节，4个字节
	buffer[cnt++] = TowerCraneData.PlcInState;//PLC输入状态低字节，4个字节
	buffer[cnt++] = TowerCraneData.PlcInState>>24;//PLC输入状态高字节，4个字节
	buffer[cnt++] = TowerCraneData.PlcInState>>16;//PLC输入状态低字节，4个字节
	//73~74（双字）
	buffer[cnt++] = TowerCraneData.PlcOutState>>8;//PLC输出状态高字节，4个字节
	buffer[cnt++] = TowerCraneData.PlcOutState;//PLC输出状态低字节，4个字节
	buffer[cnt++] = TowerCraneData.PlcOutState>>24;//PLC输出状态高字节，4个字节
	buffer[cnt++] = TowerCraneData.PlcOutState>>16;//PLC输出状态低字节，4个字节
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	USART_SetEvenVerify();
	for(crc=0;crc<cnt;crc++)
	{
		Server_SendBufferData(Server_UsartServerPort,Server_UsartServerBuffer,buffer[crc]);//发送数据
	}
	TowerCraneData.RunMode = SERVER_RunModeWrit60to74;
}
//---
/*
	函数名：void Server_Writ30to44(void)
	描述：	该函数为向A1模块写45~59地址寄存器
	08 10 00 4B 00 0F 1E D0 13 0A 2C 00 00 00
	00 00 00 00 00 00 00 00 00 00 00 00 00 00
	00 00 00 00 00 00 00 00 00 22 04
*/
void Server_Writ75to76(void)
{
	u8 buffer[40];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = TowerCraneData.Address;//地址
	buffer[cnt++] = 0x10;//写命令
	buffer[cnt++] = 0x00;//起始地址0x001E(30)，2个字节
	buffer[cnt++] = 0x4B;
	buffer[cnt++] = 0x00;//字数据长度0x000F(15),2个字节
	buffer[cnt++] = 0x0F;
	buffer[cnt++] = 0x1E;//字节数据长度0x1E(30),1个字节
	//75~76（双字）
	buffer[cnt++] = TowerCraneData.LocalNumber>>8;//A1模块序列号，4个字节
	buffer[cnt++] = TowerCraneData.LocalNumber;//A1模块序列号，4个字节
	buffer[cnt++] = TowerCraneData.LocalNumber>>24;//A1模块序列号，4个字节
	buffer[cnt++] = TowerCraneData.LocalNumber>>16;//A1模块序列号，4个字节
	//77
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//78
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//79
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//80
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//81
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//82
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//83
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//84
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//85
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//86
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//87
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//88
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	//89
	buffer[cnt++] = 0;//无
	buffer[cnt++] = 0;
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	USART_SetEvenVerify();
	for(crc=0;crc<cnt;crc++)
	{
		Server_SendBufferData(Server_UsartServerPort,Server_UsartServerBuffer,buffer[crc]);//发送数据
	}
	TowerCraneData.RunMode = SERVER_RunModeWrit75to76;
}
//---
/*
	函数名：void Server_Read0to6(void)
	描述：	该函数为向A1模块写45~59地址寄存器
	08 03 00 00 00 07 04 91
*/
void Server_Read0to6(void)
{
	u8 buffer[10];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = TowerCraneData.Address;//地址
	buffer[cnt++] = 0x03;//读命令
	buffer[cnt++] = 0x00;//起始地址0x001E(30)，2个字节
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x00;//字数据长度0x000F(15),2个字节
	buffer[cnt++] = 0x07;
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	USART_SetEvenVerify();
	for(crc=0;crc<cnt;crc++)
	{
		Server_SendBufferData(Server_UsartServerPort,Server_UsartServerBuffer,buffer[crc]);//发送数据
	}
	TowerCraneData.RunMode = SERVER_RunModeRead0to6;
}
//---
//---
/*
	函数名：void Server_WritNewModule(void)
	描述：	该函数为向A1模块写30~44地址寄存器
	08 10 00 1E 00 0F 1E 00 03 00 00 00 00 00 
	00 00 00 00 00 00 00 00 00 00 00 00 00 00 
	00 00 00 00 00 00 00 00 00 5F 12
*/
void Server_WritNewModule(void)
{
	u8 buffer[200];
	u8 cnt=0;
	u16 crc;
	TwoBytes two;
	FourBytes four;
	buffer[cnt++] = 0x0C;//TowerCraneData.Address;//地址
	buffer[cnt++] = 0x10;//写命令
	buffer[cnt++] = 0x00;//起始地址0x0010(30)，2个字节
	buffer[cnt++] = 0x10;
	buffer[cnt++] = 0x00;//字数据长度0x0016(22),2个字节
	buffer[cnt++] = 0x16;
	buffer[cnt++] = 0x2C;//字节数据长度0x2C(44),1个字节
	//0x10（单字）塔机制造商（无符号）
	two.U16Data = Tower_GetBaseData()->Manufacturer;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x11（单字）塔机型号（无符号）
	two.U16Data = Tower_GetBaseData()->Version;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x12/Ox13（双字）塔机编号（无符号）
	four.U32Data = Tower_GetBaseData()->SerialNumber;
	buffer[cnt++] = four.U8Data[3];
	buffer[cnt++] = four.U8Data[2];
	buffer[cnt++] = four.U8Data[1];
	buffer[cnt++] = four.U8Data[0];
	//0x14(单字）塔机高度（无符号）
	two.U16Data = (u16)(Tower_GetBaseData()->Height/1000);
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x15(单字)塔机臂长（无符号）
	two.U16Data = (u16)(Tower_GetBaseData()->Length/1000);
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x16(单字)额定起升重量(无符号）
	two.U16Data = Tower_GetBaseData()->Length;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x17(单字)户外温度（有符号）
	if(DS18B20_GetData()->Map == 1)
		two.S16Data = (s16)(DS18B20_GetData()->Tem1 * 10);
	else
		two.S16Data = (s16)(DS18B20_GetData()->Tem2 * 10);
	two.U16Data = Tower_GetBaseData()->Length;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x18(单字)户外风速（无符号）
	two.U16Data = Wind_GetWindData()->Speed;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x19(单字)开机时间：时（无符号）
	two.U16Data = TowerCraneData.UpHour;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x1A(单字)开机时间：分（无符号）
	two.U16Data = TowerCraneData.UpMinutes;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x1B(单字)开机时间：秒（无符号）
	two.U16Data = TowerCraneData.UpSec;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x1C(单字)工作时间：时（无符号）
	two.U16Data = TowerCraneData.WorkingHour;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x1D(单字)工作时间：分（无符号）
	two.U16Data = TowerCraneData.WorkingMinutes;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x1E(单字)工作时间：秒（无符号）
	two.U16Data = TowerCraneData.WorkingSec;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x1F(单字)回转（无符号）
	two.U16Data = (u16)Tower_GetSlewingData()->Rotation;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x20(单字)变幅（无符号）
	two.U16Data = (u16)(Tower_GetTrolleyingData()->Range/10);
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x21(单字)起升（无符号）
	two.U16Data = (u16)(Tower_GetTrolleyingData()->Range/10);
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x22(单字)吊钩高度（无符号）
	two.U16Data = (u16)(Tower_GetHoistingData()->HookHeight/10);
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x23(单字)起吊重量（无符号）
	two.U16Data = (u16)((float)Thbss01_GetThbss01Data()->Hosting/1000);;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x24(单字)功率（无符号）
	two.U16Data = (u16)(Ohrd23_GetOhrd23Data()->Base.PZY/10);;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x25(单字)力矩（无符号）
	two.U16Data = (u16)(Tower_GetBaseData()->Momental*100);;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	Server_UsartModeSet();
	for(crc=0;crc<cnt;crc++)
	{
		Server_SendBufferData(Server_UsartServerPort,Server_UsartServerBuffer,buffer[crc]);//发送数据
	}
	TowerCraneData.RunMode = SERVER_RunModeWritNewModule;
}
void Server_UsartModeSet(void)
{
	MCH58_UartModeSet(Server_UsartServerPort,9600,'E','1');
}
//---
//-------------------------------------调试函数----------------------------------------
//---
/*
	函数名： void Server_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Server.c文件的ASCII调试文件
*/
void Server_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1://
		{//:000|55|001|1|100
			Server_InitTowerCraneData();//初始化TowerCraneData结构体变量数据
			break;
		}
		case 2:
		{
			Server_SetComEvenVerify();
			Server_Writ30to44();
			printf("向A1模块写30~44地址寄存器\r\n");
			break;
		}
		case 3:
		{
			Server_SetComEvenVerify();
			Server_Writ45to59();
			printf("向A1模块写45~59地址寄存器\r\n");
			break;
		}
		case 4:
		{
			Server_SetComEvenVerify();
			Server_Writ60to74();
			printf("向A1模块写60~74地址寄存器\r\n");
			break;
		}
		case 5:
		{
			Server_SetComEvenVerify();
			Server_Writ75to76();
			printf("向A1模块写75~76地址寄存器\r\n");
			break;
		}
		case 6:
		{
			Server_SetComEvenVerify();
			Server_Read0to6();
			printf("向A1模块读0~6地址寄存器\r\n");
			break;
		}
		//-------------------------------------------------------------------------------
		//修改并保存相关数据,后期需要加入保存到EEPROM中
		case 100://修改塔机制造商代码（00000~65535）
		{//:000|55|100|12345
			TowerCraneData.Manufacturer = Calculate_CharToU16(p+4,5);
			printf("塔机制造商%u\r\n",TowerCraneData.PlcOutState);
			break;
		}
		case 101://修改塔机型号（00000~65535）
		{//:000|55|100|12345
			TowerCraneData.PlcOutState = Calculate_CharToU16(p+4,5);
			printf("塔机制造商%u\r\n",TowerCraneData.Version);
			break;
		}
		case 102://修改塔机型号（0000000000~4294967296）
		{//:000|55|100|12345
			TowerCraneData.SerialNumber = Calculate_CharToU16(p+4,10);
			printf("塔机制造商%u\r\n",TowerCraneData.Version);
			break;
		}
		//-------------------------------------------------------------------------------
		//修改当前塔机相关数据，不保存
		case 200://修改塔机型号（0000000000~4294967296）
		{//:000|55|100|12345
			TowerCraneData.SerialNumber = Calculate_CharToU16(p+4,10);
			printf("塔机制造商%u\r\n",TowerCraneData.Version);
			break;
		}
		//-------------------------------------------------------------------------------
		//读取当前塔机相关数据
		case 300://读取当前塔机型号
		{//:000|55|300
			printf("塔机制造商：%u\r\n",TowerCraneData.Version);
			break;
		}
		case 301://读取当前塔机型号
		{//:000|55|301
			printf("塔机型号：%u\r\n",TowerCraneData.Version);
			break;
		}
		case 302://读取当前塔机编号
		{//:000|55|302
			printf("塔机编号：%u\r\n",TowerCraneData.SerialNumber);
			break;
		}
		case 303://读取当前塔机高度
		{//:000|55|303
			printf("塔机高度：%u\r\n",TowerCraneData.Height);
			break;
		}
		case 304://读取当前塔机高度
		{//:000|55|304
			printf("塔臂长度：%u\r\n",TowerCraneData.Length);
			break;
		}
		case 305://读取额定载重
		{//:000|55|305
			printf("塔机额定载重：%u\r\n",TowerCraneData.RatedLoad);
			break;
		}
		case 306://读取当前户外温度
		{//:000|55|306
			printf("当前户外温度：%d\r\n",TowerCraneData.OutdoorsTem);
			break;
		}
		case 307://读取当前户外风速
		{//:000|55|307
			printf("当前户外风速：%u\r\n",TowerCraneData.WindSpeed);
			break;
		}
		case 308://读取当前开机时间
		{//:000|55|308
			printf("当前开机时间：小时：%u\r\n",TowerCraneData.UpHour);
			printf("当前开机时间：分钟：%u\r\n",TowerCraneData.UpHour);
			printf("当前开机时间：	 秒：%u\r\n",TowerCraneData.UpHour);
			break;
		}
		case 309://读取当前工作时间
		{//:000|55|309
			printf("当前工作时间：小时：%u\r\n",TowerCraneData.WorkingHour);
			printf("当前工作时间：分钟：%u\r\n",TowerCraneData.WorkingHour);
			printf("当前工作时间：	 秒：%u\r\n",TowerCraneData.WorkingHour);
			break;
		}
		case 310://读取当前回转角度
		{//:000|55|310
			printf("当前塔机回转角度：%u\r\n",TowerCraneData.Rotation);
			break;
		}
		case 311://读取当前塔机变幅
		{//:000|55|311
			printf("当前塔机变幅：%ucm\r\n",TowerCraneData.Range);
			break;
		}
		case 312://读取当前塔机起升
		{//:000|55|312
			printf("当前塔机起升：%ucm\r\n",TowerCraneData.Raising);
			break;
		}
		case 313://读取当前塔机吊钩高度
		{//:000|55|313
			printf("当前塔机吊钩高度：%ucm\r\n",TowerCraneData.HookHeight);
			break;
		}
		case 314://读取当前塔机起吊重量
		{//:000|55|314
			printf("当前塔机起吊重量：%u\r\n",TowerCraneData.Hoisting);
			break;
		}
		case 315://读取当前塔机当前功率
		{//:000|55|315
			printf("当前塔机功率：%u\r\n",TowerCraneData.CurrentPower);
			break;
		}
		case 316://读取当前塔机力矩
		{//:000|55|316
			printf("当前塔机力矩：%u\r\n",TowerCraneData.Momental);
			break;
		}
		case 317://读取当前塔机错误代码
		{//:000|55|316
			printf("当前塔机错误代码：%u\r\n",TowerCraneData.TaErrorCode);
			break;
		}
		case 318://读取当前变频器错误代码
		{//:000|55|318
			printf("当前变频器错误代码：%u\r\n",TowerCraneData.BianErrorCode);
			break;
		}
		case 319://读取当前PLC输入状态
		{//:000|55|319
			printf("当前PLC输入状态：%u\r\n",TowerCraneData.PlcInState);
			break;
		}
		case 320://读取当前PLC输出状态
		{//:000|55|320
			printf("当前PLC输出状态：%u\r\n",TowerCraneData.PlcOutState);
			break;
		}
		//-------------------------------------------------------------------------------
		
		
		
		default:
		{
			printf("Server.c接收到无效调试指令%u!\r\n",cmd);
			break;
		}
	}
}
//---
/*
	函数名： void Server_DebugRTU((u8 *p)
	描述：		该函数为Server.c文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*/
void Server_DebugRTU(u8 *p,u8 len)
{
	switch(TowerCraneData.RunMode)//查看是哪个功能码
	{
		case SERVER_RunModeWrit30to44:
		{
//			printf("写30~44完毕\r\n");
			break;
		}
		case SERVER_RunModeWrit45to59:
		{
//			printf("写45~59完毕\r\n");
			break;
		}
		case SERVER_RunModeWrit60to74:
		{
//			printf("写60~74完毕\r\n");
			break;
		}
		case SERVER_RunModeWrit75to76:
		{
//			printf("写75~76完毕\r\n");
			break;
		}
		case SERVER_RunModeRead0to6:
		{
//			printf("读0~6完毕\r\n");
			break;
		}
		case SERVER_RunModeWritNewModule:
		{
			break;
		}
	}
	TowerCraneData.RunMode = SERVER_RunModeNone;
}
//---










