/*
	screen库函数
*/

#ifndef __SCREEN_H
#define __SCREEN_H

#include "stm32f4xx.h"
#include <stdbool.h>

//校准方法选择
#define SCREEN_MAINTAIN			//注释该宏定义后为旧校准
//#define SCREEN_PointDebug		//调试时显示小数点位数

#define SCREE_CRC										//屏幕通讯开启CRC校验	
//常量宏定义
#define SCREEN_BFLEN								64			//结构体Screen_DataStructure变量的缓存大小
//键盘常量按键值宏定义
#define SCREEN_KEY_0								0x0030
#define SCREEN_KEY_1								0x0031
#define SCREEN_KEY_2								0x0032
#define SCREEN_KEY_3								0x0033
#define SCREEN_KEY_4								0x0034
#define SCREEN_KEY_5								0x0035
#define SCREEN_KEY_6								0x0036
#define SCREEN_KEY_7								0x0037
#define SCREEN_KEY_8								0x0038
#define SCREEN_KEY_9								0x0039
#define SCREEN_KEY_Point							0x002E														
#define SCREEN_KEY_A								0x0041
#define SCREEN_KEY_B								0x0042
#define SCREEN_KEY_C								0x0043
#define SCREEN_KEY_D								0x0044
#define SCREEN_KEY_E								0x0045
#define SCREEN_KEY_F								0x0046
#define SCREEN_KEY_G								0x0047
#define SCREEN_KEY_H								0x0048
#define SCREEN_KEY_I								0x0049
#define SCREEN_KEY_J								0x004A
#define SCREEN_KEY_K								0x004B
#define SCREEN_KEY_L								0x004C
#define SCREEN_KEY_M								0x004D
#define SCREEN_KEY_N								0x004E
#define SCREEN_KEY_O								0x004F
#define SCREEN_KEY_P								0x0050
#define SCREEN_KEY_Q								0x0051
#define SCREEN_KEY_R								0x0052
#define SCREEN_KEY_S								0x0053
#define SCREEN_KEY_T								0x0054
#define SCREEN_KEY_U								0x0055
#define SCREEN_KEY_V								0x0056
#define SCREEN_KEY_W								0x0057
#define SCREEN_KEY_X								0x0058
#define SCREEN_KEY_Y								0x0059
#define SCREEN_KEY_Z								0x005A
#define SCREEN_KEY_OK								0x00F1
#define SCREEN_KEY_ESC								0x00F0
#define SCREEN_KEY_Back								0x00F2
#define SCREEN_KEY_Blank							0x0020					

//常量相关宏定义
#define SCREEN_FACTORY							0XAA		//回复出厂设置码
#define SCREEN_KEYADD							0X0001	//键值变量地址
#define SCREEN_SAMPLE_ADRESS					0X0200	//采样地址变量地址

#define SCREEN_KEYADD_ESC						0X0002	//软键盘ESC按键返回值地址



//按键值宏定义
#define SCREEN_NONE_KEY							0//无
#define SCREEN_UP_KEY							10//向上
#define SCREEN_R_KEY							11//向右
#define SCREEN_SW_KEY							12//切换
#define SCREEN_ESC_KEY							13//退出
#define SCREEN_OK_KEY							14//确认

/********************************************    迪文屏显示控件的返回地址   ***************************************************************/
/********************************** 0-开机密码页 ********************************************/
#define SCREEN_ADD_KJMM  						0x9001			//密码	2020-05-20 hjl add	

