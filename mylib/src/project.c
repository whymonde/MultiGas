/*
	本文件为该项目的相关函数
	具体函数如下
	void Init_Project(void)		//本项目文件基本信息初始化函数
	struct Project_DataStructure *Get_SYSBuffer(void)	//返回系统变量结构体的地址函数
	void ProjectTimer_Timer6Init(void)	//此函数为整个项目的定时器，周期为1ms
	void Control_SetCmd(u8 *p)	//控制系统外设设备
	void SYS_SetCmd(u8 *p) 	//系统设置相关
*/

#include "project.h"
#include "timer.h"
#include "button.h"
#include "FM24CL64.h"
//声明系统结构体变量并初始化变量
Project_SystemStructure Project_SysBuffer;

//Project_BufferStructure ProjectBuffer;

/*
	函数名： void Project_Init(void)
	描述：	 本函数为函数初始化
*/
void Project_Init(void)
{
	{
	RCC_ClocksTypeDef RCC_Clocks;
  /* SysTick end of count event each 1ms */
//	RCC_PLLConfig(RCC_PLLSource_HSE,4,336,2,14);
  RCC_GetClocksFreq(&RCC_Clocks);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断向量模式
	//----------------
	//硬件配置相关初始化
	Project_TimerInit();	//初始化本项目的定时器	
	USART_InitAll();			//初始化串口
	TIM4_Int_Init();
	Power_Init();
	Printf_Init();
	__enable_irq();		//增加开全局中断, 不然通过Bootloader引导到应用程序后会死机, add by alfred, 200.06.20
	printf("系统重启了!\r\n");	//STM32F1-0x1FFFF7E8		STM32F4-0x1FFF7A10
	printf("产品唯一身份标识(Unique Device ID)为：[%X-%X-%X]！\r\n", 
								*(u32*)(0x1FFF7A10), *(u32*)(0x1FFF7A10+4), *(u32*)(0x1FFF7A10+8));
	printf("该芯片ROM为：[%uKb]！\r\n\r\n",*(u16*)(0x1FFF7A22));//STM32F1-0x1FFFF7E0		STM32F4-0x1FFF7A22
	printf("SYSCLK_Frequency=%u!\r\n",RCC_Clocks.SYSCLK_Frequency);
	printf("HCLK_Frequency=%u!\r\n",RCC_Clocks.HCLK_Frequency);
	printf("PCLK1_Frequency=%u!\r\n",RCC_Clocks.PCLK1_Frequency);
	printf("PCLK2_Frequency=%u!\r\n\r\n",RCC_Clocks.PCLK2_Frequency);	
	printf("USART_DataStructure=%u!\r\n\r\n",sizeof(USART_DataStructure));	
	printf("Slave_DataStructure=%u!\r\n\r\n",sizeof(Slave_DataStructure));
	Project_IWDGInit(3000);//初始化独立看门狗
	}

	Speak_Init();
	Button_Init();
	TIM4_Int_Init();
	FM24CL64_Init();
	delay_us(100);
	Flashisp_Init();
	Slave_Init();
    Screen_Init();
	SdFile_Init();
	Usb_Init();	//
	AT_Init();
	APP_Init();
	Print_Init();
	Flux_Init();
#ifdef NEWBOARD	
	Battery_Init();  
#endif	
    Fan_Init();

	
	TestPower_Init();
	LTC2944_Init();
	DS18B20_Init();
//	RTU_Init();
//	UsbInit();
//	Project_InitLED();
	
}
//---
/*
	函数名： void Project_Timer(void)
	描述：	 系统定时器，1ms
*/
void Project_Timer(void)
{
	if(Project_SysBuffer.DelayTimre != 0) Project_SysBuffer.DelayTimre--;	//系统定时器
	Project_SysBuffer.TimerPro++;
	
	USART_Timer();	
	Slave_Timer();	
	Screen_Timer();
	AT_Timer();
	APP_Timer();
	PrintTimer();
	Button_Timer();
    SdFile_Timer();
    Usb_Timer();
	Speak_Timer();
	TestPower_Timer();
	FM24CL64_Timer();
	Power_Timer();
	LTC2944_Timer();
    DS18B20_Timer();
//	PRO_LED1(2)
//	PRO_LED2(2)
}
//---
/*
	函数名： void Project_SecTimer(void)
	描述：	该函数为系统精确秒定时器，每隔1秒钟，系统自动进入该函数一
					次，时钟源由迪文屏幕提供秒时钟
*/
void Project_SecTimer(void)
{

	Screen_SecTimer();
	Flux_SecTimer();
	ScreenJPG_Timer();
}
//---
/*
	函数名：	void Project_ClearProjectSysBuffer(void)
	描述：		该函数用于复位Projec_SysBuffer所有的数据
*/
void Project_ClearProjectSysBuffer(void)
{
	Project_SysBuffer.Address = 0;
	Project_SysBuffer.RunMode = 0;
	Project_SysBuffer.Timer = 0;
	Project_SysBuffer.DelayTimre = 0;
}
/*
	函数名： Project_DataStructure *Project_GetProjectSysBuffer(void)
	描述：	 	返回系统变量结构体的地址，其内容包括
					u8 flag;	//系统运行状态(0为还未进入运行，1为正常运行，2为调试模式)
					u32 timer;	//系统累计运行时间，单位秒
*/
Project_SystemStructure *Project_GetData(void)
{
	return &Project_SysBuffer;
}
//---
//--------------------------硬件层----------------------------
//---
/*
	函数名： void Project_TimerInit(void)
	描述：	 	此函数为整个项目的定时器，周期为1ms
	说明：	 	该函数使用到定时器4
*/
void Project_TimerInit(void)
{
	if(SysTick_Config(168000))
	{
		printf("Systick定时器启动失败\r\n");
	}
}
//---
/*
	系统嘀嗒定时器中断，整个系统定时器，1ms
*/

