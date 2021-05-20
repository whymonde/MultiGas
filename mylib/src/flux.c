/*
	���ļ�Ϊ����Ŀ������������غ���
	���庯������
*/

#include "flux.h"

Flux_DataStructure FluxData;
/*20200603
	��������void Flux_Main(void)
	������	flux.cʾ������
*/
void Flux_Main(void)
{
	Flux_ChannelRun('B');
	Flux_ChannelRun('C');
}
//---
/*
	��������void Flux_Init(void)
	������	Flux.c�ļ���ʼ������
*/
void Flux_Init(void)
{
	
	FluxData.DQPressSelectFlag=0;                 //��ʼ������ѹȡֵλ��
	FluxData.JwSelectFlag=0;                      //��ʼ������ȡֵλ��
	Flux_ChannelDataInit(&FluxData.ChannelA);     //��ʼ��A����ͨ������--����  
	Flux_ChannelDataInit(&FluxData.ChannelB);     //��ʼ��B����ͨ������--�̳� 
	Flux_ChannelDataInit(&FluxData.ChannelC);     //��ʼ��B����ͨ������--���� 
	Flux_ReadFlashSamplingData('A');              //�ӹ���������ȡA·�����趨ֵ
	Flux_ReadFlashSamplingData('B');              //�ӹ���������ȡB·�����趨ֵ
	Flux_ReadFlashSamplingData('C');              //�ӹ���������ȡC·�����趨ֵ
	Flux_ReadFlashShowMode();                     //����������ʾ������ֵ����ڣ�������̶ȣ�
	
}
//---
/*
	��������void Flux_Timer(void)
	������Flux.c�ļ���ʱ����������ϵͳ1ms��ʱ������ FluxData.BWXTem
*/
void Flux_Timer(void)
{
    if(FluxData.ChannelA.Timer != 0) FluxData.ChannelA.Timer--;
	if(FluxData.ChannelB.Timer != 0) FluxData.ChannelB.Timer--;
	if(FluxData.ChannelC.Timer != 0) FluxData.ChannelC.Timer--;
}
/*20200603
	��������void Flux_SecTimer(void)
	������	�ú���Ϊflux.c�ļ����뺯������ϵͳ1�뺯������
*/
void Flux_SecTimer(void)
{
	Flux_SaveSamplingRunData();//ʵʱ������������
	Flux_CalVolume(&FluxData.ChannelA);//�ۼ��������
	Flux_CalVolume(&FluxData.ChannelB);//�ۼ��������
	Flux_CalVolume(&FluxData.ChannelC);//�ۼ��������
	//������ʱ�������ȴ���ʱ
	//----------------------------------------------------
	if(FluxData.ChannelB.RunMode==2)  //�̳�����������
	{
			FluxData.ChannelB.Sampling.IntTime++;  //�������ʱ�������
		    FluxData.ChannelB.Sampling.SecTimer++;  //�������ʱ�������
			if(FluxData.ChannelB.Sampling.IntTime>=FluxData.ChannelB.Sampling.DinSiTime) //�Ƚ��趨�ĵ������ʱ��
			{ FluxData.ChannelB.Sampling.IntTime=0; 
				//ʱ�䵽�����¼
				
				FluxData.ChannelB.Sampling.YiciFlg=1;
				FluxData.ChannelB.Sampling.Cnt++;
				if(FluxData.ChannelB.Sampling.Cnt>FluxData.ChannelB.Sampling.Count) //��ʱ�������
				{//����
				
				  FluxData.ChannelB.Sampling.YiciFlg=0;
				  FluxData.ChannelB.Sampling.Cnt=1;
				  FluxData.ChannelB.Sampling.EndFlg=1;
					
				  //�жϽ�����ȥ������
                  if(Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&0x02)	
                  {					  
				  FluxData.ChannelB.RunMode=4; //��ʱ���ת��
				  FluxData.ChannelB.Sampling.DXTime=FLUX_FangDaoXiTimeB;   //10�������
				  FluxData.ChannelB.Sampling.FdxFlg=0;
				  Flux_ContinueSampling('B',20);//������  20L/min  60s
				  }
                  else
                  {					  
				    FluxData.ChannelB.RunMode=0; //��ʱ���ת��  
					FluxData.ChannelB.Sampling.FdxFlg=1; //�Զ���ɣ�δ����
	                Flux_StopSampling('B');
				  }
				}
			}
	}
	else if(FluxData.ChannelB.RunMode==3) //��ͣ
	{
		//  TwoBytes two;
		  if(Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&0x02)	
                  {	
		           if(FluxData.ChannelB.Sampling.DXTime>0)FluxData.ChannelB.Sampling.DXTime--;
                   else
                  {  
			       Flux_StopSampling('B');   //ʱ�䵽��ֹͣ
		           }
			      }			
         else
         Flux_StopSampling('B');   //ʱ�䵽��ֹͣ			 
	}
	else if(FluxData.ChannelB.RunMode==4)                  //��������ʱ
	{   TwoBytes two;
		if(FluxData.ChannelB.Sampling.DXTime>0)FluxData.ChannelB.Sampling.DXTime--;
        else
        {  
			FluxData.ChannelB.Sampling.FdxFlg=1;
			FluxData.ChannelB.RunMode=0;
			Flux_StopSampling('B');   //���ֹͣ
			
			two.U8Data[0] =FluxData.ChannelB.RunMode;
		    two.U8Data[1] = 0;
		    Flashisp_GetData()->Data[FLASHIS_AdrCJMOD] 	= two.U16Data; 			    //����ģʽ+״̬��־λ
			Flashisp_GetData()->Flag=1;
			
		}			
	}

	//----------------------------------------------
	if(FluxData.ChannelC.RunMode==2)  //��������������
	{
		FluxData.ChannelC.Sampling.SecTimer++;
		if(FluxData.ChannelC.Sampling.JpjFlg==1)  //��ƽ��
		{
			FluxData.ChannelC.Sampling.IntTime++;  //��ƽ����ʱ�������
			if(FluxData.ChannelC.Sampling.IntTime>=FluxData.ChannelC.Sampling.DinSiTime) //�Ƚ��趨�ļ�¼ʱ��
			{ FluxData.ChannelC.Sampling.IntTime=0; 
				//ʱ�䵽�����¼
				FluxData.ChannelC.Sampling.YiciFlg=1;
				FluxData.ChannelC.Sampling.Cnt++;
				if(FluxData.ChannelC.Sampling.Cnt>FluxData.ChannelC.Sampling.Count) //��ʱ�������
				{//����
				  FluxData.ChannelC.RunMode=4; //��ʱ���ת��
				  FluxData.ChannelC.Sampling.DXTime=FLUX_FangDaoXiTimeC;   //180����ϴ
				  FluxData.ChannelC.Sampling.EndFlg=1;//������ɣ��ȴ����֣����뱣�������־
				  FluxData.ChannelC.Sampling.FdxFlg=0;//
				}
			}
		}
	}
	else if(FluxData.ChannelC.RunMode==4) //��ϴ��
	{
		if(FluxData.ChannelC.Sampling.DXTime>0)FluxData.ChannelC.Sampling.DXTime--;
        else
        {   
			FluxData.ChannelC.Sampling.FdxFlg=1;
			FluxData.ChannelC.RunMode=0;
			Flux_StopSampling('C');   //���ֹͣ
		}			
	}
}
//---
/*
	��������Flux_DataStructure* Flux_GetData(void)
	��������ȡ����Flux.c�ļ�ȫ�ֱ����ṹ��
*/
Flux_DataStructure* Flux_GetData(void)
{
	return &FluxData;
}
//---
//----------------------------------��ʼ�����--------------------------------------
//---
/*
	��������void Flux_ChannelDataInit(Flux_ChannelStructure *p)
	������	�ú���Ϊ��ʼ��ͨ��RAM����
*/
void Flux_ChannelDataInit(Flux_ChannelStructure *p)
{
	p->RunMode = 0;
	p->ShowFlux = 0;  //��ʾ����Ϊ0
	Flux_FluxDataInit(&p->FluxGK);
	Flux_FluxDataInit(&p->FluxRK);
	Flux_FluxDataInit(&p->FluxCB);
	Flux_FluxDataInit(&p->FluxKD);
	Flux_FluxDataInit(&p->FluxTowData);

}
//---
/*
	��������void Flux_FluxDataInit(Flux_FluxStructure *p)
	������	��ʼ����������
*/
void Flux_FluxDataInit(Flux_FluxStructure *p)
{
	p->Actual = FLUX_ColFAULT;
	p->Raw = FLUX_ColFAULT;
	p->K = 1;
	p->B = 0;
	p->Cnt = 0;
	p->Sum = 0;
	p->Filter.Flag = 0;
	p->Filter.Cnt = 0;
	p->Filter.Sum = 0;
}

