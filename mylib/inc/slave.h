/*
	slave.c�⺯��
*/

#ifndef __SLAVE_H
#define __SLAVE_H

#include "stm32f4xx.h"
#define SCREEN_MAINTAIN //����У����ʽ
#define SLAVE_HJtoBWX							//����󣬻����¶ȴ������뱣�����¶ȴ���������
//�궨��
#define SLAVE_FILTER_DEPTH					16	//�˲�ѭ���������
#define SLAVE_MIDDLEFILTER_DEPTH            4   //��ֵ�˲����
#define SLAVE_ColFAULT							1000000	//�ɼ����ݹ���
#define SLAVE_BaseTem								50		//�¶Ȼ�ֵ����λ��
#define SLAVE_BaseCPress						500//0		//��ѹ�Ļ�ֵ����λkPa
#define SLAVE_BaseGPress						0//100		//��ѹ�Ļ�ֵ����λkPa
#define SLAVE_BaseDPress						0		//����ѹ��ֵ����λkPa
#define SLAVE_BaseGas                   000             //�����ֵ
#define SLAVE_SensorRefreshTime			5			//����������ˢ�����ڣ���λ5ms					
#define SLAVE_DataMax               18  //�����С·��������0-16(��17��),17����û��

//����PT100������������  20200525
#define WdMax   650
#define WdMin   -80
#define AdMin   525
#define AdMax   2927
#define PT100_K               (float)(WdMax-WdMin)/(AdMax-AdMin)
#define PT100_B               WdMin
//Ӳ���˿�ӳ��



//���湲17��������
#define SLAVE_MAP_DQPress		2	//����ѹӳ���ַ(5805)
#define SLAVE_MAP_ACPress		3	//A·��ѹ����ѹ��ӳ���ַ  ��������·
#define SLAVE_MAP_AGPress		6	//A·��ѹ����ѹ��ӳ���ַ
#define SLAVE_MAP_BCPress		4	//B·��ѹ����ѹ��ӳ���ַ  �̳�·
#define SLAVE_MAP_BGPress		7	//B·��ѹ����ѹ��ӳ���ַ
#define SLAVE_MAP_CCPress		5	//C·��ѹ����ѹ��ӳ���ַ  ����·

#define SLAVE_MAP_HWTem			1//�����¶�ӳ���ַ(5805)
#define SLAVE_MAP_YWTem         8 //����   ���Թ���ʱ����
#define SLAVE_MAP_GQTem         8 //����   ���Ժ�ʪ������
#define SLAVE_MAP_SQTem         9 //ʪ��
#define SLAVE_MAP_JWTem			0//�����£�DS18B20�������̳�ʱ����

#define SLAVE_MAP_O2Concent     16//����Ũ��ӳ���ַ
#define SLAVE_MAP_SO2Concent    11//����Ũ��ӳ���ַ
#define SLAVE_MAP_NOConcent     15//����Ũ��ӳ���ַ
#define SLAVE_MAP_NO2Concent    14//����Ũ��ӳ���ַ
#define SLAVE_MAP_COConcent     13//����Ũ��ӳ���ַ
#define SLAVE_MAP_H2SConcent    12//����Ũ��ӳ���ַ
#define SLAVE_MAP_CO2Concent    10//����Ũ��ӳ���ַ

#define SLAVE_MAP_E28DY         18  //E28������ѹӳ���ַ
#define SLAVE_MAP_E28JY         19  //E28������ѹӳ���ַ
#define SLAVE_MAP_E28YW         20  //E28��������ӳ���ַ


//Ӳ���˿ں궨��
#define Slave_SendBufferData	 		USARTx_StoreBufferData			//RTU���ݷ��ͺ���
#define Slave_UsartPort						USART2											//��Ӧ��������˿�
#define Slave_UsartBuffer					Get_USART2Buffer()					//��Ӧ���ڽṹ��


//�˲��ṹ��
typedef struct
{
	u8 Flag;			//B0=1��ʾ�Ѿ�����������������B1=1��ʾ�Ѿ�����������ֵ�˲�������
	u8 Cnt;				//������
	float Sum;		//��
	float Buffer[SLAVE_FILTER_DEPTH];//������
	float BufferMiddle[SLAVE_MIDDLEFILTER_DEPTH];           //��ֵ�˲�������
	float OldMiddle;                //��һ����ֵ�˲�ֵ
	u8 CntMiddle;                   //��ֵ�˲�����
}Slave_FilterStructure;

