/*
	���ļ�Ϊ����Ŀ����Ļ��غ���
	���庯������
*/

#include "S18B20.h"

S18B20_DataStructure S18B20Data;
/*
	��������void S18B20_Main(void)
	������	��Ļ��ʾ������
*/
void S18B20_Main(void)
{
	short temp;
	temp=DS18B20_Get_Temp();
	if(temp<0)
	{
		temp=-temp;
		printf("-");
	}						 
	printf("��ǰ�¶�Ϊ:%d.%dC\r\n",temp/10, temp%10);
	
	
//	S18B20Data.WorkMode = 1;
//	S18B20_DelayTime(500);//��ʼ��������500us
	Delay_s(2);
}
//---
/*
	��������void DS18B20_Work(void)
	������	�ú���ΪDS18B20�������Ĺ�������
	˵����	�ú�����DS18B20_Timer��ʱ�����ã���ʱ����ʱֵΪ0.25us
*/
void S18B20_Work(void)
{
	switch(S18B20Data.WorkMode)
	{
		case 1:
		{//������500us
			S18B20_IO_OUT();
			S18B20_TX(0);
			S18B20Data.WorkMode = 2;
			S18B20_DelayTime(500);//��ʼ��������500us
			break;
		}
		case 2:
		{//��ʱ40us����ܽŵ�ƽ
			S18B20_TX(1);
			S18B20Data.WorkMode = 3;
			S18B20_DelayTime(30);//��ʱ40us���������ϵĵ�ƽ
			break;
		}
		case 3:
		{
			S18B20_IO_IN();
			S18B20Data.CntOne = 0;
			S18B20Data.CntTwo = 0;
			S18B20Data.WorkMode = 4;
			S18B20_DelayTime(10);//��ʱ10us���������ϵĵ�ƽ
			break;
		}
		case 4:
		{
			if(S18B20_RX() == 0) S18B20Data.CntOne++;
			S18B20Data.CntTwo++;
			if(S18B20Data.CntTwo == 5) S18B20Data.WorkMode = 5;
			S18B20_DelayTime(10);//��ʱ10us���������ϵĵ�ƽ
			break;
		}
		case 5:
		{
			if(S18B20Data.CntOne>2)
			{
				printf("�յ�DS18B20�Ļظ�\r\n");
			}
			else printf("û���յ�DS18B20�Ļظ�\r\n");
			S18B20Data.WorkMode = 0;
			break;
		}
		case 11:
		{//����Command����
			break;
		}
	}
}
//---
/*
	��������void S18B20_Init(void)
	������	S18B20.c�ļ���ʼ������
*/
void S18B20_Init(void)
{
//	S18B20_TimerInit();
//	S18B20_InitGPIO();
	
	SZ_STM32_SysTickInit(1000000);
	while(DS18B20_Init())//��ʼ��DS18B20,����18B20
	{
			printf("\n\r����DS18B20�¶ȴ������Ƿ�װ��...");
		Delay_s(1);		  
	}
	printf("\n\rDS18B20�¶ȴ�������ʼ���ɹ�\n\r");
}
//---
/*
	��������void S18B20_Timer(void)
	������S18B20.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void S18B20_Timer(void)
{
	
}
//---
/*
	��������void DS18B20_TimerInit(void)
	������	�ú���Ϊ��DS18B20��ʱ�����г�ʼ��
	˵����	����ʱ����ʼ��Ϊ0.25us��ʹ�õ���ʱ��3
*/
void S18B20_TimerInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 1000-1;				//����ֵ1000   
	TIM_TimeBaseStructure.TIM_Prescaler = 18-1;    		//Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���,72/18=4MHz.0.25us
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	//������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//����ʱ�����жϵȼ���Ϊ���
	NVIC_InitStructure.NVIC_IRQChannel= TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE); 
}