/********************************** 1-系统设置页 ********************************************/
#define SCREEN_ADD_RIQI							0x9111			//日期						*/																																	
#define SCREEN_ADD_SHIJIAN  					0x9112			//时间						*/
#define SCREEN_ADD_DAQIYA  						0x9113			//大气压					*/
#define SCREEN_ADD_PSJG  						0x9118			//排水间隔					*/
#define SCREEN_ADD_PSSS  						0x9119			//排水时长					*/
//#define SCREEN_ADD_YC_MIN						0x9121			//烟气分钟设置				*/
//#define SCREEN_ADD_YC_TIMES					0x9122			//烟气次数					*/
#define SCREEN_ADD_HSL_MIN						0x911A			//含湿量分钟				*/
/********************************************************************************************/
/********************************** 2-烟道布点 **********************************************/
#define SCREEN_ADD_CIRCLE_DIAMETER  			0x9211			//1-圆形					*/																																	
#define SCREEN_ADD_CIRCLE_AREA  				0x9212			//							*/
#define SCREEN_ADD_CIRCLE_PIPE		 			0x9213			//							*/
#define SCREEN_ADD_CIRCLE_RING  				0x9214			//							*/
#define SCREEN_ADD_CIRCLE_HOLE  				0x9215			//							*/
#define SCREEN_ADD_RECT_A  						0x9221			//2-矩形					*/																																	
#define SCREEN_ADD_RECT_B  						0x9222			//							*/
#define SCREEN_ADD_RECT_AREA		 			0x9223			//							*/
#define SCREEN_ADD_RECT_PIPE  					0x9224			//							*/
#define SCREEN_ADD_RECT_HOLE  					0x9225			//							*/
#define SCREEN_ADD_RECT_TESTPOINT  				0x9226			//							*/
#define SCREEN_ADD_CROSS_SECTION				0x9231			//3-其它，烟道截面			*/
#define SCREEN_ADD_SAMPLES  					0x9232			//							*/
#define SCREEN_ADD_FLUE_FIND  					0x9241			//							*/
/********************************************************************************************/
/********************************** 3-工况预测 **********************************************/
#define SCREEN_ADD_YANWENG				  		0x9321			//2-烟温测量				*/																																	
#define SCREEN_ADD_FILE_CHECK				  	0x9341			//4-查询文件号				*/
/********************************************************************************************/
/********************************** 4-测含湿量 **********************************************/
#define SCREEN_ADD_MOISTURE				  		0x9411			//1-含湿量输入				*/																																	
/********************************************************************************************/
/********************************** 5-测量烟气 **********************************************/
#define SCREEN_ADD_YC_MIN					  	0x9501			//1-计平均时间				*/
#define SCREEN_ADD_YC_TIMES					  	0x9502			//1-计平均次数				*/
#define	SCREEN_ADD_GAS_FIND						0x9521			//1-烟气查询				*/
#define	SCREEN_ADD_CALC_FACTOR					0x9561			//1-折算系数				*/
#define	SCREEN_ADD_OXYGEN_BASE					0x9562			//2-基准含量				*/
#define	SCREEN_ADD_LOAD_FACTOR					0x9563			//3-负荷系数				*/
#define	SCREEN_ADD_O2_INPUT						0x95A1			//1-O2						*/
#define	SCREEN_ADD_SO2_INPUT					0x95A2			//2-SO2						*/
#define	SCREEN_ADD_NO_INPUT						0x95A3			//3-NO						*/
#define	SCREEN_ADD_NO2_INPUT					0x95A4			//4-NO2						*/
#define	SCREEN_ADD_CO_INPUT						0x95A5			//5-CO						*/
#define	SCREEN_ADD_H2S_INPUT					0x95A6			//6-H2S						*/
#define	SCREEN_ADD_CO2_INPUT					0x95A7			//7-CO2						*/
#define	SCREEN_ADD_START_NUMBER					0x9571			//1-开始文件号				*/
#define	SCREEN_ADD_END_NUMBER					0x9572			//2-结束文件号				*/
/********************************************************************************************/
/********************************** 6-烟尘采样 **********************************************/
#define SCREEN_ADD_CC_SAMPLE			  		0x9611			//1-恒流采样				*/
#define SCREEN_ADD_CARTRIDGE_NUMBER  			0x9612			//2-滤筒号					*/
#define SCREEN_ADD_TIME_MINUTE			 		0x9613			//3-单点采时分钟			*/
#define SCREEN_ADD_TIME_SECOND			 		0x9614			//4-单点采时秒钟			*/
#define SCREEN_ADD_MOUTH_DIAMETER	  			0x9615			//5-采嘴直径				*/
#define SCREEN_ADD_TOTAL_SAMPLE			  		0x9616			//6-总采点					*/
/********************************************************************************************/
/********************************** 7-文件查询 **********************************************/
#define	SCREEN_ADD_OILGAS_FIND					0x9721			//1-油烟查询				*/
#define	SCREEN_ADD_CONCENTRATIO					0x9761			//1-溶液浓度				*/
#define	SCREEN_ADD_VOLUME						0x9762			//2-定容体积				*/
#define	SCREEN_ADD_RANGES_NUMBER				0x9763			//3-灶头数目				*/
#define	SCREEN_ADD_START_NUMBER2				0x9771			//1-开始文件号				*/
#define	SCREEN_ADD_END_NUMBER2					0x9772			//2-结束文件号				*/
#define	SCREEN_ADD_DUST_WEIGHT					0x97A1			//1-烟尘净重				*/
#define	SCREEN_ADD_DUST_OXYGEN					0x97A2			//2-含氧量					*/
#define	SCREEN_ADD_DUST_LOAD_FACTOR				0x97A3			//3-负荷系数				*/
#define	SCREEN_ADD_DUST_CALC_FACTOR				0x97A4			//4-折算系数				*/
#define	SCREEN_ADD_DUST_OXYGEN_BASE				0x97A5			//5-基准含氧量				*/
#define	SCREEN_ADD_DUST_FILE_NUM				0x97A6			//6-烟气文件号				*/
/********************************************************************************************/
/********************************** 9-校准维护 **********************************************/
#define SCREEN_ADD_PASSWORD_INPUT				0x9801			//输入密码					*/
//1-系统配置																				*/
#define SCREEN_ADD_CALIB_NOx				  	0x9811			//NO2/NOX					*/
#define SCREEN_ADD_CO_SO2						0x9812			//COvsSO2					*/
#define SCREEN_ADD_NO2_SO2					  	0x9813			//NO2vsSO2					*/
#define SCREEN_ADD_PTG_Kp						0x9814			//皮托管Kp					*/
#define SCREEN_ADD_SMOKE_DENSITY		  		0x9815			//烟气密度					*/
#define SCREEN_ADD_CALC_RATIO				  	0x9816			//折算系数					*/
#define SCREEN_ADD_OXYGEN_REF				  	0x9817			//基准含氧量				*/
//1-系统配置-CO vs SO2																		*/
#define SCREEN_ADD_COSO2_TEXT01				  	0x98A1			// 							*/
#define SCREEN_ADD_COSO2_TEXT02				  	0x98A2			// 							*/
#define SCREEN_ADD_COSO2_TEXT03				  	0x98A3			// 							*/
#define SCREEN_ADD_COSO2_TEXT04				  	0x98A4			// 							*/
#define SCREEN_ADD_COSO2_TEXT05				  	0x98A5			// 							*/
#define SCREEN_ADD_COSO2_TEXT06				  	0x98A6			// 							*/
#define SCREEN_ADD_COSO2_TEXT07				  	0x98A7			// 							*/
#define SCREEN_ADD_COSO2_TEXT08				  	0x98A8			// 							*/
#define SCREEN_ADD_COSO2_TEXT09				  	0x98A9			// 							*/
#define SCREEN_ADD_COSO2_TEXT0A				  	0x98AA			// 							*/
#define SCREEN_ADD_COSO2_TEXT0B				  	0x98AB			// 							*/
#define SCREEN_ADD_COSO2_TEXT0C				  	0x98AC			// 							*/
#define SCREEN_ADD_COSO2_TEXT0D				  	0x98AD			// 							*/
#define SCREEN_ADD_COSO2_TEXT0E				  	0x98AE			// 							*/
#define SCREEN_ADD_COSO2_TEXT0F				  	0x98AF			// 							*/
#define SCREEN_ADD_COSO2_TEXT10				  	0x98B0			// 							*/
#define SCREEN_ADD_COSO2_TEXT11				  	0x98B1			// 							*/
#define SCREEN_ADD_COSO2_TEXT12				  	0x98B2			// 							*/
#define SCREEN_ADD_COSO2_TEXT13				  	0x98B3			// 							*/
#define SCREEN_ADD_COSO2_TEXT14				  	0x98B4			// 							*/
//2-流量校准																				*/
#define SCREEN_ADD_STD_FLOW						0x9821			//标准流量					*/
#define SCREEN_ADD_RATIO_TEXT02				  	0x9822			// 20-倍率					*/
#define SCREEN_ADD_RATIO_TEXT03				  	0x9823			// 30-倍率					*/
#define SCREEN_ADD_RATIO_TEXT04				  	0x9824			// 							*/
#define SCREEN_ADD_RATIO_TEXT05				  	0x9825			// 							*/
#define SCREEN_ADD_RATIO_TEXT06				  	0x9826			// 							*/
#define SCREEN_ADD_RATIO_TEXT07				  	0x9827			// 							*/
#define SCREEN_ADD_RATIO_TEXT08				  	0x9828			// 							*/
#define SCREEN_ADD_RATIO_TEXT09				  	0x9829			// 							*/
#define SCREEN_ADD_RATIO_TEXT0A				  	0x982A			// 							*/
#define SCREEN_ADD_RATIO_TEXT0B				  	0x982B			// 							*/
#define SCREEN_ADD_RATIO_TEXT0C				  	0x982C			//温度						*/
#define SCREEN_ADD_RATIO_TEXT0D				  	0x982D			//大气压					*/
//3-密码设置																				*/
#define SCREEN_ADD_ORIG_PASS					0x9831			//原密码					*/
#define SCREEN_ADD_NEW_PASS				  		0x9832			//新密码					*/
#define SCREEN_ADD_CONFIRM_PASS				  	0x9833			//确认密码					*/
#define SCREEN_ADD_PRINT_PASS				  	0x9834			//打印蓝牙密码 				*/
#define SCREEN_ADD_APP_PASS				  		0x9835			//APP蓝牙密码				*/
#define SCREEN_ADD_E28ADR_PASS                  0x9836          //E28地址
#define SCREEN_ADD_E28CH_PASS                   0x9837          //E28信道
//4-浓度校准																				*/
#define SCREEN_ADD_O2_TEXT01					0x9841			//O2零点					*/
#define SCREEN_ADD_SO2_TEXT02				  	0x9842			//SO2零点					*/
#define SCREEN_ADD_NO_TEXT03				  	0x9843			//NO零点					*/
#define SCREEN_ADD_NO2_TEXT04				  	0x9844			// 							*/
#define SCREEN_ADD_CO_TEXT05				  	0x9845			//							*/
#define SCREEN_ADD_H2S_TEXT06				  	0x9846			// 							*/
#define SCREEN_ADD_CO2_TEXT07				  	0x9847			// 							*/
//4-浓度校准->倍置设置																		*/
#define SCREEN_ADD_MID_RATIO				  	0x984A			//中端倍率K 				*/
#define SCREEN_ADD_LOW_RATIO				  	0x984B			//低端倍率K1				*/
#define SCREEN_ADD_HIGH_RATIO				  	0x984C			//高端倍率K2				*/
#define SCREEN_ADD_MID_STD				  		0x984D			//中端标气浓度				*/
#define SCREEN_ADD_LOW_STD				  		0x984E			//低端标气浓度				*/
#define SCREEN_ADD_HIGH_STD				  		0x984F			//高端标气浓度				*/
//5-压力校准																				*/
#define SCREEN_ADD_AIR_PRESSURE			  		0x9851			//1-大气压	 				*/
#define SCREEN_ADD_CALC_PRESSURE		  		0x9852			//2-计压					*/
#define SCREEN_ADD_STATIC_PRESSURE	  			0x9853			//3-静压					*/
#define SCREEN_ADD_DYNAMIC_PRESSURE  			0x9854			//4-动压					*/
#define SCREEN_ADD_DUST_PRESSURE	  			0x9855			//5-尘压					*/
#define SCREEN_ADD_FLOW_PRESSURE	  			0x9856			//6-气流压					*/
//7-温度校准																				*/
#define SCREEN_ADD_DRY_BALL			  			0x9871			//1-烟温干球零点	 		*/
#define SCREEN_ADD_WET_BALL		  				0x9872			//2-湿球零点				*/
#define SCREEN_ADD_CALC_TEMP	  				0x9873			//3-计温零点				*/
#define SCREEN_ADD_CIRCLE_TEMP  				0x9874			//4-环温零点				*/
#define SCREEN_ADD_DRY_RATIO	  				0x9875			//5-干球倍率				*/
#define SCREEN_ADD_WET_RATIO	  				0x9876			//6-湿球倍率				*/
//7-电池标定																				*/
#define SCREEN_ADD_BATTERY_STD			  		0x9881			//1-电池标定				*/
//超级维护																					*/																								*/
#define SCREEN_ADD_INSTRUMENT_NUM		  		0x9A11			//1-仪器编号				*/
#define SCREEN_ADD_O2_DATE				  		0x9A21			//1-O2安装日期				*/
#define SCREEN_ADD_SO2_DATE		  				0x9A22			//2-SO2安装日期				*/
#define SCREEN_ADD_NO_DATE				  		0x9A23			//3-NO安装日期				*/
#define SCREEN_ADD_NO2_DATE		  				0x9A24			//4-NO2安装日期				*/
#define SCREEN_ADD_CO_DATE				  		0x9A25			//5-CO安装日期				*/
#define SCREEN_ADD_H2S_DATE		  				0x9A26			//6-H2S安装日期				*/
#define SCREEN_ADD_CO2_DATE		  				0x9A27			//7-CO2安装日期				*/

