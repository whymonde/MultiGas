/*
	OHRD23库函数
*/

#ifndef __OHRD23_H
#define __OHRD23_H

#include "stm32f10x.h"

//命令码
#define OHRD23_CmdReadWords						0x03		//读取寄存器数个字命令
#define OHRD23_CmdWriteOneWord				0x06		//向目标寄存器写入一个字命令
#define OHRD23_CmdWriteWords					0x10		//向目标寄存器写入多个字命令

//地址
//基本电量参数地址
#define OHRD23_AdrBase								0x0100	//基本电量参数基址
//电能参数地址
#define OHRD23_AdrEnergy							0x0600	//电能参数基址
//配置参数地址
#define OHRD23_AdrConf								0x0903	//配置参数基址
#define OHRD23_AdrConfVT							0x0903	//电压倍率
#define OHRD23_AdrConfIT							0x0904	//电流倍率
#define OHRD23_AdrConfLM							0x0905	//接线方式
#define OHRD23_AdrConfADR							0x0906	//通讯地址
#define OHRD23_AdrConfBR							0x0907	//通讯波特率
//存储地址
#define OHRD23_AdrStoreBase						W25XXX_AdrOHRD23Base
#define OHRD23_AdrConfBase						0
#define OHRD23_AdrStoreVT							OHRD23_AdrConfBase
#define OHRD23_AdrStoreIT							OHRD23_AdrStoreVT+2
#define OHRD23_AdrStoreLM							OHRD23_AdrStoreIT+2
#define OHRD23_AdrStoreADR						OHRD23_AdrStoreLM+1
#define OHRD23_AdrStoreBR							OHRD23_AdrStoreADR+1
#define OHRD23_AdrStoreEnd						OHRD23_AdrStoreBR+2


//运行模式

//硬件宏定义
//---------------------------------------------------------------
//硬件端口宏定义
#define Ohrd23_SendBufferData	 				USARTx_StoreBufferData			//RTU数据发送函数
//采集端口(RS485)
#define Ohrd23_UsartPort							USART3											//对应串口输出端口
#define Ohrd23_UsartBuffer()					Get_USART3Buffer()					//对应串口结构体
#define Ohrd23_UartModeSet						USART_ModeSet
#define Ohrd23_SetComNoneVerify				USART_SetNoneVerify				//设置端口号为无校验模式

//存储地址
//电量参数存储地址
//#define Ohrd23_AdrStoreBase					W25XXX_AdrOHRD23Base
//#define Ohrd23_AdrStoreDYBL					0		//电压倍率
//#define Ohrd23_AdrStoreDLBL					Ohrd23_AdrStoreDYBL+2//电流倍率
//#define Ohrd23_AdrStoreJXFS					Ohrd23_AdrStoreDLBL+2//接线方式
//#define Ohrd23_AdrStoreTXDZ					Ohrd23_AdrStoreJXFS+1//通讯地址
//#define Ohrd23_AdrStoreTXBTL				Ohrd23_AdrStoreTXDZ+1//通讯波特率
//#define Ohrd23_AdrStoreEND					Ohrd23_AdrStoreTXBTL+2//电量参数末端




//OHRD23电量仪基本电量参数数据结构体
typedef struct
{
	s32 VA;//A相相电压
	s32 VB;//B相相电压
	s32 VC;//C相相电压
	s32 VAB;//A-B线电压
	s32 VBC;//B-C线电压
	s32 VCA;//C-A线电压
	s32 IA;//A相电流
	s32 IB;//B相电流
	s32 IC;//C相电流	
	float PAY;//A相有功功率
	float PBY;//B相有功功率
	float PCY;//C相有功功率
	float PZY;//总相有功功率
	float PAW;//A相无功功率
	float PBW;//B相无功功率
	float PCW;//C相无功功率
	float PZW;//总相无功功率
	float PAS;//A相视在功率
	float PBS;//B相视在功率
	float PCS;//C相视在功率
	float PZS;//总相视在功率
	s32 FA;//A相功率因数
	s32 FB;	//B相功率因数
	s32 FC;	//C相功率因数
	s32 FZ;	//C相功率因数
	s32 Frequency;//频率
}OHRD23_BaseStructure;
//OHRD23电量仪电能参数数据结构体
typedef struct
{
	s32 WHP;//正向有功电能
	s32 WHN;//反向有功电能
	s32 VARHP;//正向无功电能
	s32 VARHN;//反向无功电能
	u32 SWHP;//总有功电能
	u32 SWHN;//总无功电能
}OHRD23_EnergyStructure;
//OHRD23电量仪配置参数数据结构体
typedef struct
{
	s16 VT;//电压倍率
	s16 IT;//电流倍率
	s16 LM;//接线模式
	s16 ADR;//通讯地址
	s16 BR;//通讯波特率
}OHRD23_ConfigurationStructure;

/*
	OHRD23数据结构体
*/
typedef struct
{
	u8 Flag;		//B0=1代表与OHRD23通讯故障,B1=1超时未接收到数据
	u8 RunMode;	//运行模式，RunMode=通讯处理完成
	u8 CollectMode;	//采集模式
	u8 CollectCommand;//采集命令
	u8 ErrorCnt;	//通讯错误累计
	u16 Timer;	//定时器
	OHRD23_BaseStructure Base;
	OHRD23_EnergyStructure Energy;
	OHRD23_ConfigurationStructure Conf;
}Ohrd23_DataStructure;
//---




//----------------------------------------------------
void Ohrd23_Main(void);
void Ohrd23_Collect(void);
void Ohrd23_Init(void);
void Ohrd23_Timer(void);
Ohrd23_DataStructure* Ohrd23_GetOhrd23Data(void);
//读取数据
void Ohrd23_ReadBase(void);
void Ohrd23_ReadEnergy(void);
void Ohrd23_ReadConf(void);
//参数配置
void Ohrd23_SetVT(s16 rate);
void Ohrd23_SetIT(s16 rate);
void Ohrd23_SetLM(u8 mode);
void Ohrd23_SetAddress(u8 address);
void Ohrd23_SetBaudRate(u8 baudrate);
void Ohrd23_SetConfiguration(OHRD23_ConfigurationStructure *p);
//通讯
void Ohrd23_ReadData(u16 address,u16 amount);
void Ohrd23_WriteOneWord(u16 address,u16 data);
void Ohrd23_WriteMultiWord(u16 address,u16 *p,u16 len);
//数据换算
void Ohrd23_BaseChange(u8 *p,OHRD23_BaseStructure* Base);
void Ohrd23_EnergyChange(u8 *p,OHRD23_EnergyStructure* Energy);
void Ohrd23_ConfigurationChange(u8 *p,OHRD23_ConfigurationStructure *conf);
//硬件配置
void Ohrd23_SearchOhrd23(void);
void Ohrd23_UsartModeSet(void);

void Ohrd23_Operation(u16 mode,u8 *p,u8 len);
//存储相关
void Ohrd23_ReadStoreData(u8 address,u8 *p,u8 len);
void Ohrd23_StoreData(u8 address,u8 *p,u8 len);
void Ohrd23_RestoreFactorySetting(void);
//通讯相关
void Ohrd23_DebugASCII(u8 *p,u8 len);
void Ohrd23_DebugRTU(u8 *p,u8 len);
void Ohrd23_Receive(u8 *p,u8 len);





#endif










