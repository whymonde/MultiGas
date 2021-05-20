/*
  ���ļ�Ϊ��Ļ���������ļ�
*/
#include "button.h"
#include "ScreenButton.h"
#include "ScreenDwFunction.h"
#include "ScreenDisCalculate.h"
#include "CalculationFormula.h"
extern Button_DataStructure ButtonData;
extern u8 uScreenRcvBuffer[10];
extern Flux_DataStructure FluxData;

//TEMP_CARIBRATION_STATE_MACHINE g_TempCaribrationStateMachine;

/*
	��������void Buttom_MainPage(void)
	������	�ú���Ϊ�����水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Buttom_UpdataProgramPage(void)
{ 
	if(ButtonData.KeyData==BUTTON_NONE) return;
	Screen_DataStructure *ps = Screen_GetData();
		switch(ButtonData.KeyData)
	{
			case BUTTON_LEFT: 
			{
			   if(ps->Key.Opt==2) ps->Key.Opt=1;
				 break;
			}
			case BUTTON_RIGHT: 
			{
			   if(ps->Key.Opt==1) ps->Key.Opt=2;
				 break;
			}
			case BUTTON_OK: 
			{
			   if(ps->Key.Opt==1) ps->Button=0x0200;
				 if(ps->Key.Opt==2) ps->Button=0x0204;
				 break;
			}	
			case BUTTON_BACK: 
			{
			   ps->Key.Opt=2; ps->Button=0x0204;
				 break;
			}
			
	}
	ButtonData.KeyData=BUTTON_NONE;
}
//----
/*
	��������void Buttom_MainPage(void)
	������	�ú���Ϊ�����水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Buttom_MainPage(void)
{
	Screen_DataStructure *ps = Screen_GetData();
	switch(ButtonData.KeyData)
	{
		case BUTTON_DOWN:
		{
			if(ps->Key.Opt == 1)ps->Key.Opt = 5;  //��ҳ�ܹ���8���˵�ͼ�꣬�����ǰѡ���ǵ�һ����¡�DOWN����ô�л���ѡ�в˵�5
			if(ps->Key.Opt == 2)ps->Key.Opt = 6;
			if(ps->Key.Opt == 3)ps->Key.Opt = 7;
			if(ps->Key.Opt == 4)ps->Key.Opt = 8;
			break;
		}
		case BUTTON_UP:
		{
			if(ps->Key.Opt == 5)ps->Key.Opt = 1;
			if(ps->Key.Opt == 6)ps->Key.Opt = 2;
			if(ps->Key.Opt == 7)ps->Key.Opt = 3;
			if(ps->Key.Opt == 8)ps->Key.Opt = 4;
			break;
		}	
		case BUTTON_LEFT:
		{
			if(ps->Key.Opt == 1)ps->Key.Opt = 8;
			else 
				ps->Key.Opt--;
			break;
		}
		case BUTTON_RIGHT:
		{
			if(ps->Key.Opt == 8)ps->Key.Opt = 1;
			else 
				ps->Key.Opt++;
			break;
		}
		case BUTTON_1:  ps->Button=1; break;		
		case BUTTON_2:  ps->Button=2; break;		
        case BUTTON_3:	ps->Button=3; break;	
        case BUTTON_4:	ps->Button=4; break;
        case BUTTON_5:	ps->Button=5; break;	
        case BUTTON_6:	ps->Button=6; break;
        case BUTTON_7:	ps->Button=7; break;	
        case BUTTON_8:	ps->Button=8; break;		
		case BUTTON_OK:
		{
			
			ps->Button = ps->Key.Opt;//��ֵ����ֵ
			break;
		}
	}
	ButtonData.KeyData = BUTTON_NONE;
}
//---
/*
	��������void Buttom_MainPage(void)
	������	�ú���Ϊ�����水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Buttom_PowerOffPage(void)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  	ScreenFirstOpButton(ps,2,1); //�ײ㴦��			
			break;
		}
	}
	//��Ļ��������
	
		if(ps->Button==0x0001) //�˳�
			{ TwoBytes two;
					   ps->RunModeStat=1;
					   ps->RunMode=51;
				       Flashisp_GetData()->Data[FLASHIS_AdrCJMOD]=0;
				       Flux_GetData()->ChannelB.RunMode=0;
				
				      	two.U8Data[0] =Flux_GetData()->ChannelB.RunMode;
		               two.U8Data[1] = 0;
		               Flashisp_GetData()->Data[FLASHIS_AdrCJMOD] 	= two.U16Data; 			    //����ģʽ+״̬��־λ
			           Flashisp_GetData()->Flag=1;
				
				       pf->Flag=1;
			}
		else if(ps->Button==0x0002)//����
			{
				      //to do �ɼ��߼�����
				      if((Flashisp_GetData()->Data[FLASHIS_AdrCJMOD]==2)||(Flashisp_GetData()->Data[FLASHIS_AdrCJMOD]==3))//������ͣ��//������
					  {
						  
				       ps->RunMode=0x6200; //��ʼ����
		               ps->Function=0;
		               ps->Key.Opt=7;	
					   ps->RunModeStat=1;  
					   ps->YC_Pause=0;
					   if(Flashisp_GetData()->Data[FLASHIS_AdrCJMOD]==2)	  //�������
					   {
						if(pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]==1) //����
						{
					      Flux_StarSampling('B',Flux_GetData()->ChannelA.ShowFlux); //Ŀ������
						}
				         else
				         {  
					       Flux_StarSampling('B',GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CYLS)); //Ŀ������
				         }  	
					   }
					   else
					   {
						ps->YC_Pause=1;     //��ͣ
					   }
						  
				      }
					  else if(Flashisp_GetData()->Data[FLASHIS_AdrCJMOD]==4) //������
					  {
						 Flux_GetData()->ChannelB.Sampling.DXTime=FLUX_FangDaoXiTimeB;   //60��ŵ���
				         Flux_GetData()->ChannelB.Sampling.FdxFlg=0;
				         Flux_StarSampling('B',20);//������  20L/min  10s
						 ps->RunMode=0x6220;
			             ps->YC_Stat=0;
			             ScreenChangeDataInit(ps);//�������
					  }
					 Flux_GetData()->ChannelB.RunMode=Flashisp_GetData()->Data[FLASHIS_AdrCJMOD];

			}

		ScreenChangeDataInit(ps);
}

//---
/*
	��������void Button_SysSetPage(void)
	������	�ú���Ϊϵͳ���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��	1. 2020-05-20 hjl ��������̴���
				2. 2020-05-25 modify by alfred ,�޸ĳ�ʱ��⴦��
				3. �������޸��˱�ҳ��ʾ���ݣ���Ӧ�޸��˴��룬���Ӻͼ���һЩ��ʾ, modify by alfred, 2020.06.12.
*/

void Button_SysSetPage(void)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  	ScreenFirstOpButton(ps,12,1); //�ײ㴦��			
			break;
		}
		
		case 1:
		{//����������������		
			if(ps->Key.bOutsideKeyboard != true)  //���������У�����¼�룩��Ļ�������֣��������������������룩
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);//��Χ�а������봦�����أ��ӳ���
			
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))   //�����˽��ճ�ʱ�붨ʱ��
				{     
					ScreenBackLastOpt(ps) ;  //���ػ����˳�����
				}
			
				if(ps->sRcvType !=0)
				{
					SystemSetPageWriteDate(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}				
			}
			else       ////���������У�����¼�룩�ⲿ�����������֣�û�е������������������룩
			{
				ScreenTowOpButton(ps,1,9,1);
				if(ps->Key.TwoOpt==4&&ps->Key.KeyDir==2)   //������λ����Щ�������ֵ�λ���硰-��
					ps->Key.TwoOpt=5;
				else if(ps->Key.TwoOpt==4) 
					ps->Key.TwoOpt=3;
				
				if(ps->Key.TwoOpt==7&&ps->Key.KeyDir==2) 
					ps->Key.TwoOpt=8;
				else if(ps->Key.TwoOpt==7) 
					ps->Key.TwoOpt=6;	
				if(ps->Button==0x0Aff) //ȷ��OK
				{    
					ps->Key.Mode=2;
					ps->Key.TwoOpt=0;
					u8 date[15];
					ps->Clock.Clock.tm_year=Calculate_CharToU16((&ps->Key.Buffer[0]),4)-SCREEN_YearBase; //�޸�����
					if(ps->Clock.Clock.tm_year>138) 
						ps->Clock.Clock.tm_year = 138;
					ps->Clock.Clock.tm_mon=Calculate_CharToU16((&ps->Key.Buffer[5]),2)-1;
					if(ps->Clock.Clock.tm_mon >11)
						ps->Clock.Clock.tm_mon = 11;
					ps->Clock.Clock.tm_mday=Calculate_CharToU16((&ps->Key.Buffer[8]),2);
					if(ps->Clock.Clock.tm_mday >31)
						ps->Clock.Clock.tm_mday = 31;
					Screen_SetClock(&(ps->Clock.Clock));
					Screen_ResetKeyDataBuffer();//�������ֵ
					sprintf((char*)date,"%02d:%02d:%02d",ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
					strcpy((char*)ps->Key.Buffer,(char*)date);

					ps->Timer=1000;
				}
				
				if(ps->Button==0x0Afe) //�˻���һ��
				{
					ps->Key.Mode=0;
					ps->RunModeStat=1;//ˢ������
					ps->Button=0xffff;
				}
				if(ps->Key.number>=0x30&&ps->Key.number<=0x39)
				{
					ps->Key.Buffer[ps->Key.TwoOpt]=ps->Key.number;
					if(ps->Key.TwoOpt<9) 
					{
						ps->Key.TwoOpt++; 
						ps->Key.KeyDir=2;
						if(ps->Key.TwoOpt==4&&ps->Key.KeyDir==2) 
							ps->Key.TwoOpt=5;
						if(ps->Key.TwoOpt==7&&ps->Key.KeyDir==2) 
							ps->Key.TwoOpt=8;
					}
				}
			}
			break;
		}
		
		case 2:
		{//��������ʱ������
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}

				if(ps->sRcvType !=0)
				{
					SystemSetPageWriteTime(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}
			}
			else
			{
				ScreenTowOpButton(ps,1,5,1);
				if(ps->Key.TwoOpt==2&&ps->Key.KeyDir==2) 
					ps->Key.TwoOpt=3;
				else if(ps->Key.TwoOpt==2) 
					ps->Key.TwoOpt=1;
				if(ps->Key.TwoOpt==5&&ps->Key.KeyDir==2) 
					ps->Key.TwoOpt=6;
				else if(ps->Key.TwoOpt==5) 
					ps->Key.TwoOpt=4;	
				if(ps->Button==0x0Aff) //ȷ��
				{
					ps->Clock.Clock.tm_hour=Calculate_CharToU16((&ps->Key.Buffer[0]),2); //�޸�ʱ��
					ps->Clock.Clock.tm_min=Calculate_CharToU16((&ps->Key.Buffer[3]),2);
					ps->Clock.Clock.tm_sec=Calculate_CharToU16((&ps->Key.Buffer[6]),2);
					ps->Clock.Clock.tm_mon-=1;
					if(ps->Clock.Clock.tm_mon >11)
					ps->Clock.Clock.tm_mon = 11;
					Screen_SetClock(&(ps->Clock.Clock));
					ps->Timer=1000;

				}
				if(ps->Button==0x0Afe||ps->Button==0x0Aff) //�˻���һ��
				{
					ps->Key.Mode=0;
					ps->RunModeStat=1;//ˢ������
					ps->Button=0xffff;
				}
				if(ps->Key.number>=0x30&&ps->Key.number<=0x39)
				{
					ps->Key.Buffer[ps->Key.TwoOpt]=ps->Key.number;
					if(ps->Key.TwoOpt<7) 
					{
						ps->Key.TwoOpt++; 
						ps->Key.KeyDir=2;
						if(ps->Key.TwoOpt==2&&ps->Key.KeyDir==2) 
							ps->Key.TwoOpt=3;
						if(ps->Key.TwoOpt==5&&ps->Key.KeyDir==2) 
							ps->Key.TwoOpt=6;
					}
				}
			}
			break;			
		}
		
		case 3:  //����ѹ
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);
			
			if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
		
			  if(ps->sRcvType !=0)
				 {
					FourBytes four;
					four.FloatData = (float)atof((char*)&(uScreenRcvBuffer[0]));
					if(four.FloatData<(float)50.0) four.FloatData=(float)50.0; //���� ����50Kp
					if(four.FloatData>(float)130.0) four.FloatData=(float)130.0; //���� С����130Kp 
					SaveFlaotToFlash(four.FloatData,FLASHIS_AdrXTSZDQY);		
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff)
				{
					FourBytes four;
					four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));
					if(four.FloatData<(float)50.0) four.FloatData=(float)50.0; //���� ����50Kp
					if(four.FloatData>(float)130.0) four.FloatData=(float)130.0; //���� С����130Kp
					SaveFlaotToFlash(four.FloatData,FLASHIS_AdrXTSZDQY);				
				}
				if(ps->Button==0x0Afe||ps->Button==0x0Aff) //�˻���һ��
				{
					ps->Key.Mode=0;
					ps->RunModeStat=1;//ˢ������
					ps->Button=0xffff;
				}
				ScreenEnterNumberText(ps,2,6); //��ֵ����
				/*
				if((ps->Key.number>=0x30&&ps->Key.number<=0x39)||ps->Key.number==0x2e)
				{
					ps->Key.Buffer[ps->Key.TwoOpt++]=ps->Key.number;
					if(ps->Key.TwoOpt>7) ps->Key.TwoOpt=7;//�������볤��
				}
				*/
				
			}
			break;
		}
		case 5:   //�����˵�ֻҪ�����û��ȡ������Ok����������������ȥ��
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
			ScreenTowOpButton_KeyControl_adr(ps);
	
			if(ps->Button==0x0Afe)
			{     
			  ScreenBackLastOpt(ps);
			}

			}
			
			break;
		}
		case 7:
		case 8:
		case 10:
		{//��������ʱ����
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
			
				if(ps->sRcvType != 0)
				{
					TwoBytes two;
					two.U16Data = (int)atoi((char*)&(uScreenRcvBuffer[0]));
					if( ps->Key.Mode==7)
						pf->Data[FLASHIS_AdrXTSZZDPSJG]=two.U16Data; //������ˮ���ֵ
					else if( ps->Key.Mode==8)
						pf->Data[FLASHIS_AdrXTSZZDPSSC]=two.U16Data; //������ˮʱ��ֵ
					else if( ps->Key.Mode==10)
					{   if(two.U16Data<=0)two.U16Data=0;
						pf->Data[FLASHIS_AdrXTSZCHSLSJ]=two.U16Data; //���溬ʪ��ʱ��ֵ		
					}						
					pf->Flag=1;		
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff)
				{
					TwoBytes two;
					two.U16Data = (int)atoi((char*)&(ps->Key.Buffer[0]));
					if( ps->Key.Mode==7)
						pf->Data[FLASHIS_AdrXTSZZDPSJG]=two.U16Data; //������ˮ���ֵ
					else if( ps->Key.Mode==8)
						pf->Data[FLASHIS_AdrXTSZZDPSSC]=two.U16Data; //������ˮʱ��ֵ
					else if( ps->Key.Mode==10)
						{   if(two.U16Data<=0)two.U16Data=1;
					pf->Data[FLASHIS_AdrXTSZCHSLSJ]=two.U16Data; //���溬ʪ��ʱ��ֵ	
						}
					pf->Flag=1;
	

				}		
				ScreenBackFirstOpt(ps);
				
				ScreenEnterNumberText(ps,2,2); //��ֵ����
				/*
				if((ps->Key.number>=0x30&&ps->Key.number<=0x39)||ps->Key.number==0x2e)
				{
					ps->Key.Buffer[ps->Key.TwoOpt++]=ps->Key.number;
					if(ps->Key.TwoOpt>4) ps->Key.TwoOpt=4;//�������볤��		
				}
				*/
			}
			break;
		}
	
	}
	
	{ //��Ļ��������
		ps->OldMode=ps->RunMode;
		switch(ps->Button)
		{		
			case 0x0AfE:
			case 0x000D:   //����
			{
				ps->Key.Opt=ps->Key.OldOpt;
				ps->Key.LastOpt=0;
				ps->RunMode =52; //������ҳ��
				ps->RunModeStat=1;//���¾�̬����
				break;
			}
			
			case 0x0001:    //����(��Ļ�����޸�����)
			{
				if(ps->Key.bOutsideKeyboard != true)
				{
					Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5
					ScreenTowOptChose(ps,ps->Button);		//��������ѡ����˸����	
				}	
				else
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					ps->Key.Mode=1;
					ps->Key.TwoOpt=0;
					ps->Key.Opt=1;
					u8 date[15];
					sprintf((char*)date,"%02d-%02d-%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday);
					strcpy((char*)ps->Key.Buffer,(char*)date);
					ps->RunModeStat=1;//ˢ������
				}
				break;
			}
			
			case 0x0002:    //ʱ��
			{
				if(ps->Key.bOutsideKeyboard != true)
				{
					Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5
					ScreenTowOptChose(ps,ps->Button);		//��������ѡ����˸����
					ps->Key.Opt=1;							//ʱ������ڶ���ÿһ���˵���ѡ��
				}	
				else
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					ps->Key.Mode=2;
					ps->Key.TwoOpt=0;
					ps->Key.Opt=1;
					u8 Time[15];
					sprintf((char*)Time,"%02d-%02d-%02d",ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
					strcpy((char*)ps->Key.Buffer,(char*)Time);
					ps->RunModeStat=1;//ˢ������
				}
				break;
			}
			
			case 0x0003:    //����ѹ			
			{
				if((pf->Data[FLASHIS_AdrXTSZBOX]&(1>>0))==0x0000)
				{
					TwoBytesBitFunction(pf->Data[FLASHIS_AdrXTSZBOX],0,1);
					if(ps->Key.bOutsideKeyboard != true)
					{
						
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5
					}	
					else
					{
						TwoBytesBitFunction(pf->Data[FLASHIS_AdrXTSZBOX],0,1);
					}
					ScreenTowOptChose(ps,ps->Button);		//��������ѡ����˸����	
				}else
				{
					TwoBytesBitFunction(pf->Data[FLASHIS_AdrXTSZBOX],0,0);
					ps->Key.Mode=0;
				}
				pf->Flag=1;
				ps->Key.Opt=2;
				ScreenSetJitu(ps,0);	
				ps->RunModeStat=1;//ˢ������
				break;
			}
			
//				case 0x0A06:    //����	
//				{
//					 ps->Key.Opt=3;
//					if(pf->Data[FLASHIS_AdrXTSZCYLX]==0)
//					{
//						pf->Data[FLASHIS_AdrXTSZCYLX]=1;
//					}else
//					{
//						pf->Data[FLASHIS_AdrXTSZCYLX]=0;
//					}
//					ps->Key.Mode=0;
//					ScreenSetJitu(ps,1);
//					break;
//				}	

			case 0x0004:    //����
			{ 
				ps->Key.Opt=3;
				Set_SysCheckBox(FLASHIS_AdrXTSZBOX,1);	
				pf->Flag=1;
				ps->Key.Mode=0;
				ScreenSetJitu(ps,2);	
				ps->RunModeStat=1;//ˢ������
				break;
			}	
			
			case 0x0005:    //��������
			{  
				ps->Key.Opt=4;
				Set_SysCheckBox(FLASHIS_AdrXTSZBOX,2);
				if(pf->Data[FLASHIS_AdrXTSZBOX]&(1<<2))
					AT_Power_On();
				else AT_Power_Off();
				pf->Flag=1;
				ps->Key.Mode=0;
				ScreenSetJitu(ps,3);	
				ps->RunModeStat=1;//ˢ������
				break;
			}	
			
			case 0x0006:   
				ps->Key.Opt=5; 
				ps->Key.Mode=0; 	
				ps->RunModeStat=1;//ˢ������
			    Screen_WriteRegisterOneData(0x4f,0xA0+0X0C);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5
				ScreenTowOptChose(ps,ps->Button-1);		//��������ѡ����˸����	
			    ps->Key.bOutsideKeyboard=0;
				break;//������ַ
				
			case 0x0007:    //�Զ���ˮ����
			{
				Set_SysCheckBox(FLASHIS_AdrXTSZBOX,3);
				pf->Flag=1;
				if(pf->Data[FLASHIS_AdrXTSZBOX ]&0x08)  //�ж��Ƿ�ѡ���Զ���ˮ20200701
				Screen_WaterPumOpenServo();	
				else
				Screen_WaterPumCloseServo();
				ps->Key.Opt=6;
				ps->Key.Mode=0;
				ScreenSetJitu(ps,4);
				ps->RunModeStat=1;//ˢ������
				break;
			}	
			
			case 0x000A:    //��������
			{
				Set_SysCheckBox(FLASHIS_AdrXTSZBOX,4);	
				pf->Flag=1;
				ps->Key.Opt=9;
				ps->Key.Mode=0;
				ScreenSetJitu(ps,9);
				ps->RunModeStat=1;//ˢ������
				break;
			}
			
			case 0x000C:    //�̳���������
			{
				Set_SysCheckBox(FLASHIS_AdrXTSZBOX,6);	//ѡ���ñ�ע
				pf->Flag=1;
				ps->Key.Opt=0X0B;
				ps->Key.Mode=0;
				ScreenSetJitu(ps,7);   //���Ĵ򹳻�ͼ
				ps->RunModeStat=1;//ˢ������
				break;
			}	
			
			case 0x0008:    //��ˮ���
			case 0x0009:    //��ˮʱ��
			case 0x000B:
			{  
				if(ps->Key.bOutsideKeyboard != true)
				{
					Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5
					ScreenTowOptChose(ps,ps->Button - 1);		//��������ѡ����˸����	
				}	
				else
				{
					ps->Key.Opt=ps->Button-1;
					ScreenTowOptChose(ps,ps->Key.Opt);
					//Screen_WriteRegisterOneData(0x4f,0xA8);//����������� 2020-05-19 hjl del
				}
				break;
			}	

		}
	} 
			ScreenChangeDataInit(ps);
	
}
//---
/*
	��������void Button_Sys_CYSZPage(void)
	������	�ú���Ϊϵͳ����-�������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
/*
void Button_Sys_CYSZPage(void)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,7,1); //�ײ㴦��			
			break;
		}
		case 3:
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}		
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnter(ps,FLASHIS_AdrXTSZYQCLSJ,1,30,1);	
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������					
				}
			}	
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrXTSZYQCLSJ,1,30,1);
					Screen_ResetKeyDataBuffer();//�������ֵ
					ps->Key.Mode=4;
					ps->Key.TwoOpt=0;	
					ps->RunModeStat=1;//���¾�̬����				
				}
				else
				{
					ScreenBackFirstOpt(ps);	       //�˳����
				}			    	
				ScreenEnterNumberText(ps,0,2); //��ֵ����
			}
		}break;
		case 4:
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}		
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnter(ps,FLASHIS_AdrXTSZYQCLCS,1,60,1);		//��ƽ������������Ʋ�����60��		
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				 }
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������					
				}
			}			
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrXTSZYQCLCS,1,60,1);			
				}				
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,0,2); //��ֵ����
			}	
		}break;
		case 5:
		{//��������ʱ����
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}		
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnter(ps,FLASHIS_AdrXTSZCHSLSJ,1,100,1);		
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				 }
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������					
				}
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrXTSZCHSLSJ,1,100,1);			
				}				
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,0,2); //��ֵ����
			}
			break;
		}		
	}
	 		{ //��Ļ��������
				ps->OldMode=ps->RunMode;
				switch(ps->Button)
			{		
				case 0x0AfE:
				case 0x0008:   ScreenBackLastPag(ps,0x1000);break;//����
				case 0x0007:   //������ҳ
					{
						ps->Key.Opt=ps->Key.OldOpt;
						ps->Key.LastOpt=0;
						ps->RunMode =52; //������ҳ��
						ps->RunModeStat=1;//���¾�̬����
							break;
					 }
				case 0x0001:    //����
				{
						Set_SysCheckBox(FLASHIS_AdrXTSZBOX,4);	pf->Flag=1;
						ps->Key.Opt=1;
						ps->Key.Mode=0;
						ScreenSetJitu(ps,9);
		      }break;
				case 0x0002:    //��λ
				{
					Set_SysCheckBox(FLASHIS_AdrXTSZBOX,5);	pf->Flag=1;
					ps->Key.Opt=2;
					ps->Key.Mode=0;
					ScreenSetJitu(ps,6);
					break;
				}
				case 0x0003:    //����ʱ��		
				{
					ScreenTowOptChose(ps,3);
										if(ps->Key.bOutsideKeyboard != true)
						{
								Screen_WriteRegisterOneData(0x4f,0xA1);//�����������
						}
					break;
				}				
				case 0x0004:    //����
				{ 
					  ScreenTowOptChose(ps,4);
					  ps->Key.Opt=3;
					  										if(ps->Key.bOutsideKeyboard != true)
						{
								Screen_WriteRegisterOneData(0x4f,0xA2);//�����������
						}
					break;
				}	
				case 0x0005:    //��ʪ��
				{ 
					ScreenTowOptChose(ps,5);
					 ps->Key.Opt=4;
					if(ps->Key.bOutsideKeyboard != true)
						{
								Screen_WriteRegisterOneData(0x4f,0xA3);//�����������
						}
					break;
				}	
				case 0x0006:    //����
				{
	       Set_SysCheckBox(FLASHIS_AdrXTSZBOX,6);	pf->Flag=1;
					ps->Key.Opt=5;
					ps->Key.Mode=0;
					ScreenSetJitu(ps,7);
				}	break;
		
		}
	} 
			ScreenChangeDataInit(ps);
		//ScreenButtonReset();
}
*/
//---
/*20200527
	��������void Button_SysSetPage(void)
	������	�ú���Ϊ�̵��������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/

void Button_FlueSetPage(void)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{	
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,5,1); //�ײ㴦��			
			break;
		}
	}	
	{ //��Ļ��������//��Ļ��������
		  ps->OldMode=ps->RunMode;
				switch(ps->Button)
			{
				case 0x0AFe:
				case 0x0005:   //����
					{
						ps->Key.Opt=ps->Key.OldOpt;
						ps->Key.LastOpt=0;
						ps->RunMode =51; //������ҳ��
						ps->RunModeStat=1;//���¾�̬����
						pf->Flag=1;     //20200522
							break;
					 }
				case 0x0001:    //����Բ��
				{
					ps->RunMode =0x2100; 
					pf->Data[FLASHIS_AdrYDBDLX]=1;
					ps->Key.Opt=1;
					pf->Flag=1;     //20200527
					break;
				}
				case 0x0002:    //���뷽��
				{

					ps->RunMode =0x2200; 
					pf->Data[FLASHIS_AdrYDBDLX]=2;
					ps->Key.Opt=2;
					pf->Flag=1;     //20200527
					break;
				}
				case 0x0003:    ////������������
				{
					ps->RunMode =0x2300; 
                    pf->Data[FLASHIS_AdrYDBDLX]=3;
					ps->Key.Opt=3;
					pf->Flag=1;     //20200527
					break;
				}
				case 0x0004:    //�̵���ѯ
				{
					ps->RunMode =0x2400; 

					ps->Key.Opt=4;
					SdFile_Serch_MaxofFlueLinkFile();  //��������ļ���20200527
					ps->FileNumber=SdFile_GetData()->FlueLinkFileMax;
					break;
				}
				
		}
		ScreenChangeDataInit(ps);
	} 
		//ScreenButtonReset();
}
//---
/*20200527
	��������void Button_FlueCircularPage(void)
	������	�ú���Ϊ�̵�����-Բ�����ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
    �޸ļ�¼�� 2020-05-25 modify by alfred,  �޸ĳ�ʱ����
*/