#define SCREEN_ADD_O2_LQ				  		0x9A28			//1-O2安装量程				*/
#define SCREEN_ADD_SO2_LQ		  				0x9A29			//2-SO2安装量程				*/
#define SCREEN_ADD_NO_LQ				  		0x9A2A			//3-NO安装量程    		*/
#define SCREEN_ADD_NO2_LQ		  				0x9A2B			//4-NO2安装量程   		*/
#define SCREEN_ADD_CO_LQ				  		0x9A2C			//5-CO安装量程	    	*/
#define SCREEN_ADD_H2S_LQ		  				0x9A2D			//6-H2S安装量程 			*/
#define SCREEN_ADD_CO2_LQ		  				0x9A2E			//7-CO2安装量程  			*/




/********************************************************************************************/



//时钟相关宏定义
#define SCREEN_YearBase							1900//年基值：1900年，显示时，需要增加该值
#define SCREEN_TimeZone							8		//时区：东八区
#define SCREEN_YearScreen						100 //屏幕时钟年与time.c库转换差值


//硬件端口宏定义
#define SCREEN_SendBufferData	 			USARTx_StoreBufferData			//RTU数据发送函数
#define SCREEN_UsartPort						USART3											//对应串口输出端口
#define SCREEN_UsartBuffer					Get_USART3Buffer()					//对应串口结构体
//
//屏幕公历时间
typedef struct
{
	struct tm Clock;
	u8 Flag;				//b0用作秒计时作用；B1=1表示已获取到时钟数据；B2=1表示上次掉电数据已保存，可以从新获取新时间
	u8 ClockSec;		//用作秒计时用
	u16 Msec;				//读时钟定时器，单位：毫秒
}Screen_TimeStructure;

