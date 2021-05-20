/*
 CalculationFormula.c 文件
 
 用于YLB-3330 公式计算
 
 得到计算后的值

*/
#include "CalculationFormula.h"
#include "math.h"
Calculation_FormulaStructure C_F_Structure;
/*
	函数名： CalculationFormulaStructure *Project_GetCalculationFormular(void)
	描述：	 	返回计算公式变量结构体的地址，其内容包括
	说明：   无				

*/
Calculation_FormulaStructure *CalculationFormula_GetData(void)
{
	return &C_F_Structure;
}
//---
/*
void  CalculationFormula_LoadfromSlave(void)	
	描述： 从副板装载传感器数据
说明：   
         Ps: 副板烟气静压
         Pd:副板烟气动压
         Pcs: 副板烟尘静压
         Pcd:副板烟尘动压

         Ba:副板大气压
         Tc:副板干球温度
         Tb:副板湿球温度
*/
void  CalculationFormula_LoadfromSlave(void)	
{
	  FourBytes four;
	  Slave_DataStructure* Sp=Slave_GetData();
	  Flashisp_DataStructure* Fp=Flashisp_GetData();
	 /*压力值装载*/
		C_F_Structure.Pt=Sp->AGPress.Value;  //全压KPa    ，IC读出的就是全压

		C_F_Structure.Pd=Sp->ACPress.Value;  //动压  pa
		C_F_Structure.Pcs=Sp->BGPress.Value; //尘计压20200602   Kpa
	
	    C_F_Structure.Pb=Sp->BGPress.Value;  //湿球表面气压  单位KPa         -----等同于静压  ？？？？？还有点疑问的
	
		C_F_Structure.Pcd=Sp->BCPress.Value; //尘动压20200602   Pa
	   
	    C_F_Structure.Pqd=Sp->CCPress.Value; //气流压pa
	
	 	if(Fp->Data[FLASHIS_AdrXTSZBOX]&0x01) //输入大气压Kpa
		{
			four.U16Data[0]=Fp->Data[FLASHIS_AdrXTSZDQY];
			four.U16Data[1]=Fp->Data[FLASHIS_AdrXTSZDQY+1];
		    C_F_Structure.Ba=four.FloatData;
		}else //测量
		{
		  C_F_Structure.Ba=Sp->DQPress.Value;
		}
		/*温度装载*/
		C_F_Structure.Tb=Sp->SQTem.Value; //湿球温度
		C_F_Structure.Tc=Sp->GQTem.Value; //干球温度
		//判断烟温是用输入值，还是采样值
		if(Fp->Data[FLASHIS_AdrGKYC_YWCL_FS]==0)  //测量
		{
		C_F_Structure.Ts=Sp->YWTem.Value; //烟温温度
		}
		else         //输入值
		{
			four.U16Data[0]=Fp->Data[FLASHIS_AdrGKYC_YWCL_SRYW];
			four.U16Data[1]=Fp->Data[FLASHIS_AdrGKYC_YWCL_SRYW+1];
		    C_F_Structure.Ts=four.FloatData;
		}
		C_F_Structure.Th=Sp->HWTem.Value; //环温
		C_F_Structure.Tj=Sp->JWTem.Value; //计温
		
		four.U16Data[0]=Fp->Data[FLASHIS_AdrYDBD_MJ];
		four.U16Data[1]=Fp->Data[FLASHIS_AdrYDBD_MJ+1];
		C_F_Structure.F=four.FloatData;//烟道面积
		four.U16Data[0]=Fp->Data[FLASHIS_AdrWHJZ_XTSZ_PTG_KP];
		four.U16Data[1]=Fp->Data[FLASHIS_AdrWHJZ_XTSZ_PTG_KP+1];
		C_F_Structure.Kp=four.FloatData;//皮托管系数
	
		CalculationFormula_Ps(C_F_Structure.Pt,C_F_Structure.Pd,C_F_Structure.Kp);//计算获得烟气静压
		CalculationFormula_Phv(C_F_Structure.Tb);//计算获得湿球表面压力
}
//---

/*
	函数名： void  CalculationFormula_LoadVfromFlux(float v)	
	描述： 从Flux 模块加载烟尘采样采样体积,计算标况体积前调用
          
说明：v: 烟尘采样的体积 
         
*/
void  CalculationFormula_LoadVfromFlux(float v)	
{
		C_F_Structure.V=v;
}

//---
/*
	函数名： void  CalculationFormula_LoadQrsfromFlux(float Qrs)	
	描述： 从Flux 得到等速采样的流量（实测烟气采样流量-不用电机那一路）
          
  说明：Qrs: 等速采样的流量 
         
*/
void  CalculationFormula_LoadQrsfromFlux(float Qrs)	
{
		C_F_Structure.Qrs=Qrs;
}