void Button_FlueCircularPage(void)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	
	// u16 Single_hole_Number=1;

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{	
		case 0:
		{//û��ѡ�ж�������
			ScreenFirstOpButton(ps,8,1); //�ײ㴦��	
			ps->Key.LastMode=0;			
			break;
		}			
		case 1:
		{//���������ھ�
			
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBDYXNJ,2,50.0,0.01,CALC_HAVE_POINT,CALC_NO_SIGNS);	
	
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //ȷ��
				{	
					ScreenConfirmEnter(ps,FLASHIS_AdrYDBDYXNJ,2,50.0,0.01);
		
					ps->Key.LastMode= ps->Key.Mode;	

				}      
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,2,5); //��ֵ����
			}
			break;
		}
		case 2:
		{//�����������
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBD_MJ,2,2500.0,0.0001,CALC_HAVE_POINT,CALC_NO_SIGNS);		
                				
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrYDBD_MJ,2,2500.0,0.0001);
					ps->Key.LastMode= ps->Key.Mode;	

					
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,2,9); //��ֵ����
			}
			break;
		}
		case 3:
		{ //�����������
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBD_GT,2,10.0,0.01,CALC_HAVE_POINT,CALC_NO_SIGNS);				
			}			
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //ȷ��
				{				 
				ScreenConfirmEnter(ps,FLASHIS_AdrYDBD_GT,2,10.0,0.01);
				ps->Key.LastMode= ps->Key.Mode;					
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,2,5); //��ֵ����
			}
			break;
		}
		case 4:
		{ //����������
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBDYXHS,1,5,0,CALC_NO_POINT,CALC_NO_SIGNS);		
             	
			}	
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrYDBDYXHS,1,5,0);
					
					ps->Key.LastMode= ps->Key.Mode;	
                 
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,0,1); //��ֵ����
			}
			break;
		}
		case 5:
		{ //�����������
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBDYXKS,1,10,1,CALC_NO_POINT,CALC_NO_SIGNS);				
			}	
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrYDBDYXKS,1,10,1);			
					ps->Key.LastMode= ps->Key.Mode;				
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,0,2); //��ֵ����
			}
			break;
		}	
		case 6:
		{  //���������ַ
			if(ps->Key.bOutsideKeyboard != true)
			{
			ScreenTowOpButton_KeyControl_adr(ps);
	
			if(ps->Button==0x0Afe)
			{     
			  ScreenBackLastOpt(ps);
			}
			}
			break;
			
		}
	}
	 { //��Ļ��������
		switch(ps->Button)
		{				
			case 0x0AfE:

			case 0x0008:   //������һҳ 
			{
				ScreenBackLastPag(ps,0x2000);
				break;
			}
			case 0x0006:   
			{
				// to do ��ַ����
				ps->Key.Opt=6; 
				ps->Key.Mode=0; 	
				ps->RunModeStat=1;//ˢ������
			    Screen_WriteRegisterOneData(0x4f,0xA0+0X0C);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5
				ScreenTowOptChose(ps,ps->Button);		//��������ѡ����˸����	
			    ps->Key.bOutsideKeyboard=0;

				break;
			}
			case 0x0007:  
			{
				// to do ����Բ���̵�����
				pf->Flag=1;
				float saveData[21];
				memset(saveData,0,sizeof(saveData));
				saveData[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_MJ);  //���
				saveData[1]=1;                                               //����
				saveData[2]=(float)pf->Data[FLASHIS_AdrYDBDYXKS];                   //Բ�ο�����������������, modify by alfred, 2020.07.03
				saveData[3]=(float)pf->Data[FLASHIS_AdrYDBD_CYDS];                  //����
				for(int i=0;i< pf->Data[FLASHIS_AdrYDBDYXHS]*2;i++)	// modify by alfred, 2020.07.15
				{
					saveData[i+4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDCDJL1+i*2);
				}
				saveData[15]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDYXNJ);   //�ھ�D
				saveData[16]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT);    //�׹�L
				saveData[17]=(float)pf->Data[FLASHIS_AdrYDBDYXHS];   //����
				saveData[18]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBA);   //��A��
				saveData[19]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBB);   //��B��
				saveData[20]=(float)pf->Data[FLASHIS_AdrYDBDFXDKCD]; //���ײ��

				SaveFlueData(saveData);
				ps->Key.Mode=0;
				break;
			}
			default :
			{
				if(ps->Button>0&&ps->Button<=5)
				{
					if(ps->Key.bOutsideKeyboard != true)
					{
						if(ps->Button<6)
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5
					}							
						ScreenTowOptChose(ps,ps->Button);		//��������ѡ����˸����	
				}
			}	
			break;				
		}
	} 
}
//---
/*20200527
	��������void Button_FlueSeekPage(void)
	������	�ú���Ϊ�̵�����-����-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25 modify by alfred,�޸ĳ�ʱ����
*/

void Button_FlueSeekPage(float *FileCode)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,5,1); //�ײ㴦��			
			break;
		}			
		case 1:
		{//�̵���ѯ--�ļ���
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInputFileName(ps,FileCode,0,9999999,1,CALC_NO_POINT,CALC_NO_SIGNS);				
			}	
			else
			{				
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //ȷ��
				{	
					ScreenConfirmEnterDisBuffer(ps,FileCode,0,9999999,1);
				}      
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,0,7); //��ֵ����
			}
			break;
		}		
	}
	 { //��Ļ��������
				switch(ps->Button)
			{				
				case 0x0AfE:
				
				case 0x0005:   //������һҳ 
					{
					ScreenBackLastPag(ps,0x2000);
//					ps->Key.Opt = 0;
							break;
					}
						case 0x0002:   //��Ѱ
					{
						if(*FileCode>1)
							(*FileCode)--;
					    ReadFlueData(*FileCode);ps->Key.Mode=0; ps->Key.Opt= ps->Button;	
							break;
					 }
							case 0x0003:   //��Ѱ
					{
						if(*FileCode<SdFile_GetData()->FlueLinkFileMax)
							(*FileCode)++;
					    ReadFlueData(*FileCode);ps->Key.Mode=0; ps->Key.Opt= ps->Button;	
							break;
					}
          			case 0x0004:   //����
					{
						ApplicationFlueFileData();
						ScreenBackLastPag(ps,0x2000);ps->Key.Mode=0;
						if(pf->Data[FLASHIS_AdrYDBDLX]==1)             //20200527
						ps->Key.LastMode=2;
						else
						if(pf->Data[FLASHIS_AdrYDBDLX]==2)
						ps->Key.LastMode=3;	
						pf->Flag=1;
							break;
					 }

					default :
				{
				  if(ps->Button==1)
							{
								if(ps->Key.bOutsideKeyboard != true)
								Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5
								ScreenTowOptChose(ps,ps->Button);		//��������ѡ����˸����	
							}	
           if(ps->Button<=5&&ps->Button>0) ps->Key.Opt= ps->Button;					
				}	
					break;				
		}
	} 
}
//---
/*20200527
	��������void Button_FlueSquarePage(void)
	������	�ú���Ϊ�̵�����-�������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25 modify by alfred,�޸ĳ�ʱ����
*/

void Button_FlueSquarePage(void)
{
  Screen_DataStructure *ps = Screen_GetData();
  Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,9,1); //�ײ㴦��			
			break;
		}
			
		case 1:	
		{//��������ױ�A
				if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBDFXBA,2,50.0,0.01,CALC_HAVE_POINT,CALC_NO_SIGNS);				
							}	
				else{			
								ScreenTowOpButton(ps,1,0,0);
								if(ps->Button==0x0Aff) //ȷ��
										{
												ScreenConfirmEnter(ps,FLASHIS_AdrYDBDFXBA,2,50.0,0.01);

											    ps->Key.LastMode= ps->Key.Mode;		
										} 
								ScreenBackFirstOpt(ps);	       //�˳����
								ScreenEnterNumberText(ps,2,5); //��ֵ����
						}
			break;
	  }
		case 2:
		{//��������ױ�B
				if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBDFXBB,2,50.0,0.01,CALC_HAVE_POINT,CALC_NO_SIGNS);				
							}	
				else{
								ScreenTowOpButton(ps,1,0,0);
								if(ps->Button==0x0Aff) //ȷ��
										{	
												ScreenConfirmEnter(ps,FLASHIS_AdrYDBDFXBB,2,50.0,0.01);ps->Key.LastMode= ps->Key.Mode;		
										} 
								ScreenBackFirstOpt(ps);	       //�˳����
								ScreenEnterNumberText(ps,2,5); //��ֵ����
						}
			break;
	  }
		case 3:
		{//��� 
				if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBD_MJ,2,2500.0,0.0001,CALC_HAVE_POINT,CALC_NO_SIGNS);				
							}
				else{							
								ScreenTowOpButton(ps,1,0,0);
								if(ps->Button==0x0Aff) //ȷ��
										{
												ScreenConfirmEnter(ps,FLASHIS_AdrYDBD_MJ,2,2500.0,0.0001);ps->Key.LastMode= ps->Key.Mode;						
										} 
								ScreenBackFirstOpt(ps);	       //�˳����
								ScreenEnterNumberText(ps,2,9); //��ֵ����
						}
			break;
		}
		case 4:
		{ //�׹�
				if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBD_GT,2,10.0,0.01,CALC_HAVE_POINT,CALC_NO_SIGNS);				
							}
				else{							
								ScreenTowOpButton(ps,1,0,0);
								if(ps->Button==0x0Aff) //ȷ��
										{
												ScreenConfirmEnter(ps,FLASHIS_AdrYDBD_GT,2,10.0,0.01);ps->Key.LastMode= ps->Key.Mode;						
										} 
								ScreenBackFirstOpt(ps);	       //�˳����
								ScreenEnterNumberText(ps,2,5); //��ֵ����
						}
			break;
		}
		case 5:
		{ //����
			if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBDFXKS,1,10,1,CALC_HAVE_POINT,CALC_NO_SIGNS);				
							}
			else{
								ScreenTowOpButton(ps,1,0,0);
								if(ps->Button==0x0Aff) //ȷ��
											{
													ScreenConfirmEnter(ps,FLASHIS_AdrYDBDFXKS,1,10,1);ps->Key.LastMode= ps->Key.Mode;					
											} 
								ScreenBackFirstOpt(ps);	       //�˳����
								ScreenEnterNumberText(ps,0,2); //��ֵ����
						}
			break;
		}
			case 6:
		{//���ײ��
			if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBDFXDKCD,1,10,1,CALC_HAVE_POINT,CALC_NO_SIGNS);				
							}			
			else{
								ScreenTowOpButton(ps,1,0,0);
								if(ps->Button==0x0Aff) //ȷ��
											{
												ScreenConfirmEnter(ps,FLASHIS_AdrYDBDFXDKCD,1,10,1);ps->Key.LastMode= ps->Key.Mode;						
											} 
								ScreenBackFirstOpt(ps);	       //�˳����
								ScreenEnterNumberText(ps,0,2); //��ֵ����
				}
			break;
		}	
			case 7:
			{
              //���������ַ
			if(ps->Key.bOutsideKeyboard != true)
			{
			ScreenTowOpButton_KeyControl_adr(ps);
	
			if(ps->Button==0x0Afe)
			{     
			  ScreenBackLastOpt(ps);
			}
			}
			break;
			}				
	
	}
	
	 { //��Ļ��������
				switch(ps->Button)
			{
				
				case 0x0AfE:
				
				case 0x0009:   //������һҳ
					{
					ScreenBackLastPag(ps,0x2000);
							break;
					 }
						case 0x0007:   //������һҳ
					{
					 // to do ��ַ����
					   ps->Key.Opt=7; 
				       ps->Key.Mode=0; 	
				       ps->RunModeStat=1;//ˢ������
			           Screen_WriteRegisterOneData(0x4f,0xA0+0X0C);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5
				       ScreenTowOptChose(ps,ps->Button);		//��������ѡ����˸����	
			           ps->Key.bOutsideKeyboard=0;

					   break;
					 }
					case 0x0008:   
					{
					 // to do ���淽���̵�����

						float saveData[21];
						memset(saveData,0,sizeof(saveData));
						saveData[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_MJ);  //���
						saveData[1]=2;                                               //����
						saveData[2]=(float)pf->Data[FLASHIS_AdrYDBDFXKS];                   //����
						saveData[3]=(float)pf->Data[FLASHIS_AdrYDBD_CYDS];                  //����
						for(int i=0;i<pf->Data[FLASHIS_AdrYDBDFXDKCD];i++)
						{
								saveData[i+4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDCDJL1+i*2);
						}
						saveData[15]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDYXNJ);   //�ھ�D
						saveData[16]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT);    //�׹�L
						saveData[17]=(float)pf->Data[FLASHIS_AdrYDBDYXHS];   //����
						saveData[18]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBA);   //��A��
						saveData[19]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBB);   //��B��
						saveData[20]=(float)pf->Data[FLASHIS_AdrYDBDFXDKCD]; //���ײ��

						SaveFlueData(saveData);
						ps->Key.Mode=0; ps->Key.Opt= ps->Button;
							pf->Flag=1;
							break;
					 }
				default :
				{
				  if(ps->Button>0&&ps->Button<=6)
						{
							if(ps->Key.bOutsideKeyboard != true)
									Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5
							ScreenTowOptChose(ps,ps->Button);		//��������ѡ����˸����	

						}			
				}
				break;
				
		}
	} 
	//ScreenButtonReset();
}
/*
	��������void Button_FlueOtherPage(void)
	������	�ú���Ϊ�̵�����-�������水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25 modify by alfred,�޸ĳ�ʱ����
*/