typedef enum
{
	E28_SET_STATE_MACHINE_SLEEP=0,
	E28_SET_STATE_MACHINE_INIT=1,
	E28_SET_STATE_MACHINE_WAITOK=2,
	E28_SET_STATE_MACHINE_FINISH=3,
}E28_SET_STATE_MACHINE;

typedef struct
{
	u32 uCounter1S;					//定时器
	u32 uCounter1ms;				//1ms计数器
	u32 uCounter100ms;				//100ms计数器
	bool bTime100ms;				//100ms计数标志位
	bool bTime1S;
	u8 ucCurrentPage;
	u8 uMinDataPage;				//分钟数据页
	u8 cIsCalcAverage;				//正在计平均；
	u8 cAverageCount;				//平均次数计算；
	u32 Timer;
	double JpjSum[8];               //累加计平均
	u16    JpjSuncnt;            //累加次数
    u8 JpjEndFlg;                //时间完成标记
}Screen_JPGStructure;

//按键相关结构体
typedef struct
{
	u8 KeyData;					//实体按键值
	u8 Opt;							//当前页面选中对象(一级对象中具体对象)
	u8 LastOpt;					//当前页面上一次对象(一级对象中具体对象)
	u8 OldOpt;					//上一界面选中对象
	u8 TwoOpt;					//二级对象
	u8 Mode;						//0:一级对象，>=1:二级对象的具体对象
	u8 LastMode;				//0:一级对象，>=1:二级对象的具体对象
	u8 State;						//用于数字闪烁
	u8 Buffer[32];			//缓存器
	u8 number;          //缓存按键数字
	u8 KeyDir;             //按键加减方向
	u8 bOutsideKeyboard;	//外部键盘
}Screen_keyStructure;

