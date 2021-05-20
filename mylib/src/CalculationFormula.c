/*
 CalculationFormula.c �ļ�
 
 ����YLB-3330 ��ʽ����
 
 �õ�������ֵ

*/
#include "CalculationFormula.h"
#include "math.h"
Calculation_FormulaStructure C_F_Structure;
/*
	�������� CalculationFormulaStructure *Project_GetCalculationFormular(void)
	������	 	���ؼ��㹫ʽ�����ṹ��ĵ�ַ�������ݰ���
	˵����   ��				

*/
Calculation_FormulaStructure *CalculationFormula_GetData(void)
{
	return &C_F_Structure;
}
//---
/*
void  CalculationFormula_LoadfromSlave(void)	
	������ �Ӹ���װ�ش���������
˵����   
         Ps: ����������ѹ
         Pd:����������ѹ
         Pcs: �����̳���ѹ
         Pcd:�����̳���ѹ

         Ba:�������ѹ
         Tc:��������¶�
         Tb:����ʪ���¶�
*/
void  CalculationFormula_LoadfromSlave(void)	
{
	  FourBytes four;
	  Slave_DataStructure* Sp=Slave_GetData();
	  Flashisp_DataStructure* Fp=Flashisp_GetData();
	 /*ѹ��ֵװ��*/
		C_F_Structure.Pt=Sp->AGPress.Value;  //ȫѹKPa    ��IC�����ľ���ȫѹ

		C_F_Structure.Pd=Sp->ACPress.Value;  //��ѹ  pa
		C_F_Structure.Pcs=Sp->BGPress.Value; //����ѹ20200602   Kpa
	
	    C_F_Structure.Pb=Sp->BGPress.Value;  //ʪ�������ѹ  ��λKPa         -----��ͬ�ھ�ѹ  �������������е����ʵ�
	
		C_F_Structure.Pcd=Sp->BCPress.Value; //����ѹ20200602   Pa
	   
	    C_F_Structure.Pqd=Sp->CCPress.Value; //����ѹpa
	
	 	if(Fp->Data[FLASHIS_AdrXTSZBOX]&0x01) //�������ѹKpa
		{
			four.U16Data[0]=Fp->Data[FLASHIS_AdrXTSZDQY];
			four.U16Data[1]=Fp->Data[FLASHIS_AdrXTSZDQY+1];
		    C_F_Structure.Ba=four.FloatData;
		}else //����
		{
		  C_F_Structure.Ba=Sp->DQPress.Value;
		}
		/*�¶�װ��*/
		C_F_Structure.Tb=Sp->SQTem.Value; //ʪ���¶�
		C_F_Structure.Tc=Sp->GQTem.Value; //�����¶�
		//�ж�������������ֵ�����ǲ���ֵ
		if(Fp->Data[FLASHIS_AdrGKYC_YWCL_FS]==0)  //����
		{
		C_F_Structure.Ts=Sp->YWTem.Value; //�����¶�
		}
		else         //����ֵ
		{
			four.U16Data[0]=Fp->Data[FLASHIS_AdrGKYC_YWCL_SRYW];
			four.U16Data[1]=Fp->Data[FLASHIS_AdrGKYC_YWCL_SRYW+1];
		    C_F_Structure.Ts=four.FloatData;
		}
		C_F_Structure.Th=Sp->HWTem.Value; //����
		C_F_Structure.Tj=Sp->JWTem.Value; //����
		
		four.U16Data[0]=Fp->Data[FLASHIS_AdrYDBD_MJ];
		four.U16Data[1]=Fp->Data[FLASHIS_AdrYDBD_MJ+1];
		C_F_Structure.F=four.FloatData;//�̵����
		four.U16Data[0]=Fp->Data[FLASHIS_AdrWHJZ_XTSZ_PTG_KP];
		four.U16Data[1]=Fp->Data[FLASHIS_AdrWHJZ_XTSZ_PTG_KP+1];
		C_F_Structure.Kp=four.FloatData;//Ƥ�й�ϵ��
	
		CalculationFormula_Ps(C_F_Structure.Pt,C_F_Structure.Pd,C_F_Structure.Kp);//������������ѹ
		CalculationFormula_Phv(C_F_Structure.Tb);//������ʪ�����ѹ��
}
//---