//---
/*
	函数名： void  CalculationFormula_LoadC(float C)	
	描述： 从其他地方得到实测烟尘浓度
          
  说明：C: 实测烟尘浓度 
         
*/
void  CalculationFormula_LoadC(float C)	
{
		C_F_Structure.C=C;
}

//---
/*
	函数名： void  CalculationFormula_LoadXsw(float Xsw)	
	描述： 烟气湿度：公式求得/或输入 /阻容法（通信）
          
  说明：Xsw: 烟气湿度
         
*/
void  CalculationFormula_LoadXsw(float Xsw)	
{
		C_F_Structure.Xsw=Xsw/100;   //百分数变小数
}
//---
/*
	函数名： CalculationFormula_Vnd(float Tj,float V,float Ba,float Ps)
	描述：	 计算标况采样体积（单位L）
  说明：   Tj:计温(单位0C)--由副板检查烟温IC 提供，读副板传上来的温度数据（计温）
           V:实际采样体积(单位L)--由采样过程中检测的平均流量与检测所用时间的积得来的
           Ba:大气压(单位KPa)-由副板上传的大气压数据得到
           Ps:静压（单位KPa）-由副板数据得到（尘静压）
  
*/
float  CalculationFormula_Vnd(float Tj,float V,float Ba,float Ps)
{
	  float Vnd=(float)(V*(273/(273+Tj))*((Ba+Ps)/101.325f));
	  C_F_Structure.Vnd=Vnd;
	  return Vnd;
}
/*
	函数名： CalculationFormula_Pt(float Ps,float Pd,float Kp)
	描述：	 计算烟气全压（Kpa）
  说明：   Ps:烟气静压（单位KPa）------由副板提供
           Pd:烟气动压（单位Pa）-------由副板提供
           Kp：皮托管系数（取值0.83~0.84）  
*/
float  CalculationFormula_Pt(float Ps,float Pd,float Kp)
{
	 float Pt=Ps+(Pd*pow(Kp,2))/1000;                                     //20200602
	 C_F_Structure.Pt=Pt;
	 return Pt;
}

/*
	函数名： CalculationFormula_Ps(float Pt,float Pd,float Kp)               
	描述：	 计算烟气静压（Kpa）
  说明：   Pt:烟气全压（单位KPa）------由副板提供
           Pd:烟气动压（单位Pa）-------由副板提供
           Kp：皮托管系数（取值0.83~0.84）  
*/
float  CalculationFormula_Ps(float Pt,float Pd,float Kp)
{
	 float Ps=Pt-(Pd*pow(Kp,2))/1000;                                     //20200602
	 C_F_Structure.Ps=Ps;
	 return Ps;
}

//---
/*
	函数名： CalculationFormula_p(float Ts,float Ba,float Ps)
	描述：	 计算烟气密度(单位kg/m^3),他作为计算烟气流速的依据
	说明：   Ts：烟温(单位0C)--由副板提供
           Ba: 大气压(单位KPa)--由副板提供
           Ps: 烟气静压(单位KPa)--由副板提供
           
*/
float  CalculationFormula_p(float Ts,float Ba,float Ps)
{   float p;
	  p=1.34f*(273/(273+Ts))*((Ba+Ps)/101.325f);
	  C_F_Structure.p=p;
	  return  (p);
}
//---
/*
	函数名： CalculationFormula_Vs(float Kp,float Pd,float p)
	描述：	计算烟气流速(单位)
  说明：  Pd:烟气动压（单未Pa)---由副板提供
          Kp:皮托管系数	(取0.83~0.84）
          p:烟气密度(单位KPa)--由上面公式提供
  
*/

float  CalculationFormula_Vs(float Kp,float Pd,float p)	
{   float Vs;
	float pbuf=(u32)Pd;  //去除小数
	Vs=(float)1.414*Kp*(sqrt(pbuf/p));
	C_F_Structure.Vs=Vs;
	return Vs;
}
//---
/*
	函数名： float  CalculationFormula_Qs(float Vs,float F)	
	描述：	计算烟气流量(单位m^3/h)：这个是烟气管道的流量
  说明：  F:烟道截面积(m^2)----在设置里取出
          Vs:烟气流速(m/s)----由上面的烟气流速公式提供
*/
float  CalculationFormula_Qs(float Vs,float F)	
{     float Qs;
	  Qs=Vs*F*3600;
	  C_F_Structure.Qs=Qs;
	  return Qs;
}


