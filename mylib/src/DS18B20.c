/*
	���ļ�Ϊ����Ŀ����Ļ��غ���
	���庯������
*/

#include "DS18B20.h"

//--------------------------------------------------------------------
//CRC = X8 + X5 + X4 + 1
const u8 Crc8Table[256]={
0,  94, 188,  226,  97,  63,  221,  131,  194,  156,  126,  32,  163,  253,  31,  65,
157,  195,  33,  127,  252,  162,  64,  30,  95,  1,  227,  189,  62,  96,  130,  220,
35,  125,  159,  193,  66,  28,  254,  160,  225,  191,  93,  3,  128,  222,  60,  98,
190,  224,  2,  92,  223,  129,  99,  61,  124,  34,  192,  158,  29,  67,  161,  255,
70,  24,  250,  164,  39,  121,  155,  197,  132,  218,  56,  102,  229,  187,  89,  7,
219,  133, 103,  57,  186,  228,  6,  88,  25,  71,  165,  251,  120,  38,  196,  154,
101,  59, 217,  135,  4,  90,  184,  230,  167,  249,  27,  69,  198,  152,  122,  36,
248,  166, 68,  26,  153,  199,  37,  123,  58,  100,  134,  216,  91,  5,  231,  185,
140,  210, 48,  110,  237,  179,  81,  15,  78,  16,  242,  172,  47,  113,  147,  205,
17,  79,  173,  243,  112,  46,  204,  146,  211,  141,  111,  49,  178,  236,  14,  80,
175,  241, 19,  77,  206,  144,  114,  44,  109,  51,  209,  143,  12,  82,  176,  238,
50,  108,  142,  208,  83,  13,  239,  177,  240,  174,  76,  18,  145,  207,  45,  115,
202,  148, 118,  40,  171,  245,  23,  73,  8,  86,  180,  234,  105,  55,  213, 139,
87,  9,  235,  181,  54,  104,  138,  212,  149,  203,  41,  119,  244,  170,  72,  22,
233,  183,  85,  11,  136,  214,  52,  106,  43,  117,  151,  201,  74,  20,  246,  168,
116,  42,  200,  150,  21,  75,  169,  247,  182,  232,  10,  84,  215,  137,  107,  53}; 
//---
DS18B20_DataStructure DS18B20Data;
/*
	��������void DS18B20_Main(void)
	������	DS18B20��ʾ������
*/
void DS18B20_Main(void)
{
	switch(DS18B20Data.RunMode)
	{
		case 1:
		{//��λDDS18B20
			DS18B20_Reset();
			DS18B20Data.NewMode = 2;
			DS18B20Data.RunMode = 250;
			break;
		}
		case 2:
		{//�ж�DDS18B20�Ƿ����
			if(DS18B20Data.CntOne == 0xFF)
			{
				DS18B20Data.RunMode = 3;//����
//				printf("����\r\n");
			}
			else 
			{//������
				if(DS18B20Data.ErrorCnt < 250) DS18B20Data.ErrorCnt++;
				else DS18B20Data.Tem = DS18B20_ColFAULT;//����
				DS18B20Data.CntError++;
				DS18B20Data.Timer = 10;
				DS18B20Data.RunMode = 251;
				DS18B20Data.NewMode = 1;
//				printf("������\r\n");
			}
			break;
		}
		case 3:
		{//��������ROM����
			DS18B20_SendData(0xCC);
			DS18B20Data.NewMode = 4;
			DS18B20Data.RunMode = 250;
			break;
		}
		case 4:
		{//�����¶�ת������
			DS18B20_SendData(0x44);
			DS18B20Data.NewMode = 5;
			DS18B20Data.RunMode = 250;
			break;
		}
		case 5:
		{//��λDS18B20
			DS18B20_Reset();
			DS18B20Data.NewMode = 6;
			DS18B20Data.RunMode = 250;
			break;
		}
		case 6:
		{//��������ROM����
			DS18B20_SendData(0xCC);
			DS18B20Data.NewMode = 7;
			DS18B20Data.RunMode = 250;
			break;
		}
		case 7:
		{//���Ͷ�ȡ�ݴ�����
			DS18B20_SendData(0xBE);
			DS18B20Data.CntOne = 0;
			DS18B20Data.NewMode = 8;
			DS18B20Data.RunMode = 250;
			break;
		}
		case 8:
		{//������
			DS18B20_ReceiveData();
			DS18B20Data.NewMode = 9;
			DS18B20Data.RunMode = 250;
			break;
		}
		case 9:
		{//����������
			DS18B20Data.Data[DS18B20Data.CntOne++] = DS18B20Data.Buffer;
			if(DS18B20Data.CntOne<9)DS18B20Data.RunMode = 8;
			else DS18B20Data.RunMode = 10;
			break;
		}
		case 10:
		{//У��
			if(DS18B20_CRC8(DS18B20Data.Data,9) == 0)
			{//У��ɹ�
				DS18B20Data.ErrorCnt = 0;
				DS18B20Data.Tem = DS18B20_CalculateTem(DS18B20Data.Data);
//				printf("��ǰ�¶�Ϊ��%f\r\n",DS18B20Data.Tem);
			}
			else 
			{
				DS18B20Data.CntError++;
				if(DS18B20Data.ErrorCnt < 250) DS18B20Data.ErrorCnt++;
				else DS18B20Data.Tem = DS18B20_ColFAULT;//����
//				printf("У��ʧ��\r\n");
			}
//			Flux_GetData()->BWXTem.Raw = DS18B20Data.Tem;
//			Flux_GetData()->BWXTem.Actual = Flux_CalibrationData(Flux_GetData()->BWXTem.Raw,Flux_GetData()->BWXTem.K,Flux_GetData()->BWXTem.B);
			DS18B20Data.Cnt++;
			DS18B20Data.Timer = 760;
			DS18B20Data.NewMode = 1;
			DS18B20Data.RunMode = 251;
//			DS18B20Data.RunMode = 0;
			break;
		}
		//--------------------------------------------------------------------
		case 250:
		{//�ȴ��������
			if(DS18B20Data.WorkMode == 0)DS18B20Data.RunMode = DS18B20Data.NewMode;
			break;
		}
		case 251:
		{//�ȴ���ʱ���
			if(DS18B20Data.Timer == 0)DS18B20Data.RunMode = DS18B20Data.NewMode;
			break;
		}
	}			
}
//---
/*
	��������void DS18B20_Work(void)
	������	�ú���ΪDS18B20�������Ĺ�������
	˵����	�ú�����TIM3_IRQHandler��ʱ�����ã���ʱ����ʱֵΪ0.25us
*/
void DS18B20_Work(void)
{
	switch(DS18B20Data.WorkMode)
	{
		//��λDS18B20
		case 1:
		{//������500us
			DS18B20_IO_OUT();
			DS18B20_TX(0);
			DS18B20Data.WorkMode = 2;
			DS18B20_DelayTime(500);//��ʼ��������500us
			break;
		}
		case 2:
		{//��ʱ40us����ܽŵ�ƽ
			DS18B20_TX(1);
			DS18B20Data.WorkMode = 3;
			DS18B20_DelayTime(20);//��ʱ40us���������ϵĵ�ƽ
			break;
		}
		case 3:
		{//��ʼ����⸴λ��ƽ
			DS18B20_IO_IN();
			DS18B20Data.CntOne = 0;
			DS18B20Data.CntTwo = 0;
			DS18B20Data.WorkMode = 4;
			DS18B20_DelayTime(5);//��ʱ10us���������ϵĵ�ƽ
			break;
		}
		case 4:
		{//��⸴λ��ƽ
			if(DS18B20_RX() == 0) DS18B20Data.CntOne++;
			DS18B20Data.CntTwo++;
			if(DS18B20Data.CntTwo == 5) DS18B20Data.WorkMode = 5;
			DS18B20_DelayTime(5);//��ʱ10us���������ϵĵ�ƽ
			break;
		}
		case 5:
		{//�жϲ�������λ��ƽ
			if(DS18B20Data.CntOne>2) DS18B20Data.CntOne = 0xFF;//printf("�յ�DS18B20�Ļظ�\r\n");
			else DS18B20Data.CntOne = 0; //printf("û���յ�DS18B20�Ļظ�\r\n");
			DS18B20_DelayTime(80);
			DS18B20Data.WorkMode = 6;
			break;
		}
		case 6:
		{//������λ����
			DS18B20_IO_OUT();
			DS18B20Data.WorkMode = 0;
			break;
		}
		//------------------------------------------------------------
		//210~219Ϊ��DS18B20д��һ���ֽ�����
		case 11:
		{//дһ���ֽڳ�ʼ��
			DS18B20Data.CntTwo = 0;
			DS18B20_DelayTime(1);
			DS18B20Data.WorkMode = 12;
			break;
		}
		case 12:
		{
			if(DS18B20Data.CntTwo < 8)
			{
				if(DS18B20Data.Buffer & b0)
				{//д1
					DS18B20_DelayTime(1);
					DS18B20Data.WorkMode = 15;//����д1ʱ��
				}
				else
				{//д0
					DS18B20_DelayTime(1);
					DS18B20Data.WorkMode = 18;//����д0ʱ��
				}
				DS18B20Data.CntTwo++;
				DS18B20Data.Buffer = DS18B20Data.Buffer>>1;
			}
			else DS18B20Data.WorkMode = 0;//һ���ֽ����ݷ������
			break;
		}
		case 15:
		{//����д1������10us
			DS18B20_TX(0);
			DS18B20_DelayTime(10);
			DS18B20Data.WorkMode = 16;
			break;
		}
		case 16:
		{//д��1,����50us
			DS18B20_TX(1);
			DS18B20_DelayTime(40);
			DS18B20Data.WorkMode = 17;
			break;
		}
		case 17:
		{//д��1��ϣ�������һ����
			DS18B20_DelayTime(1);
			DS18B20Data.WorkMode = 12;
			break;
		}
		case 18:
		{//����д0�����߱��ֵ͵�ƽ60us
			DS18B20_TX(0);
			DS18B20_DelayTime(50);
			DS18B20Data.WorkMode = 19;
			break;
		}
		case 19:
		{//д��0��ϣ�������һ����
			DS18B20_TX(1);
			DS18B20_DelayTime(3);
			DS18B20Data.WorkMode = 12;
			break;
		}
		//------------------------------------------------------------
		//��һ���ֽ�����
		case 21:
		{//��һ���ֽ�ʱ���ʼ��������һ�����ݺ󽫷��ص�DS18B20Data.OldWorkMode����ģʽ
			DS18B20Data.CntTwo = 0;
			DS18B20Data.Buffer = 0;
			DS18B20_DelayTime(1);
			DS18B20Data.WorkMode = 22;
			break;
		}
		case 22:
		{//ѭ���ɼ�8bit����
			if(DS18B20Data.CntTwo < 8)
			{//����������4us������ȡ����
				DS18B20_TX(0);
				DS18B20Data.CntTwo++;
				DS18B20_DelayTime(4);
				DS18B20Data.WorkMode = 23;
			}
			else
			{//��ȡ��ϣ�������һ��
				DS18B20_IO_OUT();
				DS18B20_TX(1);
				DS18B20Data.WorkMode = 0;
			}
			break;
		}
		case 23:
		{//��ʱ4us��ʼ��ȡ����
			DS18B20_TX(1);
			DS18B20_IO_IN();
			DS18B20_DelayTime(6);
			DS18B20Data.WorkMode = 24;
			break;
		}
		case 24:
		{//�ɼ�����
			u8 bit=0;
			if(DS18B20_RX() == RESET) bit = 0;
			else bit = 1;
			DS18B20Data.Buffer = DS18B20Data.Buffer>>1;
			DS18B20Data.Buffer += (bit<<7);
			DS18B20_DelayTime(40);
			DS18B20Data.WorkMode = 22;
			break;
		}
	}
}
//---
/*
	��������void DS18B20_DataInit(void)
	������	DS18B20.c���ݳ�ʼ��
*/
void DS18B20_DataInit(void)
{
	DS18B20Data.RunMode = 1;
	DS18B20Data.Tem = DS18B20_ColFAULT;
}
//---
/*
	��������void DS18B20_Init(void)
	������	DS18B20.c�ļ���ʼ������
*/
void DS18B20_Init(void)
{
	DS18B20_GpioInit();
	DS18B20_TX(1);
	DS18B20_TimerInit();
	DS18B20_DataInit();
}
//---
/*
	��������void DS18B20_Timer(void)
	������DS18B20.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void DS18B20_Timer(void)
{
	if(DS18B20Data.Timer != 0)DS18B20Data.Timer--;
}
//---
/*
	��������void DS18B20_TimerInit(void)
	������	�ú���Ϊ��DS18B20��ʱ�����г�ʼ��
	˵����	����ʱ����ʼ��Ϊ0.25us��ʹ�õ���ʱ��3
*/
void DS18B20_TimerInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 1000-1;				//����ֵ1000   
	TIM_TimeBaseStructure.TIM_Prescaler = 22-1;    		//Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���,84/21=4MHz.0.25us
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	//������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseStructure);
	TIM_ITConfig(TIM7,TIM_IT_Update, ENABLE); 
	
	NVIC_InitStructure.NVIC_IRQChannel= TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM7,ENABLE);
}
//---
/*
	ͨ�ö�ʱ��7�жϣ�����ϵͳ��ʱ��
*/
void TIM7_IRQHandler(void)
{
	if(TIM7->SR & 0X0001)	//����ж�
	{
		TIM7->CR1  &= ~B0;//�رն�ʱ��
		DS18B20_Work();
		TIM7->SR &= ~(1<<0);	//����жϱ�־λ
	}
}
//---
/*
	��������void DS18B20_DelayTime(u16 tim)
	������	�ú���Ϊ�������ö�ʱ����ʱֵ����������ʱ��
	���룺	u16 tim��ʱֵ����λus
*/
void DS18B20_DelayTime(u16 tim)
{
	TIM7->ARR = tim*4; 
	TIM7->CNT = 0;
	TIM7->CR1  |= B0;//������ʱ��
}
//---
/*
	��������void DS18B20_InitGPIO(void)
	������	��DS18B20�ܽ�GPIO��ʼ��
	˵����	��ʼ��Ϊ�������
*/
void DS18B20_GpioInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(DS18B20_DATA_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = DS18B20_DATA_PIN;			
  GPIO_Init(DS18B20_DATA_PORT, &GPIO_InitStructure);
	DS18B20_TX(1);
}
//---
/*
	��������void DS18B20_SetGPIO_In(void)
	��������DS18B20����IO�ڳ�ʼ��Ϊ���
*/
void DS18B20_SetGPIO_Out(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
//RCC_AHB1PeriphClockCmd(DS18B20_DATA_RCC, ENABLE)
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = DS18B20_DATA_PIN;			
  GPIO_Init(DS18B20_DATA_PORT, &GPIO_InitStructure);
}
//---
/*
	��������void DS18B20_SetGPIO_Out(void)
	��������DS18B20����IO�ڳ�ʼ��Ϊ����
*/
void DS18B20_SetGPIO_In(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
//  RCC_AHB1PeriphClockCmd(DS18B20_DATA_RCC, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = DS18B20_DATA_PIN;			
  GPIO_Init(DS18B20_DATA_PORT, &GPIO_InitStructure);
}
//---
/*
	��������DS18B20_DataStructure* DS18B20_GetData(void)
	��������ȡ����DS18B20.c�ļ�ȫ�ֱ����ṹ��
*/
DS18B20_DataStructure* DS18B20_GetData(void)
{
	return &DS18B20Data;
}
/*
	��������void DS18B20_Reset(void)	
	������	��λDS18B20
	˵����	��λ��ɺ� DS18B20Data.WorkMode = 0
					����λ�ɹ� DS18B20Data.CntOne = 0xFF
					����λʧ�� DS18B20Data.CntOne = 0x00
*/
void DS18B20_Reset(void)	   
{        
	DS18B20Data.WorkMode = 1;
	DS18B20_DelayTime(1);
}
//---
/*
	��������void DS18B20_SendData(u8 buffer)
	������	��DS18B20����һ���ֽ�����
*/
void DS18B20_SendData(u8 buffer)
{
	DS18B20Data.Buffer = buffer;
	DS18B20Data.WorkMode = 11;
	DS18B20_DelayTime(1);
}
//---
/*
	��������void DS18B20_ReceiveData(void)
	��������DS18B20����һ���ֽ�
	˵����������Ϻ����ݴ��ڻ�����DS18B20Data.Buffer
*/
void DS18B20_ReceiveData(void)
{
	DS18B20Data.WorkMode = 21;
	DS18B20_DelayTime(1);
}

