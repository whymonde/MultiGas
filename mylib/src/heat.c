/*
	本文件为该项目的屏幕相关函数
	具体函数如下
*/

#include "heat.h"

Heat_DataStructure HeatData;
/*
	函数名：void Heat_Main(void)
	描述：	heat.c主函数
*/
void Heat_Main(void)
{
//	switch(HeatData.RunMode)
//	{
//		case 1:
//		{//正常模式
//			if(Flashisp_GetData()->Data[FLASHIS_AdrXTSZJRSZKG])
//			{//打开加热
//				Heat_DataInit();
//				HeatData.RunMode = 2;
//			}
//			break;
//		}
//		case 2:
//		{
//			if(Flashisp_GetData()->Data[FLASHIS_AdrXTSZJRSZKG])
//			{//打开加热
//				if(Slave_GetData()->BWXTem.Value != FLUX_ColFAULT)
//				{//环境温度传感器无故障
//					HeatData.PidData.Pv = Slave_GetData()->BWXTem.Value;
//					HeatData.PidData.Sv = Flashisp_GetData()->Data[FLASHIS_AdrXTSZJRSZWD];//用户设定温度
//					Pid_Calculate(&HeatData.PidData);
//					Heat_OnOff(1,(u16)HeatData.PidData.OUT);
//				}
//				else Heat_OnOff(0,0);//关闭加热
//			}
//			else Heat_OnOff(0,0);//关闭加热
//			break;
//		}
//		default:
//		{
//			HeatData.RunMode = 1;
//			break;
//		}
//	}
}
//---
/*
	函数名：void Heat_Timer(void)
	描述：Heat.c文件定时器函数，由系统1ms定时器调用
*/
void Heat_Timer(void)
{
	HeatData.PidData.Timer++;
	
//	HEAT_OUT(2)
}
//---
/*
	函数名：void Heat_Init(void)
	描述：	Heat.c文件初始化函数
*/
void Heat_Init(void)
{
	Heat_GpioInit();
	Heat_TimerInit();
	Heat_DataInit();
//	Heat_OnOff(0,0);
}
//---
/*
	函数名：void Heat_DataInit(void)
	描述：	该函数为Heat.c数据初始化
*/
void Heat_DataInit(void)
{
	HeatData.RunMode = 1;
	//PID初始化
	HeatData.PidData.Sv = 50;//用户设定温度
	HeatData.PidData.Kp = 3000;//比例系数
	HeatData.PidData.T = 800;//计算周期，1000ms
	HeatData.PidData.Ti = 16000;//积分周期
	HeatData.PidData.Td = 8000;//微分时间
	HeatData.PidData.PWMcycle = 65534;
	HeatData.PidData.OUT0 = 1000;
	HeatData.PidData.SEk = 0;
	HeatData.PidData.Ek = 0;
	HeatData.PidData.Ek_1 = 0;
//	HeatData.PidData.Avg = 0;
	
	
	HeatData.Mode = 1;
	
}
//---
/*
	函数名：void Heat_GpioInit(void)
	描述：	该函数为Heat.c通用IO口初始化函数
*/
void Heat_GpioInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(HEAT_APP, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_Pin = HEAT_PIN;	  
  GPIO_Init(HEAT_PORT, &GPIO_InitStructure);
	HEAT_OUT(0);
}
//---
/*
	函数名： void Project_TimerInit(void)
	描述：	 	此函数为整个项目的定时器，周期为1ms
	说明：	 	该函数使用到定时器4
*/
void Heat_TimerInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStruct;	
	//配置定时器基本配置
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 71;    		//预分频,此值+1为分频的除数,72/18=4MHz.1us
	TIM_TimeBaseStructure.TIM_Period = 65535;				//计数值   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	//采样分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//TIM_CH2配置
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//指定TIM模式
	TIM_OCInitStruct.TIM_OutputState = 	TIM_OutputState_Disable;//TIM_OutputState_Enable;//指定TIM输出比较状态
	TIM_OCInitStruct.TIM_OutputNState =	TIM_OutputNState_Disable;//指定TIM互补输出比较状态
	TIM_OCInitStruct.TIM_Pulse =	20000;//指定要加载到捕获比较寄存器中的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;//指定输出极性
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;//指定互补输出极性
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OutputState_Enable;//指定空闲状态期间的TIM输出比较引脚状态
	TIM_OCInitStruct.TIM_OCNIdleState =	TIM_OutputNState_Disable;//指定空闲状态期间的TIM输出比较引脚状态
	TIM_OC2Init(TIM2,&TIM_OCInitStruct);
