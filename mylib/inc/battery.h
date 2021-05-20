/*
	battery�⺯��
*/

#ifndef __BATTERY_H
#define __BATTERY_H

#include "stm32f4xx.h"


#define BATTERY_DATALEN        	100		//��Դ��ѹ�ɼ����ݴ�������
#define BATTERY_VPT_ON					(float)19.2	//��Դ��ѹ������ֵ�����ڸ�ֵʱ������ɼ�������ѹС�ڸ�ֵʱ��Ϊ�������Ϊ0V
#define BATTERY_VPT_OFF					(float)18.9	//��Դ��ѹ�ر���ֵ��С�ڸõ�ѹʱ����ֹ�ɼ�����
#define BATTERY_Full100					29		//��ѹ����29Vʱ��ʾ��ǰ��س�������
#define BATTERY_Vref           (float) 3.0      //AD �ο���ѹ
#define BATTERY_OP              11              //��·������ѹ����
#define LowPOWER                20.0f           //�������͵ĵ�ѹ�Ƚ�20.0f
	
//��ر����ṹ��
typedef struct
{
	u16 Data[BATTERY_DATALEN];
	u8 Cnt;				//������
	float CollectV;	//�ɼ���ѹ
	float ActualV;	//ʵ�ʵ�ѹ���ɲɼ���ѹ���Ա��ʵ���
	float Rate;			//��ѹ����
	u8 Ratio;				//��������ٷֱȣ�ȡֵΪ0~100
}Battery_BatteryStructure;
//��Դ�ṹ��
typedef struct
{
	u8 Mode;							//Mode=0��ѹ<BATTERY_VPT_OFF��Mode=1,��ѹ>BATTERY_VPT_ON
	u8 LostPowerFlag;     //�����׼
	u8 CurChanl;          //��ǰת��ͨ��
	Battery_BatteryStructure Battery; //��ص�ѹ  pc1  =ad11
	Battery_BatteryStructure LostPV;  //�����ѹ  pc0  =ad10
	Battery_BatteryStructure Current;  //����õ����PC3=AD13
}Battery_DataStructure;


//----------------------------------------------------
void Battery_Main(void);
void Battery_Init(void);
void Battery_Timer(void);
Battery_DataStructure* Battery_GetData(void);


void Battery_DataInit(void);
void Battery_ReadFlashData(void);
void Battery_InitADC(void);
void Battery_GetVoltage(void);
void Battery_DealWith(void);

void Battery_DebugASCII(u8 *p,u8 len);
//void Battery_DebugRTU(u8 *p,u8 len);






#endif










