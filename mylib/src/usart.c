/*
	本文件为串口相关函数文件
	编写与2014年4月19日
*/

#include "usart.h"

#define OPEN_USART1_COM
#define OPEN_USART2_COM	  //主从通讯
#define OPEN_USART3_COM			
#define OPEN_UART4_COM   //蓝牙打印
#define OPEN_UART5_COM
//#define OPEN_USART6_COM			//与上位机通讯
//#define OPEN_USART7_COM			//与从板通讯
//#define OPEN_USART8_COM
/*
	函数名： USART_InitAll()
	描述：	 初始化串口文件USART
*/
extern u8 AnalysisCRC(u8 *pstr);
void USART_InitAll(void)
{
	#ifdef OPEN_USART1_COM
	USART1_Configuration();		
  #endif 
#ifdef OPEN_USART2_COM
	USART2_Configuration();		
  #endif 
#ifdef OPEN_USART3_COM
	USART3_Configuration();		
  #endif 
#ifdef OPEN_UART4_COM
	UART4_Configuration();		
  #endif 	
#ifdef OPEN_UART5_COM
	UART5_Configuration();		
  #endif 	
	
#ifdef OPEN_USART6_COM
	USART6_Configuration();	
#endif 
}
//---
/*
	函数名：	void USART_main(void)
	描述：		该函数为usart.c文件的主函数，被系统主函数调用
	时间：		20150804
*/
void USART_main(void)
{
	#ifdef OPEN_USART1_COM
	USARTx_DW(USART1,Get_USART1Buffer());
	#endif 
//	#ifdef OPEN_USART2_COM
//	USARTx_DW(USART2,Get_USART2Buffer());
//	#endif 
//	#ifdef OPEN_USART3_COM
//	USARTx_DW(USART3,Get_USART3Buffer());
//	#endif 
	#ifdef OPEN_USART6_COM
  USARTx_DW(USART6,Get_USART6Buffer());
	#endif 
	
	
	
}
//---
/***************************
	函数名： void USART3_Timer(USART_TypeDef* USARTx,USART_DataStructure *p)
	描述：	 该函数为该文件(串口文件)定时器，由USART_Timer()总调用
	调用函数：
				USART3_Timer(); //串口3定时器
**************************************************************/
void USART2or3_Timer(USART_TypeDef* USARTx,USART_DataStructure *p)
 {
	if(p->Timer>0)
	{ 
		p->Timer--;  //计算超时
		if(p->Timer==0)
		{
			USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);		//关中断
			p->RxFlag=0;     
      		//重新开始接收
			USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);		//开中断
		}
	}
	else
	{
		p->RxFlag=0;
	}
 }
//----
/*
	函数名： void USART_Timer(void)
	描述：	 该函数为该文件(串口文件)定时器，1ms，由1ms定时器常调用
	调用函数：
				USART3_Timer(); //串口3定时器
*/
void USART_Timer(void)
{
		#ifdef OPEN_USART1_COM
		USARTx_Timer(USART1,Get_USART1Buffer());
	#endif 
	
	#ifdef OPEN_USART2_COM
	 USART2or3_Timer(USART2,Get_USART2Buffer());	
	#endif 
	 #ifdef OPEN_USART3_COM
	 USART2or3_Timer(USART3,Get_USART3Buffer());
  	#endif

}
//-----------------------------------------------------------------------------------
//串口1相关
#ifdef OPEN_USART1_COM
//---
/*
	函数名：void USART1_Configuration(void)
	描述：	串口1初始化
*/
void USART1_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//使能USART1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	//USART1端口配置
	//配置USART1 TX(PA.09)为复用功能推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置USART1 RX(PA.10)为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置USART1串行口
	USART_InitStructure.USART_BaudRate = 115200;// 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	//使能发送中断，该中断产生时的USART1发送数据寄存器为空
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	//使能USART1全局中断
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//使能接收中断，该中断产生时，USART1接收数据寄存器不为空
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//使能USART1
	USART_Cmd(USART1, ENABLE);
}
//---
USART_DataStructure USART1_Buffer;		//串口2结构体
/*
	函数名：USART_DataStructure *Get_USART1Buffer(void)
	描述：	获取串口1结构体数据
*/
USART_DataStructure *Get_USART1Buffer(void)
{
	return &USART1_Buffer;
}
//---
/*
	函数名： void USART1_IRQHandler(void)
	描述：	 串口1中断服务程序,串口3产生中断时，进入该函数

*/