/*
	�������� void  CalculationFormula_LoadVfromFlux(float v)	
	������ ��Flux ģ������̳������������,���������ǰ����
          
˵����v: �̳���������� 
         
*/
void  CalculationFormula_LoadVfromFlux(float v)	
{
		C_F_Structure.V=v;
}

//---
/*
	�������� void  CalculationFormula_LoadQrsfromFlux(float Qrs)	
	������ ��Flux �õ����ٲ�����������ʵ��������������-���õ����һ·��
          
  ˵����Qrs: ���ٲ��������� 
         
*/
void  CalculationFormula_LoadQrsfromFlux(float Qrs)	
{
		C_F_Structure.Qrs=Qrs;
}

//---
/*
	�������� void  CalculationFormula_LoadC(float C)	
	������ �������ط��õ�ʵ���̳�Ũ��
          
  ˵����C: ʵ���̳�Ũ�� 
         
*/
void  CalculationFormula_LoadC(float C)	
{
		C_F_Structure.C=C;
}

//---
/*
	�������� void  CalculationFormula_LoadXsw(float Xsw)	
	������ ����ʪ�ȣ���ʽ���/������ /���ݷ���ͨ�ţ�
          
  ˵����Xsw: ����ʪ��
         
*/
void  CalculationFormula_LoadXsw(float Xsw)	
{
		C_F_Structure.Xsw=Xsw/100;   //�ٷ�����С��
}
//---
/*
	�������� CalculationFormula_Vnd(float Tj,float V,float Ba,float Ps)
	������	 �����������������λL��
  ˵����   Tj:����(��λ0C)--�ɸ���������IC �ṩ�������崫�������¶����ݣ����£�
           V:ʵ�ʲ������(��λL)--�ɲ��������м���ƽ��������������ʱ��Ļ�������
           Ba:����ѹ(��λKPa)-�ɸ����ϴ��Ĵ���ѹ���ݵõ�
           Ps:��ѹ����λKPa��-�ɸ������ݵõ�������ѹ��
  
*/
float  CalculationFormula_Vnd(float Tj,float V,float Ba,float Ps)
{
	  float Vnd=(float)(V*(273/(273+Tj))*((Ba+Ps)/101.325f));
	  C_F_Structure.Vnd=Vnd;
	  return Vnd;
}
/*
	�������� CalculationFormula_Pt(float Ps,float Pd,float Kp)
	������	 ��������ȫѹ��Kpa��
  ˵����   Ps:������ѹ����λKPa��------�ɸ����ṩ
           Pd:������ѹ����λPa��-------�ɸ����ṩ
           Kp��Ƥ�й�ϵ����ȡֵ0.83~0.84��  
*/
float  CalculationFormula_Pt(float Ps,float Pd,float Kp)
{
	 float Pt=Ps+(Pd*pow(Kp,2))/1000;                                     //20200602
	 C_F_Structure.Pt=Pt;
	 return Pt;
}

/*
	�������� CalculationFormula_Ps(float Pt,float Pd,float Kp)               
	������	 ����������ѹ��Kpa��
  ˵����   Pt:����ȫѹ����λKPa��------�ɸ����ṩ
           Pd:������ѹ����λPa��-------�ɸ����ṩ
           Kp��Ƥ�й�ϵ����ȡֵ0.83~0.84��  
*/
float  CalculationFormula_Ps(float Pt,float Pd,float Kp)
{
	 float Ps=Pt-(Pd*pow(Kp,2))/1000;                                     //20200602
	 C_F_Structure.Ps=Ps;
	 return Ps;
}

