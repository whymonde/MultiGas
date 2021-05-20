/*
	Flux�⺯��
*/

#ifndef __FLUX_H
#define __FLUX_H

#include "stm32f4xx.h"


#define FLUX_HJtoBWX							//����󣬻����¶ȴ������뱣�����¶ȴ���������
//�궨��
#define FLUX_FILTER_DEPTH					100	//�˲�ѭ���������
#define FLUX_MIDDLEFILTER_DEPTH   34  //��ֵ�˲��������34
#define FLUX_ColFAULT							1000000	//�ɼ����ݹ���
#define FLUX_BaseTem							50		//�¶Ȼ�ֵ����λ��
#define FLUX_BaseCPress						0//0.1		//��ѹ�Ļ�ֵ����λkPa
#define FLUX_BaseGPress						0//100		//��ѹ�Ļ�ֵ����λkPa
#define FLUX_BaseDPress						100		//����ѹ��ֵ����λkPa

#define FLUX_StnPress							(float)101.325	//��׼����ѹֵ
#define FLUX_AbsTem								(float)273.15	//�������ֵ
#define FLUX_BasicVal							(float)1.34		//�������ֵ

//Ӳ���˿ں궨��
#define FLUX_SendBufferData	 			    USARTx_StoreBufferData			//RTU���ݷ��ͺ���
#define FLUX_UsartPort						USART2											//��Ӧ��������˿�
#define FLUX_UsartBuffer					Get_USART2Buffer()					//��Ӧ���ڽṹ��

#define FLUX_FangDaoXiTimeB   10         //������ʱ����60��
#define FLUX_FangDaoXiTimeC   180         //������ʱ����60��

//�˲��ṹ��
typedef struct
{
	u8 Flag;			//B0=1��ʾ�Ѿ���������������,B1=1,��ʾ��ֵ������
	u8 Cnt;				//������
	float Sum;		//��
	float Buffer[FLUX_FILTER_DEPTH];//������
	float BufferMiddle[FLUX_MIDDLEFILTER_DEPTH];           //��ֵ�˲�������
	float OldMiddle;                //��һ����ֵ�˲�ֵ
	u8 CntMiddle;                   //��ֵ�˲�����
}Flux_FilterStructure;

//����ṹ��
typedef struct
{
	u8 OnOff;				//״̬
	u8 RunMode;
	u16 Timer;
	u16 Ratio;						//ռ�ձ�
	PID_DataStructure Pid;//PID����
}Flux_MotorStructure;

//�¶�(T(��))ѹ��(P(kPa))����(F(L/min))�ṹ��
typedef struct
{
	float Actual;		//��ʵֵ��У׼���������ʾ
	float Raw;			//ԭʼֵ��û��У׼��
	float K;				//�����¶Ⱥ���Kֵ
	float B;				//�����¶Ⱥ���Bֵ
	u16 Cnt;				//���ڼ�������ã�������
	float Sum;			//���ڼ�������ã���ֵ
	Flux_FilterStructure Filter;//�˲���
}Flux_FluxStructure;

//�ɼ��ṹ��
typedef struct
{
	u8 RunMode;				//����״̬
	u8 Flag;				//B0=1��ص�ѹ�͵���Ȼ��������
	u32 SecTimer;			//������ʱ����λS 
	u32 IntTime;			//��ƽ��ʱ���ʱ
	float Flux;				//�û��趨������������λL/min
	u32 SamTime;			//����ʱ��(SamplingTime)����λ��,�ܲ���ʱ��,Ԥ�ȼ�����SecTimer�Ƚ�
	u32 DinSiTime;          //����������ʱʱ�䣨��¼ʱ�䣩,Ԥ�ȼ��أ���IntTime�Ƚ�//�̳��ǵ����ʱ
	u32	StrTime;			//����ʱ��(StartTime),��λ��
	u16 DXTime;              //������ʱ�����
    u8 AutoMod;             //�������Զ��ֶ�ģʽ
	u8 JpjFlg;              //��ƽ����־           �ⲿ��������   
	u16 Count;				//��������,�ܴ���,Ԥ�ȼ���//�̳����ܵ���
	u16 Cnt;				//�����ƴ���
	u8 YiciFlg;             //һ�λ�һ������׼
	u8 EndFlg;              //������ɱ�־
	u8 FdxFlg;              //��������ɱ�־
	float SumVolume;	    //�ۼ��������λL
	float RefVolume;	    //�α��������λL
}Flux_SamplingStructure;

