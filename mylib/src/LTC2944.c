
#include"LTC2944.h"
#include"delay.h"


LTC2944_DataStructure LTC2944_Data;

/*
 * 函数名：LTC2944_DataStructure* LTC2944_GetData(void)
 * 描述  ：返回结构体
 * 输入  ：无
 * 输出  ：无
 */
LTC2944_DataStructure* LTC2944_GetData(void)
{
		return &LTC2944_Data;
}

/*
 * 函数名：LTC2944_Main(void)
 * 描述  ：本模块的监控线程
 * 输入  ：无
 * 输出  ：无
 */
void LTC2944_Main(void)
{
	switch(LTC2944_Data.RunMode)
	{
		case 0:
		break;
		case 1:
		break;
		case 2:
		break;
		case 3:
		break;
		default:
		break;
				
	}
	
	if(LTC2944_Data.Timer==0)  //1秒时间到
	{
		LTC2944_Data.Timer=5000;  //重载5秒
	    LTC2944_ReadNByte(0X00,0X18,&LTC2944_Data.unData);  //采集
		LTC2944_Data.V=LTC2944_Cal_V(LTC2944_Data.unData.Reg.Voltage);
		LTC2944_Data.I=LTC2944_Cal_I(LTC2944_Data.unData.Reg.Current);
		LTC2944_Data.Temp=LTC2944_Cal_Temp(LTC2944_Data.unData.Reg.Temperature);
		LTC2944_Data.PTC=LTC2944_Cal_PTC(LTC2944_Data.unData.Reg.AccumulatCharge);
		
		LTC2944_Data.flag++;
		if(LTC2944_Data.flag>=3)LTC2944_Data.flag=3;
		
	  if(LTC2944_Data.PTC<BATTERY_ACCmin)   //最低电量
		{
			LTC2944_Data.Mode=1;
		}
		else if(LTC2944_Data.PTC>(BATTERY_ACCmin+2))
		{
			LTC2944_Data.Mode=0;
		}
    // LTC2944_WriteByte(0x01,0xFC);
		
//		printf("Status=%x;\n",LTC2944_Data.unData.Reg.Status);          //test 20200509
//		printf("Control=%x;\n",LTC2944_Data.unData.Reg.Control);
//		printf("AccumulatCharge=%x;\n",LTC2944_Data.unData.Reg.AccumulatCharge);
//		printf("Voltage=%f;\n",LTC2944_Data.V);
//		printf("Current=%f;\n",LTC2944_Data.I);
//		printf("Temperature=%f;\n",LTC2944_Data.Temp);
//		
//		printf("ChargeThresholdHigh=%x;\n",LTC2944_Data.unData.Reg.ChargeThresholdHigh);
//		printf("ChargeThresholdLow=%x;\n",LTC2944_Data.unData.Reg.ChargeThresholdLow);
		
	}
}


/*
 * 函数名：void LTC2944SDA_Mode_IPU(void)
 * 描述  ：数据引脚设为上拉输入
 * 输入  ：无
 * 输出  ：无
 */

static void LTC2944SDA_Mode_IPU(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(LTC2944_SDA_RCC, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 		
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = LTC2944_SDA_PIN;			
  GPIO_Init(LTC2944_SDA_PORT, &GPIO_InitStructure);
}



/*
 * 函数名：LTC2944SDA_Mode_Out_PP(void)
 * 描述  ：数据引脚设为推输出
 * 输入  ：无
 * 输出  ：无
 */
static void LTC2944SDA_Mode_Out_PP(void)
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(LTC2944_SDA_RCC, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin =LTC2944_SDA_PIN;			
  GPIO_Init(LTC2944_SDA_PORT, &GPIO_InitStructure);
}

/*
 * 函数名：LTC2944_Init(void)
 * 描述  ：端口初始化与数据变量初始化
 * 输入  ：无
 * 输出  ：无
 */
void LTC2944_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(	LTC2944_SDA_RCC, ENABLE );	
	GPIO_InitStructure.GPIO_Pin = LTC2944_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(LTC2944_SDA_PORT, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(	LTC2944_SCL_RCC, ENABLE );	
	GPIO_InitStructure.GPIO_Pin = LTC2944_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(LTC2944_SCL_PORT, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(	LTC2944_ALT_RCC, ENABLE );	
	GPIO_InitStructure.GPIO_Pin = LTC2944_ALT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LTC2944_ALT_PORT, &GPIO_InitStructure);
	
	
    GPIO_SetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);	
    GPIO_SetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);	 
	
	
	//DATA init
  LTC2944_DataInit();
	
}


