#include "Screen.h"
#include "Button.h"
#include "ScreenDwFunction.h"
#include "CalculationFormula.h"
#include "ScreenButton.h"
#include "ScreenDisCalculate.h"
#include "Screenlogic.h"
#define BACK_BUTTON Screen_PastePicture(0x0800,19,(718<<16)+370,(787<<16)+440,(718<<16)+370);
static float FileValue=00000001;//�ļ��� �����ļ���ѯ

extern float gMinuteData[60][8];	//��������,������ʾ�ݴ�
extern u8 gSystemTime[60][10];		//��������ʱ��


//---
/*
	��������void Screen_programUpdata(Screen_DataStructure *p)
	������	�����������洦����
*/
void Screen_programUpdata(Screen_DataStructure *p)
{
   if(p->Page==2) //
	 {
	    ScreenFirstOpButton(p,2,1);//��ȡ����
		 if(p->Button==0x0002&p->Key.Mode==0) //�񰴼�
		 {
			 p->Key.Opt=2;
		     p->RunMode=11; 	//�˳���������
		 }
		 else if(p->Button==0x0001&&p->Key.Mode==0) //�ǰ���
		 {
			 p->Key.Opt=1;
			 Screen_ResetDataBuffer();
			 sprintf((char*)p->Buffer,"Star programUpdata");//����汾
			 Screen_RefreshDataMulti(0x0100,p->Buffer,strlen((const char*)p->Buffer));
			 p->Key.Mode=1; 
		 }
		 if(p->Key.LastOpt!=p->Key.Opt) //ѡ�����
		 {
		   
			  switch(p->Key.Opt)
				{
					case 01: Screen_PastePicture(0x0800,0x0003,0x00D900f5,0x01480137,0x00D900f5);break;
					case 02: Screen_PastePicture(0x0800,0x0003,0x01e000f5,0x02530137,0x01e000f5);break;
					default: p->Key.Opt=2;Screen_PastePicture(0x0800,0x0003,0x01e000f5,0x02530137,0x01e000f5); break;
				}
				p->Key.LastOpt =p->Key.Opt;
		 }
		 if(p->Button!=0xffff) //����״̬��λ
		 {
		    p->Button=0xffff;
		 }
		 if(p->Key.Mode==1)//��ʼ����
		 {
		    //���������������
		 }
	 }
}
//---

/*20200529
	��������void Screen_programUpdata(Screen_DataStructure *p)
	������	�����������洦����
*/
void Screen_AutoCheck(Screen_DataStructure *p)
{     
	    FourBytes four; 
	    Flashisp_DataStructure *fP=Flashisp_GetData();
	    if(p->Timer>0) return;
	    Screen_PastePicture(0x0240,0x0005,0x001d01c2,(((p->Key.Opt)*93+80)<<16)+0x030c,0x001d01c2);
      switch(p->Key.Opt)
			{
				case 1: 
				        {//����汾
								Screen_ResetDataBuffer();
								GetU16dataFormFlash2Word(&four,FLASHIS_AdrCJWHRJBB,fP);
								sprintf((char*)p->Buffer,"V%.2f        ",four.FloatData);//����汾
								Screen_RefreshDataMulti(0x0400,p->Buffer,12*2);
								//�������
								Screen_ResetDataBuffer();
								GetU16dataFormFlash2Word(&four,FLASHIS_AdrCJWHYQBH,fP);
					     	    sprintf((char*)p->Buffer,"%u           ",four.U32Data);//�������
								Screen_RefreshDataMulti(0x0410,p->Buffer,12*2);
								 //��������ʱ��
								Screen_ResetDataBuffer();
								GetU16dataFormFlash2Word(&four,FLASHIS_AdrYQYXSJ,fP);
								Screen_ResetDataBuffer();
								sprintf((char*)p->Buffer,"��������:%uh%umin          ",four.U32Data/3600,four.U32Data%3600/60);//�������
								Screen_RefreshDataMulti(0x0500,p->Buffer,12*2);
				         //��������ʱ��
								GetU16dataFormFlash2Word(&four,FLASHIS_AdrCBYXSJ,fP);
								sprintf((char*)p->Buffer,"��������:%uh%umin",four.U32Data/3600,four.U32Data%3600/60);//�������
								Screen_RefreshDataMulti(0x0520,p->Buffer,12*2);
							}				
							{		//���崫������װʱ��
							  	   p->LastChoice=fP->Data[FLASHIS_AdrWHJZ_QTCGQXZ];
										for(int i=0;i<7;i++)
										{
										 UpdataSensorFixDate(&four,FLASHIS_AdrO2AZSJ+i*2,fP,p,i);//O2
										}		
										
							}
							//�������ʾ����, modify by alfred, 2020.06.16
                             for(u8 i = 0; i<7; i++)
                             {
                             Screen_RefreshDataMulti(0x0120+i*0x20,(u8 *)"                              ",12*2);
                             }
							p->Key.Opt=2;
							p->Timer=100;
					      break;
				case 2:   //���Ӱ�ͨ��
				     {        
					     if(Slave_GetData()->Comm.ErrorCnt == 0)//�Լ췵�� �ɹ�1  ʧ��0
				        {
                               sprintf((char*)p->Buffer,"�Լ���Ŀ%d������ͨ������",p->Key.Opt-1); 
						}else
						{
								  sprintf((char*)p->Buffer,"�Լ���Ŀ%d������ͨ���쳣",p->Key.Opt-1); 
						}
								Screen_RefreshDataMulti(0x0120+(p->Key.Opt-2)*0x20,p->Buffer,12*2);
								p->Key.Opt=3;
							  p->Timer=1000;
				
							  break;
					}
				case 3:  //���SD�� 
					   {        
					     if(SD_GetData()->flag != 0)//�Լ췵�� �ɹ�1  ʧ��0
				        {
                                sprintf((char*)p->Buffer,"�Լ���Ŀ%d��SD������",p->Key.Opt-1); 
						}else
						{
								  sprintf((char*)p->Buffer,"�Լ���Ŀ%d��SD���쳣",p->Key.Opt-1); 
						}
								Screen_RefreshDataMulti(0x0120+(p->Key.Opt-2)*0x20,p->Buffer,12*2);
								p->Key.Opt=4;
							    p->Timer=1000;
				
							  break;
							}
				case 4:  //��ӡ����  
					{        
					     if(AT_GetData()->Mode!=2)//�Լ췵�� �ɹ�1  ʧ��0
				        {
                              sprintf((char*)p->Buffer,"�Լ���Ŀ%d����ӡ��������",p->Key.Opt-1); 
						}else
						{
								sprintf((char*)p->Buffer,"�Լ���Ŀ%d����ӡ�����쳣",p->Key.Opt-1); 
						}
								Screen_RefreshDataMulti(0x0120+(p->Key.Opt-2)*0x20,p->Buffer,12*2);
								p->Key.Opt=9;  //5
							    p->Timer=1000;
				         //��������״̬��/�ر�����״̬
						 if(fP->Data[FLASHIS_AdrXTSZBOX]&0x04)
						 {
							 //�򿪴�ӡ����
							  AT_Power_On();
						 }
						 else
						 {
							  //�رմ�ӡ����
							  AT_Power_Off();
						 }
							  break;
							}
					     
				case 5:  //APP���� 
					{        
					    if(AT_GetData()->Mode!=2)//�Լ췵�� �ɹ�1  ʧ��0 AT_GetData()->Mode!=2
				        {
                               sprintf((char*)p->Buffer,"�Լ���Ŀ%d��APP��������",p->Key.Opt-1); 
						}else
						{
								  sprintf((char*)p->Buffer,"�Լ���Ŀ%d��APP�����쳣",p->Key.Opt-1); 
						}
								Screen_RefreshDataMulti(0x0120+(p->Key.Opt-2)*0x20,p->Buffer,12*2);
								p->Key.Opt=6;
							    p->Timer=1000;
				
							  break;
							}
				case 6:  //4Gģ��
					{        
					     if(1)//�Լ췵�� �ɹ�1  ʧ��0
				        {
                               sprintf((char*)p->Buffer,(const char*)("�Լ���Ŀ%d��4G����"),p->Key.Opt-1); 
						}else
						{
								  sprintf((char*)p->Buffer,"�Լ���Ŀ%d��4G�쳣",p->Key.Opt-1); 
						}
								Screen_RefreshDataMulti(0x0120+(p->Key.Opt-2)*0x20,p->Buffer,12*2);
								p->Key.Opt=7;
							    p->Timer=1000;
				
							  break;
							}
				case 7:   
					{        
					     if(0)//�Լ췵�� �ɹ�1  ʧ��0
				        {
                                 sprintf((char*)p->Buffer,"�Լ���Ŀ%d������",p->Key.Opt-1); 
						}else
						{
								  sprintf((char*)p->Buffer,"�Լ���Ŀ%d���쳣",p->Key.Opt-1); 
						}
								Screen_RefreshDataMulti(0x0120+(p->Key.Opt-2)*0x20,p->Buffer,12*2);
								p->Key.Opt=8;
							    p->Timer=1000;
				
							  break;
							}
					case 8:   
						{        
					     if(0)//�Լ췵�� �ɹ�1  ʧ��0
				        {
                                  sprintf((char*)p->Buffer,"�Լ���Ŀ%d������",p->Key.Opt-1); 
						}else
						{
								  sprintf((char*)p->Buffer,"�Լ���Ŀ%d���쳣",p->Key.Opt-1); 
						}
								Screen_RefreshDataMulti(0x0120+(p->Key.Opt-2)*0x20,p->Buffer,12*2);
								p->Key.Opt=9;
							    p->Timer=1000;
				
							  break;
							}
				case 9:   	
							{ //������
								p->RunMode=13;//�Լ���ɺ����������
								p->Timer=1000;
								p->Key.OldOpt=0;
								p->RunModeStat=1;
									break;
							}
				default :break;
			}
	    
}

//---
/* date 2019-12-04
	��������void Screen_programUpdata(Screen_DataStructure *p)
	������	���������洦����
*/
void Screen_MainLayer(Screen_DataStructure *p)
{
 
	//������⴦��
	Buttom_MainPage();
		 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				if(p->RunModeStat==1)
				{
					 Screen_ShowPage(8);
					 p->RunModeStat=2;
					 p->Key.LastOpt=0;
				}
			 }
			 break;
		 }
    		//�����߼�����
	p->OldMode=p->RunMode;
	if(p->Button>0&&p->Button<=8)
	{
		p->RunMode=p->Button*0x1000;
	}
	if(p->Button==0x0002) p->Key.Opt=Flashisp_GetData()->Data[FLASHIS_AdrYDBDLX];
	
	if(p->Button==0x0005) //��������,������ڲ����У�ֱ�ӽ����������
	{ 
		
		if((Flux_GetData()->ChannelC.RunMode==2)||(Flux_GetData()->ChannelC.RunMode==3))//������ͣ�л������
		{ //��������ҳ��
		  p->RunMode=0x5200;
		}
		else if((Flux_GetData()->ChannelC.RunMode==4)&&(Flux_GetData()->ChannelC.Sampling.EndFlg==0))//������ϴҳ��
		{
			ScreenBackLastPag(p,0X5220);
		}
		else if(Flux_GetData()->ChannelC.Sampling.EndFlg==1)
		{Flux_GetData()->ChannelC.Sampling.EndFlg=0;
			//�����������
			p->Key.Opt=5;
			p->RunModeStat=1;		 
			p->RunMode=0x5210;//��ʾ�Ƿ����
		}
		else
		{
			p->YC_Pause=0;
		}

	}
	
	if(p->Button==0x0006) //�̳�������������ڲ����У�ֱ�ӽ����������
	{ 
		if((Flux_GetData()->ChannelB.RunMode==2)||(Flux_GetData()->ChannelB.RunMode==3))//������ͣ��//������
		{
			p->RunMode=0x6200; //��ʼ����
		    p->Function=0;
		    p->Key.Opt=7;	
		}
	
		else if(Flux_GetData()->ChannelB.RunMode==4)//��������,�����ŵ���ҳ��
		{
			 p->RunMode=0x6220;
			 p->YC_Stat=0;
			 ScreenChangeDataInit(p);//�������
		}
		else if(Flux_GetData()->ChannelB.Sampling.FdxFlg)//���������,ȥ������
		{       Flux_GetData()->ChannelB.Sampling.FdxFlg=0;
				p->Key.Opt=2;
				p->RunMode=0x6210;
		}
		else
		{
			p->YC_Pause=0;
		}

	}
  ScreenChangeDataInit(p);
  p->Button=0xffff;
	if(p->RunMode!=52) return;
	//���洦��	 
	if(p->Key.LastOpt!=p->Key.Opt) 
	{
		p->Key.LastOpt=p->Key.Opt;
		switch(p->Key.Opt)
		 {
			case 1:  //����ѡ��
				{//����
					Screen_PastePicture(0x0800,0x0009,0x04B006B,0x0D600F9,0x04B006B);
					break;
				}
				case 2:	
				{//�̵�����
					Screen_PastePicture(0x0800,0x0009,0x0F60069,0x18100F7,0x0F60069);
					break;
				}
				case 3:	
				{//����Ԥ��
					Screen_PastePicture(0x0800,0x0009,0x1A2006B,0x022D00F9,0x1A2006B);
					break;
				}
				case 4:
				{//�⺬ʪ��
					Screen_PastePicture(0x0800,0x0009,0x24D0068,0x2D800F6,0x24D0068);
					break;
				}
				case 5:	
				{//��������
					Screen_PastePicture(0x0800,0x0009,0x0490106,0x0D40194,0x0490106);
					break;
				}
				case 6:	
				{//�̳�����
					Screen_PastePicture(0x0800,0x0009,0x0F40107,0x017F0195,0x0F40107);
					break;
				}
					case 7:	
				{//�ļ���ѯ
					Screen_PastePicture(0x0800,0x0009,0x1A10107,0x022C0195,0x1A10107);
					break;
				}
					case 8:	
				{//У׼ά��
					Screen_PastePicture(0x0800,0x0009,0x24C0108,0x02D70196,0x24C0108);
					break;
				}
				default: 
				{
					p->Key.Opt = 1;
					Screen_PastePicture(0x0800,0x0009,0x0054006e,0x00be00d3,0x0054006e);
					break;
				}
		 }
 }
}
//---
/* date 2019-12-06
	��������void Screen_PowerOffPag(Screen_DataStructure *p)
	������	���������洦����
*/
void Screen_PowerOffPag(Screen_DataStructure *p)
{
    FourBytes four;
	   
	  //������⴦��
    	Buttom_PowerOffPage();
	   	if(p->RunMode!=31) return;  //����������Ǳ�ҳ�����˳�
	 //��ͼ��ʾ
	
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		    p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
				case 0x01: Screen_PastePicture(0x0800,7,(450<<16)+372,(565<<16)+434,(450<<16)+372);break;
				case 0x02: Screen_PastePicture(0x0800,7,(573<<16)+370,(790<<16)+435,(573<<16)+370);break;
				default:
				{   Screen_PastePicture(0x0800,7,(450<<16)+372,(565<<16)+434,(450<<16)+372);
					p->Key.Opt=1;
					break;
				}
			}
		}

	//�ı���̬��̬��ʾ
	  switch(p->RunModeStat)
		{
			case 1:
			 { //��ʾ��̬���� ����

				Screen_ShowPage(6);
				Flux_GetData()->ChannelB.RunMode=0;
				Flux_GetData()->ChannelB.Sampling.Cnt=Flashisp_GetData()->Data[FLASHIS_AdrCJCNT]; //�����ڲ����ĵڼ���������

				Flux_GetData()->ChannelB.Sampling.DinSiTime=(u32)Flashisp_GetData()->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]; //�������ʱ��
				Flux_GetData()->ChannelB.Sampling.Count=Flashisp_GetData()->Data[FLASHIS_AdrYDBD_CYDS]; //�ܲ�����
				Flux_GetData()->ChannelB.Sampling.SamTime=(Flashisp_GetData()->Data[FLASHIS_AdrYCCJ_CYSE_DDCS])*Flux_GetData()->ChannelB.Sampling.Count;//��ʱ��
	            four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrCJGKTJ);	//�������
				four.FloatData =GetFlaotdataFormFlash2Word(FLASHIS_AdrCJWJH);                     //�ļ���
				Flux_GetData()->ChannelB.SaveFileNumber=four.FloatData;
				 
				Flux_GetData()->ChannelB.Sampling.YiciFlg=0;
				Flux_GetData()->ChannelB.Sampling.EndFlg=0;
				Flux_GetData()->ChannelB.Sampling.FdxFlg=0; //��������ɱ�־
				Flux_GetData()->ChannelB.FluxRK.Sum = 0;
		        Flux_GetData()->ChannelB.FluxRK.Cnt = 0;
		        Flux_GetData()->ChannelB.FluxKD.Sum = 0;
		        Flux_GetData()->ChannelB.FluxKD.Cnt = 0;
		        Flux_GetData()->ChannelB.FluxCB.Sum = 0;
		        Flux_GetData()->ChannelB.FluxCB.Cnt = 0;

				four.U32Data=GetU32dataFormFlash2Word(FLASHIS_AdrCJKSSK); //��ʼ�ɼ�ʱ��
				struct tm *myTm=localtime(&four.U32Data);
				sprintf((char*)p->Buffer,"%u-%02u-%02u %02u:%02u:%02u",myTm->tm_year+SCREEN_YearBase,myTm->tm_mon,myTm->tm_mday,myTm->tm_hour,myTm->tm_min,myTm->tm_sec);//����汾
				Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);
				 
				four.U32Data=GetU32dataFormFlash2Word(FLASHIS_AdrCJLJSH);	//�ۼƲɼ�ʱ��
				sprintf((char*)p->Buffer,"%dH %dMin %dS",four.U32Data/3600,four.U32Data%3600/60,(four.U32Data%3600)%60);//
				Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);
				  
				Flux_GetData()->ChannelB.Sampling.SecTimer=four.U32Data;  //��ȡ����ʱ��
				Flux_GetData()->ChannelB.Sampling.IntTime=Flux_GetData()->ChannelB.Sampling.SecTimer-Flux_GetData()->ChannelB.Sampling.DinSiTime*(Flux_GetData()->ChannelB.Sampling.Cnt-1);  //���㿪ʼ
				 	
				four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrCJGKTJ);	//�������
				sprintf((char*)p->Buffer,"%10.2fL",four.FloatData);
				Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);
				Flux_GetData()->ChannelB.Sampling.SumVolume=four.FloatData;  //�ۼ����
				
				four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrCJBQTJ);	//������
				sprintf((char*)p->Buffer,"%10.2fL",four.FloatData);
				Screen_RefreshDataMulti(0x0160,p->Buffer,15*2);
				Flux_GetData()->ChannelB.Sampling.RefVolume=four.FloatData;  //�ۼ����
				 
				four.U32Data=GetU32dataFormFlash2Word(FLASHIS_AdrDDTime); //����ʱ��
				myTm=localtime(&four.U32Data);
				sprintf((char*)p->Buffer,"����ʱ��:%04u-%02u-%02u %02u:%02u:%02u",myTm->tm_year+SCREEN_YearBase,myTm->tm_mon,myTm->tm_mday,myTm->tm_hour,myTm->tm_min,myTm->tm_sec);//����汾
				Dis_Tips(p,p->Buffer,0x0180);
				p->RunModeStat=2;
				p->OldMode=p->RunMode;
				break;
			 }
			case 2:
				break;

		}
		
		
  
}
/* date 2019-12-12
	��������void Screen_SystemSetPag(Screen_DataStructure *p)
	������	ϵͳ���ý����߼�����
	�޸ļ�¼��1. �������޸��˱�ҳ��ʾ���ݣ���Ӧ�޸��˴��룬���Ӻͼ���һЩ��ʾ, modify by alfred, 2020.06.12.
*/
void Screen_SystemSetPag(Screen_DataStructure *p)
{
	FourBytes four;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//��������
	Button_SysSetPage();
	if(p->RunMode!=0x1000) return;
	//���ݸ���
	switch(p->RunModeStat)
	{
		case 0x01: //��̬������ʾ			
		{
			Screen_ShowPage(10); 	
			// Screen_PastePicture(0x0800,0x000B,(54<<16)+66,(433<<16)+111,(54<<16)+66); //����
			for(int i=0;i<10;i++)
			{
				ScreenSetJitu(p,i);
			}
			//����
			sprintf((char*)p->Buffer,"%02d-%02d-%02d",p->Clock.Clock.tm_year+SCREEN_YearBase,p->Clock.Clock.tm_mon,p->Clock.Clock.tm_mday);
			Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);//��
			//ʱ��
			sprintf((char*)p->Buffer,"%02d:%02d:%02d",p->Clock.Clock.tm_hour,p->Clock.Clock.tm_min,p->Clock.Clock.tm_sec);
			Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);//��					
			if(((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX])&(1>>0))!=0x0000)//����ѹ �ֶ�����
			{//����
				four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrXTSZDQY);
				sprintf((char*)p->Buffer,"%.2f",four.FloatData);
				Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);//����ѹ
			}
			//��ʾ��ַ��Ϣ
			{
				const char uCursor[]={0xff,0xff,0x00}; //��������0XFFʹ���λ����ȷ��ʾ��
				sprintf((char*)p->Buffer,"%s",(u8 *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));
				strcat((char*)p->Buffer, uCursor);
				
			}				
			Screen_RefreshDataMulti(0x0200,p->Buffer,15*2);
			//��ˮ���
			Screen_ResetDataBuffer();
			sprintf((char*)p->Buffer,"%.2d",Flashisp_GetData()->Data[FLASHIS_AdrXTSZZDPSJG]);
			Screen_RefreshDataMulti(0x0180,p->Buffer,15*2);
			//��ˮʱ��
			Screen_ResetDataBuffer();
			sprintf((char*)p->Buffer,"%.2d",Flashisp_GetData()->Data[FLASHIS_AdrXTSZZDPSSC]);
			Screen_RefreshDataMulti(0x01A0,p->Buffer,15*2);
			
			//��ʪ��ʱ��
			Screen_ResetDataBuffer();
			sprintf((char*)p->Buffer,"%.2d",Flashisp_GetData()->Data[FLASHIS_AdrXTSZCHSLSJ]);
			Screen_RefreshDataMulti(0x01C0,p->Buffer,15*2);	
		
			
			p->RunModeStat=2;
			p->Timer=50;
			break;
		}
			
		case 0x02: //��̬������ʾ
		{
			   	if(p->Timer==0)
					{
					  p->Timer=500;
						Screen_ResetDataBuffer();
						if(p->Key.Mode == 1) //����
						{
							if(p->Key.bOutsideKeyboard != false)
								ScreenEnterNumberTwinkle(p,0x0100);
						}
						else
						{
							sprintf((char*)p->Buffer,"%02d-%02d-%02d",p->Clock.Clock.tm_year+SCREEN_YearBase,p->Clock.Clock.tm_mon,p->Clock.Clock.tm_mday);
							Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);//��
							//strcpy((char*)p->Key.Buffer,(char*)p->Buffer);
						}
						Screen_ResetDataBuffer();
						if(p->Key.Mode == 2) //ʱ��
						{
							if(p->Key.bOutsideKeyboard != false)
								ScreenEnterNumberTwinkle(p,0x0120);
						}
						else
						{
							sprintf((char*)p->Buffer,"%02d:%02d:%02d",p->Clock.Clock.tm_hour,p->Clock.Clock.tm_min,p->Clock.Clock.tm_sec);
							Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);//��
							//strcpy((char*)p->Key.Buffer,(char*)p->Buffer);
						}

				        if((p->Key.Mode != 0) && (p->Key.bOutsideKeyboard != false))	//2020-05-20 hjl modify
				       {
					    switch(p->Key.Mode)
					   {
						case 3:
						{//����ѹ������˸
							if((pf->Data[FLASHIS_AdrXTSZBOX]&(1>>0))!=0x0000)
							{
								ScreenEnterNumberTwinkle(p,0x0140);
								break;
							}
						}
						
						case 7:
						{//��ˮ�����˸
							ScreenEnterNumberTwinkle(p,0x0180);
							break;
						
						}
						
						case 8:
						{//��ˮʱ����˸
							ScreenEnterNumberTwinkle(p,0x01A0);
							break;
						}
						
						case 10:
						{//��ʪ��ʱ��
							ScreenEnterNumberTwinkle(p,0x01C0);
							break;
						
						}
					  }	
				    }
					else
					{
						
						
						if((pf->Data[FLASHIS_AdrXTSZBOX]&(1>>0))==0x0000)//����ѹ����ֵ
						{//����ѹ
						Screen_ResetDataBuffer();
					    if(Slave_GetData()->DQPress.Value == FLUX_ColFAULT) 
						sprintf((char*)p->Buffer,"����");
					    else 
						sprintf((char*)p->Buffer,"%.2f",Slave_GetData()->DQPress.Value);//Slave_GetData()->DQPress.Value
					    Screen_RefreshDataMulti(0x0140,p->Buffer,6*2);//����ѹ
				        }
				        else
				        {//����ѹ����ֵ
					     four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrXTSZDQY);
					     sprintf((char*)p->Buffer,"%.2f",four.FloatData);
					     Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);//����ѹ
				        }
					}
			}
		}	
	}
	//ѡ���ͼ����
	ScreenSetJitu(p,5);
}


/* date 2019-12-12
	��������void Screen_Sys_CYSZPage(Screen_DataStructure *p)
	������	ϵͳ���ý�����������߼�����
*/
/*
void Screen_Sys_CYSZPage(Screen_DataStructure *p)
{

	Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	Button_Sys_CYSZPage();
	if(p->RunMode!=0x1100) return;
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(138); 	
				 ScreenSetJitu(p,9);
				 ScreenSetJitu(p,6);
				 ScreenSetJitu(p,7);
				for(int i=0;i<3;i++)
			 {
			   p->DisValue[i]=pf->Data[FLASHIS_AdrXTSZYQCLSJ+i];
				 sprintf((char*)p->Buffer,"%.0f",p->DisValue[i]);
	       Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);
			 }

				p->RunModeStat=2;
				p->Timer=50;
			}
			break;
			case 0x02: //��̬������ʾ
			{
			   	if(p->Timer==0)
					{
					  p->Timer=500;
						if(p->Key.Mode >= 3&&p->Key.Mode <= 5) //ʱ��
						{
							if(p->Key.bOutsideKeyboard != false)
								ScreenEnterNumberTwinkle(p,0x0100+0x20*(p->Key.Mode-3));  //�˴���ַ��Ӧ�������ĵ�ַ����Ϊ�������Ĵ��ؼ�ֵΪ3��4��5���Ӧ���ı����ַΪ0X0100��0X0120��0X0140����Ҫ��Ӧ����0x20
						}
						
					
			}
		}	
  }
		//ѡ���ͼ����
	  ScreenSetJitu(p,8);
}
*/
//---
/* date 20200513
	��������void Screen_FluePag(Screen_DataStructure *p)
	������	�̵��������ý����߼�����
*/
void Screen_FluePag(Screen_DataStructure *p)
{
//	FourBytes four;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  static u16 LastChoice=0;
	 //��������
	  Button_FlueSetPage();
	  if(p->RunMode!=0x2000) return;
			//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
			  
				case 0x01: Screen_PastePicture(0x0800,13,(155<<16)+103,(354<<16)+170,(155<<16)+103);break;
				case 0x02: Screen_PastePicture(0x0800,13,(155<<16)+217,(353<<16)+285,(155<<16)+217);break;
				case 0x03: Screen_PastePicture(0x0800,13,(443<<16)+103,(643<<16)+160,(443<<16)+103);break;
				case 0x04: Screen_PastePicture(0x0800,13,(536<<16)+370,(714<<16)+440,(536<<16)+370);break;
				case 0x05: Screen_PastePicture(0x0800,13,(711<<16)+372,(795<<16)+441,(711<<16)+372);break;
						
				default:break;

			}
//			u32 ttt = pf->Data[FLASHIS_AdrYDBDLX];
			switch(pf->Data[FLASHIS_AdrYDBDLX])
			{
				case 0x01: Dis_CheckBox(0x0C20,1,(116<<16)+130);break;
				case 0x02: Dis_CheckBox(0x0C20,1,(116<<16)+243);break;
				case 0x03: Dis_CheckBox(0x0C20,1,(404<<16)+130);break;
				//	case 0x04: Dis_CheckBox(0x0C20,1,(404<<16)+243);break;            //20200513
				default:break;
			}
//			switch(pf->Data[FLASHIS_AdrYDBDLX])		// 2020-05-25 add by alfred.
//				 {
//						case 0x01: Screen_PastePicture(0x0800,13,(155<<16)+103,(354<<16)+170,(155<<16)+103);break;
//						case 0x02: Screen_PastePicture(0x0800,13,(155<<16)+217,(353<<16)+285,(155<<16)+217);break;
//						case 0x03: Screen_PastePicture(0x0800,13,(443<<16)+103,(643<<16)+160,(443<<16)+103);break;
//				 }

			
		}
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				Screen_ShowPage(12); 
				LastChoice=0;	                      //20200513
				p->RunModeStat=2;				 
			 }
			break;
			case 0x02: //��̬������ʾ
			{			
				if(LastChoice!=pf->Data[FLASHIS_AdrYDBDLX])	
				{
				  LastChoice=pf->Data[FLASHIS_AdrYDBDLX];
					switch(LastChoice)
					{
					    case 0x01: Dis_CheckBox(0x0C20,1,(116<<16)+130);break;
						  case 0x02: Dis_CheckBox(0x0C20,1,(116<<16)+243);break;
						  case 0x03: Dis_CheckBox(0x0C20,1,(404<<16)+130);break;
						//	case 0x04: Dis_CheckBox(0x0C20,1,(404<<16)+243);break;            //20200513
						default:break;
					}
				}
			}
			break;
		}

}
/* date 2019-12-18
	��������void Screen_FlueCircularPag(Screen_DataStructure *p)
	������	�̵�����-Բ�����ý����߼�����
*/

