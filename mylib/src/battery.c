/*
	���ļ�Ϊ����Ŀ�ĵ����غ���
	���庯������
	
	˵����
	�����
	�ɼ���ѹ��Battery_GetData()->Battery.ActualV 
	��ֵ�жϣ�Battery_GetData()->Mode ��=1����ִ�вɼ�����
*/

#include "battery.h"

Battery_DataStructure BatteryData;
/*
	��������void Battery_Main(void)
	������	Battery.c�ļ�������
*/
void Battery_Main(void)
{
	Battery_GetVoltage();
	Battery_DealWith();
}
//---
/*
	��������void Battery_Init(void)
	������	Battery.c�ļ���ʼ������
*/
void Battery_Init(void)
{
	Battery_InitADC();
	Battery_DataInit();
}
//---
/*
	��������void Battery_Timer(void)
	������Battery.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void Battery_Timer(void)
{
	
}
//---
/*
	��������Battery_DataStructure* Battery_GetData(void)
	��������ȡ����Battery.c�ļ�ȫ�ֱ����ṹ��
*/
Battery_DataStructure* Battery_GetData(void)
{
	return &BatteryData;
}
//---
/*
	��������void Battery_DataInit(void)
	������	Battery.c�ļ����ݳ�ʼ������
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
	��������void Battery_ReadFlashData(void)
	������	�ú���Ϊ��ȡflash�洢���ڵ�����
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
	��������void Tem_InitADC(void)
	������	��ʼ��ADCģ��
*/
/* STM32оƬADCת�����DR�Ĵ�������ַ */
void Battery_InitADC(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��
	
	//�ȳ�ʼ��ADC1ͨ��10 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//pc0 ͨ��10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  

  //�ȳ�ʼ��ADC1ͨ��11 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//Pc1 ͨ��11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  
	
 
    //�ȳ�ʼ��ADC1ͨ��13 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//Pc3 ͨ��13
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��  	
	
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Left;//�����	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
	ADC_Cmd(ADC1, ENABLE);//����ADת����	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
	ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
	BatteryData.CurChanl=1;
}
//---
/*
	��������void Battery_GetVoltage(void)
	������	�ú���Ϊ��ȡ��Դ��ѹ
*/
void Battery_GetVoltage(void)
{   	
	u8 i;
	u32 sum;// = 0;
	
	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ))
	{
		if(BatteryData.CurChanl==1)  //��ͨ��PC1  ch11
		{
		  BatteryData.Battery.Data[BatteryData.Battery.Cnt++] = ADC_GetConversionValue(ADC1)>>4;
		 if(BatteryData.Battery.Cnt >= BATTERY_DATALEN) BatteryData.Battery.Cnt = 0;                //��
		 {//���ݴ���
		  sum= 0;
			for(i=0;i<BATTERY_DATALEN;i++)
			{
				sum += BatteryData.Battery.Data[i];
			}
			BatteryData.Battery.CollectV = ((float)sum / BATTERY_DATALEN);
			BatteryData.Battery.CollectV *= (BATTERY_Vref/4096);

			BatteryData.Battery.ActualV = BatteryData.Battery.CollectV * BatteryData.Battery.Rate * BATTERY_OP;
		 }
     //������һͨ��PC0
		 ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
		 ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
		 BatteryData.CurChanl=2;
	  }
		else if(BatteryData.CurChanl==2) //��ͨ��PC0  ch10
		{
		 BatteryData.LostPV.Data[BatteryData.LostPV.Cnt++] = ADC_GetConversionValue(ADC1)>>4;
		 if(BatteryData.LostPV.Cnt >= BATTERY_DATALEN) BatteryData.LostPV.Cnt = 0;                //��
		 {//���ݴ���
			  sum= 0;
			for(i=0;i<BATTERY_DATALEN;i++)
			{
				sum += BatteryData.LostPV.Data[i];
			}
			BatteryData.LostPV.CollectV = ((float)sum / BATTERY_DATALEN);
			BatteryData.LostPV.CollectV *= (BATTERY_Vref/4096);

			BatteryData.LostPV.ActualV = BatteryData.LostPV.CollectV * 1 * BATTERY_OP;
			
		 }
     //������һͨ��PC1
		
		 
		 ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
		 ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
		 BatteryData.CurChanl=3;
		}
		
		else if(BatteryData.CurChanl==3)  //��ͨ��PC0  ch12
		{
		 BatteryData.Current.Data[BatteryData.Current.Cnt++] = ADC_GetConversionValue(ADC1)>>4;
		 if(BatteryData.Current.Cnt >= BATTERY_DATALEN) BatteryData.Current.Cnt = 0;                //��
		 {//���ݴ���
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
     //������һͨ��PC1
		 ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
		 ADC_SoftwareStartConv(ADC1);		//ʹ��ָ����ADC1�����ת����������	
		 BatteryData.CurChanl=1;
		}
	
	}

}
//---
/*
	��������void Battery_DealWith(void)
	������	�ú���Ϊ�Բɼ����ĵ�ѹ������ֵ�ж�
*/
void Battery_DealWith(void)
{
	
		//��ֵ�ж�
	if(BatteryData.LostPV.ActualV > BATTERY_VPT_ON)
	{
		BatteryData.LostPowerFlag = 1;
	}
	else if(BatteryData.LostPV.ActualV < BATTERY_VPT_OFF)
	{
		BatteryData.LostPowerFlag = 0;
	}

	//��ֵ�ж�
	if(BatteryData.Battery.ActualV > BATTERY_VPT_ON)
	{
		BatteryData.Mode = 1;
	}
	else if(BatteryData.Battery.ActualV < BATTERY_VPT_OFF)
	{
		BatteryData.Mode = 0;
	}
	//�����������
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
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Battery_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪBattery.c�ļ���ASCII�����ļ�
*/
void Battery_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 101:
		{//���ص�ǰ��ѹ	:000|07|101
			printf("�ɼ���ѹ��%f\r\n",BatteryData.Battery.CollectV);
			printf("�����ѹ��%f\r\n",BatteryData.Battery.ActualV);
			printf("��ѹ���ʣ�%f\r\n",BatteryData.Battery.Rate);
			break; 
		}
		case 201:
		{//���õ�ѹ����
			FourBytes four;
			four.U16Data[0] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHDYBL];
			four.U16Data[1] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHDYBL+1];
			printf("��ǰ���õ�ѹ���ʣ�%f\r\n",four.FloatData);
			break;
		}
		default:
		{
			printf("battery.c���յ���Ч����ָ��%u!\r\n",cmd);
			break;
		}
	}
}
//---
/*
	�������� 	void Battery_DebugRTU((u8 *p)
	������		�ú���ΪBattery.c�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*//*
void Battery_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//�鿴���ĸ�������
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





