typedef struct
{ u32 Printwho;                         //打印那种文件1:采样; 2:瞬时;3:掉电
	u32 PDatashunshi;                    //针对瞬时文件的文件号
	u32 PDataFrom,PDataTo;               //打印的文件开始，结束  ,入口
  u32 PDatacurrent;	                   //正在打印的号码
	u32 SampTimeFrom,SampTimeTo;          //采样文件开始时间,结束时间
	u32 SampNumberFrom,SampNumberTo;      //开始文件号，结束文件号
	u32 SunshiFileNumber;                 //瞬时文件号
	u32 SunShiNumberFrom,SunShiNumberTo;  //瞬时开始序列号，瞬时结束序列号
	u32 LosePowerNumberFrom,LosePowerNumberTo;  //掉电开始文件号，掉电结束文件号
}Screen_Page58Structure;

typedef union __tagSDFileStateBit
{
	struct
	{
		unsigned int m_uDustFile		:1;		//
		unsigned int m_uGasFile			:1;		//
		unsigned int m_uWorkFile		:1;		//
		unsigned int m_uAllFile			:1;
		unsigned int m_data0			:1;		
		unsigned int m_factorySet		:1;		//出厂设置
	}BitField;
	u8 m_byData;
}SysConfigWaringBits;

//屏幕结构体2020-05-21
/*
	修改记录：2020-05-25 modify by alfred， 删除无用的uTempKeymode
*/
typedef struct
{
	u16 RunMode;				//运行模式
	u8 RunModeStat;		//运行模式 状态
	u16 NextMode;				//运行模式
	u16 OldMode;				//旧模式
	u16 Button;					//屏幕按键值
	u8 Cnt;							//计数器
	u8 SubCnt;          //局部计算器
	u8 Con;							//数值
	//u8 DataBuffer[32];	//数据缓存，只在一个子程序内有效
	u8 Buffer[SCREEN_BFLEN];	//缓存
	u8 Page;						//当前系统显示页面
	u8 OldPage;					//上一界面
	u16 Timer;					//定时器
	u8 CT;                     //定时器的计算器
	u16 AgvTimer;					//平均时间--烟气测量使用
	u16 AgvTimes;					//平均次数--烟气测量使用
	u16 TimerSecUp;					//定时器
	
	u16 LastTimerSecUp;					//上次定时器
	u16 LastTimerMinUp;					//上次定时器
	u16 TimerSec;        //秒定时器
	u16 BacklightSecTimer;//背光控制计时器
	Screen_TimeStructure Clock;//时钟
	Screen_keyStructure Key;//按键
//	Screen_Page58Structure Screen_Page58;
//  screen

	u16 sValScreenKeyEsc; 			//软件的ESC键键值
	u16 sReveiveDataTimer;   //按下确认后等待收到数据，用于判断超时
	u8 uEnableRecDataTimer;  //启动定时器
	u16	sRcvType;            //按下确认后屏幕收到的数据的返回类型
	u8 uTempState;		//临时状态全局变量, 用于不同函数之间数据传递,使用完后必须置零 //2020-05-30
	
	u8 uE28CtlState;   //设置E28的启动标志，完成归零
	u16 uE28Timer;     //设置E28的监控定时器，判断是否超时
	E28_SET_STATE_MACHINE g_E28_Set_StateMachine;  //E28的状态机寄存器
	
	Screen_JPGStructure JiPingJun;

	u16 BatflshTimer;   //电池动态定时器
	u8  Batflshcnt;     //电池动态计数器
	u8 ScreenButtonLock;// 锁按键
	u8 OldScreenButtonLock;// 锁按键
	u8 Warning;          //警告
	u8 Function;          //功能  00停止 01开始确认
	u8 LastWarning;          //上次警告
	u8 LastFunction;          //上次功能  00停止 01开始确认
	u32 LastChoice;        //上次选择
	u8 LastCoiceonoff;     //上次启动/停止选择
	u8 LoadFlag;             //进入界面装载数据
	u8 StarCun;              //从一开始计数 
	u16 SampleCun;              //烟气采样当前点数
	u8 RecordCun;           //Gas 记录  计算平均时用
	u8 LastRecordCun;           //Gas 记录
	uint64_t UseGasCal;          //参与数据平均计算（按位来确定）
	u8 PauseOrStop;                 //通用暂停标志 ==B0表示暂停 ==BO已启动
	u8 YC_Pause;                 //烟尘界面的 用于调用烟气检测时避免混淆 暂停标志 ==B0表示暂停 ==BO已启动
	u8 YC_Type;                 //烟尘采样类型
	u8 QimiStat;             //气密未检测==0  检测失败==1 检测成功==2
	u32 YC_SampleAllTime;    //烟尘采样总时间
	u8 YC_Stat;        //烟尘状态 ==1 表示在进行烟尘采样
	u16 YC_SampleTimerSec;        //烟尘采样秒定时器
	float YC_FollowRate;        //跟踪率
	float YC_TrageFlux;         //目标流量
	float FileNumber;
	float DisValue[40];
	u16 RandomNum;		//随机数 2020-05-19 hjl add 
	float ZSXS;                //折算系数           ，烟气查询计算页面用
	float JZHYL;               //基准含氧量
	float FHXS;                //负荷系数
	float GSXS;                //过剩系数
	float HYL;                 //含氧量
	float YCJZ;                //烟尘净重
	u32   DRWJH;               //导入文件号
	float RYLD;                //溶液浓度
	float DRTZ;                //定容体积
	float ZTSM;                //灶头数目
	u8 WaterPumRunmod;         //抽水泵运行模式
	u32 WaterPumTimer;         //抽水泵定时器
	u16 ChangTimer;            //监控定时器
	u8 staflg;                 //启动自加定时器标志
	u16 ZjTimer;               //自加定时器
	u8 gzdselect;              //高中低选择
	u8 Scan_status;            //数字录入状态b0=0触摸屏幕没录入状态,b0=1触摸屏录入状态， B1=1,启动检测，B1=0,不启动检测
	u16 Scan_Stimer;           //定时器
	SysConfigWaringBits gSysConfigWaringBits;	//用于文件删除状态提示
	u8 mystatus;              //局部用状态
	char recorddat[20];   //记录时间,
	char recorstarddat[20];   //记录开始采样时间，关键记录计平均的时间
	u8 uMenuPosition;
}Screen_DataStructure;