void Screen_FlueCircularPag(Screen_DataStructure *p)
{

	  Flashisp_DataStructure* pf=Flashisp_GetData();
	u16 Single_hole_Number=1;
	 //��������
	  Button_FlueCircularPage();
	  if(p->RunMode!=0x2100) return;
				//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
			p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{		  
				case 0x01: Screen_PastePicture(0x0800,15,(53<<16)+56,(178<<16)+106,(53<<16)+55);break;
				case 0x02: Screen_PastePicture(0x0800,15,(53<<16)+107,(178<<16)+157,(53<<16)+107);break;
				case 0x03: Screen_PastePicture(0x0800,15,(53<<16)+158,(178<<16)+208,(53<<16)+159);break;
				case 0x04: Screen_PastePicture(0x0800,15,(53<<16)+209,(178<<16)+255,(53<<16)+209);break;
				case 0x05: Screen_PastePicture(0x0800,15,(53<<16)+259,(178<<16)+310,(53<<16)+259);break;
				case 0x06: Screen_PastePicture(0x0800,15,(53<<16)+310,(178<<16)+360,(53<<16)+310);break;
				case 0x07: Screen_PastePicture(0x0800,15,(720<<16)+310,(790<<16)+372,(720<<16)+310);break;
				case 0x08: Screen_PastePicture(0x0800,15,(720<<16)+375,(790<<16)+440,(720<<16)+375);break;
				default:break;
			}
		}
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(14);
				 //��ʾ����װ��
				  p->DisValue[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDYXNJ); 
				  p->DisValue[1]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_MJ); 
				 
				 
				 if(p->Key.LastMode==2)   //һ����뾲̬������˭����˭��ֱ��-���������
				 {
					p->DisValue[1]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_MJ); 
					p->DisValue[0]=2.0f*sqrt(p->DisValue[1]/PI); //���������뾶
					SaveFlaotToFlash(p->DisValue[0],FLASHIS_AdrYDBDYXNJ);//����ֱ��
			        CalculationCircularMeasurePoint(1,p->DisValue[0],GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT),&(pf->Data[FLASHIS_AdrYDBDYXHS]),&Single_hole_Number,&(p->DisValue[6]));
				 }
				 else if(p->Key.LastMode==1) //�ɰ뾶�������
				 {
					 p->DisValue[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDYXNJ); 
					 p->DisValue[1]= pow(p->DisValue[0]/2,2)*PI;//����԰�����
					 SaveFlaotToFlash(p->DisValue[1],FLASHIS_AdrYDBD_MJ);//�������
					 CalculationCircularMeasurePoint(1,p->DisValue[0],GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT),&(pf->Data[FLASHIS_AdrYDBDYXHS]),&Single_hole_Number,&(p->DisValue[6]));
				 }	
                 else				 
				 CalculationCircularMeasurePoint(0,p->DisValue[0],GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT),&(pf->Data[FLASHIS_AdrYDBDYXHS]),&Single_hole_Number,&(p->DisValue[6]));
				 p->DisValue[2]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT);//����
		
				 p->DisValue[3]= pf->Data[FLASHIS_AdrYDBDYXHS]; //����
				 p->DisValue[4]= pf->Data[FLASHIS_AdrYDBDYXKS]; //����
				 p->DisValue[5]= Single_hole_Number* p->DisValue[4]; 
				 pf->Data[FLASHIS_AdrYDBD_CYDS]=p->DisValue[5];//�ܲ�������
				 pf->Flag=1;//����
				//��ʾ����
				for(int i=0;i<6;i++)
				{
					if(i<3)
					{				
						if(i==1)  
							sprintf((char*)p->Buffer,"%.4f",p->DisValue[i]);
						else
							sprintf((char*)p->Buffer,"%.2f",p->DisValue[i]);
					}
					else
					{
						sprintf((char*)p->Buffer,"%d",(u32)p->DisValue[i]);
					}				
					Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);
				}
				//��ʾ��ַ��Ϣ
				{
				 char uCursor[]={0xff,0xff,0x00}; //��������0XFFʹ���λ����ȷ��ʾ��
				 sprintf((char*)p->Buffer,"%s",(u8 *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));
				 strcat((char*)p->Buffer, uCursor);
				//  sprintf((char*)p->Buffer,"%s",(u8 *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));
				}				
				Screen_RefreshDataMulti(0x01A0,p->Buffer,15*2);
				//��ʾ������
				for(int i=0;i<10;i++)
				{
					Screen_ResetDataBuffer();
					if(i<(Single_hole_Number)){
					  
						sprintf((char*)p->Buffer,"%.3f",p->DisValue[i+6]);
					}
				  if(i%2==0) //ż��
					{
					  Screen_RefreshDataMulti(0x0A00+(0x10)*(i/2+1),p->Buffer,15*2);//��
					}else      //����
					{
					   Screen_RefreshDataMulti(0x0A00+(0x10)*(i/2+1)+8,p->Buffer,15*2);//��
					}
				}
                sprintf((char*)p->Buffer,"%d",(u32)p->DisValue[5]);		
				Screen_RefreshDataMulti(0x01C0,p->Buffer,15*2); 
				p->RunModeStat=2;				 
			 }
			break;
			case 0x02: //��̬������ʾ
			{	
				if(p->Timer==0)
				{
					if(p->Key.Mode != 0)
					{
						if(p->Key.bOutsideKeyboard != false)
						ScreenEnterNumberTwinkle(p,0x00e0+0x20*p->Key.Mode);		
					}
						p->Timer=500;
				}
			}
			break;
		}

}
/* date 2019-12-18
	��������void Screen_FlueCircularPag(Screen_DataStructure *p)
	������	�̵�����-�̵���ѯ�����߼�����
*/
void Screen_FlueSeekPag(Screen_DataStructure *p)
{

	  
	  Flashisp_DataStructure* pf=Flashisp_GetData();
		//��������
	  Button_FlueSeekPage(&p->FileNumber);
	  if(p->RunMode!=0x2400) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
			p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{		  
				case 0x01: Screen_PastePicture(0x0800,137,(24<<16)+46,(158<<16)+91,(24<<16)+46);break;
				case 0x02: Screen_PastePicture(0x0800,137,(298<<16)+374,(417<<16)+439,(298<<16)+374);break;
				case 0x03: Screen_PastePicture(0x0800,137,(419<<16)+374,(538<<16)+440,(419<<16)+374);break;
				case 0x04: Screen_PastePicture(0x0800,137,(541<<16)+374,(655<<16)+440,(541<<16)+374);break;
				case 0x05: Screen_PastePicture(0x0800,137,(661<<16)+375,(746<<16)+440,(661<<16)+375);break;
				default:break;
			}
		}
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(136);
				 
				//�����ʾ����
				for(int i=0;i<10;i++)
				{
					if(i<=1) 
					{
						Screen_RefreshDataMulti(0x01C0+(0x20)*(i),(u8 *)"                    ",15*2);
					}
					else      
					{
						Screen_RefreshDataMulti(0x0400+(0x20)*(i-2),(u8 *)"                    ",15*2);
					}
				}   
				 
				 SdFile_Serch_MaxofFlueLinkFile();//�ȶ�ȡ����ļ���
				 ReadFlueData(p->FileNumber);
				 //��ʾ����װ�� 
				//��ʾ����
					 p->RunModeStat=2;				 
			 }
			break;
			case 0x02: //��̬������ʾ
			{	
				if(p->Timer==0)
				{
					if(p->Key.Mode != 0)
					{	
						if(p->Key.bOutsideKeyboard != false)
							   ScreenEnterNumberTwinkle(p,0x00e0+0x20*p->Key.Mode);		
					}
					p->Timer=500;	
			    }
			}
			break;
		}

}
	/* date 2019-12-18
	��������void Screen_FlueOtherPag(Screen_DataStructure *p)
	������	�̵�����-������״���ý����߼�����  
*/
void Screen_FlueOtherPag(Screen_DataStructure *p)
{
    FourBytes four;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	  Button_FlueOtherPage();
	  if(p->RunMode!=0x2300) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
			  
					    case 0x01: Screen_PastePicture(0x0800,19,(141<<16)+120,(343<<16)+165,(141<<16)+120);break;
						  case 0x02: Screen_PastePicture(0x0800,19,(141<<16)+215,(343<<16)+265,(141<<16)+215);break;
				      case 0x03: Screen_PastePicture(0x0800,19,(141<<16)+305,(343<<16)+350,(141<<16)+305);break;
				      case 0x04: Screen_PastePicture(0x0800,19,(640<<16)+370,(718<<16)+440,(640<<16)+370);break;			
						  case 0x05: BACK_BUTTON;break;
						  default:break;
			}
		}
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				Screen_ShowPage(18);
		
				four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_MJ);//���
				sprintf((char*)p->Buffer,"%.4f",four.FloatData);
				Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);
				sprintf((char*)p->Buffer,"%d",pf->Data[FLASHIS_AdrYDBD_CYDS]);//�����ܵ���
				Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);    
				p->RunModeStat=2;	
		//��ʾ��ַ��Ϣ
				{
									 char uCursor[]={0xff,0xff,0x00}; //��������0XFFʹ���λ����ȷ��ʾ��
				 sprintf((char*)p->Buffer,"%s",(u8 *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));
				 strcat((char*)p->Buffer, uCursor);
				 // sprintf((char*)p->Buffer,"%s",(u8 *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));
				}				
				Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);				 
			 }
			break;
			case 0x02: //��̬������ʾ
			{	
				if(p->Timer==0)
				{
				if(p->Key.Mode != 0)
					{			
							if(p->Key.bOutsideKeyboard != false)
							   ScreenEnterNumberTwinkle(p,0x00e0+0x20*p->Key.Mode);
					}
					p->Timer=500;
				}
			}
			break;
		}

		
}
/* date 2019-12-18
	��������void Screen_FlueSquarePag(Screen_DataStructure *p)
	������	�̵����㷽-�����ý����߼�����  
*/
void Screen_FlueSquarePag(Screen_DataStructure *p)
{
 
	  Flashisp_DataStructure* pf=Flashisp_GetData();
      u16 buf;
	 //��������
	  Button_FlueSquarePage();
	  if(p->RunMode!=0x2200) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{ 
					 case 0x01: Screen_PastePicture(0x0800,17,(53<<16)+43,(180<<16)+85,(53<<16)+43);break;
					 case 0x02: Screen_PastePicture(0x0800,17,(53<<16)+90,(180<<16)+130,(53<<16)+90);break;
					 case 0x03: Screen_PastePicture(0x0800,17,(53<<16)+135,(180<<16)+175,(53<<16)+135);break;
				     case 0x04: Screen_PastePicture(0x0800,17,(53<<16)+179,(180<<16)+220,(53<<16)+179);break;
					 case 0x05: Screen_PastePicture(0x0800,17,(53<<16)+224,(180<<16)+265,(53<<16)+224);break;
				     case 0x06: Screen_PastePicture(0x0800,17,(53<<16)+267,(180<<16)+305,(53<<16)+267);break;
				     case 0x07: Screen_PastePicture(0x0800,17,(53<<16)+310,(180<<16)+355,(53<<16)+310);break;
				     case 0x08: Screen_PastePicture(0x0800,17,(720<<16)+310,(790<<16)+372,(720<<16)+310);break;
				     case 0x09: Screen_PastePicture(0x0800,17,(720<<16)+375,(790<<16)+440,(720<<16)+375);break;
                     default:break;
			}
		}
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {       
				Screen_ShowPage(16);
			  //��ʾ����װ��

				 if(p->Key.LastMode==3) //���������B��
				 {
	
					 p->DisValue[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBA); //A ��
					 p->DisValue[2] =GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_MJ); //���
					 p->DisValue[1]=p->DisValue[2]/p->DisValue[0];
					 SaveFlaotToFlash(p->DisValue[1],FLASHIS_AdrYDBDFXBB); //����B��
				 }else //��AB���� ���
				 {
		
					 p->DisValue[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBA); //A ��
					 p->DisValue[1]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBB); //B ��
					 p->DisValue[2]=p->DisValue[1]*p->DisValue[0];
					 SaveFlaotToFlash(p->DisValue[2],FLASHIS_AdrYDBD_MJ); //�������
				
				 }
				 p->DisValue[3]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT); //����
				 p->DisValue[4]= pf->Data[FLASHIS_AdrYDBDFXKS];
                 p->DisValue[5]= pf->Data[FLASHIS_AdrYDBDFXDKCD];
				 
				  if((p->Key.LastMode>=1)&&(p->Key.LastMode<=3))
				 { 
                     buf=SquareCalculationMeasureHold(p->DisValue[2],p->DisValue[0],p->DisValue[1]); //������������ײ��
				
                     p->DisValue[4]=(float)((buf&0xff00)>>8);
					 SaveU16ToFlash(((buf&0xff00)>>8),FLASHIS_AdrYDBDFXKS);
					 p->DisValue[5]=(float)(buf&0x00ff);
					 SaveU16ToFlash(buf&0x00ff,FLASHIS_AdrYDBDFXDKCD);
					 
				 }
	
					 //���������λ��
					SquareCalculationMeasurePointN(&(pf->Data[FLASHIS_AdrYDBDFXDKCD]),p->DisValue[4],p->DisValue[1],p->DisValue[3],p->DisValue[0],&(p->DisValue[7])); 
			
				

				 p->DisValue[6]=p->DisValue[4]*p->DisValue[5];
				 pf->Data[FLASHIS_AdrYDBD_CYDS]=p->DisValue[6];
				 pf->Flag=1;
				//��ʾ����
				for(int i=0;i<7;i++)
				{
				if(i<4)
				{
					if(i==2)  sprintf((char*)p->Buffer,"%.4f",p->DisValue[i]);
					else
				  sprintf((char*)p->Buffer,"%.2f",p->DisValue[i]);
				}else
				{
				  sprintf((char*)p->Buffer,"%d",(u32)p->DisValue[i]);
				}
				
				Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);
				}
				//��ʾ��ַ��Ϣ
				{
				 char uCursor[]={0xff,0xff,0x00}; //��������0XFFʹ���λ����ȷ��ʾ��
				 sprintf((char*)p->Buffer,"%s",(u8 *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));
				 strcat((char*)p->Buffer, uCursor);
				 // sprintf((char*)p->Buffer,"%s",(u8 *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));
				}				
				Screen_RefreshDataMulti(0x01C0,p->Buffer,15*2);
				for(int i=0;i<10;i++)
				{
					Screen_ResetDataBuffer();
					if(i<pf->Data[FLASHIS_AdrYDBDFXDKCD])
					sprintf((char*)p->Buffer,"%.3f",p->DisValue[i+7]);
				  if(i%2==0) //ż��
					{
					  Screen_RefreshDataMulti(0x0A00+(0x10)*(i/2+1),p->Buffer,15*2);//��
					}else      //����
					{
					   Screen_RefreshDataMulti(0x0A00+(0x10)*(i/2+1)+8,p->Buffer,15*2);//��
					}
				}  
				 sprintf((char*)p->Buffer,"%d",(u32)p->DisValue[6]);		
				Screen_RefreshDataMulti(0x01E0,p->Buffer,15*2); 
			 p->RunModeStat=2;				 
			 }
			break;
	
			case 0x02: //��̬������ʾ
			{	
				if(p->Timer==0)
				{
				if(p->Key.Mode != 0)
					{
						if(p->Key.bOutsideKeyboard != false)
							ScreenEnterNumberTwinkle(p,0x00e0+0x20*p->Key.Mode);		
			    }
					p->Timer=500;
			}
			}
			break;
		}
	
}
//---
/* date 2019-12-19
	��������void Screen_WCP_SetPag(Screen_DataStructure *p)
	������	����Ԥ��
*/
void Screen_WCP_SetPag(Screen_DataStructure *p)
{
  //	FourBytes four;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	  Button_WCP_SetPag(5,1,p->DisValue);
	  if(p->RunMode!=0x3000) return;
			//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
					      case 0x01: Screen_PastePicture(0x0800,21,(124<<16)+141,(370<<16)+221,(124<<16)+141);break;
						  case 0x02: Screen_PastePicture(0x0800,21,(442<<16)+141,(688<<16)+221,(442<<16)+141);break;
				          case 0x03: Screen_PastePicture(0x0800,21,(124<<16)+241,(370<<16)+321,(124<<16)+241);break;
						  case 0x04: Screen_PastePicture(0x0800,21,(442<<16)+241,(688<<16)+321,(442<<16)+241);break;
						  case 0x05: Screen_PastePicture(0x0800,21,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;					
				default:break;
			}
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(20); 	
				 p->RunModeStat=2;				 
			 }
			break;
			}
		}
}
//----
/* date 2019-12-19
	��������void Screen_WCP_PressureZeroPag(Screen_DataStructure *p)
	������	����Ԥ��-ѹ������
*/
void Screen_WCP_PressureZeroPag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  Slave_DataStructure* Sp=Slave_GetData();
		 //��������
	  Button_WCP_PressureZeroPag(2,1,p->DisValue);
	  if(p->RunMode!=0x3100) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
			  
					  case 0x01: Screen_PastePicture(0x0800,23,(583<<16)+360,(708<<16)+440,(583<<16)+360);break;		
				      case 0x02: Screen_PastePicture(0x0800,23,(721<<16)+360,(790<<16)+440,(721<<16)+360);break;
					  default:break;

			}
		}
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
       
				Screen_ShowPage(22);
                p->Timer=0;
				p->TimerSec=10;
				p->Function=2;
				p->RunModeStat=2;				 
			 }
			break;
	
			case 0x02: //��̬������ʾ
			{	
				      if(p->Timer==0)
							{

							Screen_ResetDataBuffer();
							sprintf((char*)p->Buffer,"%d",(u32)Sp->ACPress.Value);//������ѹ
							Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);//
							sprintf((char*)p->Buffer,"%.2f",Sp->AGPress.Value);//������ѹ
							Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);//
						    sprintf((char*)p->Buffer,"%.2f",Sp->BGPress.Value);//��ǰѹ��
							Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);//
                            sprintf((char*)p->Buffer,"%d",(u32)Sp->BCPress.Value);//����ѹ		
							Screen_RefreshDataMulti(0x0160,p->Buffer,15*2);//
	                        sprintf((char*)p->Buffer,"%d",(u32)Sp->CCPress.Value);//����ѹ		
							Screen_RefreshDataMulti(0x0180,p->Buffer,15*2);// ����ѹPA
						    p->Timer=500;
							if(p->TimerSec==0)
							{
							  //to ȷ�����ֵ
								if(p->Function==2)
                                {									
								Slave_Zero_Press();
								Screen_RefreshDataMulti(0x01A0,p->Buffer,15*2);//����ʱ
								sprintf((char*)p->Buffer,"�������!");
								Screen_RefreshDataMulti(0x01A0,p->Buffer,15*2);//����ʱ	
								Speak_Buzzer(300);	
								}
								p->Function=0;

							}else
							{
							  sprintf((char*)p->Buffer,"%2dS",p->TimerSec);
							  Screen_RefreshDataMulti(0x01A0,p->Buffer,15*2);//����ʱ
							}
						}
			}
			break;
		}
}
//--
/* date 2019-12-19
	��������void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p)
	������	����Ԥ��-���²���
*/
void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p)
{

	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  Slave_DataStructure* Sp=Slave_GetData();
		 //��������
	  Button_WCP_FumeTemperaturPag(2,1,p->DisValue);
	  if(p->RunMode!=0x3200) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
					    case 0x01: Screen_PastePicture(0x0800,25,(66<<16)+188,(200<<16)+234,(66<<16)+188);break;		
				        case 0x02: Screen_PastePicture(0x0800,25,(721<<16)+375,(790<<16)+440,(721<<16)+375);break;
						default:break;
			}
		}
	
		{ 
		 p->LastChoice=pf->Data[FLASHIS_AdrGKYC_YWCL_FS] ;
			switch(p->LastChoice)
			{	  
					  case 0x00: Screen_PastePicture(0x1820,25,(231<<16)+201,(259<<16)+226,(231<<16)+201);break;		
				      case 0x01: Screen_PastePicture(0x1820,25,(338<<16)+201,(366<<16)+226,(338<<16)+201);break;
					  default:   Screen_PastePicture(0x1820,25,(338<<16)+201,(366<<16)+226,(338<<16)+201);break;
			}
		}
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				Screen_ShowPage(24);
                p->Timer=0;
				p->TimerSec=10;
				p->RunModeStat=2;				 
			 }
			break;	
			case 0x02: //��̬������ʾ
			{	
				      if(p->Timer==0)
							{
								if(p->Key.Mode==1)
								{
									if(p->Key.bOutsideKeyboard != false)
									ScreenEnterNumberTwinkle(p,0x0100);
								}
								else
								{
									Screen_ResetDataBuffer();   
									 if(pf->Data[FLASHIS_AdrGKYC_YWCL_FS]==0)
									 {
									     sprintf((char*)p->Buffer,"%.1f",Sp->YWTem.Value);
									 }
									else 
										 {
											
						       	  sprintf((char*)p->Buffer,"%.1f", GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_YWCL_SRYW)); //��������
										 }   
							     Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);//����ʱ
								}
															
						  p->Timer=500;
						}
			}
			break;
		}
}
//--
/* date 2019-12-19
	��������void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p)
	������	����Ԥ��-Ԥ������
*/
void Screen_WCP_PredictedVelocityPag(Screen_DataStructure *p)
{
//    FourBytes four;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	Slave_DataStructure* Sp=Slave_GetData();
//	FourBytes four;
	Calculation_FormulaStructure * Cp=CalculationFormula_GetData();
	 //��������
	Button_WCP_PredictedVelocityPag(3,1,p->DisValue);
	if(p->RunMode!=0x3300) return;
	//ѡ���ͼ����
	if(p->Key.LastOpt!=p->Key.Opt)
	{ 
		p->Key.LastOpt=p->Key.Opt; 
		switch(p->Key.Opt)
		{  
			case 0x01: Screen_PastePicture(0x0800,27,(409<<16)+375,(520<<16)+440,(409<<16)+375);break;
			case 0x02: Screen_PastePicture(0x0800,27,(523<<16)+375,(712<<16)+440,(523<<16)+375);break;				
			case 0x03: Screen_PastePicture(0x0800,27,(721<<16)+375,(790<<16)+440,(721<<16)+375);break;
			default:break;
		}
	}
	 //���ݸ���
    switch(p->RunModeStat)
	{
		case 0x01: //��̬������ʾ			
		{
			Screen_ShowPage(26);
			p->Timer=0;
			p->RunModeStat=2;
			
			Screen_RefreshDataMulti(0x0420,(u8*)"                              ",15*2);// �̵�����	
			Screen_ResetDataBuffer(); 
			sprintf((char*)p->Buffer,"%s",GetYDBD_Describe(pf->Data[FLASHIS_AdrYDBDLX]));
			Screen_RefreshDataMulti(0x0460,p->Buffer,15*2);// �̵�����		
			Dis_Tips(p,(u8*)"���ڲ�����������",0x0401);//��ʾ��ʾ	
			Screen_ResetDataBuffer(); 
			sprintf((char*)p->Buffer,"%2d",p->StarCun); //��ʾ����
			Screen_RefreshDataMulti(0x0440,p->Buffer,15*2);// �̵�����
			Screen_ResetDataBuffer(); 
			sprintf((char*)p->Buffer,"��%2d��",pf->Data[FLASHIS_AdrYDBD_CYDS]); //��ʾ�ܹ�����
			Screen_RefreshDataMulti(0x0480,p->Buffer,15*2);// �̵�����
			
            //���p->DisValue	
         

		}
		break;
		case 0x02: //��̬������ʾ
		{	
			if(p->Timer==0)
			{		
				p->DisValue[0]=Cp->Pd;//��ѹ
				p->DisValue[1]=Cp->Pt;//����ȫѹ
				p->DisValue[2]=Cp->Ps;	//��ѹ	
				p->DisValue[3]=Cp->Ba;	//����ѹ
				p->DisValue[5]=Cp->Ts ; //����
				p->DisValue[4]=Cp->Vs; //��������
				p->DisValue[6]=Cp->F;//��ȡ�̵����
                p->DisValue[7]=Cp->Qs;//��������	
				for(int i=0;i<8;i++)
				{
					if(i==0)  
					{
						sprintf((char*)p->Buffer,"%d",	(u32)p->DisValue[i]);
					}
					else if((i==1)||(i==2))
					{
						 sprintf((char*)p->Buffer,"%.2f",p->DisValue[i]);
					}
					else if(i==3||(i==4)||(i==5)||(i==7))
					{
						 sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);
					}else if(i==6)
					{
						 sprintf((char*)p->Buffer,"%.4f",p->DisValue[i]);
					}
					Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);
				}									
			p->Timer=500;
			}
		}
		break;
	}
} 
//---
/* date 2019-12-19
	��������void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p)
	������	����Ԥ��-��ƽ��ֵ
*/
void Screen_WCP_CheckAveragePag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
		 //��������
	  Button_WCP_CheckAveragePag(5,1,&p->FileNumber);
	  if(p->RunMode!=0x3400) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{			  
				case 0x01: Screen_PastePicture(0x0800,29,(58<<16)+391,(177<<16)+429,(58<<16)+391);break;		//modify by alfred,2020.07.03
				case 0x02: Screen_PastePicture(0x0800,29,(257<<16)+375,(470<<16)+440,(257<<16)+375);break;
				case 0x03: Screen_PastePicture(0x0800,29,(473<<16)+375,(589<<16)+440,(473<<16)+375);break;
				case 0x04: Screen_PastePicture(0x0800,29,(595<<16)+375,(710<<16)+440,(595<<16)+375);break;				
				case 0x05: Screen_PastePicture(0x0800,29,(715<<16)+375,(790<<16)+440,(715<<16)+375);break;
				default:break;
			}
		}
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
					Screen_ShowPage(28);
					p->Timer=0;
					p->RunModeStat=2;
					//��ʾ����װ��
                    if(p->LoadFlag==1)
					{
					  p->LoadFlag=0;
						SdFile_Serch_MaxofWorkLinkFile();//�ȶ�ȡ����ļ���
						p->FileNumber=SdFile_GetData()->WorkLinkFileMax;
					}
					ReadWorkData(p->FileNumber);
                    Dis_Tips(p,(u8*)"�ļ���ѯ�ɹ�",0x3B20);//��ʾ��ʾ							
			 }
			break;
			case 0x02: //��̬������ʾ
			{	
				      if(p->Timer==0)
							{ 
								if(p->Key.Mode==1)
								{
									if(p->Key.bOutsideKeyboard != false)
											ScreenEnterNumberTwinkle(p,0x0440);
								}									
						  p->Timer=500;
						    }
			}
			break;
		}
}
//---
/* date 2019-12-25
	��������void Screen_CHSL_SetPag(Screen_DataStructure *p)
	������	�⺬ʪ������
*/
void Screen_CHSL_SetPag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	  Button_CHSL_SetPag(4,1,&FileValue);
	  if(p->RunMode!=0x4000) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{		  
					    case 0x01: Screen_PastePicture(0x0800,31,(137<<16)+63,(480<<16)+132,(137<<16)+63);break;
						case 0x02: Screen_PastePicture(0x0800,31,(137<<16)+161,(480<<16)+230,(137<<16)+161);break;
				        case 0x03: Screen_PastePicture(0x0800,31,(137<<16)+264,(480<<16)+333,(137<<16)+264);break;
						case 0x04: Screen_PastePicture(0x0800,31,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;
						
				default:break;

			}
		}
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(30); 	
				sprintf((char*)p->Buffer,"%.2f",GetFlaotdataFormFlash2Word(FLASHIS_AdrCHSL_HSL));//��ʪ��
				Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);		
				 p->RunModeStat=2;				 
			 }
			break;
      case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
							{							 
								if(p->Key.Mode==3)
								{
									if(p->Key.bOutsideKeyboard != false)
									ScreenEnterNumberTwinkle(p,0x0100);
								}			 							
						  p->Timer=500;
						}			 
			 }
			break;
			}
		
}
//---
/* date 2019-12-25
	��������void Screen_CHSL_SetPag(Screen_DataStructure *p)
	������	�⺬ʪ����ʪ��
*/
void Screen_CHSL_GSQF_Pag(Screen_DataStructure *p)
{
   Flashisp_DataStructure* pf=Flashisp_GetData();
	 Calculation_FormulaStructure * Cp=CalculationFormula_GetData();
	 //��������
	  Button_CHSL_GSQF_Pag(2,1,p->DisValue);
	  if(p->RunMode!=0x4100) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
					    case 0x01: Screen_PastePicture(0x0800,33,(545<<16)+375,(712<<16)+440,(545<<16)+375);break;
						  case 0x02: Screen_PastePicture(0x0800,33,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;
				default:break;
			}
		}
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(32);
				 p->TimerSec=pf->Data[FLASHIS_AdrXTSZCHSLSJ]*60;
				  p->mystatus=0;
				 if( p->TimerSec==0)
				 	p->mystatus=1;
                 Flux_StarSampling('B',15.0f);			//������25L/MIN  ,GB��15L/min	 
				 p->RunModeStat=2;	
       // Dis_Tips(p,"��ʼ����",0x01e0);			 
			 }
			break;
      case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
							{	
								 p->DisValue[0]=	CalculationFormula_Xsw(CalculationFormula_GetData()->Phv,(CalculationFormula_GetData()->Tc)-(CalculationFormula_GetData()->Tb),
								(CalculationFormula_GetData()->Ba+CalculationFormula_GetData()->Pb));
                                 p->DisValue[1]=Flux_GetData()->ChannelB.ShowFlux;
								 p->DisValue[2]=Cp->Tc;
								 p->DisValue[3]=Cp->Tb;
								 p->DisValue[4]=Slave_GetData()->DustMotor.Ratio*100/Slave_GetData()->DustMotor.Pid.PWMcycle;//���㹦��
								 p->DisValue[5]=Cp->Pcs;
								 p->DisValue[6]=Cp->Tj;
								 if(p->mystatus==0)
								 sprintf((char*)p->Buffer,"����ʱ��%d S",p->TimerSec);
								 else
								 sprintf((char*)p->Buffer,"                    ");	 
                                 Dis_Tips(p,p->Buffer,0x01e0);				
 
							
								for(int i=0;i<7;i++)
								{
									Screen_ResetDataBuffer();
									if(i==0||i==5||i==6)
									sprintf((char*)p->Buffer,"%.2f",p->DisValue[i]);
									else if(i==4) 
									sprintf((char*)p->Buffer,"%.2d",(u32)p->DisValue[i]);
									else 			
									sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);							
									Screen_RefreshDataMulti(0x0100+i*0x20,p->Buffer,15*2);	
								}											
						   p->Timer=500;
						}			 
			 }
			break;
			}
}
//---
/* date 2019-12-25
	��������void Screen_CHSL_ZRF_Pag(Screen_DataStructure *p)
	������	�⺬ʪ�����ݷ�
*/
void Screen_CHSL_ZRF_Pag(Screen_DataStructure *p)
{
    float DisData={11.01};//0��ʪ�� 1���� 2�����¶� 3ʪ���¶� 4���� 5��ѹ 6����
	  Flashisp_DataStructure* pf=Flashisp_GetData();
		//���ݳ�ʼ��

	 //��������
	  Button_CHSL_ZRF_Pag(2,1,&FileValue);
	  if(p->RunMode!=0x4200) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
				case 0x01: Screen_PastePicture(0x0800,119,(545<<16)+375,(712<<16)+440,(545<<16)+375);break;
				case 0x02: Screen_PastePicture(0x0800,119,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;
				default:break;
			}
		}
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(118); 
                 Dis_Tips(p,(u8*)"ͨѶ�쳣",0x0120);						 
				 p->RunModeStat=2;				 
			 }
			break;
           case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
				{			

					Screen_ResetDataBuffer();

					sprintf((char*)p->Buffer,"%.2f",DisData);

					Screen_RefreshDataMulti(0x0101,p->Buffer,15*2);
					p->Timer=500;	
				}								
									
						   
								 
			 }
			break;
			}
}
//---
/* date 2019-12-25
	��������void Screen_CHSL_HSLCX_Pag(Screen_DataStructure *p)
	������	��ʪ����ѯ
*/
void Screen_CHSL_HSLCX_Pag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	  Button_CHSL_HSLCX_Pag(5,1,&p->FileNumber);
	  if(p->RunMode!=0x4400) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
					    case 0x01: Screen_PastePicture(0x0800,51,(181<<16)+67,(282<<16)+109,(181<<16)+67);break;
						case 0x02: Screen_PastePicture(0x0800,51,(95<<16)+375,(214<<16)+430,(95<<16)+375);break;
      				    case 0x03: Screen_PastePicture(0x0800,51,(216<<16)+375,(335<<16)+430,(216<<16)+375);break;
						case 0x04: Screen_PastePicture(0x0800,51,(343<<16)+375,(463<<16)+430,(343<<16)+375);break;
				        case 0x05: Screen_PastePicture(0x0800,51,(582<<16)+375,(671<<16)+440,(582<<16)+375);break;
				default:break;
			}
		}
	 //���ݸ���
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
					Screen_ShowPage(50);
					p->Timer=0;
					p->RunModeStat=2;
					//��ʾ����װ��
          
				  //������ʾ 
					sprintf((char*)p->Buffer,"%07d",(u32)FileValue);
					Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);// �ļ���						 
					sprintf((char*)p->Buffer,"2019-12-24");
					Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);// ����ʱ��	
					sprintf((char*)p->Buffer,"16:10");
					Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);// ����ʱ��					 
               for(int i=0;i<6;i++)
					{
						if(i==0)  
						{
							sprintf((char*)p->Buffer,"%d",(u32)p->DisValue[i]);
						}
						else 
						{
							 sprintf((char*)p->Buffer,"%.2f",p->DisValue[i]);
						}
				
					Screen_RefreshDataMulti(0x0160+0x20*i,p->Buffer,15*2);
					}								
			 }
			break;
			case 0x02: //��̬������ʾ
			{	
				      if(p->Timer==0)
							{ 
								if(p->Key.Mode==1)
								{
										ScreenEnterNumberTwinkle(p,0x0100);
								}									
						  p->Timer=500;
						}
			}
			break;
		}
}
//---2020-05-22
/* date 2019-12-26
	��������void Screen_CLYQ_SetPag(Screen_DataStructure *p)
	������	��������
	�޸ļ�¼��1. �������޸��˱�ҳ��ʾ���ݣ���Ӧ�޸��˴��룬ȥ������ʱ�������Ӽ�ƽ��ʱ���ͼ�ƽ������������, modify by alfred, 2020.06.13.
*/
void Screen_CLYQ_SetPag(Screen_DataStructure *p)
{
//	static u8 LastChoice=11;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//��������
	Button_CLYQ_SetPag(5,1,&FileValue);
	if(p->RunMode!=0x5000) return;
	//ѡ���ͼ����
	if(p->Key.LastOpt!=p->Key.Opt)
	{ 
		p->Key.LastOpt=p->Key.Opt; 
		switch(p->Key.Opt)
		{			  
			case 0x01: Screen_PastePicture(0x0800,35,(47<<16)+180,(256<<16)+262,(47<<16)+180);break;
			case 0x02: Screen_PastePicture(0x0800,35,(295<<16)+180,(505<<16)+262,(295<<16)+180);break;
			case 0x03: Screen_PastePicture(0x0800,35,(553<<16)+180,(763<<16)+262,(553<<16)+180);break;
			case 0x04: Screen_PastePicture(0x0800,35,(75<<16)+387,(204<<16)+424,(75<<16)+387);break;
			case 0x05: Screen_PastePicture(0x0800,35,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;						
			default:break;
		}
	}	  		
//	if(LastChoice!=pf->Data[FLASHIS_AdrCLYQ_CLFS])	
//	{
//		LastChoice=pf->Data[FLASHIS_AdrCLYQ_CLFS];
//		switch(LastChoice)
//		{
//			case 0x00: Screen_PastePicture(0x2260,35,(231<<16)+396,(257<<16)+421,(231<<16)+396);break;
//			case 0x01: Screen_PastePicture(0x2260,35,(332<<16)+396,(358<<16)+421,(332<<16)+396);break;
//			default:break;
//		}
//	}
			

	//���ݸ���
	switch(p->RunModeStat)
	{
		case 0x01: //��̬������ʾ			
		{
			Screen_ShowPage(34); 	
//			Screen_ResetDataBuffer();		
//			sprintf((char*)p->Buffer,"%2d",pf->Data[FLASHIS_AdrCLYQ_JSSC]);
//			Screen_RefreshDataMulti(0x0101,p->Buffer,15*2);	
			
			Screen_ResetDataBuffer();		
			sprintf((char*)p->Buffer,"%2d",pf->Data[FLASHIS_AdrXTSZYQCLSJ]);	//��ƽ��ʱ��
			Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);	
			Screen_ResetDataBuffer();		
			sprintf((char*)p->Buffer,"%2d",pf->Data[FLASHIS_AdrXTSZYQCLCS]);	//��ƽ������
			Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);	
			
			p->RunModeStat=2;				 
		}
		break;
		case 0x02: //��̬̬������ʾ			
		{
			if(p->Timer==0)
			{							 
				if((p->Key.Mode>=4)&&(p->Key.Mode<=5)&&(p->Key.bOutsideKeyboard != false))
				{
					ScreenEnterNumberTwinkle(p,0x0100+(p->Key.Mode-4)*0x20);
				}			 							
				p->Timer=500;
			}			 
		}
		break;
	}
}
//----
/* date 2019-12-26
	��������void Screen_CLYQ_YQJZ_Pag(Screen_DataStructure *p)
	������	��������-����У׼
*/
void Screen_CLYQ_YQJZ_Pag(Screen_DataStructure *p)
{
 	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  Slave_DataStructure*  Sp=Slave_GetData();
	 //��������
	  Button_CLYQ_YQJZ_Pag(2,1,&FileValue);
	  if(p->RunMode!=0x5100) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{  
				case 0x01: Screen_PastePicture(0x0800,37,(595<<16)+375,(707<<16)+440,(595<<16)+375);break;
				case 0x02: Screen_PastePicture(0x0800,37,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;			
				default:break;
			}
		}
		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(36); 
				 p->TimerSecUp=0;
				 Flux_StarSampling('C',1.0);//������			 
				 p->RunModeStat=2;	
				 p->Function=0;	
				 p->TimerSec=0;				 
			 }
			break;
      case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
							{		
								p->TimerSecUp++;								
								Screen_ResetDataBuffer(); //ʱ����Ϣ		
								sprintf((char*)p->Buffer,"%2dM%2dS",p->TimerSecUp/120,p->TimerSecUp/2%60);
								Screen_RefreshDataMulti(0x0420,p->Buffer,15*2);										
								//ʱ�䵹��ʱ
								if(p->TimerSec==0)
								{
									 if(p->Function==1)
									{
									  ScreenBackLastPag(p,0X5000);
										p->Function=0;
										ScreenChangeDataInit(p);
										Flux_StopSampling('C');//�ر�����	
				             
									}
									sprintf((char*)p->Buffer," ");
									Screen_RefreshDataMulti(0x0400,p->Buffer,15*2);	
								}
								else
								{
									sprintf((char*)p->Buffer,"%2dS�����У׼��",p->TimerSec);
									Screen_RefreshDataMulti(0x0400,p->Buffer,15*2);	
								}
								Screen_ResetDataBuffer(); //ʱ�䵹��ʱ
								sprintf((char*)p->Buffer,"%.1f",Flux_GetData()->ChannelC.ShowFlux);//����ֵ
								Screen_RefreshDataMulti(0x0440,p->Buffer,15*2);
								sprintf((char*)p->Buffer,"%.1f %%",Slave_GetData()->GasMotor.Ratio*100/Slave_GetData()->GasMotor.Pid.PWMcycle);//����ֵ, modify by alfred, 2020.07.04
								Screen_RefreshDataMulti(0x0460,p->Buffer,15*2);									
								p->DisValue[0]=Sp->O2Concent.Value;
								p->DisValue[1]=Sp->NOConcent.Value;
								p->DisValue[2]=Sp->NO2Concent.Value;
								p->DisValue[3]=Sp->NOxConcent.Value;
								p->DisValue[4]=Sp->SO2Concent.Value;
								p->DisValue[5]=Sp->COConcent.Value;
								p->DisValue[6]=Sp->H2SConcent.Value;
								p->DisValue[7]=Sp->CO2Concent.Value;
								for(int i=0;i<8;i++)
								{
								if(	p->DisValue[i]==SLAVE_ColFAULT)
								{
								  sprintf((char*)p->Buffer,"------");//����ֵ
								}
								else
								{
									//if(p->DisValue[i]>0)
									{   if((i==0)||(i==7))
										sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);//˲ʱ 
										else
										sprintf((char*)p->Buffer,"%.0f",p->DisValue[i]);//˲ʱ 	
									}
									/*
									else
									{			
										 if((i==0)||(i==7))
										sprintf((char*)p->Buffer,"%.1f",0.0f);	//	ȥ��������ʾ��modify by alfred, 2020.07.15
										 else
										sprintf((char*)p->Buffer,"%.0f",0.0f);	//	ȥ��������ʾ��modify by alfred, 2020.07.15
									}
									*/
								}
								Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);	
								}
								if(p->Function==1)
								{
								    //to do У׼�߼�
									//p->Function=0;
									Slave_Zero_QT();
									
								}
						    p->Timer=500;
						}			 
			 }
			break;
			}
}