//产生LTC2944起始信号
void LTC2944_Start(void)
{
	LTC2944SDA_Mode_Out_PP();     //sda线输出
	delay_us(8);
	GPIO_SetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);	  	  
	GPIO_SetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);
	delay_us(8);
 	GPIO_ResetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);//START:when CLK is high,DATA change form high to low 
	delay_us(8);
	GPIO_ResetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);//钳住I2C总线，准备发送或接收数据 
	delay_us(4);
}	  
//产生LTC2944停止信号
void LTC2944_Stop(void)
{
	LTC2944SDA_Mode_Out_PP();//sda线输出
	delay_us(8);
	GPIO_ResetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);
	GPIO_ResetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	GPIO_SetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN); 
	delay_us(4);
	GPIO_SetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 LTC2944_Wait_Ack(void)
{
	u8 ucErrTime=0;
	LTC2944SDA_Mode_IPU();      //SDA设置为输入  
	delay_us(8);
	GPIO_SetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);
	delay_us(8);	   
	GPIO_SetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);
	delay_us(8);	 
	while(READLTC2944_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			LTC2944_Stop();
			return 1;
		}
	}
	GPIO_ResetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);//时钟输出0 	
	delay_us(8);
	return 0;  
} 
//产生ACK应答
void LTC2944_Ack(void)
{
	GPIO_ResetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);
	delay_us(2);
	LTC2944SDA_Mode_Out_PP();
	delay_us(8);
	GPIO_ResetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);
	delay_us(8);
	GPIO_SetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);
	delay_us(8);
	GPIO_ResetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);
	delay_us(8);
}
//不产生ACK应答		    
void LTC2944_NAck(void)
{
	GPIO_ResetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);
	delay_us(2);
	LTC2944SDA_Mode_Out_PP();
	delay_us(8);
	GPIO_SetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);
	delay_us(8);
	GPIO_SetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);
	delay_us(8);
	GPIO_ResetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);
	delay_us(8);
}					 				     
//LTC2944发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void LTC2944_Send_Byte(u8 txd)
{                        
    u8 t;   
		LTC2944SDA_Mode_Out_PP(); 	    
	  delay_us(8);   //
    GPIO_ResetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);//拉低时钟开始数据传输
	  delay_us(8);   //
    for(t=0;t<8;t++)
    {              
       
			  if((txd&0x80)>>7)
			  GPIO_SetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);
				else
				GPIO_ResetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);	
			
        txd<<=1; 	  
		delay_us(8);   //对TEA5767这三个延时都是必须的
		GPIO_SetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);
		delay_us(8); 
		GPIO_ResetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);	
		delay_us(8);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 LTC2944_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	LTC2944SDA_Mode_IPU();//SDA设置为输入
	delay_us(8);
  for(i=0;i<8;i++ )
	{
    GPIO_ResetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN); 
    delay_us(8);
		GPIO_SetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);
    receive<<=1;
    if(READLTC2944_SDA)receive++;   
		delay_us(8); 
  }					 
	if (!ack)
			LTC2944_NAck();//发送nACK
	else
			LTC2944_Ack(); //发送ACK   
	return receive;
}
/*
  写1个寄存器
  参数：
   regadrr:寄存器地址00~17h
   data:   发送数据
*/
void LTC2944_WriteByte(uint8_t regadrr,uint8_t data)
{
	LTC2944_Start();  
	LTC2944_Send_Byte(0xC8);	    //发器件地址与写位1100100  0
	LTC2944_Wait_Ack(); 

	LTC2944_Send_Byte(regadrr);     //发送字节							   
  LTC2944_Wait_Ack();  		    
	
  LTC2944_Send_Byte(data);     //发送字节							   
  LTC2944_Wait_Ack();  		    	   
	
  LTC2944_Stop();//产生一个停止条件 
	delay_us(8);
}