//---
/*
	函数名： float  CalculationFormula_Phv(float Tb)	
	描述：	计算饱和水蒸气压力
  说明：  Pbv:饱和水蒸气压力（单位KPa）,根据Tb温度值查表
          20      30      40     50     60     70      80     90
          2338    4243    7376   12334  19916  31157   47342  70095
          Tb为湿球温度
        
 
*/
float  CalculationFormula_Phv(float Tb)
{
	 float Phv;
	      if(Tb>=90)
			Phv=70095;
			else if(Tb>=80)
	        Phv=47342;
				else if(Tb>=70)
	        Phv=31157;
				else if(Tb>=60)
	        Phv=19916;
				else if(Tb>=50)
	        Phv=12334;
				else if(Tb>=40)
	        Phv=7376;
				else if(Tb>=30)
	        Phv=4243;
				else if(Tb>=20)
	        Phv=2338;
				else
			Phv=2338;
			Phv=Phv/1000;	
	        C_F_Structure.Phv=Phv;
			return Phv;
}
//---
/*
	函数名： float  CalculationFormula_Xsw(float Pbv,float delT,float sumPb,float sumPs)	
	描述：	计算含湿量(单位%)参考https://www.docin.com/p-835056561.html
  说明：  Pbv:饱和水蒸气压力（单位KPa）,根据Tb温度值查表
          20      30      40     50     60     70      80     90
          2338    4243    7376   12334  19916  31157   47342  70095
          delT: 干湿球温差(Tc-Tb) (单位0C)  ;Tc为干球温度;Tb为湿球温度
          sumPb: 湿球表面负压Pb(用计压来代替)与大气压Ba之和(单位KPa)
 
*/
float  CalculationFormula_Xsw(float Pbv,float delT,float sumPb)	
{   float Xsw;
	if(sumPb==0) return 0;
	else 
	{
	Xsw=(Pbv-0.00066f*delT*(sumPb-Pbv))*100.0f/sumPb;
	
	 // C_F_Structure.Xsw=Xsw;
	return Xsw;
    }
	}
//---
/*
	函数名： float  CalculationFormula_Qsn(float Qs,float Ts,float sumPs,float Xsw)	
	描述：	计算标杆烟气流量(单位m^3/h)
  说明：  Ts:烟气温度(单位0C)---由副板提供
          QS:烟气流量(单位m^3/h),由流速与截面求的（参考公式）
          sumPs: 烟气静压Ps与大气压Ba之和(单位KPa)
          Xsw:为含湿量(单位%)，取输入值或测量值（公式求得）
  
*/
float  CalculationFormula_Qsnd(float Qs,float Ts,float sumPs,float Xsw)	
{   float Qsnd;
	  Qsnd=Qs*(273/(273+Ts))*(sumPs/101.325f)*(1-Xsw);
	  C_F_Structure.Qsnd=Qsnd;
	  return Qsnd;
}
//---
/*
	函数名： float  CalculationFormula_d(void)	
	描述：	计算采样嘴直径d（单位mm)----在预测工况时用来计算采样嘴直径
  说明：  Qrs:等速采样流量(单位L/min) ----等速采样时的流量平均值
          Vs:烟气流速（单位m/S） ----由烟气流速公式求出
          Xsw:含湿量（单位%）----由含湿量公式求得，或采用手动输入的值
  
*/
float  CalculationFormula_d(float Qrs,float Vs,float Xsw)	
{   float d_out;
	  if((Vs==0)||(Xsw==1))
		  return 0;
	  else
	  {
	  d_out=sqrt(Qrs/(0.047f*Vs*(1-Xsw)));
	  if(d_out<4.5f)
  	  d_out=4;
	  else if(d_out<5.5f)
	  d_out=5;
	  else if(d_out<6.5f)
	  d_out=6;
	  else if(d_out<7.5f)
	  d_out=7;
	  else if(d_out<8.5f)
	  d_out=8;	  
	  else if(d_out<9.5f)
	  d_out=9;
	  else if(d_out<10.5f)
	  d_out=10;
	  else if(d_out<11.5f)
	  d_out=11;	  
	  else 
	  d_out=12;
	  C_F_Structure.d_out=d_out;
	  return d_out;
	  }
}
//---
/*
	函数名： float  CalculationFormula_Qrs(float d,float Vs,float Xsw)	
	描述：	计算等速采样流量Qrs(单位L/min)----在
  说明：  d_in:采样嘴直径（单位mm)  ----采样嘴直径由输入界面录入
          Vs:烟气流速（单位m/S） ----由烟气流速公式求出
          Xsw:含湿量（单位%）----由含湿量公式求得，或采用手动输入的值
 
*/
float  CalculationFormula_Qrs(float d,float Vs,float Xsw)	
{     float Qrs;
	  Qrs=pow(d,2)*(0.047f*Vs*(1-Xsw));
	  C_F_Structure.Qrs=Qrs;
	  return Qrs;
}
//---
/*
	函数名： float  CalculationFormula_C(float g1,float g2,float Vnd)	
	描述：	计算烟尘浓度(单位mg/m^3)
  说明：  g2: 滤筒终量(单位g)
          g1: 滤筒初重(单位g)
          Vnd:标况采样体积(单位L)
*/
float  CalculationFormula_C(float g1,float g2,float Vnd)	
{   float C;
	  if(Vnd==0) return 0;
	  else
	  {
	  C=pow(10,6)*(g2-g1)/Vnd;
	  C_F_Structure.C=C;
	  return C;
	  }
}

