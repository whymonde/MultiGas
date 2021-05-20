/*
	Power.c�⺯��
*/

#ifndef __POWER_H
#define __POWER_H

//#include "stm32f4xx_conf.h"
#include "stm32f4xx.h"

//���
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
					
//�ṹ��
typedef struct
{
	u8 RunMode;					//����ģʽ
	u8 Flg;                     //bit0:����Ϊ0����������ҳ�����Ϊ1�������1�ſ�ʼ��⿪�ػ�����
	                            //bit1:�ⲿ����Ŀ���״̬
	                            //bit2:��ع���Ŀ���״̬
	u16 Timer;					//��ʱ��
    u16 Timer10ms;              //10MS��ʱ��
	u16 Cnt;                    //��10MS�Ĵ���
    u8 starFlg;                 //����ʱ�ص��־
	u16 Tcnt;                    //����ʱʱ��
    u8 StaCnt;                  //�ߵ�ƽ������
	u8 V220Cnt;                 //10ms����
}Power_DataStructure;


//----------------------------------------------------
void Power_Main(void);
void Power_Timer(void);
void EX0_Init(void);
void Power_Init(void);
Power_DataStructure* Power_GetData(void);
void Power_SetFlg(void); //����ӿ�

void Power_ReadData(u8 address,u8 *p,u8 len);
void Power_StoreData(u8 address,u8 *p,u8 len);
void Power_DebugASCII(u8 *p,u8 len);

#endif










