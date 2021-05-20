/*
	本文件为该项目的编码器相关函数
	具体函数如下

*/

#include "encoder.h"

Encoder_DataStructure EncoderData;//Encoder.c结构体变量
/*
	函数名：void Encoder_Init(void)
	描述：该函数为encoder.c文件初始化函数
*/
void Encoder_Init(void)
{
//	Encoder_GpioInit();
	Encoder_EncoderDataInit();
}
//---
/*
	函数名：void Encoder_Timer(void)
	描述：	Encoder.c定时器，1ms，由系统定时器调用
*/
void Encoder_Timer(void)
{
	
}
//---
/*
	函数名：void Encoder_GpioInit(void)
	描述：该函数为Encoder.c相关GPIO口初始化
	说明：该函数初始化回转、变幅、吊高编码器配置管脚的初始化
	管脚：PA9		回转控制，0=启动控制，1=关闭控制
				PA10	变幅控制，0=启动控制，1=关闭控制
				PA11	吊高控制，0=启动控制，1=关闭控制
*/
void Encoder_GpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(ENCODER_Slewing_RCC, ENABLE); //打开时钟
	RCC_APB2PeriphClockCmd(ENCODER_Trolleying_RCC, ENABLE); 
	RCC_APB2PeriphClockCmd(ENCODER_Hoisting_RCC, ENABLE); 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin = ENCODER_Slewing_PIN;
	GPIO_Init(ENCODER_Slewing_PORT, &GPIO_InitStructure);//设置DS18B20电源端口
	GPIO_InitStructure.GPIO_Pin = ENCODER_Trolleying_PIN;
	GPIO_Init(ENCODER_Trolleying_PORT, &GPIO_InitStructure);//设置DS18B20发送端口
	GPIO_InitStructure.GPIO_Pin = ENCODER_Hoisting_PIN;
	GPIO_Init(ENCODER_Hoisting_PORT, &GPIO_InitStructure);//设置DS18B20发送端口
	ENCODER_SlewingRESET();
	ENCODER_TrolleyingRESET();
	ENCODER_HoistingRESET();
}
//---
/*
	函数名：void Encoder_EncoderDataInit(void)
	描述：	该函数为回转采集编码器初始化后函数
*/
void Encoder_EncoderDataInit(void)
{
	EncoderData.Slewing.Address = ENCODER_SlewingAddress;
	EncoderData.Trolleying.Address = ENCODER_TrolleyingAddress;
	EncoderData.Hoisting.Address = ENCODER_HoistingAddress;
	EncoderData.CollectMode = 0;
}
//---
/*
	函数名：Encoder_DataStructure* Encoder_GetData(void)
	描述：	该函数用于获取EncoderData结构体变量地址
*/
Encoder_DataStructure* Encoder_GetData(void)
{
	return &EncoderData;
}
/*
	函数名：Encoder_EncoderStructure Encoder_GetSlewingData(void)
	描述：	该函数为获取EncoderSlewing变量地址
*/
Encoder_EncoderStructure* Encoder_GetSlewingData(void)
{
	return &EncoderData.Slewing;
}
//---
/*
	函数名：Encoder_EncoderStructure Encoder_GetTrolleyingData(void)
	描述：	该函数为获取EncoderTrolleying变量地址
*/
Encoder_EncoderStructure* Encoder_GetTrolleyingData(void)
{
	return &EncoderData.Trolleying;
}
//---
/*
	函数名：Encoder_EncoderStructure Encoder_GetHoistingData(void)
	描述：	该函数为获取EncoderHoisting变量地址
*/
Encoder_EncoderStructure* Encoder_GetHoistinggData(void)
{
	return &EncoderData.Hoisting;
}
//---
/*
	函数名：void Encoder_CollectValue(Encoder_EncoderStructure *p)
	描述：	该函数为启动发送采集编码器值函数
	输入：	Encoder_EncoderStructure *p需要采集的编码器结构体地址
*/
void Encoder_CollectValue(Encoder_EncoderStructure *p)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = p->Address;
	buffer[cnt++] = 0x03;//读取命令
	buffer[cnt++] = 0x00;//起始地址，2字节
	buffer[cnt++] = 0x02;
	buffer[cnt++] = 0x00;//数据长度，2字节
	buffer[cnt++] = 0x02;
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	Encoder_SetComNoneVerify();//设置为无校验模式
	for(crc=0;crc<cnt;crc++)
	{
		Encoder_SendBufferData(Encoder_UsartPort,Encoder_UsartBuffer,buffer[crc]);//发送数据
	}
	if(p == &EncoderData.Slewing)
	{
		EncoderData.CollectMode = ENCODER_SlewingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdReadData);
	}
	else if(p == &EncoderData.Trolleying)
	{
		EncoderData.CollectMode = ENCODER_TrolleyingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdReadData);
	}
	else if(p == &EncoderData.Hoisting)
	{
		EncoderData.CollectMode = ENCODER_HoistingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdReadData);
	}
}
//---
/*
	函数名：void Encoder_SetAddress(Encoder_EncoderStructure *p,u8 adr)
	描述：	该函数为设置编码器的地址
	输入：	Encoder_EncoderStructure *p需要设置的编码器结构体地址
					u8 adr设置的地址，运行范围为0x01~0x0F
*/
void Encoder_SetAddress(Encoder_EncoderStructure *p,u8 adr)
{
	if(p == &EncoderData.Slewing)
	{//回转
		ENCODER_SlewingSET();
		EncoderData.CollectMode = ENCODER_SlewingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdSetAddress);
	}
	else if(p == &EncoderData.Trolleying)
	{//变幅
		ENCODER_TrolleyingSET();
		EncoderData.CollectMode = ENCODER_TrolleyingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdSetAddress);
	}
	else if(p == &EncoderData.Hoisting)
	{//吊高
		ENCODER_HoistingSET();
		EncoderData.CollectMode = ENCODER_HoistingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdSetAddress);
	}
	if((adr > 0x0F) || (adr == 0))
	{
		adr = 0x01;
	}
	Encoder_SetComNoneVerify();//设置为无校验模式
	Encoder_SendBufferData(Encoder_UsartPort,Encoder_UsartBuffer,0xA0+adr);//发送数据
}
//---
/*
	函数名：void Encoder_SetCycle(Encoder_EncoderStructure *p,u8 cycle)
	描述：	该函数为设置编码器的地址
	输入：	Encoder_EncoderStructure *p需要设置的编码器结构体地址
					u8 cycle设置的圈数，运行范围为0x01~0x0F
*/
void Encoder_SetCycle(Encoder_EncoderStructure *p,u8 cycle)
{
	if(p == &EncoderData.Slewing)
	{//回转
		ENCODER_SlewingSET();
		EncoderData.CollectMode = ENCODER_SlewingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdSetCircle);
	}
	else if(p == &EncoderData.Trolleying)
	{//变幅
		ENCODER_TrolleyingSET();
		EncoderData.CollectMode = ENCODER_TrolleyingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdSetCircle);
	}
	else if(p == &EncoderData.Hoisting)
	{//吊高
		ENCODER_HoistingSET();
		EncoderData.CollectMode = ENCODER_HoistingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdSetCircle);
	}
	if((cycle > 0x0F) || (cycle == 0))
	{
		cycle = 0x01;
	}
	Encoder_SetComNoneVerify();//设置为无校验模式
	Encoder_SendBufferData(Encoder_UsartPort,Encoder_UsartBuffer,0xA0+cycle);//发送数据
}
//---
/*
	函数名：void Encoder_SetCommand(Encoder_EncoderStructure *p,u8 cmd)
	描述：该函数为设置编码器命令
*/
void Encoder_SetCommand(Encoder_EncoderStructure *p,u8 cmd)
{
	if(p == &EncoderData.Slewing)
	{//回转
		ENCODER_SlewingSET();
		EncoderData.CollectMode = ENCODER_SlewingRunModeBasic + Encoder_SetRunMode(cmd);
	}
	else if(p == &EncoderData.Trolleying)
	{//变幅
		ENCODER_TrolleyingSET();
		EncoderData.CollectMode = ENCODER_TrolleyingRunModeBasic + Encoder_SetRunMode(cmd);
	}
	else if(p == &EncoderData.Hoisting)
	{//吊高
		ENCODER_HoistingSET();
		EncoderData.CollectMode = ENCODER_HoistingRunModeBasic + Encoder_SetRunMode(cmd);
	}
	Encoder_SetComNoneVerify();//设置为无校验模式
	Encoder_SendBufferData(Encoder_UsartPort,Encoder_UsartBuffer,cmd);//发送数据
}
//---
/*
	函数名：u8 Encoder_SetRunMode(u8 cmd)
	描述：	设置不同的命令不同的CollectMode值
*/
u8 Encoder_SetRunMode(u8 cmd)
{
	u8 buffer=0;
	switch(cmd)
	{
		case ENCODER_CmdReadData:
		{//返回编码器数值
			buffer = 1;
			break;
		}
		case ENCODER_CmdSetAddress:
		{//设置地址
			buffer = 2;
			break;
		}
		case ENCODER_CmdBaudRate4800:
		case ENCODER_CmdBaudRate9600:
		case ENCODER_CmdBaudRate19200:
		case ENCODER_CmdBaudRate57600:
		case ENCODER_CmdBaudRate115200:
		{//设置波特率
			buffer = 3;
			break;
		}
		case ENCODER_CmdReturnSet:
		{//读取设定值
			buffer = 4;
			break;
		}
		case ENCODER_CmdReturnValue:
		{//读取当前编码器数值
			buffer = 5;
			break;
		}
		case ENCODER_CmdRetrunBit:
		{//读取单圈与多圈位长度
			buffer = 6;
			break;
		}
		case ENCODER_CmdSet2047Circle:
		{//设置当前圈数为2047圈
			buffer = 7;
			break;
		}
		case ENCODER_CmdSetCircle:
		{//设定当前圈数0~15圈
			buffer = 8;
			break;
		}
		case ENCODER_CmdClockwiseAdd:
		{//设置为顺时针+计数
			buffer = 9;
			break;
		}
		case ENCODER_CmdAnticlockwiseAdd:
		{//设置为逆时针+计数
			buffer = 10;
			break;
		}
	}
	return buffer;
}
//---
//-------------------------------------调试函数----------------------------------------
//---
/*
	函数名： void Encoder_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Encoder.c文件的ASCII调试文件
	:000|57|XXX······
*/
void Encoder_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		//------------------------------------------------------------------------
		case 1://
		{//:000|57|001
			Encoder_CollectValue(&EncoderData.Slewing);
			printf("发送采集回转编码器命令!\r\n");
			break;
		}
		case 2://:000|57|002|10
		{//设置编码器地址命令
			u8 adr = Calculate_CharToU16(p+4,2);//计算命令
			if(adr>0x0F || adr==0)
			{
				printf("输入参数%u错误，请输入参数为01~15\r\n",adr);
			}
			else 
			{
				Encoder_SetAddress(&EncoderData.Slewing,adr);
				printf("设置回转编码器地址为%u\r\n",adr);
			}
			break;
		}
		case 3://:000|57|003|1
		{//设置编码器波特率
			u8 adr = Calculate_CharToU16(p+4,1);//计算命令
			switch(adr)
			{
				case 1:
				{//4800
					Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdBaudRate4800);
					printf("设置回转编码器波特率为4800\r\n");
					break;
				}
				case 2:
				{//9600
					Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdBaudRate9600);
					printf("设置回转编码器波特率为9600\r\n");
					break;
				}
				case 3:
				{//19200
					Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdBaudRate19200);
					printf("设置回转编码器波特率为19200\r\n");
					break;
				}
				case 4:
				{//57600
					Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdBaudRate57600);
					printf("设置回转编码器波特率为57600\r\n");
					break;
				}
				case 5:
				{//115200
					Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdBaudRate115200);
					printf("设置回转编码器波特率为115200\r\n");
					break;
				}
				default:
				{
					printf("输入参数错误，参数范围1~5\r\n");
					break;
				}
			}
			break;
		}
		case 4:
		{//读取设定值，返回地址、波特率、方向代码
			Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdReturnSet);
			printf("读取Slewing的设定值，返回地址、波特率、方向代码\r\n");
			break;
		}
		case 5:
		{//读取编码器当前多圈与单圈数据
			Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdReturnValue);
			printf("读取Slewing编码器当前多圈与单圈数据\r\n");
			break;
		}
		case 6:
		{//读取编码器当前多圈与单圈数据
			Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdRetrunBit);
			printf("读取Slewing编码器多圈与单圈位数\r\n");
			break;
		}
		case 7:
		{//读取编码器当前多圈与单圈数据
			Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdSet2047Circle);
			printf("设定Slewing编码器当前圈数为2047圈\r\n");
			break;
		}
		case 8://:000|57|208|11
		{//设置编码器圈数,可设参数为00~15
			u8 cycle = Calculate_CharToU16(p+4,2);//计算命令
			if(cycle>0x0F)
			{
				printf("输入参数%u错误，请输入参数为00~15\r\n",cycle);
			}
			else 
			{
				Encoder_SetCycle(&EncoderData.Slewing,cycle);
				printf("设置Slewing编码器当前圈数为%u\r\n",cycle);
			}
			break;
		}
		case 9:
		{//设定Slewing编码器是顺时针加计数，逆时针减计数，输入参数为0则为顺时针，非0为逆时针
			if(*(p+4) == '0')
			{
				Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdClockwiseAdd);
				printf("设定Slewing编码器顺时针加计数\r\n");
			}
			else 
			{
				Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdAnticlockwiseAdd);
				printf("设定Slewing编码器逆时针加计数\r\n");
			}
			break;
		}
		//------------------------------------------------------------------------
		case 101://
		{//:000|57|101
			Encoder_CollectValue(&EncoderData.Trolleying);
			printf("发送采集变幅编码器命令!\r\n");
			break;
		}
		case 102://:000|57|202|11
		{//设置编码器地址命令
			u8 adr = Calculate_CharToU16(p+4,2);//计算命令
			if(adr>0x0F || adr==0)
			{
				printf("输入参数%u错误，请输入参数为00~15\r\n",adr);
			}
			else 
			{
				Encoder_SetAddress(&EncoderData.Trolleying,adr);
				printf("设置变幅编码器地址为%u\r\n",adr);
			}
			break;
		}
		case 103://:000|57|003|1
		{//设置编码器波特率
			u8 adr = Calculate_CharToU16(p+4,1);//计算命令
			switch(adr)
			{
				case 1:
				{//4800
					Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdBaudRate4800);
					printf("设置变幅编码器波特率为4800\r\n");
					break;
				}
				case 2:
				{//9600
					Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdBaudRate9600);
					printf("设置变幅编码器波特率为9600\r\n");
					break;
				}
				case 3:
				{//19200
					Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdBaudRate19200);
					printf("设置变幅编码器波特率为19200\r\n");
					break;
				}
				case 4:
				{//57600
					Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdBaudRate57600);
					printf("设置变幅编码器波特率为57600\r\n");
					break;
				}
				case 5:
				{//115200
					Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdBaudRate115200);
					printf("设置变幅编码器波特率为115200\r\n");
					break;
				}
				default:
				{
					printf("输入参数错误，参数范围1~5\r\n");
					break;
				}
			}
			break;
		}
		case 104:
		{//读取设定值，返回地址、波特率、方向代码
			Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdReturnSet);
			printf("读取Trolleying的设定值，返回地址、波特率、方向代码\r\n");
			break;
		}
		case 105:
		{//读取编码器当前多圈与单圈数据
			Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdReturnValue);
			printf("读取Trolleying编码器当前多圈与单圈数据\r\n");
			break;
		}
		case 106:
		{//读取编码器当前多圈与单圈数据
			Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdRetrunBit);
			printf("读取Trolleying编码器多圈与单圈位数\r\n");
			break;
		}	
		case 107:
		{//读取编码器当前多圈与单圈数据
			Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdSet2047Circle);
			printf("设定Trolleying编码器当前圈数为2047圈\r\n");
			break;
		}
		case 108://:000|57|208|11
		{//设置编码器圈数,可设参数为00~15
			u8 cycle = Calculate_CharToU16(p+4,2);//计算命令
			if(cycle>0x0F)
			{
				printf("输入参数%u错误，请输入参数为00~15\r\n",cycle);
			}
			else 
			{
				Encoder_SetCycle(&EncoderData.Trolleying,cycle);
				printf("设置Trolleying编码器当前圈数为%u\r\n",cycle);
			}
			break;
		}
		case 109:
		{//设定Trolleying编码器是顺时针加计数，逆时针减计数，输入参数为0则为顺时针，非0为逆时针
			if(*(p+4) == '0')
			{
				Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdClockwiseAdd);
				printf("设定Trolleying编码器顺时针加计数\r\n");
			}
			else 
			{
				Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdAnticlockwiseAdd);
				printf("设定Trolleying编码器逆时针加计数\r\n");
			}
			break;
		}
		//------------------------------------------------------------------------
		case 201://
		{//:000|57|201
			Encoder_CollectValue(&EncoderData.Hoisting);
			printf("发送采集吊高编码器命令!\r\n");
			break;
		}
		case 202://:000|57|202|11
		{//设置编码器地址命令
			u8 adr = Calculate_CharToU16(p+4,2);//计算命令
			if(adr>0x0F || adr==0)
			{
				printf("输入参数%u错误，请输入参数为00~15\r\n",adr);
			}
			else 
			{
				Encoder_SetAddress(&EncoderData.Hoisting,adr);
				printf("设置吊高编码器地址为%u\r\n",adr);
			}
			break;
		}
		case 203://:000|57|203|1
		{//设置编码器波特率
			u8 adr = Calculate_CharToU16(p+4,1);//计算命令
			switch(adr)
			{
				case 1:
				{//4800
					Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdBaudRate4800);
					printf("设置吊高编码器波特率为4800\r\n");
					break;
				}
				case 2:
				{//9600
					Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdBaudRate9600);
					printf("设置吊高编码器波特率为9600\r\n");
					break;
				}
				case 3:
				{//19200
					Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdBaudRate19200);
					printf("设置吊高编码器波特率为19200\r\n");
					break;
				}
				case 4:
				{//57600
					Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdBaudRate57600);
					printf("设置吊高编码器波特率为57600\r\n");
					break;
				}
				case 5:
				{//115200
					Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdBaudRate115200);
					printf("设置吊高编码器波特率为115200\r\n");
					break;
				}
				default:
				{
					printf("输入参数错误，参数范围1~5\r\n");
					break;
				}
			}
			break;
		}
		case 204:
		{//读取设定值，返回地址、波特率、方向代码
			Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdReturnSet);
			printf("读取Hoisting的设定值，返回地址、波特率、方向代码\r\n");
			break;
		}
		case 205:
		{//读取编码器当前多圈与单圈数据
			Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdReturnValue);
			printf("读取Hoisting编码器当前多圈与单圈数据\r\n");
			break;
		}
		case 206:
		{//读取编码器当前多圈与单圈数据
			Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdRetrunBit);
			printf("读取Hoisting编码器多圈与单圈位数\r\n");
			break;
		}
		case 207:
		{//读取编码器当前多圈与单圈数据
			Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdSet2047Circle);
			printf("设定Hoisting编码器当前圈数为2047圈\r\n");
			break;
		}
		case 208://:000|57|208|11
		{//设置编码器圈数,可设参数为00~15
			u8 cycle = Calculate_CharToU16(p+4,2);//计算命令
			if(cycle>0x0F)
			{
				printf("输入参数%u错误，请输入参数为00~15\r\n",cycle);
			}
			else 
			{
				Encoder_SetCycle(&EncoderData.Hoisting,cycle);
				printf("设置Hoisting编码器当前圈数为%u\r\n",cycle);
			}
			break;
		}
		case 209:
		{//设定Hoisting编码器是顺时针加计数，逆时针减计数，输入参数为0则为顺时针，非0为逆时针
			if(*(p+4) == '0')
			{
				Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdClockwiseAdd);
				printf("设定Hoisting编码器顺时针加计数\r\n");
			}
			else 
			{
				Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdAnticlockwiseAdd);
				printf("设定Hoisting编码器逆时针加计数\r\n");
			}
			break;
		}
		//
		default:
		{
			printf("Encoder.c接收到无效调试指令%u!\r\n",cmd);
			break;
		}
	}
}
//---
/*
	函数名： void Encoder_DebugRTU((u8 *p)
	描述：		该函数为Encoder.c文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*/