//---
/*
	��������void DS18B20_DelayTime(u16 tim)
	������	�ú���Ϊ�������ö�ʱ����ʱֵ����������ʱ��
	���룺	u16 tim��ʱֵ����λus
*/
void S18B20_DelayTime(u16 tim)
{
	TIM3->ARR = tim*4; 
	TIM3->CNT = 0;
	TIM3->CR1  |= B0;//������ʱ��
}
//---
/*
	��������void S18B20_InitGPIO(void)
	������	��ʼ��DS18B20����IO��
*/
void S18B20_InitGPIO(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(S18B20_RCC_GPIO, ENABLE);
  GPIO_InitStructure.GPIO_Pin = S18B20_GPIO_PIN;				//PORTG.0 �������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(S18B20_GPIO_PORT, &GPIO_InitStructure);
	S18B20_TX(1);
}
/*
	ͨ�ö�ʱ��3�жϣ�����ϵͳ��ʱ��
*/
void TIM3_IRQHandler(void)
{
	if(TIM3->SR & 0X0001)	//����ж�
	{
		TIM3->CR1  &= ~B0;//�رն�ʱ��
		S18B20_Work();
		TIM3->SR &= ~(1<<0);	//����жϱ�־λ
	}
}
//---
/*
	��������S18B20_DataStructure* S18B20_GetData(void)
	��������ȡ����S18B20.c�ļ�ȫ�ֱ����ṹ��
*/
S18B20_DataStructure* S18B20_GetData(void)
{
	return &S18B20Data;
}
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void S18B20_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪS18B20�ļ���ASCII�����ļ�
*/
void S18B20_DebugASCII(u8 *p,u8 len)
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
//---
/*
	�������� 	void S18B20_DebugRTU((u8 *p)
	������		�ú���ΪS18B20�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*/
void S18B20_DebugRTU(u8 *p,u8 len)
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
}
//---


//--------------------------------------------------------------------
//CRC = X8 + X5 + X4 + 1
const u8 Crc8Table [256]={
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

/*******************************************************************************
* Function Name  : SysTick_Configuration
* Description    : Configure a SysTick Base time to 1 us.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

//��λDS18B20  ��λ���壨���Ϊ480uS�ĵ͵�ƽ�źţ�
void DS18B20_Reset(void)	   
{        
	DS18B20_IO_OUT(); //SET AS OUTPUT
	DS18B20_OUT_LOW;      //����DQ
    Delay_us(750);        //����750us
    DS18B20_OUT_HIGH;	   
	Delay_us(15);       // 15US
}

//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
    
	DS18B20_IO_IN();    // �����������ͷŴ��߲�������շ�ʽ��Rx����
	/*DSl820 �ȴ�15.60uS ���ҽ��ŷ��ʹ������壨60-240uS�ĵ͵�ƽ�źš���*/
  while ((DS18B20_DATA_IN == 1) && (retry<200))
	{
		retry++;
		Delay_us(1);
	};	 
    
	if(retry>=200)
        return 1;
	else retry=0;

  while ((DS18B20_DATA_IN == 0) && (retry<240))
	{
		retry++;
		Delay_us(1);
	};
	if(retry>=240)
     return 1;    
	return 0;
}

//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
u8 DS18B20_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//PORTG.0 �������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA,GPIO_Pin_0);    //���1

  DS18B20_Reset();
  return DS18B20_Check();
}  

//��DS18B20��ȡһ��λ
//����ֵ��1/0
u8 DS18B20_Read_Bit(void) 			 // read one bit
{
    u8 data;
    
	DS18B20_IO_OUT();   //SET AS OUTPUT
    DS18B20_OUT_LOW; 
	Delay_us(3);
    DS18B20_OUT_HIGH; 
	DS18B20_IO_IN();    //SET AS INPUT
	Delay_us(6);
	if(DS18B20_DATA_IN)
    {
        data=1;
    }
    else
    {
        data=0;  
    }
    Delay_us(50); 
    
    return data;
}

//��DS18B20��ȡһ���ֽ�
//����ֵ������������
u8 DS18B20_Read_Byte(void)    // read one byte
{        
    u8 i,j,dat;
    dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=dat>>1;
        dat+=(j<<7);
    }	

    return dat;
}

//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
/******************************* дDS18B20���� ******************************************/
void DS18B20_Write_Byte(u8 dat)     
{             
    u8 j;
    u8 testb;
	DS18B20_IO_OUT();//SET AS OUTPUT;
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;

        DS18B20_OUT_LOW;// Write 1
        Delay_us(2);  
        if (testb) 
        {
            DS18B20_OUT_HIGH;
        }
        Delay_us(55);
        DS18B20_OUT_HIGH;
        Delay_us(5);                          
    }
}