/*
  写2个寄存器
  参数：
   regadrr:寄存器地址00~17h
   data:   发送数据H,L
*/
void LTC2944_WriteTwoByte(uint8_t regadrr,uint16_t data)
{
	LTC2944_Start();  
	LTC2944_Send_Byte(0xC8);	    //发器件地址与写位1100100  0
	LTC2944_Wait_Ack(); 

	LTC2944_Send_Byte(regadrr);     //发送字节							   
  LTC2944_Wait_Ack();  		    
	
  LTC2944_Send_Byte((uint8_t)(data>>8)); //发送高字节							   
  LTC2944_Wait_Ack();  
	
	LTC2944_Send_Byte((uint8_t)(data)); //发送低字节							   
  LTC2944_Wait_Ack();  
	
  LTC2944_Stop();//产生一个停止条件 
	delay_us(8);
}

/*
  写N个寄存器
  参数：
   regadrr:寄存器地址00~17h
   data:   发送数据指针
*/
void LTC2944_WriteNByte(uint8_t regadrr,uint8_t N,uint8_t * data)
{ uint8_t i;
	LTC2944_Start();  
	LTC2944_Send_Byte(0xC8);	    //发器件地址与写位1100100  0
	LTC2944_Wait_Ack(); 

	LTC2944_Send_Byte(regadrr);     //发送字节							   
  LTC2944_Wait_Ack();  		    
	
	for(i=0;i<N;i++)
	{
  LTC2944_Send_Byte(*data); //发送高字节							   
  LTC2944_Wait_Ack();  
	data++;	
	}

  LTC2944_Stop();//产生一个停止条件 
	delay_us(8);
}
/*
  读1个寄存器
  参数：
   regadrr:寄存器地址00~17h
  返回1字节
*/

