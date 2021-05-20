//#include "sys.h"
#include "AT.h"
//#include "usart.h"
//#include "delay.h"
//#include "stdlib.h" 
//#include "string.h"
//#include "stdio.h"

At_ModeStructure At_Mode;


At_ModeStructure *AT_GetData(void)
{
	return & At_Mode;
}

//设置高电平清理记忆
void AT_Key_Init() 
	{

	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(HC06_KEY_APP, ENABLE); //使能GPIOD时钟

	GPIO_InitStructure.GPIO_Pin = HC06_KEY_PIN;             //连接26/Key管脚 输出高电平
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    //50MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;     //下拉
	GPIO_Init(HC06_KEY_PORT, &GPIO_InitStructure);                //初始化
	AT_BLUETOOTH_KEY=0;

}

void AT_CTPOWER_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(HC06_CTPOWER_APP, ENABLE); //使能GPIOD时钟

	GPIO_InitStructure.GPIO_Pin = HC06_CTPOWER_PIN;             //连接26/Key管脚 输出高电平
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    //50MHz
	GPIO_Init(HC06_CTPOWER_PORT, &GPIO_InitStructure);                //初始化
	AT_BLUETOOTH_POWER=0;
}

void AT_Statekey_Init(){
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(HC06_LED_APP, ENABLE);

	GPIO_InitStructure.GPIO_Pin = HC06_LED_PIN;              //接24/STA管脚 读高电平
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //普通输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;         //上拉无法读取任意时间，下拉可以
	GPIO_Init(HC06_LED_PORT, &GPIO_InitStructure);                 //初始化
                                          
}

void AT_Init(void)
{ 
	AT_Key_Init();
	AT_Statekey_Init();
	AT_CTPOWER_Init();
	At_Mode.Mode=1; //初始AT模式
	At_Mode.H_time=10;
	At_Mode.LevelCnt=0;
	At_Mode.LevelTimeCnt=0;
	At_Mode.Baudrate=4;
	AT_Power_On();

}

//查询AT状态,返回0 透传，1 AT，外部调用
u8 AT_Mode_Check(void)
{
	return At_Mode.Mode;
}
	
//握手
void AT_AT_Set(void)
{  
   	Printf_GetData()->Printf_Mode=4;
		printf("AT");
	  Printf_GetData()->Printf_Mode=1;
}
//密码设置
void AT_Password_Set()
{   Printf_GetData()->Printf_Mode=4;
		printf("AT+PIN");
		printf("%s",&At_Mode.Password[0]);
	  Printf_GetData()->Printf_Mode=1;
}
//主模式设置
void AT_Role_Set(void)
{   Printf_GetData()->Printf_Mode=4;
		printf("AT+ROLE=M");//S
	  Printf_GetData()->Printf_Mode=1;
}

//波特率设置
void AT_BaudRate_Set()
	{ 
	int baud_ratio;
	switch (At_Mode.Baudrate) {
		case 1200:{
			baud_ratio = 1;
			break;
		}
		case 2400:{
			baud_ratio = 2;
		  break;
		}
		case 4800:{
			baud_ratio = 3;
			break;
		}
		case 9600:{
			baud_ratio = 4;
			break;
		}
		case 19200:{
			baud_ratio = 5;
			break;
		}
		case 38400:{
			baud_ratio = 6;
			break;
		}
		case 57600:{
			baud_ratio = 7;
			break;
		}
		case 115200:{
			baud_ratio = 8;
			break;
		}
		case 230400:{
			baud_ratio = 9;
			break;
		}
		default:break;
	}
	 Printf_GetData()->Printf_Mode=4;
		printf("AT+BAUD");
		printf("%d", baud_ratio);
   Printf_GetData()->Printf_Mode=1;
}
	
//电源复位命令
void AT_Reset_Com(void)
{ 
	At_Mode.RunMode = 1;        // 	
	At_Mode.NextMode = 9;       //  
}
	//握手入口
void AT_AT_Com(void)   //AT
{  
	 At_Mode.RunMode = 102;      //
	 At_Mode.Error=0;
	 At_Mode.OkFLG=0;  //清除命令完成标志
			
	 At_Mode.H_time=10;   //定义10MS  开电后检测是否进入AT状态
	 At_Mode.LevelCnt=0;
	 At_Mode.LevelTimeCnt=0;
}
//密码设置入口,对外接口
void AT_Password_Com(u8 * Password)
{ 
	strncpy((char *)&At_Mode.Password[0],(char *)Password,4);
	At_Mode.Password[4]=0;
	At_Mode.RunMode = 3;   //
	At_Mode.Error=0;
	At_Mode.OkFLG=0;  //清除命令完成标志
	At_Mode.H_time=10;   //定义10MS  开电后检测是否进入AT状态
	At_Mode.LevelCnt=0;
	At_Mode.LevelTimeCnt=0;
	
}


