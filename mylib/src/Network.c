/*
	���ļ�Ϊ����Ŀ��������غ���
	���庯������
*/

#include "Network.h"

Network_DataStructure NetworkData;

/*
	��������void Network_Main(void)
	������	��Ļ��ʾ������
*/
void Network_Main(void)
{
  #ifndef	OPEN_PRINTF
	char databuf[50];
if(NetworkData.AtMode==0) //��������ģʽ
	{
	switch(NetworkData.RunMode)
	{
		case 0:    //����״̬   
		{
			break;
		}
		case 1:    //���ݳ�ʼ��
		{
      NetworkData.SendCnt=0;
			NetworkData.RunMode=2;
			break;
		}
		case 2:    //��������
		{	NetworkData.Network_ReceivData.flag=0;
			Network_SendOneFrameData();    //�������ݽṹ���������
			NetworkData.Timer=1000;        //��ʱ1��ȴ���������
			NetworkData.RunMode=3;
			break;
		}
		case 3:    //�ڶ�ʱʱ���ڵȴ���������
		{
			if(NetworkData.Timer==0)   //��ʱ
			{
				NetworkData.RunMode=4;
			}
			else                       //��ѯ�Ƿ������ݽӻ���
			{
				if(NetworkData.Network_ReceivData.flag)  //�з������ݣ���������
				{
					if(NetworkData.Network_ReceivData.DataArr[2]==0xFA)                   //��λ��Ҫ����(0XAA,0X55,0X01,0XFA)����֤����λ������������ȷ
					{
						NetworkData.RunMode=5;  //��������
					}
					else  //���մ���
					{
						NetworkData.RunMode=4;
					}
				}
			}
			break;
		}
		case 4:    //��ʱ��������մ���
		{

			NetworkData.SendCnt++;
			if(NetworkData.SendCnt>=3)  //�ظ�����
			{
				NetworkData.RunMode=0;
			}
			else
			{
				NetworkData.RunMode=2;   //�ظ�����
			}
			break;
		}
		case 5:     //��������
		{
			NetworkData.RunMode=0;   //ת������״̬
			break;
		}
			
		case 100:     //��ʱ�ȴ�
		{
			if(NetworkData.Timer==0)
			{
				
		    NetworkData.RunMode=NetworkData.NextMode;
			}
			break;
		}

	}
 }
	else   //AT����ģʽNetworkData.AtMode=1
	{
		 switch(NetworkData.RunMode)
		 {
			 case 0:    //����״̬   
		   {
			  break;
		   }
			 case 1:    //��ʼ������,Ϊ��һ����׼��
		   { NetworkData.SendCnt=0;   //���ּ�����
				 NetworkData.LinkErrFlag=0;
				 NetworkData.RunMode=2;
			  break;
		   }
			 case 2:    //
		   {
				NetworkData.Timer=100;   //����ǰ��ʱ100ms,������100ms
				NetworkData.RunMode=3;
			  break;
		   }
			 case 3:    //
		   {
				if(NetworkData.Timer==0)
				{
					NetworkData.RunMode=4;
				}
					  break;
		   }
       case 4:    //
		   {  Printf_GetData()->Printf_Mode=1;  //��sprintf ��ʹ�ø���usart1
				  sprintf((char*)&databuf[0],"+++");
				  Network_SendNByteData((u8 *)&databuf,3);  //����+++
				  NetworkData.USART_RX_CNT=0;
				  NetworkData.SEND_OK=0;    
				  
				  NetworkData.Timer=1000;   //���ּ�����100ms
				  NetworkData.RunMode=5;
					break;
		   }
       case 5:    //
		   {  if(NetworkData.Timer==0)  //��ʱ������ʱ
				  {
						NetworkData.SendCnt++;
						if(NetworkData.SendCnt>=3)  //3�����Ӳ��ϣ�����
						{
							NetworkData.RunMode=0;
							NetworkData.LinkErrFlag=1;  //��ʱ
						}
						else
						{
							NetworkData.RunMode=2;  //�����·���+++
						}
					}
          else  //��û��ʱʱ���ڼ����յ������Ƿ���ȷ
					{
						if(NetworkData.SEND_OK==1)   //��������'a' 
						{
							NetworkData.RunMode=6;
						}
					}						
				  break;
			 }
			 case 6:
			 {  databuf[0]='a';
				  Printf_GetData()->Printf_Mode=1;  //��sprintf ��ʹ�ø���usart1
				  Network_SendNByteData((u8 *)&databuf,1);  //����'a'
				  NetworkData.USART_RX_CNT=0;
				  NetworkData.SEND_OK=0;
				  NetworkData.Temp='1';
				  NetworkData.Timer=1000;   //���ּ�����100ms
				  NetworkData.RunMode=7;
				  break;
			 }
			 case 7:
			 { if(NetworkData.Timer==0)  //��ʱ
				  {
							NetworkData.RunMode=0;
							NetworkData.LinkErrFlag=1;  //��ʱ
					}
				 else //�ڷǳ�ʱʱ���ڼ��
				  {
						if(NetworkData.SEND_OK==1)   //��������'a' 
						{
							NetworkData.RunMode=8;
						}
				  }
				 break;
			 }
			case 8:
			 {  
				 FourBytes four;
				 u16 i;
				 char buf[20];
			   u16 *pf = Flashisp_GetData()->Data;//Flashָ��
			   four.U16Data[0] = pf[FLASHIS_AdrWLSZIPDZ];
			   four.U16Data[1] = pf[FLASHIS_AdrWLSZIPDZ+1];
				 Printf_GetData()->Printf_Mode=1;  //��sprintf ��ʹ�ø���usart1
			   //IP��ַ
			   databuf[0]=0;
				 sprintf((char*)&databuf[0],"AT+SOCKA=TCP,");
			   sprintf((char*)&buf[0],"%u",four.U8Data[3]);//192
		     strcat(databuf,(char *)&buf[0]);		  
			   strcat(databuf,".");	
			   sprintf((char*)&buf[0],"%u",four.U8Data[2]);//168
			   strcat(databuf,(char *)&buf[0]);		  
			   strcat(databuf,".");	
			   sprintf((char*)&buf[0],"%u",four.U8Data[1]);//1
			   strcat(databuf,(char *)&buf[0]);		  
			   strcat(databuf,".");	
			   sprintf((char*)&buf[0],"%u",four.U8Data[0]);//1
		     strcat(databuf,(char *)&buf[0]);		  
			   strcat(databuf,",");	
			   //�˿ں�
			   sprintf((char*)&buf[0],"%u",pf[FLASHIS_AdrWLSZDKH]);
				 strcat(databuf,(char *)&buf[0]);
	       strcat(databuf,"\r\n\0");
				 for(i=0;i<sizeof(databuf);i++)
				 {
					 if(databuf[i]=='\0')
					 {
						 break;
					 }
				 }
				
				  Network_SendNByteData((u8 *)&databuf,i);  //����'AT+SOCKA=TCP,test.usr.cn,2317'
				  NetworkData.USART_RX_CNT=0;
				  NetworkData.SEND_OK=0;
				  NetworkData.Temp='1';
				  NetworkData.Timer=1000;   //���ּ�����100ms
				  NetworkData.RunMode=9;
				  break;
			 }
			 case 9:
			 { if(NetworkData.Timer==0)  //��ʱ
				  {
					NetworkData.RunMode=0;
					NetworkData.LinkErrFlag=1;  //��ʱ
					}
				 else //�ڷǳ�ʱʱ���ڼ��
				  {
						if(NetworkData.SEND_OK==1)   //��������'a' 
						{
							NetworkData.RunMode=10;
						}
				  }
				 break;
			 }
			 case 10:             //�����˳�AT����ģʽ,������ͨ��
			 {  Printf_GetData()->Printf_Mode=1;  //��sprintf ��ʹ�ø���usart1
				  sprintf((char*)&databuf[0],"AT+Z\r\n");
				  Network_SendNByteData((u8 *)&databuf,6);  //����'��λ'
				  NetworkData.USART_RX_CNT=0;
				  NetworkData.SEND_OK=0;
				  NetworkData.Temp='1';
				  NetworkData.Timer=1000;   //���ּ�����100ms
				  NetworkData.RunMode=11;
				  break;
			 }
			 case 11:
			 { if(NetworkData.Timer==0)  //��ʱ
				  {
					NetworkData.RunMode=0;
					NetworkData.LinkErrFlag=1;  //��ʱ
					}
				 else //�ڷǳ�ʱʱ���ڼ��
				  {
						if(NetworkData.SEND_OK==1)   //��������'��λ' 
						{
							NetworkData.RunMode=0;
							NetworkData.AtMode=0;     //������������ģʽ
							NetworkData.LinkErrFlag=2;
						}
				
				  }
				 break;
			 }
			 case 31:   //��ʱ
			 {
				 NetworkData.Timer=10000;  //������ʱ1s ,��ģ��
				 NetworkData.HandOKflag=0;
				 NetworkData.SendCnt=0;
				 NetworkData.RunMode=32;
				 break;
			 }
			 case 32:  //���ֲ��� ����+++��
			 {
				if(NetworkData.Timer==0) //��ʱ��,������������
				 {
					Printf_GetData()->Printf_Mode=1;  //��sprintf ��ʹ�ø���usart1
				  sprintf((char*)&databuf[0],"+++");
				  Network_SendNByteData((u8 *)&databuf,3);  //����+++
				  NetworkData.USART_RX_CNT=0;
				  NetworkData.SEND_OK=0;    //��ս��ճɹ���־
				  NetworkData.Timer=500;   //���ּ�����100ms
					NetworkData.RunMode=33;
				 }
				 break;
			 }
			 case 33:  //�ȴ�����a
			 {
				 if(NetworkData.Timer==0)  //��ʱ��,��ʱ
				  {
						NetworkData.SendCnt++;
						if(NetworkData.SendCnt>=3)  //3�����Ӳ��ϣ�����
						{
							NetworkData.RunMode=0;
							NetworkData.AtMode=0;     //������������ģʽ
							NetworkData.HandOKflag=2;  //��ʱ,����ʧ��
						}
						else
						{
							NetworkData.RunMode=32;  //�����·���+++
						}
					}
          else  //��û��ʱʱ���ڼ����յ������Ƿ���ȷ
					{
						if(NetworkData.SEND_OK==1)   //��������'a' 
						{
							NetworkData.HandOKflag=1;  //���ֳɹ�
							NetworkData.AtMode=0;     //������������ģʽ
							NetworkData.RunMode=0;
						}
					}					
				 break;
			 }
					 
		 }
	}
	#endif
}
//---
/*
	��������void Network_Timer(void)
	������Network.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void Network_Timer(void)
{ 
	#ifndef	OPEN_PRINTF
	if(NetworkData.Timer>0)
	NetworkData.Timer--;
	
	NetworkData.Timer_50ms++;
	if(NetworkData.Timer_50ms>=50)
	{
		NetworkData.Timer_50ms=0;
		 if(NETLED_4G_IN)                  //����Ϊ�ߵ�ƽ???????????????????????????????????????????????
		 {
			  NetworkData.Hcnt++;
		 }
		NetworkData.Timer_3s++;
		if(NetworkData.Timer_3s>=60)
		{
			NetworkData.Timer_3s=0;
			
			if((NetworkData.Hcnt>6)&&(NetworkData.Hcnt<50))   //���ߵ�ƽ�ж�С��
			{
				NetworkData.NetworkStatus=1;                    //��������
			}
			else
			{
				NetworkData.NetworkStatus=0;                     //����ʧ��
			}
			NetworkData.Hcnt=0;
		}
	}
		#endif
}
//---
/*
	��������void  Network_RegInit(void)
	������	Network.C ���ݽṹ�ĳ�ʼ������
*/
void Network_RegInit(void)
{
 NetworkData.SendCnt=0;     //��ʼ��Ϊ0
 NetworkData.Hcnt=0;        //�ߵ�ƽ������
 NetworkData.NetworkStatus=0;   //����״̬��ʼ��	
 NetworkData.RunMode=31;
 NetworkData.AtMode=1; //��ʼ��ΪAT ����ģʽ���ڿ�������
}
/*
	��������void  Network_Text(void)
������	������������Ƿ���ͨ��
*/
void Network_Test(void)
{

 NetworkData.Timer=100;  //������ʱ1s ,��ģ��
 NetworkData.HandOKflag=0;
 NetworkData.SendCnt=0;
 NetworkData.RunMode=32;
 NetworkData.AtMode=1; //����ΪAT ����ģʽ���ڿ�������
}
//---
/*
	��������void Network_Init(void)
	������	Network.c�ļ���ʼ������
*/
void Network_Init(void)
{
		#ifndef	OPEN_PRINTF
  //���Ӽ������״̬������ŵ�GPIO ����
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(NETLED_4G_APP, ENABLE);
	RCC_AHB1PeriphClockCmd(POWER_4G_APP, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = NETLED_4G_PIN;	
  GPIO_Init(NETLED_4G_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //��Դ���ƽ�����Ϊ���
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = POWER_4G_PIN;	
	GPIO_Init(POWER_4G_PORT, &GPIO_InitStructure);
	POWER_4G_OUT(1);  //�͵�ƽ��Ч
	
	Network_RegInit();
	
	#endif
	
}


//---
/*
	��������Network_DataStructure* Network_GetData(void)
	��������ȡ����Network.c�ļ�ȫ�ֱ����ṹ��
*/
Network_DataStructure* Network_GetData(void)
{
	return &NetworkData;
}
//---
//-------------------------���ݶ�ȡ���洢----------------------------
//---
/*
	��������void Network_ReadData(u8 address,u8 *p,u8 len)
	������	��ȡ����
*/
void Network_ReadData(u8 address,u8 *p,u8 len)
{
//	Flashisp_ReadDataU8(NETWORK_AdrStoreBase+address,p,len);
}


//---
/*
��������void Network_SendOneFrameData(u8 *p)
�������ú���Ϊ����һ֡���ݺ���
˵����
*/

void Network_SendOneFrameData(void)
{ 
       u16 i;
	     USART1_SendByte(NetworkData.Network_SendData.HanderH);   //֡ͷ��һ�ֽ�
	     USART1_SendByte(NetworkData.Network_SendData.HanderL);   //֡ͷ�ڶ��ֽ�
	     USART1_SendByte(NetworkData.Network_SendData.Length);    //���ݳ���
	     for(i=0;i<=NetworkData.Network_SendData.Length;i++)      //����+CRC
	      {
				 USART1_SendByte(NetworkData.Network_SendData.DataArr[i]);
				}
}
//---
/*
��������void Network_SendOneFrameData(u8 *p)
�������ú���Ϊ����һ֡���ݺ���
˵����
*/

void Network_SendNByteData(u8 *p,u8 Nbyte)
{ u8 i;
	for(i=0;i<Nbyte;i++)
	{
		USART1_SendByte(*p);
		p++;
	}
	
}
//---
/*
	��������void Network_FactoryReset(void)
	�������ú���Network.c�洢���ݻָ���������
*/
void Network_FactoryReset(void)
{

	printf("Network������ʼ��\r\n");
}
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Network_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪNetwork�ļ���ASCII�����ļ�
*/
void Network_DebugASCII(u8 *p,u8 len)
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

//---





/*******************************************************************************
	��������USART1_IRQHandler
	��  ��:
	��  ��:
	����˵����
	����1�жϷ������
*/
#ifndef	OPEN_PRINTF
void USART1_IRQHandler(void)                
{
	u8 Res;
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0xaa 0x55��ʼ)
	{
	  Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
	if(NetworkData.AtMode==0)   //��������ģʽ
	  {
		 if(NetworkData.USART_RX_STA==0)
		   {
			    if (Res==0Xaa) {NetworkData.USART_RX_STA=1;}
		   }
		 else if(NetworkData.USART_RX_STA==1)
		   {
				  if (Res==0X55) {NetworkData.USART_RX_STA=2;}
		   }
			 else if(NetworkData.USART_RX_STA==2)   //���ռ��ֽ���
			 {  
				  if(Res>USART_REC_LEN-1)  //��Ŀ���������մ���
          {NetworkData.USART_RX_STA=0;}        //���¿�ʼ����
          else
          {
						NetworkData.USART_RX_BUF[0]=Res;   //���泤��
						NetworkData.USART_RX_STA=3;
						NetworkData.USART_RX_CNT=1;
	 				}						
			 }
		 else if(NetworkData.USART_RX_STA==3)     //��ʼ��������
		   {
				  NetworkData.USART_RX_BUF[NetworkData.USART_RX_CNT]=Res ;
				  NetworkData.USART_RX_CNT++;
				  if(NetworkData.USART_RX_CNT>NetworkData.USART_RX_BUF[0])
					{
					NetworkData.USART_RX_STA=0;
						
					Network_GetData()->Network_ReceivData.flag=1;
						
					}  //�������,������ʱ��ѯ���־�������������յ�����
		   }
		 }
		 else   //AT ����ģʽ��������
		 {
		    if(NetworkData.USART_RX_CNT>=USART_REC_LEN)  //��������
				 {
				   NetworkData.USART_RX_CNT=0; //���´�0����
			   }

				 NetworkData.USART_RX_BUF[NetworkData.USART_RX_CNT++] =Res;
				 
         switch(NetworkData.RunMode)
				 {
				 case 5:
				 case 33:
				  {
					  if(Res==0x61)
						 {
						   if(NetworkData.USART_RX_CNT<=2)
							  {
								 NetworkData.SEND_OK=1;
							  }
						  }
					 break;
				  }
					case 7:
					{
					    if(Res==0x6B)  //'k'
							 {
							  if(NetworkData.Temp==0x6F) //'o'
								 {
								  NetworkData.SEND_OK=1;
							   }
						   }
	         NetworkData.Temp=Res;
					 break;
				  }
					case 9: 
					case 11:
					{
					 if(Res==0x4B)  //'K'
							 {
							  if(NetworkData.Temp==0x4F) //'O'
								 {
								  NetworkData.SEND_OK=1;
							   }
						   }
	         NetworkData.Temp=Res;
					 break;
				  }
			   }
	   }
		 
  } 

} 
#endif




























