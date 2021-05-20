#include "timer.h"


#define TIM4Period      8
#define TIM4Prescaler   9

void TIM4_Int_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIM1ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = TIM4Period; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=TIM4Prescaler;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0; 
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʼ��TIM1
	TIM_ARRPreloadConfig(TIM4,ENABLE); //ʹ�ܶ�ʱ��1
	
}

void delay_us(int Times)//u16 Times
{
TIM4->CR1 |=TIM_CR1_CEN;
while(Times--)
{
while((TIM4->SR & TIM_FLAG_Update)==RESET);
TIM4->SR &= ~TIM_FLAG_Update;
}
TIM4->CR1 &=~TIM_CR1_CEN;
}
 
void delay_ms(u16 nms)
{
while(nms--)
 {
  delay_us(1000);
 }
}

////ͨ�ö�ʱ��3�жϳ�ʼ��
////arr���Զ���װֵ��
////psc��ʱ��Ԥ��Ƶ��
////��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
////Ft=��ʱ������Ƶ��,��λ:Mhz
////����ʹ�õ��Ƕ�ʱ��3!
//void TIM3_Int_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
//	
//	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
//	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
//	TIM_Cmd(TIM3,ENABLE); //ʹ�ܶ�ʱ��3
//	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//}

////��ʱ��3�жϷ�����
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
//	{ 
//		sec_flag = 1;
//	}
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
//}
