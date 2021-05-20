/*
	本文件为该项目的屏幕相关函数
	具体函数如下
*/

#include "S18B20.h"

S18B20_DataStructure S18B20Data;
/*
	函数名：void S18B20_Main(void)
	描述：	屏幕显示主函数
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
	printf("当前温度为:%d.%dC\r\n",temp/10, temp%10);
	
	
//	S18B20Data.WorkMode = 1;
//	S18B20_DelayTime(500);//初始化，拉低500us
	Delay_s(2);
}
//---
/*
	函数名：void DS18B20_Work(void)
	描述：	该函数为DS18B20传感器的工作函数
	说明：	该函数由DS18B20_Timer定时器调用，定时器定时值为0.25us
*/
void S18B20_Work(void)
{
	switch(S18B20Data.WorkMode)
	{
		case 1:
		{//先拉低500us
			S18B20_IO_OUT();
			S18B20_TX(0);
			S18B20Data.WorkMode = 2;
			S18B20_DelayTime(500);//初始化，拉低500us
			break;
		}
		case 2:
		{//延时40us后检测管脚电平
			S18B20_TX(1);
			S18B20Data.WorkMode = 3;
			S18B20_DelayTime(30);//延时40us检测接收线上的电平
			break;
		}
		case 3:
		{
			S18B20_IO_IN();
			S18B20Data.CntOne = 0;
			S18B20Data.CntTwo = 0;
			S18B20Data.WorkMode = 4;
			S18B20_DelayTime(10);//延时10us检测接收线上的电平
			break;
		}
		case 4:
		{
			if(S18B20_RX() == 0) S18B20Data.CntOne++;
			S18B20Data.CntTwo++;
			if(S18B20Data.CntTwo == 5) S18B20Data.WorkMode = 5;
			S18B20_DelayTime(10);//延时10us检测接收线上的电平
			break;
		}
		case 5:
		{
			if(S18B20Data.CntOne>2)
			{
				printf("收到DS18B20的回复\r\n");
			}
			else printf("没有收到DS18B20的回复\r\n");
			S18B20Data.WorkMode = 0;
			break;
		}
		case 11:
		{//发送Command命令
			break;
		}
	}
}
//---
/*
	函数名：void S18B20_Init(void)
	描述：	S18B20.c文件初始化函数
*/
void S18B20_Init(void)
{
//	S18B20_TimerInit();
//	S18B20_InitGPIO();
	
	SZ_STM32_SysTickInit(1000000);
	while(DS18B20_Init())//初始化DS18B20,兼检测18B20
	{
			printf("\n\r请检查DS18B20温度传感器是否安装好...");
		Delay_s(1);		  
	}
	printf("\n\rDS18B20温度传感器初始化成功\n\r");
}
//---
/*
	函数名：void S18B20_Timer(void)
	描述：S18B20.c文件定时器函数，由系统1ms定时器调用
*/
void S18B20_Timer(void)
{
	
}
//---
/*
	函数名：void DS18B20_TimerInit(void)
	描述：	该函数为对DS18B20定时器进行初始化
	说明：	将定时器初始化为0.25us，使用到定时器3
*/
void S18B20_TimerInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 1000-1;				//计数值1000   
	TIM_TimeBaseStructure.TIM_Prescaler = 18-1;    		//预分频,此值+1为分频的除数,72/18=4MHz.0.25us
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	//采样分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	//将定时器的中断等级设为最低
	NVIC_InitStructure.NVIC_IRQChannel= TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE); 
}