//---
/*
	函数名： float  CalculationFormula_Cc(float Cs,float V,float V0)	
	描述：	计算油烟浓度(单位mg/m^3)
  说明：  Cs: 滤筒清洗液浓度mg/L
          V: 定容体积mL
          VO:标准状态下干烟气采样体积m^3
*/
float  CalculationFormula_Cc(float Cs,float V,float VO)	
{   float Cc;
	  if(VO==0) return 0;
	  else
	  {
	  Cc=(Cs*V)/(1000*VO);
	  C_F_Structure.C=Cc;
	  return Cc;
	  }
}
//---
/*
	函数名： CalculationFormula_Yyzs(float Cc,float Q,u16 N);//计算油烟折算 
	描述：	计算油烟折算(单位mg/m^3)
    说明：  Cc: 油烟浓度(单位mg/m^3)
            Q: 烟气流量(单位m^3/h)
            N:灶头数目
*/
float  CalculationFormula_Yyzs(float Cc,float Q,u16 N)//计算油烟折算 
 {
	 float Yyzs;
	 Yyzs=Cc*Q;
	 if (N<=1) N=1;
	 Yyzs=Yyzs/N;
	 Yyzs=Yyzs/2000;   //2000 m^3/h
	 return Yyzs;
 }

//---
/*
	函数名： float  CalculationFormula_a(void)	
	描述：	计算空气过剩系数
	说明：  O2氧气含量 (单位 %)
*/
float  CalculationFormula_a(float O2)	
{    float a;
    if(O2==21)
	  {
	  a=1000000;  //分母不能为零 
	  }
	  else
	  {
	  a=21/(21-O2);
	  }
	  C_F_Structure.a=a;
	  return  a;
}
//---
/*
	函数名： float  CalculationFormula_G(float C,float Qsnd)	
描述：	G:计算烟尘排放总量(单位kg/h)
说明：  C:实测烟尘浓度mg/M^3
        Qsnd:标干烟气流量 M^3/H
*/
float  CalculationFormula_G(float C,float Qsnd)	
{   float G;
	G=C*Qsnd*0.000001f;
	return  G;
}
//---
/*
	函数名： float  CalculationFormula_Ca(float C,float a,float a1,float K)	
	描述：	计算烟尘折算浓度（单位mg/m^3）
  说明：C:实制烟尘浓度（单位mg/m^3)
        a:空气过剩系数---由公式计算
        a1:排放标准中规定的过量空气系数；   -----折算系数输入
        K:锅炉出力系数                      -----也叫负荷系数，由其他输入页面提供

*/
float  CalculationFormula_Ca(float C,float a,float a1,float K)	
{    
	  float Ca=C*(a/a1)*K;
	  return  Ca;
}
//---
/*
	函数名： float  CalculationFormula_NOX(float NO,float NO2)	
	描述：计算NOX
    说明：NO:浓度（单位mg/m^3)
          NO2:浓度（单位mg/m^3)
*/

float  CalculationFormula_NOX(float NO,float NO2) //由NO与NO2计算NOX浓度
{   float NOX=NO*1.5333f+NO2;
	return(NOX);
}
/*
	函数名： float  CalculationFormula_G(void)	
	描述：	 装载计算所需的参数
	说明：  上层函数可以直接调用该函数作为计算   --对外接口
  
*/
void  CalculationFormula_All(void)	
{  
	FourBytes four;
	Slave_DataStructure* Sp=Slave_GetData();
	Flashisp_DataStructure* Fp=Flashisp_GetData();
	
	CalculationFormula_LoadfromSlave();//参数装载
	
	if((Fp->Data[FLASHIS_AdrXTSZBOX]&0x40)==0)
	CalculationFormula_p(C_F_Structure.Ts,C_F_Structure.Ba,C_F_Structure.Ps);//烟气密度
	else
	CalculationFormula_p(C_F_Structure.Th,C_F_Structure.Ba,0.0);//烟气密度
	
	CalculationFormula_Vs(C_F_Structure.Kp,C_F_Structure.Pd,C_F_Structure.p);//烟气流速
	CalculationFormula_Qs(C_F_Structure.Vs,C_F_Structure.F);//计算烟气流量

	four.U16Data[0]=Flashisp_GetData()->Data[FLASHIS_AdrCHSL_HSL];
	four.U16Data[1]=Flashisp_GetData()->Data[FLASHIS_AdrCHSL_HSL+1];
	
	CalculationFormula_LoadXsw(four.FloatData);
	CalculationFormula_Qsnd(C_F_Structure.Qs,C_F_Structure.Ts,C_F_Structure.Ba +C_F_Structure.Ps,C_F_Structure.Xsw);//标杆流量
    CalculationFormula_a(Sp->O2Concent.Value);//由氧气百分比计算空气过剩系数

}
//---

