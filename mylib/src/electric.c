/*
	本文件为该项目的电量相关函数
	具体函数如下

*/

#include "electric.h"

Electric_DataStructure ElectricData;//电量结构体变量


/*
	函数名：void Electric_Init(void)
	描述：	该函数为Electric.c初始化文件
*/
void Electric_Init(void)
{
	Electric_DataInit();
}
//---
/*
	函数名：void Electric_DataInit(void)
	描述：	该函数为ElectricData数据初始化
*/
void Electric_DataInit(void)
{
//	ElectricData.Flag = 0;
	ElectricData.Address = ELECTRIC_DefaultAddress;
	ElectricData.RunMode = ELECTRIC_RunModeNone;//清运行模式
}
//---
/*
	函数名：Electric_DataStructure* Electric_GetData(void)
	描述：	该函数为获取ElectricData变量地址
*/
Electric_DataStructure* Electric_GetData(void)
{
	return &ElectricData;
}
//---
/*
	函数名：void Electric_CollectBasicData(void)
	描述：	该函数为用于发送采集OHR-D23基本数据命令，共有26个字数据
	01 03 01 00 00 34 45 E1
*/
void Electric_CollectBasicData(void)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = ElectricData.Address;
	buffer[cnt++] = ELECTRIC_ReadWords;//读取命令
	buffer[cnt++] = 0x01;//起始地址0x0100，2字节
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x00;//数据长度0x0034(52个字数据)，2字节
	buffer[cnt++] = 0x34;
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	for(crc=0;crc<cnt;crc++)
	{
		Electric_SendBufferData(Electric_UsartPort,Electric_UsartBuffer,buffer[crc]);//发送数据
	}
	ElectricData.RunMode = ELECTRIC_RunModeBasicData;
}
//---
/*
	函数名：void Electric_SendReadCmd(u16 address,u16 len)
	描述：	该函数为发送读取命令
	输入：	address：起始地址
					len：读取数据的长度，以字为单位
*/
void Electric_SendReadCmd(u16 address,u16 len)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = ElectricData.Address;
	buffer[cnt++] = ELECTRIC_ReadWords;//读取命令
	buffer[cnt++] = address>>8;//起始地址高字节，0x0100、2字节
	buffer[cnt++] = address;
	buffer[cnt++] = len>>8;//数据长度0x0034(52个字数据)，2字节
	buffer[cnt++] = len;
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	for(crc=0;crc<cnt;crc++)
	{
		Electric_SendBufferData(Electric_UsartPort,Electric_UsartBuffer,buffer[crc]);//发送数据
	}
	switch(address)
	{
		case ELECTRIC_AddBasicData:
		{//当前读取基本电量参数
			ElectricData.RunMode = ELECTRIC_RunModeBasicData;
			break;
		}
		case ELECTRIC_AddEnergyData:
		{//当前读取电能参数
			ElectricData.RunMode = ELECTRIC_RunModeEnergyData;
			break;
		}
		case ELECTRIC_AddParameterData:
		{//当前读取配置参数
			ElectricData.RunMode = ELECTRIC_RunModeParameterData;
			break;
		}
	}
}
//---
/*
	函数名：void Electric_WriteOneWord(u16 address,u16 data)
	描述：	该函数为向目标寄存器写入一个字内容
*/
void Electric_WriteOneWord(u16 address,u16 data)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = ElectricData.Address;
	buffer[cnt++] = ELECTRIC_WriteOneWord;//读取命令
	buffer[cnt++] = address>>8;//起始地址高字节，0x0100、2字节
	buffer[cnt++] = address;
	buffer[cnt++] = data>>8;//数据长度0x0034(52个字数据)，2字节
	buffer[cnt++] = data;
	crc = CRC16(buffer,cnt);//计算CRC数据
	buffer[cnt++] = crc>>8;//CRC高字节，2个字节
	buffer[cnt++] = crc;//CRC低字节，2个字节
	for(crc=0;crc<cnt;crc++)
	{
		Electric_SendBufferData(Electric_UsartPort,Electric_UsartBuffer,buffer[crc]);//发送数据
	}
	switch(address)
	{
		case ELECTRIC_AddVoltageRatio:
		{//电压倍率
			ElectricData.RunMode = ELECTRIC_RunModeVoltageRatio;
			break;
		}
		case ELECTRIC_AddCurrentRatio:
		{//电流倍率
			ElectricData.RunMode = ELECTRIC_RunModeCurrentRatio;
			break;
		}
		case ELECTRIC_AddWiringWay:
		{//接线方式
			ElectricData.RunMode = ELECTRIC_RunModeWiringWay;
			break;
		}
		case ELECTRIC_AddCommAddress:
		{//通讯地址
			ElectricData.RunMode = ELECTRIC_RunModeCommAddress;
			break;
		}
		case ELECTRIC_AddBaudRate:
		{//波特率
			ElectricData.RunMode = ELECTRIC_RunModeBaudRate;
			break;
		}
	}
}
//---
//-------------------------------------调试函数----------------------------------------
//---
/*
	函数名： void Electric_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Electricc文件的ASCII调试文件
	:000|60|XXX······
*/
void Electric_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1://
		{//:000|60|001
			Electric_SetComNoneVerify();//无校验
			Electric_CollectBasicData();			//发送命令
			printf("发送读取电量基本数据命令!\r\n");
			break;
		}
		case 2:
		{//:000|60|002
			Electric_SetComNoneVerify();//无校验
			Electric_SendReadCmd(ELECTRIC_AddBasicData,ELECTRIC_LenBasicData);//
			printf("发送读取电量基本数据命令!\r\n");
			break;
		}
		case 3:
		{//:000|60|003
			Electric_SetComNoneVerify();//无校验
			Electric_SendReadCmd(ELECTRIC_AddEnergyData,ELECTRIC_LenEnergyData);//
			printf("发送读取电能数据命令!\r\n");
			break;
		}
		case 4:
		{//:000|60|004
			Electric_SetComNoneVerify();//无校验
			Electric_SendReadCmd(ELECTRIC_AddParameterData,ELECTRIC_LenParameterData);//
			printf("发送读取电量配置参数数据命令!\r\n");
			break;
		}
		case 30:
		{//修改电压倍率，设置范围0~1200	:000|60|030|1200\r\n
			u16 buffer = Calculate_CharToU16(p+4,4);
			Electric_SetComNoneVerify();//无校验
			Electric_WriteOneWord(ELECTRIC_AddVoltageRatio,buffer);//
			printf("修改电压倍率为：%u\r\n",buffer);
			break;
		}
		case 31:
		{//修改电流倍率，设置范围0~1200	:000|60|031|1200\r\n
			u16 buffer = Calculate_CharToU16(p+4,4);
			Electric_SetComNoneVerify();//无校验
			Electric_WriteOneWord(ELECTRIC_AddCurrentRatio,buffer);//
			printf("修改电流倍率为：%u\r\n",buffer);
			break;
		}
		case 32:
		{//修改接线方式（0为三相四线，1为三相三线，2为3V3A）	:000|60|032|0\r\n
			u16 buffer = Calculate_CharToU16(p+4,1);
			Electric_SetComNoneVerify();//无校验
			Electric_WriteOneWord(ELECTRIC_AddWiringWay,buffer);//
			printf("修改接线方式：%u\r\n",buffer);
			break;
		}
		case 33:
		{//修仪表通讯地址，设置范围1~253		:000|60|033|001\r\n
			u16 buffer = Calculate_CharToU16(p+4,3);
			Electric_SetComNoneVerify();//无校验
			Electric_WriteOneWord(ELECTRIC_AddCommAddress,buffer);//
			printf("修改通信地址为：%u\r\n",buffer);
			break;
		}
		case 34:
		{//修仪表通讯波特率，设置范围0~4（0为1200,1为2400,2为4800,3为9600,4为19200）		
			//:000|60|034|3\r\n
			u16 buffer = Calculate_CharToU16(p+4,1);
			Electric_SetComNoneVerify();//无校验
			Electric_WriteOneWord(ELECTRIC_AddBaudRate,buffer);//
			printf("修改通讯波特率为：%u\r\n",buffer);
			break;
		}
		default:
		{
			printf("Electric.c接收到无效调试指令%u!\r\n",cmd);
			break;
		}
	}
}
//---
/*
	函数名： void Electric_DebugRTU((u8 *p)
	描述：	该函数为Electric.c文件的RTU调试文件
	说明：	调用该函数之前需要先进行CRC校验
*/
void Electric_DebugRTU(u8 *p,u8 len)
{
	switch(ElectricData.RunMode)//查看是哪个功能码
	{
		case ELECTRIC_RunModeBasicData:
		{//电量基本数据处理
			
				
//			u32 energy;

//			u8 buffer[10];
//			u8 cnt = 0;
//			u8 *pdata = p+3+12*4;
//			float PAY = 0;
//			pdata = (u8*)&PAY;
//			buffer[cnt++] = *pdata++;		
//			buffer[cnt++] = *pdata++;			
//			buffer[cnt++] = *pdata++;			
//			buffer[cnt++] = *pdata;	
//			energy = PAY;
//			Server_GetTowerCraneData()->CurrentPower = (u16)energy;
//			
//			printf("功率：%x,%x,%x,%x\r\n",*(p+3+12*4),*(p+4+12*4),*(p+5+12*4),*(p+6+12*4));
//			
//			printf("功率：%f\r\n",PAY);
//			energy = *(p+3+12*4)<<24 | *(p+4+12*4)<<16 | *(p+5+12*4)<<8 | *(p+6+12*4);//计算总有功功率
//			Server_GetTowerCraneData()->CurrentPower = (u16)((float)energy / 10000000.0);//当前功率，换算为0.1KW
//			printf("当前总功率为%u\r\n",energy);
//			printf("当前总功率为%fw\r\n",((float)Server_GetTowerCraneData()->CurrentPower/10.0));
			break;
		}
		case ELECTRIC_RunModeEnergyData:
		{
			printf("读取电能数据\r\n");
			break;
		}
		case ELECTRIC_RunModeParameterData:
		{
			printf("读取电量配置参数\r\n");
			break;
		}
		//写入
		case ELECTRIC_RunModeVoltageRatio:
		{
			printf("写入电压倍率\r\n");
			break;
		}
		case ELECTRIC_RunModeCurrentRatio:
		{
			printf("写入电流倍率\r\n");
			break;
		}
		case ELECTRIC_RunModeWiringWay:
		{
			printf("写入接线方式\r\n");
			break;
		}
		case ELECTRIC_RunModeCommAddress:
		{
			printf("写入通讯地址\r\n");
			break;
		}
		case ELECTRIC_RunModeBaudRate:
		{
			printf("写入通讯波特率\r\n");
			break;
		}
		
	}
	ElectricData.RunMode = ELECTRIC_RunModeNone;//清运行模式
}
//---





