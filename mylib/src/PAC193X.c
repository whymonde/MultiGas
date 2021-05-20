/*
	本文件为该项目的PAC1934芯片相关函数
	具体函数如下
*/

#include "PAC193X.h"

PAC193X_DataStructure PAC193XData;
/*
	函数名：void PAC193X_Main(void)
	描述：	屏幕显示主函数
*/
void PAC193X_Main(void)
{
	//数据采集
	if((PAC193XData.CollectTimer == 0) && (PAC193XData.WorkMode == 0))
	{
		u8 i;
		PAC193XData.CollectTimer = 50;
		PAC193X_DelayTime(2);
		PAC193XData.WorkMode = 1;
		for(i=0;i<4;i++)
		{
			TwoBytes two;
			two.U8Data[1] = PAC193XData.VBuffer[i*2];
			two.U8Data[0] = PAC193XData.VBuffer[i*2+1];
			PAC193XData.Channel[i] = two.U16Data;
			PAC193XData.Voltage[i] = 32.0*PAC193XData.Channel[i] / 65535.0;
		}
		PAC193X_GetData()->Facility[0].ADValue = PAC193XData.Channel[1];
		PAC193X_GetData()->Facility[1].ADValue = PAC193XData.Channel[0];
		PAC193X_GetData()->Facility[2].ADValue = PAC193XData.Channel[2];
//		PAC193XData.ADValue[3] = PAC193XData.Channel[3];
		PAC193X_CalMapValue(&PAC193X_GetData()->Facility[0]);
		PAC193X_CalMapValue(&PAC193X_GetData()->Facility[1]);
		PAC193X_CalMapValue(&PAC193X_GetData()->Facility[2]);
	}
}
//---
/*
	函数名：void PAC193X_Work(void)
	描述：该函数为用于系统与PAC1934芯片通讯
*/
void PAC193X_Work(void)
{
	switch(PAC193XData.WorkMode)
	{
		case 1:
			{//发送起始信号
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 201;
			PAC193XData.OldWorkMode = 2;
			break;
		}
		case 2:
		{//发送地址，写
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer = PAC193X_ADDRESS;
			PAC193XData.WorkMode = 231;
			PAC193XData.OldWorkMode = 11;
			PAC193X_DelayTime(2);
			break;
		}
		case 3:
		{//发送地址，读
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer = PAC193X_ADDRESS+1;
			PAC193XData.WorkMode = 231;
			PAC193XData.OldWorkMode = 0;
			PAC193X_DelayTime(2);
			break;
		}
		//REFRESH命令0x00
		case 11:
		{
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer = 0x00;
			PAC193XData.WorkMode = 231;
			PAC193XData.OldWorkMode = 12;
			PAC193X_DelayTime(2);
			break;
		}
		//REFRESH_G命令0x1F
		case 12:
		{
			PAC193XData.WorkMode = 211;
			PAC193XData.OldWorkMode = 13;
			PAC193X_DelayTime(2);
			break;
		}
		case 13:
		{
			PAC193XData.WorkMode = 51;
			PAC193X_DelayTime(5000);
			break;
		}
		//连续读取4通道电压数据
		case 51:
		{
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 201;
			PAC193XData.OldWorkMode = 52;
			break;
		}
		case 52:
		{
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer = PAC193X_ADDRESS;
			PAC193XData.WorkMode = 231;
			PAC193XData.OldWorkMode = 53;
			PAC193X_DelayTime(2);
			break;
		}
		case 53:
		{
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer = 0x0F;//地址
			PAC193XData.WorkMode = 231;
			PAC193XData.OldWorkMode = 54;
			PAC193X_DelayTime(2);
			break;
		}
		case 54:
		{//发送起始信号
			PAC193X_SCL(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 201;
			PAC193XData.OldWorkMode = 55;
			break;
		}
		case 55:
		{
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer = PAC193X_ADDRESS+1;
			PAC193X_DelayTime(2);
			PAC193XData.CntByte = 0;
			PAC193XData.WorkMode = 231;
			PAC193XData.OldWorkMode = 56;
			break;
		}
		case 56:
		{//开始读取数据
			PAC193XData.CntBit=0;
			PAC193XData.RTxBuffer=0;
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 241;
			PAC193XData.OldWorkMode = 57;
			break;
		}
		case 57:
		{
			PAC193XData.VBuffer[PAC193XData.CntByte] = PAC193XData.RTxBuffer;
			PAC193XData.CntByte++;
			if(PAC193XData.CntByte < 8) 
			{
				PAC193XData.WorkMode = 243;
				PAC193XData.OldWorkMode = 56;
			}
			else 
			{
				PAC193XData.WorkMode = 247;
				PAC193XData.OldWorkMode = 58;
			}
			PAC193X_DelayTime(2);
			break;
		}
		case 58:
		{
			PAC193XData.WorkMode = 211;
			PAC193XData.OldWorkMode = 0;
			PAC193X_DelayTime(2);
			break;
		}
		//发送启动信号，进之前，保证SCL=1、SDA=1，退出时SCL=0，SDA=0
		case 201:
		{
			PAC193X_SDA(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 202;
			break;
		}
		case 202:
		{
			PAC193X_SCL(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = PAC193XData.OldWorkMode;
			break;
		}
		//发送结束信号,进之前，保证SCL=0，SDA不做要求，退出时SCL=1，SDA=1
		case 211:
		{
			PAC193X_SDA(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 212;
			break;
		}
		case 212:
		{
			PAC193X_SCL(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 213;
			break;
		}
		case 213:
		{
			PAC193X_SDA(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = PAC193XData.OldWorkMode;
			break;
		}
		//发送应答信号，进之前保证SCL=0、SDA不做要求；退出时SCL=0、SDA=0
		case 221:
		{
			PAC193X_SDA(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 222;
			break;
		}
		case 222:
		{
			PAC193X_SCL(1)
			PAC193X_DelayTime(3);
			PAC193XData.WorkMode = 223;
			break;
		}
		case 223:
		{
			PAC193X_SCL(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 224;
			break;
		}
		case 224:
		{
			PAC193X_SDA(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = PAC193XData.OldWorkMode;
			break;
		}
		//发送一个字节数据，进之前保证SCL=0、SDA不作要求，退出时SCL=0,SDA=1,AC193XData.CntBit=0
		//要发送的数据缓冲区为AC193XData.RTxBuffer
		case 231:
		{
			if(PAC193XData.RTxBuffer & B7) 
			{
				PAC193X_SDA(1)
			}
			else
			{
				PAC193X_SDA(0)
			}
			PAC193XData.RTxBuffer = PAC193XData.RTxBuffer<<1;	
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 232;	
			break;
		}
		case 232:
		{
			PAC193X_SCL(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 233;
			break;
		}
		case 233:
		{
			PAC193X_SCL(0)
			PAC193XData.CntBit++;
			PAC193X_DelayTime(2);
			if(PAC193XData.CntBit<8) PAC193XData.WorkMode = 231;
			else PAC193XData.WorkMode = 234;
			break;
		}
		case 234:
		{
			PAC193X_SDA(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 235;
			break;
		}
		case 235:
		{//判断应答
			PAC193X_SCL(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 236;
			break;
		}
		case 236:
		{
			PAC193X_SCL(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = PAC193XData.OldWorkMode;
			break;
		}
		//接收一个字节数据，进之前保证SCL=0、SDA=1不作要求，出时SCL=0,SDA=1,PAC193XData.CntBit=0,PAC193XData.RTxBuffer=0
		//要接收的数据缓冲区为PAC193XData.RTxBuffer
		case 241:
		{
			PAC193X_SCL(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 242;
			break;
		}
		case 242:
		{
			if(PAC193X_SDAIN()) PAC193XData.RTxBuffer |= B0;
			PAC193X_SCL(0)
			PAC193XData.CntBit++;
			PAC193X_DelayTime(2);
			if(PAC193XData.CntBit<8)
			{
				PAC193XData.WorkMode = 241;
				PAC193XData.RTxBuffer = PAC193XData.RTxBuffer<<1;
			}
			else PAC193XData.WorkMode = PAC193XData.OldWorkMode;
			break;
		}
		case 243:
		{//应答0
			PAC193X_SDA(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 244;
			break;
		}
		case 244:
		{
			PAC193X_SCL(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 245;
			break;
		}
		case 245:
		{
			PAC193X_SCL(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 246;
			break;
		}
		case 246:
		{
//			printf("接收的数据为：0x%X\r\n",PAC193XData.RTxBuffer);
			PAC193X_SDA(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = PAC193XData.OldWorkMode;
			break;
		}
		case 247:
		{//应答1
			PAC193X_SDA(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 244;
			break;
		}
		
		
		case 250:
		{
			PAC193XData.WorkMode = 211;
			PAC193XData.OldWorkMode = 0;
			PAC193X_DelayTime(2);
			break;
		}
	}
}
//---
/*
	函数名：void PAC193X_Operation(u16 mode,u8 *p,u8 len)
	描述：该函数为PAC193X.c的数据操作
*/
void PAC193X_Operation(u16 mode,u8 *p,u8 len)
{
	switch(mode)
	{
		//AD1参数读取
		case 2:
		{//读AD1原点值
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD1Orgin,p,2);
			break;
		}
		case 3:
		{//读AD1端点值
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD1Endpoint,p,2);
			break;
		}
		case 4:
		{//读AD1参考值
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD1Refer,p,2);
			break;
		}
		case 5:
		{//读AD1设备映射
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD1Map,p,1);
			break;
		}
		//AD2参数读取
		case 22:
		{//读AD2原点值
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD2Orgin,p,2);
			break;
		}
		case 23:
		{//读AD2端点值
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD2Endpoint,p,2);
			break;
		}
		case 24:
		{//读AD1参考值
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD2Refer,p,2);
			break;
		}
		case 25:
		{//读AD2设备映射
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD2Map,p,1);
			break;
		}
		//AD3参数读取
		case 42:
		{//读AD3原点值
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD3Orgin,p,2);
			break;
		}
		case 43:
		{//读AD3端点值
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD3Endpoint,p,2);
			break;
		}
		case 44:
		{//读AD3参考值
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD3Refer,p,2);
			break;
		}
		case 45:
		{//读AD3设备映射
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD3Map,p,1);
			break;
		}
		//AD1参数写入
		case 102:
		{//写入AD1原点值
			PAC193X_StoreData(PAC193X_AdrStoreAD1Orgin,p,2);
			break;
		}
		case 103:
		{//写入AD1端点值
			PAC193X_StoreData(PAC193X_AdrStoreAD1Endpoint,p,2);
			break;
		}
		case 104:
		{//写入AD1参考值
			PAC193X_StoreData(PAC193X_AdrStoreAD1Refer,p,2);
			break;
		}
		case 105:
		{//写入AD1设备映射
			PAC193X_StoreData(PAC193X_AdrStoreAD1Map,p,1);
			break;
		}
		//AD2参数写入
		case 122:
		{//写入AD2原点值
			PAC193X_StoreData(PAC193X_AdrStoreAD2Orgin,p,2);
			break;
		}
		case 123:
		{//写入AD2端点值
			PAC193X_StoreData(PAC193X_AdrStoreAD2Endpoint,p,2);
			break;
		}
		case 124:
		{//写入AD2参考值
			PAC193X_StoreData(PAC193X_AdrStoreAD2Refer,p,2);
			break;
		}
		case 125:
		{//写入AD2设备映射
			PAC193X_StoreData(PAC193X_AdrStoreAD2Map,p,1);
			break;
		}
		//AD3参数写入
		case 142:
		{//写入AD3原点值
			PAC193X_StoreData(PAC193X_AdrStoreAD3Orgin,p,2);
			break;
		}
		case 143:
		{//写入AD3端点值
			PAC193X_StoreData(PAC193X_AdrStoreAD3Endpoint,p,2);
			break;
		}
		case 144:
		{//写入AD3参考值
			PAC193X_StoreData(PAC193X_AdrStoreAD3Refer,p,2);
			break;
		}
		case 145:
		{//写入AD3设备映射
			PAC193X_StoreData(PAC193X_AdrStoreAD3Map,p,1);
			break;
		}
	}
}
//---
/*
	函数名：void PAC193X_Operation(u16 mode,u8 *p,u8 len)
	描述：PAC193X.c文件的数据操作
*/
//---
/*
	函数名：void PAC193X_Timer(void)
	描述：screen.c文件定时器函数，由系统1ms定时器调用
*/
void PAC193X_Timer(void)
{
	if(PAC193XData.Timer != 0)PAC193XData.Timer--;
	if(PAC193XData.CollectTimer != 0)PAC193XData.CollectTimer--;
}
//---
/*
	函数名：void PAC193X_Init(void)
	描述：	screen.c文件初始化函数
*/
void PAC193X_Init(void)
{
	PAC193X_InitGPIO();
	PAC193X_TimerInit();
	PAC193X_InitData();
	//启动采集
	PAC193X_DelayTime(2);
	PAC193XData.WorkMode = 1;
	PAC193XData.Timer = 100;
	while(PAC193XData.Timer);
}
//---
/*
	函数名：void PAC193X_InitData(void)
*/
void PAC193X_InitData(void)
{
	u8 bf[PAC193X_AdrStoreEND];
	u16 b,c;
	TwoBytes two;
	//AD1通道数据初始化
	PAC193X_ReadStoreData(PAC193X_AdrStoreStart,bf,PAC193X_AdrStoreEND);
	two.U8Data[0] = bf[PAC193X_AdrStoreAD1Orgin];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD1Orgin+1];
	PAC193XData.Facility[0].Orgin = two.U16Data;
	two.U8Data[0] = bf[PAC193X_AdrStoreAD1Endpoint];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD1Endpoint+1];
	b = two.U16Data;
	two.U8Data[0] = bf[PAC193X_AdrStoreAD1Refer];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD1Refer+1];
	c = two.U16Data;
	PAC193XData.Facility[0].Rate = (float)c/((s32)b-PAC193XData.Facility[0].Orgin);
	if((bf[PAC193X_AdrStoreAD1Map] == 1) || (bf[PAC193X_AdrStoreAD1Map] == 2) || (bf[PAC193X_AdrStoreAD1Map] == 3))
		PAC193XData.Facility[0].Map = bf[PAC193X_AdrStoreAD1Map];
	else 
	{
		two.U8Data[0] = 1;
		PAC193X_Operation(105,two.U8Data,1);
		PAC193XData.Facility[0].Map = 1;
	}
	//AD2通道数据初始化
	two.U8Data[0] = bf[PAC193X_AdrStoreAD2Orgin];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD2Orgin+1];
	PAC193XData.Facility[1].Orgin = two.U16Data;
	two.U8Data[0] = bf[PAC193X_AdrStoreAD2Endpoint];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD2Endpoint+1];
	b = two.U16Data;
	two.U8Data[0] = bf[PAC193X_AdrStoreAD2Refer];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD2Refer+1];
	c = two.U16Data;
	PAC193XData.Facility[1].Rate = (float)c/((s32)b-PAC193XData.Facility[1].Orgin);
	if((bf[PAC193X_AdrStoreAD2Map] == 1) || (bf[PAC193X_AdrStoreAD2Map] == 2) || (bf[PAC193X_AdrStoreAD2Map] == 3))
		PAC193XData.Facility[1].Map = bf[PAC193X_AdrStoreAD2Map];
	else 
	{
		two.U8Data[0] = 1;
		PAC193X_Operation(125,two.U8Data,1);
		PAC193XData.Facility[1].Map = 1;
	}
	//AD3通道数据初始化
	two.U8Data[0] = bf[PAC193X_AdrStoreAD3Orgin];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD3Orgin+1];
	PAC193XData.Facility[2].Orgin = two.U16Data;
	two.U8Data[0] = bf[PAC193X_AdrStoreAD3Endpoint];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD3Endpoint+1];
	b = two.U16Data;
	two.U8Data[0] = bf[PAC193X_AdrStoreAD3Refer];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD3Refer+1];
	c = two.U16Data;
	PAC193XData.Facility[2].Rate = (float)c/((s32)b-PAC193XData.Facility[2].Orgin);
	if((bf[PAC193X_AdrStoreAD3Map] == 1) || (bf[PAC193X_AdrStoreAD3Map] == 2) || (bf[PAC193X_AdrStoreAD3Map] == 3))
		PAC193XData.Facility[2].Map = bf[PAC193X_AdrStoreAD3Map];
	else 
	{
		two.U8Data[0] = 1;
		PAC193X_Operation(145,two.U8Data,1);
		PAC193XData.Facility[2].Map = 1;
	}
	PAC193XData.Address = PAC193X_ADDRESS;
	
	printf("\r\n");
	for(b=0;b<3;b++)
	{
		printf("AD%u通道原点值%u,比率%f,映射",b+1,PAC193XData.Facility[b].Orgin,PAC193XData.Facility[b].Rate);
		if(PAC193XData.Facility[b].Map == 1)	printf("回转数据采集\r\n");
		else if(PAC193XData.Facility[b].Map == 2)	printf("变幅数据采集\r\n");
		else if(PAC193XData.Facility[b].Map == 3)	printf("吊高数据采集\r\n");
	}
}
//---
/*
	函数名：void PAC193X_CalMapValue(PAC193X_TowerStructure *p)
	描述：根据AD采集的映射对象，计算映射值
*/
void PAC193X_CalMapValue(PAC193X_TowerStructure *p)
{
	if(p->Map == 1)
	{//计算回转
		p->MapValue = (p->ADValue - p->Orgin) * p->Rate;
		while(p->MapValue > 360)
			p->MapValue -=360;
		while(p->MapValue < 0)
			p->MapValue +=360;
		Tower_GetSlewingData()->Rotation = p->MapValue;
	}
	else if(p->Map == 2)
	{//计算变幅
		p->MapValue = (p->ADValue - p->Orgin) * p->Rate;
//		while(p->MapValue > 360)
//			p->MapValue -=360;
//		while(p->MapValue < 0)
//			p->MapValue +=360;
		Tower_GetTrolleyingData()->Range = p->MapValue*1000;
	}
	else if(p->Map == 3)
	{//计算吊高
		p->MapValue = (p->ADValue - p->Orgin) * p->Rate;
//		while(p->MapValue > 360)
//			p->MapValue -=360;
//		while(p->MapValue < 0)
//			p->MapValue +=360;
		Tower_GetHoistingData()->HookHeight =  p->MapValue*1000;
	}
	
//	PAC193XData.Facility[0].MapValue = ((s32)PAC193XData.ADValue[0] - PAC193XData.Facility[0].Orgin)* PAC193XData.Facility[0].Rate;
//	PAC193XData.Facility[1].MapValue = ((s32)PAC193XData.ADValue[1] - PAC193XData.Facility[1].Orgin)* PAC193XData.Facility[1].Rate;
//	PAC193XData.Facility[2].MapValue = ((s32)PAC193XData.ADValue[2] - PAC193XData.Facility[2].Orgin)* PAC193XData.Facility[2].Rate;
//	//回转
//	if(PAC193XData.Facility[0].Map == 1)
//		Tower_GetSlewingData()->Rotation = PAC193XData.Facility[0].MapValue;
//	else if(PAC193XData.Facility[1].Map == 1)
//		Tower_GetSlewingData()->Rotation = PAC193XData.Facility[0].MapValue;
//	else if(PAC193XData.Facility[2].Map == 1)
//		Tower_GetSlewingData()->Rotation = PAC193XData.Facility[0].MapValue;
//	//变幅
//	if(PAC193XData.Facility[0].Map == 2)
//		Tower_GetTrolleyingData()->Range = PAC193XData.Facility[1].MapValue;
//	else if(PAC193XData.Facility[1].Map == 2)
//		Tower_GetTrolleyingData()->Range = PAC193XData.Facility[1].MapValue;
//	else if(PAC193XData.Facility[2].Map == 2)
//		Tower_GetTrolleyingData()->Range = PAC193XData.Facility[1].MapValue;
//	//吊高
//	if(PAC193XData.Facility[0].Map == 3)
//		Tower_GetHoistingData()->HookHeight = PAC193XData.Facility[2].MapValue;
//	else if(PAC193XData.Facility[1].Map == 3)
//		Tower_GetHoistingData()->HookHeight = PAC193XData.Facility[2].MapValue;
//	else if(PAC193XData.Facility[2].Map == 3)
//		Tower_GetHoistingData()->HookHeight = PAC193XData.Facility[2].MapValue;
		
}
//---
/*
	函数名：void PAC193X_Cal_Rotation(PAC193X_TowerStructure *p)
*/
void PAC193X_Cal_Rotation(PAC193X_TowerStructure *p)
{
	if(p->Map == 1)
	{//计算回转
		p->MapValue = (p->ADValue - p->Orgin) * p->Rate;
		while(p->MapValue > 360)
			p->MapValue -=360;
		while(p->MapValue < 0)
			p->MapValue +=360;
		
		
		
	
	}
}
//---
/*
	函数名：void PAC193X_InitGPIO(void)
	描述：PAC193X通讯控制管脚初始化
*/
void PAC193X_InitGPIO(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(PAC193X_SCL_RCC,ENABLE);
  RCC_APB2PeriphClockCmd(PAC193X_SDA_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(PAC193X_SCLIN_RCC,ENABLE);
  RCC_APB2PeriphClockCmd(PAC193X_SDAIN_RCC,ENABLE);
  RCC_APB2PeriphClockCmd(PAC193X_PWRDN_RCC,ENABLE);
  RCC_APB2PeriphClockCmd(PAC193X_SLAL_RCC,ENABLE);
	//输出配置
	GPIO_InitStructure.GPIO_Pin = PAC193X_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_Init(PAC193X_SCL_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PAC193X_SDA_PIN;
	GPIO_Init(PAC193X_SDA_PORT, &GPIO_InitStructure);
	//输入配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin = PAC193X_SCLIN_PIN;
	GPIO_Init(PAC193X_SCLIN_PORT, &GPIO_InitStructure);//设置温感器1输入管脚
	GPIO_InitStructure.GPIO_Pin = PAC193X_SDAIN_PIN;
	GPIO_Init(PAC193X_SDAIN_PORT, &GPIO_InitStructure);//设置温感器2输入管脚
	//PWRDN/SLAL
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin =  PAC193X_PWRDN_PIN;
  GPIO_Init(PAC193X_PWRDN_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin =  PAC193X_SLAL_PIN;
  GPIO_Init(PAC193X_SLAL_PORT, &GPIO_InitStructure);
	PAC193X_SCL(1);
	PAC193X_SDA(1);
	PAC193X_PWRDN(1);
	PAC193X_SLAL(0);
}
//---
/*
	函数名：void PAC193X_TimerInit(void)
	描述：	该函数为对DS18B20定时器进行初始化
	说明：	将定时器初始化为0.5us，使用到定时器2
*/
void PAC193X_TimerInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 1000-1;				//计数值1000   
	TIM_TimeBaseStructure.TIM_Prescaler = 18-1;    		//预分频,此值+1为分频的除数,72/36=4MHz.0.25us
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//将定时器的中断等级设为最低
	NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE); 
//	TIM_Cmd(TIM2, ENABLE); 
}
//---
/*
	函数名：void DS18B20_DelayTime(u16 tim)
	描述：	该函数为用于设置定时器延时值，并启动定时器
	输入：	u16 tim延时值，单位0.25us
*/
void PAC193X_DelayTime(u16 tim)
{
	TIM2->ARR = tim; 
	TIM2->CNT = 0;
	TIM2->CR1  |= B0;//启动定时器
}
//---
/*
	通用定时器3中断，整个系统定时器
*/
void TIM2_IRQHandler(void)
{
	if(TIM2->SR & 0X0001)	//溢出中断
	{
		TIM2->CR1  &= ~B0;//关闭定时器
		PAC193X_Work();
		TIM2->SR &= ~(1<<0);	//清除中断标志位
	}
}
//---
/*
	函数名：PAC193X_DataStructure* PAC193X_GetData(void)
	描述：获取返回screen.c文件全局变量结构体
*/
PAC193X_DataStructure* PAC193X_GetData(void)
{
	return &PAC193XData;
}
//---
//-------------------------数据读取、存储----------------------------
//---
/*
	函数名：void PAC193X_ReadStoreData(u8 address,u8 *p,u8 len)
	描述：	读取数据
*/
void PAC193X_ReadStoreData(u8 address,u8 *p,u8 len)
{
	W25Xxx_ReadData(p,PAC193X_AdrStoreBase+address,len);
}
//---
/*
	函数名：void PAC193X_StoreData(u8 address,u8 *p,u8 len)
	描述：该函数为存储PAC193X配置参数函数
	说明：读取数据
				擦除数据
				修改数据
				保存数据
*/
void PAC193X_StoreData(u8 address,u8 *p,u8 len)
{
	u8 buffer[PAC193X_AdrStoreEND],cnt;
	W25Xxx_ReadData(buffer,PAC193X_AdrStoreBase,PAC193X_AdrStoreEND);//读所有配置参数
	W25Xxx_EraseSector(PAC193X_AdrStoreBase);//擦除数据
	for(cnt=0;cnt<len;cnt++)
	{
		buffer[address++] = *p;
		p++;
	}
	W25Xxx_PageWrite(buffer,PAC193X_AdrStoreBase,PAC193X_AdrStoreEND);//保存数据
	
	PAC193X_InitData();
}
//---
//-------------------------调试函数----------------------------
//---
/*
	函数名： void PAC193X_DebugASCII(u8 *p,u8 len)
	描述：		该函数为PAC193X文件的ASCII调试文件
*/
void PAC193X_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1://:000|88|001
		{//读取所有配置参数
			u8 bf[PAC193X_AdrStoreEND];
			TwoBytes two;
			PAC193X_ReadStoreData(PAC193X_AdrStoreStart,bf,PAC193X_AdrStoreEND);
			//AD1
			two.U8Data[0] = bf[PAC193X_AdrStoreAD1Orgin];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD1Orgin+1];
			printf("AD1原点值=%u\r\n",two.U16Data);
			two.U8Data[0] = bf[PAC193X_AdrStoreAD1Endpoint];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD1Endpoint+1];
			printf("AD1端点值=%u\r\n",two.U16Data);
			two.U8Data[0] = bf[PAC193X_AdrStoreAD1Refer];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD1Refer+1];
			printf("AD1参照值=%u\r\nAD1映射",two.U16Data);
			if(bf[PAC193X_AdrStoreAD1Map] == 1)	printf("回转数据采集\r\n");
			else if(bf[PAC193X_AdrStoreAD1Map] == 2)	printf("变幅数据采集\r\n");
			else if(bf[PAC193X_AdrStoreAD1Map] == 3)	printf("吊高数据采集\r\n");
			else  
			{
				two.U8Data[0] = 1;
				PAC193X_Operation(105,two.U8Data,1);
				printf("回转数据采集\r\n");
			}
			//AD2
			two.U8Data[0] = bf[PAC193X_AdrStoreAD2Orgin];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD2Orgin+1];
			printf("AD2原点值=%u\r\n",two.U16Data);
			two.U8Data[0] = bf[PAC193X_AdrStoreAD2Endpoint];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD2Endpoint+1];
			printf("AD2端点值=%u\r\n",two.U16Data);
			two.U8Data[0] = bf[PAC193X_AdrStoreAD2Refer];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD2Refer+1];
			printf("AD2参照值=%u\r\nAD2映射",two.U16Data);
			if(bf[PAC193X_AdrStoreAD2Map] == 1)	printf("回转数据采集\r\n");
			else if(bf[PAC193X_AdrStoreAD2Map] == 2)	printf("变幅数据采集\r\n");
			else if(bf[PAC193X_AdrStoreAD2Map] == 3)	printf("吊高数据采集\r\n");
			else  
			{
				two.U8Data[0] = 1;
				PAC193X_Operation(125,two.U8Data,1);
				printf("回转数据采集\r\n");
			}
			//AD3
			two.U8Data[0] = bf[PAC193X_AdrStoreAD3Orgin];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD3Orgin+1];
			printf("AD3原点值=%u\r\n",two.U16Data);
			two.U8Data[0] = bf[PAC193X_AdrStoreAD3Endpoint];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD3Endpoint+1];
			printf("AD3端点值=%u\r\n",two.U16Data);
			two.U8Data[0] = bf[PAC193X_AdrStoreAD3Refer];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD3Refer+1];
			printf("AD3参照值=%u\r\nAD3映射",two.U16Data);
			if(bf[PAC193X_AdrStoreAD3Map] == 1)	printf("回转数据采集\r\n");
			else if(bf[PAC193X_AdrStoreAD3Map] == 2)	printf("变幅数据采集\r\n");
			else if(bf[PAC193X_AdrStoreAD3Map] == 3)	printf("吊高数据采集\r\n");
			else  
			{
				two.U8Data[0] = 1;
				PAC193X_Operation(145,two.U8Data,1);
				printf("回转数据采集\r\n");
			}
			break;
		}
		case 2:
		{
			break;
		}
		case 101:
		{
			break;
		}
		case 501:
		{//发起始信号
			printf("发起始信号\r\n");
			PAC193X_SCL(1)
			PAC193X_SDA(1)
			PAC193XData.WorkMode = 201;
			PAC193XData.OldWorkMode = 250;
			PAC193X_DelayTime(2);//初始化，拉低500us
			break;
		}
		case 502:
		{//发停止信号
			printf("发结束信号\r\n");
			PAC193X_SCL(0)
			PAC193XData.WorkMode = 211;
			PAC193XData.OldWorkMode = 250;
			PAC193X_DelayTime(2);//初始化，拉低500us
			break;
		}
		case 503:
		{//发送应答信号
			printf("发应答信号\r\n");
			PAC193X_SCL(0)
			PAC193XData.WorkMode = 221;
			PAC193XData.OldWorkMode = 250;
			PAC193X_DelayTime(2);//初始化，拉低500usak;
		}
		case 504:
		{//发送一个字节数据
			printf("发送一个字节数据\r\n");
			PAC193X_SCL(0)
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer = 0xAA;
			PAC193XData.WorkMode = 231;
			PAC193XData.OldWorkMode = 250;
			PAC193X_DelayTime(2);//初始化，拉低500usak;
			break;
		}
		case 505:
		{//读取一个字节数据
			printf("读取一个字节数据\r\n");
			PAC193X_SCL(0)
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer= 0;
			PAC193XData.RTxBuffer = 0xAA;
			PAC193XData.WorkMode = 241;
			PAC193XData.OldWorkMode = 250;
			PAC193X_DelayTime(2);//初始化，拉低500usak;
			break;
		}
		case 506:
		{//读取电压数据
			printf("读取电压数据\r\n");
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 51;
			break;
		}
		case 507:
		{//刷新电压数据
			printf("刷新电压数据\r\n");
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 1;
			break;
		}
		case 520:
		{//返回电压数据
			u8 i;
			for(i=0;i<4;i++)
			{
				printf("通道%u电压为：%fV\r\n",i+1,PAC193XData.Voltage[i]);
			}
			break;
		}
	}
}
//---
/*
	函数名： 	void PAC193X_DebugRTU((u8 *p)
	描述：		该函数为PAC193X文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*/
void PAC193X_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//查看是哪个功能码
	{
		case 0x01:
		{
			break;
		}
	}
}
//---





