#ifdef	OPEN_PRINTF
void USART1_IRQHandler(void)                	//串口3中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USARTx_Receive(USART1,&USART1_Buffer,USART1->DR);
	} 
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USARTx_Send(USART1,&USART1_Buffer);
	}
} 
#endif

//---
//函数名： USART1_SendByte(u8 TxData)

//	描述：USART1口发送一字节数据	

void USART1_SendByte(u8 TxData)
{
	while((USART1->SR & 0X40)==0);												//
	USART1->DR = (u8) TxData;  
}


#endif


//-----------------------------------------------------------------------------------
//串口2
#ifdef OPEN_USART2_COM
//---
/*
	函数名：void USART1_Configuration(void)
	描述：	串口1初始化
*/
void USART2_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//使能USART1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	//USART1端口配置
	//配置USART2 TX(PA.02)为复用功能推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置USART2 RX(PA.3)为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置USART2串行口
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	//使能发送中断，该中断产生时的USART2发送数据寄存器为空
//	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	//使能接收中断，该中断产生时，USART2接收数据寄存器不为空
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//使能USART12
	USART_Cmd(USART2, ENABLE);
	//使能USART2全局中断
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//---
USART_DataStructure USART2_Buffer;		//串口2结构体
/*
	函数名：USART_DataStructure *Get_UASRT2Buffer(void)
	描述：	获取串口2结构体数据
*/
USART_DataStructure *Get_USART2Buffer(void)
{
	return &USART2_Buffer;
}
//---
/*
	函数名： void USART2_IRQHandler(void)
	描述：	 串口2中断服务程序,串口3产生中断时，进入该函数
*/
void USART2_IRQHandler(void)                	//串口3中断服务程序
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		//这里编写中断协议 :0XAA,长度,数据效验
		Res =USART_ReceiveData(USART2);			//中断先取出数据
	                                      //	USART3_Buffer.RxBuffer[USART3_Buffer.RxCount] = Res;                   //
		if(USART2_Buffer.RxFlag==0)         //当前接收的状态为等待接收0X5A
		{
			 if(Res==0xAA)
			 {
			 USART2_Buffer.RxFlag=1;
			 //启动帧定时器 30ms
			 USART2_Buffer.Timer=30;
				 
			 }
		}
		else if(USART2_Buffer.RxFlag==1)
		{
			if(Res<94)  //限制接收字节数最大值
			{
				USART2_Buffer.RxCount=Res;   //保存接收字节数
				USART2_Buffer.RxFlag=2;
				USART2_Buffer.i=0;
				USART2_Buffer.RxBuffer[USART2_Buffer.i]=Res; //第一字节保存下面所接收的字节数
				USART2_Buffer.i=1;
			}
			else
			{
				USART2_Buffer.RxFlag=0;   //接收字节超出范围，有误
			}
		}
		else if (USART2_Buffer.RxFlag==2)
		{
			USART2_Buffer.RxBuffer[USART2_Buffer.i]=Res;
			USART2_Buffer.i++;
			if(USART2_Buffer.i>USART2_Buffer.RxCount)  //接收完处理
			{
			 //	if(USART1_Buffer.RxBuffer[1]==0x01)  //
				{
					for(Res=0;Res<=USART2_Buffer.RxCount;Res++)
					 {
						 USART2_Buffer.ProgressBuffer1[Res]= USART2_Buffer.RxBuffer[Res];
						 USART2_Buffer.BufFlg1=1;   //第一缓冲区有数据
					 }
				}

				USART2_Buffer.Timer=0;  //清时间
			}
		}
		else
		{
		USART2_Buffer.RxFlag=0;	
		}

	} 
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)  //发送中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USARTx_Send(USART2,&USART2_Buffer);
	}
} 
//---
#endif
//-----------------------------------------------------------------------------------
//串口3
#ifdef OPEN_USART3_COM
//---
/*
	函数名： USART3_Init()
	描述：	 串口3初始化函数
	输入：	 无
	输出：	 无
	返回：	 无
*/
void USART3_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
//	RCC_AHB1PeriphClockCmd(RS485DIR_APP, ENABLE);	//使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	//使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3时钟
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	//USART1端口配置
	//配置USART3 TX(PB.10)为复用功能推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//配置USART3 RX(PB.11)为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//配置USART1串行口
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	//使能发送中断，该中断产生时的USART1发送数据寄存器为空
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	//使能接收中断，该中断产生时，USART1接收数据寄存器不为空
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//	USART_ITConfig(USART3, USART_IT_TC, ENABLE);		//允许数据发送完毕产生中断
	//使能USART1
	USART_Cmd(USART3, ENABLE);
	//使能USART3中断
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//---
USART_DataStructure USART3_Buffer;		//串口3结构体变量
//USART_DataStructure USART11_Buffer;		//串口3结构体变量
/*
	函数名：USART_DataStructure *Get_UASRT3Buffer(void)
	描述：	获取串口3结构体数据
*/
USART_DataStructure *Get_USART3Buffer(void)
{
	return &USART3_Buffer;
}
//---
/*
	函数名： void USART3_IRQHandler(void)
	描述：	 串口3中断服务程序,串口3产生中断时，进入该函数
*/
void USART3_IRQHandler(void)                	//串口3中断服务程序
{  u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		//这里编写中断协议
		Res =USART_ReceiveData(USART3);			//中断先取出数据
	                                      //	USART3_Buffer.RxBuffer[USART3_Buffer.RxCount] = Res;                   //
		if(USART3_Buffer.RxFlag==0)         //当前接收的状态为等待接收0X5A
		{
			 if(Res==0x5a)
			 {
			 USART3_Buffer.RxFlag=1;
			 //启动帧定时器 30ms
			 USART3_Buffer.Timer=30;
				 
			 }
		}
		else if(USART3_Buffer.RxFlag==1)    //当前接收的状态为等待接收0XA5
		{
			if(Res==0xa5)
			{
			 USART3_Buffer.RxFlag=2;
			}
			else
			{
			 USART3_Buffer.RxFlag=0;   //第二字节不符，重头开始接收
			}
		}
		else if(USART3_Buffer.RxFlag==2)
		{
			if(Res<50)
			{
				USART3_Buffer.RxCount=Res;   //保存接收字节数
				USART3_Buffer.RxFlag=3;
				USART3_Buffer.i=0;
				USART3_Buffer.RxBuffer[USART3_Buffer.i]=Res; //第一字节保存下面所接收的字节数
				USART3_Buffer.i=1;
			}
			else
			{
				USART3_Buffer.RxFlag=0;   //接收字节超出范围，有误
			}
		}
		else if (USART3_Buffer.RxFlag==3)
		{
			USART3_Buffer.RxBuffer[USART3_Buffer.i]=Res;
			USART3_Buffer.i++;
			if(USART3_Buffer.i>USART3_Buffer.RxCount)  //接收完处理
			{
				if(USART3_Buffer.RxBuffer[1]==0x81)  //接收是0x81指令
				{
					for(Res=0;Res<=USART3_Buffer.RxCount;Res++)
					 {
						 USART3_Buffer.ProgressBuffer1[Res]= USART3_Buffer.RxBuffer[Res];
						 USART3_Buffer.BufFlg1=1;   //第一缓冲区有数据
//						 DEBUG_MESSAGE("0x%X,	",USART3_Buffer.ProgressBuffer1[Res]);
					 }
//					 DEBUG_MESSAGE("\r\n");
				}
				else                                  //接收是0x83指令/或错误数据
				{
					for(Res=0;Res<=USART3_Buffer.RxCount;Res++)
					 {
						   USART3_Buffer.ProgressBuffer2[Res]= USART3_Buffer.RxBuffer[Res];
						   USART3_Buffer.BufFlg2=1; //第二缓冲区有数据
						 DEBUG_MESSAGE("0x%X,	",USART3_Buffer.ProgressBuffer2[Res]);
					 }
					 DEBUG_MESSAGE("\r\n");
				}
				USART3_Buffer.Timer=0;  //清时间
			}
			
		}
		else
		{
		USART3_Buffer.RxFlag=0;	
		}
		
	} 
	
	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USARTx_Send(USART3,&USART3_Buffer);
	}
} 