uint8_t LTC2944_ReadOneByte(uint8_t regadrr)
 { 
	  
		uint8_t data;
	 	LTC2944_Start();  
		LTC2944_Send_Byte(0XC8);	
		LTC2944_Wait_Ack();
	  LTC2944_Send_Byte(regadrr);     //发送字节							   
    LTC2944_Wait_Ack();  
	 
	  LTC2944_Start(); 
    LTC2944_Send_Byte(0XC9);	
		LTC2944_Wait_Ack();	 
		data=LTC2944_Read_Byte(0);
		LTC2944_Stop();//产生一个停止条件	
         delay_us(8);	 
	  return data;
 }
 /*
  读2个寄存器
  参数：
   regadrr:寄存器地址00~17h
  返回1字节
*/
uint16_t LTC2944_ReadTwoByte(uint8_t regadrr)
 {
	  uint8_t buf[2];
		uint16_t data;
	 	LTC2944_Start();  
		LTC2944_Send_Byte(0XC8);	
		LTC2944_Wait_Ack();
	  LTC2944_Send_Byte(regadrr);     //发送字节							   
    LTC2944_Wait_Ack();  
	 
	  LTC2944_Start(); 
    LTC2944_Send_Byte(0XC9);	
		LTC2944_Wait_Ack();	 
		buf[0]=LTC2944_Read_Byte(1);   //中间读取的数据返回0应答电平
		buf[1]=LTC2944_Read_Byte(0);   //读取最后返回1应答电平
		LTC2944_Stop();//产生一个停止条件	   
	  data=buf[0];
	  data=data*256;
	  data=data+buf[1];
	  return data;
 }
 /*
  读N个寄存器
  参数：
   regadrr:寄存器地址00~17h
  返回1字节
*/
 void LTC2944_ReadNByte(uint8_t regadrr,uint8_t N,LTC2944_un * data)
 {  uint8_t i;
	  LTC2944_un buf;
	  LTC2944_Start();  
	  LTC2944_Send_Byte(0XC8);	
	  LTC2944_Wait_Ack();
	  LTC2944_Send_Byte(regadrr);     //发送字节							   
      LTC2944_Wait_Ack();  
	 
	  LTC2944_Start(); 
      LTC2944_Send_Byte(0XC9);	
	  LTC2944_Wait_Ack();	 

		 for(i=0;i<N-1;i++)
	  {
		 buf.arr[i]=LTC2944_Read_Byte(1);   //中间读取的数据返回0应答电平
	  }
		buf.arr[i]=LTC2944_Read_Byte(0);   //读取最后返回1应答电平
		LTC2944_Stop();//产生一个停止条件	  

		//数据处理
		data->Reg.Status=buf.Reg.Status;
		data->Reg.Control=buf.Reg.Control;
		data->Reg.AccumulatCharge=((buf.Reg.AccumulatCharge<<8)&0xff00)+((buf.Reg.AccumulatCharge>>8)&0x00ff);
		data->Reg.ChargeThresholdHigh=((buf.Reg.ChargeThresholdHigh<<8)&0xff00)+((buf.Reg.ChargeThresholdHigh>>8)&0x00ff);
		data->Reg.ChargeThresholdLow=((buf.Reg.ChargeThresholdLow<<8)&0xff00)+((buf.Reg.ChargeThresholdLow>>8)&0x00ff);
		
		data->Reg.Voltage=((buf.Reg.Voltage<<8)&0xff00)+((buf.Reg.Voltage>>8)&0x00ff);
		data->Reg.VoltageThresholdHigh=((buf.Reg.VoltageThresholdHigh<<8)&0xff00)+((buf.Reg.VoltageThresholdHigh>>8)&0x00ff);
		data->Reg.VoltageThresholdLow=((buf.Reg.VoltageThresholdLow<<8)&0xff00)+((buf.Reg.VoltageThresholdLow>>8)&0x00ff);
		
	  data->Reg.Current=((buf.Reg.Current<<8)&0xff00)+((buf.Reg.Current>>8)&0x00ff);
		data->Reg.CurrentThresholdHigh=((buf.Reg.CurrentThresholdHigh<<8)&0xff00)+((buf.Reg.CurrentThresholdHigh>>8)&0x00ff);
		data->Reg.CurrentThresholdLow=((buf.Reg.CurrentThresholdLow<<8)&0xff00)+((buf.Reg.CurrentThresholdLow>>8)&0x00ff);
		data->Reg.Temperature=((buf.Reg.Temperature<<8)&0xff00)+((buf.Reg.Temperature>>8)&0x00ff);
		data->Reg.TemperatureThresholdHigh=buf.Reg.TemperatureThresholdHigh;
		data->Reg.TemperatureThresholdLow=buf.Reg.TemperatureThresholdLow;
 }
 
/*
 硬件数据初始化
 
 
*/ 
void LTC2944_DataInit(void)
   {
		 u16 buf;
		 LTC2944_DataStructure Dbuf;
	 
		 Dbuf.unData.Reg.Control=0xFC;   //启动芯片
	//	 Dbuf.Reg.AccumulatCharge=0xff7f;
		 buf=LTC2944_ReadTwoByte(0x02); //先读出记忆值
	
		 if((buf>=0x7ff0)&&(buf<=0X8005))
		 {
			 buf=LTC2944_ACCMin;
		 }
		 else if(buf>LTC2944_ACCMax+0x005)
		 {
			 buf=LTC2944_ACCMax;
		 }
		 Dbuf.unData.Reg.AccumulatCharge=((buf<<8)&0xff00)+((buf>>8)&0x00ff);

		 Dbuf.unData.Reg.ChargeThresholdHigh=0xf5ff;
		 Dbuf.unData.Reg.ChargeThresholdLow=0x0000;
		 Dbuf.unData.Reg.Voltage=0x0000;
	     Dbuf.unData.Reg.VoltageThresholdHigh=0xffFF;
	     Dbuf.unData.Reg.VoltageThresholdLow=0x0000;
	
	     Dbuf.unData.Reg.Current=0x0000;
	     Dbuf.unData.Reg.CurrentThresholdHigh=0xffff;
	     Dbuf.unData.Reg.CurrentThresholdLow=0x0000;
	
	     Dbuf.unData.Reg.Temperature=0x0000;
	     Dbuf.unData.Reg.TemperatureThresholdHigh=0xff;
	     Dbuf.unData.Reg.TemperatureThresholdLow=0x00;
		 
		 LTC2944_WriteNByte(0x01,0x17,(u8 *)&Dbuf.unData.arr[1]);
		 
		 LTC2944_Data.RunMode=0;
         LTC2944_Data.Timer=2000; //秒
         LTC2944_Data.Mode=0;
		 
		// LTC2944_WriteByte(0X01,0XFC);
		 
	 }
	 
	 
	 //---