//---
/*
	��������void Flux_ClearFluxDataSum(Flux_FluxStructure *p)
	������	�ú���Ϊ�������ṹ�������ʼ��
*/
void Flux_ClearFluxDataSum(Flux_FluxStructure *p)
{
	p->Cnt = 0;
	p->Sum = 0;
}
//---
/*
	��������void Flux_ReadFlashSamplingData(char pump)
	������	��ʼ����������
*/
void Flux_ReadFlashSamplingData(char pump)
{
	u16 *pf;
	FourBytes four;
	Flux_ChannelStructure *ps;
	if(pump == 'A') 
	{
		pf = &Flashisp_GetData()->Data[FLASHIS_AdrCYSZALCYLL];   //��ȡ����ֵ���Ĵ�����
		ps = &FluxData.ChannelA;
	}
	else if(pump == 'B') 
	{
		pf = &Flashisp_GetData()->Data[FLASHIS_AdrCYSZBLCYLL];  //��ȡ����ֵ���Ĵ�����
		ps = &FluxData.ChannelB;
	}
	else if(pump == 'C') 
	{
		pf = &Flashisp_GetData()->Data[FLASHIS_AdrCYSZCLCYLL];  //��ȡ����ֵ���Ĵ�����
		ps = &FluxData.ChannelC;
	}
	four.U16Data[0] = pf[0];
	four.U16Data[1] = pf[1];
	ps->SetFlux = four.FloatData;//�趨��������-----------------------------PID�Ľӽ�ֵ

}
//---
/*
	��������void Flux_ReadFlashShowMode(void)
	������	�ú���Ϊ��ȡA�á�B�á�TSP����ʾ��������
*/
void Flux_ReadFlashShowMode(void)
{
	FluxData.ChannelA.ShowMode ='R';
	FluxData.ChannelB.ShowMode ='R';
	FluxData.ChannelC.ShowMode ='R';
}



