/*
	���ļ�Ϊ����Ŀ��EEPROM�洢оƬFM24CL64��غ���
	���庯������
*/

#include "FM24CL64.h"

FM24CL64_DataStructure FM24CL64Data;
/*
	��������void FM24CL64_Main(void)
	������	heat.c������
*/
void FM24CL64_Main(void)
{
	
}
//---
/*
	��������void FM24CL64_Timer(void)
	������FM24CL64.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void FM24CL64_Timer(void)
{
	if(FM24CL64Data.Timer != 0) FM24CL64Data.Timer--;
}
//---
/*
	��������void FM24CL64_Work(void)
*/
//---
/*
	��������void FM24CL64_Init(void)
	������	FM24CL64.c�ļ���ʼ������
*/
void FM24CL64_Init(void)
{	
	IIC_Init();

}
//---
/*
	��������FM24CL64_DataStructure* FM24CL64_GetData(void)
	��������ȡ����FM24CL64.c�ļ�ȫ�ֱ����ṹ�����
*/
FM24CL64_DataStructure* FM24CL64_GetData(void)
{
	return &FM24CL64Data;
}
//---
/*
	��������void FM24CL64_WriteData(void)
	������	�ú���Ϊ��������
*/

u8 FM24CL64_ReadOneByte(u16 ReadAddr)
{				  
  	u8 temp=0;		  	    																 
    IIC_Start();  
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ	    
  	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	  IIC_Wait_Ack();	    
	  IIC_Start();  	 	   
	  IIC_Send_Byte(0XA1);           //�������ģʽ			   
	  IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void FM24CL64_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{				   	  	    																 
    IIC_Start();  
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ	  
  	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	  IIC_Wait_Ack(); 	 										  		   
	  IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	  IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	  delay_ms(10);	 
}



void FM24CL64_WriteData(void)
{
	u16 i,j,yMAX,xMAX;
	u16 len = FLASHIS_AdrEnd;//�������ݳ���
	len=len*2;
	u8 *p = (u8*)&(Flashisp_GetData()->Data[0]);
	yMAX=(u16)(len/32);   //����������
	if(len%32)  //������Ϊ��
	yMAX++;         //��32Ϊ1ҳȥд ,����jҳ
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
	IIC_Send_Byte(FM24CL64_CMD_WRITE);//д����
	IIC_Wait_Ack();
	IIC_Send_Byte((u8)(j*32/256));		//�ӵ�ַj*32��ʼд
	IIC_Wait_Ack();
	IIC_Send_Byte((u8)((j*32)%256));
	IIC_Wait_Ack();
	for(i=0;i<xMAX;i++)
	{//��ʼд����
		IIC_Send_Byte(*p);
		IIC_Wait_Ack();
		p++;
	}
	IIC_Stop();
	//------------
	delay_us(10); //��ʱ10us��һҳ
 }

}
void FM24CL64_TestWriteData(void)
{
	u16 i;
	IIC_Start();
	IIC_Send_Byte(FM24CL64_CMD_WRITE);//д����
	IIC_Wait_Ack();
	IIC_Send_Byte(0);		//�ӵ�ַ0��ʼд
	IIC_Wait_Ack();
	IIC_Send_Byte(0);
	IIC_Wait_Ack();
	for(i=0;i<500;i++)
	{//��ʼд����
		IIC_Send_Byte(i);
		IIC_Wait_Ack();
	}
	IIC_Stop();
}
//---
/*
	��������void FM24CL64_WriteData(void)
	������	�ú���Ϊ��ȡ����
*/
void FM24CL64_ReadData(void)
{
	u16 i;
	u16 len =FLASHIS_AdrEnd;
  
	u8 *p = (u8*)&(Flashisp_GetData()->Data[0]);
	len=len	* 2;//��ȡ���ݳ���
	
	for(i=0;i<len;i++)
	{//��ʼ������
	
		*p =(u8)FM24CL64_ReadOneByte(i);
		p++;
	}

}
void FM24CL64_TestReadData(void)
{
	u16 i;
	IIC_Start();
	IIC_Send_Byte(FM24CL64_CMD_WRITE);//д����
	IIC_Wait_Ack();
	IIC_Send_Byte(0);		//�ӵ�ַ0��ʼ��
	IIC_Wait_Ack();
	IIC_Send_Byte(0);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(FM24CL64_CMD_READ);//������
	IIC_Wait_Ack();
	for(i=0;i<500;i++)
	{//��ʼ������
		printf("%x ",IIC_Read_Byte(1));
	}
	IIC_Stop();
	printf("\r\n\r\n ");
}
//---
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void FM24CL64_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪFM24CL64.c�ļ���ASCII�����ļ�
*/
void FM24CL64_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
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
	�������� 	void FM24CL64_DebugRTU((u8 *p)
	������		�ú���ΪFM24CL64.c�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*//*
void FM24CL64_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//�鿴���ĸ�������
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





































