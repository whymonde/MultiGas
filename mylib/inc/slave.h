/*
	slave.c库函数
*/

#ifndef __SLAVE_H
#define __SLAVE_H

#include "stm32f4xx.h"
#define SCREEN_MAINTAIN //两种校正方式
#define SLAVE_HJtoBWX							//定义后，环境温度传感器与保温箱温度传感器互换
//宏定义
#define SLAVE_FILTER_DEPTH					16	//滤波循环队列深度
#define SLAVE_MIDDLEFILTER_DEPTH            4   //中值滤波深度
#define SLAVE_ColFAULT							1000000	//采集数据故障
#define SLAVE_BaseTem								50		//温度基值，单位℃
#define SLAVE_BaseCPress						500//0		//差压的基值，单位kPa
#define SLAVE_BaseGPress						0//100		//差压的基值，单位kPa
#define SLAVE_BaseDPress						0		//大气压基值，单位kPa
#define SLAVE_BaseGas                   000             //气体基值
#define SLAVE_SensorRefreshTime			5			//传感器数据刷新周期，单位5ms					
#define SLAVE_DataMax               18  //定义多小路采样数据0-16(共17个),17备用没用

//定义PT100采样环境参数  20200525
#define WdMax   650
#define WdMin   -80
#define AdMin   525
#define AdMax   2927
#define PT100_K               (float)(WdMax-WdMin)/(AdMax-AdMin)
#define PT100_B               WdMin
//硬件端口映射



//下面共17个浮点数
#define SLAVE_MAP_DQPress		2	//大气压映射地址(5805)
#define SLAVE_MAP_ACPress		3	//A路差压（流压）映射地址  工况烟气路
#define SLAVE_MAP_AGPress		6	//A路差压（计压）映射地址
#define SLAVE_MAP_BCPress		4	//B路差压（流压）映射地址  烟尘路
#define SLAVE_MAP_BGPress		7	//B路差压（计压）映射地址
#define SLAVE_MAP_CCPress		5	//C路差压（流压）映射地址  烟气路

#define SLAVE_MAP_HWTem			1//环境温度映射地址(5805)
#define SLAVE_MAP_YWTem         8 //烟温   测试工况时候用
#define SLAVE_MAP_GQTem         8 //干球   测试含湿量候用
#define SLAVE_MAP_SQTem         9 //湿球
#define SLAVE_MAP_JWTem			0//（计温）DS18B20，测试烟尘时候用

#define SLAVE_MAP_O2Concent     16//氧气浓度映射地址
#define SLAVE_MAP_SO2Concent    11//氧气浓度映射地址
#define SLAVE_MAP_NOConcent     15//氧气浓度映射地址
#define SLAVE_MAP_NO2Concent    14//氧气浓度映射地址
#define SLAVE_MAP_COConcent     13//氧气浓度映射地址
#define SLAVE_MAP_H2SConcent    12//氧气浓度映射地址
#define SLAVE_MAP_CO2Concent    10//氧气浓度映射地址

#define SLAVE_MAP_E28DY         18  //E28过来动压映射地址
#define SLAVE_MAP_E28JY         19  //E28过来静压映射地址
#define SLAVE_MAP_E28YW         20  //E28过来烟温映射地址


//硬件端口宏定义
#define Slave_SendBufferData	 		USARTx_StoreBufferData			//RTU数据发送函数
#define Slave_UsartPort						USART2											//对应串口输出端口
#define Slave_UsartBuffer					Get_USART2Buffer()					//对应串口结构体


//滤波结构体
typedef struct
{
	u8 Flag;			//B0=1表示已经存满整个缓冲区，B1=1表示已经存满整个中值滤波缓冲区
	u8 Cnt;				//计数器
	float Sum;		//和
	float Buffer[SLAVE_FILTER_DEPTH];//缓存区
	float BufferMiddle[SLAVE_MIDDLEFILTER_DEPTH];           //中值滤波缓冲区
	float OldMiddle;                //上一次中值滤波值
	u8 CntMiddle;                   //中值滤波计数
}Slave_FilterStructure;

