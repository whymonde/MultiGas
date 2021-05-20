/*
	CalculationFormula�⺯��
*/

#ifndef __CALCULATIONFORMULA_H
#define __CALCULATIONFORMULA_H

#include "stm32f4xx.h"

#define Vref                    (float)3.00
#define Null_Value							1000000	//δ�����ֵ
#define Ppmtomgm3_SO2 2.86f               //����SO2��PPM��mgm3��ת����
#define Ppmtomgm3_NO  1.339f              //����NO��PPM��mgm3��ת����
#define Ppmtomgm3_NO2 2.05f               //����NO2��PPM��mgm3��ת����
#define Ppmtomgm3_CO  1.25f               //����CO��PPM��mgm3��ת����
#define Ppmtomgm3_H2S 1.517f               //����SO2��PPM��mgm3��ת����


#define YCa1 1.8                           //�����̳�������ʣϵ��a1
#define YYa1 1.2                           //�������̿�����ʣϵ��a1
/*
	ϵͳ�ṹ��
*/
typedef struct 
{
	float F;  //�̵����      ��λ m2         -----��flash�ϻ��
	float Kp; //Ƥ�й�ϵ��    0.83~0.84       -----��Flash�ϻ��	
	
	float Ba; //����ѹ        ��λKPa         -----�Ӹ���ģ�����ṩ����Slave_GetData()->DQPress.Value
	float Ps; //������ѹ      ��λKPa         -----��ʽ���
	float Pd; //������ѹ��������ѹ��λPa      -----�Ӹ���ģ�����ṩ����Slave_GetData()->ACPress.Value
	float Pcs;//�̳���ѹ������ѹ�� KPa        -----�Ӹ���ģ�����ṩ����Slave_GetData()->BGPress.Value
	float Pcd;//�̳���ѹ������ѹ�� Pa         -----�Ӹ���ģ�����ṩ����Slave_GetData()->BCPress.Value
	float Pqd;//����ѹ             Pa         -----�Ӹ���ģ�����ṩ����Slave_GetData()->CCPress.Value
	float Pt; //ȫѹѹ        ��λKPa         -----�Ӹ���ģ�����ṩ����Slave_GetData()->AGPress.Value
	float Pb; //ʪ�������ѹ  ��λKPa         -----��ͬ��ȫѹ Slave_GetData()->AGPress.Value
	float Phv; //����������ѹ ��λKPa         -----����ʪ���¶Ȳ��õ�(CalculationFormula_Phv(float Tb)������

	float Ts ; //�����¶�     ��λ��          -----�Ӹ���ģ�����ṩ����  Slave_GetData()->YWTem.Value
	float Tb ; //ʪ���¶�     ��λ��          -----�Ӹ���ģ�����ṩ���� Slave_GetData()->SQTem.Value  �� ���£�ʪ���¶�ͬһ��������  ��
	float Tc ; //�����¶�     ��λ��          -----�Ӹ���ģ�����ṩ����Slave_GetData()->GQTem.Value
	float Th ; //�����¶�     ��λ��          -----�Ӹ���ģ�����ṩ����Slave_GetData()->HJTemp.Value
	float Tj ; //���¶�       ��λ��          -----�Ӹ���ģ�����ṩ����Slave_GetData()->JWTemp.Value
	
	float V ;   //ʵ�ʲ������  ��λL         -----�ɲ�������ģ�飨�̳�����-����������õ���
	float Vnd ; //����������    ��λL       -----��ʽ���
	
	float p;    //ʪ�����ܶ�       ��λkg/m^3 -----��ʽ���
	float Vs ;  //��������         ��λM/s    -----��ʽ���
	float Qs ;  //��������     ��λM3/h        -----��ʽ���   ����������
	float Qsnd ; //�����������    ��λM3/h   -----��ʽ���
	
	float Qrs ; //����������     ��λL/min    -----�ɲ���ģ��FLUX.C�ṩ(��������-��������)Flux_GetData()->ChannelA.ShowFlux
	
	float d_in ;   //����ֱ��         ��λmm  -----���ٲ���ʱ����¼��
	float d_out;   //����ֱ��         ��λmm  -----Ԥ�⹤��ʱ�ɹ�ʽ���
	float C   ; //ʵ���̳�Ũ��     ��λM3/h   -----����һ��ҳ���������̳������������������ã���������ģ���ṩ�ɣ�
	float a ;  //������ʣϵ��                 -----��ʽ���
	float G ;  //�̳��ŷ���     KG/h          -----��ʽ���
	float Xsw ; //��ʪ������                  -----��ʽ���/������ /���ݷ���ͨ�ţ�     
	float Ca ; //�̳�����Ũ��                 -----��ʽ���
	float CaofSO2,CaofNO,CaofNO2,CaofCO,CaofH2S;            //���������������Ũ��
	float GaofSO2,GaofNO,GaofNO2,GaofCO,GaofH2S;             //�������������ŷ�
	float SO2FromCO,SO2FromNO2,SO2end;                       //
}Calculation_FormulaStructure;
//�⺯��

Calculation_FormulaStructure *CalculationFormula_GetData(void);
void  CalculationFormula_LoadfromSlave1(float Ps,float Pd,float Pcs,float Pcd);//�Ӹ���װ�ض���ѹ����
void  CalculationFormula_LoadfromSlave2(float Ba,float Tb,float Tc);//�Ӹ���װ�ش���ѹ���ȶ�
void  CalculationFormula_LoadVfromFlux(float v);//��Flux ģ������̳������������,���������ǰ����
void  CalculationFormula_LoadQrsfromFlux(float Qrs);//��Flux �õ����ٲ�����������ʵ��������������-���õ����һ·��
void  CalculationFormula_LoadC(float C);//�������ط��õ�ʵ���̳�Ũ��	
void  CalculationFormula_LoadXsw(float Xsw); //����ʪ�ȣ���ʽ���/������ /���ݷ���ͨ�ţ�


float  CalculationFormula_Vnd(float Tj,float V,float Ba,float Ps);  //�����������������λL��
float  CalculationFormula_p(float Ts,float Ba,float Ps);//���������ܶ�(��λkg/m^3)
float  CalculationFormula_Vs(float Kp,float Pd,float p);//������������(��λ)
float  CalculationFormula_Qs(float Vs,float F);//������������(��λm^3/h)
float  CalculationFormula_Xsw(float Pbv,float delT,float sumPb);//���㺬ʪ��(��λ%)
float  CalculationFormula_Qsnd(float Qs,float Ts,float sumPs,float Xsw);//��������������(��λm^3/h)
float  CalculationFormula_d(float Qrs,float Vs,float Xsw);//���������ֱ��d����λmm)  Ԥ�⹤��ʱ��������
float  CalculationFormula_Qrs(float d,float Vs,float Xsw);//������ٲ�������Qrs(��λL/min)  -----���ٲ���ʱ������Ҫ�Ĳ������� 
float  CalculationFormula_Pt(float Ps,float Pd,float Kp);//��������ȫѹ��Kpa��
float  CalculationFormula_Ps(float Pt,float Pd,float Kp);//����������ѹ��Kpa��
float  CalculationFormula_Phv(float Tb);//��������������ѹ��Kpa��

float  CalculationFormula_a(float O2); //������ʣϵ��	
float  CalculationFormula_Ca(float C,float a,float a1,float K);//��������Ũ��
float  CalculationFormula_G(float C,float Qsnd);//�����̳��ŷ�����
float  CalculationFormula_C(float g1,float g2,float Vnd);      //�����̳�Ũ��
float  CalculationFormula_Cc(float Cs,float V,float VO);       //��������Ũ��
float  CalculationFormula_Yyzs(float Cc,float Q,u16 N);//������������  
float  CalculationFormula_ZdK(float Sv,float Tv,float B,float YK);//�Զ�����Kֵ
void  CalculationFormula_CaALL(float *PV,float a1,float Qsnd,float clk);  //������������������Ũ�����ŷ���
float  CalculationFormula_NOX(float NO,float NO2); //��NO��NO2����NOXŨ��
void CalculationFormula_SO2FomCO(float SO2,float CO); //��CO�����SO2�ĸ��š�
void CalculationFormula_SO2FomNO2(float SO2,float NO2); //��CO�����SO2�ĸ��š�
void CalculationFormula_SO2END(void); //��������SO2���
float CalcAdcToTemperature(float adcVal);//�����¶�

/*
	�������� float  CalculationFormula_G(void)	
	������	 װ�ؼ�������Ĳ���
	˵����  �ϲ㺯������ֱ�ӵ��øú�����Ϊ����   --����ӿ�
  
*/
void  CalculationFormula_All(void);

/*
�������� void  CalculationGas(u8 type)	
	������	�������庬��
	˵����  Type=1 --O2 2--SO2  3--NO  4--NO2  5--CO  6--H2S  7---CO2
  
*/

void  CalculationGas(void)	;
#endif