void Button_FlueOtherPage(void)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,5,1); //�ײ㴦��			
			break;
		}	
		case 1:
		{//���������̵�����
				if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBD_MJ,2,100.0,0.01,CALC_HAVE_POINT,CALC_NO_SIGNS);				
							}	
				else{							
								ScreenTowOpButton(ps,1,0,0);  //���
								if(ps->Button==0x0Aff) //ȷ��
										{
												ScreenConfirmEnter(ps,FLASHIS_AdrYDBD_MJ,2,100.0,0.01);	
										} 
								ScreenBackFirstOpt(ps);	       //�˳����
								ScreenEnterNumberText(ps,2,8); //��ֵ����
						}
			break;
	  }
		case 2:
		{//��������
				if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBD_CYDS,1,10,1,CALC_NO_POINT,CALC_NO_SIGNS);				
							}	
				else{				
								ScreenTowOpButton(ps,1,0,0);//��������
								if(ps->Button==0x0Aff) //ȷ��
										{				
												ScreenConfirmEnter(ps,FLASHIS_AdrYDBD_CYDS,1,10,1);
										} 
								ScreenBackFirstOpt(ps);	       //�˳����
								ScreenEnterNumberText(ps,2,2); //��ֵ����
						}
			break;
	  }	
		case 3:
        {
			//���������ַ
			if(ps->Key.bOutsideKeyboard != true)
			{
			ScreenTowOpButton_KeyControl_adr(ps);
	
			if(ps->Button==0x0Afe)
			{     
			  ScreenBackLastOpt(ps);
			}
			}
			break;
		}			
	}
	
	{ //��Ļ��������
				switch(ps->Button)
			{
				
				case 0x0AfE:
				
				case 0x0005:   //������һҳ
					{
					ScreenBackLastPag(ps,0x2000);
							break;
					 }
						case 0x0003:   //������һҳ
					{
					 // to do ��ַ����
					  ps->Key.Opt=3; 
				      ps->Key.Mode=0; 	
				      ps->RunModeStat=1;//ˢ������
			          Screen_WriteRegisterOneData(0x4f,0xA0+0X0C);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5
				      ScreenTowOptChose(ps,ps->Button);		//��������ѡ����˸����	
			          ps->Key.bOutsideKeyboard=0;

					  break;
					 }
					case 0x0004:   //������һҳ
					{
						// to do ���������̵�����
						pf->Flag=1;
						float saveData[21];
						memset(saveData,0,sizeof(saveData));
						saveData[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_MJ);  //���
						saveData[1]=3;                                               //����
						saveData[2]=(float)pf->Data[FLASHIS_AdrYDBDFXKS];                   //����
						saveData[3]=(float)pf->Data[FLASHIS_AdrYDBD_CYDS];                  //����
						for(int i=0;i<pf->Data[FLASHIS_AdrYDBDYXHS];i++)
						{
							//saveData[i+4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDCDJL1+i*2);	//�����̵������ݲ���ȷ������0��modify by alfred, 2020.07.15
						}
						saveData[15]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDYXNJ);   //�ھ�D
						saveData[16]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT);    //�׹�L
						saveData[17]=(float)pf->Data[FLASHIS_AdrYDBDYXHS];   //����
						saveData[18]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBA);   //��A��
						saveData[19]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBB);   //��B��
						saveData[20]=(float)pf->Data[FLASHIS_AdrYDBDFXDKCD]; //���ײ��

						SaveFlueData(saveData);
						ps->Key.Mode=0; ps->Key.Opt= ps->Button;
						break;
					 }
				default :
					{
						if(ps->Button>0&&ps->Button<=2)
								{
									if(ps->Key.bOutsideKeyboard != true)
												Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5
									ScreenTowOptChose(ps,ps->Button);		//��������ѡ����˸����	
								}			
					}
					break;
		}
	} 
	//ScreenButtonReset();
}
//--
/*
	��������void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����Ԥ�����ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
  //����������
	switch(ps->Key.Mode)//�鿴��������
	{
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		
	}	
	 { //��Ļ��������
		   ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
				  ScreenBackLastPag(ps,51);
				}
				else if(ps->Button!=0xffff)  //�����Ӧ�Ľ���
				{
				 	ps->RunMode =ps->RunMode+0x100*ps->Button; 	
					  for(int i=0;i<8;i++)
                        {
				         ps->DisValue[10+i]=0;
			            }			
				}
	ScreenChangeDataInit(ps);
	} 
	//ScreenButtonReset();
}
//---
/*
	��������void Button_WCP_PressureZeroPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)

	������	�ú���Ϊ����Ԥ��������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WCP_PressureZeroPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}

		}	
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x3000);
			
				}
				else if(ps->Button==(0x001))  //����ֵ����
				{
				  //to do ȷ�ϵ���
					ps->Function=2;
					ps->TimerSec=0;
					
				}
		ScreenChangeDataInit(ps);
	} 
	//ScreenButtonReset();
}
//--
/*
	��������void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����Ԥ���������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25 modify by alfred,�޸ĳ�ʱ����
*/
void Button_WCP_FumeTemperaturPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		case 0:
		{//û��ѡ�ж�������
			ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:		
		{	
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrGKYC_YWCL_SRYW,2,800.0,0.5,CALC_HAVE_POINT,CALC_NO_SIGNS);				
			}	
			else
			{							
				ScreenTowOpButton(ps,1,0,0);  //����
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrGKYC_YWCL_SRYW,2,800.0,0.5);					
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,2,5); //��ֵ����
			}
			break;
		}				
	}
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
		if(ps->Button==0x0AFe||ps->Button==Max_Opt)
		{
			 ScreenBackLastPag(ps,0x3000);
		}
		else if(ps->Button==(0x0001))  //����ֵ����
		{
			//to do ����ѡ��
			 if(pf->Data[FLASHIS_AdrGKYC_YWCL_FS]==0)
			 {
				 pf->Data[FLASHIS_AdrGKYC_YWCL_FS]=1;pf->Flag=1;
				 if(ps->Key.bOutsideKeyboard != true)
									Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5
				 ScreenTowOptChose(ps,1);	
				 
			 }else
			 {
				 pf->Data[FLASHIS_AdrGKYC_YWCL_FS]=0;pf->Flag=1;
				 ps->Key.Mode=0;
			 }
		}
	ScreenChangeDataInit(ps);
	} 
	//ScreenButtonReset();
} 
//---
/*
	��������void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����Ԥ��Ԥ���������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WCP_PredictedVelocityPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
Slave_DataStructure* Sp=Slave_GetData();

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		}	
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					ScreenBackLastPag(ps,0x3000);			
				}
				else if(ps->Button==(0x0001))  //����
				{
				  //to do ����Ԥ��  �ȼ���ƽ���ٱ���
					if(ps->StarCun==1)
					{
					 ScreenBackLastPag(ps,0x3000);return;
					}
					for(int i=0;i<8;i++)
					{
					  DisBuff[i]=DisBuff[i+10]/(ps->StarCun-1);
					}
					//to du ����
					DisBuff[8]=CalculationFormula_GetData()->Qsnd;//�������
					float SaveBuff[20]={0};
					SaveBuff[0]=DisBuff[0]; //ƽ����ѹ
					SaveBuff[1]=DisBuff[2];//ƽ����ѹ
					SaveBuff[2]=DisBuff[1];//ƽ��ȫѹ
					SaveBuff[3]=DisBuff[3];//����ѹ
					SaveBuff[4]=DisBuff[6];//��ȡ�̵����
					SaveBuff[5]=CalculationFormula_d(25.0f,DisBuff[4],CalculationFormula_GetData()->Xsw);//��A·�����������ֱ��
					SaveBuff[6]=DisBuff[5]; //����
					SaveBuff[7]=DisBuff[4];//��������
					SaveBuff[8]=DisBuff[7];//��������
					
					SaveBuff[9]=CalculationFormula_Qsnd(SaveBuff[8],SaveBuff[6],CalculationFormula_GetData()->Ps+CalculationFormula_GetData()->Ba,CalculationFormula_GetData()->Xsw); //�������   2021.3.9
					
					SaveBuff[10]=CalculationFormula_GetData()->Xsw*100;//��ʪ��
					SaveBuff[11]=GetFlaotdataFormFlash2Word(FLASHIS_AdrCLYQ_WJCX_JS_ZSXS);//����ϵ��
					SaveBuff[12]=GetFlaotdataFormFlash2Word(FLASHIS_AdrCLYQ_WJCX_JS_FHXS);//����ϵ��
					SaveBuff[13]=CalculationFormula_GetData()->Kp ;//Ƥ �� ��
					SaveWorkData(SaveBuff,0x00);
					SdFile_Serch_MaxofWorkLinkFile();//�ȶ�ȡ����ļ���
					SdFile_WriteWorkFile(&SdFile_GetData()->InterFace_data,SdFile_GetData()->WorkLinkFileMax+1);  //д��6���ļ�
                    //����һЩƽ��ֵ�Ա�����ʱ����
                    SaveFlaotToFlash(DisBuff[4],FLASHIS_AdrGKYC_YQLS_YQLS); //20210311   �����ﱣ��ƽ������ֵ
                    SaveFlaotToFlash(DisBuff[0],FLASHIS_AdrGKYC_CPJZ_PJDY); //20210311   �����ﱣ��ƽ����ѹֵ
			        SaveFlaotToFlash(DisBuff[2],FLASHIS_AdrGKYC_CPJZ_PJJY); //20210311   �����ﱣ��ƽ����ѹֵ
					SaveFlaotToFlash(DisBuff[1],FLASHIS_AdrGKYC_CPJZ_PJQY); //20210311   �����ﱣ��ƽ��ȫѹֵ
					SaveFlaotToFlash(DisBuff[5],FLASHIS_AdrGKYC_CPJZ_PJYW); //20210311   �����ﱣ��ƽ������ֵ
					SaveFlaotToFlash(DisBuff[7],FLASHIS_AdrGKYC_CPJZ_YGLL); //20210311   �����ﱣ��ƽ������ֵ
                    SaveFlaotToFlash(SaveBuff[9],FLASHIS_AdrGKYC_CPJZ_BGLL); //20210311   �����ﱣ��ƽ������ֵ
					SaveFlaotToFlash(SaveBuff[5],FLASHIS_AdrYCCJ_CYSE_CZZJ);//20210311   �����ﱣ�������ֱ��
					
                    //��ת����
					ps->RunMode=0x3400;//��ת����ֵ�ļ���ѯ
					ps->Key.Opt=1;
				}
				else if(ps->Button==(0x0002))  //ȷ�ϵ�ǰ
				{
				  //to do ȷ�ϵ�ǰֵ   ����ScreenData.Con �����ڼ���
					ps->StarCun++;
					if(ps->StarCun>99) 
					{
						ps->StarCun=99;
					}
					else
					{
					  for(int i=0;i<8;i++)
						{
						  DisBuff[10+i]+=DisBuff[i];
						}
					}

					ps->Key.Opt=2;
					ps->RunModeStat=1;
				}
	ScreenChangeDataInit(ps);
	} 
}
//---
/*
	��������void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����Ԥ���ѯƽ��ֵ���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25 modify by alfred,�޸ĳ�ʱ����
*/
void Button_WCP_CheckAveragePag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
    Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:		
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInputFileName(ps,DisBuff,0,9999999,0,CALC_NO_POINT,CALC_NO_SIGNS);				
			}				
			else{
					ScreenTowOpButton(ps,1,0,0);  //�ļ���
					if(ps->Button==0x0Aff) //ȷ��
					{
						ScreenConfirmEnterDisBuffer(ps,DisBuff,0,9999999,0);
					} 
					ScreenBackFirstOpt(ps);	       //�˳����
					ScreenEnterNumberText(ps,0,7); //��ֵ����
				}
			break;
		}		
	}
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x3000);
			
				}
				else if(ps->Button==(0x0001))  //����ֵ����
				{
						if(ps->Key.bOutsideKeyboard != true)
									Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5
				  //to do �����ļ����
						 ScreenTowOptChose(ps,1);	
				
				}else if(ps->Button==0x002)  //����ֵ����
				{
				  //to do ��Ѱ
			
					 if(ps->FileNumber>1)  ps->FileNumber--;    //20200528
					  ps->RunModeStat=1;
					  ps->Key.Opt=2;
				
				}else if(ps->Button==0x0003)  //����ֵ����
				{
				  //to do ��ѮѰ
						if(ps->FileNumber<SdFile_GetData()->WorkLinkFileMax)  ps->FileNumber++;
					  ps->RunModeStat=1;
					 ps->Key.Opt=3;
						
				}else if(ps->Button==0x0004)  //����ֵ����
				{
				  //to do ��ӡ
					 ps->Key.Opt=4;
					
					if(AT_GetData()->Mode==0)
					{
					u8 pn[33];
					float buffer[5];
					for(int i=0;i<33;i++)
					 pn[i]=1;
					buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
					buffer[1]=ps->FileNumber;
					buffer[2]=ps->FileNumber;
					buffer[3]=ps->FileNumber;
			        PrintWorkFile((u8 *)&pn[0],(float *)&buffer[0]);
					}
					else
					{
						Screen_ResetDataBuffer();
	                    strcpy((char *)ps->Buffer,"û���Ӵ�ӡ��!");//����ʾ20200519
	                    Screen_RefreshDataMulti(0x3B20,ps->Buffer,15*2);
					}

					
				}
		ScreenChangeDataInit(ps);
	} 
	//ScreenButtonReset();
}
//---2020-05-21
/*
	��������void Button_CHSL_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ�⺬ʪ�����ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25 modify by alfred,�޸ĳ�ʱ����
*/
void Button_CHSL_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 3:
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnter(ps,FLASHIS_AdrCHSL_HSL,2,99.99,0.00);
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}		
			}
			else{	
					ScreenTowOpButton(ps,1,0,0);  
					if(ps->Button==0x0Aff) //ȷ��
					{
						ScreenConfirmEnter(ps,FLASHIS_AdrCHSL_HSL,2,99.99,0.00);					
					} 
					ScreenBackFirstOpt(ps);	       //�˳����
					ScreenEnterNumberText(ps,2,5); //��ֵ����
					}
			break;
		}			
	}
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt||ps->Button==5)  //����Ϊ5������Ϊ4 �����ؼ���
				{
					 ScreenBackLastPag(ps,51);
			
				}
				else if(ps->Button==0x0001)  //����ֵ����
				{
				  //to do ��ʪ�򷨲���
					ps->Key.Opt=1;			    
					 ps->RunMode=0x4100;	
				
				}else if(ps->Button==0x0002)  //����ֵ����
				{
				  //to do ���ݷ�����
					 ps->RunMode=0x4200;	
						ps->Key.Opt=2;
		
				
				}else if(ps->Button==(0x0003))  //����ֵ����
				{
				  //to do �ֶ�����
							if(ps->Key.bOutsideKeyboard != true)
										Screen_WriteRegisterOneData(0x4f,0xA1);//�����������					
							 ScreenTowOptChose(ps,3);	
						}
				/*
				else if(ps->Button==(0x0004))  //����ֵ����
				{
				  //to do ��ʪ����ѯ
			     ps->RunMode=0x4400;	
						ps->Key.Opt=4;
				}
						*/
		ScreenChangeDataInit(ps);
	} 
	//ScreenButtonReset();
}
/*
	��������void Button_CHSL_GSQF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ�⺬ʪ��-��ʪ�����ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CHSL_GSQF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
	}
	 { //��Ļ��������
		   ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)  //����
				{
					 ScreenBackLastPag(ps,0x4000);
					 Flux_StopSampling('B');
				}
				else if(ps->Button==(0x0001)||((ps->mystatus==0)&&(ps->RunModeStat==2)&&(ps->TimerSec==0)))  //����ֵ����
				{
					ps->Key.Opt=1;
					FourBytes four;
					four.FloatData=DisBuff[0];//CalculationFormula_GetData()->Xsw;
					SaveFlaotToFlash(four.FloatData,FLASHIS_AdrCHSL_HSL);
					ScreenBackLastPag(ps,0x4000);
					Flux_StopSampling('B');
					Speak_Buzzer(200);//��������ʾ	
				  //to do ȷ��
					
				}
	ScreenChangeDataInit(ps);
	} 
}
/*
	��������void Button_CHSL_ZRF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ�⺬ʪ��-���ݷ����ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CHSL_ZRF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
	}
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x4000);
				}
				else if(ps->Button==0x0001)  //����ֵ����
				{
				  //to do ȷ��
					ps->Key.Opt = 1;
					ScreenBackLastPag(ps,0x4000);
				}
	ScreenChangeDataInit(ps);
	} 
	//ScreenButtonReset();
}
//-----
/*	��������void Button_CHSL_ZRF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ�⺬ʪ��-��ʪ����ѯ-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CHSL_HSLCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:		ScreenTowOpButton(ps,1,0,0);  //����
							if(ps->Button==0x0Aff) //ȷ��
							{
									ScreenConfirmEnterDisBuffer(ps,DisBuff,0,9999999,0);
							} 
							ScreenBackFirstOpt(ps);	       //�˳����
							ScreenEnterNumberText(ps,0,7); //��ֵ����
			break;
	  }
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x4000);
			
				}
				else if(ps->Button==(0x0001))  //����ֵ����
				{
				  //to do �����ļ����
					
						 ScreenTowOptChose(ps,1);	
				
				}else if(ps->Button==0x002)  //����ֵ����
				{
				  //to do ��Ѱ
			    ps->Key.Opt=2;					
				
				}else if(ps->Button==0x0003)  //����ֵ����
				{
				  //to do ��ѮѰ
						ps->Key.Opt=3;		
				}else if(ps->Button==0x0004)  //����ֵ����
				{
					ps->Key.Opt=4;		
				  //to do ��ӡ
			
				}
		ScreenChangeDataInit(ps);
	} 
	//ScreenButtonReset();
}
//---
/*
//20200517	,20200522
	��������void Button_CLYQ_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ�����������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
    �޸ļ�¼��	1. 2020-05-25,�޸ĳ�ʱ���� modify by alfred.
				2. ȥ����ʱʱ�������Ӽ�ƽ��ʱ��ʹ���������,2020.06.13, modify by alfred.
*/
void Button_CLYQ_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 4://��ƽ��ʱ��
		case 5://��ƽ������		
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrXTSZYQCLSJ + (ps->Key.Mode-4),1,60,1,CALC_NO_POINT,CALC_NO_SIGNS);	//��������ƽ������������60��
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrXTSZYQCLSJ + (ps->Key.Mode-4),1,60,1);
					if(ps->Key.Mode == 4)
					{
						ps->Key.Mode=5;
						ps->Key.TwoOpt=0;
					}
					else ScreenBackFirstOpt(ps);
				}
				if(ps->Button==0x0Afe)
				{
					ScreenBackFirstOpt(ps);	 
				}					//�˳����
				ScreenEnterNumberText(ps,0,2); //��ֵ����
			}
			break;
		}
	}
		
	{ //��Ļ��������
		ps->OldMode=ps->RunMode;
		if(ps->Button==0x0AFe||ps->Button==0x0006)
		{
			ScreenBackLastPag(ps,51);
	
		}
		else if(ps->Button==0x0001)  //����ֵ����
		{
			//to do ����У׼		
			ps->RunMode=0x5100;	
		
		}
		else if(ps->Button==0x0002)  //����ֵ����
		{
			//to do ��ʼ����	  
			ps->RunMode=0x5200;		
            ScreenJPG_ClearSum(); //����ۼ�ƽ������
			
		}
		else if(ps->Button==0x0003)  //����ֵ����
		{
			//to do ������ѯ
			ps->RunMode=0x5300;	
			ScreenTowOptChose(ps,3);	
		}
		else if(ps->Button==0x0004||ps->Button==0x0005)  //����ֵ����
		{ //��ʱʱ��
			if(ps->Key.bOutsideKeyboard != true)
				Screen_WriteRegisterOneData(0x4f,0xA1+(ps->Button-4));//�����������		
			ScreenTowOptChose(ps,ps->Button);	
		}
		ScreenChangeDataInit(ps);
	} 
	//ScreenButtonReset();
}
//----
/*
	��������void Button_CLYQ_YQJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ������-����У׼���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CLYQ_YQJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
	  }
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0X5000);
			     Flux_StopSampling('C');//�ر�����	
				}
				else if(ps->Button==0x0001)  //����ֵ����
				{
				  //to do У׼�߼�
				    ps->TimerSec=10;
					  ps->Key.Opt=1;
					  ps->Function=1;
					//  ps->Function=2;
					// ps->RunMode=0x5100;	
				
				}
				
	ScreenChangeDataInit(ps);
	}
}


//---2020-06-05
/*
	��������Button_CLYQ_JIPINGJUN_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ�⺬ʪ�����ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��
*/
void Button_CLYQ_JIPINGJUN_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	 SdFile_DataStruct* SdFp=SdFile_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{		
		case 0:
		{	//û��ѡ�ж�������
			ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:
		case 2:	
		case 3:
		case 4:				
		case 5:			
		{
			break;
		}
		default: break;
	}
		
	{
		//��Ļ��������
		ps->OldMode=ps->RunMode;
		if(ps->Button==0x0AFe||ps->Button==Max_Opt)
		{
			 ScreenBackLastPag(ps,51);
//			ps->JiPingJun.ucCurrentPage = 0xff; //��ʼ����ǰҳ��ֵ��
		}
		else if(ps->Button==0x0001)  //����ֵ����
		{
			//to do ���
			ps->Key.Opt=1;			    
			//to do �����߼�
			ps->JiPingJun.cIsCalcAverage = 0;
			ps->JiPingJun.cAverageCount = 0;
			ps->RunMode=0x5210;//��ʾ�Ƿ����
			ps->Function=0;
			ps->JiPingJun.ucCurrentPage = 0xff; //��ʼ����ǰҳ��ֵ��
			
			//��һ��ƽ������
			//�Ѳ���ʱ������
			Flux_GetData()->ChannelC.Sampling.SecTimer=Flux_GetData()->ChannelC.Sampling.SecTimer-Flux_GetData()->ChannelC.Sampling.SecTimer%(pf->Data[FLASHIS_AdrXTSZYQCLSJ]*60);

			Flux_GetData()->ChannelC.Sampling.JpjFlg=0; //���ƽ����־
			Flux_GetData()->ChannelC.RunMode=4;
			Flux_StarSampling('C',1.0) ;//������ 
			Flux_GetData()->ChannelC.Sampling.DXTime=FLUX_FangDaoXiTimeC;   //180����ϴ
		
		}else if(ps->Button==0x0002)  //��һҳ
		{
			//to do ��һҳ
			//��ʾ��һҳ���ݣ�
				ps->Key.Opt=2;	
			if(ps->JiPingJun.uMinDataPage>0)
			{
				ps->JiPingJun.uMinDataPage--;
				ps->JiPingJun.Timer = JPG_DELAY_TIME;	
			}
			ps->RunModeStat=1;
			
		}else if(ps->Button==(0x0003))  //��һҳ
		{
			//to do ��һҳ
			//��ʾ��һҳ���ݣ�
			if(ps->JiPingJun.uMinDataPage < (ps->JiPingJun.cAverageCount-1)/TABLE_ROW)	//�����߼�����, modify by alfred, 2020.07.14
			{
				ps->JiPingJun.uMinDataPage++;
				ps->JiPingJun.Timer = JPG_DELAY_TIME;
			}
			ps->Key.Opt=3;
			ps->RunModeStat=1;
		}
		else if(ps->Button==(0x0004))  //��λ�л�
		{
			//to do ��λת��		 	
			ps->Key.Opt=4;
			if(pf->Data[FLASHIS_AdrCLYQ_DWLX]==0)
			{
				pf->Data[FLASHIS_AdrCLYQ_DWLX]=1;
			}else
			{
				pf->Data[FLASHIS_AdrCLYQ_DWLX]=0;
			}
			ps->JiPingJun.Timer = JPG_DELAY_TIME;
			ps->RunModeStat=1;
		
		}
		ScreenChangeDataInit(ps);
	} 
	//ScreenButtonReset();
}


/*
20200513
	��������void Button_CLYQ_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ��������-��ʼ�������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
extern const u8 FOLDER_OF_Gas00[];
void Button_CLYQ_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
    

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{

		case 0:
		{//û��ѡ�ж�������
			ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
	}
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					ps->Key.Opt=ps->Key.OldOpt;   //20200605
				    ps->Key.LastOpt=0;
				    ps->RunMode =52; //������ҳ��
				    ps->RunModeStat=1;//���¾�̬����
				
				}
				else if(ps->Button==0x0001)  //����ֵ����
				{
				  //to do ��ӡ�߼�
					ps->Key.Opt=1;
					if(AT_GetData()->Mode==0)
					{
					u8 pn[33];
					u8 pn8[8]={1,1,1,1,1,1,1,1};
					float buffer[10];
					for(int i=0;i<7;i++)
					{
					   if(pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ]&(1<<i))
						 {
						  pn8[i]=1;
						 }else
						 {
						  pn8[i]=0;
						 }
					}
					for(int i=0;i<33;i++)
						pn[i]=1;
					buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
					
					/*
					Slave_TPActualStructure *Qtp=&Slave_GetData()->O2Concent;	
					for(int i=0;i<7;i++)
					{
						if(i<=3) buffer[i+1]=Qtp[i].Value;
						else buffer[i+2]=Qtp[i].Value;
					}
					buffer[5]=Qtp[7].Value;		//����NOX��CO2�����ӡֵ��modify by alfred, 2020.07.04
					*/
					buffer[1]=ps->DisValue[0];
					buffer[2]=ps->DisValue[4];
					buffer[3]=ps->DisValue[1];
					buffer[4]=ps->DisValue[2];
					buffer[5]=ps->DisValue[3];
					buffer[6]=ps->DisValue[5];
					buffer[7]=ps->DisValue[6];
					buffer[8]=ps->DisValue[7];
					for(int i=0;i<8;i++)
					{
					   if(ps->DisValue[i]<0)
					    buffer[i+1] =0;  
					}

					PrintCurTest((float *)&buffer[0],(u8 *)&pn[0],pn8);
				    }
					else
					{
						Screen_ResetDataBuffer();
	                    strcpy((char *)ps->Buffer,"û���Ӵ�ӡ��!");//����ʾ20200519
	                    Screen_RefreshDataMulti(0x2E20,ps->Buffer,15*2);
					}
				}
				else if(ps->Button==0x0002)  //����ֵ����
				{ //to do ��ͣ�߼�
					ps->Key.Opt=2;
					ps->Key.LastOpt=0;
					if(ps->PauseOrStop==B0)
					{
					 ps->PauseOrStop=0;
					 Flux_GetData()->ChannelC.RunMode=2;
					 Flux_StarSampling('C',1.0) ;//������	
					}else
					{
					   ps->PauseOrStop=B0;
					   Flux_GetData()->ChannelC.RunMode=3;	
					   Flux_StopSampling('C');//������		
					}
				}
				else if(ps->Button==0x0003&&(ps->PauseOrStop!=B0))  //��ƽ��
				{  //to do ����ƽ��ֵ�߼�
					ps->JiPingJun.cIsCalcAverage = 1; //��ƽ����־λ=1
					
					ps->JiPingJun.uCounter1ms = 0;//��ʱ������
					ps->JiPingJun.cAverageCount = 0; //ƽ����������������					
					SdFile_GetData()->GasRecordMax = 0;	//��ƽ��ǰ����������������
					
					
	                sprintf((char*) ps->recorddat,"%02d-%02d-%02d %02d:%02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
	                ps->recorddat[16]=0;   //��β�ȼӸ���
					
					sprintf((char*)ps->recorstarddat,"%02d-%02d-%02d %02d:%02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
					ps->recorstarddat[19]=0;  //���濪ʼ��ƽ���Ŀ�ʼʱ��

					u8 buf[5];
					sprintf((char *)&buf[0],"%02d",(pf->Data[FLASHIS_AdrXTSZYQCLSJ]));  //
	                strcat((char *)&ps->recorddat[0],(char *)&buf[0]);
					strcat((char *)&ps->recorddat[0],"9");

                    Flux_GetData()->ChannelC.Sampling.SecTimer=0;   //����ʱ�����¿�ʼ   20210420
					
					if(ps->Key.Opt!=3)                                             //20200513
					ps->Key.Opt=3;
					ps->RunMode = 0x5230;
//					  ps->RunModeStat=1;
					  ps->Function=1; //���ܼ���ƽ��
					  Flux_GetData()->ChannelC.Sampling.JpjFlg=1;                  //20200606
					  ps->PauseOrStop=0;  //����״̬
					  ps->JiPingJun.JpjEndFlg=0;

				}
				else if(ps->Button==0x0004)  //����ֵ����
				{    //to do �����߼�
					ps->Key.Opt=4;
					Flux_GetData()->ChannelC.Sampling.JpjFlg=0; //���ƽ����־
					Flux_GetData()->ChannelC.RunMode=4;
					Flux_StarSampling('C',1.0) ;//������	
					Flux_GetData()->ChannelC.Sampling.DXTime=FLUX_FangDaoXiTimeC;   //180����ϴ
					//Flux_StopSampling('C');//������		
					ps->RunMode=0x5210;//��ʾ�Ƿ����
					ps->Function=0;
				}
				/*
				if((pf->Data[FLASHIS_AdrCLYQ_CLFS]==1)&&(Flux_GetData()->ChannelC.RunMode==4)) //���Զ�ģʽ�ж��Ƿ�����ϴ
					{
						ps->Key.Opt=5;
						// Flux_StopSampling('C');//������		
						//to do �����߼�				

						ps->RunMode=0x5210;//��ʾ�Ƿ����
					}
				*/	
		if(ps->RunMode==0X5210)
		{
		ps->LastTimerSecUp = Flux_GetData()->ChannelC.Sampling.SecTimer;
	
		ps->LastFunction=ps->Function;
		ps->LastWarning=ps->Warning;

		}
	}
	ScreenChangeDataInit(ps);
	if(ps->RunMode==0x5000&&ps->YC_Stat==1) //���̳�����
	{
    ps->RunMode=0x6200;
		Screen_ShowPage(64); 
	}
}
//----
/*
	��������void Button_CLYQ_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ��������-��ʼ����-�����������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
extern unsigned int writeWorkfileEnable;
void Button_CLYQ_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	Slave_DataStructure*  Sp=Slave_GetData();
	//	 SdFile_DataStruct* SdFp=SdFile_GetData();
	//Button_DataStructure *p = Button_GetData();

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		
	  }

	 { //��Ļ��������
		    ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					ScreenBackLastPag(ps,0X5220);
					char pathbuf[202];   //�������ļ�·��
					SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],ps->FileNumber); //���ļ���·���ŵ�·��������
					FS_DeleteFolderOrFile(pathbuf);
					f_unlink("0:/YLB3330/Gas/link.txt");// ɾ��ԭ�е������ļ�, add by alfred, 2020.07.13
					mf_copy((u8*)"0:/YLB3330/Gas/temp0.txt",(u8*)"0:/YLB3330/Gas/link.txt",1);//������ǰ����������ļ����ݸ��Ƶ�link.txt�С�
					Flux_GetData()->ChannelC.Sampling.EndFlg = 0;
					writeWorkfileEnable = 1;
					//Flux_GetData()->ChannelC.RunMode=0;	
			
				}
				else if(ps->Button==0x0001)  //����ֵ����
				{
				  //to do ���������߼�
					/*��������*/
					
					ScreenBackLastPag(ps,0X5220);
					float buffer[40];
					float gas[8];
					gas[0]=Sp->O2Concent.Value;
	                gas[1]=Sp->SO2Concent.Value;
	                gas[2]=Sp->NOConcent.Value;
	                gas[3]=Sp->NO2Concent.Value;
	                gas[4]=Sp->COConcent.Value;
	                gas[5]=Sp->H2SConcent.Value;
	                gas[6]=Sp->CO2Concent.Value;
					SaveGasDataLoad(buffer,gas);
					SaveGasData(buffer,Flux_GetData()->ChannelC.SaveFileNumber,1);
			      
					SdFile_WriteGasFile(&SdFile_GetData()->InterFace_data,Flux_GetData()->ChannelC.SaveFileNumber,0,0);
					Flux_GetData()->ChannelC.Sampling.EndFlg = 0;
				
//					Flux_GetData()->ChannelC.RunMode=0;
				}
				else if(ps->Button==0x0002)  //����ֵ����
				{    
					ScreenBackLastPag(ps,0X5220);//ȡ������
					char pathbuf[202];   //�������ļ�·��
					SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],ps->FileNumber); //���ļ���·���ŵ�·��������
					FS_DeleteFolderOrFile(pathbuf);
					
					f_unlink("0:/YLB3330/Gas/link.txt");// ɾ��ԭ�е������ļ�, add by alfred, 2020.07.06
					mf_copy((u8*)"0:/YLB3330/Gas/temp0.txt",(u8*)"0:/YLB3330/Gas/link.txt",1);//������ǰ����������ļ����ݸ��Ƶ�link.txt�С�
					Flux_GetData()->ChannelC.Sampling.EndFlg = 0;
				
				  //to do ��ͣ�߼�
				}
