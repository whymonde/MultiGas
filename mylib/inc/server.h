/*
	server库函数
*/

#ifndef __SERVER_H
#define __SERVER_H

#include "stm32f10x.h"
//硬件宏定义
//#define SERVERPWR_APP					RCC_APB2Periph_GPIOA	//ServerPWR电源管脚的时钟桥
//#define SERVERPWR_PORT				GPIOA									//ServerPWR电源管脚端口
//#define SERVERPWR_PIN					GPIO_Pin_10						//ServerPWR电源管脚
//#define SERVERPWR_OFF					GPIO_SetBits(SERVERPWR_PORT, SERVERPWR_PIN)		//ServerPWR电源关闭
//#define SERVERPWR_OPEN				GPIO_ResetBits(SERVERPWR_PORT, SERVERPWR_PIN)	//ServerPWR电源打开

//数据宏定义
#define SERVER_DefaultAddress					0x08		//A1通讯模块默认地址

//运行模式
#define SERVER_RunModeNone						0				//什么模式也没有运行
#define SERVER_RunModeWrit30to44			0x10		//运行在写30~44模式
#define SERVER_RunModeWrit45to59			0x11		//运行在写45~59模式
#define SERVER_RunModeWrit60to74			0x12		//运行在写60~74模式
#define SERVER_RunModeWrit75to76			0x13		//运行在写75~76模式
#define SERVER_RunModeRead0to6				0x14		//运行在读0~6模式
#define SERVER_RunModeWritNewModule		0x15		//写新模块

//---------------------------------------------------------------
//硬件端口宏定义
#define Server_SendBufferData	 			USARTx_StoreBufferData			//RTU数据发送函数
//A1端口(RS485)
#define Server_UsartServerPort			USART3											//对应串口输出端口
#define Server_UsartServerBuffer		Get_USART3Buffer()					//对应串口结构体
#define Server_SetComEvenVerify			USART_SetEvenVerify					//设置端口号为偶校验模式


/*
	塔机向服务器发送数据结构体结构体
*/
typedef struct 
{
	u8 Flag;						//运行状态标志位，b0=1(系统开机），b1=1(系统工作)
	u8 Address;					//A1通讯模块地址
	u8 RunMode;					//运行模式
	u32 LocalNumber;		//A1模块序列号
	u16 Timer;					//系统累计运行时间，单位秒
	u32 SysTime;
	u16 Manufacturer;		//塔机制造商（塔机制造商代码，永茂建机的代码为22861）
	u16 Version;				//塔机型号（塔机型号，例如STT553型塔机，则为553）
	u32 SerialNumber;		//塔机编号（塔机序列号，序列号以32位无符号整数表示，最大支持10位数字）
	u16	Height;					//塔机高度（塔机当前安装高度，单位为米）
	u16 Length;					//塔臂长度（塔机当前臂长，单位为米）
	u16 RatedLoad;			//额定载重(塔机的额定起升重量，单位为吨)
	s16 OutdoorsTem;		//户外温度(塔机驾驶室附近气温，单位为0.1摄氏度，即假设户外温度为24.5度，则为245)
	u16 WindSpeed;			//风速（塔机所在地的当前风速，单位为0.1米/秒）
	u16 UpHour;					//开机时间：小时（塔机自开机后经过的时间的小时部分，此时间须在关机后清零）
	u16 UpMinutes;			//开机时间：分钟
	u16 UpSec;					//开机时间：秒
	u16 WorkingHour;		//工作时间：小时（塔机自开机后有效工作的时间的小时部分，此时间须在关机后清零）
	u16 WorkingMinutes;	//工作时间：分钟
	u16 WorkingSec;			//工作时间：秒
	u16 Rotation;				//回转角度（塔机回转数据，单位为度，取值范围0～359，塔机指向正北时输出为0度）
	u16 Range;					//变幅（塔机变幅数据，单位为0.01米）
	u16 Raising;				//起升（塔机起升数据，单位为0.01米）
	u16 HookHeight;			//吊钩高度（塔机目前吊钩高度，单位为0.01米，以吊钩位于地面为0米）
	u16 Hoisting;				//当前起吊重量（塔机当前吊起的重量，单位为0.01吨，即假设重量为2583千克，则为258）
	u16 CurrentPower;		//当前功率（当前塔机的总工作功率，单位为0.1千瓦）
	u16 Momental;				//力矩（当前塔机起升力矩达到最大力矩的百分比，取值范围为0～100）
	u16 TaErrorCode; 		//塔机错误代码（塔机系统错误代码，具体内容后续确认）
	u16 BianErrorCode;	//变频器错误代码（塔机变频器错误代码，具体内容后续确认）
	u32 PlcInState;			//PLC输入状态（PLC输入点的信号状态，最低位Bit对应X0，依此类推。0为无信号，1为有信号）
	u32 PlcOutState;		//PLC输出状态（PLC输出点的信号状态，同上）
}Sever_TowerCraneStructure;



//库函数
void Server_Init(void);//该函数为server.c初始化函数
void Server_Timer(void);//该函数为server.c文件的时钟函数，时钟单位为1ms

void Server_PwrIoInt(void);//A1电源初始化端口

void Server_InitTowerCraneData(void);//对TowerCraneData结构体变量进行初始化
Sever_TowerCraneStructure *Server_GetTowerCraneData(void);//该函数为用于获取Server.c结构体变量TowerCraneData的地址

void Server_Writ30to44(void);
void Server_Writ45to59(void);
void Server_Writ60to74(void);
void Server_Writ75to76(void);
void Server_Read0to6(void);
void Server_WritNewModule(void);
void Server_UsartModeSet(void);
//调试、通讯函数
void Server_DebugASCII(u8 *p,u8 len);//该函数为Server.c文件的ASCII调试文件
void Server_DebugRTU(u8 *p,u8 len);//该函数为Server.c文件的RTU调试文件


#endif




















