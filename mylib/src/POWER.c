/*
	本文件为该项目的屏幕相关函数
	具体函数如下
*/

#include "Power.h"
#include "stm32f4xx_exti.h"
Power_DataStructure PowerData;
/*
	函数名：void Power_Main(void)
	描述：	heat.c主函数
*/
void Power_Main(void)
{
	
}
//---
/*
	函数名：void Power_Timer(void)
	描述：Power.c文件定时器函数，由系统1ms定时器调用
*/
void Power_Timer(void)
{ u32 i;
	//以下为10MS检测一次power 状态模型
	if(PowerData.Flg&0x01)    //进入正常界面就可检测按键
	{
	if(PowerData.Timer10ms>0)
	{
		PowerData.Timer10ms--;
	}
	else  //
	{
		  //10MS
		{  //处理电源撤换
			PowerData.V220Cnt++;
		  if(PowerData.V220Cnt>=5) //检测到没外接220V   ,50MS
		  { //撤换电源供电
			  PowerData.V220Cnt=5;
			 
			 if((PowerData.Flg&0x04)==0) //撤换电源供电 PowerData.Flg=PowerData.Flg|0x02;  //设置外部电源标记
			  {
		        CTRbat_OUT(0);   //打开电池电源	
				for(i=0;i<=9965535;i++)
			   {
			   PowerData.Flg=PowerData.Flg|0x04;  //设置电池标记
			   PowerData.Flg=PowerData.Flg&0xfd;  //设置电池标记
			   }
			   CTRexPower_OUT(1);//关闭外部电源
			  }
		  }
		}
		
		PowerData.Timer10ms=10;   //定义10MS
		
		//if(PowerData.Flg&0x02) //外电压打开？
		//{
		if((TexPower_LED)&&(Tbat_LED))  //检测电平
		PowerData.StaCnt++;
		
		//}
		//else                   //电池打开了
		//{
		//if(Tbat_LED)  //检测电平
		//PowerData.StaCnt++;
		//}
		
		PowerData.Cnt++;
		if(PowerData.Cnt>=10)  //100m秒时间到,1s判断一次
		{
			//检测有多小个高电平
			if(PowerData.StaCnt==10)  //按键没按下
			{
				PowerData.starFlg = 0;  //无电
			}
			else if(PowerData.StaCnt==0)  //按键按下
			{
				if(PowerData.starFlg==0)
				{
				PowerData.starFlg = 1;//有电
				PowerData.Tcnt=0;
				}
				else
				{
				PowerData.Tcnt++;   //100ms加1次
				}
			}
			PowerData.StaCnt=0;
			PowerData.Cnt=0;
		}
	}
 }
}





