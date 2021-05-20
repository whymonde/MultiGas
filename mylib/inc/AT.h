/*
	battery�⺯��
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
	u8 RunMode;					//����ģʽ
	u16 Timer;					  //��ʱ��
	u16 H_time;           //��ʱ�����ڼ��ATģʽ������/͸����
	u16 sBTtimer;		//��ʱ��������������ʼ��
	u8 EnableBTtimer;     //������ʱ��������������ʼ��
	u16 LevelTimeCnt;     //��ƽʱ�������
	u16 LevelCnt;         //��ƽ����������
	u8 NextMode;
	u8 Error;
	u8 OkFLG;             //�������
	u8 Password[5];
	u32 Baudrate;
	u8 Mode;  //=0͸��ģʽ, =1����ģʽ,=2 û����ģ��
}At_ModeStructure;

At_ModeStructure *  AT_GetData(void);
void AT_Key_Init(void);  //Ӳ����ʼ��
void AT_Statekey_Init(void); //Ӳ����ʼ��
////���ַ
void AT_Keyset_Com(void);   //����ӿ�
u8 AT_Mode_Check(void);
void AT_Power_On(void);
void AT_Power_Off(void);
//����
void AT_AT_Set(void);

void AT_Reset_Com(void); //AT Ӳ�������ϵ�

void AT_AT_Com(void);  //AT ��������

//����

void AT_Password_Com(u8 * Password);  //����ӿ�
//��ģʽ
void AT_Role_Set(void);
void AT_Role_Com(void); //����ӿ�
//������
void AT_BaudRate_Set(void);
void AT_BautRate_Com(u32 baudrate); //����ӿ�

void AT_Init(void);
void AT_main(void);
void AT_Timer(void);
void AT_DebugASCII(u8 *p,u8 len);


#endif