//主从模式设置入口,对外接口                  
void AT_Role_Com(void)
{ 
	At_Mode.RunMode = 5;
	At_Mode.Error=0;
	At_Mode.OkFLG=0;  //清除命令完成标志
	At_Mode.H_time=10;   //定义10MS  开电后检测是否进入AT状态
	At_Mode.LevelCnt=0;
	At_Mode.LevelTimeCnt=0;
	
}
//波特率设置入口     (设置玩主模式后设置波特率)
void AT_BautRate_Com(u32 baudrate)
{ 
	At_Mode.Baudrate=baudrate;
	At_Mode.RunMode = 7;   //
}


//打开AT电源
void AT_Power_On(void)
{
	AT_BLUETOOTH_POWER=0;
}

//关闭AT电源
void AT_Power_Off(void)
{
	AT_BLUETOOTH_POWER=1;
}

//AT 定时器
void AT_Timer()
	{//AT 
	if(At_Mode.Timer>0) At_Mode.Timer--;
	if(At_Mode.EnableBTtimer&&At_Mode.sBTtimer>0) At_Mode.sBTtimer--;
	
	//以下为10MS检测一次AT 状态模型
	if(At_Mode.H_time>0)
	{
		At_Mode.H_time--;
	}
	else  //
	{
		At_Mode.H_time=10;   //定义10MS
		
		if(AT_BLUETOOTH_LED)  //检测电平
			At_Mode.LevelCnt++;
		
		At_Mode.LevelTimeCnt++;
		if(At_Mode.LevelTimeCnt>=100)  //1秒时间到,1s判断一次
		{
			//检测有多小个高电平
			if(At_Mode.LevelCnt==100)
			{
				At_Mode.Mode = 0;  //透传模式
				
			}
			else
			{
				At_Mode.Mode = 1;//AT命令模式
				if(At_Mode.LevelCnt==0)  //没蓝牙模块
				{
					At_Mode.Mode =2;
					
				}
			}
			At_Mode.LevelTimeCnt=0;
			At_Mode.LevelCnt=0;
		}
	}
}