//---
/*
	�������� CalculationFormula_p(float Ts,float Ba,float Ps)
	������	 ���������ܶ�(��λkg/m^3),����Ϊ�����������ٵ�����
	˵����   Ts������(��λ0C)--�ɸ����ṩ
           Ba: ����ѹ(��λKPa)--�ɸ����ṩ
           Ps: ������ѹ(��λKPa)--�ɸ����ṩ
           
*/
float  CalculationFormula_p(float Ts,float Ba,float Ps)
{   float p;
	  p=1.34f*(273/(273+Ts))*((Ba+Ps)/101.325f);
	  C_F_Structure.p=p;
	  return  (p);
}
//---
/*
	�������� CalculationFormula_Vs(float Kp,float Pd,float p)
	������	������������(��λ)
  ˵����  Pd:������ѹ����δPa)---�ɸ����ṩ
          Kp:Ƥ�й�ϵ��	(ȡ0.83~0.84��
          p:�����ܶ�(��λKPa)--�����湫ʽ�ṩ
  
*/

float  CalculationFormula_Vs(float Kp,float Pd,float p)	
{   float Vs;
	float pbuf=(u32)Pd;  //ȥ��С��
	Vs=(float)1.414*Kp*(sqrt(pbuf/p));
	C_F_Structure.Vs=Vs;
	return Vs;
}
//---
/*
	�������� float  CalculationFormula_Qs(float Vs,float F)	
	������	������������(��λm^3/h)������������ܵ�������
  ˵����  F:�̵������(m^2)----��������ȡ��
          Vs:��������(m/s)----��������������ٹ�ʽ�ṩ
*/
float  CalculationFormula_Qs(float Vs,float F)	
{     float Qs;
	  Qs=Vs*F*3600;
	  C_F_Structure.Qs=Qs;
	  return Qs;
}


//---
/*
	�������� float  CalculationFormula_Phv(float Tb)	
	������	���㱥��ˮ����ѹ��
  ˵����  Pbv:����ˮ����ѹ������λKPa��,����Tb�¶�ֵ���
          20      30      40     50     60     70      80     90
          2338    4243    7376   12334  19916  31157   47342  70095
          TbΪʪ���¶�
        
 
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
	�������� float  CalculationFormula_Xsw(float Pbv,float delT,float sumPb,float sumPs)	
	������	���㺬ʪ��(��λ%)�ο�https://www.docin.com/p-835056561.html
  ˵����  Pbv:����ˮ����ѹ������λKPa��,����Tb�¶�ֵ���
          20      30      40     50     60     70      80     90
          2338    4243    7376   12334  19916  31157   47342  70095
          delT: ��ʪ���²�(Tc-Tb) (��λ0C)  ;TcΪ�����¶�;TbΪʪ���¶�
          sumPb: ʪ����渺ѹPb(�ü�ѹ������)�����ѹBa֮��(��λKPa)
 
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
	�������� float  CalculationFormula_Qsn(float Qs,float Ts,float sumPs,float Xsw)	
	������	��������������(��λm^3/h)
  ˵����  Ts:�����¶�(��λ0C)---�ɸ����ṩ
          QS:��������(��λm^3/h),�������������ģ��ο���ʽ��
          sumPs: ������ѹPs�����ѹBa֮��(��λKPa)
          Xsw:Ϊ��ʪ��(��λ%)��ȡ����ֵ�����ֵ����ʽ��ã�
  
*/
float  CalculationFormula_Qsnd(float Qs,float Ts,float sumPs,float Xsw)	
{   float Qsnd;
	  Qsnd=Qs*(273/(273+Ts))*(sumPs/101.325f)*(1-Xsw);
	  C_F_Structure.Qsnd=Qsnd;
	  return Qsnd;
}
//---
/*
	�������� float  CalculationFormula_d(void)	
	������	���������ֱ��d����λmm)----��Ԥ�⹤��ʱ�������������ֱ��
  ˵����  Qrs:���ٲ�������(��λL/min) ----���ٲ���ʱ������ƽ��ֵ
          Vs:�������٣���λm/S�� ----���������ٹ�ʽ���
          Xsw:��ʪ������λ%��----�ɺ�ʪ����ʽ��ã�������ֶ������ֵ
  
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
	�������� float  CalculationFormula_Qrs(float d,float Vs,float Xsw)	
	������	������ٲ�������Qrs(��λL/min)----��
  ˵����  d_in:������ֱ������λmm)  ----������ֱ�����������¼��
          Vs:�������٣���λm/S�� ----���������ٹ�ʽ���
          Xsw:��ʪ������λ%��----�ɺ�ʪ����ʽ��ã�������ֶ������ֵ
 
*/
float  CalculationFormula_Qrs(float d,float Vs,float Xsw)	
{     float Qrs;
	  Qrs=pow(d,2)*(0.047f*Vs*(1-Xsw));
	  C_F_Structure.Qrs=Qrs;
	  return Qrs;
}
//---
/*
	�������� float  CalculationFormula_C(float g1,float g2,float Vnd)	
	������	�����̳�Ũ��(��λmg/m^3)
  ˵����  g2: ��Ͳ����(��λg)
          g1: ��Ͳ����(��λg)
          Vnd:����������(��λL)
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
	�������� float  CalculationFormula_Cc(float Cs,float V,float V0)	
	������	��������Ũ��(��λmg/m^3)
  ˵����  Cs: ��Ͳ��ϴҺŨ��mg/L
          V: �������mL
          VO:��׼״̬�¸������������m^3
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
	�������� CalculationFormula_Yyzs(float Cc,float Q,u16 N);//������������ 
	������	������������(��λmg/m^3)
    ˵����  Cc: ����Ũ��(��λmg/m^3)
            Q: ��������(��λm^3/h)
            N:��ͷ��Ŀ
*/
float  CalculationFormula_Yyzs(float Cc,float Q,u16 N)//������������ 
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
	�������� float  CalculationFormula_a(void)	
	������	���������ʣϵ��
	˵����  O2�������� (��λ %)
