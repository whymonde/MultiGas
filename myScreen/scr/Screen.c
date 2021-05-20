/*
	���ļ�Ϊ����Ŀ����Ļ��غ���
	���庯������
*/
#include "button.h"
#include "Screen.h"
#include "Screenlogic.h"
#include "ScreenDwFunction.h"
#include "usbh_usr.h"
#include <stdbool.h>
Screen_DataStructure ScreenData;

u8 uScreenRcvBuffer[15];

u8* RcvBuffer_GetData(void)
{
	return &uScreenRcvBuffer[0];
}
/*
	��������void Screen_Main(void)
	������	��Ļ��ʾ������
*/
void Screen_Main(void)
{

	Screen_SecClock();//��ȡ��Ļʱ��
	Screen_DealWihtReceiveData();//��ȡ��Ļ����
	Screen_BacklightDW();//���⴦��
	Screen_changPowerState();  //��س�����
	Screen_ShowScreenLockState();//����������
	Screen_WaterPumServo();  //ˮ�ü��  20200528
	Screen_ShowWifi();       //E28ͼ��
	Screen_ScanSchack();     //�����Ļ����¼��״̬�Ĵ���
	//---
	if(ScreenData.RunMode < 0x1000)//������
		Screen_MainPage(&ScreenData);
	else if(ScreenData.RunMode < 0x2000)//ϵͳ����
	{
	  Screen_SystemSet(&ScreenData);
	}	
	else if(ScreenData.RunMode < 0x3000)//�̵�����
	{ 
	   Screen_FlueSet(&ScreenData);
	}	
	else if(ScreenData.RunMode < 0x4000)//����Ԥ��
	{
	  Screen_WorkingConditionPrediction(&ScreenData);
	}	
	else if(ScreenData.RunMode < 0x5000)//�⺬ʪ��
	{
	   Screen_MeasureHumidity(&ScreenData);
	}	
	else if(ScreenData.RunMode < 0x6000)//��������
	{
	   Screen_MeasureSomke(&ScreenData);
	}	
	else if(ScreenData.RunMode < 0x7000)//�̳��ɼ�
	{
	   Screen_SmokeSampling(&ScreenData);
	}
	else if(ScreenData.RunMode < 0x8000)//�ļ���ѯ
	{
	  Screen_FileSeek(&ScreenData);
	}
	else if(ScreenData.RunMode < 0x9000)//У׼ά��
	{
	  Screen_CalibrationFix(&ScreenData);
	}
	else if(ScreenData.RunMode < 0xA000)
	{
		Screen_Backlight(&ScreenData);//���뱳�����
	}
	else if(ScreenData.RunMode < 0XB000)
	{
		Screen_PowerDowm(&ScreenData);//�ػ�
	}
	
	 ScreenButtonReset(&ScreenData);
	//---
}
//---
/*
	��������void Screen_Timer(void)
	������screen.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void Screen_Timer(void)
{   if(ScreenData.Scan_Stimer!=0)ScreenData.Scan_Stimer--;
	if(ScreenData.Timer != 0) ScreenData.Timer--;	
	if(ScreenData.ChangTimer != 0) ScreenData.ChangTimer--;	
	if((ScreenData.sReveiveDataTimer != 0)&&(ScreenData.uEnableRecDataTimer == 1)) ScreenData.sReveiveDataTimer--;	//������Ļ���ݳ�ʱ����ʱ��
	if(ScreenData.BatflshTimer>0)ScreenData.BatflshTimer--;
	if(ScreenData.Clock.Msec<500) ScreenData.Clock.Msec++;//����Ļʱ��
 
	
	if(ScreenData.JiPingJun.Timer != 0) ScreenData.JiPingJun.Timer--;	
//	if(ScreenData.uCounter1ms%1000==0) ScreenData.uCounter1S++;	//������1��
	if(ScreenData.JiPingJun.uCounter1ms == 0)
	{
		ScreenData.JiPingJun.uCounter100ms = 0;
		ScreenData.JiPingJun.uCounter1S = 0;
		ScreenData.JiPingJun.bTime100ms = false;
		ScreenData.JiPingJun.bTime1S = false;
	}
	if(ScreenData.JiPingJun.uCounter1ms>100) 
	{
		ScreenData.JiPingJun.uCounter1ms = 0;
		ScreenData.JiPingJun.bTime100ms = true;		
	}
	ScreenData.JiPingJun.uCounter1ms++;
	
	if(ScreenData.uE28Timer>0)ScreenData.uE28Timer--;

}

void ScreenJPG_Timer(void)
{
	ScreenData.JiPingJun.bTime1S = true;
}

//---
/*
	��������void Screen_SecTimer(void)
	�������ú���ΪScreen.c�ļ����뺯��,��ϵͳ�뺯������
    ִ��ʱ����¡�ͼ����µ�
*/
void ScreenJPG_ClearSum(void)
{
	u8 i;
    for(i=0;i<8;i++)
	ScreenData.JiPingJun.JpjSum[i]=0;

	ScreenData.JiPingJun.JpjSuncnt=0;	

}

//---
/*
	��������void Screen_SecTimer(void)
	������	�ú���ΪScreen.c�ļ����뺯������ϵͳ�뺯������
  ִ��ʱ����¡�ͼ����µ�
*/
void Screen_SecTimer(void)
{
	if(ScreenData.TimerSec>0) ScreenData.TimerSec--;
	 if(ScreenData.WaterPumTimer != 0) ScreenData.WaterPumTimer--; 
	
	if(ScreenData.staflg)                 //�����ԼӶ�ʱ����־
	{
		if(ScreenData.ZjTimer<10000)  ScreenData.ZjTimer++;
	}

	if(ScreenData.BacklightSecTimer < 30*60) ScreenData.BacklightSecTimer++;
    Screen_ShowDynamicData(); 
//YC_SampleControl();//20200604
}
//---
/*
	��������void Screen_Init(void)
	������	screen.c�ļ���ʼ������
*/
void Screen_Init(void)
{
	Screen_InitData();
	Screen_ColckInit();
	Screen_WaterPumOpenServo();  //�⺯��Ҫ�ڶ���EEPROM���ݺ�
}
//---
/*
	��������Screen_DataStructure* Screen_GetData(void)
	��������ȡ����screen.c�ļ�ȫ�ֱ����ṹ��
*/
Screen_DataStructure* Screen_GetData(void)
{
	return &ScreenData;
}
//---
/*20200529
	��������void Screen_MainPage(Screen_DataStructure *p)
	������	��ʾ���������,�����������������桢������
	RunMode��Χ��1~99
*/

void Screen_MainPage(Screen_DataStructure *p)
{ //u32 buf;
	switch(p->RunMode)
	{
	  
		case 1: 
		{//�ȴ���Ļ�ϵ��ȶ�
			p->Timer = 1000;
			p->RunMode = 2;	//2020-05-19 hjl ���ӿ���������ʾ��RunMode = 100�� //2020-05-22��ʱʱ��ʱ��Ϊ51, by Alfred.
			p->RunModeStat=1;
			break;
		}
		case 2:
		{//��ʾ����
			if(p->Timer<=0)
			{
			Screen_WriteRegisterOneData(0x02,50);//���÷����� 50*10ms
			Screen_ShowPage(0);
			Screen_WriteRegisterOneData(0x01,0x40);//�������� 0x00 --0x 40
			p->Timer = 2000;
			p->RunMode = 3;
			}
			break;
		}case 3:
		{//��ʾ����
			if(p->Timer<=0)
			{			  
			Screen_ShowPage(1);			
			p->Timer = 3000;
			p->RunMode = 4;
			}
			break;
		}
		case 4:
		{//�Ȱ�����Ӧ
			if((p->Button == 0) || (p->Timer == 0)) 
			{p->RunMode = 5;
			}
			break;
		}
		case 5:
		{//�����������ļ� 
		//	if(1)   //��������ļ� ���ļ�����1
		//	{
		//	  p->RunMode = 6;
		//	  Screen_ShowPage(2);
		////	  p->Key.Opt=2;		
		//	}else  //������ת���ļ����
			{
			  p->RunMode = 11;
			}
			break;
		}
		case 6:
		{//�����������洦��
			
			//	Screen_programUpdata(p);
			
			break;
		}
		case 11:
		{//��ʾ�Լ����
			Screen_ShowPage(4);
			p->RunMode = 12;
			p->Key.Opt=1;
			break;
		}
		case 12:
		{//�Լ�����߼�����
			Screen_AutoCheck(p);
			break;
		}
		case 13:
		{
			p->RunMode =100;
			p->Timer=1000;
			break;
		}
       case 14:
		{
			if((Flashisp_GetData()->Data[FLASHIS_AdrCJMOD]>=2)&&(Flashisp_GetData()->Data[FLASHIS_AdrCJMOD]<=4))  //�ж��Ƿ���������е���
			{   p->RunMode =31;
				p->Key.Opt=1;   //Ԥ��ѡ���˳�ѡ��
				p->Key.LastOpt=0;
				p->Key.Mode=0; //һ���˵�
				p->RunModeStat=1; //��̬��ʾ
			}
			else
			p->RunMode =51;	
			break;

		}
		case 31:
		{
			Screen_PowerOffPag(p);

			break;
		}
	
		//------------------------------------------------------------
		case 51:
		{//����������
			Screen_ShowPage(8);
			p->Key.OldOpt=0; //��ѡ��
			p->Key.Opt=1; 	//��ѡ��
			p->RunMode = 52;
			
			Power_SetFlg(); //�����������
			break;
		}
		case 52:
		{//��̬������ʾ
			Screen_MainLayer(p);
			//p->RunMode = 52;
			break;
		}

		case 100:
			//2020-05-19 hjl add
			//�ж��Ƿ������������
		{
			if(p->Timer <= 0)
			{
				if(Slave_GetData()->Stm8Data.Data == 0xFE)
				{
					p->RunMode = 101;
					Power_SetFlg(); //�����������
					p->Key.LastOpt=0;
					p->Key.Opt=1;
			  }
			else
			{
				p->RunMode = 14;
			}
			}	
			p->RunModeStat=1;
			break;
		}
		case 101:
		{
			//2020-05-19 hjl add
			//���������������
			
			Screen_KJ_MMSR_Pag(p);
			break;
        }
		}
}