/* date 2020.06.05
	��������void Screen_CLYQ_JIPINGJUN_Pag(Screen_DataStructure *p)
	������	��������-��ƽ��
*/
void Screen_CLYQ_JIPINGJUN_Pag(Screen_DataStructure *p)
{
	Flashisp_DataStructure* pf=Flashisp_GetData();
	Slave_DataStructure*  Sp=Slave_GetData();
	

	u8 uDataDisplayBuf[60], uTimeDisplayBuf[10], uTempBuf[8]; //����������ʾ�������飺������ʾ��ʱ����ʾ��������ʱ����
	u8 uDisRow;	//��ʾ������
	float buf[8]={1,Ppmtomgm3_SO2,Ppmtomgm3_NO,Ppmtomgm3_NO2,Ppmtomgm3_CO,1,Ppmtomgm3_H2S,1};
	//��������
	Button_CLYQ_JIPINGJUN_Pag(5,1,&FileValue);
	if(p->RunMode!=0x5230) return;
	//ѡ���ͼ����

	if(p->Key.LastOpt!=p->Key.Opt)
	{ 
		p->Key.LastOpt=p->Key.Opt; 
		
		switch(p->Key.Opt)
		{  
			case 0x01: Screen_PastePicture(0x0800,141,(680<<16)+102,(800<<16)+155,(680<<16)+102);break;
			case 0x02: Screen_PastePicture(0x0800,141,(680<<16)+155,(800<<16)+207,(680<<16)+155);break;
			case 0x03: Screen_PastePicture(0x0800,141,(680<<16)+205,(800<<16)+263,(680<<16)+205);break;
			case 0x04: Screen_PastePicture(0x0800,141,(680<<16)+263,(800<<16)+318,(680<<16)+263);break;
			case 0x05: Screen_PastePicture(0x0800,141,(720<<16)+370,(800<<16)+441,(720<<16)+370);break;			
			default:break;
		}
			
	}		
		//���ݸ���
    switch(p->RunModeStat)
	{  
		case 0x01: //��̬������ʾ			
		{   
			Screen_ShowPage(140); 		 
			p->RunModeStat=2;

			//��ʾ��λ
			if(pf->Data[FLASHIS_AdrCLYQ_DWLX]==1)//��λ�л�
			{
			
				Screen_RefreshDataMulti(0x0120,(u8 *)"ppm    ",10);
			}
			else
			{
			
				Screen_RefreshDataMulti(0x0120,(u8 *)"mg/m^3 ",10);
			}
						
			//����ҳʱ������ʱ������ʱδ��ʱ����ʾ��ǰҳ��
			if(p->JiPingJun.Timer==0)
				p->JiPingJun.uMinDataPage = (p->JiPingJun.cAverageCount-1)/TABLE_ROW;
			
			//�����ʾ��һҳ����һҳ������ҳ�������ʾ����

				p->JiPingJun.ucCurrentPage = p->JiPingJun.uMinDataPage;
				for(int i=0;i<8;i++)
				{
					Screen_RefreshDataMulti(0x0400+0x80*i,(u8 *)"                                                            ",60);
					Screen_RefreshDataMulti(0x0900+0x10*i,(u8 *)"          ",10);
				}

			
			memset(uTempBuf,0,sizeof(uTempBuf));
			sprintf((char*)uTempBuf,"Page: %d",p->JiPingJun.uMinDataPage+1);	//,p->JiPingJun.uMinDataPageΪ���ʱ����ʾ��һҳ
			Dis_Tips(p,uTempBuf,0x0130); //ˢ��ҳ��
			if(p->JiPingJun.cAverageCount>0)
			{
				if(p->JiPingJun.uMinDataPage!= (p->JiPingJun.cAverageCount -1)/TABLE_ROW)	//��������������=����,��������,��ǰ��ʾ��uDisRow = TABLE_ROW-1������
							//ҳ�벻���ڵ�ǰ����ҳ����ǰ��ʾ��uDisRow = TABLE_ROW������,���ڷ�ҳʱ������ʾ
				{
					uDisRow = TABLE_ROW-1;
				}
				else uDisRow = (p->JiPingJun.cAverageCount-1)%TABLE_ROW;	//��ǰ���� = ��ǰ������������%����
								
				for(u8 i=0+p->JiPingJun.uMinDataPage*TABLE_ROW; i<=uDisRow+p->JiPingJun.uMinDataPage*TABLE_ROW;i++)	//ÿҳ��TABLE_ROW��,Ĭ��8��
				{//��ʾ��������
					memset(uDataDisplayBuf,0,sizeof(uDataDisplayBuf));
					memset(uTimeDisplayBuf,0,sizeof(uTimeDisplayBuf));
					
					for(u8 k=0;k<8;k++)	//ȡһ������
					{				
						memset(uTempBuf,0,sizeof(uTempBuf));

						if(gMinuteData[i][k] == SLAVE_ColFAULT) 
						sprintf((char*)uTempBuf,"%-7s","-----");//�������ֵ����������ʾ-----
						else
                        {
                        if(pf->Data[FLASHIS_AdrCLYQ_DWLX]==1)
						{ if((k==0)||(k==5))
							sprintf((char*)uTempBuf,"%-7.1f",gMinuteData[i][k]/buf[k]);	//���ݵ�λ�趨������Ӧϵ����ʾ
						  else
						    sprintf((char*)uTempBuf,"%-7.0f",gMinuteData[i][k]/buf[k]);	//���ݵ�λ�趨������Ӧϵ����ʾ	  
						}
						else
						{ if((k==0)||(k==5))
						  sprintf((char*)uTempBuf,"%-7.1f",gMinuteData[i][k]);
						  else
						  sprintf((char*)uTempBuf,"%-7.0f",gMinuteData[i][k]);  
                        }							
						}
						strcat((char*)uDataDisplayBuf,(char*)uTempBuf);
					}
					Screen_RefreshDataMulti(0x0400+0x80*(i-p->JiPingJun.uMinDataPage*TABLE_ROW),uDataDisplayBuf,60);
					strcpy((char*)uTimeDisplayBuf,(char*)gSystemTime[i]);
					Screen_RefreshDataMulti(0x0900+0x10*(i-p->JiPingJun.uMinDataPage*TABLE_ROW),uTimeDisplayBuf,10);					
				}
			}			
		}
		break;
		case 0x02: //��̬̬������ʾ			
		{
			if(p->Timer==0)
			{							 

				Screen_ResetDataBuffer();
				sprintf((char*)p->Buffer,"%.1f",Flux_GetData()->ChannelC.ShowFlux);//����ֵ
				Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);	
							
				Screen_ResetDataBuffer();
				sprintf((char*)p->Buffer,"%02d:%02d",p->JiPingJun.uCounter1S/60,p->JiPingJun.uCounter1S%60);//ƽ��ʱ����ʾ20200505
				Screen_RefreshDataMulti(0x0110,p->Buffer,15*2);
							
				p->DisValue[0]=Sp->O2Concent.Value;
				p->DisValue[1]=Sp->SO2Concent.Value;
				p->DisValue[2]=Sp->NOConcent.Value;
				p->DisValue[3]=Sp->NO2Concent.Value;
				p->DisValue[4]=Sp->COConcent.Value;
				p->DisValue[5]=Sp->CO2Concent.Value;
				p->DisValue[6]=Sp->H2SConcent.Value;
				p->DisValue[7]=Sp->NOxConcent.Value;
				
				if(pf->Data[FLASHIS_AdrCLYQ_DWLX]==1)//��λ�л�
				{
					for(int i=0;i<8;i++) 
					{
					if(p->DisValue[i]!=SLAVE_ColFAULT)
					 p->DisValue[i]=p->DisValue[i]/buf[i];	//˲ʱֵ
				    }
				}
				for(int i=0;i<8;i++)
				{ 
					Screen_ResetDataBuffer();
					if(p->DisValue[i]==SLAVE_ColFAULT)
					{
						sprintf((char*)p->Buffer,"-----");//����ֵ
					}
					else
					{
						if(p->DisValue[i]>0)
						{if((i==0)||(i==5))
						sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);//˲ʱ 
						else
						sprintf((char*)p->Buffer,"%.0f",p->DisValue[i]);//˲ʱ 	
                        }
						else
                        {if((i==0)||(i==5))
						  sprintf((char*)p->Buffer,"%.1f",0.0f);//˲ʱ 
						  else
						  sprintf((char*)p->Buffer,"%.0f",0.0f);
						}
					}
					Screen_RefreshDataMulti(0x0810+0x10*i,p->Buffer,8*2);
				}
			p->Timer=500;
			}			 
		}
		break;
	}
}


