/*
	本文件为该项目的EEPROM存储芯片FM24CL64相关函数
	具体函数如下
*/

#include "FM24CL64.h"

FM24CL64_DataStructure FM24CL64Data;
/*
	函数名：void FM24CL64_Main(void)
	描述：	heat.c主函数
*/
void FM24CL64_Main(void)
{
	
}
//---
/*
	函数名：void FM24CL64_Timer(void)
	描述：FM24CL64.c文件定时器函数，由系统1ms定时器调用
*/
void FM24CL64_Timer(void)
{
	if(FM24CL64Data.Timer != 0) FM24CL64Data.Timer--;
}
//---
/*
	函数名：void FM24CL64_Work(void)
*/
//---
/*
	函数名：void FM24CL64_Init(void)
	描述：	FM24CL64.c文件初始化函数
*/
void FM24CL64_Init(void)
{	
	IIC_Init();

}
//---
/*
	函数名：FM24CL64_DataStructure* FM24CL64_GetData(void)
	描述：获取返回FM24CL64.c文件全局变量结构体变量
*/
FM24CL64_DataStructure* FM24CL64_GetData(void)
{
	return &FM24CL64Data;
}
//---
/*
	函数名：void FM24CL64_WriteData(void)
	描述：	该函数为保存数据
*/

u8 FM24CL64_ReadOneByte(u16 ReadAddr)
{				  
  	u8 temp=0;		  	    																 
    IIC_Start();  
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址	    
  	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	  IIC_Wait_Ack();	    
	  IIC_Start();  	 	   
	  IIC_Send_Byte(0XA1);           //进入接收模式			   
	  IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void FM24CL64_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址	  
  	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	  IIC_Wait_Ack(); 	 										  		   
	  IIC_Send_Byte(DataToWrite);     //发送字节							   
	  IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	  delay_ms(10);	 
}



void FM24CL64_WriteData(void)
{
	u16 i,j,yMAX,xMAX;
	u16 len = FLASHIS_AdrEnd;//保存数据长度
	len=len*2;
	u8 *p = (u8*)&(Flashisp_GetData()->Data[0]);
	yMAX=(u16)(len/32);   //求整数部分
	if(len%32)  //余数不为零
	yMAX++;         //以32为1页去写 ,共有j页
	for(j=0;j<yMAX;j++)
	{
		if(j<(yMAX-1))
			xMAX=32;
		else
		{ xMAX=len%32;
			if(xMAX==0)
				xMAX=32;
		}

	//-----------
	IIC_Start();
	IIC_Send_Byte(FM24CL64_CMD_WRITE);//写命令
	IIC_Wait_Ack();
	IIC_Send_Byte((u8)(j*32/256));		//从地址j*32开始写
	IIC_Wait_Ack();
	IIC_Send_Byte((u8)((j*32)%256));
	IIC_Wait_Ack();
	for(i=0;i<xMAX;i++)
	{//开始写数据
		IIC_Send_Byte(*p);
		IIC_Wait_Ack();
		p++;
	}
	IIC_Stop();
	//------------
	delay_us(10); //延时10us下一页
 }

}
void FM24CL64_TestWriteData(void)
{
	u16 i;
	IIC_Start();
	IIC_Send_Byte(FM24CL64_CMD_WRITE);//写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(0);		//从地址0开始写
	IIC_Wait_Ack();
	IIC_Send_Byte(0);
	IIC_Wait_Ack();
	for(i=0;i<500;i++)
	{//开始写数据
		IIC_Send_Byte(i);
		IIC_Wait_Ack();
	}
	IIC_Stop();
}
//---
/*
	函数名：void FM24CL64_WriteData(void)
	描述：	该函数为读取数据
*/
void FM24CL64_ReadData(void)
{
	u16 i;
	u16 len =FLASHIS_AdrEnd;
  
	u8 *p = (u8*)&(Flashisp_GetData()->Data[0]);
	len=len	* 2;//读取数据长度
	
	for(i=0;i<len;i++)
	{//开始读数据
	
		*p =(u8)FM24CL64_ReadOneByte(i);
		p++;
	}

}
void FM24CL64_TestReadData(void)
{
	u16 i;
	IIC_Start();
	IIC_Send_Byte(FM24CL64_CMD_WRITE);//写命令
	IIC_Wait_Ack();
	IIC_Send_Byte(0);		//从地址0开始读
	IIC_Wait_Ack();
	IIC_Send_Byte(0);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(FM24CL64_CMD_READ);//读命令
	IIC_Wait_Ack();
	for(i=0;i<500;i++)
	{//开始读数据
		printf("%x ",IIC_Read_Byte(1));
	}
	IIC_Stop();
	printf("\r\n\r\n ");
}
//---
//-------------------------调试函数----------------------------
//---
/*
	函数名： void FM24CL64_DebugASCII(u8 *p,u8 len)
	描述：		该函数为FM24CL64.c文件的ASCII调试文件
*/
void FM24CL64_DebugASCII(u8 *p,u8 len)
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
	函数名： 	void FM24CL64_DebugRTU((u8 *p)
	描述：		该函数为FM24CL64.c文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*//*
void FM24CL64_DebugRTU(u8 *p,u8 len)
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





































