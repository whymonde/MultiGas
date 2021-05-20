/*
	���ļ�Ϊ������غ����ļ�
	��д��2014��4��19��
*/

#include "usart.h"

#define OPEN_USART1_COM
#define OPEN_USART2_COM	  //����ͨѶ
#define OPEN_USART3_COM			
#define OPEN_UART4_COM   //������ӡ
#define OPEN_UART5_COM
//#define OPEN_USART6_COM			//����λ��ͨѶ
//#define OPEN_USART7_COM			//��Ӱ�ͨѶ
//#define OPEN_USART8_COM
/*
	�������� USART_InitAll()
	������	 ��ʼ�������ļ�USART
*/
extern u8 AnalysisCRC(u8 *pstr);
void USART_InitAll(void)
{
	#ifdef OPEN_USART1_COM
	USART1_Configuration();		
  #endif 
#ifdef OPEN_USART2_COM
	USART2_Configuration();		
  #endif 
#ifdef OPEN_USART3_COM
	USART3_Configuration();		
  #endif 
#ifdef OPEN_UART4_COM
	UART4_Configuration();		
  #endif 	
#ifdef OPEN_UART5_COM
	UART5_Configuration();		
  #endif 	
	
#ifdef OPEN_USART6_COM
	USART6_Configuration();	
#endif 
}
//---
/*
	��������	void USART_main(void)
	������		�ú���Ϊusart.c�ļ�������������ϵͳ����������
	ʱ�䣺		20150804
*/
void USART_main(void)
{
	#ifdef OPEN_USART1_COM
	USARTx_DW(USART1,Get_USART1Buffer());
	#endif 
//	#ifdef OPEN_USART2_COM
//	USARTx_DW(USART2,Get_USART2Buffer());
//	#endif 
//	#ifdef OPEN_USART3_COM
//	USARTx_DW(USART3,Get_USART3Buffer());
//	#endif 
	#ifdef OPEN_USART6_COM
  USARTx_DW(USART6,Get_USART6Buffer());
	#endif 
	
	
	
}
//---
/***************************
	�������� void USART3_Timer(USART_TypeDef* USARTx,USART_DataStructure *p)
	������	 �ú���Ϊ���ļ�(�����ļ�)��ʱ������USART_Timer()�ܵ���
	���ú�����
				USART3_Timer(); //����3��ʱ��
**************************************************************/
void USART2or3_Timer(USART_TypeDef* USARTx,USART_DataStructure *p)
 {
	if(p->Timer>0)
	{ 
		p->Timer--;  //���㳬ʱ
		if(p->Timer==0)
		{
			USART_ITConfig(USARTx, USART_IT_RXNE, DISABLE);		//���ж�
			p->RxFlag=0;     
      		//���¿�ʼ����
			USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);		//���ж�
		}
	}
	else
	{
		p->RxFlag=0;
	}
 }
//----
/*
	�������� void USART_Timer(void)
	������	 �ú���Ϊ���ļ�(�����ļ�)��ʱ����1ms����1ms��ʱ��������
	���ú�����
				USART3_Timer(); //����3��ʱ��
*/
void USART_Timer(void)
{
		#ifdef OPEN_USART1_COM
		USARTx_Timer(USART1,Get_USART1Buffer());
	#endif 
	
	#ifdef OPEN_USART2_COM
	 USART2or3_Timer(USART2,Get_USART2Buffer());	
	#endif 
	 #ifdef OPEN_USART3_COM
	 USART2or3_Timer(USART3,Get_USART3Buffer());
  	#endif

}
//-----------------------------------------------------------------------------------
//����1���
#ifdef OPEN_USART1_COM
//---
/*
	��������void USART1_Configuration(void)
	������	����1��ʼ��
*/
void USART1_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//ʹ��USART1ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	//USART1�˿�����
	//����USART1 TX(PA.09)Ϊ���ù����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//����USART1 RX(PA.10)Ϊ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//����USART1���п�
	USART_InitStructure.USART_BaudRate = 115200;// 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	//ʹ�ܷ����жϣ����жϲ���ʱ��USART1�������ݼĴ���Ϊ��
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	//ʹ��USART1ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//ʹ�ܽ����жϣ����жϲ���ʱ��USART1�������ݼĴ�����Ϊ��
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//ʹ��USART1
	USART_Cmd(USART1, ENABLE);
}
//---
USART_DataStructure USART1_Buffer;		//����2�ṹ��
/*
	��������USART_DataStructure *Get_USART1Buffer(void)
	������	��ȡ����1�ṹ������
*/
USART_DataStructure *Get_USART1Buffer(void)
{
	return &USART1_Buffer;
}
//---
/*
	�������� void USART1_IRQHandler(void)
	������	 ����1�жϷ������,����3�����ж�ʱ������ú���

*/

