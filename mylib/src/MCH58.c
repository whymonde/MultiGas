/*
	本文件为该项目的MCH58编码器相关函数
	具体函数如下
*/

#include "MCH58.h"

MCH58_DataStructure MCH58Data;
/*
	函数名：void MCH58_Main(void)
	描述：	屏幕显示主函数
*/
void MCH58_Main(void)
{
	switch(MCH58Data.RunMode)
	{
		//编码器1
		case 1:
		{
			MCH58_CollectValue(&MCH58Data.Mch58[0]);
			MCH58Data.Timer = 200;
			MCH58Data.RunMode = 2;
			break;
		}
		case 2:
		{
			if(MCH58Data.CollectMode == 0)
			{
				MCH58Data.RunMode = 1;
			}
			else if(MCH58Data.Timer == 0)
			{
				MCH58Data.RunMode = 1;
			}
			break;
		}
		//编码器2
		case 11:
		{
			MCH58_CollectValue(&MCH58Data.Mch58[1]);
			MCH58Data.Timer = 200;
			MCH58Data.RunMode = 12;
			break;
		}
		case 12:
		{
			if(MCH58Data.CollectMode == 0)
			{
				MCH58Data.RunMode = 11;
			}
			else if(MCH58Data.Timer == 0)
			{
				MCH58Data.RunMode = 11;
			}
			break;
		}
		//编码器3
		case 21:
		{
			MCH58_CollectValue(&MCH58Data.Mch58[2]);
			MCH58Data.Timer = 200;
			MCH58Data.RunMode = 22;
			break;
		}
		case 22:
		{
			if(MCH58Data.CollectMode == 0)
			{
				MCH58Data.RunMode = 21;
			}
			else if(MCH58Data.Timer == 0)
			{
				MCH58Data.RunMode = 21;
			}
			break;
		}
		case 31:
		{//设编码器1计数方向
			MCH58_UartModeSet(MCH58_UsartPort,9600,'n','1');
			MCH58_Con1_ON();
			MCH58Data.Timer = 20;
			MCH58Data.RunMode = 32;
			break;
		}
		case 32:
		{
			if(MCH58Data.Timer == 0)
			{
				u8 bf;
				MCH58_Operation(4,&bf,1);
				if(bf == 2)
				{
					MCH58_SendBufferData(MCH58_UsartPort,MCH58_UsartBuffer(),0xF2);
					printf("逆时针加计数\r\n");
				}
				else
				{
					MCH58_SendBufferData(MCH58_UsartPort,MCH58_UsartBuffer(),0xF1);
					printf("顺时针加计数\r\n");
				}
				MCH58Data.Timer = 300;
				MCH58Data.RunMode = 33;
			}
			break;
		}
		case 33:
		{
			if(MCH58Data.Timer == 0)
			{
				MCH58_PWR_OFF();
				MCH58Data.Timer = 20;
				MCH58Data.RunMode = 34;
			}
			break;
		}
		case 34:
		{
			if(MCH58Data.Timer == 0)
			{
				MCH58_PWR_ON();
				MCH58_Con1_OFF();
				printf("设置完毕\r\n");
				MCH58Data.RunMode = 0;
			}
			break;
		}
	}
}
//---
/*
	函数名：void MCH58_Init(void)
	描述：	MCH58.c文件初始化函数
*/
void MCH58_Init(void)
{
	MCH58_GpioInit();
	MCH58_DataInit();
}
//---
/*
	函数名：void MCH58_DataInit(void)
	描述：MCH58编码器数据初始化
*/
void MCH58_DataInit(void)
{
	TwoBytes two;
	//编码器1
	//地址
	MCH58_Operation(6,two.U8Data,1);
	MCH58Data.Mch58[0].Address = two.U8Data[0];
	//波特率
	MCH58_Operation(7,two.U8Data,2);
	MCH58Data.Mch58[0].BaudRate = two.U16Data;
	//映射
	MCH58_Operation(5,two.U8Data,1);
	MCH58Data.Mch58[0].Map = two.U8Data[0];
	//零点值
	MCH58_Operation(2,two.U8Data,2);
	MCH58Data.Mch58[0].ZeroValue = two.U16Data * 4096;
	MCH58_Operation(3,two.U8Data,2);
	MCH58Data.Mch58[0].ZeroValue += two.U16Data;
	//编码器2
	//地址
	MCH58_Operation(106,two.U8Data,1);
	MCH58Data.Mch58[1].Address = two.U8Data[0];
	//波特率
	MCH58_Operation(107,two.U8Data,2);
	MCH58Data.Mch58[1].BaudRate = two.U16Data;
	//映射
	MCH58_Operation(105,two.U8Data,1);
	MCH58Data.Mch58[1].Map = two.U8Data[0];
	//零点值
	MCH58_Operation(102,two.U8Data,2);
	MCH58Data.Mch58[1].ZeroValue = two.U16Data * 4096;
	MCH58_Operation(103,two.U8Data,2);
	MCH58Data.Mch58[1].ZeroValue += two.U16Data;
	//编码器3
	//地址
	MCH58_Operation(206,two.U8Data,1);
	MCH58Data.Mch58[2].Address = two.U8Data[0];
	//波特率
	MCH58_Operation(207,two.U8Data,2);
	MCH58Data.Mch58[2].BaudRate = two.U16Data;
	//映射
	MCH58_Operation(205,two.U8Data,1);
	MCH58Data.Mch58[2].Map = two.U8Data[0];
	//零点值
	MCH58_Operation(202,two.U8Data,2);
	MCH58Data.Mch58[2].ZeroValue = two.U16Data * 4096;
	MCH58_Operation(203,two.U8Data,2);
	MCH58Data.Mch58[2].ZeroValue += two.U16Data;
}
//---
/*
	函数名：void MCH58_GpioInit(void)
	描述：	MCH58所有编码器相关GPIO管脚初始化
*/
void MCH58_GpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(PwrV24_PWR_RCC, ENABLE); //打开时钟
	RCC_APB2PeriphClockCmd(MCH58_Con1_RCC, ENABLE); 
	RCC_APB2PeriphClockCmd(MCH58_Con2_RCC, ENABLE); 
	RCC_APB2PeriphClockCmd(MCH58_Con3_RCC, ENABLE); 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStructure.GPIO_Pin = PwrV24_PWR_PIN;
	GPIO_Init(PwrV24_PWR_PORT, &GPIO_InitStructure);//24V电源
	GPIO_InitStructure.GPIO_Pin = MCH58_Con1_PIN;
	GPIO_Init(MCH58_Con1_PORT, &GPIO_InitStructure);//编码器1控制端口
	GPIO_InitStructure.GPIO_Pin = MCH58_Con2_PIN;
	GPIO_Init(MCH58_Con2_PORT, &GPIO_InitStructure);//编码器2控制端口
	GPIO_InitStructure.GPIO_Pin = MCH58_Con3_PIN;
	GPIO_Init(MCH58_Con3_PORT, &GPIO_InitStructure);//编码器3控制端口
	MCH58_PWR_ON();
	MCH58_Con1_OFF();
	MCH58_Con2_OFF();
	MCH58_Con3_OFF();
}
//---
/*
	函数名：void MCH58_Timer(void)
	描述：MCH58.c文件定时器函数，由系统1ms定时器调用
*/
void MCH58_Timer(void)
{
	if(MCH58Data.Timer != 0)MCH58Data.Timer--;
}
//---
/*
	函数名：MCH58_DataStructure* MCH58_GetMCH58Data(void)
	描述：获取返回MCH58.c文件全局变量结构体
*/
MCH58_DataStructure* MCH58_GetMCH58Data(void)
{
	return &MCH58Data;
}
//---
/*
	函数名：void MCH58_CollectValue(MCH58_Structure *p)
	描述：	该函数为启动发送采集编码器值函数
	输入：	MCH58_Structure *p需要采集的编码器结构体地址
*/
void MCH58_CollectValue(MCH58_Structure *p)
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
	MCH58_UsartModeSet(p);//配置端口模式
	for(crc=0;crc<cnt;crc++)
	{
		MCH58_SendBufferData(MCH58_UsartPort,MCH58_UsartBuffer(),buffer[crc]);//发送数据
	}
	if(p == &MCH58Data.Mch58[0])
	{
		MCH58Data.CollectMode = MCH58_Con1RunModeBasic + MCH58_SetCollectMode(MCH58_CmdReadData);
	}
	else if(p == &MCH58Data.Mch58[1])
	{
		MCH58Data.CollectMode = MCH58_Con2RunModeBasic + MCH58_SetCollectMode(MCH58_CmdReadData);
	}
	else if(p == &MCH58Data.Mch58[2])
	{
		MCH58Data.CollectMode = MCH58_Con3RunModeBasic + MCH58_SetCollectMode(MCH58_CmdReadData);
	}
}
//---
/*
	函数名：u8 MCH58_SetCollectMode(u8 cmd)
	描述：	设置不同的命令不同的CollectMode值
*/
u8 MCH58_SetCollectMode(u8 cmd)
{
	u8 buffer=0;
	switch(cmd)
	{
		case MCH58_CmdReadData:
		{//返回编码器数值
			buffer = 1;
			break;
		}
		case MCH58_CmdSetAddress:
		{//设置地址
			buffer = 2;
			break;
		}
		case MCH58_CmdBaudRate4800:
		case MCH58_CmdBaudRate9600:
		case MCH58_CmdBaudRate19200:
		case MCH58_CmdBaudRate57600:
		case MCH58_CmdBaudRate115200:
		{//设置波特率
			buffer = 3;
			break;
		}
		case MCH58_CmdReturnSet:
		{//读取设定值
			buffer = 4;
			break;
		}
		case MCH58_CmdReturnValue:
		{//读取当前编码器数值
			buffer = 5;
			break;
		}
		case MCH58_CmdRetrunBit:
		{//读取单圈与多圈位长度
			buffer = 6;
			break;
		}
		case MCH58_CmdSet2047Circle:
		{//设置当前圈数为2047圈
			buffer = 7;
			break;
		}
		case MCH58_CmdSetCircle:
		{//设定当前圈数0~15圈
			buffer = 8;
			break;
		}
		case MCH58_CmdClockwiseAdd:
		{//设置为顺时针+计数
			buffer = 9;
			break;
		}
		case MCH58_CmdAnticlockwiseAdd:
		{//设置为逆时针+计数
			buffer = 10;
			break;
		}
	}
	return buffer;
}
//---
/*
	函数名：void MCH58_Operation(u16 mode,u8 *p,u8 len)
	描述：数据操作
*/
void MCH58_Operation(u16 mode,u8 *p,u8 len)
{
	switch(mode)
	{
		//---------------------------
		//读编码器1配置参数
		case 1:
		{//读编码器1所有参数
			MCH58_ReadStoreData(MCH58_AdrStoreBM1Base,p,len);
			break;
		}
		case 2:
		{//读编码器1零点多圈值
			MCH58_ReadStoreData(MCH58_AdrStoreBM1ZeroH,p,len);
			break;
		}
		case 3:
		{//读编码器1零点单圈值
			MCH58_ReadStoreData(MCH58_AdrStoreBM1ZeroL,p,len);
			break;
		}
		case 4:
		{//读编码器1加计数方向
			MCH58_ReadStoreData(MCH58_AdrStoreBM1CD,p,len);
			break;
		}
		case 5:
		{//读编码器1设备映射
			MCH58_ReadStoreData(MCH58_AdrStoreBM1YSSB,p,len);
			break;
		}
		case 6:
		{//读编码器1设备地址
			MCH58_ReadStoreData(MCH58_AdrStoreBM1ADR,p,len);
			break;
		}
		case 7:
		{//读编码器1通讯波特率
			MCH58_ReadStoreData(MCH58_AdrStoreBM1BD,p,len);
			break;
		}
		//修改编码器1配置参数
		case 51:
		{//修改编码器1所有参数
			MCH58_StoreData(MCH58_AdrStoreBM1Base,p,len);
			break;
		}
		case 52:
		{//修改编码器1零点多圈值
			MCH58_StoreData(MCH58_AdrStoreBM1ZeroH,p,len);
			break;
		}
		case 53:
		{//修改编码器1零点单圈值
			MCH58_StoreData(MCH58_AdrStoreBM1ZeroL,p,len);
			break;
		}
		case 54:
		{//修改编码器1加计数方向
			MCH58_StoreData(MCH58_AdrStoreBM1CD,p,len);
			break;
		}
		case 55:
		{//修改编码器1设备映射
			MCH58_StoreData(MCH58_AdrStoreBM1YSSB,p,len);
			break;
		}
		case 56:
		{//修改编码器1设备地址
			MCH58_StoreData(MCH58_AdrStoreBM1ADR,p,len);
			break;
		}
		case 57:
		{//修改编码器1通讯波特率
			MCH58_StoreData(MCH58_AdrStoreBM1BD,p,len);
			break;
		}
		//---------------------------
		//读编码器2配置参数
		case 101:
		{//读编码器2所有参数
			MCH58_ReadStoreData(MCH58_AdrStoreBM2Base,p,len);
			break;
		}
		case 102:
		{//读编码器2零点多圈值
			MCH58_ReadStoreData(MCH58_AdrStoreBM2ZeroH,p,len);
			break;
		}
		case 103:
		{//读编码器2零点单圈值
			MCH58_ReadStoreData(MCH58_AdrStoreBM2ZeroL,p,len);
			break;
		}
		case 104:
		{//读编码器2加计数方向
			MCH58_ReadStoreData(MCH58_AdrStoreBM2CD,p,len);
			break;
		}
		case 105:
		{//读编码器2设备映射
			MCH58_ReadStoreData(MCH58_AdrStoreBM2YSSB,p,len);
			break;
		}
		case 106:
		{//读编码器2设备地址
			MCH58_ReadStoreData(MCH58_AdrStoreBM2ADR,p,len);
			break;
		}
		case 107:
		{//读编码器2通讯波特率
			MCH58_ReadStoreData(MCH58_AdrStoreBM2BD,p,len);
			break;
		}
		//修改编码器2配置参数
		case 151:
		{//修改编码器2所有参数
			MCH58_StoreData(MCH58_AdrStoreBM2Base,p,len);
			break;
		}
		case 152:
		{//修改编码器2零点多圈值
			MCH58_StoreData(MCH58_AdrStoreBM2ZeroH,p,len);
			break;
		}
		case 153:
		{//修改编码器2零点单圈值
			MCH58_StoreData(MCH58_AdrStoreBM2ZeroL,p,len);
			break;
		}
		case 154:
		{//修改编码器2加计数方向
			MCH58_StoreData(MCH58_AdrStoreBM2CD,p,len);
			break;
		}
		case 155:
		{//修改编码器2设备映射
			MCH58_StoreData(MCH58_AdrStoreBM2YSSB,p,len);
			break;
		}
		case 156:
		{//修改编码器2设备地址
			MCH58_StoreData(MCH58_AdrStoreBM2ADR,p,len);
			break;
		}
		case 157:
		{//修改编码器2通讯波特率
			MCH58_StoreData(MCH58_AdrStoreBM2BD,p,len);
			break;
		}
		//---------------------------
		//读编码器3配置参数
		case 201:
		{//读编码器3所有参数
			MCH58_ReadStoreData(MCH58_AdrStoreBM3Base,p,len);
			break;
		}
		case 202:
		{//读编码器3零点多圈值
			MCH58_ReadStoreData(MCH58_AdrStoreBM3ZeroH,p,len);
			break;
		}
		case 203:
		{//读编码器3零点单圈值
			MCH58_ReadStoreData(MCH58_AdrStoreBM3ZeroL,p,len);
			break;
		}
		case 204:
		{//读编码器3加计数方向
			MCH58_ReadStoreData(MCH58_AdrStoreBM3CD,p,len);
			break;
		}
		case 205:
		{//读编码器3设备映射
			MCH58_ReadStoreData(MCH58_AdrStoreBM3YSSB,p,len);
			break;
		}
		case 206:
		{//读编码器3设备地址
			MCH58_ReadStoreData(MCH58_AdrStoreBM3ADR,p,len);
			break;
		}
		case 207:
		{//读编码器3通讯波特率
			MCH58_ReadStoreData(MCH58_AdrStoreBM3BD,p,len);
			break;
		}
		//修改编码器3配置参数
		case 251:
		{//修改编码器3所有参数
			MCH58_StoreData(MCH58_AdrStoreBM3Base,p,len);
			break;
		}
		case 252:
		{//修改编码器3零点多圈值
			MCH58_StoreData(MCH58_AdrStoreBM3ZeroH,p,len);
			break;
		}
		case 253:
		{//修改编码器3零点单圈值
			MCH58_StoreData(MCH58_AdrStoreBM3ZeroL,p,len);
			break;
		}
		case 254:
		{//修改编码器3加计数方向
			MCH58_StoreData(MCH58_AdrStoreBM3CD,p,len);
			break;
		}
		case 255:
		{//修改编码器3设备映射
			MCH58_StoreData(MCH58_AdrStoreBM3YSSB,p,len);
			break;
		}
		case 256:
		{//修改编码器3设备地址
			MCH58_StoreData(MCH58_AdrStoreBM3ADR,p,len);
			break;
		}
		case 257:
		{//修改编码器3通讯波特率
			MCH58_StoreData(MCH58_AdrStoreBM3BD,p,len);
			break;
		}
	}
}
//---
/*
	v函数名：void MCH58_UsartModeSet(MCH58_Structure *p)
	描述：为MCH58配置串口通讯模式
*/
void MCH58_UsartModeSet(MCH58_Structure *p)
{
	MCH58_UartModeSet(MCH58_UsartPort,p->BaudRate,'n','1');
}
//---
//-------------------------数据读取、存储----------------------------
//---
/*
	函数名：void MCH58_ReadStoreData(u8 address,u8 *p,u8 len)
	描述：	读取数据
*/
void MCH58_ReadStoreData(u8 address,u8 *p,u8 len)
{
	W25Xxx_ReadData(p,MCH58_AdrStoreBase+address,len);
}
//---
/*
	函数名：void MCH58_StoreData(u8 address,u8 *p,u8 len)
	描述：该函数为存储MCH58配置参数函数
	说明：读取数据
				擦除数据
				修改数据
				保存数据
*/
void MCH58_StoreData(u8 address,u8 *p,u8 len)
{
	u8 buffer[MCH58_AdrStoreEnd],cnt;
	W25Xxx_ReadData(buffer,MCH58_AdrStoreBase,MCH58_AdrStoreEnd);//读所有配置参数
	W25Xxx_EraseSector(MCH58_AdrStoreBase);//擦除数据
	for(cnt=0;cnt<len;cnt++)
	{
		buffer[address++] = *p;
		p++;
	}
	W25Xxx_PageWrite(buffer,MCH58_AdrStoreBase,MCH58_AdrStoreEnd);//保存数据
}
//---
/*
	函数名：void MCH58_ChangeToMap(MCH58_Structure *p)
	描述：该函数为吧MCH58的编码值转换为对应的目标数据
*/
void MCH58_ChangeToMap(MCH58_Structure *p)
{
//	s32 bf= p->CodedValue - p->ZeroValue;
//	if(p->Map == 1)
//		Tower_CalSlewing(bf);
//	else if(p->Map == 2)
//	{
//		Tower_CalTrolleying(bf);
//	}
//	else if(p->Map == 3)
//	{
//		Tower_CalHoisting(bf);
//	}
}