//�¶�(T(��))ѹ��(P(kPa))ͨѶ�����ṹ��
typedef struct
{
	float Raw;										//ԭʼֵ(ƽ��ֵ)��û��У׼��
	Slave_FilterStructure Filter;	//�˲���
}Slave_TPRawStructure;
//�¶�(T(��))ѹ��(P(kPa))�ṹ��
typedef struct
{
	float Value;		//ƽ��
	float Value0,Value1,Value2;     //��Ҫ���������Ӧ���ʼ���ֵ Value0=��Ũ�Ȳ���ֵ;Value1=��Ũ�Ȳ���ֵ;Value2=��Ũ�Ȳ���ֵ
	float K;				//�����¶Ⱥ���Kֵ ��Ũ�ȱ���
	float K1,K2;    //K1,K2���ʣ���Ҫ��������K1=��Ũ�ȱ���;K2=��Ũ�ȱ���;
	float B;				//�����¶Ⱥ���Bֵ
	union
	{
		u8 All;
		struct
		{
			u8 : 2;
			u8 RawMode : 3;		//0��ʾ�ޣ�1,��������ģʽ����
			u8 ActualMode : 3;	//0��ʾ�޲�����1��ʾ����ģʽ��2��ʾ����ģʽ
		}Bits;
	}Mode;
	u16 Cnt;		//���ڼ�������ã�������
	u16	Timer;		//���㶨ʱ������λms
	float Sum;		//���ڼ�������ã���ֵ
	float Max;		//������ֵ
	float Min;		//�����Сֵ
	float OldRaw;	//Raw��ֵ
}Slave_TPActualStructure;

//ͨѶ�ṹ��
typedef struct
{
	u8 Error;
	u8 ErrorCnt;
	u8 RunMode;
	u8 NewMode;
	u16 Timer;					//��ʱ��
}Slave_CommStructure;

//����ṹ��
typedef struct
{   char rame;
	u8 OnOff;				//״̬
	u8 RunMode;
	u16 Timer;
	u16 Ratio;						//ռ�ձ�
	PID_DataStructure Pid;//PID����
}Slave_MotorStructure;

//E28-2.4G���߽ṹ��,�������Ҫ�õ�
typedef struct
{   
	u8 STATE,STATERD24G;	//״̬,��λ���·����ݵ�״̬,STATE.B0=1:����;   STATE.B0=0,������;  STATERD24G.B0=1������;STATERD24G.B0=0�Ͽ�;
	                        //STATERD24G.B1=1���óɹ�;STATERD24G.B1=0����ʧ��;
	u8 CHset,CHRD24G;      //�ŵ�����ֵ,�ŵ��Ӹ��������ֵ
	u16 ADRRset,ADRRRD24G; //��ַ����ֵ,��ַ�Ӹ��������ֵ
}Slave_E28Structure;

//STM8�ṹ�� 2020-05-22 hjl add
typedef struct
{
	u8 Data;					//��״̬
	u8 rw_ctrl;					//��д��������
}STM8_DataStructure;