//---
/* date 20200505
	��������void Screen_CLYQ_KSJC_Pag(Screen_DataStructure *p)
	������	��������-��ʼ���
*/
void Screen_CLYQ_KSJC_Pag(Screen_DataStructure *p)
{
	static u8 LastChoiceDw=11,LastChoiceJx=11;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	Slave_DataStructure*  Sp=Slave_GetData();
	 SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure *ps = Screen_GetData();
	//��������
	Button_CLYQ_KSCL_Pag(5,1,&FileValue);	//ȥ������λ���˵���modify by alfred, 2020.07.11
	if(p->RunMode!=0x5200) return;
	//ѡ���ͼ����
	
	if(LastChoiceJx!=p->PauseOrStop)	
	{
		LastChoiceJx=p->PauseOrStop;
		if(p->PauseOrStop==B0) //��ͣ
		{
			Screen_PastePicture(0x26A0,39,(568<<16)+278,(678<<16)+340,(377<<16)+373);	//2020.07.15 ������ʾ����
		}else
		{
			Screen_PastePicture(0x26A0,38,(377<<16)+373,(489<<16)+400,(377<<16)+373);
		}
	 }
	if(p->Key.LastOpt!=p->Key.Opt)
	{ 
		p->Key.LastOpt=p->Key.Opt; 
		
		switch(p->Key.Opt)
		{  
			case 0x01: Screen_PastePicture(0x0800,39,(257<<16)+373,(371<<16)+440,(257<<16)+373);break;
			case 0x02: if(p->PauseOrStop==B0) //��ͣ
			{
				Screen_PastePicture(0x0800,39,(680<<16)+278,(790<<16)+346,(375<<16)+373);break;
			}else
			{
				Screen_PastePicture(0x0800,39,(375<<16)+373,(488<<16)+440,(375<<16)+373);break;
			}

//			case 0x03: Screen_PastePicture(0x0800,39,(379<<16)+375,(489<<16)+440,(379<<16)+375);break;
			case 0x03: Screen_PastePicture(0x0800,39,(490<<16)+373,(600<<16)+440,(490<<16)+373);break;
			case 0x04: Screen_PastePicture(0x0800,39,(605<<16)+373,(715<<16)+440,(605<<16)+373);break;
			case 0x05: Screen_PastePicture(0x0800,39,(718<<16)+373,(787<<16)+440,(718<<16)+373);break;			
			default:break;
		}
			
	}
	if(LastChoiceDw!=pf->Data[FLASHIS_AdrCLYQ_DWLX])	
	{
		LastChoiceDw=pf->Data[FLASHIS_AdrCLYQ_DWLX];
		switch(LastChoiceDw)
		{
			//case 0x00: Screen_PastePicture(0x2660,38,(42<<16)+98,(506<<16)+300,(42<<16)+98);break;
			//case 0x01: Screen_PastePicture(0x2660,39,(42<<16)+98,(506<<16)+300,(42<<16)+98);break;
			case 0x00: Screen_PastePicture(0x2660,38,(42<<16)+75,(772<<16)+270,(42<<16)+75);break;
			case 0x01: Screen_PastePicture(0x2660,39,(42<<16)+75,(772<<16)+270,(42<<16)+75);break;
			default:break;
		}
	}
		
		//���ݸ���
    switch(p->RunModeStat)
	{
		case 0x01: //��̬������ʾ			
		{  FourBytes four;
			u8 buf[80]={0};			
			float buffer[40];
			float gas[8];
			Screen_ShowPage(38); 
			p->TimerSecUp=0;	 
			p->RunModeStat=2;	
			if(Flux_GetData()->ChannelC.RunMode==0)  //
			{
				
				//�ȿ����������׼������,����ϵ��1.0
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_ZSXS]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_ZSXS];
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_ZSXS+1]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_ZSXS+1];
				
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_JZYHL]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_JZYHL];
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_JZYHL+1]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_JZYHL+1];
                four.FloatData=1.0f;
				SaveFlaotToFlash(four.FloatData,FLASHIS_AdrCLYQ_WJCX_JS_FHXS); //��Ĭ�ϸ���ϵ��Ϊ1
				
				Flux_StarSampling('C',1.0);//������		
				Flux_GetData()->ChannelC.RunMode=1;

				mf_copy((u8*)"0:/YLB3330/Gas/link.txt",(u8*)"0:/YLB3330/Gas/temp0.txt",1);	//���������ļ�, add by alfred, 2020.07.06					 
				//�ȱ��濪ʼʱ��
				SdFile_ramlodair((char *)&SdFp->InterFace_data.dat[0],Interf_dat_length);  //�����ո�
				sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);

				//strncpy((char *)&SdFp->InterFace_data.dat[0],(char *)&buf[0],Interf_dat_length); 
				strncpy((char *)&ps->recorstarddat[0],(char *)&buf[0],Interf_dat_length); 

				SdFile_Serch_MaxofGasLinkFile();                                     
				p->FileNumber=SdFile_GetData()->GasLinkFileMax+1;     //��������ļ���
				Flux_GetData()->ChannelC.SaveFileNumber=p->FileNumber;                

				
				gas[0]=Sp->O2Concent.Value;
	            gas[1]=Sp->SO2Concent.Value;
				if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x01))==1)  //
				{
					
				//p->DisValue[4]=p->DisValue[4]-p->DisValue[5]*GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_XTSZ_CO_SO2); //������
                //if(p->DisValue[4]<0)p->DisValue[4]=0;
				 gas[1]=CalculationFormula_GetData()->SO2FromCO;//������
                }

	            gas[2]=Sp->NOConcent.Value;
	            gas[3]=Sp->NO2Concent.Value;
	            gas[4]=Sp->COConcent.Value;
	            gas[5]=Sp->H2SConcent.Value;
	            gas[6]=Sp->CO2Concent.Value;
				SaveGasDataLoad(buffer,gas);
				SaveGasData(buffer,p->FileNumber,1);
				SdFile_WriteGasFile(&SdFile_GetData()->InterFace_data,p->FileNumber,0,1);	//���������ļ����ļ�����	
					 
			}	
			else
			{
				p->FileNumber= Flux_GetData()->ChannelC.SaveFileNumber;
			}					 
			if(Flux_GetData()->ChannelC.RunMode!=3)
			{
				p->PauseOrStop=0;
			}
			else
			{
				 p->PauseOrStop=1;
				 LastChoiceJx=0;
			}
			p->Function=0;	 
			p->AgvTimer=pf->Data[FLASHIS_AdrXTSZYQCLSJ];
			p->AgvTimes=pf->Data[FLASHIS_AdrXTSZYQCLCS]; 				 
			for(int i=0;i<10;i++)
			{
				p->DisValue[i+10]=-1;
			}	
				
			Screen_ResetDataBuffer();
			sprintf((char*)p->Buffer,"%d",p->AgvTimer);//ƽ��ʱ��
			Screen_RefreshDataMulti(0x0420,p->Buffer,15*2);	
					
			Screen_ResetDataBuffer();
			sprintf((char*)p->Buffer,"%d",p->AgvTimes);//ƽ������
			Screen_RefreshDataMulti(0x0460,p->Buffer,15*2);
   
			 }
			break;
      case 0x02: //��̬̬������ʾ			
		{
			if(p->Timer==0)
			{							 

				Screen_ResetDataBuffer();
				sprintf((char*)p->Buffer,"%.1f",Flux_GetData()->ChannelC.ShowFlux);//����ֵ
				Screen_RefreshDataMulti(0x0400,p->Buffer,15*2);	

                Screen_ResetDataBuffer();
				sprintf((char*)p->Buffer,"%02d:%02d",Flux_GetData()->ChannelC.Sampling.SecTimer/60,(Flux_GetData()->ChannelC.Sampling.SecTimer)%60);//ƽ��ʱ����ʾ20200505
				Screen_RefreshDataMulti(0x0440,p->Buffer,15*2);

				p->DisValue[0]=Sp->O2Concent.Value;
				p->DisValue[1]=Sp->NOConcent.Value;
				p->DisValue[2]=Sp->NO2Concent.Value;
				p->DisValue[3]=Sp->NOxConcent.Value;
				
				p->DisValue[4]=Sp->SO2Concent.Value;
				
				p->DisValue[5]=Sp->COConcent.Value;
				
				if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x01))==1)  //
				{
				//p->DisValue[4]=p->DisValue[4]-p->DisValue[5]*GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_XTSZ_CO_SO2); //������
                //if(p->DisValue[4]<0)p->DisValue[4]=0;
				 p->DisValue[4]=CalculationFormula_GetData()->SO2FromCO;//������
					
                }
				
				p->DisValue[6]=Sp->H2SConcent.Value;
				p->DisValue[7]=Sp->CO2Concent.Value;
				if(pf->Data[FLASHIS_AdrCLYQ_DWLX]==1)//��λ�л�
				{
					for(int i=0;i<5;i++) 
					 p->DisValue[1+i]*=0.17f;
				}
				for(int i=0;i<8;i++)
				{ 
					Screen_ResetDataBuffer();
					if(p->DisValue[i]==SLAVE_ColFAULT)
					{
						sprintf((char*)p->Buffer,"------");//����ֵ
					}
					else
					{
						if(p->DisValue[i]>0)
						{
						if((i==0)||(i==7))	
						sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);//˲ʱ 
						else
						sprintf((char*)p->Buffer,"%.0f",p->DisValue[i]);//˲ʱ 	
						}
						else  //�Ѹ�������ʾ��
						{	
						if((i==0)||(i==7))	
						sprintf((char*)p->Buffer,"%.1f",0.0f);//˲ʱ 
						else
						sprintf((char*)p->Buffer,"%.0f",0.0f);
						}
					}
					Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);
					
					Screen_ResetDataBuffer();
					if( (p->DisValue[10+i]==-1)|| (p->DisValue[10+i]==SLAVE_ColFAULT))
					{
						sprintf((char*)p->Buffer,"-------");//����
					}else
					{					
                        if(p->DisValue[i+10]>0)
						{
						if((i==0)||(i==7))	
						sprintf((char*)p->Buffer,"%.1f",p->DisValue[i+10]);//˲ʱ 
						else
						sprintf((char*)p->Buffer,"%.0f",p->DisValue[i+10]);//˲ʱ 	
						}
						else 
						{	
						if((i==0)||(i==7))	
						sprintf((char*)p->Buffer,"%.1f",0.0f);//˲ʱ 
						else
						sprintf((char*)p->Buffer,"%.0f",0.0f);
						}
					}
					Screen_RefreshDataMulti(0x0500+0x20*i,p->Buffer,15*2);	
				}

				
			p->Timer=500;
			}			 
		}
		break;
	}
}
//---
/* date 2019-12-26
	��������void Screen_CLYQ_CLJS_Pag(Screen_DataStructure *p)
	������	��������-��������
*/
void Screen_CLYQ_CLJS_Pag(Screen_DataStructure *p)
{
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//��������
	  Button_CLYQ_CLJS_Pag(3,1,&FileValue);
	  if(p->RunMode!=0x5210) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{  
						case 0x01: Screen_PastePicture(0x0800,41,(270<<16)+240,(390<<16)+320,(270<<16)+240);break;
						case 0x02: Screen_PastePicture(0x0800,41,(420<<16)+240,(550<<16)+320,(420<<16)+240);break;
						case 0x03: Screen_PastePicture(0x0800,41,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;			
						default:break;
			}
		}
		
		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(40); 
				 p->FileNumber=Flux_GetData()->ChannelC.SaveFileNumber;  //ȡ���ļ���
				 p->RunModeStat=2;				 
			 }
			break;
			}
}
//---
//---
/* date 2019-12-26
	��������void Screen_CLYQ_YQAX_Pag(Screen_DataStructure *p)
	������	��������-��������-������ϴ
*/
void Screen_CLYQ_YQAX_Pag(Screen_DataStructure *p)
{
		  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	  Button_CLYQ_YQQX_Pag(1,0,&FileValue);
	  if(p->RunMode!=0x5220) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{  
						case 0x01: Screen_PastePicture(0x0800,43,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;			
				   default:break;

			}
		}
		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
			 Screen_ShowPage(42); 				 
			 p->RunModeStat=2;	
             p->TimerSec=240;//5Min				 
			 }
			break;
			   case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
							{							 
								//ʱ�䵹��ʱ
							sprintf((char*)p->Buffer,"%02d��%02d��",Flux_GetData()->ChannelC.Sampling.DXTime/60,Flux_GetData()->ChannelC.Sampling.DXTime%60);//
                            Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);	
						    p->Timer=500;
						    }			 
			 }
			 break;
			}
}
//---2020-05-22
/* date 2019-12-26
	��������void Screen_CLYQ_YQCX_Pag(Screen_DataStructure *p)
	������	��������-������ѯ
*/
void Screen_CLYQ_YQCX_Pag(Screen_DataStructure *p)
{
		  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	  Button_CLYQ_YQCX_Pag(11,1,&p->FileNumber);
	  if(p->RunMode!=0x5300) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
			p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
				case 0x01: Screen_PastePicture(0x0800,47,(61<<16)+61,(174<<16)+111,(61<<16)+61);break;
				case 0x02: Screen_PastePicture(0x0800,47,(179<<16)+68,(300<<16)+108,(179<<16)+68);break;
				case 0x03: Screen_PastePicture(0x0800,47,(479<<16)+62,(595<<16)+111,(479<<16)+62);break;
				
				case 0x04: Screen_PastePicture(0x0800,47,(655<<16)+56,(798<<16)+107,(655<<16)+56);break;
				case 0x05: Screen_PastePicture(0x0800,47,(655<<16)+108,(798<<16)+159,(655<<16)+108);break;
				
				case 0x06: Screen_PastePicture(0x0800,47,(655<<16)+161,(798<<16)+212,(655<<16)+161);break;
				case 0x07: Screen_PastePicture(0x0800,47,(655<<16)+214,(798<<16)+265,(655<<16)+214);break;
				
				case 0x08: Screen_PastePicture(0x0800,47,(655<<16)+266,(798<<16)+317,(655<<16)+266);break;
				case 0x09: Screen_PastePicture(0x0800,47,(655<<16)+318,(798<<16)+369,(655<<16)+318);break;
				
				case 0x0A: Screen_PastePicture(0x0800,47,(655<<16)+372,(728<<16)+441,(655<<16)+372);break; 
				case 0x0B: Screen_PastePicture(0x0800,47,(727<<16)+371,(798<<16)+440,(727<<16)+371);break;
						
				default:break;
			}
		}
	  		


		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {	
				/*���ݶ�ȡ*/				
				 if(p->StarCun==1){ //��һ�ν������	
					SdFile_Serch_MaxofGasLinkFile();
					p->FileNumber=SdFile_GetData()->GasLinkFileMax;
					p->StarCun=0; 
					 p->Function=0;	
				 }
				 if(p->Function==0)
				 {
					 Screen_ShowPage(46); 
				 ReadGasFirstData(p->FileNumber); //��һҳ����
				 }
				else
				{
					Screen_ShowPage(48);  
					GetGasSecondData(p->FileNumber); //�ڶ�ҳ����					
				}
								 
				p->RunModeStat=2;	
				//��ҳָʾ		 
			 }
			break;
      case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
							{							 
								if((p->Key.Mode==2)&&(p->Key.bOutsideKeyboard != false))
								{
										ScreenEnterNumberTwinkle(p,0x0100);
								}			 							
						  p->Timer=500;
						}			 
			 }
			break;
			}
}
//---
/* date 20200518, 2020-05-22
	��������void Screen_CLYQ_YQCX_JS_Pag(Screen_DataStructure *p)
	������	��������-�ļ���ѯ-����
*/
void Screen_CLYQ_YQCX_JS_Pag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	  Button_CLYQ_YQCX_JS_Pag(4,1,&FileValue);
	  if(p->RunMode!=0x5310) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
					  case 0x01: Screen_PastePicture(0x0800,55,(154<<16)+92,(384<<16)+147,(154<<16)+92);break;
					  case 0x02: Screen_PastePicture(0x0800,55,(154<<16)+166,(384<<16)+221,(154<<16)+166);break;
				      case 0x03: Screen_PastePicture(0x0800,55,(154<<16)+237,(384<<16)+292,(154<<16)+237);break;
				    //  case 0x04: Screen_PastePicture(0x0800,55,(154<<16)+313,(384<<16)+368,(154<<16)+313);break;
					  case 0x04: Screen_PastePicture(0x0800,55,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;			
				      default:break;

			}
		}
		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(54); 	
				 
				 Screen_ResetDataBuffer();
				 sprintf((char*)p->Buffer,"%.2f",p->ZSXS);             //����ϵ��
				 Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);
				 
				 Screen_ResetDataBuffer();
				 sprintf((char*)p->Buffer,"%.2f",p->JZHYL);           //��׼������
				 Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);
				 
				 Screen_ResetDataBuffer();
				 sprintf((char*)p->Buffer,"%.2f",p->FHXS);           //����ϵ��
				 Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);
				 /*
				 for(int i=0;i<3;i++)
				 {	
					sprintf((char*)p->Buffer,"%.2f",GetFlaotdataFormFlash2Word(FLASHIS_AdrCLYQ_WJCX_JS_ZSXS+i*2));
					Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);	
					}					 
				 */
					
				 p->RunModeStat=2;				 
			 }
			break;
      case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
							{							 
								if((p->Key.Mode<=3)&&(p->Key.bOutsideKeyboard != false))
								{
										ScreenEnterNumberTwinkle(p,0x0100+(p->Key.Mode-1)*0x20);
								}			 							
						  p->Timer=500;
						}			 
			 }
			break;
			}
}
//---
/* date 20200518,2020-05-22
	��������void Screen_CLYQ_YQCX_JS_YQSR_Pag(Screen_DataStructure *p)
	������	��������-�ļ���ѯ-����-��������
*/
void Screen_CLYQ_YQCX_JS_YQSR_Pag(Screen_DataStructure *p)
{

	Flashisp_DataStructure* pf=Flashisp_GetData();

	//��������
	Button_CLYQ_YQCX_JS_YQSR_Pag(10,1,p->DisValue);
	if(p->RunMode!=0x5311) return;
	//ѡ���ͼ����
	if(p->Key.LastOpt!=p->Key.Opt)
	{ 
		p->Key.LastOpt=p->Key.Opt; 
		switch(p->Key.Opt)
		{
			case 0x01: Screen_PastePicture(0x0800,57,(53<<16)+74,(145<<16)+116,(53<<16)+74);break;
			case 0x02: Screen_PastePicture(0x0800,57,(53<<16)+117,(145<<16)+159,(53<<16)+117);break;
			case 0x03: Screen_PastePicture(0x0800,57,(53<<16)+160,(145<<16)+202,(53<<16)+160);break;
			case 0x04: Screen_PastePicture(0x0800,57,(53<<16)+203,(145<<16)+245,(53<<16)+203);break;
			case 0x05: Screen_PastePicture(0x0800,57,(53<<16)+246,(145<<16)+288,(53<<16)+246);break;
			case 0x06: Screen_PastePicture(0x0800,57,(53<<16)+290,(145<<16)+332,(53<<16)+290);break;
			case 0x07: Screen_PastePicture(0x0800,57,(53<<16)+333,(145<<16)+375,(53<<16)+333);break;
			case 0x08: Screen_PastePicture(0x0800,57,(247<<16)+375,(583<<16)+440,(247<<16)+375);break;
			case 0x09: Screen_PastePicture(0x0800,57,(593<<16)+375,(705<<16)+440,(593<<16)+375);break;
			case 0x0A: Screen_PastePicture(0x0800,57,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;

			default:break;

		}
	}
	//���ݸ���
	switch(p->RunModeStat)
	{
		case 0x01: //��̬������ʾ			,�ȶ��뵱ǰ����������
		{
			Screen_ShowPage(56); 
			Screen_ResetDataBuffer();
			if(p->LoadFlag==1)
			{
				p->LoadFlag=0;
				GetGasLongDuData(p->FileNumber,&p->DisValue[0]);            

				p->Function=0;
			}						

			for(int i=0;i<7;i++)
			{
				Screen_ResetDataBuffer();	
				if(p->DisValue[i]!=1000000)
				{
			//	if(i==6)				
			//	sprintf((char*)p->Buffer,"%.2f",p->DisValue[i]);
			//	else 
				sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);
				Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);	
				}
				else
				{
			//	if(i==6)				
			//	sprintf((char*)p->Buffer,"0.00");
			//	else 
				sprintf((char*)p->Buffer,"0.0");
				Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);
				}
			}					 
			p->RunModeStat=2;				 
		}
		break;
		case 0x02: //��̬̬������ʾ			
		{
			if(p->Timer==0)
			{							 
				if((p->Key.Mode<=7)&&p->Key.bOutsideKeyboard != false)
				{
					ScreenEnterNumberTwinkle(p,0x0100+(p->Key.Mode-1)*0x20);
				}
				if(p->Function==1)
				{ 
					sprintf((char*)p->Buffer,"����·��:%7d",(u32)p->FileNumber);

					p->Timer=1500;
					p->Function=0;
				}else if(p->Function==2)
				{
					sprintf((char*)p->Buffer,"���Ϊ·��:%7d",(u32)SdFile_GetData()->GasLinkFileMax);
					p->Function=0;
					p->Timer=1500;
				}
				else
				{		
					sprintf((char*)p->Buffer,"");								
					p->Timer=500;
				}
				Screen_RefreshDataMulti(0x01E0,p->Buffer,15*2);
			}			 
		}
		break;
	}
}
//---
/* date 2019-12-26, 2020-05-22
	��������void Screen_CLYQ_YQCX_WJGL_Pag(Screen_DataStructure *p)
	������	��������-�ļ���ѯ-�ļ�����
*/
void Screen_CLYQ_YQCX_WJGL_Pag(Screen_DataStructure *p)
{

	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  static float DisBuffer[2]={1,1};	//modify by alfred, 2020.06.04
	 //��������
	  Button_CLYQ_YQCX_WJGL_Pag(5,1,DisBuffer);
	  if(p->RunMode!=0x5320) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
				case 0x01: Screen_PastePicture(0x0800,59,(157<<16)+146,(375<<16)+215,(157<<16)+146);break;
				case 0x02: Screen_PastePicture(0x0800,59,(157<<16)+245,(375<<16)+314,(157<<16)+245);break;
				case 0x03: Screen_PastePicture(0x0800,59,(480<<16)+375,(590<<16)+440,(480<<16)+375);break;
				case 0x04: Screen_PastePicture(0x0800,59,(595<<16)+375,(710<<16)+440,(595<<16)+375);break;
				case 0x05: Screen_PastePicture(0x0800,59,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;
						
				default:break;
			}
		}
		//���ݸ���
    switch(p->RunModeStat)
	{
		case 0x01: //��̬������ʾ			
		{
			Screen_ShowPage(58); 
			if(p->LoadFlag)
			{
				p->LoadFlag = 0;
				Screen_ResetDataBuffer();		
				sprintf((char*)p->Buffer,"          ");
				Screen_RefreshDataMulti(0x3B20,p->Buffer,15*2);	
				
				//��ʼ����ʼ�ļ���Ϊ1, �����ļ���Ϊ����ļ���, modify by alfred, 2020.07.09
				DisBuffer[0] = 1;
				DisBuffer[1] = SdFile_GetData()->GasLinkFileMax;
			}					 
				 
			for(int i=0;i<2;i++)
			{
				Screen_ResetDataBuffer();		
				sprintf((char*)p->Buffer,"%.0f",DisBuffer[i]);
				Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);	
			}					 
			p->RunModeStat=2;				 
		}
		break;
		case 0x02: //��̬̬������ʾ			
		{
			if(p->Timer==0)
			{							 
				if((p->Key.Mode<=3)&&(p->Key.bOutsideKeyboard != false))
				{
					ScreenEnterNumberTwinkle(p,0x0100+(p->Key.Mode-1)*0x20);
				}
				
				//�����ļ�����ʱ��״̬��ʾ, add by alfred, 2020.07.11
				if(Usb_GetData()->upstatus == 6)
				{
					Dis_Tips(p,(u8*)"��ȡU��ʧ�ܣ����髲�����U��!",0x3B20);					
				}
				else if(Usb_GetData()->upstatus == 1)
				{
					Dis_Tips(p,(u8*)"�����ļ�ʧ��!",0x3B20);					
				}
				else if(Usb_GetData()->upstatus == 4)
				{
					Dis_Tips(p,(u8*)"�����ļ��ɹ�!",0x3B20);					
				}
				else if(Usb_GetData()->upstatus == 5)
				{
					Dis_Tips(p,(u8*)"              ",0x3B20);					
				}	
				
				p->Timer=500;
			}			 
		}
		break;
	}
}
//---
/* date 2019-12-26
	��������void Screen_CLYQ_YQCX_DYXX_Pag(Screen_DataStructure *p)
	������	��������-�ļ���ѯ-��ӡѡ��
*/
void Screen_CLYQ_YQCX_DYXX_Pag(Screen_DataStructure *p)
{

	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  u16 BoxChoseY[8]={83,119,157,192,227,263,300,334};
		u16 BoxChoseX[4]={52,239,421,607};
	  float DisBuffer[2]={1,10};
	 //��������
	  Button_CLYQ_YQCX_DYXX_Pag(33,0,DisBuffer);
	  if(p->RunMode!=0x5330) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
							case 0x01: Screen_PastePicture(0x0800,53,(81<<16)+77,(224<<16)+112,(81<<16)+77);break;
							case 0x02: Screen_PastePicture(0x0800,53,(81<<16)+113,(224<<16)+148,(81<<16)+113);break;
							case 0x03: Screen_PastePicture(0x0800,53,(81<<16)+149,(224<<16)+184,(81<<16)+149);break;
							case 0x04: Screen_PastePicture(0x0800,53,(81<<16)+185,(224<<16)+220,(81<<16)+185);break;
							case 0x05: Screen_PastePicture(0x0800,53,(81<<16)+221,(224<<16)+256,(81<<16)+221);break;
							case 0x06: Screen_PastePicture(0x0800,53,(81<<16)+257,(224<<16)+292,(81<<16)+257);break;
							case 0x07: Screen_PastePicture(0x0800,53,(81<<16)+293,(224<<16)+328,(81<<16)+293);break;
							case 0x08: Screen_PastePicture(0x0800,53,(81<<16)+329,(224<<16)+362,(81<<16)+329);break;
				      
				      case 0x09: Screen_PastePicture(0x0800,53,(267<<16)+77,(402<<16)+112,(267<<16)+77);break;
							case 0x0A: Screen_PastePicture(0x0800,53,(267<<16)+113,(402<<16)+148,(267<<16)+113);break;
							case 0x0B: Screen_PastePicture(0x0800,53,(267<<16)+149,(402<<16)+184,(267<<16)+149);break;
							case 0x0C: Screen_PastePicture(0x0800,53,(267<<16)+185,(402<<16)+220,(267<<16)+185);break;
							case 0x0D: Screen_PastePicture(0x0800,53,(267<<16)+221,(402<<16)+256,(267<<16)+221);break;
							case 0x0E: Screen_PastePicture(0x0800,53,(267<<16)+257,(402<<16)+292,(267<<16)+257);break;
							case 0x0F: Screen_PastePicture(0x0800,53,(267<<16)+293,(402<<16)+328,(267<<16)+293);break;
							case 0x10: Screen_PastePicture(0x0800,53,(267<<16)+329,(402<<16)+362,(267<<16)+329);break;
				
				      case 0x11: Screen_PastePicture(0x0800,53,(450<<16)+77,(591<<16)+112,(450<<16)+77);break;
							case 0x12: Screen_PastePicture(0x0800,53,(450<<16)+113,(591<<16)+148,(450<<16)+113);break;
							case 0x13: Screen_PastePicture(0x0800,53,(450<<16)+149,(591<<16)+184,(450<<16)+149);break;
							case 0x14: Screen_PastePicture(0x0800,53,(450<<16)+185,(591<<16)+220,(450<<16)+185);break;
							case 0x15: Screen_PastePicture(0x0800,53,(450<<16)+221,(591<<16)+256,(450<<16)+221);break;
							case 0x16: Screen_PastePicture(0x0800,53,(450<<16)+257,(591<<16)+292,(450<<16)+257);break;
							case 0x17: Screen_PastePicture(0x0800,53,(450<<16)+293,(591<<16)+328,(450<<16)+293);break;
							case 0x18: Screen_PastePicture(0x0800,53,(450<<16)+329,(591<<16)+362,(450<<16)+329);break;
							
							case 0x19: Screen_PastePicture(0x0800,53,(636<<16)+77,(770<<16)+112,(636<<16)+77);break;
							case 0x1A: Screen_PastePicture(0x0800,53,(636<<16)+113,(770<<16)+148,(636<<16)+113);break;
							case 0x1B: Screen_PastePicture(0x0800,53,(636<<16)+149,(770<<16)+184,(636<<16)+149);break;
							case 0x1C: Screen_PastePicture(0x0800,53,(636<<16)+185,(770<<16)+220,(636<<16)+185);break;
							case 0x1D: Screen_PastePicture(0x0800,53,(636<<16)+221,(770<<16)+256,(636<<16)+221);break;
							case 0x1E: Screen_PastePicture(0x0800,53,(636<<16)+257,(770<<16)+292,(636<<16)+257);break;
							case 0x1F: Screen_PastePicture(0x0800,53,(636<<16)+293,(770<<16)+328,(636<<16)+293);break;
							case 0x20: Screen_PastePicture(0x0800,53,(636<<16)+329,(770<<16)+362,(636<<16)+329);break;

						  case 0x21: Screen_PastePicture(0x0800,53,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;
						
				default:break;

			}
		}
		
		if(p->LastChoice!=pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX]+((pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX+1])<<16))
		{
		  p->LastChoice=pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX]+((pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX+1])<<16);
			u16 addr=0x3500;
			for(int j=0;j<4;j++)
			{
				addr=0x3500+0x0200*j;
				for(int i=0;i<8;i++)
				{
				
					 if((p->LastChoice>>(i+j*8))&0x0001)
						{
							Screen_PastePicture(addr+i*0x20,53,(52<<16)+83,(75<<16)+106,(BoxChoseX[j]<<16)+BoxChoseY[i]);
						}
						else
						{
						 Screen_PastePicture(addr+i*0x20,52,(52<<16)+83,(75<<16)+106,(BoxChoseX[j]<<16)+BoxChoseY[i]);
						}
				}
			}
			
		}
		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
					Screen_ShowPage(52); 
			 
				 p->RunModeStat=2;				 
			 }
			break;
			}
}
//---
/* date 20200516
	��������void Screen_CLYQ_YQCX_FZSJ_Pag(Screen_DataStructure *p)
	������	��������-������ѯ-��������
*/
void Screen_CLYQ_YQCX_FZSJ_Pag(Screen_DataStructure *p)
{

	static int64_t LastChose=0;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//��������
	Button_CLYQ_YQCX_FZSJ_Pag(7,1,&FileValue);
	if(p->RunMode!=0x5340) return;
	//ѡ���ͼ����
	if(p->Key.LastOpt!=p->Key.Opt)
	{ 
		p->Key.LastOpt=p->Key.Opt; 
		switch(p->Key.Opt)
		{
			case 0x01: Screen_PastePicture(0x0800,45,(378<<16)+72,(546<<16)+106,(378<<16)+72);break;
			case 0x02: Screen_PastePicture(0x0800,45,(623<<16)+58,(766<<16)+104,(623<<16)+58);break;
			case 0x03: Screen_PastePicture(0x0800,45,(623<<16)+111,(766<<16)+157,(623<<16)+111);break;
			case 0x04: Screen_PastePicture(0x0800,45,(623<<16)+163,(766<<16)+209,(623<<16)+163);break;
			case 0x05: Screen_PastePicture(0x0800,45,(623<<16)+217,(766<<16)+263,(623<<16)+217);break;
			case 0x06: Screen_PastePicture(0x0800,45,(623<<16)+269,(766<<16)+312,(623<<16)+269);break;
			case 0x07: Screen_PastePicture(0x0800,45,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;

			default:break;

		}
	}
	//���ݸ���
	switch(p->RunModeStat)
	{
		case 0x01: //��̬������ʾ			
		{
			Screen_ShowPage(44); 	
			if(p->LoadFlag==1)
			{
				p->LoadFlag=0;
				p->RecordCun=1;
				p->UseGasCal=0;//����ƽ����־
				SdFile_Serch_MaxofGasRecord(p->FileNumber);
				if(SdFile_GetData()->GasRecordMax>64) SdFile_GetData()->GasRecordMax=64; //һ��������64��ƽ������
			}
			//��ȡ��������
			printf("3p->RecordCun=%d\r\n",p->RecordCun);
			printf("(3p->FileNumber=%d\r\n",(u32)p->FileNumber);
			GetGasFileRecodData(p->FileNumber,p->RecordCun,p->DisValue,1);
			if(p->DisValue[8]==1)
				p->UseGasCal=1;   //ȡ����������־
			else
				p->UseGasCal=0;   //ȡ����������־
				p->RunModeStat=2;				 
		}
		break;
		case 0x02: //��̬������ʾ������ʾ			
		{
			break;
		}				 
			 
	}
	if(LastChose!=p->UseGasCal||p->RecordCun!=p->LastRecordCun)		//��������־��ͼ��ʾ
	{
		LastChose=p->UseGasCal;
		p->LastRecordCun=p->RecordCun;
		//if((LastChose&(1<<(p->RecordCun-1)))==0x00)
		if(LastChose==0)
		{
			Screen_PastePicture(0x2C60,44,(554<<16)+75,(583<<16)+105,(554<<16)+75);
		}
		else
		{
			Screen_PastePicture(0x2C60,45,(554<<16)+75,(583<<16)+105,(554<<16)+75);
		}
	}   
}
//---
/* date 2019-12-27
	��������void Screen_YCCY_CYSZ_Pag(Screen_DataStructure *p)
	������	�̳�����-��������
*/
void Screen_YCCY_CYSZ_Pag(Screen_DataStructure *p)
{
	 
    Slave_DataStructure*  Sp=Slave_GetData();
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  Calculation_FormulaStructure * Cp=CalculationFormula_GetData();
	 //��������
	  Button_YCCY_CYSZ_Pag(9,1,p->DisValue);
		
		
		//�������ݼ���
	  if(p->RunMode!=0x6000) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
					case 0x01: Screen_PastePicture(0x0800,61,(44<<16)+85,(186<<16)+138,(44<<16)+85);break;
				    case 0x02: Screen_PastePicture(0x0800,61,(569<<16)+85,(623<<16)+138,(569<<16)+85);break;
					case 0x03: Screen_PastePicture(0x0800,61,(44<<16)+139,(186<<16)+192,(44<<16)+139);break;
					case 0x04: Screen_PastePicture(0x0800,61,(44<<16)+193,(186<<16)+246,(44<<16)+193);break;
					case 0x05: Screen_PastePicture(0x0800,61,(44<<16)+247,(186<<16)+300,(44<<16)+247);break;
					case 0x06: Screen_PastePicture(0x0800,61,(44<<16)+301,(186<<16)+354,(44<<16)+301);break;
					case 0x07: Screen_PastePicture(0x0800,61,(383<<16)+374,(566<<16)+440,(383<<16)+375);break;
					case 0x08: Screen_PastePicture(0x0800,61,(556<<16)+374,(712<<16)+440,(556<<16)+375);break;
				    case 0x09: Screen_PastePicture(0x0800,61,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;

				default:break;

			}
		}
	  if(p->LastChoice!=pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS])		
		{
		  p->LastChoice=pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS];
			if(p->LastChoice==0)
			{
			 Screen_PastePicture(0x3C60,61,(202<<16)+99,(228<<16)+125,(202<<16)+98);;
			}else
			{
			 Screen_PastePicture(0x3C60,61,(539<<16)+99,(565<<16)+125,(539<<16)+99);;
			}
		}
		//���ݸ���
    switch(p->RunModeStat)
		{
		    case 0x01: //��̬������ʾ			
			 {
				 //��ʾֵ����
			
					p->DisValue[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CYLS);	//װ�ز�������
					p->DisValue[1]=GetU32dataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_LTH);//��Ͳ��
					p->DisValue[2]=pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS];
				    p->DisValue[2]=p->DisValue[2]/60; //��������ʱ��min ���99	     
					p->DisValue[3]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ); //����ֱ��
					p->DisValue[4]=pf->Data[FLASHIS_AdrYDBD_CYDS]; //�ܲ�����
					p->DisValue[5]=(pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS])*p->DisValue[4];//��ʱ��
				 if(pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]==1)
					{	p->DisValue[6]=CalculationFormula_Qrs(GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ),GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_YQLS_YQLS),GetFlaotdataFormFlash2Word(FLASHIS_AdrCHSL_HSL)/100);
						p->DisValue[7]=p->DisValue[0]*(p->DisValue[2])*(p->DisValue[4]); //Ԥ�ƹ������
						p->DisValue[8]=CalculationFormula_Vnd(Cp->Tj,p->DisValue[7],Cp->Ba ,Cp->Pcs);  
					}else
					{
						p->DisValue[6]=p->DisValue[0];
						p->DisValue[7]=p->DisValue[0]*(p->DisValue[2])*(p->DisValue[4]); //Ԥ�ƹ������
						p->DisValue[8]=CalculationFormula_Vnd(Cp->Tj,p->DisValue[7],Cp->Ba ,Cp->Pcs);              //Ԥ�Ʊ�����
					}
				 Screen_ShowPage(60);// Dis_Tips(p,"��Ϣ��ʾ",0x440);
        if(((pf->Data[FLASHIS_AdrXTSZBOX])&(1<<4))==0)	//������������
				{
				  sprintf((char*)p->Buffer,"�̳�����");
				}	else
                {				
					sprintf((char*)p->Buffer,"���̲���");
				}
				Screen_RefreshDataMulti(0x0460,p->Buffer,15*2);	
				Screen_ResetDataBuffer();	
				GetYDBD_Describe(pf->Data[FLASHIS_AdrYDBDLX]);	//�̵�����			
				Screen_RefreshDataMulti(0x0420,p->Buffer,15*2);	
        //������ʾ				
				for(int i=0;i<10;i++)
				 {
					 if(i==1) 	
						 sprintf((char*)p->Buffer,"%07d",(u32)p->DisValue[i]);
					 else if(i==2||i==4) 
						 sprintf((char*)p->Buffer,"%d",(u32)p->DisValue[i]);	
                     else if(i==5)		
						 sprintf((char*)p->Buffer,"%02d:%02d:%02d",(u32)p->DisValue[i]/3600,(u32)p->DisValue[i]%3600/60,(u32)p->DisValue[i]%60);								 
					 else
						sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);
					 
						Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);	
				 }
				sprintf((char*)p->Buffer,"%2d",pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]%60);//����ʱ��S
				Screen_RefreshDataMulti(0x0480,p->Buffer,15*2);
				p->RunModeStat=2;	
				if(p->QimiStat==0)
                Dis_Tips(p,(u8 *)"δ���������",0x0440);
                else if(p->QimiStat==1)
				 Dis_Tips(p,(u8 *)"�����Բ�ͨ��",0x0440);
				else if(p->QimiStat==2)
				 Dis_Tips(p,(u8 *)"������ͨ��  ",0x0440);
					
			 }
			break;
			case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
							{	
									if(p->Key.bOutsideKeyboard != false)
									{
											if(p->Key.Mode==1)
													{
													ScreenEnterNumberTwinkle(p,0x0100);
													}
											else if(p->Key.Mode==3)
													{
													ScreenEnterNumberTwinkle(p,0x0120);
													}			
											else if(p->Key.Mode==4)
													{
													ScreenEnterNumberTwinkle(p,0x0140);   //480
													}		
                                            else if(p->Key.Mode==5)	
											       {
											    	ScreenEnterNumberTwinkle(p,0x0480);	   
											       }			
                                 												   
											else if(p->Key.Mode==6)    //
													{
													ScreenEnterNumberTwinkle(p,0x0160);
											        }
											else if(p->Key.Mode==7)    //�ܲ�����
													{
													ScreenEnterNumberTwinkle(p,0x0180);
											        }
													
									}								
						  p->Timer=500;
						}			 
			 }
			break;
			}
}
//---
/* date 2019-12-25
	��������void Screen_YCCY_QMJC_Pag(Screen_DataStructure *p)
	������	�̳�����-���ܼ��
*/
void Screen_YCCY_QMJC_Pag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	   Calculation_FormulaStructure * Cp=CalculationFormula_GetData();
	 //��������
	  Button_YCCY_QMJC_Pag(2,1,&FileValue);
	  if(p->RunMode!=0x6100) return;
		//ѡ���ͼ����
	 	if(p->LastChoice!=p->PauseOrStop)	
		{
			p->LastChoice=p->PauseOrStop;
		if(p->PauseOrStop==B0) //��ͣ
		 {
			 Screen_PastePicture(0x3E20,63,(194<<16)+375,(360<<16)+440,(545<<16)+375);
		 }else
		 {
		   Screen_PastePicture(0x3E20,62,(545<<16)+375,(712<<16)+440,(545<<16)+375);
		 }
	    }
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
		
				switch(p->Key.Opt)
				{
								case 0x01: if(p->PauseOrStop==B0)
								                      {
													    Screen_PastePicture(0x0800,63,(370<<16)+375,(540<<16)+440,(545<<16)+375);
													  }
													  else
													  {
													  Screen_PastePicture(0x0800,63,(545<<16)+375,(712<<16)+440,(545<<16)+375);
													  }
									         break;
								case 0x02: Screen_PastePicture(0x0800,63,(718<<16)+375,(787<<16)+440,(718<<16)+375);
					                break;
					default:break;
				}
		
		}

	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(62); 	
         p->TimerSecUp=0;				 
				 p->RunModeStat=2;	

				// p->PauseOrStop=0;
         Dis_Tips(p,(u8 *)"",0x0160);//�����ʾ	 
			 }
			break;
      case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
							{	
                               if(p->PauseOrStop==B0)
								{ 
								   p->TimerSecUp++;
								   Dis_Tips(p,(u8 *)"��ʼ�������",0x0160);	
								}
                                p->DisValue[0]=	p->TimerSecUp;							
								p->DisValue[1]= Cp->Pcs;
								p->DisValue[2]= Flux_GetData()->ChannelB.ShowFlux;
								for(int i=0;i<3;i++)
								{
									if(i==0)
									sprintf((char*)p->Buffer,"%d",(u32)p->DisValue[i]);		
									else
									sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);							
									Screen_RefreshDataMulti(0x0100+i*0x20,p->Buffer,15*2);	
								}	
                               if(p->TimerSecUp<60&& p->DisValue[1]<-13.0f) //���ͨ��,��15L����������60���ڴﲻ���趨ֵ������
                               {
								    p->QimiStat=2;  //��������
									p->TimerSecUp=0;
									p->PauseOrStop=0;
						            Flux_StopSampling('B');
									Dis_Tips(p,(u8 *)"���ܼ������",0x0160);	
								}
							   else if(p->TimerSecUp>60)
								{
								   p->QimiStat=1; //©��
								   p->PauseOrStop=0;
								   p->TimerSecUp=0;
						           Flux_StopSampling('B');
								   Dis_Tips(p,(u8 *)"���ܼ��©��",0x0160);	
								}								
						    p->Timer=1000;
								
						}			 
			 }
			break;
			}
}
//---
/* date 2019-12-26
	��������void Screen_YCCY_KSJC_Pag(Screen_DataStructure *p)
	������	�̳�����-��ʼ���
*/
void Screen_YCCY_KSJC_Pag(Screen_DataStructure *p)
{
  	Slave_DataStructure* Sp=Slave_GetData();
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  Calculation_FormulaStructure * Cp=CalculationFormula_GetData();
	 //��������
	  Button_YCCY_KSCL_Pag(4,1,&FileValue);
	  if(p->RunMode!=0x6200) return;
		//ѡ���ͼ����
	if(p->LastChoice!=p->YC_Pause)	
		{
			p->LastChoice=p->YC_Pause;
		if(p->YC_Pause==B0) //��ͣ
		 {
		   Screen_PastePicture(0x40E0,65,(100<<16)+375,(212<<16)+440,(600<<16)+375);
		 }else
		 {
		   Screen_PastePicture(0x40E0,64,(600<<16)+375,(714<<16)+440,(600<<16)+375);
		 }
	    }
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{  
					    case 0x01: Screen_PastePicture(0x0800,65,(331<<16)+375,(479<<16)+440,(331<<16)+375);break;
				      case 0x03: if(p->YC_Pause==B0) //��ͣ
							           {
												 Screen_PastePicture(0x0800,65,(218<<16)+375,(330<<16)+440,(600<<16)+375);break;
												 }else
												 {
												 Screen_PastePicture(0x0800,65,(600<<16)+375,(714<<16)+440,(600<<16)+375);break;
												 }
								         
				      case 0x02: Screen_PastePicture(0x0800,65,(485<<16)+375,(600<<16)+440,(485<<16)+375);break;
						  case 0x04: Screen_PastePicture(0x0800,65,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;			
				default:break;

			}
		}

		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {FourBytes four;
		        Screen_ShowPage(64); 
				//�ȿ����������׼������,����ϵ��1.0
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_ZSXS]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_ZSXS];
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_ZSXS+1]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_ZSXS+1];
				
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_JZYHL]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_JZYHL];
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_JZYHL+1]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_JZYHL+1];
                four.FloatData=1.0f;
				SaveFlaotToFlash(four.FloatData,FLASHIS_AdrCLYQ_WJCX_JS_FHXS); //��Ĭ�ϸ���ϵ��Ϊ1		 
				 
                p->YC_SampleAllTime=0;
                p->YC_SampleTimerSec=pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS];		//���β���ʱ��		 
		        p->RunModeStat=2;
				 
         if(Flux_GetData()->ChannelB.RunMode==0)
		 {   FourBytes four;
			 SdFile_Serch_MaxofDustLinkFile();
			 p->FileNumber=SdFile_GetData()->DustLinkFileMax+1;	 
			 Flux_GetData()->ChannelB.SaveFileNumber=p->FileNumber;  //����̳�����ļ���
			 //���濪ʼ����ʱ��
			 four.U32Data = mktime(&Screen_GetData()->Clock.Clock);
			 Flashisp_GetData()->Data[FLASHIS_AdrCJKSSK] = four.U16Data[0];
			 Flashisp_GetData()->Data[FLASHIS_AdrCJKSSK+1] = four.U16Data[1];

		 }
		 else
		 {
			p->FileNumber= Flux_GetData()->ChannelB.SaveFileNumber; //��ȡ�����е��ļ���
		 }

		 if(Flux_GetData()->ChannelB.RunMode!=3)	
         {			 
         p->YC_Pause=0;	
         Dis_Tips(p,(u8 *)"������",0x0520);	 	
		 }

		 p->YC_Stat=1; //��ʼ������־				 
                    
         if(pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]==1) //����
				{   if((Flux_GetData()->ChannelB.RunMode!=2)&&(Flux_GetData()->ChannelB.RunMode!=3))
					{
					Flux_StarSampling('B',CalculationFormula_Qrs(GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ),GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_YQLS_YQLS),GetFlaotdataFormFlash2Word(FLASHIS_AdrCHSL_HSL)/100)); //����������Ŀ������
					Flux_GetData()->ChannelB.RunMode=1;  //�����߳�
					}
				}
				else
				{   if((Flux_GetData()->ChannelB.RunMode!=2)&&(Flux_GetData()->ChannelB.RunMode!=3))
					{
					Flux_StarSampling('B',GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CYLS)); //Ŀ������
					Flux_GetData()->ChannelB.RunMode=1;  //�����߳�
					}
				}  	
     		
			 }
			break;
      case 0x02: //��̬̬������ʾ			
			 { //  FourBytes four;
				 if(p->Timer==0)
							{   p->Timer=250;//1 S ����һ��
                                
                               	p->DisValue[9]=Flux_DataMiddleFilter(&Flux_GetData()->ChannelB.FluxTowData.Filter,Flux_GetData()->ChannelB.ShowFlux);
								if(p->DisValue[9]<Slave_GetData()->DustMotor.Pid.Sv*0.95f)
				                p->DisValue[9]=Flux_GetData()->ChannelB.ShowFlux;//�̳�����
								
								p->CT++;
								if(p->CT>=2)
								{  p->CT=0;

								u32 cal=Flux_GetData()->ChannelB.Sampling.DinSiTime-Flux_GetData()->ChannelB.Sampling.IntTime;	//20200604					 
								sprintf((char*)p->Buffer,"%02d:%02d", cal/60,cal%60);//����ʱ
								Screen_RefreshDataMulti(0x0501,p->Buffer,15*2);	
								sprintf((char*)p->Buffer,"%2d %2d", Flux_GetData()->ChannelB.Sampling.Cnt,pf->Data[FLASHIS_AdrYDBD_CYDS]);//����ֵ
								Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);	
								//����װ��
								p->DisValue[0]=Sp->DustMotor.Ratio/Sp->DustMotor.Pid.PWMcycle*100; //����	
								p->DisValue[2]=Flux_GetData()->ChannelB.Sampling.SecTimer;
								
								if(pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]==1) //����
								{
									if((cal%4==0)&&(Flux_GetData()->ChannelB.Sampling.IntTime!=0))
									{
								      //p->DisValue[3]=Flux_GetData()->ChannelA.ShowFlux;//Ŀ������
									  p->DisValue[3]=CalculationFormula_Qrs(GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ),CalculationFormula_GetData()->Vs,GetFlaotdataFormFlash2Word(FLASHIS_AdrCHSL_HSL)/100); //����������Ŀ������
									}
									if(Flux_GetData()->ChannelB.Sampling.IntTime==0)
									{
									 p->DisValue[3]=CalculationFormula_Qrs(GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ),GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_YQLS_YQLS),GetFlaotdataFormFlash2Word(FLASHIS_AdrCHSL_HSL)/100);
									}
								}
								else
								{
								  p->DisValue[3]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CYLS);//Ŀ������
								} 
								//p->DisValue[9]=Flux_GetData()->ChannelB.ShowFlux;//��������


								p->DisValue[1]=p->YC_FollowRate;                 //������       								
								
								p->DisValue[5]=Cp->Pd;                           //������ѹ
								p->DisValue[6]=Cp->Ps;                           //������ѹ
								p->DisValue[7]=Cp->Ts;                           //�����¶�
								
								/*
								
								float Pvalue;
								if((pf->Data[FLASHIS_AdrXTSZBOX]&0x40)==0)
								Pvalue=CalculationFormula_p(p->DisValue[7],Cp->Ba,p->DisValue[6]);//���������ܶ�(���£�����ѹ����ѹ��
				                else
								Pvalue=CalculationFormula_p(Cp->Th,Cp->Ba,0.0f);//���������ܶ�(���£�����ѹ����ѹ��
								
								four.U16Data[0]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_PTG_KP];// Ƥ�й�   84/100.0=0.84
	                            four.U16Data[1]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_PTG_KP+1];
				                p->DisValue[4]=CalculationFormula_Vs(four.FloatData,p->DisValue[5],Pvalue);	//�������٣�Ƥ�йܣ���ѹ���ܶȣ�
				                p->DisValue[8]=CalculationFormula_Qs(p->DisValue[4],Cp->F);//��������	
                                */
								
                                p->DisValue[4]=Cp->Vs;  //��������
								p->DisValue[8]=Cp->Qs; //��������

								p->DisValue[10]=Cp->Tj;                          //��ǰ�¶�
								p->DisValue[11]=Cp->Pcs;                         //��ǰѹ��

								p->DisValue[12]=Flux_GetData()->ChannelB.Sampling.SumVolume;  //�������20200604
								p->DisValue[13]=Flux_GetData()->ChannelB.Sampling.RefVolume;  //������20200604
								
								p->DisValue[14]=CalculationFormula_GetData()->Qsnd;//�������
								for(int i=0;i<15;i++)
								{
									if(i==2) 	
									sprintf((char*)p->Buffer,"%02d:%02d:%02d",(u32)p->DisValue[i]/3600,(u32)p->DisValue[i]%3600/60,(u32)p->DisValue[i]%60);//����ֵ
									else if((i==6)||(i==11))
									sprintf((char*)p->Buffer,"%.2f",p->DisValue[i]);//��ѹ����ѹ
									else if((i==5)||(i==0))
									sprintf((char*)p->Buffer,"%.0f",p->DisValue[i]);//��ѹ,����
									else
									sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);//����ֵ
									Screen_RefreshDataMulti(0x0120+0x20*i,p->Buffer,15*2);	
								}				
							 if(p->YC_Pause!=B0)   //20200529 	
					           {
                                  Dis_Tips(p,(u8 *)"������",0x0520);	                              //20200529 								   
							   }
						       
								YC_SampleControl();   
						   }
						}		
               //����Ϊ����ѹ 20200529
          if(p->YC_Pause!=B0)
					{
						 //��ͣ
					 if((Battery_GetData()->LostPV.ActualV<LowPOWER)&&(Power_GetData()->Flg&0x04))
					 {	 
						 p->YC_Pause=B0;
						 Dis_Tips(p,(u8 *)"��������",0x0520);	                              //20200529 
						 Flux_StopSampling('B');
						 Flux_GetData()->ChannelB.RunMode=3;  //��ͣ20200604
						 p->Key.Opt=3;
						 p->Key.LastOpt=0;
					 }
					 
					}

			 }
			break;
			}
}
//---
/* date 2020-01-14
	��������void Screen_CLYQ_CLJS_Pag(Screen_DataStructure *p)
	������	�̳�����-������������
*/
void Screen_YCCY_CLJS_Pag(Screen_DataStructure *p)
{
	Slave_DataStructure* Sp=Slave_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	Calculation_FormulaStructure * Cp=CalculationFormula_GetData();
	//��������
	Button_YCCY_CLJS_Pag(3,1,&FileValue);
	if(p->RunMode!=0x6210) return;
	//ѡ���ͼ����
	if(p->Key.LastOpt!=p->Key.Opt)
	{ 
		p->Key.LastOpt=p->Key.Opt; 
		switch(p->Key.Opt)
		{  
			case 0x01: Screen_PastePicture(0x0800,67,(485<<16)+375,(597<<16)+440,(485<<16)+375);break;
			case 0x02: Screen_PastePicture(0x0800,67,(599<<16)+375,(714<<16)+440,(599<<16)+375);break;
			case 0x03: Screen_PastePicture(0x0800,67,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;			
			default:break;
		}
	}
	//����װ��
	p->FileNumber=Flux_GetData()->ChannelB.SaveFileNumber;
	p->DisValue[0]=p->FileNumber; //�ļ���
	p->DisValue[1]=GetU32dataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_LTH);//��Ͳ��
	p->DisValue[2]=Cp->Vs;                           //��������
	p->DisValue[3]=Cp->Pd;                           //������ѹ
	p->DisValue[4]=Cp->Ps;                           //������ѹ
	p->DisValue[5]=Cp->Ts;                           //�����¶�
	p->DisValue[6]=p->YC_FollowRate;                 //������      ʵ������/Ŀ������   
	p->DisValue[7]=Flux_GetData()->ChannelB.Sampling.SecTimer;//p->YC_SampleAllTime;                           //�ۼƲ�ʱ		
			
	p->DisValue[8]=Flux_GetData()->ChannelB.Sampling.SumVolume;  //�������20200604
	p->DisValue[9]=Flux_GetData()->ChannelB.Sampling.RefVolume;  //������20200604
	p->DisValue[10]=CalculationFormula_GetData()->Qsnd;//�������20200605
	//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				Screen_ShowPage(66); 
				 p->Timer=0;						 
				 p->RunModeStat=2;	
                 break;				 
			 }
			
			 case 0x02: 
			 {
				  if(p->Timer==0)
				  {    p->Timer=500;
				           for(int i=0;i<11;i++)
								{
								if(i==0||i==1) 	sprintf((char*)p->Buffer,"%07d",(u32)p->DisValue[i]);
									else
								  sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);//����ֵ
								 Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);	
					
								}		
				  }								
			     break;
			  }
			}
}
//---
//---
/* date 2020-01-14
	��������void Screen_CLYQ_YQAX_Pag(Screen_DataStructure *p)
	������	�̳�����-��������-�̳�������
*/
void Screen_YCCY_YCFDX_Pag(Screen_DataStructure *p)
{
		 Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	  Button_YCCY_YCFDX_Pag(1,0,&FileValue);
	  if(p->RunMode!=0x6220) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{  
						case 0x01: Screen_PastePicture(0x0800,69,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;			
				   default:break;

			}
		}
		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(68); 				 
				 p->RunModeStat=2;	
             p->TimerSec=10;//5Min	
        // Flux_ContinueSampling('B',10);//������  10L/min  10s				 
			 }
			break;
			   case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
				{	p->DisValue[0]=Flux_GetData()->ChannelB.ShowFlux;						
					sprintf((char*)p->Buffer,"%.1f",p->DisValue[0]);//����ֵ
                    Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);
              	    sprintf((char*)p->Buffer,"%2d",Flux_GetData()->ChannelB.Sampling.DXTime);//����ʱʱ��
                    Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);									
					p->Timer=500;
				}			 
			 }
			 break;
			}
}
//---
/* date 20200521
	��������void Screen_CLYQ_YQCX_Pag(Screen_DataStructure *p)
	������	��������-�̳��ļ���ѯ
*/
void Screen_YCCY_YCCX_Pag(Screen_DataStructure *p)
{

	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	//��������
	Button_YCCY_YCCX_Pag(10,1,&(p->FileNumber));
	if(p->RunMode!=0x7000) return;
	//ѡ���ͼ����
	if(p->Key.LastOpt!=p->Key.Opt)
	{ 
		p->Key.LastOpt=p->Key.Opt; 
		switch(p->Key.Opt)
		{
			//�����˵���ʾλ��, modify by alfred, 2020.07.13
			case 0x01: Screen_PastePicture(0x0800,73,(52<<16)+56,(176<<16)+112,(52<<16)+56);break;
			case 0x02: Screen_PastePicture(0x0800,73,(185<<16)+60,(297<<16)+98,(185<<16)+60);break;	//modify by aflred, 2020.07.14
			case 0x03: Screen_PastePicture(0x0800,73,(478<<16)+55,(591<<16)+109,(478<<16)+55);break;
			case 0x04: Screen_PastePicture(0x0800,73,(666<<16)+59,(799<<16)+103,(666<<16)+59);break;
			case 0x05: Screen_PastePicture(0x0800,73,(666<<16)+112,(799<<16)+156,(666<<16)+112);break;
			case 0x06: Screen_PastePicture(0x0800,73,(666<<16)+165,(799<<16)+209,(666<<16)+165);break;
			case 0x07: Screen_PastePicture(0x0800,73,(666<<16)+218,(799<<16)+262,(666<<16)+218);break;
			case 0x08: Screen_PastePicture(0x0800,73,(666<<16)+270,(799<<16)+314,(666<<16)+270);break;
			case 0x09: Screen_PastePicture(0x0800,73,(666<<16)+323,(799<<16)+372,(666<<16)+323);break;
			case 0x0A: Screen_PastePicture(0x0800,73,(666<<16)+375,(799<<16)+440,(666<<16)+375);break;
			default:break;
		}
	}
	  		
		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {   p->RunModeStat=2;	
				/*���ݶ�ȡ*/				
				 if(p->StarCun==1){ //��һ�ν������
					SdFile_Serch_MaxofDustLinkFile();
					p->FileNumber=SdFile_GetData()->DustLinkFileMax;
					p->StarCun=0; 
					p->Function=0;	
				 }
		
				 if(p->Function==0)                    
				 { 
					
					//���ݶ����������������������ж�Ҫ��ʾ�����̳��ļ����������ļ�20200907
					 Screen_ShowPage(72);   //�̳�ҳ 
					
					 ReadDustData(p->FileNumber);
					 if(p->DisValue[31]==0)
					 {  //��ͼCOPY �̳�

						Screen_PastePicture(0x4120,72,(6<<16)+108,(661<<16)+436,(6<<16)+108);
						ReadDustFirstData(p->FileNumber); //�̳���һҳ����,
						 
					 }
					 else
					 { //��ͼCOPY ����
                        
						Screen_PastePicture(0x4120,112,(6<<16)+108,(661<<16)+436,(6<<16)+108);
					    ReadLampBlackFirstData(p->FileNumber);    //���̵�һҳ
						 
					 }
					
				 }
				 else
				 {
					Screen_ShowPage(74);  
					GetDustSecondData(p->FileNumber); //�ڶ�ҳ����
				 }

			 	break;
			 }
		
		case 0x02: //��̬̬������ʾ			
		{
			 if(p->Timer==0)
			{							 
				if((p->Key.Mode==2)&&p->Key.bOutsideKeyboard != false)
				{
					ScreenEnterNumberTwinkle(p,0x0100);
				}			 							
				p->Timer=500;
			}	
		break;	
		}
		/*
		case 0x03:
		{
			if(p->Timer==0)
			{
					 ReadDustData(p->FileNumber);
					 if(p->DisValue[31]==1)
					 {  //��ͼCOPY �̳�

						Screen_PastePicture(0x4120,112,(6<<16)+108,(661<<16)+436,(6<<16)+108);//4120
						ReadDustFirstData(p->FileNumber); //�̳���һҳ����,
					 }
					 else
					 { //��ͼCOPY ����
						

						 Screen_PastePicture(0x4120,112,(6<<16)+108,(661<<16)+436,(6<<16)+108);
						 ReadLampBlackFirstData(p->FileNumber);                      //���̵�һҳ
					 }
					p->RunModeStat=2;
			}
			break;
		}
		*/
		}
}
//---
/* date 20200519
	��������void Screen_YCCY_YCCX_JS_YC_Pag(Screen_DataStructure *p)
	������	��������-�ļ���ѯ-����
*/
void Screen_YCCY_YCCX_JS_YC_Pag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	  Button_YCCY_YCCX_JS_YC_Pag(9,1,p->DisValue);
	  if(p->RunMode!=0x7100) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
					  case 0x01: Screen_PastePicture(0x0800,81,(41<<16)+72,(209<<16)+118,(41<<16)+72);break;
					  case 0x02: Screen_PastePicture(0x0800,81,(41<<16)+121,(209<<16)+167,(41<<16)+121);break;
				      case 0x03: Screen_PastePicture(0x0800,81,(41<<16)+170,(209<<16)+217,(41<<16)+170);break;
				      case 0x04: Screen_PastePicture(0x0800,81,(41<<16)+219,(209<<16)+266,(41<<16)+219);break;
				      case 0x05: Screen_PastePicture(0x0800,81,(41<<16)+269,(209<<16)+316,(41<<16)+269);break;
				      case 0x06: Screen_PastePicture(0x0800,81,(41<<16)+318,(209<<16)+369,(41<<16)+318);break;
				      case 0x07: Screen_PastePicture(0x0800,81,(476<<16)+375,(589<<16)+440,(476<<16)+375);break;
				      case 0x08: Screen_PastePicture(0x0800,81,(596<<16)+375,(709<<16)+440,(596<<16)+375);break;	
					  case 0x09: Screen_PastePicture(0x0800,81,(710<<16)+375,(790<<16)+440,(710<<16)+375);break;			
				default:break;
			}
		}
	
		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(80);
				 if(p->LoadFlag==1)
				 {
					p->LoadFlag=0;
					Dis_Tips(p,(u8 *)"        ",0x01C0);
				 }

				 sprintf((char*)p->Buffer,"%.5f",p->DisValue[19]);  //�̳�����[19]
				 Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);
				 
				 sprintf((char*)p->Buffer,"%.1f",p->DisValue[20]); //������[20]
				 Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);
				 
				 sprintf((char*)p->Buffer,"%.2f",p->DisValue[10]); //����ϵ��[10]
				 Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);
				 
				 sprintf((char*)p->Buffer,"%.2f",p->DisValue[9]); //����ϵ��[9]
				 Screen_RefreshDataMulti(0x0160,p->Buffer,15*2);
				 
				 sprintf((char*)p->Buffer,"%.1f",p->DisValue[35]); //��׼������[35]
				 Screen_RefreshDataMulti(0x0180,p->Buffer,15*2);
				 
				 sprintf((char*)p->Buffer,"%07d",(u32)p->DRWJH);
				 Screen_RefreshDataMulti(0x01A0,p->Buffer,15*2);			
				 p->RunModeStat=2;				 
			 }
			break;
      case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
							{							 
								if((p->Key.Mode<=6)&&p->Key.bOutsideKeyboard != false)
								{
										ScreenEnterNumberTwinkle(p,0x0100+(p->Key.Mode-1)*0x20);
								}
						  p->Timer=500;
						}			 
			 }
			break;
			}
}
//---
/* date 20200519, 2020-05-23
	��������void Screen_YCCY_YCCX_JS_YY_Pag(Screen_DataStructure *p)
������	��������-�ļ���ѯ-����-����
*/
void Screen_YCCY_YCCX_JS_YY_Pag(Screen_DataStructure *p)
{
		
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	  Button_YCCY_YCCX_JS_YY_Pag(4,1,p->DisValue);
	  if(p->RunMode!=0x7400) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
					  case 0x01: Screen_PastePicture(0x0800,85,(178<<16)+105,(620<<16)+150,(178<<16)+105);break;
					  case 0x02: Screen_PastePicture(0x0800,85,(178<<16)+206,(620<<16)+256,(178<<16)+206);break;
				      case 0x03: Screen_PastePicture(0x0800,85,(178<<16)+290,(620<<16)+340,(178<<16)+290);break;
				      case 0x04: Screen_PastePicture(0x0800,85,(710<<16)+375,(790<<16)+440,(710<<16)+375);break;			
				      default:break;

			}
		}
	
		//���ݸ���
    switch(p->RunModeStat)
		{
		  case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(84);
	             /*
				                   //��ҺŨ��[32]
					               //�������[33]
					               //��ͷ��Ŀ[34]
				 
				*/
                 //Dis_Tips(p,(u8 *)"��ʾ��Ϣ��---",0x01C0);	
				 
				 for(int i=0;i<3;i++)
				 {Screen_ResetDataBuffer();
                     if(i==2)					 
					  sprintf((char*)p->Buffer,"%d",(u32)p->DisValue[32+i]);
				     else
					  {
					  sprintf((char*)p->Buffer,"%.1f",p->DisValue[32+i]);
					  }
					 Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);	
				}					 
				 p->RunModeStat=2;				 
			 }
			break;
      case 0x02: //��̬̬������ʾ			
			 {
				if(p->Timer==0)
				{							 
					if((p->Key.Mode<=3)&&p->Key.bOutsideKeyboard != false)
					{
						ScreenEnterNumberTwinkle(p,0x0100+(p->Key.Mode-1)*0x20);
					}			 							
					p->Timer=500;
				}			 
			 }
			break;
			}
}
//---
/* date 2019-12-26, 2020-05-23
	��������void Screen_YCCY_YCCX_WJGL_Pag(Screen_DataStructure *p)
	������	�̳�����-�ļ���ѯ-�ļ�����
*/
void Screen_YCCY_YCCX_WJGL_Pag(Screen_DataStructure *p)
{
	Flashisp_DataStructure* pf=Flashisp_GetData();
	static float DisBuffer[2]={1,1};	//modify by alfred, 2020.06.04
	//��������
	Button_YCCY_YCCX_WJGL_Pag(5,1,DisBuffer);
	if(p->RunMode!=0x7200) return;
	//ѡ���ͼ����
	if(p->Key.LastOpt!=p->Key.Opt)
	{ 
		p->Key.LastOpt=p->Key.Opt; 
		switch(p->Key.Opt)
		{
			case 0x01: Screen_PastePicture(0x0800,83,(157<<16)+146,(375<<16)+215,(157<<16)+146);break;
			case 0x02: Screen_PastePicture(0x0800,83,(157<<16)+245,(375<<16)+314,(157<<16)+245);break;
			case 0x03: Screen_PastePicture(0x0800,83,(475<<16)+375,(590<<16)+440,(475<<16)+375);break;
			case 0x04: Screen_PastePicture(0x0800,83,(595<<16)+375,(710<<16)+440,(595<<16)+375);break;
			case 0x05: Screen_PastePicture(0x0800,83,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;
			default:break;
		}
	}
	//���ݸ���
	switch(p->RunModeStat)
	{
		case 0x01: //��̬������ʾ			
		{
			Screen_ShowPage(82); 	//������ʾҳ��, modify by alfred, 2020.06.04
			if(p->LoadFlag)
			{			
				p->LoadFlag = 0;				
				Screen_ResetDataBuffer();		
				sprintf((char*)p->Buffer,"          ");
				Screen_RefreshDataMulti(0x3B20,p->Buffer,15*2);		
				
				
				//��ʼ����ʼ�ļ���Ϊ1, �����ļ���Ϊ����ļ���, modify by alfred, 2020.07.09
				DisBuffer[0] = 1;
				DisBuffer[1] = SdFile_GetData()->DustLinkFileMax;
			}	 
			for(int i=0;i<2;i++)
			{
				Screen_ResetDataBuffer();		
				sprintf((char*)p->Buffer,"%.0f",DisBuffer[i]);
				Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);	
			}					 
			p->RunModeStat=2;				 
		}
		break;
		case 0x02: //��̬̬������ʾ			
		{
			if(p->Timer==0)
			{							 
				if((p->Key.Mode<=3)&&p->Key.bOutsideKeyboard != false)
				{
					ScreenEnterNumberTwinkle(p,0x0100+(p->Key.Mode-1)*0x20);
				}
				if(Usb_GetData()->upstatus == 6)
				{
					Dis_Tips(p,(u8*)"��ȡU��ʧ�ܣ����髲�����U��!",0x3B20);					
				}
				else if(Usb_GetData()->upstatus == 1)
				{
					Dis_Tips(p,(u8*)"�����ļ�ʧ��!",0x3B20);					
				}
				else if(Usb_GetData()->upstatus == 4)
				{
					Dis_Tips(p,(u8*)"�����ļ��ɹ�!",0x3B20);					
				}
				else if(Usb_GetData()->upstatus == 5)
				{
					Dis_Tips(p,(u8*)"              ",0x3B20);					
				}				
				p->Timer=500;
			}
			
			
		}
		break;
	}
}
//---
/* date 2020-01-15
	��������void Screen_YCCY_YCCX_DYXX_Pag(Screen_DataStructure *p)
	������	�̳�����-�ļ���ѯ-��ӡѡ��
*/
void Screen_YCCY_YCCX_DYXX_Pag(Screen_DataStructure *p)
{
    u16 BoxChoseY[8]={90,128,164,201,234,270,307,341};
		u16 BoxChoseX[4]={52,239,421,607};
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  static u32 LastChose=0x00;
	 //��������
	  Button_YCCY_YCCX_DYXX_Pag(34,0,p->DisValue);
	  if(p->RunMode!=0x7300) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
							case 0x01: Screen_PastePicture(0x0800,79,(83<<16)+49,(224<<16)+83,(83<<16)+49);break;
							case 0x02: Screen_PastePicture(0x0800,79,(83<<16)+85,(224<<16)+119,(83<<16)+85);break;
							case 0x03: Screen_PastePicture(0x0800,79,(83<<16)+121,(224<<16)+156,(83<<16)+121);break;
							case 0x04: Screen_PastePicture(0x0800,79,(83<<16)+158,(224<<16)+192,(83<<16)+158);break;
							case 0x05: Screen_PastePicture(0x0800,79,(83<<16)+194,(224<<16)+228,(83<<16)+194);break;
							case 0x06: Screen_PastePicture(0x0800,79,(83<<16)+230,(224<<16)+264,(83<<16)+230);break;
							case 0x07: Screen_PastePicture(0x0800,79,(83<<16)+266,(224<<16)+300,(83<<16)+266);break;
							case 0x08: Screen_PastePicture(0x0800,79,(83<<16)+300,(224<<16)+334,(83<<16)+300);break;
				      case 0x09: Screen_PastePicture(0x0800,79,(83<<16)+336,(224<<16)+370,(83<<16)+336);break;
				      
				      case 0x0A: Screen_PastePicture(0x0800,79,(269<<16)+85,(401<<16)+119,(269<<16)+85);break;
							case 0x0B: Screen_PastePicture(0x0800,79,(269<<16)+121,(401<<16)+156,(269<<16)+121);break;
							case 0x0C: Screen_PastePicture(0x0800,79,(269<<16)+158,(401<<16)+192,(269<<16)+158);break;
							case 0x0D: Screen_PastePicture(0x0800,79,(269<<16)+194,(401<<16)+228,(269<<16)+194);break;
							case 0x0E: Screen_PastePicture(0x0800,79,(269<<16)+230,(401<<16)+264,(269<<16)+230);break;
							case 0x0F: Screen_PastePicture(0x0800,79,(269<<16)+266,(401<<16)+300,(269<<16)+266);break;
							case 0x10: Screen_PastePicture(0x0800,79,(269<<16)+300,(401<<16)+334,(269<<16)+300);break;
				      case 0x11: Screen_PastePicture(0x0800,79,(269<<16)+336,(401<<16)+370,(269<<16)+336);break;
				
				      case 0x12: Screen_PastePicture(0x0800,79,(452<<16)+85,(590<<16)+119,(452<<16)+85);break;
							case 0x13: Screen_PastePicture(0x0800,79,(452<<16)+121,(590<<16)+156,(452<<16)+121);break;
							case 0x14: Screen_PastePicture(0x0800,79,(452<<16)+158,(590<<16)+192,(452<<16)+158);break;
							case 0x15: Screen_PastePicture(0x0800,79,(452<<16)+194,(590<<16)+228,(452<<16)+194);break;
							case 0x16: Screen_PastePicture(0x0800,79,(452<<16)+230,(590<<16)+264,(452<<16)+230);break;
							case 0x17: Screen_PastePicture(0x0800,79,(452<<16)+266,(590<<16)+300,(452<<16)+266);break;
							case 0x18: Screen_PastePicture(0x0800,79,(452<<16)+300,(590<<16)+334,(452<<16)+300);break;
				      case 0x19: Screen_PastePicture(0x0800,79,(452<<16)+336,(590<<16)+370,(452<<16)+336);break;
							
							case 0x1A: Screen_PastePicture(0x0800,79,(633<<16)+85,(771<<16)+119,(633<<16)+85);break;
							case 0x1B: Screen_PastePicture(0x0800,79,(633<<16)+121,(771<<16)+156,(633<<16)+121);break;
							case 0x1C: Screen_PastePicture(0x0800,79,(633<<16)+158,(771<<16)+192,(633<<16)+158);break;
							case 0x1D: Screen_PastePicture(0x0800,79,(633<<16)+194,(771<<16)+228,(633<<16)+194);break;
							case 0x1E: Screen_PastePicture(0x0800,79,(633<<16)+230,(771<<16)+264,(633<<16)+230);break;
							case 0x1F: Screen_PastePicture(0x0800,79,(633<<16)+266,(771<<16)+300,(633<<16)+266);break;
							case 0x20: Screen_PastePicture(0x0800,79,(633<<16)+300,(771<<16)+334,(633<<16)+300);break;
				      case 0x21: Screen_PastePicture(0x0800,79,(633<<16)+336,(771<<16)+370,(633<<16)+336);break;

						  case 0x22: Screen_PastePicture(0x0800,79,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;
						
				default:break;

			}
		}
		
		if(LastChose!=(pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+1]+((pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+2])<<16)))
		{
		  LastChose=pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+1]+((pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+2])<<16);
			u16 addr=0x3500;
			for(int j=0;j<4;j++)
			{
				addr=0x4F00+0x0200*j;
				for(int i=0;i<8;i++)
				{
				
					 if((LastChose>>(i+j*8))&0x0001)
						{
							Screen_PastePicture(addr+i*0x20,53,(52<<16)+83,(75<<16)+106,(BoxChoseX[j]<<16)+BoxChoseY[i]);
						}
						else
						{
						 Screen_PastePicture(addr+i*0x20,52,(52<<16)+83,(75<<16)+106,(BoxChoseX[j]<<16)+BoxChoseY[i]);
						}
				}
			}
			
		}
		 if(pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX]==1)
						{
							Screen_PastePicture(0x4ee0,53,(52<<16)+83,(75<<16)+106,(54<<16)+58);
						}
						else
						{
						 Screen_PastePicture(0x4ee0,52,(52<<16)+83,(75<<16)+106,(54<<16)+58);
						}
		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
					Screen_ShowPage(78); 
			 
				 p->RunModeStat=2;				 
			 }
			break;
			}
}
//---
/* date 2020-1-15
	��������void Screen_JZWH_Pag(Screen_DataStructure *p)
	������	У׼ά��-��������-�����߼�����
*/
void Screen_JZWH_MMSR_Pag(Screen_DataStructure *p)
{
	//	FourBytes four;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//��������
	Button_WHJZ_MMSR_Pag(2,1,p->DisValue);
	if(p->RunMode!=0x8000) return;
	//ѡ���ͼ����
	if(p->Key.LastOpt!=p->Key.Opt)
	{ 
		p->Key.LastOpt=p->Key.Opt; 
		switch(p->Key.Opt)
		{
			case 0x01: Screen_PastePicture(0x0800,87,(236<<16)+196,(363<<16)+250,(236<<16)+196);break;
			case 0x02: Screen_PastePicture(0x0800,87,(711<<16)+375,(795<<16)+441,(711<<16)+375);break;
			default:break;
		}
	}
	 //���ݸ���
	switch(p->RunModeStat)
	{
		case 0x01: //��̬������ʾ			
		{
			Screen_ShowPage(86); 
			sprintf((char*)p->Buffer,"         ");
			Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);				 
			p->RunModeStat=2;
		}
		break;
		case 0x02: //��̬������ʾ
		{			
			if(p->Timer==0)
			{	
				if(p->Warning==1)
				{
					Dis_Tips(p,(u8 *)"�����������",0x0120);			
					p->Timer=2000;
					p->Warning=0;
				}
				else if(p->Warning==2)
				{
					Dis_Tips(p,(u8 *)"�ٴ���������",0x0120);			
					p->Timer=2000;
					p->Warning=5;
				}
				else
				{
					Dis_Tips(p,(u8 *)"            ",0x0120);											
					if(p->Key.Mode==1)
					{
						if(p->Key.bOutsideKeyboard != false)									
							ScreenEnterNumberTwinkle(p,0x0100+(p->Key.Mode-1)*0x20);
					}			 							
					p->Timer=500;
				}
			}	
		}
		break;
	}	
}