//---
#endif
//-----------------------------------------------------------------------------------
//串口4
#ifdef OPEN_UART4_COM	
//---
USART_Com4Structure USART_COM4_Data;

USART_Com4Structure *USART_COM4_GetData(void)
{
	return &USART_COM4_Data;
}
//---
/*
	函数名： void UART4_Configuration(void)
	描述：	 串口4初始化函数
	输入：	 无
	输出：	 无
	返回：	 无
*/
void UART4_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//使能USART4时钟
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
	//USART4端口配置
	//配置UART4 TX(PC.10)为复用功能推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//配置UART4 RX(PC.11)为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//配置UART4串行口
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);
	//使能发送中断，该中断产生时的USART1发送数据寄存器为空
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	//使能接收中断，该中断产生时，USART1接收数据寄存器不为空
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	//使能USART1
	USART_Cmd(UART4, ENABLE);
	//使能USART3中断
	NVIC_InitStructure.NVIC_IRQChannel=UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}




void USART_COM4_Enable(void)
{
	USART_COM4_Data.EnReceveFlg=1;
}
void USART_COM4_Disable(void)
{
	USART_COM4_Data.EnReceveFlg=0;
}

/*

ReceveMode:=0透传模式 1：AT 模式
ReceveMax:接收的个数
*/
void USART_COM4_Receve_com(u8 ReceveMode,u8 ReceveMax)
{
	USART_COM4_Data.ReceveMax=ReceveMax;      //接收个数
	USART_COM4_Data.ReceveMode=ReceveMode;    //0:透传模式下接收
	USART_COM4_Data.ReceveOkFlg=0;            //清接收完标志
	USART_COM4_Data.ReceveCnt=0;
	USART_COM4_Data.EnReceveFlg=1;
}
//---