//---
/*
	��������void Screen_SysSet(Screen_DataStructure *p)
	������	ϵͳ�������
	RunMode��Χ��0x1000-0x1fff
	�޸ļ�¼��1. ɾ��ϵͳ����ҳ, modify by alfred, 2020.06.13
*/
void Screen_SystemSet(Screen_DataStructure *p)
{
	
	switch(p->RunMode)
	{
		case 0x1000:
		{
			Screen_SystemSetPag(p);
			break;
		} 
//		case 0x1100:
//		{
//		  Screen_Sys_CYSZPage(p);
//			break;
//		}
		default: break;
	}
}

//---
/*
	��������void Screen_YanDaoBuDianSet(Screen_DataStructure *p)
	������	�̵��������ý���
	RunMode��Χ��0x2000-0x2fff
*/
void Screen_FlueSet(Screen_DataStructure *p)
{
  	switch(p->RunMode)
	{
		case 0x2000:
		{
			Screen_FluePag(p);
			break;
		} 
		case 0x2100:
		{
			Screen_FlueCircularPag(p); //Բ��
			break;
		} 
		case 0x2200:
		{
			Screen_FlueSquarePag(p);  //����
			break;
		} 
		case 0x2300:
		{
			Screen_FlueOtherPag(p);   //����
			break;
		}
	    case 0x2400:
		{
			Screen_FlueSeekPag(p);   //�̵���ѯ
			break;
		} 		
		
	}
}
//---
/*
	��������void Screen_WorkingConditionPrediction(Screen_DataStructure *p)
	������	����Ԥ�����ý��� 
	RunMode��Χ��0x3000-0x3fff
*/
void Screen_WorkingConditionPrediction(Screen_DataStructure *p)
{
  	switch(p->RunMode)
	{
		case 0x3000:
		{
			Screen_WCP_SetPag(p);
			break;
		} 
		case 0x3100:
		{
			Screen_WCP_PressureZeroPag(p); //ѹ������
			break;
		} 
		case 0x3200:
		{
			Screen_WCP_FumeTemperaturPag(p);  //���²���
			break;
		} 
		case 0x3300:
		{
			Screen_WCP_PredictedVelocityPag(p);   //Ԥ������
			break;
		}
		case 0x3400:
		{
			Screen_WCP_CheckAveragePag(p);   //��ƽ��ֵ
			break;
		}		
	}
}
//---

/*
	��������void Screen_WorkingConditionPrediction(Screen_DataStructure *p)
	������	�⺬ʪ�����ý��� 
	RunMode��Χ��0x3000-0x3fff
*/
void Screen_MeasureHumidity(Screen_DataStructure *p)
{
  	switch(p->RunMode)
	{
		case 0x4000:
		{
			Screen_CHSL_SetPag(p); //ѡ��
			break;
		} 
		case 0x4100:
		{
			Screen_CHSL_GSQF_Pag(p); //��ʪ��
			break;
		} 
		case 0x4200:
		{
			Screen_CHSL_ZRF_Pag(p);  //���ݷ�����
			break;
		} 
//			case 0x4400:	//
//		{
//			Screen_CHSL_HSLCX_Pag(p);  //��ʪ����ѯ
//			break;
//		}
		
	
	}
}
//---
/*
	��������void Screen_WorkingConditionPrediction(Screen_DataStructure *p)
	������	�����������ý��� 
	RunMode��Χ��0x5000-0x5fff
*/
void Screen_MeasureSomke(Screen_DataStructure *p)
{
	
	if(p->JiPingJun.cIsCalcAverage)	//������ڼ�ƽ����ֱ��������ƽ��ҳ��.
		p->RunMode = 0x5230;
	
  	switch(p->RunMode)
	{
		case 0x5000:
		{
			 Screen_CLYQ_SetPag(p);//ѡ��
			break;
		} 
		case 0x5100:
		{
		 Screen_CLYQ_YQJZ_Pag(p);//����У׼
			break;
		} 
	
		case 0x5200:
		{
		 Screen_CLYQ_KSJC_Pag(p); //��ʼ����
			break;
		}		
		case 0x5210:
		{
		 Screen_CLYQ_CLJS_Pag(p);//������������
			break;
		}
			case 0x5220:
		{
		 Screen_CLYQ_YQAX_Pag(p);//��ǹ��ϴ
			break;
		}
			case 0x5230:
		{
			Screen_CLYQ_JIPINGJUN_Pag(p);//��ƽ��
			break;
		}		
	 
			case 0x5300:
		{
			  Screen_CLYQ_YQCX_Pag(p);//������ѯ
			break;
		}
		case 0x5310:
		{
			  Screen_CLYQ_YQCX_JS_Pag(p);//������ѯ-����
			break;
		} 		
	  	case 0x5311:
		{
			  Screen_CLYQ_YQCX_JS_YQSR_Pag(p);//������ѯ-����-��������
			break;
		}
			case 0x5320:
		{
			  Screen_CLYQ_YQCX_WJGL_Pag(p);//������ѯ-�ļ�����
			break;
		} 
		case 0x5330:
			{
			  Screen_CLYQ_YQCX_DYXX_Pag(p);//������ѯ-��ӡѡ��
			break;
		} 
		case 0x5340:
		{
			Screen_CLYQ_YQCX_FZSJ_Pag(p);//������ѯ-��������
			break;
		} 
		
	}
}
//---
/*
	��������void Screen_SmokeSampling(Screen_DataStructure *p)
	������	�̳��������ý��� 
	RunMode��Χ��0x6000-0x6fff
*/
void Screen_SmokeSampling(Screen_DataStructure *p)
{
  	switch(p->RunMode)
	{
		case 0x6000:
		{
			Screen_YCCY_CYSZ_Pag(p); //����
			break;
		} 
		case 0x6100:
		{
			Screen_YCCY_QMJC_Pag(p);//�����Լ��
			break;
		} 
		case 0x6200:
		{
			Screen_YCCY_KSJC_Pag(p);//��ʼ����

			break;
		}
	  case 0x6210:
		{
			Screen_YCCY_CLJS_Pag(p);//��������
			break;
		}
   case 0x6220:
		{
			Screen_YCCY_YCFDX_Pag(p);//�̳�������
			break;
		}  		

	}
}
//---
/*
	��������void Screen_FileSeek(Screen_DataStructure *p)
	������	�ļ���ѯ���ý��� 
	RunMode��Χ��0x7000-0x7fff
*/
void Screen_FileSeek(Screen_DataStructure *p)
{
  	switch(p->RunMode)
	{
		case 0x7000:
		{
			Screen_YCCY_YCCX_Pag(p);//�ļ�ѡ��
			break;
		} 
		case 0x7100:
		{
		    Screen_YCCY_YCCX_JS_YC_Pag(p);//�̳�����
			break;
		} 
		case 0x7200:
		{
		    Screen_YCCY_YCCX_WJGL_Pag(p);//�ļ�����
			break;
		} 
			case 0x7300:
		{
			Screen_YCCY_YCCX_DYXX_Pag(p);//��ӡѡ��
			break;
		}
		case 0x7400:
		{
			Screen_YCCY_YCCX_JS_YY_Pag(p);//���̼���
			break;
		} 		
	
	}
}
//---
/*
	��������void Screen_FileSeek(Screen_DataStructure *p)
	������	У׼ά�����ý��� 
	RunMode��Χ��0x8000-0x8fff
*/
void Screen_CalibrationFix(Screen_DataStructure *p)
{
  	switch(p->RunMode)
	{
		case 0x8000:
		{
			Screen_JZWH_MMSR_Pag(p);//��������ѡ��
			break;
		} 
		case 0x8100:
		{
		 Screen_JZWH_WHXZ_Pag(p);//����ά��ѡ��
			break;
		} 
		case 0x8110:
		{
		  Screen_WHJZ_WHXZ_STSZ_Pag(p);//ϵͳ����
			break;
		} 
			case 0x8111:
		{
		  Screen_WHJZ_WHXZ_STSZ_CO_SO2_Pag(p);//CO ��SO2У׼
			break;
		} 
		
			case 0x8120:
		{
			  Screen_WHJZ_WHXZ_LLJZ_Pag(p);//����У׼
			break;
		} 
			case 0x8130:
		{
			  Screen_WHJZ_WHXZ_MMSZ_Pag(p);//�������
			break;
		} 
	  	case 0x8140:
		{
			  Screen_WHJZ_WHXZ_NDJZ_Pag(p);//Ũ��У׼
			break;
		}
		  	case 0x8141:
		{
			  Screen_WHJZ_WHXZ_NDJZ_BLSZ_Pag(p);//Ũ�ȱ�������У׼
			break;
		}

			case 0x8150:
		{
			  Screen_WHJZ_WHXZ_YLJZ_Pag(p);//ѹ��У׼
			break;
		}
	case 0x8160:
		{
			  Screen_WHJZ_YQCGQXZ_Pag(p);//������ѡ��
			break;
		} 
	case 0x8170:
		{
			  Screen_WHJZ_WHXZ_WDJZ_Pag(p);//�¶�У׼
			break;
		} 
  	case 0x8180:
		{
			  Screen_WHJZ_WHXZ_DCBD_Pag(p);//���У׼
			break;
		}
		 	case 0x8200:
		{
			  Screen_JZWH_CJWH_Pag(p);//����ά��
			break;
		}
		case 0x8210:
		{
			  Screen_WHJZ_CJWH_QTCGQPZ_Pag(p);//����ά��-���崫����ѡ��
			break;
		}
		
 		
	}
}
//---
/*
	��������void Screen_Backlight(Screen_DataStructure *p)
	������	�ú���Ϊ������Ļ������ش���
*/
void Screen_Backlight(Screen_DataStructure *p)
{
	switch(p->RunMode)
	{
		case 0X9001:
		{//���ּ��̽�����ʾ
		//	Screen_ShowPage(205);
			p->RunMode = 0X9002;
			break;
		}
		case 0X9002:
		{//��̬������ʾ
			p->RunMode = 0X9003;
			break;
		}
		case 0X9003:
		{//��̬������ʾ
			p->RunMode = 0X9090;
			break;
		}
		case 0X9090:
		{//����������
			if((p->Button != 0xFFFF) || (p->Key.KeyData != SCREEN_NONE_KEY))
			{
				Screen_ShowPage(p->OldPage);
				p->Key.Mode = 0;
				p->Key.OldOpt = 0;
				p->Key.KeyData = SCREEN_NONE_KEY;
				p->Button = 0xFFFF;
				ScreenData.RunMode = ScreenData.OldMode;
			}
			break;
		}
	}
}
//---
/*
	��������Screen_PowerDowm(Screen_DataStructure *p)
	������	�ػ�
*/
void Screen_PowerDowm(Screen_DataStructure *p)
    {
	 switch(p->RunMode)
	   {
		case 0XA100:
			//��ʾ����ҳ
		    Screen_ShowPage(162);
		    //��Ӳ��
		    CTRbat_OUT(1);  //�ص�Դ
			CTRexPower_OUT(1);
		    //�ظ���
		    Flux_StopSampling('B');
		    Flux_StopSampling('C');
		    Slave_CloseWaterPump();//�رճ�ˮ��
		
		    //�ط���
		    Fan_Close();
		    p->RunMode=0XA101;
			break;
		case 0XA101:
			
			break;
	   }
    }

