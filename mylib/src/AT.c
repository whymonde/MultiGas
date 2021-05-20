//#include "sys.h"
#include "AT.h"
//#include "usart.h"
//#include "delay.h"
//#include "stdlib.h" 
//#include "string.h"
//#include "stdio.h"

At_ModeStructure At_Mode;


At_ModeStructure *AT_GetData(void)
{
	return & At_Mode;
}

//���øߵ�ƽ�������
void AT_Key_Init() 
	{

	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(HC06_KEY_APP, ENABLE); //ʹ��GPIODʱ��

	GPIO_InitStructure.GPIO_Pin = HC06_KEY_PIN;             //����26/Key�ܽ� ����ߵ�ƽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    //50MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;     //����
	GPIO_Init(HC06_KEY_PORT, &GPIO_InitStructure);                //��ʼ��
	AT_BLUETOOTH_KEY=0;

}

void AT_CTPOWER_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(HC06_CTPOWER_APP, ENABLE); //ʹ��GPIODʱ��

	GPIO_InitStructure.GPIO_Pin = HC06_CTPOWER_PIN;             //����26/Key�ܽ� ����ߵ�ƽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    //50MHz
	GPIO_Init(HC06_CTPOWER_PORT, &GPIO_InitStructure);                //��ʼ��
	AT_BLUETOOTH_POWER=0;
}

void AT_Statekey_Init(){
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(HC06_LED_APP, ENABLE);

	GPIO_InitStructure.GPIO_Pin = HC06_LED_PIN;              //��24/STA�ܽ� ���ߵ�ƽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //��ͨ����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;         //�����޷���ȡ����ʱ�䣬��������
	GPIO_Init(HC06_LED_PORT, &GPIO_InitStructure);                 //��ʼ��
                                          
}

void AT_Init(void)
{ 
	AT_Key_Init();
	AT_Statekey_Init();
	AT_CTPOWER_Init();
	At_Mode.Mode=1; //��ʼATģʽ
	At_Mode.H_time=10;
	At_Mode.LevelCnt=0;
	At_Mode.LevelTimeCnt=0;
	At_Mode.Baudrate=4;
	AT_Power_On();

}

//��ѯAT״̬,����0 ͸����1 AT���ⲿ����
u8 AT_Mode_Check(void)
{
	return At_Mode.Mode;
}
	
//����
void AT_AT_Set(void)
{  
   	Printf_GetData()->Printf_Mode=4;
		printf("AT");
	  Printf_GetData()->Printf_Mode=1;
}
//��������
void AT_Password_Set()
{   Printf_GetData()->Printf_Mode=4;
		printf("AT+PIN");
		printf("%s",&At_Mode.Password[0]);
	  Printf_GetData()->Printf_Mode=1;
}
//��ģʽ����
void AT_Role_Set(void)
{   Printf_GetData()->Printf_Mode=4;
		printf("AT+ROLE=M");//S
	  Printf_GetData()->Printf_Mode=1;
}

//����������
void AT_BaudRate_Set()
	{ 
	int baud_ratio;
	switch (At_Mode.Baudrate) {
		case 1200:{
			baud_ratio = 1;
			break;
		}
		case 2400:{
			baud_ratio = 2;
		  break;
		}
		case 4800:{
			baud_ratio = 3;
			break;
		}
		case 9600:{
			baud_ratio = 4;
			break;
		}
		case 19200:{
			baud_ratio = 5;
			break;
		}
		case 38400:{
			baud_ratio = 6;
			break;
		}
		case 57600:{
			baud_ratio = 7;
			break;
		}
		case 115200:{
			baud_ratio = 8;
			break;
		}
		case 230400:{
			baud_ratio = 9;
			break;
		}
		default:break;
	}
	 Printf_GetData()->Printf_Mode=4;
		printf("AT+BAUD");
		printf("%d", baud_ratio);
   Printf_GetData()->Printf_Mode=1;
}
	
//��Դ��λ����
void AT_Reset_Com(void)
{ 
	At_Mode.RunMode = 1;        // 	
	At_Mode.NextMode = 9;       //  
}
	//�������
void AT_AT_Com(void)   //AT
{  
	 At_Mode.RunMode = 102;      //
	 At_Mode.Error=0;
	 At_Mode.OkFLG=0;  //���������ɱ�־
			
	 At_Mode.H_time=10;   //����10MS  ��������Ƿ����AT״̬
	 At_Mode.LevelCnt=0;
	 At_Mode.LevelTimeCnt=0;
}
//�����������,����ӿ�
void AT_Password_Com(u8 * Password)
{ 
	strncpy((char *)&At_Mode.Password[0],(char *)Password,4);
	At_Mode.Password[4]=0;
	At_Mode.RunMode = 3;   //
	At_Mode.Error=0;
	At_Mode.OkFLG=0;  //���������ɱ�־
	At_Mode.H_time=10;   //����10MS  ��������Ƿ����AT״̬
	At_Mode.LevelCnt=0;
	At_Mode.LevelTimeCnt=0;
	
}


//����ģʽ�������,����ӿ�                  
void AT_Role_Com(void)
{ 
	At_Mode.RunMode = 5;
	At_Mode.Error=0;
	At_Mode.OkFLG=0;  //���������ɱ�־
	At_Mode.H_time=10;   //����10MS  ��������Ƿ����AT״̬
	At_Mode.LevelCnt=0;
	At_Mode.LevelTimeCnt=0;
	
}
//�������������     (��������ģʽ�����ò�����)
void AT_BautRate_Com(u32 baudrate)
{ 
	At_Mode.Baudrate=baudrate;
	At_Mode.RunMode = 7;   //
}


//��AT��Դ
void AT_Power_On(void)
{
	AT_BLUETOOTH_POWER=0;
}

//�ر�AT��Դ
void AT_Power_Off(void)
{
	AT_BLUETOOTH_POWER=1;
}

//AT ��ʱ��
void AT_Timer()
	{//AT 
	if(At_Mode.Timer>0) At_Mode.Timer--;
	if(At_Mode.EnableBTtimer&&At_Mode.sBTtimer>0) At_Mode.sBTtimer--;
	
	//����Ϊ10MS���һ��AT ״̬ģ��
	if(At_Mode.H_time>0)
	{
		At_Mode.H_time--;
	}
	else  //
	{
		At_Mode.H_time=10;   //����10MS
		
		if(AT_BLUETOOTH_LED)  //����ƽ
			At_Mode.LevelCnt++;
		
		At_Mode.LevelTimeCnt++;
		if(At_Mode.LevelTimeCnt>=100)  //1��ʱ�䵽,1s�ж�һ��
		{
			//����ж�С���ߵ�ƽ
			if(At_Mode.LevelCnt==100)
			{
				At_Mode.Mode = 0;  //͸��ģʽ
				
			}
			else
			{
				At_Mode.Mode = 1;//AT����ģʽ
				if(At_Mode.LevelCnt==0)  //û����ģ��
				{
					At_Mode.Mode =2;
					
				}
			}
			At_Mode.LevelTimeCnt=0;
			At_Mode.LevelCnt=0;
		}
	}
}