//主函数
void AT_main()
	{
	//功能线程	
	switch(At_Mode.RunMode)
		{
		case 0:{break;}
		case 1:{ 
			      AT_Power_Off();                 //关电0.5秒
			      At_Mode.Timer =500;             //1秒超时设置
			      At_Mode.RunMode=103;
	
			      break;
		       }
		case 103:
		      { if(At_Mode.Timer ==0)
						{
						  AT_Power_On();
						  At_Mode.Timer =2000;             //1秒超时设置
			        At_Mode.RunMode=101;
			        At_Mode.Error=0;
	            At_Mode.OkFLG=0;  //清除命令完成标志
			
			        At_Mode.H_time=10;   //定义10MS  开电后检测是否进入AT状态
			        At_Mode.LevelCnt=0;
		          At_Mode.LevelTimeCnt=0;
						}
						break;
		      }
		case 101:	
		       {
						if(At_Mode.Timer ==0)   //开电2秒完成
						{
							 At_Mode.RunMode= At_Mode.NextMode;
						}
			     break;
		       }
					 
		case 102:  //AT 握手
		       {			
			      if(At_Mode.Mode==1)
						{
			      USART_COM4_Receve_com(0x1,0x02);  //接收OK两个字符
			      AT_AT_Set();                      //发送AT 握手字符
			      At_Mode.Timer = 1000;             //1秒超时设置
			      At_Mode.RunMode = 2;
						}
						else
						{
							At_Mode.RunMode = 0;
							At_Mode.Error=1;
						}
			      break;
		       }
		case 2:{ //握手完成
		       	if(At_Mode.Timer == 0)              //时间到超时
						{	//超时
							At_Mode.RunMode = 0;
							At_Mode.Error=1;
						}
			      else 
						{
							if(USART_COM4_GetData()->ReceveOkFlg)  //接收完
							{
								At_Mode.RunMode = 0;
								if(strncmp((char *)&USART_COM4_GetData()->USART_RX_BUF[0],"OK",2) == 0)//比较接收的转发是否为OK   
								{
									At_Mode.RunMode =14;  //去延时关电源
									At_Mode.Timer =100;
									printf("AT握手成功");
								}
							}
						}
	
			      break;
		       }  
		case 3:{ //密码线程入口       
			if(At_Mode.Mode==1)
			{
			USART_COM4_Receve_com(0x1,0x08);  //AT模式下 接收OKsetPIN 8个字符
			AT_Password_Set();        //设置密码
			At_Mode.Timer = 1000;     //1秒定时
			At_Mode.RunMode = 4;
			}
			else
			{
				At_Mode.RunMode = 0;
				At_Mode.Error=1;		
  
			}
			break;
		}
		case 4:{
			if(At_Mode.Timer == 0)              //时间到超时
						{	//超时
							At_Mode.RunMode = 0;
							At_Mode.Error=1;

						}
			      else 
						{
							if(USART_COM4_GetData()->ReceveOkFlg)  //接收完
							{
								At_Mode.RunMode = 0;
								if(strncmp((char *)&USART_COM4_GetData()->USART_RX_BUF[0],"OKsetPIN",8) == 0)//比较接收的转发是否为OK   
								{
									At_Mode.RunMode =15;  //去延时关电源   14
									At_Mode.Timer =2000;  //1S后关电源     1000

									printf("设置密码成功");
								}
							}
						
						}
			     break;
		     }
		case 5:{ //设置主模式线程入口       
			
			if(At_Mode.Mode==1)
			{
			USART_COM4_Receve_com(0x1,0x09);  //接收OK+ROLE:M 9个字符
			AT_Role_Set();        //设置主模式
			At_Mode.Timer = 1000;
			At_Mode.RunMode = 6;
			}
			else
			{
				At_Mode.RunMode = 0;
				At_Mode.Error=1;

			}
			break;
		}
		case 6:{
			if(At_Mode.Timer == 0)              //时间到超时
						{	//超时
							At_Mode.RunMode = 0;
							At_Mode.Error=1;
						}
			      else 
						{
							if(USART_COM4_GetData()->ReceveOkFlg)  //接收完
							{
								At_Mode.RunMode = 0;
								if(strncmp((char *)&USART_COM4_GetData()->USART_RX_BUF[0],"OK+ROLE:M",9) == 0)//比较接收的转发是否为OK   //S
								{
							  	At_Mode.Timer = 1500; //两个指令之间间隔
			            At_Mode.RunMode = 60;
									printf("设置主模式成功");
								}
							}
						
						}
			break;
		}
		case 60://波特率线程入口
		{
			if(At_Mode.Timer==0)
			{
					At_Mode.Baudrate=9600;
	        At_Mode.RunMode = 7;   //
			    //AT_BautRate_Com(9600);
			}
			break;
		}
		case 7:{                          
			     if(At_Mode.Mode==1)
			      {
			      USART_COM4_Receve_com(0x1,0x06);  //接收OK9600 6个字符
			      AT_BaudRate_Set();
			      At_Mode.Timer = 1000;             //1秒超时设置
			      At_Mode.RunMode = 8;
						}
					else
			     {
				    At_Mode.RunMode = 0;
				    At_Mode.Error=1;

			      }
				break;
		}
		case 8:{
			   	if(At_Mode.Timer == 0)              //时间到超时
						{	//超时
							At_Mode.RunMode = 0;
							At_Mode.Error=1;

						}
			      else 
						{
							if(USART_COM4_GetData()->ReceveOkFlg)  //接收完
							{
								At_Mode.RunMode = 0;
								if(strncmp((char *)&USART_COM4_GetData()->USART_RX_BUF[0],"OK9600",6) == 0)//比较接收的转发是否为OK   
								{
									At_Mode.RunMode =14;  //去延时关电源14
									At_Mode.Timer =1500;  //1.5S后关电源1000
									printf("设置9600波特率成功");
								}
							}
						
						}
			break;
		}
		
		case 9:{
		       AT_BLUETOOTH_KEY=1;  //设置高电平清除从机地址数据
			     At_Mode.Timer = 200;   //延时20MS
			     At_Mode.RunMode = 10;
			     break;
		      }
		case 10:{
			if(At_Mode.Timer == 0)              //时间到超时
						{	
							At_Mode.RunMode = 0;
							AT_BLUETOOTH_KEY=0;         //恢复低电平
						  At_Mode.OkFLG=1;
							At_Mode.RunMode =0;         //上电结束
							printf("清除记忆成功");
						}
			break;
		}
   case 14:  //延时关电源
		  {
	   if(At_Mode.Timer == 0)            
			{
				AT_Power_Off();    //重新关开电
//				At_Mode.OkFLG=1;   //完成
				At_Mode.Timer=500;
				At_Mode.RunMode=15;
			}
			break;
		}
	case 15:  //延时开电源
		  {
	   if(At_Mode.Timer == 0)            
			{
				AT_Power_On();
				At_Mode.RunMode=0;
				At_Mode.OkFLG=1;
			}
			break;
		  }

	}
}




//-------------------------调试函数----------------------------
//---
/*
	函数名： void AT_DebugASCII(u8 *p,u8 len)
	描述：		该函数为AT.c文件的ASCII调试文件
						:000|11|001
*/
void AT_DebugASCII(u8 *p,u8 len)
{ //u8 i;
	//char buf[202];
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:  //设置主模式
		{//
			if(At_Mode.Mode==1)  //在AT模式下才能设置
			AT_Role_Com();      
			else
			printf("不在AT 模式");
			break;
		}
		case 2:  //设置密码
		{
			 if(At_Mode.Mode==1)  //在AT模式下才能设置
			 AT_Password_Com((u8 *)"1234");
			 else
			 printf("不在AT 模式");
			break;
		}
		case 3:    //握手
		{//
			 if(At_Mode.Mode==1)  //在AT模式下才能设置
			 AT_AT_Com();
			 else
			 printf("不在AT 模式");
			break;
		}
		case 4://设置波特率
		{
			 if(At_Mode.Mode==1)  //在AT模式下才能设置
			AT_BautRate_Com(9600);
			 else
			 printf("不在AT 模式");
			break;
		}
		case 5://重新上电
		{
			if(At_Mode.Mode==1)  //在AT模式下才能设置
			AT_Reset_Com();
			else
			 printf("不在AT 模式");
			break;
		}
		default:
		{
		break;
		}
	}
}
