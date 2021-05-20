/*
	本文件为该项目的相关函数
	具体函数如下

*/

#include "collect.h"

Collect_DataStructure CollectData;
/*
	函数名：void Collect_main(void)
	描述：	该函数为采集函数主函数，由主函数调用
*/
void Collect_main(void)
{
	switch(CollectData.RunMode)
	{
		//发送A1命令处理
		case 1:
		{
			if(CollectData.Timer == 0)
			{
				Server_WritNewModule();
				CollectData.Timer = 300;
				CollectData.TimerTwo = 2000;
				CollectData.RunMode = 2;
			}
			break;
		}
		case 2:
		{
			if(Server_GetTowerCraneData()->RunMode == 0)
			{
				CollectData.Timer = 30;
				CollectData.RunMode = 11;
			}
			else if(CollectData.Timer == 0)
			{
				printf("A1新模块故障\r\n");
				CollectData.RunMode = 11;
			}
			break;
		}
		//-----读电量传感器-----
		case 11:
		{
			if(CollectData.Timer==0)
			{
				Ohrd23_ReadBase();
				CollectData.Timer = 300;
				CollectData.RunMode = 12;
			}
			break;
		}
		case 12:
		{
			if(Ohrd23_GetOhrd23Data()->CollectMode  == 0)
			{
				if(Ohrd23_GetOhrd23Data()->CollectCommand != 0)
				{
					CollectData.Timer = 30;
					CollectData.RunMode = 13;
				}
				else
				{
					CollectData.RunMode = 21;
				}
			}
			else if(CollectData.Timer == 0)
			{
				printf("电量传感器故障\r\n");
				CollectData.RunMode = 21;
			}
			break;
		}
		case 13:
		{
			CollectData.RunMode = 21;
			break;
		}
		//-----读风速传感器------
		case 21:
		{
			if(CollectData.Timer == 0)
			{
				YCFS260_CollectWindSpeed();
				CollectData.Timer = 300;
				CollectData.RunMode = 22;
			}
			break;
		}
		case 22:
		{
			if(Wind_GetWindData()->RunMode == 0)
			{
				CollectData.Timer = 30;
				CollectData.RunMode = 31;
			}
			else if(CollectData.Timer == 0)
			{
				printf("风速传感器故障\r\n");
				CollectData.RunMode = 31;
			}
			break;
		}
		//-----读称重传感器-----
		case 31:
		{
			if(CollectData.Timer == 0)
			{
				Thbss01_Operation(2,0,0);
				CollectData.Timer = 400;
				CollectData.RunMode = 32;
			}
			break;
		}
		case 32:
		{
			if(Thbss01_GetThbss01Data()->CollectMode == 0)
			{
				CollectData.Timer = 30;
				CollectData.RunMode = 41;
			}
			else if(CollectData.Timer == 0)
			{
				printf("称重传感器故障\r\n");
				CollectData.RunMode = 200;
			}
			break;
		}
		//-----读编码器1编码值------
		case 41:
		{
			if(CollectData.Timer == 0)
			{
				MCH58_CollectValue(&MCH58_GetMCH58Data()->Mch58[0]);
				CollectData.Timer = 300;
				CollectData.RunMode = 42;
			}
			break;
		}
		case 42:
		{
			if(MCH58_GetMCH58Data()->CollectMode == 0)
			{
				CollectData.Timer = 30;
				CollectData.RunMode = 51;
			}
			else if(CollectData.Timer == 0)
			{
				printf("编码器1故障\r\n");
				CollectData.RunMode = 51;
			}
			break;
		}
		//-----读编码器2编码值-----
		case 51:
		{
			if(CollectData.Timer == 0)
			{
				MCH58_CollectValue(&MCH58_GetMCH58Data()->Mch58[1]);
				CollectData.Timer = 300;
				CollectData.RunMode = 52;
			}
			break;
		}
		case 52:
		{
			if(MCH58_GetMCH58Data()->CollectMode == 0)
			{
				CollectData.Timer = 30;
				CollectData.RunMode = 61;
			}
			else if(CollectData.Timer == 0)
			{
				printf("编码器2故障\r\n");
				CollectData.RunMode = 61;
			}
			break;
		}
		//-----读变码器3编码值-----
		case 61:
		{
			if(CollectData.Timer == 0)
			{
				MCH58_CollectValue(&MCH58_GetMCH58Data()->Mch58[2]);
				CollectData.Timer = 300;
				CollectData.RunMode = 62;
			}
			break;
		}
		case 62:
		{
			if(MCH58_GetMCH58Data()->CollectMode == 0)
			{
				CollectData.RunMode = 200;
			}
			else if(CollectData.Timer == 0)
			{
				printf("编码器3故障\r\n");
				CollectData.RunMode = 200;
			}
			break;
		}
		
		case 200:
		{
			if(CollectData.TimerTwo == 0)
			{
				CollectData.RunMode = 1;
			}
			else
			{
				CollectData.Timer = 30;
				CollectData.RunMode = 11;
			}
			break;
		}
	}
}
//---
/*
	函数名：void Collect_Init(void)
	描述：	该函数为collect.c文件初始化函数
*/
void Collect_Init(void)
{
	Collect_CollectDataInit();
}
//---
/*
	函数名：void Collect_Timer(void)
	描述：	该函数为collect.c文件定时器函数，1ms，由系统1ms定时器调用
*/
void Collect_Timer(void)
{
	if(CollectData.Timer != 0)CollectData.Timer--;
	if(CollectData.TimerTwo != 0)CollectData.TimerTwo--;
}
//---
/*
	函数名：Collect_DataStructure* Collect_GetCollectData(void)
	说明：	获取CollectData数据地址指针
*/
Collect_DataStructure* Collect_GetCollectData(void)
{
	return &CollectData;
}
//---
/*
	函数名：void Collect_CollectDataInit(void)
	描述：	初始化CollectData数据
*/
void Collect_CollectDataInit(void)
{
	CollectData.RunMode = 1;
}