//					else if(ps->Button==0x0003)  //����ֵ����
//				{
//					// ScreenBackLastPag(ps,0X5200); //��������
//					ps->RunMode=0x5200;
//					FluxData.ChannelC.Sampling.EndFlg=1;//������ɣ��ȴ����֣����뱣�������־
//				}
				ScreenChangeDataInit(ps);
 /* if(ps->RunMode==0X5200)
	{
			ps->TimerSecUp=ps->LastTimerSecUp;
			ps->TimerMinUp=ps->LastTimerMinUp;
			ps->Function=ps->LastFunction;
			ps->Warning=ps->LastWarning;
			ps->RunModeStat=2;
		 //Flux_StarSampling('C',1.0);//������	
     Screen_ShowPage(38); 		
	}
	*/			
	}
}
//----
/*
	��������void Button_CLYQ_YQQX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ��������-��ʼ����-��������-��ϴ���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CLYQ_YQQX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
	  }
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt||(ps->RunModeStat==2&&Flux_GetData()->ChannelC.Sampling.FdxFlg))
				{
					 ScreenBackLastPag(ps,0X5000);
					 Flux_StopSampling('C');//�ر�
					 Flux_GetData()->ChannelC.RunMode=0;
					 Flux_GetData()->ChannelC.Sampling.FdxFlg=0;
				}
		
	ScreenChangeDataInit(ps);
	}
}
/*//2020-05-22
20200516
	��������void Button_CLYQ_YQCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ��������-������ѯ���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25,�޸ĳ�ʱ���� modify by alfred.