//----------------------------------------------------
void Screen_Main(void);
void Screen_Timer(void);
void Screen_SecTimer(void);
void Screen_Init(void);
Screen_DataStructure* Screen_GetData(void);

void Screen_MainPage(Screen_DataStructure *p);
void Screen_SystemSet(Screen_DataStructure *p);
void Screen_FlueSet(Screen_DataStructure *p);
void Screen_SamplingA(Screen_DataStructure *p);
void Screen_SamplingB(Screen_DataStructure *p);
void Screen_SamplingTSP(Screen_DataStructure *p);
void Screen_MaintainPressureSet(Screen_DataStructure *p);
void Screen_MaintainTemSet(Screen_DataStructure *p);
void Screen_MaintainFlowSet(Screen_DataStructure *p);
void Screen_MaintainSystemSet(Screen_DataStructure *p);
void Screen_SuperParameterSet(Screen_DataStructure *p);
void Screen_SuperPIDSet(Screen_DataStructure *p);
void Screen_SuperBackupsSet(Screen_DataStructure *p);
void Screen_SuperNumberSet(Screen_DataStructure *p);
void Screen_SamplingInquire(Screen_DataStructure *p);
void Screen_SamplingPrint(Screen_DataStructure *p);
void Screen_USBFlashExport(Screen_DataStructure *p);
void Screen_NetworkSettings(Screen_DataStructure *p);
void Screen_KeyInput(Screen_DataStructure *p);
void Screen_KeyBoard(Screen_DataStructure *p);
void Screen_Backlight(Screen_DataStructure *p);
void Screen_BacklightDW(void);
void Screen_SearchPrinter(Screen_DataStructure *p);//搜索打印机 
void Screen_PowerDowm(Screen_DataStructure *p);//关机	
void Screen_changPowerState(void);	
	
