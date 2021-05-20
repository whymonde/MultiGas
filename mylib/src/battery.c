/*
	本文件为该项目的电池相关函数
	具体函数如下
	
	说明：
	输出：
	采集电压：Battery_GetData()->Battery.ActualV 
	阈值判断：Battery_GetData()->Mode （=1方可执行采集任务）
*/

#include "battery.h"

Battery_DataStructure BatteryData;
/*
	函数名：void Battery_Main(void)
	描述：	Battery.c文件主函数
*/
void Battery_Main(void)
{
	Battery_GetVoltage();
	Battery_DealWith();
}
//---
/*
	函数名：void Battery_Init(void)
	描述：	Battery.c文件初始化函数
*/
void Battery_Init(void)
{
	Battery_InitADC();
	Battery_DataInit();
}
//---
/*
	函数名：void Battery_Timer(void)
	描述：Battery.c文件定时器函数，由系统1ms定时器调用
*/
void Battery_Timer(void)
{
	
}
//---
/*
	函数名：Battery_DataStructure* Battery_GetData(void)
	描述：获取返回Battery.c文件全局变量结构体
*/
Battery_DataStructure* Battery_GetData(void)
{
	return &BatteryData;
}
//---
/*
	函数名：void Battery_DataInit(void)
	描述：	Battery.c文件数据初始化函数
*/
void Battery_DataInit(void)
{
	u8 i;
	BatteryData.Mode = 0;
	BatteryData.LostPowerFlag=0;
	
	BatteryData.Battery .Rate = 0;
	BatteryData.Battery.ActualV = 0;
	BatteryData.Battery.CollectV = 0;
	BatteryData.Battery.Rate = 0;
	BatteryData.Battery.Cnt = 0;
	{
		
		for(i=0;i<BATTERY_DATALEN;i++)
		BatteryData.Battery.Data[i] = 0;
	}
	Battery_ReadFlashData();
	
	BatteryData.LostPV .Rate = 0;
	BatteryData.LostPV.ActualV = 0;
	BatteryData.LostPV.CollectV = 0;
	BatteryData.LostPV.Rate = 1;
	BatteryData.LostPV.Cnt = 0;
	{
		
		for(i=0;i<BATTERY_DATALEN;i++)
		BatteryData.LostPV.Data[i] = 0;
	}
	
	BatteryData.Current.Rate = 0;
	BatteryData.Current.ActualV = 0;
	BatteryData.Current.CollectV = 0;
	BatteryData.Current.Rate = 1;
	BatteryData.Current.Cnt = 0;
	{

		for(i=0;i<BATTERY_DATALEN;i++)
		BatteryData.Current.Data[i] = 0;
	}

	
}
//---
/*
	函数名：void Battery_ReadFlashData(void)
	描述：	该函数为读取flash存储器内的数据
*/
void Battery_ReadFlashData(void)
{
	FourBytes four;
	four.U16Data[0] = Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_DCJZ_BL];    //20200528
	four.U16Data[1] = Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_DCJZ_BL+1];  //20200528
	BatteryData.Battery.Rate = four.FloatData;
}
//---
/*
	函数名：void Tem_InitADC(void)
	描述：	初始化ADC模块
*/
/* STM32芯片ADC转换结果DR寄存器基地址 */
void Battery_InitADC(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //使能ADC1时钟
	
	//先初始化ADC1通道10 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//pc0 通道10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化  

  //先初始化ADC1通道11 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//Pc1 通道11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化  
	
 
    //先初始化ADC1通道13 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//Pc3 通道13
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化  	
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;//左对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
	
	ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	BatteryData.CurChanl=1;
}
//---
/*
	函数名：void Battery_GetVoltage(void)
	描述：	该函数为获取电源电压
*/
void Battery_GetVoltage(void)
{   	
	u8 i;
	u32 sum;// = 0;
	
	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
	{
		if(BatteryData.CurChanl==1)  //读通道PC1  ch11
		{
		  BatteryData.Battery.Data[BatteryData.Battery.Cnt++] = ADC_GetConversionValue(ADC1)>>4;
		 if(BatteryData.Battery.Cnt >= BATTERY_DATALEN) BatteryData.Battery.Cnt = 0;                //满
		 {//数据处理
		  sum= 0;
			for(i=0;i<BATTERY_DATALEN;i++)
			{
				sum += BatteryData.Battery.Data[i];
			}
			BatteryData.Battery.CollectV = ((float)sum / BATTERY_DATALEN);
			BatteryData.Battery.CollectV *= (BATTERY_Vref/4096);

			BatteryData.Battery.ActualV = BatteryData.Battery.CollectV * BatteryData.Battery.Rate * BATTERY_OP;
		 }
     //启动另一通道PC0
		 ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
		 ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
		 BatteryData.CurChanl=2;
	  }
		else if(BatteryData.CurChanl==2) //读通道PC0  ch10
		{
		 BatteryData.LostPV.Data[BatteryData.LostPV.Cnt++] = ADC_GetConversionValue(ADC1)>>4;
		 if(BatteryData.LostPV.Cnt >= BATTERY_DATALEN) BatteryData.LostPV.Cnt = 0;                //满
		 {//数据处理
			  sum= 0;
			for(i=0;i<BATTERY_DATALEN;i++)
			{
				sum += BatteryData.LostPV.Data[i];
			}
			BatteryData.LostPV.CollectV = ((float)sum / BATTERY_DATALEN);
			BatteryData.LostPV.CollectV *= (BATTERY_Vref/4096);

			BatteryData.LostPV.ActualV = BatteryData.LostPV.CollectV * 1 * BATTERY_OP;
			
		 }
     //启动另一通道PC1
		
		 
		 ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
		 ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
		 BatteryData.CurChanl=3;
		}
		
		else if(BatteryData.CurChanl==3)  //读通道PC0  ch12
		{
		 BatteryData.Current.Data[BatteryData.Current.Cnt++] = ADC_GetConversionValue(ADC1)>>4;
		 if(BatteryData.Current.Cnt >= BATTERY_DATALEN) BatteryData.Current.Cnt = 0;                //满
		 {//数据处理
			  sum= 0;
			for(i=0;i<BATTERY_DATALEN;i++)
			{
				sum += BatteryData.Current.Data[i];
			}
			BatteryData.Current.CollectV = ((float)sum / BATTERY_DATALEN);
			BatteryData.Current.CollectV *= (BATTERY_Vref/4096);
            //BatteryData.Current.ActualV =(3.0f*2.487f-BatteryData.Current.CollectV-5.0f)/0.37f;
			//BatteryData.Current.ActualV =(2.5f-BatteryData.Current.CollectV)/0.25f* 1 ;
			BatteryData.Current.ActualV=12.5f-5.26f*BatteryData.Current.CollectV;
			
		 }
     //启动另一通道PC1
		 ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
		 ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
		 BatteryData.CurChanl=1;
		}
	
	}

}
//---
/*
	函数名：void Battery_DealWith(void)
	描述：	该函数为对采集到的电压进行阈值判断
*/
void Battery_DealWith(void)
{
	
		//阈值判断
	if(BatteryData.LostPV.ActualV > BATTERY_VPT_ON)
	{
		BatteryData.LostPowerFlag = 1;
	}
	else if(BatteryData.LostPV.ActualV < BATTERY_VPT_OFF)
	{
		BatteryData.LostPowerFlag = 0;
	}

	//阈值判断
	if(BatteryData.Battery.ActualV > BATTERY_VPT_ON)
	{
		BatteryData.Mode = 1;
	}
	else if(BatteryData.Battery.ActualV < BATTERY_VPT_OFF)
	{
		BatteryData.Mode = 0;
	}
	//电池容量计算
	if(BatteryData.Battery.ActualV <= BATTERY_VPT_ON)
	{
		BatteryData.Battery.Ratio = 0;
	}
	else if(BatteryData.Battery.ActualV >= BATTERY_Full100)
	{
		BatteryData.Battery.Ratio = 100;
	}
	else 
	{
		BatteryData.Battery.Ratio = (u8)((((BatteryData.Battery.ActualV - BATTERY_VPT_ON)/(BATTERY_Full100 - BATTERY_VPT_ON)) * 100));
//		printf("Battery.Ratio = %u\r\n",BatteryData.Battery.Ratio);
	}
}
//---
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Battery_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Battery.c文件的ASCII调试文件
*/
void Battery_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 101:
		{//返回当前电压	:000|07|101
			printf("采集电压：%f\r\n",BatteryData.Battery.CollectV);
			printf("计算电压：%f\r\n",BatteryData.Battery.ActualV);
			printf("电压倍率：%f\r\n",BatteryData.Battery.Rate);
			break; 
		}
		case 201:
		{//设置电压倍率
			FourBytes four;
			four.U16Data[0] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHDYBL];
			four.U16Data[1] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHDYBL+1];
			printf("当前设置电压倍率：%f\r\n",four.FloatData);
			break;
		}
		default:
		{
			printf("battery.c接收到无效调试指令%u!\r\n",cmd);
			break;
		}
	}
}
//---
/*
	函数名： 	void Battery_DebugRTU((u8 *p)
	描述：		该函数为Battery.c文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*//*
void Battery_DebugRTU(u8 *p,u8 len)
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





