//---
/*
	函数名： void UART4_IRQHandler(void)
	描述：	 串口4中断服务程序,串口3产生中断时，进入该函数
*/
void UART4_IRQHandler(void)                	//串口3中断服务程序
{
u8 Res;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(UART4);//(USART1->DR);	//读取接收到的数据
		if(USART_COM4_Data.EnReceveFlg)  //是启动接收
		{ if(USART_COM4_Data.ReceveMode)    //AT模式下接收
			{
			USART_COM4_Data.USART_RX_BUF[USART_COM4_Data.ReceveCnt]=Res;  //保存到缓冲区
		  USART_COM4_Data.ReceveCnt++;
			if(USART_COM4_Data.ReceveCnt>=USART_COM4_Data.ReceveMax)  
			{ //接收完毕
				USART_COM4_Data.EnReceveFlg=0; //关闭接收
				USART_COM4_Data.ReceveOkFlg=1;  //接收完成
			}
		  }
			else   //透传模式下接收
			{
			USART_COM4_Data.USART_RX_BUF[USART_COM4_Data.ReceveCnt]=Res;  //保存到缓冲区
		    USART_COM4_Data.ReceveCnt++;
			if(USART_COM4_Data.ReceveCnt>=USART_COM4_Data.ReceveMax)  
			{ //接收完毕
				USART_COM4_Data.EnReceveFlg=0;  //关闭接收
				USART_COM4_Data.ReceveOkFlg=1;  //接收完成
			}
			}
		}
	}
} 
//---
#endif
//-----------------------------------------------------------------------------------
//串口5
#ifdef OPEN_UART5_COM
//---
//-----------------------------------------------------------------------------------

//---
USART_Com4Structure USART_COM5_Data;

USART_Com4Structure *USART_COM5_GetData(void)
{
	return &USART_COM5_Data;
}
//---
/*
	函数名： void UART5_Configuration(void)
	描述：	 串口5初始化函数
	输入：	 无
	输出：	 无
	返回：	 无
*/
void UART5_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//使能GPIOC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	//使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//使能USART5时钟
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
	//USART5端口配置
	//配置UART5 TX(PC.12)为复用功能推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//配置UART5 RX(PD.2)为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//配置UART5串行口
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);
	//使能发送中断，该中断产生时的USART5发送数据寄存器为空
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	//使能接收中断，该中断产生时，USART5接收数据寄存器不为空
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	//使能USART5
	USART_Cmd(UART5, ENABLE);
	//使能USART5中断
	NVIC_InitStructure.NVIC_IRQChannel=UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void USART_COM5_Enable(void)
{
	USART_COM5_Data.EnReceveFlg=1;
}
void USART_COM5_Disable(void)
{
	USART_COM5_Data.EnReceveFlg=0;
}

/*

ReceveMode:=0透传模式 1：AT 模式
ReceveMax:接收的个数
*/
void USART_COM5_Receve_com(u8 ReceveMode,u8 ReceveMax)
{
	USART_COM5_Data.ReceveMax=ReceveMax;      //接收个数
	USART_COM5_Data.ReceveMode=ReceveMode;    //0:透传模式下接收
	USART_COM5_Data.ReceveOkFlg=0;            //清接收完标志
	USART_COM5_Data.ReceveCnt=0;
	USART_COM5_Data.EnReceveFlg=1;
}
//---