*/
void Button_CLYQ_YQCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 2:		
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ps->FileNumber = (float)atoi((char*)&(ps->Key.Buffer[0]));
					if(ps->FileNumber<1) ps->FileNumber=1; //���� ����50Kp
					if(ps->FileNumber>9999999) ps->FileNumber=9999999; //���� С����130Kp
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}		
			}
			else
			{			
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
					ps->FileNumber = (float)atoi((char*)&(ps->Key.Buffer[0]));
					if(ps->FileNumber<1) ps->FileNumber=1; //���� ����50Kp
					if(ps->FileNumber>9999999) ps->FileNumber=9999999; //���� С����130Kp
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,0,2); //��ֵ����
			}				
			break;
		}	
	}
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x5000);	
					SdFile_GetData()->GasRecordMax = 0; //�˳���ѯʱ�����������������
				}
				else if(ps->Button==0x0001)  //����ֵ����
				{
					    //��Ѱ
						
							if(ps->FileNumber>1) ps->FileNumber--; //���� ����50Kp20200528
						  ps->Key.Opt=1;
					    ps->RunModeStat=1;
				
				}
				else if(ps->Button==0x0002)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA1);//�����������
				  //to do ����
			    ScreenTowOptChose(ps,2);	
				}
				else if(ps->Button==0x0003)  //����ֵ����
				{
				  //to do ��Ѯ
						ps->FileNumber++;
							if(ps->FileNumber>SdFile_GetData()->GasLinkFileMax) ps->FileNumber=SdFile_GetData()->GasLinkFileMax; //���� ����50Kp	
					 ps->Key.Opt=3;
					 ps->RunModeStat=1;
				}
				else if(ps->Button==0x0004)  //����ֵ����
				{
				  //to do ��һҳ
					 ps->Function=0;		//��ҳָʾ		 
     ps->RunModeStat=1;
					ps->Key.Opt=4;			
				}
				else if(ps->Button==0x0005)  //����ֵ����
				{
					//to do ��һҳ
					 ps->Function=1;		//��ҳָʾ
				 ps->RunModeStat=1;
					ps->Key.Opt=5;
				}
				else if(ps->Button==0x0006)  //����ֵ����
				{
					//to do ����
					ps->Key.Opt=6;
					ps->RunMode=0x5310;
				}
				else if(ps->Button==0x0007)  //����ֵ����
				{
					//to do �ļ�����
					ps->Key.Opt=7;
					ps->RunMode=0x5320;
				}
				else if(ps->Button==0x0008)  //����ֵ����
				{
					//to do ��ӡѡ��
						ps->RunMode=0x5330;
					ps->Key.Opt=8;
				}
				else if(ps->Button==0x0009)  //����ֵ����
				{
					//to do ��������
					if(SdFile_GetData()->GasRecordMax)                  //20200516
					{
					ps->RunMode=0x5340;
					}
					ps->Key.Opt=9;
				}
				else if(ps->Button==0x000A)  //����ֵ����
				{
					//to do ��ӡ
					ps->Key.Opt=10;
					if(AT_GetData()->Mode==0)
					{
					
					u8 pn[33]={0};
					float buffer[5];
					u32 pns=GetU32dataFormFlash2Word( FLASHIS_AdrCLYQ_WJCX_DYXX);
					for(int i=0;i<32;i++)
					{
					   if(pns&(1<<i)) pn[i]=1;
							
					}
					buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
					buffer[1]=ps->FileNumber;
					buffer[2]=ps->FileNumber;
					buffer[3]=ps->FileNumber;
					PrintGasFile((u8 *)&pn[0],(float *)&buffer[0]);
				    }
					else
					{
						Screen_ResetDataBuffer();
	                    strcpy((char *)ps->Buffer,"û���Ӵ�ӡ��!");//����ʾ20200519
	                    Screen_RefreshDataMulti(0x2E20,ps->Buffer,15*2);
					}
				}
	ScreenChangeDataInit(ps);
	} 
}
/*20200518
	��������void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ��������-������ѯ-�������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25,�޸ĳ�ʱ���� modify by alfred.
*/
void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//��Χ��������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:
		case 2:	
		case 3:
		{		
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
					ps->StarCun=0; 
				
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					FourBytes four;
					if(ps->Key.Mode==1)  //����ϵ��
					{					
						four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));  //�������,������
						if(four.FloatData<=1)                                                 //20200526
                        four.FloatData=1; //���� С����130Kp
						else if(four.FloatData>99999)
						 four.FloatData=99999.00f;	
						ps->ZSXS=four.FloatData;   //�ݴ�����ϵ��
							
						//������
						four.FloatData=21.0f/four.FloatData;
						four.FloatData=21-four.FloatData;
						ps->JZHYL=four.FloatData;  //�ݴ��׼������						
					}
					else if(ps->Key.Mode==2)//��׼������
					{
						four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));  //�������,������
						if(four.FloatData>=20) four.FloatData=20; //����
                    						
						ps->JZHYL=four.FloatData;   //�ݴ�����ϵ��	//20200526					
						//������ϵ��
						four.FloatData=21.0f-four.FloatData;
						four.FloatData=21.0f/four.FloatData;
					
						ps->ZSXS=four.FloatData;   //�ݴ�����ϵ��
						
					}
					else if(ps->Key.Mode==3)   //����ϵ��	
					{
						four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));  //�������,������
						if(four.FloatData<0.01f) four.FloatData=0.01f; //���� ����50Kp
						if(four.FloatData>10.0f) four.FloatData=10.0f; //���� С����130Kp
						ps->FHXS=four.FloatData;   //�ݴ渺��ϵ��							
					}
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}			
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
					FourBytes four;
					if(ps->Key.Mode==1) //����ϵ��
					{					
						four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));  //�������,������
						if(four.FloatData<1)                                                 //20200526
                        four.FloatData=1; //���� С����130Kp
						else if(four.FloatData>99999)
						four.FloatData=99999.00f;	
						ps->ZSXS=four.FloatData;   //�ݴ�����ϵ��
							
						//���׼������
						four.FloatData=21.0f/four.FloatData;
						four.FloatData=21-four.FloatData;
						ps->JZHYL=four.FloatData;  //�ݴ��׼������						
					}
					else if(ps->Key.Mode==2)//��׼������
					{
						four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));  //�������,������
						if(four.FloatData>=20) four.FloatData=20; //���� 
						ps->JZHYL=four.FloatData;   //�ݴ�����ϵ��	//20200526					
						//������ϵ��
						four.FloatData=21.0f-four.FloatData;
						four.FloatData=21.0f/four.FloatData;
					
						ps->ZSXS=four.FloatData;   //�ݴ�����ϵ��
						
					}
					else if(ps->Key.Mode==3)//����ϵ��	
						{
							four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));  //�������,������
							if(four.FloatData<0.01f) four.FloatData=0.01f; //���� ����50Kp
							if(four.FloatData>10.0f) four.FloatData=10.0f; //���� С����130Kp
							ps->FHXS=four.FloatData;   //�ݴ渺��ϵ��								
						}
					} 
					ScreenBackFirstOpt(ps);	       //�˳����
					ScreenEnterNumberText(ps,2,6); //��ֵ����
				}
			break;
		}
	}
		
	 { //��Ļ�������� +��Χ�������մ���
		        ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x5300);
						
				}
				else if(ps->Button==0x0001||ps->Button==0x0002||ps->Button==0x0003)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0 + ps->Button);//�����������					
					ScreenTowOptChose(ps,ps->Button);	
				
				}
				/* �������Σ���Ҫ���ڵ���������������
				else if(ps->Button==0x0004)  //����ֵ����
				{
				  //to do ��������
					ps->RunMode=0x5311;
					ps->Key.Opt=4;
				}
				*/
	ScreenChangeDataInit(ps);
	 ps->StarCun=0; 			
	} 
}
//---
/* 20200518,2020-05-22
	��������void Button_CLYQ_YQCX_JS_YQSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	������	�ú���Ϊ��������-������ѯ-����-�����������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25,�޸ĳ�ʱ����, modify by alfred.
*/
void Button_CLYQ_YQCX_JS_YQSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	//Button_DataStructure *p = Button_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		default :
		{
			if(ps->Key.Mode<=7)
			{
				if(ps->Key.bOutsideKeyboard != true)
				{
					ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);	//�����ⲿ���̲�������С���������, modify by alfred,2020.07.07 
					if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
					{     
						ScreenBackLastOpt(ps);
					}
					if(ps->sRcvType !=0)
					{
						Screen_ResetKeyDataBuffer();//�������ֵ
						strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
						if(ps->Key.Mode==1)  //O2
						ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,30,0.0);
						if(ps->Key.Mode==2)  //SO2
						ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,1400,0.0);
						if(ps->Key.Mode==3)  //NO
						ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,6700,0.0);
						if(ps->Key.Mode==4)  //NO2
						ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,2000,0.0);
						if(ps->Key.Mode==5)  //CO
						ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,2500,0.0);
						if(ps->Key.Mode==6)  //H2S
						ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,2000,0.0);
						if(ps->Key.Mode==7)  //CO2
						ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,30,0.0);
						
						ScreenBackLastOpt(ps);
						ps->sRcvType = 0;
					}
					else if(ps->Button==0x0Aff)
					{
						ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
						ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
					}
				}
				else
				{             							 
					ScreenTowOpButton(ps,1,0,0);  
					if(ps->Button==0x0Aff) //ȷ��
					{
                        if(ps->Key.Mode==1)  //O2
						ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,30,0.0);
						if(ps->Key.Mode==2)  //SO2
						ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,1400,0.0);
						if(ps->Key.Mode==3)  //NO
						ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,6700,0.0);
						if(ps->Key.Mode==4)  //NO2
						ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,2000,0.0);
						if(ps->Key.Mode==5)  //CO
						ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,2500,0.0);
						if(ps->Key.Mode==6)  //H2S
						ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,2000,0.0);
						if(ps->Key.Mode==7)  //CO2
						ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,30,0.0);
					} 
					ScreenBackFirstOpt(ps);	       //�˳����
					ScreenEnterNumberText(ps,2,6); //��ֵ����
				}
			}
			break;
		}		
	}
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x5310);
			
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x0007)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//�����������					
					ScreenTowOptChose(ps,ps->Button);	
				
				}
				else if(ps->Button==0x0008)  //����ֵ����
				{
				  //to do ��������
					/*��������*/
			//		float buffer[40];
					
					SaveGasAverageRecordData((float * )&ps->DisValue[0],ps->FileNumber);   //�������ŵ�������20200517
					SaveGas_Z_J_F_RecordData(); //20200518
					SdFile_WriteGasFile_Z_J_F_Record(&SdFile_GetData()->InterFace_data,ps->FileNumber);//��������ϵ������׼������������ϵ��������Ũ�ȵ�SD��
					//SdFile_WriteGasFileAverageRecord(&SdFile_GetData()->InterFace_data,ps->FileNumber);
	
					ps->Function=1;
					ps->Key.Opt=8;
				}
				else if(ps->Button==0x0009)  //����ֵ����
				{
				  //to do ���Ϊ����
					/*��������*/
					float buffer[40];
					SdFile_Serch_MaxofGasLinkFile();//����Ѱ����ļ���
					SdFile_GetData()->GasLinkFileMax++;
					SaveGasINputDataLoad(buffer);
					SaveGasData(buffer,SdFile_GetData()->GasLinkFileMax,1);
					
					//SdFile_Serch_MaxofGasRecord(ps->FileNumber+1); //��ȡ���ļ�¼��
					SdFile_WriteGasFile(&SdFile_GetData()->InterFace_data,SdFile_GetData()->GasLinkFileMax,0,1);  //20200518
					ps->Key.Opt=9;
					ps->Function=2;
				}
				
	ScreenChangeDataInit(ps);
	} 
}
//---
/*
	��������void Button_CLYQ_YQCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	������	�ú���Ϊ��������-������ѯ-�ļ�����-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25,�޸ĳ�ʱ����, modify by alfred.
*/
void Button_CLYQ_YQCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:
		case 2:
		{			
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);	//��Ļ���������ʱ֧���ⲿ�������룬add by alfred,2020.07.07 
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,SdFile_GetData()->GasLinkFileMax,1); //���Ƶ���������ļ��Ų����������ļ������ֵ����Ȼ�ᵼ������, modify by alfred, 2020.07.08
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}	
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,SdFile_GetData()->GasLinkFileMax,1);
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,0,7); //��ֵ����
			}
			break;
		}		
	}
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x5300);			
				}
				else if(ps->Button==0x0001||ps->Button==0x0002)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//�����������						
					ScreenTowOptChose(ps,ps->Button);	
				
				}
				else if(ps->Button==0x0003)  //����ֵ����
				{
				  //to do ��������
					char uMachineNumBuffer[10], uSysDateBuffer[10];
					FourBytes four;
					four.U16Data[0]=pf->Data[FLASHIS_AdrCJWHYQBH];
					four.U16Data[1]=pf->Data[FLASHIS_AdrCJWHYQBH+1];
					
					sprintf((char*)uMachineNumBuffer,"%08d",four.U32Data);
					sprintf((char*)uSysDateBuffer,"%02d%02d%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday);					
					Usb_OutGasFileInf(DisBuffer[0],DisBuffer[1],uSysDateBuffer,uMachineNumBuffer);
					ps->Key.Opt=3;
				}
				else if(ps->Button==0x0004)  //����ֵ����
				{
					//to do ��ӡ
					ps->Key.Opt=10;
					
					if(AT_GetData()->Mode==0)
					{
					u8 pn[33]={0};
					float buffer[5];
					u32 pns=GetU32dataFormFlash2Word( FLASHIS_AdrCLYQ_WJCX_DYXX);
					for(int i=0;i<32;i++)
					{
						if(pns&(1<<i)) pn[i]=1;
					}
					buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
					buffer[1]= DisBuffer[0];	//��ʼ�ļ���
					buffer[2]= DisBuffer[1];	//�����ļ���
					buffer[3]= DisBuffer[0];	//�������������ļ���
					PrintGasFile((u8 *)&pn[0],(float *)&buffer[0]);
				   }
				    else
					{
						Screen_ResetDataBuffer();
	                    strcpy((char *)ps->Buffer,"û���Ӵ�ӡ��!");//����ʾ20200519
	                    Screen_RefreshDataMulti(0x3B20,ps->Buffer,15*2);
					}
					
					ps->Key.Opt=4;
				}
	ScreenChangeDataInit(ps);
	} 
}
//---
/*
	��������void Button_CLYQ_YQCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	������	�ú���Ϊ��������-������ѯ-��ӡѡ��-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CLYQ_YQCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}

	  }
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x5300);
			         pf->Flag=1;    //20200523
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x20)  //����ֵ����
				{
					ps->Key.Opt= ps->Button;
					u32 data=pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX]+((pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX+1])<<16);;
					u8 point=ps->Key.Opt-1;
					if(point>15)
					{      data=data>>16;
						     point-=16;
					if((data>>point)&0x00000001)
					{
					  pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX+1]&=~(1<<point);//λ��0
					}else
					{
					 pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX+1]|=(1<<point);//λ��1
					}
					}else
					{
						if((data>>point)&0x00000001)
						{
							pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX]&=~(1<<point);//λ��0
						}else
						{
							pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX]|=(1<<point);//λ��1
						}
					}				
				}
	ScreenChangeDataInit(ps);
	} 
}
//---
/*
20200515
	��������void Button_CLYQ_YQCX_FZSJ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ��������-������ѯ-�����������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CLYQ_YQCX_FZSJ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
    SdFile_DataStruct* SDp=SdFile_GetData();

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{	
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
	 }
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x5300);
										
				}
				else if(ps->Button==0x0001)  //����ֵ����
				{ //�������
					
					
					/*
				   if(ps->UseGasCal&(1<<(ps->RecordCun-1)))
					 {
					 ps->UseGasCal&=~(1<<(ps->RecordCun-1));
					 }else
					 {
					  ps->UseGasCal|=(1<<(ps->RecordCun-1));
					 }
					 */
					if(ps->UseGasCal)
					{
						ps->UseGasCal=0;
						SdFile_WriteGasFiletxt(ps->FileNumber,ps->RecordCun,'0');
					}
					else
					{
						ps->UseGasCal=1;
						SdFile_WriteGasFiletxt(ps->FileNumber,ps->RecordCun,'9');
					}
					 
					ps->Key.Opt=1;
				
				}
				else if(ps->Button==0x0002)  //����ֵ����
				{
				  //to do ��һ��������
					if(ps->RecordCun>1) ps->RecordCun--; 
			  	 ps->Key.Opt=2;
					ps->RunModeStat=1;

				}
				else if(ps->Button==0x0003)  //����ֵ����
				{
				  //to do ��һ��������
					if((SDp->GasRecordMax)>(ps->RecordCun)) ps->RecordCun++;
			  	 ps->Key.Opt=3;
					ps->RunModeStat=1;
					 
				}
				else if(ps->Button==0x0004)  //����ֵ����
				{
				  //to do ��ӡ��ǰ
					
					ps->Key.Opt=4;			
						//to do ��ӡ
					ps->Key.Opt=10;
					if(AT_GetData()->Mode==0)
					{
					u8 pn[33]={0};
					float buffer[5];
					u32 pns=GetU32dataFormFlash2Word( FLASHIS_AdrCLYQ_WJCX_DYXX);
					for(int i=0;i<32;i++)
					{
					   if(pns&(1<<i)) pn[i]=1;
							
					}
					Printer_GetData()->PrintSelect=0; //��ӡ��ǰ
					Printer_GetData()->record_Cnt =0;
					buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
					buffer[1]=ps->RecordCun;
					buffer[2]=ps->RecordCun;
					buffer[3]=ps->FileNumber;
			        PrintGasRecordFile((u8 *)&pn[0],(float *)&buffer[0]);
				    }
					else
					{
						Screen_ResetDataBuffer();
	                    strcpy((char *)ps->Buffer,"û���Ӵ�ӡ��!");//����ʾ20200519
	                    Screen_RefreshDataMulti(0x3B20,ps->Buffer,15*2);
					}
					
				}
				else if(ps->Button==0x0005)  //����ֵ����
				{
					//to do ��ӡѡ��
					ps->Key.Opt=5;
					ps->Key.Opt=4;			
						//to do ��ӡ
					ps->Key.Opt=10;
					if(AT_GetData()->Mode==0)
					{
					u8 pn[33]={0};
					float buffer[5];
					u32 pns=GetU32dataFormFlash2Word( FLASHIS_AdrCLYQ_WJCX_DYXX);
					for(int i=0;i<32;i++)
					{
					   if(pns&(1<<i)) pn[i]=1;
							
					}
					Printer_GetData()->PrintSelect=ps->UseGasCal; //��ӡ��ǰ
					if(ps->UseGasCal==0)
					{	
						Printer_GetData()->record_Cnt =64;
					}
					else
					{
						Printer_GetData()->record_Cnt =0;
					}
					
					buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
					buffer[1]=ps->RecordCun;
					buffer[2]=ps->RecordCun;
					buffer[3]=ps->FileNumber;
					PrintGasRecordFile((u8 *)&pn[0],(float *)&buffer[0]);
				   }
					else
					{
						Screen_ResetDataBuffer();
	                    strcpy((char *)ps->Buffer,"û���Ӵ�ӡ��!");//����ʾ20200519
	                    Screen_RefreshDataMulti(0x3B20,ps->Buffer,15*2);
					}
					
				}
				else if(ps->Button==0x0006)  //����ֵ����
				{
					//to do ����ƽ��ֵ
					ps->Key.Opt=6;
				  u8 SumCun=0;//����ƽ������
					float SumData[8]={0};//����
					
					for(int i=0;i<SDp->GasRecordMax;i++)                       //20200515
					{
	
							GetGasFileRecodData(ps->FileNumber,i+1,ps->DisValue,1);
							if(ps->DisValue[8]==1)   //�������
							{
								SumCun++;
								for(int j=0;j<8;j++)
								{							
									SumData[j]+=ps->DisValue[j];
								}							
							}
	
					}	
					  for(int j=0;j<8;j++)
					  ps->DisValue[j]=SumData[j]/SumCun;
					  for(int j=4;j<7;j++)
					  ps->DisValue[j]=ps->DisValue[j+1]; //�޳�NOx
					
					if(SumCun>0) //���������    //����ƽ������
					{

						SaveGasAverageRecordData((float * )&ps->DisValue[0],ps->FileNumber);   //�������ŵ�������20200515
						SdFile_WriteGasFileAverageRecord(&SDp->InterFace_data,ps->FileNumber);
					  ScreenBackLastPag(ps,0x5300);
					}
						
				}
	ScreenChangeDataInit(ps);
	ps->StarCun=0; 
					
	} 
}
//----2020-05-21
/*
	��������void Button_YCCY_CYSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ�̳�����-�����������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25,�޸ĳ�ʱ����, modify by alfred.
*/
void Button_YCCY_CYSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
   Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYCCJ_CYSE_CYLS,2,110.0,5,CALC_HAVE_POINT,CALC_NO_SIGNS);
			}
			else
			{							
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrYCCJ_CYSE_CYLS,2,110.0,5);    //test718
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,2,5); //��ֵ����
			}
			break;
		}
		case 3:  //��Ͳ
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYCCJ_CYSE_LTH,3,9999999,1,CALC_NO_POINT,CALC_NO_SIGNS);
			}
			else
			{					
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrYCCJ_CYSE_LTH,3,9999999,1);
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,0,5); //��ֵ����
			}
			break;
		}
		case 4:		//�����ʱ
		case 5:
		{						
			if(ps->Key.bOutsideKeyboard != true) //��Ļ
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					u16 Datai=atoi((char*)&(ps->Key.Buffer[0]));
					if(Datai<=0) Datai=0; //���� ����50Kp
					if(Datai>99) Datai=99; //���� С����130Kp
					if(ps->Key.Mode==4)
					{
						pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]=Datai*60; //����ֵ
					}else
					{
						if(Datai<=0) Datai=0; //���� ����50Kp
						if(Datai>59) Datai=59; //���� С����130Kp
						pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]=pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]/60*60+Datai; //����ֵ
					}
					pf->Flag=1;
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}	
			}
			else
			{							
					ScreenTowOpButton(ps,1,0,0);  
					if(ps->Button==0x0Aff) //ȷ��
						{
							u16 Datai=atoi((char*)&(ps->Key.Buffer[0]));
							if(Datai<=0) Datai=0; //
							if(Datai>99) Datai=99; //
							if(ps->Key.Mode==4)
							{
								pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]=Datai*60; //����ֵ
								ps->Key.Mode=5;
								ps->Key.Buffer[0]=' ';
								ps->Key.Buffer[1]=' ';
								ps->Key.TwoOpt=0;
								ps->Button=0;
								ps->RunModeStat=1;
							}
							else //key.Mode==5;
							{
								if(Datai<=0) Datai=0; //
								if(Datai>59) Datai=59; //
								pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]=pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]/60*60+Datai; //����ֵ
								
							}
							pf->Flag=1;
						} 
					ScreenBackFirstOpt(ps);	       //�˳����
					ScreenEnterNumberText(ps,0,2); //��ֵ����						
			}
			break;
		}
		case 6: //����ֱ��
		{				
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYCCJ_CYSE_CZZJ,2,20.0,4,CALC_HAVE_POINT,CALC_NO_SIGNS);
			}
			else
			{						
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrYCCJ_CYSE_CZZJ,2,20.0,4);
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,2,5); //��ֵ����
			}
		 break;
		}			
		case 7:	//�ܲɵ�
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBD_CYDS,1,50,1,CALC_NO_POINT,CALC_NO_SIGNS);
			}	
			else
			{								
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrYDBD_CYDS,1,50,1);
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,0,5); //��ֵ����
			}
			break;
		}		
	}
		
	 { //��Ļ��������
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==0x0a)
				{
					 ScreenBackLastPag(ps,52);
			
				}
				else if(ps->Button==0x0001)  //����ֵ����
				{
         			
							pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]=0;
							if(ps->Key.bOutsideKeyboard != true)
							Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//����������� 
							ScreenTowOptChose(ps,ps->Button);	
				            ps->Key.Opt=1;
				}
			  else if(ps->Button==0x0002)  //����ֵ����
				{
					     pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]=1;
					     ps->RunModeStat=1;
						 ps->Key.Opt=2;
				
				}
				else if(ps->Button==0x0003)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA2);//����������� 					
				    ScreenTowOptChose(ps,3);
			  	    ps->Key.Opt=3;
				}
				else if(ps->Button==0x0004)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA3);//����������� 					
				    ScreenTowOptChose(ps,4);
			  	    ps->Key.Opt=4;
					 
				}
				else if(ps->Button==0x0005)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA4);//����������� 					
				    ScreenTowOptChose(ps,5);
			  	    ps->Key.Opt=4;
				}
				else if(ps->Button==0x0006)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA5);//����������� 					
				    ScreenTowOptChose(ps,6);
					ps->Key.Opt=5;			
				}
				else if(ps->Button==0x0007)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
					 Screen_WriteRegisterOneData(0x4f,0xA6);//����������� 					
					 ScreenTowOptChose(ps,7);
					
					  ps->Key.Opt=6;
				}
				else if(ps->Button==0x0008)  //����ֵ����
				{
					
					ps->RunMode=0x6100; //�����Լ��
					ps->Key.Opt=1;
				    ps->Key.State=0;
				}
				else if(ps->Button==0x0009)  //����ֵ����
				{

					ps->RunMode=0x6200; //��ʼ����
					Flux_ClearMidBuff(&Flux_GetData()->ChannelB.FluxTowData.Filter,Flux_GetData()->ChannelB.ShowFlux); //��ջ�����
					ps->Function=0;
					ps->Key.Opt=1;			

				}
	ScreenChangeDataInit(ps);
	} 
}
//----
/*
	��������void Button_YCCY_QMJC_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ�̳�����-�����Լ����水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_YCCY_QMJC_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
	}
	 { //��Ļ��������
		    ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x6000);	Flux_StopSampling('B');
				}
				else if(ps->Button==(0x0001))  //����ֵ����
				{
					ps->Key.Opt=1;
					if(ps->PauseOrStop==0)  //=0 ֹͣ����
					{
					  ps->PauseOrStop=B0;//��ʼ����
					  ps->TimerSecUp=0;
					  Flux_StarSampling('B',15.0);//100L/min
					}else
					{
					 ps->PauseOrStop=0;
					 Flux_StopSampling('B');
					 ps->TimerSecUp=0;
					}
					ps->Key.LastOpt=0;
				  //to do ȷ��
				}
	ScreenChangeDataInit(ps);
	} 
}
//----
/*
	��������void Button_YCCY_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ��������-��ʼ�������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_YCCY_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
   Flux_DataStructure  *pfd =Flux_GetData();

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
	  }
		
	 { //��Ļ��������
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					
				    ps->Key.Opt=ps->Key.OldOpt;   //20200605
				    ps->Key.LastOpt=0;
				    ps->RunMode =52; //������ҳ��
				    ps->RunModeStat=1;//���¾�̬����
			    
				}
				else if(ps->Button==0x0001)  //����ֵ����
				{
				  //to do ��������
					if(Flux_GetData()->ChannelC.RunMode==0)
					{
					ps->RunMode=0x5000;
					ps->Key.Opt=1;
					}
				}
				else if(ps->Button==0x0002)  //����ֵ����
				{	
				//	TwoBytes two;
					ps->Key.Opt=2;
					  //����
					 
				    ps->RunMode=0x6210; //ȥ����

				  //pfd->ChannelB.Sampling.YiciFlg=0;
				 // pfd->ChannelB.Sampling.Cnt=1;
				 // pfd->ChannelB.Sampling.EndFlg=1;

					//�øñ�־��ʾ��ͣ
				    //to do ��ͣ�߼�
				}
				else if(ps->Button==0x0003)  //����ֵ����
				{
				  //to do ��λ�߼�
						ps->Key.Opt=3;
					if(ps->YC_Pause==B0)
					{
						 //��ͣ
					 ps->YC_Pause=0;
					  if(pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]==1) //����
				      { Flux_ClearMidBuff(&Flux_GetData()->ChannelB.FluxTowData.Filter,Flux_GetData()->ChannelB.ShowFlux);
					    Flux_StarSampling('B',CalculationFormula_Qrs(GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ),GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_YQLS_YQLS),GetFlaotdataFormFlash2Word(FLASHIS_AdrCHSL_HSL)/100)); //��A��SU
						Flux_GetData()->ChannelB.RunMode=2;  //�����߳�
				      }else
				      { Flux_ClearMidBuff(&Flux_GetData()->ChannelB.FluxTowData.Filter,Flux_GetData()->ChannelB.ShowFlux);
					    Flux_StarSampling('B',GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CYLS)); //Ŀ������
						Flux_GetData()->ChannelB.RunMode=2;  //�����߳�
				      }  		
					}else
					{
					   ps->YC_Pause=B0;

					   FluxData.ChannelB.Sampling.DXTime=10;   //��ͣ�����У�10�������
				       Flux_ContinueSampling('B',20);//������  20L/min  10s
					   Flux_GetData()->ChannelB.RunMode=3;  //��ͣ

					}
					ps->Key.LastOpt=0;	
				}

			else if(Flux_GetData()->ChannelB.Sampling.EndFlg) //��������
			{
					 ps->RunMode=0x6210; //ȥ����
					 Flux_GetData()->ChannelB.Sampling.EndFlg=0;
			}  
					
	ScreenChangeDataInit(ps);
	}
}
//----
/*
	��������void Button_YCCY_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ�̳��ɼ�-��ʼ����-�����������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_YCCY_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
   TwoBytes two;
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
	  }	
	 { //��Ļ��������
		    ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					ps->Key.Opt=6;   //20200605
				    ps->Key.LastOpt=0;
				    ps->RunMode =52; //������ҳ��
				    ps->RunModeStat=1;//���¾�̬����
				}
				else if(ps->Button==0x0001)  //����ֵ����
				{
				  //to do ���������߼�
						//�ɼ���� ===��������
					  	/*��������*/
						float buffer[40];
						SaveDustDataLoad(buffer);
						SaveDustData(buffer,ps->FileNumber);
						SdFile_WriteDustFile(&SdFile_GetData()->InterFace_data,ps->FileNumber);
					    Flux_GetData()->ChannelB.Sampling.FdxFlg=0; //�������
					
					//������������ѯҳ    
					 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&0x02)&&((Flux_GetData()->ChannelB.RunMode==2)||((Flux_GetData()->ChannelB.RunMode==3)&&(Flux_GetData()->ChannelB.Sampling.DXTime!=0)))) //������ѡ��
                     {		
                         //������ҳ
				           ps->Key.Opt=1;   //
				           ps->Key.LastOpt=0;
				           ps->RunMode =0x6220; //������ҳ
				           ps->RunModeStat=1;//���¾�̬����
						   Flux_GetData()->ChannelB.Sampling.DXTime=FLUX_FangDaoXiTimeB;   //60��ŵ���
				           Flux_StarSampling('B',20);//������  20L/min  10s
						   Flux_GetData()->ChannelB.RunMode=4;
				    }
					 else
					 {
						Flux_GetData()->ChannelB.RunMode=0; 
						Flux_StopSampling('B');
						ps->Key.Opt=1;   //
				        ps->Key.LastOpt=0;
				        ps->RunMode =0x7000; //ȥ��ѯ
						ps->RunModeStat=1;//���¾�̬����
					 }
					 
					two.U8Data[0] = Flux_GetData()->ChannelB.RunMode;
		            two.U8Data[1] = 0;
		            pf->Data[FLASHIS_AdrCJMOD] 	= two.U16Data; 			    //����ģʽ+״̬��־λ  
					pf->Flag = 1; 
             		return;
				}
				else if(ps->Button==0x0002)  //����ֵ����
				{   	/*�����汣������*/

					
				    Flux_GetData()->ChannelB.Sampling.FdxFlg=0;  //���������
				    ps->Key.Opt=6;   //20200605
			        ps->Key.LastOpt=0;
				    ps->RunMode =52; //������ҳ��
				    ps->RunModeStat=1;//���¾�̬����
					pf->Flag = 1;
					Flux_StopSampling('B');  //ֹͣ����
					Flux_GetData()->ChannelB.RunMode=0;
					two.U8Data[0] = Flux_GetData()->ChannelB.RunMode;
		            two.U8Data[1] = 0;
		            pf->Data[FLASHIS_AdrCJMOD] 	= two.U16Data; 			    //����ģʽ+״̬��־λ 
					 
				  //to do
				}

	              ScreenChangeDataInit(ps);
				  if(ps->RunMode==0x6200)
					{	
					 ps->RunModeStat=2;//���¾�̬����	
                     Screen_ShowPage(64); 
					}
	}
}
//----
/*
	��������void Button_YCCY_YCFDX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ��������-��ʼ����-��������-�̳�������-�ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_YCCY_YCFDX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
	  }
		
	 { //��Ļ��������
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt||(ps->RunModeStat==2&&Flux_GetData()->ChannelB.Sampling.DXTime==0))
				{   TwoBytes two;
				    Flux_GetData()->ChannelB.Sampling.FdxFlg=0;
				    Flux_GetData()->ChannelB.RunMode=0;
				    ps->Key.Opt=1;   //20200605
				    ps->Key.LastOpt=0;
				    ps->RunMode =0x7000; //ת����ѯҳ
				    ps->RunModeStat=1;//���¾�̬����
			        Flux_StopSampling('B');
					two.U8Data[0] =Flux_GetData()->ChannelB.RunMode;
		            two.U8Data[1] = 0;
		            pf->Data[FLASHIS_AdrCJMOD] 	= two.U16Data; 			    //����ģʽ+״̬��־λ
					pf->Flag=1;
				}
		   
	ScreenChangeDataInit(ps);

	}
}
//---
/*
	��������void Button_YCCY_YCCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ��������-�̳���ѯ���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25,�޸ĳ�ʱ����, modify by alfred.
*/
void Button_YCCY_YCCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 2:
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ps->FileNumber = (float)atoi((char*)&(ps->Key.Buffer[0]));
					if(ps->FileNumber<1) ps->FileNumber=1; //���� ����50Kp
					if(ps->FileNumber>=SdFile_GetData()->DustLinkFileMax) ps->FileNumber=SdFile_GetData()->DustLinkFileMax; //���� С����130Kp
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}
			}
			else
			{  			
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
						ps->FileNumber = (float)atoi((char*)&(ps->Key.Buffer[0]));
						if(ps->FileNumber<1) ps->FileNumber=1; //���� ����50Kp
						if(ps->FileNumber>=SdFile_GetData()->DustLinkFileMax) ps->FileNumber=SdFile_GetData()->DustLinkFileMax; //���� С����130Kp
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,0,8); //��ֵ����
			}
			break;
		}		
	}
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,51);
		
				}
				else if(ps->Button==0x0001)  //����ֵ����
				{
					    //��Ѱ
							
						if(ps->FileNumber>1) ps->FileNumber--; //���� ����50Kp
						ps->Key.Opt=1;
					    ps->RunModeStat=1;
					    ps->Key.Mode=0;
					 ps->Function=0;  
				
				}
				else if(ps->Button==0x0002)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA1);//�����������
					//to do ����
					ScreenTowOptChose(ps,2);	
				}
				else if(ps->Button==0x0003)  //����ֵ����
				{
				  //to do ��Ѯ
						ps->FileNumber++;
							if(ps->FileNumber>SdFile_GetData()->DustLinkFileMax) ps->FileNumber=SdFile_GetData()->DustLinkFileMax; //���� ����50Kp	
					 ps->Key.Opt=3;
					 ps->RunModeStat=1;
					 ps->Key.Mode=0;
					 ps->Function=0;  
				}
				else if(ps->Button==0x0004)  //����ֵ����
				{
				  //to do ��һҳ
					ps->Key.Opt=4;
		      ps->Function=0;		//������ָʾ	 
          ps->RunModeStat=1;					
          ps->Key.Mode=0;					
				}
				else if(ps->Button==0x0005)  //����ֵ����
				{
					//to do ��һҳ
					ps->Key.Opt=5;
					 ps->Function=1;		//������ָʾ	 
     ps->RunModeStat=1;	
					ps->Key.Mode=0;
				}
				else if(ps->Button==0x0006)  //����ֵ����
				{
					//to do ����
					ps->Key.Opt=6;
					ps->Key.Mode=0;
					if(ps->YC_Type==0) //�̳�
					ps->RunMode=0x7100;  
					else
					{
					 	ps->RunMode=0x7400; //����
					}
				}
				else if(ps->Button==0x0007)  //����ֵ����
				{
					//to do �ļ�����
					ps->Key.Opt=7;
					ps->Key.Mode=0;
					ps->RunMode=0x7200;
				}
				else if(ps->Button==0x0008)  //����ֵ����
				{
					//to do ��ӡѡ��
					ps->RunMode=0x7300;
					ps->Key.Mode=0;
					ps->Key.Opt=8;
				}
				else if(ps->Button==0x0009)  //����ֵ����
				{
					//to do ��ӡ
				//	ps->RunMode=0x7400;
					ps->Key.Mode=0;
					ps->Key.Opt=9;
					if(AT_GetData()->Mode==0)     //��͸��ģʽ
					{
					u8 pn[33]={0};
					float buffer[5];
					pn[0]=	pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX];
					u32 pns=GetU32dataFormFlash2Word( FLASHIS_AdrYCCJ_YCWJCX_DYXX+1);
					for(int i=0;i<32;i++)
					{
					   if(pns&(1<<i)) pn[i+1]=1;
							
					}
					buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
					buffer[1]=ps->FileNumber;
					buffer[2]=ps->FileNumber;
					buffer[3]=ps->FileNumber;
			        PrintDustFile((u8 *)&pn[0],(float *)&buffer[0]);
				    }
					else
					{
						Screen_ResetDataBuffer();
	                    strcpy((char *)ps->Buffer,"û���Ӵ�ӡ��!");//����ʾ20200519
	                    Screen_RefreshDataMulti(0x2E20,ps->Buffer,15*2);
					}
					
				}
	ScreenChangeDataInit(ps);
				
	} 
}
//----
/*20200521, 2020-05-23
	��������void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ��������-������ѯ-�������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25,�޸ĳ�ʱ����, modify by alfred.
*/
void Button_YCCY_YCCX_JS_YC_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	FourBytes four;
	//Button_DataStructure *p = Button_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:
		case 2:	
		case 3:	
		case 4:
		case 5:
		{			
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
		
					if(ps->Key.Mode==1) //�̳�����[19]
					{  
					ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,1000.00,0.0);
					four.FloatData=DisBuff[ps->Key.Mode-1];
					ps->DisValue[19]=ps->YCJZ=four.FloatData;
					}
					else if(ps->Key.Mode==2)  //������[20]   20200526
					{
						ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,30,0.0);
					    four.FloatData=DisBuff[ps->Key.Mode-1];
						ps->DisValue[20]=ps->HYL=four.FloatData;
						if(ps->DisValue[20]!=21)
						{
							ps->DisValue[38]=21.0f/(21.0f-ps->DisValue[20]);
						}
						else
						{
							ps->DisValue[38]=1000000;  //��ʣϵ�� ,����
							ps->DisValue[20]=ps->HYL=21; //������
						}										
					}
					else if(ps->Key.Mode==3)   //����ϵ��[10]
					{   ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,2.0,0.2);
					    four.FloatData=DisBuff[ps->Key.Mode-1];
						ps->DisValue[10]=ps->FHXS=four.FloatData;
					}								
					else if(ps->Key.Mode==4)  //����ϵ��[9]  20200526
					{ 
						ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,99999.0,1.0);
					    four.FloatData=DisBuff[ps->Key.Mode-1];
						ps->DisValue[9]=ps->ZSXS=four.FloatData;
						if(ps->DisValue[9]!=0)
						{
							ps->DisValue[35]=ps->JZHYL=21.0f-(21.0f/ps->ZSXS);
						}
						else
						{
							ps->DisValue[9]=ps->ZSXS=1;
							ps->DisValue[35]=ps->JZHYL=0;
							
						}																				
					}
					else if(ps->Key.Mode==5)  //��׼������[35]  20200526
					{   ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,20.0,0.0);
					    four.FloatData=DisBuff[ps->Key.Mode-1];
						ps->DisValue[35]=ps->JZHYL=four.FloatData;
						if(ps->JZHYL!=21)
							ps->DisValue[9]=ps->ZSXS=21.0f/(21.0f-ps->JZHYL);
						else
						{   ps->DisValue[35]=ps->JZHYL=20;
							ps->DisValue[9]=ps->ZSXS=21;	
							
						}							
					}
					else if(ps->Key.Mode==6) //�����ļ���
					{
						ps->DRWJH=four.FloatData;
					}
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}
			}
			else
			{   			
				ScreenTowOpButton(ps,1,0,0);  
						if(ps->Button==0x0Aff) //ȷ��
						{
							
							
				// p->DisValue[0]=p->YCJZ;               //�̳�����[19]
				// p->DisValue[1]=p->HYL;                 //������[20]
				// p->DisValue[2]=p->FHXS;                //����ϵ��[10]
				// p->DisValue[3]=p->ZSXS;                //����ϵ��[9]
				// p->DisValue[4]=p->JZHYL;               //��׼������[35]
				// p->DisValue[5]=(float)p->DRWJH;        //�����ļ���
							  if(ps->Key.Mode==1) //�̳�����[19]
								{
									ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,1000,0.0);
							        four.FloatData=DisBuff[ps->Key.Mode-1];
									ps->DisValue[19]=ps->YCJZ=four.FloatData;
								}
								else if(ps->Key.Mode==2)  //������[20]
								{
									ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,30,0.0);
							        four.FloatData=DisBuff[ps->Key.Mode-1];
									ps->DisValue[20]=ps->HYL=four.FloatData;
									if(ps->DisValue[20]!=21)
									{
										ps->DisValue[38]=21.0f/(21.0f-ps->DisValue[20]);
									}
									else
									{
										ps->DisValue[38]=0;  //��ʣϵ��
									}
									
								}
								else if(ps->Key.Mode==3)   //����ϵ��[10]
								{   ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,2.0,0.2);
							        four.FloatData=DisBuff[ps->Key.Mode-1];
									ps->DisValue[10]=ps->FHXS=four.FloatData;
								}
							
								else if(ps->Key.Mode==4)  //����ϵ��[9]
								{   ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,99999,1.0);
							        four.FloatData=DisBuff[ps->Key.Mode-1];
									ps->DisValue[9]=ps->ZSXS=four.FloatData;
									if(ps->DisValue[9]!=21)
									{
										ps->DisValue[35]=ps->JZHYL=21.0f/(21.0f-ps->ZSXS);
									}
									else
									{
										ps->DisValue[35]=ps->JZHYL=0;
									}
									
									
								}
								else if(ps->Key.Mode==5)  //��׼������[35]
								{
									ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,20,0.0);
							        four.FloatData=DisBuff[ps->Key.Mode-1];
									ps->DisValue[35]=ps->JZHYL=four.FloatData;
									if(ps->JZHYL!=0)
									ps->DisValue[9]=ps->ZSXS=21.0f-(21.0f/ps->JZHYL);
									else
									ps->DisValue[9]=ps->ZSXS=0;
									
								}
								else if(ps->Key.Mode==6) //�����ļ���
								{
									
									ps->DRWJH=four.FloatData;
									
								}
							
							
							 // SaveFlaotToFlash(four.FloatData,FFLASHIS_AdrYCCJ_YCWJCX_JSYCZL+2*(ps->Key.Mode-1));//�������� 
							
							
						} 
						ScreenBackFirstOpt(ps);	       //�˳����
						ScreenEnterNumberText(ps,2,10); //��ֵ����
			}
			break;
		}
		case 6:
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{   
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnterDisBuffer(ps,&four.FloatData,0,SdFile_GetData()->GasLinkFileMax,1);
					ps->DRWJH = four.FloatData;
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);    //�����ļ���
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnterDisBuffer(ps,&four.FloatData,0,SdFile_GetData()->GasLinkFileMax,1); 
					ps->DRWJH = four.FloatData;
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,0,10); //��ֵ����
			}
			break;
		}						
	}
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x7000);
			
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x0006)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//�����������					
					ScreenTowOptChose(ps,ps->Button);	
				
				}
				else if(ps->Button==0x0007)  //����ֵ����
				{
				  //to do ��������
					if(GetDustJS_INpuData(ps->DRWJH))  //���������ļ���
					{
					ps->RunModeStat=1;
//					ps->LoadFlag=1;  // modify by alfred, 2020.07.08
						Dis_Tips(ps,(u8 *)"����ɹ�",0x01C0);
					}
					else
					{
						Dis_Tips(ps,(u8 *)"����ʧ��",0x01C0);
					}

					ps->Key.Opt=7;
				}
				else if(ps->Button==0x0008)  //����ֵ����
				{
				  //to do ��������
						/*��������*/
					//	float buffer[40];
					//	SaveGasJS_InputDataLoad(buffer);
						SaveDustData(ps->DisValue,ps->FileNumber);
						if(SdFile_WriteDustRecord(&SdFile_GetData()->InterFace_data,ps->FileNumber)==0)
						{
						   Dis_Tips(ps,(u8 *)"����ɹ�",0x01C0);	
						}else
						{
						   Dis_Tips(ps,(u8 *)"����ʧ��",0x01C0);	
						}
						ps->Timer=3000;
					  ps->Key.Opt=8;
				}
				
	  ScreenChangeDataInit(ps);
					ps->StarCun=0;//�������ļ���
	} 
}
//---
/*
	��������void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
  ������	�ú���Ϊ��������-������ѯ-�������ý��水������---����
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25,�޸ĳ�ʱ����, modify by alfred.
*/
void Button_YCCY_YCCX_JS_YY_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	FourBytes four;
	//Button_DataStructure *p = Button_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:
		case 2:	
		case 3:
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					int cunt=ps->Key.Mode-1;
					ScreenConfirmEnterDisBuffer(ps,DisBuff,cunt,1000000,0.0);
					four.FloatData=DisBuff[cunt];
					if(cunt<2)
						ps->DisValue[32+cunt]=four.FloatData;
					else if(cunt==2)
					{
						ps->DisValue[32+cunt]=(u32)four.FloatData;
					}
					
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}	
			}
			else
			{   			
				ScreenTowOpButton(ps,1,0,0);  		        
				if(ps->Button==0x0Aff) //ȷ��
				{
					int cunt=ps->Key.Mode-1;
					ScreenConfirmEnterDisBuffer(ps,DisBuff,cunt,1000000,0.0);
					four.FloatData=DisBuff[cunt];
					if(cunt<2)
						ps->DisValue[32+cunt]=four.FloatData;
					else if(cunt==2)
					{
						ps->DisValue[32+cunt]=(u32)four.FloatData;
					}
				//	pf->Flag=1;//��������
				} 
					ScreenBackFirstOpt(ps);	       //�˳����
					ScreenEnterNumberText(ps,2,10); //��ֵ����
			}
			break;
		}			
	}		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x7000);
			        SaveDustData(ps->DisValue,ps->FileNumber);
					SdFile_WriteDustRecord(&SdFile_GetData()->InterFace_data,ps->FileNumber);
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x0003)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//�����������					
					ScreenTowOptChose(ps,ps->Button);	
				
				}
			
				
	ScreenChangeDataInit(ps);
					ps->StarCun=0;//�������ļ���
	} 
}
//---2020-05-23
/*
	��������void Button_YCCY_YCCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	������	�ú���Ϊ�̳�����-�̳���ѯ-�ļ�����-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��	1.2020-05-25,�޸ĳ�ʱ����, modify by alfred.
				2. �����̳�������by alfred, 2020.06.04
*/
void Button_YCCY_YCCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:
		case 2:
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,SdFile_GetData()->DustLinkFileMax,1);	// modify by alfred, 2020.07.09
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}	
			}
			else
			{  			
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,SdFile_GetData()->DustLinkFileMax,1);
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,0,7); //��ֵ����
			}
			break;
		}	
	}
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x7000);
			
				}
				else if(ps->Button==0x0001||ps->Button==0x0002)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//�����������					
					ScreenTowOptChose(ps,ps->Button);	
				
				}
				else if(ps->Button==0x0003)  //����ֵ����
				{
				  //to do ��������
					char uMachineNumBuffer[10], uSysDateBuffer[10];
					FourBytes four;
					four.U16Data[0]=pf->Data[FLASHIS_AdrCJWHYQBH];
					four.U16Data[1]=pf->Data[FLASHIS_AdrCJWHYQBH+1];
					
					sprintf((char*)uMachineNumBuffer,"%08d",four.U32Data);
					sprintf((char*)uSysDateBuffer,"%02d%02d%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday);
					
					Usb_OutDustFileInf(DisBuffer[0],DisBuffer[1], uSysDateBuffer, uMachineNumBuffer);
					ps->Key.Opt=3;
				}
						else if(ps->Button==0x0004)  //����ֵ����
				{
				  //to do ��ӡ				
					ps->Key.Mode=0;
					ps->Key.Opt=4;
					if(AT_GetData()->Mode==0)
					{
					u8 pn[33]={0};
					float buffer[5];
					pn[0]=	pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX];
					u32 pns=GetU32dataFormFlash2Word( FLASHIS_AdrYCCJ_YCWJCX_DYXX+1);
					for(int i=0;i<32;i++)
					{
					   if(pns&(1<<i)) pn[i+1]=1;
							
					}
					buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
					buffer[1] = DisBuffer[0];
					buffer[2] = DisBuffer[1];
					buffer[3] = DisBuffer[0];
					PrintDustFile((u8 *)&pn[0],(float *)&buffer[0]);
				   }
					else
					{
						Screen_ResetDataBuffer();
	                    strcpy((char *)ps->Buffer,"û���Ӵ�ӡ��!");//����ʾ20200519
	                    Screen_RefreshDataMulti(0x3B20,ps->Buffer,15*2);
					}
					
				}
	ScreenChangeDataInit(ps);
	} 
}
//---
/*
	��������void Button_YCCY_YCCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	������	�ú���Ϊ�̳�����-�̳���ѯ-��ӡѡ��-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_YCCY_YCCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}

	  }
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x7000);
			           pf->Flag=1;                         //20200522  
				}
				else if(ps->Button>=0x0002&&ps->Button<=0x21)  //����ֵ����
				{
					ps->Key.Opt= ps->Button;
					u32 data=pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+1]+((pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+2])<<16);;
					u8 point=ps->Key.Opt-2;
					if(point>15)
					{      data=data>>16;
						     point-=16;
					if((data>>point)&0x00000001)
					{
					  pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+2]&=~(1<<point);//λ��0
					}else
					{
					 pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+2]|=(1<<point);//λ��1
					}
					}else
					{
									if((data>>point)&0x00000001)
						{
							pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+1]&=~(1<<point);//λ��0
						}else
						{
						 pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+1]|=(1<<point);//λ��1
						}
					}
			
				
				}if(ps->Button==0x01)
				{
				  			if(pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX]==1)
						{
							pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX]=0;//λ��0
						}else
						{
						 pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX]=1;//λ��1
						}
				}
	ScreenChangeDataInit(ps);
	} 
}
//---
/*
	��������void Button_YCCY_YCCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	������	�ú���Ϊ����ά��-��������-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼�� 2020-05-25, �޸Ľ��ճ�ʱ����, modify by alfred.
*/
void Button_WHJZ_MMSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
     case 1:
		 {
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)RcvBuffer_GetData()); 
					u16 PassWord=(u16)atoi((char*)&(ps->Key.Buffer[0]));
					ps->sRcvType = 0;
					ScreenBackLastOpt(ps);	       //�˳����
					//u16 jjj=YLB3330D_CJMM;//pf->Data[FLASHIS_AdrCJWHMM];    //TEST
					if(PassWord==pf->Data[FLASHIS_AdrXTWHMM])
					{
					  ps->Button=3; //����ͨ��
					}else if(PassWord==YLB3330D_CJMM)//pf->Data[FLASHIS_AdrCJWHMM])
					{
					   ps->Button=5; //������������
					}
						else
					{
					   ps->Button=4; //�������
					}	
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}																
			}
			else
			{					
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
					u16 PassWord=(u16)atoi((char*)&(ps->Key.Buffer[0]));
					ScreenBackFirstOpt(ps);	       //�˳����
					if(PassWord==pf->Data[FLASHIS_AdrXTWHMM])
					{
					  ps->Button=3; //����ͨ��
					}else if(PassWord==YLB3330D_CJMM)//pf->Data[FLASHIS_AdrCJWHMM])
					{
					   ps->Button=5; //������������
					}
					else
					{
					   ps->Button=4; //�������
					}
					break;
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,0,5); //��ֵ����
			}
			break;
		}			 						
	}
		
	 { //��Ļ��������
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,52);
			
				}
				else if(ps->Button==0x0001)  //����ֵ����
				{
						if(ps->Key.bOutsideKeyboard != true)
									Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5					
				   ScreenTowOptChose(ps,ps->Button);	
						
				}
				else if(ps->Button==0x0003)  //����ֵ����
				{
					 if(ps->Warning==5)
					 { 
					   ps->RunMode=0x8200;	//���볬��ά��
					 }else
					 {
					  ps->RunMode=0x8100;	
					 }
				  
						
				}else if(ps->Button==0x0004)  //����ֵ����
				{
				   ps->Warning=1;	
						
				}else if(ps->Button==0x0005)  //����ֵ����
				{
				   ps->Warning=2;	
						
				}
	ScreenChangeDataInit(ps);
	} 
}
//---
/*
	��������void Button_WHJZ_WHXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	������	�ú���Ϊ����ά��-ά��ѡ��-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��	1. �޸Ľ�����������ҳ��ǰ�Ƚ���ʾ�������ʾ�������, modify by alfred, 2020.06.19.
*/
void Button_WHJZ_WHXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	//Button_DataStructure *p = Button_GetData();
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{

		case 0:
		{//û��ѡ�ж�������
			ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}

	}
		
	{ //��Ļ��������
		ps->OldMode=ps->RunMode;
		if(ps->Button==0x0AFe||ps->Button==Max_Opt)
		{
			ScreenBackLastPag(ps,52);

		}
		else if(ps->Button>=0x0001&&ps->Button<Max_Opt)  //����ֵ����
		{
			ps->RunMode=0x8100+0x10*ps->Button;	
			if(ps->RunMode==0x8130)
			{	//�����һ����������ά��ҳ�棬����������������ʾ��ʹ����ҳ��ʱ��������ʾ�հס�
				ps->DisValue[0] = -1;
				ps->DisValue[1] = -1;
				ps->DisValue[2] = -1;
				Slave_GetData()->E28.ADRRset=Slave_GetData()->E28.ADRRRD24G;   //�ȴӸ���������ߵĵ�ַ
			    Slave_GetData()->E28.CHset=Slave_GetData()->E28.CHRD24G;       //�ȴӸ���������ߵ��ŵ�
			}
			
			if(ps->RunMode==0x8140)   //�ر�ҳ��������
			ps->PauseOrStop=0;		
			
			ps->Key.Opt=ps->Button;
			FourBytes four;
			//four.FloatData=20;
			//SaveFlaotToFlash(four.FloatData,FLASHIS_AdrWHJZ_LLJZ_BZLL);

			switch(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ])
			{
				  case 0x01: four.FloatData=20;
					  break;
				  case 0x02: four.FloatData=30;
					  break;
				  case 0x03:four.FloatData=40;
					  break;
				  case 0x04: four.FloatData=50;
					  break;
				  case 0x05:four.FloatData=60;
					  break;
				  case 0x06: four.FloatData=70;
					  break;
				  case 0x07: four.FloatData=80;
					  break;
				  case 0x08:four.FloatData=90;
					  break;
				  case 0x09:four.FloatData=100;
					  break;
				  case 0x0a: four.FloatData=1.0;
					  break;
                  default:four.FloatData=20;break;
			
			  }
			 SaveFlaotToFlash(four.FloatData,FLASHIS_AdrWHJZ_LLJZ_BZLL);
			if(ps->Key.Opt==7)  ps->SubCnt=1;
		}			
		ScreenChangeDataInit(ps);    //��������
	} 
}
//--2020-05-17-//
/*20200526
	��������void Button_WHJZ_WHXZ_STSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-ϵͳ����-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼�� 2020-05-25, �޸Ľ��ճ�ʱ����, modify by alfred.
*/
void Button_WHJZ_WHXZ_STSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	FourBytes four;
//	FRESULT res;

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{

		case 0:
		{//û��ѡ�ж�������
			ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:	
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)RcvBuffer_GetData()); 
					ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_XTSZ_NO2_NOX+2*(ps->Key.Mode-1),2,100.0,0);

					if(ps->Key.Mode==2)
					{
						if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x01))==0)
						{
							pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]|=(0x01);
						}else
						{
							pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&=~(0x01);
						}
					} 
					//20200526
					if(ps->Key.Mode==6)   //������ϵ�����׼������  20200526
					{
						four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_XTSZ_ZSXS);  //��ȡ����ϵ��
						if(four.FloatData==0)
						{
							SaveFlaotToFlash(0.0f,FLASHIS_AdrWHJZ_XTSZ_ZSXS);
							SaveFlaotToFlash(1.0f,FLASHIS_AdrWHJZ_XTSZ_JZYHL); 
						}
						else
						{
							four.FloatData=21.0f-(21.0f/four.FloatData);
							SaveFlaotToFlash(four.FloatData,FLASHIS_AdrWHJZ_XTSZ_JZYHL);
						}
					}
					else if(ps->Key.Mode==7)  //�ɻ�׼������������ϵ�� 20200526
					{
						four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_XTSZ_JZYHL);  //��ȡ��׼������
						if(four.FloatData==21)
						{
							SaveFlaotToFlash(21.0f,FLASHIS_AdrWHJZ_XTSZ_ZSXS);
							SaveFlaotToFlash(20.0f,FLASHIS_AdrWHJZ_XTSZ_JZYHL);
						}	
						else
						{
							four.FloatData=21.0f/(21.0f-four.FloatData);
							SaveFlaotToFlash(four.FloatData,FLASHIS_AdrWHJZ_XTSZ_ZSXS);
						}		 
					}

					ps->sRcvType = 0;    
					ScreenBackLastOpt(ps);     
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}		
			}					
			else
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_XTSZ_NO2_NOX+2*(ps->Key.Mode-1),2,100.0,0);
					if(ps->Key.Mode==2)
					{
						if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x01))==0)
						{
							pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]|=(0x01);
						}else
						{		
							pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&=~(0x01);
						}
					}
					//20200526
					if(ps->Key.Mode==6)   //������ϵ�����׼������  20200526
					{
					 four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_XTSZ_ZSXS);  //��ȡ����ϵ��
					 if(four.FloatData==0)
					 {
						 SaveFlaotToFlash(0.0f,FLASHIS_AdrWHJZ_XTSZ_ZSXS);
		                 SaveFlaotToFlash(1.0f,FLASHIS_AdrWHJZ_XTSZ_JZYHL); 
					 }
					 else
					 {
						 four.FloatData=21.0f-(21.0f/four.FloatData);
						 SaveFlaotToFlash(four.FloatData,FLASHIS_AdrWHJZ_XTSZ_JZYHL);
					 }
					
					}
					else if(ps->Key.Mode==7)  //�ɻ�׼������������ϵ�� 20200526
					     {
                         four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_XTSZ_JZYHL);  //��ȡ��׼������
	                    if(four.FloatData==21)
	                     {
		                 SaveFlaotToFlash(21.0f,FLASHIS_AdrWHJZ_XTSZ_ZSXS);
		                 SaveFlaotToFlash(20.0f,FLASHIS_AdrWHJZ_XTSZ_JZYHL);
	                     }	
                         else
                         {
		                 four.FloatData=21.0f/(21.0f-four.FloatData);
		                 SaveFlaotToFlash(four.FloatData,FLASHIS_AdrWHJZ_XTSZ_ZSXS);
	                     }		 

                        }
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,2,5); //��ֵ����
			}
			 break;			
	  }	
	}
		
	{ //��Ļ��������
		ps->OldMode=ps->RunMode;
		if(ps->Button==0x0AFe||ps->Button==Max_Opt)
		{
			ScreenBackLastPag(ps,0x8100);
			pf->Flag=1;                               //20200526
		}
		else if(ps->Button>=0x0001&&ps->Button<=0x00007)  //����ֵ����
		{
			if(ps->Button==0x0002)
			{ 
				ps->RunMode=0x8111;
				Flux_StarSampling('C',1.0);   //��������C��   20200525
			
			}
			else if(ps->Button==0x0003)                   //20200525
			{
				if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x02))==0)
				{
					pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]|=(0x02); //20200526
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5						 
					ScreenTowOptChose(ps,ps->Button);	
				}else
				{
					pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&=~(0x02);  //20200526
				}
			}
			else
			{
				if(ps->Key.bOutsideKeyboard != true)
				Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5						 
				ScreenTowOptChose(ps,ps->Button);	
			}
		}
		else if(ps->Button==0x0008)  //����ֵ����
		{
			//to do �̳��ļ�ɾ�� 
			if(SdFile_DeleteDustFiles() == FR_OK)	//���ɾ���ɹ�����Ļ��ӡ��ʾ��Ϣ, add by alfred, 2020.07.13
			{
				ps->gSysConfigWaringBits.BitField.m_uDustFile = 1;
			}				
			ps->Key.Opt=ps->Button;		
			ps->Key.Mode=0;ps->RunModeStat=1;	
		}
		else if(ps->Button==0x0009)  //����ֵ����
		{
			// to do �����ļ�ɾ��
			if(SdFile_DeleteGasFiles() == FR_OK)	//���ɾ���ɹ�����Ļ��ӡ��ʾ��Ϣ, add by alfred, 2020.07.13
			{
				ps->gSysConfigWaringBits.BitField.m_uGasFile = 1;
			}
			else
			{
				ps->gSysConfigWaringBits.BitField.m_data0 = 1;	//�ļ�ɾ��ʧ��
			}			
			ps->Key.Opt=ps->Button;	
			ps->Key.Mode=0;	ps->RunModeStat=1;					
		}
		else if(ps->Button==0x000A)  //����ֵ����
		{
			// to do �����ļ�ɾ��
			if(SdFile_DeleteWorkFiles() == FR_OK)	//���ɾ���ɹ�����Ļ��ӡ��ʾ��Ϣ, add by alfred, 2020.07.13
			{
				ps->gSysConfigWaringBits.BitField.m_uWorkFile = 1;
			}
			else
			{
				ps->gSysConfigWaringBits.BitField.m_data0 = 1;	//�ļ�ɾ��ʧ��
			}			
			ps->Key.Opt=ps->Button;	
			ps->Key.Mode=0;	ps->RunModeStat=1;					

		}
		else if(ps->Button==0x000B)  //����ֵ����
		{
			// to do �ָ���������
			Flashisp_FactoryReset(0);   //20200524
			Flashisp_Restore();
			ps->Key.Opt=ps->Button;		
			ps->Key.Mode=0;ps->RunModeStat=1;	
		}
		else if(ps->Button==0x000C)  //����ֵ����
		{
			// to do ɾ�������ļ�
			//SdFile_DeleteGasFiles(); //20200524
			//SdFile_DeleteWorkFiles();      //20200524
			//SdFile_DeleteDustFiles();//20200524
			//SdFile_DeleteFlueFiles();
			if((SdFile_DeleteGasFiles() == FR_OK)&&(SdFile_DeleteWorkFiles() == FR_OK)&&(SdFile_DeleteDustFiles() == FR_OK)&&(SdFile_DeleteFlueFiles() == FR_OK))	//���ɾ���ɹ�����Ļ��ӡ��ʾ��Ϣ, add by alfred, 2020.07.13
			{
				ps->gSysConfigWaringBits.BitField.m_uAllFile = 1;
			}
			else
			{
				ps->gSysConfigWaringBits.BitField.m_data0 = 1;	//�ļ�ɾ��ʧ��
			}
			ps->Key.Opt=ps->Button;	
			ps->Key.Mode=0;		
			ps->RunModeStat=1;					
		}		
		ScreenChangeDataInit(ps);
	} 
}

