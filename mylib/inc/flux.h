/*
	Flux库函数
*/

#ifndef __FLUX_H
#define __FLUX_H

#include "stm32f4xx.h"


#define FLUX_HJtoBWX							//定义后，环境温度传感器与保温箱温度传感器互换
//宏定义
#define FLUX_FILTER_DEPTH					100	//滤波循环队列深度
#define FLUX_MIDDLEFILTER_DEPTH   34  //中值滤波队列深度34
#define FLUX_ColFAULT							1000000	//采集数据故障
#define FLUX_BaseTem							50		//温度基值，单位℃
#define FLUX_BaseCPress						0//0.1		//差压的基值，单位kPa
#define FLUX_BaseGPress						0//100		//差压的基值，单位kPa
#define FLUX_BaseDPress						100		//大气压基值，单位kPa

#define FLUX_StnPress							(float)101.325	//标准大气压值
#define FLUX_AbsTem								(float)273.15	//绝对零度值
#define FLUX_BasicVal							(float)1.34		//计算参数值

//硬件端口宏定义
#define FLUX_SendBufferData	 			    USARTx_StoreBufferData			//RTU数据发送函数
#define FLUX_UsartPort						USART2											//对应串口输出端口
#define FLUX_UsartBuffer					Get_USART2Buffer()					//对应串口结构体

#define FLUX_FangDaoXiTimeB   10         //防倒吸时间是60秒
#define FLUX_FangDaoXiTimeC   180         //防倒吸时间是60秒

//滤波结构体
typedef struct
{
	u8 Flag;			//B0=1表示已经存满整个缓冲区,B1=1,表示中值缓冲区
	u8 Cnt;				//计数器
	float Sum;		//和
	float Buffer[FLUX_FILTER_DEPTH];//缓存区
	float BufferMiddle[FLUX_MIDDLEFILTER_DEPTH];           //中值滤波缓冲区
	float OldMiddle;                //上一次中值滤波值
	u8 CntMiddle;                   //中值滤波计数
}Flux_FilterStructure;

//电机结构体
typedef struct
{
	u8 OnOff;				//状态
	u8 RunMode;
	u16 Timer;
	u16 Ratio;						//占空比
	PID_DataStructure Pid;//PID数据
}Flux_MotorStructure;

//温度(T(℃))压力(P(kPa))流量(F(L/min))结构体
typedef struct
{
	float Actual;		//真实值，校准后的用来显示
	float Raw;			//原始值，没有校准的
	float K;				//计算温度函数K值
	float B;				//计算温度函数B值
	u16 Cnt;				//用于计算零点用，计数器
	float Sum;			//用于计算零点用，总值
	Flux_FilterStructure Filter;//滤波器
}Flux_FluxStructure;

//采集结构体
typedef struct
{
	u8 RunMode;				//采样状态
	u8 Flag;				//B0=1电池电压低但依然继续采样
	u32 SecTimer;			//采样计时，单位S 
	u32 IntTime;			//计平均时间计时
	float Flux;				//用户设定采样流量，单位L/min
	u32 SamTime;			//采样时长(SamplingTime)，单位秒,总采样时长,预先加载与SecTimer比较
	u32 DinSiTime;          //烟气采样定时时间（记录时间）,预先加载，与IntTime比较//烟尘是单点采时
	u32	StrTime;			//启动时刻(StartTime),单位秒
	u16 DXTime;              //防倒吸时间计算
    u8 AutoMod;             //烟气的自动手动模式
	u8 JpjFlg;              //计平均标志           外部传入数据   
	u16 Count;				//采样次数,总次数,预先加载//烟尘是总点数
	u16 Cnt;				//采样计次数
	u8 YiciFlg;             //一次或一点采完标准
	u8 EndFlg;              //结束完成标志
	u8 FdxFlg;              //防倒吸完成标志
	float SumVolume;	    //累计体积，单位L
	float RefVolume;	    //参比体积，单位L
}Flux_SamplingStructure;