void SysTick_Handler(void)
{
	Project_Timer();
}

/*
	函数名：void Project_InitLED(void)
	描述：初始化LED指示灯
*/
void Project_InitLED(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(PRO_LED1_RCC, ENABLE); 
	RCC_AHB1PeriphClockCmd(PRO_LED2_RCC, ENABLE); 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = PRO_LED1_PIN;
  GPIO_Init(PRO_LED1_GPIO, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = PRO_LED2_PIN;
  GPIO_Init(PRO_LED2_GPIO, &GPIO_InitStructure);
	PRO_LED1(0);
	PRO_LED2(1)
}
//---

//---
/*
	函数名：void 
	描述：该函数用于启动独立看门狗
	设置范围：2-3276ms
*/
void Project_IWDGInit(u32 TimeoutFreMs)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//Enable write access to IWDG_PR and IWDG_RLR registers
    IWDG_SetPrescaler(IWDG_Prescaler_32);// IWDG counter clock: 40KHz(LSI) / 32 = 1.25KHz
    if(TimeoutFreMs < 2)TimeoutFreMs = 2;
    if(TimeoutFreMs > 3276) TimeoutFreMs = 3276;
    TimeoutFreMs = ((TimeoutFreMs * 1250)/1000) - 1;
    IWDG_SetReload(TimeoutFreMs);  //Set counter reload value to 349, range from 0 to 4095
//    printf("IWDG超时周期%dms\r\n", TimeoutFreMs);
    IWDG_ReloadCounter();//Reload IWDG counter
  //  IWDG_Enable();//Enable IWDG (the LSI oscillator will be enabled by hardware)
}
//---
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Project_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Porject文件的ASCII调试文件
	命令：	:000|90|xxx
*/
void Project_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:
		{//系统复位
			Flashisp_StoreData();	//保存数据
			NVIC_SystemReset();		//系统软件复位
			break;
		}
		case 2:
		{//置位Projec_SysBuffer所有的数据
			Project_ClearProjectSysBuffer();
			break;
		}
	}
}
//---











