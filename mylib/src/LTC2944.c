
#include"LTC2944.h"
#include"delay.h"


LTC2944_DataStructure LTC2944_Data;

/*
 * ��������LTC2944_DataStructure* LTC2944_GetData(void)
 * ����  �����ؽṹ��
 * ����  ����
 * ���  ����
 */
LTC2944_DataStructure* LTC2944_GetData(void)
{
		return &LTC2944_Data;
}

/*
 * ��������LTC2944_Main(void)
 * ����  ����ģ��ļ���߳�
 * ����  ����
 * ���  ����
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
	
	if(LTC2944_Data.Timer==0)  //1��ʱ�䵽
	{
		LTC2944_Data.Timer=5000;  //����5��
	    LTC2944_ReadNByte(0X00,0X18,&LTC2944_Data.unData);  //�ɼ�
		LTC2944_Data.V=LTC2944_Cal_V(LTC2944_Data.unData.Reg.Voltage);
		LTC2944_Data.I=LTC2944_Cal_I(LTC2944_Data.unData.Reg.Current);
		LTC2944_Data.Temp=LTC2944_Cal_Temp(LTC2944_Data.unData.Reg.Temperature);
		LTC2944_Data.PTC=LTC2944_Cal_PTC(LTC2944_Data.unData.Reg.AccumulatCharge);
		
		LTC2944_Data.flag++;
		if(LTC2944_Data.flag>=3)LTC2944_Data.flag=3;
		
	  if(LTC2944_Data.PTC<BATTERY_ACCmin)   //��͵���
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
 * ��������void LTC2944SDA_Mode_IPU(void)
 * ����  ������������Ϊ��������
 * ����  ����
 * ���  ����
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
 * ��������LTC2944SDA_Mode_Out_PP(void)
 * ����  ������������Ϊ�����
 * ����  ����
 * ���  ����
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
 * ��������LTC2944_Init(void)
 * ����  ���˿ڳ�ʼ�������ݱ�����ʼ��
 * ����  ����
 * ���  ����
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


//����LTC2944��ʼ�ź�
void LTC2944_Start(void)
{
	LTC2944SDA_Mode_Out_PP();     //sda�����
	delay_us(8);
	GPIO_SetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);	  	  
	GPIO_SetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);
	delay_us(8);
 	GPIO_ResetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);//START:when CLK is high,DATA change form high to low 
	delay_us(8);
	GPIO_ResetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);//ǯסI2C���ߣ�׼�����ͻ�������� 
	delay_us(4);
}	  
//����LTC2944ֹͣ�ź�
void LTC2944_Stop(void)
{
	LTC2944SDA_Mode_Out_PP();//sda�����
	delay_us(8);
	GPIO_ResetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);
	GPIO_ResetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	GPIO_SetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN); 
	delay_us(4);
	GPIO_SetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 LTC2944_Wait_Ack(void)
{
	u8 ucErrTime=0;
	LTC2944SDA_Mode_IPU();      //SDA����Ϊ����  
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
	GPIO_ResetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);//ʱ�����0 	
	delay_us(8);
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//LTC2944����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void LTC2944_Send_Byte(u8 txd)
{                        
    u8 t;   
		LTC2944SDA_Mode_Out_PP(); 	    
	  delay_us(8);   //
    GPIO_ResetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);//����ʱ�ӿ�ʼ���ݴ���
	  delay_us(8);   //
    for(t=0;t<8;t++)
    {              
       
			  if((txd&0x80)>>7)
			  GPIO_SetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);
				else
				GPIO_ResetBits(LTC2944_SDA_PORT, LTC2944_SDA_PIN);	
			
        txd<<=1; 	  
		delay_us(8);   //��TEA5767��������ʱ���Ǳ����
		GPIO_SetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);
		delay_us(8); 
		GPIO_ResetBits(LTC2944_SCL_PORT, LTC2944_SCL_PIN);	
		delay_us(8);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 LTC2944_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	LTC2944SDA_Mode_IPU();//SDA����Ϊ����
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
			LTC2944_NAck();//����nACK
	else
			LTC2944_Ack(); //����ACK   
	return receive;
}
/*
  д1���Ĵ���
  ������
   regadrr:�Ĵ�����ַ00~17h
   data:   ��������
*/
void LTC2944_WriteByte(uint8_t regadrr,uint8_t data)
{
	LTC2944_Start();  
	LTC2944_Send_Byte(0xC8);	    //��������ַ��дλ1100100  0
	LTC2944_Wait_Ack(); 

	LTC2944_Send_Byte(regadrr);     //�����ֽ�							   
  LTC2944_Wait_Ack();  		    
	
  LTC2944_Send_Byte(data);     //�����ֽ�							   
  LTC2944_Wait_Ack();  		    	   
	
  LTC2944_Stop();//����һ��ֹͣ���� 
	delay_us(8);
}

