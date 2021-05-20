
#ifndef __LTC2944_H
#define __LTC2944_H

//������ͷ�ļ�
#include "stm32f4xx.h"
//IO��������	 

#define READLTC2944_SDA   GPIO_ReadInputDataBit(LTC2944_SDA_PORT,LTC2944_SDA_PIN)  //����SDA 
#define LTC2944_Reg_Depth  0X18   //0x18���ֽ�

#define Status_adrr   0x00
#define Control_adrr  0x01
#define AccumulatCharge_adrr  0x02
#define ChargeThresholdHigh_adrr  0x04
#define ChargeThresholdLow_adrr   0x06
#define Voltage_adrr   0x08
#define VoltageThresholdHigh_adrr   0x0a
#define VoltageThresholdLow_adrr    0x0c

#define Current_adrr    0x0E
#define CurrentThresholdHigh_adrr    0x10
#define CurrentThresholdLow_adrr    0x12
#define Temperature_adrr    0x14
#define TemperatureThresholdHigh_adrr    0x16
#define TemperatureThresholdLow_adrr    0x17

typedef struct
{
	uint8_t Status;
	uint8_t Control;
	uint16_t AccumulatCharge;
	uint16_t ChargeThresholdHigh;
	uint16_t ChargeThresholdLow;
	
	uint16_t Voltage;
	uint16_t VoltageThresholdHigh;
	uint16_t VoltageThresholdLow;
	
	uint16_t Current;
	uint16_t CurrentThresholdHigh;
	uint16_t CurrentThresholdLow;
	
	uint16_t Temperature;
	uint8_t TemperatureThresholdHigh;
	uint8_t TemperatureThresholdLow;
}LTC2944_Reg;

typedef union {
	LTC2944_Reg Reg;
	u8 arr[LTC2944_Reg_Depth];
}LTC2944_un;

typedef struct
{
	LTC2944_un unData;	
	u16 Timer;
	u8 RunMode;
	float V;
	float I;
	float Temp;
	float ACC;   //�ٷ���
	float PTC;   //�ٷ���
	u8 Mode;	 //����<10% =1  ;>10%=0
	u8 flag; //=1��ʾ�ϵ��һ�ζ�����ACC,����������ã�����
}LTC2944_DataStructure;                  



#define LTC2944_RefR  5       //5��ŷ

#define LTC2944_V_K  1       //��������
#define LTC2944_V_b  -1.2f      //����ƫ��

#define LTC2944_I_K  1       //��������
#define LTC2944_I_b  0      //����ƫ��

#define LTC2944_Temp_K  1       //��������
#define LTC2944_Temp_b  0      //����ƫ��

#define LTC2944_ACCMax   0xA000  //�������ֵ
#define LTC2944_ACC_K  (float)1.76       //��������
#define LTC2944_ACC_b  0         //����ƫ��

#define Li_ion_Capacity  9600   //mAH   �������Ϊ9.6AH
#define LTC2944_ACCMin  LTC2944_ACCMax-(Li_ion_Capacity)/LTC2944_ACC_K/(0.34*(50/LTC2944_RefR))    //0.2Ϊ20%
 
#define BATTERY_VMAX  (float)29.4   //25.9*1.14 
#define BATTERY_Vcmp  (float)19.2   //����ʱ�����Ƚ� 		
#define BATTERY_ACCmin  10   //������Ͱٷ���
 
//LTC2944���в�������
LTC2944_DataStructure* LTC2944_GetData(void);  //

void LTC2944_Init(void);        //��ʼ��LTC2944��IO��			
void LTC2944_Timer(void);
void LTC2944_Main(void);


void LTC2944_Start(void);				//����LTC2944��ʼ�ź�
void LTC2944_Stop(void);	  			//����LTC2944ֹͣ�ź�
void LTC2944_Send_Byte(u8 txd);			//LTC2944����һ���ֽ�
u8 LTC2944_Read_Byte(unsigned char ack);//LTC2944��ȡһ���ֽ�
u8 LTC2944_Wait_Ack(void); 				//LTC2944�ȴ�ACK�ź�
void LTC2944_Ack(void);					//LTC2944����ACK�ź�
void LTC2944_NAck(void);				//LTC2944������ACK�ź�
 
void LTC2944_WriteByte(uint8_t regadrr,uint8_t data);
void LTC2944_WriteTwoByte(uint8_t regadrr,uint16_t data);
void LTC2944_WriteNByte(uint8_t regadrr,uint8_t N,uint8_t * data);

uint8_t LTC2944_ReadOneByte(uint8_t regadrr);
uint16_t LTC2944_ReadTwoByte(uint8_t regadrr);
void LTC2944_ReadNByte(uint8_t regadrr,uint8_t N,LTC2944_un * data );
void LTC2944_DataInit(void);
float LTC2944_Cal_V(u16 RedV);
float LTC2944_Cal_I(u16 RedI);
float LTC2944_Cal_Temp(u16 RedTemp);
u16 LTC2944_Cal_ACC(float VOL);
float LTC2944_Cal_PTC(u16 RedACC);

#endif