/*
	函数名： CalculationFormula_CaALL(u8 mode)	
	描述：	计算所有其他的折算浓度
说明：  a1:折算系数
Qsnd:烟气的标杆流量
&PV :几种其他的采样值地址
clk :出力系数
*/

void  CalculationFormula_CaALL(float *PV,float a1,float Qsnd,float clk)  //计算所有其他的折算浓度
{ float buf1;
  buf1=a1;
	Slave_DataStructure* Sp=Slave_GetData();
	Flashisp_DataStructure* Fp=Flashisp_GetData();

   C_F_Structure.CaofSO2= CalculationFormula_Ca(PV[0],C_F_Structure.a,buf1,clk);  //计算折算浓度
	 C_F_Structure.CaofNO= CalculationFormula_Ca(PV[1],C_F_Structure.a,buf1,clk);
	 C_F_Structure.CaofNO2= CalculationFormula_Ca(PV[2],C_F_Structure.a,buf1,clk);
	 C_F_Structure.CaofCO= CalculationFormula_Ca(PV[3],C_F_Structure.a,buf1,clk);
	 C_F_Structure.CaofH2S= CalculationFormula_Ca(PV[4],C_F_Structure.a,buf1,clk);
	  //计算排放量
	C_F_Structure.GaofSO2=CalculationFormula_G(PV[0],Qsnd);
	C_F_Structure.GaofNO=CalculationFormula_G(PV[1],Qsnd);
	C_F_Structure.GaofNO2=CalculationFormula_G(PV[2],Qsnd);
	C_F_Structure.GaofCO=CalculationFormula_G(PV[3],Qsnd);
	C_F_Structure.GaofH2S=CalculationFormula_G(PV[4],Qsnd);
	
}
/*
   函数名： void  Calculation(void)
   描述：计算曲线上以标定点为中心的偏移位置点浓度
   输入：calibration1：较大的标定值
         calibration0：较小标定值
         percentage：百分数 <1
        
   输出：偏移位置点浓度

*/
float CAL_DiscreteConcentration(float calibration0,float calibration1,float percentage)
{
	return(calibration0+(calibration1-calibration0)*percentage);
}

/*
   函数名： void  Calculation(void)
   描述：计算曲线上以标定点为中心的偏移位置点浓度的原始值Y=KX+B ,X=(Y-B)/K
   输入：calibration1：较大的标定值
         calibration0：较小标定值
         percentage：百分数 <1
   输出：偏移位置点浓度原始值
*/
float CAL_X(float Y,float B,float K)
{
	return((Y-B)/K);
}

/*
   函数名： void  Calculationsinglygas
   描述：
   输入：p为气体指针
         LC为量程
   输出：偏移位置点浓度原始值
*/
void Calculationsinglygas(Slave_TPActualStructure *p,u32 LC)
  {
	Flashisp_DataStructure* pf=Flashisp_GetData();
	FourBytes		four;
	float K0buf,K2buf,K4buf;  //
	
		K0buf=p->K1;                      //低
		K2buf=p->K;                       //中
		K4buf=p->K2;                      //高
		//根据新K值求各段的值

		p->Value0=p->Value0 * K2buf + p->B;  //中 ,以中浓度为准
		p->Value1=p->Value0 * K0buf;  //低
		p->Value2=p->Value0* K4buf;  //高
		
	    four.U16Data[0]=pf->Data[LC];
		four.U16Data[1]=pf->Data[LC+1];
	  
	    if(p->Value0<=four.U32Data/3)
	    p->Value=p->Value1;
		else if(p->Value0<=(four.U32Data*2/3))
		p->Value=p->Value0;
		else
		p->Value=p->Value2;

  }
  
  /*
   函数名： void  Calculationsinglygas
   描述：
   输入：p为气体指针
         LC为量程
   输出：偏移位置点浓度原始值
*/
void CalculationsinglyO2(Slave_TPActualStructure *p,u32 LC)
  {
	Flashisp_DataStructure* pf=Flashisp_GetData();
	FourBytes		four;
	float K0buf,K2buf,K4buf;  //
	
		K0buf=p->K1;                      //低
		K2buf=p->K;                       //中
		K4buf=p->K2;                      //高
		//根据新K值求各段的值

		p->Value0=p->Value0 * K2buf + p->B;  //中 ,以中浓度为准
	  
	    if((p->Value0>=20)&&(p->Value0<=22))
		{
			p->Value1=p->Value0;
			p->Value2=p->Value0;
		}
		else
		{
		p->Value1=p->Value0 * K0buf;  //低
		p->Value2=p->Value0* K4buf;  //高
		}
		
	    four.U16Data[0]=pf->Data[LC];
		four.U16Data[1]=pf->Data[LC+1];
	  
	   
	    if(p->Value0<=four.U32Data/3)
	    p->Value=p->Value1;
		else if(p->Value0<=(four.U32Data*2/3))
		p->Value=p->Value0;
		else
		p->Value=p->Value2;

  }