//---

//---
/*20200527
	��������void Flux_FlowCalculate(void)
	������	�ú���Ϊ�����������
	������������
*/
void Flux_CalculateFlux(char pump)
{
	float differP,press,tem, b;
	Flux_ChannelStructure *p;
	//A·��������
	if(pump == 'A')
	{
		p = &FluxData.ChannelA;
		tem = Flux_GetJWTem()/*Slave_GetData()->YWTem.Value*/;	//����ֵ/����ֵ  ,�������¶Ȳ�������ֵ
		differP = Slave_GetData()->ACPress.Value;	//��ѹ
		press = Slave_GetData()->AGPress.Value;		//��ѹ
		b =0.3258;// 0.4925;	 //	//ϵ��=������ʾ*��/Ч�������ʾ
		
	if(Flux_GetDQPress()/*Slave_GetData()->DQPress.Value*/ == FLUX_ColFAULT)//����ѹ����
		press = FLUX_ColFAULT;
	else
		press += Flux_GetDQPress()/*Slave_GetData()->DQPress.Value*/;
	
	}
	//�̳�
	else if(pump == 'B')
	{
		p = &FluxData.ChannelB;
		tem = Flux_GetJWTem()/*Slave_GetData()->JWTem.Value*/;	//����ֵ/����ֵ
		differP = Slave_GetData()->BCPress.Value;	//��ѹ
		press = Slave_GetData()->BGPress.Value;		//��ѹ
		b =0.3842;// 0.4925;	 //	//ϵ��=������ʾ*��b/Ч�������ʾ
		
	if(Flux_GetDQPress()/*Slave_GetData()->DQPress.Value*/ == FLUX_ColFAULT)//����ѹ����
		press = FLUX_ColFAULT;
	else
	{
	if(Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&0x0040)  //ѡ�����������ѹ
		{
		press += Flux_GetDQPress();/*Slave_GetData()->DQPress.Value*/;
	    }
		else
		{
		press=Flux_GetDQPress();//ѡ�����ѹ
		}
	}
	}
	//
	else if(pump == 'C')
	{
		p = &FluxData.ChannelC;
		tem = Flux_GetJWTem()/*Slave_GetData()->JWTem.Value*/;	//����ֵ/����ֵ
		differP = Slave_GetData()->CCPress.Value;	//��ѹ
		press =0;		                              //��ѹ������
		b = 40;										//������ϵ��
													//ϵ��
		if(Flux_GetDQPress()/*Slave_GetData()->DQPress.Value*/ == FLUX_ColFAULT)//����ѹ����
		press = FLUX_ColFAULT;
	    else
		press += Flux_GetDQPress()/*Slave_GetData()->DQPress.Value*/;
		press=press;
	}
	
	
	
	//------------------
	//��������
	if((differP == FLUX_ColFAULT) || (press == FLUX_ColFAULT) ||(tem == FLUX_ColFAULT))
	{
		p->FluxGK.Actual = FLUX_ColFAULT;//��������
		p->FluxRK.Actual = FLUX_ColFAULT;
		p->FluxCB.Actual = FLUX_ColFAULT;//�α�����
		p->FluxKD.Actual = FLUX_ColFAULT;//�̶�����
	}
	else
	{//��������
//		FourBytes four;
	

		//��������
		Flux_FluxDataDealWith(&p->FluxGK,Flux_FluxCalculate(differP,tem,press,b));       //ԭֵ΢��
		p->FluxGK.Actual = Flux_CalibrationFlux(pump,p->SetFlux,p->FluxGK.Actual);       //��������ֵ�鱶��
		//�������
		p->FluxRK.Actual = (float)(p->FluxGK.Actual * press / Flux_GetDQPress() * ((float)273.15 + Slave_GetData()->HWTem.Value)/((float)273.15 + tem));
		//�α�����/�������
		p->FluxCB.Actual = (float)(p->FluxGK.Actual * press / (float)101.325 * (float)273.15/((float)273.15 + tem));

		
	if(p->RunMode == 2)
		{//�ۼ����������
			p->FluxRK.Sum += p->FluxRK.Actual;
			p->FluxRK.Cnt++;
			p->FluxCB.Sum += p->FluxCB.Actual;
			p->FluxCB.Cnt++;
			p->FluxKD.Sum += p->FluxKD.Actual;
			p->FluxKD.Cnt++;
		}
		
	}
	Flux_ShowFluxSet(p);//��ʾ����

}
//---
/*
	��������void Flux_FluxDataDealWith(Flux_FluxStructure *p,float value)
	������	�ú���Ϊ���������ݽ��д��������˲�	
	���룺	*p�����������������
					value������ֵ
*/
void Flux_FluxDataDealWith(Flux_FluxStructure *p,float value)
{float buf;
	if(value == FLUX_ColFAULT) p->Actual = FLUX_ColFAULT;
	else
	{
		buf=Flux_DataMiddleFilter(&p->Filter,value);                    //test3   //��ֵ�˲�
		p->Raw = buf;// Flux_DataFilter(&p->Filter,buf);                                //ƽ���˲�
		p->Actual = p->Raw * p->K + p->B;
	}		
}
//---
/*
	
*/
//---
/*
	��������float Flux_DataFilter(Flux_FilterStructure *p,float value)
	������	�ú���Ϊ�����ݽ���ѭ���ж��˲��������㲢�����˲�ֵ
	���룺	p�����˲��ṹ��
					data���²���ֵ
	���أ�	�����˲�ֵ
*/
float Flux_DataFilter(Flux_FilterStructure *p,float value)
{
	if(p->Flag & B0)
	{//����ģʽ
		p->Sum += value;
		p->Sum -= p->Buffer[p->Cnt];
		p->Buffer[p->Cnt++] = value;
		if(p->Cnt == FLUX_FILTER_DEPTH) p->Cnt = 0;
		value = p->Sum/FLUX_FILTER_DEPTH;
	}
	else
	{//�ϵ��ʼģʽ
		p->Buffer[p->Cnt++] = value;
		p->Sum += value;
		if(p->Cnt == FLUX_FILTER_DEPTH) 
		{
			p->Cnt = 0;
			p->Flag |= B0;
			value = p->Sum/FLUX_FILTER_DEPTH;
		}
	}
	return value;
}

