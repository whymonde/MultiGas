/*
	本文件为该项目的与从机交互相关函数
	具体函数如下
*/

#include "slave.h"
#include "CalculationFormula.h"
Slave_DataStructure SlaveData;
/*
	函数名：void Slave_Main(void)
	描述：	heat.c主函数
*/
void Slave_Main(void)
{
	
	Slave_Communicate();  //下发数据，与接收数据(采集数据过程)
	Slave_GetSensorData();//把得到的数据进行数据转换
	//Slave_MotorRun(&SlaveData.AMotor,Flux_GetData()->ChannelA.ShowFlux);
	Slave_MotorRun(&SlaveData.DustMotor,Flux_GetData()->ChannelB.ShowFlux);
    Slave_MotorRun(&SlaveData.GasMotor,Flux_GetData()->ChannelC.ShowFlux);
}
//---
/*
	函数名：void Slave_Init(void)
	描述：	slave.c文件初始化函数
*/
void Slave_Init(void)
{
	Slave_InitComm();
	Slave_InitRawData();

	//初始化参数
	Slave_InitActualData(&SlaveData.HWTem);
	Slave_InitActualData(&SlaveData.GQTem);
	Slave_InitActualData(&SlaveData.SQTem);
	Slave_InitActualData(&SlaveData.YWTem);
	Slave_InitActualData(&SlaveData.JWTem);
	
	Slave_InitActualData(&SlaveData.DQPress);
	Slave_InitActualData(&SlaveData.ACPress);
	Slave_InitActualData(&SlaveData.AGPress);
	Slave_InitActualData(&SlaveData.BCPress);
	Slave_InitActualData(&SlaveData.BGPress);
	Slave_InitActualData(&SlaveData.CCPress);
	
	Slave_InitActualData(&SlaveData.O2Concent);
	Slave_InitActualData(&SlaveData.SO2Concent);
	Slave_InitActualData(&SlaveData.NOConcent);
	Slave_InitActualData(&SlaveData.NO2Concent);
	Slave_InitActualData(&SlaveData.COConcent);
	Slave_InitActualData(&SlaveData.H2SConcent);
	Slave_InitActualData(&SlaveData.CO2Concent);
	
	
	Slave_InitActualData(&SlaveData.BeforO2Concent);
	Slave_InitActualData(&SlaveData.BeforSO2Concent);
	Slave_InitActualData(&SlaveData.BeforNOConcent);
	Slave_InitActualData(&SlaveData.BeforNO2Concent);
	Slave_InitActualData(&SlaveData.BeforCOConcent);
	Slave_InitActualData(&SlaveData.BeforH2SConcent);
	Slave_InitActualData(&SlaveData.BeforCO2Concent);
	
	

	//更新flash数据,读取零点数据
	Slave_ReadFlashData();
	
	
	//电机相关初始化
	
	SlaveData.WaterMotor.rame='W';
	SlaveData.SlvdMotor.rame='S';
	SlaveData.GasMotor.rame='G';    //烟气
	SlaveData.DustMotor.rame='D';   //烟尘
	
	Slave_MotorInit(&SlaveData.WaterMotor);
	Slave_MotorInit(&SlaveData.SlvdMotor);
	Slave_MotorInit(&SlaveData.GasMotor);
	Slave_MotorInit(&SlaveData.DustMotor);

	Slave_MotorPidDataInit(&SlaveData.WaterMotor);            //把PID参数初始化
	Slave_MotorPidDataInit(&SlaveData.SlvdMotor);
	Slave_MotorPidDataInit(&SlaveData.GasMotor);
	Slave_MotorPidDataInit(&SlaveData.DustMotor);
	
	//E28 2.4G模块数据寄存器初始化
	SlaveData.E28.STATE=0;
	SlaveData.E28.ADRRRD24G=0;
	SlaveData.E28.CHRD24G=0;
	SlaveData.E28.ADRRset=0;
	SlaveData.E28.CHset=0;
}
//---
/*
	函数名：void Slave_Timer(void)
	描述：slave.c文件定时器函数，由系统1ms定时器调用
*/
void Slave_Timer(void)
{
	if(SlaveData.Comm.Timer != 0)SlaveData.Comm.Timer--;	//通讯定时器
	if(SlaveData.GetTimer != 0)SlaveData.GetTimer--;			//数据采集定时器
	if(SlaveData.GasMotor.Timer!= 0)SlaveData.GasMotor.Timer--;			//数据采集定时器
	if(SlaveData.DustMotor.Timer!= 0)SlaveData.DustMotor.Timer--;			//数据采集定时器

	/*
	//归零定时器
	if(SlaveData.DQPress.Timer != 0)SlaveData.DQPress.Timer--;  //大气压
	if(SlaveData.ACPress.Timer != 0)SlaveData.ACPress.Timer--;
	if(SlaveData.AGPress.Timer != 0)SlaveData.AGPress.Timer--;
	if(SlaveData.BCPress.Timer != 0)SlaveData.BCPress.Timer--;
	if(SlaveData.BGPress.Timer != 0)SlaveData.BGPress.Timer--;
	if(SlaveData.CCPress.Timer != 0)SlaveData.CCPress.Timer--;
	*/
	
	SlaveData.WaterMotor.Pid.Timer++;
	SlaveData.SlvdMotor.Pid.Timer++;
	SlaveData.GasMotor.Pid.Timer++;
	SlaveData.DustMotor.Pid.Timer++;
	
}
//---
/*
	函数名：Slave_DataStructure* Slave_GetData(void)
	描述：获取返回slave.c文件全局变量结构体变量
*/
Slave_DataStructure* Slave_GetData(void)
{
	return &SlaveData;
}
//---
/*
	函数名：void Slave_InitComm(void)
	描述：	该函数为通讯初始化
*/
void Slave_InitComm(void)
{
	SlaveData.Comm.RunMode = 1;
	SlaveData.Comm.ErrorCnt = 20;
}
//---
/*
	函数名：void Slave_InitRawData(void)
	描述：	该函数为相关参数初始化
*/
void Slave_InitRawData(void)
{
	u8 i;
	for(i=0;i<SLAVE_DataMax;i++)                //初始化采样的数据
	{
		SlaveData.Data[i].Filter.Cnt = 0;
		SlaveData.Data[i].Filter.Flag = 0;   //开始为0，还没有保存整个缓冲区时状态
		SlaveData.Data[i].Filter.Sum = 0;
		SlaveData.Data[i].Filter.CntMiddle= 0;
		SlaveData.Data[i].Raw = SLAVE_ColFAULT;
	}
}
//---
/*
	函数名：void Slave_InitActualData(Slave_TPActualStructure *p)
	描述：	该函数为初始化环境温度相关参数
*/
#include <stm32f4xx.h>
void Slave_InitActualData(Slave_TPActualStructure *p)
{
	p->Value = SLAVE_ColFAULT;
	p->Mode.All = 0;
	p->Cnt = 0;
	p->Sum = 0;
	p->Max = 0;
	p->Min = 0;
	p->Timer = 0;
	p->OldRaw = 0;
	p->K=1;   //
	p->B=0;   //
}
//---
/*
	函数名：void Slave_ReadFlashData(void)
	描述：	该函数为读取Flash数据关键获取零点数据到数据结构的B
*/
void Slave_ReadFlashData(void)  //关键是获取零点数据
{
 //零点
  Slave_ReadFlashZOData(&SlaveData.HWTem,FLASHIS_AdrWHJZ_WDJZ_LDHW,SLAVE_BaseTem);
  Slave_ReadFlashZOData(&SlaveData.GQTem,FLASHIS_AdrWHJZ_WDJZ_LDGQ,SLAVE_BaseTem);
  Slave_ReadFlashZOData(&SlaveData.SQTem,FLASHIS_AdrWHJZ_WDJZ_LDSQ,SLAVE_BaseTem);
  Slave_ReadFlashZOData(&SlaveData.YWTem,FLASHIS_AdrWHJZ_WDJZ_LDYW,SLAVE_BaseTem);
  Slave_ReadFlashZOData(&SlaveData.JWTem,FLASHIS_AdrWHJZ_WDJZ_LDJW,SLAVE_BaseTem);
	
  Slave_ReadFlashZOData(&SlaveData.DQPress,FLASHIS_AdrWHJZ_YLJZ_DQY,SLAVE_BaseCPress);
  Slave_ReadFlashZOData(&SlaveData.ACPress,FLASHIS_AdrWHJZ_YLJZ_DY,SLAVE_BaseCPress);
  Slave_ReadFlashZOData(&SlaveData.AGPress,FLASHIS_AdrWHJZ_YLJZ_JY,SLAVE_BaseCPress);
  Slave_ReadFlashZOData(&SlaveData.BCPress,FLASHIS_AdrWHJZ_YLJZ_CLY,SLAVE_BaseCPress);
  Slave_ReadFlashZOData(&SlaveData.BGPress,FLASHIS_AdrWHJZ_YLJZ_JJY,SLAVE_BaseCPress);
  Slave_ReadFlashZOData(&SlaveData.CCPress,FLASHIS_AdrWHJZ_YLJZ_QLY,SLAVE_BaseCPress);
	
  Slave_ReadFlashZOData(&SlaveData.O2Concent,FLASHIS_AdrWHJZ_NDJZ_LDO2,SLAVE_BaseGas);
  Slave_ReadFlashZOData(&SlaveData.SO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDSO2,SLAVE_BaseGas);
  Slave_ReadFlashZOData(&SlaveData.NOConcent,FLASHIS_AdrWHJZ_NDJZ_LDNO,SLAVE_BaseGas);
  Slave_ReadFlashZOData(&SlaveData.NO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDNO2,SLAVE_BaseGas);
  Slave_ReadFlashZOData(&SlaveData.COConcent,FLASHIS_AdrWHJZ_NDJZ_LDCO,SLAVE_BaseGas);
  Slave_ReadFlashZOData(&SlaveData.CO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDCO2,SLAVE_BaseGas);
  Slave_ReadFlashZOData(&SlaveData.H2SConcent,FLASHIS_AdrWHJZ_NDJZ_LDH2S,SLAVE_BaseGas);
  //倍率
  Slave_ReadFlashBLData(&SlaveData.GQTem,FLASHIS_AdrWHJZ_WDJZ_BLGQ); //干球倍率
  Slave_ReadFlashBLData(&SlaveData.JWTem,FLASHIS_AdrWHJZ_WDJZ_BLYW);//烟温倍率
  Slave_ReadFlashBLData(&SlaveData.SQTem,FLASHIS_AdrWHJZ_WDJZ_BLSQ);//湿球倍率
  
  Slave_ReadFlashBLData(&SlaveData.O2Concent,FLASHIS_AdrWHJZ_NDJZ_BLO2_1);   //氧气的三个倍率K
  Slave_ReadFlashBLDatak1(&SlaveData.O2Concent,FLASHIS_AdrWHJZ_NDJZ_BLO2_2);  //K1
  Slave_ReadFlashBLDatak2(&SlaveData.O2Concent,FLASHIS_AdrWHJZ_NDJZ_BLO2_3);  //K2  
   
  Slave_ReadFlashBLData(&SlaveData.SO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLSO2_1);   //SO2的三个倍率K
  Slave_ReadFlashBLDatak1(&SlaveData.SO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLSO2_2);  //K1
  Slave_ReadFlashBLDatak2(&SlaveData.SO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLSO2_3);  //K2
  
  Slave_ReadFlashBLData(&SlaveData.NOConcent,FLASHIS_AdrWHJZ_NDJZ_BLNO_1);   //NO的三个倍率K
  Slave_ReadFlashBLDatak1(&SlaveData.NOConcent,FLASHIS_AdrWHJZ_NDJZ_BLNO_2);  //K1
  Slave_ReadFlashBLDatak2(&SlaveData.NOConcent,FLASHIS_AdrWHJZ_NDJZ_BLNO_3);  //K2 
  
  Slave_ReadFlashBLData(&SlaveData.NO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLNO2_1);   //NO的三个倍率K
  Slave_ReadFlashBLDatak1(&SlaveData.NO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLNO2_2);  //K1
  Slave_ReadFlashBLDatak2(&SlaveData.NO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLNO2_3);  //K2 
  
  Slave_ReadFlashBLData(&SlaveData.COConcent,FLASHIS_AdrWHJZ_NDJZ_BLCO_1);   //NO的三个倍率K
  Slave_ReadFlashBLDatak1(&SlaveData.COConcent,FLASHIS_AdrWHJZ_NDJZ_BLCO_2);  //K1
  Slave_ReadFlashBLDatak2(&SlaveData.COConcent,FLASHIS_AdrWHJZ_NDJZ_BLCO_3);  //K2 
  
  Slave_ReadFlashBLData(&SlaveData.H2SConcent,FLASHIS_AdrWHJZ_NDJZ_BLH2S_1);   //NO的三个倍率K
  Slave_ReadFlashBLDatak1(&SlaveData.H2SConcent,FLASHIS_AdrWHJZ_NDJZ_BLH2S_2);  //K1
  Slave_ReadFlashBLDatak2(&SlaveData.H2SConcent,FLASHIS_AdrWHJZ_NDJZ_BLH2S_3);  //K2 
  
  Slave_ReadFlashBLData(&SlaveData.CO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLCO2_1);   //NO的三个倍率K
  Slave_ReadFlashBLDatak1(&SlaveData.CO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLCO2_2);  //K1
  Slave_ReadFlashBLDatak2(&SlaveData.CO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLCO2_3);  //K2 
  
  //
}


