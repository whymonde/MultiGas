/*
	usart库函数
*/

#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"
//#include <stdio.h>

//IO口宏定义
//USART1
#define USART1_TX_GPIO	GPIOA
#define USART1_TX_Pin		GPIO_Pin_9
#define USART1_RX_GPIO	GPIOA
#define USART1_RX_Pin		GPIO_Pin_10
//USART2
#define USART2_TX_GPIO	GPIOD	
#define USART2_TX_Pin		GPIO_Pin_5
#define USART2_RX_GPIO	GPIOD
#define USART2_RX_Pin		GPIO_Pin_6
//USART3
#define USART3_TX_GPIO	GPIOD
#define USART3_TX_Pin		GPIO_Pin_8
#define USART3_RX_GPIO	GPIOD
#define USART3_RX_Pin		GPIO_Pin_9
//UART4
#define USART4_TX_GPIO	GPIOA	
#define USART4_TX_Pin		GPIO_Pin_0
#define USART4_RX_GPIO	GPIOA
#define USART4_RX_Pin		GPIO_Pin_1
//UART5
#define USART5_TX_GPIO	GPIOC
#define USART5_TX_Pin		GPIO_Pin_12
#define USART5_RX_GPIO	GPIOD
#define USART5_RX_Pin		GPIO_Pin_2
//USART6
#define USART6_TX_GPIO	GPIOC
#define USART6_TX_Pin		GPIO_Pin_6
#define USART6_RX_GPIO	GPIOC
#define USART6_RX_Pin		GPIO_Pin_7
//UART7
#define USART7_TX_GPIO	GPIOE
#define USART7_TX_Pin		GPIO_Pin_7
#define USART7_RX_GPIO	GPIOE
#define USART7_RX_Pin		GPIO_Pin_8
//UART8
#define USART8_TX_GPIO	GPIOE		
#define USART8_TX_Pin		GPIO_Pin_1
#define USART8_RX_GPIO	GPIOE	
#define USART8_RX_Pin		GPIO_Pin_0


#define RXBF_LEN							100			//接收缓冲区定为100个字节
#define TXBF_LEN							1024			//发送缓冲区定为250(不能大于255个)个字节
#define STX										':'			//接收开始
#define RXCR									'\r'		//回车
#define RXLF									'\n'		//换行
#define RECEIVING							0X01		//正在接收
#define RECEIVE_SUCCESS				0X02		//接收成功
#define RECEIVE_WRONG					0X04		//接受错误
#define RECEIVE_OVER					0X08		//接收完毕
#define RECEIVE_TIME					0X10		//接收定时器计时到

//RS485通讯接口定义
//#define RS485DIR_OPEN																//RS485DIR打开
//#define RS485DIR_APP					RCC_AHB1Periph_GPIOA	//RS485DIR管脚的时钟桥
//#define RS485DIR_PORT					GPIOA									//RS485DIR管脚端口
//#define RS485DIR_PIN					GPIO_Pin_2						//RS485DIR管脚
//#define RS485DIR_SEND					GPIO_SetBits(RS485DIR_PORT, RS485DIR_PIN)		//RS485通讯允许发送
//#define RS485DIR_RECEIVE			GPIO_ResetBits(RS485DIR_PORT, RS485DIR_PIN)		//RS485通讯允许接收
//RS485硬件端口定义（用于其他端口映射）
#define RS485_Port						USART3										//RS485端口
#define RS485_Buffer					Get_USART3Buffer()				//RS485串口结构体指针
#define RS485_SendData				USARTx_StoreBufferData		//RS485发送一个字节数据

#define MAX485CTR_OUT(a)\
					if (a==0)\
						GPIO_ResetBits(MAX485CTR_PORT,MAX485CTR_PIN);\
					else if (a==1)\
						GPIO_SetBits(MAX485CTR_PORT,MAX485CTR_PIN);\
					else if(a==2)\
						MAX485CTR_PORT->ODR ^= MAX485CTR_PIN;
