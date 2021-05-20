/*
	本文件为该项目的屏幕相关函数
	具体函数如下
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
	函数名：void DS18B20_Main(void)
	描述：	DS18B20显示主函数
*/
void DS18B20_Main(void)
{
	switch(DS18B20Data.RunMode)
	{
		case 1:
		{//复位DDS18B20
			DS18B20_Reset();
			DS18B20Data.NewMode = 2;
			DS18B20Data.RunMode = 250;
			break;
		}
		case 2:
		{//判断DDS18B20是否存在
			if(DS18B20Data.CntOne == 0xFF)
			{
				DS18B20Data.RunMode = 3;//存在
//				printf("存在\r\n");
			}
			else 
			{//不存在
				if(DS18B20Data.ErrorCnt < 250) DS18B20Data.ErrorCnt++;
				else DS18B20Data.Tem = DS18B20_ColFAULT;//故障
				DS18B20Data.CntError++;
				DS18B20Data.Timer = 10;
				DS18B20Data.RunMode = 251;
				DS18B20Data.NewMode = 1;
//				printf("不存在\r\n");
			}
			break;
		}
		case 3:
		{//发送跳过ROM命令
			DS18B20_SendData(0xCC);
			DS18B20Data.NewMode = 4;
			DS18B20Data.RunMode = 250;
			break;
		}
		case 4:
		{//发送温度转换命令
			DS18B20_SendData(0x44);
			DS18B20Data.NewMode = 5;
			DS18B20Data.RunMode = 250;
			break;
		}
		case 5:
		{//复位DS18B20
			DS18B20_Reset();
			DS18B20Data.NewMode = 6;
			DS18B20Data.RunMode = 250;
			break;
		}
		case 6:
		{//发送跳过ROM命令
			DS18B20_SendData(0xCC);
			DS18B20Data.NewMode = 7;
			DS18B20Data.RunMode = 250;
			break;
		}
		case 7:
		{//发送读取暂存命令
			DS18B20_SendData(0xBE);
			DS18B20Data.CntOne = 0;
			DS18B20Data.NewMode = 8;
			DS18B20Data.RunMode = 250;
			break;
		}
		case 8:
		{//读数据
			DS18B20_ReceiveData();
			DS18B20Data.NewMode = 9;
			DS18B20Data.RunMode = 250;
			break;
		}
		case 9:
		{//连续读数据
			DS18B20Data.Data[DS18B20Data.CntOne++] = DS18B20Data.Buffer;
			if(DS18B20Data.CntOne<9)DS18B20Data.RunMode = 8;
			else DS18B20Data.RunMode = 10;
			break;
		}
		case 10:
		{//校验
			if(DS18B20_CRC8(DS18B20Data.Data,9) == 0)
			{//校验成功
				DS18B20Data.ErrorCnt = 0;
				DS18B20Data.Tem = DS18B20_CalculateTem(DS18B20Data.Data);
//				printf("当前温度为：%f\r\n",DS18B20Data.Tem);
			}
			else 
			{
				DS18B20Data.CntError++;
				if(DS18B20Data.ErrorCnt < 250) DS18B20Data.ErrorCnt++;
				else DS18B20Data.Tem = DS18B20_ColFAULT;//故障
//				printf("校验失败\r\n");
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
		{//等待命令完成
			if(DS18B20Data.WorkMode == 0)DS18B20Data.RunMode = DS18B20Data.NewMode;
			break;
		}
		case 251:
		{//等待延时完成
			if(DS18B20Data.Timer == 0)DS18B20Data.RunMode = DS18B20Data.NewMode;
			break;
		}
	}			
}
//---
/*
	函数名：void DS18B20_Work(void)
	描述：	该函数为DS18B20传感器的工作函数
	说明：	该函数由TIM3_IRQHandler定时器调用，定时器定时值为0.25us
*/
void DS18B20_Work(void)
{
	switch(DS18B20Data.WorkMode)
	{
		//复位DS18B20
		case 1:
		{//先拉低500us
			DS18B20_IO_OUT();
			DS18B20_TX(0);
			DS18B20Data.WorkMode = 2;
			DS18B20_DelayTime(500);//初始化，拉低500us
			break;
		}
		case 2:
		{//延时40us后检测管脚电平
			DS18B20_TX(1);
			DS18B20Data.WorkMode = 3;
			DS18B20_DelayTime(20);//延时40us检测接收线上的电平
			break;
		}
		case 3:
		{//初始化检测复位电平
			DS18B20_IO_IN();
			DS18B20Data.CntOne = 0;
			DS18B20Data.CntTwo = 0;
			DS18B20Data.WorkMode = 4;
			DS18B20_DelayTime(5);//延时10us检测接收线上的电平
			break;
		}
		case 4:
		{//检测复位电平
			if(DS18B20_RX() == 0) DS18B20Data.CntOne++;
			DS18B20Data.CntTwo++;
			if(DS18B20Data.CntTwo == 5) DS18B20Data.WorkMode = 5;
			DS18B20_DelayTime(5);//延时10us检测接收线上的电平
			break;
		}
		case 5:
		{//判断并结束复位电平
			if(DS18B20Data.CntOne>2) DS18B20Data.CntOne = 0xFF;//printf("收到DS18B20的回复\r\n");
			else DS18B20Data.CntOne = 0; //printf("没有收到DS18B20的回复\r\n");
			DS18B20_DelayTime(80);
			DS18B20Data.WorkMode = 6;
			break;
		}
		case 6:
		{//结束复位程序
			DS18B20_IO_OUT();
			DS18B20Data.WorkMode = 0;
			break;
		}
		//------------------------------------------------------------
		//210~219为向DS18B20写入一个字节数据
		case 11:
		{//写一个字节初始化
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
				{//写1
					DS18B20_DelayTime(1);
					DS18B20Data.WorkMode = 15;//进入写1时序
				}
				else
				{//写0
					DS18B20_DelayTime(1);
					DS18B20Data.WorkMode = 18;//进入写0时序
				}
				DS18B20Data.CntTwo++;
				DS18B20Data.Buffer = DS18B20Data.Buffer>>1;
			}
			else DS18B20Data.WorkMode = 0;//一个字节数据发送完毕
			break;
		}
		case 15:
		{//启动写1，拉低10us
			DS18B20_TX(0);
			DS18B20_DelayTime(10);
			DS18B20Data.WorkMode = 16;
			break;
		}
		case 16:
		{//写入1,保持50us
			DS18B20_TX(1);
			DS18B20_DelayTime(40);
			DS18B20Data.WorkMode = 17;
			break;
		}
		case 17:
		{//写入1完毕，返回上一操作
			DS18B20_DelayTime(1);
			DS18B20Data.WorkMode = 12;
			break;
		}
		case 18:
		{//启动写0，总线保持低电平60us
			DS18B20_TX(0);
			DS18B20_DelayTime(50);
			DS18B20Data.WorkMode = 19;
			break;
		}
		case 19:
		{//写入0完毕，返回上一操作
			DS18B20_TX(1);
			DS18B20_DelayTime(3);
			DS18B20Data.WorkMode = 12;
			break;
		}
		//------------------------------------------------------------
		//读一个字节数据
		case 21:
		{//读一个字节时序初始化，读完一个数据后将返回到DS18B20Data.OldWorkMode运行模式
			DS18B20Data.CntTwo = 0;
			DS18B20Data.Buffer = 0;
			DS18B20_DelayTime(1);
			DS18B20Data.WorkMode = 22;
			break;
		}
		case 22:
		{//循环采集8bit数据
			if(DS18B20Data.CntTwo < 8)
			{//以拉低总线4us启动读取程序
				DS18B20_TX(0);
				DS18B20Data.CntTwo++;
				DS18B20_DelayTime(4);
				DS18B20Data.WorkMode = 23;
			}
			else
			{//读取完毕，返回上一层
				DS18B20_IO_OUT();
				DS18B20_TX(1);
				DS18B20Data.WorkMode = 0;
			}
			break;
		}
		case 23:
		{//延时4us开始读取数据
			DS18B20_TX(1);
			DS18B20_IO_IN();
			DS18B20_DelayTime(6);
			DS18B20Data.WorkMode = 24;
			break;
		}
		case 24:
		{//采集数据
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
	函数名：void DS18B20_DataInit(void)
	描述：	DS18B20.c数据初始化
*/
void DS18B20_DataInit(void)
{
	DS18B20Data.RunMode = 1;
	DS18B20Data.Tem = DS18B20_ColFAULT;
}
//---
/*
	函数名：void DS18B20_Init(void)
	描述：	DS18B20.c文件初始化函数
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
	函数名：void DS18B20_Timer(void)
	描述：DS18B20.c文件定时器函数，由系统1ms定时器调用
*/
void DS18B20_Timer(void)
{
	if(DS18B20Data.Timer != 0)DS18B20Data.Timer--;
}
//---
/*
	函数名：void DS18B20_TimerInit(void)
	描述：	该函数为对DS18B20定时器进行初始化
	说明：	将定时器初始化为0.25us，使用到定时器3
*/
void DS18B20_TimerInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 1000-1;				//计数值1000   
	TIM_TimeBaseStructure.TIM_Prescaler = 22-1;    		//预分频,此值+1为分频的除数,84/21=4MHz.0.25us
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	//采样分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
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
	通用定时器7中断，整个系统定时器
*/
void TIM7_IRQHandler(void)
{
	if(TIM7->SR & 0X0001)	//溢出中断
	{
		TIM7->CR1  &= ~B0;//关闭定时器
		DS18B20_Work();
		TIM7->SR &= ~(1<<0);	//清除中断标志位
	}
}
//---
/*
	函数名：void DS18B20_DelayTime(u16 tim)
	描述：	该函数为用于设置定时器延时值，并启动定时器
	输入：	u16 tim延时值，单位us
*/
void DS18B20_DelayTime(u16 tim)
{
	TIM7->ARR = tim*4; 
	TIM7->CNT = 0;
	TIM7->CR1  |= B0;//启动定时器
}
//---
/*
	函数名：void DS18B20_InitGPIO(void)
	描述：	对DS18B20管脚GPIO初始化
	说明：	初始化为推挽输出
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
	函数名：void DS18B20_SetGPIO_In(void)
	描述：将DS18B20数据IO口初始化为输出
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
	函数名：void DS18B20_SetGPIO_Out(void)
	描述：将DS18B20数据IO口初始化为输入
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
	函数名：DS18B20_DataStructure* DS18B20_GetData(void)
	描述：获取返回DS18B20.c文件全局变量结构体
*/
DS18B20_DataStructure* DS18B20_GetData(void)
{
	return &DS18B20Data;
}
/*
	函数名：void DS18B20_Reset(void)	
	描述：	复位DS18B20
	说明：	复位完成后 DS18B20Data.WorkMode = 0
					若复位成功 DS18B20Data.CntOne = 0xFF
					若复位失败 DS18B20Data.CntOne = 0x00
*/
void DS18B20_Reset(void)	   
{        
	DS18B20Data.WorkMode = 1;
	DS18B20_DelayTime(1);
}
//---
/*
	函数名：void DS18B20_SendData(u8 buffer)
	描述：	向DS18B20发送一个字节数据
*/
void DS18B20_SendData(u8 buffer)
{
	DS18B20Data.Buffer = buffer;
	DS18B20Data.WorkMode = 11;
	DS18B20_DelayTime(1);
}
//---
/*
	函数名：void DS18B20_ReceiveData(void)
	描述：向DS18B20接收一个字节
	说明：接收完毕后，数据存在缓冲区DS18B20Data.Buffer
*/
void DS18B20_ReceiveData(void)
{
	DS18B20Data.WorkMode = 21;
	DS18B20_DelayTime(1);
}

//---
/*
	函数名：u8 DS18B20_CRC8(u8 *p,u16 len)
	描述：	DS18B20的CRC校验函数，及计算CRC8校验码
					若数组的最后一个字节为CRC校验码，如果校验成功，则返回0
*/
u8 DS18B20_CRC8(u8 *p, u16 len)
{ 
    u8 crc_data=0;
   	u16 i;
  	for(i=0;i<len;i++)  //查表校验
    { 
    	crc_data = Crc8Table[crc_data^p[i]];
    }
    return (crc_data);
}
//---
/*
	函数名：float DS18B20_CalculateTem(u8 *p)
	描述：	该函数为计算采集到的温度
	输入：	接收到DS18B20的9个字节暂存器数据
	返回：	计算的温度值
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
		{//9位分辨率
//			printf("9位分辨率");
			tem = 0.5;
			break;
		}
		case 0x20:
		{//10位分辨率
//			printf("10位分辨率");
			tem = 0.25;
			break;
		}
		case 0x40:
		{//11位分辨率
//			printf("11位分辨率");
			tem = 0.125;
			break;
		}
		case 0x60:
		{//12位分辨率
			tem = 0.0625;
//			printf("12位分辨率");
			break;
		}
	}
	//计算温度
	if(Tem & B15)
	{//负温度
		Tem = 0xFFFF - Tem + 1;
		tem = Tem * tem * (-1);
	}
	else
	{//正温度
		tem = Tem * tem;
	}
	return tem;
}
//---
//---
//-------------------------调试函数----------------------------
//---
/*
	函数名： void DS18B20_DebugASCII(u8 *p,u8 len)
	描述：		该函数为DS18B20文件的ASCII调试文件
通讯格式：:000|03|xxx
*/
void DS18B20_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:
		{//:000|03|001
			printf("当前温度为：%f\r\n",DS18B20Data.Tem);
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