#ifdef	OPEN_PRINTF
void USART1_IRQHandler(void)                	//����3�жϷ������
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		USARTx_Receive(USART1,&USART1_Buffer,USART1->DR);
	} 
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		USARTx_Send(USART1,&USART1_Buffer);
	}
} 
#endif

//---
//�������� USART1_SendByte(u8 TxData)

//	������USART1�ڷ���һ�ֽ�����	

void USART1_SendByte(u8 TxData)
{
	while((USART1->SR & 0X40)==0);												//
	USART1->DR = (u8) TxData;  
}


#endif


//-----------------------------------------------------------------------------------
//����2
#ifdef OPEN_USART2_COM
//---
/*
	��������void USART1_Configuration(void)
	������	����1��ʼ��
*/
void USART2_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//ʹ��USART1ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	//USART1�˿�����
	//����USART2 TX(PA.02)Ϊ���ù����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//����USART2 RX(PA.3)Ϊ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//����USART2���п�
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	//ʹ�ܷ����жϣ����жϲ���ʱ��USART2�������ݼĴ���Ϊ��
//	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	//ʹ�ܽ����жϣ����жϲ���ʱ��USART2�������ݼĴ�����Ϊ��
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//ʹ��USART12
	USART_Cmd(USART2, ENABLE);
	//ʹ��USART2ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//---