/*
	��������void Screen_BacklightDW(void)
	������	���⴦����
*/
void Screen_BacklightDW(void)
{
	if(1)
	{
		if((ScreenData.Button != 0xFFFF) || (ScreenData.Key.KeyData != SCREEN_NONE_KEY))
		{
			Screen_WriteRegisterOneData(0x01,0x40);
			ScreenData.BacklightSecTimer = 0;
		}
		else if(ScreenData.BacklightSecTimer == 30*60)
		{
			ScreenData.BacklightSecTimer += 10;
			//Screen_WriteRegisterOneData(0x4F,0x01);
			//Screen_WriteRegisterOneData(0x4F,0x23);
			Screen_WriteRegisterOneData(0x01,00);
			ScreenData.OldMode = ScreenData.RunMode;
			ScreenData.RunMode = 0X9001;
		}
	}
}


//---
/*
	��������void Screen_ShowDynamicData(void)
  ����ײ�ʱ�䡢����ѹ��⴦��ͼ�괦��
*/
void Screen_ShowDynamicData(void)
{   FourBytes four;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	Screen_DataStructure* p = &ScreenData;
	struct tm *pt = &ScreenData.Clock.Clock;
	char * buf[50]={0};
	Screen_ResetDataBuffer();
	sprintf((char*)p->Buffer,"%02d:%02d:%02d",pt->tm_hour,pt->tm_min,pt->tm_sec);//ʱ��
	Screen_RefreshDataMulti(0x0320,p->Buffer,15*2);
	Screen_ResetDataBuffer();
	
	if(Power_GetData()->starFlg ==1)
	{
		sprintf((char*)p->Buffer,"%02d���رյ�Դ",(3-(u8)Power_GetData()->Tcnt/10));
		
		if((3-(u8)Power_GetData()->Tcnt/10)<=0)
		{
			p->RunMode=0XA100;
		}
	}
	else
	{
	if(Flux_GetData()->ChannelB.RunMode==2)
	{
		sprintf((char*)&buf,"�̳�������");
	}
	else if(Flux_GetData()->ChannelB.RunMode==3)
	{
		sprintf((char*)&buf,"�̳���ͣ  ");
	}	
	else if(Flux_GetData()->ChannelB.RunMode==4)
	{
		sprintf((char*)&buf,"�̳�������");
	}
	else if(Flux_GetData()->ChannelB.Sampling.FdxFlg)
	{
		sprintf((char*)&buf,"�̳�δ����");
	}
	else
	{
		
					if((pf->Data[FLASHIS_AdrXTSZBOX]&(1>>0))==0x0000)//����ѹ����ֵ
						{//����ѹ
						
					    if(Slave_GetData()->DQPress.Value == FLUX_ColFAULT) 
						sprintf((char*)&buf,"����");
					    else 
						sprintf((char*)&buf,"%0.1fkPa",Slave_GetData()->DQPress.Value);
				        }
				        else
				        {//����ѹ����ֵ
					     four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrXTSZDQY);
					     sprintf((char*)&buf,"%0.1fkPa",four.FloatData);
				        }
		
		
		
	}
	strcpy((char*)p->Buffer,(char*)buf); 
	if(Flux_GetData()->ChannelC.RunMode==2)
	{
		sprintf((char*)&buf," ����������");
	}
	else if(Flux_GetData()->ChannelC.RunMode==3)
	{
		sprintf((char*)&buf," ������ͣ  ");
	}
	else if(Flux_GetData()->ChannelC.RunMode==4)
	{
		sprintf((char*)&buf," ������ϴ  ");
	}
	else if(Flux_GetData()->ChannelC.Sampling.EndFlg)
	{
		sprintf((char*)&buf," ����δ����");
	}
	else
	{
		sprintf((char*)&buf,"           ");
	}
	
	strcat((char*)p->Buffer,(char*)buf);
    }
	
	Screen_RefreshDataMulti(0x0340,p->Buffer,15*2);
	
	//��Դ��ⲿ��
	Screen_ShowPowerState();
/*	if(p->Warning==1)//����
	{
	  Screen_RefreshVarData(0x0020,1);
	}else
	{
	   Screen_RefreshVarData(0x0020,0);
	}
	*/
	if(USH_User_App_flag == DF_USH_USER_APP_OK)//USB
	{
	  Screen_RefreshVarData(0x0040,2);
	}else
	{
	   Screen_RefreshVarData(0x0040,0);
	}
	if(AT_GetData()->Mode==0)//����͸��ģʽ
	{
	  Screen_RefreshVarData(0x0060,5);
	}else
	{
	   Screen_RefreshVarData(0x0060,0);
	}
	
}