//---
/*
	函数名：float Tower_CalAngle(u32 buffer)
	描述：	计算塔机回转角度					
*/
float Collec_CalSlewing(u32 buffer)
{
	s32 bf=0;
	if(MCH58_GetMCH58Data()->Mch58[0].Map == 1)
		bf = MCH58_GetMCH58Data()->Mch58[0].CodedValue - MCH58_GetMCH58Data()->Mch58[0].ZeroValue;
	else if(MCH58_GetMCH58Data()->Mch58[1].Map == 1)
		bf = MCH58_GetMCH58Data()->Mch58[1].CodedValue - MCH58_GetMCH58Data()->Mch58[1].ZeroValue;
	else if(MCH58_GetMCH58Data()->Mch58[2].Map == 1)
		bf = MCH58_GetMCH58Data()->Mch58[2].CodedValue - MCH58_GetMCH58Data()->Mch58[2].ZeroValue;
	Tower_GetSlewingData()->Rotation = (float)((360.0*bf)/(((double)Tower_GetSlewingData()->BigGear / Tower_GetSlewingData()->LittleGear)*TOWER_EnDiv));
	return 0;
}
//---
//---
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Collect_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Collect文件的ASCII调试文件
*/
void Collect_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:
		{//系统复位
			CollectData.RunMode = 1;
			printf("继续上传数据\r\n");
			break;
		}
		case 2:
		{//:000|56|002
			CollectData.RunMode = 0;
			printf("停止上传数据\r\n");
			break;
		}
	}
}
//---
/*
	函数名： void Project_DebugRTU((u8 *p)
	描述：		该函数为Project文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*/
void Collect_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//查看是哪个功能码
	{
		//-----------------------------------------
		case 0x01:
		{//复位系统
			NVIC_SystemReset();		//系统软件复位
			break;
		}
		case 0x02:
		{//置位Projec_SysBuffer所有的数据
			Project_ClearProjectSysBuffer();
			break;
		}
		case 0x03:
		{//
			
			break;
		}
		case 0x04:
		{//调试命令0x04
			break;
		}
	}
}
//---




