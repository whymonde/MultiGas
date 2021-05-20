/*
	CalculationFormula库函数
*/

#ifndef __CALCULATIONFORMULA_H
#define __CALCULATIONFORMULA_H

#include "stm32f4xx.h"

#define Vref                    (float)3.00
#define Null_Value							1000000	//未计算的值
#define Ppmtomgm3_SO2 2.86f               //定义SO2的PPM与mgm3的转换率
#define Ppmtomgm3_NO  1.339f              //定义NO的PPM与mgm3的转换率
#define Ppmtomgm3_NO2 2.05f               //定义NO2的PPM与mgm3的转换率
#define Ppmtomgm3_CO  1.25f               //定义CO的PPM与mgm3的转换率
#define Ppmtomgm3_H2S 1.517f               //定义SO2的PPM与mgm3的转换率


#define YCa1 1.8                           //国标烟尘空气过剩系数a1
#define YYa1 1.2                           //国标油烟空气过剩系数a1
/*
	系统结构体
*/
typedef struct 
{
	float F;  //烟道面积      单位 m2         -----从flash上获得
	float Kp; //皮托管系数    0.83~0.84       -----从Flash上获得	
	
	float Ba; //大气压        单位KPa         -----从副板模块中提供数据Slave_GetData()->DQPress.Value
	float Ps; //烟气静压      单位KPa         -----公式求得
	float Pd; //烟气动压（气流流压单位Pa      -----从副板模块中提供数据Slave_GetData()->ACPress.Value
	float Pcs;//烟尘静压（尘计压） KPa        -----从副板模块中提供数据Slave_GetData()->BGPress.Value
	float Pcd;//烟尘动压（尘动压） Pa         -----从副板模块中提供数据Slave_GetData()->BCPress.Value
	float Pqd;//气流压             Pa         -----从副板模块中提供数据Slave_GetData()->CCPress.Value
	float Pt; //全压压        单位KPa         -----从副板模块中提供数据Slave_GetData()->AGPress.Value
	float Pb; //湿球表面气压  单位KPa         -----等同于全压 Slave_GetData()->AGPress.Value
	float Phv; //蒸汽饱和气压 单位KPa         -----根据湿球温度查表得到(CalculationFormula_Phv(float Tb)函数）

	float Ts ; //烟气温度     单位℃          -----从副板模块中提供数据  Slave_GetData()->YWTem.Value
	float Tb ; //湿球温度     单位℃          -----从副板模块中提供数据 Slave_GetData()->SQTem.Value  （ 烟温，湿球温度同一个传感器  ）
	float Tc ; //干球温度     单位℃          -----从副板模块中提供数据Slave_GetData()->GQTem.Value
	float Th ; //环境温度     单位℃          -----从副板模块中提供数据Slave_GetData()->HJTemp.Value
	float Tj ; //计温度       单位℃          -----从副板模块中提供数据Slave_GetData()->JWTemp.Value
	
	float V ;   //实际采样体积  单位L         -----由采样过程模块（烟尘采样-求标况体积采用到）
	float Vnd ; //标况采样体积    单位L       -----公式求得
	
	float p;    //湿烟气密度       单位kg/m^3 -----公式求得
	float Vs ;  //烟气流速         单位M/s    -----公式求得
	float Qs ;  //烟气流量     单位M3/h        -----公式求得   工况计算用
	float Qsnd ; //标杆烟气流量    单位M3/h   -----公式求得
	
	float Qrs ; //采样流量用     单位L/min    -----由采样模块FLUX.C提供(工况采样-烟气流量)Flux_GetData()->ChannelA.ShowFlux
	
	float d_in ;   //采嘴直径         单位mm  -----等速采样时界面录入
	float d_out;   //采嘴直径         单位mm  -----预测工况时由公式求得
	float C   ; //实测烟尘浓度     单位M3/h   -----由另一个页面里输入烟尘净重与采样体积相比求得（就是其他模块提供吧）
	float a ;  //空气过剩系数                 -----公式求得
	float G ;  //烟尘排放量     KG/h          -----公式求得
	float Xsw ; //含湿量计算                  -----公式求得/或输入 /阻容法（通信）     
	float Ca ; //烟尘折算浓度                 -----公式求得
	float CaofSO2,CaofNO,CaofNO2,CaofCO,CaofH2S;            //计算各种气体折算浓度
	float GaofSO2,GaofNO,GaofNO2,GaofCO,GaofH2S;             //计算各种气体的排放
	float SO2FromCO,SO2FromNO2,SO2end;                       //
}Calculation_FormulaStructure;
//库函数