*/
float  CalculationFormula_a(float O2)	
{    float a;
    if(O2==21)
	  {
	  a=1000000;  //��ĸ����Ϊ�� 
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
	�������� float  CalculationFormula_G(float C,float Qsnd)	
������	G:�����̳��ŷ�����(��λkg/h)
˵����  C:ʵ���̳�Ũ��mg/M^3
        Qsnd:����������� M^3/H
*/
float  CalculationFormula_G(float C,float Qsnd)	
{   float G;
	G=C*Qsnd*0.000001f;
	return  G;
}
//---
/*
	�������� float  CalculationFormula_Ca(float C,float a,float a1,float K)	
	������	�����̳�����Ũ�ȣ���λmg/m^3��
  ˵����C:ʵ���̳�Ũ�ȣ���λmg/m^3)
        a:������ʣϵ��---�ɹ�ʽ����
        a1:�ŷű�׼�й涨�Ĺ�������ϵ����   -----����ϵ������
        K:��¯����ϵ��                      -----Ҳ�и���ϵ��������������ҳ���ṩ

*/
float  CalculationFormula_Ca(float C,float a,float a1,float K)	
{    
	  float Ca=C*(a/a1)*K;
	  return  Ca;
}
//---
/*
	�������� float  CalculationFormula_NOX(float NO,float NO2)	
	����������NOX
    ˵����NO:Ũ�ȣ���λmg/m^3)
          NO2:Ũ�ȣ���λmg/m^3)
*/

float  CalculationFormula_NOX(float NO,float NO2) //��NO��NO2����NOXŨ��
{   float NOX=NO*1.5333f+NO2;
	return(NOX);
}
/*
	�������� float  CalculationFormula_G(void)	
	������	 װ�ؼ�������Ĳ���
	˵����  �ϲ㺯������ֱ�ӵ��øú�����Ϊ����   --����ӿ�
  
*/
void  CalculationFormula_All(void)	
{  
	FourBytes four;
	Slave_DataStructure* Sp=Slave_GetData();
	Flashisp_DataStructure* Fp=Flashisp_GetData();
	
	CalculationFormula_LoadfromSlave();//����װ��
	
	if((Fp->Data[FLASHIS_AdrXTSZBOX]&0x40)==0)
	CalculationFormula_p(C_F_Structure.Ts,C_F_Structure.Ba,C_F_Structure.Ps);//�����ܶ�
	else
	CalculationFormula_p(C_F_Structure.Th,C_F_Structure.Ba,0.0);//�����ܶ�
	
	CalculationFormula_Vs(C_F_Structure.Kp,C_F_Structure.Pd,C_F_Structure.p);//��������
	CalculationFormula_Qs(C_F_Structure.Vs,C_F_Structure.F);//������������

	four.U16Data[0]=Flashisp_GetData()->Data[FLASHIS_AdrCHSL_HSL];
	four.U16Data[1]=Flashisp_GetData()->Data[FLASHIS_AdrCHSL_HSL+1];
	
	CalculationFormula_LoadXsw(four.FloatData);
	CalculationFormula_Qsnd(C_F_Structure.Qs,C_F_Structure.Ts,C_F_Structure.Ba +C_F_Structure.Ps,C_F_Structure.Xsw);//�������
    CalculationFormula_a(Sp->O2Concent.Value);//�������ٷֱȼ��������ʣϵ��

}
//---

/*
	�������� CalculationFormula_CaALL(u8 mode)	
	������	������������������Ũ��
˵����  a1:����ϵ��
Qsnd:�����ı������
&PV :���������Ĳ���ֵ��ַ
clk :����ϵ��
*/

void  CalculationFormula_CaALL(float *PV,float a1,float Qsnd,float clk)  //������������������Ũ��
{ float buf1;
  buf1=a1;
	Slave_DataStructure* Sp=Slave_GetData();
	Flashisp_DataStructure* Fp=Flashisp_GetData();

   C_F_Structure.CaofSO2= CalculationFormula_Ca(PV[0],C_F_Structure.a,buf1,clk);  //��������Ũ��
	 C_F_Structure.CaofNO= CalculationFormula_Ca(PV[1],C_F_Structure.a,buf1,clk);
	 C_F_Structure.CaofNO2= CalculationFormula_Ca(PV[2],C_F_Structure.a,buf1,clk);
	 C_F_Structure.CaofCO= CalculationFormula_Ca(PV[3],C_F_Structure.a,buf1,clk);
	 C_F_Structure.CaofH2S= CalculationFormula_Ca(PV[4],C_F_Structure.a,buf1,clk);
	  //�����ŷ���
	C_F_Structure.GaofSO2=CalculationFormula_G(PV[0],Qsnd);
	C_F_Structure.GaofNO=CalculationFormula_G(PV[1],Qsnd);
	C_F_Structure.GaofNO2=CalculationFormula_G(PV[2],Qsnd);
	C_F_Structure.GaofCO=CalculationFormula_G(PV[3],Qsnd);
	C_F_Structure.GaofH2S=CalculationFormula_G(PV[4],Qsnd);
	
}
/*
   �������� void  Calculation(void)
   �����������������Ա궨��Ϊ���ĵ�ƫ��λ�õ�Ũ��
   ���룺calibration1���ϴ�ı궨ֵ
         calibration0����С�궨ֵ
         percentage���ٷ��� <1
        
   �����ƫ��λ�õ�Ũ��

*/
float CAL_DiscreteConcentration(float calibration0,float calibration1,float percentage)
{
	return(calibration0+(calibration1-calibration0)*percentage);
}

/*
   �������� void  Calculation(void)
   �����������������Ա궨��Ϊ���ĵ�ƫ��λ�õ�Ũ�ȵ�ԭʼֵY=KX+B ,X=(Y-B)/K
   ���룺calibration1���ϴ�ı궨ֵ
         calibration0����С�궨ֵ
         percentage���ٷ��� <1
   �����ƫ��λ�õ�Ũ��ԭʼֵ
*/
float CAL_X(float Y,float B,float K)
{
	return((Y-B)/K);
}

/*
   �������� void  Calculationsinglygas
   ������
   ���룺pΪ����ָ��
         LCΪ����
   �����ƫ��λ�õ�Ũ��ԭʼֵ
*/
void Calculationsinglygas(Slave_TPActualStructure *p,u32 LC)
  {
	Flashisp_DataStructure* pf=Flashisp_GetData();
	FourBytes		four;
	float K0buf,K2buf,K4buf;  //
	
		K0buf=p->K1;                      //��
		K2buf=p->K;                       //��
		K4buf=p->K2;                      //��
		//������Kֵ����ε�ֵ

		p->Value0=p->Value0 * K2buf + p->B;  //�� ,����Ũ��Ϊ׼
		p->Value1=p->Value0 * K0buf;  //��
		p->Value2=p->Value0* K4buf;  //��
		
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
   �������� void  Calculationsinglygas
   ������
   ���룺pΪ����ָ��
         LCΪ����
   �����ƫ��λ�õ�Ũ��ԭʼֵ
*/
void CalculationsinglyO2(Slave_TPActualStructure *p,u32 LC)
  {
	Flashisp_DataStructure* pf=Flashisp_GetData();
	FourBytes		four;
	float K0buf,K2buf,K4buf;  //
	
		K0buf=p->K1;                      //��
		K2buf=p->K;                       //��
		K4buf=p->K2;                      //��
		//������Kֵ����ε�ֵ

		p->Value0=p->Value0 * K2buf + p->B;  //�� ,����Ũ��Ϊ׼
	  
	    if((p->Value0>=20)&&(p->Value0<=22))
		{
			p->Value1=p->Value0;
			p->Value2=p->Value0;
		}
		else
		{
		p->Value1=p->Value0 * K0buf;  //��
		p->Value2=p->Value0* K4buf;  //��
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
�������� void  CalculationGas(void)	
	������	�������庬��
	˵����  Type=1 --O2 2--SO2  3--NO  4--NO2  5--CO  6--H2S  7---CO2
  
*/
//
#include "ScreenDwFunction.h"
//�ֶν���
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
		  K=4.24*1870; //4.24=1/ln(1.266) , 1.266=1/(1-0.21), ad=1870 ,������Ũ��21%���Գ�K
		  Sp->O2Concent.Value=(1-1.0/pow((float)2.718281828,Sp->Data[SLAVE_MAP_O2Concent].Raw/K))*100;
		  Sp->O2Concent.Value0=Sp->O2Concent.Value;
		  Sp->O2Concent.Value1=Sp->O2Concent.Value;
		  Sp->O2Concent.Value2=Sp->O2Concent.Value;
		//*****************************5������ֵ�㷨************************
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
		K=  Vref*1992/4096;  //��Ũ�� (PPM) ,1992�Ǹ��ݵ�·��ķŴ����õ�510R+2K
    //  K=  Vref*217/4096;	 //��Ũ��(PPM)  ,217�Ǹ��ݵ�·��ķŴ����õ� 3K+20K
	    Sp->SO2Concent.Value=Sp->Data[SLAVE_MAP_SO2Concent].Raw*K*Ppmtomgm3_SO2;
			
			
		Sp->SO2Concent.Value0=Sp->SO2Concent.Value;
		Sp->SO2Concent.Value1=Sp->SO2Concent.Value;
		Sp->SO2Concent.Value2=Sp->SO2Concent.Value;
			
       
	  //*****************************5������ֵ�㷨************************
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

	  //*****************************5������ֵ�㷨************************
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
		//ppm=52.47*Vref(3v)*ad/4096 -24.24 ,(51k+510ŷ����)
		//ppm=131.8*Vref(3v)*ad/4096 -60.89 ,(20k+510ŷ����)
		K=Vref*131.8f/4096;
		Sp->NO2Concent.Value=(Sp->Data[SLAVE_MAP_NO2Concent].Raw*K-60.89f)*Ppmtomgm3_NO2;
		Sp->NO2Concent.Value0=Sp->NO2Concent.Value;
		Sp->NO2Concent.Value1=Sp->NO2Concent.Value;
		Sp->NO2Concent.Value2=Sp->NO2Concent.Value;

			  //*****************************5������ֵ�㷨************************
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
		
		//*****************************5������ֵ�㷨************************
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
				//ppm=247.476*Vref(3v)*ad/4096 -111.56 ,(20k+5.1k����)
		 K=Vref*247.476f/4096;   //
		 Sp->H2SConcent.Value=(Sp->Data[SLAVE_MAP_H2SConcent].Raw*K-111.56f)*Ppmtomgm3_H2S;
		//Sp->H2SConcent.Value=Sp->Data[SLAVE_MAP_H2SConcent].Raw*1.7f;
		 Sp->H2SConcent.Value0=Sp->H2SConcent.Value;	
		 Sp->H2SConcent.Value1=Sp->H2SConcent.Value;
		 Sp->H2SConcent.Value2=Sp->H2SConcent.Value;
			
		//*****************************5������ֵ�㷨************************
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
		 Sp->CO2Concent.Value=(Sp->Data[SLAVE_MAP_CO2Concent].Raw*K-0.4f)/1.6f*20;  //����0~20
		 if(Sp->CO2Concent.Value>=100)Sp->CO2Concent.Value=20;
		 Sp->CO2Concent.Value0=Sp->CO2Concent.Value;
		 Sp->CO2Concent.Value1=Sp->CO2Concent.Value;
		 Sp->CO2Concent.Value2=Sp->CO2Concent.Value;
			
	     //*****************************5������ֵ�㷨************************
        Calculationsinglygas(&Sp->CO2Concent,FLASHIS_AdrCO2LQ);

      //***************************************************************	
			
		}
		//���㱶�ʺ�����
		/*
		Slave_TPActualStructure * BeforGasp=&Sp->O2Concent;//����ǰ
		Slave_TPActualStructure *Gasp=&Sp->O2Concent;//���ʺ�
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
		
// CO��SO2������
		CalculationFormula_SO2FomCO(Sp->SO2Concent.Value,Sp->COConcent.Value);
		
//NO2��SO2������		
		
		
}

//---

/*
�������� CalculationFormula_ZdK(float Sv,float Tv,float B,float YK)
	������	�Զ�����Kֵ
	˵����  Sv :�ֶ�����ı���ֵ��
	Tv :  ����������ֵ
	B  :ƫ����
	YK :ԭ�������Kֵ
  
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
��������  CalculationFormula_SO2FomCO(float SO2,float CO) 
	������	��CO�����SO2�ĸ��š�
	˵����  
     ���룺SO2 =����ֵ
           CO =����ֵ
     ����� C_F_Structure.SO2FROMCO  ;���ź�����
           
*/
void CalculationFormula_SO2FomCO(float SO2,float CO) //��CO�����SO2�ĸ��š�
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
	buf=pf->Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ1+i];  //ȡ�������ٷֱȣ�0~100��
	four.FloatData=buf;
	four.FloatData=four.FloatData*CO/10000;                          //�ٷֱ�
	C_F_Structure.SO2FromCO=SO2-four.FloatData;
}

void CalculationFormula_SO2FomNO2(float SO2,float NO2) //��CO�����SO2�ĸ��š�
{
	
}
void CalculationFormula_SO2END(void) //��������SO2���
{
	
}

/***********************************************************************
* ��������    CalcAdcToTemperature
* ����������  �ɲ�õ�ADCֵ�����Ӧ���¶�ֵ��
* ���������  float adcVal��������adcƽ��ֵ

* ����ֵ��    �������¶�ֵ
***********************************************************************/
float CalcAdcToTemperature(float adcVal)
{

 float tmpValue;
 if(adcVal>1800)adcVal=1800;
 
	/*
 tmpValue = (adcVal - 1449)*(250 - 200)/(1494-1449) + 200; //200������
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
 tmpValue = (adcVal -Res_Degree0)*(195- 0)/(Res_Degree195 - Res_Degree0) + 0; //19������
 
 //tmpValue = (fbuf - ADC_MIN_VALUE)*(TEMP_MAX_VALUE-TEMP_MIN_VALUE)/(ADC_MAX_VALUE - ADC_MIN_VALUE) + TEMP_MIN_VALUE; 
 return tmpValue;
}