//������
void AT_main()
	{
	//�����߳�	
	switch(At_Mode.RunMode)
		{
		case 0:{break;}
		case 1:{ 
			      AT_Power_Off();                 //�ص�0.5��
			      At_Mode.Timer =500;             //1�볬ʱ����
			      At_Mode.RunMode=103;
	
			      break;
		       }
		case 103:
		      { if(At_Mode.Timer ==0)
						{
						  AT_Power_On();
						  At_Mode.Timer =2000;             //1�볬ʱ����
			        At_Mode.RunMode=101;
			        At_Mode.Error=0;
	            At_Mode.OkFLG=0;  //���������ɱ�־
			
			        At_Mode.H_time=10;   //����10MS  ��������Ƿ����AT״̬
			        At_Mode.LevelCnt=0;
		          At_Mode.LevelTimeCnt=0;
						}
						break;
		      }
		case 101:	
		       {
						if(At_Mode.Timer ==0)   //����2�����
						{
							 At_Mode.RunMode= At_Mode.NextMode;
						}
			     break;
		       }
					 
		case 102:  //AT ����
		       {			
			      if(At_Mode.Mode==1)
						{
			      USART_COM4_Receve_com(0x1,0x02);  //����OK�����ַ�
			      AT_AT_Set();                      //����AT �����ַ�
			      At_Mode.Timer = 1000;             //1�볬ʱ����
			      At_Mode.RunMode = 2;
						}
						else
						{
							At_Mode.RunMode = 0;
							At_Mode.Error=1;
						}
			      break;
		       }
		case 2:{ //�������
		       	if(At_Mode.Timer == 0)              //ʱ�䵽��ʱ
						{	//��ʱ
							At_Mode.RunMode = 0;
							At_Mode.Error=1;
						}
			      else 
						{
							if(USART_COM4_GetData()->ReceveOkFlg)  //������
							{
								At_Mode.RunMode = 0;
								if(strncmp((char *)&USART_COM4_GetData()->USART_RX_BUF[0],"OK",2) == 0)//�ȽϽ��յ�ת���Ƿ�ΪOK   
								{
									At_Mode.RunMode =14;  //ȥ��ʱ�ص�Դ
									At_Mode.Timer =100;
									printf("AT���ֳɹ�");
								}
							}
						}
	
			      break;
		       }  
		case 3:{ //�����߳����       
			if(At_Mode.Mode==1)
			{
			USART_COM4_Receve_com(0x1,0x08);  //ATģʽ�� ����OKsetPIN 8���ַ�
			AT_Password_Set();        //��������
			At_Mode.Timer = 1000;     //1�붨ʱ
			At_Mode.RunMode = 4;
			}
			else
			{
				At_Mode.RunMode = 0;
				At_Mode.Error=1;		
  
			}
			break;
		}
		case 4:{
			if(At_Mode.Timer == 0)              //ʱ�䵽��ʱ
						{	//��ʱ
							At_Mode.RunMode = 0;
							At_Mode.Error=1;

						}
			      else 
						{
							if(USART_COM4_GetData()->ReceveOkFlg)  //������
							{
								At_Mode.RunMode = 0;
								if(strncmp((char *)&USART_COM4_GetData()->USART_RX_BUF[0],"OKsetPIN",8) == 0)//�ȽϽ��յ�ת���Ƿ�ΪOK   
								{
									At_Mode.RunMode =15;  //ȥ��ʱ�ص�Դ   14
									At_Mode.Timer =2000;  //1S��ص�Դ     1000

									printf("��������ɹ�");
								}
							}
						
						}
			     break;
		     }
		case 5:{ //������ģʽ�߳����       
			
			if(At_Mode.Mode==1)
			{
			USART_COM4_Receve_com(0x1,0x09);  //����OK+ROLE:M 9���ַ�
			AT_Role_Set();        //������ģʽ
			At_Mode.Timer = 1000;
			At_Mode.RunMode = 6;
			}
			else
			{
				At_Mode.RunMode = 0;
				At_Mode.Error=1;

			}
			break;
		}
		case 6:{
			if(At_Mode.Timer == 0)              //ʱ�䵽��ʱ
						{	//��ʱ
							At_Mode.RunMode = 0;
							At_Mode.Error=1;
						}
			      else 
						{
							if(USART_COM4_GetData()->ReceveOkFlg)  //������
							{
								At_Mode.RunMode = 0;
								if(strncmp((char *)&USART_COM4_GetData()->USART_RX_BUF[0],"OK+ROLE:M",9) == 0)//�ȽϽ��յ�ת���Ƿ�ΪOK   //S
								{
							  	At_Mode.Timer = 1500; //����ָ��֮����
			            At_Mode.RunMode = 60;
									printf("������ģʽ�ɹ�");
								}
							}
						
						}
			break;
		}
		case 60://�������߳����
		{
			if(At_Mode.Timer==0)
			{
					At_Mode.Baudrate=9600;
	        At_Mode.RunMode = 7;   //
			    //AT_BautRate_Com(9600);
			}
			break;
		}
		case 7:{                          
			     if(At_Mode.Mode==1)
			      {
			      USART_COM4_Receve_com(0x1,0x06);  //����OK9600 6���ַ�
			      AT_BaudRate_Set();
			      At_Mode.Timer = 1000;             //1�볬ʱ����
			      At_Mode.RunMode = 8;
						}
					else
			     {
				    At_Mode.RunMode = 0;
				    At_Mode.Error=1;

			      }
				break;
		}
		case 8:{
			   	if(At_Mode.Timer == 0)              //ʱ�䵽��ʱ
						{	//��ʱ
							At_Mode.RunMode = 0;
							At_Mode.Error=1;

						}
			      else 
						{
							if(USART_COM4_GetData()->ReceveOkFlg)  //������
							{
								At_Mode.RunMode = 0;
								if(strncmp((char *)&USART_COM4_GetData()->USART_RX_BUF[0],"OK9600",6) == 0)//�ȽϽ��յ�ת���Ƿ�ΪOK   
								{
									At_Mode.RunMode =14;  //ȥ��ʱ�ص�Դ14
									At_Mode.Timer =1500;  //1.5S��ص�Դ1000
									printf("����9600�����ʳɹ�");
								}
							}
						
						}
			break;
		}
		
		case 9:{
		       AT_BLUETOOTH_KEY=1;  //���øߵ�ƽ����ӻ���ַ����
			     At_Mode.Timer = 200;   //��ʱ20MS
			     At_Mode.RunMode = 10;
			     break;
		      }
		case 10:{
			if(At_Mode.Timer == 0)              //ʱ�䵽��ʱ
						{	
							At_Mode.RunMode = 0;
							AT_BLUETOOTH_KEY=0;         //�ָ��͵�ƽ
						  At_Mode.OkFLG=1;
							At_Mode.RunMode =0;         //�ϵ����
							printf("�������ɹ�");
						}
			break;
		}
   case 14:  //��ʱ�ص�Դ
		  {
	   if(At_Mode.Timer == 0)            
			{
				AT_Power_Off();    //���¹ؿ���
//				At_Mode.OkFLG=1;   //���
				At_Mode.Timer=500;
				At_Mode.RunMode=15;
			}
			break;
		}
	case 15:  //��ʱ����Դ
		  {
	   if(At_Mode.Timer == 0)            
			{
				AT_Power_On();
				At_Mode.RunMode=0;
				At_Mode.OkFLG=1;
			}
			break;
		  }

	}
}