/*
函数名： void  CalculationGas(void)	
	描述：	计算气体含量
	说明：  Type=1 --O2 2--SO2  3--NO  4--NO2  5--CO  6--H2S  7---CO2
  
*/
//
#include "ScreenDwFunction.h"
//分段矫正
const float GasDivide[7][2]={
{10,20},
{1881,2763},
{429,858},
{66,132},
{1650,3300},
{99,198},
{7,13},
};

void  CalculationGas(void)	
{ 
	Slave_DataStructure* Sp=Slave_GetData();
    u16 QTCGQXZ=Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_QTCGQXZ];
	Flashisp_DataStructure* pf=Flashisp_GetData();
//	FourBytes		four;
	float K;

	  if(QTCGQXZ&(1<<0))
		{
		  K=4.24*1870; //4.24=1/ln(1.266) , 1.266=1/(1-0.21), ad=1870 ,用氧气浓度21%调试出K
		  Sp->O2Concent.Value=(1-1.0/pow((float)2.718281828,Sp->Data[SLAVE_MAP_O2Concent].Raw/K))*100;
		  Sp->O2Concent.Value0=Sp->O2Concent.Value;
		  Sp->O2Concent.Value1=Sp->O2Concent.Value;
		  Sp->O2Concent.Value2=Sp->O2Concent.Value;
		//*****************************5段曲线值算法************************
		 CalculationsinglyO2(&Sp->O2Concent,FLASHIS_AdrO2LQ);
		}else
		{ Sp->O2Concent.Value=SLAVE_ColFAULT;
		  Sp->O2Concent.Value0=SLAVE_ColFAULT;
		  Sp->O2Concent.Value1=SLAVE_ColFAULT;
		  Sp->O2Concent.Value2=SLAVE_ColFAULT;
	 	}
//SO2
	  if((QTCGQXZ&(1<<1))==0x00)
		{
		  Sp->SO2Concent.Value=SLAVE_ColFAULT;
		  Sp->SO2Concent.Value0=SLAVE_ColFAULT;
		  Sp->SO2Concent.Value1=SLAVE_ColFAULT;
		  Sp->SO2Concent.Value2=SLAVE_ColFAULT;
			
		}else
		{ //SO2:1PPM=2.86mg/m^3
		K=  Vref*1992/4096;  //高浓度 (PPM) ,1992是根据电路板的放大倍数得到510R+2K
    //  K=  Vref*217/4096;	 //低浓度(PPM)  ,217是根据电路板的放大倍数得到 3K+20K
	    Sp->SO2Concent.Value=Sp->Data[SLAVE_MAP_SO2Concent].Raw*K*Ppmtomgm3_SO2;
			
			
		Sp->SO2Concent.Value0=Sp->SO2Concent.Value;
		Sp->SO2Concent.Value1=Sp->SO2Concent.Value;
		Sp->SO2Concent.Value2=Sp->SO2Concent.Value;
			
       
	  //*****************************5段曲线值算法************************
		Calculationsinglygas(&Sp->SO2Concent,FLASHIS_AdrSO2LQ);
			

			
      //***************************************************************			

		}
//NO
	  if((QTCGQXZ&(1<<2))==0x00)
		{
		  Sp->NOConcent.Value=SLAVE_ColFAULT;
		  Sp->NOConcent.Value0=SLAVE_ColFAULT;
		  Sp->NOConcent.Value1=SLAVE_ColFAULT;
		  Sp->NOConcent.Value2=SLAVE_ColFAULT;
			
		}else
		{//NO: 1PPM=1.339mg/m^3
			K=  Vref/(4096*0.0012f);	 //
			Sp->NOConcent.Value=(Sp->Data[SLAVE_MAP_NOConcent].Raw*K-583)*Ppmtomgm3_NO;
			Sp->NOConcent.Value0=Sp->NOConcent.Value;
			Sp->NOConcent.Value1=Sp->NOConcent.Value;
			Sp->NOConcent.Value2=Sp->NOConcent.Value;

	  //*****************************5段曲线值算法************************
       	Calculationsinglygas(&Sp->NOConcent,FLASHIS_AdrNOLQ);

      //***************************************************************					
			

		}
