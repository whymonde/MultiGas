/*
	Power.c库函数
*/

#ifndef __POWER_H
#define __POWER_H

//#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"

//输出
#define CTRbat_OUT(a)\
					if (a==0)\
						GPIO_ResetBits(CTRbat_PORT,CTRbat_PIN);\
					else if (a==1)\
						GPIO_SetBits(CTRbat_PORT,CTRbat_PIN);\
					else if(a==2)\
						CTRbat_PORT->ODR ^= CTRbat_PIN;
					
#define CTRexPower_OUT(a)\
					if (a==0)\
						GPIO_ResetBits(CTRexPower_PORT,CTRexPower_PIN);\
					else if (a==1)\
						GPIO_SetBits(CTRexPower_PORT,CTRexPower_PIN);\
					else if(a==2)\
						CTRexPower_PORT->ODR ^= CTRexPower_PIN;
					
#define Tbat_LED        PEin(0)
#define TexPower_LED    PEin(6)
					
//结构体
typedef struct
{
	u8 RunMode;					//运行模式
	u8 Flg;                     //bit0:开机为0，调到正常页面就设为1，如果是1才开始检测开关机按键
	                            //bit1:外部供电的控制状态
	                            //bit2:电池供电的控制状态
	u16 Timer;					//定时器
    u16 Timer10ms;              //10MS定时器
	u16 Cnt;                    //计10MS的次数
    u8 starFlg;                 //倒计时关电标志
	u16 Tcnt;                    //倒计时时间
    u8 StaCnt;                  //高电平计数器
	u8 V220Cnt;                 //10ms数器
}Power_DataStructure;


//----------------------------------------------------
void Power_Main(void);
void Power_Timer(void);
void EX0_Init(void);
void Power_Init(void);
Power_DataStructure* Power_GetData(void);
void Power_SetFlg(void); //对外接口

void Power_ReadData(u8 address,u8 *p,u8 len);
void Power_StoreData(u8 address,u8 *p,u8 len);
void Power_DebugASCII(u8 *p,u8 len);

#endif