USART_DataStructure USART2_Buffer;		//����2�ṹ��
/*
	��������USART_DataStructure *Get_UASRT2Buffer(void)
	������	��ȡ����2�ṹ������
*/
USART_DataStructure *Get_USART2Buffer(void)
{
	return &USART2_Buffer;
}
//---
/*
	�������� void USART2_IRQHandler(void)
	������	 ����2�жϷ������,����3�����ж�ʱ������ú���
*/
void USART2_IRQHandler(void)                	//����3�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		//�����д�ж�Э�� :0XAA,����,����Ч��
		Res =USART_ReceiveData(USART2);			//�ж���ȡ������
	                                      //	USART3_Buffer.RxBuffer[USART3_Buffer.RxCount] = Res;                   //
		if(USART2_Buffer.RxFlag==0)         //��ǰ���յ�״̬Ϊ�ȴ�����0X5A
		{
			 if(Res==0xAA)
			 {
			 USART2_Buffer.RxFlag=1;
			 //����֡��ʱ�� 30ms
			 USART2_Buffer.Timer=30;
				 
			 }
		}
		else if(USART2_Buffer.RxFlag==1)
		{
			if(Res<94)  //���ƽ����ֽ������ֵ
			{
				USART2_Buffer.RxCount=Res;   //��������ֽ���
				USART2_Buffer.RxFlag=2;
				USART2_Buffer.i=0;
				USART2_Buffer.RxBuffer[USART2_Buffer.i]=Res; //��һ�ֽڱ������������յ��ֽ���
				USART2_Buffer.i=1;
			}
			else
			{
				USART2_Buffer.RxFlag=0;   //�����ֽڳ�����Χ������
			}
		}
		else if (USART2_Buffer.RxFlag==2)
		{
			USART2_Buffer.RxBuffer[USART2_Buffer.i]=Res;
			USART2_Buffer.i++;
			if(USART2_Buffer.i>USART2_Buffer.RxCount)  //�����괦��
			{
			 //	if(USART1_Buffer.RxBuffer[1]==0x01)  //
				{
					for(Res=0;Res<=USART2_Buffer.RxCount;Res++)
					 {
						 USART2_Buffer.ProgressBuffer1[Res]= USART2_Buffer.RxBuffer[Res];
						 USART2_Buffer.BufFlg1=1;   //��һ������������
					 }
				}

				USART2_Buffer.Timer=0;  //��ʱ��
			}
		}
		else
		{
		USART2_Buffer.RxFlag=0;	
		}

	} 
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		USARTx_Send(USART2,&USART2_Buffer);
	}
} 
//---
#endif
//-----------------------------------------------------------------------------------
//����3
#ifdef OPEN_USART3_COM
//---
/*
	�������� USART3_Init()
	������	 ����3��ʼ������
	���룺	 ��
	�����	 ��
	���أ�	 ��
*/
void USART3_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
//	RCC_AHB1PeriphClockCmd(RS485DIR_APP, ENABLE);	//ʹ��GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	//ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3ʱ��
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
	//USART1�˿�����
	//����USART3 TX(PB.10)Ϊ���ù����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//����USART3 RX(PB.11)Ϊ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//����USART1���п�
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	//ʹ�ܷ����жϣ����жϲ���ʱ��USART1�������ݼĴ���Ϊ��
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	//ʹ�ܽ����жϣ����жϲ���ʱ��USART1�������ݼĴ�����Ϊ��
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//	USART_ITConfig(USART3, USART_IT_TC, ENABLE);		//�������ݷ�����ϲ����ж�
	//ʹ��USART1
	USART_Cmd(USART3, ENABLE);
	//ʹ��USART3�ж�
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//---
USART_DataStructure USART3_Buffer;		//����3�ṹ�����
//USART_DataStructure USART11_Buffer;		//����3�ṹ�����
/*
	��������USART_DataStructure *Get_UASRT3Buffer(void)
	������	��ȡ����3�ṹ������
*/
USART_DataStructure *Get_USART3Buffer(void)
{
	return &USART3_Buffer;
}
//---
/*
	�������� void USART3_IRQHandler(void)
	������	 ����3�жϷ������,����3�����ж�ʱ������ú���
*/
void USART3_IRQHandler(void)                	//����3�жϷ������
{  u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		//�����д�ж�Э��
		Res =USART_ReceiveData(USART3);			//�ж���ȡ������
	                                      //	USART3_Buffer.RxBuffer[USART3_Buffer.RxCount] = Res;                   //
		if(USART3_Buffer.RxFlag==0)         //��ǰ���յ�״̬Ϊ�ȴ�����0X5A
		{
			 if(Res==0x5a)
			 {
			 USART3_Buffer.RxFlag=1;
			 //����֡��ʱ�� 30ms
			 USART3_Buffer.Timer=30;
				 
			 }
		}
		else if(USART3_Buffer.RxFlag==1)    //��ǰ���յ�״̬Ϊ�ȴ�����0XA5
		{
			if(Res==0xa5)
			{
			 USART3_Buffer.RxFlag=2;
			}
			else
			{
			 USART3_Buffer.RxFlag=0;   //�ڶ��ֽڲ�������ͷ��ʼ����
			}
		}
		else if(USART3_Buffer.RxFlag==2)
		{
			if(Res<50)
			{
				USART3_Buffer.RxCount=Res;   //��������ֽ���
				USART3_Buffer.RxFlag=3;
				USART3_Buffer.i=0;
				USART3_Buffer.RxBuffer[USART3_Buffer.i]=Res; //��һ�ֽڱ������������յ��ֽ���
				USART3_Buffer.i=1;
			}
			else
			{
				USART3_Buffer.RxFlag=0;   //�����ֽڳ�����Χ������
			}
		}
		else if (USART3_Buffer.RxFlag==3)
		{
			USART3_Buffer.RxBuffer[USART3_Buffer.i]=Res;
			USART3_Buffer.i++;
			if(USART3_Buffer.i>USART3_Buffer.RxCount)  //�����괦��
			{
				if(USART3_Buffer.RxBuffer[1]==0x81)  //������0x81ָ��
				{
					for(Res=0;Res<=USART3_Buffer.RxCount;Res++)
					 {
						 USART3_Buffer.ProgressBuffer1[Res]= USART3_Buffer.RxBuffer[Res];
						 USART3_Buffer.BufFlg1=1;   //��һ������������
//						 DEBUG_MESSAGE("0x%X,	",USART3_Buffer.ProgressBuffer1[Res]);
					 }
//					 DEBUG_MESSAGE("\r\n");
				}
				else                                  //������0x83ָ��/���������
				{
					for(Res=0;Res<=USART3_Buffer.RxCount;Res++)
					 {
						   USART3_Buffer.ProgressBuffer2[Res]= USART3_Buffer.RxBuffer[Res];
						   USART3_Buffer.BufFlg2=1; //�ڶ�������������
						 DEBUG_MESSAGE("0x%X,	",USART3_Buffer.ProgressBuffer2[Res]);
					 }
					 DEBUG_MESSAGE("\r\n");
				}
				USART3_Buffer.Timer=0;  //��ʱ��
			}
			
		}
		else
		{
		USART3_Buffer.RxFlag=0;	
		}
		
	} 
	
	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		USARTx_Send(USART3,&USART3_Buffer);
	}
} 


//---
#endif
//-----------------------------------------------------------------------------------
//����4
#ifdef OPEN_UART4_COM	
//---
USART_Com4Structure USART_COM4_Data;

