
#include "APPAT.h"

APP_ModeStructure APP_Mode;

APP_ModeStructure *APP_GetData(void)
{
	return & APP_Mode;
}

//���øߵ�ƽ�������
void APP_Key_Init() 
	{

	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(HC06B_KEY_APP, ENABLE); //ʹ��GPIODʱ��

	GPIO_InitStructure.GPIO_Pin = HC06B_KEY_PIN;             //����26/Key�ܽ� ����ߵ�ƽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    //50MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;     //����
	GPIO_Init(HC06B_KEY_PORT, &GPIO_InitStructure);                //��ʼ��
	APP_BLUETOOTH_KEY=0;

}

void APP_CTPOWER_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(HC06B_CTPOWER_APP, ENABLE); //ʹ��GPIODʱ��

	GPIO_InitStructure.GPIO_Pin = HC06B_CTPOWER_PIN;             //����26/Key�ܽ� ����ߵ�ƽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    //50MHz
	GPIO_Init(HC06B_CTPOWER_PORT, &GPIO_InitStructure);                //��ʼ��
	APP_BLUETOOTH_POWER=0;
}

void APP_Statekey_Init(){
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(HC06B_LED_APP, ENABLE);

	GPIO_InitStructure.GPIO_Pin = HC06B_LED_PIN;              //��24/STA�ܽ� ���ߵ�ƽ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;           //��ͨ����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;         //�����޷���ȡ����ʱ�䣬��������
	GPIO_Init(HC06B_LED_PORT, &GPIO_InitStructure);                 //��ʼ��
                                          
}

void APP_Init(void)
{ 
	APP_Key_Init();
	APP_Statekey_Init();
	APP_CTPOWER_Init();
	APP_Mode.Mode=1; //��ʼATģʽ
	APP_Mode.H_time=10;
	APP_Mode.LevelCnt=0;
	APP_Mode.LevelTimeCnt=0;
	APP_Mode.Baudrate=4;
	APP_Power_On();

}

//��ѯAT״̬,����0 ͸����1 AT���ⲿ����
u8 APP_Mode_Check(void)
{
	return APP_Mode.Mode;
}
	
//����
void APP_AT_Set(void)
{  
   	Printf_GetData()->Printf_Mode=5;
		printf("AT");
	  Printf_GetData()->Printf_Mode=1;
}
//��������
void APP_Password_Set()
{   Printf_GetData()->Printf_Mode=5;
		printf("AT+PIN");
		printf("%s",&APP_Mode.Password[0]);
	  Printf_GetData()->Printf_Mode=1;
}
//��ģʽ����
void APP_Role_Set(void)
{   Printf_GetData()->Printf_Mode=5;
		printf("AT+ROLE=M");    //printf("AT+ROLE=M"); 
	  Printf_GetData()->Printf_Mode=1;
}