//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
short DS18B20_Get_Temp(void)
{
    u8 i, temp;
    u8 TL,TH;
	short tem;
    u8 str[9];
    
    //��ʼ�¶�ת��
  DS18B20_Reset();	   
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);// skip rom
	DS18B20_Write_Byte(0x44);// convert

    //��ʼ��ȡ�¶�
    DS18B20_Reset();
	DS18B20_Check();
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0xbe);// convert	

    for (i=0;i<9;i++) 
	{    
	    str[i] = DS18B20_Read_Byte();
    }
//    if(GetCRC(str, 9) == 0)
//        printf(" CRC OK  ");
//    else
//        printf(" CRC ERR ");
    
    TL = str[0]; // LSB   
    TH = str[1]; // MSB   
	   
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//�¶�Ϊ��  
    }
    else
    {
        temp=1;//�¶�Ϊ��         
    }
    tem=TH<<8 | TL; //��ò�������λ��11λ�¶�ֵ

    //ת�� *0.625
    tem = tem*10;
    tem = tem>>4;

	if(temp)return tem; //�����¶�ֵ
	else return -tem;    
} 

/************************************************************
*Function:CRCУ��
*parameter:
*Return:
*Modify:
*************************************************************/
u8 GetCRC(u8 *str, u32 length)
{ 
    u8 crc_data=0;
   	u32 i;
  	for(i=0;i<length;i++)  //���У��
    { 
    	crc_data = Crc8Table[crc_data^str[i]];
    }
    return (crc_data);
}

u8 DS18B20_Get_Serial(u8 *serial)
{
    u8 i;
    u8 crcdata;    

    DS18B20_Reset();
	DS18B20_Check();

    DS18B20_Write_Byte(0x33);// read rom

    for(i=0;i<8;i++)
    {
       *(serial + i) = DS18B20_Read_Byte();
    }
    
    crcdata = GetCRC(serial,8); // Ϊ0��У����ȷ

    return (crcdata);
}

//--------------------------------------------------------------------
static __IO u32 TimingDelay;
/**-------------------------------------------------------
  * @������ SZ_STM32_SysTickInit
  * @����   ��ʼ��ϵͳ��ʱ��SysTick
  *         �û����Ը�����Ҫ�޸�
  * @����   ÿ���жϴ���
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_SysTickInit(uint32_t HzPreSecond)
{
    /* HzPreSecond = 1000 to Setup SysTick Timer for 1 msec interrupts.
     ------------------------------------------
    1. The SysTick_Config() function is a CMSIS function which configure:
       - The SysTick Reload register with value passed as function parameter.
       - Configure the SysTick IRQ priority to the lowest value (0x0F).
       - Reset the SysTick Counter register.
       - Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
       - Enable the SysTick Interrupt.
       - Start the SysTick Counter.
    
    2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
       SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
       SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
       inside the misc.c file.

    3. You can change the SysTick IRQ priority by calling the
       NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function 
       call. The NVIC_SetPriority() is defined inside the core_cm3.h file.

    4. To adjust the SysTick time base, use the following formula:
                            
         Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)
       - Reload Value is the parameter to be passed for SysTick_Config() function
       - Reload Value should not exceed 0xFFFFFF
    */
    if (SysTick_Config(SystemCoreClock / HzPreSecond))
    { 
        /* Capture error */ 
        while (1);
    }
}

/*
 * ��������TimingDelay_Decrement
 * ����  ����ȡ���ĳ���
 * ����  ����
 * ���  ����
 * ����  ���� SysTick �жϺ��� SysTick_Handler()����
 */  
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

/*
 * ��������Delay_ms
 * ����  ��ms��ʱ����,1msΪһ����λ
 * ����  ��- nTime
 * ���  ����
 * ����  ��Delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1 * 1ms = 1ms
 *       ���ⲿ���� 
 */

void Delay_us(__IO u32 nTime)
{ 
	TimingDelay = nTime;
		
	// ʹ�ܵδ�ʱ��  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;	

	while(TimingDelay != 0);

	// �رյδ�ʱ��  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}

void Delay_ms(__IO u32 nTime)
{ 
	Delay_us(nTime*1000);
}

void Delay_s(__IO u32 nTime)
{ 
	Delay_us(nTime*1000000);
}

//--------------------------------------------------------------------
