//---
//-------------------------调试函数----------------------------
//---
/*
	函数名： void MCH58_DebugASCII(u8 *p,u8 len)
	描述：		该函数为MCH58文件的ASCII调试文件
*/
void MCH58_DebugASCII(u8 *p,u8 len)
{
	TwoBytes two;
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		//编码器1
		case 2:
		{//读编码器1零点多圈值
			MCH58_Operation(2,two.U8Data,2);
			printf("编码器1零点多圈值为：%u\r\n",two.U16Data);
			break;
		}
		case 3:
		{//读编码器1零点单圈值
			MCH58_Operation(3,two.U8Data,2);
			printf("编码器1零点单圈值为：%u\r\n",two.U16Data);
			break;
		}
		case 4:
		{//读编码器1加计数方向
			MCH58_Operation(4,two.U8Data,1);
			if(two.U8Data[0] == 1)
				printf("顺时针加计数\r\n");
			else if(two.U8Data[0] == 2)
				printf("逆时针加计数\r\n");
			else
				printf("乱码\r\n");
			break;
		}
		case 5:
		{//读编码器1设备映射
			MCH58_Operation(5,two.U8Data,1);
			if(two.U8Data[0] == 1)
				printf("回转数据采集\r\n");
			else if(two.U8Data[0] == 2)
				printf("变幅数据采集\r\n");
			else if(two.U8Data[0] == 3)
				printf("吊高数据采集\r\n");
			else
				printf("乱码\r\n");
			break;
		}
		case 6:
		{//读编码器1通讯地址
			MCH58_Operation(6,two.U8Data,1);
			printf("编码器1通讯地址为：%u\r\n",two.U8Data[0]);
			break;
		}
		case 7:
		{//读编码器1通讯波特率
			MCH58_Operation(7,two.U8Data,2);
			printf("编码器1通讯波特率为：%u\r\n",two.U16Data);
			break;
		}
		case 52:
		{//修改编码器1零点多圈值
			two.U16Data = Calculate_CharToU16(p+4,4);//计算命令
			if(two.U16Data <4097)
			{
				MCH58_Operation(52,two.U8Data,2);
				printf("修改编码器1零点多圈值为：%u\r\n",two.U16Data);
			}
			else
				printf("输入参数错误，输入范围为0~4096\r\n");
			break;
		}
		case 53:
		{//读编码器1零点单圈值
			two.U16Data = Calculate_CharToU16(p+4,4);//计算命令
			if(two.U16Data <4097)
			{
				MCH58_Operation(53,two.U8Data,2);
				printf("修改编码器1零点单圈值为：%u\r\n",two.U16Data);
			}
			else
				printf("输入参数错误，输入范围为0~4096\r\n");
			break;
		}
		case 54:
		{//读编码器1加计数方向
			if(p[4] == '1')
			{
				two.U8Data[0] = 1;
				printf("设置为顺时针加计数\r\n");
			}
			else
			{
				two.U8Data[0] = 2;
				printf("设置为逆时针加计数\r\n");
			}
			MCH58_Operation(54,two.U8Data,1);
			break;
		}
		case 55:
		{//读编码器1设备映射
			if(p[4] == '2')
			{
				two.U8Data[0] = 2;
				printf("设置为变幅数据采集\r\n");
			}
			else if(p[4] == '3')
			{
				two.U8Data[0] = 3;
				printf("设置为吊高数据采集\r\n");
			}
			else
			{
				two.U8Data[0] = 1;
				printf("设置为回转数据采集\r\n");
			}
			MCH58_Operation(55,two.U8Data,1);
			break;
		}
		case 56:
		{//读编码器1通讯地址
			u8 bf = Calculate_CharToU16(p+4,3);//计算命令
			if(bf>0 && bf<251)
			{
				MCH58_Operation(56,&bf,1);
				printf("编码器1通讯地址为：%u\r\n",bf);
			}
			else
				printf("输入参数错误，请输入1~250\r\n");
			break;
		}
		case 57:
		{//读编码器1通讯波特率
			two.U16Data = Calculate_CharToU16(p+4,5);//计算命令
			if((two.U16Data==4800) || (two.U16Data==9600) || (two.U16Data==19200) ||(two.U16Data==57600))
			{
				MCH58_Operation(57,two.U8Data,2);
				printf("编码器1通讯波特率为：%u\r\n",two.U16Data);
			}
			else
				printf("输入参数错误，请输入4800/9600/19200/57600\r\n");
			break;
		}
		//编码器2
		case 102:
		{//读编码器2零点多圈值
			MCH58_Operation(102,two.U8Data,2);
			printf("编码器2零点多圈值为：%u\r\n",two.U16Data);
			break;
		}
		case 103:
		{//读编码器2零点单圈值
			MCH58_Operation(103,two.U8Data,2);
			printf("编码器2零点单圈值为：%u\r\n",two.U16Data);
			break;
		}
		case 104:
		{//读编码器2加计数方向
			MCH58_Operation(104,two.U8Data,1);
			if(two.U8Data[0] == 1)
				printf("顺时针加计数\r\n");
			else if(two.U8Data[0] == 2)
				printf("逆时针加计数\r\n");
			else
				printf("乱码\r\n");
			break;
		}
		case 105:
		{//读编码器2设备映射
			MCH58_Operation(105,two.U8Data,1);
			if(two.U8Data[0] == 1)
				printf("回转数据采集\r\n");
			else if(two.U8Data[0] == 2)
				printf("变幅数据采集\r\n");
			else if(two.U8Data[0] == 3)
				printf("吊高数据采集\r\n");
			else
				printf("乱码\r\n");
			break;
		}
		case 106:
		{//读编码器2通讯地址
			MCH58_Operation(106,two.U8Data,1);
			printf("编码器2通讯地址为：%u\r\n",two.U8Data[0]);
			break;
		}
		case 107:
		{//读编码器2通讯波特率
			MCH58_Operation(107,two.U8Data,2);
			printf("编码器2通讯波特率为：%u\r\n",two.U16Data);
			break;
		}
		case 152:
		{//修改编码器2零点多圈值
			two.U16Data = Calculate_CharToU16(p+4,4);//计算命令
			if(two.U16Data <4097)
			{
				MCH58_Operation(152,two.U8Data,2);
				printf("修改编码器2零点多圈值为：%u\r\n",two.U16Data);
			}
			else
				printf("输入参数错误，输入范围为0~4096\r\n");
			break;
		}
		case 153:
		{//读编码器2零点单圈值
			two.U16Data = Calculate_CharToU16(p+4,4);//计算命令
			if(two.U16Data <4097)
			{
				MCH58_Operation(153,two.U8Data,2);
				printf("修改编码器2零点单圈值为：%u\r\n",two.U16Data);
			}
			else
				printf("输入参数错误，输入范围为0~4096\r\n");
			break;
		}
		case 154:
		{//读编码器2加计数方向
			if(p[4] == '1')
			{
				two.U8Data[0] = 1;
				printf("设置为顺时针加计数\r\n");
			}
			else
			{
				two.U8Data[0] = 2;
				printf("设置为逆时针加计数\r\n");
			}
			MCH58_Operation(154,two.U8Data,1);
			break;
		}
		case 155:
		{//读编码器2设备映射
			if(p[4] == '1')
			{
				two.U8Data[0] = 1;
				printf("设置为回转数据采集\r\n");
			}
			else if(p[4] == '3')
			{
				two.U8Data[0] = 3;
				printf("设置为吊高数据采集\r\n");
			}
			else
			{
				two.U8Data[0] = 2;
				printf("设置为变幅数据采集\r\n");
			}
			MCH58_Operation(155,two.U8Data,1);
			break;
		}
		case 156:
		{//读编码器2通讯地址
			u8 bf = Calculate_CharToU16(p+4,3);//计算命令
			if(bf>0 && bf<251)
			{
				MCH58_Operation(156,&bf,1);
				printf("编码器2通讯地址为：%u\r\n",bf);
			}
			else
				printf("输入参数错误，请输入1~250\r\n");
			break;
		}
		case 157:
		{//读编码器2通讯波特率
			two.U16Data = Calculate_CharToU16(p+4,5);//计算命令
			if((two.U16Data==4800) || (two.U16Data==9600) || (two.U16Data==19200) ||(two.U16Data==57600))
			{
				MCH58_Operation(157,two.U8Data,2);
				printf("编码器2通讯波特率为：%u\r\n",two.U16Data);
			}
			else
				printf("输入参数错误，请输入4800/9600/19200/57600\r\n");
			break;
		}
		//编码器3
		case 202:
		{//读编码器3零点多圈值
			MCH58_Operation(202,two.U8Data,2);
			printf("编码器3零点多圈值为：%u\r\n",two.U16Data);
			break;
		}
		case 203:
		{//读编码器3零点单圈值
			MCH58_Operation(203,two.U8Data,2);
			printf("编码器3零点单圈值为：%u\r\n",two.U16Data);
			break;
		}
		case 204:
		{//读编码器3加计数方向
			MCH58_Operation(204,two.U8Data,1);
			if(two.U8Data[0] == 1)
				printf("顺时针加计数\r\n");
			else if(two.U8Data[0] == 2)
				printf("逆时针加计数\r\n");
			else
				printf("乱码\r\n");
			break;
		}
		case 205:
		{//读编码器3设备映射
			MCH58_Operation(205,two.U8Data,1);
			if(two.U8Data[0] == 1)
				printf("回转数据采集\r\n");
			else if(two.U8Data[0] == 2)
				printf("变幅数据采集\r\n");
			else if(two.U8Data[0] == 3)
				printf("吊高数据采集\r\n");
			else
				printf("乱码\r\n");
			break;
		}
		case 206:
		{//读编码器3通讯地址
			MCH58_Operation(206,two.U8Data,1);
			printf("编码器3通讯地址为：%u\r\n",two.U8Data[0]);
			break;
		}
		case 207:
		{//读编码器3通讯波特率
			MCH58_Operation(207,two.U8Data,2);
			printf("编码器3通讯波特率为：%u\r\n",two.U16Data);
			break;
		}
		case 252:
		{//修改编码器3零点多圈值
			two.U16Data = Calculate_CharToU16(p+4,4);//计算命令
			if(two.U16Data <4097)
			{
				MCH58_Operation(252,two.U8Data,2);
				printf("修改编码器3零点多圈值为：%u\r\n",two.U16Data);
			}
			else
				printf("输入参数错误，输入范围为0~4096\r\n");
			break;
		}
		case 253:
		{//读编码器3零点单圈值
			two.U16Data = Calculate_CharToU16(p+4,4);//计算命令
			if(two.U16Data <4097)
			{
				MCH58_Operation(253,two.U8Data,2);
				printf("修改编码器3零点单圈值为：%u\r\n",two.U16Data);
			}
			else
				printf("输入参数错误，输入范围为0~4096\r\n");
			break;
		}
		case 254:
		{//读编码器3加计数方向
			if(p[4] == '1')
			{
				two.U8Data[0] = 1;
				printf("设置为顺时针加计数\r\n");
			}
			else
			{
				two.U8Data[0] = 2;
				printf("设置为逆时针加计数\r\n");
			}
			MCH58_Operation(254,two.U8Data,1);
			break;
		}
		case 255:
		{//读编码器3设备映射
			if(p[4] == '2')
			{
				two.U8Data[0] = 2;
				printf("设置为变幅数据采集\r\n");
			}
			else if(p[4] == '1')
			{
				two.U8Data[0] = 1;
				printf("设置为回转数据采集\r\n");
			}
			else
			{
				two.U8Data[0] = 3;
				printf("设置为吊高数据采集\r\n");
			}
			MCH58_Operation(255,two.U8Data,1);
			break;
		}
		case 256:
		{//读编码器3通讯地址
			u8 bf = Calculate_CharToU16(p+4,3);//计算命令
			if(bf>0 && bf<251)
			{
				MCH58_Operation(256,&bf,1);
				printf("编码器3通讯地址为：%u\r\n",bf);
			}
			else
				printf("输入参数错误，请输入1~250\r\n");
			break;
		}
		case 257:
		{//读编码器3通讯波特率
			two.U16Data = Calculate_CharToU16(p+4,5);//计算命令
			if((two.U16Data==4800) || (two.U16Data==9600) || (two.U16Data==19200) ||(two.U16Data==57600))
			{
				MCH58_Operation(257,two.U8Data,2);
				printf("编码器3通讯波特率为：%u\r\n",two.U16Data);
			}
			else
				printf("输入参数错误，请输入4800/9600/19200/57600\r\n");
			break;
		}
		case 801://:000|87|801|1
		{//开/关电源
			if(p[4] == '0')
			{
				MCH58_PWR_OFF();
				printf("关闭VCC_24V输出\r\n");
			}
			else
			{
				MCH58_PWR_ON();
				printf("打开VCC_24V输出\r\n");
			}
			break;
		}
		case 802:
		{//开/关CON1
			if(p[4] == '0')
			{
				MCH58_Con1_OFF();
				printf("24V_OUT1=0\r\n");
			}
			else
			{
				MCH58_Con1_ON();
				printf("24V_OUT1=1\r\n");
			}
			break;
		}
		case 803:
		{//开/关CON2
			if(p[4] == '0')
			{
				MCH58_Con2_OFF();
				printf("24V_OUT2=0\r\n");
			}
			else
			{
				MCH58_Con2_ON();
				printf("24V_OUT2=1\r\n");
			}
			break;
		}
		case 804:
		{//开/关CON3
			if(p[4] == '0')
			{
				MCH58_Con3_OFF();
				printf("24V_OUT3=0\r\n");
			}
			else
			{
				MCH58_Con3_ON();
				printf("24V_OUT3=1\r\n");
			}
			break;
		}
	}
}
//---
/*
	函数名： 	void MCH58_DebugRTU((u8 *p)
	描述：		该函数为MCH58文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*/