//---
/*
	��������void Screen_changPowerState(void)
	������	��س�����
*/
void Screen_changPowerState(void)
{
	u16 buf;
	
	if(LTC2944_GetData()->flag==1)  //�ϵ�ʱȥ�жϵ������
	{  LTC2944_GetData()->flag=2;
	   if((LTC2944_GetData()->PTC<1)&&(Battery_GetData()->Battery.ActualV>=BATTERY_Vcmp))  //�ж�Ϊ�ε����Ȼ���ز��ϣ��ָ�������
       {
		 buf=LTC2944_Cal_ACC(Battery_GetData()->Battery.ActualV);  
		 LTC2944_WriteTwoByte(AccumulatCharge_adrr,buf);
	   }		   
	}
	if(ScreenData.ChangTimer == 0)
	{ ScreenData.ChangTimer=2000;    //������һ��
	if(TestPower_GetData()->Changflg)//�г����ʱ
	{

		if((LTC2944_GetData()->I<0.1f)&&(LTC2944_GetData()->I>=0.0f)&&(Battery_GetData()->Battery.ActualV>=BATTERY_VMAX-0.7f))  //�õ���,��ѹȥ��д������
		{
			LTC2944_WriteTwoByte(AccumulatCharge_adrr,LTC2944_ACCMax);
		}

	}
 }
}
//---
/*
	��������void Screen_SecClock(void)
	������	�ú���Ϊ��Ļ�����Ӽ�ʱ��������ϵͳ�ľ�ȷ��ʱ��Դ
	˵����	�ú���Ϊ���ڼ������ʹ��
*/
void Screen_SecClock(void)
{
	if(ScreenData.Clock.Msec >= 500)
	{
		ScreenData.Clock.Msec = 0;
		Screen_ReadRegister(0x20,0x07);//����Ļʱ��
		if(ScreenData.Clock.Flag & b0)
		{
			if(ScreenData.Clock.Clock.tm_sec != ScreenData.Clock.ClockSec)
			{
				ScreenData.Clock.Flag &= ~b0;
				ScreenData.Clock.ClockSec = ScreenData.Clock.Clock.tm_sec; 
				Project_SecTimer();
			}
		}
		else
		{
			if(ScreenData.Clock.Clock.tm_sec != ScreenData.Clock.ClockSec)
			{
				ScreenData.Clock.Flag |= b0;
				ScreenData.Clock.ClockSec = ScreenData.Clock.Clock.tm_sec; 
				Project_SecTimer();
			}
		}
	}
}
//---
/*
	��������void Screen_ShowNetworkState(void)
	������	��ʾ����ͼ��
*/
void Screen_ShowNetworkState(void)
{

//	if(Flashisp_GetData()->Data[FLASHIS_AdrWLSZKG])
//	{
//		if(Network_GetData()->NetworkStatus)   
//			Screen_PastePicture(0xA240,0x00CA,0x16900B9,0x17D00CC,0x017c0002);
//		else
//			Screen_PastePicture(0xA240,0x00CA,0x16900D3,0x17D00E4,0x017c0002);
//	}
//	else Screen_PastePicture(0xA240,0x0002,0x1820003,0x1A20014,0x17c0003);
}
//---
/*
	��������void Screen_ShowNetworkState(void)
	������	��ʾ��Ļ��ͼ��
*/
void Screen_ShowScreenLockState(void)
{

	if(ScreenData.Button==0x0030)
	{
	  if(ScreenData.ScreenButtonLock!=2) ScreenData.ScreenButtonLock=2;
		else ScreenData.ScreenButtonLock=1;
		ScreenData.Button=0xffff;
	}
	else
	{
	   if(ScreenData.ScreenButtonLock==2) //��Ļ����ʧЧ
	   {
			 ScreenData.Button=0xffff;
		  
	   }
	}
	
	
	if(ScreenData.OldScreenButtonLock!=ScreenData.ScreenButtonLock)
	{
		ScreenData.OldScreenButtonLock=ScreenData.ScreenButtonLock;
		if(ScreenData.ScreenButtonLock==2)
		{
		Screen_RefreshVarData(0x0a0,14);
		}
		else
		{
		  Screen_RefreshVarData(0x0a0,15);
		  ScreenData.ScreenButtonLock=1;
		}
		
	}
}
//---
/*
	��������void Screen_ShowPowerState(void)
	������	��ʾ��ؼ���Դͼ��
*/
void Screen_ShowPowerState(void)
{ char bufdata[10];
	TwoBytes buf;
	if(Power_GetData()->Flg&0x02)//if(TestPower_GetData()->flag)//��ʾ�ⲿ��Դͼ��  PowerData.Flg
	{
		Screen_RefreshVarData(0x0080,6);
	}
	else
	{
	 Screen_RefreshVarData(0x0080,0);
	}
	//��Դ������ʾ
	
	
	if(TestPower_GetData()->Changflg==1)   //�г����
	{
	 if(ScreenData.BatflshTimer==0)
		{  ScreenData.BatflshTimer=250;
			 ScreenData.Batflshcnt++;
			 if(ScreenData.Batflshcnt>4)
				ScreenData.Batflshcnt=1;
			 
			  buf.U8Data[0]=0;
	         buf.U8Data[1]=ScreenData.Batflshcnt+7;   //ͼ��
			 if(LTC2944_GetData()->PTC>=100)//ͼ��
				{
				buf.U8Data[1]=LTC2944_GetData()->PTC/25+7;
			    //Screen_RefreshDataMulti(0x0020,&buf.U8Data[0],1*2);
				}
				else
				{
			    //Screen_RefreshDataMulti(0x0020,&buf.U8Data[0],1*2);
				}
		}
	}
	else
	{
	    buf.U8Data[0]=0;
		if(LTC2944_GetData()->PTC==0)
	    {
		buf.U8Data[1]=0;
	    }
	    else
	    {
	     buf.U8Data[1]=LTC2944_GetData()->PTC/25+1;
	     if(buf.U8Data[1]>=4)
		  buf.U8Data[1]=4;
	    }
	   //Screen_RefreshDataMulti(0x0020,&buf.U8Data[0],1*2);
	}
	//��ʾ��ذٷֱ�
	if(LTC2944_GetData()->PTC<100)
	sprintf((char *)&bufdata[0]," %02u%%",(u8)LTC2944_GetData()->PTC);//������Ϣ
	else
	sprintf((char *)&bufdata[0],"%u%%",(u8)LTC2944_GetData()->PTC);//������Ϣ
	Screen_RefreshDataMulti(0x0360,(u8 *)&bufdata[0],4*2);
	
}

//---
/*
	��������Screen_ShowWifi(void)
	������	��ʾWiFiͼ��
*/
void Screen_ShowWifi(void)
{
   if(Slave_GetData()->E28.STATERD24G&0x01) //E28����
   {
	   Screen_RefreshVarData(0x00E0,18);
   }
   else
   {
	   Screen_RefreshVarData(0x00E0,17);
   }
}
//---
/*
	��������Screen_ShowWifi(void)
	������	�������¼���״̬
*/
void Screen_ScanSchack(void)   //�����Ļ¼���״̬
{
	if(ScreenData.Scan_status&0x02)   //����ʱ���
	{
		if(ScreenData.Scan_Stimer==0)
		{
		   ScreenData.Scan_Stimer=1000; //1����һ��
			//������
           Screen_ReadRegister(0xE9,0x01);  //�������ȡ0xe9����
			
		}
		//����׼λ
		if(ScreenData.Scan_Stimer<900)  //100���뻺��ż����û��
		{
		if(ScreenData.Scan_status&0x01)  //������¼��״̬
		{
			
		}
		else                            //�˳���Ҫ����
		{
			ScreenData.Scan_status=ScreenData.Scan_status&0xFD;
			ScreenData.Key.Mode=0;
		}
		
	    }
		
	}
}