//---
/*
	��������float Flux_DataFilter(Flux_FilterStructure *p,float value)
	������	�ú���Ϊ�����ݽ���ѭ���ж��˲��������㲢�����˲�ֵ
	���룺	p�����˲��ṹ��
					data���²���ֵ
	���أ�	�����˲�ֵ
*/
/*
float Flux_DataMiddleFilter(Flux_FilterStructure *p,float value)
{u8 i,j;
	float temp;
	float buf[FLUX_MIDDLEFILTER_DEPTH];
	if(p->Flag & B1)
	{//����ģʽ
		p->BufferMiddle[p->CntMiddle++] = value;
		if(p->CntMiddle >= FLUX_MIDDLEFILTER_DEPTH) p->CntMiddle = 0;
	}
	else
	{//�ϵ��ʼģʽ
		p->BufferMiddle[p->CntMiddle++] = value;
		if(p->CntMiddle == FLUX_MIDDLEFILTER_DEPTH) 
		{
			p->CntMiddle = 0;
			p->Flag |= B1;
		
		}
	}
	//�����м�ֵ
	for(i=0;i<FLUX_MIDDLEFILTER_DEPTH;i++)
	buf[i]=p->BufferMiddle[i];
	
	for(j = 0;j<(FLUX_MIDDLEFILTER_DEPTH-1);j++)
	 {
		for(i = 0;i<(FLUX_MIDDLEFILTER_DEPTH-j-1);i++)
		if(buf[i]>buf[i+1])
		{
			temp = buf[i];
			buf[i] =buf[i+1];
			buf[i+1] = temp;
		}
	 }
	 temp=buf[(FLUX_MIDDLEFILTER_DEPTH-1)/2];
	 temp=temp+p->OldMiddle;
	 temp=temp/2;
   p->OldMiddle=buf[(FLUX_MIDDLEFILTER_DEPTH-1)/2];  //������һ����ֵ
   return temp;
}
*/
//---
/*
	��������float Flux_DataFilter(Flux_FilterStructure *p,float value)
	������	�ú���Ϊ�����ݽ���ѭ���ж��˲��������㲢�����˲�ֵ
	���룺	p�����˲��ṹ��
					data���²���ֵ
	���أ�	�����˲�ֵ
*/
float Flux_DataMiddleFilter(Flux_FilterStructure *p,float value)
{
u8 i;
	float temp,temp1;
	float buf[FLUX_MIDDLEFILTER_DEPTH];
	if(p->Flag & B1)
	{//���Ǳ��淽ʽ
		p->BufferMiddle[p->CntMiddle++] = value;
		if(p->CntMiddle >= FLUX_MIDDLEFILTER_DEPTH) p->CntMiddle = 0;
	}
	else
	{//δ�����淽ʽ
		p->BufferMiddle[p->CntMiddle++] = value;
		if(p->CntMiddle >= FLUX_MIDDLEFILTER_DEPTH) 
		{
			p->CntMiddle = 0;
			p->Flag |= B1;
		
		}
	}
	//
	for(i=0;i<FLUX_MIDDLEFILTER_DEPTH;i++)
	buf[i]=p->BufferMiddle[i];
	//ȥ�����ֵ
	for(i=0;i<FLUX_MIDDLEFILTER_DEPTH-1;i++)
	 {
		 if(buf[i]>buf[i+1])
		 {
			 temp1=buf[i+1];
			 buf[i+1]=buf[i];
			 buf[i]=temp1;
		 }
	 }
	//ȥ����Сֵ
	for(i=0;i<FLUX_MIDDLEFILTER_DEPTH-2;i++)
	 {
		  if(buf[i]<buf[i+1])
		 {
			 temp1=buf[i+1];
			 buf[i+1]=buf[i];
			 buf[i]=temp1;
		 }
	 }

	temp=0;
	for(i=0;i<(FLUX_MIDDLEFILTER_DEPTH-2);i++)
	temp=temp+buf[i];
	temp=temp/(FLUX_MIDDLEFILTER_DEPTH-2);    //ȥ�������С���ƽ��ֵ
	temp1=temp;
	temp=temp+p->OldMiddle;
	temp=temp/2;
	p->OldMiddle=temp1;
	return temp;
	
}

