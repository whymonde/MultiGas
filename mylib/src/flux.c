/*
	本文件为该项目的流量计算相关函数
	具体函数如下
*/

#include "flux.h"

Flux_DataStructure FluxData;
/*20200603
	函数名：void Flux_Main(void)
	描述：	flux.c示主函数
*/
void Flux_Main(void)
{
	Flux_ChannelRun('B');
	Flux_ChannelRun('C');
}
//---
/*
	函数名：void Flux_Init(void)
	描述：	Flux.c文件初始化函数
*/
void Flux_Init(void)
{
	
	FluxData.DQPressSelectFlag=0;                 //初始化大气压取值位置
	FluxData.JwSelectFlag=0;                      //初始化计温取值位置
	Flux_ChannelDataInit(&FluxData.ChannelA);     //初始化A气体通道数据--工况  
	Flux_ChannelDataInit(&FluxData.ChannelB);     //初始化B气体通道数据--烟尘 
	Flux_ChannelDataInit(&FluxData.ChannelC);     //初始化B气体通道数据--烟气 
	Flux_ReadFlashSamplingData('A');              //从公共变量读取A路流量设定值
	Flux_ReadFlashSamplingData('B');              //从公共变量读取B路流量设定值
	Flux_ReadFlashSamplingData('C');              //从公共变量读取C路流量设定值
	Flux_ReadFlashShowMode();                     //设置流量显示是那种值（入口，标况，刻度）
	
}
//---
/*
	函数名：void Flux_Timer(void)
	描述：Flux.c文件定时器函数，由系统1ms定时器调用 FluxData.BWXTem
*/
void Flux_Timer(void)
{
    if(FluxData.ChannelA.Timer != 0) FluxData.ChannelA.Timer--;
	if(FluxData.ChannelB.Timer != 0) FluxData.ChannelB.Timer--;
	if(FluxData.ChannelC.Timer != 0) FluxData.ChannelC.Timer--;
}
/*20200603
	函数名：void Flux_SecTimer(void)
	描述：	该函数为flux.c文件的秒函数，由系统1秒函数调用
*/
void Flux_SecTimer(void)
{
	Flux_SaveSamplingRunData();//实时保存运行数据
	Flux_CalVolume(&FluxData.ChannelA);//累计体积计算
	Flux_CalVolume(&FluxData.ChannelB);//累计体积计算
	Flux_CalVolume(&FluxData.ChannelC);//累计体积计算
	//采样计时、采样等待计时
	//----------------------------------------------------
	if(FluxData.ChannelB.RunMode==2)  //烟尘采样过程中
	{
			FluxData.ChannelB.Sampling.IntTime++;  //单点采样时间计算器
		    FluxData.ChannelB.Sampling.SecTimer++;  //单点采样时间计算器
			if(FluxData.ChannelB.Sampling.IntTime>=FluxData.ChannelB.Sampling.DinSiTime) //比较设定的单点采样时间
			{ FluxData.ChannelB.Sampling.IntTime=0; 
				//时间到保存记录
				
				FluxData.ChannelB.Sampling.YiciFlg=1;
				FluxData.ChannelB.Sampling.Cnt++;
				if(FluxData.ChannelB.Sampling.Cnt>FluxData.ChannelB.Sampling.Count) //计时次数完成
				{//处理
				
				  FluxData.ChannelB.Sampling.YiciFlg=0;
				  FluxData.ChannelB.Sampling.Cnt=1;
				  FluxData.ChannelB.Sampling.EndFlg=1;
					
				  //判断进不进去防倒吸
                  if(Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&0x02)	
                  {					  
				  FluxData.ChannelB.RunMode=4; //计时完成转到
				  FluxData.ChannelB.Sampling.DXTime=FLUX_FangDaoXiTimeB;   //10秒防倒吸
				  FluxData.ChannelB.Sampling.FdxFlg=0;
				  Flux_ContinueSampling('B',20);//防倒吸  20L/min  60s
				  }
                  else
                  {					  
				    FluxData.ChannelB.RunMode=0; //计时完成转到  
					FluxData.ChannelB.Sampling.FdxFlg=1; //自动完成，未保存
	                Flux_StopSampling('B');
				  }
				}
			}
	}
	else if(FluxData.ChannelB.RunMode==3) //暂停
	{
		//  TwoBytes two;
		  if(Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&0x02)	
                  {	
		           if(FluxData.ChannelB.Sampling.DXTime>0)FluxData.ChannelB.Sampling.DXTime--;
                   else
                  {  
			       Flux_StopSampling('B');   //时间到，停止
		           }
			      }			
         else
         Flux_StopSampling('B');   //时间到，停止			 
	}
	else if(FluxData.ChannelB.RunMode==4)                  //防倒吸计时
	{   TwoBytes two;
		if(FluxData.ChannelB.Sampling.DXTime>0)FluxData.ChannelB.Sampling.DXTime--;
        else
        {  
			FluxData.ChannelB.Sampling.FdxFlg=1;
			FluxData.ChannelB.RunMode=0;
			Flux_StopSampling('B');   //最后停止
			
			two.U8Data[0] =FluxData.ChannelB.RunMode;
		    two.U8Data[1] = 0;
		    Flashisp_GetData()->Data[FLASHIS_AdrCJMOD] 	= two.U16Data; 			    //采样模式+状态标志位
			Flashisp_GetData()->Flag=1;
			
		}			
	}

	//----------------------------------------------
	if(FluxData.ChannelC.RunMode==2)  //烟气采样过程中
	{
		FluxData.ChannelC.Sampling.SecTimer++;
		if(FluxData.ChannelC.Sampling.JpjFlg==1)  //计平均
		{
			FluxData.ChannelC.Sampling.IntTime++;  //计平均的时间计数器
			if(FluxData.ChannelC.Sampling.IntTime>=FluxData.ChannelC.Sampling.DinSiTime) //比较设定的记录时间
			{ FluxData.ChannelC.Sampling.IntTime=0; 
				//时间到保存记录
				FluxData.ChannelC.Sampling.YiciFlg=1;
				FluxData.ChannelC.Sampling.Cnt++;
				if(FluxData.ChannelC.Sampling.Cnt>FluxData.ChannelC.Sampling.Count) //计时次数完成
				{//处理
				  FluxData.ChannelC.RunMode=4; //计时完成转到
				  FluxData.ChannelC.Sampling.DXTime=FLUX_FangDaoXiTimeC;   //180秒清洗
				  FluxData.ChannelC.Sampling.EndFlg=1;//采样完成，等待保持，进入保存请这标志
				  FluxData.ChannelC.Sampling.FdxFlg=0;//
				}
			}
		}
	}
	else if(FluxData.ChannelC.RunMode==4) //清洗中
	{
		if(FluxData.ChannelC.Sampling.DXTime>0)FluxData.ChannelC.Sampling.DXTime--;
        else
        {   
			FluxData.ChannelC.Sampling.FdxFlg=1;
			FluxData.ChannelC.RunMode=0;
			Flux_StopSampling('C');   //最后停止
		}			
	}
}
//---
/*
	函数名：Flux_DataStructure* Flux_GetData(void)
	描述：获取返回Flux.c文件全局变量结构体
*/
Flux_DataStructure* Flux_GetData(void)
{
	return &FluxData;
}
//---
//----------------------------------初始化相关--------------------------------------
//---
/*
	函数名：void Flux_ChannelDataInit(Flux_ChannelStructure *p)
	描述：	该函数为初始化通道RAM数据
*/
void Flux_ChannelDataInit(Flux_ChannelStructure *p)
{
	p->RunMode = 0;
	p->ShowFlux = 0;  //显示流量为0
	Flux_FluxDataInit(&p->FluxGK);
	Flux_FluxDataInit(&p->FluxRK);
	Flux_FluxDataInit(&p->FluxCB);
	Flux_FluxDataInit(&p->FluxKD);
	Flux_FluxDataInit(&p->FluxTowData);

}
//---
/*
	函数名：void Flux_FluxDataInit(Flux_FluxStructure *p)
	描述：	初始化流量参数
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
	函数名：void Flux_ClearFluxDataSum(Flux_FluxStructure *p)
	描述：	该函数为对流量结构数据体初始化
*/
void Flux_ClearFluxDataSum(Flux_FluxStructure *p)
{
	p->Cnt = 0;
	p->Sum = 0;
}
//---
/*
	函数名：void Flux_ReadFlashSamplingData(char pump)
	描述：	初始化流量参数
*/
void Flux_ReadFlashSamplingData(char pump)
{
	u16 *pf;
	FourBytes four;
	Flux_ChannelStructure *ps;
	if(pump == 'A') 
	{
		pf = &Flashisp_GetData()->Data[FLASHIS_AdrCYSZALCYLL];   //读取流量值（寄存器）
		ps = &FluxData.ChannelA;
	}
	else if(pump == 'B') 
	{
		pf = &Flashisp_GetData()->Data[FLASHIS_AdrCYSZBLCYLL];  //读取流量值（寄存器）
		ps = &FluxData.ChannelB;
	}
	else if(pump == 'C') 
	{
		pf = &Flashisp_GetData()->Data[FLASHIS_AdrCYSZCLCYLL];  //读取流量值（寄存器）
		ps = &FluxData.ChannelC;
	}
	four.U16Data[0] = pf[0];
	four.U16Data[1] = pf[1];
	ps->SetFlux = four.FloatData;//设定采样流量-----------------------------PID的接近值

}
//---
/*
	函数名：void Flux_ReadFlashShowMode(void)
	描述：	该函数为读取A泵、B泵、TSP泵显示流量对象
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
	函数名：void Flux_FlowCalculate(void)
	描述：	该函数为计算采样流量
	工况流量计算
*/
void Flux_CalculateFlux(char pump)
{
	float differP,press,tem, b;
	Flux_ChannelStructure *p;
	//A路工况流量
	if(pump == 'A')
	{
		p = &FluxData.ChannelA;
		tem = Flux_GetJWTem()/*Slave_GetData()->YWTem.Value*/;	//输入值/测试值  ,工况的温度采用烟温值
		differP = Slave_GetData()->ACPress.Value;	//差压
		press = Slave_GetData()->AGPress.Value;		//管压
		b =0.3258;// 0.4925;	 //	//系数=计算显示*本/效验机器显示
		
	if(Flux_GetDQPress()/*Slave_GetData()->DQPress.Value*/ == FLUX_ColFAULT)//大气压故障
		press = FLUX_ColFAULT;
	else
		press += Flux_GetDQPress()/*Slave_GetData()->DQPress.Value*/;
	
	}
	//烟尘
	else if(pump == 'B')
	{
		p = &FluxData.ChannelB;
		tem = Flux_GetJWTem()/*Slave_GetData()->JWTem.Value*/;	//输入值/测试值
		differP = Slave_GetData()->BCPress.Value;	//差压
		press = Slave_GetData()->BGPress.Value;		//管压
		b =0.3842;// 0.4925;	 //	//系数=计算显示*本b/效验机器显示
		
	if(Flux_GetDQPress()/*Slave_GetData()->DQPress.Value*/ == FLUX_ColFAULT)//大气压故障
		press = FLUX_ColFAULT;
	else
	{
	if(Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&0x0040)  //选择烟温与大气压
		{
		press += Flux_GetDQPress();/*Slave_GetData()->DQPress.Value*/;
	    }
		else
		{
		press=Flux_GetDQPress();//选择大气压
		}
	}
	}
	//
	else if(pump == 'C')
	{
		p = &FluxData.ChannelC;
		tem = Flux_GetJWTem()/*Slave_GetData()->JWTem.Value*/;	//输入值/测试值
		differP = Slave_GetData()->CCPress.Value;	//差压
		press =0;		                              //计压先用零
		b = 40;										//流量计系数
													//系数
		if(Flux_GetDQPress()/*Slave_GetData()->DQPress.Value*/ == FLUX_ColFAULT)//大气压故障
		press = FLUX_ColFAULT;
	    else
		press += Flux_GetDQPress()/*Slave_GetData()->DQPress.Value*/;
		press=press;
	}
	
	
	
	//------------------
	//流量计算
	if((differP == FLUX_ColFAULT) || (press == FLUX_ColFAULT) ||(tem == FLUX_ColFAULT))
	{
		p->FluxGK.Actual = FLUX_ColFAULT;//工况流量
		p->FluxRK.Actual = FLUX_ColFAULT;
		p->FluxCB.Actual = FLUX_ColFAULT;//参比流量
		p->FluxKD.Actual = FLUX_ColFAULT;//刻度流量
	}
	else
	{//流量计算
//		FourBytes four;
	

		//工况流量
		Flux_FluxDataDealWith(&p->FluxGK,Flux_FluxCalculate(differP,tem,press,b));       //原值微调
		p->FluxGK.Actual = Flux_CalibrationFlux(pump,p->SetFlux,p->FluxGK.Actual);       //根据设置值查倍率
		//入口流量
		p->FluxRK.Actual = (float)(p->FluxGK.Actual * press / Flux_GetDQPress() * ((float)273.15 + Slave_GetData()->HWTem.Value)/((float)273.15 + tem));
		//参比流量/标况流量
		p->FluxCB.Actual = (float)(p->FluxGK.Actual * press / (float)101.325 * (float)273.15/((float)273.15 + tem));

		
	if(p->RunMode == 2)
		{//累计体积计算用
			p->FluxRK.Sum += p->FluxRK.Actual;
			p->FluxRK.Cnt++;
			p->FluxCB.Sum += p->FluxCB.Actual;
			p->FluxCB.Cnt++;
			p->FluxKD.Sum += p->FluxKD.Actual;
			p->FluxKD.Cnt++;
		}
		
	}
	Flux_ShowFluxSet(p);//显示流量

}
//---
/*
	函数名：void Flux_FluxDataDealWith(Flux_FluxStructure *p,float value)
	描述：	该函数为对流量数据进行处理，包括滤波	
	输入：	*p：被处理的流量对象
					value：采样值
*/
void Flux_FluxDataDealWith(Flux_FluxStructure *p,float value)
{float buf;
	if(value == FLUX_ColFAULT) p->Actual = FLUX_ColFAULT;
	else
	{
		buf=Flux_DataMiddleFilter(&p->Filter,value);                    //test3   //中值滤波
		p->Raw = buf;// Flux_DataFilter(&p->Filter,buf);                                //平均滤波
		p->Actual = p->Raw * p->K + p->B;
	}		
}
//---
/*
	
*/
//---
/*
	函数名：float Flux_DataFilter(Flux_FilterStructure *p,float value)
	描述：	该函数为读数据进行循环列队滤波处理，计算并返回滤波值
	输入：	p：被滤波结构体
					data：新采样值
	返回：	最新滤波值
*/
float Flux_DataFilter(Flux_FilterStructure *p,float value)
{
	if(p->Flag & B0)
	{//正常模式
		p->Sum += value;
		p->Sum -= p->Buffer[p->Cnt];
		p->Buffer[p->Cnt++] = value;
		if(p->Cnt == FLUX_FILTER_DEPTH) p->Cnt = 0;
		value = p->Sum/FLUX_FILTER_DEPTH;
	}
	else
	{//上电初始模式
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
	函数名：float Flux_DataFilter(Flux_FilterStructure *p,float value)
	描述：	该函数为读数据进行循环列队滤波处理，计算并返回滤波值
	输入：	p：被滤波结构体
					data：新采样值
	返回：	最新滤波值
*/
/*
float Flux_DataMiddleFilter(Flux_FilterStructure *p,float value)
{u8 i,j;
	float temp;
	float buf[FLUX_MIDDLEFILTER_DEPTH];
	if(p->Flag & B1)
	{//正常模式
		p->BufferMiddle[p->CntMiddle++] = value;
		if(p->CntMiddle >= FLUX_MIDDLEFILTER_DEPTH) p->CntMiddle = 0;
	}
	else
	{//上电初始模式
		p->BufferMiddle[p->CntMiddle++] = value;
		if(p->CntMiddle == FLUX_MIDDLEFILTER_DEPTH) 
		{
			p->CntMiddle = 0;
			p->Flag |= B1;
		
		}
	}
	//查找中间值
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
   p->OldMiddle=buf[(FLUX_MIDDLEFILTER_DEPTH-1)/2];  //保存上一次中值
   return temp;
}
*/
//---
/*
	函数名：float Flux_DataFilter(Flux_FilterStructure *p,float value)
	描述：	该函数为读数据进行循环列队滤波处理，计算并返回滤波值
	输入：	p：被滤波结构体
					data：新采样值
	返回：	最新滤波值
*/
float Flux_DataMiddleFilter(Flux_FilterStructure *p,float value)
{
u8 i;
	float temp,temp1;
	float buf[FLUX_MIDDLEFILTER_DEPTH];
	if(p->Flag & B1)
	{//满是保存方式
		p->BufferMiddle[p->CntMiddle++] = value;
		if(p->CntMiddle >= FLUX_MIDDLEFILTER_DEPTH) p->CntMiddle = 0;
	}
	else
	{//未满保存方式
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
	//去除最大值
	for(i=0;i<FLUX_MIDDLEFILTER_DEPTH-1;i++)
	 {
		 if(buf[i]>buf[i+1])
		 {
			 temp1=buf[i+1];
			 buf[i+1]=buf[i];
			 buf[i]=temp1;
		 }
	 }
	//去除最小值
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
	temp=temp/(FLUX_MIDDLEFILTER_DEPTH-2);    //去除最大，最小后的平均值
	temp1=temp;
	temp=temp+p->OldMiddle;
	temp=temp/2;
	p->OldMiddle=temp1;
	return temp;
	
}

/*
	函数名：Flux_ClearMidBuff(Flux_FilterStructure *p)
	描述：	清除中值滤波器的值,或值等于当前平均值
	输入：	p：被滤波结构体
					data：新采样值
	返回：	最新滤波值
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
	函数名：float Slave_GetHJTem(void)
	描述：	获取环境温度，输入值或传感器值
*/
float Flux_GetJWTem(void)
{  
	if(FluxData.JwSelectFlag)
	return(FluxData.JWbuf);
	else
	{	//if(Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&0x0040)  //选择
		//return(Slave_GetData()->HWTem.Value);    //取环温
		//else
	    return(Slave_GetData()->JWTem.Value);    //取计温 return(Slave_GetData()->JWTem.Value); 
	}
}
/*20200604
	函数名：void Flux_CalVolume(Flux_ChannelStructure *p)
	描述：	该函数为计算累计体积函数，由定时器1S调用
*/
void Flux_CalVolume(Flux_ChannelStructure *p)
{
	if(p->RunMode == 2)
	{//采样进行中
		float total=0;
		Flux_FluxStructure *pt;
		//累计体积计算
		if(p->ShowMode == 'K') pt = &p->FluxKD;//刻度流量
		else if(p->ShowMode == 'C') pt = &p->FluxCB;//参比流量
		else pt = &p->FluxRK;//工控流量
		if(pt->Cnt!=0)
		total = (pt->Sum/ pt->Cnt)/60;
		else total=0;
	
		p->Sampling.SumVolume += total;
		p->FluxRK.Sum = 0;
		p->FluxRK.Cnt = 0;
		p->FluxKD.Sum = 0;
		p->FluxKD.Cnt = 0;
		//参比体积计算
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
	函数名：float Slave_GetDQPress(void)
	描述：	获取大气压，输入值或传感器值
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
	函数名：void Flux_LoadJWTem(float Jwbuf);
	描述：	获取计温，输入值或传感器值
          select:0:计温选择检测值,1：计温选择

*/

void Flux_LoadJWTem(u8 select,float Jwbuf)   //先赋予计温临时值 -对外接口
{
	FluxData.JwSelectFlag=select;   //把标记同时设为1
	FluxData.JWbuf=Jwbuf;
}
//---
/*
	函数名：void Flux_LoadDQPress(float Jwbuf)
	描述：	获取大气压，输入值或传感器值
*/
void Flux_LoadDQPress(u8 select,float DQPressbuf) //先赋予计温临时值 -对外接口
 {
    FluxData.DQPressSelectFlag=select;
	  FluxData.DQPressbuf=DQPressbuf;
 }

//---
/*
	函数名：float Flux_CalibrationFlux(char pump,float set,float flux)
	描述：	A/B/TSP路流量校正
	输入：	pump：需要校准的泵，'A'=工况，'B'=尘泵，'C'=气泵
					set：设定流量
					flux：当前采样流量
	返回：	若pump参数错误，则返回值=flux
					若set参数错误，则返回值=flux
					所有参数正确，怎返回校准值
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
   // p = &Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_LLJZ_YCBL01+sw*2];   //烟尘的地址数据
		p = &Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_LLJZ_YCBL01];   //烟尘的地址数据 
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
				four.FloatData = 1;//其他校准
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
	else four.FloatData = 1;//参数错误
	return(flux*four.FloatData);//返回校准后流量   倍率*测量值
}
//---
/*
	函数名：void Flux_ShowFluxSet(Flux_ChannelStructure *p)
	描述：	该函数为显示需要显示的流量数据，工况、参比、刻度流量
*/
void Flux_ShowFluxSet(Flux_ChannelStructure *p)
{ Flux_ReadFlashShowMode();
	switch(p->ShowMode)
	{
		case 'R':
			{
			p->ShowFlux = p->FluxRK.Actual;
			break;//入口流量
		  }
		case 'C': 
			{
				p->ShowFlux = p->FluxCB.Actual;
			break;//参比流量
		  }
		case 'K': 
			{
			p->ShowFlux = p->FluxKD.Actual;
			break;//刻度流量
		  }
	}
	if(p->ShowFlux<0)
		p->ShowFlux=0;   //保证数据大于0
}
//---
/*
	函数名：float Flux_FluxCalculate(float differP,float tem,float press,float b)
	描述：	该函数为计算工况流量，未校准流量
	输入：	differP：压差，输入单位kPa，需要将其转换为Pa
					tem：温度，输入单位℃
					press：大气压，输入单位，单位KPa
					b：比值，常量
	返回：	流量，单位L/min
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
	函数名：u8 Flux_StarSampling(char pump,u8 mode)
	描述：	该函数为用于启动泵采样
	输入：	pump：启动泵对象，'A'=A路泵，'B'=B路泵，'T'=TSP路泵
				
	返回：	0=启动成功
					255=pumb参数错误（pumb不是'A'、'B'、'C'中的任意一个）
					254=mode参数错误
*/
u8 Flux_StarSampling(char pump,float Flux)
{
      	Flux_ChannelStructure *p;
	      u8 re=0;
	      if(pump == 'A') p = &FluxData.ChannelA;
	      else if(pump == 'B') p = &FluxData.ChannelB;
	      else if(pump == 'C') p = &FluxData.ChannelC;
	
	          Flux_ClearFluxDataSum(&p->FluxKD);  //启动前初始化
			  Flux_ClearFluxDataSum(&p->FluxGK);
			  Flux_ClearFluxDataSum(&p->FluxCB);

      	if(pump=='A')  //第一路工况烟气不需要启动
				p->SetFlux=	Flux;
			
				else if(pump=='B')
				{
				//还有初始化一些参数
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
	函数名：u8 Flux_ContinueSampling(char pump,float Flux)
	描述：	该函数为用于启动泵采样
	输入：	pump：启动泵对象，'A'=A路泵，'B'=B路泵，'T'=TSP路泵
				
	返回：	0=启动成功
					255=pumb参数错误（pumb不是'A'、'B'、'C'中的任意一个）
					254=mode参数错误
*/
u8 Flux_ContinueSampling(char pump,float Flux)
{
      	Flux_ChannelStructure *p;
	      u8 re=0;
	      if(pump == 'A') p = &FluxData.ChannelA;
	      else if(pump == 'B') p = &FluxData.ChannelB;
	      else if(pump == 'C') p = &FluxData.ChannelC;
	
	    //  Flux_ClearFluxDataSum(&p->FluxKD);  //启动前初始化
			//  Flux_ClearFluxDataSum(&p->FluxGK);
			//  Flux_ClearFluxDataSum(&p->FluxCB);

      	if(pump=='A')  //第一路工况烟气不需要启动
				p->SetFlux=	Flux;
			
				else if(pump=='B')
				{
				//还有初始化一些参数
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
	函数名：u8 Flux_StopSampling(char pump,u8 mode)
	描述：	该函数为用于停止泵采样
	输入：	pump：启动泵对象，'A'=A路，'B'=B路尘泵，'C'=气泵
					
	返回：	0=启动成功
					255=pumb参数错误（pumb不是'A'、'B'、'C'中的任意一个）
*/
u8 Flux_StopSampling(char pump)
{ 


	     u8 re=0;
				if(pump=='A')  //第一路工况烟气不需要启动
					;
				else if(pump=='B')
				{
				//还有初始化一些参数
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
	函数名：void Flux_SaveSamplingRunData(void)
	描述：	该函数为保存采样运行数据
*/
void Flux_SaveSamplingRunData(void)
{
	TwoBytes two;
	FourBytes four;
	Flux_ChannelStructure *ps;
	Flashisp_DataStructure *pf = Flashisp_GetData();

	//b路采样数据保存
	ps = &FluxData.ChannelB;
	if(ps->RunMode) //启动运行就开始每秒钟保存
	{
		two.U8Data[0] = ps->RunMode;
		two.U8Data[1] = 0;
		pf->Data[FLASHIS_AdrCJMOD] 	= two.U16Data; 			    //采样模式+状态标志位
		
		//开始时刻

	    four.U32Data = mktime(&Screen_GetData()->Clock.Clock);
		Flashisp_GetData()->Data[FLASHIS_AdrDDTime] = four.U16Data[0];
		Flashisp_GetData()->Data[FLASHIS_AdrDDTime+1] = four.U16Data[1];
		
		four.U32Data=ps->Sampling.SecTimer;
		pf->Data[FLASHIS_AdrCJLJSH] 	= four.U16Data[0];	    //采样计时
		pf->Data[FLASHIS_AdrCJLJSH+1]   = four.U16Data[1];
		
		pf->Data[FLASHIS_AdrCJCNT] 	= ps->Sampling.Cnt;			//采样点数
		
		four.FloatData = ps->Sampling.SumVolume;				//累计体积
		pf->Data[FLASHIS_AdrCJGKTJ]   = four.U16Data[0];
		pf->Data[FLASHIS_AdrCJGKTJ+1] = four.U16Data[1];
		
		four.FloatData = ps->Sampling.RefVolume;				//参比体积
		pf->Data[FLASHIS_AdrCJBQTJ]   = four.U16Data[0];
		pf->Data[FLASHIS_AdrCJBQTJ+1] = four.U16Data[1];

		four.FloatData =ps->SaveFileNumber;                     //文件号
		pf->Data[FLASHIS_AdrCJWJH]=four.U16Data[0];  
		pf->Data[FLASHIS_AdrCJWJH+1]=four.U16Data[1]; 

//		printf("保存数据");
		pf->Flag = 1;
	}
}
//------------------------------------------------------------------------
//---
/*20200602
	函数名：void Flux_ChannelRun_B(char pump)
	描述：	该函数为通道运行主函数
    这线程主要是全程监控，并带数据保存
*/
void Flux_ChannelRun(char pump)
{
	Flux_ChannelStructure *p;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	if(pump == 'A') p = &FluxData.ChannelA;
	else if(pump == 'B') p = &FluxData.ChannelB;
	else if(pump == 'C') p = &FluxData.ChannelC;
	if(p->RunMode != 0)  //空闲
	switch(p->RunMode)//看作是状态
	{
		case 1: //启动初始化(启动命令)
		{
			if(pump == 'B')    //烟尘
			{
				p->Sampling.SecTimer=0;  //从零开始
				p->Sampling.IntTime=0;  //从零开始
			
				p->Sampling.DinSiTime=(u32)pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]; //单点采样时间
				p->Sampling.Count=pf->Data[FLASHIS_AdrYDBD_CYDS]; //总采样点
				p->Sampling.SamTime=(pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS])*p->Sampling.Count;//总时长
	
				p->Sampling.Cnt=1;  //第0次开始计数
				p->Sampling.SumVolume=0;  //累计体积
				p->Sampling.RefVolume=0;  //累计体积
				p->Sampling.YiciFlg=0;
				p->Sampling.EndFlg=0;
				p->Sampling.FdxFlg=0; //防倒吸完成标志
				p->FluxRK.Sum = 0;
		        p->FluxRK.Cnt = 0;
		        p->FluxKD.Sum = 0;
		        p->FluxKD.Cnt = 0;
		        p->FluxCB.Sum = 0;
		        p->FluxCB.Cnt = 0;

			}
			else if(pump == 'C')//烟气
			{
				p->Sampling.SecTimer=0;  //从零开始
				p->Sampling.IntTime=0;  //从零开始
				p->Sampling.SamTime=(u32)pf->Data[FLASHIS_AdrCLYQ_JSSC]; //先取采样总长
				p->Sampling.DinSiTime=(u32)pf->Data[FLASHIS_AdrXTSZYQCLSJ]*60; //定时保存时间,分钟化成秒
				p->Sampling.Count=pf->Data[FLASHIS_AdrXTSZYQCLCS];  //计数次数
				p->Sampling.Cnt=1;  //第0次开始计数
				p->Sampling.SumVolume=0;  //累计体积
				p->Sampling.RefVolume=0;  //累计体积
				p->Sampling.YiciFlg=0;
				p->Sampling.EndFlg=0;
				p->Sampling.FdxFlg=0; //防倒吸完成标志
				p->Sampling.AutoMod=(u8)pf->Data[FLASHIS_AdrCLYQ_CLFS];  //手动自动
				p->Sampling.JpjFlg=0; //刚开始不在计平均状态
				p->FluxRK.Sum = 0;
		        p->FluxRK.Cnt = 0;
		        p->FluxKD.Sum = 0;
		        p->FluxKD.Cnt = 0;
		        //参比体积计算
	
		        p->FluxCB.Sum = 0;
		        p->FluxCB.Cnt = 0;

			}
			p->RunMode=2;
			break;
		}
		case 2: //采样中()
		{
			if(pump == 'B')    //烟尘
			{
				
			}
			else if(pump == 'C')//烟气
			{
				
			}
			break;
		}
		case 3: //暂停中
		{
			if(pump == 'B')    //烟尘
			{
				
			}
			else if(pump == 'C')//烟气
			{
				
			}
			break;
		}
		case 4: //防倒吸中
		{
			if(pump == 'B')    //烟尘
			{
				
			}
			else if(pump == 'C')//烟气清洗
			{
				
			}
			break;
		}
		case 5: //暂停中有防倒吸
		{
			if(pump == 'B')    //烟尘
			{
				
			}
			break;
		}
		
	}
}


//---
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Flux_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Flux文件的ASCII调试文件
	命令：	 :000|10|XXX
*/
void Flux_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 101:
		{//以某流量/启动烟气泵
		   Flux_StarSampling('C',0.8);  //对外接口
			 printf("以0.8L/min 启动气泵\r\n");
			 break;
		}
		case 201:
		{//停止烟气泵
		  Flux_StopSampling('C');
			printf("关闭气泵\r\n");
			break;
		}
		case 301:
		{//以某占空比启动泵TSP电机
			//以某流量/启动烟气泵
		   Flux_StarSampling('B',50.0);  //对外接口
			 printf("以50L/min 启动尘泵\r\n");
		  break;
		}
		case 401:
		{//停止烟气泵
		  Flux_StopSampling('B');
			printf("关闭尘泵\r\n");
			break;
		}
		case 501:
		{//读取原始数据
			printf("以下数据没有校准\r\n");
			printf("A路工况流量：%f\r\n",FluxData.ChannelA.FluxGK.Raw);
			printf("B路工况流量：%f\r\n",FluxData.ChannelB.FluxGK.Raw);
			printf("C路工况流量：%f\r\n",FluxData.ChannelC.FluxGK.Raw);
			break;
		}
		case 502:
		{//读取原始数据
			printf("以下数据为校准数据\r\n");
			printf("A路入口流量：%f\r\n",FluxData.ChannelA.ShowFlux);
			printf("B路入口流量：%f\r\n",FluxData.ChannelB.ShowFlux);
			printf("C路入口流量：%f\r\n",FluxData.ChannelC.ShowFlux);
			break;
		}
	}
}
//---
/*
	函数名： 	void Flux_DebugRTU((u8 *p)
	描述：		该函数为Flux文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*//*
void Flux_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//查看是哪个功能码
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






