//-------------------------���Ժ���----------------------------
//---
/*
	�������� void AT_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪAT.c�ļ���ASCII�����ļ�
						:000|11|001
*/
void AT_DebugASCII(u8 *p,u8 len)
{ //u8 i;
	//char buf[202];
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:  //������ģʽ
		{//
			if(At_Mode.Mode==1)  //��ATģʽ�²�������
			AT_Role_Com();      
			else
			printf("����AT ģʽ");
			break;
		}
		case 2:  //��������
		{
			 if(At_Mode.Mode==1)  //��ATģʽ�²�������
			 AT_Password_Com((u8 *)"1234");
			 else
			 printf("����AT ģʽ");
			break;
		}
		case 3:    //����
		{//
			 if(At_Mode.Mode==1)  //��ATģʽ�²�������
			 AT_AT_Com();
			 else
			 printf("����AT ģʽ");
			break;
		}
		case 4://���ò�����
		{
			 if(At_Mode.Mode==1)  //��ATģʽ�²�������
			AT_BautRate_Com(9600);
			 else
			 printf("����AT ģʽ");
			break;
		}
		case 5://�����ϵ�
		{
			if(At_Mode.Mode==1)  //��ATģʽ�²�������
			AT_Reset_Com();
			else
			 printf("����AT ģʽ");
			break;
		}
		default:
		{
		break;
		}
	}
}