//----2020-05-18-----
/*
	��������void Button_WHJZ_WHXZ_STSZ_CO_SO2_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-ϵͳ����-���ý���-CO-SO2У׼��������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼�� 2020-05-25, �޸Ľ��ճ�ʱ����, modify by alfred.
*/
void Button_WHJZ_WHXZ_STSZ_CO_SO2_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();

	float dbuf;
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		default:	
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				if(ps->Key.Mode>0&&ps->Key.Mode<=10)
				{
					ProcessSoftKeyboardInput(ps,FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1+(ps->Key.Mode-1),1,10000,0,CALC_NO_POINT,CALC_NO_SIGNS);				
				}
				else if(ps->Key.Mode>10)
				{
					ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);
					if(ps->Button==0x0Afe)
					{     
						ScreenBackLastOpt(ps);
					}	
					if(ps->sRcvType != 0)
					{
						Screen_ResetKeyDataBuffer();//�������ֵ
						strcpy((char*)ps->Key.Buffer,(char*)RcvBuffer_GetData()); 									
						ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,100.00,0);
						pf->Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1+ps->Key.Mode-1]=DisBuff[ps->Key.Mode-1]*100;
						ps->sRcvType = 0;	
						ScreenBackLastOpt(ps);
					}
					else if(ps->Button==0x0Aff)
					{
						ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;
						ps->uEnableRecDataTimer = 1;  //�������OK��������1.5S��ʱ����鳬ʱ			
					}		
				}
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Key.Mode>0&&ps->Key.Mode<=10)
				{
					if(ps->Button==0x0Aff) //ȷ��
					{
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1+(ps->Key.Mode-1),1,10000,0);				
					} 
					ScreenEnterNumberText(ps,0,5); //��ֵ����
				}else if(ps->Key.Mode>10)
				{
					ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,100.00,0);
					ScreenEnterNumberText(ps,2,5); //��ֵ����
					pf->Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1+ps->Key.Mode-1]=DisBuff[ps->Key.Mode-1]*100;					
				}			
				ScreenBackFirstOpt(ps);	       //�˳����
			}							
			 break;			
		}			
	}
		
	 { //��Ļ��������
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)   //������һҳ
				{
					 ScreenBackLastPag(ps,0x8110);
			          Flux_StopSampling('C');               //ֹͣC��20200525
					  //�ѵ�������������FLASHIS_AdrWHJZ_XTSZ_CO_SO2������
				
					  dbuf=ps->DisValue[5+10];
					  SaveFlaotToFlash(dbuf,FLASHIS_AdrWHJZ_XTSZ_CO_SO2);  //�����õ�ֵ
				}
				else if(ps->Button>=0x0003&&ps->Button<=0x000016)  //����ֵ����
				{
						if(ps->Key.bOutsideKeyboard != true)
						 Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button-2);//�����������
						 ScreenTowOptChose(ps,ps->Button-2);	
					     ps->Key.Opt=ps->Button;	
					     ps->RunModeStat=1;	
				}
			  else if(ps->Button==0x0001)  //�л���λ
				{
					 if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x04))==0)
							{
							  pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]|=(0x04);
							}else
							{
							 pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&=~(0x04);
							}
				 ps->Key.Opt=ps->Button;		
				 ps->Key.Mode=0;ps->RunModeStat=1;	
				}
				else if(ps->Button==0x0002)  //ѡ��
				{
				  		  if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x01))==0)  //20200525
							{
							  pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]|=(0x01);   //20200525
							}else
							{
							 pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&=~(0x01);  //20200525
							}
							ps->Key.Opt=ps->Button;		
							ps->Key.Mode=0;ps->RunModeStat=1;				
				}
				
	ScreenChangeDataInit(ps);
	} 
}

//----2020-05-18