/*
	��������Flux_ClearMidBuff(Flux_FilterStructure *p)
	������	�����ֵ�˲�����ֵ,��ֵ���ڵ�ǰƽ��ֵ
	���룺	p�����˲��ṹ��
					data���²���ֵ
	���أ�	�����˲�ֵ
*/
void Flux_ClearMidBuff(Flux_FilterStructure *p,float vol)
{
	u16 i;
	for(i=0;i<FLUX_MIDDLEFILTER_DEPTH;i++)
	p->BufferMiddle[i]=vol;
	p->CntMiddle = 0;
	p->Flag |= B1;
}

//---
/*20200527
	��������float Slave_GetHJTem(void)
	������	��ȡ�����¶ȣ�����ֵ�򴫸���ֵ
*/
float Flux_GetJWTem(void)
{  
	if(FluxData.JwSelectFlag)
	return(FluxData.JWbuf);
	else
	{	//if(Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&0x0040)  //ѡ��
		//return(Slave_GetData()->HWTem.Value);    //ȡ����
		//else
	    return(Slave_GetData()->JWTem.Value);    //ȡ���� return(Slave_GetData()->JWTem.Value); 
	}
}
/*20200604
	��������void Flux_CalVolume(Flux_ChannelStructure *p)
	������	�ú���Ϊ�����ۼ�����������ɶ�ʱ��1S����
*/
void Flux_CalVolume(Flux_ChannelStructure *p)
{
	if(p->RunMode == 2)
	{//����������
		float total=0;
		Flux_FluxStructure *pt;
		//�ۼ��������
		if(p->ShowMode == 'K') pt = &p->FluxKD;//�̶�����
		else if(p->ShowMode == 'C') pt = &p->FluxCB;//�α�����
		else pt = &p->FluxRK;//��������
		if(pt->Cnt!=0)
		total = (pt->Sum/ pt->Cnt)/60;
		else total=0;
	
		p->Sampling.SumVolume += total;
		p->FluxRK.Sum = 0;
		p->FluxRK.Cnt = 0;
		p->FluxKD.Sum = 0;
		p->FluxKD.Cnt = 0;
		//�α��������
		if(p->FluxCB.Cnt!=0)
		total = (p->FluxCB.Sum/ p->FluxCB.Cnt)/60;
		else
		total=0;	
		p->Sampling.RefVolume += total;
		p->FluxCB.Sum = 0;
		p->FluxCB.Cnt = 0;
	}
}
//---
/*
	��������float Slave_GetDQPress(void)
	������	��ȡ����ѹ������ֵ�򴫸���ֵ
*/
float Flux_GetDQPress(void)
{
	if(FluxData.DQPressSelectFlag)
		return(FluxData.DQPressbuf);
	else
	{
		if(Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&0x0001)
		{
		  FourBytes four;
			four.U16Data[0]=Flashisp_GetData()->Data[FLASHIS_AdrXTSZDQY];
			four.U16Data[1]=Flashisp_GetData()->Data[FLASHIS_AdrXTSZDQY+1];
			return four.FloatData;
		}else
		{
		 return(Slave_GetData()->DQPress.Value);
		}
	}
}

//---
/*
	��������void Flux_LoadJWTem(float Jwbuf);
	������	��ȡ���£�����ֵ�򴫸���ֵ
          select:0:����ѡ����ֵ,1������ѡ��

*/

void Flux_LoadJWTem(u8 select,float Jwbuf)   //�ȸ��������ʱֵ -����ӿ�
{
	FluxData.JwSelectFlag=select;   //�ѱ��ͬʱ��Ϊ1
	FluxData.JWbuf=Jwbuf;
}
//---
/*
	��������void Flux_LoadDQPress(float Jwbuf)
	������	��ȡ����ѹ������ֵ�򴫸���ֵ
*/
void Flux_LoadDQPress(u8 select,float DQPressbuf) //�ȸ��������ʱֵ -����ӿ�
 {
    FluxData.DQPressSelectFlag=select;
	  FluxData.DQPressbuf=DQPressbuf;
 }