//---
//---
//-------------------------��ʼ�����----------------------------
//---
/*
	��������void Screen_InitData(void)
	������	screen.c�ļ���ʼ������
*/
void Screen_InitData(void)
{
	ScreenData.RunMode = 1;
	ScreenData.Button = 0XFFFF;
	ScreenData.Key.Opt = 1;
	ScreenData.Key.OldOpt = 0;
	ScreenData.Key.Mode = 0;
	ScreenData.Key.KeyData = SCREEN_NONE_KEY;
	ScreenData.ScreenButtonLock=1;
	ScreenData.Warning=0;		//2020-05-19 hjl �޸ĳ�ʼֵΪ0
	ScreenData.uEnableRecDataTimer = 0; //Ĭ�ϲ������������ݳ�ʱ��ʱ����//2020-05-23 by Alfred.
	ScreenData.JiPingJun.ucCurrentPage = 0xff; //��ʼ����ƽ����ǰҳ��ֵ��
	ScreenData.gSysConfigWaringBits.m_byData = 0x00;	//��ʼ����־λ��
	ScreenData.QimiStat=0;                      //�������δ���������
}
//---
/*
	��������void Screen_ColckInit(void)
	������	�ú���Ϊ��ʼ��ʱ�����
*/
void Screen_ColckInit(void)
{
	ScreenData.Clock.Flag = 0;
	ScreenData.Clock.ClockSec = 0;
	ScreenData.Clock.Msec = 0;
//	Screen_ReadRegister(0x20,0x07);//����Ļʱ��
}

void ScreenReadMemory(u16 address,u8 len)
{

	u8 buffer[20];
	u8 cnt=0;
	u8 i;
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x83;
	buffer[cnt++] = (u8)(address>>8);
	buffer[cnt++] = (u8)(address&0x00ff);
	buffer[cnt++] = len;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
}


const u8 SendTestbuffer[]="20190302";
//---
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Screen_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪScreen�ļ���ASCII�����ļ�
	��ʽ��		:000|02|002
*/
void Screen_DebugASCII(u8 *p,u8 len)
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
		case 98:
		{
			Screen_WriteRegisterOneData(0x4f,1);
		}
		case 99:
		{
			ScreenReadMemory(0x0200,0x01);
		}
		case 101:
		{//��ȡRunMode
			printf("p->RunMode=%u\r\n",ScreenData.RunMode);
			printf("p->RunModeStat=%u\r\n",ScreenData.RunModeStat);
			printf("p->Page=%u\r\n", ScreenData.Page);
			break;
		}
		case 102:
		{//��ȡRunMode
			u16 bf = Calculate_CharToU16(p+4,5);//��������
			ScreenData.Button = bf;
			printf("���յ�����=0x%X\r\n",ScreenData.Button);
			break;
		}
		case 103:
		{//������
			struct tm pt = ScreenData.Clock.Clock;
			pt.tm_year = Calculate_CharToU16(p+4,2) + SCREEN_YearScreen;//��������;
			if(pt.tm_year>138) pt.tm_year = 138;
			printf("���յ���=%u\r\n",pt.tm_year);
			Screen_SetClock(&pt);
			break;
		}
		case 104:
		{//������
			struct tm pt = ScreenData.Clock.Clock;
			pt.tm_mon = Calculate_CharToU16(p+4,2) - 1;//��������;
			if(pt.tm_mon>11) pt.tm_mon = 11;
			printf("���յ���=%u\r\n",pt.tm_mon);
			Screen_SetClock(&pt);
			break;
		}
		case 105:
		{//������
			struct tm pt = ScreenData.Clock.Clock;
			pt.tm_mday = Calculate_CharToU16(p+4,2);//��������;
			if(pt.tm_mday>31) pt.tm_mday = 31;
			printf("���յ���=%u\r\n",pt.tm_mday);
			Screen_SetClock(&pt);
			break;
		}
		case 106:
		{//����ʱ
			struct tm pt = ScreenData.Clock.Clock;
			pt.tm_hour = Calculate_CharToU16(p+4,2);//��������;
			if(pt.tm_hour>23) pt.tm_hour = 23;
			printf("���յ�ʱ=%u\r\n",pt.tm_hour);
			Screen_SetClock(&pt);
			break;
		}
		case 107:
		{//���÷�
			struct tm pt = ScreenData.Clock.Clock;
			pt.tm_min = Calculate_CharToU16(p+4,2);//��������;
			if(pt.tm_min>59) pt.tm_min = 59;
			printf("���յ���=%u\r\n",pt.tm_min);
			Screen_SetClock(&pt);
			break;
		}
		case 108:
		{//���÷�
			struct tm pt = ScreenData.Clock.Clock;
			pt.tm_sec = Calculate_CharToU16(p+4,2);//��������;
			if(pt.tm_sec>59) pt.tm_sec = 59;
			printf("���յ���=%u\r\n",pt.tm_sec);
			Screen_SetClock(&pt);
			break;
		}
		case 601:
		{//����ScreenData.Opt
			ScreenData.Key.Opt = Calculate_CharToU16(p+4,3);
			printf("���õ���OptֵΪ��%u\r\n",ScreenData.Key.Opt);
			break;
		}
		case 701:
		{//��ȡ��ǰʱ�Ӽ�����ʱ��
			FourBytes four;
			struct tm tf;
			four.U32Data = mktime(&Screen_GetData()->Clock.Clock);//��ǰʱ��
			printf("��ǰ��Ϣ��%u\r\n",four.U32Data);
			tf = ScreenData.Clock.Clock;
			printf("��ǰʱ��ʱ�䣺%u��%02u��%02u�� %02uʱ%02u��%02u��\r\n",tf.tm_year+SCREEN_YearBase,tf.tm_mon,tf.tm_mday,tf.tm_hour,tf.tm_min,tf.tm_sec);
			four.U16Data[0] = Flashisp_GetData()->Data[FLASHIS_AdrDDTime];
			four.U16Data[1] = Flashisp_GetData()->Data[FLASHIS_AdrDDTime+1];
			printf("������Ϣ��%u\r\n",four.U32Data);
			tf = *localtime(&four.U32Data);//����ʱ��
			printf("����ʱ�䣺%u��%02u��%02u�� %02uʱ%02u��%02u��\r\n",tf.tm_year+SCREEN_YearBase,tf.tm_mon,tf.tm_mday,tf.tm_hour,tf.tm_min,tf.tm_sec);
			
			break;
		}
		case 801: 
		case 802:
		case 803: 
		case 804: 
		case 805: 
		case 806: 
		case 807: 
		case 808: 
		case 809: 
		case 810: 
		case 811: 	
		case 812: 
		case 813: Screen_ShowPage(cmd-800); break;

	}
}
//---
/*
	��������void Screen_SavePowerDownMsg(void)
	������	�ú���Ϊ���������Ϣ��SD����
*/
void Screen_SavePowerDownMsg(void)
{
	u8 bf = 0;
//	TwoBytes two;
	Flashisp_DataStructure *pf = Flashisp_GetData();
//	two.U16Data = pf->Data[FLASHIS_AdrCYSZALDDMODE];
//	bf += two.U8Data[0];
//	two.U16Data = pf->Data[FLASHIS_AdrCYSZBLDDMODE];
//	bf += two.U8Data[0];
//	two.U16Data = pf->Data[FLASHIS_AdrCYSZTLDDMODE];
//	bf += two.U8Data[0];
//	Flashisp_GetData()->Data[FLASHIS_AdrDDYear]//��
//	Flashisp_GetData()->Data[FLASHIS_AdrDDMonth]//��
//	Flashisp_GetData()->Data[FLASHIS_AdrDDDate]//��
//	Flashisp_GetData()->Data[FLASHIS_AdrDDHour]//ʱ
//	Flashisp_GetData()->Data[FLASHIS_AdrDDMinute]//��
//	Flashisp_GetData()->Data[FLASHIS_AdrDDSec]//��
	ScreenData.Clock.Flag |= B2;
	if(bf != 0)
	{
		u8 dbuf[150];
		u8 temp[20];
		u16 nomber;
		u8 hh,mm,ss;
		u32 A_LostPowerTime;
		struct tm tf;
		FourBytes four;
	//	printf("��������¼");
		//������ʱ���
		four.U16Data[0] = Flashisp_GetData()->Data[FLASHIS_AdrDDTime];
		four.U16Data[1] = Flashisp_GetData()->Data[FLASHIS_AdrDDTime+1];
		tf = *localtime(&four.U32Data);//����ʱ������tm�ṹ
		A_LostPowerTime = mktime(&ScreenData.Clock.Clock); //��ǰʱ������
		A_LostPowerTime = (u32)difftime(A_LostPowerTime,four.U32Data);
		hh = A_LostPowerTime/3600;
		mm = (A_LostPowerTime%3600)/60;
		ss = A_LostPowerTime%60;

//		Serch_MaxPowerFile(); //�ҳ�����¼
		nomber=SdFile_GetData()->PowerFileMax+1; //�õ����µ��к�
		
		SPutl(0x4,&temp[0],nomber);
		temp[4]=0;
		strcpy((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],",");

		SPutl(0x2,&temp[0],tf.tm_year+SCREEN_YearBase); //������
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],"-");
		
		SPutl(0x2,&temp[0],tf.tm_mon+1); //������
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],"-");
		
		SPutl(0x2,&temp[0],tf.tm_mday); //������
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);

		strcat((char *)&dbuf[0]," ");
		
		SPutl(0x2,&temp[0],tf.tm_hour); //����ʱ
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],":");
		
		SPutl(0x2,&temp[0],tf.tm_min); //�����
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],":");
		
		SPutl(0x2,&temp[0],tf.tm_sec); //������
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		
		strcat((char *)&dbuf[0],",");
		
		SPutl(0x2,&temp[0],Screen_GetData()->Clock.Clock.tm_year); //������
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],"-");
		
		SPutl(0x2,&temp[0],Screen_GetData()->Clock.Clock.tm_mon+1); //������
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],"-");
		
		SPutl(0x2,&temp[0],Screen_GetData()->Clock.Clock.tm_mday); //������
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);

		strcat((char *)&dbuf[0]," ");
		
		SPutl(0x2,&temp[0],Screen_GetData()->Clock.Clock.tm_hour); //����ʱ
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],":");
		
		SPutl(0x2,&temp[0],Screen_GetData()->Clock.Clock.tm_min); //�����
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],":");
		
		SPutl(0x2,&temp[0],Screen_GetData()->Clock.Clock.tm_sec); //������
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);

		strcat((char *)&dbuf[0],",");
		
		SPutl(0x2,&temp[0],hh); //����ʱ
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],"h");
		
		SPutl(0x2,&temp[0],mm); //�����
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],"min");
		
		SPutl(0x2,&temp[0],ss); //������
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],"s");
		