/*
	函数名：void DS18B20_Timer(void)
	描述：DS18B20.c文件定时器函数，由系统1ms定时器调用
*/
void LTC2944_Timer(void)
{
	if(LTC2944_Data.Timer != 0)LTC2944_Data.Timer--;
}

	 
	 //---
/*
	函数名：float LTC2944_Cal_V(u16 RedV)
	描述： 计算实制电压值70.8*regV/65535
*/ 

float LTC2944_Cal_V(u16 RedV)
{
	float buf;
	buf=70.8;
	buf=buf*(float)RedV;
	buf=buf/65535;
	buf=buf*LTC2944_V_K+LTC2944_V_b;
	return buf;
}


/*
	函数名：LTC2944_Cal_I(u16 RedI)
	描述： 计算实制电流值64mv/LTC2944_RefR*(RedI-32767)/32767
*/ 

float LTC2944_Cal_I(u16 RedI)
{
	float buf;
	buf=64/LTC2944_RefR;
	buf=buf*(RedI-32767);
	buf=buf/32767;
	buf=buf*LTC2944_I_K+LTC2944_I_b;
	return buf;
}


/*
	函数名：LTC2944_Cal_Temp(u16 RedTemp);
	描述： 计算实制温度值
*/ 
float LTC2944_Cal_Temp(u16 RedTemp)
{
	return((float)510*RedTemp/65535-271);
}

/*
	函数名：LTC2944_Cal_PTC(u16 RedACC)
	描述： 计算剩下容量
RedACC :LTC2944所读的库伦值
ACCmax:为充满电时的值
返回剩下容量0~100范围
*/ 
float LTC2944_Cal_PTC(u16 RedACC)
{

	float bufmin,buf1;
	bufmin=LTC2944_ACCMin;
  if(RedACC>LTC2944_ACCMax)
 	RedACC=LTC2944_ACCMax;
	else if(RedACC<=bufmin)
	RedACC=bufmin+10;
	
	buf1=(RedACC-bufmin)/(LTC2944_ACCMax-bufmin)*100;
	if(buf1>=100)
	buf1=100;
	return buf1;

}
/*
	函数名：LTC2944_Cal_ACC(float VOL)
	描述： 计算剩下容量
VOL :电池电压值
ACCmax:为充满电时的值
返回剩下容量0~100范围
*/ 
u16 LTC2944_Cal_ACC(float VOL)
{
	float bufmin,buf1;
	bufmin=LTC2944_ACCMin;
	buf1=(VOL-BATTERY_Vcmp)*(LTC2944_ACCMax-bufmin)/(BATTERY_VMAX-BATTERY_Vcmp)+bufmin;
	return buf1;
}
//-------------------------调试函数----------------------------
//---
/*
	函数名： void DS18B20_DebugASCII(u8 *p,u8 len)
	描述：		该函数为DS18B20文件的ASCII调试文件
通讯格式：:000|03|xxx
*/
void LTC2944_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:
		{//:000|03|001
		
			break;
		}
		case 2:
		{
			break;
		}
	}
}