//---
/*
	函数名：void Slave_ReadFlashZOData()
	描述：	该函数为读取H2S零点相关Flash数据
*/
void Slave_ReadFlashZOData(Slave_TPActualStructure *P,u32 adrr,float offset)
{
	P->B = Slave_ReadTPFlashData(adrr) - offset;
}

//---
/* 20200524
	函数名：Slave_ReadFlashBLData()
	描述：	该函数为读取倍率相关Flash数据
*/
void Slave_ReadFlashBLData(Slave_TPActualStructure *P,u32 adrr)
{
	P->K = Slave_ReadTPFlashData(adrr);
}
void Slave_ReadFlashBLDatak1(Slave_TPActualStructure *P,u32 adrr)
{
	P->K1 = Slave_ReadTPFlashData(adrr);
}
void Slave_ReadFlashBLDatak2(Slave_TPActualStructure *P,u32 adrr)
{
	P->K2 = Slave_ReadTPFlashData(adrr);
}

//---
/*
	函数名：float Slave_ReadTPFlashData(u32 addr)
	函数名：该函数为读取温度、压力校准数据的Flash两个字节的浮点型数据
	输入：	u32 addr：flash内存地址
*/
float Slave_ReadTPFlashData(u32 addr)
{ 
	FourBytes Cfour;
	u16* p= Flashisp_GetData()->Data;
	Cfour.U16Data[0] = p[addr];
	Cfour.U16Data[1] = p[addr+1];
	return(Cfour.FloatData);
	
}
//---
/*
	函数名：void Slave_GetSensorData(void)
	描述：	该函数为获取传感器数据
*/
void Slave_GetSensorData(void)
{
	if(SlaveData.GetTimer == 0) 
	{
		SlaveData.GetTimer = SLAVE_SensorRefreshTime;
	//Slave_DataDealWith(&SlaveData.Data[10],DS18B20_GetData()->Tem);//主板DS18B20
		{//重新映射
			
			Slave_DataMaping(&SlaveData.JWTem,SlaveData.Data[SLAVE_MAP_JWTem].Raw);	   //计温   //主板的DS18B20
			Slave_DataMaping(&SlaveData.HWTem,SlaveData.Data[SLAVE_MAP_HWTem].Raw);	   //环境温度副板5805上温度
 			//Slave_DataMaping(&SlaveData.HWTem,DS18B20_GetData()->Tem);	   //环境温度主板上保温位置 *************
			if(SlaveData.E28.STATERD24G&0x01)  //这里判断
			{
				
			//Slave_DataMaping(&SlaveData.YWTem,SlaveData.Data[SLAVE_MAP_E28YW].Raw);//烟温温度  20200525
				
			SlaveData.YWTem.Value=	SlaveData.Data[SLAVE_MAP_E28YW].Raw;
			Slave_DataMaping(&SlaveData.ACPress,SlaveData.Data[SLAVE_MAP_E28DY].Raw*1000);//A路 动压 把数据转换成PA
			Slave_DataMaping(&SlaveData.AGPress,SlaveData.Data[SLAVE_MAP_E28JY].Raw);//A路     全压Kpa
				
			}
			else
			{
			//Slave_DataMaping(&SlaveData.YWTem,(SlaveData.Data[SLAVE_MAP_YWTem].Raw-AdMin)*PT100_K+PT100_B);//烟温温度  20200525
			Slave_DataMaping(&SlaveData.YWTem,CalcAdcToTemperature(SlaveData.Data[SLAVE_MAP_YWTem].Raw));//烟温温度 20200525				
			Slave_DataMaping(&SlaveData.ACPress,SlaveData.Data[SLAVE_MAP_ACPress].Raw*1000);//A路 动压 把数据转换成PA
			Slave_DataMaping(&SlaveData.AGPress,SlaveData.Data[SLAVE_MAP_AGPress].Raw);//A路      全压KPA
			}

			//Slave_DataMaping(&SlaveData.GQTem,(SlaveData.Data[SLAVE_MAP_GQTem].Raw-AdMin)*PT100_K+PT100_B);//干球温度  20200525
			//Slave_DataMaping(&SlaveData.SQTem,(SlaveData.Data[SLAVE_MAP_SQTem].Raw-AdMin)*PT100_K+PT100_B);//湿球温度  20200525
			Slave_DataMaping(&SlaveData.GQTem,CalcAdcToTemperature(SlaveData.Data[SLAVE_MAP_GQTem].Raw));//干球温度  20200525

			Slave_DataMaping(&SlaveData.SQTem,CalcAdcToTemperature(SlaveData.Data[SLAVE_MAP_SQTem].Raw));//湿球温度  20200525
		
			Slave_DataMaping(&SlaveData.DQPress,SlaveData.Data[SLAVE_MAP_DQPress].Raw);//大气压

			Slave_DataMaping(&SlaveData.BCPress,SlaveData.Data[SLAVE_MAP_BCPress].Raw*1000);//尘流压   把数据转换成PA
			
			Slave_DataMaping(&SlaveData.BGPress,SlaveData.Data[SLAVE_MAP_BGPress].Raw);//d路差压（流压）
			Slave_DataMaping(&SlaveData.CCPress,SlaveData.Data[SLAVE_MAP_CCPress].Raw*1000);//e路气流压     把数据转换成PA
			
	
		}
		Flux_CalculateFlux('A');
		Flux_CalculateFlux('B');
		Flux_CalculateFlux('C');
		//气体计算
		CalculationGas();
		//计算所有需要的数据
		CalculationFormula_All();
	}
}
//---
/*
	函数名：void Slave_DataDealWith(Flux_FluxStructure *p,float value)
	描述：	该函数为对温度、压力数据进行处理，包括滤波	
	输入：	*p：被处理的温度/压力/流量对象
					value：采样值
*/
void Slave_DataDealWith(Slave_TPRawStructure *p,float value,u8 flag)
{   float buf;
	if(value == SLAVE_ColFAULT)
	{	p->Raw = SLAVE_ColFAULT;
	}
	else 
	{  buf=Slave_DataMiddleFilter(&p->Filter,value);
		if(flag)
		{
         p->Raw = Slave_DataFilter(&p->Filter,buf);
		}
		else
		{
		 p->Raw =buf;	
		}
	}
}
//---
/*
	函数名：float Slave_DataFilter(Slave_FilterStructure *p,float value)
	描述：	该函数为读数据进行循环列队滤波处理，计算并返回滤波值
	输入：	p：被滤波结构体
					data：新采样值
	返回：	最新滤波值
*/
float Slave_DataFilter(Slave_FilterStructure *p,float value)
{
	if(p->Flag & B0)   //这种方法减小时间开消
	{//正常模式
		p->Sum += value;             //加上新值
		p->Sum -= p->Buffer[p->Cnt]; //减去旧对应位置值 
		p->Buffer[p->Cnt++] = value;
		if(p->Cnt == SLAVE_FILTER_DEPTH) p->Cnt = 0;
		value = p->Sum/SLAVE_FILTER_DEPTH;
	}
	else
	{//上电初始模式
		p->Buffer[p->Cnt++] = value;
		p->Sum += value;
		if(p->Cnt == SLAVE_FILTER_DEPTH) 
		{
			p->Cnt = 0;
			p->Flag |= B0;
			value = p->Sum/SLAVE_FILTER_DEPTH;
		}
	}
	return value;
}