//---
/*
	��������float Flux_CalibrationFlux(char pump,float set,float flux)
	������	A/B/TSP·����У��
	���룺	pump����ҪУ׼�ıã�'A'=������'B'=���ã�'C'=����
					set���趨����
					flux����ǰ��������
	���أ�	��pump���������򷵻�ֵ=flux
					��set���������򷵻�ֵ=flux
					���в�����ȷ��������У׼ֵ
*/
float Flux_CalibrationFlux(char pump,float set,float flux)
{
	FourBytes four;
	u16 *p;
	u8 sw = (u8)(set/10);
	if (sw>=10) sw=10;
	if(sw>2) sw-=2;
	else
	{
	 sw=0;
	}
 if(pump == 'B')
	 {
   // p = &Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_LLJZ_YCBL01+sw*2];   //�̳��ĵ�ַ����
		p = &Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_LLJZ_YCBL01];   //�̳��ĵ�ַ���� 
		switch(sw)
		{
			case 0:             //20
			{
				four.U16Data[0] = p[0];
				four.U16Data[1] = p[1];
				break;
			}
			case 1:           //30
			{
				four.U16Data[0] = p[2];
				four.U16Data[1] = p[3];
				break;
			}
			case 2:          //40
			{
				four.U16Data[0] = p[4];
				four.U16Data[1] = p[5];
				break;
			}
			case 3:         //50
			{
				four.U16Data[0] = p[6];
				four.U16Data[1] = p[7];
				break;
			}
			case 4:         //60
			{
				four.U16Data[0] = p[8];
				four.U16Data[1] = p[9];
				break;
			}
			case 5:         //70
			{
				four.U16Data[0] = p[10];
				four.U16Data[1] = p[11];
				break;
			}
			case 6:        //80
			{
				four.U16Data[0] = p[12];
				four.U16Data[1] = p[13];
				break;
			}
			case 7:        //90
			{
				four.U16Data[0] = p[14];
				four.U16Data[1] = p[15];
				break;
			}
			case 8:       //100
			{
				four.U16Data[0] = p[16];
				four.U16Data[1] = p[17];
				break;
			}
//			case 10:
//			{
//				four.U16Data[0] = p[18];
//				four.U16Data[1] = p[19];
//				break;
//			}
			default:
			{
				four.FloatData = 1;//����У׼
				break;
			}
		}
	}
	else if(pump == 'C') 
	{
        p = &Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_LLJZ_YQBL01];
		four.U16Data[0] = p[0];
		four.U16Data[1] = p[1];
	}
	else four.FloatData = 1;//��������
	return(flux*four.FloatData);//����У׼������   ����*����ֵ
}
//---
/*
	��������void Flux_ShowFluxSet(Flux_ChannelStructure *p)
	������	�ú���Ϊ��ʾ��Ҫ��ʾ���������ݣ��������αȡ��̶�����
*/
void Flux_ShowFluxSet(Flux_ChannelStructure *p)
{ Flux_ReadFlashShowMode();
	switch(p->ShowMode)
	{
		case 'R':
			{
			p->ShowFlux = p->FluxRK.Actual;
			break;//�������
		  }
		case 'C': 
			{
				p->ShowFlux = p->FluxCB.Actual;
			break;//�α�����
		  }
		case 'K': 
			{
			p->ShowFlux = p->FluxKD.Actual;
			break;//�̶�����
		  }
	}
	if(p->ShowFlux<0)
		p->ShowFlux=0;   //��֤���ݴ���0
}
//---
/*
	��������float Flux_FluxCalculate(float differP,float tem,float press,float b)
	������	�ú���Ϊ���㹤��������δУ׼����
	���룺	differP��ѹ����뵥λkPa����Ҫ����ת��ΪPa
					tem���¶ȣ����뵥λ��
					press������ѹ�����뵥λ����λKPa
					b����ֵ������
	���أ�	��������λL/min
*/
float Flux_FluxCalculate(float differP,float tem,float press,float b)
{
	float flux;
	
	flux=differP ;  					//PA
	flux=flux/FLUX_BasicVal*(tem + FLUX_AbsTem)/FLUX_AbsTem*FLUX_StnPress/press;
	
	/*
	differP = differP * 1000;  					//PA
	tem = (float)tem + FLUX_AbsTem;		 	//
	flux = differP * tem * FLUX_StnPress;
	flux = flux /(FLUX_BasicVal * FLUX_AbsTem * (float)press);
	*/
	
	if(flux<0)
		return 0;
	else
	return (float)(sqrt(flux)/b);
}