//温度(T(℃))压力(P(kPa))通讯采样结构体
typedef struct
{
	float Raw;										//原始值(平均值)，没有校准的
	Slave_FilterStructure Filter;	//滤波器
}Slave_TPRawStructure;
//温度(T(℃))压力(P(kPa))结构体
typedef struct
{
	float Value;		//平均
	float Value0,Value1,Value2;     //主要用于气体对应倍率计算值 Value0=中浓度测量值;Value1=低浓度测量值;Value2=高浓度测量值
	float K;				//计算温度函数K值 中浓度倍率
	float K1,K2;    //K1,K2倍率，主要用于气体K1=低浓度倍率;K2=高浓度倍率;
	float B;				//计算温度函数B值
	union
	{
		u8 All;
		struct
		{
			u8 : 2;
			u8 RawMode : 3;		//0表示无，1,启动归零模式处理
			u8 ActualMode : 3;	//0表示无操作，1表示归零模式，2表示归零模式
		}Bits;
	}Mode;
	u16 Cnt;		//用于计算零点用，计数器
	u16	Timer;		//归零定时器，单位ms
	float Sum;		//用于计算零点用，总值
	float Max;		//零点最大值
	float Min;		//零点最小值
	float OldRaw;	//Raw旧值
}Slave_TPActualStructure;

//通讯结构体
typedef struct
{
	u8 Error;
	u8 ErrorCnt;
	u8 RunMode;
	u8 NewMode;
	u16 Timer;					//定时器
}Slave_CommStructure;

//电机结构体
typedef struct
{   char rame;
	u8 OnOff;				//状态
	u8 RunMode;
	u16 Timer;
	u16 Ratio;						//占空比
	PID_DataStructure Pid;//PID数据
}Slave_MotorStructure;

//E28-2.4G无线结构体,界面操作要用到
typedef struct
{   
	u8 STATE,STATERD24G;	//状态,上位机下发数据的状态,STATE.B0=1:设置;   STATE.B0=0,无命令;  STATERD24G.B0=1连接上;STATERD24G.B0=0断开;
	                        //STATERD24G.B1=1设置成功;STATERD24G.B1=0设置失败;
	u8 CHset,CHRD24G;      //信道设置值,信道从副板读出的值
	u16 ADRRset,ADRRRD24G; //地址设置值,地址从副板读出的值
}Slave_E28Structure;

//STM8结构体 2020-05-22 hjl add
typedef struct
{
	u8 Data;					//拆状态
	u8 rw_ctrl;					//读写控制命令
}STM8_DataStructure;

//结构体
typedef struct
{
	u8 RunMode;					//运行模式
	u16 Timer;					//定时器
	u8 GetTimer;				//主板温度获取定时器
	Slave_CommStructure Comm;				//通讯结构体
	Slave_TPRawStructure Data[SLAVE_DataMax+3];	//从机返回数据结构体8组数据(采集数据体) //20200602 ，加多无线采样的三路，动压，静压，烟温

	Slave_TPActualStructure DQPress;//大气压，单位kPa
	Slave_TPActualStructure ACPress;//A路差压（流压），单位kPa
	Slave_TPActualStructure AGPress;//a路差压（计压），单位kPa
	Slave_TPActualStructure BCPress;//B路差压（流压），单位kPa
	Slave_TPActualStructure BGPress;//b路差压（计压），单位kPa
	Slave_TPActualStructure CCPress;//C路差压（流压），单位kPa
	
	Slave_TPActualStructure JWTem;	//管温（计温）D18B20
	Slave_TPActualStructure HWTem;	//环境温度，单位℃5805
	Slave_TPActualStructure YWTem;  //烟温        
	Slave_TPActualStructure GQTem;  //干球
	Slave_TPActualStructure SQTem;  //湿球
	
	Slave_TPActualStructure O2Concent; //O2浓度
	Slave_TPActualStructure SO2Concent;//SO2浓度
	Slave_TPActualStructure NOConcent; //NO浓度
	Slave_TPActualStructure NO2Concent; //NO2浓度

	Slave_TPActualStructure COConcent; //CO浓度	
	Slave_TPActualStructure H2SConcent; //H2S浓度	
	Slave_TPActualStructure CO2Concent; //CO2浓度
	Slave_TPActualStructure NOxConcent; //NOx浓度
		
	Slave_TPActualStructure BeforO2Concent; //O2矫正前浓度
	Slave_TPActualStructure BeforSO2Concent;//SO2浓度
	Slave_TPActualStructure BeforNOConcent; //NO浓度
	Slave_TPActualStructure BeforNO2Concent; //NO2浓度
	
	Slave_TPActualStructure BeforCOConcent; //CO浓度	
	Slave_TPActualStructure BeforH2SConcent; //H2S浓度	
	Slave_TPActualStructure BeforCO2Concent; //CO2浓度
	Slave_TPActualStructure BeforNOxConcent; //NOx浓度
 	
	Slave_MotorStructure WaterMotor;  //水泵
	Slave_MotorStructure SlvdMotor;  //电磁阀
	Slave_MotorStructure GasMotor;  //气泵
	Slave_MotorStructure DustMotor; //尘泵
	STM8_DataStructure Stm8Data;	//2020-05-22 hjl add stm8数据
	Slave_E28Structure  E28;
	
}Slave_DataStructure;