void MCH58_DebugRTU(u8 *p,u8 len)
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
/*
	函数名：void MCH58_Receive(u8 *p,u8 len)
	描述：处理来自接收到Thbss01的一帧数据
	说明：进入该函数之前，需要先进行CRC校验
*/
void MCH58_Receive(u8 *p,u8 len)
{
	switch(MCH58Data.CollectMode)
	{
		case MCH58_Con1RunModeBasic+1:
		{//收到编码器1编码数据
			MCH58Data.Mch58[0].CodedValue = (u16)(*(p+3)<<8 | *(p+4))*4096 + (u16)(*(p+5)<<8 | *(p+6));
			MCH58_ChangeToMap(&MCH58Data.Mch58[0]);
			break;
		}
		case MCH58_Con2RunModeBasic+1:
		{//收到编码器2编码数据
			MCH58Data.Mch58[1].CodedValue = (u16)(*(p+3)<<8 | *(p+4))*4096 + (u16)(*(p+5)<<8 | *(p+6));
			MCH58_ChangeToMap(&MCH58Data.Mch58[1]);
			break;
		}
		case MCH58_Con3RunModeBasic+1:
		{//收到编码器3编码数据
			MCH58Data.Mch58[2].CodedValue = (u16)(*(p+3)<<8 | *(p+4))*4096 + (u16)(*(p+5)<<8 | *(p+6));
			MCH58_ChangeToMap(&MCH58Data.Mch58[2]);
			break;
		}
		//计数方向
		case MCH58_Con1RunModeBasic+9:
		case MCH58_Con1RunModeBasic+10:
		{//
			printf("成功设置编码器1计数方向\r\n");
			break;
		}
	}
	MCH58Data.CollectMode = 0;
}
//---



