//---
/*
	��������u8 Flux_StarSampling(char pump,u8 mode)
	������	�ú���Ϊ���������ò���
	���룺	pump�������ö���'A'=A·�ã�'B'=B·�ã�'T'=TSP·��
				
	���أ�	0=�����ɹ�
					255=pumb��������pumb����'A'��'B'��'C'�е�����һ����
					254=mode��������
*/
u8 Flux_StarSampling(char pump,float Flux)
{
      	Flux_ChannelStructure *p;
	      u8 re=0;
	      if(pump == 'A') p = &FluxData.ChannelA;
	      else if(pump == 'B') p = &FluxData.ChannelB;
	      else if(pump == 'C') p = &FluxData.ChannelC;
	
	          Flux_ClearFluxDataSum(&p->FluxKD);  //����ǰ��ʼ��
			  Flux_ClearFluxDataSum(&p->FluxGK);
			  Flux_ClearFluxDataSum(&p->FluxCB);

      	if(pump=='A')  //��һ·������������Ҫ����
				p->SetFlux=	Flux;
			
				else if(pump=='B')
				{
				//���г�ʼ��һЩ����
				//p=&Flux_GetData()->ChannelB;	
				p->SetFlux=	Flux;
				re=Slave_StartPump(&Slave_GetData()->DustMotor,p->SetFlux);
					
		
				}
				else if(pump=='C')
				{
				//p=&Flux_GetData()->ChannelC;	
				p->SetFlux=	Flux;
				re=Slave_StartPump(&Slave_GetData()->GasMotor,p->SetFlux);
					
				
				}
				return re;
}
//---
/*
	��������u8 Flux_ContinueSampling(char pump,float Flux)
	������	�ú���Ϊ���������ò���
	���룺	pump�������ö���'A'=A·�ã�'B'=B·�ã�'T'=TSP·��
				
	���أ�	0=�����ɹ�
					255=pumb��������pumb����'A'��'B'��'C'�е�����һ����
					254=mode��������
*/
u8 Flux_ContinueSampling(char pump,float Flux)
{
      	Flux_ChannelStructure *p;
	      u8 re=0;
	      if(pump == 'A') p = &FluxData.ChannelA;
	      else if(pump == 'B') p = &FluxData.ChannelB;
	      else if(pump == 'C') p = &FluxData.ChannelC;
	
	    //  Flux_ClearFluxDataSum(&p->FluxKD);  //����ǰ��ʼ��
			//  Flux_ClearFluxDataSum(&p->FluxGK);
			//  Flux_ClearFluxDataSum(&p->FluxCB);

      	if(pump=='A')  //��һ·������������Ҫ����
				p->SetFlux=	Flux;
			
				else if(pump=='B')
				{
				//���г�ʼ��һЩ����
				//p=&Flux_GetData()->ChannelB;	
				p->SetFlux=	Flux;
				re=Slave_ContinuePump(&Slave_GetData()->DustMotor,p->SetFlux);
				}
				else if(pump=='C')
				{
				//p=&Flux_GetData()->ChannelC;	
				p->SetFlux=	Flux;
				re=Slave_ContinuePump(&Slave_GetData()->GasMotor,p->SetFlux);
				}
				return re;
}
//---
/*
	��������u8 Flux_StopSampling(char pump,u8 mode)
	������	�ú���Ϊ����ֹͣ�ò���
	���룺	pump�������ö���'A'=A·��'B'=B·���ã�'C'=����
					
	���أ�	0=�����ɹ�
					255=pumb��������pumb����'A'��'B'��'C'�е�����һ����
*/
u8 Flux_StopSampling(char pump)
{ 


	     u8 re=0;
				if(pump=='A')  //��һ·������������Ҫ����
					;
				else if(pump=='B')
				{
				//���г�ʼ��һЩ����
				re=Slave_StopPump(&Slave_GetData()->DustMotor);
				
				}
				else if(pump=='C')
				{
					
				re=Slave_StopPump(&Slave_GetData()->GasMotor);
				
				}
				return re;
}
//---
//---
/*
	��������void Flux_SaveSamplingRunData(void)
	������	�ú���Ϊ���������������
*/
void Flux_SaveSamplingRunData(void)
{
	TwoBytes two;
	FourBytes four;
	Flux_ChannelStructure *ps;
	Flashisp_DataStructure *pf = Flashisp_GetData();

	//b·�������ݱ���
	ps = &FluxData.ChannelB;
	if(ps->RunMode) //�������оͿ�ʼÿ���ӱ���
	{
		two.U8Data[0] = ps->RunMode;
		two.U8Data[1] = 0;
		pf->Data[FLASHIS_AdrCJMOD] 	= two.U16Data; 			    //����ģʽ+״̬��־λ
		
		//��ʼʱ��

	    four.U32Data = mktime(&Screen_GetData()->Clock.Clock);
		Flashisp_GetData()->Data[FLASHIS_AdrDDTime] = four.U16Data[0];
		Flashisp_GetData()->Data[FLASHIS_AdrDDTime+1] = four.U16Data[1];
		
		four.U32Data=ps->Sampling.SecTimer;
		pf->Data[FLASHIS_AdrCJLJSH] 	= four.U16Data[0];	    //������ʱ
		pf->Data[FLASHIS_AdrCJLJSH+1]   = four.U16Data[1];
		
		pf->Data[FLASHIS_AdrCJCNT] 	= ps->Sampling.Cnt;			//��������
		
		four.FloatData = ps->Sampling.SumVolume;				//�ۼ����
		pf->Data[FLASHIS_AdrCJGKTJ]   = four.U16Data[0];
		pf->Data[FLASHIS_AdrCJGKTJ+1] = four.U16Data[1];
		
		four.FloatData = ps->Sampling.RefVolume;				//�α����
		pf->Data[FLASHIS_AdrCJBQTJ]   = four.U16Data[0];
		pf->Data[FLASHIS_AdrCJBQTJ+1] = four.U16Data[1];

		four.FloatData =ps->SaveFileNumber;                     //�ļ���
		pf->Data[FLASHIS_AdrCJWJH]=four.U16Data[0];  
		pf->Data[FLASHIS_AdrCJWJH+1]=four.U16Data[1]; 

//		printf("��������");
		pf->Flag = 1;
	}
}
//------------------------------------------------------------------------
//---
/*20200602
	��������void Flux_ChannelRun_B(char pump)
	������	�ú���Ϊͨ������������
    ���߳���Ҫ��ȫ�̼�أ��������ݱ���
*/
void Flux_ChannelRun(char pump)
{
	Flux_ChannelStructure *p;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	if(pump == 'A') p = &FluxData.ChannelA;
	else if(pump == 'B') p = &FluxData.ChannelB;
	else if(pump == 'C') p = &FluxData.ChannelC;
	if(p->RunMode != 0)  //����
	switch(p->RunMode)//������״̬
	{
		case 1: //������ʼ��(��������)
		{
			if(pump == 'B')    //�̳�
			{
				p->Sampling.SecTimer=0;  //���㿪ʼ
				p->Sampling.IntTime=0;  //���㿪ʼ
			
				p->Sampling.DinSiTime=(u32)pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]; //�������ʱ��
				p->Sampling.Count=pf->Data[FLASHIS_AdrYDBD_CYDS]; //�ܲ�����
				p->Sampling.SamTime=(pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS])*p->Sampling.Count;//��ʱ��
	
				p->Sampling.Cnt=1;  //��0�ο�ʼ����
				p->Sampling.SumVolume=0;  //�ۼ����
				p->Sampling.RefVolume=0;  //�ۼ����
				p->Sampling.YiciFlg=0;
				p->Sampling.EndFlg=0;
				p->Sampling.FdxFlg=0; //��������ɱ�־
				p->FluxRK.Sum = 0;
		        p->FluxRK.Cnt = 0;
		        p->FluxKD.Sum = 0;
		        p->FluxKD.Cnt = 0;
		        p->FluxCB.Sum = 0;
		        p->FluxCB.Cnt = 0;

			}
			else if(pump == 'C')//����
			{
				p->Sampling.SecTimer=0;  //���㿪ʼ
				p->Sampling.IntTime=0;  //���㿪ʼ
				p->Sampling.SamTime=(u32)pf->Data[FLASHIS_AdrCLYQ_JSSC]; //��ȡ�����ܳ�
				p->Sampling.DinSiTime=(u32)pf->Data[FLASHIS_AdrXTSZYQCLSJ]*60; //��ʱ����ʱ��,���ӻ�����
				p->Sampling.Count=pf->Data[FLASHIS_AdrXTSZYQCLCS];  //��������
				p->Sampling.Cnt=1;  //��0�ο�ʼ����
				p->Sampling.SumVolume=0;  //�ۼ����
				p->Sampling.RefVolume=0;  //�ۼ����
				p->Sampling.YiciFlg=0;
				p->Sampling.EndFlg=0;
				p->Sampling.FdxFlg=0; //��������ɱ�־
				p->Sampling.AutoMod=(u8)pf->Data[FLASHIS_AdrCLYQ_CLFS];  //�ֶ��Զ�
				p->Sampling.JpjFlg=0; //�տ�ʼ���ڼ�ƽ��״̬
				p->FluxRK.Sum = 0;
		        p->FluxRK.Cnt = 0;
		        p->FluxKD.Sum = 0;
		        p->FluxKD.Cnt = 0;
		        //�α��������
	
		        p->FluxCB.Sum = 0;
		        p->FluxCB.Cnt = 0;

			}
			p->RunMode=2;
			break;
		}
		case 2: //������()
		{
			if(pump == 'B')    //�̳�
			{
				
			}
			else if(pump == 'C')//����
			{
				
			}
			break;
		}
		case 3: //��ͣ��
		{
			if(pump == 'B')    //�̳�
			{
				
			}
			else if(pump == 'C')//����
			{
				
			}
			break;
		}
		case 4: //��������
		{
			if(pump == 'B')    //�̳�
			{
				
			}
			else if(pump == 'C')//������ϴ
			{
				
			}
			break;
		}
		case 5: //��ͣ���з�����
		{
			if(pump == 'B')    //�̳�
			{
				
			}
			break;
		}
		
	}
}


