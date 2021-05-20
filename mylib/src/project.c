/*
	���ļ�Ϊ����Ŀ����غ���
	���庯������
	void Init_Project(void)		//����Ŀ�ļ�������Ϣ��ʼ������
	struct Project_DataStructure *Get_SYSBuffer(void)	//����ϵͳ�����ṹ��ĵ�ַ����
	void ProjectTimer_Timer6Init(void)	//�˺���Ϊ������Ŀ�Ķ�ʱ��������Ϊ1ms
	void Control_SetCmd(u8 *p)	//����ϵͳ�����豸
	void SYS_SetCmd(u8 *p) 	//ϵͳ�������
*/

#include "project.h"
#include "timer.h"
#include "button.h"
#include "FM24CL64.h"
//����ϵͳ�ṹ���������ʼ������
Project_SystemStructure Project_SysBuffer;

//Project_BufferStructure ProjectBuffer;

/*
	�������� void Project_Init(void)
	������	 ������Ϊ������ʼ��
*/
void Project_Init(void)
{
	{
	RCC_ClocksTypeDef RCC_Clocks;
  /* SysTick end of count event each 1ms */
//	RCC_PLLConfig(RCC_PLLSource_HSE,4,336,2,14);
  RCC_GetClocksFreq(&RCC_Clocks);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�ж�����ģʽ
	//----------------
	//Ӳ��������س�ʼ��
	Project_TimerInit();	//��ʼ������Ŀ�Ķ�ʱ��	
	USART_InitAll();			//��ʼ������
	TIM4_Int_Init();
	Power_Init();
	Printf_Init();
	__enable_irq();		//���ӿ�ȫ���ж�, ��Ȼͨ��Bootloader������Ӧ�ó���������, add by alfred, 200.06.20
	printf("ϵͳ������!\r\n");	//STM32F1-0x1FFFF7E8		STM32F4-0x1FFF7A10
	printf("��ƷΨһ��ݱ�ʶ(Unique Device ID)Ϊ��[%X-%X-%X]��\r\n", 
								*(u32*)(0x1FFF7A10), *(u32*)(0x1FFF7A10+4), *(u32*)(0x1FFF7A10+8));
	printf("��оƬROMΪ��[%uKb]��\r\n\r\n",*(u16*)(0x1FFF7A22));//STM32F1-0x1FFFF7E0		STM32F4-0x1FFF7A22
	printf("SYSCLK_Frequency=%u!\r\n",RCC_Clocks.SYSCLK_Frequency);
	printf("HCLK_Frequency=%u!\r\n",RCC_Clocks.HCLK_Frequency);
	printf("PCLK1_Frequency=%u!\r\n",RCC_Clocks.PCLK1_Frequency);
	printf("PCLK2_Frequency=%u!\r\n\r\n",RCC_Clocks.PCLK2_Frequency);	
	printf("USART_DataStructure=%u!\r\n\r\n",sizeof(USART_DataStructure));	
	printf("Slave_DataStructure=%u!\r\n\r\n",sizeof(Slave_DataStructure));
	Project_IWDGInit(3000);//��ʼ���������Ź�
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
	�������� void Project_Timer(void)
	������	 ϵͳ��ʱ����1ms
*/
void Project_Timer(void)
{
	if(Project_SysBuffer.DelayTimre != 0) Project_SysBuffer.DelayTimre--;	//ϵͳ��ʱ��
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
	�������� void Project_SecTimer(void)
	������	�ú���Ϊϵͳ��ȷ�붨ʱ����ÿ��1���ӣ�ϵͳ�Զ�����ú���һ
					�Σ�ʱ��Դ�ɵ�����Ļ�ṩ��ʱ��
*/
void Project_SecTimer(void)
{

	Screen_SecTimer();
	Flux_SecTimer();
	ScreenJPG_Timer();
}
//---
/*
	��������	void Project_ClearProjectSysBuffer(void)
	������		�ú������ڸ�λProjec_SysBuffer���е�����
*/
void Project_ClearProjectSysBuffer(void)
{
	Project_SysBuffer.Address = 0;
	Project_SysBuffer.RunMode = 0;
	Project_SysBuffer.Timer = 0;
	Project_SysBuffer.DelayTimre = 0;
}
/*
	�������� Project_DataStructure *Project_GetProjectSysBuffer(void)
	������	 	����ϵͳ�����ṹ��ĵ�ַ�������ݰ���
					u8 flag;	//ϵͳ����״̬(0Ϊ��δ�������У�1Ϊ�������У�2Ϊ����ģʽ)
					u32 timer;	//ϵͳ�ۼ�����ʱ�䣬��λ��
*/
Project_SystemStructure *Project_GetData(void)
{
	return &Project_SysBuffer;
}
//---
//--------------------------Ӳ����----------------------------
//---
/*
	�������� void Project_TimerInit(void)
	������	 	�˺���Ϊ������Ŀ�Ķ�ʱ��������Ϊ1ms
	˵����	 	�ú���ʹ�õ���ʱ��4
*/
void Project_TimerInit(void)
{
	if(SysTick_Config(168000))
	{
		printf("Systick��ʱ������ʧ��\r\n");
	}
}
//---
/*
	ϵͳ��શ�ʱ���жϣ�����ϵͳ��ʱ����1ms
*/

void SysTick_Handler(void)
{
	Project_Timer();
}

/*
	��������void Project_InitLED(void)
	��������ʼ��LEDָʾ��
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
	��������void 
	�������ú������������������Ź�
	���÷�Χ��2-3276ms
*/
void Project_IWDGInit(u32 TimeoutFreMs)
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);//Enable write access to IWDG_PR and IWDG_RLR registers
    IWDG_SetPrescaler(IWDG_Prescaler_32);// IWDG counter clock: 40KHz(LSI) / 32 = 1.25KHz
    if(TimeoutFreMs < 2)TimeoutFreMs = 2;
    if(TimeoutFreMs > 3276) TimeoutFreMs = 3276;
    TimeoutFreMs = ((TimeoutFreMs * 1250)/1000) - 1;
    IWDG_SetReload(TimeoutFreMs);  //Set counter reload value to 349, range from 0 to 4095
//    printf("IWDG��ʱ����%dms\r\n", TimeoutFreMs);
    IWDG_ReloadCounter();//Reload IWDG counter
  //  IWDG_Enable();//Enable IWDG (the LSI oscillator will be enabled by hardware)
}
//---
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Project_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪPorject�ļ���ASCII�����ļ�
	���	:000|90|xxx
*/
void Project_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:
		{//ϵͳ��λ
			Flashisp_StoreData();	//��������
			NVIC_SystemReset();		//ϵͳ�����λ
			break;
		}
		case 2:
		{//��λProjec_SysBuffer���е�����
			Project_ClearProjectSysBuffer();
			break;
		}
	}
}
//---











