/*
	本文件为该项目与电脑通讯的相关函数文件
	具体函数如下
*/
#include "computer.h"

/*
	函数名：void Computer_main(void)
	描述：	该函数为computer.c文件主函数，由系统主函数调用
*/
void Computer_main(void)
{
	
}
//---
/*
	函数名：void Computer_Init(void)
	描述：	该函数为computer.c文件初始化函数，有系统初始化函数调用
*/
void Computer_Init(void)
{
	
}
//---
/*
	函数名：void Computer_Timer(void)
	描述：	该函数为computer.c文件定时器，由系统1ms定时器函数调用
*/
void Computer_Timer(void)
{
	
}
//---
/*
	函数名：void Computer_DebugRTU(u8 *p,u8 len)
	描述：	该函数为处理来自电脑发送的数据
*/
void Computer_DebugRTU(u8 *p,u8 len)
{
	switch(*(p+1))
	{
		case 0x00:
		{//首页数据显示
			switch(*(p+2))
			{//主页面数据显示
				case 0x01:
				{//查询当前实时数据
					break;
				}
			}
			break;
		}
		case 0x01:
		{//塔机相关
			switch(*(p+2))
			{
				case 0x01:
				{//读塔机基本信息
					break;
				}
				case 0x21:
				{//读回转参数
					break;
				}
				case 0x41:
				{//读回转参数
					break;
				}
				case 0x61:
				{//读回转参数
					break;
				}
			}
			break;
		}
		case 0x02:
		{//编码器相关
			switch(*(p+2))
			{
				case 1:
				{//查询编码器1实时值
					
					break;
				}
				case 2:
				{//查询编码器1零点值
					break;
				}
				case 0x21:
				{//查询编码器2实时值
					break;
				}
				case 0x22:
				{//查询编码器2零点值
					break;
				}
				case 0x41:
				{//查询编码器3实时值
					break;
				}
				case 0x42:
				{//查询编码器3零点值
					break;
				}
				
			}
			break;
		}
		case 0x03:
		{//称重相关
			switch(*(p+2))
			{//称重相关
				case 0x01:
				{//读当前称重实时值
					break;
				}
				case 0x81:
				{//写入零点值
					break; 
				}
				case 0x82:
				{//写入砝码值
					break; 
				}
				case 0x83:
				{//写入砝码重量
					break; 
				}
				case 0x84:
				{//去皮
					break; 
				}
				case 0x85:
				{//取消去皮
					break; 
				}
			}
			break;
		}
		case 0x04:
		{//电量相关
			switch(*(p+2))
			{
				case 0x01:
				{//读取基本电量参数
					break;
				}
				case 0x02:
				{//读取电能参数
					break;
				}
				case 0x03:
				{//读取配置参数
					break;
				}
				case 0x81:
				{//设置电压倍率
					break;
				}
				case 0x82:
				{//设置电流倍率
					break;
				}
				case 0x83:
				{//设置接线方式
					break;
				}
				case 0x84:
				{//设置通讯地址
					break;
				}
				case 0x85:
				{//设置通讯波特率
					break;
				}
				case 0x12:
				{//读取电能参数
					break;
				}
			}
			break;
		}
		case 0x05:
		{//风速传感器
			switch(*(p+2))
			{
				case 0x01:
				{//读风速值
					break;
				}
				case 0x02:
				{//读配置参数
					break;
				}
				case 0x81:
				{//设置地址
					break;
				}
				case 0x82:
				{//设置波特率
					break;
				}
			}
			break;
		}
		case 0x06:
		{//温度传感器
			break;
		}
		case 0x07:
		{//系统相关
			break;
		}
	}
}
//---
void Computer_Send(u8 *p,u8 len)
{
	u8 buffer[30];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = 0xF1;//地址
	for(crc=0;crc<len;crc++)
	{
		buffer[cnt++]= *p;
		p++;
	}
	crc = Calculate_CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc;//CRC低字节，2个字节
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	for(crc=0;crc<cnt;crc++)
	{
		Server_SendBufferData(RTU_UsartComputerPort,RTU_UsartComputerBuffer,buffer[crc]);//发送数据
	}
}
//---











