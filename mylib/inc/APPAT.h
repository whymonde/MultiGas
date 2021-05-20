/*
	battery库函数
*/

#ifndef __APPAT_H
#define __APPAT_H

//#include "stdio.h"	
#include "stm32f4xx_conf.h"
//#include "sys.h" 
#define	APP_BLUETOOTH_KEY 	  PDout(5)
#define	APP_BLUETOOTH_POWER  PDout(6)
#define APP_BLUETOOTH_LED    PDin(4)

typedef struct
{
	u8 RunMode;					//运行模式
	u16 Timer;					  //定时器
	u16 H_time;           //定时器用于检测AT模式（命令/透传）
	u16 LevelTimeCnt;     //电平时间计数器
	u16 LevelCnt;         //电平个数计数器
	u8 NextMode;
	u8 Error;
	u8 OkFLG;             //功能完成
	u8 Password[5];
	u32 Baudrate;
	u8 Mode;  //=0透传模式, =1命令模式,=2 没蓝牙模块
}APP_ModeStructure;

APP_ModeStructure *  APP_GetData(void);
void APP_Key_Init(void);  //硬件初始化
void APP_Statekey_Init(void); //硬件初始化
////清地址
void APP_Keyset_Com(void);   //对外接口
u8 APP_Mode_Check(void);
void APP_Power_On(void);
void APP_Power_Off(void);
//握手
void APP_AT_Set(void);

void APP_Reset_Com(void); //AT 硬件重新上电

void APP_AT_Com(void);  //AT 握手命令

//密码

void APP_Password_Com(u8 * Password);  //对外接口
//主模式
void APP_Role_Set(void);
void APP_Role_Com(void); //对外接口
//波特率
void APP_BaudRate_Set(void);
void APP_BautRate_Com(u32 baudrate); //对外接口

void APP_Init(void);
void APP_main(void);
void APP_Timer(void);
void APP_DebugASCII(u8 *p,u8 len);

#endif





