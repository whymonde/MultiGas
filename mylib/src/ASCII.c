/*
	本文件建与2017年7月28日
	该文件为用于本系统与其他系统之间的RTU通讯
通讯格式
	上位机对下位机：起始码、地址码、功能码、命令码、数据长度码、数据1···数据n、校验码高、校验码低、停止码
	下位机对上位机：起始码、本机地址码、功能码、命令码、执行结果返回码、数据长度位、数据1···数据n、校验码、停止码
位码：
	起始码：0XAA
	地址码：0X01~0XFF，0X00为特殊地址码
	功能码：指示当前的功能
	数据长度码：可有可无，0X00~0XFF
	校验码：
	停止码：0X55
*/
#include "ASCII.h"
#include "Button.h"


/*
	函数名： void ASCII_DebugCmd(u8 *p,u8 len)
	描述：	 控制系统外设设备
	输入：	 指令码首地址，说明
					 0~2个为指令码，第3个为分隔符"|"，4~5个为数据长度，第6个为分隔符"|",7~7+n个为数据
	格式：	 :000|01|······
*/
void ASCII_CommunicateReceive(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,2);//计算命令
	switch(cmd)
	{
		case 1:
		{
			Flashisp_DebugASCII(p+3,len-3);
			break;
		}
		case 2:
		{
			Screen_DebugASCII(p+3,len-3);
			break;
		}
		case 3:
		{
			File_DebugASCII(p+3,len-3);
			break;
		}
		case 4:
		{
			Flux_DebugASCII(p+3,len-3);
			break;
		}
		case 5:
		{
			Print_DebugASCII(p+3,len-3);
			break;
		}
		case 6:
		{
			Button_DebugASCII(p+3,len-3);
			break;
		}
		case 7:
		{
			Battery_DebugASCII(p+3,len-3);
			break;
		}
		case 8:
		{
			Slave_DebugASCII(p+3,len-3);
			break;
		}
		case 9:
		{
			Usb_DebugASCII(p+3,len-3);
			break;
		}		
		case 90:
		{
			Project_DebugASCII(p+3,len-3);
			break;
		}
			case 10:
		{
			Flux_DebugASCII(p+3,len-3);
			break;
		}
		case 11:
		{
			AT_DebugASCII(p+3,len-3);
			break;
		}
		case 12:
		{
			APP_DebugASCII(p+3,len-3);
			break;
		}
		default:
		{
			printf("ASCII.c接收到无效调试指令%u!\r\n",cmd);
			break;
		}
	}
}
//---