//通道结构体
typedef struct
{
	u8 RunMode;  //0:待机 1：采样启动初始化；2：采样过程中
    u32 SaveFileNumber;             //正在保存的文件号
	u16 Timer;											//定时器
	float SetFlux;                  //设置流量
	float ShowFlux;									//显示流量,要求采样的流量---目的
	
	u8 ShowMode;										//显示流量模式，'R'=入口流量'K'=刻度流量、'C'=参比流量
	
	Flux_FluxStructure FluxGK;			//工况流量（过程量）	
	Flux_FluxStructure FluxRK;			//入口流量（过程量）
	Flux_FluxStructure FluxCB;			//参比流量 ---备用
	Flux_FluxStructure FluxKD;			//刻度流量 ---备用
	Flux_FluxStructure FluxTowData; //二次滤波用
	Flux_SamplingStructure Sampling;//采样信息，这来暂时没有

}Flux_ChannelStructure;
					
//流量系统结构体
typedef struct
{
//	u16 Timer;
	u8 DQPressSelectFlag;   //大气压选择标准 0：测量值，1：选择值（标定界面进入设1,退出回复为0，）
	float  DQPressbuf;      //进入页面把他赋予检测值;公共大气压
	u8 JwSelectFlag;   //计温选择标记//输入值=1，测量值=0,（标定界面进入设1,退出回复为0，）
	float  JWbuf; //进入页面把他赋予检测值;
	
	Flux_ChannelStructure ChannelA;//A路 不用泵
	Flux_ChannelStructure ChannelB;//B路 尘泵
	Flux_ChannelStructure ChannelC;//C路 气泵
}Flux_DataStructure;


//----------------------------------------------------
void Flux_Main(void);
void Flux_Init(void);
void Flux_Timer(void);
void Flux_SecTimer(void);
Flux_DataStructure* Flux_GetData(void);

void Flux_ChannelRun(char pump);  //20200603

void Flux_ChannelDataInit(Flux_ChannelStructure *p);
void Flux_FluxDataInit(Flux_FluxStructure *p);

void Flux_ClearFluxDataSum(Flux_FluxStructure *p);

void Flux_ReadFlashSamplingData(char pump);
void Flux_ReadFlashShowMode(void);
u8 Flux_ChannelPowerOn(char pump,time_t dTime,time_t nTime);
u8 Flux_RegainLastdata(u8 pump);
u8 Flux_RegainLastState(u8 pump);


float Flux_GetJWTem(void);   //获取计温（输入值测试值）
float Flux_GetDQPress(void);   //或取大气压值是输入或测试

void Flux_CalculateFlux(char pump);
void Flux_FluxDataDealWith(Flux_FluxStructure *p,float value);
float Flux_DataFilter(Flux_FilterStructure *p,float value);
float Flux_DataMiddleFilter(Flux_FilterStructure *p,float value);
float Flux_CalibrationFlux(char pump,float set,float flux);
void Flux_ShowFluxSet(Flux_ChannelStructure *p);
float Flux_FluxCalculate(float differP,float tem,float press,float b);
void Flux_CalVolume(Flux_ChannelStructure *p);

void Flux_ClearMidBuff(Flux_FilterStructure *p,float vol);

void Flux_LoadJWTem(u8 select,float Jwbuf);        //先赋予计温临时值 -对外接口,把选择标记同时设为1
void Flux_LoadDQPress(u8 select,float DQPressbuf); //先赋予计温临时值 -对外接口,把选择标记同时设为1

u8 Flux_StarSampling(char pump,float Flux);  //对外接口
u8 Flux_StopSampling(char pump);             //对外接口
u8 Flux_ContinueSampling(char pump,float Flux);

void Flux_SaveSamplingRunData(void);
void Flux_SvaeSamplingData(u8 mode,Flux_ChannelStructure *p);
void Flux_SvaeSamplingTimeData(Flux_ChannelStructure *p);

void Flux_DebugASCII(u8 *p,u8 len);


#endif