/*
	��������void Button_WHJZ_WHXZ_LLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-����У׼-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼�� 2020-05-25, �޸Ľ��ճ�ʱ����, modify by alfred.
*/
void Button_WHJZ_WHXZ_LLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:	
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}	
				if(ps->sRcvType != 0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)RcvBuffer_GetData()); 									
					if(ps->Key.Mode==1)
					{   if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]==10)
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_LLJZ_BZLL+2*(ps->Key.Mode-1),2,2.0,0.1);
						else
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_LLJZ_BZLL+2*(ps->Key.Mode-1),2,110.0,5.0);
						
					}
					else if(ps->Key.Mode==13)
						ScreenConfirmEnterDisBuffer(ps,DisBuff,16,130,50);
					else if(ps->Key.Mode==12)
						ScreenConfirmEnterDisBuffer(ps,DisBuff,13,130,0);
					else
					{
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_LLJZ_BZLL+2*(ps->Key.Mode-1),2,2.0,0.2);
						pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]=ps->Key.Mode-1;
					}
					ps->sRcvType = 0;	
					ScreenBackLastOpt(ps);
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}										
			}	
			else
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
					if(ps->Key.Mode==1)
					{  if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]==10)
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_LLJZ_BZLL+2*(ps->Key.Mode-1),2,2.0,0.1);
						else
					   ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_LLJZ_BZLL+2*(ps->Key.Mode-1),2,110.0,5.0);
					}
					else if(ps->Key.Mode==13)
					ScreenConfirmEnterDisBuffer(ps,DisBuff,16,130,50);
					else if(ps->Key.Mode==12)
					ScreenConfirmEnterDisBuffer(ps,DisBuff,13,130,0);
					else
					{
					  ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_LLJZ_BZLL+2*(ps->Key.Mode-1),2,2.0,0.2);
					  pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]=ps->Key.Mode-1;
				  }
				  
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,2,6); //��ֵ����
			}
		 break;			
		}			
	}
		
	 { //��Ļ��������
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x8100);
					//�˳�ǰ�ر�
					 ps->Function=0;
					 if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]<10)
						 {
						     Flux_StopSampling('B');
						 }else  if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]==10)
						 {
						     Flux_StopSampling('C');
						 }
						 	Flux_LoadJWTem(0, ps->DisValue[13]);
		                    Flux_LoadDQPress(0,ps->DisValue[16]);
				}
				else if(ps->Button>=0x0002&&ps->Button<=0x0000B)  //����ֵ����
				{	    
					if((ps->LastChoice+1)==ps->Button)
					{
						ScreenTowOptChose(ps,ps->Button);	
					}
					else
					{
					    ps->Key.Opt=ps->Button;	ps->RunModeStat=1;
					   	pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]=ps->Button-1;	
						FourBytes four;
					    four.FloatData=	pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]*10+10;
					    if(four.FloatData>100) four.FloatData=1.0;
					    SaveFlaotToFlash(four.FloatData,FLASHIS_AdrWHJZ_LLJZ_BZLL);	

					}
				  	//ѡ���ֹͣ��
					   ps->Function=0;
					   Flux_StopSampling('B');
					   Flux_StopSampling('C');
					   ps->PauseOrStop=0;	
					
				}	else if(ps->Button==0x000c||ps->Button==0x0001||ps->Button==0x0000d)  //����ֵ����
				{
							if(ps->Key.bOutsideKeyboard != true)
							Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//�����������
							ScreenTowOptChose(ps,ps->Button);	
							ps->RunModeStat=1;
					
				}
				else if(ps->Button>=0x0022&&ps->Button<=0x0002b)  //����ֵ����
				{
							if(ps->Key.bOutsideKeyboard != true)
										Screen_WriteRegisterOneData(0x4f,0xA0+(ps->Button&0x000f));//�����������
      				ScreenTowOptChose(ps,(ps->Button&0x000f));
              // pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]=ps->Key.Mode-1;					
              ps->RunModeStat=1;					
				}
			  else if(ps->Button==0x000e)  //����ֵ����
				{
					//to do ������ӡ  	
					ps->Key.Opt=ps->Button;		
				    ps->Key.Mode=0;ps->RunModeStat=1;	
					float buffer[15];
					buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
					buffer[1]=ps->DisValue[13];
					buffer[2]=ps->DisValue[16];
					buffer[3]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_LLJZ_YQBL01);
					for(int i=0;i<9;i++)
					buffer[4+i]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_LLJZ_YCBL01+2*i);
					
					PrintFlowPara(buffer);//��ӡ��������
				}
				else if(ps->Button==0x000f)  //����ֵ����
				{
				  // to do ����
					 if(ps->PauseOrStop==B0)
					 {
						ps->PauseOrStop=0;
						 if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]<10)
						 {
						     Flux_StopSampling('B');
						 }else  if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]==10)
						 {
						     Flux_StopSampling('C');
						 }
					 }else
					 {
						   FourBytes four;
							GetU16dataFormFlash2Word(&four,FLASHIS_AdrWHJZ_LLJZ_BZLL,pf);	
						  if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]<10)
						 {
						   Flux_StarSampling('B',four.FloatData);
						   Flux_ClearMidBuff(&Flux_GetData()->ChannelB.FluxTowData.Filter,Flux_GetData()->ChannelB.ShowFlux);
						 }else  if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]==10)
						 {
						    Flux_StarSampling('C',1.0);
							Flux_ClearMidBuff(&Flux_GetData()->ChannelC.FluxTowData.Filter,Flux_GetData()->ChannelC.ShowFlux);
						 }
						 
					     ps->PauseOrStop=B0;
					 }
			  		ps->Key.Opt=ps->Button;	
					  ps->Key.LastOpt=0xff;
            ps->Key.Mode=0;	ps->RunModeStat=1;					
				}
				
	ScreenChangeDataInit(ps);
	} 
}
//----2020-05-19
/*
	��������void Button_WHJZ_WHXZ_LLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-��������-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��	1. 2020-05-25,�޸ĳ�ʱ����, modify by alfred.
				2. 2020-05-27���޸�����¼�봥���ַ���޸�ά�����벻�ܱ�������⣬�޸���������flash��ַ

*/
void Button_WHJZ_WHXZ_MMSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
//	static u8 uTempKeymode; 

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //��Χ��������			
		  break;
		}
		case 1:    //�����˵��Ĵ����������룩
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			if(ps->Key.bOutsideKeyboard != true)  //��Ļ����
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)RcvBuffer_GetData()); 
					if(ps->Key.Mode<=3)
					{
					ScreenConfirmEnterDisBuffer(ps,DisBuff,(ps->Key.Mode-1),65535,0);
					}
					else if(ps->Key.Mode==4)   //��ӡ��������
					{
					ScreenConfirmEnter(ps,FLASHIS_AdrXTWHLYPDM,1,65535,0);
					}
					else if(ps->Key.Mode==5)  //APP��������
					{								
					ScreenConfirmEnter(ps,FLASHIS_AdrXTWHAPPLYPDM,1,65535,0);
					}
					else if(ps->Key.Mode==6) //E28��ַ
					{	
					 ScreenConfirmEnterDisBuffer(ps,DisBuff,(ps->Key.Mode-1),65535,0);
					 Slave_GetData()->E28.ADRRset=(u16)DisBuff[5];
					 ps->g_E28_Set_StateMachine=E28_SET_STATE_MACHINE_INIT;	 //����дE28�߳�
					 ps->uE28CtlState=1;
					 ps->Warning=6;	
					}
					else if(ps->Key.Mode==7) //E28�ŵ�
					{	
					 ScreenConfirmEnterDisBuffer(ps,DisBuff,(ps->Key.Mode-1),255,0);	
					 Slave_GetData()->E28.CHset=(u8)DisBuff[6];
					 ps->g_E28_Set_StateMachine=E28_SET_STATE_MACHINE_INIT;	 //����дE28�߳�
					 ps->uE28CtlState=1;
					 ps->Warning=6;	
					}
					
					ps->sRcvType = 0;
					ScreenBackLastOpt(ps);	
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}			
			}		
			else                                //��Χ��������
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
					if(ps->Key.Mode<=3)
						ScreenConfirmEnterDisBuffer(ps,DisBuff,(ps->Key.Mode-1),65535,0);
					else if(ps->Key.Mode==4)
					{
						ScreenConfirmEnter(ps,FLASHIS_AdrXTWHLYPDM,1,65535,0);
								
					}
					else if(ps->Key.Mode==5)
					{						
						ScreenConfirmEnter(ps,FLASHIS_AdrXTWHAPPLYPDM,1,65535,0);
					}			
                    else if(ps->Key.Mode==6) //E28��ַ
					{	
					 ScreenConfirmEnterDisBuffer(ps,DisBuff,(ps->Key.Mode-1),65535,0);
					 Slave_GetData()->E28.ADRRset=(u16)DisBuff[5];
					 ps->g_E28_Set_StateMachine=E28_SET_STATE_MACHINE_INIT;	 //����дE28�߳�
					 ps->uE28CtlState=1;
					 ps->Warning=6;	
						
					}
					else if(ps->Key.Mode==7) //E28�ŵ�
					{	
					 ScreenConfirmEnterDisBuffer(ps,DisBuff,(ps->Key.Mode-1),255,0);	
					 Slave_GetData()->E28.CHset=(u8)DisBuff[6];
					 ps->g_E28_Set_StateMachine=E28_SET_STATE_MACHINE_INIT;	  //����дE28�߳�
					 ps->uE28CtlState=1;
					 ps->Warning=6;	
					}
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,2,6); //��ֵ����
			}
		 break;			
		}					
	    }
		
	    { //��Ļ��������һ���˵�ʱ
		        ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)  //������һҳ
				{
						ScreenBackLastPag(ps,0x8100);
			
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x00003)  //����ֵ����
				{
 						if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);      
						ScreenTowOptChose(ps,ps->Button);	
					
				}
			    else if(ps->Button==0x0004)  //����ֵ����
				{
					//to do ȷ�������޸�  	
					ps->Key.Opt=ps->Button;		
					ps->Key.Mode=0;ps->RunModeStat=1;
//					u16 ttt = pf->Data[FLASHIS_AdrXTWHMM];
					if((ps->DisValue[0]==pf->Data[FLASHIS_AdrXTWHMM])&&(ps->DisValue[1]==ps->DisValue[2]))
					{
						pf->Data[FLASHIS_AdrXTWHMM] = ps->DisValue[1];	//2020-05-26, �����޸ĺ������, modify by alfred.
						ps->Warning=1;
						pf->Flag = 1;
					}else
					{
					   ps->Warning=2;
					}	
						ps->DisValue[0]=-1;	
						ps->DisValue[1]=-1;	
						ps->DisValue[2]=-1;						
				}
				else if(ps->Button==0x0005)  //����ֵ����
				{   //���������
					if(pf->Data[FLASHIS_AdrXTSZBOX]&(1<<2))	//�����ַ��ΪFLASHIS_AdrXTSZBOX, 2020-05-27, by Alfred
					{
						pf->Data[FLASHIS_AdrXTSZBOX]&=~(1<<2);
						AT_Power_Off();
					}else
					{
						pf->Data[FLASHIS_AdrXTSZBOX]|=(1<<2);
						AT_Power_On();
					}	
					pf->Flag=1;
					ps->Key.Mode=0;
					ps->RunModeStat=1;//ˢ������
			  		ps->Key.Opt=ps->Button;	 
				}
	
				else if(ps->Button==0x0006)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA0+4);  
					// to do ȷ��
					ScreenTowOptChose(ps,4);	
			  		ps->Key.Opt=ps->Button;	 
				} 				
				else if((ps->Button==0x0007)&&(ps->uTempState!= true))  //��������
				{
					// to do ������ʼ����
					if(pf->Data[FLASHIS_AdrXTSZBOX]&(1<<2)) //�������ش�ʱ�ų�ʼ��
					{
						ps->Key.Opt=ps->Button;		
						ps->Key.Mode=0;ps->RunModeStat=1;
						ps->Key.Opt=ps->Button;
						ps->uTempState = true; //������ʼ���������߳�
						ps->Warning = 6;
					}
					else
					{
						ps->Warning = 3;	//�����������û�򿪣�����Ļ����ʾ��ʾ��Ϣ��
					}
				}
				else if(ps->Button==0x0008)  //��������
				{  //APP����ѡ��
					if(pf->Data[FLASHIS_AdrXTWHLYPDMKG]&0x02)
					{
						 pf->Data[FLASHIS_AdrXTWHLYPDMKG]&=~(0x02);
					}else
					{
						pf->Data[FLASHIS_AdrXTWHLYPDMKG]|=(0x02);
					}
					pf->Flag=1;
					ps->Key.Mode=0;
					ps->RunModeStat=1;//ˢ������
			  		ps->Key.Opt=ps->Button;	
				} 				
				else if(ps->Button==0x0009)  //����ֵ����
				{ //APP��������
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+5);  
				       // 
					   ScreenTowOptChose(ps,5);	
			  		   ps->Key.Opt=ps->Button;	 
				} 
				else if(ps->Button==0x000A)  //����ֵ����
				{   // to do APP������ʼ���� ?????????????????
					ps->Key.Mode=0;ps->RunModeStat=1;
			  		ps->Key.Opt=ps->Button;	 
				}				
				else if(ps->Button==0x000B)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+6);  
				       //
					   ScreenTowOptChose(ps,6);	
			  		   ps->Key.Opt=ps->Button;	 
				} 
				else if(ps->Button==0x000C)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+7);  
				       // to do ȷ��
					   ScreenTowOptChose(ps,7);	
			  		   ps->Key.Opt=ps->Button;	 
				} 
             
	ScreenChangeDataInit(ps);
	} 
}
//----2020-05-18
/*
	��������void Button_WHJZ_WHXZ_NDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-Ũ��У׼-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼�� 2020-05-25, �޸Ľ��ճ�ʱ����, modify by alfred.
*/
void Button_WHJZ_WHXZ_NDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		default :    //�ѽ�������˵�
		{
			if(ps->Key.bOutsideKeyboard != true)//��Ļ����
			{
				ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType != 0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)RcvBuffer_GetData()); 									
					ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_NDJZ_LDO2+(ps->Key.Mode-1)*2,2,2000.0,0);

					Slave_ReadFlashZOData(&Slave_GetData()->O2Concent,FLASHIS_AdrWHJZ_NDJZ_LDO2,SLAVE_BaseGas);
                    Slave_ReadFlashZOData(&Slave_GetData()->SO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDSO2,SLAVE_BaseGas);
                    Slave_ReadFlashZOData(&Slave_GetData()->NOConcent,FLASHIS_AdrWHJZ_NDJZ_LDNO,SLAVE_BaseGas);
                    Slave_ReadFlashZOData(&Slave_GetData()->NO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDNO2,SLAVE_BaseGas);
                    Slave_ReadFlashZOData(&Slave_GetData()->COConcent,FLASHIS_AdrWHJZ_NDJZ_LDCO,SLAVE_BaseGas);
                    Slave_ReadFlashZOData(&Slave_GetData()->CO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDCO2,SLAVE_BaseGas);
                    Slave_ReadFlashZOData(&Slave_GetData()->H2SConcent,FLASHIS_AdrWHJZ_NDJZ_LDH2S,SLAVE_BaseGas);
					
					ps->sRcvType = 0;	
					ScreenBackLastOpt(ps);
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}											
			}	
			else                                //��Χ���̴��� 
			{								
				if(ps->Key.Mode<=14)  //����뱶�ʵ�����
				{        
					ScreenTowOpButton(ps,1,0,0);   //�����˵���������
					
					
					if(ps->Button==0x0Aff) //ȷ��
					{
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_NDJZ_LDO2+(ps->Key.Mode-1)*2,2,2000.0,0);

						Slave_ReadFlashZOData(&Slave_GetData()->O2Concent,FLASHIS_AdrWHJZ_NDJZ_LDO2,SLAVE_BaseGas);
                        Slave_ReadFlashZOData(&Slave_GetData()->SO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDSO2,SLAVE_BaseGas);
                        Slave_ReadFlashZOData(&Slave_GetData()->NOConcent,FLASHIS_AdrWHJZ_NDJZ_LDNO,SLAVE_BaseGas);
                        Slave_ReadFlashZOData(&Slave_GetData()->NO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDNO2,SLAVE_BaseGas);
                        Slave_ReadFlashZOData(&Slave_GetData()->COConcent,FLASHIS_AdrWHJZ_NDJZ_LDCO,SLAVE_BaseGas);
                        Slave_ReadFlashZOData(&Slave_GetData()->CO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDCO2,SLAVE_BaseGas);
                        Slave_ReadFlashZOData(&Slave_GetData()->H2SConcent,FLASHIS_AdrWHJZ_NDJZ_LDH2S,SLAVE_BaseGas);

						if(ps->Key.Mode<=7)  //�����ȷ�Ϻ�ת������
						{
							/*
							ScreenTowOptChose(ps,ps->Key.Mode+7);
							ps->Key.Opt=ps->Key.Mode-7;//
							*/
							//test
							ps->Button=ps->Key.Mode+0x20;
							
							break;
						}	
					}							
					ScreenBackFirstOpt(ps);	       //�˳����
					ScreenEnterNumberText(ps,2,6); //��ֵ����
				}
			}
			break;			
		}					
	}
		
	 { //��Ļ��������(�����ⰴ��)
		        ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt) //�˳�
				{
					 ScreenBackLastPag(ps,0x8100);
					 ps->Function=0;
					 Flux_StopSampling('C');
			
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x00007)  //�ⰴ��ֵ����Ļ��������
				{
					 if(ps->Key.bOutsideKeyboard != true)
					 Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//�����������       
					 ScreenTowOptChose(ps,ps->Button);	 //��������봦��
				}	
				else if(ps->Button>=0x0021&&ps->Button<=0x00027)  //��Ļ����ֵ����(�ⰴ���������⼸��ֵ)
				{
                     
                      ps->Key.Opt=ps->Button&0x000f;//	 ����ѡ��
                      ps->RunMode=0x8141;	
			          ps->gzdselect=0;	  //��ȥĬ�ϲ���ѡ��
                      ps->SubCnt=	ps->Key.Opt-1;	
                      ps->DisValue[0]=100;
					  ps->DisValue[1]=1000;//��׼��Դ
					  ps->DisValue[2]=10000;
					  ps->LastChoice=0xff;
					  if(ps->PauseOrStop==B0)
					  {
					   ps->ZjTimer=0;
					   ps->staflg=1;
					   ps->TimerSec=10;
					  }

 					//  Flux_StopSampling('C');
					
				}
			  else if(ps->Button==0x0008)  //����ֵ����
				{
					//to do ����ȷ��	 ��ӡ
					ps->Key.Opt=ps->Button;		
					ps->Key.Mode=0;ps->RunModeStat=1;
					float buffer[30];
					buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
					
					for(int i=0;i<7;i++ )
					{
						 buffer[1+i*4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_NDJZ_LDO2+2*i);  //���
						 buffer[2+i*4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_NDJZ_BLO2_1+2*i); //��
						 buffer[3+i*4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_NDJZ_BLO2_2+2*i); //��
						 buffer[4+i*4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_NDJZ_BLO2_3+2*i); //��
					}
					PrintConcentraPara(buffer);//��ӡŨ������
				}
				else if(ps->Button==0x0009)  //����ֵ����
				{
				  // to do ����
					 if(ps->PauseOrStop==B0)
					 { ps->Key.Opt=ps->Button;	
					   ps->PauseOrStop=0;
					   Flux_StopSampling('C');
						 
					 }else
					 {
						Flux_StarSampling('C',1.0);
					    ps->PauseOrStop=B0;
					  	ps->Key.Opt=ps->Button;		
				        ps->Key.Mode=0;ps->RunModeStat=1;
					 }
				} 	
				else if(ps->Button==0x000A)  //����ֵ����
				{
				  // to do У��
				 Slave_Zero_QT();
				 ps->Key.Opt=ps->Button;		
				 ps->Key.Mode=0;ps->RunModeStat=1;
				} 	

	ScreenChangeDataInit(ps);
	} 
}
//----2020-05-19
/*
	��������void Button_WHJZ_WHXZ_NDJZ_BLSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-Ũ��У׼-��������-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼�� 2020-05-25, �޸Ľ��ճ�ʱ����, modify by alfred.
*/
void Button_WHJZ_WHXZ_NDJZ_BLSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
  Slave_DataStructure* Sp=Slave_GetData();

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��	
			
		  break;
		}
		default :
		{
			if(ps->Key.bOutsideKeyboard != true)  //��Ļ����
			{
				ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType != 0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)RcvBuffer_GetData()); 									
					if(ps->Key.Mode<=3) //ȷ��,���뱶��ֵ����
					{
						u16 Addr=	FLASHIS_AdrWHJZ_NDJZ_BLO2_1+ps->SubCnt*2+(ps->Key.Mode-1)*14;	
						ScreenConfirmEnter(ps,Addr,2,2.0,0.0);
					}
					else if(ps->Key.Mode<=6)//���뱶��ֵ����
					{
						u16 Addr=	FLASHIS_AdrWHJZ_NDJZ_BQLDO2_1+ps->SubCnt*2+(ps->Key.Mode-4)*14;		
						ScreenConfirmEnter(ps,Addr,2,999999.0,0.0);
	

					}
					
					Slave_ReadFlashData(); 
					ps->sRcvType = 0;	
					ScreenBackLastOpt(ps);
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}									
			}	
			else  //�ⰴ��
			{	               
				ScreenTowOpButton(ps,1,0,0);							
				if(ps->Button==0x0Aff)
				{              					
					if(ps->Key.Mode>=0x21&&ps->Key.Mode<=0x23) //ȷ��
					{
						u16 Addr=	FLASHIS_AdrWHJZ_NDJZ_BLO2_1+ps->SubCnt*2+(ps->Key.Mode-0x21)*14;	
						ScreenConfirmEnter(ps,Addr,2,2.0,0.0);
						
						ScreenTowOptChose(ps,ps->Key.Mode+3);  //������һ��
						ps->Key.Opt=ps->Key.Mode-0x23;//
						break;
						
					}
					else if(ps->Key.Mode>=0x24&&ps->Key.Mode<=0x26)
					{
						u16 Addr=	FLASHIS_AdrWHJZ_NDJZ_BQLDO2_1+ps->SubCnt*2+(ps->Key.Mode-0x24)*14;		
						ScreenConfirmEnter(ps,Addr,2,999999.0,0.0);

					}
                       Slave_ReadFlashData(); 

				}																				
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,2,6); //��ֵ����
			}						
		 break;			
	  }				
	}
		
	 { //��Ļ��������
		        ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)   //����
				{
					 ScreenBackLastPag(ps,0x8140);
			         ps->ZjTimer=0;
					 ps->staflg=0;
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x00003)  //����ֵ����
				{
                    if(ps->Button==ps->gzdselect)
					{  //�ٰ���ѡ���,�ٰ�OK��
						 ScreenTowOptChose(ps,ps->Button+0x20);	 //��������봦��
					     ps->Key.Opt=ps->Key.Mode-0x20;
					}
					else
					{

					ps->Key.Opt=ps->Button;	ps->Key.Mode=0;
					ps->RunModeStat=1;
					ps->gzdselect=ps->Button;
						
					}

					
				}	
				else if(ps->Button>=0x0021&&ps->Button<=0x00026)  //����ֵ����
				{
						 if(ps->Key.bOutsideKeyboard != true)
						 Screen_WriteRegisterOneData(0x4f,0xA0+(ps->Button&0x000f));//�����������					       
						 ScreenTowOptChose(ps,(ps->Button&0x000f));
                         ps->Key.Opt=ps->Button&0x000f;//	 ����ѡ��
                         if(ps->Key.Opt>3) ps->Key.Opt-=3;			
					
				}
			  else if(ps->Button==0x0004)  //����ֵ����
				{
					//to do ����
					ps->Key.Opt=ps->Button;		
				  ps->Key.Mode=0;
					ps->RunModeStat=1;
					if(ps->PauseOrStop==B0)
					{ ps->ZjTimer=0;
					  ps->staflg=0;
					  ps->PauseOrStop=0;
					  Flux_StopSampling('C');//ֹͣ����
					  ps->TimerSec=0;
					}else
					{  ps->ZjTimer=0;
					   ps->staflg=1;
					   ps->PauseOrStop=B0;
					   Flux_StarSampling('C',1);//��ʼ���� �� 1L/Min�ɼ�
					   ps->TimerSec=10;
					}
        			 
				}
				else if(ps->Button==0x0005)  //����ֵ����
				{ Slave_TPActualStructure * BeforGasp=&Sp->O2Concent;//����ǰ
				  // to do �Զ�����
				if(ps->gzdselect==1)   //ѡ����Ũ��
				{
					BeforGasp[ps->SubCnt].K=CalculationFormula_ZdK(ps->DisValue[0],ps->DisValue[3],BeforGasp[ps->SubCnt].B,BeforGasp[ps->SubCnt].K);

				  
				  SaveFlaotToFlash(BeforGasp[ps->SubCnt].K,FLASHIS_AdrWHJZ_NDJZ_BLO2_1+ps->SubCnt*2); 
				  
				}
				else if(ps->gzdselect==2)  //ѡ���Ũ��
				{
					
				 // BeforGasp[ps->SubCnt].K1=CalculationFormula_ZdK(ps->DisValue[1],ps->DisValue[4],BeforGasp[ps->SubCnt].B,BeforGasp[ps->SubCnt].K1);
				  if(ps->DisValue[3]!=0)
				  BeforGasp[ps->SubCnt].K1=ps->DisValue[1]/ps->DisValue[3];
				  else 
				  BeforGasp[ps->SubCnt].K1=1;
				  
				  SaveFlaotToFlash(BeforGasp[ps->SubCnt].K1,FLASHIS_AdrWHJZ_NDJZ_BLO2_2+ps->SubCnt*2);
				}
				else if(ps->gzdselect==3)  //ѡ���Ũ�ȡ�
				{
				//  BeforGasp[ps->SubCnt].K2=CalculationFormula_ZdK(ps->DisValue[2],ps->DisValue[5],BeforGasp[ps->SubCnt].B,BeforGasp[ps->SubCnt].K2);

				  if(ps->DisValue[3]!=0)
				  BeforGasp[ps->SubCnt].K2=ps->DisValue[2]/ps->DisValue[3];
				  else 
				  BeforGasp[ps->SubCnt].K2=1;
					
				  SaveFlaotToFlash(BeforGasp[ps->SubCnt].K2,FLASHIS_AdrWHJZ_NDJZ_BLO2_3+ps->SubCnt*2);
				}
				 pf->Flag=1;	
				 ps->Key.Opt=ps->Button;		
				 ps->Key.Mode=0;ps->RunModeStat=1;
				} 	
	ScreenChangeDataInit(ps);
	} 
}
//----2020-05-20
/*
	��������void Button_WHJZ_WHXZ_NDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-ѹ��У׼-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25,�޸ĳ�ʱ����, modify by alfred.
*/
void Button_WHJZ_WHXZ_YLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
//	static u8 uTempKeymode = 0;

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 

					if((ps->Key.Mode==2)||(ps->Key.Mode==3))
					{
					 ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_YLJZ_DQY+(ps->Key.Mode-1)*2,2,600,400);
						
					}
					else
                    {						
						
					ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_YLJZ_DQY+(ps->Key.Mode-1)*2,2,600,400);	
						
					}
					
					 Slave_ReadFlashZOData(&Slave_GetData()->DQPress,FLASHIS_AdrWHJZ_YLJZ_DQY,SLAVE_BaseCPress);
                     Slave_ReadFlashZOData(&Slave_GetData()->ACPress,FLASHIS_AdrWHJZ_YLJZ_DY,SLAVE_BaseCPress);
                     Slave_ReadFlashZOData(&Slave_GetData()->AGPress,FLASHIS_AdrWHJZ_YLJZ_JY,SLAVE_BaseCPress);
                     Slave_ReadFlashZOData(&Slave_GetData()->BCPress,FLASHIS_AdrWHJZ_YLJZ_CLY,SLAVE_BaseCPress);
                     Slave_ReadFlashZOData(&Slave_GetData()->BGPress,FLASHIS_AdrWHJZ_YLJZ_JJY,SLAVE_BaseCPress);
                     Slave_ReadFlashZOData(&Slave_GetData()->CCPress,FLASHIS_AdrWHJZ_YLJZ_QLY,SLAVE_BaseCPress);
					
					
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}		
			}
			else  //��Χ����
			{					
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
		
					if((ps->Key.Mode==2)||(ps->Key.Mode==3))
					{
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_YLJZ_DQY+(ps->Key.Mode-1)*2,2,600,400);	
					}
					else	
					{
						
					ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_YLJZ_DQY+(ps->Key.Mode-1)*2,2,600,400);	 //�����Ӿ����
						
					}
					
					 Slave_ReadFlashZOData(&Slave_GetData()->DQPress,FLASHIS_AdrWHJZ_YLJZ_DQY,SLAVE_BaseCPress); //����ʵ�����
                     Slave_ReadFlashZOData(&Slave_GetData()->ACPress,FLASHIS_AdrWHJZ_YLJZ_DY,SLAVE_BaseCPress);
                     Slave_ReadFlashZOData(&Slave_GetData()->AGPress,FLASHIS_AdrWHJZ_YLJZ_JY,SLAVE_BaseCPress);
                     Slave_ReadFlashZOData(&Slave_GetData()->BCPress,FLASHIS_AdrWHJZ_YLJZ_CLY,SLAVE_BaseCPress);
                     Slave_ReadFlashZOData(&Slave_GetData()->BGPress,FLASHIS_AdrWHJZ_YLJZ_JJY,SLAVE_BaseCPress);
                     Slave_ReadFlashZOData(&Slave_GetData()->CCPress,FLASHIS_AdrWHJZ_YLJZ_QLY,SLAVE_BaseCPress);

				}             							
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,2,6); //��ֵ����
			}
			break;			
		}
	}
		
	 { //��Ļ��������
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x8100);
			
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x00006)  //����ֵ����
				{
							if(ps->Key.bOutsideKeyboard != true)
							Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//�����������         
							ScreenTowOptChose(ps,ps->Button);	
					
				}	
			  else if(ps->Button==0x0007)  //����ֵ����
				{
					//to do ����ȷ��	 ��ӡ
					ps->Key.Opt=ps->Button;		
				 ps->Key.Mode=0;ps->RunModeStat=1;
					float buffer[10];
					buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
					for(int i=0;i<6;i++)
					{
					  buffer[i+1]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_YLJZ_DQY+i*2);//װ���������
					}
					PrintPressurePara(buffer);//��ӡѹ������
        			 
				}
				else if(ps->Button==0x0008)  //����ֵ����
				{
				  // to do У��
					Slave_Zero_Press();
					ps->Key.Opt=ps->Button;		
				  ps->Key.Mode=0;
					ps->RunModeStat=1;
				} 	
				
				
	ScreenChangeDataInit(ps);
	} 
}
//----
/*
	��������void Button_WHJZ_YQCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-����������ѡ��-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WHJZ_YQCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
	
	}
		
	 { //��Ļ��������
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x8100);
			
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x00007)  //����ֵ����
				{
         
						ps->Key.Opt=ps->Button;		
				        ps->Key.Mode=0;
					    if((pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ]&(1<<(ps->Button-1)))==0)
							{
							  pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ]|=(1<<(ps->Button-1));
							}else
							{
							 pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ]&=~(1<<(ps->Button-1));
							}
							ps->RunModeStat=1;
					    
				}	
		
				
				
	ScreenChangeDataInit(ps);
	} 
}





//----2020-05-20
/*
	��������void Button_WHJZ_WDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-�¶�У׼-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��	1. 2020-05-25,�޸ĳ�ʱ����, modify by alfred.
				2. �޸Ĳ˵�1~4������ʾʱ�ı��ɫ, 2020-05-29, modify by alfred.
				3. �������±���Bֵ���󱣴浽���½ṹ����������⡣
*/
void Button_WHJZ_WDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//ywTempPointY1:�����㣬ywTempPointY2������ֵ�㣻
	//static u8 uMenuPosition;	
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer);
					if((ps->Key.Mode>=1)&&(ps->Key.Mode<=4))
					{
						if(ps->Key.Mode == 1)
						{
							ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_WDJZ_LDGQ+(ps->Key.Mode-1)*2,2,100.0,0.2);
						}													
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_WDJZ_LDYW+(ps->Key.Mode-1)*2,2,100.0,0.2);
						Slave_ReadFlashZOData(&Slave_GetData()->HWTem,FLASHIS_AdrWHJZ_WDJZ_LDHW,SLAVE_BaseTem); //20200526
            Slave_ReadFlashZOData(&Slave_GetData()->GQTem,FLASHIS_AdrWHJZ_WDJZ_LDGQ,SLAVE_BaseTem);
            Slave_ReadFlashZOData(&Slave_GetData()->SQTem,FLASHIS_AdrWHJZ_WDJZ_LDSQ,SLAVE_BaseTem);
            Slave_ReadFlashZOData(&Slave_GetData()->YWTem,FLASHIS_AdrWHJZ_WDJZ_LDYW,SLAVE_BaseTem);
            Slave_ReadFlashZOData(&Slave_GetData()->JWTem,FLASHIS_AdrWHJZ_WDJZ_LDJW,SLAVE_BaseTem);
					}
					else if((ps->Key.Mode>=5)&&(ps->Key.Mode<=6))
					{
						if(ps->Key.Mode == 5)
							{
								ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_WDJZ_BLGQ+(ps->Key.Mode-5)*2,2,2.0,0.2);
							}													
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_WDJZ_BLYW+(ps->Key.Mode-5)*2,2,2.0,0.2);
							
						Slave_ReadFlashBLData(&Slave_GetData()->GQTem,FLASHIS_AdrWHJZ_WDJZ_BLGQ); //������ 20200526
						Slave_ReadFlashBLData(&Slave_GetData()->YWTem,FLASHIS_AdrWHJZ_WDJZ_BLYW);//���±���  //2020-05-30, modify by alfred.
						Slave_ReadFlashBLData(&Slave_GetData()->SQTem,FLASHIS_AdrWHJZ_WDJZ_BLSQ);//ʪ����
					}											
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}		
			}
			else
			{				
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{							
					if((ps->Key.Mode>=1)&&(ps->Key.Mode<=4))
					{
						if(ps->Key.Mode == 1)
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_WDJZ_LDGQ+(ps->Key.Mode-1)*2,2,100.0,0.2);											
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_WDJZ_LDYW+(ps->Key.Mode-1)*2,2,100.0,0.2);
						
						 Slave_ReadFlashZOData(&Slave_GetData()->HWTem,FLASHIS_AdrWHJZ_WDJZ_LDHW,SLAVE_BaseTem);  //20200526
                         Slave_ReadFlashZOData(&Slave_GetData()->GQTem,FLASHIS_AdrWHJZ_WDJZ_LDGQ,SLAVE_BaseTem);
                         Slave_ReadFlashZOData(&Slave_GetData()->SQTem,FLASHIS_AdrWHJZ_WDJZ_LDSQ,SLAVE_BaseTem);
                         Slave_ReadFlashZOData(&Slave_GetData()->YWTem,FLASHIS_AdrWHJZ_WDJZ_LDYW,SLAVE_BaseTem);
                         Slave_ReadFlashZOData(&Slave_GetData()->JWTem,FLASHIS_AdrWHJZ_WDJZ_LDJW,SLAVE_BaseTem);
					}
					if(ps->Key.Mode<=2)
					{
						ScreenTowOptChose(ps,ps->Key.Mode+4);
						ps->Key.Opt=ps->Key.Mode-4;//
						break;
					}														
					else if((ps->Key.Mode>=5)&&(ps->Key.Mode<=6))
					{
						if(ps->Key.Mode == 5)
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_WDJZ_BLGQ+(ps->Key.Mode-5)*2,2,2.0,0.2);												
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_WDJZ_BLYW+(ps->Key.Mode-5)*2,2,2.0,0.2);
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_WDJZ_BLGQ+(ps->Key.Mode-5)*2,2,2.0,0.2);												
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_WDJZ_BLYW+(ps->Key.Mode-5)*2,2,2.0,0.2);
						Slave_ReadFlashBLData(&Slave_GetData()->GQTem,FLASHIS_AdrWHJZ_WDJZ_BLGQ); //������  //20200526
						Slave_ReadFlashBLData(&Slave_GetData()->YWTem,FLASHIS_AdrWHJZ_WDJZ_BLYW);//���±���	//2020-05-30, modify by alfred.
						Slave_ReadFlashBLData(&Slave_GetData()->SQTem,FLASHIS_AdrWHJZ_WDJZ_BLSQ);//ʪ����	
					}	

				}						
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,2,6); //��ֵ����
			}
			break;			
		}			
	}
		
	{ //��Ļ��������
		ps->OldMode=ps->RunMode;
		if(ps->Button==0x0AFe||ps->Button==Max_Opt)
		{
			ScreenBackLastPag(ps,0x8100);
			ps->Warning = 0;	//��������
			ps->uMenuPosition = 0; //���λ������
		}
		else if(ps->Button>=0x0001&&ps->Button<=0x0004)  //����ֵ����
		{
			if(ps->Button==0x0001)
			{
				ps->uMenuPosition = 1;
				ps->Warning = 0;
			}
			else if(ps->Button==0x0002)
			{
				ps->Warning = 0;
				ps->uMenuPosition = 2;
			}
			ps->Key.Opt=ps->Button;		
			ps->Key.Mode=0;ps->RunModeStat=1;			
//			ScreenTowOptChose(ps,ps->Button);						
		}	
		else if(ps->Button>=0x0021&&ps->Button<=0x00024)  //����ֵ����
		{			
			if(ps->Key.bOutsideKeyboard != true)
				Screen_WriteRegisterOneData(0x4f,0xA0+(ps->Button&0x000f));//����������� 
			ScreenTowOptChose(ps,ps->Button&0x000f);
			ps->Warning = 0;
			ps->uMenuPosition = ps->Key.Opt;
			
		}	else if(ps->Button>=0x0031&&ps->Button<=0x00032)  //����ֵ����
		{
			if(ps->Key.bOutsideKeyboard != true)
				Screen_WriteRegisterOneData(0x4f,0xA0+(ps->Button&0x000f)+4);//�����������              
			ScreenTowOptChose(ps,(ps->Button&0x000f)+4);	
			ps->Key.Opt=ps->Button&0x000f;
			ps->Warning = 0;
			ps->uMenuPosition = ps->Key.Opt;
		}
		else if(ps->Button==0x0005)  //����ֵ����
		{
			//to do ����	 ��ӡ
			ps->Key.Opt=ps->Button;		
			ps->Key.Mode=0;ps->RunModeStat=1;
			float buffer[10];
			buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
			buffer[1]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_WDJZ_LDGQ);
			buffer[2]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_WDJZ_BLGQ);
			buffer[3]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_WDJZ_LDSQ);
			buffer[4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_WDJZ_BLSQ);
			buffer[5]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_WDJZ_LDJW);
			PrintTemperaturePara(buffer);//��ӡѹ������			 
		}
		else if(ps->Button==0x0006)  //����ֵ����
		{
						  // to do ȷ��
			ps->Key.Opt=ps->Button;		
			ps->Key.Mode=0;ps->RunModeStat=1;
			if(ps->Key.LastOpt == 0x01) ps->uMenuPosition = 1;

			TemperatureCaribration(&ps->Warning,ps->uMenuPosition);
			if(ps->Warning == 0xAF) 	//�궨���
			{	
				pf->Flag = 1;
				ps->RunModeStat=1;  //���¾�̬����, ������ȷ�ϻ򷵻�ʱps->RunModeStat=1����ҳ�����ݡ� 
				ps->Button=0XffFF;
			}
		} 									
	ScreenChangeDataInit(ps);
	} 
}





