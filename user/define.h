/*
	本模块作用：该库文件主要用于各种常用的宏定义，管脚重命名、地址定义、自定义头文件的包含等等相关
	使用方法：	将新增的头文件在此进行包含，则其他的文件不必再包含
				同时将本文件包含在stm32f4xx_conf.h头文件的目录下
	修改日期：
	修改说明：
*/
#ifndef __DEFINE_H
#define __DEFINE_H

//常用宏定义
#define NEWBOARD

//位宏定义
#define	b0		0x01
#define	B0		0x01
#define	b1		0x02
#define	B1		0x02
#define	b2		0x04
#define	B2		0x04
#define	b3		0x08
#define	B3		0x08
#define	b4		0x10
#define	B4		0x10
#define	b5		0x20
#define	B5		0x20
#define	b6		0x40
#define	B6		0x40
#define	b7		0x80
#define	B7		0x80
#define	b8		0x100
#define	B8		0x100
#define	b9		0x200
#define	B9		0x200
#define	b10		0x400
#define	B10		0x400
#define	b11		0x800
#define	B11		0x800
#define	b12		0x1000
#define	B12		0x1000
#define	b13		0x2000
#define	B13		0x2000
#define	b14		0x4000
#define	B14		0x4000
#define	b15		0x8000
#define	B15		0x8000
#define	b16		0x10000
#define	B16		0x10000
#define	b17		0x20000
#define	B17		0x20000
#define	b18		0x40000
#define	B18		0x40000
#define	b19		0x80000
#define	B19		0x80000
#define	b20		0x100000
#define	B20		0x100000
#define	b21		0x200000
#define	B21		0x200000
#define	b22		0x400000
#define	B22		0x400000
#define	b23		0x800000
#define	B23		0x800000
#define	b24		0x1000000
#define	B24		0x1000000
#define	b25		0x2000000
#define	B25		0x2000000
#define	b26		0x4000000
#define	B26		0x4000000
#define	b27		0x8000000
#define	B27		0x8000000
#define	b28		0x10000000
#define	B28		0x10000000
#define	b29		0x20000000
#define	B29		0x20000000
#define	b30		0x40000000
#define	B30		0x40000000
#define	b31		0x80000000
#define	B31		0x80000000

//常用常量宏定义
#define	PI		3.1415926f		//π值
#define	TABLE_ROW		8		//默认值为8，即每页表格总共有8行
#define	JPG_DELAY_TIME		10000 //默认值20000(20秒)

//2020-05-21
#define	TIM_OVERTIME_CHECK		1500		//超时时间1.5秒

#define true 1
#define false 0

#define TwoBytesBitFunction(a,b,c)\
                           if(c==1)\
													 a|=(1<<b);\
													 else if(c==0)\
													 a&=~(1<<b); 




typedef union 
{
	unsigned char			U8Data[30];
	unsigned short			U16Data[15];
}AdressData;


//共用体定义
//用于两个字节数据转换为无符号字节
typedef union 
{
	char					S8Data[2];
	unsigned char			U8Data[2];
	signed short int 		S16Data;
	unsigned short int		U16Data;
}TwoBytes;
//用于四个字节数据转换为无符号字节
typedef union 
{
	char					S8Data[4];
	unsigned char 			U8Data[4];
	signed short int		S16Data[2];
	unsigned short int		U16Data[2];
	signed int 				S32Data;
	unsigned int			U32Data;
	float 					FloatData;
}FourBytes;
//用于八个字节数据转换为无符号字节
typedef union 
{
	char					S8Data[8];
	unsigned char 			U8Data[8];
	signed short int 		S16Data[4];
	unsigned short int		U16Data[4];
	signed int 				S32Data[2];
	unsigned int			U32Data[2];
	long long 				S64Data;
	unsigned long long 		U64Data;
	float 					FloatData[2];
	double 					DoubleData;
}EightBytes;


//PID结构体
typedef struct
{
	float Sv;//用户设置值
	float Pv;//当前值
	float Kp;//比例系数
	float T; //PID计算周期（多少时间计算一次PID)
	float Ti;//积分时间常数
	float Td;//微分系数
	float Ek;//本次偏差
	float Ek_1;//上次偏差
	float Ek_2;//上上次偏差
	float SEk;//历史偏差和
	float OUT0;//输出常数
	float OUT;//计算结果（控制值）
//	float Avg;//积分平均值
	float PWMcycle;//PWM周期
	unsigned short int Timer;//计时器
}PID_DataStructure;