//NO2
		if((QTCGQXZ&(1<<3))==0x00)
		{
		  Sp->NO2Concent.Value=SLAVE_ColFAULT;
		  Sp->NO2Concent.Value0=SLAVE_ColFAULT;
		  Sp->NO2Concent.Value1=SLAVE_ColFAULT;
		  Sp->NO2Concent.Value2=SLAVE_ColFAULT;

		}else
		{
		//NO2: 1PPM=2.05mg/m^3
		//ppm=52.47*Vref(3v)*ad/4096 -24.24 ,(51k+510欧电阻)
		//ppm=131.8*Vref(3v)*ad/4096 -60.89 ,(20k+510欧电阻)
		K=Vref*131.8f/4096;
		Sp->NO2Concent.Value=(Sp->Data[SLAVE_MAP_NO2Concent].Raw*K-60.89f)*Ppmtomgm3_NO2;
		Sp->NO2Concent.Value0=Sp->NO2Concent.Value;
		Sp->NO2Concent.Value1=Sp->NO2Concent.Value;
		Sp->NO2Concent.Value2=Sp->NO2Concent.Value;

			  //*****************************5段曲线值算法************************
        Calculationsinglygas(&Sp->NO2Concent,FLASHIS_AdrNO2LQ);

      //***************************************************************			
	

		}

//CO	
		if((QTCGQXZ&(1<<4))==0x00)
		{
		  Sp->COConcent.Value=SLAVE_ColFAULT;
		  Sp->COConcent.Value0=SLAVE_ColFAULT;
		  Sp->COConcent.Value1=SLAVE_ColFAULT;
		  Sp->COConcent.Value2=SLAVE_ColFAULT;

		}else
		{
			//co: 1PPM=1.25mg/m^3
		 K=Vref*6640/4096;                  //K=Vref*6640/4096;
	     Sp->COConcent.Value=Sp->Data[SLAVE_MAP_COConcent].Raw*K*Ppmtomgm3_CO;
		 Sp->COConcent.Value0=Sp->COConcent.Value;			
		 Sp->COConcent.Value1=Sp->COConcent.Value;
		 Sp->COConcent.Value2=Sp->COConcent.Value;
		
		//*****************************5段曲线值算法************************
        Calculationsinglygas(&Sp->COConcent,FLASHIS_AdrCOLQ);

      //***************************************************************	
	
	
		}
//H2S
		if((QTCGQXZ&(1<<5))==0x00)
		{
		  Sp->H2SConcent.Value=SLAVE_ColFAULT;
		  Sp->H2SConcent.Value0=SLAVE_ColFAULT;
		  Sp->H2SConcent.Value1=SLAVE_ColFAULT;
		  Sp->H2SConcent.Value2=SLAVE_ColFAULT;
		}else
		{
			//H2S:1PPM=1.517mg/m^3
				//ppm=247.476*Vref(3v)*ad/4096 -111.56 ,(20k+5.1k电阻)
		 K=Vref*247.476f/4096;   //
		 Sp->H2SConcent.Value=(Sp->Data[SLAVE_MAP_H2SConcent].Raw*K-111.56f)*Ppmtomgm3_H2S;
		//Sp->H2SConcent.Value=Sp->Data[SLAVE_MAP_H2SConcent].Raw*1.7f;
		 Sp->H2SConcent.Value0=Sp->H2SConcent.Value;	
		 Sp->H2SConcent.Value1=Sp->H2SConcent.Value;
		 Sp->H2SConcent.Value2=Sp->H2SConcent.Value;
			
		//*****************************5段曲线值算法************************
       	 Calculationsinglygas(&Sp->H2SConcent,FLASHIS_AdrH2SLQ);

      //***************************************************************	
			
		}
//CO2	
		if((QTCGQXZ&(1<<6))==0x00)
		{
		  Sp->CO2Concent.Value=SLAVE_ColFAULT;
		  Sp->CO2Concent.Value0=SLAVE_ColFAULT;
		  Sp->CO2Concent.Value1=SLAVE_ColFAULT;
		  Sp->CO2Concent.Value2=SLAVE_ColFAULT;
			
		}else
		{
		 //(vout-0.4)/(2-0.4)*20
		 K=Vref/4096;	
		 Sp->CO2Concent.Value=(Sp->Data[SLAVE_MAP_CO2Concent].Raw*K-0.4f)/1.6f*20;  //量程0~20
		 if(Sp->CO2Concent.Value>=100)Sp->CO2Concent.Value=20;
		 Sp->CO2Concent.Value0=Sp->CO2Concent.Value;
		 Sp->CO2Concent.Value1=Sp->CO2Concent.Value;
		 Sp->CO2Concent.Value2=Sp->CO2Concent.Value;
			
	     //*****************************5段曲线值算法************************
        Calculationsinglygas(&Sp->CO2Concent,FLASHIS_AdrCO2LQ);

      //***************************************************************	
			
		}
		//计算倍率后数据
		/*
		Slave_TPActualStructure * BeforGasp=&Sp->O2Concent;//倍率前
		Slave_TPActualStructure *Gasp=&Sp->O2Concent;//倍率后
		for(int i=0;i<7;i++)
		{
			 if(BeforGasp[i].Value==SLAVE_ColFAULT)
			 {
			    Gasp[i].Value=BeforGasp[i].Value;
			 }else if(BeforGasp[i].Value<=GasDivide[i][0])
			 {
			   Gasp[i].Value=BeforGasp[i].Value*GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_NDJZ_BLO2_1+i*2);
			 }else if(BeforGasp[i].Value<=GasDivide[i][1])
			 {
			    Gasp[i].Value=BeforGasp[i].Value*GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_NDJZ_BLO2_2+i*2);
			 }else
			 {
			    Gasp[i].Value=BeforGasp[i].Value*GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_NDJZ_BLO2_3+i*2);
			 }
			 
		}
		*/