//ͨ���ṹ��
typedef struct
{
	u8 RunMode;  //0:���� 1������������ʼ����2������������
    u32 SaveFileNumber;             //���ڱ�����ļ���
	u16 Timer;											//��ʱ��
	float SetFlux;                  //��������
	float ShowFlux;									//��ʾ����,Ҫ�����������---Ŀ��
	
	u8 ShowMode;										//��ʾ����ģʽ��'R'=�������'K'=�̶�������'C'=�α�����
	
	Flux_FluxStructure FluxGK;			//������������������	
	Flux_FluxStructure FluxRK;			//�����������������
	Flux_FluxStructure FluxCB;			//�α����� ---����
	Flux_FluxStructure FluxKD;			//�̶����� ---����
	Flux_FluxStructure FluxTowData; //�����˲���
	Flux_SamplingStructure Sampling;//������Ϣ��������ʱû��

}Flux_ChannelStructure;
					
//����ϵͳ�ṹ��
typedef struct
{
//	u16 Timer;
	u8 DQPressSelectFlag;   //����ѹѡ���׼ 0������ֵ��1��ѡ��ֵ���궨���������1,�˳��ظ�Ϊ0����
	float  DQPressbuf;      //����ҳ�����������ֵ;��������ѹ
	u8 JwSelectFlag;   //����ѡ����//����ֵ=1������ֵ=0,���궨���������1,�˳��ظ�Ϊ0����
	float  JWbuf; //����ҳ�����������ֵ;
	
	Flux_ChannelStructure ChannelA;//A· ���ñ�
	Flux_ChannelStructure ChannelB;//B· ����
	Flux_ChannelStructure ChannelC;//C· ����
}Flux_DataStructure;


//----------------------------------------------------
void Flux_Main(void);
void Flux_Init(void);
void Flux_Timer(void);
void Flux_SecTimer(void);
Flux_DataStructure* Flux_GetData(void);

void Flux_ChannelRun(char pump);  //20200603

void Flux_ChannelDataInit(Flux_ChannelStructure *p);
void Flux_FluxDataInit(Flux_FluxStructure *p);

void Flux_ClearFluxDataSum(Flux_FluxStructure *p);

void Flux_ReadFlashSamplingData(char pump);
void Flux_ReadFlashShowMode(void);
u8 Flux_ChannelPowerOn(char pump,time_t dTime,time_t nTime);
u8 Flux_RegainLastdata(u8 pump);
u8 Flux_RegainLastState(u8 pump);


float Flux_GetJWTem(void);   //��ȡ���£�����ֵ����ֵ��
float Flux_GetDQPress(void);   //��ȡ����ѹֵ����������

void Flux_CalculateFlux(char pump);
void Flux_FluxDataDealWith(Flux_FluxStructure *p,float value);
float Flux_DataFilter(Flux_FilterStructure *p,float value);
float Flux_DataMiddleFilter(Flux_FilterStructure *p,float value);
float Flux_CalibrationFlux(char pump,float set,float flux);
void Flux_ShowFluxSet(Flux_ChannelStructure *p);
float Flux_FluxCalculate(float differP,float tem,float press,float b);
void Flux_CalVolume(Flux_ChannelStructure *p);

void Flux_ClearMidBuff(Flux_FilterStructure *p,float vol);

void Flux_LoadJWTem(u8 select,float Jwbuf);        //�ȸ��������ʱֵ -����ӿ�,��ѡ����ͬʱ��Ϊ1
void Flux_LoadDQPress(u8 select,float DQPressbuf); //�ȸ��������ʱֵ -����ӿ�,��ѡ����ͬʱ��Ϊ1

u8 Flux_StarSampling(char pump,float Flux);  //����ӿ�
u8 Flux_StopSampling(char pump);             //����ӿ�
u8 Flux_ContinueSampling(char pump,float Flux);

void Flux_SaveSamplingRunData(void);
void Flux_SvaeSamplingData(u8 mode,Flux_ChannelStructure *p);
void Flux_SvaeSamplingTimeData(Flux_ChannelStructure *p);

void Flux_DebugASCII(u8 *p,u8 len);


#endif