void Screen_ShowPage(u16 page);
void Screen_RefreshDataMulti(u16 address,u8 *p,u8 len);
/*
	函数名：void Screen_RefreshVarData(u16 address,u16 data)
	描述：向屏幕变量存储器发送位图选项
	输入：address 地址
				data数据
			
*/
void Screen_RefreshVarData(u16 address,u16 data);
void Screen_ShowScreenLockState(void);
void Screen_Reset(void);
void Screen_ResetDataBuffer(void);
void Screen_ResetKeyDataBuffer(void);
void Screen_WriteRegister(u8 address,u8 len,u8 *p);
void Screen_WriteRegisterOneData(u8 address,u8 data);
void Screen_ReadRegister(u8 address,u8 len);
void Screen_PastePicture(u16 address,u16 pid,u32 ctlc,u32 clrc,u32 ptlc);
void Screen_RectangleFilling(u16 address,u32 ctlc,u32 clrc,u16 colour);
//void Screen_SetClock(u16 year,u8 month,u8 day,u8 hour,u8 minute,u8 sec);
void Screen_SetClock(struct tm *sTime);
void Screen_ShowDynamicData(void);
void Screen_SecClock(void);
void Screen_ShowNetworkState(void);
void Screen_ShowPowerState(void);
void Screen_ShowWifi(void);
void Screen_ScanSchack(void);   //检测屏幕录入的状态