//		strncpy((char *)&(SdFile_GetData()->power_file_chaxun),(char *)&dbuf[0],PowerFile_Record_Length-3);
///		SdFile_GetData()->power_file_chaxun.sdata.douhao4[0]='\r';
//		SdFile_GetData()->power_file_chaxun.sdata.douhao4[1]='\n';
//		SdFile_GetData()->power_file_chaxun.sdata.douhao4[2]='\0';

//		SavePowerFile_ABT(&SdFile_GetData()->power_file_chaxun);
	}
}

uint32_t reversebytes_uint32t(uint32_t value)
{
    return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 | 
        (value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24; 
}
//---
/*
��������void Screen_DealWihtReceiveData(void)
������������յ�������Ļ������
˵�����Ѿ�ȥ��֡ͷ
*/
//extern Flashisp_DataStructure FlashispData;
void Screen_DealWihtReceiveData(void)
{
	int uTemp;
  float fTemp;	
		if(Get_USART3Buffer()->BufFlg1==1)
	{ 
		Get_USART3Buffer()->BufFlg1=0;
		u8 *p=&Get_USART3Buffer()->ProgressBuffer1[0];
		if(AnalysisCRC(p))  //CRCУ��
		{
		
				switch(p[2])
					{
						case 0x00:
						{//���ذ汾��04 81 00 01 71
	//						ScreenData.Button = (*(p+2)<<8) + *(p+4);
							break;
						}
						case 0x20:
						{//����ʱ��ֵ(5A A5 0A 81 20 07 19 01 23 03 16 38 57)YY:MM:DD:WW:HH:MM:SS
							ScreenData.Clock.Clock.tm_year = (p[4] % 0x10 + (p[4] / 0x10) * 10) + SCREEN_YearScreen;//��
							ScreenData.Clock.Clock.tm_mon  = (p[5] % 0x10 + (p[5] / 0x10) * 10); //- 1;								//��
							ScreenData.Clock.Clock.tm_mday = p[6] % 0x10 + (p[6] / 0x10) * 10;											//��
							ScreenData.Clock.Clock.tm_wday = (p[7] % 0x10 + (p[7] / 0x10) * 10) ;//- 1;								//����
							ScreenData.Clock.Clock.tm_hour = p[8] % 0x10 + (p[8] / 0x10) * 10;											//ʱ
							ScreenData.Clock.Clock.tm_min  = p[9] % 0x10 + (p[9] / 0x10) * 10;											//��
							ScreenData.Clock.Clock.tm_sec  = p[10] % 0x10 + (p[10] / 0x10) * 10;										//��
							ScreenData.Clock.Flag |= B1;//��־ʱ���Ѹ���
							if(ScreenData.Clock.Flag & B2)
							{//ʵʱ����ʱ��
								FourBytes four;
								four.U32Data = mktime(&ScreenData.Clock.Clock);
								Flashisp_GetData()->Data[FLASHIS_AdrDDTime] = four.U16Data[0];
								Flashisp_GetData()->Data[FLASHIS_AdrDDTime+1] = four.U16Data[1];
								Flashisp_GetData()->Flag = 1;
							}
							break;
						}
						case 0XE9:  //��������¼��״̬
						{//���ذ汾��04 81 E9 01 71
							//=p[4];
							if(p[4]==0x01)
							ScreenData.Scan_status=ScreenData.Scan_status|0x01;
							else
							ScreenData.Scan_status=ScreenData.Scan_status&0xFE;	
							break;
						}
					}
		}
	
	}
	if(Get_USART3Buffer()->BufFlg2==1)
	{
		 Get_USART3Buffer()->BufFlg2=0;
		if(AnalysisCRC(&Get_USART3Buffer()->ProgressBuffer2[0]))  //CRCУ��
		{
			u8 *p=&Get_USART3Buffer()->ProgressBuffer2[0];
			u8 ucAdressDataLength = *p; //
			{//�������洢��ָ��ָ��
				u16 address = (*(p+2)<<8) + *(p+3);//������ַ
				Flashisp_DataStructure *pf = Flashisp_GetData();//Flashָ��
				Screen_DataStructure *ps = Screen_GetData();
				switch(address)
				{
					case SCREEN_KEYADD:
					{//���յ�����ָ��
						ScreenData.BacklightSecTimer = 0;
						ScreenData.Button = (*(p+5)<<8) + *(p+6);
						printf("���յ�����=0x%X\r\n",ScreenData.Button);
						break;
					}

					case SCREEN_KEYADD_ESC:
					{//���յ�����ָ��
						ScreenData.sValScreenKeyEsc = (*(p+5)<<8) + *(p+6);
						printf("���յ�����=0x%X\r\n",ScreenData.sValScreenKeyEsc);
						break;
					}

					case SCREEN_SAMPLE_ADRESS:
					{
					//							AdressData tempAdressdData;
						for(u8 i=0; i<15;i++)  //���15����
						{
							pf->Data[FLASHIS_AdrXTSZCYDZ+i] = 0;
						}						
						//2020-05-20 hjl �޸��޶�15���֣����ⳬ����Ӱ����������
						//�����ı���¼��1���յ��ķ����������£�0x0A, 0x83, 0x02, 0x00, 0x02, 0x31, 0x30, 0xFF, 0xFF, 0x5D, 0x3D,
//						for(u8 i=0;i<((ucAdressDataLength-6)/2 > 15 ? 15 : (ucAdressDataLength-6)/2);i++)    //ȥ�����ȣ�ָ���룬CRCУ���λ�ܹ�6���ֽڣ�ʣ�µ�����Ч����λ(0x31, 0x30, 0xFF, 0xFF)
//						{
//							pf->Data[FLASHIS_AdrXTSZCYDZ+i] = *(u16*)(p+5+i*2);
//						}
						for(u8 i=0;i<((ucAdressDataLength-8)/2 > 15 ? 15 : (ucAdressDataLength-8)/2);i++)    //ȥ�����ȣ�ָ����, ����0XFF��CRCУ���λ�ܹ�8���ֽڣ�ʣ�µ�����Ч����λ(0x31, 0x30)
						{
							pf->Data[FLASHIS_AdrXTSZCYDZ+i] = *(u16*)(p+5+i*2);
						}						
							pf->Flag = 1;
						    ps->Key.Mode=0;   //����ר����Ե�ַ����
						    ScreenBackLastOpt(ps);
							break;
					}
					

					/*************************************     1-��������ҳ����ֵ����     *******************************************/	
					//�������� 2020-05-20 hjl add
					case SCREEN_ADD_KJMM:			
					{					
						uTemp = *(u32*)(p+5);
						uTemp = reversebytes_uint32t(uTemp);						
						memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
						sprintf((char*)uScreenRcvBuffer,"%04d",uTemp);
						ps->sRcvType = SCREEN_ADD_KJMM;
						break;
					}
					
					/*************************************     1-ϵͳ����ҳ����ֵ����     *******************************************/	
					/*************************************     ϵͳ����ҳ����ֵ����     *******************************************/							

					//��������
					case SCREEN_ADD_RIQI:			
					{					
						uTemp = *(u32*)(p+5);
						uTemp = reversebytes_uint32t(uTemp);						
						memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
						sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
						ps->sRcvType = SCREEN_ADD_RIQI;
						break;
					}
					
					//����ʱ��
					case SCREEN_ADD_SHIJIAN:	
					{					
						uTemp = *(u32*)(p+5);
						uTemp = reversebytes_uint32t(uTemp);						
						memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
						sprintf((char*)uScreenRcvBuffer,"%06d",uTemp);	//2020-05-20 hjl �޸����벻��6λ��0������СʱΪһλ����ʾ����
						ps->sRcvType = SCREEN_ADD_SHIJIAN;
						break;
					}

					//����ѹ
					case SCREEN_ADD_DAQIYA:		
					{					
						uTemp = *(u32*)(p+5);
						uTemp = reversebytes_uint32t(uTemp);		
						fTemp  = (float)uTemp/100;          //��������˴��ؼ���������λС��������ֵ�Զ�����100�����Դ˴���Ӧ��100��
						memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
						sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
						ps->sRcvType = SCREEN_ADD_DAQIYA;
						break;
					}	
					
					//��ˮ���
					case SCREEN_ADD_PSJG:			
					{
						uTemp = *(u32*)(p+5);
						uTemp = reversebytes_uint32t(uTemp);						
						memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
						sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
						ps->sRcvType = SCREEN_ADD_PSJG;
						break;
					}	
					
					//��ˮʱ��
					case SCREEN_ADD_PSSS:			
					{				
						uTemp = *(u32*)(p+5);
						uTemp = reversebytes_uint32t(uTemp);						
						memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
						sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
						ps->sRcvType = SCREEN_ADD_PSSS;
						break;
					}


//					//��ʪ��
//					case SCREEN_ADD_YC_MIN:	
//											{				
//												uTemp = *(u32*)(p+5);
//												uTemp = reversebytes_uint32t(uTemp);						
//												memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
//												sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
//												ps->sRcvType = SCREEN_ADD_YC_MIN;
//												break;
//											}
//					//��������
//					case SCREEN_ADD_YC_TIMES:
//											{				
//												uTemp = *(u32*)(p+5);
//												uTemp = reversebytes_uint32t(uTemp);						
//												memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
//												sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
//												ps->sRcvType = SCREEN_ADD_YC_TIMES;
//												break;
//											}						
					//��ʪ��
					case SCREEN_ADD_HSL_MIN:	
											{				
												uTemp = *(u32*)(p+5);
												uTemp = reversebytes_uint32t(uTemp);						
												memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
												sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
												ps->sRcvType = SCREEN_ADD_HSL_MIN;
												break;
											}	
						
					/************************************     2-�̵��������ֵ����     **************************************************************************/	
					/*************************************     �̵�����--Բ��--����--����--��ѯ	     *****************************************************/
					//�ھ�,�׹�--2λС��
					case SCREEN_ADD_CIRCLE_DIAMETER:				
					case SCREEN_ADD_CIRCLE_PIPE:
					case SCREEN_ADD_RECT_A:			
					case SCREEN_ADD_RECT_B:
					case SCREEN_ADD_RECT_PIPE:
											{			
												uTemp = *(u32*)(p+5);
												uTemp = reversebytes_uint32t(uTemp);	
												fTemp  = (float)uTemp/100;          
												memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
												sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
												ps->sRcvType = address;
												break;
											}
																			
					//���--4λС��
					case SCREEN_ADD_CIRCLE_AREA:
					case SCREEN_ADD_RECT_AREA:
					case SCREEN_ADD_CROSS_SECTION:		
												{					
													uTemp = *(u32*)(p+5);
													uTemp = reversebytes_uint32t(uTemp);		
													fTemp  = (float)uTemp/10000;          
													memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
													sprintf((char*)uScreenRcvBuffer,"%.4f",fTemp);
													ps->sRcvType = address;;
													break;
												}
					//����������---����
					case SCREEN_ADD_CIRCLE_RING:																	
					case SCREEN_ADD_CIRCLE_HOLE:
					case SCREEN_ADD_RECT_HOLE:
					case SCREEN_ADD_RECT_TESTPOINT:
					case SCREEN_ADD_SAMPLES:
												{				
													uTemp = *(u32*)(p+5);
													uTemp = reversebytes_uint32t(uTemp);						
													memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
													sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
													ps->sRcvType = address;
													break;
												}
					//�����ļ���--7λ��������������						
					case SCREEN_ADD_FLUE_FIND:				
												{				
													uTemp = *(u32*)(p+5);
													uTemp = reversebytes_uint32t(uTemp);						
													memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
													sprintf((char*)uScreenRcvBuffer,"%7d",uTemp);
													ps->sRcvType = address;
													break;
												}

					/*************************************     3-����Ԥ�����ֵ����     ***********************************************************************************************/	
					/*************************************     ����Ԥ��--���²���--��ѯƽ��	     *************************************************/
					//����--1λС��
					case SCREEN_ADD_YANWENG:				
											{					
												uTemp = *(u32*)(p+5);
												uTemp = reversebytes_uint32t(uTemp);		
												fTemp  = (float)uTemp/10;          
												memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
												sprintf((char*)uScreenRcvBuffer,"%.1f",fTemp);
												ps->sRcvType = address;
												break;
											}
					//ƽ����ѯ�ļ���
					case SCREEN_ADD_FILE_CHECK:				
											{				
												uTemp = *(u32*)(p+5);
												uTemp = reversebytes_uint32t(uTemp);						
												memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
												sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
												ps->sRcvType = address;
												break;
											}
						
					/*************************************     4-�⺬ʪ������ֵ����     **********************************************************************/	
					/*************************************     �⺬ʪ��--��ʪ������	     *************************************************/
					//1-��ʪ������-2020-05-21						
					case SCREEN_ADD_MOISTURE:				
											{	//--��λС��				
												uTemp = *(u32*)(p+5);
												uTemp = reversebytes_uint32t(uTemp);		
												fTemp  = (float)uTemp/100;          
												memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
												sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
												ps->sRcvType = address;
												break;
											}
						
					/*************************************     5-������������ֵ����     *****************************************************************/	
					/*************************************     ��������--��ʱ	     *************************************************/
					//1-��ʱʱ��-2020-05-22																							
					case SCREEN_ADD_YC_MIN:
					case SCREEN_ADD_YC_TIMES:	
					case SCREEN_ADD_GAS_FIND:
					case SCREEN_ADD_START_NUMBER:
					case SCREEN_ADD_END_NUMBER:
												{	//--����				
													uTemp = *(u32*)(p+5);
													uTemp = reversebytes_uint32t(uTemp);		         
													memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
													sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
													ps->sRcvType = address;
													break;
												}

						
					case SCREEN_ADD_O2_INPUT:	
					case SCREEN_ADD_SO2_INPUT:	
					case SCREEN_ADD_NO_INPUT:	
					case SCREEN_ADD_NO2_INPUT:	
					case SCREEN_ADD_CO_INPUT:	
					case SCREEN_ADD_H2S_INPUT:
											{	//--һλС��				
												uTemp = *(u32*)(p+5);
												uTemp = reversebytes_uint32t(uTemp);		
												fTemp  = (float)uTemp/10;          
												memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
												sprintf((char*)uScreenRcvBuffer,"%.1f",fTemp);
												ps->sRcvType = address;
												break;
											}	
                    case SCREEN_ADD_CALC_FACTOR:	
					case SCREEN_ADD_LOAD_FACTOR:	
					case SCREEN_ADD_OXYGEN_BASE:											
					case SCREEN_ADD_CO2_INPUT:
											{	//--��λС��				
												uTemp = *(u32*)(p+5);
												uTemp = reversebytes_uint32t(uTemp);		
												fTemp  = (float)uTemp/100;          
												memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
												sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
												ps->sRcvType = address;
												break;
											}						
																							
					/*************************************     6-�̳�����     ***********************************************************************************************/	
					/*************************************     �̳�����--��������	     *************************************************/
					//������ʽ����������ֱ��						
					case SCREEN_ADD_CC_SAMPLE:			  		
					case SCREEN_ADD_MOUTH_DIAMETER:	  
													{	//--һλС��				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/10;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.1f",fTemp);
														ps->sRcvType = address;
														break;
													}
					//��Ͳ�ţ������ʱ���ܲɵ�						
					case SCREEN_ADD_CARTRIDGE_NUMBER: 
					case SCREEN_ADD_TIME_MINUTE:			
					case SCREEN_ADD_TIME_SECOND:
					case SCREEN_ADD_TOTAL_SAMPLE:	
													{	//����			
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);						
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
														ps->sRcvType = address;
														break;
													}

					/*************************************     7-�ļ���ѯ     ***********************************************************************************************/	
					/*************************************     �ļ���ѯ--����--�ļ�����	     *************************************************/
					//1-�ļ���ѯ-2020-05-22																						
					case SCREEN_ADD_OILGAS_FIND:
					case SCREEN_ADD_RANGES_NUMBER:
					case SCREEN_ADD_START_NUMBER2:
					case SCREEN_ADD_END_NUMBER2:
					case SCREEN_ADD_DUST_FILE_NUM:
													{	//--����				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		         
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
														ps->sRcvType = address;
														break;
													}
					case SCREEN_ADD_CONCENTRATIO:	
					case SCREEN_ADD_VOLUME:
					case SCREEN_ADD_DUST_OXYGEN:
					case SCREEN_ADD_DUST_OXYGEN_BASE:						
													{	//--һλС��				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/10;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.1f",fTemp);
														ps->sRcvType = address;
														break;
													}
					case SCREEN_ADD_DUST_LOAD_FACTOR:	
					case SCREEN_ADD_DUST_CALC_FACTOR:	
													{	//--��λС��				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/100;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
														ps->sRcvType = address;
														break;
													}
					case SCREEN_ADD_DUST_WEIGHT:	
													{	//--��λС��				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/100000;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.4f",fTemp);
														ps->sRcvType = address;
														break;
													}														
																							
					/*************************************     9-У׼ά������ֵ����     ***********************************************************************************************/	
					/*************************************     У׼ά��--��������--����У׼	     *************************************************/
					//����---����
					case SCREEN_ADD_PASSWORD_INPUT:		
													{				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);						
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
														ps->sRcvType = address;
														break;
													}
					//1-ϵͳ����-2020-05-17						
					case SCREEN_ADD_CALIB_NOx:
					case SCREEN_ADD_CO_SO2:
					case SCREEN_ADD_NO2_SO2:
					case SCREEN_ADD_PTG_Kp:
					case SCREEN_ADD_SMOKE_DENSITY:
					case SCREEN_ADD_CALC_RATIO:
					case SCREEN_ADD_OXYGEN_REF:				
													{	//--��λС��				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/100;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
														ps->sRcvType = address;
														break;
													}			
					//1-ϵͳ����--CO��SO2����-2020-05-18
					case SCREEN_ADD_COSO2_TEXT01:		
					case SCREEN_ADD_COSO2_TEXT02:		
					case SCREEN_ADD_COSO2_TEXT03:		
					case SCREEN_ADD_COSO2_TEXT04:		
					case SCREEN_ADD_COSO2_TEXT05:		
					case SCREEN_ADD_COSO2_TEXT06:		
					case SCREEN_ADD_COSO2_TEXT07:		
					case SCREEN_ADD_COSO2_TEXT08:		
					case SCREEN_ADD_COSO2_TEXT09:		
					case SCREEN_ADD_COSO2_TEXT0A:			
													{				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);						
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
														ps->sRcvType = address;
														break;
													}							
					case SCREEN_ADD_COSO2_TEXT0B:		
					case SCREEN_ADD_COSO2_TEXT0C:		
					case SCREEN_ADD_COSO2_TEXT0D:		
					case SCREEN_ADD_COSO2_TEXT0E:		
					case SCREEN_ADD_COSO2_TEXT0F:		
					case SCREEN_ADD_COSO2_TEXT10:		
					case SCREEN_ADD_COSO2_TEXT11:		
					case SCREEN_ADD_COSO2_TEXT12:		
					case SCREEN_ADD_COSO2_TEXT13:		
					case SCREEN_ADD_COSO2_TEXT14:						
													{	//--��λС��				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/100;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
														ps->sRcvType = address;
														break;
													}
					//2-����У׼============================================================================================//																							
						
						
					case SCREEN_ADD_RATIO_TEXT02:
					case SCREEN_ADD_RATIO_TEXT03:
					case SCREEN_ADD_RATIO_TEXT04:
					case SCREEN_ADD_RATIO_TEXT05:
					case SCREEN_ADD_RATIO_TEXT06:
					case SCREEN_ADD_RATIO_TEXT07:
					case SCREEN_ADD_RATIO_TEXT08:
					case SCREEN_ADD_RATIO_TEXT09:
					case SCREEN_ADD_RATIO_TEXT0A:
					case SCREEN_ADD_RATIO_TEXT0B:
						                          {	//--4λС��				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/10000;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.4f",fTemp);
														ps->sRcvType = address;
														break;
													}			
					case SCREEN_ADD_STD_FLOW:	
					case SCREEN_ADD_RATIO_TEXT0C:
					case SCREEN_ADD_RATIO_TEXT0D:						
													{	//--��λС��				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/100;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
														ps->sRcvType = address;
														break;
													}																							
					//3-��������============================================================================================//
					case SCREEN_ADD_ORIG_PASS:		
					case SCREEN_ADD_NEW_PASS:			
					case SCREEN_ADD_CONFIRM_PASS:	
					case SCREEN_ADD_PRINT_PASS:		
					case SCREEN_ADD_APP_PASS:	
                    case SCREEN_ADD_E28ADR_PASS:
                    case SCREEN_ADD_E28CH_PASS:						
													{				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);						
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
														ps->sRcvType = address;
														break;
													}	
					//4-Ũ��У׼																							
					case SCREEN_ADD_O2_TEXT01:	
					case SCREEN_ADD_SO2_TEXT02:	
					case SCREEN_ADD_NO_TEXT03:
					case SCREEN_ADD_NO2_TEXT04:	
					case SCREEN_ADD_CO_TEXT05:	
					case SCREEN_ADD_H2S_TEXT06:	
					case SCREEN_ADD_CO2_TEXT07:		
												   {	//--��λС��				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/100;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
														ps->sRcvType = address;
														break;
													}
					case SCREEN_ADD_MID_RATIO:	
					case SCREEN_ADD_LOW_RATIO:	
					case SCREEN_ADD_HIGH_RATIO:	
													{	//--��λС��				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/1000;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.3f",fTemp);
														ps->sRcvType = address;
														break;
													}
					case SCREEN_ADD_MID_STD:		
					case SCREEN_ADD_LOW_STD:		
					case SCREEN_ADD_HIGH_STD:					
													{	//--1λС��				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/10;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.1f",fTemp);
														ps->sRcvType = address;
														break;
													}
					//5-ѹ��У׼																							
					case SCREEN_ADD_AIR_PRESSURE:			
					case SCREEN_ADD_CALC_PRESSURE:		
					case SCREEN_ADD_STATIC_PRESSURE:	
					case SCREEN_ADD_DYNAMIC_PRESSURE:
					case SCREEN_ADD_DUST_PRESSURE:	 
					case SCREEN_ADD_FLOW_PRESSURE:		
													{	//--��λС��				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/100;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
														ps->sRcvType = address;
														break;
													}
					//7-����У׼																							
					case SCREEN_ADD_DRY_BALL:		
					case SCREEN_ADD_WET_BALL:		
					case SCREEN_ADD_CALC_TEMP:	
					case SCREEN_ADD_CIRCLE_TEMP:
					case SCREEN_ADD_DRY_RATIO:	
					case SCREEN_ADD_WET_RATIO:				
													{	//--��λС��				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/100;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
														ps->sRcvType = address;
														break;
													}
					//8-��ر궨																							
					case SCREEN_ADD_BATTERY_STD:				
													{	//--��λС��				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/10000;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.4f",fTemp);
														ps->sRcvType = address;
														break;
													}																							
					//����ά��->�������																							
					case SCREEN_ADD_INSTRUMENT_NUM:
					case SCREEN_ADD_O2_DATE:	
					case SCREEN_ADD_SO2_DATE:
					case SCREEN_ADD_NO_DATE:	
					case SCREEN_ADD_NO2_DATE:
					case SCREEN_ADD_CO_DATE:	
					case SCREEN_ADD_H2S_DATE:
					case SCREEN_ADD_CO2_DATE:							
													{	//--����				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		         
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
														ps->sRcvType = address;
														break;
													}	
					case SCREEN_ADD_O2_LQ:	
					case SCREEN_ADD_SO2_LQ:
					case SCREEN_ADD_NO_LQ:	
					case SCREEN_ADD_NO2_LQ:
					case SCREEN_ADD_CO_LQ:	
					case SCREEN_ADD_H2S_LQ:
					case SCREEN_ADD_CO2_LQ:							
													{	//--����	
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		         
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
														ps->sRcvType = address;														
														break;
                          }														
													
						
					default:
								{
									printf("���յ���Ļδ֪����=0x%X\r\n",*(p+1));
									break;
								}
					}
				
				}
		}
	}
//	else printf("����1\r\n");
}
//---
