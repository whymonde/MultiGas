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
#include "RTU.h"

RTU_DataStructure RTUData;
//---
/*
	函数名：void RTU_main(void)
	描述：	该函数为RTU.c主函数
*/
void RTU_main(void)
{
	/*
	switch(RTUData.RunMode)
	{
		case 1:
		{
			Flux_CommSendData(0xAA);
			RTUData.Error = 0xFF;
			RTUData.Timer = 100;
			RTUData.RunMode = 2;
			break;
		}
		case 2:
		{
			if(RTUData.Error == 0)
			{
//				printf("通讯成功\r\n");
				RTUData.ErrorCnt = 0;
				RTUData.Timer = 2;
				RTUData.NewMode = 1;
				RTUData.RunMode = 251;
			}
			else if(RTUData.Timer == 0)
			{//规定时间内没有接收到数据，认为故障
				if(RTUData.ErrorCnt < 20) 
				{
					RTUData.ErrorCnt++;//通讯错误统计
				}
				else
				{//故障
					Flux_GetData()->HJTem.Actual 					 = FLUX_ColFAULT; 	//环境温度
					Slave_GetData()->AGPress.Value = FLUX_ColFAULT;		//A路管压（计压）
					Slave_GetData()->AGTem.Value 	 = FLUX_ColFAULT;		//A路管温（计温）
					Flux_GetData()->ChannelA.CTem.Actual 	 = FLUX_ColFAULT;		//A路差温（流温）
					Slave_GetData()->ACPress.Value = FLUX_ColFAULT;		//A路差压（流压）
					Slave_GetData()->BGPress.Value = FLUX_ColFAULT;		//B路管压（计压）
					Slave_GetData()->BGTem.Value 	 = FLUX_ColFAULT;		//B路管温（计温）
					Slave_GetData()->BCPress.Value = FLUX_ColFAULT;		//B路差压（流压）
					Flux_GetData()->ChannelB.CTem.Actual 	 = FLUX_ColFAULT;		//B路差温（流温）
					Slave_GetData()->TGPress.Value = FLUX_ColFAULT;		//TSP路静压（计压）
					Flux_GetData()->ChannelT.GTem.Actual 	 = FLUX_ColFAULT;		//TSP路静温（计温）
					Slave_GetData()->TCPress.Value = FLUX_ColFAULT;		//TSP路差压（流压）
					Flux_GetData()->ChannelT.CTem.Actual 	 = FLUX_ColFAULT;		//TSP路差温（流温）
					Flux_GetData()->DQPress.Actual 				 = FLUX_ColFAULT;		//大气压
					Flux_GetData()->DQTem.Actual 					 = FLUX_ColFAULT;		//大气温
//					printf("主从通讯故障\r\n");
				}
				RTUData.Timer = 2;
				RTUData.NewMode = 1;
				RTUData.RunMode = 251;
			}
			break;
		}
		case 251:
		{
			if(RTUData.Timer == 0) RTUData.RunMode = RTUData.NewMode;
			break;
		}
	}
	*/
}
//---
/*
	函数名：void RTU_Timer(void)
	描述：RTU.c文件定时器函数，由系统1ms定时器调用
*/
void RTU_Timer(void)
{
//	if(RTUData.Timer != 0) RTUData.Timer--;
}
//---
/*
	函数名：void RTU_Init(void)
	描述：	该函数为RUT.c文件初始化
*/
void RTU_Init(void)
{
//	RTUData.RunMode = 1;
}
//---
/*
	函数名：	void Master_ReceiveData(u8 *p,u8 len)
	描述：		该函数为处理来自主机发送的命令
	调用：		该函数由通讯端口（串口）调用
	通讯格式：
	上位机对下位机：起始码、地址码、功能码、数据长度码、数据1···数据n、校验码、停止码
	下位机对上位机：起始码、本机地址码、功能码、命令码、执行结果返回码、数据长度位、数据1···数据n、校验码、停止码

*/
void RTU_CommunicateReceive(u8 *p,u8 len)
{
	switch(*(p))//查看是哪个功能码
	{
		case 0x10:
		{
			Project_DebugRTU(p+1,len-1);
			break;
		}
		default:
		{//无相关功能码
			printf("RTU收到无效命令码%u\r\n",*(p));
			break;
		}
	}
}
//---
/*
	函数名：void RTU_ReceiveSlave(u8 *p,u8 len)
	描述：	接收到从机的传感器数据
*//*
void RTU_ReceiveSlave(u8 *p,u8 len)
{
	switch(p[0])
	{
		case 0x01:
		{
			if(len == 63) 
			{
				RTUData.Error = 0;
				Flux_CommReceiveData(&p[1]);
			}
			break;
		}
	}
}*/
//---
/*
	函数名：void RTU_ReadSlaveBase(void)
	描述：	该函数为读取从机的基本参数
*//*
void RTU_ReadSlaveBase(u8 address)
{
	u8 bf[10];
	u8 cnt = 0,i;
	TwoBytes two;
	bf[cnt++] = 0x01;
	two.U16Data = CRC16(bf,cnt);
	bf[cnt++] = two.U16Data>>8;
	bf[cnt++] = two.U16Data;
	RTU_SendBufferData(RTU_UsartComputerPort,RTU_UsartComputerBuffer,address);
	RTU_SendBufferData(RTU_UsartComputerPort,RTU_UsartComputerBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		RTU_SendBufferData(RTU_UsartComputerPort,RTU_UsartComputerBuffer,bf[i]);
	}
}*/























//---