//---
/*
	函数名： void UART5_IRQHandler(void)
	描述：	 串口5中断服务程序,串口3产生中断时，进入该函数
*/
void UART5_IRQHandler(void)                	//串口5中断服务程序
{
u8 Res;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res =USART_ReceiveData(UART5);//(USART1->DR);	//读取接收到的数据
		if(USART_COM5_Data.EnReceveFlg)  //是启动接收
		{ if(USART_COM5_Data.ReceveMode)    //AT模式下接收
			{
			USART_COM5_Data.USART_RX_BUF[USART_COM5_Data.ReceveCnt]=Res;  //保存到缓冲区
		  USART_COM5_Data.ReceveCnt++;
			if(USART_COM5_Data.ReceveCnt>=USART_COM5_Data.ReceveMax)  
			{ //接收完毕
				USART_COM5_Data.EnReceveFlg=0; //关闭接收
				USART_COM5_Data.ReceveOkFlg=1;  //接收完成
			}
		  }
			else   //透传模式下接收
			{
			USART_COM5_Data.USART_RX_BUF[USART_COM5_Data.ReceveCnt]=Res;  //保存到缓冲区
		  USART_COM5_Data.ReceveCnt++;
			if(USART_COM5_Data.ReceveCnt>=USART_COM5_Data.ReceveMax)  
			{ //接收完毕
				USART_COM5_Data.EnReceveFlg=0;  //关闭接收
				USART_COM5_Data.ReceveOkFlg=1;  //接收完成
			}
			}
		}
	}
} 
#endif
//-----------------------------------------------------------------------------------
//串口6  MAX3485 IC
#ifdef OPEN_USART6_COM
//--
USART_DataStructure USART6_Buffer;		//串口7结构体全局变量
void USART6_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);	//使能USART4时钟
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
	//USART6端口配置
	//配置USART6 TX(PC.6)为复用功能推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//配置USART6 RX(PC.7)为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//配置USART6串行口
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART6, &USART_InitStructure);
	//使能发送中断，该中断产生时的USART1发送数据寄存器为空
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	//使能接收中断，该中断产生时，USART1接收数据寄存器不为空
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
	//使能USART6
	USART_Cmd(USART6, ENABLE);
	//使能USART6中断
	NVIC_InitStructure.NVIC_IRQChannel=USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_AHB1PeriphClockCmd(MAX485CTR_APP, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = MAX485CTR_PIN;	
  GPIO_Init(MAX485CTR_PORT, &GPIO_InitStructure);
	MAX485CTR_OUT(1);  //允许发出
	
}


/*
	函数名：USART_DataStructure *USART6_Buffer(void)
	描述：	获取串口7结构体数据
*/
USART_DataStructure *Get_USART6Buffer(void)
{
	return &USART6_Buffer;
}
//---
/*
	函数名： void USART6_IRQHandler(void)
	描述：	 串口7中断服务程序,串口3产生中断时，进入该函数
*/
void USART6_IRQHandler(void)                	//串口3中断服务程序
{
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //接收中断
	{//接收一个字节
//	
		USARTx_Receive(USART6,&USART6_Buffer,USART6->DR);
	} 
	if(USART_GetITStatus(USART6, USART_IT_TXE) != RESET)  //接收中断
	{//发送一个字节

		USARTx_Send(USART6,&USART6_Buffer);	
	}
}
//---
#endif

