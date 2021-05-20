/*
	本文件为该项目的屏幕相关函数
	具体函数如下
*/

#include "screen.h"

Screen_DataStructure ScreenData;
/*
	函数名：void Screen_Main(void)
	描述：	heat.c主函数
*/
void Screen_Main(void)
{
	
}
//---
/*
	函数名：void Screen_Timer(void)
	描述：screen.c文件定时器函数，由系统1ms定时器调用
*/
void Screen_Timer(void)
{
	
}
//---
/*
	函数名：void Screen_Init(void)
	描述：	screen.c文件初始化函数
*/
void Screen_Init(void)
{
	
}
//---
/*
	函数名：Screen_DataStructure* Screen_GetData(void)
	描述：获取返回screen.c文件全局变量结构体变量
*/
Screen_DataStructure* Screen_GetData(void)
{
	return &ScreenData;
}//-------------------------数据读取、存储----------------------------
//---
/*
	函数名：void Screen_ReadData(u8 address,u8 *p,u8 len)
	描述：	读取数据
*/
void Screen_ReadData(u8 address,u8 *p,u8 len)
{
//	W25Xxx_ReadData(p,AD7689_AdrStoreBase+address,len);
}
//---
/*
	函数名：void Screen_StoreData(u8 address,u8 *p,u8 len)
	描述：该函数为存储Screen.c配置参数函数
	说明：读取数据
				擦除数据
				修改数据
				保存数据
*/
void Screen_StoreData(u8 address,u8 *p,u8 len)
{
//	u8 buffer[AD7689_AdrStoreEND],cnt;
//	W25Xxx_ReadData(buffer,AD7689_AdrStoreBase,AD7689_AdrStoreEND);//读所有配置参数
//	W25Xxx_EraseSector(AD7689_AdrStoreBase);//擦除数据
//	for(cnt=0;cnt<len;cnt++)
//	{
//		buffer[address++] = *p;
//		p++;
//	}
//	W25Xxx_PageWrite(buffer,AD7689_AdrStoreBase,AD7689_AdrStoreEND);//保存数据
}
//---
/*
	函数名：void Screen_FactoryReset(void)
	描述：该函数Screen.c存储数据恢复出厂设置
*/
void Screen_FactoryReset(void)
{
	//AD4通道
//	i = 4;
//	AD7689_StoreData(AD7689_AdrStoreAD4Map,&i,1);//设备映射
//	two.U16Data = 100;
//	AD7689_StoreData(AD7689_AdrStoreAD4Orgin,two.U8Data,2);//原点值
//	two.U16Data = 60000;
//	AD7689_StoreData(AD7689_AdrStoreAD4Endpoint,two.U8Data,2);//端点值
//	four.FloatData = 50000.0;
//	AD7689_StoreData(AD7689_AdrStoreAD4Refer,four.U8Data,4);//参照值
	//初始化标志位
//	two.U8Data[0] = AD7689_FACTORY;
//	AD7689_StoreData(AD7689_AdrStoreFactory,two.U8Data,1);//恢复出厂设置标志
	printf("Screen参数初始化\r\n");
}
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Screen_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Screen.c文件的ASCII调试文件
*/
void Screen_DebugASCII(u8 *p,u8 len)
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
/*
	函数名： 	void Screen_DebugRTU((u8 *p)
	描述：		该函数为Screen.c文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*//*
void Screen_DebugRTU(u8 *p,u8 len)
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





