/*
  д2���Ĵ���
  ������
   regadrr:�Ĵ�����ַ00~17h
   data:   ��������H,L
*/
void LTC2944_WriteTwoByte(uint8_t regadrr,uint16_t data)
{
	LTC2944_Start();  
	LTC2944_Send_Byte(0xC8);	    //��������ַ��дλ1100100  0
	LTC2944_Wait_Ack(); 

	LTC2944_Send_Byte(regadrr);     //�����ֽ�							   
  LTC2944_Wait_Ack();  		    
	
  LTC2944_Send_Byte((uint8_t)(data>>8)); //���͸��ֽ�							   
  LTC2944_Wait_Ack();  
	
	LTC2944_Send_Byte((uint8_t)(data)); //���͵��ֽ�							   
  LTC2944_Wait_Ack();  
	
  LTC2944_Stop();//����һ��ֹͣ���� 
	delay_us(8);
}

/*
  дN���Ĵ���
  ������
   regadrr:�Ĵ�����ַ00~17h
   data:   ��������ָ��
*/
void LTC2944_WriteNByte(uint8_t regadrr,uint8_t N,uint8_t * data)
{ uint8_t i;
	LTC2944_Start();  
	LTC2944_Send_Byte(0xC8);	    //��������ַ��дλ1100100  0
	LTC2944_Wait_Ack(); 

	LTC2944_Send_Byte(regadrr);     //�����ֽ�							   
  LTC2944_Wait_Ack();  		    
	
	for(i=0;i<N;i++)
	{
  LTC2944_Send_Byte(*data); //���͸��ֽ�							   
  LTC2944_Wait_Ack();  
	data++;	
	}

  LTC2944_Stop();//����һ��ֹͣ���� 
	delay_us(8);
}
/*
  ��1���Ĵ���
  ������
   regadrr:�Ĵ�����ַ00~17h
  ����1�ֽ�
*/