USART_Com4Structure *USART_COM4_GetData(void)
{
	return &USART_COM4_Data;
}
//---
/*
	�������� void UART4_Configuration(void)
	������	 ����4��ʼ������
	���룺	 ��
	�����	 ��
	���أ�	 ��
*/
void UART4_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//ʹ��USART4ʱ��
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
	//USART4�˿�����
	//����UART4 TX(PC.10)Ϊ���ù����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//����UART4 RX(PC.11)Ϊ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//����UART4���п�
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure);
	//ʹ�ܷ����жϣ����жϲ���ʱ��USART1�������ݼĴ���Ϊ��
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	//ʹ�ܽ����жϣ����жϲ���ʱ��USART1�������ݼĴ�����Ϊ��
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
	//ʹ��USART1
	USART_Cmd(UART4, ENABLE);
	//ʹ��USART3�ж�
	NVIC_InitStructure.NVIC_IRQChannel=UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}




void USART_COM4_Enable(void)
{
	USART_COM4_Data.EnReceveFlg=1;
}
void USART_COM4_Disable(void)
{
	USART_COM4_Data.EnReceveFlg=0;
}

/*

ReceveMode:=0͸��ģʽ 1��AT ģʽ
ReceveMax:���յĸ���
*/
void USART_COM4_Receve_com(u8 ReceveMode,u8 ReceveMax)
{
	USART_COM4_Data.ReceveMax=ReceveMax;      //���ո���
	USART_COM4_Data.ReceveMode=ReceveMode;    //0:͸��ģʽ�½���
	USART_COM4_Data.ReceveOkFlg=0;            //��������־
	USART_COM4_Data.ReceveCnt=0;
	USART_COM4_Data.EnReceveFlg=1;
}
//---

//---
/*
	�������� void UART4_IRQHandler(void)
	������	 ����4�жϷ������,����3�����ж�ʱ������ú���
*/
void UART4_IRQHandler(void)                	//����3�жϷ������
{
u8 Res;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(UART4);//(USART1->DR);	//��ȡ���յ�������
		if(USART_COM4_Data.EnReceveFlg)  //����������
		{ if(USART_COM4_Data.ReceveMode)    //ATģʽ�½���
			{
			USART_COM4_Data.USART_RX_BUF[USART_COM4_Data.ReceveCnt]=Res;  //���浽������
		  USART_COM4_Data.ReceveCnt++;
			if(USART_COM4_Data.ReceveCnt>=USART_COM4_Data.ReceveMax)  
			{ //�������
				USART_COM4_Data.EnReceveFlg=0; //�رս���
				USART_COM4_Data.ReceveOkFlg=1;  //�������
			}
		  }
			else   //͸��ģʽ�½���
			{
			USART_COM4_Data.USART_RX_BUF[USART_COM4_Data.ReceveCnt]=Res;  //���浽������
		    USART_COM4_Data.ReceveCnt++;
			if(USART_COM4_Data.ReceveCnt>=USART_COM4_Data.ReceveMax)  
			{ //�������
				USART_COM4_Data.EnReceveFlg=0;  //�رս���
				USART_COM4_Data.ReceveOkFlg=1;  //�������
			}
			}
		}
	}
} 
//---
#endif
//-----------------------------------------------------------------------------------
//����5
#ifdef OPEN_UART5_COM
//---
//-----------------------------------------------------------------------------------

//---
USART_Com4Structure USART_COM5_Data;

USART_Com4Structure *USART_COM5_GetData(void)
{
	return &USART_COM5_Data;
}
//---
/*
	�������� void UART5_Configuration(void)
	������	 ����5��ʼ������
	���룺	 ��
	�����	 ��
	���أ�	 ��
*/
void UART5_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//ʹ��GPIOCʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	//ʹ��GPIODʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//ʹ��USART5ʱ��
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
	//USART5�˿�����
	//����UART5 TX(PC.12)Ϊ���ù����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//����UART5 RX(PD.2)Ϊ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	//����UART5���п�
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART5, &USART_InitStructure);
	//ʹ�ܷ����жϣ����жϲ���ʱ��USART5�������ݼĴ���Ϊ��
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	//ʹ�ܽ����жϣ����жϲ���ʱ��USART5�������ݼĴ�����Ϊ��
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	//ʹ��USART5
	USART_Cmd(UART5, ENABLE);
	//ʹ��USART5�ж�
	NVIC_InitStructure.NVIC_IRQChannel=UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void USART_COM5_Enable(void)
{
	USART_COM5_Data.EnReceveFlg=1;
}
void USART_COM5_Disable(void)
{
	USART_COM5_Data.EnReceveFlg=0;
}

/*

ReceveMode:=0͸��ģʽ 1��AT ģʽ
ReceveMax:���յĸ���
*/
void USART_COM5_Receve_com(u8 ReceveMode,u8 ReceveMax)
{
	USART_COM5_Data.ReceveMax=ReceveMax;      //���ո���
	USART_COM5_Data.ReceveMode=ReceveMode;    //0:͸��ģʽ�½���
	USART_COM5_Data.ReceveOkFlg=0;            //��������־
	USART_COM5_Data.ReceveCnt=0;
	USART_COM5_Data.EnReceveFlg=1;
}
//---