//---
/*
	Slave_DataMiddleFilter(Flux_FilterStructure *p,float value)
    中·值滤波·
*/

float Slave_DataMiddleFilter(Slave_FilterStructure *p,float value)
{   u8 i;
	float temp,temp1;
	float buf[SLAVE_MIDDLEFILTER_DEPTH];
	if(p->Flag & B1)
	{//满是保存方式
		p->BufferMiddle[p->CntMiddle++] = value;
		if(p->CntMiddle >= SLAVE_MIDDLEFILTER_DEPTH) p->CntMiddle = 0;
	}
	else
	{//未满保存方式
		p->BufferMiddle[p->CntMiddle++] = value;
		if(p->CntMiddle >=SLAVE_MIDDLEFILTER_DEPTH) 
		{
			p->CntMiddle = 0;
			p->Flag |= B1;
		
		}
	}
	//
	for(i=0;i<SLAVE_MIDDLEFILTER_DEPTH;i++)
	buf[i]=p->BufferMiddle[i];
	//去除最大值
	for(i=0;i<SLAVE_MIDDLEFILTER_DEPTH-1;i++)
	 {
		 if(buf[i]>buf[i+1])
		 {
			 temp1=buf[i+1];
			 buf[i+1]=buf[i];
			 buf[i]=temp1;
		 }
	 }
	//去除最小值
	for(i=0;i<SLAVE_MIDDLEFILTER_DEPTH-2;i++)
	 {
		  if(buf[i]<buf[i+1])
		 {
			 temp1=buf[i+1];
			 buf[i+1]=buf[i];
			 buf[i]=temp1;
		 }
	 }
	

	temp=0;
	for(i=0;i<(SLAVE_MIDDLEFILTER_DEPTH-2);i++)
	temp=temp+buf[i];
	temp=temp/(SLAVE_MIDDLEFILTER_DEPTH-2);    //去除最大，最小后的平均值
	temp1=temp;
	temp=temp+p->OldMiddle;  //滑动平均
	temp=temp/2;
    p->OldMiddle=temp1;
   return temp;
}

