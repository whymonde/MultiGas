/*
	battery�⺯��
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
	u8 RunMode;					//����ģʽ
	u16 Timer;					  //��ʱ��
	u16 H_time;           //��ʱ�����ڼ��ATģʽ������/͸����
	u16 LevelTimeCnt;     //��ƽʱ�������
	u16 LevelCnt;         //��ƽ����������
	u8 NextMode;
	u8 Error;
	u8 OkFLG;             //�������
	u8 Password[5];
	u32 Baudrate;
	u8 Mode;  //=0͸��ģʽ, =1����ģʽ,=2 û����ģ��
}APP_ModeStructure;

APP_ModeStructure *  APP_GetData(void);
void APP_Key_Init(void);  //Ӳ����ʼ��
void APP_Statekey_Init(void); //Ӳ����ʼ��
////���ַ
void APP_Keyset_Com(void);   //����ӿ�
u8 APP_Mode_Check(void);
void APP_Power_On(void);
void APP_Power_Off(void);
//����
void APP_AT_Set(void);

void APP_Reset_Com(void); //AT Ӳ�������ϵ�

void APP_AT_Com(void);  //AT ��������

//����

void APP_Password_Com(u8 * Password);  //����ӿ�
//��ģʽ
void APP_Role_Set(void);
void APP_Role_Com(void); //����ӿ�
//������
void APP_BaudRate_Set(void);
void APP_BautRate_Com(u32 baudrate); //����ӿ�

void APP_Init(void);
void APP_main(void);
void APP_Timer(void);
void APP_DebugASCII(u8 *p,u8 len);

#endif