//---
/*
	函数名：EX0_Init(void)
	描述：	Power.c文件外中断初始化函数
*/
void EX0_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能时钟
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource11);//把PD11链接到外中断功能

    /* 配置EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line11;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升源触发;  EXTI_Trigger_Falling下降源
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	EXTI_Init(&EXTI_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断11
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置

}

//---
/*
	函数名：void Power_Init(void)
	描述：	Power.c文件初始化函数
*/
void Power_Init(void)
{ 
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(Tbat_APP, ENABLE);
  RCC_AHB1PeriphClockCmd(CTRbat_APP, ENABLE);
  RCC_AHB1PeriphClockCmd(TexPower_APP, ENABLE);
  RCC_AHB1PeriphClockCmd(CTRexPower_APP, ENABLE);
	//以下设置输出引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_InitStructure.GPIO_Pin = CTRbat_PIN;	 //***********
  GPIO_Init(CTRbat_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = CTRexPower_PIN;	 //***********
  GPIO_Init(CTRexPower_PORT, &GPIO_InitStructure);	
 
	//以下设置输入引脚

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //普通输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;         //上拉无法读取任意时间，下拉可以

    GPIO_InitStructure.GPIO_Pin = Tbat_PIN;              //
    GPIO_Init(Tbat_PORT, &GPIO_InitStructure);                 //初始化
	
	GPIO_InitStructure.GPIO_Pin = TexPower_PIN;              //
    GPIO_Init(TexPower_PORT, &GPIO_InitStructure);                 //初始化
	
	//初始化状态寄存器
	PowerData.Flg=0;
	delay_ms(10);   //延时10毫秒

	//检测外部电源是否有点
	//if(Tbat_LED)  //高电平没外接电源GPIO_ReadOutputDataBit (GPIOC,GPIO_Pin_13)==0
	if(GPIO_ReadOutputDataBit (CTRexPower_PORT,CTRexPower_PIN)==0)
	{
			
		 CTRexPower_OUT(0);//打开外部电源

		 PowerData.Flg=PowerData.Flg|0x02;  //设置外部电源标记
		 PowerData.Flg=PowerData.Flg&0xFB;  //清设置电池标记
		 PowerData.V220Cnt=0;
		 CTRbat_OUT(1);//关闭电池电源
	}
	else
	{  
		 CTRbat_OUT(0);   //打开电池电源
    
		 PowerData.Flg=PowerData.Flg|0x04;  //设置电池标记
		 PowerData.Flg=PowerData.Flg&0xFD;  //清外部电源标记
		 CTRexPower_OUT(1);//关闭外部电源

	}
  
	EX0_Init();    //初始化外中断
	
}
//---
/*
	函数名：Power_DataStructure* Power_GetData(void)
	描述：获取返回Power.c文件全局变量结构体变量
*/
Power_DataStructure* Power_GetData(void)
{
	return &PowerData;
}



//外中断服务程序
void EXTI15_10_IRQHandler(void)
{  u32 i;
	   PowerData.V220Cnt=0;  //有中断清空计数器
	

	   if(PowerData.Flg&0x01)    //进入正常界面就可检测按键
	   {
			 if(PowerData.Flg&0x04)  //如果是电池供电   
			 {
			 CTRexPower_OUT(0);//打开外部电源
			 for(i=0;i<=9965535;i++)
			 {
		       PowerData.Flg=PowerData.Flg|0x02;  //设置外部电源标记
		       PowerData.Flg=PowerData.Flg&0xFB;  //清设置电池标记
			 }
			 CTRbat_OUT(1);//关闭电池电源
			 EXTI_ClearITPendingBit(EXTI_Line10); //清除中断标志
			 }
	   }
	   EXTI_ClearITPendingBit(EXTI_Line10); //清除中断标志
	   EXTI_ClearITPendingBit(EXTI_Line11); //清除中断标志
	   EXTI_ClearITPendingBit(EXTI_Line12); //清除中断标志
	   EXTI_ClearITPendingBit(EXTI_Line13); //清除中断标志
	   EXTI_ClearITPendingBit(EXTI_Line14); //清除中断标志
	   EXTI_ClearITPendingBit(EXTI_Line15); //清除中断标志
	
}	

//-------------------------数据读取、存储----------------------------
/*
	函数名：Power_SetFlg(void)
	描述：如果是1才开始检测开关机按键
*/
void Power_SetFlg(void)//对外接口
 {
   #ifdef NEWBOARD
	 PowerData.Flg=PowerData.Flg|0x1;  //正式时要重新
  #endif
	 
	 PowerData.Timer10ms=10;
	 PowerData.StaCnt=0;
	 PowerData.Cnt=0;
 }

//---
/*
	函数名：void Power_ReadData(u8 address,u8 *p,u8 len)
	描述：	读取数据
*/
void Power_ReadData(u8 address,u8 *p,u8 len)
{

}
//---
/*
	函数名：void Power_StoreData(u8 address,u8 *p,u8 len)
	描述：该函数为存储Power.c配置参数函数
	说明：读取数据
				擦除数据
				修改数据
				保存数据
*/
void Power_StoreData(u8 address,u8 *p,u8 len)
{

}
//---
/*
	函数名：void Power_FactoryReset(void)
	描述：该函数Power.c存储数据恢复出厂设置
*/
void Power_FactoryReset(void)
{

	printf("Power参数初始化\r\n");
}
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Power_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Power.c文件的ASCII调试文件
*/
void Power_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:
		{
			break;
		}
		case 2:
		{
			break;
		}
	}
}