/*
	串口收发结构体
*/
typedef struct
{
	u8 RxBuffer[RXBF_LEN];	//接收缓冲区
	
	u8 ProgressBuffer1[RXBF_LEN];	//接收缓冲区
	u8 ProgressBuffer2[RXBF_LEN];	//接收缓冲区
	u8 BufFlg1;           //
	u8 BufFlg2;
	u8 i;
	u8 RxCount;					//接收长度
	u8 Timer;						//接收及处理计时器
	u8 TxBuffer[TXBF_LEN];	//发送缓冲区
	u8 RxFlag;					//接收标志位，b1(正在接收),b2(接收溢出),b3(接收完毕),b8=1(RTU模式接收)
	u16 TxLen;						//发送长度
	u16 TxSendPoint;			//发送计指针
	u16 TxStorePoint;		//存储指针
	u8 TxFlag;					//发送标志位，b8=1(RTU模式发送)	
}USART_DataStructure;

#define UART4_REC_LEN  	 100  	//定义最大接收字节数 100
	  	
typedef struct
{ u8 EnReceveFlg;       //允许接收标志
	u8 ReceveOkFlg;       //接收完毕标志 出口
	u8 ReceveMax;         //接收字节数
	u8 ReceveCnt;         //接收计数器
	u8 ReceveMode;        //AT /透传
	u8 USART_RX_BUF[UART4_REC_LEN];//接收缓冲区  出口
}USART_Com4Structure;			

//全局相关
void USART_InitAll(void);		//初始化串口文件USART
void USART_main(void);			//该函数为usart.c文件的主函数，被系统主函数调用
void USART_Timer(void);			//串口文件定时器

void USARTx_InitUsart(USART_TypeDef* USARTx,u32 BaudRate);//初始化串口1~8
void USARTx_StoreBufferData(USART_TypeDef* USARTx,USART_DataStructure *p,unsigned char b);//将需要打印的数据存入打印缓冲区
void USARTx_Send(USART_TypeDef* USARTx,USART_DataStructure *p);//该函数为发送打印缓冲区的一个字节数据，由发送中断常调用，每调用一次发送一个字节数据
void USARTx_Receive(USART_TypeDef* USARTx,USART_DataStructure *p,unsigned char b);//串口接收一个数据处理
void USARTx_Timer(USART_TypeDef* USARTx,USART_DataStructure *p);//该函数为串口x定时器，用于串口x计时使用，包括接收超时计时，处理超时计时
void USARTx_DW(USART_TypeDef* USARTx,USART_DataStructure *p);//串口x接收数据处理函数，主程序主循环调用
void Usart3_pro(void);//串口3接收数据处理函数，主程序主循环调用


USART_DataStructure *Get_USART1Buffer(void);	//串口1结构体地址获取
USART_DataStructure *Get_USART2Buffer(void);	//串口2结构体地址获取
USART_DataStructure *Get_USART3Buffer(void);//获取串口3结构体数据
USART_DataStructure *Get_UART4Buffer(void);	//串口1结构体地址获取
//USART_DataStructure *Get_UART5Buffer(void);	//串口5结构体地址获取
USART_DataStructure *Get_USART6Buffer(void);//获取串口7结构体数据
//USART_DataStructure *Get_UART7Buffer(void);//获取串口7结构体数据
//USART_DataStructure *Get_UART8Buffer(void);//获取串口7结构体数据
void USARTx_Printf(USART_TypeDef* USARTx,u8 mode);//输出故障详情

void USART1_Configuration(void);
void USART2_Configuration(void);
void USART3_Configuration(void);
void UART4_Configuration(void);
void UART5_Configuration(void);
void USART6_Configuration(void);

void USART_SetEvenVerify(void);
void USART_SetNoneVerify(void);
void USART_ModeSet(USART_TypeDef* USARTx,u32 BaudRate,u8 Parity,u8 StopBits);
void USART1_SendByte(u8 TxData);


void USART_COM4_Enable(void);
void USART_COM4_Disable(void);
void USART_COM4_Receve_com(u8 ReceveMode,u8 ReceveMax);

void USART_COM5_Enable(void);
void USART_COM5_Disable(void);
void USART_COM5_Receve_com(u8 ReceveMode,u8 ReceveMax);

USART_Com4Structure *USART_COM4_GetData(void);

USART_Com4Structure *USART_COM5_GetData(void);

#endif