//----2020-05-20
/*
	��������void Button_WHJZ_DCBD_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-��ر궨-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25,�޸ĳ�ʱ����, modify by alfred.
*/
void Button_WHJZ_DCBD_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_DCJZ_BL+(ps->Key.Mode-1)*2,2,2.0,0.2);
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
					Battery_ReadFlashData();  //20200528
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
					
				}		
			}
			else
			{				
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_DCJZ_BL+(ps->Key.Mode-1)*2,2,2.0,0.2);
					    Battery_ReadFlashData();  //20200528
				}						
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,2,6); //��ֵ����
			}
		 break;			
		}		
	}
		
	 { //��Ļ��������
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x8100);
			
				}
				else if(ps->Button==0x0001)  //����ֵ����
				{
							if(ps->Key.bOutsideKeyboard != true)
										Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//�����������         
						 ScreenTowOptChose(ps,ps->Button);	
					
				}
				
	ScreenChangeDataInit(ps);
	} 
}
//----2020-05-21
/*
	��������void Button_WHJZ_CJWH_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-����ά��-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25,�޸ĳ�ʱ����, modify by alfred.
*/
void Button_WHJZ_CJWH_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		case 1:
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrCJWHYQBH+(ps->Key.Mode-1)*2,3,9999999999,1,CALC_NO_POINT,CALC_NO_SIGNS);
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
				ScreenConfirmEnter(ps,FLASHIS_AdrCJWHYQBH+(ps->Key.Mode-1)*2,3,9999999999,1);
				}						
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,0,10); //��ֵ����
			}
		 break;			
		}			
	}
		
	 { //��Ļ��������
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,52);
			
				}
				else if(ps->Button==0x0001)  //����ֵ����
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//�����������        
					ScreenTowOptChose(ps,ps->Button);	
					
				}else if(ps->Button==0x0002)  //ϵͳ����
				{
					ps->Key.Opt=ps->Button;
					Flashisp_Backup();         //ϵͳ����
					ps->Function=1;
					
				}else if(ps->Button==0x0003)  //ת�����崫��������
				{      
					ps->RunMode=0x8210;	
					ps->Key.Opt=ps->Button;
					ps->LastChoice=0xffff;
				}	
	      ScreenChangeDataInit(ps);
	} 
}


/******************  ϵͳ����ҳ��д������  ********************/
void SystemSetPageWriteDate(Screen_DataStructure *ps)
{
//						ps->Key.Mode=2;
						ps->Key.TwoOpt=0;
						u8 date[15];
						Screen_ResetKeyDataBuffer();//�������ֵ
						strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer);
				    ps->Clock.Clock.tm_year=Calculate_CharToU16((&ps->Key.Buffer[0]),4)-SCREEN_YearBase; //�޸�����
				    if(ps->Clock.Clock.tm_year>138) ps->Clock.Clock.tm_year = 138;
				    ps->Clock.Clock.tm_mon=Calculate_CharToU16((&ps->Key.Buffer[4]),2)-1;
				    if(ps->Clock.Clock.tm_mon >11)ps->Clock.Clock.tm_mon = 11;
				    ps->Clock.Clock.tm_mday=Calculate_CharToU16((&ps->Key.Buffer[6]),2);
				   if(ps->Clock.Clock.tm_mday >31)ps->Clock.Clock.tm_mday = 31;
				    Screen_SetClock(&(ps->Clock.Clock));
				    Screen_ResetKeyDataBuffer();//�������ֵ
				    sprintf((char*)date,"%02d:%02d:%02d",ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
						strcpy((char*)ps->Key.Buffer,(char*)date);
				    
//						ps->Button = 0x0002; //���������ں�ģ������ʱ������򣬵�������̼�������ʱ�䡣
						ps->Key.Mode=0;
						ps->RunModeStat=1;//ˢ������
						ps->Button=0xffff;
						ps->Timer=1000;				
					 
}

/******************  ϵͳ����ҳ��д��ʱ��  ********************/
void SystemSetPageWriteTime(Screen_DataStructure *ps)
{
						Screen_ResetKeyDataBuffer();//�������ֵ
						strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer);
						ps->Clock.Clock.tm_hour=Calculate_CharToU16((&ps->Key.Buffer[0]),2); //�޸�ʱ��
						ps->Clock.Clock.tm_min=Calculate_CharToU16((&ps->Key.Buffer[2]),2);
						ps->Clock.Clock.tm_sec=Calculate_CharToU16((&ps->Key.Buffer[4]),2);
						ps->Clock.Clock.tm_mon-=1;
						if(ps->Clock.Clock.tm_mon >11)ps->Clock.Clock.tm_mon = 11;
						Screen_SetClock(&(ps->Clock.Clock));
					 
						ps->Timer=1000;
						ps->Key.Mode=0;
						ps->RunModeStat=1;//ˢ������
						ps->Button=0xffff;
}



//----
/*
	��������void Button_WHJZ_CJWH_QTCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-����ά��-���崫����ѡ��-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��2020-05-25,�޸ĳ�ʱ����, modify by alfred.
*/
void Button_WHJZ_CJWH_QTCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
  FourBytes four;
	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		   //�����˰�װ����
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:		
			  //����������
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:		

		{//����������������	
			if(ps->Key.bOutsideKeyboard != true)
			{  //��Ļ����(����������Ļ����)
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 

					if(ps->Key.Mode<=7)  //���������������
					{
					pf->Data[FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2+1]=Calculate_CharToU16((&ps->Key.Buffer[0]),4);
					pf->Data[FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2]=(Calculate_CharToU16((&ps->Key.Buffer[4]),2)<<8)+Calculate_CharToU16((&ps->Key.Buffer[6]),2);
					}
					else                //���������������
					{
						ScreenConfirmEnter(ps,FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2,3,20000,0);
					}
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
					pf->Flag = 1;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}	
			}
			else
			{  	//�Ȼ�ȡ���̰���ֵ
				if(ps->Key.Mode<=7)  //�ڲ�ͬ�Ķ����˵���,������ʾ���ܲ�ͬ
				{
				ScreenTowOpButton(ps,1,9,1);  //�����÷����������Ҫ�����漸�����
				if(ps->Key.TwoOpt==4&&ps->Key.KeyDir==2) ps->Key.TwoOpt=5;
				else if(ps->Key.TwoOpt==4) ps->Key.TwoOpt=3;
					
				if(ps->Key.TwoOpt==7&&ps->Key.KeyDir==2) ps->Key.TwoOpt=8;
				else if(ps->Key.TwoOpt==7) ps->Key.TwoOpt=6;
				}
				else
				{
						ScreenTowOpButton(ps,1,9,0);
				}
				
				if(ps->Button==0x0Aff) //ȷ�ϰ���ֵ����
				{
						if(ps->Key.Mode<=7)  //���������������
					{
					pf->Data[FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2+1]=Calculate_CharToU16((&ps->Key.Buffer[0]),4);
					pf->Data[FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2]=(Calculate_CharToU16((&ps->Key.Buffer[5]),2)<<8)+Calculate_CharToU16((&ps->Key.Buffer[8]),2);
				  }
					else
					{
						ScreenConfirmEnter(ps,FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2,3,20000,0);
					}
				}										
				ScreenBackFirstOpt(ps);	       //�˳�����ֵ����
				if(ps->Key.number>=0x30&&ps->Key.number<=0x39) //���ְ���ֵ����
				{
					if(ps->Key.Mode<=7)
					{	ps->Key.Buffer[ps->Key.TwoOpt]=ps->Key.number;
					if(ps->Key.TwoOpt<9) {ps->Key.TwoOpt++; ps->Key.KeyDir=2;
					if(ps->Key.TwoOpt==4&&ps->Key.KeyDir==2) ps->Key.TwoOpt=5;
					if(ps->Key.TwoOpt==7&&ps->Key.KeyDir==2) ps->Key.TwoOpt=8;
					}
				  }
					else if(ps->Key.Mode==8)
					{  
							if(ps->Key.TwoOpt<2) {	ps->Key.Buffer[ps->Key.TwoOpt]=ps->Key.number;ps->Key.TwoOpt++;}
					}
					else if(ps->Key.Mode==9)
					{ 
							if(ps->Key.TwoOpt<4) {	ps->Key.Buffer[ps->Key.TwoOpt]=ps->Key.number;ps->Key.TwoOpt++;}
					}
					else if(ps->Key.Mode==10)
					{
								 if(ps->Key.TwoOpt<4) {	ps->Key.Buffer[ps->Key.TwoOpt]=ps->Key.number;ps->Key.TwoOpt++;}
					}
				  else if(ps->Key.Mode==11)
					{
						   if(ps->Key.TwoOpt<4) {	ps->Key.Buffer[ps->Key.TwoOpt]=ps->Key.number;ps->Key.TwoOpt++;}
					}
				  else if(ps->Key.Mode==12)
					{
						   if(ps->Key.TwoOpt<4) {	ps->Key.Buffer[ps->Key.TwoOpt]=ps->Key.number;ps->Key.TwoOpt++;}
					}
				  else if(ps->Key.Mode==13)
					{
							  if(ps->Key.TwoOpt<3) {	ps->Key.Buffer[ps->Key.TwoOpt]=ps->Key.number;ps->Key.TwoOpt++;}	 
				  }
				  else if(ps->Key.Mode==14)
 				  {
					      if(ps->Key.TwoOpt<2) {	ps->Key.Buffer[ps->Key.TwoOpt]=ps->Key.number;ps->Key.TwoOpt++;}
					}
			 }
			}			
			break;
		}		
	}
	{ //��Ļ��������
				ps->OldMode=ps->RunMode;
			switch(ps->Button)
			{		
				case 0x0AfE:
				case 0x008:   //����
					{
					ScreenBackLastPag(ps,0x8200);
							break;
					 }
				case 0x0009:    //����   2020-09-03  LIN
				case 0x000a:
				case 0x000b:
				case 0x000c:
				case 0x000d:
				case 0x000e:
				case 0x000f:
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button-8);//����������� 					
					ScreenTowOptChose(ps,(ps->Button-8));  //����������˵�:1~7
					ps->Key.Opt=ps->Button;  //������9~f
					sprintf((char*)ps->Key.Buffer,"%02d-%02d-%02d",pf->Data[FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2+1],pf->Data[FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2]>>8,pf->Data[FLASHIS_AdrO2AZSJ+(ps->Key.Mode)*2]&0x00FF);			
					break;
				}
				case 0x0001:    //ѡ��
				case 0x0002: 
                case 0x0003: 
                case 0x0004: 	
			    case 0x0005: 	
                case 0x0006: 
                case 0x0007: 					
				{
		      	 	ps->Key.Opt=ps->Button; //������1~7
				      ps->Key.Mode=0;  //һ���˵�,�޶����˵�
					    ps->RunModeStat=1;
					    u8 Chose=(pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ])>>8;
					    if((Chose&(1<<(ps->Key.Opt-1)))==0)
							{
							  Chose|=(1<<(ps->Key.Opt-1));
								//��ʱ˳���������˵�
								ScreenTowOptChose(ps,(ps->Button-1+8));  //�����˵���8~14
								ps->Key.Opt=ps->Button; //������1~7
							}else
							{
							 Chose&=~(1<<(ps->Key.Opt-1));
							}
							pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ]=(Chose<<8)+(pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ]&0x00ff);
					break;
				}
				case 0x0010:    //��������
				case 0x0011: 
                case 0x0012: 
                case 0x0013: 	
				case 0x0014: 	
                case 0x0015: 
        case 0x0016: 	
        {
	        if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,(0xA0+(ps->Button-0x10)+8));//����������� 					
					ScreenTowOptChose(ps,(ps->Button-0x10+8));  //�����˵���8~14
                    ps->Key.Opt=ps->Button-0x10+1;   //������1~7
					four.U16Data[0]=pf->Data[FLASHIS_AdrO2LQ+(ps->Button-0x10)*2];
					four.U16Data[0]=pf->Data[FLASHIS_AdrO2LQ+(ps->Button-0x10)*2+1];
					sprintf((char*)ps->Key.Buffer,"%05d",four.U32Data);	
					break;
				}					
				
				
		}
	} 
			ScreenChangeDataInit(ps);

}

/*  date 2020-5-13 hjl add
	��������void Button_KJ_MMSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	������	�ú���Ϊ����-��������-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼�� 2020-05-25, �޸Ľ��ճ�ʱ����, modify by alfred.
*/
void Button_KJ_MMSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
   	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	u16 PassWord;

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  	ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //�ײ㴦��			
			break;
		}
		
     	case 1:	
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
			
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ps->Key.Mode=0;
					ps->RunModeStat=1;//ˢ������
					ps->Button=0xffff;
				}
			
				if(ps->sRcvType !=0)
				{
					PassWord = (u16)atof((char*)&(uScreenRcvBuffer[0]));	
					ScreenBackLastOpt(ps);
					if(PassWord==GetKjMm())
					{
					  	ps->Button=3; //����ͨ��
					}
					else
					{
					   	ps->Button=4; //�������
					}
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
				}				
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //ȷ��
				{
					PassWord=(u16)atoi((char*)&(ps->Key.Buffer[0]));
					ScreenBackFirstOpt(ps);	       //�˳����
					if(PassWord==GetKjMm())
					{
					  	ps->Button=3; //����ͨ��
					}
					else
					{
					   	ps->Button=4; //�������
					}
					break;
				} 
				ScreenBackFirstOpt(ps);	       //�˳����
				ScreenEnterNumberText(ps,0,5); //��ֵ����
			}
			break;
	}
		
	{ //��Ļ��������
		ps->OldMode=ps->RunMode;
		if(ps->Button==0x0AFe||ps->Button==Max_Opt)
		{
			 ScreenBackLastPag(ps,101);
		}
		else if(ps->Button==0x0001)  //����ֵ����
		{
			if(ps->Key.bOutsideKeyboard != true)
				Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//����������� ���̿�ֵ�ֱ�Ϊ0XA1~0XA5						   
		   	ScreenTowOptChose(ps,ps->Button);			
		}
		else if(ps->Button==0x0003)  //����ֵ����
		{
			ps->RunMode=14;	//����������
			//˳��дSTM8 ���λΪ0
			Slave_GetData()->Stm8Data.rw_ctrl=1;
			Slave_GetData()->Stm8Data.Data=0;
		}
		else if(ps->Button==0x0004)  //����ֵ����
		{
		   	ps->Warning=1;	
		}
		ScreenChangeDataInit(ps);
	} 
}