/* date 2020-1-15
	��������void Screen_JZWH_WHXZ_Pag(Screen_DataStructure *p)
	������	У׼ά��-ά��ѡ��-�����߼�����
*/
void Screen_JZWH_WHXZ_Pag(Screen_DataStructure *p)
{
//	FourBytes four;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
		  Button_WHJZ_WHXZ_Pag(9,1,p->DisValue);
	  if(p->RunMode!=0x8100) return;
			//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
			  
					    case 0x01: Screen_PastePicture(0x0800,89,(45<<16)+97,(257<<16)+179,(45<<16)+97);break;
						  case 0x02: Screen_PastePicture(0x0800,89,(295<<16)+97,(506<<16)+179,(295<<16)+97);break;
						  case 0x03: Screen_PastePicture(0x0800,89,(554<<16)+97,(765<<16)+179,(554<<16)+97);break;
				      case 0x04: Screen_PastePicture(0x0800,89,(45<<16)+198,(257<<16)+280,(45<<16)+198);break;
							case 0x05: Screen_PastePicture(0x0800,89,(295<<16)+198,(506<<16)+280,(295<<16)+198);break;
							case 0x06: Screen_PastePicture(0x0800,89,(554<<16)+198,(765<<16)+280,(554<<16)+198);break;
							case 0x07: Screen_PastePicture(0x0800,89,(45<<16)+293,(257<<16)+385,(45<<16)+293);break;
							case 0x08: Screen_PastePicture(0x0800,89,(295<<16)+293,(506<<16)+385,(295<<16)+293);break;
						  case 0x09: Screen_PastePicture(0x0800,89,(711<<16)+375,(795<<16)+441,(711<<16)+375);break;
						
				default:break;

			}
		}
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
			 Screen_ShowPage(88); 	
       p->RunModeStat=2;				 
			 }

			break;
		}
	
}
//---
/* date 2020-1-16
	��������void Screen_WHJZ_WHXZ_STSZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-ϵͳ����
*/
void Screen_WHJZ_WHXZ_STSZ_Pag(Screen_DataStructure *p)
{

	  static u8 LastChoice=11;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	  Button_WHJZ_WHXZ_STSZ_Pag(13,1,p->DisValue);
	  if(p->RunMode!=0x8110) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
			  
					    case 0x01: Screen_PastePicture(0x0800,91,(55<<16)+61,(237<<16)+106,(55<<16)+61);break;
						  case 0x02: Screen_PastePicture(0x0800,91,(55<<16)+108,(237<<16)+153,(55<<16)+108);break;
				      case 0x03: Screen_PastePicture(0x0800,91,(55<<16)+155,(237<<16)+198,(55<<16)+155);break;
				      case 0x04: Screen_PastePicture(0x0800,91,(55<<16)+199,(237<<16)+244,(55<<16)+199);break;
				      case 0x05: Screen_PastePicture(0x0800,91,(55<<16)+245,(237<<16)+290,(55<<16)+245);break;
				      case 0x06: Screen_PastePicture(0x0800,91,(55<<16)+291,(237<<16)+336,(55<<16)+291);break;
				      case 0x07: Screen_PastePicture(0x0800,91,(55<<16)+337,(237<<16)+382,(55<<16)+337);break;
				      case 0x08: Screen_PastePicture(0x0800,91,(603<<16)+58,(770<<16)+103,(603<<16)+58);break;
						  case 0x09: Screen_PastePicture(0x0800,91,(603<<16)+111,(770<<16)+156,(603<<16)+111);break;
						  case 0x0a: Screen_PastePicture(0x0800,91,(603<<16)+164,(770<<16)+209,(603<<16)+164);break;
						  case 0x0b: Screen_PastePicture(0x0800,91,(603<<16)+217,(770<<16)+262,(603<<16)+217);break;
						  case 0x0c: Screen_PastePicture(0x0800,91,(603<<16)+265,(770<<16)+318,(603<<16)+265);break;
						  case 0x0d: Screen_PastePicture(0x0800,91,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;
						
				default:break;

			}
		}
	  		
		if(LastChoice!=pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ])	
		{
			LastChoice=pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ];
				switch(LastChoice&0x01)
			{
					case 0x00: Screen_PastePicture(0x5A60,90,(515<<16)+115,(544<<16)+143,(515<<16)+115);break;  //20200525
					case 0x01: Screen_PastePicture(0x5A60,91,(515<<16)+115,(544<<16)+143,(515<<16)+115);break;//20200525
					default:break;
			}
			switch(LastChoice&0x02)
			{
					case 0x00: Screen_PastePicture(0x5A80,90,(515<<16)+162,(544<<16)+190,(515<<16)+162);break;//20200525
					case 0x02: Screen_PastePicture(0x5A80,91,(515<<16)+162,(544<<16)+190,(515<<16)+162);break;//20200525
					default:break;
			}
		}
		for(int i=0;i<7;i++)
		{		
				p->DisValue[i]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_XTSZ_NO2_NOX+i*2);
		}
	

		//���ݸ���
    switch(p->RunModeStat)
	{
	case 0x01: //��̬������ʾ			
	{
		Screen_ShowPage(90); 		
		sprintf((char*)p->Buffer,"");
		Screen_RefreshDataMulti(0x01E0,p->Buffer,15*2);	
		for(int i=0;i<7;i++)
		{
			if((i==0)||(i==1))
			sprintf((char*)p->Buffer,"%.0f%%",p->DisValue[i]);
			else
			{
			if(i==2) 
			sprintf((char*)p->Buffer,"-%.2f",p->DisValue[i]);
			else					
			sprintf((char*)p->Buffer,"%.2f",p->DisValue[i]);
		    }
			Screen_RefreshDataMulti(0x0100+i*0x20,p->Buffer,15*2);
		}				 
		p->RunModeStat=2;				 
	}
	break;
	case 0x02: //��̬̬������ʾ			
	{
		if(p->Timer==0)
		{	
			if(p->gSysConfigWaringBits.m_byData!=0)
			{
				if(p->gSysConfigWaringBits.BitField.m_uDustFile == 1)
				{
					Dis_Tips(p,(u8*)"�̳��ļ�ɾ���ɹ�!",0x01E0);		
				}
				else if(p->gSysConfigWaringBits.BitField.m_uGasFile == 1)
				{
					Dis_Tips(p,(u8*)"�����ļ�ɾ���ɹ�!",0x01E0);
				}
				else if(p->gSysConfigWaringBits.BitField.m_uWorkFile == 1)
				{
					Dis_Tips(p,(u8*)"�����ļ�ɾ���ɹ�!",0x01E0);
				}
				else if(p->gSysConfigWaringBits.BitField.m_uAllFile == 1)
				{
					Dis_Tips(p,(u8*)"ȫ���ļ�ɾ���ɹ���",0x01E0);
				}
				else if(p->gSysConfigWaringBits.BitField.m_data0 == 1)
				{
					Dis_Tips(p,(u8*)"�ļ�ɾ��ʧ�ܣ�",0x01E0);
				}
				else if(p->gSysConfigWaringBits.BitField.m_factorySet == 1)
				{
					Dis_Tips(p,(u8*)"ϵͳ�ѻָ��������ã�",0x01E0);
				}				
				p->gSysConfigWaringBits.m_byData = 0x00;	
				p->Timer=2000;									
			}
			else
			{
				Dis_Tips(p,(u8*)" ",0x01E0);	
				if(p->Key.Mode!=0)
				{
					if(p->Key.bOutsideKeyboard != false)
					ScreenEnterNumberTwinkle(p,p->Key.Mode*0x20+0x00e0);
				}			 							
				p->Timer=500;
			 }		

		}			 
	}
		break;
	}
}
//----
/* date 2020-05-17
	��������void Screen_WHJZ_WHXZ_STSZ_CO_SO2_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-ϵͳ����-CO��SO2����
*/
void Screen_WHJZ_WHXZ_STSZ_CO_SO2_Pag(Screen_DataStructure *p)
{

	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 Slave_DataStructure* Sp=Slave_GetData();   //20200525
	 //��������
	  Button_WHJZ_WHXZ_STSZ_CO_SO2_Pag(23,0,p->DisValue);
	  if(p->RunMode!=0x8111) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
			  
//					    case 0x01: if(pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&0x004)//��λ�л�
//								          {
//														 Screen_PastePicture(0x0800,115,(37<<16)+56,(180<<16)+82,(37<<16)+56);
//													}else
//													{
//								             Screen_PastePicture(0x0800,115,(426<<16)+56,(575<<16)+82,(37<<16)+56);
//													}break;
				      case 0x01:Screen_PastePicture(0x0800,115,(37<<16)+56,(180<<16)+82,(37<<16)+56);break;
					  case 0x02: Screen_PastePicture(0x0800,115,(204<<16)+56,(318<<16)+82,(204<<16)+56);break;
													
				      case 0x03: Screen_PastePicture(0x0800,115,(52<<16)+85,(92<<16)+120,(52<<16)+85);break;
				      case 0x04: Screen_PastePicture(0x0800,115,(52<<16)+120,(92<<16)+155,(52<<16)+120);break;
				      case 0x05: Screen_PastePicture(0x0800,115,(52<<16)+155,(92<<16)+192,(52<<16)+155);break;
				      case 0x06: Screen_PastePicture(0x0800,115,(52<<16)+192,(92<<16)+228,(52<<16)+192);break;
				      case 0x07: Screen_PastePicture(0x0800,115,(52<<16)+228,(92<<16)+265,(52<<16)+228);break;
				      case 0x08: Screen_PastePicture(0x0800,115,(52<<16)+265,(92<<16)+300,(52<<16)+265);break;
					  case 0x09: Screen_PastePicture(0x0800,115,(52<<16)+300,(92<<16)+336,(52<<16)+300);break;
					  case 0x0a: Screen_PastePicture(0x0800,115,(52<<16)+336,(92<<16)+373,(52<<16)+336);break;
					  case 0x0b: Screen_PastePicture(0x0800,115,(52<<16)+373,(92<<16)+409,(52<<16)+373);break;
					  case 0x0c: Screen_PastePicture(0x0800,115,(52<<16)+409,(92<<16)+445,(52<<16)+409);break;
													
					  case 0x0d: Screen_PastePicture(0x0800,115,(194<<16)+85,(334<<16)+120,(194<<16)+85);break;
				      case 0x0e: Screen_PastePicture(0x0800,115,(194<<16)+120,(334<<16)+155,(194<<16)+120);break;
				      case 0x0f: Screen_PastePicture(0x0800,115,(194<<16)+155,(334<<16)+192,(194<<16)+155);break;
				      case 0x10: Screen_PastePicture(0x0800,115,(194<<16)+192,(334<<16)+228,(194<<16)+192);break;
				      case 0x11: Screen_PastePicture(0x0800,115,(194<<16)+228,(334<<16)+265,(194<<16)+228);break;
				      case 0x12: Screen_PastePicture(0x0800,115,(194<<16)+265,(334<<16)+300,(194<<16)+265);break;
					  case 0x13: Screen_PastePicture(0x0800,115,(194<<16)+300,(334<<16)+336,(194<<16)+300);break;
					  case 0x14: Screen_PastePicture(0x0800,115,(194<<16)+336,(334<<16)+373,(194<<16)+336);break;
					  case 0x15: Screen_PastePicture(0x0800,115,(194<<16)+373,(334<<16)+409,(194<<16)+373);break;
					  case 0x16: Screen_PastePicture(0x0800,115,(194<<16)+409,(334<<16)+445,(194<<16)+409);break;
						  
					  case 0x17: Screen_PastePicture(0x0800,115,(690<<16)+380,(787<<16)+450,(690<<16)+380);break;
						
				default:break;

			}
		}
	  		
		if(p->LastChoice!=pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ])	
		{
			p->LastChoice=pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ];
			switch(p->LastChoice&0x01)                 //20200525
			{
					case 0x00: Screen_PastePicture(0x7260,114,(325<<16)+59,(342<<16)+77,(325<<16)+59);break;   //20200525
					case 0x01: Screen_PastePicture(0x7260,115,(325<<16)+59,(342<<16)+77,(325<<16)+59);break;   //20200525
					default:break;
			}
		}


		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				Screen_ShowPage(114); 
                for(int i=0;i<10;i++) //Ũ��
				{		

				  p->DisValue[i]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1+i];
				  sprintf((char*)p->Buffer,"%d",(u32)p->DisValue[i]);
				  Screen_RefreshDataMulti(0x0100+i*0x20,p->Buffer,15*2);
				  p->DisValue[i+10]=(float)pf->Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ1+i]/100;  //��Ϊ���Ǹ�����������Ҫ�Ŵ�����С���ܱ���С��
				  sprintf((char*)p->Buffer,"%.0f",p->DisValue[i+10]);
				  Screen_RefreshDataMulti(0x0400+i*0x20,p->Buffer,15*2);
				}	
				 p->RunModeStat=2;	
               //  Flux_StarSampling('C',1.0);   //��������C��   20200525				
			 }
			break;
      case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
							{	
								if(p->Key.bOutsideKeyboard != false)
								{
										if(p->Key.Mode>0&&p->Key.Mode<=10)
												{
														ScreenEnterNumberTwinkle(p,p->Key.Mode*0x20+0x00e0);
												}else if(p->Key.Mode>10)
														{ 
															ScreenEnterNumberTwinkle(p,(p->Key.Mode-10)*0x20+0x03e0);
														}
								}								
                                float DisPlay[6];	
								float  databuf=(float)3000/4096;   //��ADÿһ����ѹ��λmV/��
 								DisPlay[0]=Sp->Data[SLAVE_MAP_COConcent].Raw*databuf   ;//COԭʼֵ20200525
								DisPlay[1]=Sp->COConcent.Value;  //CO����ֵ20200525
								DisPlay[2]=Sp->Data[SLAVE_MAP_SO2Concent].Raw*databuf      ;//SO2ԭʼֵ 20200525
								DisPlay[3]=Sp->SO2Concent.Value;  //SO2����ֵ20200525 ,����ǰ
								DisPlay[4]=Sp->SO2Concent.Value;  //SO2����ֵ20200525 ,����ǰ
								if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x01))==1)  //
				                {
								DisPlay[4]=CalculationFormula_GetData()->SO2FromCO; //������SO2
                                //if(DisPlay[4]<0)DisPlay[4]=0;   //����
				                }
								DisPlay[5]=Flux_GetData()->ChannelC.ShowFlux;//����  20200525
								for(int i=0;i<6;i++)
								{   if((i==0)||(i==2))
									sprintf((char*)p->Buffer,"%.0f",DisPlay[i]);
									else
									{
									if(i==5)
									sprintf((char*)p->Buffer,"%.1f",DisPlay[i]);
									else
								 	sprintf((char*)p->Buffer,"%.0f",DisPlay[i]);
								    }
				                    Screen_RefreshDataMulti(0x0540+i*0x20,p->Buffer,15*2);
								}
						    p->Timer=500;
						}			 
			 }
			break;
			}
}
//----
/* date 2020-05-18
	��������void Screen_WHJZ_WHXZ_LLJZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-����У׼
*/
void Screen_WHJZ_WHXZ_LLJZ_Pag(Screen_DataStructure *p)
{
      FourBytes four; 
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  Slave_DataStructure* Sp =Slave_GetData();
	 //��������
	  Button_WHJZ_WHXZ_LLJZ_Pag(0x10,1,p->DisValue);
	  if(p->RunMode!=0x8120) return;
		//ѡ���ͼ����
	 if(p->LastCoiceonoff!=p->PauseOrStop)	
		{
			p->LastCoiceonoff=p->PauseOrStop;
			if(p->PauseOrStop==B0) //ֹͣ
			 {
				 Screen_PastePicture(0x40E0,99,(660<<16)+48,(770<<16)+109,(660<<16)+190);
				  Dis_Tips(p,(u8 *)"",0x0520);	                              //20200529 
			 }else  //=0
			 {
				 Screen_PastePicture(0x40E0,98,(660<<16)+190,(770<<16)+258,(660<<16)+190);
			 }
		}
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
			  
				case 0x01: Screen_PastePicture(0x0800,99,(345<<16)+60,(466<<16)+102,(345<<16)+60);break;
				case 0x02: Screen_PastePicture(0x0800,99,(25<<16)+140,(56<<16)+190,(25<<16)+140);break;
				case 0x03: Screen_PastePicture(0x0800,99,(25<<16)+190,(56<<16)+235,(25<<16)+190);break;
				case 0x04: Screen_PastePicture(0x0800,99,(25<<16)+235,(56<<16)+280,(25<<16)+235);break;
				case 0x05: Screen_PastePicture(0x0800,99,(25<<16)+280,(56<<16)+325,(25<<16)+280);break;
				case 0x06: Screen_PastePicture(0x0800,99,(25<<16)+325,(56<<16)+370,(25<<16)+325);break;
				case 0x07: Screen_PastePicture(0x0800,99,(25<<16)+370,(56<<16)+415,(25<<16)+370);break;
				case 0x08: Screen_PastePicture(0x0800,99,(238<<16)+140,(263<<16)+190,(238<<16)+140);break;
				case 0x09: Screen_PastePicture(0x0800,99,(238<<16)+190,(263<<16)+235,(238<<16)+190);break;
				case 0x0a: Screen_PastePicture(0x0800,99,(238<<16)+235,(263<<16)+280,(238<<16)+235);break;
				case 0x0b: Screen_PastePicture(0x0800,99,(448<<16)+140,(480<<16)+190,(448<<16)+140);break;
				case 0x0c: Screen_PastePicture(0x0800,99,(238<<16)+375,(340<<16)+415,(238<<16)+375);break;
				case 0x0d: Screen_PastePicture(0x0800,99,(448<<16)+375,(550<<16)+415,(448<<16)+375);break;
				case 0x0e: Screen_PastePicture(0x0800,99,(651<<16)+120,(800<<16)+180,(651<<16)+120);break;
				case 0x0f: if(p->PauseOrStop==B0) //��ͣ
							 {
								Screen_PastePicture(0x0800,99,(662<<16)+265,(770<<16)+330,(660<<16)+190);break;
							 }else
							 {
								Screen_PastePicture(0x0800,99,(662<<16)+188,(770<<16)+260,(660<<16)+190);break;
							 }
				case 0x10: Screen_PastePicture(0x0800,99,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;
						
				default:break;

			}
		}
	  		
		if(p->LastChoice!=pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ])	 //��
		{
			p->LastChoice=pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ];
			switch(p->LastChoice)
			{
				  case 0x01: Screen_PastePicture(0x62A0,99,(60<<16)+152,(85<<16)+180,(60<<16)+152);break;
				  case 0x02: Screen_PastePicture(0x62A0,99,(60<<16)+152,(85<<16)+180,(60<<16)+197);break;
				  case 0x03: Screen_PastePicture(0x62A0,99,(60<<16)+152,(85<<16)+180,(60<<16)+245);break;
				  case 0x04: Screen_PastePicture(0x62A0,99,(60<<16)+152,(85<<16)+180,(60<<16)+291);break;
				  case 0x05: Screen_PastePicture(0x62A0,99,(60<<16)+152,(85<<16)+180,(60<<16)+338);break;
				  case 0x06: Screen_PastePicture(0x62A0,99,(60<<16)+152,(85<<16)+180,(60<<16)+384);break;
				  case 0x07: Screen_PastePicture(0x62A0,99,(60<<16)+152,(85<<16)+180,(265<<16)+150);break;
				  case 0x08: Screen_PastePicture(0x62A0,99,(60<<16)+152,(85<<16)+180,(265<<16)+197);break;
				  case 0x09: Screen_PastePicture(0x62A0,99,(60<<16)+152,(85<<16)+180,(265<<16)+245);break;
				  case 0x0a: Screen_PastePicture(0x62A0,99,(60<<16)+152,(85<<16)+180,(485<<16)+147);break;
                  default:break;
			}
		}
	
	
		  for(int i=0;i<11;i++)
				{		
					GetU16dataFormFlash2Word(&four,FLASHIS_AdrWHJZ_LLJZ_BZLL+i*2,pf);
					p->DisValue[i+1]=four.FloatData;
				}
       /*				
	   if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]<0x0a)	 //�̳�
		 {
		   	p->DisValue[14]=Sp->BCPress.Value;//��ѹ
	    	p->DisValue[15]=Sp->BGPress.Value;//��ѹ
  			//p->DisValue[0]=Flux_GetData()->ChannelB.ShowFlux;//�̳�����
			p->DisValue[0]=Flux_DataMiddleFilter(&Flux_GetData()->ChannelB.FluxTowData.Filter,Flux_GetData()->ChannelB.ShowFlux);  //������ֵ�˲�20200529
			if(p->DisValue[0]<Slave_GetData()->DustMotor.Pid.Sv*0.8f)
			p->DisValue[0]=Flux_GetData()->ChannelB.ShowFlux;//�̳�����
		 }
		 else if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]==0x0a) //����
		 {
		   p->DisValue[14]=Sp->CCPress.Value;//C·��ѹ����ѹ������λkPaSp->ACPress.Value;//��ѹ
	       p->DisValue[15]=0.02;//Sp->AGPress.Value;//��ѹ
		 //  p->DisValue[0]=Flux_GetData()->ChannelC.ShowFlux;//��������
			p->DisValue[0]=Flux_DataMiddleFilter(&Flux_GetData()->ChannelC.FluxTowData.Filter,Flux_GetData()->ChannelC.ShowFlux);  //������ֵ�˲� 20200529
		    if(p->DisValue[0]<Slave_GetData()->GasMotor.Pid.Sv*0.8f)
			p->DisValue[0]=Flux_GetData()->ChannelC.ShowFlux;//�̳�����
		 }
		
		*/		
		p->DisValue[12]=Sp->JWTem.Value;//����
		if(p->LoadFlag==1)
		{
		  p->DisValue[13]=p->DisValue[12];
		  p->DisValue[16]=Flux_GetDQPress();   //����ѹ
		  p->LoadFlag=0;
		}
		Flux_LoadJWTem(1, p->DisValue[13]);
		Flux_LoadDQPress(1,p->DisValue[16]);
		
		
		
		//GetU16dataFormFlash2Word(&four,FLASHIS_AdrXTSZHJWD,pf); //����
//		//p->DisValue[13]=four.FloatData;
//		if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]<0x0a)	
//	//	GetU16dataFormFlash2Word(&four,FLASHIS_AdrXTSZDQY,pf); //����ѹ
//		p->DisValue[16]=four.FloatData; //����ѹ

		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(98); 
                for(int i=0;i<17;i++)
				{
				 if((i>=2)&&(i<=11))
				 sprintf((char*)p->Buffer,"%.4f",p->DisValue[i]);
				 else 
				 {
				 if(i==15)
				 sprintf((char*)p->Buffer,"%.2f",p->DisValue[i]);  //��ѹ 2λ
				 else
				 sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);
			     }
				 Screen_RefreshDataMulti(0x0100+i*0x20,p->Buffer,15*2);
				}				 
				 Dis_Tips(p,(u8 *)"",0x0520);	
				 p->RunModeStat=2;		
			    p->Timer=0;
			 }
			break;
      case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
							{	
                              	 p->Timer=200;
								
		                       if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]<0x0a)	 //�̳�
								{
								p->DisValue[14]=Sp->BCPress.Value;//��ѹ
								p->DisValue[15]=Sp->BGPress.Value;//��ѹ
								//p->DisValue[0]=Flux_GetData()->ChannelB.ShowFlux;//�̳�����
								p->DisValue[0]=Flux_DataMiddleFilter(&Flux_GetData()->ChannelB.FluxTowData.Filter,Flux_GetData()->ChannelB.ShowFlux);  //������ֵ�˲�20200529
								if(p->DisValue[0]<Slave_GetData()->DustMotor.Pid.Sv*0.95f)
								p->DisValue[0]=Flux_GetData()->ChannelB.ShowFlux;//�̳�����
								}
							   else if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]==0x0a) //����
								{
		                        p->DisValue[14]=Sp->CCPress.Value;//C·��ѹ����ѹ������λkPaSp->ACPress.Value;//��ѹ
	                            p->DisValue[15]=0.02;//Sp->AGPress.Value;//��ѹ
		                        //  p->DisValue[0]=Flux_GetData()->ChannelC.ShowFlux;//��������
			                    p->DisValue[0]=Flux_DataMiddleFilter(&Flux_GetData()->ChannelC.FluxTowData.Filter,Flux_GetData()->ChannelC.ShowFlux);  //������ֵ�˲� 20200529
		                        if(p->DisValue[0]<Slave_GetData()->GasMotor.Pid.Sv*0.95f)
			                    p->DisValue[0]=Flux_GetData()->ChannelC.ShowFlux;//�̳�����
		                        }
								p->CT++;
								if(p->CT>=5)
								{
                                 p->CT=0;
               	                for(int i=0;i<17;i++)
								{
									if(i==1||(i>=2&&i<=11)) continue;
									 if(i==15)
				                     sprintf((char*)p->Buffer,"%.2f",p->DisValue[i]);  //��ѹ 2λ
				                     else
				                     sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);

									Screen_RefreshDataMulti(0x0100+i*0x20,p->Buffer,15*2);
								 }
									if(p->Key.bOutsideKeyboard != false)    //�ⰴ��
									{        
											if(p->Key.Mode==12)
													{
														ScreenEnterNumberTwinkle(p,0x02A0);
													}else  if(p->Key.Mode==13)
													{
														ScreenEnterNumberTwinkle(p,0x0300);
													}										
											else if(p->Key.Mode!=0)
													{
														ScreenEnterNumberTwinkle(p,p->Key.Mode*0x20+0x0100);
													}
									}  
								}			  
									
						}	
							
						
						
                 //����Ϊ����ѹ 20200529, �������ʱ���ӵ�أ����δ���, ��ʽ����ʱ��򿪡�
                  	if(p->PauseOrStop==B0)
					{
						 //��ͣ
					 if((Battery_GetData()->LostPV.ActualV<LowPOWER)&&(Power_GetData()->Flg&0x04))	
					 {	 
						 p->PauseOrStop=0;
						 p->LastChoice=0xff;
						 Dis_Tips(p,(u8 *)"��������",0x0520);	                              //20200529 
						 Flux_StopSampling('B');
						 Flux_StopSampling('C');
					 }
					}
				 }
			break;
			}
}
//----
/* date 2020-1-16
	��������void Screen_WHJZ_WHXZ_MMSZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-��������
*/
void Screen_WHJZ_WHXZ_MMSZ_Pag(Screen_DataStructure *p)
{

	  static u8 LastChoice=255;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	  Button_WHJZ_WHXZ_MMSZ_Pag(0x0D,1,p->DisValue);
	  if(p->RunMode!=0x8130) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
			p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
				case 0x01: Screen_PastePicture(0x0800,93,(54<<16)+48,(514<<16)+95,(54<<16)+48);break;  //ԭ����
				case 0x02: Screen_PastePicture(0x0800,93,(54<<16)+97,(514<<16)+144,(54<<16)+97);break;//������
				case 0x03: Screen_PastePicture(0x0800,93,(54<<16)+146,(514<<16)+193,(54<<16)+146);break;//ȷ��������
				case 0x04: Screen_PastePicture(0x0800,93,(580<<16)+94,(772<<16)+141,(580<<16)+94);break;//ȷ�������޸�
				case 0x05: Screen_PastePicture(0x0800,93,(54<<16)+193,(272<<16)+240,(54<<16)+193);break;  //��ӡ��������ѡ��
				case 0x06: Screen_PastePicture(0x0800,93,(312<<16)+194,(511<<16)+241,(312<<16)+194);break;//�����ӡ��������
				case 0x07: Screen_PastePicture(0x0800,93,(577<<16)+181,(767<<16)+225,(577<<16)+181);break;//��ӡ������ʼ��
				case 0x08: Screen_PastePicture(0x0800,93,(54<<16)+242,(269<<16)+289,(54<<16)+242);break;  //APP��������ѡ��
				case 0x09: Screen_PastePicture(0x0800,93,(313<<16)+243,(510<<16)+290,(313<<16)+243);break;	//����APP��������	
				case 0x0A: Screen_PastePicture(0x0800,93,(581<<16)+233,(778<<16)+280,(581<<16)+233);break;  //8 app������ʼ��
				case 0x0B: Screen_PastePicture(0x0800,93,(54<<16)+291,(515<<16)+337,(54<<16)+291);break; //e28-2.4g���ߵ�ַ                                                
				case 0x0C: Screen_PastePicture(0x0800,93,(54<<16)+339,(515<<16)+385,(54<<16)+339);break; //e28-2.4g�����ŵ�
				case 0x0D: Screen_PastePicture(0x0800,93,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;  //����Ƿ���
				default:break;
			}
		}
	  		
		if(LastChoice!=pf->Data[FLASHIS_AdrXTSZBOX])	
		{
			LastChoice=pf->Data[FLASHIS_AdrXTSZBOX];
			switch(LastChoice&(1<<2))
			{
				case 0x00: Screen_PastePicture(0x5D60,92,(282<<16)+201,(308<<16)+230,(282<<16)+201);break;
				case 0x04: Screen_PastePicture(0x5D60,93,(282<<16)+201,(308<<16)+230,(282<<16)+201);break;
				default:break;
			}
		}		
		switch(pf->Data[FLASHIS_AdrXTWHLYPDMKG]&0x02)
		{
			case 0x00: Screen_PastePicture(0x5D80,92,(282<<16)+252,(308<<16)+281,(282<<16)+252);break;
			case 0x02: Screen_PastePicture(0x5D80,93,(282<<16)+252,(308<<16)+281,(282<<16)+252);break;
			default:break;
		}

		//���ݸ���
	switch(p->RunModeStat)
	{
		case 0x01: //��̬������ʾ			
		{
			Screen_ShowPage(92); 
           	p->DisValue[3]=pf->Data[FLASHIS_AdrXTWHLYPDM]; //���������
		    p->DisValue[4]=pf->Data[FLASHIS_AdrXTWHAPPLYPDM]; //����APP

			p->DisValue[5]=Slave_GetData()->E28.ADRRset;  //
			p->DisValue[6]=Slave_GetData()->E28.CHset;    //

     		for(int i=0;i<7;i++)
			{
				if(p->DisValue[i]==-1)
				{
				sprintf((char*)p->Buffer,"   ");
				}else
				{
				sprintf((char*)p->Buffer,"%d",(u32)p->DisValue[i]);
				}			
				Screen_RefreshDataMulti(0x0100+i*0x20,p->Buffer,15*2);
			}				 
			p->RunModeStat=2;				 
		}
		break;
		case 0x02: //��̬̬������ʾ			
		{
			if(p->Timer==0)
			{				
				if(p->Warning!=0)
				{
					if(p->Warning==1)
					{
						Dis_Tips(p,(u8*)"�����޸ĳɹ�",0x01E0);		
					}
					else if(p->Warning==2)
					{
						Dis_Tips(p,(u8*)"�����޸�ʧ��---ԭ������������������벻һ��",0x01E0);
					}
					else if(p->Warning==3)
					{
						Dis_Tips(p,(u8*)"��������δ���������ȿ�����",0x01E0);
					}
					else if(p->Warning==4)
					{
						Dis_Tips(p,(u8*)"��ʱ����ʼ��ʧ�ܣ������³�ʼ����",0x01E0);
					}
					else if(p->Warning==5)
					{
						Dis_Tips(p,(u8*)"������ʼ���ɹ���",0x01E0);
					}
					else if(p->Warning==6)
					{
						Dis_Tips(p,(u8*)"���ڳ�ʼ�������Ժ�.....",0x01E0);
					}
					
					else if(p->Warning==8)
					{
						Dis_Tips(p,(u8*)"��ʱ,E28��������ʧ��,�����²�����",0x01E0);
					}
					else if(p->Warning==9)
					{
						Dis_Tips(p,(u8*)"E28�������óɹ���",0x01E0);
					}
					if(p->Warning != 6)
						p->Warning=0;	
					p->Timer=2000;									
				}else
				{
					Dis_Tips(p,(u8*)" ",0x01E0);	//���״̬
					if(p->Key.Mode!=0)
					{
						if(p->Key.bOutsideKeyboard != false)
							ScreenEnterNumberTwinkle(p,p->Key.Mode*0x20+0x00e0);  //Key.Mode=1~7
					}
					p->Timer=500;
				 }								
							 
			}
			if(p->uTempState == 1)  //�������
			{  //����������ʼ��
				u8 res = ProcessBlueToothInitial((u32)p->DisValue[3]);  //�����߳�
				if(res!=0)
				{
					if(res == 1)
						p->Warning = 4; //��ʱ����ʼ��ʧ�ܣ������³�ʼ����
					else if(res == 2)
						p->Warning = 5;	//������ʼ���ɹ���					
					p->uTempState = 0;
				}
			}
			
			if(p->uE28CtlState== 1)  //�������
			{   u8 Eres=ProcessE28Initial(Slave_GetData()->E28.ADRRset,Slave_GetData()->E28.CHset);
				if(Eres!=0)
				{
					if(Eres == 1)
						p->Warning = 8; //��ʱ������ʧ�ܣ����������á�
					else if(Eres== 2)
						p->Warning = 9;	//E28��ʼ���ɹ���		
					
					    p->uE28CtlState = 0;
				}
				
			}
			
			
		}
		break;
	}
}
//----
/* date 2020-05-18
	��������void Screen_WHJZ_WHXZ_NDJZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-Ũ��У׼
*/
void Screen_WHJZ_WHXZ_NDJZ_Pag(Screen_DataStructure *p)
{

	  Flashisp_DataStructure* pf=Flashisp_GetData();
	   Slave_DataStructure* Sp=Slave_GetData();		
	 //��������
	  Button_WHJZ_WHXZ_NDJZ_Pag(0x0b,1,p->DisValue);
	
	  if(p->RunMode!=0x8140) return;
		//ѡ���ͼ����

	 if(p->PauseOrStop==B0) //����״̬����ʾֹͣ
		 { if(p->Key.Opt!=9)
		   Screen_PastePicture(0x40E0,101,(630<<16)+43,(750<<16)+106,(630<<16)+210);
		   
		 }else
		 { if(p->Key.Opt!=9) //ֹͣ״̬����ʾ����
		   Screen_PastePicture(0x40E0,100,(630<<16)+210,(750<<16)+273,(630<<16)+210);
		 }
	  
		switch(p->Key.Opt)
		{
		case 0x09: if(p->PauseOrStop==B0) //����״̬  ,��ʾֹͣ  ��
						{  Screen_PastePicture(0x0800,101,(630<<16)+410,(750<<16)+475,(630<<16)+210);break;
															//Screen_PastePicture(0x0800,103,(105<<16)+375,(210<<16)+440,(337<<16)+375);break;
						}else //ֹͣ״̬����ʾ����
						{  Screen_PastePicture(0x0800,101,(630<<16)+210,(750<<16)+273,(630<<16)+210);break;
															//Screen_PastePicture(0x0800,103,(337<<16)+375,(452<<16)+440,(337<<16)+375);break;
						}
									 
		}
	
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{ 
					case 0x01: Screen_PastePicture(0x0800,101,(30<<16)+105,(120<<16)+150,(30<<16)+105);break;
					case 0x02: Screen_PastePicture(0x0800,101,(30<<16)+150,(120<<16)+195,(30<<16)+150);break;
					case 0x03: Screen_PastePicture(0x0800,101,(30<<16)+195,(120<<16)+240,(30<<16)+195);break;
					case 0x04: Screen_PastePicture(0x0800,101,(30<<16)+240,(120<<16)+295,(30<<16)+240);break;
				    case 0x05: Screen_PastePicture(0x0800,101,(30<<16)+295,(120<<16)+340,(30<<16)+295);break;
					case 0x06: Screen_PastePicture(0x0800,101,(30<<16)+340,(120<<16)+385,(30<<16)+340);break;
					case 0x07: Screen_PastePicture(0x0800,101,(30<<16)+385,(120<<16)+430,(30<<16)+385);break;
				    case 0x08: Screen_PastePicture(0x0800,101,(600<<16)+135,(750<<16)+204,(600<<16)+135);break;
				
				    case 0x0A: Screen_PastePicture(0x0800,101,(630<<16)+275,(750<<16)+340,(630<<16)+275);break;
					case 0x0B: Screen_PastePicture(0x0800,101,(640<<16)+346,(720<<16)+408,(640<<16)+346);break;
						
				default:break;

			}
		}
			 
		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
			Screen_ShowPage(100); 
            Screen_PastePicture(0x40E0,100,(220<<16)+375,(220<<16)+375,(337<<16)+375);				 
     		for(int i=0;i<7;i++)
				{
				    FourBytes four;
					
				//	if((pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>i)&0x01)  //�ж���û�а�װ
					{
					GetU16dataFormFlash2Word(&four,FLASHIS_AdrWHJZ_NDJZ_LDO2+i*2,pf);
					if((i==0)||(i==6))
					sprintf((char*)p->Buffer,"%.1f",four.FloatData); //�����������̼��ʾС����
					else
					sprintf((char*)p->Buffer,"%.0f",four.FloatData);	//�ĳɸ�����һ��������ʾС��, modify by alfred, 2020.07.09
					
				    Screen_RefreshDataMulti(0x0200+i*0x20,p->Buffer,15*2);
					GetU16dataFormFlash2Word(&four,FLASHIS_AdrWHJZ_NDJZ_BLO2_1+i*2,pf);
					sprintf((char*)p->Buffer,"%.3f",four.FloatData);
				    Screen_RefreshDataMulti(0x0500+i*0x20,p->Buffer,15*2);
					}
				/*	else
					{
					
					sprintf((char*)p->Buffer,"------");
				    Screen_RefreshDataMulti(0x0200+i*0x20,p->Buffer,15*2);
					GetU16dataFormFlash2Word(&four,FLASHIS_AdrWHJZ_NDJZ_BLO2_1+i*2,pf);
					sprintf((char*)p->Buffer,"------");
				    Screen_RefreshDataMulti(0x0500+i*0x20,p->Buffer,15*2);	
					}
					*/
					
				}				 
				 p->RunModeStat=2;				 
			 }
			break;
      case 0x02: //��̬̬������ʾ			
			 {float databuf;
				 if(p->Timer==0)
							{	
									if(p->Key.bOutsideKeyboard != false) //��Χ������������ʱ������
											{								             							
												if(p->Key.Mode>=1&&p->Key.Mode<=7)
														{
														ScreenEnterNumberTwinkle(p,p->Key.Mode*0x20+0x01e0);  //��
														}
												else  if(p->Key.Mode>=8&&p->Key.Mode<=14)		
														{
														ScreenEnterNumberTwinkle(p,(p->Key.Mode-7)*0x20+0x04e0);
														}
											}
							 //Ũ������װ��
							 {
								 p->DisValue[0]= Sp->O2Concent.Value;
								 p->DisValue[1]= Sp->SO2Concent.Value;
								 p->DisValue[2]= Sp->NOConcent.Value;
								 p->DisValue[3]= Sp->NO2Concent.Value;
								 p->DisValue[4]= Sp->COConcent.Value;
								 p->DisValue[5]= Sp->H2SConcent.Value;
								 p->DisValue[6]= Sp->CO2Concent.Value;
								 
								  databuf=(float)3000/4096;   //��ADÿһ����ѹ��λmV/��
								  p->DisValue[7]=Sp->Data[SLAVE_MAP_O2Concent].Raw*databuf;
								  p->DisValue[8]=Sp->Data[SLAVE_MAP_SO2Concent].Raw*databuf;
								  p->DisValue[9]=Sp->Data[SLAVE_MAP_NOConcent].Raw*databuf;
								  p->DisValue[10]=Sp->Data[SLAVE_MAP_NO2Concent].Raw*databuf;
								  p->DisValue[11]=Sp->Data[SLAVE_MAP_COConcent].Raw*databuf;
								  p->DisValue[12]=Sp->Data[SLAVE_MAP_H2SConcent].Raw*databuf;
								  p->DisValue[13]=Sp->Data[SLAVE_MAP_CO2Concent].Raw*databuf;
							 }
							
							for(int i=0;i<7;i++)
							{
								if((pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>i)&0x01)  //�ж���û�а�װ
					         {      if((i==0)||(i==6))
									sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);
								    else
									sprintf((char*)p->Buffer,"%.0f",p->DisValue[i]);  //����ʾС����
									
									Screen_RefreshDataMulti(0x0100+i*0x20,p->Buffer,15*2);
									
									sprintf((char*)p->Buffer,"%d",(u32)p->DisValue[i+7]);
									Screen_RefreshDataMulti(0x0400+i*0x20,p->Buffer,15*2);
								   }
								 else
								   {
									sprintf((char*)p->Buffer,"-----");
									Screen_RefreshDataMulti(0x0100+i*0x20,p->Buffer,15*2);
									sprintf((char*)p->Buffer,"------");
									Screen_RefreshDataMulti(0x0400+i*0x20,p->Buffer,15*2);
								   }
							}	
							sprintf((char*)p->Buffer,"%.1f",Flux_GetData()->ChannelC.ShowFlux); //����
							Screen_RefreshDataMulti(0x04E0,p->Buffer,15*2);
						    p->Timer=500;
						}			 
			 }
			break;
			}
}
//----
/* date 2020-1-16
	��������void Screen_WHJZ_WHXZ_NDJZ_BLSZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-Ũ��У׼-��������
*/
  const char strLC[7][30]={ //����
		"O2����0%~30%",
		"SO2����0-5700mg/m3",
		"NO����0-1300mg/m3",
		"NO2����0-200mg/m3",
		"CO����0-5000mg/m3",
		"H2S����0-300mg/m3",
		"CO2����0-20%0mg/m3",
		};
		 const char strFDLC[7][3][30]={ //�ֶ�����
		{"10%~20%","0%-10%","20%-30%"},//O2
		{"1881-3762mg/m3","0-1881mg/m3","3762-5700mg/m3"},
		{"429-858mg/m3","0-429mg/m3","858-1300mg/m3"},
		{"66-132mg/m3","0-66mg/m3","132-200mg/m3"},
		{"1650-3300mg/m3","0-1650mg/m3","3300-5000mg/m3"},
		{"99-198mg/m3","0-99mg/m3","198-300mg/m3"},
		{"7%-13%","0%-7%","13%-20%"},
		};
		 
		
