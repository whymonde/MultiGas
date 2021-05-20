/*
	battery库函数
*/

#ifndef __AT_H
#define __AT_H

//#include "stdio.h"	
#include "stm32f4xx_conf.h"
//#include "sys.h" 
#define	AT_BLUETOOTH_KEY 	  PDout(1)
#define	AT_BLUETOOTH_POWER  PDout(3)
#define AT_BLUETOOTH_LED    PDin(0)



typedef enum {
	BT_DEFAULT = 0,	
	BT_TIME_OUT = 1,
	BT_OK = 2,
} BT_RESULT;

typedef struct
{
	u8 RunMode;					//运行模式
	u16 Timer;					  //定时器
	u16 H_time;           //定时器用于检测AT模式（命令/透传）
	u16 sBTtimer;		//定时器，用于蓝牙初始化
	u8 EnableBTtimer;     //启动定时器，用于蓝牙初始化
	u16 LevelTimeCnt;     //电平时间计数器
	u16 LevelCnt;         //电平个数计数器
	u8 NextMode;
	u8 Error;
	u8 OkFLG;             //功能完成
	u8 Password[5];
	u32 Baudrate;
	u8 Mode;  //=0透传模式, =1命令模式,=2 没蓝牙模块
}At_ModeStructure;

At_ModeStructure *  AT_GetData(void);
void AT_Key_Init(void);  //硬件初始化
void AT_Statekey_Init(void); //硬件初始化
////清地址
void AT_Keyset_Com(void);   //对外接口
u8 AT_Mode_Check(void);
void AT_Power_On(void);
void AT_Power_Off(void);
//握手
void AT_AT_Set(void);

void AT_Reset_Com(void); //AT 硬件重新上电

void AT_AT_Com(void);  //AT 握手命令

//密码

void AT_Password_Com(u8 * Password);  //对外接口
//主模式
void AT_Role_Set(void);
void AT_Role_Com(void); //对外接口
//波特率
void AT_BaudRate_Set(void);
void AT_BautRate_Com(u32 baudrate); //对外接口

void AT_Init(void);
void AT_main(void);
void AT_Timer(void);
void AT_DebugASCII(u8 *p,u8 len);


#endif





