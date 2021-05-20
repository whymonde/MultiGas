
#include "APPAT.h"

APP_ModeStructure APP_Mode;

APP_ModeStructure *APP_GetData(void)
{
	return & APP_Mode;
}

//设置高电平清理记忆
void APP_Key_Init() 
	{

	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(HC06B_KEY_APP, ENABLE); //使能GPIOD时钟

	GPIO_InitStructure.GPIO_Pin = HC06B_KEY_PIN;             //连接26/Key管脚 输出高电平
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    //50MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;     //下拉
	GPIO_Init(HC06B_KEY_PORT, &GPIO_InitStructure);                //初始化
	APP_BLUETOOTH_KEY=0;

}

void APP_CTPOWER_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(HC06B_CTPOWER_APP, ENABLE); //使能GPIOD时钟

	GPIO_InitStructure.GPIO_Pin = HC06B_CTPOWER_PIN;             //连接26/Key管脚 输出高电平
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    //50MHz
	GPIO_Init(HC06B_CTPOWER_PORT, &GPIO_InitStructure);                //初始化
	APP_BLUETOOTH_POWER=0;
}

void APP_Statekey_Init(){
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(HC06B_LED_APP, ENABLE);

	GPIO_InitStructure.GPIO_Pin = HC06B_LED_PIN;              //接24/STA管脚 读高电平
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //普通输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;         //上拉无法读取任意时间，下拉可以
	GPIO_Init(HC06B_LED_PORT, &GPIO_InitStructure);                 //初始化
                                          
}

void APP_Init(void)
{ 
	APP_Key_Init();
	APP_Statekey_Init();
	APP_CTPOWER_Init();
	APP_Mode.Mode=1; //初始AT模式
	APP_Mode.H_time=10;
	APP_Mode.LevelCnt=0;
	APP_Mode.LevelTimeCnt=0;
	APP_Mode.Baudrate=4;
	APP_Power_On();

}

//查询AT状态,返回0 透传，1 AT，外部调用
u8 APP_Mode_Check(void)
{
	return APP_Mode.Mode;
}
	
//握手
void APP_AT_Set(void)
{  
   	Printf_GetData()->Printf_Mode=5;
		printf("AT");
	  Printf_GetData()->Printf_Mode=1;
}
//密码设置
void APP_Password_Set()
{   Printf_GetData()->Printf_Mode=5;
		printf("AT+PIN");
		printf("%s",&APP_Mode.Password[0]);
	  Printf_GetData()->Printf_Mode=1;
}
//主模式设置
void APP_Role_Set(void)
{   Printf_GetData()->Printf_Mode=5;
		printf("AT+ROLE=M");    //printf("AT+ROLE=M"); 
	  Printf_GetData()->Printf_Mode=1;
}