//---2020-09-04		
void Screen_WHJZ_WHXZ_NDJZ_BLSZ_Pag(Screen_DataStructure *p)
{
      Slave_DataStructure* Sp=Slave_GetData();
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  FourBytes four;
	 //��������
	  Button_WHJZ_WHXZ_NDJZ_BLSZ_Pag(0x06,1,p->DisValue);
	  if(p->RunMode!=0x8141) return;
		//ѡ���ͼ����
	
	     //����ֹͣ
	//  if(p->LastChoice!=p->PauseOrStop)	
		{
		//	p->LastChoice=p->PauseOrStop;
		if(p->PauseOrStop==B0) //����״̬����ʾֹͣ
		 { if(p->Key.Opt!=4)
		   Screen_PastePicture(0x40E0,103,(220<<16)+375,(330<<16)+440,(337<<16)+375);
		   
		 }else
		 { if(p->Key.Opt!=4) //ֹͣ״̬����ʾ����
		   Screen_PastePicture(0x40E0,102,(337<<16)+375,(452<<16)+440,(337<<16)+375);
		 }
	     }
		switch(p->Key.Opt)
		{
		case 0x04: if(p->PauseOrStop==B0) //����״̬  ,��ʾֹͣ  ��
									 {  Screen_PastePicture(0x800,103,(102<<16)+375,(216<<16)+440,(337<<16)+375);break;
															//Screen_PastePicture(0x0800,103,(105<<16)+375,(210<<16)+440,(337<<16)+375);break;
									 }else //ֹͣ״̬����ʾ����
									 {  Screen_PastePicture(0x800,103,(337<<16)+375,(452<<16)+440,(337<<16)+375);break;
															//Screen_PastePicture(0x0800,103,(337<<16)+375,(452<<16)+440,(337<<16)+375);break;
									 }
									
		}
		//��λ��
		switch(p->gzdselect)
		{
			case 1:
				Screen_PastePicture(0x6660,103,(50<<16)+170,(87<<16)+208,(50<<16)+170);
				break;
			case 2:
				Screen_PastePicture(0x6660,103,(50<<16)+218,(87<<16)+260,(50<<16)+218);
				break;
			case 3:
				Screen_PastePicture(0x6660,103,(50<<16)+266,(87<<16)+ 310,(50<<16)+266);
				break;
			default:
				p->gzdselect=0;
			    Screen_PastePicture(0x6660,102,(50<<16)+170,(87<<16)+208,(50<<16)+170);
				break;
		}
 
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{  
					        case 0x01: 
								Screen_PastePicture(0x0800,103,(95<<16)+170,(265<<16)+208,(95<<16)+170);

							break;
				
							case 0x02: 
								Screen_PastePicture(0x0800,103,(95<<16)+218,(265<<16)+260,(95<<16)+218);
				 
							break;
				
							case 0x03: 
								Screen_PastePicture(0x0800,103,(95<<16)+266,(265<<16)+310,(95<<16)+266);

							break;
				       
							case 0x05: Screen_PastePicture(0x0800,103,(456<<16)+375,(718<<16)+440,(456<<16)+375);break;
							case 0x06: Screen_PastePicture(0x0800,103,(720<<16)+375,(790<<16)+440,(720<<16)+375);break;
						
				default:break;

			}
		}
			 
		//���ݸ���
    switch(p->RunModeStat)
		{
		
		   case 0x01: //��̬������ʾ			
			 {
					Screen_ShowPage(102); 		
				  if(p->SubCnt==0)   //O2
					{   
					four.U16Data[0]=pf->Data[FLASHIS_AdrO2LQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrO2LQ+1];
					sprintf((char*)p->Buffer,"O2����:0%%~%d%%",four.U32Data);	
					Screen_RefreshDataMulti(0x02A0,p->Buffer,15*2);	

					//��ʾ���α�������
					sprintf((char*)p->Buffer,"(%d%%~%d%%)",four.U32Data/3,four.U32Data*2/3);	
					Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);		
				
                    sprintf((char*)p->Buffer,"(0%%~%d%%)",four.U32Data/3);	
					Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);		

                    sprintf((char*)p->Buffer,"(%d%%~%d%%)",four.U32Data*2/3,four.U32Data);	
					Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);		

                    sprintf((char*)p->Buffer,"(  %%  )");	
					Screen_RefreshDataMulti(0x0400,p->Buffer,15*2);	
					sprintf((char*)p->Buffer,"(  %%  )");	
					Screen_RefreshDataMulti(0x0420,p->Buffer,15*2);

					}
					else if(p->SubCnt==1)  //SO2
					{
					four.U16Data[0]=pf->Data[FLASHIS_AdrSO2LQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrSO2LQ+1];
					sprintf((char*)p->Buffer,"SO2����:0~%d mg/m^3",four.U32Data);	
					Screen_RefreshDataMulti(0x02A0,p->Buffer,15*2);	

					//��ʾ���α�������
					sprintf((char*)p->Buffer,"(%d~%d)",four.U32Data/3,four.U32Data*2/3);	
					Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);		
				
					sprintf((char*)p->Buffer,"(0~%d)",four.U32Data/3);	
					Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);		

					sprintf((char*)p->Buffer,"(%d~%d)",four.U32Data*2/3,four.U32Data);	
					Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);	

                    sprintf((char*)p->Buffer,"(mg/m^3)");	
					Screen_RefreshDataMulti(0x0400,p->Buffer,15*2);	
					sprintf((char*)p->Buffer,"(mg/m^3)");	
					Screen_RefreshDataMulti(0x0420,p->Buffer,15*2);
						
					}
					else if(p->SubCnt==2)  //NO
					{
					four.U16Data[0]=pf->Data[FLASHIS_AdrNOLQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrNOLQ+1];
						sprintf((char*)p->Buffer,"NO����:0~%d mg/m^3",four.U32Data);	
					Screen_RefreshDataMulti(0x02A0,p->Buffer,15*2);	

					//��ʾ���α�������
					sprintf((char*)p->Buffer,"(%d~%d)",four.U32Data/3,four.U32Data*2/3);	
					Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);		
				
					sprintf((char*)p->Buffer,"(0~%d)",four.U32Data/3);	
					Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);		

					sprintf((char*)p->Buffer,"(%d~%d)",four.U32Data*2/3,four.U32Data);	
					Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);

					sprintf((char*)p->Buffer,"(mg/m^3)");	
					Screen_RefreshDataMulti(0x0400,p->Buffer,15*2);	
					sprintf((char*)p->Buffer,"(mg/m^3)");	
					Screen_RefreshDataMulti(0x0420,p->Buffer,15*2);	
						
					}
					else if(p->SubCnt==3)  //NO2
					{
					four.U16Data[0]=pf->Data[FLASHIS_AdrNO2LQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrNO2LQ+1];
					sprintf((char*)p->Buffer,"NO2����:0~%d mg/m^3",four.U32Data);	
					Screen_RefreshDataMulti(0x02A0,p->Buffer,15*2);	

					//��ʾ���α�������
					sprintf((char*)p->Buffer,"(%d~%d)",four.U32Data/3,four.U32Data*2/3);	
					Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);		
				
                    sprintf((char*)p->Buffer,"(0~%d)",four.U32Data/3);	
					Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);		

                    sprintf((char*)p->Buffer,"(%d~%d)",four.U32Data*2/3,four.U32Data);	
					Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);		
						
                    sprintf((char*)p->Buffer,"(mg/m^3)");	
					Screen_RefreshDataMulti(0x0400,p->Buffer,15*2);	
					sprintf((char*)p->Buffer,"(mg/m^3)");	
					Screen_RefreshDataMulti(0x0420,p->Buffer,15*2);
						
					}
					else if(p->SubCnt==4)  //CO
					{
					four.U16Data[0]=pf->Data[FLASHIS_AdrCOLQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrCOLQ+1];
						sprintf((char*)p->Buffer,"CO����:0~%d mg/m^3",four.U32Data);	
					Screen_RefreshDataMulti(0x02A0,p->Buffer,15*2);	

					//��ʾ���α�������
					sprintf((char*)p->Buffer,"(%d~%d)",four.U32Data/3,four.U32Data*2/3);	
					Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);		
				
					sprintf((char*)p->Buffer,"(0~%d)",four.U32Data/3);	
					Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);		

					sprintf((char*)p->Buffer,"(%d~%d)",four.U32Data*2/3,four.U32Data);	
					Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);	

					sprintf((char*)p->Buffer,"(mg/m^3)");	
					Screen_RefreshDataMulti(0x0400,p->Buffer,15*2);	
					sprintf((char*)p->Buffer,"(mg/m^3)");	
					Screen_RefreshDataMulti(0x0420,p->Buffer,15*2);
						
					}
					else if(p->SubCnt==5)  //H2S
					{
					four.U16Data[0]=pf->Data[FLASHIS_AdrH2SLQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrH2SLQ+1];
						sprintf((char*)p->Buffer,"H2S����:0~%d mg/m^3",four.U32Data);	
					Screen_RefreshDataMulti(0x02A0,p->Buffer,15*2);	

					//��ʾ���α�������
					sprintf((char*)p->Buffer,"(%d~%d)",four.U32Data/3,four.U32Data*2/3);	
					Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);		
				
					sprintf((char*)p->Buffer,"(0~%d)",four.U32Data/3);	
					Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);		

					sprintf((char*)p->Buffer,"(%d~%d)",four.U32Data*2/3,four.U32Data);	
					Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);	
						
                    sprintf((char*)p->Buffer,"(mg/m^3)");	
					Screen_RefreshDataMulti(0x0400,p->Buffer,15*2);	
					sprintf((char*)p->Buffer,"(mg/m^3)");	
					Screen_RefreshDataMulti(0x0420,p->Buffer,15*2);

						
					}
					else if(p->SubCnt==6)  //CO2
					{
					four.U16Data[0]=pf->Data[FLASHIS_AdrCO2LQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrCO2LQ+1];
						sprintf((char*)p->Buffer,"CO2����:0%%~%d%%",four.U32Data);	
					Screen_RefreshDataMulti(0x02A0,p->Buffer,15*2);	

					//��ʾ���α�������
					sprintf((char*)p->Buffer,"(%d%%~%d%%)",four.U32Data/3,four.U32Data*2/3);	
					Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);		
				
					sprintf((char*)p->Buffer,"(0%%~%d%%)",four.U32Data/3);	
					Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);		

					sprintf((char*)p->Buffer,"(%d%%~%d%%)",four.U32Data*2/3,four.U32Data);	
					Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);	

                    sprintf((char*)p->Buffer,"(  %%  )");	
					Screen_RefreshDataMulti(0x0400,p->Buffer,15*2);	
					sprintf((char*)p->Buffer,"(  %%  )");	
					Screen_RefreshDataMulti(0x0420,p->Buffer,15*2);						
												
					}
					
					//��������Ũ�ȱ궨ֵ
					four.U16Data[0]=pf->Data[FLASHIS_AdrWHJZ_NDJZ_BQLDO2_1+p->SubCnt*2];
					four.U16Data[1]=pf->Data[FLASHIS_AdrWHJZ_NDJZ_BQLDO2_1+p->SubCnt*2+1];
					p->DisValue[0]=four.FloatData;  
					if((p->SubCnt==0)||(p->SubCnt==6))
					sprintf((char*)p->Buffer,"%.1f",p->DisValue[0]); //����ֵ
                    else
					sprintf((char*)p->Buffer,"%.0f",p->DisValue[0]);
					
					Screen_RefreshDataMulti(0x01E0,p->Buffer,15*2);
					
					four.U16Data[0]=pf->Data[FLASHIS_AdrWHJZ_NDJZ_BQLDO2_2+p->SubCnt*2];
					four.U16Data[1]=pf->Data[FLASHIS_AdrWHJZ_NDJZ_BQLDO2_2+p->SubCnt*2+1];
					p->DisValue[1]=four.FloatData;
                    if((p->SubCnt==0)||(p->SubCnt==6))
					sprintf((char*)p->Buffer,"%.1f",p->DisValue[1]); //����ֵ
                    else
					sprintf((char*)p->Buffer,"%.0f",p->DisValue[1]);
					Screen_RefreshDataMulti(0x0200,p->Buffer,15*2);			


					four.U16Data[0]=pf->Data[FLASHIS_AdrWHJZ_NDJZ_BQLDO2_3+p->SubCnt*2];
					four.U16Data[1]=pf->Data[FLASHIS_AdrWHJZ_NDJZ_BQLDO2_3+p->SubCnt*2+1];
					p->DisValue[2]=four.FloatData;
					
					if((p->SubCnt==0)||(p->SubCnt==6))
					sprintf((char*)p->Buffer,"%.1f",p->DisValue[2]); //����ֵ
                    else
					sprintf((char*)p->Buffer,"%.0f",p->DisValue[2]);
					Screen_RefreshDataMulti(0x0220,p->Buffer,15*2);								 				 
				 
					//��ʾ����
					FourBytes four;
					GetU16dataFormFlash2Word(&four,FLASHIS_AdrWHJZ_NDJZ_BLO2_1+p->SubCnt*2,pf);
					p->DisValue[6]=four.FloatData;
					sprintf((char*)p->Buffer,"%.3f",four.FloatData);
					Screen_RefreshDataMulti(0x0180,p->Buffer,15*2);
					GetU16dataFormFlash2Word(&four,FLASHIS_AdrWHJZ_NDJZ_BLO2_2+p->SubCnt*2,pf);
					p->DisValue[7]=four.FloatData;
					sprintf((char*)p->Buffer,"%.3f",four.FloatData);
					Screen_RefreshDataMulti(0x01A0,p->Buffer,15*2);
					GetU16dataFormFlash2Word(&four,FLASHIS_AdrWHJZ_NDJZ_BLO2_3+p->SubCnt*2,pf);
					 p->DisValue[8]=four.FloatData;
					sprintf((char*)p->Buffer,"%.3f",four.FloatData);
					Screen_RefreshDataMulti(0x01C0,p->Buffer,15*2);
					p->RunModeStat=2;				 
			 }
			break;
      case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
							{				
             		if(p->Key.bOutsideKeyboard != false)
										{   
											if(p->Key.Mode>=0x21&&p->Key.Mode<=0x26)
													{
															
														    ScreenEnterNumberTwinkle(p,(p->Key.Mode-0x20)*0x20+0x0160);
													}
										}
          
							for(int i=0;i<3;i++)
							{

								Slave_TPActualStructure * BeforGasp=&Sp->O2Concent;//����ǰ
                                if(i==0)
								{
								p->DisValue[3]=BeforGasp[p->SubCnt].Value0;
								if((p->SubCnt==0)||(p->SubCnt==6))
								sprintf((char*)p->Buffer,"%.1f",BeforGasp[p->SubCnt].Value0); //����ֵ
                                else								
								sprintf((char*)p->Buffer,"%.0f",BeforGasp[p->SubCnt].Value0); //����ֵ
									
								}
								else if(i==1)
								{
								p->DisValue[4]=BeforGasp[p->SubCnt].Value1;
								if((p->SubCnt==0)||(p->SubCnt==6))
								sprintf((char*)p->Buffer,"%.1f",BeforGasp[p->SubCnt].Value1); //����ֵ
                                else
								sprintf((char*)p->Buffer,"%.0f",BeforGasp[p->SubCnt].Value1); //����ֵ
								}
								else if(i==2)
								{
								p->DisValue[5]=BeforGasp[p->SubCnt].Value2;
									
								if((p->SubCnt==0)||(p->SubCnt==6))
								sprintf((char*)p->Buffer,"%.1f",BeforGasp[p->SubCnt].Value2); //����ֵ
                                else
								sprintf((char*)p->Buffer,"%.0f",BeforGasp[p->SubCnt].Value2); //����ֵ
								}
								
								Screen_RefreshDataMulti(0x0240+i*0x20,p->Buffer,15*2);
																
							}	
							sprintf((char*)p->Buffer,"%.1f",Flux_GetData()->ChannelC.ShowFlux); //����
							Screen_RefreshDataMulti(0x0160,p->Buffer,15*2);

						    p->Timer=500;
							if(p->staflg)
							{
								sprintf((char*)p->Buffer,"%dS",p->ZjTimer); //����ʱS
								Screen_RefreshDataMulti(0x02E0,p->Buffer,15*2);
							}else
							{
								if(p->PauseOrStop==B0)
								{
								   p->Function=0;
						       Flux_StopSampling('C');//ֹͣ����
								}
							  Dis_Tips(p,(u8 *)" ",0x02E0);
							}
							}
            							
			 }
			break;
			}
}
//----
/* date 2020-05-20
	��������void Screen_WHJZ_WHXZ_YLJZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-ѹ��У׼
*/
void Screen_WHJZ_WHXZ_YLJZ_Pag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  	Slave_DataStructure* Sp=Slave_GetData();
	 //��������
	  Button_WHJZ_WHXZ_YLJZ_Pag(0x09,1,p->DisValue);
	  if(p->RunMode!=0x8150) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
					case 0x01: Screen_PastePicture(0x0800,95,(30<<16)+105,(130<<16)+155,(30<<16)+105);break;
					case 0x02: Screen_PastePicture(0x0800,95,(30<<16)+155,(130<<16)+205,(30<<16)+155);break;
					case 0x03: Screen_PastePicture(0x0800,95,(30<<16)+205,(130<<16)+250,(30<<16)+205);break;
					case 0x04: Screen_PastePicture(0x0800,95,(30<<16)+255,(130<<16)+300,(30<<16)+255);break;
					case 0x05: Screen_PastePicture(0x0800,95,(30<<16)+305,(130<<16)+345,(30<<16)+305);break;
					case 0x06: Screen_PastePicture(0x0800,95,(30<<16)+355,(130<<16)+400,(30<<16)+355);break;
					case 0x07: Screen_PastePicture(0x0800,95,(650<<16)+105,(790<<16)+170,(650<<16)+105);break;
					case 0x08: Screen_PastePicture(0x0800,95,(654<<16)+200,(765<<16)+260,(654<<16)+200);break;
					case 0x09: Screen_PastePicture(0x0800,95,(720<<16)+375,(790<<16)+440,(720<<16)+375);break;
					default:break;

			}
		}
			 
		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(94); 		
     		for(int i=0;i<7;i++)
				{
				    FourBytes four;
					  GetU16dataFormFlash2Word(&four,FLASHIS_AdrWHJZ_YLJZ_DQY+i*2,pf);
					  sprintf((char*)p->Buffer,"%.2f",four.FloatData);
				    Screen_RefreshDataMulti(0x0200+i*0x20,p->Buffer,15*2);
				}				 
				 p->RunModeStat=2;				 
			 }
			break;
      case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
							{				
             									
								if(p->Key.Mode>=1&&p->Key.Mode<=7)
								{
									if(p->Key.bOutsideKeyboard != false)
									ScreenEnterNumberTwinkle(p,p->Key.Mode*0x20+0x01e0);
								}
          	
								p->DisValue[0]=Sp->DQPress.Value; //����ѹ
								p->DisValue[1]=Sp->BGPress.Value; //��ѹ(����ѹ)
								p->DisValue[2]=Sp->AGPress.Value; //��ѹ
								p->DisValue[3]=Sp->ACPress.Value; //��ѹ
								p->DisValue[4]=Sp->BCPress.Value; //����ѹ
								p->DisValue[5]=Sp->CCPress.Value; //����ѹ
						 
				
							for(int i=0;i<6;i++)
							{   if((i>=0)&&(i<3))
								sprintf((char*)p->Buffer,"%.2f",p->DisValue[i]);
								else
								sprintf((char*)p->Buffer,"%.0f",p->DisValue[i]);	
								
								Screen_RefreshDataMulti(0x0100+i*0x20,p->Buffer,15*2);
							}		 							
						  p->Timer=500;
						}			 
			 }
			break;
			}
}
//----
/* date 2020-1-17
	��������void Screen_WHJZ_YQCGQXZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-����������ѡ��
*/
void Screen_WHJZ_YQCGQXZ_Pag(Screen_DataStructure *p)
{
   
	  static u8 LastChoice=255;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	  Button_WHJZ_YQCGQXZ_Pag(0x08,1,p->DisValue);
	  if(p->RunMode!=0x8160) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
					case 0x01: Screen_PastePicture(0x0800,105,(546<<16)+84,(597<<16)+120,(546<<16)+84);break;
					case 0x02: Screen_PastePicture(0x0800,105,(546<<16)+120,(597<<16)+156,(546<<16)+120);break;
					case 0x03: Screen_PastePicture(0x0800,105,(546<<16)+156,(597<<16)+192,(546<<16)+156);break;
					case 0x04: Screen_PastePicture(0x0800,105,(546<<16)+192,(597<<16)+229,(546<<16)+192);break;
					case 0x05: Screen_PastePicture(0x0800,105,(546<<16)+229,(597<<16)+265,(546<<16)+229);break;
					case 0x06: Screen_PastePicture(0x0800,105,(546<<16)+265,(597<<16)+295,(546<<16)+265);break;
					case 0x07: Screen_PastePicture(0x0800,105,(546<<16)+300,(597<<16)+334,(546<<16)+300);break;
					case 0x08: Screen_PastePicture(0x0800,105,(720<<16)+375,(790<<16)+440,(720<<16)+375);break;
					default:break;

			}
		}
		if(LastChoice!=pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ])	
		{
			LastChoice=pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ];
			for(int i=0;i<7;i++){
				
			switch(LastChoice&0x01)
			{
					case 0x00: Screen_PastePicture(0x6900+i*0x20,104,(621<<16)+91,(644<<16)+113,(621<<16)+90+i*36);break;
					case 0x01: Screen_PastePicture(0x6900+i*0x20,105,(621<<16)+91,(644<<16)+113,(621<<16)+90+i*36);break;
					default:break;
			}
			LastChoice>>=1;
		}LastChoice=pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ];
		}	 
		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
				 Screen_ShowPage(104); 	
                 FourBytes		four;	
				 p->LastChoice=pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ];
							
          for(int i=0;i<7;i++)
          {
					 UpdataSensorFixDate(&four,FLASHIS_AdrO2AZSJ+i*2,pf,p,i);//O2
					}				 	 
				 p->RunModeStat=2;				 
			 }
			break;
     
			}
}
//----
/* date 2020-05-20
	��������void Screen_WHJZ_WHXZ_WDJZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-�¶�У׼
*/
void Screen_WHJZ_WHXZ_WDJZ_Pag(Screen_DataStructure *p)
{
	Flashisp_DataStructure* pf=Flashisp_GetData();
	Slave_DataStructure* Sp=Slave_GetData();  //20200525
	//��������
	Button_WHJZ_WDJZ_Pag(0x07,1,p->DisValue);
	if(p->RunMode!=0x8170) return;
	//ѡ���ͼ����
	if(p->Key.LastOpt!=p->Key.Opt)
	{ 
		p->Key.LastOpt=p->Key.Opt; 
		switch(p->Key.Opt)
		{
			case 0x01: Screen_PastePicture(0x0800,97,(30<<16)+130,(105<<16)+200,(30<<16)+130);break;
			case 0x02: Screen_PastePicture(0x0800,97,(30<<16)+210,(105<<16)+280,(30<<16)+210);break;
			case 0x03: Screen_PastePicture(0x0800,97,(30<<16)+280,(105<<16)+350,(30<<16)+280);break;
			case 0x04: Screen_PastePicture(0x0800,97,(30<<16)+355,(105<<16)+430,(30<<16)+355);break;
			case 0x05: Screen_PastePicture(0x0800,97,(640<<16)+98,(789<<16)+165,(640<<16)+98);break;
			case 0x06: Screen_PastePicture(0x0800,97,(640<<16)+176,(750<<16)+240,(640<<16)+176);break;
			case 0x07: Screen_PastePicture(0x0800,97,(720<<16)+375,(790<<16)+440,(720<<16)+375);break;						
			default:break;
		}
	}
			 
		//���ݸ���
    switch(p->RunModeStat)
	{
		case 0x01: //��̬������ʾ			
		{
			Screen_ShowPage(96);
//			Dis_Tips(p,(u8*)"��ʾ------",0x0480);	
			for(int i=0;i<4;i++)
			{
				sprintf((char*)p->Buffer,"%.2f",GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_WDJZ_LDYW+i*2));	////2020-05-20 ��FLASH��ַƫ����һ��λ,�˴�������
				Screen_RefreshDataMulti(0x0100+i*0x20,p->Buffer,15*2);

				sprintf((char*)p->Buffer,"%.2f",GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_WDJZ_BLYW+i*2)); //2020-05-20 ��FLASH��ַƫ����һ��λ,�˴�������
				Screen_RefreshDataMulti(0x0500+i*0x20,p->Buffer,15*2);
			}				 
			p->RunModeStat=2;				 
		}
		break;
		case 0x02: //��̬̬������ʾ			
		{
			if(p->Timer==0)
			{				
				if(p->Key.bOutsideKeyboard != false)
				{									
					if(p->Key.Mode>=1&&p->Key.Mode<=4)
					{
						ScreenEnterNumberTwinkle(p,p->Key.Mode*0x20+0x00e0);
					}else  if(p->Key.Mode>=5&&p->Key.Mode<=6)		
					{
						ScreenEnterNumberTwinkle(p,(p->Key.Mode-4)*0x20+0x04e0);
					}
				}
				p->DisValue[0]=Sp->GQTem.Value;//�������²���ֵSp->YWTem.Value;
				p->DisValue[1]=Sp->SQTem.Value;  //ʪ�����ֵ	
				p->DisValue[2]=Sp->JWTem.Value;  //���²���ֵ
				p->DisValue[3]=Sp->HWTem.Value;  //���²���ֵ	
				p->DisValue[4]=Sp->Data[SLAVE_MAP_YWTem].Raw;  //mV  ԭʼֵ  20200525
				p->DisValue[5]=Sp->Data[SLAVE_MAP_SQTem].Raw;  //mV  ԭʼֵ  20200525

				for(int i=0;i<4;i++)  //����ֵ
				{
					sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);
					Screen_RefreshDataMulti(0x0200+i*0x20,p->Buffer,15*2);
				}	
				for(int i=0;i<2;i++)   //mV
				{
					sprintf((char*)p->Buffer,"%d",(u32)p->DisValue[i+4]);     //20200525
					Screen_RefreshDataMulti(0x0400+i*0x20,p->Buffer,15*2);
				}		 							
				p->Timer=500;
					
				if(p->uTempState == 0)
				{
					Dis_Tips(p,(u8*)" ",0x0480);
				}
					
				if(p->Warning != 0)
				{   if(p->uMenuPosition==1)
					{
					if(p->Warning == 0xA1||p->Warning == 0xA2)
					{						
						
						Dis_Tips(p,(u8*)"����/����:�����100R����",0x0480);
					}
					else if(p->Warning == 0XA3||p->Warning == 0XA4)
					{
						
						Dis_Tips(p,(u8*)"����/����:�����174R����",0x0480);

					}
					else if(p->Warning == 0xAA)
					{
						Dis_Tips(p,(u8*)"����/����:100ŷ�����������",0x0480);
						p->Warning = 0xA1;				
						p->Timer = 2000;
					}
					else if(p->Warning == 0xAB)
					{
						Dis_Tips(p,(u8*)"����/����:174ŷ�����������",0x0480);
						p->Warning = 0xA3;
						p->Timer = 2000;
					}
					else if(p->Warning == 0xAF)
					{
						Dis_Tips(p,(u8*)"����/����:�¶ȱ궨��ɣ�",0x0480);
						p->Warning = 0;
						p->Timer = 2000;
					}					
					else
					{
						p->Warning = 0;
						p->Timer = 500;
					}	
				   }
				   else
				   {
					if(p->Warning == 0xA1||p->Warning == 0xA2)
					{						
						
						Dis_Tips(p,(u8*)"ʪ��:�����100R����",0x0480);
					}
					else if(p->Warning == 0XA3||p->Warning == 0XA4)
					{
						
						Dis_Tips(p,(u8*)"ʪ��:�����174R����",0x0480);

					}
					else if(p->Warning == 0xAA)
					{
						Dis_Tips(p,(u8*)"ʪ��:100ŷ�����������",0x0480);
						p->Warning = 0xA1;				
						p->Timer = 2000;
					}
					else if(p->Warning == 0xAB)
					{
						Dis_Tips(p,(u8*)"ʪ��:174ŷ�����������",0x0480);
						p->Warning = 0xA3;
						p->Timer = 2000;
					}
					else if(p->Warning == 0xAF)
					{
						Dis_Tips(p,(u8*)"ʪ��:�¶ȱ궨��ɣ�",0x0480);
						p->Warning = 0;
						p->Timer = 2000;
					}					
					else
					{
						p->Warning = 0;
						p->Timer = 500;
					}   
				   }
				}


			}
			
		 }
	break;
	}
}
//----
/* date 2020-05-20
	��������void Screen_WHJZ_WHXZ_DCBD_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-��ر궨
*/
void Screen_WHJZ_WHXZ_DCBD_Pag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  Battery_DataStructure* pb=Battery_GetData();
	 //��������
	  Button_WHJZ_DCBD_Pag(0x02,1,p->DisValue);
	  if(p->RunMode!=0x8180) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
			  
					case 0x01: Screen_PastePicture(0x0800,117,(200<<16)+195,(330<<16)+260,(200<<16)+195);break;
					case 0x02: Screen_PastePicture(0x0800,117,(720<<16)+375,(790<<16)+440,(720<<16)+375);break;
						
				default:break;

			}
		}
			 
		//���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
					Screen_ShowPage(116); 		
					Dis_Tips(p,(u8*)"",0x0140);
     
					sprintf((char*)p->Buffer,"%.4f",GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_DCJZ_BL));
				    Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);
				
							 
				 p->RunModeStat=2;				 
			 }
			break;
      case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
							{				
             									
								 if(p->Key.Mode>=1)
								{
									if(p->Key.bOutsideKeyboard != false)
										ScreenEnterNumberTwinkle(p,p->Key.Mode*0x20+0x00e0);
								}
							        p->DisValue[0]=pb->Battery.ActualV; //20200528
									sprintf((char*)p->Buffer,"%.2f",p->DisValue[0]);
									Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);
				
								 							
						  p->Timer=500;
						}			 
			 }
			break;
			}
}
//----
/* date 2020-05-21
	��������void Screen_JZWH_CJWH_Pag(Screen_DataStructure *p)
	������	У׼ά��-����ά��-�����߼�����
*/
void Screen_JZWH_CJWH_Pag(Screen_DataStructure *p)
{
//	FourBytes four;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
		  Button_WHJZ_CJWH_Pag(4,1,p->DisValue);
	  if(p->RunMode!=0x8200) return;
			//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
			  
						  case 0x01: Screen_PastePicture(0x0800,107,(140<<16)+117,(345<<16)+160,(140<<16)+117);break;
						  case 0x02: Screen_PastePicture(0x0800,107,(140<<16)+220,(360<<16)+260,(140<<16)+220);break;
						  case 0x03: Screen_PastePicture(0x0800,107,(140<<16)+320,(460<<16)+365,(140<<16)+320);break;
						  case 0x04: Screen_PastePicture(0x0800,107,(711<<16)+375,(795<<16)+441,(711<<16)+375);break;
						
				default:break;

			}
		}
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
					Screen_ShowPage(106); 
					Dis_Tips(p,(u8*)"",0x0120);    
					FourBytes four;
					four.U16Data[0]=pf->Data[FLASHIS_AdrCJWHYQBH];
					four.U16Data[1]=pf->Data[FLASHIS_AdrCJWHYQBH+1];
					sprintf((char*)p->Buffer,"%08d",four.U32Data);	//��Ϊ8λ��ʾ��by alfred, 2020.06.04
					Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);		 			 
					p->RunModeStat=2;	
				    p->Function=0;
			 }

			break;
			case 0x02: //��̬̬������ʾ			
			{
				if(p->Timer==0)
				{										
					 if(p->Key.Mode>=1)
					{
						if(p->Key.bOutsideKeyboard != false)
						ScreenEnterNumberTwinkle(p,p->Key.Mode*0x20+0x00e0);
					}			 							

					
					if(p->Function==1)
					{
						Screen_RefreshDataMulti(0x0120,(u8 *)"���ڱ���",15*2);	
						p->Function=2;
					}
					else if(p->Function==2)
					{
					   Screen_RefreshDataMulti(0x0120,(u8 *)"�������",15*2);	
						p->Function=3;
					}
					else if(p->Function==3)
					{
					  Screen_RefreshDataMulti(0x0120,(u8 *)"        ",15*2);	
						p->Function=0;
					}
					
					
                  	p->Timer=500;
										
				}			 
			}
			break;
		}
	
}
//---
//----
/* date 2020-0305-17
	��������void Screen_WHJZ_CJWH_QTCGQPZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-����ά��-����������ѡ��
*/
void Screen_WHJZ_CJWH_QTCGQPZ_Pag(Screen_DataStructure *p)
{
   
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	  Button_WHJZ_CJWH_QTCGQXZ_Pag(0x0F,1,p->DisValue);  //һ����15��0xf
	  if(p->RunMode!=0x8210) return;
		//ѡ���ͼ����
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)         //һ��˵�����
			{
				case 0x01: Screen_PastePicture(0x0800,109,(467<<16)+85,(513<<16)+120,(467<<16)+85);break;
				case 0x02: Screen_PastePicture(0x0800,109,(467<<16)+120,(513<<16)+156,(467<<16)+120);break;
				case 0x03: Screen_PastePicture(0x0800,109,(467<<16)+156,(513<<16)+192,(467<<16)+156);break;
				case 0x04: Screen_PastePicture(0x0800,109,(467<<16)+192,(513<<16)+229,(467<<16)+192);break;
				case 0x05: Screen_PastePicture(0x0800,109,(467<<16)+229,(513<<16)+265,(467<<16)+229);break;
				case 0x06: Screen_PastePicture(0x0800,109,(467<<16)+265,(513<<16)+295,(467<<16)+265);break;
				case 0x07: Screen_PastePicture(0x0800,109,(467<<16)+300,(513<<16)+334,(467<<16)+300);break;
				case 0x08: Screen_PastePicture(0x0800,109,(720<<16)+375,(790<<16)+440,(720<<16)+375);break;
				
				case 0x09: Screen_PastePicture(0x0800,109,(267<<16)+85,(448<<16)+120,(267<<16)+85);break;
				case 0x0A: Screen_PastePicture(0x0800,109,(267<<16)+85,(448<<16)+120,(267<<16)+120);break;
				case 0x0B: Screen_PastePicture(0x0800,109,(267<<16)+85,(448<<16)+120,(267<<16)+155);break;
				case 0x0C: Screen_PastePicture(0x0800,109,(267<<16)+85,(448<<16)+120,(267<<16)+193);break;
				case 0x0D: Screen_PastePicture(0x0800,109,(267<<16)+85,(448<<16)+120,(267<<16)+228);break;
				case 0x0E: Screen_PastePicture(0x0800,109,(267<<16)+85,(448<<16)+120,(267<<16)+264);break;
				case 0x0F: Screen_PastePicture(0x0800,109,(267<<16)+85,(448<<16)+120,(267<<16)+299);break;
					
				default:break;

			}
		}
		if(p->LastChoice!=pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ])	  //ѡ���
		{
			p->LastChoice=pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ];
			p->LastChoice=p->LastChoice>>8;
			for(int i=0;i<7;i++)
			{
				
			switch(p->LastChoice&0x01)
			{
					case 0x00: Screen_PastePicture(0x6900+i*0x20,108,(522<<16)+91,(546<<16)+113,(522<<16)+90+i*36);break;
				  case 0x01: Screen_PastePicture(0x6900+i*0x20,109,(522<<16)+91,(546<<16)+113,(522<<16)+90+i*36);break;
					default:break;
			}
			p->LastChoice>>=1;
		}
			p->LastChoice=pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ];
		}	 
		//���ݸ��� TXT
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ	 ,�����и���ʱ������1������		
			 {
					Screen_ShowPage(108); 
					FourBytes		four;	
					four.U16Data[0]=pf->Data[FLASHIS_AdrO2LQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrO2LQ+1];
					sprintf((char*)p->Buffer,"0%%~%02d%%",four.U32Data);	//O2����0~30
					Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);	

					four.U16Data[0]=pf->Data[FLASHIS_AdrSO2LQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrSO2LQ+1];
					sprintf((char*)p->Buffer,"0~%05d",four.U32Data);	//
				  //	sprintf((char*)p->Buffer,"0-5700ppm");//SO2����
					Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);

					four.U16Data[0]=pf->Data[FLASHIS_AdrNOLQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrNOLQ+1];
					sprintf((char*)p->Buffer,"0~%05d",four.U32Data);	//
					//sprintf((char*)p->Buffer,"0-1300ppm");//NO����
					Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);
				 
					four.U16Data[0]=pf->Data[FLASHIS_AdrNO2LQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrNO2LQ+1];
					sprintf((char*)p->Buffer,"0~%05d",four.U32Data);	//
					//sprintf((char*)p->Buffer,"0-200ppm");//NO2����
					Screen_RefreshDataMulti(0x0160,p->Buffer,15*2);
					
					four.U16Data[0]=pf->Data[FLASHIS_AdrCOLQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrCOLQ+1];
					sprintf((char*)p->Buffer,"0~%05d",four.U32Data);	//
					//sprintf((char*)p->Buffer,"0-5000ppm");//cO����
					Screen_RefreshDataMulti(0x0180,p->Buffer,15*2);
					
					four.U16Data[0]=pf->Data[FLASHIS_AdrH2SLQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrH2SLQ+1];
					sprintf((char*)p->Buffer,"0~%05d",four.U32Data);	//
					//sprintf((char*)p->Buffer,"0-75ppm");//H2S����
					Screen_RefreshDataMulti(0x01A0,p->Buffer,15*2);
					
					four.U16Data[0]=pf->Data[FLASHIS_AdrCO2LQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrCO2LQ+1];
					sprintf((char*)p->Buffer,"0%%~%02d%%",four.U32Data);	//
					//sprintf((char*)p->Buffer,"0-200ppm");//CO2����
					Screen_RefreshDataMulti(0x01C0,p->Buffer,15*2);			
				 
					p->LastChoice=pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ];
					p->LastChoice=p->LastChoice>>8;
                   for(int i=0;i<7;i++)
                   {
					 UpdataSensorFixDate(&four,FLASHIS_AdrO2AZSJ+i*2,pf,p,i);//   ��ʾ��װʱ��
					}				 	 
				 p->RunModeStat=2;				 
			 }
			break;
			 case 0x02: //��̬̬������ʾ			
			 {
				 if(p->Timer==0)
							{										
								if((p->Key.Mode>=1)&&(p->Key.Mode<=7)&&(p->Key.bOutsideKeyboard != false))  //��װ�����������������
								{                                                                           //��˸��Ӧ��λ�ַ�
										ScreenEnterNumberTwinkle(p,p->Key.Mode*0x20+0x0400);
								}	
								
								if((p->Key.Mode>=8)&&(p->Key.Mode<=14)&&(p->Key.bOutsideKeyboard != false)) //���������������������
								{                                                                            //��˸��Ӧ��λ�ַ�
										ScreenEnterNumberTwinkle(p,(p->Key.Mode-8)*0x20+0x0100);
								}	
								
						    p->Timer=500;
						  }			 
			 }
			 break;
     
			}
}
//----