//-----------------------------------------------------------------------------------
//串口7
#ifdef OPEN_USART7_COM
//---
USART_DataStructure UART7_Buffer;		//串口7结构体全局变量
/*
	函数名：USART_DataStructure *UART7_Buffer(void)
	描述：	获取串口7结构体数据
*/
USART_DataStructure *Get_UART7Buffer(void)
{
	return &UART7_Buffer;
}
//---
/*
	函数名： void UART7_IRQHandler(void)
	描述：	 串口7中断服务程序,串口3产生中断时，进入该函数
*/
void UART7_IRQHandler(void)                	//串口3中断服务程序
{
	if(USART_GetITStatus(UART7, USART_IT_RXNE) != RESET)  //接收中断
	{//接收一个字节
		USARTx_Receive(UART7,&UART7_Buffer,UART7->DR);	
	} 
	if(USART_GetITStatus(UART7, USART_IT_TXE) != RESET)  //接收中断
	{//发送一个字节
//		Printf_Send();	
		USARTx_Send(UART7,&UART7_Buffer);		
	}
}
//---
#endif
//-----------------------------------------------------------------------------------
//串口8
#ifdef OPEN_USART8_COM
//---
USART_DataStructure UART8_Buffer;		//串口7结构体全局变量
/*
	函数名：USART_DataStructure *UART8_Buffer(void)
	描述：	获取串口7结构体数据
*/
USART_DataStructure *Get_UART8Buffer(void)
{
	return &UART8_Buffer;
}
//---
/*
	函数名： void UART8_IRQHandler(void)
	描述：	 串口7中断服务程序,串口3产生中断时，进入该函数
*/
void UART8_IRQHandler(void)                	//串口3中断服务程序
{
	if(USART_GetITStatus(UART8, USART_IT_RXNE) != RESET)  //接收中断
	{//接收一个字节
		USARTx_Receive(UART8,&UART8_Buffer,UART8->DR);
	} 
	if(USART_GetITStatus(UART8, USART_IT_TXE) != RESET)  //接收中断
	{//发送一个字节
		USARTx_Send(UART8,&UART8_Buffer);					
	}
}
//---
#endif
//-----------------------------------------------------------------------------------
//---
/*
	函数名： void USARTx_StoreBufferData(USART_TypeDef* USARTx,USART_DataStructure *p,unsigned char b)
	描述：	 	将需要打印的数据存入打印缓冲区，按照FIFO原则打印出来每次调用后，需要启动发送中断，做程序移植时，需修
					改最后一句语句，即启动发送中断即可
	输出：	 	无
	输入：	 	USART_TypeDef* USARTx：需要被传输的串口端口（USART1、USART2、USART3、UART4、UART5、USART6、UART7、UART8）
					USART_DataStructure *p：对应端口的数据结构体地址
					unsigned char b：需要被打印的数据
	返回：	 	无
*/
void USARTx_StoreBufferData(USART_TypeDef* USARTx,USART_DataStructure *p,unsigned char b)
{
	while(p->TxLen == TXBF_LEN-1) USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);	//当缓冲区填满时，需要等待
	p->TxBuffer[p->TxStorePoint++]=b;	//存储发送数据
	if(p->TxStorePoint == TXBF_LEN) p->TxStorePoint=0;	//存储指针指向表顶时，修正指针指向表低
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);		//发送完毕，关闭发送中断
	p->TxLen++;																					//发送数据个数+1
	USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);				//打开中断,开始发送

}
//---
/*
	函数名：void 
*/
//---
/*
	函数名： void USARTx_Send(USART_TypeDef* USARTx,USART_DataStructure *p)
	描述：	 	该函数为发送打印缓冲区的一个字节数据，由发送中断常调用，每调用一次发送一个字节数据
					属于底层驱动相关，因此程序做移植时，主要修改此函数
	输入：		需要发送的对应串口硬件端口
					USART_TypeDef* USARTx：需要被传输的串口端口（USART1、USART2、USART3、UART4、UART5、USART6、UART7、UART8）
					USART_DataStructure *p：对应端口的数据结构体地址
*/
void USARTx_Send(USART_TypeDef* USARTx,USART_DataStructure *p)
{
	if(p->TxLen)
	{
	 USART_SendData(USARTx, p->TxBuffer[p->TxSendPoint++]);	//发送一个字节数据
	 if(p->TxSendPoint == TXBF_LEN) p->TxSendPoint=0;		//若指针指向表顶，修改指针
		p->TxLen--;
		if(p->TxLen==0)
		{
		USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);		//发送完毕，关闭发送中断	
		USART_ClearFlag(USARTx,USART_FLAG_TXE);
		}
	}
	else
	{
		USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);		//发送完毕，关闭发送中断
		USART_ClearFlag(USARTx,USART_FLAG_TXE);
	}

}
//---
/*
	函数名：	void USARTx_Receive(USART_TypeDef* USARTx,USART_DataStructure *p,unsigned char b)
	描述：		串口接收一个数据处理
					数据帧：起始字 设备编号 指令字 逻辑号 【长度+数据】 校验字 结束字
						:XXX|XXX|XXX|········
	输入：		u8 tmp：接收到的数据,串口1接收中断常调用
					接收结构说明：该函数接收来自串口的每一个数据，若连续5毫秒内没有接收到数据
					则认为接收完成(清b0置b2同时停止接收),若接收的数据个数等于RXBF_LEN-1，则接
					收溢出(清b0置b1同时停止接收)；接收完成后，若再10毫秒内没有处理数据，则当前
					这帧数据作废处理，重新启动接收
					器(见void USART3_Timer(void))
					USART_TypeDef* USARTx：需要被传输的串口端口（USART1、USART2、USART3、UART4、UART5、USART6、UART7、UART8）
					USART_DataStructure *p：对应端口的数据结构体地址
					unsigned char b：接收到的数据
	输出：		无
 	返回：		无
//标志位含义： b0(正在接收),b1(接收溢出),b2(接收完毕),b7=1(RTU模式接收)
*/
void USARTx_Receive(USART_TypeDef* USARTx,USART_DataStructure *p,unsigned char b)
{
	p->RxBuffer[p->RxCount++] = b;  //向存储区存储一个接收字节数据
	p->Timer=0;						//清除接收定时计数器
	if((p->RxCount == RXBF_LEN-1) || (p->RxFlag & b1) == 1 )	//判断是否接收溢出
	{//接收溢出（接收溢出后，前面接收的数据一律认为无效数据，清除数据，从新接收）
		p->RxCount = 0;			//清计数器
		p->RxFlag &= ~b0;		//清正在接收标志
 		p->RxFlag |= b1;		//置接收溢出标志位
		USARTx->CR1 &= ~(1<<2);	//关闭接收，硬件控制，程序移植修改此处
	}
	else
	{//正在接收
		p->RxFlag |= b0;	//置正在接收标志位
	}
}
//---
/*
	函数名： void USARTx_Timer(USART_TypeDef* USARTx,USART_DataStructure *p)
	描述：	 	该函数为串口x定时器，用于串口x计时使用，包括接收超时计时，处理超时计时
					1毫秒定时器调用该函数
	解释:		1、当接收启动后，若2毫秒没有收到数据，则认为当前一帧数据接收完成。
					2、当接收数据完成后，若5毫秒内仍然没有处理数据，则将这帧数据丢弃
	输入：		USART_TypeDef* USARTx：对应的硬件串口端口号（USART1、USART2、USART3、UART4、UART5、USART6、UART7、UART8）
					USART_DataStructure *p：对应的相应串口数据结构体指针
//标志位含义： b0(正在接收),b1(接收溢出),b2(接收完毕),b7=1(RTU模式接收)
*/
void USARTx_Timer(USART_TypeDef* USARTx,USART_DataStructure *p)
{
	if(p->RxFlag & b2)
	{//接收完毕后，需要对数据进行处理，若超过100ms没有处理数据，则清除该帧数据，重新启动接收，数据处理计时
		p->Timer++;
		if(p->Timer == 100)
		{//接收超时
			p->Timer=0;
			p->RxCount = 0;	//清计接收数器
			p->RxFlag &= ~(b0 | b1 | b2);	//清正在接收标志、接收溢出标志位、接收完毕标志位
			USARTx->CR1 |= 1<<2;	//开启接收
//			printf("USARTx接收处理超时\r\n");
		//	USARTx_Printf(USARTx,1);
		}
	}	
	else if(p->RxFlag & b1)
	{//查询到当前接收溢出，10ms后重新开启接收
		p->Timer++;
		if(p->Timer == 10)
		{
			p->Timer=0;
			p->RxCount = 0;//清计接收数器
			p->RxFlag &= ~(b0 | b1 | b2);	//清正在接收标志、接收溢出标志位、接收完毕标志位
			USARTx->CR1 |= 1<<2;	//开启接收
		//	USARTx_Printf(USARTx,2);
		}
	}
	else if(p->RxFlag & b0)
	{//正在接收计时
		p->Timer++;
		if(p->Timer == 4)//这里存在一个bug，及如果实时性要求比较高到时候无法做到及时处理数据，因为接收完毕数据后至少需要在2ms后才能得到处理
		{//若2ms后还没接收到数据则认为接收完毕
			p->Timer=0;
			p->RxFlag &= ~b0;				//清正在接收标志位
			p->RxFlag |= b2;				//置接收完毕标志位
			USARTx->CR1 &= ~(1<<2);	//暂停接收
		}
	}
}
//---
/*
	函数名： void USARTx_DW(USART_TypeDef* USARTx,USART_DataStructure *p)
	描述：	 串口x接收数据处理函数，主程序主循环调用
	输入：		USART_TypeDef* USARTx：对应的硬件串口端口号（USART1、USART2、USART3、UART4、UART5、USART6、UART7、UART8）
					USART_DataStructure *p：对应的相应串口数据结构体指针
	协议格式：数据帧：起始字 设备编号 指令字 逻辑号 【长度+数据】 校验字 结束字
	//标志位含义： b0(正在接收),b1(接收溢出),b2(接收完毕),b7=1(RTU模式接收)
*/
void USARTx_DW(USART_TypeDef* USARTx,USART_DataStructure *p)
{
  if(p->RxFlag & b2)
	{//缓冲区接收完成
		u8 *sp=p->RxBuffer;//获取接收首地址
		u8 n=p->RxCount;//接收到的数据个数
		p->RxFlag &= ~b2;//清接收完成标志位
		sp[p->RxCount] = 0;//帧末端设置=0用于字符串处理
	//	if(USARTx == USART3)//屏幕通讯
	//	Screen_DealWihtReceiveData(sp,n);
	//	else if(USARTx == UART4)//蓝牙通讯
	//	HC05_ReceiveData(sp,n);
	if(USARTx == USART1)   //USART1,printf.h,printf.c
	{
    USART_A1:	
		if(*sp==':')
		{
			if((*sp==':') && (n > 8))//
			{//ASCII: :XXX|XX|XXX|贩贩贩(??????|???|???|??贩贩CRC?)
				u8 i=7;
				u8 *p_s=sp;//
				sp=sp+7;
				for(i=7;i<n;i++)
				{
					if(*sp=='\r')//ASCII
					{
						if(*(sp+1)=='\n')
						{//
							ASCII_CommunicateReceive(p_s+5,i-5);
							i+=2;
							if(i<n)
							{
								sp=sp+2;
								n=n-i;
								goto USART_A1;
							}
							break;
						}
					}
					else sp++;
				}
			}
		}
	}		
		p->RxCount=0;//清计数器
		USARTx->CR1 |= 1<<2;//使能串口，允许接收
	}
}
//---
/*
	函数名：void USARTx_Printf(USART_TypeDef* USARTx,u8 mode)
	描述：		该函数用于输出USART故障详情
*/
void USARTx_Printf(USART_TypeDef* USARTx,u8 mode)
{
	if(USARTx == USART1)
	{
		printf("USART1");
	}
	else if(USARTx == USART2)
	{
		printf("USART2");
	}
	else if(USARTx == USART3)
	{
		printf("USART3");
	}
	else if(USARTx == UART4)
	{
		printf("UART4");
	}
	else if(USARTx == UART5)
	{
		printf("UART5");
	}
	switch(mode)
	{
		case 1:
		{
			printf("接收处理超时\r\n");
			break;
		}
		case 2:
		{
			printf("溢出接收\r\n");
			break;
		}
	}
}
//---
//---
/*
函数名：void USART2_SetEven(void)
说明：	将串口2设置为偶校验
*/
void USART_SetEvenVerify(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;// 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
}
//---
/*
函数名：void USART2_SetEven(void)
说明：	将串口2设置为无校验
*/
void USART_SetNoneVerify(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;// 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
}
//---
/*
	函数名：void USART_ModeSet(USART_TypeDef* USARTx,u32 BaudRate,u8 Parity,u8 StopBits)
	描述：设置串口通讯模式
	输入：USART_TypeDef* USARTx需要配置的端口
				u32 BaudRate波特率
				u8 Parity校验位
				u8 StopBits停止位
*/
void USART_ModeSet(USART_TypeDef* USARTx,u32 BaudRate,u8 Parity,u8 StopBits)
{
	USART_InitTypeDef USART_InitStructure;
	//配置USART1串行口
	USART_Cmd(USARTx, DISABLE);
	USART_InitStructure.USART_BaudRate = BaudRate;
	if(StopBits == '1')//1位停止位
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
	else if(StopBits == '2')//2位停止位
		USART_InitStructure.USART_StopBits = USART_StopBits_2;
	else//1.5位停止位
		USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
	if((Parity == 'E') || (Parity == 'e'))
	{//偶校验
		USART_InitStructure.USART_Parity = USART_Parity_Even;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	}
	else if((Parity == 'O') || (Parity == 'o'))
	{//奇校验
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	}
	else
	{//无校验
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	}
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStructure);
	//使能USART1
	USART_Cmd(USARTx, ENABLE);
}

//---









