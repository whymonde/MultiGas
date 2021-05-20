/*
	���ļ�Ϊ����Ŀ����Ļ��غ���
	���庯������
*/

#include "heat.h"

Heat_DataStructure HeatData;
/*
	��������void Heat_Main(void)
	������	heat.c������
*/
void Heat_Main(void)
{
//	switch(HeatData.RunMode)
//	{
//		case 1:
//		{//����ģʽ
//			if(Flashisp_GetData()->Data[FLASHIS_AdrXTSZJRSZKG])
//			{//�򿪼���
//				Heat_DataInit();
//				HeatData.RunMode = 2;
//			}
//			break;
//		}
//		case 2:
//		{
//			if(Flashisp_GetData()->Data[FLASHIS_AdrXTSZJRSZKG])
//			{//�򿪼���
//				if(Slave_GetData()->BWXTem.Value != FLUX_ColFAULT)
//				{//�����¶ȴ������޹���
//					HeatData.PidData.Pv = Slave_GetData()->BWXTem.Value;
//					HeatData.PidData.Sv = Flashisp_GetData()->Data[FLASHIS_AdrXTSZJRSZWD];//�û��趨�¶�
//					Pid_Calculate(&HeatData.PidData);
//					Heat_OnOff(1,(u16)HeatData.PidData.OUT);
//				}
//				else Heat_OnOff(0,0);//�رռ���
//			}
//			else Heat_OnOff(0,0);//�رռ���
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
	��������void Heat_Timer(void)
	������Heat.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void Heat_Timer(void)
{
	HeatData.PidData.Timer++;
	
//	HEAT_OUT(2)
}
//---
/*
	��������void Heat_Init(void)
	������	Heat.c�ļ���ʼ������
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
	��������void Heat_DataInit(void)
	������	�ú���ΪHeat.c���ݳ�ʼ��
*/
void Heat_DataInit(void)
{
	HeatData.RunMode = 1;
	//PID��ʼ��
	HeatData.PidData.Sv = 50;//�û��趨�¶�
	HeatData.PidData.Kp = 3000;//����ϵ��
	HeatData.PidData.T = 800;//�������ڣ�1000ms
	HeatData.PidData.Ti = 16000;//��������
	HeatData.PidData.Td = 8000;//΢��ʱ��
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
	��������void Heat_GpioInit(void)
	������	�ú���ΪHeat.cͨ��IO�ڳ�ʼ������
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
	�������� void Project_TimerInit(void)
	������	 	�˺���Ϊ������Ŀ�Ķ�ʱ��������Ϊ1ms
	˵����	 	�ú���ʹ�õ���ʱ��4
*/
void Heat_TimerInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStruct;	
	//���ö�ʱ����������
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 71;    		//Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���,72/18=4MHz.1us
	TIM_TimeBaseStructure.TIM_Period = 65535;				//����ֵ   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	//������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//TIM_CH2����
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//ָ��TIMģʽ
	TIM_OCInitStruct.TIM_OutputState = 	TIM_OutputState_Disable;//TIM_OutputState_Enable;//ָ��TIM����Ƚ�״̬
	TIM_OCInitStruct.TIM_OutputNState =	TIM_OutputNState_Disable;//ָ��TIM��������Ƚ�״̬
	TIM_OCInitStruct.TIM_Pulse =	20000;//ָ��Ҫ���ص�����ȽϼĴ����е�����ֵ
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_Low;//ָ���������
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;//ָ�������������
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OutputState_Enable;//ָ������״̬�ڼ��TIM����Ƚ�����״̬
	TIM_OCInitStruct.TIM_OCNIdleState =	TIM_OutputNState_Disable;//ָ������״̬�ڼ��TIM����Ƚ�����״̬
	TIM_OC2Init(TIM2,&TIM_OCInitStruct);
//	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable);//ʹ��TIMx-OCxPEλ
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
	ͨ�ö�ʱ��2�жϣ�����ϵͳ��ʱ����1ms
*/
void TIM2_IRQHandler(void)
{
	if(TIM2->SR & B0)	
	{
		if(HeatData.Mode == 1) 
			if(HeatData.PidData.OUT  != 0){HEAT_OUT(1)}
		TIM2->SR &= ~B0;	//����жϱ�־λ
	}
	if(TIM2->SR & B2)	
	{//����Ƚ�2�ж�
		HEAT_OUT(0)
		TIM2->SR &= ~B2;	//����жϱ�־λ
	}
}
//---
/*
	��������Heat_DataStructure* Heat_GetData(void)
	��������ȡ����Heat.c�ļ�ȫ�ֱ����ṹ�����
*/
Heat_DataStructure* Heat_GetData(void)
{
	return &HeatData;
}
//---
/*
	��������void Heat_OnOff(u8 sw,u16 pwm)
	������	�ú���Ϊ��ͣ������
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
//-------------------------���ݶ�ȡ���洢----------------------------
//---
/*
	��������void Heat_ReadData(u8 address,u8 *p,u8 len)
	������	��ȡ����
*/
void Heat_ReadData(u8 address,u8 *p,u8 len)
{
//	W25Xxx_ReadData(p,AD7689_AdrStoreBase+address,len);
}
//---
/*
	��������void Heat_StoreData(u8 address,u8 *p,u8 len)
	�������ú���Ϊ�洢Heat.c���ò�������
	˵������ȡ����
				��������
				�޸�����
				��������
*/
void Heat_StoreData(u8 address,u8 *p,u8 len)
{
//	u8 buffer[AD7689_AdrStoreEND],cnt;
//	W25Xxx_ReadData(buffer,AD7689_AdrStoreBase,AD7689_AdrStoreEND);//���������ò���
//	W25Xxx_EraseSector(AD7689_AdrStoreBase);//��������
//	for(cnt=0;cnt<len;cnt++)
//	{
//		buffer[address++] = *p;
//		p++;
//	}
//	W25Xxx_PageWrite(buffer,AD7689_AdrStoreBase,AD7689_AdrStoreEND);//��������
}
//---
/*
	��������void Heat_FactoryReset(void)
	�������ú���Heat.c�洢���ݻָ���������
*/
void Heat_FactoryReset(void)
{
	//AD4ͨ��
//	i = 4;
//	AD7689_StoreData(AD7689_AdrStoreAD4Map,&i,1);//�豸ӳ��
//	two.U16Data = 100;
//	AD7689_StoreData(AD7689_AdrStoreAD4Orgin,two.U8Data,2);//ԭ��ֵ
//	two.U16Data = 60000;
//	AD7689_StoreData(AD7689_AdrStoreAD4Endpoint,two.U8Data,2);//�˵�ֵ
//	four.FloatData = 50000.0;
//	AD7689_StoreData(AD7689_AdrStoreAD4Refer,four.U8Data,4);//����ֵ
	//��ʼ����־λ
//	two.U8Data[0] = AD7689_FACTORY;
//	AD7689_StoreData(AD7689_AdrStoreFactory,two.U8Data,1);//�ָ��������ñ�־
	printf("Heat������ʼ��\r\n");
}
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Heat_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪHeat.c�ļ���ASCII�����ļ�
���		:000|05|xxx
*/
void Heat_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:
		{
			u16 bf = Calculate_CharToU16(p+6,5);//��������
			if(*(p+4)=='0') 
			{
				Heat_OnOff(0,0);
				printf("ֹͣ����\r\n");
			}
			else 
			{
				Heat_OnOff(1,bf);
				printf("��������:%u\r\n",bf);
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





