//---
/*
	��������u8 DS18B20_CRC8(u8 *p,u16 len)
	������	DS18B20��CRCУ�麯����������CRC8У����
					����������һ���ֽ�ΪCRCУ���룬���У��ɹ����򷵻�0
*/
u8 DS18B20_CRC8(u8 *p, u16 len)
{ 
    u8 crc_data=0;
   	u16 i;
  	for(i=0;i<len;i++)  //���У��
    { 
    	crc_data = Crc8Table[crc_data^p[i]];
    }
    return (crc_data);
}
//---
/*
	��������float DS18B20_CalculateTem(u8 *p)
	������	�ú���Ϊ����ɼ������¶�
	���룺	���յ�DS18B20��9���ֽ��ݴ�������
	���أ�	������¶�ֵ
*/
float DS18B20_CalculateTem(u8 *p)
{
	u8 cnf=*(p+4) & 0x60;
	u16 Tem;
	float tem=0;
	Tem = (*(p+1)<<8) + *p;
	switch(cnf)
	{
		case 0x0:
		{//9λ�ֱ���
//			printf("9λ�ֱ���");
			tem = 0.5;
			break;
		}
		case 0x20:
		{//10λ�ֱ���
//			printf("10λ�ֱ���");
			tem = 0.25;
			break;
		}
		case 0x40:
		{//11λ�ֱ���
//			printf("11λ�ֱ���");
			tem = 0.125;
			break;
		}
		case 0x60:
		{//12λ�ֱ���
			tem = 0.0625;
//			printf("12λ�ֱ���");
			break;
		}
	}
	//�����¶�
	if(Tem & B15)
	{//���¶�
		Tem = 0xFFFF - Tem + 1;
		tem = Tem * tem * (-1);
	}
	else
	{//���¶�
		tem = Tem * tem;
	}
	return tem;
}
//---
//---
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void DS18B20_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪDS18B20�ļ���ASCII�����ļ�
ͨѶ��ʽ��:000|03|xxx
*/
void DS18B20_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:
		{//:000|03|001
			printf("��ǰ�¶�Ϊ��%f\r\n",DS18B20Data.Tem);
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