//管脚宏定义
#define AKX_V1		//硬件版本选择

#ifdef AKX_V1		//V1版本LED端口

//PRO_LED1指示灯IO口PB8
#define PRO_LED1_RCC         			RCC_AHB1Periph_GPIOB
#define PRO_LED1_GPIO        			GPIOB    
#define PRO_LED1_PIN         			GPIO_Pin_1  
#define PRO_LED2_RCC		          RCC_AHB1Periph_GPIOB
#define PRO_LED2_GPIO			        GPIOB    
#define PRO_LED2_PIN 			        GPIO_Pin_2  

//RS485方向控制管脚
//#define RS485DIR_APP							RCC_AHB1Periph_GPIOA	//RS485DIR管脚的时钟桥
//#define RS485DIR_PORT							GPIOA									//RS485DIR管脚端口
//#define RS485DIR_PIN							GPIO_Pin_9						//RS485DIR管脚
//4G模块
//PA15管脚配置为输入 ，检测是否联网信号
#define NETLED_4G_APP       	    RCC_AHB1Periph_GPIOA	
#define NETLED_4G_PORT      	    GPIOA    
#define NETLED_4G_PIN       	    GPIO_Pin_15
//PB4管脚配置为输出    4G电源控制管脚
#define POWER_4G_APP							RCC_AHB1Periph_GPIOB	
#define POWER_4G_PORT						  GPIOB								
#define POWER_4G_PIN							GPIO_Pin_4


//蜂鸣器
#define PROJECT_BUZZER_RCC       	RCC_AHB1Periph_GPIOC	
#define PROJECT_BUZZER_PORT      	GPIOC  
#ifdef NEWBOARD
#define PROJECT_BUZZER_PIN       	GPIO_Pin_4
#else
#define PROJECT_BUZZER_PIN       	GPIO_Pin_3
#endif
//蜂鸣器
#define SPEAK_APP       	RCC_AHB1Periph_GPIOC
#define SPEAK_PORT      	GPIOC    
#ifdef NEWBOARD
#define SPEAK_PIN       	GPIO_Pin_4
#else
#define SPEAK_PIN       	GPIO_Pin_3
#endif

//DS18B20总线IO口PC0
//#define DS18B20_DATA_RCC         	RCC_AHB1Periph_GPIOC
//#define DS18B20_DATA_PORT        	GPIOC    
//#define DS18B20_DATA_PIN         	GPIO_Pin_0 
#define DS18B20_DATA_RCC         	RCC_AHB1Periph_GPIOE
#define DS18B20_DATA_PORT        	GPIOE    
#define DS18B20_DATA_PIN         	GPIO_Pin_2 

//蓝牙模块控制及采集管脚
//PIO8管脚，配置为输入
#define HC05_PI8_APP							RCC_AHB1Periph_GPIOC	
#define HC05_PI8_PORT							GPIOC								
#define HC05_PI8_PIN							GPIO_Pin_12	

//PIO9管脚，配置为输入
//#define HC05_PI9_APP							RCC_AHB1Periph_GPIOD	
//#define HC05_PI9_PORT							GPIOD								
//#define HC05_PI9_PIN							GPIO_Pin_2	
//PIO11管脚，配置为输出
//#define HC05_PO11_APP							RCC_AHB1Periph_GPIOB	
//#define HC05_PO11_PORT						GPIOB								
//#define HC05_PO11_PIN							GPIO_Pin_3	

//PD1 控制蓝牙清除记忆
#define HC06_KEY_APP							  RCC_AHB1Periph_GPIOD	
#define HC06_KEY_PORT							  GPIOD								
#define HC06_KEY_PIN							  GPIO_Pin_1	

//PD3 控制蓝牙供电
#define HC06_CTPOWER_APP						RCC_AHB1Periph_GPIOD	
#define HC06_CTPOWER_PORT						GPIOD								
#define HC06_CTPOWER_PIN						GPIO_Pin_3	

//PD0 检测蓝牙当前模式
#define HC06_LED_APP						    RCC_AHB1Periph_GPIOD	
#define HC06_LED_PORT						    GPIOD								
#define HC06_LED_PIN						    GPIO_Pin_0	

//PD5 控制蓝牙清除记忆
#define HC06B_KEY_APP							  RCC_AHB1Periph_GPIOD	
#define HC06B_KEY_PORT							  GPIOD								
#define HC06B_KEY_PIN							  GPIO_Pin_5	