//----------------------------------------------------
void Slave_Main(void);
void Slave_Timer(void);
void Slave_Init(void);
Slave_DataStructure* Slave_GetData(void);

void Slave_InitComm(void);
void Slave_InitRawData(void);
void Slave_InitActualData(Slave_TPActualStructure *p);
void Slave_ReadFlashData(void);

void Slave_ReadFlashZOData(Slave_TPActualStructure *P,u32 adrr,float offset);  //读取零点数据到数据结构B

void Slave_ReadFlashBLData(Slave_TPActualStructure *P,u32 adrr);//读取倍率数据到数据结构温度，气体20200524
void Slave_ReadFlashBLDatak1(Slave_TPActualStructure *P,u32 adrr);//读取倍率数据
void Slave_ReadFlashBLDatak2(Slave_TPActualStructure *P,u32 adrr);//读取倍率数据

float Slave_ReadTPFlashData(u32 addr); //读公共变量的数据

void Slave_GetSensorData(void);

void Slave_DataDealWith(Slave_TPRawStructure *p,float value,u8 flag);
float Slave_DataFilter(Slave_FilterStructure *p,float value);
float Slave_DataMiddleFilter(Slave_FilterStructure *p,float value);//中值滤波
void Slave_DataMaping(Slave_TPActualStructure *p,float value);

void Slave_Zero(Slave_TPActualStructure *p,u32 Addr,float offset); //并保存到flash
void Slave_Zero_QT(void);  //所有压力值调零  对外接口
void Slave_Zero_Tem(void);  //所有温度调零 对外接口
void Slave_Zero_Press(void);  //所有压力值调零 对外接口


//---------电机相关---------
void Slave_MotorRun(Slave_MotorStructure *p,float pv);
u8 Slave_StartMotor(Slave_MotorStructure *P,u16 ratio);
u8 Slave_StopMotor(Slave_MotorStructure *P);
u8 Slave_StartPump(Slave_MotorStructure *P,float flux);
u8 Slave_StopPump(Slave_MotorStructure *P);
void Slave_MotorInit(Slave_MotorStructure *p);
u8 Slave_MotorPidDataInit(Slave_MotorStructure *P);
void Slave_MotorPidPara(Slave_MotorStructure *P);
u8 Slave_ContinuePump(Slave_MotorStructure *P,float flux);
void Slave_OpenWaterPump(void);       //打开水泵20200528
void Slave_CloseWaterPump(void);      //关闭水泵20200528


//---------主从通讯相关---------
void Slave_Communicate(void);
void Slave_CommSendData(u8 address);
u8 Slave_CommReceiveData(u8 *p);

void Slave_DebugASCII(u8 *p,u8 len);
//void Slave_DebugRTU(u8 *p,u8 len);

#endif