//---
/*
	函数名：void Slave_DataMaping(Slave_TPActualStructure *p,float value)
	描述：	该函数是将传感器数值映射为对象值
*/
void Slave_DataMaping(Slave_TPActualStructure *p,float value)
{
	if(value == SLAVE_ColFAULT) p->Value = SLAVE_ColFAULT;
	else
	{//数据处理	
		p->Value = value * p->K + p->B;
	
		switch(p->Mode.Bits.ActualMode)
		{//Actual端处理
			case 1:
			{//对于流压值小于1时认为值为0
				if(fabs(p->Value*1000) < 1) p->Value = 0;
				break;
			}
			case 2:
			{
				if(fabs(p->Value*1000) < 5) p->Value = 0;
				break;
			}
		}
	}
}

//---
/*
	函数名：void Slave_StopZero(Slave_TPActualStructure *p)
	描述：	该函数为调零
	输入：	Slave_TPActualStructure *p：调零对象
          Addr fla
					
*/


void Slave_Zero(Slave_TPActualStructure *p,u32 Addr,float offset) //并保存到flash
{
	FourBytes four;
	u16* PTR= Flashisp_GetData()->Data;
	p->B=p->B-p->Value;                      //改变偏移量加上测量值得到新的偏移量
	four.FloatData=p->B+offset;              //20200703
	PTR[Addr]=four.U16Data[0];
	PTR[Addr+1]=four.U16Data[1];

}
//---
/*
	函数名：void Slave_Zero_Press()
	描述：	该函数为调零
	输入：	所有压力值为调零对象
 
*/
void Slave_Zero_Press(void)  //所以压力值调零
{
//	Slave_Zero(&SlaveData.DQPress,FLASHIS_AdrWHJZ_YLJZ_DQY);
   	Slave_Zero(&SlaveData.ACPress,FLASHIS_AdrWHJZ_YLJZ_DY,SLAVE_BaseCPress);
	Slave_Zero(&SlaveData.AGPress,FLASHIS_AdrWHJZ_YLJZ_JY,SLAVE_BaseCPress);
	Slave_Zero(&SlaveData.BCPress,FLASHIS_AdrWHJZ_YLJZ_CLY,SLAVE_BaseCPress);
	Slave_Zero(&SlaveData.BGPress,FLASHIS_AdrWHJZ_YLJZ_JJY,SLAVE_BaseCPress);
    Slave_Zero(&SlaveData.CCPress,FLASHIS_AdrWHJZ_YLJZ_QLY,SLAVE_BaseCPress);
	Flashisp_GetData()->Flag=1; //启动保持
}
//---
/*
	函数名：void Slave_Zero_Tem()
	描述：	该函数为调零
	输入：	所有温度值为调零对象
 
*/
void Slave_Zero_Tem(void)  //所有压力值调零
{
  Slave_Zero(&SlaveData.HWTem,FLASHIS_AdrWHJZ_WDJZ_LDHW,SLAVE_BaseTem);
  Slave_Zero(&SlaveData.GQTem,FLASHIS_AdrWHJZ_WDJZ_LDGQ,SLAVE_BaseTem);
  Slave_Zero(&SlaveData.SQTem,FLASHIS_AdrWHJZ_WDJZ_LDSQ,SLAVE_BaseTem);
  Slave_Zero(&SlaveData.YWTem,FLASHIS_AdrWHJZ_WDJZ_LDYW,SLAVE_BaseTem);
  Slave_Zero(&SlaveData.JWTem,FLASHIS_AdrWHJZ_WDJZ_LDJW,SLAVE_BaseTem);
	
  Flashisp_GetData()->Flag=1; //启动保持
}
//---