uint8_t LTC2944_ReadOneByte(uint8_t regadrr)
 { 
	  
		uint8_t data;
	 	LTC2944_Start();  
		LTC2944_Send_Byte(0XC8);	
		LTC2944_Wait_Ack();
	  LTC2944_Send_Byte(regadrr);     //�����ֽ�							   
    LTC2944_Wait_Ack();  
	 
	  LTC2944_Start(); 
    LTC2944_Send_Byte(0XC9);	
		LTC2944_Wait_Ack();	 
		data=LTC2944_Read_Byte(0);
		LTC2944_Stop();//����һ��ֹͣ����	
         delay_us(8);	 
	  return data;
 }
 /*
  ��2���Ĵ���
  ������
   regadrr:�Ĵ�����ַ00~17h
  ����1�ֽ�
*/
uint16_t LTC2944_ReadTwoByte(uint8_t regadrr)
 {
	  uint8_t buf[2];
		uint16_t data;
	 	LTC2944_Start();  
		LTC2944_Send_Byte(0XC8);	
		LTC2944_Wait_Ack();
	  LTC2944_Send_Byte(regadrr);     //�����ֽ�							   
    LTC2944_Wait_Ack();  
	 
	  LTC2944_Start(); 
    LTC2944_Send_Byte(0XC9);	
		LTC2944_Wait_Ack();	 
		buf[0]=LTC2944_Read_Byte(1);   //�м��ȡ�����ݷ���0Ӧ���ƽ
		buf[1]=LTC2944_Read_Byte(0);   //��ȡ��󷵻�1Ӧ���ƽ
		LTC2944_Stop();//����һ��ֹͣ����	   
	  data=buf[0];
	  data=data*256;
	  data=data+buf[1];
	  return data;
 }
 /*
  ��N���Ĵ���
  ������
   regadrr:�Ĵ�����ַ00~17h
  ����1�ֽ�
*/
 void LTC2944_ReadNByte(uint8_t regadrr,uint8_t N,LTC2944_un * data)
 {  uint8_t i;
	  LTC2944_un buf;
	  LTC2944_Start();  
	  LTC2944_Send_Byte(0XC8);	
	  LTC2944_Wait_Ack();
	  LTC2944_Send_Byte(regadrr);     //�����ֽ�							   
      LTC2944_Wait_Ack();  
	 
	  LTC2944_Start(); 
      LTC2944_Send_Byte(0XC9);	
	  LTC2944_Wait_Ack();	 

		 for(i=0;i<N-1;i++)
	  {
		 buf.arr[i]=LTC2944_Read_Byte(1);   //�м��ȡ�����ݷ���0Ӧ���ƽ
	  }
		buf.arr[i]=LTC2944_Read_Byte(0);   //��ȡ��󷵻�1Ӧ���ƽ
		LTC2944_Stop();//����һ��ֹͣ����	  

		//���ݴ���
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
 Ӳ�����ݳ�ʼ��
 
 
*/ 
void LTC2944_DataInit(void)
   {
		 u16 buf;
		 LTC2944_DataStructure Dbuf;
	 
		 Dbuf.unData.Reg.Control=0xFC;   //����оƬ
	//	 Dbuf.Reg.AccumulatCharge=0xff7f;
		 buf=LTC2944_ReadTwoByte(0x02); //�ȶ�������ֵ
	
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
         LTC2944_Data.Timer=2000; //��
         LTC2944_Data.Mode=0;
		 
		// LTC2944_WriteByte(0X01,0XFC);
		 
	 }
	 
	 
	 //---
/*
	��������void DS18B20_Timer(void)
	������DS18B20.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void LTC2944_Timer(void)
{
	if(LTC2944_Data.Timer != 0)LTC2944_Data.Timer--;
}

	 
	 //---
/*
	��������float LTC2944_Cal_V(u16 RedV)
	������ ����ʵ�Ƶ�ѹֵ70.8*regV/65535
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
	��������LTC2944_Cal_I(u16 RedI)
	������ ����ʵ�Ƶ���ֵ64mv/LTC2944_RefR*(RedI-32767)/32767
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
	��������LTC2944_Cal_Temp(u16 RedTemp);
	������ ����ʵ���¶�ֵ
*/ 
float LTC2944_Cal_Temp(u16 RedTemp)
{
	return((float)510*RedTemp/65535-271);
}

/*
	��������LTC2944_Cal_PTC(u16 RedACC)
	������ ����ʣ������
RedACC :LTC2944�����Ŀ���ֵ
ACCmax:Ϊ������ʱ��ֵ
����ʣ������0~100��Χ
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
	��������LTC2944_Cal_ACC(float VOL)
	������ ����ʣ������
VOL :��ص�ѹֵ
ACCmax:Ϊ������ʱ��ֵ
����ʣ������0~100��Χ
*/ 
u16 LTC2944_Cal_ACC(float VOL)
{
	float bufmin,buf1;
	bufmin=LTC2944_ACCMin;
	buf1=(VOL-BATTERY_Vcmp)*(LTC2944_ACCMax-bufmin)/(BATTERY_VMAX-BATTERY_Vcmp)+bufmin;
	return buf1;
}
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void DS18B20_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪDS18B20�ļ���ASCII�����ļ�
ͨѶ��ʽ��:000|03|xxx
*/
void LTC2944_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
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
