/*
	本文件为该项目的网络相关函数
	具体函数如下
*/

#include "Network.h"

Network_DataStructure NetworkData;

/*
	函数名：void Network_Main(void)
	描述：	屏幕显示主函数
*/
void Network_Main(void)
{
  #ifndef	OPEN_PRINTF
	char databuf[50];
if(NetworkData.AtMode==0) //正常工作模式
	{
	switch(NetworkData.RunMode)
	{
		case 0:    //待机状态   
		{
			break;
		}
		case 1:    //数据初始化
		{
      NetworkData.SendCnt=0;
			NetworkData.RunMode=2;
			break;
		}
		case 2:    //发送数据
		{	NetworkData.Network_ReceivData.flag=0;
			Network_SendOneFrameData();    //发送数据结构里面的数据
			NetworkData.Timer=1000;        //定时1秒等待接收数据
			NetworkData.RunMode=3;
			break;
		}
		case 3:    //在定时时间内等待接收数据
		{
			if(NetworkData.Timer==0)   //超时
			{
				NetworkData.RunMode=4;
			}
			else                       //查询是否有数据接回来
			{
				if(NetworkData.Network_ReceivData.flag)  //有返回数据，分析数据
				{
					if(NetworkData.Network_ReceivData.DataArr[2]==0xFA)                   //上位机要返回(0XAA,0X55,0X01,0XFA)才能证明上位机接收数据正确
					{
						NetworkData.RunMode=5;  //接收正常
					}
					else  //接收错误
					{
						NetworkData.RunMode=4;
					}
				}
			}
			break;
		}
		case 4:    //超时处理与接收错误
		{

			NetworkData.SendCnt++;
			if(NetworkData.SendCnt>=3)  //重复三次
			{
				NetworkData.RunMode=0;
			}
			else
			{
				NetworkData.RunMode=2;   //重复三次
			}
			break;
		}
		case 5:     //正常接收
		{
			NetworkData.RunMode=0;   //转到待机状态
			break;
		}
			
		case 100:     //延时等待
		{
			if(NetworkData.Timer==0)
			{
				
		    NetworkData.RunMode=NetworkData.NextMode;
			}
			break;
		}

	}
 }
	else   //AT命令模式NetworkData.AtMode=1
	{
		 switch(NetworkData.RunMode)
		 {
			 case 0:    //待机状态   
		   {
			  break;
		   }
			 case 1:    //初始化数据,为下一步做准备
		   { NetworkData.SendCnt=0;   //握手计数器
				 NetworkData.LinkErrFlag=0;
				 NetworkData.RunMode=2;
			  break;
		   }
			 case 2:    //
		   {
				NetworkData.Timer=100;   //握手前延时100ms,计数器100ms
				NetworkData.RunMode=3;
			  break;
		   }
			 case 3:    //
		   {
				if(NetworkData.Timer==0)
				{
					NetworkData.RunMode=4;
				}
					  break;
		   }
       case 4:    //
		   {  Printf_GetData()->Printf_Mode=1;  //把sprintf 的使用付给usart1
				  sprintf((char*)&databuf[0],"+++");
				  Network_SendNByteData((u8 *)&databuf,3);  //发送+++
				  NetworkData.USART_RX_CNT=0;
				  NetworkData.SEND_OK=0;    
				  
				  NetworkData.Timer=1000;   //握手计数器100ms
				  NetworkData.RunMode=5;
					break;
		   }
       case 5:    //
		   {  if(NetworkData.Timer==0)  //定时到，超时
				  {
						NetworkData.SendCnt++;
						if(NetworkData.SendCnt>=3)  //3次链接不上，报错
						{
							NetworkData.RunMode=0;
							NetworkData.LinkErrFlag=1;  //超时
						}
						else
						{
							NetworkData.RunMode=2;  //再重新发送+++
						}
					}
          else  //在没超时时间内检测接收的数据是否正确
					{
						if(NetworkData.SEND_OK==1)   //正常接收'a' 
						{
							NetworkData.RunMode=6;
						}
					}						
				  break;
			 }
			 case 6:
			 {  databuf[0]='a';
				  Printf_GetData()->Printf_Mode=1;  //把sprintf 的使用付给usart1
				  Network_SendNByteData((u8 *)&databuf,1);  //发送'a'
				  NetworkData.USART_RX_CNT=0;
				  NetworkData.SEND_OK=0;
				  NetworkData.Temp='1';
				  NetworkData.Timer=1000;   //握手计数器100ms
				  NetworkData.RunMode=7;
				  break;
			 }
			 case 7:
			 { if(NetworkData.Timer==0)  //超时
				  {
							NetworkData.RunMode=0;
							NetworkData.LinkErrFlag=1;  //超时
					}
				 else //在非超时时间内检测
				  {
						if(NetworkData.SEND_OK==1)   //正常接收'a' 
						{
							NetworkData.RunMode=8;
						}
				  }
				 break;
			 }
			case 8:
			 {  
				 FourBytes four;
				 u16 i;
				 char buf[20];
			   u16 *pf = Flashisp_GetData()->Data;//Flash指针
			   four.U16Data[0] = pf[FLASHIS_AdrWLSZIPDZ];
			   four.U16Data[1] = pf[FLASHIS_AdrWLSZIPDZ+1];
				 Printf_GetData()->Printf_Mode=1;  //把sprintf 的使用付给usart1
			   //IP地址
			   databuf[0]=0;
				 sprintf((char*)&databuf[0],"AT+SOCKA=TCP,");
			   sprintf((char*)&buf[0],"%u",four.U8Data[3]);//192
		     strcat(databuf,(char *)&buf[0]);		  
			   strcat(databuf,".");	
			   sprintf((char*)&buf[0],"%u",four.U8Data[2]);//168
			   strcat(databuf,(char *)&buf[0]);		  
			   strcat(databuf,".");	
			   sprintf((char*)&buf[0],"%u",four.U8Data[1]);//1
			   strcat(databuf,(char *)&buf[0]);		  
			   strcat(databuf,".");	
			   sprintf((char*)&buf[0],"%u",four.U8Data[0]);//1
		     strcat(databuf,(char *)&buf[0]);		  
			   strcat(databuf,",");	
			   //端口号
			   sprintf((char*)&buf[0],"%u",pf[FLASHIS_AdrWLSZDKH]);
				 strcat(databuf,(char *)&buf[0]);
	       strcat(databuf,"\r\n\0");
				 for(i=0;i<sizeof(databuf);i++)
				 {
					 if(databuf[i]=='\0')
					 {
						 break;
					 }
				 }
				
				  Network_SendNByteData((u8 *)&databuf,i);  //发送'AT+SOCKA=TCP,test.usr.cn,2317'
				  NetworkData.USART_RX_CNT=0;
				  NetworkData.SEND_OK=0;
				  NetworkData.Temp='1';
				  NetworkData.Timer=1000;   //握手计数器100ms
				  NetworkData.RunMode=9;
				  break;
			 }
			 case 9:
			 { if(NetworkData.Timer==0)  //超时
				  {
					NetworkData.RunMode=0;
					NetworkData.LinkErrFlag=1;  //超时
					}
				 else //在非超时时间内检测
				  {
						if(NetworkData.SEND_OK==1)   //正常接收'a' 
						{
							NetworkData.RunMode=10;
						}
				  }
				 break;
			 }
			 case 10:             //发送退出AT设置模式,到正常通信
			 {  Printf_GetData()->Printf_Mode=1;  //把sprintf 的使用付给usart1
				  sprintf((char*)&databuf[0],"AT+Z\r\n");
				  Network_SendNByteData((u8 *)&databuf,6);  //发送'复位'
				  NetworkData.USART_RX_CNT=0;
				  NetworkData.SEND_OK=0;
				  NetworkData.Temp='1';
				  NetworkData.Timer=1000;   //握手计数器100ms
				  NetworkData.RunMode=11;
				  break;
			 }
			 case 11:
			 { if(NetworkData.Timer==0)  //超时
				  {
					NetworkData.RunMode=0;
					NetworkData.LinkErrFlag=1;  //超时
					}
				 else //在非超时时间内检测
				  {
						if(NetworkData.SEND_OK==1)   //正常接收'复位' 
						{
							NetworkData.RunMode=0;
							NetworkData.AtMode=0;     //进入正常工作模式
							NetworkData.LinkErrFlag=2;
						}
				
				  }
				 break;
			 }
			 case 31:   //延时
			 {
				 NetworkData.Timer=10000;  //开机延时1s ,等模块
				 NetworkData.HandOKflag=0;
				 NetworkData.SendCnt=0;
				 NetworkData.RunMode=32;
				 break;
			 }
			 case 32:  //握手测试 发”+++“
			 {
				if(NetworkData.Timer==0) //延时到,发生握手命令
				 {
					Printf_GetData()->Printf_Mode=1;  //把sprintf 的使用付给usart1
				  sprintf((char*)&databuf[0],"+++");
				  Network_SendNByteData((u8 *)&databuf,3);  //发送+++
				  NetworkData.USART_RX_CNT=0;
				  NetworkData.SEND_OK=0;    //清空接收成功标志
				  NetworkData.Timer=500;   //握手计数器100ms
					NetworkData.RunMode=33;
				 }
				 break;
			 }
			 case 33:  //等待接收a
			 {
				 if(NetworkData.Timer==0)  //定时到,超时
				  {
						NetworkData.SendCnt++;
						if(NetworkData.SendCnt>=3)  //3次链接不上，报错
						{
							NetworkData.RunMode=0;
							NetworkData.AtMode=0;     //进入正常工作模式
							NetworkData.HandOKflag=2;  //超时,握手失败
						}
						else
						{
							NetworkData.RunMode=32;  //再重新发送+++
						}
					}
          else  //在没超时时间内检测接收的数据是否正确
					{
						if(NetworkData.SEND_OK==1)   //正常接收'a' 
						{
							NetworkData.HandOKflag=1;  //握手成功
							NetworkData.AtMode=0;     //进入正常工作模式
							NetworkData.RunMode=0;
						}
					}					
				 break;
			 }
					 
		 }
	}
	#endif
}
//---
/*
	函数名：void Network_Timer(void)
	描述：Network.c文件定时器函数，由系统1ms定时器调用
*/
void Network_Timer(void)
{ 
	#ifndef	OPEN_PRINTF
	if(NetworkData.Timer>0)
	NetworkData.Timer--;
	
	NetworkData.Timer_50ms++;
	if(NetworkData.Timer_50ms>=50)
	{
		NetworkData.Timer_50ms=0;
		 if(NETLED_4G_IN)                  //引脚为高电平???????????????????????????????????????????????
		 {
			  NetworkData.Hcnt++;
		 }
		NetworkData.Timer_3s++;
		if(NetworkData.Timer_3s>=60)
		{
			NetworkData.Timer_3s=0;
			
			if((NetworkData.Hcnt>6)&&(NetworkData.Hcnt<50))   //检测高电平有多小个
			{
				NetworkData.NetworkStatus=1;                    //网络正常
			}
			else
			{
				NetworkData.NetworkStatus=0;                     //网络失败
			}
			NetworkData.Hcnt=0;
		}
	}
		#endif
}
//---
/*
	函数名：void  Network_RegInit(void)
	描述：	Network.C 数据结构的初始化函数
*/
void Network_RegInit(void)
{
 NetworkData.SendCnt=0;     //初始化为0
 NetworkData.Hcnt=0;        //高电平计数器
 NetworkData.NetworkStatus=0;   //网络状态初始化	
 NetworkData.RunMode=31;
 NetworkData.AtMode=1; //初始化为AT 命令模式用于开机握手
}
/*
	函数名：void  Network_Text(void)
描述：	发送握手命令看是否有通信
*/
void Network_Test(void)
{

 NetworkData.Timer=100;  //开机延时1s ,等模块
 NetworkData.HandOKflag=0;
 NetworkData.SendCnt=0;
 NetworkData.RunMode=32;
 NetworkData.AtMode=1; //测试为AT 命令模式用于开机握手
}
//---
/*
	函数名：void Network_Init(void)
	描述：	Network.c文件初始化函数
*/
void Network_Init(void)
{
		#ifndef	OPEN_PRINTF
  //增加检测网络状态检测引脚的GPIO 设置
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(NETLED_4G_APP, ENABLE);
	RCC_AHB1PeriphClockCmd(POWER_4G_APP, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = NETLED_4G_PIN;	
  GPIO_Init(NETLED_4G_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //电源控制脚设置为输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = POWER_4G_PIN;	
	GPIO_Init(POWER_4G_PORT, &GPIO_InitStructure);
	POWER_4G_OUT(1);  //低点平有效
	
	Network_RegInit();
	
	#endif
	
}


//---
/*
	函数名：Network_DataStructure* Network_GetData(void)
	描述：获取返回Network.c文件全局变量结构体
*/
Network_DataStructure* Network_GetData(void)
{
	return &NetworkData;
}
//---
//-------------------------数据读取、存储----------------------------
//---
/*
	函数名：void Network_ReadData(u8 address,u8 *p,u8 len)
	描述：	读取数据
*/
void Network_ReadData(u8 address,u8 *p,u8 len)
{
//	Flashisp_ReadDataU8(NETWORK_AdrStoreBase+address,p,len);
}


//---
/*
函数名：void Network_SendOneFrameData(u8 *p)
描述：该函数为发送一帧数据函数
说明：
*/

void Network_SendOneFrameData(void)
{ 
       u16 i;
	     USART1_SendByte(NetworkData.Network_SendData.HanderH);   //帧头第一字节
	     USART1_SendByte(NetworkData.Network_SendData.HanderL);   //帧头第二字节
	     USART1_SendByte(NetworkData.Network_SendData.Length);    //数据长度
	     for(i=0;i<=NetworkData.Network_SendData.Length;i++)      //内容+CRC
	      {
				 USART1_SendByte(NetworkData.Network_SendData.DataArr[i]);
				}
}
//---
/*
函数名：void Network_SendOneFrameData(u8 *p)
描述：该函数为发送一帧数据函数
说明：
*/

void Network_SendNByteData(u8 *p,u8 Nbyte)
{ u8 i;
	for(i=0;i<Nbyte;i++)
	{
		USART1_SendByte(*p);
		p++;
	}
	
}
//---
/*
	函数名：void Network_FactoryReset(void)
	描述：该函数Network.c存储数据恢复出厂设置
*/
void Network_FactoryReset(void)
{

	printf("Network参数初始化\r\n");
}
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Network_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Network文件的ASCII调试文件
*/
void Network_DebugASCII(u8 *p,u8 len)
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

//---





/*******************************************************************************
	函数名：USART1_IRQHandler
	输  入:
	输  出:
	功能说明：
	串口1中断服务程序
*/
#ifndef	OPEN_PRINTF
void USART1_IRQHandler(void)                
{
	u8 Res;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0xaa 0x55开始)
	{
	  Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
	if(NetworkData.AtMode==0)   //正常工作模式
	  {
		 if(NetworkData.USART_RX_STA==0)
		   {
			    if (Res==0Xaa) {NetworkData.USART_RX_STA=1;}
		   }
		 else if(NetworkData.USART_RX_STA==1)
		   {
				  if (Res==0X55) {NetworkData.USART_RX_STA=2;}
		   }
			 else if(NetworkData.USART_RX_STA==2)   //接收几字节数
			 {  
				  if(Res>USART_REC_LEN-1)  //数目超出，接收错误
          {NetworkData.USART_RX_STA=0;}        //从新开始接收
          else
          {
						NetworkData.USART_RX_BUF[0]=Res;   //保存长度
						NetworkData.USART_RX_STA=3;
						NetworkData.USART_RX_CNT=1;
	 				}						
			 }
		 else if(NetworkData.USART_RX_STA==3)     //开始接收数据
		   {
				  NetworkData.USART_RX_BUF[NetworkData.USART_RX_CNT]=Res ;
				  NetworkData.USART_RX_CNT++;
				  if(NetworkData.USART_RX_CNT>NetworkData.USART_RX_BUF[0])
					{
					NetworkData.USART_RX_STA=0;
						
					Network_GetData()->Network_ReceivData.flag=1;
						
					}  //接收完毕,主程序定时查询这标志用来分析所接收的数据
		   }
		 }
		 else   //AT 命令模式接收数据
		 {
		    if(NetworkData.USART_RX_CNT>=USART_REC_LEN)  //缓冲区满
				 {
				   NetworkData.USART_RX_CNT=0; //重新从0计数
			   }

				 NetworkData.USART_RX_BUF[NetworkData.USART_RX_CNT++] =Res;
				 
         switch(NetworkData.RunMode)
				 {
				 case 5:
				 case 33:
				  {
					  if(Res==0x61)
						 {
						   if(NetworkData.USART_RX_CNT<=2)
							  {
								 NetworkData.SEND_OK=1;
							  }
						  }
					 break;
				  }
					case 7:
					{
					    if(Res==0x6B)  //'k'
							 {
							  if(NetworkData.Temp==0x6F) //'o'
								 {
								  NetworkData.SEND_OK=1;
							   }
						   }
	         NetworkData.Temp=Res;
					 break;
				  }
					case 9: 
					case 11:
					{
					 if(Res==0x4B)  //'K'
							 {
							  if(NetworkData.Temp==0x4F) //'O'
								 {
								  NetworkData.SEND_OK=1;
							   }
						   }
	         NetworkData.Temp=Res;
					 break;
				  }
			   }
	   }
		 
  } 

} 
#endif




