Calculation_FormulaStructure *CalculationFormula_GetData(void);
void  CalculationFormula_LoadfromSlave1(float Ps,float Pd,float Pcs,float Pcd);//从副板装载动静压数据
void  CalculationFormula_LoadfromSlave2(float Ba,float Tb,float Tc);//从副板装载大气压与稳定
void  CalculationFormula_LoadVfromFlux(float v);//从Flux 模块加载烟尘采样采样体积,计算标况体积前调用
void  CalculationFormula_LoadQrsfromFlux(float Qrs);//从Flux 得到等速采样的流量（实测烟气采样流量-不用电机那一路）
void  CalculationFormula_LoadC(float C);//从其他地方得到实测烟尘浓度	
void  CalculationFormula_LoadXsw(float Xsw); //烟气湿度：公式求得/或输入 /阻容法（通信）


float  CalculationFormula_Vnd(float Tj,float V,float Ba,float Ps);  //计算标况采样体积（单位L）
float  CalculationFormula_p(float Ts,float Ba,float Ps);//计算烟气密度(单位kg/m^3)
float  CalculationFormula_Vs(float Kp,float Pd,float p);//计算烟气流速(单位)
float  CalculationFormula_Qs(float Vs,float F);//计算烟气流量(单位m^3/h)
float  CalculationFormula_Xsw(float Pbv,float delT,float sumPb);//计算含湿量(单位%)
float  CalculationFormula_Qsnd(float Qs,float Ts,float sumPs,float Xsw);//计算标杆烟气流量(单位m^3/h)
float  CalculationFormula_d(float Qrs,float Vs,float Xsw);//计算采样嘴直径d（单位mm)  预测工况时用来计算
float  CalculationFormula_Qrs(float d,float Vs,float Xsw);//计算等速采样流量Qrs(单位L/min)  -----等速采样时计算需要的采样流量 
float  CalculationFormula_Pt(float Ps,float Pd,float Kp);//计算烟气全压（Kpa）
float  CalculationFormula_Ps(float Pt,float Pd,float Kp);//计算烟气静压（Kpa）
float  CalculationFormula_Phv(float Tb);//计算蒸汽饱和气压（Kpa）

float  CalculationFormula_a(float O2); //空气过剩系数	
float  CalculationFormula_Ca(float C,float a,float a1,float K);//计算折算浓度
float  CalculationFormula_G(float C,float Qsnd);//计算烟尘排放总量
float  CalculationFormula_C(float g1,float g2,float Vnd);      //计算烟尘浓度
float  CalculationFormula_Cc(float Cs,float V,float VO);       //计算油烟浓度
float  CalculationFormula_Yyzs(float Cc,float Q,u16 N);//计算油烟折算  
float  CalculationFormula_ZdK(float Sv,float Tv,float B,float YK);//自动计算K值
void  CalculationFormula_CaALL(float *PV,float a1,float Qsnd,float clk);  //计算所有其他的折算浓度与排放量
float  CalculationFormula_NOX(float NO,float NO2); //由NO与NO2计算NOX浓度
void CalculationFormula_SO2FomCO(float SO2,float CO); //由CO计算对SO2的干扰。
void CalculationFormula_SO2FomNO2(float SO2,float NO2); //由CO计算对SO2的干扰。
void CalculationFormula_SO2END(void); //计算最终SO2结果
float CalcAdcToTemperature(float adcVal);//计算温度

/*
	函数名： float  CalculationFormula_G(void)	
	描述：	 装载计算所需的参数
	说明：  上层函数可以直接调用该函数作为计算   --对外接口
  
*/
void  CalculationFormula_All(void);

/*
函数名： void  CalculationGas(u8 type)	
	描述：	计算气体含量
	说明：  Type=1 --O2 2--SO2  3--NO  4--NO2  5--CO  6--H2S  7---CO2
  
*/

void  CalculationGas(void)	;
#endif



