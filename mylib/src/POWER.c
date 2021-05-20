/*
	���ļ�Ϊ����Ŀ����Ļ��غ���
	���庯������
*/

#include "Power.h"
#include "stm32f4xx_exti.h"
Power_DataStructure PowerData;
/*
	��������void Power_Main(void)
	������	heat.c������
*/
void Power_Main(void)
{
	
}
//---
/*
	��������void Power_Timer(void)
	������Power.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void Power_Timer(void)
{ u32 i;
	//����Ϊ10MS���һ��power ״̬ģ��
	if(PowerData.Flg&0x01)    //������������Ϳɼ�ⰴ��
	{
	if(PowerData.Timer10ms>0)
	{
		PowerData.Timer10ms--;
	}
	else  //
	{
		  //10MS
		{  //�����Դ����
			PowerData.V220Cnt++;
		  if(PowerData.V220Cnt>=5) //��⵽û���220V   ,50MS
		  { //������Դ����
			  PowerData.V220Cnt=5;
			 
			 if((PowerData.Flg&0x04)==0) //������Դ���� PowerData.Flg=PowerData.Flg|0x02;  //�����ⲿ��Դ���
			  {
		        CTRbat_OUT(0);   //�򿪵�ص�Դ	
				for(i=0;i<=9965535;i++)
			   {
			   PowerData.Flg=PowerData.Flg|0x04;  //���õ�ر��
			   PowerData.Flg=PowerData.Flg&0xfd;  //���õ�ر��
			   }
			   CTRexPower_OUT(1);//�ر��ⲿ��Դ
			  }
		  }
		}
		
		PowerData.Timer10ms=10;   //����10MS
		
		//if(PowerData.Flg&0x02) //���ѹ�򿪣�
		//{
		if((TexPower_LED)&&(Tbat_LED))  //����ƽ
		PowerData.StaCnt++;
		
		//}
		//else                   //��ش���
		//{
		//if(Tbat_LED)  //����ƽ
		//PowerData.StaCnt++;
		//}
		
		PowerData.Cnt++;
		if(PowerData.Cnt>=10)  //100m��ʱ�䵽,1s�ж�һ��
		{
			//����ж�С���ߵ�ƽ
			if(PowerData.StaCnt==10)  //����û����
			{
				PowerData.starFlg = 0;  //�޵�
			}
			else if(PowerData.StaCnt==0)  //��������
			{
				if(PowerData.starFlg==0)
				{
				PowerData.starFlg = 1;//�е�
				PowerData.Tcnt=0;
				}
				else
				{
				PowerData.Tcnt++;   //100ms��1��
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
	��������EX0_Init(void)
	������	Power.c�ļ����жϳ�ʼ������
*/
void EX0_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��ʱ��
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource11);//��PD11���ӵ����жϹ���

    /* ����EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line11;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //����Դ����;  EXTI_Trigger_Falling�½�Դ
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
	EXTI_Init(&EXTI_InitStructure);//����

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ⲿ�ж�11
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//������2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����

}

//---
/*
	��������void Power_Init(void)
	������	Power.c�ļ���ʼ������
*/
void Power_Init(void)
{ 
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(Tbat_APP, ENABLE);
  RCC_AHB1PeriphClockCmd(CTRbat_APP, ENABLE);
  RCC_AHB1PeriphClockCmd(TexPower_APP, ENABLE);
  RCC_AHB1PeriphClockCmd(CTRexPower_APP, ENABLE);
	//���������������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_InitStructure.GPIO_Pin = CTRbat_PIN;	 //***********
  GPIO_Init(CTRbat_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = CTRexPower_PIN;	 //***********
  GPIO_Init(CTRexPower_PORT, &GPIO_InitStructure);	
 
	//����������������

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //��ͨ����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;         //�����޷���ȡ����ʱ�䣬��������

    GPIO_InitStructure.GPIO_Pin = Tbat_PIN;              //
    GPIO_Init(Tbat_PORT, &GPIO_InitStructure);                 //��ʼ��
	
	GPIO_InitStructure.GPIO_Pin = TexPower_PIN;              //
    GPIO_Init(TexPower_PORT, &GPIO_InitStructure);                 //��ʼ��
	
	//��ʼ��״̬�Ĵ���
	PowerData.Flg=0;
	delay_ms(10);   //��ʱ10����

	//����ⲿ��Դ�Ƿ��е�
	//if(Tbat_LED)  //�ߵ�ƽû��ӵ�ԴGPIO_ReadOutputDataBit (GPIOC,GPIO_Pin_13)==0
	if(GPIO_ReadOutputDataBit (CTRexPower_PORT,CTRexPower_PIN)==0)
	{
			
		 CTRexPower_OUT(0);//���ⲿ��Դ

		 PowerData.Flg=PowerData.Flg|0x02;  //�����ⲿ��Դ���
		 PowerData.Flg=PowerData.Flg&0xFB;  //�����õ�ر��
		 PowerData.V220Cnt=0;
		 CTRbat_OUT(1);//�رյ�ص�Դ
	}
	else
	{  
		 CTRbat_OUT(0);   //�򿪵�ص�Դ
    
		 PowerData.Flg=PowerData.Flg|0x04;  //���õ�ر��
		 PowerData.Flg=PowerData.Flg&0xFD;  //���ⲿ��Դ���
		 CTRexPower_OUT(1);//�ر��ⲿ��Դ

	}
  
	EX0_Init();    //��ʼ�����ж�
	
}
//---
/*
	��������Power_DataStructure* Power_GetData(void)
	��������ȡ����Power.c�ļ�ȫ�ֱ����ṹ�����
*/
Power_DataStructure* Power_GetData(void)
{
	return &PowerData;
}



//���жϷ������
void EXTI15_10_IRQHandler(void)
{  u32 i;
	   PowerData.V220Cnt=0;  //���ж���ռ�����
	

	   if(PowerData.Flg&0x01)    //������������Ϳɼ�ⰴ��
	   {
			 if(PowerData.Flg&0x04)  //����ǵ�ع���   
			 {
			 CTRexPower_OUT(0);//���ⲿ��Դ
			 for(i=0;i<=9965535;i++)
			 {
		       PowerData.Flg=PowerData.Flg|0x02;  //�����ⲿ��Դ���
		       PowerData.Flg=PowerData.Flg&0xFB;  //�����õ�ر��
			 }
			 CTRbat_OUT(1);//�رյ�ص�Դ
			 EXTI_ClearITPendingBit(EXTI_Line10); //����жϱ�־
			 }
	   }
	   EXTI_ClearITPendingBit(EXTI_Line10); //����жϱ�־
	   EXTI_ClearITPendingBit(EXTI_Line11); //����жϱ�־
	   EXTI_ClearITPendingBit(EXTI_Line12); //����жϱ�־
	   EXTI_ClearITPendingBit(EXTI_Line13); //����жϱ�־
	   EXTI_ClearITPendingBit(EXTI_Line14); //����жϱ�־
	   EXTI_ClearITPendingBit(EXTI_Line15); //����жϱ�־
	
}	

//-------------------------���ݶ�ȡ���洢----------------------------
/*
	��������Power_SetFlg(void)
	�����������1�ſ�ʼ��⿪�ػ�����
*/
void Power_SetFlg(void)//����ӿ�
 {
   #ifdef NEWBOARD
	 PowerData.Flg=PowerData.Flg|0x1;  //��ʽʱҪ����
  #endif
	 
	 PowerData.Timer10ms=10;
	 PowerData.StaCnt=0;
	 PowerData.Cnt=0;
 }

//---
/*
	��������void Power_ReadData(u8 address,u8 *p,u8 len)
	������	��ȡ����
*/
void Power_ReadData(u8 address,u8 *p,u8 len)
{

}
//---
/*
	��������void Power_StoreData(u8 address,u8 *p,u8 len)
	�������ú���Ϊ�洢Power.c���ò�������
	˵������ȡ����
				��������
				�޸�����
				��������
*/
void Power_StoreData(u8 address,u8 *p,u8 len)
{

}
//---
/*
	��������void Power_FactoryReset(void)
	�������ú���Power.c�洢���ݻָ���������
*/
void Power_FactoryReset(void)
{

	printf("Power������ʼ��\r\n");
}
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Power_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪPower.c�ļ���ASCII�����ļ�
*/
void Power_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
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