//---
/*
	�������� void UART5_IRQHandler(void)
	������	 ����5�жϷ������,����3�����ж�ʱ������ú���
*/
void UART5_IRQHandler(void)                	//����5�жϷ������
{
u8 Res;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(UART5);//(USART1->DR);	//��ȡ���յ�������
		if(USART_COM5_Data.EnReceveFlg)  //����������
		{ if(USART_COM5_Data.ReceveMode)    //ATģʽ�½���
			{
			USART_COM5_Data.USART_RX_BUF[USART_COM5_Data.ReceveCnt]=Res;  //���浽������
		  USART_COM5_Data.ReceveCnt++;
			if(USART_COM5_Data.ReceveCnt>=USART_COM5_Data.ReceveMax)  
			{ //�������
				USART_COM5_Data.EnReceveFlg=0; //�رս���
				USART_COM5_Data.ReceveOkFlg=1;  //�������
			}
		  }
			else   //͸��ģʽ�½���
			{
			USART_COM5_Data.USART_RX_BUF[USART_COM5_Data.ReceveCnt]=Res;  //���浽������
		  USART_COM5_Data.ReceveCnt++;
			if(USART_COM5_Data.ReceveCnt>=USART_COM5_Data.ReceveMax)  
			{ //�������
				USART_COM5_Data.EnReceveFlg=0;  //�رս���
				USART_COM5_Data.ReceveOkFlg=1;  //�������
			}
			}
		}
	}
} 
#endif
//-----------------------------------------------------------------------------------
//����6  MAX3485 IC
#ifdef OPEN_USART6_COM
//--
USART_DataStructure USART6_Buffer;		//����7�ṹ��ȫ�ֱ���
void USART6_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);	//ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);	//ʹ��USART4ʱ��
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
	//USART6�˿�����
	//����USART6 TX(PC.6)Ϊ���ù����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//����USART6 RX(PC.7)Ϊ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	//����USART6���п�
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART6, &USART_InitStructure);
	//ʹ�ܷ����жϣ����жϲ���ʱ��USART1�������ݼĴ���Ϊ��
	//USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	//ʹ�ܽ����жϣ����жϲ���ʱ��USART1�������ݼĴ�����Ϊ��
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);
	//ʹ��USART6
	USART_Cmd(USART6, ENABLE);
	//ʹ��USART6�ж�
	NVIC_InitStructure.NVIC_IRQChannel=USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_AHB1PeriphClockCmd(MAX485CTR_APP, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = MAX485CTR_PIN;	
  GPIO_Init(MAX485CTR_PORT, &GPIO_InitStructure);
	MAX485CTR_OUT(1);  //������
	
}


/*
	��������USART_DataStructure *USART6_Buffer(void)
	������	��ȡ����7�ṹ������
*/
USART_DataStructure *Get_USART6Buffer(void)
{
	return &USART6_Buffer;
}
//---
/*
	�������� void USART6_IRQHandler(void)
	������	 ����7�жϷ������,����3�����ж�ʱ������ú���
*/
void USART6_IRQHandler(void)                	//����3�жϷ������
{
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)  //�����ж�
	{//����һ���ֽ�
//	
		USARTx_Receive(USART6,&USART6_Buffer,USART6->DR);
	} 
	if(USART_GetITStatus(USART6, USART_IT_TXE) != RESET)  //�����ж�
	{//����һ���ֽ�

		USARTx_Send(USART6,&USART6_Buffer);	
	}
}
//---
#endif