//---
/*
	函数名：void DS18B20_DelayTime(u16 tim)
	描述：	该函数为用于设置定时器延时值，并启动定时器
	输入：	u16 tim延时值，单位us
*/
void S18B20_DelayTime(u16 tim)
{
	TIM3->ARR = tim*4; 
	TIM3->CNT = 0;
	TIM3->CR1  |= B0;//启动定时器
}
//---
/*
	函数名：void S18B20_InitGPIO(void)
	描述：	初始化DS18B20总线IO口
*/
void S18B20_InitGPIO(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(S18B20_RCC_GPIO, ENABLE);
  GPIO_InitStructure.GPIO_Pin = S18B20_GPIO_PIN;				//PORTG.0 推挽输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(S18B20_GPIO_PORT, &GPIO_InitStructure);
	S18B20_TX(1);
}
/*
	通用定时器3中断，整个系统定时器
*/
void TIM3_IRQHandler(void)
{
	if(TIM3->SR & 0X0001)	//溢出中断
	{
		TIM3->CR1  &= ~B0;//关闭定时器
		S18B20_Work();
		TIM3->SR &= ~(1<<0);	//清除中断标志位
	}
}
//---
/*
	函数名：S18B20_DataStructure* S18B20_GetData(void)
	描述：获取返回S18B20.c文件全局变量结构体
*/
S18B20_DataStructure* S18B20_GetData(void)
{
	return &S18B20Data;
}
//-------------------------调试函数----------------------------
//---
/*
	函数名： void S18B20_DebugASCII(u8 *p,u8 len)
	描述：		该函数为S18B20文件的ASCII调试文件
*/
void S18B20_DebugASCII(u8 *p,u8 len)
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
//---
/*
	函数名： 	void S18B20_DebugRTU((u8 *p)
	描述：		该函数为S18B20文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*/
void S18B20_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//查看是哪个功能码
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

//复位DS18B20  复位脉冲（最短为480uS的低电平信号）
void DS18B20_Reset(void)	   
{        
	DS18B20_IO_OUT(); //SET AS OUTPUT
	DS18B20_OUT_LOW;      //拉低DQ
    Delay_us(750);        //拉低750us
    DS18B20_OUT_HIGH;	   
	Delay_us(15);       // 15US
}

//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
u8 DS18B20_Check(void) 	   
{   
	u8 retry=0;
    
	DS18B20_IO_IN();    // 总线主机便释放此线并进入接收方式（Rx）。
	/*DSl820 等待15.60uS 并且接着发送存在脉冲（60-240uS的低电平信号。）*/
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

//初始化DS18B20的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在    	 
u8 DS18B20_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//PORTG.0 推挽输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA,GPIO_Pin_0);    //输出1

  DS18B20_Reset();
  return DS18B20_Check();
}  

//从DS18B20读取一个位
//返回值：1/0
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

//从DS18B20读取一个字节
//返回值：读到的数据
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

//写一个字节到DS18B20
//dat：要写入的字节
/******************************* 写DS18B20函数 ******************************************/
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

//从ds18b20得到温度值
//精度：0.1C
//返回值：温度值 （-550~1250） 
short DS18B20_Get_Temp(void)
{
    u8 i, temp;
    u8 TL,TH;
	short tem;
    u8 str[9];
    
    //开始温度转换
  DS18B20_Reset();	   
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);// skip rom
	DS18B20_Write_Byte(0x44);// convert

    //开始读取温度
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
        temp=0;//温度为负  
    }
    else
    {
        temp=1;//温度为正         
    }
    tem=TH<<8 | TL; //获得不带符号位的11位温度值

    //转换 *0.625
    tem = tem*10;
    tem = tem>>4;

	if(temp)return tem; //返回温度值
	else return -tem;    
} 

/************************************************************
*Function:CRC校验
*parameter:
*Return:
*Modify:
*************************************************************/
u8 GetCRC(u8 *str, u32 length)
{ 
    u8 crc_data=0;
   	u32 i;
  	for(i=0;i<length;i++)  //查表校验
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
    
    crcdata = GetCRC(serial,8); // 为0则校验正确

    return (crcdata);
}

//--------------------------------------------------------------------
static __IO u32 TimingDelay;
/**-------------------------------------------------------
  * @函数名 SZ_STM32_SysTickInit
  * @功能   初始化系统定时器SysTick
  *         用户可以根据需要修改
  * @参数   每秒中断次数
  * @返回值 无
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
 * 函数名：TimingDelay_Decrement
 * 描述  ：获取节拍程序
 * 输入  ：无
 * 输出  ：无
 * 调用  ：在 SysTick 中断函数 SysTick_Handler()调用
 */  
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

/*
 * 函数名：Delay_ms
 * 描述  ：ms延时程序,1ms为一个单位
 * 输入  ：- nTime
 * 输出  ：无
 * 调用  ：Delay_us( 1 ) 则实现的延时为 1 * 1ms = 1ms
 *       ：外部调用 
 */

void Delay_us(__IO u32 nTime)
{ 
	TimingDelay = nTime;
		
	// 使能滴答定时器  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;	

	while(TimingDelay != 0);

	// 关闭滴答定时器  
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
































