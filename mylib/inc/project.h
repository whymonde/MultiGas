/*
	Project库函数
*/

#ifndef __PROJECT_H
#define __PROJECT_H

#include "stm32f4xx.h"

#define  YLB3330_SOFE_VERSION           "V1.0"   //软件版本
#define  YLB3330_HEAT_VERSION           "V1.0"   //硬件版本
#define  COMPANY_TELEPHON  "0757-22629008"               //电话号码
#define  COMPANY_WEBSITE   "http://www.zcqk88.com"       //网站  

//输出
#define PRO_LED1(a)\
					if (a==1)\
						GPIO_SetBits(PRO_LED1_GPIO,PRO_LED1_PIN);\
					else if (a==0)\
						GPIO_ResetBits(PRO_LED1_GPIO,PRO_LED1_PIN);\
					else if(a==2)\
						PRO_LED1_GPIO->ODR ^= PRO_LED1_PIN;
#define PRO_LED2(a)\
					if (a==1)\
						GPIO_SetBits(PRO_LED2_GPIO,PRO_LED2_PIN);\
					else if (a==0)\
						GPIO_ResetBits(PRO_LED2_GPIO,PRO_LED2_PIN);\
					else if(a==2)\
						PRO_LED2_GPIO->ODR ^= PRO_LED2_PIN;

#define PROJECT_BUZZER(a)\
					if (a==1)\
						GPIO_ResetBits(PROJECT_BUZZER_PORT,PROJECT_BUZZER_PIN);\
					else if (a==0)\
						GPIO_SetBits(PROJECT_BUZZER_PORT,PROJECT_BUZZER_PIN);\
					else if(a==2)\
						PROJECT_BUZZER_PORT->ODR ^= PROJECT_BUZZER_PIN;	
//输入
					
/*
	系统结构体
*/
//typedef struct 
//{
//	float D_Tem;
//	float MA_Press;
//	float MA_Tem;
//	float MB_Press;
//	float MB_Tem;
//	float MC_Press;
//	float MC_Tem;
//	float SA_Press;
//	float SA_Tem;
//	float SB_Press;
//	float SB_Tem;
//	float SC_Press;
//	float SC_Tem;
//	float SD_Press;
//	float SD_Tem;
//	float A_Flux;
//	float B_Flux;
//	float C_Flux;
//}Project_BufferStructure;


/*
	系统结构体
*/
typedef struct 
{
	u8 Address;			//本机地址
	u8 RunMode;			//运行模式
	u32 Timer;			//系统累计运行时间，单位秒
	u16 TimerPro;		
	u16 DelayTimre;	//想用延时定时器用，在项目定时器中若不为0就--，直到减为0为止，不可同时使用
	
}Project_SystemStructure;


//库函数
void Project_Init(void);				//本函数为函数初始化
void Project_Timer(void);				//系统定时器
void Project_SecTimer(void);
void Porject_Debug(u8 *p);			//该函数为与系统项目相关的调试函数
void Project_ClearProjectSysBuffer(void);//该函数用于复位Projec_SysBuffer所有的数据
Project_SystemStructure *Project_GetData(void); //返回系统变量结构体的地址
void Project_TimerInit(void);		//初始化本项目的定时器	
void Project_InitLED(void);
void Project_IWDGInit(u32 TimeoutFreMs);

//Project_BufferStructure *Project_GetBuffer(void);
	

void Project_DebugASCII(u8 *p,u8 len);//
void Project_DebugRTU(u8 *p,u8 len);//

#endif



