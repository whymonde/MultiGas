                        //本模块为各串口共用printf()函数

/*
	版本：		V1.0
	编写日期：	
	编辑人：	
	本模块作用：该模块用于printf输出打印，带有缓冲区，缓冲区使用循环列表方式
	使用方法：	1、使用printf输出功能是，需要先将Target Options→Target设置的Use MicroLIB选项点勾
				2、初始化输出对象（如串口）的各项参数，并开启发送中断功能
				3、在打印的输出对象（如串口输出）的发送中断（如串口的发送中断）调用Printf_Send()函数
				4、需要将printf.h文件的第7行语句(//#define OPEN_PRINTF)注释去掉
				5、Printf_Data.Printf_Mode初始化为1，默认调试打印串口，若使用其他口，一定要按使用情况把开关Printf_Data.Printf_Mode调到指定值，用完要恢复=1。
	修改日期：
	修改说明：
*/
#include "printf.h"

Printf_DataStructure  Printf_Data; 

Printf_DataStructure * Printf_GetData(void)
{
	return &Printf_Data;
}

//初始化
void Printf_Init(void)
{
	Printf_Data.Printf_Mode=1;
}

/*
	函数原名称： int fputc(int ch, FILE *f)
	说明：	该函数由Printf输出函数调用，将需要打印的数据逐个存储到打印缓冲区中
	输入：	int ch;
	输出：	int ch
	返回：	int ch
*/
PUTCHAR_PROTOTYPE
{
	/* ARMJISHU 零等待的Printf串口打印函数将数据
     写入Buffer后立即返回(Buffer未满时) */
 switch(Printf_Data.Printf_Mode)
 {
	 case 1:   //初始化默认为 1打印端口
	 {
	#ifdef	OPEN_PRINTF    
	USARTx_StoreBufferData(USART1,Get_USART1Buffer(),(unsigned char)ch);		//从串口1把数据发送出去
	#endif
		 break;
	 }
	 case 2:break;
	 case 3:break;
	 case 4:
	 {
		 while((UART4->SR&0X40)==0) //循环发送,直到发送完毕   打印
		 { ;}
	   UART4->DR = (u8) ch; 
    break;		 
	 }
	 case 5:
	 {
		 while((UART5->SR&0X40)==0) //循环发送,直到发送完毕  手机APP 
		 { ;}
	   UART5->DR = (u8) ch; 
		 break;
	 }
	 default:break;
 }
	return ch;
}


	//函数名： void myprintf(char * usart,const char * __restrict /*format*/, ...) __attribute__((__nonnull__(1)))
/*	描述：	 将需要打印的数据存入打印缓冲区，按照FIFO原则打印出来每次调用后，需要启
			 动发送中断，做程序移植时，需修改最后一句语句，即启动发送中断即可
	输出：	 无
	输入：	 (unsigned char ch)需要被打印的数据
	返回：	 无
*/
	