//NOx	  
		if((QTCGQXZ&(1<<2))==0x00||(QTCGQXZ&(1<<3))==0x00)
		{
		  Sp->NOxConcent.Value=SLAVE_ColFAULT;
		}else
		{
		Sp->NOxConcent.Value=	CalculationFormula_NOX(Sp->NOConcent.Value,Sp->NO2Concent.Value);
		}
		
// CO对SO2的修正
		CalculationFormula_SO2FomCO(Sp->SO2Concent.Value,Sp->COConcent.Value);
		
//NO2对SO2的修正		
		
		
}

//---

/*
函数名： CalculationFormula_ZdK(float Sv,float Tv,float B,float YK)
	描述：	自动计算K值
	说明：  Sv :手动输入的标量值，
	Tv :  测出计算的量值
	B  :偏移量
	YK :原来计算的K值
  
*/
float  CalculationFormula_ZdK(float Sv,float Tv,float B,float YK)//
{   float buf;
	if(Tv==B)
    return YK;
	else
	{
	buf=(Sv-B)*YK/(Tv-B);
	return buf;
	}
}

//---
/*
函数名：  CalculationFormula_SO2FomCO(float SO2,float CO) 
	描述：	由CO计算对SO2的干扰。
	说明：  
     输入：SO2 =测量值
           CO =测量值
     输出： C_F_Structure.SO2FROMCO  ;干扰后修正
           
*/
void CalculationFormula_SO2FomCO(float SO2,float CO) //由CO计算对SO2的干扰。
{   Flashisp_DataStructure* pf=Flashisp_GetData();
	FourBytes		four;
	u16 buf;
	u8 i;
	for(i=0;i<10;i++)
	{   
	if(CO<pf->Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1+i])
	{
		break;
	}
	}
	buf=pf->Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ1+i];  //取出修正百分比（0~100）
	four.FloatData=buf;
	four.FloatData=four.FloatData*CO/10000;                          //百分比
	C_F_Structure.SO2FromCO=SO2-four.FloatData;
}

void CalculationFormula_SO2FomNO2(float SO2,float NO2) //由CO计算对SO2的干扰。
{
	
}
void CalculationFormula_SO2END(void) //计算最终SO2结果
{
	
}

/***********************************************************************
* 函数名：    CalcAdcToTemperature
* 功能描述：  由测得的ADC值求出对应的温度值。
* 输入参数：  float adcVal：测量的adc平均值

* 返回值：    计算后的温度值
***********************************************************************/
float CalcAdcToTemperature(float adcVal)
{

 float tmpValue;
 if(adcVal>1800)adcVal=1800;
 
	/*
 tmpValue = (adcVal - 1449)*(250 - 200)/(1494-1449) + 200; //200度以上
 else if(adcVal>=1409)
 tmpValue = (adcVal - 1409)*(199 - 180)/(1441-1409) + 180; // 180~199
 else if(adcVal>=1357)
 tmpValue = (adcVal - 1357)*(179 - 160)/(1403-1357) + 160;  // 160~179
 else if(adcVal>=1296)
 tmpValue = (adcVal - 1296)*(159 - 140)/(1353 - 1296) + 140; // 140~159
 else if(adcVal>=1230)
 tmpValue = (adcVal - 1230)*(139 - 120)/(1293 - 1230) + 120; //120~139
 else if(adcVal>=1163)
 tmpValue = (adcVal - 1163)*(119 - 100)/(1228 - 1163) + 100; //100~119
 else if(adcVal>=1095)
 tmpValue = (adcVal - 1095)*(99 - 80)/(1161 - 1095) + 80; //80~99
 else if(adcVal>=1027)
 tmpValue = (adcVal - 1027)*(79 - 60)/(1093 - 1027) + 60; //60~79
 else if(adcVal>=959)
 tmpValue = (adcVal - 959)*(59 - 40)/(1025 - 959) + 40; //40~59
 else if(adcVal>=891)
 tmpValue = (adcVal - 891)*(39 - 20)/(957 - 891) + 20; //20~39     
 else// if(fbuf>=845)
 */
 tmpValue = (adcVal -Res_Degree0)*(195- 0)/(Res_Degree195 - Res_Degree0) + 0; //19度以下
 
 //tmpValue = (fbuf - ADC_MIN_VALUE)*(TEMP_MAX_VALUE-TEMP_MIN_VALUE)/(ADC_MAX_VALUE - ADC_MIN_VALUE) + TEMP_MIN_VALUE; 
 return tmpValue;
}