//波特率设置
void APP_BaudRate_Set()
	{ 
	int baud_ratio;
	switch (APP_Mode.Baudrate) {
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
	 Printf_GetData()->Printf_Mode=5;
		printf("AT+BAUD");
		printf("%d", baud_ratio);
   Printf_GetData()->Printf_Mode=1;
}
	
//电源复位命令
void APP_Reset_Com(void)
{ 
	APP_Mode.RunMode = 1;        // 	
	APP_Mode.NextMode = 9;       //  
}
	//握手入口
void APP_AT_Com(void)   //AT
{  
	 APP_Mode.RunMode = 102;      //
	 APP_Mode.Error=0;
	 APP_Mode.OkFLG=0;  //清除命令完成标志
			
	 APP_Mode.H_time=10;   //定义10MS  开电后检测是否进入AT状态
	 APP_Mode.LevelCnt=0;
	 APP_Mode.LevelTimeCnt=0;
}
//密码设置入口,对外接口
void APP_Password_Com(u8 * Password)
{ 
	strncpy((char *)&APP_Mode.Password[0],(char *)Password,4);
	APP_Mode.Password[4]=0;
	APP_Mode.RunMode = 3;   //
	APP_Mode.Error=0;
	APP_Mode.OkFLG=0;  //清除命令完成标志
	APP_Mode.H_time=10;   //定义10MS  开电后检测是否进入AT状态
	APP_Mode.LevelCnt=0;
	APP_Mode.LevelTimeCnt=0;
	
}


//主从模式设置入口,对外接口                  
void APP_Role_Com(void)
{ 
	APP_Mode.RunMode = 5;
	APP_Mode.Error=0;
	APP_Mode.OkFLG=0;  //清除命令完成标志
	APP_Mode.H_time=10;   //定义10MS  开电后检测是否进入AT状态
	APP_Mode.LevelCnt=0;
	APP_Mode.LevelTimeCnt=0;
	
}
//波特率设置入口     (设置玩主模式后设置波特率)
void APP_BautRate_Com(u32 baudrate)
{ 
	APP_Mode.Baudrate=baudrate;
	APP_Mode.RunMode = 7;   //
}


//打开AT电源
void APP_Power_On(void)
{
	APP_BLUETOOTH_POWER=0;
}

//关闭AT电源
void APP_Power_Off(void)
{
	APP_BLUETOOTH_POWER=1;
}

//AT 定时器
void APP_Timer()
	{//AT 
	if(APP_Mode.Timer>0) APP_Mode.Timer--;
	
	//以下为10MS检测一次AT 状态模型
	if(APP_Mode.H_time>0)
	{
		APP_Mode.H_time--;
	}
	else  //
	{
		APP_Mode.H_time=10;   //定义10MS
		
		if(APP_BLUETOOTH_LED)  //检测电平
			APP_Mode.LevelCnt++;
		
		APP_Mode.LevelTimeCnt++;
		if(APP_Mode.LevelTimeCnt>=100)  //1秒时间到,1s判断一次
		{
			//检测有多小个高电平
			if(APP_Mode.LevelCnt==100)
			{
				APP_Mode.Mode = 0;  //透传模式
				
			}
			else
			{
				APP_Mode.Mode = 1;//AT命令模式
				if(APP_Mode.LevelCnt==0)  //没蓝牙模块
				{
					APP_Mode.Mode =2;
					
				}
			}
			APP_Mode.LevelTimeCnt=0;
			APP_Mode.LevelCnt=0;
		}
	}
}

//主函数
void APP_main()
	{
	//功能线程	
	switch(APP_Mode.RunMode)
		{
		case 0:{break;}
		case 1:{ 
			      APP_Power_Off();                 //关电0.5秒
			      APP_Mode.Timer =500;             //1秒超时设置
			      APP_Mode.RunMode=103;
	
			      break;
		       }
		case 103:
		      { if(APP_Mode.Timer ==0)
						{
						  APP_Power_On();
						  APP_Mode.Timer =2000;             //1秒超时设置
			        APP_Mode.RunMode=101;
			        APP_Mode.Error=0;
	            APP_Mode.OkFLG=0;  //清除命令完成标志
			
			        APP_Mode.H_time=10;   //定义10MS  开电后检测是否进入AT状态
			        APP_Mode.LevelCnt=0;
		          APP_Mode.LevelTimeCnt=0;
						}
						break;
		      }
		case 101:	
		       {
						if(APP_Mode.Timer ==0)   //开电2秒完成
						{
							 APP_Mode.RunMode= APP_Mode.NextMode;
						}
			     break;
		       }
					 
		case 102:  //AT 握手
		       {			
			      if(APP_Mode.Mode==1)
						{
			      USART_COM5_Receve_com(0x1,0x02);  //接收OK两个字符
			      APP_AT_Set();                      //发送AT 握手字符
			      APP_Mode.Timer = 1000;             //1秒超时设置
			      APP_Mode.RunMode = 2;
						}
						else
						{
							APP_Mode.RunMode = 0;
							APP_Mode.Error=1;
						}
			      break;
		       }
		case 2:{ //握手完成
		       	if(APP_Mode.Timer == 0)              //时间到超时
						{	//超时
							APP_Mode.RunMode = 0;
							APP_Mode.Error=1;
						}
			      else 
						{
							if(USART_COM5_GetData()->ReceveOkFlg)  //接收完
							{
								APP_Mode.RunMode = 0;
								if(strncmp((char *)&USART_COM5_GetData()->USART_RX_BUF[0],"OK",2) == 0)//比较接收的转发是否为OK   
								{
									APP_Mode.RunMode =14;  //去延时关电源
									APP_Mode.Timer =100;
									printf("AT握手成功");
								}
							}
						}
	
			      break;
		       }  
		case 3:{ //密码线程入口       
			if(APP_Mode.Mode==1)
			{
			USART_COM5_Receve_com(0x1,0x08);  //AT模式下 接收OKsetPIN 8个字符
			APP_Password_Set();        //设置密码
			APP_Mode.Timer = 1000;     //1秒定时
			APP_Mode.RunMode = 4;
			}
			else
			{
				APP_Mode.RunMode = 0;
				APP_Mode.Error=1;		
  
			}
			break;
		}
		case 4:{
			if(APP_Mode.Timer == 0)              //时间到超时
						{	//超时
							APP_Mode.RunMode = 0;
							APP_Mode.Error=1;

						}
			      else 
						{
							if(USART_COM5_GetData()->ReceveOkFlg)  //接收完
							{
								APP_Mode.RunMode = 0;
								if(strncmp((char *)&USART_COM5_GetData()->USART_RX_BUF[0],"OKsetPIN",8) == 0)//比较接收的转发是否为OK   
								{
									APP_Mode.RunMode =14;  //去延时关电源
									APP_Mode.Timer =1000;  //1S后关电源

									printf("设置密码成功");
								}
							}
						
						}
			     break;
		     }
		case 5:{ //设置主模式线程入口       
			
			if(APP_Mode.Mode==1)
			{
			USART_COM5_Receve_com(0x1,0x09);  //接收OK+ROLE:M 9个字符
			APP_Role_Set();        //设置主模式
			APP_Mode.Timer = 1000;
			APP_Mode.RunMode = 6;
			}
			else
			{
				APP_Mode.RunMode = 0;
				APP_Mode.Error=1;

			}
			break;
		}
		case 6:{
			if(APP_Mode.Timer == 0)              //时间到超时
						{	//超时
							APP_Mode.RunMode = 0;
							APP_Mode.Error=1;
						}
			      else 
						{
							if(USART_COM5_GetData()->ReceveOkFlg)  //接收完
							{
								APP_Mode.RunMode = 0;
								//if(strncmp((char *)&USART_COM5_GetData()->USART_RX_BUF[0],"OK+ROLE:M",9) == 0)//比较接收的转发是否为OK   
									if(strncmp((char *)&USART_COM5_GetData()->USART_RX_BUF[0],"OK+ROLE:M",9) == 0)//比较接收的转发是否为OK   
								{
							  	APP_Mode.Timer = 1500; //两个指令之间间隔
			            APP_Mode.RunMode = 60;
							  //	printf("设置主模式成功");
									printf("设置从模式成功");
								}
							}
						
						}
			break;
		}
		case 60://波特率线程入口
		{
			if(APP_Mode.Timer==0)
			{
					APP_Mode.Baudrate=9600;
	        APP_Mode.RunMode = 7;   //
			    //AT_BautRate_Com(9600);
			}
			break;
		}
		case 7:{                          
			     if(APP_Mode.Mode==1)
			      {
			      USART_COM5_Receve_com(0x1,0x06);  //接收OK9600 6个字符
			      APP_BaudRate_Set();
			      APP_Mode.Timer = 1000;             //1秒超时设置
			      APP_Mode.RunMode = 8;
						}
					else
			     {
				    APP_Mode.RunMode = 0;
				    APP_Mode.Error=1;

			      }
				break;
		}
		case 8:{
			   	if(APP_Mode.Timer == 0)              //时间到超时
						{	//超时
							APP_Mode.RunMode = 0;
							APP_Mode.Error=1;

						}
			      else 
						{
							if(USART_COM5_GetData()->ReceveOkFlg)  //接收完
							{
								APP_Mode.RunMode = 0;
								if(strncmp((char *)&USART_COM5_GetData()->USART_RX_BUF[0],"OK9600",6) == 0)//比较接收的转发是否为OK   
								{
									APP_Mode.RunMode =14;  //去延时关电源
									APP_Mode.Timer =1000;  //1.5S后关电源
									printf("设置9600波特率成功");
								}
							}
						
						}
			break;
		}
		
		case 9:{
		       APP_BLUETOOTH_KEY=1;  //设置高电平清除从机地址数据
			     APP_Mode.Timer = 200;   //延时20MS
			     APP_Mode.RunMode = 10;
			     break;
		      }
		case 10:{
			if(APP_Mode.Timer == 0)              //时间到超时
						{	
							APP_Mode.RunMode = 0;
							APP_BLUETOOTH_KEY=0;         //恢复低电平
						  APP_Mode.OkFLG=1;
							APP_Mode.RunMode =0;         //上电结束
							printf("清除记忆成功");
						}
			break;
		}
   case 14:  //延时关电源
		  {
	   if(APP_Mode.Timer == 0)            
			{
				APP_Power_Off();    //重新关开电
				APP_Mode.OkFLG=1;   //完成
				APP_Mode.Timer=500;
				APP_Mode.RunMode=15;
			}
			break;
		}
	case 15:  //延时开电源
		  {
	   if(APP_Mode.Timer == 0)            
			{
				APP_Power_On();
				APP_Mode.RunMode=0;
			}
			break;
		  }

	}
}
//-------------------------调试函数----------------------------
//---
/*
	函数名： void APP_DebugASCII(u8 *p,u8 len)
	描述：		该函数为AT.c文件的ASCII调试文件
						:000|12|001
*/
void APP_DebugASCII(u8 *p,u8 len)
{ //u8 i;
	//char buf[202];
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:  //设置主模式
		{//
			if(APP_Mode.Mode==1)  //在AT模式下才能设置
			APP_Role_Com();      
			else
			printf("不在AT 模式");
			break;
		}
		case 2:  //设置密码
		{
			 if(APP_Mode.Mode==1)  //在AT模式下才能设置
			 APP_Password_Com((u8 *)"1234");
			 else
			 printf("不在AT 模式");
			break;
		}
		case 3:    //握手
		{//
			 if(APP_Mode.Mode==1)  //在AT模式下才能设置
			 APP_AT_Com();
			 else
			 printf("不在AT 模式");
			break;
		}
		case 4://设置波特率
		{
			 if(APP_Mode.Mode==1)  //在AT模式下才能设置
			APP_BautRate_Com(9600);
			 else
			 printf("不在AT 模式");
			break;
		}
		case 5://重新上电
		{
			if(APP_Mode.Mode==1)  //在AT模式下才能设置
			APP_Reset_Com();
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