//PD6 控制蓝牙供电
#define HC06B_CTPOWER_APP						RCC_AHB1Periph_GPIOD	
#define HC06B_CTPOWER_PORT						GPIOD								
#define HC06B_CTPOWER_PIN						GPIO_Pin_6	

//PD4 检测蓝牙当前模式
#define HC06B_LED_APP						    RCC_AHB1Periph_GPIOD	
#define HC06B_LED_PORT						    GPIOD								
#define HC06B_LED_PIN						    GPIO_Pin_4	


//风扇控制管脚
//Pd7管脚，配置为输出
#define FAN_APP										RCC_AHB1Periph_GPIOD	
#define FAN_PORT									GPIOD								
#define FAN_PIN										GPIO_Pin_7	
//保温箱加热控制管脚
//PC2管脚，配置为输出
#define HEAT_APP									RCC_AHB1Periph_GPIOE	
#define HEAT_PORT									GPIOE							
#define HEAT_PIN									GPIO_Pin_6

//外接电源(会合点)检测引脚
#define TPOWER_APP								RCC_AHB1Periph_GPIOB	
#define TPOWER_PORT								GPIOB								
#define TPOWER_PIN								GPIO_Pin_9	

#define TCHANG_APP								RCC_AHB1Periph_GPIOB	
#define TCHANG_PORT								GPIOB								
#define TCHANG_PIN								GPIO_Pin_8	

//MAX485_CONTRAL
#define MAX485CTR_APP							RCC_AHB1Periph_GPIOD	
#define MAX485CTR_PORT						    GPIOD								
#define MAX485CTR_PIN							GPIO_Pin_15	

//电池有电检查引脚
#define Tbat_APP								RCC_AHB1Periph_GPIOE	
#define Tbat_PORT								GPIOE								
#define Tbat_PIN								GPIO_Pin_0	
//电池控制供电引脚
#define CTRbat_APP								RCC_AHB1Periph_GPIOE	
#define CTRbat_PORT								GPIOE								
#define CTRbat_PIN								GPIO_Pin_1	

//外部电源有电检查引脚
#define TexPower_APP							RCC_AHB1Periph_GPIOE	
#define TexPower_PORT							GPIOE								
#define TexPower_PIN							GPIO_Pin_6	
//外部电源供电引脚
#define CTRexPower_APP							RCC_AHB1Periph_GPIOE	
#define CTRexPower_PORT							GPIOE								
#define CTRexPower_PIN							GPIO_Pin_4	

#define LTC2944_SDA_RCC    RCC_AHB1Periph_GPIOE
#define LTC2944_SDA_PIN    GPIO_Pin_3   
#define LTC2944_SDA_PORT	 GPIOE

#define LTC2944_SCL_RCC    RCC_AHB1Periph_GPIOC
#define LTC2944_SCL_PIN    GPIO_Pin_2
#define LTC2944_SCL_PORT	 GPIOC

#define LTC2944_ALT_RCC    RCC_AHB1Periph_GPIOE
#define LTC2944_ALT_PIN    GPIO_Pin_5
#define LTC2944_ALT_PORT   GPIOE

#endif

//自定义头文件
#include <stdio.h>
#include "stdlib.h"			//标准工具库函数
#include <math.h>				
#include <string.h>			
#include <time.h>		

#include "project.h"
#include "usart.h"
#include "printf.h"
#include "RTU.h"
#include "ASCII.h"
#include "calculate.h"
#include "flashisp.h"

#include "Screen.h"
#include "DS18B20.h"
#include "Network.h"
#include "flux.h"

#include "fan.h"
#include "slave.h"
#include "battery.h"
#include "heat.h"

#include "sys.h"
#include "ff.h"
#include "file.h"
#include "malloc.h"
#include "usb.h"
#include "mmc_sd.h"
#include "exfuns.h"
#include "timer.h"
#include "process.h"
#include "AT.h"
#include "APPAT.h"
#include "printer.h"
#include "SPEAK.h"
#include "TestPower.h"
#include "myiic.h"
#include "FM24CL64.h"
#include "ScreenDisCalculate.h"
#include "Power.h"
#include "LTC2944.h"
#include "CalculationFormula.h"
//#define DEBUG_MODE

#ifdef DEBUG_MODE
#define DEBUG_MESSAGE(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define DEBUG_MESSAGE(fmt, ...)
#endif


#endif