//-----------------------------------------------------------------------------------
//����7
#ifdef OPEN_USART7_COM
//---
USART_DataStructure UART7_Buffer;		//����7�ṹ��ȫ�ֱ���
/*
	��������USART_DataStructure *UART7_Buffer(void)
	������	��ȡ����7�ṹ������
*/
USART_DataStructure *Get_UART7Buffer(void)
{
	return &UART7_Buffer;
}
//---
/*
	�������� void UART7_IRQHandler(void)
	������	 ����7�жϷ������,����3�����ж�ʱ������ú���
*/
void UART7_IRQHandler(void)                	//����3�жϷ������
{
	if(USART_GetITStatus(UART7, USART_IT_RXNE) != RESET)  //�����ж�
	{//����һ���ֽ�
		USARTx_Receive(UART7,&UART7_Buffer,UART7->DR);	
	} 
	if(USART_GetITStatus(UART7, USART_IT_TXE) != RESET)  //�����ж�
	{//����һ���ֽ�
//		Printf_Send();	
		USARTx_Send(UART7,&UART7_Buffer);		
	}
}
//---
#endif
//-----------------------------------------------------------------------------------
//����8
#ifdef OPEN_USART8_COM
//---
USART_DataStructure UART8_Buffer;		//����7�ṹ��ȫ�ֱ���
/*
	��������USART_DataStructure *UART8_Buffer(void)
	������	��ȡ����7�ṹ������
*/
USART_DataStructure *Get_UART8Buffer(void)
{
	return &UART8_Buffer;
}
//---
/*
	�������� void UART8_IRQHandler(void)
	������	 ����7�жϷ������,����3�����ж�ʱ������ú���
*/
void UART8_IRQHandler(void)                	//����3�жϷ������
{
	if(USART_GetITStatus(UART8, USART_IT_RXNE) != RESET)  //�����ж�
	{//����һ���ֽ�
		USARTx_Receive(UART8,&UART8_Buffer,UART8->DR);
	} 
	if(USART_GetITStatus(UART8, USART_IT_TXE) != RESET)  //�����ж�
	{//����һ���ֽ�
		USARTx_Send(UART8,&UART8_Buffer);					
	}
}
//---
#endif
//-----------------------------------------------------------------------------------
//---
/*
	�������� void USARTx_StoreBufferData(USART_TypeDef* USARTx,USART_DataStructure *p,unsigned char b)
	������	 	����Ҫ��ӡ�����ݴ����ӡ������������FIFOԭ���ӡ����ÿ�ε��ú���Ҫ���������жϣ���������ֲʱ������
					�����һ����䣬�����������жϼ���
	�����	 	��
	���룺	 	USART_TypeDef* USARTx����Ҫ������Ĵ��ڶ˿ڣ�USART1��USART2��USART3��UART4��UART5��USART6��UART7��UART8��
					USART_DataStructure *p����Ӧ�˿ڵ����ݽṹ���ַ
					unsigned char b����Ҫ����ӡ������
	���أ�	 	��
*/
void USARTx_StoreBufferData(USART_TypeDef* USARTx,USART_DataStructure *p,unsigned char b)
{
	while(p->TxLen == TXBF_LEN-1) USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);	//������������ʱ����Ҫ�ȴ�
	p->TxBuffer[p->TxStorePoint++]=b;	//�洢��������
	if(p->TxStorePoint == TXBF_LEN) p->TxStorePoint=0;	//�洢ָ��ָ���ʱ������ָ��ָ����
	USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);		//������ϣ��رշ����ж�
	p->TxLen++;																					//�������ݸ���+1
	USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);				//���ж�,��ʼ����

}
//---
/*
	��������void 
*/
//---
/*
	�������� void USARTx_Send(USART_TypeDef* USARTx,USART_DataStructure *p)
	������	 	�ú���Ϊ���ʹ�ӡ��������һ���ֽ����ݣ��ɷ����жϳ����ã�ÿ����һ�η���һ���ֽ�����
					���ڵײ�������أ���˳�������ֲʱ����Ҫ�޸Ĵ˺���
	���룺		��Ҫ���͵Ķ�Ӧ����Ӳ���˿�
					USART_TypeDef* USARTx����Ҫ������Ĵ��ڶ˿ڣ�USART1��USART2��USART3��UART4��UART5��USART6��UART7��UART8��
					USART_DataStructure *p����Ӧ�˿ڵ����ݽṹ���ַ
*/
void USARTx_Send(USART_TypeDef* USARTx,USART_DataStructure *p)
{
	if(p->TxLen)
	{
	 USART_SendData(USARTx, p->TxBuffer[p->TxSendPoint++]);	//����һ���ֽ�����
	 if(p->TxSendPoint == TXBF_LEN) p->TxSendPoint=0;		//��ָ��ָ������޸�ָ��
		p->TxLen--;
		if(p->TxLen==0)
		{
		USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);		//������ϣ��رշ����ж�	
		USART_ClearFlag(USARTx,USART_FLAG_TXE);
		}
	}
	else
	{
		USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);		//������ϣ��رշ����ж�
		USART_ClearFlag(USARTx,USART_FLAG_TXE);
	}

}
//---
/*
	��������	void USARTx_Receive(USART_TypeDef* USARTx,USART_DataStructure *p,unsigned char b)
	������		���ڽ���һ�����ݴ���
					����֡����ʼ�� �豸��� ָ���� �߼��� ������+���ݡ� У���� ������
						:XXX|XXX|XXX|����������������
	���룺		u8 tmp�����յ�������,����1�����жϳ�����
					���սṹ˵�����ú����������Դ��ڵ�ÿһ�����ݣ�������5������û�н��յ�����
					����Ϊ�������(��b0��b2ͬʱֹͣ����),�����յ����ݸ�������RXBF_LEN-1�����
					�����(��b0��b1ͬʱֹͣ����)��������ɺ�����10������û�д������ݣ���ǰ
					��֡�������ϴ���������������
					��(��void USART3_Timer(void))
					USART_TypeDef* USARTx����Ҫ������Ĵ��ڶ˿ڣ�USART1��USART2��USART3��UART4��UART5��USART6��UART7��UART8��
					USART_DataStructure *p����Ӧ�˿ڵ����ݽṹ���ַ
					unsigned char b�����յ�������
	�����		��
 	���أ�		��
//��־λ���壺 b0(���ڽ���),b1(�������),b2(�������),b7=1(RTUģʽ����)
*/
void USARTx_Receive(USART_TypeDef* USARTx,USART_DataStructure *p,unsigned char b)
{
	p->RxBuffer[p->RxCount++] = b;  //��洢���洢һ�������ֽ�����
	p->Timer=0;						//������ն�ʱ������
	if((p->RxCount == RXBF_LEN-1) || (p->RxFlag & b1) == 1 )	//�ж��Ƿ�������
	{//������������������ǰ����յ�����һ����Ϊ��Ч���ݣ�������ݣ����½��գ�
		p->RxCount = 0;			//�������
		p->RxFlag &= ~b0;		//�����ڽ��ձ�־
 		p->RxFlag |= b1;		//�ý��������־λ
		USARTx->CR1 &= ~(1<<2);	//�رս��գ�Ӳ�����ƣ�������ֲ�޸Ĵ˴�
	}
	else
	{//���ڽ���
		p->RxFlag |= b0;	//�����ڽ��ձ�־λ
	}
}
//---
/*
	�������� void USARTx_Timer(USART_TypeDef* USARTx,USART_DataStructure *p)
	������	 	�ú���Ϊ����x��ʱ�������ڴ���x��ʱʹ�ã��������ճ�ʱ��ʱ������ʱ��ʱ
					1���붨ʱ�����øú���
	����:		1����������������2����û���յ����ݣ�����Ϊ��ǰһ֡���ݽ�����ɡ�
					2��������������ɺ���5��������Ȼû�д������ݣ�����֡���ݶ���
	���룺		USART_TypeDef* USARTx����Ӧ��Ӳ�����ڶ˿ںţ�USART1��USART2��USART3��UART4��UART5��USART6��UART7��UART8��
					USART_DataStructure *p����Ӧ����Ӧ�������ݽṹ��ָ��
//��־λ���壺 b0(���ڽ���),b1(�������),b2(�������),b7=1(RTUģʽ����)
*/
void USARTx_Timer(USART_TypeDef* USARTx,USART_DataStructure *p)
{
	if(p->RxFlag & b2)
	{//������Ϻ���Ҫ�����ݽ��д���������100msû�д������ݣ��������֡���ݣ������������գ����ݴ����ʱ
		p->Timer++;
		if(p->Timer == 100)
		{//���ճ�ʱ
			p->Timer=0;
			p->RxCount = 0;	//��ƽ�������
			p->RxFlag &= ~(b0 | b1 | b2);	//�����ڽ��ձ�־�����������־λ��������ϱ�־λ
			USARTx->CR1 |= 1<<2;	//��������
//			printf("USARTx���մ���ʱ\r\n");
		//	USARTx_Printf(USARTx,1);
		}
	}	
	else if(p->RxFlag & b1)
	{//��ѯ����ǰ���������10ms�����¿�������
		p->Timer++;
		if(p->Timer == 10)
		{
			p->Timer=0;
			p->RxCount = 0;//��ƽ�������
			p->RxFlag &= ~(b0 | b1 | b2);	//�����ڽ��ձ�־�����������־λ��������ϱ�־λ
			USARTx->CR1 |= 1<<2;	//��������
		//	USARTx_Printf(USARTx,2);
		}
	}
	else if(p->RxFlag & b0)
	{//���ڽ��ռ�ʱ
		p->Timer++;
		if(p->Timer == 4)//�������һ��bug�������ʵʱ��Ҫ��Ƚϸߵ�ʱ���޷�������ʱ�������ݣ���Ϊ����������ݺ�������Ҫ��2ms����ܵõ�����
		{//��2ms��û���յ���������Ϊ�������
			p->Timer=0;
			p->RxFlag &= ~b0;				//�����ڽ��ձ�־λ
			p->RxFlag |= b2;				//�ý�����ϱ�־λ
			USARTx->CR1 &= ~(1<<2);	//��ͣ����
		}
	}
}
//---
/*
	�������� void USARTx_DW(USART_TypeDef* USARTx,USART_DataStructure *p)
	������	 ����x�������ݴ���������������ѭ������
	���룺		USART_TypeDef* USARTx����Ӧ��Ӳ�����ڶ˿ںţ�USART1��USART2��USART3��UART4��UART5��USART6��UART7��UART8��
					USART_DataStructure *p����Ӧ����Ӧ�������ݽṹ��ָ��
	Э���ʽ������֡����ʼ�� �豸��� ָ���� �߼��� ������+���ݡ� У���� ������
	//��־λ���壺 b0(���ڽ���),b1(�������),b2(�������),b7=1(RTUģʽ����)
*/
void USARTx_DW(USART_TypeDef* USARTx,USART_DataStructure *p)
{
  if(p->RxFlag & b2)
	{//�������������
		u8 *sp=p->RxBuffer;//��ȡ�����׵�ַ
		u8 n=p->RxCount;//���յ������ݸ���
		p->RxFlag &= ~b2;//�������ɱ�־λ
		sp[p->RxCount] = 0;//֡ĩ������=0�����ַ�������
	//	if(USARTx == USART3)//��ĻͨѶ
	//	Screen_DealWihtReceiveData(sp,n);
	//	else if(USARTx == UART4)//����ͨѶ
	//	HC05_ReceiveData(sp,n);
	if(USARTx == USART1)   //USART1,printf.h,printf.c
	{
    USART_A1:	
		if(*sp==':')
		{
			if((*sp==':') && (n > 8))//
			{//ASCII: :XXX|XX|XXX|������(??????|???|???|??����CRC?)
				u8 i=7;
				u8 *p_s=sp;//
				sp=sp+7;
				for(i=7;i<n;i++)
				{
					if(*sp=='\r')//ASCII
					{
						if(*(sp+1)=='\n')
						{//
							ASCII_CommunicateReceive(p_s+5,i-5);
							i+=2;
							if(i<n)
							{
								sp=sp+2;
								n=n-i;
								goto USART_A1;
							}
							break;
						}
					}
					else sp++;
				}
			}
		}
	}		
		p->RxCount=0;//�������
		USARTx->CR1 |= 1<<2;//ʹ�ܴ��ڣ��������
	}
}
//---
/*
	��������void USARTx_Printf(USART_TypeDef* USARTx,u8 mode)
	������		�ú����������USART��������
*/
void USARTx_Printf(USART_TypeDef* USARTx,u8 mode)
{
	if(USARTx == USART1)
	{
		printf("USART1");
	}
	else if(USARTx == USART2)
	{
		printf("USART2");
	}
	else if(USARTx == USART3)
	{
		printf("USART3");
	}
	else if(USARTx == UART4)
	{
		printf("UART4");
	}
	else if(USARTx == UART5)
	{
		printf("UART5");
	}
	switch(mode)
	{
		case 1:
		{
			printf("���մ���ʱ\r\n");
			break;
		}
		case 2:
		{
			printf("�������\r\n");
			break;
		}
	}
}
//---
//---
/*
��������void USART2_SetEven(void)
˵����	������2����ΪżУ��
*/
void USART_SetEvenVerify(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;// 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
}
//---
/*
��������void USART2_SetEven(void)
˵����	������2����Ϊ��У��
*/
void USART_SetNoneVerify(void)
{
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;// 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
}
//---
/*
	��������void USART_ModeSet(USART_TypeDef* USARTx,u32 BaudRate,u8 Parity,u8 StopBits)
	���������ô���ͨѶģʽ
	���룺USART_TypeDef* USARTx��Ҫ���õĶ˿�
				u32 BaudRate������
				u8 ParityУ��λ
				u8 StopBitsֹͣλ
*/
void USART_ModeSet(USART_TypeDef* USARTx,u32 BaudRate,u8 Parity,u8 StopBits)
{
	USART_InitTypeDef USART_InitStructure;
	//����USART1���п�
	USART_Cmd(USARTx, DISABLE);
	USART_InitStructure.USART_BaudRate = BaudRate;
	if(StopBits == '1')//1λֹͣλ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
	else if(StopBits == '2')//2λֹͣλ
		USART_InitStructure.USART_StopBits = USART_StopBits_2;
	else//1.5λֹͣλ
		USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
	if((Parity == 'E') || (Parity == 'e'))
	{//żУ��
		USART_InitStructure.USART_Parity = USART_Parity_Even;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	}
	else if((Parity == 'O') || (Parity == 'o'))
	{//��У��
		USART_InitStructure.USART_Parity = USART_Parity_Odd;
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	}
	else
	{//��У��
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	}
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStructure);
	//ʹ��USART1
	USART_Cmd(USARTx, ENABLE);
}

//---