/* date 2020-5-13 hjl add
	��������void Screen_KJ_MMSR_Pag(Screen_DataStructure *p)
	������	����-��������-�����߼�����
*/
void Screen_KJ_MMSR_Pag(Screen_DataStructure *p)
{
	FourBytes four;
	static u8 Flag = 0;

	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	//��������
	Button_KJ_MMSR_Pag(2,1,p->DisValue);
	if(p->RunMode!=101) return;
	
	//ѡ���ͼ����
	if(p->Key.LastOpt!=p->Key.Opt)
	{ 
	  	p->Key.LastOpt=p->Key.Opt; 
		switch(p->Key.Opt)
		{
			case 0x01: Screen_PastePicture(0x0800,161,(236<<16)+196,(363<<16)+250,(236<<16)+196);break;
			default:break;
		}
	}
	//���ݸ���
	switch(p->RunModeStat)
	{
		case 0x01: //��̬������ʾ			
		{
			
			Screen_ShowPage(160); 
			sprintf((char*)p->Buffer,"         ");
			Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);		
			//if(p->Timer <= 0)
			{
				p->RunModeStat=2;
				if(Flag == 0)
				{
					Flag = 1;
					//��ʾ�������
					four.U16Data[0]=pf->Data[FLASHIS_AdrCJWHYQBH];
					four.U16Data[1]=pf->Data[FLASHIS_AdrCJWHYQBH+1];
					sprintf((char*)p->Buffer,"�������:%010d",four.U32Data);
					Screen_RefreshDataMulti(0x0500,p->Buffer,15*2);	
					//��ʾ�����֤��
					sprintf((char*)p->Buffer, "��֤��:%04d", GetRandomNum());
					Screen_RefreshDataMulti(0x0400,p->Buffer,15*2);	
				}
			}
		}
		break;
		
		case 0x02: //��̬������ʾ
		{			
			if(p->Timer==0)
			{	
				if(p->Warning==1)
				{
					Dis_Tips(p,(u8 *)"�����������",0x0120);			
					p->Timer=2000;
					p->Warning=0;
				}
				else
				{
					Dis_Tips(p,(u8 *)"            ",0x0120);	
					if(p->Key.Mode==1)
					{
						ScreenEnterNumberTwinkle(p,0x0100+(p->Key.Mode-1)*0x20);
					}			 							
					p->Timer=500;
				}
			}	
		}
		break;
	}
}