//����������
void APP_BaudRate_Set()
	{ 
	int baud_ratio;
	switch (APP_Mode.Baudrate) {
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
	 Printf_GetData()->Printf_Mode=5;
		printf("AT+BAUD");
		printf("%d", baud_ratio);
   Printf_GetData()->Printf_Mode=1;
}
	
//��Դ��λ����
void APP_Reset_Com(void)
{ 
	APP_Mode.RunMode = 1;        // 	
	APP_Mode.NextMode = 9;       //  
}
	//�������
void APP_AT_Com(void)   //AT
{  
	 APP_Mode.RunMode = 102;      //
	 APP_Mode.Error=0;
	 APP_Mode.OkFLG=0;  //���������ɱ�־
			
	 APP_Mode.H_time=10;   //����10MS  ��������Ƿ����AT״̬
	 APP_Mode.LevelCnt=0;
	 APP_Mode.LevelTimeCnt=0;
}
//�����������,����ӿ�
void APP_Password_Com(u8 * Password)
{ 
	strncpy((char *)&APP_Mode.Password[0],(char *)Password,4);
	APP_Mode.Password[4]=0;
	APP_Mode.RunMode = 3;   //
	APP_Mode.Error=0;
	APP_Mode.OkFLG=0;  //���������ɱ�־
	APP_Mode.H_time=10;   //����10MS  ��������Ƿ����AT״̬
	APP_Mode.LevelCnt=0;
	APP_Mode.LevelTimeCnt=0;
	
}


//����ģʽ�������,����ӿ�                  
void APP_Role_Com(void)
{ 
	APP_Mode.RunMode = 5;
	APP_Mode.Error=0;
	APP_Mode.OkFLG=0;  //���������ɱ�־
	APP_Mode.H_time=10;   //����10MS  ��������Ƿ����AT״̬
	APP_Mode.LevelCnt=0;
	APP_Mode.LevelTimeCnt=0;
	
}
//�������������     (��������ģʽ�����ò�����)
void APP_BautRate_Com(u32 baudrate)
{ 
	APP_Mode.Baudrate=baudrate;
	APP_Mode.RunMode = 7;   //
}


//��AT��Դ
void APP_Power_On(void)
{
	APP_BLUETOOTH_POWER=0;
}

//�ر�AT��Դ
void APP_Power_Off(void)
{
	APP_BLUETOOTH_POWER=1;
}

//AT ��ʱ��
void APP_Timer()
	{//AT 
	if(APP_Mode.Timer>0) APP_Mode.Timer--;
	
	//����Ϊ10MS���һ��AT ״̬ģ��
	if(APP_Mode.H_time>0)
	{
		APP_Mode.H_time--;
	}
	else  //
	{
		APP_Mode.H_time=10;   //����10MS
		
		if(APP_BLUETOOTH_LED)  //����ƽ
			APP_Mode.LevelCnt++;
		
		APP_Mode.LevelTimeCnt++;
		if(APP_Mode.LevelTimeCnt>=100)  //1��ʱ�䵽,1s�ж�һ��
		{
			//����ж�С���ߵ�ƽ
			if(APP_Mode.LevelCnt==100)
			{
				APP_Mode.Mode = 0;  //͸��ģʽ
				
			}
			else
			{
				APP_Mode.Mode = 1;//AT����ģʽ
				if(APP_Mode.LevelCnt==0)  //û����ģ��
				{
					APP_Mode.Mode =2;
					
				}
			}
			APP_Mode.LevelTimeCnt=0;
			APP_Mode.LevelCnt=0;
		}
	}
}

//������
void APP_main()
	{
	//�����߳�	
	switch(APP_Mode.RunMode)
		{
		case 0:{break;}
		case 1:{ 
			      APP_Power_Off();                 //�ص�0.5��
			      APP_Mode.Timer =500;             //1�볬ʱ����
			      APP_Mode.RunMode=103;
	
			      break;
		       }
		case 103:
		      { if(APP_Mode.Timer ==0)
						{
						  APP_Power_On();
						  APP_Mode.Timer =2000;             //1�볬ʱ����
			        APP_Mode.RunMode=101;
			        APP_Mode.Error=0;
	            APP_Mode.OkFLG=0;  //���������ɱ�־
			
			        APP_Mode.H_time=10;   //����10MS  ��������Ƿ����AT״̬
			        APP_Mode.LevelCnt=0;
		          APP_Mode.LevelTimeCnt=0;
						}
						break;
		      }
		case 101:	
		       {
						if(APP_Mode.Timer ==0)   //����2�����
						{
							 APP_Mode.RunMode= APP_Mode.NextMode;
						}
			     break;
		       }
					 
		case 102:  //AT ����
		       {			
			      if(APP_Mode.Mode==1)
						{
			      USART_COM5_Receve_com(0x1,0x02);  //����OK�����ַ�
			      APP_AT_Set();                      //����AT �����ַ�
			      APP_Mode.Timer = 1000;             //1�볬ʱ����
			      APP_Mode.RunMode = 2;
						}
						else
						{
							APP_Mode.RunMode = 0;
							APP_Mode.Error=1;
						}
			      break;
		       }
		case 2:{ //�������
		       	if(APP_Mode.Timer == 0)              //ʱ�䵽��ʱ
						{	//��ʱ
							APP_Mode.RunMode = 0;
							APP_Mode.Error=1;
						}
			      else 
						{
							if(USART_COM5_GetData()->ReceveOkFlg)  //������
							{
								APP_Mode.RunMode = 0;
								if(strncmp((char *)&USART_COM5_GetData()->USART_RX_BUF[0],"OK",2) == 0)//�ȽϽ��յ�ת���Ƿ�ΪOK   
								{
									APP_Mode.RunMode =14;  //ȥ��ʱ�ص�Դ
									APP_Mode.Timer =100;
									printf("AT���ֳɹ�");
								}
							}
						}
	
			      break;
		       }  
		case 3:{ //�����߳����       
			if(APP_Mode.Mode==1)
			{
			USART_COM5_Receve_com(0x1,0x08);  //ATģʽ�� ����OKsetPIN 8���ַ�
			APP_Password_Set();        //��������
			APP_Mode.Timer = 1000;     //1�붨ʱ
			APP_Mode.RunMode = 4;
			}
			else
			{
				APP_Mode.RunMode = 0;
				APP_Mode.Error=1;		
  
			}
			break;
		}
		case 4:{
			if(APP_Mode.Timer == 0)              //ʱ�䵽��ʱ
						{	//��ʱ
							APP_Mode.RunMode = 0;
							APP_Mode.Error=1;

						}
			      else 
						{
							if(USART_COM5_GetData()->ReceveOkFlg)  //������
							{
								APP_Mode.RunMode = 0;
								if(strncmp((char *)&USART_COM5_GetData()->USART_RX_BUF[0],"OKsetPIN",8) == 0)//�ȽϽ��յ�ת���Ƿ�ΪOK   
								{
									APP_Mode.RunMode =14;  //ȥ��ʱ�ص�Դ
									APP_Mode.Timer =1000;  //1S��ص�Դ

									printf("��������ɹ�");
								}
							}
						
						}
			     break;
		     }
		case 5:{ //������ģʽ�߳����       
			
			if(APP_Mode.Mode==1)
			{
			USART_COM5_Receve_com(0x1,0x09);  //����OK+ROLE:M 9���ַ�
			APP_Role_Set();        //������ģʽ
			APP_Mode.Timer = 1000;
			APP_Mode.RunMode = 6;
			}
			else
			{
				APP_Mode.RunMode = 0;
				APP_Mode.Error=1;

			}
			break;
		}
		case 6:{
			if(APP_Mode.Timer == 0)              //ʱ�䵽��ʱ
						{	//��ʱ
							APP_Mode.RunMode = 0;
							APP_Mode.Error=1;
						}
			      else 
						{
							if(USART_COM5_GetData()->ReceveOkFlg)  //������
							{
								APP_Mode.RunMode = 0;
								//if(strncmp((char *)&USART_COM5_GetData()->USART_RX_BUF[0],"OK+ROLE:M",9) == 0)//�ȽϽ��յ�ת���Ƿ�ΪOK   
									if(strncmp((char *)&USART_COM5_GetData()->USART_RX_BUF[0],"OK+ROLE:M",9) == 0)//�ȽϽ��յ�ת���Ƿ�ΪOK   
								{
							  	APP_Mode.Timer = 1500; //����ָ��֮����
			            APP_Mode.RunMode = 60;
							  //	printf("������ģʽ�ɹ�");
									printf("���ô�ģʽ�ɹ�");
								}
							}
						
						}
			break;
		}
		case 60://�������߳����
		{
			if(APP_Mode.Timer==0)
			{
					APP_Mode.Baudrate=9600;
	        APP_Mode.RunMode = 7;   //
			    //AT_BautRate_Com(9600);
			}
			break;
		}
		case 7:{                          
			     if(APP_Mode.Mode==1)
			      {
			      USART_COM5_Receve_com(0x1,0x06);  //����OK9600 6���ַ�
			      APP_BaudRate_Set();
			      APP_Mode.Timer = 1000;             //1�볬ʱ����
			      APP_Mode.RunMode = 8;
						}
					else
			     {
				    APP_Mode.RunMode = 0;
				    APP_Mode.Error=1;

			      }
				break;
		}
		case 8:{
			   	if(APP_Mode.Timer == 0)              //ʱ�䵽��ʱ
						{	//��ʱ
							APP_Mode.RunMode = 0;
							APP_Mode.Error=1;

						}
			      else 
						{
							if(USART_COM5_GetData()->ReceveOkFlg)  //������
							{
								APP_Mode.RunMode = 0;
								if(strncmp((char *)&USART_COM5_GetData()->USART_RX_BUF[0],"OK9600",6) == 0)//�ȽϽ��յ�ת���Ƿ�ΪOK   
								{
									APP_Mode.RunMode =14;  //ȥ��ʱ�ص�Դ
									APP_Mode.Timer =1000;  //1.5S��ص�Դ
									printf("����9600�����ʳɹ�");
								}
							}
						
						}
			break;
		}
		
		case 9:{
		       APP_BLUETOOTH_KEY=1;  //���øߵ�ƽ����ӻ���ַ����
			     APP_Mode.Timer = 200;   //��ʱ20MS
			     APP_Mode.RunMode = 10;
			     break;
		      }
		case 10:{
			if(APP_Mode.Timer == 0)              //ʱ�䵽��ʱ
						{	
							APP_Mode.RunMode = 0;
							APP_BLUETOOTH_KEY=0;         //�ָ��͵�ƽ
						  APP_Mode.OkFLG=1;
							APP_Mode.RunMode =0;         //�ϵ����
							printf("�������ɹ�");
						}
			break;
		}
   case 14:  //��ʱ�ص�Դ
		  {
	   if(APP_Mode.Timer == 0)            
			{
				APP_Power_Off();    //���¹ؿ���
				APP_Mode.OkFLG=1;   //���
				APP_Mode.Timer=500;
				APP_Mode.RunMode=15;
			}
			break;
		}
	case 15:  //��ʱ����Դ
		  {
	   if(APP_Mode.Timer == 0)            
			{
				APP_Power_On();
				APP_Mode.RunMode=0;
			}
			break;
		  }

	}
}
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void APP_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪAT.c�ļ���ASCII�����ļ�
						:000|12|001
*/
void APP_DebugASCII(u8 *p,u8 len)
{ //u8 i;
	//char buf[202];
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:  //������ģʽ
		{//
			if(APP_Mode.Mode==1)  //��ATģʽ�²�������
			APP_Role_Com();      
			else
			printf("����AT ģʽ");
			break;
		}
		case 2:  //��������
		{
			 if(APP_Mode.Mode==1)  //��ATģʽ�²�������
			 APP_Password_Com((u8 *)"1234");
			 else
			 printf("����AT ģʽ");
			break;
		}
		case 3:    //����
		{//
			 if(APP_Mode.Mode==1)  //��ATģʽ�²�������
			 APP_AT_Com();
			 else
			 printf("����AT ģʽ");
			break;
		}
		case 4://���ò�����
		{
			 if(APP_Mode.Mode==1)  //��ATģʽ�²�������
			APP_BautRate_Com(9600);
			 else
			 printf("����AT ģʽ");
			break;
		}
		case 5://�����ϵ�
		{
			if(APP_Mode.Mode==1)  //��ATģʽ�²�������
			APP_Reset_Com();
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