//	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);//使能TIMx-OCxPE位
	NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE); 
	TIM_ITConfig(TIM2,TIM_IT_CC2, ENABLE);   
	TIM_Cmd(TIM2,ENABLE);
}
//---
/*
	通用定时器2中断，整个系统定时器，1ms
*/
void TIM2_IRQHandler(void)
{
	if(TIM2->SR & B0)	
	{
		if(HeatData.Mode == 1) 
			if(HeatData.PidData.OUT  != 0){HEAT_OUT(1)}
		TIM2->SR &= ~B0;	//清除中断标志位
	}
	if(TIM2->SR & B2)	
	{//捕获比较2中断
		HEAT_OUT(0)
		TIM2->SR &= ~B2;	//清除中断标志位
	}
}
//---
/*
	函数名：Heat_DataStructure* Heat_GetData(void)
	描述：获取返回Heat.c文件全局变量结构体变量
*/
Heat_DataStructure* Heat_GetData(void)
{
	return &HeatData;
}
//---
/*
	函数名：void Heat_OnOff(u8 sw,u16 pwm)
	描述：	该函数为启停加热器
*/
void Heat_OnOff(u8 sw,u16 pwm)
{
	if(sw)
	{
		HeatData.Mode = 1;
		TIM_SetCompare2(TIM2,pwm);
	}
	else 
	{
		HeatData.Mode = 0;
		HEAT_OUT(0);
	}
}
//---
//-------------------------数据读取、存储----------------------------
//---
/*
	函数名：void Heat_ReadData(u8 address,u8 *p,u8 len)
	描述：	读取数据
*/
void Heat_ReadData(u8 address,u8 *p,u8 len)
{
//	W25Xxx_ReadData(p,AD7689_AdrStoreBase+address,len);
}
//---
/*
	函数名：void Heat_StoreData(u8 address,u8 *p,u8 len)
	描述：该函数为存储Heat.c配置参数函数
	说明：读取数据
				擦除数据
				修改数据
				保存数据
*/
void Heat_StoreData(u8 address,u8 *p,u8 len)
{
//	u8 buffer[AD7689_AdrStoreEND],cnt;
//	W25Xxx_ReadData(buffer,AD7689_AdrStoreBase,AD7689_AdrStoreEND);//读所有配置参数
//	W25Xxx_EraseSector(AD7689_AdrStoreBase);//擦除数据
//	for(cnt=0;cnt<len;cnt++)
//	{
//		buffer[address++] = *p;
//		p++;
//	}
//	W25Xxx_PageWrite(buffer,AD7689_AdrStoreBase,AD7689_AdrStoreEND);//保存数据
}
//---
/*
	函数名：void Heat_FactoryReset(void)
	描述：该函数Heat.c存储数据恢复出厂设置
*/
void Heat_FactoryReset(void)
{
	//AD4通道
//	i = 4;
//	AD7689_StoreData(AD7689_AdrStoreAD4Map,&i,1);//设备映射
//	two.U16Data = 100;
//	AD7689_StoreData(AD7689_AdrStoreAD4Orgin,two.U8Data,2);//原点值
//	two.U16Data = 60000;
//	AD7689_StoreData(AD7689_AdrStoreAD4Endpoint,two.U8Data,2);//端点值
//	four.FloatData = 50000.0;
//	AD7689_StoreData(AD7689_AdrStoreAD4Refer,four.U8Data,4);//参照值
	//初始化标志位
//	two.U8Data[0] = AD7689_FACTORY;
//	AD7689_StoreData(AD7689_AdrStoreFactory,two.U8Data,1);//恢复出厂设置标志
	printf("Heat参数初始化\r\n");
}
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Heat_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Heat.c文件的ASCII调试文件
命令：		:000|05|xxx
*/
void Heat_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:
		{
			u16 bf = Calculate_CharToU16(p+6,5);//计算命令
			if(*(p+4)=='0') 
			{
				Heat_OnOff(0,0);
				printf("停止加热\r\n");
			}
			else 
			{
				Heat_OnOff(1,bf);
				printf("启动加热:%u\r\n",bf);
			}
			break;
		}
		case 2:
		{
			break;
		}
	}
}
//---

//---





