void Screen_InitData(void);
void Screen_ColckInit(void);

//void Screen_ReadData(u8 address,u8 *p,u8 len);
//void Screen_StoreData(u8 address,u8 *p,u8 len);
void Screen_DebugASCII(u8 *p,u8 len);
void ScreenReadMemory(u16 address,u8 len);

void Screen_DebugASCII(u8 *p,u8 len);
//void Screen_DebugRTU(u8 *p,u8 len);
void Screen_SavePowerDownMsg(void);
void Screen_DealWihtReceiveData(void);
//void Screen_VariateData(u16 address,u8 *p,u8 len);

/*
	函数名：void Screen_YanDaoBuDianSet(Screen_DataStructure *p)
	描述：	烟道布点设置界面
	RunMode范围：0x2000-0x2fff
*/
void Screen_FlueSet(Screen_DataStructure *p);
/*
	函数名：void Screen_WorkingConditionPrediction(Screen_DataStructure *p)
	描述：	工况预测设置界面 
	RunMode范围：0x3000-0x3fff
*/
void Screen_WorkingConditionPrediction(Screen_DataStructure *p);
/*
	函数名：void Screen_MeasureHumidity(Screen_DataStructure *p)
	描述：	测含湿量设置界面 
	RunMode范围：0x4000-0x4fff
*/
void Screen_MeasureHumidity(Screen_DataStructure *p);
/*
	函数名：void Screen_WorkingConditionPrediction(Screen_DataStructure *p)
	描述：	烟气测量设置界面 
	RunMode范围：0x5000-0x5fff
*/
void Screen_MeasureSomke(Screen_DataStructure *p);
/*
	函数名：void Screen_SmokeSampling(Screen_DataStructure *p)
	描述：	烟尘采样设置界面 
	RunMode范围：0x6000-0x6fff
*/
void Screen_SmokeSampling(Screen_DataStructure *p);
/*
	函数名：void Screen_FileSeek(Screen_DataStructure *p)
	描述：	文件查询设置界面 
	RunMode范围：0x7000-0x7fff
*/
void Screen_FileSeek(Screen_DataStructure *p);
/*
	函数名：void Screen_FileSeek(Screen_DataStructure *p)
	描述：	校准维护设置界面 
	RunMode范围：0x8000-0x8fff
*/
void Screen_CalibrationFix(Screen_DataStructure *p);

/*
	函数名：u8* RcvBuffer_GetData(void)
	描述：	 
	RunMode范围：
*/
u8* RcvBuffer_GetData(void);
void ScreenJPG_Timer(void);	//计平均定时器

void ScreenJPG_ClearSum(void);

#endif