//---
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Flux_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪFlux�ļ���ASCII�����ļ�
	���	 :000|10|XXX
*/
void Flux_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 101:
		{//��ĳ����/����������
		   Flux_StarSampling('C',0.8);  //����ӿ�
			 printf("��0.8L/min ��������\r\n");
			 break;
		}
		case 201:
		{//ֹͣ������
		  Flux_StopSampling('C');
			printf("�ر�����\r\n");
			break;
		}
		case 301:
		{//��ĳռ�ձ�������TSP���
			//��ĳ����/����������
		   Flux_StarSampling('B',50.0);  //����ӿ�
			 printf("��50L/min ��������\r\n");
		  break;
		}
		case 401:
		{//ֹͣ������
		  Flux_StopSampling('B');
			printf("�رճ���\r\n");
			break;
		}
		case 501:
		{//��ȡԭʼ����
			printf("��������û��У׼\r\n");
			printf("A·����������%f\r\n",FluxData.ChannelA.FluxGK.Raw);
			printf("B·����������%f\r\n",FluxData.ChannelB.FluxGK.Raw);
			printf("C·����������%f\r\n",FluxData.ChannelC.FluxGK.Raw);
			break;
		}
		case 502:
		{//��ȡԭʼ����
			printf("��������ΪУ׼����\r\n");
			printf("A·���������%f\r\n",FluxData.ChannelA.ShowFlux);
			printf("B·���������%f\r\n",FluxData.ChannelB.ShowFlux);
			printf("C·���������%f\r\n",FluxData.ChannelC.ShowFlux);
			break;
		}
	}
}
//---
/*
	�������� 	void Flux_DebugRTU((u8 *p)
	������		�ú���ΪFlux�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*//*
void Flux_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//�鿴���ĸ�������
	{
		case 0x01:
		{
			break;
		}
		case 0x02:
		{
			break;
		}
	}
}*/






























