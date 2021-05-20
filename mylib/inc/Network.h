/*
	screen库函数
*/

#ifndef __NETWORK_H
#define __NETWORK_H

#include "stm32f4xx.h"

//输出
#define POWER_4G_OUT(a)\
					if (a==0)\
						GPIO_ResetBits(POWER_4G_PORT,POWER_4G_PIN);\
					else if (a==1)\
						GPIO_SetBits(POWER_4G_PORT,POWER_4G_PIN);
#define NETLED_4G_IN 	GPIO_ReadInputDataBit(NETLED_4G_PORT,NETLED_4G_PIN)			
					

#define NETWORK_SandDataHanderH 0xaa   //设置发送帧的开始字节为:0xaa,0x55
#define NETWORK_SandDataHanderL 0x55
#define Network_ReceivLeng  50
#define USART_REC_LEN  50
//常量相关宏定义
//#define DS18B20_FACTORY					0XAAAA	//回复出厂设置码

//存储地址
//网络数据参数存储地址
#define NETWORK_AdrStoreBase					FLASHIS_AdrBaseNetwork
#define NETWORK_AdrIPAdress						0	//IP地址，2个字，4个字节
#define NETWORK_AdrPortNumber					NETWORK_AdrIPAdress+2//端口号，1个字
#define NETWORK_AdrTimeAndOnOff				NETWORK_AdrPortNumber+1//发送间断+启停
#define NETWORK_AdrFactory						NETWORK_AdrTimeAndOnOff+1//出厂设置标志位
#define NETWORK_AdrEND								NETWORK_AdrFactory+1//出厂标志

typedef struct
{ 
	u8 flag;   //有数据接收标准
	u8 DataArr[Network_ReceivLeng]; //接收数据数组
}Network_ReceivDataStructure;

//数据帧结构体
typedef struct
{ u8 HanderH;   //开始高字节
	u8 HanderL;   //开始低字节
	u8 Length;    //长度
	u8 DataArr[255];  // 内容，包含数据，效验
}Network_SendDataStructure;

/*
	Network结构体
*/
typedef struct
{
	u8 Flag;					 //标志位
	u8 HandOKflag;     //0=还没检测完,1=握手成功,2=握手失败
  u8 SEND_OK;//发送成功标识
	u8 Temp;
  u8 USART_RX_BUF[USART_REC_LEN];//
	u16 USART_RX_STA;         		//
  u16 USART_RX_CNT;
	u8 AtMode;          //=0:常工作模式;1=AT 命令模式
	u8 RunMode;					//运行模式,0:待机扫描状态
	u8 NextMode;        //下一要运行的状态
	u8 SendCnt;         //发送计数器,3次
	u16 Timer;					//定时器
	u16 Timer_50ms;     //50毫秒定时器
	u16 Timer_3s;       //3秒定时器
	u8 Hcnt;            //高电平计算器
	u8 NetworkStatus;   //1 链接上;0 断开,由专门定时程序扫描检测网络状态口，这标志也提供屏幕程序显示网络标志用
	u8 LinkErrFlag;     //链接错误标记  =0:链接正常;  =1:超时 =2:设置成功
	Network_SendDataStructure  Network_SendData;  //定义发送数据帧
	Network_ReceivDataStructure Network_ReceivData;//定义接收数据的数据结构
}Network_DataStructure;


//----------------------------------------------------
void Network_Main(void);
void Network_Timer(void);
void Network_Init(void);
Network_DataStructure* Network_GetData(void);
void Network_SendNByteData(u8 *p,u8 Nbyte);
void Network_ReadData(u8 address,u8 *p,u8 len);
void Network_StoreData(u8 address,u8 *p,u8 len);
void Network_DebugASCII(u8 *p,u8 len);
void Network_SendOneFrameData(void);
void Network_Test(void);
#endif










