/*
	battery库函数
*/

#ifndef __BATTERY_H
#define __BATTERY_H

#include "stm32f4xx.h"


#define BATTERY_DATALEN        	100		//电源电压采集数据处理缓冲区
#define BATTERY_VPT_ON					(float)19.2	//电源电压启动阈值，大于该值时，允许采集处理，电压小于该值时认为电池容量为0V
#define BATTERY_VPT_OFF					(float)18.9	//电源电压关闭阈值，小于该电压时，禁止采集处理
#define BATTERY_Full100					29		//电压大于29V时表示当前电池充满电量
#define BATTERY_Vref           (float) 3.0      //AD 参考电压
#define BATTERY_OP              11              //电路板电阻分压倍数
#define LowPOWER                20.0f           //电量过低的电压比较20.0f
	
//电池变量结构体
typedef struct
{
	u16 Data[BATTERY_DATALEN];
	u8 Cnt;				//计数器
	float CollectV;	//采集电压
	float ActualV;	//实际电压，由采集电压乘以倍率得来
	float Rate;			//电压倍率
	u8 Ratio;				//电池容量百分比，取值为0~100
}Battery_BatteryStructure;
//电源结构体
typedef struct
{
	u8 Mode;							//Mode=0电压<BATTERY_VPT_OFF；Mode=1,电压>BATTERY_VPT_ON
	u8 LostPowerFlag;     //掉电标准
	u8 CurChanl;          //当前转换通道
	Battery_BatteryStructure Battery; //电池电压  pc1  =ad11
	Battery_BatteryStructure LostPV;  //掉电电压  pc0  =ad10
	Battery_BatteryStructure Current;  //电池用电电流PC3=AD13
}Battery_DataStructure;


//----------------------------------------------------
void Battery_Main(void);
void Battery_Init(void);
void Battery_Timer(void);
Battery_DataStructure* Battery_GetData(void);


void Battery_DataInit(void);
void Battery_ReadFlashData(void);
void Battery_InitADC(void);
void Battery_GetVoltage(void);
void Battery_DealWith(void);

void Battery_DebugASCII(u8 *p,u8 len);
//void Battery_DebugRTU(u8 *p,u8 len);






#endif