/*
	函数名：void Slave_Zero_QT()
	描述：	该函数为调零
	输入：	所有气体值为调零对象
 
*/
void Slave_Zero_QT(void)  //所有压力值调零
{
	
	if((Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>0)&0x01)  //判断有没有安装
	{
    SlaveData.O2Concent.Value-=21;		
	Slave_Zero(&SlaveData.O2Concent,FLASHIS_AdrWHJZ_NDJZ_LDO2,SLAVE_BaseGas);
	//SlaveData.O2Concent.B=	SlaveData.O2Concent.B+21;
		
	}
	if((Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>1)&0x01)  //判断有没有安装
    Slave_Zero(&SlaveData.SO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDSO2,SLAVE_BaseGas);
	
	if((Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>2)&0x01)  //判断有没有安装
    Slave_Zero(&SlaveData.NOConcent,FLASHIS_AdrWHJZ_NDJZ_LDNO,SLAVE_BaseGas);
	
	if((Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>3)&0x01)  //判断有没有安装
    Slave_Zero(&SlaveData.NO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDNO2,SLAVE_BaseGas);
	
	if((Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>4)&0x01)  //判断有没有安装
    Slave_Zero(&SlaveData.COConcent,FLASHIS_AdrWHJZ_NDJZ_LDCO,SLAVE_BaseGas);
	
	if((Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>5)&0x01)  //判断有没有安装
	Slave_Zero(&SlaveData.H2SConcent,FLASHIS_AdrWHJZ_NDJZ_LDH2S,SLAVE_BaseGas);
	
	if((Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>6)&0x01)  //判断有没有安装
	{
	SlaveData.CO2Concent.Value=SlaveData.CO2Concent.Value-0.3f;
	Slave_Zero(&SlaveData.CO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDCO2,SLAVE_BaseGas);
	//SlaveData.CO2Concent.B=SlaveData.CO2Concent.B+0.3;
	}
	Flashisp_GetData()->Flag=1; //启动保持
}