void Encoder_DebugRTU(u8 *p,u8 len)
{
	switch(EncoderData.CollectMode)
	{
		//------------------------------------------------------------------
		//回转编码器
		case ENCODER_SlewingRunModeBasic+1:
		{//收到Slewing编码数据
//			u32 bite=0;
			float buffer=0;
			EncoderData.Slewing.ValCircle = *(p+3)<<8 | *(p+4);//圈数
			EncoderData.Slewing.ValAngle = *(p+5)<<8 | *(p+6);//度数
//			bite = EncoderData.Slewing.ValCircle*4096+EncoderData.Slewing.ValAngle;
//			buffer = Tower_CalSlewing(bite);
			Server_GetTowerCraneData()->Rotation = (u16)(buffer+0.5);
//			printf("当前角度为：%u\r\n",Server_GetTowerCraneData()->Rotation);
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+2:
		{//设置地址
			printf("收到ENCODER_Slewing设置地址回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+3:
		{//设置波特率
			printf("收到ENCODER_Slewing设置波特率回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+4:
		{//读取设定值
			printf("收到ENCODER_Slewing读取设定值回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+5:
		{//读取当前编码器数值
			printf("收到ENCODER_Slewing读取当前编码器数值回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+6:
		{//读取单圈与多圈位长度
			printf("收到ENCODER_Slewing读取单圈与多圈位长度回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+7:
		{//设置当前圈数为2047圈
			printf("收到ENCODER_Slewing设置当前圈数为2047圈回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+8:
		{//设定当前圈数0~15圈
			printf("收到ENCODER_Slewing设定当前圈数0~15圈回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+9:
		{//设置为顺时针+计数
			printf("收到ENCODER_Slewing设置为顺时针+计数回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+10:
		{//设置为逆时针-计数
			printf("收到ENCODER_Slewing设置为逆时针-计数回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		//------------------------------------------------------------------
		//变幅编码器
		case ENCODER_TrolleyingRunModeBasic+1:
		{//收到Trolleying编码数据
//			u32 bite=0;
			float buffer=0;
			EncoderData.Trolleying.ValCircle = *(p+3)<<8 | *(p+4);//圈数
			EncoderData.Trolleying.ValAngle = *(p+5)<<8 | *(p+6);//度数
//			bite = EncoderData.Trolleying.ValCircle*4096+EncoderData.Trolleying.ValAngle;
//			buffer = Tower_CalTrolleying(bite);
			Server_GetTowerCraneData()->Range = (u16)(buffer/10+0.5);
//			printf("当前变幅为：%f米\r\n",(float)Server_GetTowerCraneData()->Range/100);
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+2:
		{//设置地址
			printf("收到ENCODER_Trolleying设置地址回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+3:
		{//设置波特率
			printf("收到ENCODER_Trolleying设置波特率回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+4:
		{//读取设定值
			printf("收到ENCODER_Trolleying读取设定值回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+5:
		{//读取当前编码器数值
			printf("收到ENCODER_Trolleying读取当前编码器数值回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+6:
		{//读取单圈与多圈位长度
			printf("收到ENCODER_Trolleying读取单圈与多圈位长度回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+7:
		{//设置当前圈数为2047圈
			printf("收到ENCODER_Trolleying设置当前圈数为2047圈回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+8:
		{//设定当前圈数0~15圈
			printf("收到ENCODER_Trolleying设定当前圈数0~15圈回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+9:
		{//设置为顺时针+计数
			printf("收到ENCODER_Trolleying设置为顺时针+计数回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+10:
		{//设置为逆时针-计数
			printf("收到ENCODER_Trolleying设置为逆时针-计数回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		//------------------------------------------------------------------
		//吊高编码器
		case ENCODER_HoistingRunModeBasic+1:
		{//收到Slewing编码数据
//			u32 bite=0;
			float buffer=0;
			EncoderData.Hoisting.ValCircle = *(p+3)<<8 | *(p+4);//圈数
			EncoderData.Hoisting.ValAngle = *(p+5)<<8 | *(p+6);//度数
//			bite = EncoderData.Hoisting.ValCircle*4096+EncoderData.Hoisting.ValAngle;
//			buffer = Tower_CalHoisting(bite);
			Server_GetTowerCraneData()->HookHeight = (u16)(buffer/10+0.5);
//			printf("当前吊钩高度为：%f米\r\n",(float)Server_GetTowerCraneData()->HookHeight/100);
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+2:
		{//设置地址
			printf("收到ENCODER_Hoisting设置地址回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+3:
		{//设置波特率
			printf("收到ENCODER_Hoisting设置波特率回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+4:
		{//读取设定值
			printf("收到ENCODER_Hoisting读取设定值回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+5:
		{//读取当前编码器数值
			printf("收到ENCODER_Hoisting读取当前编码器数值回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+6:
		{//读取单圈与多圈位长度
			printf("收到ENCODER_Hoisting读取单圈与多圈位长度回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+7:
		{//设置当前圈数为2047圈
			printf("收到ENCODER_Hoisting设置当前圈数为2047圈回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+8:
		{//设定当前圈数0~15圈
			printf("收到ENCODER_Hoisting设定当前圈数0~15圈回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+9:
		{//设置为顺时针+计数
			printf("收到ENCODER_Hoisting设置为顺时针+计数回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+10:
		{//设置为逆时针-计数
			printf("收到ENCODER_Hoisting设置为顺时针+-计数回复，\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
	}
	if(EncoderData.CollectMode == 0)
	{
		ENCODER_SlewingRESET();
		ENCODER_TrolleyingRESET();
		ENCODER_HoistingRESET();
	}
}
//---












