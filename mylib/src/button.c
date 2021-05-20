/*
	本文件为该项目的按键相关函数
	具体函数如下
*/

#include "button.h"

//#define BUTTON_R					42//点
//#define BUTTON_ESC		            43//左
//#define BUTTON_L					44//下
//#define BUTTON_SW					45//右

Button_DataStructure ButtonData;
/*
	函数名：void Button_Main(void)
	描述：	button.c主函数
*/
void Button_Main(void)
{
}
//---
/*
	函数名：void Button_Timer(void)
	描述：button.c文件定时器函数，由系统1ms定时器调用
*/
void Button_Timer(void)
{
//	if(ButtonData.Timer != 0) ButtonData.Timer--;
	ButtonData.Timer--;
	
	if(ButtonData.Timer == 0)
	{ 
		ButtonData.Timer = 40;
		u8 Ydata;
		switch(ButtonData.RunMode)
		{
			case 1: //按键空闲
			{
				X_ALLDOWM;
				ButtonData.KeyData=0;
				ButtonData.XData=0; 
				ButtonData.XData=0;
				Ydata=GET_YDATA;
				if(Ydata!=0xff)//有按键
				{
				   ButtonData.RunMode=2;//进入下一个工作模式 取按键列值
				}
			//	printf("Y值YData=0X%2x\r\n", Ydata);
				break;
			}
			case 2:
			{  //取按键行列值
				Ydata=GET_YDATA;
				if(Ydata==0xff)  //无按键  去除干扰
				{
					ButtonData.RunMode=1;
				}else
				{
					//Y1~Y7 code 0xfe 0xfd 0xfc 0xf7 0xef 0xdf 0xcf
					ButtonData.YData=Ydata;
					ButtonData.RunMode=3;
					ButtonData.Timer = 1; //延时10ms后计算行值
				}
				break;
			}
			case 3:
			{//计算行值
				X1_UP;	
				Ydata=GET_YDATA;				
				if(Ydata==0xff)
				{
					ButtonData.XData=0x01;
					ButtonData.RunMode=4;
					break;
				}
				X2_UP;	 
				Ydata=GET_YDATA;
				if(Ydata==0xff)
				{
					ButtonData.XData=0x02;
					ButtonData.RunMode=4;
					break;
				}
				X3_UP;	 
				Ydata=GET_YDATA;
				if(Ydata==0xff)
				{
					ButtonData.XData=0x03;
					ButtonData.RunMode=4;
					break;
				}
				X4_UP;	 
				Ydata=GET_YDATA;
				if(Ydata==0xff)
				{
					ButtonData.XData=0x04;
					ButtonData.RunMode=4;
					break;
				}
				break;
			}
			case 4:
			{//计算按键值		
				int i,data;
				data=ButtonData.YData;
				for(i=0;i<7;i++)
				{
					if((data&0x01)==0x00)
					{
						break;
					}
					data>>=1;				 
				}
				ButtonData.KeyData=(ButtonData.XData)*0x10+i;
				ButtonData.RunMode=5;
			}
			case 5: 
			{ //按键处理
				printf("按键值XData=0X%2x, YData=0X%2x", ButtonData.KeyData,ButtonData.YData);
				printf("按键值KeyData=0X%2x", ButtonData.KeyData);
				ButtonData.RunMode=1;
				ButtonData.Timer=200;
				
				if(Screen_GetData()->ScreenButtonLock==2)  //按键锁
				ButtonData.KeyData=0;
				
				Speak_Buzzer(100);
				break;
			}
			default:
			{
				ButtonData.RunMode = 1;
				break;
			}
		}
	}
}
//---
/*
	函数名：void Button_Init(void)
	描述：	button.c文件初始化函数
*/
void Button_Init(void)
{
	Button_GpioInit();
	Button_DataInit();
}
//---
/*
	函数名：void Button_DataInit(void)
	描述：	该函数为button.c文件数据初始化
*/
void Button_DataInit(void)
{
	ButtonData.KeyData = 0;
	ButtonData.Timer = 20;
    ButtonData.RunMode=1;
	ButtonData.XData=0;
	ButtonData.YData=0;
}
//---
/*
	函数名：void Button_GpioInit(void)
	描述：按键端口初始化
*/
void Button_GpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE, ENABLE); 
	//Y 配置成上拉输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_13;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13;		
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_7);
	GPIO_SetBits(GPIOE,GPIO_Pin_9);
	//X 配置成输出
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_OUT;
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;		
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
//---
/*
	函数名：Button_DataStructure* Button_GetData(void)
	描述：获取返回button.c文件全局变量结构体变量
*/
Button_DataStructure* Button_GetData(void)
{
	return &ButtonData;
}
//---
//-------------------------数据读取、存储----------------------------
//---
/*
	函数名：void Button_ReadData(u8 address,u8 *p,u8 len)
	描述：	读取数据
*/
void Button_ReadData(u8 address,u8 *p,u8 len)
{
}
//---
/*
	函数名：void Button_StoreData(u8 address,u8 *p,u8 len)
	描述：该函数为存储button.c配置参数函数
	说明：读取数据
				擦除数据
				修改数据
				保存数据
*/
void Button_StoreData(u8 address,u8 *p,u8 len)
{
}
//---
/*
	函数名：void Button_FactoryReset(void)
	描述：该函数Button.c存储数据恢复出厂设置
*/
void Button_FactoryReset(void)
{
	
}
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Button_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Button.c文件的ASCII调试文件
	命令：	 :000|06|XXX|XXX
*/
void Button_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 101:
		{//设置ButtonData.KeyData值
//			ButtonData.KeyData = Calculate_CharToU16(p+4,3);
			Screen_GetData()->Key.KeyData = Calculate_CharToU16(p+4,3);
			printf("设置的新KeyData值为：%u\r\n",Screen_GetData()->Key.KeyData);
			
			break;
		}
		case 102:
		{
			break;
		}
	}
}
//---
/*
	函数名： 	void Button_DebugRTU((u8 *p)
	描述：		该函数为Button.c文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*//*
void Button_DebugRTU(u8 *p,u8 len)
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
}*/
//---





