//---
//---
//-------------------------电机相关----------------------------
//---
/*
	函数名：void Slave_MotorRun(Slave_MotorStructure *p,float pv)
	描述：	电机控制
	输入：	*p：被控制的电机
			pv：最新采样值
*/
void Slave_MotorRun(Slave_MotorStructure *p,float pv)
{   Slave_MotorPidPara(p);   //根据当前流量大小调节PID参数
	switch(p->RunMode)
	{
		case 1:
		{
			p->RunMode=2;
			p->Timer=1000;  //稳定2秒
			p->Ratio =p->Pid.OUT0;
			p->OnOff = 1;
		}
		case 2:
		{

			if(p->Timer==0)
			{  
				p->RunMode=3;
				p->Timer=1500;  //稳定2秒
				p->Pid.OUT0=p->Pid.OUT0+30;
			    p->Ratio =p->Pid.OUT0;
			    p->OnOff = 1;
			}
			break;
		}
		case 3:
			{  
			if(p->Timer==0)
			{
				p->RunMode=4;
				p->Pid.OUT=p->Pid.OUT0;
			}
			break;
		}
		case 4:
		{
			//采集模式
			if(pv != SLAVE_ColFAULT)
			{//非故障
				p->Pid.Pv = pv;//最新采集流量
				Pid_Calculate(&p->Pid);
				p->Ratio =p->Pid.OUT;
				p->OnOff = 1;
			}
			
			break;
		}
		case 101:
		{//正常运行电机，需要先设定占空比
			p->OnOff = 1;
			break;
		}
		default:
		{
			p->RunMode = 0;
			p->OnOff = 0;
			p->Ratio = 0;
			break;
		}
	}
}
//---
/*
	函数名：u8 Slave_StartMotor(char motor,u16 ratio)
	描述：	以目标占空比ratio启动A泵电机/B泵电机/TSP路电机
	输入：	motor：启动的A电机或B电机或TSP路电机，'A'=A泵电机，'B'=B泵电机，'T'=TSP路电机
					ratio：占空比，0~65535
	返回：	0=启动成功
					255=motor输入参数错误，输入非'A'、'B'、'T'之一
*/
u8 Slave_StartMotor(Slave_MotorStructure *P,u16 ratio)
{
		P->RunMode = 101;
		P->Ratio = ratio;
		P->OnOff = 1;
	    return 0;
}
//---
/*
	函数名：u8 Slave_StopMotor(char motor)
	描述：	停止A泵电机或B泵电机或TSP泵电机
	输入：	motor：停止的A电机或B电机或TSP路电机，'A'=A泵电机，'B'=B泵电机，'T'=TSP路电机
	返回：	0=停止成功
					255=motor输入参数错误，输入非'A'、'B'、'T'之一
*/
u8 Slave_StopMotor(Slave_MotorStructure *P)
{
		P->RunMode = 0;
		P->Ratio = 0;
		P->OnOff = 0;
		return 0;
}
//---
/*
	函数名：u8 Slave_StartPump(char pump,float flux)
	描述：	以目标流量flux启动A路/B路/TSP路泵
	输入：	pump：启动泵对象，'A'=A路泵，'B'=B路泵，'T'=TSP路泵
					flux：设定的目标流量，单位min/L
	返回：	0=启动成功
					255=pumb参数错误（pumb不是'A'、'B'、'T'中的任意一个）
					B0位=1：管压故障(A/B)或静压故障(TSP)
					B1位=1：差压故障(流压故障)	
					B2位=1：管温故障(A/B)或环温故障(TSP)
					B3位=1：大气压故障(TSP)
*/
u8 Slave_StartPump(Slave_MotorStructure *P,float flux)
{
	u8 bf = 0;
	Slave_MotorStructure *p;
	p=P;
	p->Pid.Sv = flux;//设定值
	p->Pid.Ek = 0;
	p->Pid.Ek_1 = 0;
	p->Pid.Ek_2 = 0;
	p->Pid.SEk = 0;
	p->Pid.OUT = 0;
	p->Pid.Timer = 1000;
	p->RunMode = 1;
	return(bf);
}
//---
//---
/*
	函数名：u8 Slave_ContinuePump(char pump,float flux)
	描述：	以目标流量flux启动A路/B路/TSP路泵
	输入：	pump：启动泵对象，'A'=A路泵，'B'=B路泵，'T'=TSP路泵
					flux：设定的目标流量，单位min/L
	返回：	0=启动成功
					255=pumb参数错误（pumb不是'A'、'B'、'T'中的任意一个）
					B0位=1：管压故障(A/B)或静压故障(TSP)
					B1位=1：差压故障(流压故障)	
					B2位=1：管温故障(A/B)或环温故障(TSP)
					B3位=1：大气压故障(TSP)
*/
u8 Slave_ContinuePump(Slave_MotorStructure *P,float flux)
{
	u8 bf = 0;
	Slave_MotorStructure *p;
	p=P;
	p->Pid.Sv = flux;//设定值
//	p->Pid.Ek = 0;
//	p->Pid.Ek_1 = 0;
//	p->Pid.SEk = 0;
//	p->Pid.Timer = 1000;
//	p->RunMode = 1;
	return(bf);
}
/*
	函数名：u8 Slave_StopPump(char pump)
	描述：	停止A泵/B泵/TSP泵
	输入：	pump：停止泵对象，'A'=A路泵，'B'=B路泵，'T'=TSP路泵
	返回：	0=关闭成功
					255=输入参数错误，输入参数不是'A'，'B'，'T'之一
*/
u8 Slave_StopPump(Slave_MotorStructure *pump)
{
	return(Slave_StopMotor(pump));
}
//---
/*
	函数名：void Slave_MotorInit(Slave_MotorStructure *p)
	描述：	该函数用于初始化从机电机相关参数
*/
void Slave_MotorInit(Slave_MotorStructure *p)
{
	p->OnOff = 0;
	p->Ratio = 0;
	p->RunMode = 0;
}
//---
/*
	函数名：u8 Slave_MotorPidDataInit(char pump)
	描述：	该函数为初始化泵的pid数据
	输入：	pump：需要校准的泵，'A'=A泵，'B'=B泵，'T'=TSP泵
	返回：	0=启动成功
					255=pumb参数错误（pumb不是'A'、'B'、'T'中的任意一个）
*/
u8 Slave_MotorPidDataInit(Slave_MotorStructure *P)
{
PID_DataStructure *p;
	if(P->rame == 'W')
	{
		return 0;
	}
	if(P->rame == 'S')
	{
		return 0;
	}
	if(P->rame == 'G')  //气泵PID数据初始化
	{
		p = &P->Pid;
		//PID初始化
		p->Kp = 2000;	//比例系数100
		p->T = 100;	 //计算周期，100ms
		p->Ti = 1000;//积分周期
		p->Td = 2000;//微分时间
		p->PWMcycle = 65000;//最大值
		p->OUT0 = 2000;
		p->SEk = 0;
		p->Ek = 0;
		p->Ek_1 = 0;
		return 0;
	}
	if(P->rame == 'D')//尘泵PID数据初始化
	{
		p = &P->Pid;
		//PID初始化
		p->Kp =1.2;	//比例系数 25  1.8
		p->T = 100;		//计算周期，100ms
		p->Ti =1000;//积分周期1800  ,越大，OUT 就越小
		p->Td =000;//微分时间1000
		p->PWMcycle = 6500;//最大值,可做限流用6500
		p->OUT0 =120;  //1000              ,150大概是20L,
		p->SEk = 0;    
		p->Ek = 0;
		p->Ek_1 = 0;
		return 0;
	}
	else return 255;
}
//---
/*
	函数名：Slave_MotorPidPara(Slave_MotorStructure *P)
	描述：根据尘泵的流量范围设定PID参数
	输入：	pump：需要校准的泵，'A'=A泵，'B'=B泵，'T'=TSP泵
	返回：
				
*/
void Slave_MotorPidPara(Slave_MotorStructure *P)
{ 
	PID_DataStructure *p;
	if(P->rame == 'D')//尘泵PID数据初始化
	{   p = &P->Pid;
		if(p->Sv<=20)
		{
		p->Kp =1.6;	//比例系数 25
		p->T = 200;		//计算周期，100ms
		p->Ti =0.8;//积分周期1800  ,越大，OUT 就越小
		p->Td = 0;		//微分时间1000
		p->OUT0 =100;  //1000              ,150大概是20L,	
		}
		else if(p->Sv<=30)
		{
		p->Kp =1.6;	//比例系数 25
		p->T = 200;		//计算周期，100ms
		p->Ti =1.0;//积分周期1800  ,越大，OUT 就越小 
		p->Td =0;		//微分时间1000
		p->OUT0 =110;  //1000       ,150大概是20L,	
		}
		else if(p->Sv<=40)
		{
		p->Kp =1.6;	//比例系数 25
		p->T = 200;		//计算周期，100ms
		p->Ti =1.8;//积分周期1800  ,越大，OUT 就越小 
		p->Td =0;		//微分时间1000
		p->OUT0 =110;  //1000       ,150大概是20L,	
		}
		else if(p->Sv<=60)
		{
		p->Kp =1.6;	//比例系数 
		p->T = 200;		//计算周期，100ms
		p->Ti =2.5;//积分周期1800  ,越大，OUT 就越小
		p->Td =0;		//微分时间1000
		p->OUT0 =110;  //1000              ,150大概是20L,	
		}
		else if(p->Sv<=80)
		{
		p->Kp =1.6;	//比例系数 
		p->T = 200;		//计算周期，100ms
		p->Ti =2.8;//积分周期1800  ,越大，OUT 就越小
		p->Td =0;		//微分时间1000
		p->OUT0 =110;  //1000              ,150大概是20L,	
		}
		else
		{
		p->Kp =1.8;	//比例系数 25      1.8
		p->T = 200;	//计算周期，   200
		p->Ti =3.0;//  ,3.0
		p->Td =0;		//微分时间1000
		p->OUT0 =110;  //1000              ,150大概是20L,	
		}
	}
}
//---
//-------------------------通讯相关函数----------------------------
//---
/*
	函数名：void Slave_Communicate(void)
	描述：	该函数为与从板通讯用的主函数，用于主板从板之间的数据交换
*/
void Slave_Communicate(void)
{
	switch(SlaveData.Comm.RunMode)
	{
		case 1:  //第一步下传数据
		{
			Slave_CommSendData(0xAA);    //发送数据给副板
			SlaveData.Comm.Error = 0xFF;
			SlaveData.Comm.Timer = 100;  //
			Slave_UsartBuffer->BufFlg1=0;
			SlaveData.Comm.RunMode = 2;
			break;
		}
		case 2:
		{
			if(Slave_UsartBuffer->BufFlg1==1) // 收到数据
			{
				Slave_UsartBuffer->BufFlg1=0;
				if(Slave_CommReceiveData(&Slave_UsartBuffer->ProgressBuffer1[0])==0)  //处理接收到的数据
    		 {  //接收错误
				  if(SlaveData.Comm.ErrorCnt < 20) 
				 {
					SlaveData.Comm.ErrorCnt++;//通讯错误统计
				  }
				else
				{//故障
					u8 i;
					for(i=0;i<SLAVE_DataMax-1;i++)
					{
						SlaveData.Data[i].Raw = SLAVE_ColFAULT;
					}
				 }
				
			  }
				else   //成功接收
				{
				SlaveData.Comm.ErrorCnt = 0;
				}
				SlaveData.Comm.Timer = 200;
				SlaveData.Comm.NewMode = 1;
				SlaveData.Comm.RunMode = 251;
				
			}
			else if(SlaveData.Comm.Timer == 0) //接收超时
			{//规定时间内没有接收到数据，认为故障
				if(SlaveData.Comm.ErrorCnt < 20) 
				{
					SlaveData.Comm.ErrorCnt++;//通讯错误统计
				}
				else
				{//故障
					u8 i;
					for(i=0;i<SLAVE_DataMax-1;i++)
					{
						SlaveData.Data[i].Raw = SLAVE_ColFAULT;
					}
				}
				SlaveData.Comm.Timer = 200;
				SlaveData.Comm.NewMode = 1;
				SlaveData.Comm.RunMode = 251;
			}
			break;
		}
		case 251:
		{
			if(SlaveData.Comm.Timer == 0) SlaveData.Comm.RunMode = SlaveData.Comm.NewMode;
			break;
		}
	}
	
}
//---
/*
	函数名：void Slave_CommSendData(void)
	描述：	该函数为读取从机的基本参数
	输入：	address：从机地址(0xAA)
0xAA,长度,0x01,水泵开关,电磁阀开关，气泵开关，气泵占空比，尘泵开关，尘泵占空比，CRC16
*/
void Slave_CommSendData(u8 address)
{
	u8 bf[20];
	u8 cnt = 0,i;
	TwoBytes two;
	bf[cnt++] = 0x01;   //1
	//抽水泵WATER开关信息
	bf[cnt++] = SlaveData.WaterMotor.OnOff;  //2
	//电磁阀SLVD开关信息
	bf[cnt++] = SlaveData.SlvdMotor.OnOff;   //3

	//气泵GAS
	bf[cnt++] = SlaveData.GasMotor.OnOff;   //4
	two.U16Data = SlaveData.GasMotor.Ratio;
	bf[cnt++] = two.U8Data[0];               //5
	bf[cnt++] = two.U8Data[1];               //6
	//尘泵DUST
	bf[cnt++] = SlaveData.DustMotor.OnOff;    //7
	two.U16Data = SlaveData.DustMotor.Ratio;
	bf[cnt++] = two.U8Data[0];        //8
	bf[cnt++] = two.U8Data[1];        //9
	bf[cnt++] = SlaveData.Stm8Data.rw_ctrl;	//10 2020-05-22 hjl add stm8读写控制状态
	if(SlaveData.Stm8Data.rw_ctrl)          //写
	SlaveData.Stm8Data.rw_ctrl=0;           //变回读 
	bf[cnt++] = SlaveData.E28.STATE;        //E28控制字   11
	if(SlaveData.E28.STATE==1)
		SlaveData.E28.STATE=0;  //写完恢复零
	bf[cnt++] = SlaveData.E28.CHset;        //E28设置信道  12
	two.U16Data =SlaveData.E28.ADRRset;     //E28设置地址
	bf[cnt++] = two.U8Data[1];        //13 先送低8位
	bf[cnt++] = two.U8Data[0];        //14 先送地8位
	
	two.U16Data = CRC16(bf,cnt);
	bf[cnt++] = two.U16Data>>8;    //15先发高字节
	bf[cnt++] = two.U16Data;       //16 再发低直接
	Slave_SendBufferData(Slave_UsartPort,Slave_UsartBuffer,address);
	Slave_SendBufferData(Slave_UsartPort,Slave_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		Slave_SendBufferData(Slave_UsartPort,Slave_UsartBuffer,bf[i]);
	}
}
/*
	函数名：u8 Slave_CommReceiveData(u8 *p,u8 len)
	描述：	该函数用于接收处理来自从机的数据
	输入：	p：数据的首地址，60个字节
	说明：          byte0：字节数 byte1=1(无用);byte2开始数据
                    1： DS18B20S温度，浮点型，4个字节
					2： MS5805-A温度，浮点型，4个字节
					3： MS5805-A压力，浮点型，4个字节
					4： SM9541-A压力，浮点型，4个字节
					5：	SM9541-B压力，浮点型，4个字节
					6：	SM9541-C压力，浮点型，4个字节
					7： SM3904-D压力，浮点型，4个字节	
                    8： SM3904-E压力，浮点型，4个字节	
*/
u8 Slave_CommReceiveData(u8 *p)
{
	u8 i;
	u8 cnt=2; //从第二字节开始接收
	FourBytes four;
   if(AnalysisCRC(p))  //分析数据是否完整CRC效验
		 {
			for(i=0;i<(SLAVE_DataMax);i++) //接收来自从板的18个浮点
		    {
			   four.U8Data[0] = p[cnt++];
			   four.U8Data[1] = p[cnt++];
			   four.U8Data[2] = p[cnt++];
			   four.U8Data[3] = p[cnt++];
			   if((i>=10)&&(i<=16))
			   Slave_DataDealWith(&SlaveData.Data[i],four.FloatData,1);  //计算滑动平均值并保存到Raw  这些是否点数据
			   else
			   Slave_DataDealWith(&SlaveData.Data[i],four.FloatData,0);  //计算滑动平均值并保存到Raw  这些是否点数据	
				//printf("\r\n %d :%f",i,four.FloatData);	
		    }
	    	//2020-05-25 hjl add拆状态数据接收
		    cnt++;
	    	//SlaveData.Stm8Data.rw_ctrl = p[cnt++];
	    	 SlaveData.Stm8Data.Data = p[cnt++];
			
			 SlaveData.E28.STATERD24G=p[cnt++];   //E28状态
			//E28动压
			 four.U8Data[0] = p[cnt++];
			 four.U8Data[1] = p[cnt++];
			 four.U8Data[2] = p[cnt++];
			 four.U8Data[3] = p[cnt++];
			Slave_DataDealWith(&SlaveData.Data[SLAVE_DataMax],four.FloatData,0);  //计算滑动平均值并保存到Raw  这些是否点数据
			//E28静压
			 four.U8Data[0] = p[cnt++];
			 four.U8Data[1] = p[cnt++];
			 four.U8Data[2] = p[cnt++];
			 four.U8Data[3] = p[cnt++];
			 Slave_DataDealWith(&SlaveData.Data[SLAVE_DataMax+1],four.FloatData,0);  //计算滑动平均值并保存到Raw  这些是否点数据
			//E28烟温
			 four.U8Data[0] = p[cnt++];
			 four.U8Data[1] = p[cnt++];
			 four.U8Data[2] = p[cnt++];
			 four.U8Data[3] = p[cnt++];
			 Slave_DataDealWith(&SlaveData.Data[SLAVE_DataMax+2],four.FloatData,0);  //计算滑动平均值并保存到Raw  这些是否点数据
			//E28地址

			 SlaveData.E28.ADRRRD24G=p[cnt++]; //高8位
			 SlaveData.E28.ADRRRD24G=SlaveData.E28.ADRRRD24G<<8;
			 SlaveData.E28.ADRRRD24G=SlaveData.E28.ADRRRD24G+p[cnt++];
            //E28信道 
	
			SlaveData.E28.CHRD24G=p[cnt++];
		    return 1;
		  }
			 else
			return 0;
}
/*
	函数名：u8 Slave_OpenWaterPump(void)
	描述：	打开抽水泵
	输入：	无
	说明：	
*/
void Slave_OpenWaterPump(void)       //打开水泵20200528
{
	SlaveData.WaterMotor.OnOff=1;  //调试打开水泵
}
/*
	函数名：Slave_CloseWaterPump(void)
	描述：	关闭抽水泵
	输入：	无
	说明：	
*/
void Slave_CloseWaterPump(void)      //关闭水泵20200528
{
	SlaveData.WaterMotor.OnOff=0;  //调试关闭水泵
}
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Slave_DebugASCII(u8 *p,u8 len)
	描述：		该函数为slave.c文件的ASCII调试文件
						:000|08|001