//�ṹ��
typedef struct
{
	u8 RunMode;					//����ģʽ
	u16 Timer;					//��ʱ��
	u8 GetTimer;				//�����¶Ȼ�ȡ��ʱ��
	Slave_CommStructure Comm;				//ͨѶ�ṹ��
	Slave_TPRawStructure Data[SLAVE_DataMax+3];	//�ӻ��������ݽṹ��8������(�ɼ�������) //20200602 ���Ӷ����߲�������·����ѹ����ѹ������

	Slave_TPActualStructure DQPress;//����ѹ����λkPa
	Slave_TPActualStructure ACPress;//A·��ѹ����ѹ������λkPa
	Slave_TPActualStructure AGPress;//a·��ѹ����ѹ������λkPa
	Slave_TPActualStructure BCPress;//B·��ѹ����ѹ������λkPa
	Slave_TPActualStructure BGPress;//b·��ѹ����ѹ������λkPa
	Slave_TPActualStructure CCPress;//C·��ѹ����ѹ������λkPa
	
	Slave_TPActualStructure JWTem;	//���£����£�D18B20
	Slave_TPActualStructure HWTem;	//�����¶ȣ���λ��5805
	Slave_TPActualStructure YWTem;  //����        
	Slave_TPActualStructure GQTem;  //����
	Slave_TPActualStructure SQTem;  //ʪ��
	
	Slave_TPActualStructure O2Concent; //O2Ũ��
	Slave_TPActualStructure SO2Concent;//SO2Ũ��
	Slave_TPActualStructure NOConcent; //NOŨ��
	Slave_TPActualStructure NO2Concent; //NO2Ũ��

	Slave_TPActualStructure COConcent; //COŨ��	
	Slave_TPActualStructure H2SConcent; //H2SŨ��	
	Slave_TPActualStructure CO2Concent; //CO2Ũ��
	Slave_TPActualStructure NOxConcent; //NOxŨ��
		
	Slave_TPActualStructure BeforO2Concent; //O2����ǰŨ��
	Slave_TPActualStructure BeforSO2Concent;//SO2Ũ��
	Slave_TPActualStructure BeforNOConcent; //NOŨ��
	Slave_TPActualStructure BeforNO2Concent; //NO2Ũ��
	
	Slave_TPActualStructure BeforCOConcent; //COŨ��	
	Slave_TPActualStructure BeforH2SConcent; //H2SŨ��	
	Slave_TPActualStructure BeforCO2Concent; //CO2Ũ��
	Slave_TPActualStructure BeforNOxConcent; //NOxŨ��
 	
	Slave_MotorStructure WaterMotor;  //ˮ��
	Slave_MotorStructure SlvdMotor;  //��ŷ�
	Slave_MotorStructure GasMotor;  //����
	Slave_MotorStructure DustMotor; //����
	STM8_DataStructure Stm8Data;	//2020-05-22 hjl add stm8����
	Slave_E28Structure  E28;
	
}Slave_DataStructure;


//----------------------------------------------------
void Slave_Main(void);
void Slave_Timer(void);
void Slave_Init(void);
Slave_DataStructure* Slave_GetData(void);

void Slave_InitComm(void);
void Slave_InitRawData(void);
void Slave_InitActualData(Slave_TPActualStructure *p);
void Slave_ReadFlashData(void);

void Slave_ReadFlashZOData(Slave_TPActualStructure *P,u32 adrr,float offset);  //��ȡ������ݵ����ݽṹB

void Slave_ReadFlashBLData(Slave_TPActualStructure *P,u32 adrr);//��ȡ�������ݵ����ݽṹ�¶ȣ�����20200524
void Slave_ReadFlashBLDatak1(Slave_TPActualStructure *P,u32 adrr);//��ȡ��������
void Slave_ReadFlashBLDatak2(Slave_TPActualStructure *P,u32 adrr);//��ȡ��������

float Slave_ReadTPFlashData(u32 addr); //����������������

void Slave_GetSensorData(void);

void Slave_DataDealWith(Slave_TPRawStructure *p,float value,u8 flag);
float Slave_DataFilter(Slave_FilterStructure *p,float value);
float Slave_DataMiddleFilter(Slave_FilterStructure *p,float value);//��ֵ�˲�
void Slave_DataMaping(Slave_TPActualStructure *p,float value);

void Slave_Zero(Slave_TPActualStructure *p,u32 Addr,float offset); //�����浽flash
void Slave_Zero_QT(void);  //����ѹ��ֵ����  ����ӿ�
void Slave_Zero_Tem(void);  //�����¶ȵ��� ����ӿ�
void Slave_Zero_Press(void);  //����ѹ��ֵ���� ����ӿ�


//---------������---------
void Slave_MotorRun(Slave_MotorStructure *p,float pv);
u8 Slave_StartMotor(Slave_MotorStructure *P,u16 ratio);
u8 Slave_StopMotor(Slave_MotorStructure *P);
u8 Slave_StartPump(Slave_MotorStructure *P,float flux);
u8 Slave_StopPump(Slave_MotorStructure *P);
void Slave_MotorInit(Slave_MotorStructure *p);
u8 Slave_MotorPidDataInit(Slave_MotorStructure *P);
void Slave_MotorPidPara(Slave_MotorStructure *P);
u8 Slave_ContinuePump(Slave_MotorStructure *P,float flux);
void Slave_OpenWaterPump(void);       //��ˮ��20200528
void Slave_CloseWaterPump(void);      //�ر�ˮ��20200528


//---------����ͨѶ���---------
void Slave_Communicate(void);
void Slave_CommSendData(u8 address);
u8 Slave_CommReceiveData(u8 *p);

void Slave_DebugASCII(u8 *p,u8 len);
//void Slave_DebugRTU(u8 *p,u8 len);

#endif