*/
void Slave_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:
		{//读取所有传感器的实时采样值
			printf("D18B20计温：%f\r\n",SlaveData.JWTem.Value);//环温
			printf("5805温度：%f\r\n",SlaveData.HWTem.Value);
			
			printf("大气压：%f\r\n",SlaveData.DQPress.Value);
			printf("A路动压：%f\r\n",SlaveData.ACPress.Value);
			printf("A路静压：%f\r\n",SlaveData.AGPress.Value);
		    printf("尘动压：%f\r\n",SlaveData.BCPress.Value);
			printf("尘静压：%f\r\n",SlaveData.BGPress.Value);
			printf("气动压：%f\r\n",SlaveData.CCPress.Value);
	
			printf("烟温：%f\r\n",SlaveData.YWTem.Value);  // 烟温温度
			printf("干球温度：%f\r\n",SlaveData.GQTem.Value);  // 干球温度
			printf("湿球温度：%f\r\n",SlaveData.SQTem.Value);  // 干球温度
			
			printf("O2 AD：%f\r\n",SlaveData.O2Concent.Value);
			printf("SO2 AD：%f\r\n",SlaveData.SO2Concent.Value);
			printf("NO AD：%f\r\n",SlaveData.NOConcent.Value);
			printf("NO2 AD：%f\r\n",SlaveData.NO2Concent.Value);
		    printf("CO AD：%f\r\n",SlaveData.COConcent.Value);
			printf("H2S AD：%f\r\n",SlaveData.H2SConcent.Value);
			printf("CO2 AD：%f\r\n",SlaveData.CO2Concent.Value);
			
		//	printf("A路计温：%f\r\n",SlaveData.AGTem.Value);
			
			break;
		}
		case 11:
		{ SlaveData.GasMotor.OnOff=1;
			SlaveData.GasMotor.Ratio+=10;
			if(SlaveData.GasMotor.Ratio>65535)
			SlaveData.GasMotor.Ratio=65535;
			printf("\n气泵PE5占空比+ %d ",SlaveData.GasMotor.Ratio);
			break;
		}
		case 12:
		{ SlaveData.GasMotor.OnOff=1;
			if(SlaveData.GasMotor.Ratio>10)SlaveData.GasMotor.Ratio-=10;
			if(SlaveData.GasMotor.Ratio<=10)SlaveData.GasMotor.Ratio=0;
			printf("\n气泵PE5占空比- %d ",SlaveData.GasMotor.Ratio);
			break;
		}
		case 13:
		{ SlaveData.DustMotor.OnOff=1;
			SlaveData.DustMotor.Ratio+=10;
			if(SlaveData.DustMotor.Ratio>65535)
			SlaveData.DustMotor.Ratio=65535;
			printf("\n尘泵PA5占空比+%d ",SlaveData.DustMotor.Ratio);
			break;
		}
		case 14:
		{ SlaveData.DustMotor.OnOff=1;
			if(SlaveData.DustMotor.Ratio>10)SlaveData.DustMotor.Ratio-=10;
			if(SlaveData.DustMotor.Ratio<=10)SlaveData.DustMotor.Ratio=0;
	  	printf("\n尘泵PA5占空比+%d ",SlaveData.DustMotor.Ratio);
			break;
		}
		case 21:
		{
			SlaveData.WaterMotor.OnOff=1;  //调试打开水泵
			printf("\n打开水泵");
			break;
		}
		case 22:
		{ SlaveData.WaterMotor.OnOff=0;  //调试关闭水泵
			printf("\n关闭水泵");
			break;
		}
		case 23:
		{
				SlaveData.SlvdMotor.OnOff=1;  //调试打开电磁阀
			printf("\n打开电磁阀");
			break;
		}
		case 24:
		{
			SlaveData.SlvdMotor.OnOff=0;  //调试关电磁阀
			printf("\n关闭电磁阀");
			break;
		}
		case 2:
		{ //Slave_Zero_QT();  //所有压力值调零  对外接口
     // Slave_Zero_Tem();  //所有温度调零 对外接口
      Slave_Zero_Press();  //所有压力值调零 对外接口
			break;
		}
		default:
		{
		  break;
		}
	}
}
//---
/*
	函数名： 	void Slave_DebugRTU((u8 *p)
	描述：		该函数为slave.c文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*//*
void Slave_DebugRTU(u8 *p,u8 len)
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
//---
