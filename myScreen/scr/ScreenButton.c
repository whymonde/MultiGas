/*
  本文件为屏幕按键处理文件
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
	函数名：void Buttom_MainPage(void)
	描述：	该函数为升级面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
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
	函数名：void Buttom_MainPage(void)
	描述：	该函数为主界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Buttom_MainPage(void)
{
	Screen_DataStructure *ps = Screen_GetData();
	switch(ButtonData.KeyData)
	{
		case BUTTON_DOWN:
		{
			if(ps->Key.Opt == 1)ps->Key.Opt = 5;  //主页总共有8个菜单图标，如果当前选中是第一项，按下“DOWN”那么切换到选中菜单5
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
			
			ps->Button = ps->Key.Opt;//赋值按键值
			break;
		}
	}
	ButtonData.KeyData = BUTTON_NONE;
}
//---
/*
	函数名：void Buttom_MainPage(void)
	描述：	该函数为掉电面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Buttom_PowerOffPage(void)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  	ScreenFirstOpButton(ps,2,1); //底层处理			
			break;
		}
	}
	//屏幕按键处理
	
		if(ps->Button==0x0001) //退出
			{ TwoBytes two;
					   ps->RunModeStat=1;
					   ps->RunMode=51;
				       Flashisp_GetData()->Data[FLASHIS_AdrCJMOD]=0;
				       Flux_GetData()->ChannelB.RunMode=0;
				
				      	two.U8Data[0] =Flux_GetData()->ChannelB.RunMode;
		               two.U8Data[1] = 0;
		               Flashisp_GetData()->Data[FLASHIS_AdrCJMOD] 	= two.U16Data; 			    //采样模式+状态标志位
			           Flashisp_GetData()->Flag=1;
				
				       pf->Flag=1;
			}
		else if(ps->Button==0x0002)//继续
			{
				      //to do 采集逻辑处理
				      if((Flashisp_GetData()->Data[FLASHIS_AdrCJMOD]==2)||(Flashisp_GetData()->Data[FLASHIS_AdrCJMOD]==3))//采样暂停中//采样中
					  {
						  
				       ps->RunMode=0x6200; //开始采样
		               ps->Function=0;
		               ps->Key.Opt=7;	
					   ps->RunModeStat=1;  
					   ps->YC_Pause=0;
					   if(Flashisp_GetData()->Data[FLASHIS_AdrCJMOD]==2)	  //启动电机
					   {
						if(pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]==1) //等速
						{
					      Flux_StarSampling('B',Flux_GetData()->ChannelA.ShowFlux); //目标流量
						}
				         else
				         {  
					       Flux_StarSampling('B',GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CYLS)); //目标流量
				         }  	
					   }
					   else
					   {
						ps->YC_Pause=1;     //暂停
					   }
						  
				      }
					  else if(Flashisp_GetData()->Data[FLASHIS_AdrCJMOD]==4) //防倒吸
					  {
						 Flux_GetData()->ChannelB.Sampling.DXTime=FLUX_FangDaoXiTimeB;   //60秒放倒吸
				         Flux_GetData()->ChannelB.Sampling.FdxFlg=0;
				         Flux_StarSampling('B',20);//防倒吸  20L/min  10s
						 ps->RunMode=0x6220;
			             ps->YC_Stat=0;
			             ScreenChangeDataInit(ps);//界面调整
					  }
					 Flux_GetData()->ChannelB.RunMode=Flashisp_GetData()->Data[FLASHIS_AdrCJMOD];

			}

		ScreenChangeDataInit(ps);
}

//---
/*
	函数名：void Button_SysSetPage(void)
	描述：	该函数为系统设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：	1. 2020-05-20 hjl 增加软键盘处理
				2. 2020-05-25 modify by alfred ,修改超时检测处理。
				3. 迪文屏修改了本页显示内容，相应修改了代码，增加和减少一些显示, modify by alfred, 2020.06.12.
*/

void Button_SysSetPage(void)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  	ScreenFirstOpButton(ps,12,1); //底层处理			
			break;
		}
		
		case 1:
		{//二级对象日期设置		
			if(ps->Key.bOutsideKeyboard != true)  //二极输入中（数据录入）屏幕输入数字（弹出按键面板的闪动输入）
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);//外围有按键输入处理（键控）子程序
			
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))   //启动了接收超时与定时器
				{     
					ScreenBackLastOpt(ps) ;  //返回或超市退出输入
				}
			
				if(ps->sRcvType !=0)
				{
					SystemSetPageWriteDate(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}				
			}
			else       ////二极输入中（数据录入）外部按键输入数字（没有弹出按键面板的闪动输入）
			{
				ScreenTowOpButton(ps,1,9,1);
				if(ps->Key.TwoOpt==4&&ps->Key.KeyDir==2)   //处理移位中有些不是数字的位，如“-”
					ps->Key.TwoOpt=5;
				else if(ps->Key.TwoOpt==4) 
					ps->Key.TwoOpt=3;
				
				if(ps->Key.TwoOpt==7&&ps->Key.KeyDir==2) 
					ps->Key.TwoOpt=8;
				else if(ps->Key.TwoOpt==7) 
					ps->Key.TwoOpt=6;	
				if(ps->Button==0x0Aff) //确定OK
				{    
					ps->Key.Mode=2;
					ps->Key.TwoOpt=0;
					u8 date[15];
					ps->Clock.Clock.tm_year=Calculate_CharToU16((&ps->Key.Buffer[0]),4)-SCREEN_YearBase; //修改日期
					if(ps->Clock.Clock.tm_year>138) 
						ps->Clock.Clock.tm_year = 138;
					ps->Clock.Clock.tm_mon=Calculate_CharToU16((&ps->Key.Buffer[5]),2)-1;
					if(ps->Clock.Clock.tm_mon >11)
						ps->Clock.Clock.tm_mon = 11;
					ps->Clock.Clock.tm_mday=Calculate_CharToU16((&ps->Key.Buffer[8]),2);
					if(ps->Clock.Clock.tm_mday >31)
						ps->Clock.Clock.tm_mday = 31;
					Screen_SetClock(&(ps->Clock.Clock));
					Screen_ResetKeyDataBuffer();//清除按键值
					sprintf((char*)date,"%02d:%02d:%02d",ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
					strcpy((char*)ps->Key.Buffer,(char*)date);

					ps->Timer=1000;
				}
				
				if(ps->Button==0x0Afe) //退回上一级
				{
					ps->Key.Mode=0;
					ps->RunModeStat=1;//刷新数据
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
		{//二级对象时间设置
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
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
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
				if(ps->Button==0x0Aff) //确定
				{
					ps->Clock.Clock.tm_hour=Calculate_CharToU16((&ps->Key.Buffer[0]),2); //修改时间
					ps->Clock.Clock.tm_min=Calculate_CharToU16((&ps->Key.Buffer[3]),2);
					ps->Clock.Clock.tm_sec=Calculate_CharToU16((&ps->Key.Buffer[6]),2);
					ps->Clock.Clock.tm_mon-=1;
					if(ps->Clock.Clock.tm_mon >11)
					ps->Clock.Clock.tm_mon = 11;
					Screen_SetClock(&(ps->Clock.Clock));
					ps->Timer=1000;

				}
				if(ps->Button==0x0Afe||ps->Button==0x0Aff) //退回上一级
				{
					ps->Key.Mode=0;
					ps->RunModeStat=1;//刷新数据
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
		
		case 3:  //大气压
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
					if(four.FloatData<(float)50.0) four.FloatData=(float)50.0; //限制 大于50Kp
					if(four.FloatData>(float)130.0) four.FloatData=(float)130.0; //限制 小于于130Kp 
					SaveFlaotToFlash(four.FloatData,FLASHIS_AdrXTSZDQY);		
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff)
				{
					FourBytes four;
					four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));
					if(four.FloatData<(float)50.0) four.FloatData=(float)50.0; //限制 大于50Kp
					if(four.FloatData>(float)130.0) four.FloatData=(float)130.0; //限制 小于于130Kp
					SaveFlaotToFlash(four.FloatData,FLASHIS_AdrXTSZDQY);				
				}
				if(ps->Button==0x0Afe||ps->Button==0x0Aff) //退回上一级
				{
					ps->Key.Mode=0;
					ps->RunModeStat=1;//刷新数据
					ps->Button=0xffff;
				}
				ScreenEnterNumberText(ps,2,6); //数值输入
				/*
				if((ps->Key.number>=0x30&&ps->Key.number<=0x39)||ps->Key.number==0x2e)
				{
					ps->Key.Buffer[ps->Key.TwoOpt++]=ps->Key.number;
					if(ps->Key.TwoOpt>7) ps->Key.TwoOpt=7;//限制输入长度
				}
				*/
				
			}
			break;
		}
		case 5:   //二极菜单只要检测有没有取消，与Ok按键，并发按键上去。
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
		{//二级对象时间间隔
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
						pf->Data[FLASHIS_AdrXTSZZDPSJG]=two.U16Data; //保存排水间隔值
					else if( ps->Key.Mode==8)
						pf->Data[FLASHIS_AdrXTSZZDPSSC]=two.U16Data; //保存排水时长值
					else if( ps->Key.Mode==10)
					{   if(two.U16Data<=0)two.U16Data=0;
						pf->Data[FLASHIS_AdrXTSZCHSLSJ]=two.U16Data; //保存含湿量时长值		
					}						
					pf->Flag=1;		
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
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
						pf->Data[FLASHIS_AdrXTSZZDPSJG]=two.U16Data; //保存排水间隔值
					else if( ps->Key.Mode==8)
						pf->Data[FLASHIS_AdrXTSZZDPSSC]=two.U16Data; //保存排水时长值
					else if( ps->Key.Mode==10)
						{   if(two.U16Data<=0)two.U16Data=1;
					pf->Data[FLASHIS_AdrXTSZCHSLSJ]=two.U16Data; //保存含湿量时长值	
						}
					pf->Flag=1;
	

				}		
				ScreenBackFirstOpt(ps);
				
				ScreenEnterNumberText(ps,2,2); //数值输入
				/*
				if((ps->Key.number>=0x30&&ps->Key.number<=0x39)||ps->Key.number==0x2e)
				{
					ps->Key.Buffer[ps->Key.TwoOpt++]=ps->Key.number;
					if(ps->Key.TwoOpt>4) ps->Key.TwoOpt=4;//限制输入长度		
				}
				*/
			}
			break;
		}
	
	}
	
	{ //屏幕按键处理
		ps->OldMode=ps->RunMode;
		switch(ps->Button)
		{		
			case 0x0AfE:
			case 0x000D:   //返回
			{
				ps->Key.Opt=ps->Key.OldOpt;
				ps->Key.LastOpt=0;
				ps->RunMode =52; //返回主页面
				ps->RunModeStat=1;//更新静态数据
				break;
			}
			
			case 0x0001:    //日期(屏幕按下修改日期)
			{
				if(ps->Key.bOutsideKeyboard != true)
				{
					Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘 键盘控值分别为0XA1~0XA5
					ScreenTowOptChose(ps,ps->Button);		//二级对象选择闪烁输入	
				}	
				else
				{
					Screen_ResetKeyDataBuffer();//清除按键值
					ps->Key.Mode=1;
					ps->Key.TwoOpt=0;
					ps->Key.Opt=1;
					u8 date[15];
					sprintf((char*)date,"%02d-%02d-%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday);
					strcpy((char*)ps->Key.Buffer,(char*)date);
					ps->RunModeStat=1;//刷新数据
				}
				break;
			}
			
			case 0x0002:    //时间
			{
				if(ps->Key.bOutsideKeyboard != true)
				{
					Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘 键盘控值分别为0XA1~0XA5
					ScreenTowOptChose(ps,ps->Button);		//二级对象选择闪烁输入
					ps->Key.Opt=1;							//时间和日期都是每一个菜单被选中
				}	
				else
				{
					Screen_ResetKeyDataBuffer();//清除按键值
					ps->Key.Mode=2;
					ps->Key.TwoOpt=0;
					ps->Key.Opt=1;
					u8 Time[15];
					sprintf((char*)Time,"%02d-%02d-%02d",ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
					strcpy((char*)ps->Key.Buffer,(char*)Time);
					ps->RunModeStat=1;//刷新数据
				}
				break;
			}
			
			case 0x0003:    //大气压			
			{
				if((pf->Data[FLASHIS_AdrXTSZBOX]&(1>>0))==0x0000)
				{
					TwoBytesBitFunction(pf->Data[FLASHIS_AdrXTSZBOX],0,1);
					if(ps->Key.bOutsideKeyboard != true)
					{
						
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘 键盘控值分别为0XA1~0XA5
					}	
					else
					{
						TwoBytesBitFunction(pf->Data[FLASHIS_AdrXTSZBOX],0,1);
					}
					ScreenTowOptChose(ps,ps->Button);		//二级对象选择闪烁输入	
				}else
				{
					TwoBytesBitFunction(pf->Data[FLASHIS_AdrXTSZBOX],0,0);
					ps->Key.Mode=0;
				}
				pf->Flag=1;
				ps->Key.Opt=2;
				ScreenSetJitu(ps,0);	
				ps->RunModeStat=1;//刷新数据
				break;
			}
			
//				case 0x0A06:    //类型	
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

			case 0x0004:    //倒吸
			{ 
				ps->Key.Opt=3;
				Set_SysCheckBox(FLASHIS_AdrXTSZBOX,1);	
				pf->Flag=1;
				ps->Key.Mode=0;
				ScreenSetJitu(ps,2);	
				ps->RunModeStat=1;//刷新数据
				break;
			}	
			
			case 0x0005:    //蓝牙开关
			{  
				ps->Key.Opt=4;
				Set_SysCheckBox(FLASHIS_AdrXTSZBOX,2);
				if(pf->Data[FLASHIS_AdrXTSZBOX]&(1<<2))
					AT_Power_On();
				else AT_Power_Off();
				pf->Flag=1;
				ps->Key.Mode=0;
				ScreenSetJitu(ps,3);	
				ps->RunModeStat=1;//刷新数据
				break;
			}	
			
			case 0x0006:   
				ps->Key.Opt=5; 
				ps->Key.Mode=0; 	
				ps->RunModeStat=1;//刷新数据
			    Screen_WriteRegisterOneData(0x4f,0xA0+0X0C);//弹出软件键盘 键盘控值分别为0XA1~0XA5
				ScreenTowOptChose(ps,ps->Button-1);		//二级对象选择闪烁输入	
			    ps->Key.bOutsideKeyboard=0;
				break;//采样地址
				
			case 0x0007:    //自动排水开关
			{
				Set_SysCheckBox(FLASHIS_AdrXTSZBOX,3);
				pf->Flag=1;
				if(pf->Data[FLASHIS_AdrXTSZBOX ]&0x08)  //判断是否选择自动排水20200701
				Screen_WaterPumOpenServo();	
				else
				Screen_WaterPumCloseServo();
				ps->Key.Opt=6;
				ps->Key.Mode=0;
				ScreenSetJitu(ps,4);
				ps->RunModeStat=1;//刷新数据
				break;
			}	
			
			case 0x000A:    //采样类型
			{
				Set_SysCheckBox(FLASHIS_AdrXTSZBOX,4);	
				pf->Flag=1;
				ps->Key.Opt=9;
				ps->Key.Mode=0;
				ScreenSetJitu(ps,9);
				ps->RunModeStat=1;//刷新数据
				break;
			}
			
			case 0x000C:    //烟尘采样流量
			{
				Set_SysCheckBox(FLASHIS_AdrXTSZBOX,6);	//选择置标注
				pf->Flag=1;
				ps->Key.Opt=0X0B;
				ps->Key.Mode=0;
				ScreenSetJitu(ps,7);   //更改打钩基图
				ps->RunModeStat=1;//刷新数据
				break;
			}	
			
			case 0x0008:    //排水间隔
			case 0x0009:    //排水时长
			case 0x000B:
			{  
				if(ps->Key.bOutsideKeyboard != true)
				{
					Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘 键盘控值分别为0XA1~0XA5
					ScreenTowOptChose(ps,ps->Button - 1);		//二级对象选择闪烁输入	
				}	
				else
				{
					ps->Key.Opt=ps->Button-1;
					ScreenTowOptChose(ps,ps->Key.Opt);
					//Screen_WriteRegisterOneData(0x4f,0xA8);//弹出软件键盘 2020-05-19 hjl del
				}
				break;
			}	

		}
	} 
			ScreenChangeDataInit(ps);
	
}
//---
/*
	函数名：void Button_Sys_CYSZPage(void)
	描述：	该函数为系统设置-采样设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
/*
void Button_Sys_CYSZPage(void)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,7,1); //底层处理			
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
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnter(ps,FLASHIS_AdrXTSZYQCLSJ,1,30,1);	
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置					
				}
			}	
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //确认
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrXTSZYQCLSJ,1,30,1);
					Screen_ResetKeyDataBuffer();//清除按键值
					ps->Key.Mode=4;
					ps->Key.TwoOpt=0;	
					ps->RunModeStat=1;//更新静态数据				
				}
				else
				{
					ScreenBackFirstOpt(ps);	       //退出检测
				}			    	
				ScreenEnterNumberText(ps,0,2); //数值输入
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
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnter(ps,FLASHIS_AdrXTSZYQCLCS,1,60,1);		//计平均次数最大限制不超过60条		
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				 }
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置					
				}
			}			
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //确认
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrXTSZYQCLCS,1,60,1);			
				}				
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,0,2); //数值输入
			}	
		}break;
		case 5:
		{//二级对象时间间隔
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}		
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnter(ps,FLASHIS_AdrXTSZCHSLSJ,1,100,1);		
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				 }
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置					
				}
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //确认
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrXTSZCHSLSJ,1,100,1);			
				}				
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,0,2); //数值输入
			}
			break;
		}		
	}
	 		{ //屏幕按键处理
				ps->OldMode=ps->RunMode;
				switch(ps->Button)
			{		
				case 0x0AfE:
				case 0x0008:   ScreenBackLastPag(ps,0x1000);break;//返回
				case 0x0007:   //返回主页
					{
						ps->Key.Opt=ps->Key.OldOpt;
						ps->Key.LastOpt=0;
						ps->RunMode =52; //返回主页面
						ps->RunModeStat=1;//更新静态数据
							break;
					 }
				case 0x0001:    //类型
				{
						Set_SysCheckBox(FLASHIS_AdrXTSZBOX,4);	pf->Flag=1;
						ps->Key.Opt=1;
						ps->Key.Mode=0;
						ScreenSetJitu(ps,9);
		      }break;
				case 0x0002:    //单位
				{
					Set_SysCheckBox(FLASHIS_AdrXTSZBOX,5);	pf->Flag=1;
					ps->Key.Opt=2;
					ps->Key.Mode=0;
					ScreenSetJitu(ps,6);
					break;
				}
				case 0x0003:    //烟气时间		
				{
					ScreenTowOptChose(ps,3);
										if(ps->Key.bOutsideKeyboard != true)
						{
								Screen_WriteRegisterOneData(0x4f,0xA1);//弹出软件键盘
						}
					break;
				}				
				case 0x0004:    //次数
				{ 
					  ScreenTowOptChose(ps,4);
					  ps->Key.Opt=3;
					  										if(ps->Key.bOutsideKeyboard != true)
						{
								Screen_WriteRegisterOneData(0x4f,0xA2);//弹出软件键盘
						}
					break;
				}	
				case 0x0005:    //含湿量
				{ 
					ScreenTowOptChose(ps,5);
					 ps->Key.Opt=4;
					if(ps->Key.bOutsideKeyboard != true)
						{
								Screen_WriteRegisterOneData(0x4f,0xA3);//弹出软件键盘
						}
					break;
				}	
				case 0x0006:    //流量
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
	函数名：void Button_SysSetPage(void)
	描述：	该函数为烟道布点设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/

void Button_FlueSetPage(void)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{	
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,5,1); //底层处理			
			break;
		}
	}	
	{ //屏幕按键处理//屏幕按键处理
		  ps->OldMode=ps->RunMode;
				switch(ps->Button)
			{
				case 0x0AFe:
				case 0x0005:   //返回
					{
						ps->Key.Opt=ps->Key.OldOpt;
						ps->Key.LastOpt=0;
						ps->RunMode =51; //返回主页面
						ps->RunModeStat=1;//更新静态数据
						pf->Flag=1;     //20200522
							break;
					 }
				case 0x0001:    //进入圆形
				{
					ps->RunMode =0x2100; 
					pf->Data[FLASHIS_AdrYDBDLX]=1;
					ps->Key.Opt=1;
					pf->Flag=1;     //20200527
					break;
				}
				case 0x0002:    //进入方形
				{

					ps->RunMode =0x2200; 
					pf->Data[FLASHIS_AdrYDBDLX]=2;
					ps->Key.Opt=2;
					pf->Flag=1;     //20200527
					break;
				}
				case 0x0003:    ////进入其他类型
				{
					ps->RunMode =0x2300; 
                    pf->Data[FLASHIS_AdrYDBDLX]=3;
					ps->Key.Opt=3;
					pf->Flag=1;     //20200527
					break;
				}
				case 0x0004:    //烟道查询
				{
					ps->RunMode =0x2400; 

					ps->Key.Opt=4;
					SdFile_Serch_MaxofFlueLinkFile();  //搜索最大文件号20200527
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
	函数名：void Button_FlueCircularPage(void)
	描述：	该函数为烟道布点-圆形设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
    修改记录： 2020-05-25 modify by alfred,  修改超时处理。
*/

void Button_FlueCircularPage(void)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	
	// u16 Single_hole_Number=1;

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{	
		case 0:
		{//没有选中二级对象
			ScreenFirstOpButton(ps,8,1); //底层处理	
			ps->Key.LastMode=0;			
			break;
		}			
		case 1:
		{//二级对象内径
			
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBDYXNJ,2,50.0,0.01,CALC_HAVE_POINT,CALC_NO_SIGNS);	
	
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //确认
				{	
					ScreenConfirmEnter(ps,FLASHIS_AdrYDBDYXNJ,2,50.0,0.01);
		
					ps->Key.LastMode= ps->Key.Mode;	

				}      
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,2,5); //数值输入
			}
			break;
		}
		case 2:
		{//二级对象面积
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBD_MJ,2,2500.0,0.0001,CALC_HAVE_POINT,CALC_NO_SIGNS);		
                				
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //确认
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrYDBD_MJ,2,2500.0,0.0001);
					ps->Key.LastMode= ps->Key.Mode;	

					
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,2,9); //数值输入
			}
			break;
		}
		case 3:
		{ //二级对象管套
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBD_GT,2,10.0,0.01,CALC_HAVE_POINT,CALC_NO_SIGNS);				
			}			
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //确认
				{				 
				ScreenConfirmEnter(ps,FLASHIS_AdrYDBD_GT,2,10.0,0.01);
				ps->Key.LastMode= ps->Key.Mode;					
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,2,5); //数值输入
			}
			break;
		}
		case 4:
		{ //二级对象环数
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBDYXHS,1,5,0,CALC_NO_POINT,CALC_NO_SIGNS);		
             	
			}	
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //确认
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrYDBDYXHS,1,5,0);
					
					ps->Key.LastMode= ps->Key.Mode;	
                 
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,0,1); //数值输入
			}
			break;
		}
		case 5:
		{ //二级对象孔数
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBDYXKS,1,10,1,CALC_NO_POINT,CALC_NO_SIGNS);				
			}	
			else
			{
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //确认
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrYDBDYXKS,1,10,1);			
					ps->Key.LastMode= ps->Key.Mode;				
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,0,2); //数值输入
			}
			break;
		}	
		case 6:
		{  //二级对象地址
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
	 { //屏幕按键处理
		switch(ps->Button)
		{				
			case 0x0AfE:

			case 0x0008:   //返回上一页 
			{
				ScreenBackLastPag(ps,0x2000);
				break;
			}
			case 0x0006:   
			{
				// to do 地址输入
				ps->Key.Opt=6; 
				ps->Key.Mode=0; 	
				ps->RunModeStat=1;//刷新数据
			    Screen_WriteRegisterOneData(0x4f,0xA0+0X0C);//弹出软件键盘 键盘控值分别为0XA1~0XA5
				ScreenTowOptChose(ps,ps->Button);		//二级对象选择闪烁输入	
			    ps->Key.bOutsideKeyboard=0;

				break;
			}
			case 0x0007:  
			{
				// to do 保存圆形烟道数据
				pf->Flag=1;
				float saveData[21];
				memset(saveData,0,sizeof(saveData));
				saveData[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_MJ);  //面积
				saveData[1]=1;                                               //类型
				saveData[2]=(float)pf->Data[FLASHIS_AdrYDBDYXKS];                   //圆形孔数，修正孔数数量, modify by alfred, 2020.07.03
				saveData[3]=(float)pf->Data[FLASHIS_AdrYDBD_CYDS];                  //点数
				for(int i=0;i< pf->Data[FLASHIS_AdrYDBDYXHS]*2;i++)	// modify by alfred, 2020.07.15
				{
					saveData[i+4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDCDJL1+i*2);
				}
				saveData[15]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDYXNJ);   //内径D
				saveData[16]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT);    //套管L
				saveData[17]=(float)pf->Data[FLASHIS_AdrYDBDYXHS];   //还数
				saveData[18]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBA);   //边A长
				saveData[19]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBB);   //边B长
				saveData[20]=(float)pf->Data[FLASHIS_AdrYDBDFXDKCD]; //单孔测点

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
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘 键盘控值分别为0XA1~0XA5
					}							
						ScreenTowOptChose(ps,ps->Button);		//二级对象选择闪烁输入	
				}
			}	
			break;				
		}
	} 
}
//---
/*20200527
	函数名：void Button_FlueSeekPage(void)
	描述：	该函数为烟道布点-查找-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25 modify by alfred,修改超时处理。
*/

void Button_FlueSeekPage(float *FileCode)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,5,1); //底层处理			
			break;
		}			
		case 1:
		{//烟道查询--文件号
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInputFileName(ps,FileCode,0,9999999,1,CALC_NO_POINT,CALC_NO_SIGNS);				
			}	
			else
			{				
				ScreenTowOpButton(ps,1,0,0);
				if(ps->Button==0x0Aff) //确认
				{	
					ScreenConfirmEnterDisBuffer(ps,FileCode,0,9999999,1);
				}      
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,0,7); //数值输入
			}
			break;
		}		
	}
	 { //屏幕按键处理
				switch(ps->Button)
			{				
				case 0x0AfE:
				
				case 0x0005:   //返回上一页 
					{
					ScreenBackLastPag(ps,0x2000);
//					ps->Key.Opt = 0;
							break;
					}
						case 0x0002:   //上寻
					{
						if(*FileCode>1)
							(*FileCode)--;
					    ReadFlueData(*FileCode);ps->Key.Mode=0; ps->Key.Opt= ps->Button;	
							break;
					 }
							case 0x0003:   //下寻
					{
						if(*FileCode<SdFile_GetData()->FlueLinkFileMax)
							(*FileCode)++;
					    ReadFlueData(*FileCode);ps->Key.Mode=0; ps->Key.Opt= ps->Button;	
							break;
					}
          			case 0x0004:   //采用
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
								Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘 键盘控值分别为0XA1~0XA5
								ScreenTowOptChose(ps,ps->Button);		//二级对象选择闪烁输入	
							}	
           if(ps->Button<=5&&ps->Button>0) ps->Key.Opt= ps->Button;					
				}	
					break;				
		}
	} 
}
//---
/*20200527
	函数名：void Button_FlueSquarePage(void)
	描述：	该函数为烟道布点-矩形设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25 modify by alfred,修改超时处理。
*/

void Button_FlueSquarePage(void)
{
  Screen_DataStructure *ps = Screen_GetData();
  Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,9,1); //底层处理			
			break;
		}
			
		case 1:	
		{//二级对象孔边A
				if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBDFXBA,2,50.0,0.01,CALC_HAVE_POINT,CALC_NO_SIGNS);				
							}	
				else{			
								ScreenTowOpButton(ps,1,0,0);
								if(ps->Button==0x0Aff) //确认
										{
												ScreenConfirmEnter(ps,FLASHIS_AdrYDBDFXBA,2,50.0,0.01);

											    ps->Key.LastMode= ps->Key.Mode;		
										} 
								ScreenBackFirstOpt(ps);	       //退出检测
								ScreenEnterNumberText(ps,2,5); //数值输入
						}
			break;
	  }
		case 2:
		{//二级对象孔边B
				if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBDFXBB,2,50.0,0.01,CALC_HAVE_POINT,CALC_NO_SIGNS);				
							}	
				else{
								ScreenTowOpButton(ps,1,0,0);
								if(ps->Button==0x0Aff) //确认
										{	
												ScreenConfirmEnter(ps,FLASHIS_AdrYDBDFXBB,2,50.0,0.01);ps->Key.LastMode= ps->Key.Mode;		
										} 
								ScreenBackFirstOpt(ps);	       //退出检测
								ScreenEnterNumberText(ps,2,5); //数值输入
						}
			break;
	  }
		case 3:
		{//面积 
				if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBD_MJ,2,2500.0,0.0001,CALC_HAVE_POINT,CALC_NO_SIGNS);				
							}
				else{							
								ScreenTowOpButton(ps,1,0,0);
								if(ps->Button==0x0Aff) //确认
										{
												ScreenConfirmEnter(ps,FLASHIS_AdrYDBD_MJ,2,2500.0,0.0001);ps->Key.LastMode= ps->Key.Mode;						
										} 
								ScreenBackFirstOpt(ps);	       //退出检测
								ScreenEnterNumberText(ps,2,9); //数值输入
						}
			break;
		}
		case 4:
		{ //套管
				if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBD_GT,2,10.0,0.01,CALC_HAVE_POINT,CALC_NO_SIGNS);				
							}
				else{							
								ScreenTowOpButton(ps,1,0,0);
								if(ps->Button==0x0Aff) //确认
										{
												ScreenConfirmEnter(ps,FLASHIS_AdrYDBD_GT,2,10.0,0.01);ps->Key.LastMode= ps->Key.Mode;						
										} 
								ScreenBackFirstOpt(ps);	       //退出检测
								ScreenEnterNumberText(ps,2,5); //数值输入
						}
			break;
		}
		case 5:
		{ //孔数
			if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBDFXKS,1,10,1,CALC_HAVE_POINT,CALC_NO_SIGNS);				
							}
			else{
								ScreenTowOpButton(ps,1,0,0);
								if(ps->Button==0x0Aff) //确认
											{
													ScreenConfirmEnter(ps,FLASHIS_AdrYDBDFXKS,1,10,1);ps->Key.LastMode= ps->Key.Mode;					
											} 
								ScreenBackFirstOpt(ps);	       //退出检测
								ScreenEnterNumberText(ps,0,2); //数值输入
						}
			break;
		}
			case 6:
		{//单孔测点
			if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBDFXDKCD,1,10,1,CALC_HAVE_POINT,CALC_NO_SIGNS);				
							}			
			else{
								ScreenTowOpButton(ps,1,0,0);
								if(ps->Button==0x0Aff) //确认
											{
												ScreenConfirmEnter(ps,FLASHIS_AdrYDBDFXDKCD,1,10,1);ps->Key.LastMode= ps->Key.Mode;						
											} 
								ScreenBackFirstOpt(ps);	       //退出检测
								ScreenEnterNumberText(ps,0,2); //数值输入
				}
			break;
		}	
			case 7:
			{
              //二级对象地址
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
	
	 { //屏幕按键处理
				switch(ps->Button)
			{
				
				case 0x0AfE:
				
				case 0x0009:   //返回上一页
					{
					ScreenBackLastPag(ps,0x2000);
							break;
					 }
						case 0x0007:   //返回上一页
					{
					 // to do 地址输入
					   ps->Key.Opt=7; 
				       ps->Key.Mode=0; 	
				       ps->RunModeStat=1;//刷新数据
			           Screen_WriteRegisterOneData(0x4f,0xA0+0X0C);//弹出软件键盘 键盘控值分别为0XA1~0XA5
				       ScreenTowOptChose(ps,ps->Button);		//二级对象选择闪烁输入	
			           ps->Key.bOutsideKeyboard=0;

					   break;
					 }
					case 0x0008:   
					{
					 // to do 保存方向烟道数据

						float saveData[21];
						memset(saveData,0,sizeof(saveData));
						saveData[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_MJ);  //面积
						saveData[1]=2;                                               //类型
						saveData[2]=(float)pf->Data[FLASHIS_AdrYDBDFXKS];                   //孔数
						saveData[3]=(float)pf->Data[FLASHIS_AdrYDBD_CYDS];                  //点数
						for(int i=0;i<pf->Data[FLASHIS_AdrYDBDFXDKCD];i++)
						{
								saveData[i+4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDCDJL1+i*2);
						}
						saveData[15]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDYXNJ);   //内径D
						saveData[16]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT);    //套管L
						saveData[17]=(float)pf->Data[FLASHIS_AdrYDBDYXHS];   //还数
						saveData[18]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBA);   //边A长
						saveData[19]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBB);   //边B长
						saveData[20]=(float)pf->Data[FLASHIS_AdrYDBDFXDKCD]; //单孔测点

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
									Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘 键盘控值分别为0XA1~0XA5
							ScreenTowOptChose(ps,ps->Button);		//二级对象选择闪烁输入	

						}			
				}
				break;
				
		}
	} 
	//ScreenButtonReset();
}
/*
	函数名：void Button_FlueOtherPage(void)
	描述：	该函数为烟道布点-其他界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25 modify by alfred,修改超时处理。
*/

void Button_FlueOtherPage(void)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,5,1); //底层处理			
			break;
		}	
		case 1:
		{//二级对象烟道截面
				if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBD_MJ,2,100.0,0.01,CALC_HAVE_POINT,CALC_NO_SIGNS);				
							}	
				else{							
								ScreenTowOpButton(ps,1,0,0);  //面积
								if(ps->Button==0x0Aff) //确认
										{
												ScreenConfirmEnter(ps,FLASHIS_AdrYDBD_MJ,2,100.0,0.01);	
										} 
								ScreenBackFirstOpt(ps);	       //退出检测
								ScreenEnterNumberText(ps,2,8); //数值输入
						}
			break;
	  }
		case 2:
		{//采样点数
				if(ps->Key.bOutsideKeyboard != true)
							{
								ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBD_CYDS,1,10,1,CALC_NO_POINT,CALC_NO_SIGNS);				
							}	
				else{				
								ScreenTowOpButton(ps,1,0,0);//采样点数
								if(ps->Button==0x0Aff) //确认
										{				
												ScreenConfirmEnter(ps,FLASHIS_AdrYDBD_CYDS,1,10,1);
										} 
								ScreenBackFirstOpt(ps);	       //退出检测
								ScreenEnterNumberText(ps,2,2); //数值输入
						}
			break;
	  }	
		case 3:
        {
			//二级对象地址
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
	
	{ //屏幕按键处理
				switch(ps->Button)
			{
				
				case 0x0AfE:
				
				case 0x0005:   //返回上一页
					{
					ScreenBackLastPag(ps,0x2000);
							break;
					 }
						case 0x0003:   //返回上一页
					{
					 // to do 地址输入
					  ps->Key.Opt=3; 
				      ps->Key.Mode=0; 	
				      ps->RunModeStat=1;//刷新数据
			          Screen_WriteRegisterOneData(0x4f,0xA0+0X0C);//弹出软件键盘 键盘控值分别为0XA1~0XA5
				      ScreenTowOptChose(ps,ps->Button);		//二级对象选择闪烁输入	
			          ps->Key.bOutsideKeyboard=0;

					  break;
					 }
					case 0x0004:   //返回上一页
					{
						// to do 保存其他烟道数据
						pf->Flag=1;
						float saveData[21];
						memset(saveData,0,sizeof(saveData));
						saveData[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_MJ);  //面积
						saveData[1]=3;                                               //类型
						saveData[2]=(float)pf->Data[FLASHIS_AdrYDBDFXKS];                   //孔数
						saveData[3]=(float)pf->Data[FLASHIS_AdrYDBD_CYDS];                  //点数
						for(int i=0;i<pf->Data[FLASHIS_AdrYDBDYXHS];i++)
						{
							//saveData[i+4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDCDJL1+i*2);	//其它烟道数据暂不明确，先置0，modify by alfred, 2020.07.15
						}
						saveData[15]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDYXNJ);   //内径D
						saveData[16]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT);    //套管L
						saveData[17]=(float)pf->Data[FLASHIS_AdrYDBDYXHS];   //还数
						saveData[18]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBA);   //边A长
						saveData[19]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBB);   //边B长
						saveData[20]=(float)pf->Data[FLASHIS_AdrYDBDFXDKCD]; //单孔测点

						SaveFlueData(saveData);
						ps->Key.Mode=0; ps->Key.Opt= ps->Button;
						break;
					 }
				default :
					{
						if(ps->Button>0&&ps->Button<=2)
								{
									if(ps->Key.bOutsideKeyboard != true)
												Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘 键盘控值分别为0XA1~0XA5
									ScreenTowOptChose(ps,ps->Button);		//二级对象选择闪烁输入	
								}			
					}
					break;
		}
	} 
	//ScreenButtonReset();
}
//--
/*
	函数名：void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为工况预测设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
  //二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
		
	}	
	 { //屏幕按键处理
		   ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
				  ScreenBackLastPag(ps,51);
				}
				else if(ps->Button!=0xffff)  //进入对应的界面
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
	函数名：void Button_WCP_PressureZeroPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)

	描述：	该函数为工况预测调零设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WCP_PressureZeroPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}

		}	
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x3000);
			
				}
				else if(ps->Button==(0x001))  //按键值处理
				{
				  //to do 确认调零
					ps->Function=2;
					ps->TimerSec=0;
					
				}
		ScreenChangeDataInit(ps);
	} 
	//ScreenButtonReset();
}
//--
/*
	函数名：void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为工况预测烟温设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25 modify by alfred,修改超时处理。
*/
void Button_WCP_FumeTemperaturPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		case 0:
		{//没有选中二级对象
			ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
				ScreenTowOpButton(ps,1,0,0);  //烟温
				if(ps->Button==0x0Aff) //确认
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrGKYC_YWCL_SRYW,2,800.0,0.5);					
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,2,5); //数值输入
			}
			break;
		}				
	}
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
		if(ps->Button==0x0AFe||ps->Button==Max_Opt)
		{
			 ScreenBackLastPag(ps,0x3000);
		}
		else if(ps->Button==(0x0001))  //按键值处理
		{
			//to do 烟温选择
			 if(pf->Data[FLASHIS_AdrGKYC_YWCL_FS]==0)
			 {
				 pf->Data[FLASHIS_AdrGKYC_YWCL_FS]=1;pf->Flag=1;
				 if(ps->Key.bOutsideKeyboard != true)
									Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘 键盘控值分别为0XA1~0XA5
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
	函数名：void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为工况预测预测流速设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WCP_PredictedVelocityPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
Slave_DataStructure* Sp=Slave_GetData();

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
		}	
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					ScreenBackLastPag(ps,0x3000);			
				}
				else if(ps->Button==(0x0001))  //结束
				{
				  //to do 结束预测  先计算平均再保存
					if(ps->StarCun==1)
					{
					 ScreenBackLastPag(ps,0x3000);return;
					}
					for(int i=0;i<8;i++)
					{
					  DisBuff[i]=DisBuff[i+10]/(ps->StarCun-1);
					}
					//to du 保存
					DisBuff[8]=CalculationFormula_GetData()->Qsnd;//标杆流量
					float SaveBuff[20]={0};
					SaveBuff[0]=DisBuff[0]; //平均动压
					SaveBuff[1]=DisBuff[2];//平均静压
					SaveBuff[2]=DisBuff[1];//平均全压
					SaveBuff[3]=DisBuff[3];//大气压
					SaveBuff[4]=DisBuff[6];//获取烟道面积
					SaveBuff[5]=CalculationFormula_d(25.0f,DisBuff[4],CalculationFormula_GetData()->Xsw);//用A路来计算采样嘴直径
					SaveBuff[6]=DisBuff[5]; //烟温
					SaveBuff[7]=DisBuff[4];//烟气流速
					SaveBuff[8]=DisBuff[7];//烟气流量
					
					SaveBuff[9]=CalculationFormula_Qsnd(SaveBuff[8],SaveBuff[6],CalculationFormula_GetData()->Ps+CalculationFormula_GetData()->Ba,CalculationFormula_GetData()->Xsw); //标干流量   2021.3.9
					
					SaveBuff[10]=CalculationFormula_GetData()->Xsw*100;//含湿量
					SaveBuff[11]=GetFlaotdataFormFlash2Word(FLASHIS_AdrCLYQ_WJCX_JS_ZSXS);//折算系数
					SaveBuff[12]=GetFlaotdataFormFlash2Word(FLASHIS_AdrCLYQ_WJCX_JS_FHXS);//负荷系数
					SaveBuff[13]=CalculationFormula_GetData()->Kp ;//皮 托 管
					SaveWorkData(SaveBuff,0x00);
					SdFile_Serch_MaxofWorkLinkFile();//先读取最大文件号
					SdFile_WriteWorkFile(&SdFile_GetData()->InterFace_data,SdFile_GetData()->WorkLinkFileMax+1);  //写进6号文件
                    //保存一些平均值以备采样时调用
                    SaveFlaotToFlash(DisBuff[4],FLASHIS_AdrGKYC_YQLS_YQLS); //20210311   在这里保存平均流速值
                    SaveFlaotToFlash(DisBuff[0],FLASHIS_AdrGKYC_CPJZ_PJDY); //20210311   在这里保存平均动压值
			        SaveFlaotToFlash(DisBuff[2],FLASHIS_AdrGKYC_CPJZ_PJJY); //20210311   在这里保存平均静压值
					SaveFlaotToFlash(DisBuff[1],FLASHIS_AdrGKYC_CPJZ_PJQY); //20210311   在这里保存平均全压值
					SaveFlaotToFlash(DisBuff[5],FLASHIS_AdrGKYC_CPJZ_PJYW); //20210311   在这里保存平均烟温值
					SaveFlaotToFlash(DisBuff[7],FLASHIS_AdrGKYC_CPJZ_YGLL); //20210311   在这里保存平均流量值
                    SaveFlaotToFlash(SaveBuff[9],FLASHIS_AdrGKYC_CPJZ_BGLL); //20210311   在这里保存平均流量值
					SaveFlaotToFlash(SaveBuff[5],FLASHIS_AdrYCCJ_CYSE_CZZJ);//20210311   在这里保存采样嘴直径
					
                    //调转界面
					ps->RunMode=0x3400;//调转界面值文件查询
					ps->Key.Opt=1;
				}
				else if(ps->Button==(0x0002))  //确认当前
				{
				  //to do 确认当前值   利用ScreenData.Con 来做第几点
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
	函数名：void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为工况预测查询平均值设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25 modify by alfred,修改超时处理。
*/
void Button_WCP_CheckAveragePag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
    Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
		case 1:		
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInputFileName(ps,DisBuff,0,9999999,0,CALC_NO_POINT,CALC_NO_SIGNS);				
			}				
			else{
					ScreenTowOpButton(ps,1,0,0);  //文件号
					if(ps->Button==0x0Aff) //确认
					{
						ScreenConfirmEnterDisBuffer(ps,DisBuff,0,9999999,0);
					} 
					ScreenBackFirstOpt(ps);	       //退出检测
					ScreenEnterNumberText(ps,0,7); //数值输入
				}
			break;
		}		
	}
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x3000);
			
				}
				else if(ps->Button==(0x0001))  //按键值处理
				{
						if(ps->Key.bOutsideKeyboard != true)
									Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘 键盘控值分别为0XA1~0XA5
				  //to do 输入文件编号
						 ScreenTowOptChose(ps,1);	
				
				}else if(ps->Button==0x002)  //按键值处理
				{
				  //to do 上寻
			
					 if(ps->FileNumber>1)  ps->FileNumber--;    //20200528
					  ps->RunModeStat=1;
					  ps->Key.Opt=2;
				
				}else if(ps->Button==0x0003)  //按键值处理
				{
				  //to do 下旬寻
						if(ps->FileNumber<SdFile_GetData()->WorkLinkFileMax)  ps->FileNumber++;
					  ps->RunModeStat=1;
					 ps->Key.Opt=3;
						
				}else if(ps->Button==0x0004)  //按键值处理
				{
				  //to do 打印
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
	                    strcpy((char *)ps->Buffer,"没连接打印机!");//清提示20200519
	                    Screen_RefreshDataMulti(0x3B20,ps->Buffer,15*2);
					}

					
				}
		ScreenChangeDataInit(ps);
	} 
	//ScreenButtonReset();
}
//---2020-05-21
/*
	函数名：void Button_CHSL_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测含湿量设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25 modify by alfred,修改超时处理。
*/
void Button_CHSL_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnter(ps,FLASHIS_AdrCHSL_HSL,2,99.99,0.00);
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}		
			}
			else{	
					ScreenTowOpButton(ps,1,0,0);  
					if(ps->Button==0x0Aff) //确认
					{
						ScreenConfirmEnter(ps,FLASHIS_AdrCHSL_HSL,2,99.99,0.00);					
					} 
					ScreenBackFirstOpt(ps);	       //退出检测
					ScreenEnterNumberText(ps,2,5); //数值输入
					}
			break;
		}			
	}
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt||ps->Button==5)  //触摸为5，按键为4 （返回键）
				{
					 ScreenBackLastPag(ps,51);
			
				}
				else if(ps->Button==0x0001)  //按键值处理
				{
				  //to do 干湿球法测量
					ps->Key.Opt=1;			    
					 ps->RunMode=0x4100;	
				
				}else if(ps->Button==0x0002)  //按键值处理
				{
				  //to do 阻容法测量
					 ps->RunMode=0x4200;	
						ps->Key.Opt=2;
		
				
				}else if(ps->Button==(0x0003))  //按键值处理
				{
				  //to do 手动输入
							if(ps->Key.bOutsideKeyboard != true)
										Screen_WriteRegisterOneData(0x4f,0xA1);//弹出软件键盘					
							 ScreenTowOptChose(ps,3);	
						}
				/*
				else if(ps->Button==(0x0004))  //按键值处理
				{
				  //to do 含湿量查询
			     ps->RunMode=0x4400;	
						ps->Key.Opt=4;
				}
						*/
		ScreenChangeDataInit(ps);
	} 
	//ScreenButtonReset();
}
/*
	函数名：void Button_CHSL_GSQF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测含湿量-干湿球法设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CHSL_GSQF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
	}
	 { //屏幕按键处理
		   ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)  //返回
				{
					 ScreenBackLastPag(ps,0x4000);
					 Flux_StopSampling('B');
				}
				else if(ps->Button==(0x0001)||((ps->mystatus==0)&&(ps->RunModeStat==2)&&(ps->TimerSec==0)))  //按键值处理
				{
					ps->Key.Opt=1;
					FourBytes four;
					four.FloatData=DisBuff[0];//CalculationFormula_GetData()->Xsw;
					SaveFlaotToFlash(four.FloatData,FLASHIS_AdrCHSL_HSL);
					ScreenBackLastPag(ps,0x4000);
					Flux_StopSampling('B');
					Speak_Buzzer(200);//蜂蜜器提示	
				  //to do 确认
					
				}
	ScreenChangeDataInit(ps);
	} 
}
/*
	函数名：void Button_CHSL_ZRF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测含湿量-阻容法设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CHSL_ZRF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
	}
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x4000);
				}
				else if(ps->Button==0x0001)  //按键值处理
				{
				  //to do 确认
					ps->Key.Opt = 1;
					ScreenBackLastPag(ps,0x4000);
				}
	ScreenChangeDataInit(ps);
	} 
	//ScreenButtonReset();
}
//-----
/*	函数名：void Button_CHSL_ZRF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测含湿量-含湿量查询-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CHSL_HSLCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
		case 1:		ScreenTowOpButton(ps,1,0,0);  //烟温
							if(ps->Button==0x0Aff) //确认
							{
									ScreenConfirmEnterDisBuffer(ps,DisBuff,0,9999999,0);
							} 
							ScreenBackFirstOpt(ps);	       //退出检测
							ScreenEnterNumberText(ps,0,7); //数值输入
			break;
	  }
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x4000);
			
				}
				else if(ps->Button==(0x0001))  //按键值处理
				{
				  //to do 输入文件编号
					
						 ScreenTowOptChose(ps,1);	
				
				}else if(ps->Button==0x002)  //按键值处理
				{
				  //to do 上寻
			    ps->Key.Opt=2;					
				
				}else if(ps->Button==0x0003)  //按键值处理
				{
				  //to do 下旬寻
						ps->Key.Opt=3;		
				}else if(ps->Button==0x0004)  //按键值处理
				{
					ps->Key.Opt=4;		
				  //to do 打印
			
				}
		ScreenChangeDataInit(ps);
	} 
	//ScreenButtonReset();
}
//---
/*
//20200517	,20200522
	函数名：void Button_CLYQ_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测量烟气设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
    修改记录：	1. 2020-05-25,修改超时处理， modify by alfred.
				2. 去掉计时时长，增加计平均时间和次数的设置,2020.06.13, modify by alfred.
*/
void Button_CLYQ_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
		case 4://计平均时间
		case 5://计平均次数		
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrXTSZYQCLSJ + (ps->Key.Mode-4),1,60,1,CALC_NO_POINT,CALC_NO_SIGNS);	//限制最大计平均次数不大于60次
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
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
				}					//退出检测
				ScreenEnterNumberText(ps,0,2); //数值输入
			}
			break;
		}
	}
		
	{ //屏幕按键处理
		ps->OldMode=ps->RunMode;
		if(ps->Button==0x0AFe||ps->Button==0x0006)
		{
			ScreenBackLastPag(ps,51);
	
		}
		else if(ps->Button==0x0001)  //按键值处理
		{
			//to do 烟气校准		
			ps->RunMode=0x5100;	
		
		}
		else if(ps->Button==0x0002)  //按键值处理
		{
			//to do 开始测量	  
			ps->RunMode=0x5200;		
            ScreenJPG_ClearSum(); //请空累计平均数据
			
		}
		else if(ps->Button==0x0003)  //按键值处理
		{
			//to do 烟气查询
			ps->RunMode=0x5300;	
			ScreenTowOptChose(ps,3);	
		}
		else if(ps->Button==0x0004||ps->Button==0x0005)  //按键值处理
		{ //计时时长
			if(ps->Key.bOutsideKeyboard != true)
				Screen_WriteRegisterOneData(0x4f,0xA1+(ps->Button-4));//弹出软件键盘		
			ScreenTowOptChose(ps,ps->Button);	
		}
		ScreenChangeDataInit(ps);
	} 
	//ScreenButtonReset();
}
//----
/*
	函数名：void Button_CLYQ_YQJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测量烟-烟气校准设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CLYQ_YQJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
	  }
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0X5000);
			     Flux_StopSampling('C');//关闭气泵	
				}
				else if(ps->Button==0x0001)  //按键值处理
				{
				  //to do 校准逻辑
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
	函数名：Button_CLYQ_JIPINGJUN_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测含湿量设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：
*/
void Button_CLYQ_JIPINGJUN_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	 SdFile_DataStruct* SdFp=SdFile_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{		
		case 0:
		{	//没有选中二级对象
			ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
		//屏幕按键处理
		ps->OldMode=ps->RunMode;
		if(ps->Button==0x0AFe||ps->Button==Max_Opt)
		{
			 ScreenBackLastPag(ps,51);
//			ps->JiPingJun.ucCurrentPage = 0xff; //初始化当前页的值。
		}
		else if(ps->Button==0x0001)  //按键值处理
		{
			//to do 完成
			ps->Key.Opt=1;			    
			//to do 结束逻辑
			ps->JiPingJun.cIsCalcAverage = 0;
			ps->JiPingJun.cAverageCount = 0;
			ps->RunMode=0x5210;//提示是否结束
			ps->Function=0;
			ps->JiPingJun.ucCurrentPage = 0xff; //初始化当前页的值。
			
			//做一个平均？？
			//把采样时长修正
			Flux_GetData()->ChannelC.Sampling.SecTimer=Flux_GetData()->ChannelC.Sampling.SecTimer-Flux_GetData()->ChannelC.Sampling.SecTimer%(pf->Data[FLASHIS_AdrXTSZYQCLSJ]*60);

			Flux_GetData()->ChannelC.Sampling.JpjFlg=0; //清计平均标志
			Flux_GetData()->ChannelC.RunMode=4;
			Flux_StarSampling('C',1.0) ;//打开气泵 
			Flux_GetData()->ChannelC.Sampling.DXTime=FLUX_FangDaoXiTimeC;   //180秒清洗
		
		}else if(ps->Button==0x0002)  //上一页
		{
			//to do 上一页
			//显示上一页数据；
				ps->Key.Opt=2;	
			if(ps->JiPingJun.uMinDataPage>0)
			{
				ps->JiPingJun.uMinDataPage--;
				ps->JiPingJun.Timer = JPG_DELAY_TIME;	
			}
			ps->RunModeStat=1;
			
		}else if(ps->Button==(0x0003))  //下一页
		{
			//to do 上一页
			//显示下一页数据；
			if(ps->JiPingJun.uMinDataPage < (ps->JiPingJun.cAverageCount-1)/TABLE_ROW)	//修正逻辑错误, modify by alfred, 2020.07.14
			{
				ps->JiPingJun.uMinDataPage++;
				ps->JiPingJun.Timer = JPG_DELAY_TIME;
			}
			ps->Key.Opt=3;
			ps->RunModeStat=1;
		}
		else if(ps->Button==(0x0004))  //单位切换
		{
			//to do 单位转换		 	
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
	函数名：void Button_CLYQ_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测量烟气-开始测量设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
extern const u8 FOLDER_OF_Gas00[];
void Button_CLYQ_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
    

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{

		case 0:
		{//没有选中二级对象
			ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
	}
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					ps->Key.Opt=ps->Key.OldOpt;   //20200605
				    ps->Key.LastOpt=0;
				    ps->RunMode =52; //返回主页面
				    ps->RunModeStat=1;//更新静态数据
				
				}
				else if(ps->Button==0x0001)  //按键值处理
				{
				  //to do 打印逻辑
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
					buffer[5]=Qtp[7].Value;		//更正NOX，CO2气体打印值，modify by alfred, 2020.07.04
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
	                    strcpy((char *)ps->Buffer,"没连接打印机!");//清提示20200519
	                    Screen_RefreshDataMulti(0x2E20,ps->Buffer,15*2);
					}
				}
				else if(ps->Button==0x0002)  //按键值处理
				{ //to do 暂停逻辑
					ps->Key.Opt=2;
					ps->Key.LastOpt=0;
					if(ps->PauseOrStop==B0)
					{
					 ps->PauseOrStop=0;
					 Flux_GetData()->ChannelC.RunMode=2;
					 Flux_StarSampling('C',1.0) ;//打开气泵	
					}else
					{
					   ps->PauseOrStop=B0;
					   Flux_GetData()->ChannelC.RunMode=3;	
					   Flux_StopSampling('C');//打开气泵		
					}
				}
				else if(ps->Button==0x0003&&(ps->PauseOrStop!=B0))  //计平均
				{  //to do 计算平均值逻辑
					ps->JiPingJun.cIsCalcAverage = 1; //计平均标志位=1
					
					ps->JiPingJun.uCounter1ms = 0;//定时器清零
					ps->JiPingJun.cAverageCount = 0; //平均次数计数器清零					
					SdFile_GetData()->GasRecordMax = 0;	//计平均前分钟数据条数清零
					
					
	                sprintf((char*) ps->recorddat,"%02d-%02d-%02d %02d:%02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
	                ps->recorddat[16]=0;   //结尾先加个零
					
					sprintf((char*)ps->recorstarddat,"%02d-%02d-%02d %02d:%02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
					ps->recorstarddat[19]=0;  //保存开始计平均的开始时间

					u8 buf[5];
					sprintf((char *)&buf[0],"%02d",(pf->Data[FLASHIS_AdrXTSZYQCLSJ]));  //
	                strcat((char *)&ps->recorddat[0],(char *)&buf[0]);
					strcat((char *)&ps->recorddat[0],"9");

                    Flux_GetData()->ChannelC.Sampling.SecTimer=0;   //采样时长重新开始   20210420
					
					if(ps->Key.Opt!=3)                                             //20200513
					ps->Key.Opt=3;
					ps->RunMode = 0x5230;
//					  ps->RunModeStat=1;
					  ps->Function=1; //功能计算平均
					  Flux_GetData()->ChannelC.Sampling.JpjFlg=1;                  //20200606
					  ps->PauseOrStop=0;  //继续状态
					  ps->JiPingJun.JpjEndFlg=0;

				}
				else if(ps->Button==0x0004)  //按键值处理
				{    //to do 结束逻辑
					ps->Key.Opt=4;
					Flux_GetData()->ChannelC.Sampling.JpjFlg=0; //清计平均标志
					Flux_GetData()->ChannelC.RunMode=4;
					Flux_StarSampling('C',1.0) ;//打开气泵	
					Flux_GetData()->ChannelC.Sampling.DXTime=FLUX_FangDaoXiTimeC;   //180秒清洗
					//Flux_StopSampling('C');//打开气泵		
					ps->RunMode=0x5210;//提示是否结束
					ps->Function=0;
				}
				/*
				if((pf->Data[FLASHIS_AdrCLYQ_CLFS]==1)&&(Flux_GetData()->ChannelC.RunMode==4)) //在自动模式判断是否到了清洗
					{
						ps->Key.Opt=5;
						// Flux_StopSampling('C');//打开气泵		
						//to do 结束逻辑				

						ps->RunMode=0x5210;//提示是否结束
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
	if(ps->RunMode==0x5000&&ps->YC_Stat==1) //有烟尘采样
	{
    ps->RunMode=0x6200;
		Screen_ShowPage(64); 
	}
}
//----
/*
	函数名：void Button_CLYQ_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测量烟气-开始测量-结束测量设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
extern unsigned int writeWorkfileEnable;
void Button_CLYQ_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	Slave_DataStructure*  Sp=Slave_GetData();
	//	 SdFile_DataStruct* SdFp=SdFile_GetData();
	//Button_DataStructure *p = Button_GetData();

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
		
	  }

	 { //屏幕按键处理
		    ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					ScreenBackLastPag(ps,0X5220);
					char pathbuf[202];   //放完整文件路径
					SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],ps->FileNumber); //把文件的路径放到路径缓冲区
					FS_DeleteFolderOrFile(pathbuf);
					f_unlink("0:/YLB3330/Gas/link.txt");// 删除原有的索引文件, add by alfred, 2020.07.13
					mf_copy((u8*)"0:/YLB3330/Gas/temp0.txt",(u8*)"0:/YLB3330/Gas/link.txt",1);//将采样前保存的索引文件内容复制到link.txt中。
					Flux_GetData()->ChannelC.Sampling.EndFlg = 0;
					writeWorkfileEnable = 1;
					//Flux_GetData()->ChannelC.RunMode=0;	
			
				}
				else if(ps->Button==0x0001)  //按键值处理
				{
				  //to do 保存数据逻辑
					/*保存数据*/
					
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
				else if(ps->Button==0x0002)  //按键值处理
				{    
					ScreenBackLastPag(ps,0X5220);//取消保存
					char pathbuf[202];   //放完整文件路径
					SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],ps->FileNumber); //把文件的路径放到路径缓冲区
					FS_DeleteFolderOrFile(pathbuf);
					
					f_unlink("0:/YLB3330/Gas/link.txt");// 删除原有的索引文件, add by alfred, 2020.07.06
					mf_copy((u8*)"0:/YLB3330/Gas/temp0.txt",(u8*)"0:/YLB3330/Gas/link.txt",1);//将采样前保存的索引文件内容复制到link.txt中。
					Flux_GetData()->ChannelC.Sampling.EndFlg = 0;
				
				  //to do 暂停逻辑
				}
//					else if(ps->Button==0x0003)  //按键值处理
//				{
//					// ScreenBackLastPag(ps,0X5200); //继续测量
//					ps->RunMode=0x5200;
//					FluxData.ChannelC.Sampling.EndFlg=1;//采样完成，等待保持，进入保存请这标志
//				}
				ScreenChangeDataInit(ps);
 /* if(ps->RunMode==0X5200)
	{
			ps->TimerSecUp=ps->LastTimerSecUp;
			ps->TimerMinUp=ps->LastTimerMinUp;
			ps->Function=ps->LastFunction;
			ps->Warning=ps->LastWarning;
			ps->RunModeStat=2;
		 //Flux_StarSampling('C',1.0);//打开气泵	
     Screen_ShowPage(38); 		
	}
	*/			
	}
}
//----
/*
	函数名：void Button_CLYQ_YQQX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测量烟气-开始测量-结束测量-清洗设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CLYQ_YQQX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
	  }
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt||(ps->RunModeStat==2&&Flux_GetData()->ChannelC.Sampling.FdxFlg))
				{
					 ScreenBackLastPag(ps,0X5000);
					 Flux_StopSampling('C');//关闭
					 Flux_GetData()->ChannelC.RunMode=0;
					 Flux_GetData()->ChannelC.Sampling.FdxFlg=0;
				}
		
	ScreenChangeDataInit(ps);
	}
}
/*//2020-05-22
20200516
	函数名：void Button_CLYQ_YQCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测量烟气-烟气查询设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25,修改超时处理， modify by alfred.
*/
void Button_CLYQ_YQCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ps->FileNumber = (float)atoi((char*)&(ps->Key.Buffer[0]));
					if(ps->FileNumber<1) ps->FileNumber=1; //限制 大于50Kp
					if(ps->FileNumber>9999999) ps->FileNumber=9999999; //限制 小于于130Kp
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}		
			}
			else
			{			
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
				{
					ps->FileNumber = (float)atoi((char*)&(ps->Key.Buffer[0]));
					if(ps->FileNumber<1) ps->FileNumber=1; //限制 大于50Kp
					if(ps->FileNumber>9999999) ps->FileNumber=9999999; //限制 小于于130Kp
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,0,2); //数值输入
			}				
			break;
		}	
	}
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x5000);	
					SdFile_GetData()->GasRecordMax = 0; //退出查询时清除分钟数据条数。
				}
				else if(ps->Button==0x0001)  //按键值处理
				{
					    //上寻
						
							if(ps->FileNumber>1) ps->FileNumber--; //限制 大于50Kp20200528
						  ps->Key.Opt=1;
					    ps->RunModeStat=1;
				
				}
				else if(ps->Button==0x0002)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA1);//弹出软件键盘
				  //to do 输入
			    ScreenTowOptChose(ps,2);	
				}
				else if(ps->Button==0x0003)  //按键值处理
				{
				  //to do 下旬
						ps->FileNumber++;
							if(ps->FileNumber>SdFile_GetData()->GasLinkFileMax) ps->FileNumber=SdFile_GetData()->GasLinkFileMax; //限制 大于50Kp	
					 ps->Key.Opt=3;
					 ps->RunModeStat=1;
				}
				else if(ps->Button==0x0004)  //按键值处理
				{
				  //to do 上一页
					 ps->Function=0;		//翻页指示		 
     ps->RunModeStat=1;
					ps->Key.Opt=4;			
				}
				else if(ps->Button==0x0005)  //按键值处理
				{
					//to do 下一页
					 ps->Function=1;		//翻页指示
				 ps->RunModeStat=1;
					ps->Key.Opt=5;
				}
				else if(ps->Button==0x0006)  //按键值处理
				{
					//to do 计算
					ps->Key.Opt=6;
					ps->RunMode=0x5310;
				}
				else if(ps->Button==0x0007)  //按键值处理
				{
					//to do 文件管理
					ps->Key.Opt=7;
					ps->RunMode=0x5320;
				}
				else if(ps->Button==0x0008)  //按键值处理
				{
					//to do 打印选项
						ps->RunMode=0x5330;
					ps->Key.Opt=8;
				}
				else if(ps->Button==0x0009)  //按键值处理
				{
					//to do 分中数据
					if(SdFile_GetData()->GasRecordMax)                  //20200516
					{
					ps->RunMode=0x5340;
					}
					ps->Key.Opt=9;
				}
				else if(ps->Button==0x000A)  //按键值处理
				{
					//to do 打印
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
	                    strcpy((char *)ps->Buffer,"没连接打印机!");//清提示20200519
	                    Screen_RefreshDataMulti(0x2E20,ps->Buffer,15*2);
					}
				}
	ScreenChangeDataInit(ps);
	} 
}
/*20200518
	函数名：void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测量烟气-烟气查询-计算设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25,修改超时处理， modify by alfred.
*/
void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//外围按键处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					FourBytes four;
					if(ps->Key.Mode==1)  //折算系数
					{					
						four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));  //输入完毕,并保存
						if(four.FloatData<=1)                                                 //20200526
                        four.FloatData=1; //限制 小于于130Kp
						else if(four.FloatData>99999)
						 four.FloatData=99999.00f;	
						ps->ZSXS=four.FloatData;   //暂存折算系数
							
						//求含氧量
						four.FloatData=21.0f/four.FloatData;
						four.FloatData=21-four.FloatData;
						ps->JZHYL=four.FloatData;  //暂存基准含氧量						
					}
					else if(ps->Key.Mode==2)//基准含氧量
					{
						four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));  //输入完毕,并保存
						if(four.FloatData>=20) four.FloatData=20; //限制
                    						
						ps->JZHYL=four.FloatData;   //暂存折算系数	//20200526					
						//求折算系数
						four.FloatData=21.0f-four.FloatData;
						four.FloatData=21.0f/four.FloatData;
					
						ps->ZSXS=four.FloatData;   //暂存折算系数
						
					}
					else if(ps->Key.Mode==3)   //负荷系数	
					{
						four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));  //输入完毕,并保存
						if(four.FloatData<0.01f) four.FloatData=0.01f; //限制 大于50Kp
						if(four.FloatData>10.0f) four.FloatData=10.0f; //限制 小于于130Kp
						ps->FHXS=four.FloatData;   //暂存负荷系数							
					}
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}			
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
				{
					FourBytes four;
					if(ps->Key.Mode==1) //折算系数
					{					
						four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));  //输入完毕,并保存
						if(four.FloatData<1)                                                 //20200526
                        four.FloatData=1; //限制 小于于130Kp
						else if(four.FloatData>99999)
						four.FloatData=99999.00f;	
						ps->ZSXS=four.FloatData;   //暂存折算系数
							
						//求基准含氧量
						four.FloatData=21.0f/four.FloatData;
						four.FloatData=21-four.FloatData;
						ps->JZHYL=four.FloatData;  //暂存基准含氧量						
					}
					else if(ps->Key.Mode==2)//基准含氧量
					{
						four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));  //输入完毕,并保存
						if(four.FloatData>=20) four.FloatData=20; //限制 
						ps->JZHYL=four.FloatData;   //暂存折算系数	//20200526					
						//求折算系数
						four.FloatData=21.0f-four.FloatData;
						four.FloatData=21.0f/four.FloatData;
					
						ps->ZSXS=four.FloatData;   //暂存折算系数
						
					}
					else if(ps->Key.Mode==3)//负荷系数	
						{
							four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));  //输入完毕,并保存
							if(four.FloatData<0.01f) four.FloatData=0.01f; //限制 大于50Kp
							if(four.FloatData>10.0f) four.FloatData=10.0f; //限制 小于于130Kp
							ps->FHXS=four.FloatData;   //暂存负荷系数								
						}
					} 
					ScreenBackFirstOpt(ps);	       //退出检测
					ScreenEnterNumberText(ps,2,6); //数值输入
				}
			break;
		}
	}
		
	 { //屏幕按键处理 +外围按键最终处理
		        ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x5300);
						
				}
				else if(ps->Button==0x0001||ps->Button==0x0002||ps->Button==0x0003)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0 + ps->Button);//弹出软件键盘					
					ScreenTowOptChose(ps,ps->Button);	
				
				}
				/* 功能屏蔽，如要打开在迪文屏上整机触控
				else if(ps->Button==0x0004)  //按键值处理
				{
				  //to do 烟气输入
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
	函数名：void Button_CLYQ_YQCX_JS_YQSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	描述：	该函数为测量烟气-烟气查询-计算-烟气输入设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25,修改超时处理, modify by alfred.
*/
void Button_CLYQ_YQCX_JS_YQSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	//Button_DataStructure *p = Button_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
		default :
		{
			if(ps->Key.Mode<=7)
			{
				if(ps->Key.bOutsideKeyboard != true)
				{
					ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);	//修正外部键盘不能输入小数点的问题, modify by alfred,2020.07.07 
					if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
					{     
						ScreenBackLastOpt(ps);
					}
					if(ps->sRcvType !=0)
					{
						Screen_ResetKeyDataBuffer();//清除按键值
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
						ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
						ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
					}
				}
				else
				{             							 
					ScreenTowOpButton(ps,1,0,0);  
					if(ps->Button==0x0Aff) //确认
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
					ScreenBackFirstOpt(ps);	       //退出检测
					ScreenEnterNumberText(ps,2,6); //数值输入
				}
			}
			break;
		}		
	}
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x5310);
			
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x0007)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘					
					ScreenTowOptChose(ps,ps->Button);	
				
				}
				else if(ps->Button==0x0008)  //按键值处理
				{
				  //to do 保存数据
					/*保存数据*/
			//		float buffer[40];
					
					SaveGasAverageRecordData((float * )&ps->DisValue[0],ps->FileNumber);   //浮点数放到缓冲区20200517
					SaveGas_Z_J_F_RecordData(); //20200518
					SdFile_WriteGasFile_Z_J_F_Record(&SdFile_GetData()->InterFace_data,ps->FileNumber);//保存折算系数，基准含氧量，负荷系数，气体浓度到SD卡
					//SdFile_WriteGasFileAverageRecord(&SdFile_GetData()->InterFace_data,ps->FileNumber);
	
					ps->Function=1;
					ps->Key.Opt=8;
				}
				else if(ps->Button==0x0009)  //按键值处理
				{
				  //to do 另存为数据
					/*保存数据*/
					float buffer[40];
					SdFile_Serch_MaxofGasLinkFile();//先搜寻最大文件数
					SdFile_GetData()->GasLinkFileMax++;
					SaveGasINputDataLoad(buffer);
					SaveGasData(buffer,SdFile_GetData()->GasLinkFileMax,1);
					
					//SdFile_Serch_MaxofGasRecord(ps->FileNumber+1); //读取最大的记录数
					SdFile_WriteGasFile(&SdFile_GetData()->InterFace_data,SdFile_GetData()->GasLinkFileMax,0,1);  //20200518
					ps->Key.Opt=9;
					ps->Function=2;
				}
				
	ScreenChangeDataInit(ps);
	} 
}
//---
/*
	函数名：void Button_CLYQ_YQCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	描述：	该函数为测量烟气-烟气查询-文件管理-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25,修改超时处理, modify by alfred.
*/
void Button_CLYQ_YQCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
		case 1:
		case 2:
		{			
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);	//屏幕弹出软件盘时支持外部键盘输入，add by alfred,2020.07.07 
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,SdFile_GetData()->GasLinkFileMax,1); //限制导出的最大文件号不超过索引文件的最大值，不然会导出错误, modify by alfred, 2020.07.08
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}	
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
				{
					ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,SdFile_GetData()->GasLinkFileMax,1);
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,0,7); //数值输入
			}
			break;
		}		
	}
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x5300);			
				}
				else if(ps->Button==0x0001||ps->Button==0x0002)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘						
					ScreenTowOptChose(ps,ps->Button);	
				
				}
				else if(ps->Button==0x0003)  //按键值处理
				{
				  //to do 导出数据
					char uMachineNumBuffer[10], uSysDateBuffer[10];
					FourBytes four;
					four.U16Data[0]=pf->Data[FLASHIS_AdrCJWHYQBH];
					four.U16Data[1]=pf->Data[FLASHIS_AdrCJWHYQBH+1];
					
					sprintf((char*)uMachineNumBuffer,"%08d",four.U32Data);
					sprintf((char*)uSysDateBuffer,"%02d%02d%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday);					
					Usb_OutGasFileInf(DisBuffer[0],DisBuffer[1],uSysDateBuffer,uMachineNumBuffer);
					ps->Key.Opt=3;
				}
				else if(ps->Button==0x0004)  //按键值处理
				{
					//to do 打印
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
					buffer[1]= DisBuffer[0];	//起始文件号
					buffer[2]= DisBuffer[1];	//结束文件号
					buffer[3]= DisBuffer[0];	//分钟数据所在文件号
					PrintGasFile((u8 *)&pn[0],(float *)&buffer[0]);
				   }
				    else
					{
						Screen_ResetDataBuffer();
	                    strcpy((char *)ps->Buffer,"没连接打印机!");//清提示20200519
	                    Screen_RefreshDataMulti(0x3B20,ps->Buffer,15*2);
					}
					
					ps->Key.Opt=4;
				}
	ScreenChangeDataInit(ps);
	} 
}
//---
/*
	函数名：void Button_CLYQ_YQCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	描述：	该函数为测量烟气-烟气查询-打印选项-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CLYQ_YQCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}

	  }
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x5300);
			         pf->Flag=1;    //20200523
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x20)  //按键值处理
				{
					ps->Key.Opt= ps->Button;
					u32 data=pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX]+((pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX+1])<<16);;
					u8 point=ps->Key.Opt-1;
					if(point>15)
					{      data=data>>16;
						     point-=16;
					if((data>>point)&0x00000001)
					{
					  pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX+1]&=~(1<<point);//位置0
					}else
					{
					 pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX+1]|=(1<<point);//位置1
					}
					}else
					{
						if((data>>point)&0x00000001)
						{
							pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX]&=~(1<<point);//位置0
						}else
						{
							pf->Data[FLASHIS_AdrCLYQ_WJCX_DYXX]|=(1<<point);//位置1
						}
					}				
				}
	ScreenChangeDataInit(ps);
	} 
}
//---
/*
20200515
	函数名：void Button_CLYQ_YQCX_FZSJ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测量烟气-烟气查询-分钟数据设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CLYQ_YQCX_FZSJ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
    SdFile_DataStruct* SDp=SdFile_GetData();

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{	
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
	 }
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x5300);
										
				}
				else if(ps->Button==0x0001)  //按键值处理
				{ //参与计算
					
					
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
				else if(ps->Button==0x0002)  //按键值处理
				{
				  //to do 上一分钟数据
					if(ps->RecordCun>1) ps->RecordCun--; 
			  	 ps->Key.Opt=2;
					ps->RunModeStat=1;

				}
				else if(ps->Button==0x0003)  //按键值处理
				{
				  //to do 下一分钟数据
					if((SDp->GasRecordMax)>(ps->RecordCun)) ps->RecordCun++;
			  	 ps->Key.Opt=3;
					ps->RunModeStat=1;
					 
				}
				else if(ps->Button==0x0004)  //按键值处理
				{
				  //to do 打印当前
					
					ps->Key.Opt=4;			
						//to do 打印
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
					Printer_GetData()->PrintSelect=0; //打印当前
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
	                    strcpy((char *)ps->Buffer,"没连接打印机!");//清提示20200519
	                    Screen_RefreshDataMulti(0x3B20,ps->Buffer,15*2);
					}
					
				}
				else if(ps->Button==0x0005)  //按键值处理
				{
					//to do 打印选中
					ps->Key.Opt=5;
					ps->Key.Opt=4;			
						//to do 打印
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
					Printer_GetData()->PrintSelect=ps->UseGasCal; //打印当前
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
	                    strcpy((char *)ps->Buffer,"没连接打印机!");//清提示20200519
	                    Screen_RefreshDataMulti(0x3B20,ps->Buffer,15*2);
					}
					
				}
				else if(ps->Button==0x0006)  //按键值处理
				{
					//to do 计算平均值
					ps->Key.Opt=6;
				  u8 SumCun=0;//计算平均技术
					float SumData[8]={0};//总数
					
					for(int i=0;i<SDp->GasRecordMax;i++)                       //20200515
					{
	
							GetGasFileRecodData(ps->FileNumber,i+1,ps->DisValue,1);
							if(ps->DisValue[8]==1)   //参与计算
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
					  ps->DisValue[j]=ps->DisValue[j+1]; //剔除NOx
					
					if(SumCun>0) //另存新数据    //保存平均数据
					{

						SaveGasAverageRecordData((float * )&ps->DisValue[0],ps->FileNumber);   //浮点数放到缓冲区20200515
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
	函数名：void Button_YCCY_CYSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为烟尘采样-采样设置设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25,修改超时处理, modify by alfred.
*/
void Button_YCCY_CYSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
   Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
				if(ps->Button==0x0Aff) //确认
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrYCCJ_CYSE_CYLS,2,110.0,5);    //test718
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,2,5); //数值输入
			}
			break;
		}
		case 3:  //滤筒
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYCCJ_CYSE_LTH,3,9999999,1,CALC_NO_POINT,CALC_NO_SIGNS);
			}
			else
			{					
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrYCCJ_CYSE_LTH,3,9999999,1);
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,0,5); //数值输入
			}
			break;
		}
		case 4:		//定点采时
		case 5:
		{						
			if(ps->Key.bOutsideKeyboard != true) //屏幕
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					u16 Datai=atoi((char*)&(ps->Key.Buffer[0]));
					if(Datai<=0) Datai=0; //限制 大于50Kp
					if(Datai>99) Datai=99; //限制 小于于130Kp
					if(ps->Key.Mode==4)
					{
						pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]=Datai*60; //保存值
					}else
					{
						if(Datai<=0) Datai=0; //限制 大于50Kp
						if(Datai>59) Datai=59; //限制 小于于130Kp
						pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]=pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]/60*60+Datai; //保存值
					}
					pf->Flag=1;
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}	
			}
			else
			{							
					ScreenTowOpButton(ps,1,0,0);  
					if(ps->Button==0x0Aff) //确认
						{
							u16 Datai=atoi((char*)&(ps->Key.Buffer[0]));
							if(Datai<=0) Datai=0; //
							if(Datai>99) Datai=99; //
							if(ps->Key.Mode==4)
							{
								pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]=Datai*60; //保存值
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
								pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]=pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]/60*60+Datai; //保存值
								
							}
							pf->Flag=1;
						} 
					ScreenBackFirstOpt(ps);	       //退出检测
					ScreenEnterNumberText(ps,0,2); //数值输入						
			}
			break;
		}
		case 6: //采嘴直径
		{				
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYCCJ_CYSE_CZZJ,2,20.0,4,CALC_HAVE_POINT,CALC_NO_SIGNS);
			}
			else
			{						
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrYCCJ_CYSE_CZZJ,2,20.0,4);
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,2,5); //数值输入
			}
		 break;
		}			
		case 7:	//总采点
		{
			if(ps->Key.bOutsideKeyboard != true)
			{
				ProcessSoftKeyboardInput(ps,FLASHIS_AdrYDBD_CYDS,1,50,1,CALC_NO_POINT,CALC_NO_SIGNS);
			}	
			else
			{								
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
				{
					ScreenConfirmEnter(ps,FLASHIS_AdrYDBD_CYDS,1,50,1);
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,0,5); //数值输入
			}
			break;
		}		
	}
		
	 { //屏幕按键处理
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==0x0a)
				{
					 ScreenBackLastPag(ps,52);
			
				}
				else if(ps->Button==0x0001)  //按键值处理
				{
         			
							pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]=0;
							if(ps->Key.bOutsideKeyboard != true)
							Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘 
							ScreenTowOptChose(ps,ps->Button);	
				            ps->Key.Opt=1;
				}
			  else if(ps->Button==0x0002)  //按键值处理
				{
					     pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]=1;
					     ps->RunModeStat=1;
						 ps->Key.Opt=2;
				
				}
				else if(ps->Button==0x0003)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA2);//弹出软件键盘 					
				    ScreenTowOptChose(ps,3);
			  	    ps->Key.Opt=3;
				}
				else if(ps->Button==0x0004)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA3);//弹出软件键盘 					
				    ScreenTowOptChose(ps,4);
			  	    ps->Key.Opt=4;
					 
				}
				else if(ps->Button==0x0005)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA4);//弹出软件键盘 					
				    ScreenTowOptChose(ps,5);
			  	    ps->Key.Opt=4;
				}
				else if(ps->Button==0x0006)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA5);//弹出软件键盘 					
				    ScreenTowOptChose(ps,6);
					ps->Key.Opt=5;			
				}
				else if(ps->Button==0x0007)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
					 Screen_WriteRegisterOneData(0x4f,0xA6);//弹出软件键盘 					
					 ScreenTowOptChose(ps,7);
					
					  ps->Key.Opt=6;
				}
				else if(ps->Button==0x0008)  //按键值处理
				{
					
					ps->RunMode=0x6100; //气密性检测
					ps->Key.Opt=1;
				    ps->Key.State=0;
				}
				else if(ps->Button==0x0009)  //按键值处理
				{

					ps->RunMode=0x6200; //开始采样
					Flux_ClearMidBuff(&Flux_GetData()->ChannelB.FluxTowData.Filter,Flux_GetData()->ChannelB.ShowFlux); //清空缓冲区
					ps->Function=0;
					ps->Key.Opt=1;			

				}
	ScreenChangeDataInit(ps);
	} 
}
//----
/*
	函数名：void Button_YCCY_QMJC_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为烟尘采样-气密性检测界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_YCCY_QMJC_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
	}
	 { //屏幕按键处理
		    ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x6000);	Flux_StopSampling('B');
				}
				else if(ps->Button==(0x0001))  //按键值处理
				{
					ps->Key.Opt=1;
					if(ps->PauseOrStop==0)  //=0 停止测量
					{
					  ps->PauseOrStop=B0;//开始测量
					  ps->TimerSecUp=0;
					  Flux_StarSampling('B',15.0);//100L/min
					}else
					{
					 ps->PauseOrStop=0;
					 Flux_StopSampling('B');
					 ps->TimerSecUp=0;
					}
					ps->Key.LastOpt=0;
				  //to do 确认
				}
	ScreenChangeDataInit(ps);
	} 
}
//----
/*
	函数名：void Button_YCCY_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测量烟气-开始测量设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_YCCY_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
   Flux_DataStructure  *pfd =Flux_GetData();

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
	  }
		
	 { //屏幕按键处理
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					
				    ps->Key.Opt=ps->Key.OldOpt;   //20200605
				    ps->Key.LastOpt=0;
				    ps->RunMode =52; //返回主页面
				    ps->RunModeStat=1;//更新静态数据
			    
				}
				else if(ps->Button==0x0001)  //按键值处理
				{
				  //to do 烟气测量
					if(Flux_GetData()->ChannelC.RunMode==0)
					{
					ps->RunMode=0x5000;
					ps->Key.Opt=1;
					}
				}
				else if(ps->Button==0x0002)  //按键值处理
				{	
				//	TwoBytes two;
					ps->Key.Opt=2;
					  //结束
					 
				    ps->RunMode=0x6210; //去保存

				  //pfd->ChannelB.Sampling.YiciFlg=0;
				 // pfd->ChannelB.Sampling.Cnt=1;
				 // pfd->ChannelB.Sampling.EndFlg=1;

					//用该标志表示暂停
				    //to do 暂停逻辑
				}
				else if(ps->Button==0x0003)  //按键值处理
				{
				  //to do 单位逻辑
						ps->Key.Opt=3;
					if(ps->YC_Pause==B0)
					{
						 //暂停
					 ps->YC_Pause=0;
					  if(pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]==1) //等速
				      { Flux_ClearMidBuff(&Flux_GetData()->ChannelB.FluxTowData.Filter,Flux_GetData()->ChannelB.ShowFlux);
					    Flux_StarSampling('B',CalculationFormula_Qrs(GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ),GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_YQLS_YQLS),GetFlaotdataFormFlash2Word(FLASHIS_AdrCHSL_HSL)/100)); //等A流SU
						Flux_GetData()->ChannelB.RunMode=2;  //启动线程
				      }else
				      { Flux_ClearMidBuff(&Flux_GetData()->ChannelB.FluxTowData.Filter,Flux_GetData()->ChannelB.ShowFlux);
					    Flux_StarSampling('B',GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CYLS)); //目标流量
						Flux_GetData()->ChannelB.RunMode=2;  //启动线程
				      }  		
					}else
					{
					   ps->YC_Pause=B0;

					   FluxData.ChannelB.Sampling.DXTime=10;   //暂停操作中，10秒防倒吸
				       Flux_ContinueSampling('B',20);//防倒吸  20L/min  10s
					   Flux_GetData()->ChannelB.RunMode=3;  //暂停

					}
					ps->Key.LastOpt=0;	
				}

			else if(Flux_GetData()->ChannelB.Sampling.EndFlg) //跳到保存
			{
					 ps->RunMode=0x6210; //去保存
					 Flux_GetData()->ChannelB.Sampling.EndFlg=0;
			}  
					
	ScreenChangeDataInit(ps);
	}
}
//----
/*
	函数名：void Button_YCCY_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为烟尘采集-开始测量-结束测量设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_YCCY_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
   TwoBytes two;
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
	  }	
	 { //屏幕按键处理
		    ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					ps->Key.Opt=6;   //20200605
				    ps->Key.LastOpt=0;
				    ps->RunMode =52; //返回主页面
				    ps->RunModeStat=1;//更新静态数据
				}
				else if(ps->Button==0x0001)  //按键值处理
				{
				  //to do 保存数据逻辑
						//采集完成 ===保存数据
					  	/*保存数据*/
						float buffer[40];
						SaveDustDataLoad(buffer);
						SaveDustData(buffer,ps->FileNumber);
						SdFile_WriteDustFile(&SdFile_GetData()->InterFace_data,ps->FileNumber);
					    Flux_GetData()->ChannelB.Sampling.FdxFlg=0; //保存完毕
					
					//保持完跳到查询页    
					 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&0x02)&&((Flux_GetData()->ChannelB.RunMode==2)||((Flux_GetData()->ChannelB.RunMode==3)&&(Flux_GetData()->ChannelB.Sampling.DXTime!=0)))) //防倒吸选择
                     {		
                         //防倒吸页
				           ps->Key.Opt=1;   //
				           ps->Key.LastOpt=0;
				           ps->RunMode =0x6220; //防倒吸页
				           ps->RunModeStat=1;//更新静态数据
						   Flux_GetData()->ChannelB.Sampling.DXTime=FLUX_FangDaoXiTimeB;   //60秒放倒吸
				           Flux_StarSampling('B',20);//防倒吸  20L/min  10s
						   Flux_GetData()->ChannelB.RunMode=4;
				    }
					 else
					 {
						Flux_GetData()->ChannelB.RunMode=0; 
						Flux_StopSampling('B');
						ps->Key.Opt=1;   //
				        ps->Key.LastOpt=0;
				        ps->RunMode =0x7000; //去查询
						ps->RunModeStat=1;//更新静态数据
					 }
					 
					two.U8Data[0] = Flux_GetData()->ChannelB.RunMode;
		            two.U8Data[1] = 0;
		            pf->Data[FLASHIS_AdrCJMOD] 	= two.U16Data; 			    //采样模式+状态标志位  
					pf->Flag = 1; 
             		return;
				}
				else if(ps->Button==0x0002)  //按键值处理
				{   	/*不保存保存数据*/

					
				    Flux_GetData()->ChannelB.Sampling.FdxFlg=0;  //防倒吸完成
				    ps->Key.Opt=6;   //20200605
			        ps->Key.LastOpt=0;
				    ps->RunMode =52; //返回主页面
				    ps->RunModeStat=1;//更新静态数据
					pf->Flag = 1;
					Flux_StopSampling('B');  //停止尘泵
					Flux_GetData()->ChannelB.RunMode=0;
					two.U8Data[0] = Flux_GetData()->ChannelB.RunMode;
		            two.U8Data[1] = 0;
		            pf->Data[FLASHIS_AdrCJMOD] 	= two.U16Data; 			    //采样模式+状态标志位 
					 
				  //to do
				}

	              ScreenChangeDataInit(ps);
				  if(ps->RunMode==0x6200)
					{	
					 ps->RunModeStat=2;//更新静态数据	
                     Screen_ShowPage(64); 
					}
	}
}
//----
/*
	函数名：void Button_YCCY_YCFDX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测量烟气-开始测量-结束测量-烟尘防倒吸-置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_YCCY_YCFDX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
	  }
		
	 { //屏幕按键处理
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt||(ps->RunModeStat==2&&Flux_GetData()->ChannelB.Sampling.DXTime==0))
				{   TwoBytes two;
				    Flux_GetData()->ChannelB.Sampling.FdxFlg=0;
				    Flux_GetData()->ChannelB.RunMode=0;
				    ps->Key.Opt=1;   //20200605
				    ps->Key.LastOpt=0;
				    ps->RunMode =0x7000; //转到查询页
				    ps->RunModeStat=1;//更新静态数据
			        Flux_StopSampling('B');
					two.U8Data[0] =Flux_GetData()->ChannelB.RunMode;
		            two.U8Data[1] = 0;
		            pf->Data[FLASHIS_AdrCJMOD] 	= two.U16Data; 			    //采样模式+状态标志位
					pf->Flag=1;
				}
		   
	ScreenChangeDataInit(ps);

	}
}
//---
/*
	函数名：void Button_YCCY_YCCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为烟气采样-烟尘查询设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25,修改超时处理, modify by alfred.
*/
void Button_YCCY_YCCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ps->FileNumber = (float)atoi((char*)&(ps->Key.Buffer[0]));
					if(ps->FileNumber<1) ps->FileNumber=1; //限制 大于50Kp
					if(ps->FileNumber>=SdFile_GetData()->DustLinkFileMax) ps->FileNumber=SdFile_GetData()->DustLinkFileMax; //限制 小于于130Kp
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}
			}
			else
			{  			
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
				{
						ps->FileNumber = (float)atoi((char*)&(ps->Key.Buffer[0]));
						if(ps->FileNumber<1) ps->FileNumber=1; //限制 大于50Kp
						if(ps->FileNumber>=SdFile_GetData()->DustLinkFileMax) ps->FileNumber=SdFile_GetData()->DustLinkFileMax; //限制 小于于130Kp
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,0,8); //数值输入
			}
			break;
		}		
	}
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,51);
		
				}
				else if(ps->Button==0x0001)  //按键值处理
				{
					    //上寻
							
						if(ps->FileNumber>1) ps->FileNumber--; //限制 大于50Kp
						ps->Key.Opt=1;
					    ps->RunModeStat=1;
					    ps->Key.Mode=0;
					 ps->Function=0;  
				
				}
				else if(ps->Button==0x0002)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA1);//弹出软件键盘
					//to do 输入
					ScreenTowOptChose(ps,2);	
				}
				else if(ps->Button==0x0003)  //按键值处理
				{
				  //to do 下旬
						ps->FileNumber++;
							if(ps->FileNumber>SdFile_GetData()->DustLinkFileMax) ps->FileNumber=SdFile_GetData()->DustLinkFileMax; //限制 大于50Kp	
					 ps->Key.Opt=3;
					 ps->RunModeStat=1;
					 ps->Key.Mode=0;
					 ps->Function=0;  
				}
				else if(ps->Button==0x0004)  //按键值处理
				{
				  //to do 上一页
					ps->Key.Opt=4;
		      ps->Function=0;		//·翻译指示	 
          ps->RunModeStat=1;					
          ps->Key.Mode=0;					
				}
				else if(ps->Button==0x0005)  //按键值处理
				{
					//to do 下一页
					ps->Key.Opt=5;
					 ps->Function=1;		//·翻译指示	 
     ps->RunModeStat=1;	
					ps->Key.Mode=0;
				}
				else if(ps->Button==0x0006)  //按键值处理
				{
					//to do 计算
					ps->Key.Opt=6;
					ps->Key.Mode=0;
					if(ps->YC_Type==0) //烟尘
					ps->RunMode=0x7100;  
					else
					{
					 	ps->RunMode=0x7400; //油烟
					}
				}
				else if(ps->Button==0x0007)  //按键值处理
				{
					//to do 文件管理
					ps->Key.Opt=7;
					ps->Key.Mode=0;
					ps->RunMode=0x7200;
				}
				else if(ps->Button==0x0008)  //按键值处理
				{
					//to do 打印选项
					ps->RunMode=0x7300;
					ps->Key.Mode=0;
					ps->Key.Opt=8;
				}
				else if(ps->Button==0x0009)  //按键值处理
				{
					//to do 打印
				//	ps->RunMode=0x7400;
					ps->Key.Mode=0;
					ps->Key.Opt=9;
					if(AT_GetData()->Mode==0)     //在透传模式
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
	                    strcpy((char *)ps->Buffer,"没连接打印机!");//清提示20200519
	                    Screen_RefreshDataMulti(0x2E20,ps->Buffer,15*2);
					}
					
				}
	ScreenChangeDataInit(ps);
				
	} 
}
//----
/*20200521, 2020-05-23
	函数名：void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测量烟气-烟气查询-计算设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25,修改超时处理, modify by alfred.
*/
void Button_YCCY_YCCX_JS_YC_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	FourBytes four;
	//Button_DataStructure *p = Button_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
		
					if(ps->Key.Mode==1) //烟尘净重[19]
					{  
					ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,1000.00,0.0);
					four.FloatData=DisBuff[ps->Key.Mode-1];
					ps->DisValue[19]=ps->YCJZ=four.FloatData;
					}
					else if(ps->Key.Mode==2)  //含氧量[20]   20200526
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
							ps->DisValue[38]=1000000;  //过剩系数 ,错误
							ps->DisValue[20]=ps->HYL=21; //含氧量
						}										
					}
					else if(ps->Key.Mode==3)   //负荷系数[10]
					{   ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,2.0,0.2);
					    four.FloatData=DisBuff[ps->Key.Mode-1];
						ps->DisValue[10]=ps->FHXS=four.FloatData;
					}								
					else if(ps->Key.Mode==4)  //折算系数[9]  20200526
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
					else if(ps->Key.Mode==5)  //基准含氧量[35]  20200526
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
					else if(ps->Key.Mode==6) //导入文件号
					{
						ps->DRWJH=four.FloatData;
					}
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}
			}
			else
			{   			
				ScreenTowOpButton(ps,1,0,0);  
						if(ps->Button==0x0Aff) //确认
						{
							
							
				// p->DisValue[0]=p->YCJZ;               //烟尘净重[19]
				// p->DisValue[1]=p->HYL;                 //含氧量[20]
				// p->DisValue[2]=p->FHXS;                //负荷系数[10]
				// p->DisValue[3]=p->ZSXS;                //折算系数[9]
				// p->DisValue[4]=p->JZHYL;               //基准含氧量[35]
				// p->DisValue[5]=(float)p->DRWJH;        //导入文件号
							  if(ps->Key.Mode==1) //烟尘净重[19]
								{
									ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,1000,0.0);
							        four.FloatData=DisBuff[ps->Key.Mode-1];
									ps->DisValue[19]=ps->YCJZ=four.FloatData;
								}
								else if(ps->Key.Mode==2)  //含氧量[20]
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
										ps->DisValue[38]=0;  //过剩系数
									}
									
								}
								else if(ps->Key.Mode==3)   //负荷系数[10]
								{   ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,2.0,0.2);
							        four.FloatData=DisBuff[ps->Key.Mode-1];
									ps->DisValue[10]=ps->FHXS=four.FloatData;
								}
							
								else if(ps->Key.Mode==4)  //折算系数[9]
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
								else if(ps->Key.Mode==5)  //基准含氧量[35]
								{
									ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,20,0.0);
							        four.FloatData=DisBuff[ps->Key.Mode-1];
									ps->DisValue[35]=ps->JZHYL=four.FloatData;
									if(ps->JZHYL!=0)
									ps->DisValue[9]=ps->ZSXS=21.0f-(21.0f/ps->JZHYL);
									else
									ps->DisValue[9]=ps->ZSXS=0;
									
								}
								else if(ps->Key.Mode==6) //导入文件号
								{
									
									ps->DRWJH=four.FloatData;
									
								}
							
							
							 // SaveFlaotToFlash(four.FloatData,FFLASHIS_AdrYCCJ_YCWJCX_JSYCZL+2*(ps->Key.Mode-1));//保存数据 
							
							
						} 
						ScreenBackFirstOpt(ps);	       //退出检测
						ScreenEnterNumberText(ps,2,10); //数值输入
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
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnterDisBuffer(ps,&four.FloatData,0,SdFile_GetData()->GasLinkFileMax,1);
					ps->DRWJH = four.FloatData;
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);    //导入文件号
				if(ps->Button==0x0Aff) //确认
				{
					ScreenConfirmEnterDisBuffer(ps,&four.FloatData,0,SdFile_GetData()->GasLinkFileMax,1); 
					ps->DRWJH = four.FloatData;
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,0,10); //数值输入
			}
			break;
		}						
	}
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x7000);
			
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x0006)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘					
					ScreenTowOptChose(ps,ps->Button);	
				
				}
				else if(ps->Button==0x0007)  //按键值处理
				{
				  //to do 导入数据
					if(GetDustJS_INpuData(ps->DRWJH))  //导入烟气文件号
					{
					ps->RunModeStat=1;
//					ps->LoadFlag=1;  // modify by alfred, 2020.07.08
						Dis_Tips(ps,(u8 *)"导入成功",0x01C0);
					}
					else
					{
						Dis_Tips(ps,(u8 *)"导入失败",0x01C0);
					}

					ps->Key.Opt=7;
				}
				else if(ps->Button==0x0008)  //按键值处理
				{
				  //to do 保存数据
						/*保存数据*/
					//	float buffer[40];
					//	SaveGasJS_InputDataLoad(buffer);
						SaveDustData(ps->DisValue,ps->FileNumber);
						if(SdFile_WriteDustRecord(&SdFile_GetData()->InterFace_data,ps->FileNumber)==0)
						{
						   Dis_Tips(ps,(u8 *)"保存成功",0x01C0);	
						}else
						{
						   Dis_Tips(ps,(u8 *)"保存失败",0x01C0);	
						}
						ps->Timer=3000;
					  ps->Key.Opt=8;
				}
				
	  ScreenChangeDataInit(ps);
					ps->StarCun=0;//不跟新文件号
	} 
}
//---
/*
	函数名：void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
  描述：	该函数为测量烟气-烟气查询-计算设置界面按键处理---油烟
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25,修改超时处理, modify by alfred.
*/
void Button_YCCY_YCCX_JS_YY_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	FourBytes four;
	//Button_DataStructure *p = Button_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
					Screen_ResetKeyDataBuffer();//清除按键值
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
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}	
			}
			else
			{   			
				ScreenTowOpButton(ps,1,0,0);  		        
				if(ps->Button==0x0Aff) //确认
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
				//	pf->Flag=1;//保存数据
				} 
					ScreenBackFirstOpt(ps);	       //退出检测
					ScreenEnterNumberText(ps,2,10); //数值输入
			}
			break;
		}			
	}		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x7000);
			        SaveDustData(ps->DisValue,ps->FileNumber);
					SdFile_WriteDustRecord(&SdFile_GetData()->InterFace_data,ps->FileNumber);
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x0003)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘					
					ScreenTowOptChose(ps,ps->Button);	
				
				}
			
				
	ScreenChangeDataInit(ps);
					ps->StarCun=0;//不跟新文件号
	} 
}
//---2020-05-23
/*
	函数名：void Button_YCCY_YCCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	描述：	该函数为烟尘采样-烟尘查询-文件管理-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：	1.2020-05-25,修改超时处理, modify by alfred.
				2. 增加烟尘导出，by alfred, 2020.06.04
*/
void Button_YCCY_YCCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,SdFile_GetData()->DustLinkFileMax,1);	// modify by alfred, 2020.07.09
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}	
			}
			else
			{  			
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
				{
					ScreenConfirmEnterDisBuffer(ps,DisBuffer,ps->Key.Mode-1,SdFile_GetData()->DustLinkFileMax,1);
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,0,7); //数值输入
			}
			break;
		}	
	}
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x7000);
			
				}
				else if(ps->Button==0x0001||ps->Button==0x0002)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘					
					ScreenTowOptChose(ps,ps->Button);	
				
				}
				else if(ps->Button==0x0003)  //按键值处理
				{
				  //to do 导出数据
					char uMachineNumBuffer[10], uSysDateBuffer[10];
					FourBytes four;
					four.U16Data[0]=pf->Data[FLASHIS_AdrCJWHYQBH];
					four.U16Data[1]=pf->Data[FLASHIS_AdrCJWHYQBH+1];
					
					sprintf((char*)uMachineNumBuffer,"%08d",four.U32Data);
					sprintf((char*)uSysDateBuffer,"%02d%02d%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday);
					
					Usb_OutDustFileInf(DisBuffer[0],DisBuffer[1], uSysDateBuffer, uMachineNumBuffer);
					ps->Key.Opt=3;
				}
						else if(ps->Button==0x0004)  //按键值处理
				{
				  //to do 打印				
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
	                    strcpy((char *)ps->Buffer,"没连接打印机!");//清提示20200519
	                    Screen_RefreshDataMulti(0x3B20,ps->Buffer,15*2);
					}
					
				}
	ScreenChangeDataInit(ps);
	} 
}
//---
/*
	函数名：void Button_YCCY_YCCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	描述：	该函数为烟尘采样-烟尘查询-打印选项-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_YCCY_YCCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}

	  }
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x7000);
			           pf->Flag=1;                         //20200522  
				}
				else if(ps->Button>=0x0002&&ps->Button<=0x21)  //按键值处理
				{
					ps->Key.Opt= ps->Button;
					u32 data=pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+1]+((pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+2])<<16);;
					u8 point=ps->Key.Opt-2;
					if(point>15)
					{      data=data>>16;
						     point-=16;
					if((data>>point)&0x00000001)
					{
					  pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+2]&=~(1<<point);//位置0
					}else
					{
					 pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+2]|=(1<<point);//位置1
					}
					}else
					{
									if((data>>point)&0x00000001)
						{
							pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+1]&=~(1<<point);//位置0
						}else
						{
						 pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+1]|=(1<<point);//位置1
						}
					}
			
				
				}if(ps->Button==0x01)
				{
				  			if(pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX]==1)
						{
							pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX]=0;//位置0
						}else
						{
						 pf->Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX]=1;//位置1
						}
				}
	ScreenChangeDataInit(ps);
	} 
}
//---
/*
	函数名：void Button_YCCY_YCCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	描述：	该函数为矫正维护-密码输入-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录： 2020-05-25, 修改接收超时处理, modify by alfred.
*/
void Button_WHJZ_MMSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)RcvBuffer_GetData()); 
					u16 PassWord=(u16)atoi((char*)&(ps->Key.Buffer[0]));
					ps->sRcvType = 0;
					ScreenBackLastOpt(ps);	       //退出检测
					//u16 jjj=YLB3330D_CJMM;//pf->Data[FLASHIS_AdrCJWHMM];    //TEST
					if(PassWord==pf->Data[FLASHIS_AdrXTWHMM])
					{
					  ps->Button=3; //密码通过
					}else if(PassWord==YLB3330D_CJMM)//pf->Data[FLASHIS_AdrCJWHMM])
					{
					   ps->Button=5; //超级密码正常
					}
						else
					{
					   ps->Button=4; //密码错误
					}	
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}																
			}
			else
			{					
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
				{
					u16 PassWord=(u16)atoi((char*)&(ps->Key.Buffer[0]));
					ScreenBackFirstOpt(ps);	       //退出检测
					if(PassWord==pf->Data[FLASHIS_AdrXTWHMM])
					{
					  ps->Button=3; //密码通过
					}else if(PassWord==YLB3330D_CJMM)//pf->Data[FLASHIS_AdrCJWHMM])
					{
					   ps->Button=5; //超级密码正常
					}
					else
					{
					   ps->Button=4; //密码错误
					}
					break;
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,0,5); //数值输入
			}
			break;
		}			 						
	}
		
	 { //屏幕按键处理
		 ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,52);
			
				}
				else if(ps->Button==0x0001)  //按键值处理
				{
						if(ps->Key.bOutsideKeyboard != true)
									Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘 键盘控值分别为0XA1~0XA5					
				   ScreenTowOptChose(ps,ps->Button);	
						
				}
				else if(ps->Button==0x0003)  //按键值处理
				{
					 if(ps->Warning==5)
					 { 
					   ps->RunMode=0x8200;	//进入超级维护
					 }else
					 {
					  ps->RunMode=0x8100;	
					 }
				  
						
				}else if(ps->Button==0x0004)  //按键值处理
				{
				   ps->Warning=1;	
						
				}else if(ps->Button==0x0005)  //按键值处理
				{
				   ps->Warning=2;	
						
				}
	ScreenChangeDataInit(ps);
	} 
}
//---
/*
	函数名：void Button_WHJZ_WHXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	描述：	该函数为矫正维护-维护选择-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：	1. 修改进入密码设置页面前先将显示密码的显示区域清空, modify by alfred, 2020.06.19.
*/
void Button_WHJZ_WHXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	//Button_DataStructure *p = Button_GetData();
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{

		case 0:
		{//没有选中二级对象
			ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}

	}
		
	{ //屏幕按键处理
		ps->OldMode=ps->RunMode;
		if(ps->Button==0x0AFe||ps->Button==Max_Opt)
		{
			ScreenBackLastPag(ps,52);

		}
		else if(ps->Button>=0x0001&&ps->Button<Max_Opt)  //按键值处理
		{
			ps->RunMode=0x8100+0x10*ps->Button;	
			if(ps->RunMode==0x8130)
			{	//如果下一步跳到密码维护页面，先清容密码区的显示，使进入页面时密码区显示空白。
				ps->DisValue[0] = -1;
				ps->DisValue[1] = -1;
				ps->DisValue[2] = -1;
				Slave_GetData()->E28.ADRRset=Slave_GetData()->E28.ADRRRD24G;   //先从副板读出无线的地址
			    Slave_GetData()->E28.CHset=Slave_GetData()->E28.CHRD24G;       //先从副板读出无线的信道
			}
			
			if(ps->RunMode==0x8140)   //特别页变量处理
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
		ScreenChangeDataInit(ps);    //公共处理
	} 
}
//--2020-05-17-//
/*20200526
	函数名：void Button_WHJZ_WHXZ_STSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-系统设置-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录： 2020-05-25, 修改接收超时处理, modify by alfred.
*/
void Button_WHJZ_WHXZ_STSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	FourBytes four;
//	FRESULT res;

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{

		case 0:
		{//没有选中二级对象
			ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
					Screen_ResetKeyDataBuffer();//清除按键值
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
					if(ps->Key.Mode==6)   //由折算系数求基准含氧量  20200526
					{
						four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_XTSZ_ZSXS);  //获取折算系数
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
					else if(ps->Key.Mode==7)  //由基准含氧量求折算系数 20200526
					{
						four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_XTSZ_JZYHL);  //获取基准含氧量
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
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}		
			}					
			else
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
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
					if(ps->Key.Mode==6)   //由折算系数求基准含氧量  20200526
					{
					 four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_XTSZ_ZSXS);  //获取折算系数
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
					else if(ps->Key.Mode==7)  //由基准含氧量求折算系数 20200526
					     {
                         four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_XTSZ_JZYHL);  //获取基准含氧量
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
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,2,5); //数值输入
			}
			 break;			
	  }	
	}
		
	{ //屏幕按键处理
		ps->OldMode=ps->RunMode;
		if(ps->Button==0x0AFe||ps->Button==Max_Opt)
		{
			ScreenBackLastPag(ps,0x8100);
			pf->Flag=1;                               //20200526
		}
		else if(ps->Button>=0x0001&&ps->Button<=0x00007)  //按键值处理
		{
			if(ps->Button==0x0002)
			{ 
				ps->RunMode=0x8111;
				Flux_StarSampling('C',1.0);   //进入启动C泵   20200525
			
			}
			else if(ps->Button==0x0003)                   //20200525
			{
				if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x02))==0)
				{
					pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]|=(0x02); //20200526
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘 键盘控值分别为0XA1~0XA5						 
					ScreenTowOptChose(ps,ps->Button);	
				}else
				{
					pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&=~(0x02);  //20200526
				}
			}
			else
			{
				if(ps->Key.bOutsideKeyboard != true)
				Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘 键盘控值分别为0XA1~0XA5						 
				ScreenTowOptChose(ps,ps->Button);	
			}
		}
		else if(ps->Button==0x0008)  //按键值处理
		{
			//to do 烟尘文件删除 
			if(SdFile_DeleteDustFiles() == FR_OK)	//如果删除成功，屏幕打印提示信息, add by alfred, 2020.07.13
			{
				ps->gSysConfigWaringBits.BitField.m_uDustFile = 1;
			}				
			ps->Key.Opt=ps->Button;		
			ps->Key.Mode=0;ps->RunModeStat=1;	
		}
		else if(ps->Button==0x0009)  //按键值处理
		{
			// to do 烟气文件删除
			if(SdFile_DeleteGasFiles() == FR_OK)	//如果删除成功，屏幕打印提示信息, add by alfred, 2020.07.13
			{
				ps->gSysConfigWaringBits.BitField.m_uGasFile = 1;
			}
			else
			{
				ps->gSysConfigWaringBits.BitField.m_data0 = 1;	//文件删除失败
			}			
			ps->Key.Opt=ps->Button;	
			ps->Key.Mode=0;	ps->RunModeStat=1;					
		}
		else if(ps->Button==0x000A)  //按键值处理
		{
			// to do 工况文件删除
			if(SdFile_DeleteWorkFiles() == FR_OK)	//如果删除成功，屏幕打印提示信息, add by alfred, 2020.07.13
			{
				ps->gSysConfigWaringBits.BitField.m_uWorkFile = 1;
			}
			else
			{
				ps->gSysConfigWaringBits.BitField.m_data0 = 1;	//文件删除失败
			}			
			ps->Key.Opt=ps->Button;	
			ps->Key.Mode=0;	ps->RunModeStat=1;					

		}
		else if(ps->Button==0x000B)  //按键值处理
		{
			// to do 恢复出厂设置
			Flashisp_FactoryReset(0);   //20200524
			Flashisp_Restore();
			ps->Key.Opt=ps->Button;		
			ps->Key.Mode=0;ps->RunModeStat=1;	
		}
		else if(ps->Button==0x000C)  //按键值处理
		{
			// to do 删除所有文件
			//SdFile_DeleteGasFiles(); //20200524
			//SdFile_DeleteWorkFiles();      //20200524
			//SdFile_DeleteDustFiles();//20200524
			//SdFile_DeleteFlueFiles();
			if((SdFile_DeleteGasFiles() == FR_OK)&&(SdFile_DeleteWorkFiles() == FR_OK)&&(SdFile_DeleteDustFiles() == FR_OK)&&(SdFile_DeleteFlueFiles() == FR_OK))	//如果删除成功，屏幕打印提示信息, add by alfred, 2020.07.13
			{
				ps->gSysConfigWaringBits.BitField.m_uAllFile = 1;
			}
			else
			{
				ps->gSysConfigWaringBits.BitField.m_data0 = 1;	//文件删除失败
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
	函数名：void Button_WHJZ_WHXZ_STSZ_CO_SO2_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-系统设置-设置界面-CO-SO2校准按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录： 2020-05-25, 修改接收超时处理, modify by alfred.
*/
void Button_WHJZ_WHXZ_STSZ_CO_SO2_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();

	float dbuf;
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
						Screen_ResetKeyDataBuffer();//清除按键值
						strcpy((char*)ps->Key.Buffer,(char*)RcvBuffer_GetData()); 									
						ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,100.00,0);
						pf->Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1+ps->Key.Mode-1]=DisBuff[ps->Key.Mode-1]*100;
						ps->sRcvType = 0;	
						ScreenBackLastOpt(ps);
					}
					else if(ps->Button==0x0Aff)
					{
						ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;
						ps->uEnableRecDataTimer = 1;  //如果按下OK键，启动1.5S定时器检查超时			
					}		
				}
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Key.Mode>0&&ps->Key.Mode<=10)
				{
					if(ps->Button==0x0Aff) //确认
					{
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1+(ps->Key.Mode-1),1,10000,0);				
					} 
					ScreenEnterNumberText(ps,0,5); //数值输入
				}else if(ps->Key.Mode>10)
				{
					ScreenConfirmEnterDisBuffer(ps,DisBuff,ps->Key.Mode-1,100.00,0);
					ScreenEnterNumberText(ps,2,5); //数值输入
					pf->Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1+ps->Key.Mode-1]=DisBuff[ps->Key.Mode-1]*100;					
				}			
				ScreenBackFirstOpt(ps);	       //退出检测
			}							
			 break;			
		}			
	}
		
	 { //屏幕按键处理
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)   //返回上一页
				{
					 ScreenBackLastPag(ps,0x8110);
			          Flux_StopSampling('C');               //停止C泵20200525
					  //把第六个修正付给FLASHIS_AdrWHJZ_XTSZ_CO_SO2外面用
				
					  dbuf=ps->DisValue[5+10];
					  SaveFlaotToFlash(dbuf,FLASHIS_AdrWHJZ_XTSZ_CO_SO2);  //保存获得的值
				}
				else if(ps->Button>=0x0003&&ps->Button<=0x000016)  //按键值处理
				{
						if(ps->Key.bOutsideKeyboard != true)
						 Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button-2);//弹出软件键盘
						 ScreenTowOptChose(ps,ps->Button-2);	
					     ps->Key.Opt=ps->Button;	
					     ps->RunModeStat=1;	
				}
			  else if(ps->Button==0x0001)  //切换单位
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
				else if(ps->Button==0x0002)  //选用
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
	函数名：void Button_WHJZ_WHXZ_LLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-流量校准-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录： 2020-05-25, 修改接收超时处理, modify by alfred.
*/
void Button_WHJZ_WHXZ_LLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
					Screen_ResetKeyDataBuffer();//清除按键值
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
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}										
			}	
			else
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
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
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,2,6); //数值输入
			}
		 break;			
		}			
	}
		
	 { //屏幕按键处理
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x8100);
					//退出前关闭
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
				else if(ps->Button>=0x0002&&ps->Button<=0x0000B)  //按键值处理
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
				  	//选择后停止泵
					   ps->Function=0;
					   Flux_StopSampling('B');
					   Flux_StopSampling('C');
					   ps->PauseOrStop=0;	
					
				}	else if(ps->Button==0x000c||ps->Button==0x0001||ps->Button==0x0000d)  //按键值处理
				{
							if(ps->Key.bOutsideKeyboard != true)
							Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘
							ScreenTowOptChose(ps,ps->Button);	
							ps->RunModeStat=1;
					
				}
				else if(ps->Button>=0x0022&&ps->Button<=0x0002b)  //按键值处理
				{
							if(ps->Key.bOutsideKeyboard != true)
										Screen_WriteRegisterOneData(0x4f,0xA0+(ps->Button&0x000f));//弹出软件键盘
      				ScreenTowOptChose(ps,(ps->Button&0x000f));
              // pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]=ps->Key.Mode-1;					
              ps->RunModeStat=1;					
				}
			  else if(ps->Button==0x000e)  //按键值处理
				{
					//to do 参数打印  	
					ps->Key.Opt=ps->Button;		
				    ps->Key.Mode=0;ps->RunModeStat=1;	
					float buffer[15];
					buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
					buffer[1]=ps->DisValue[13];
					buffer[2]=ps->DisValue[16];
					buffer[3]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_LLJZ_YQBL01);
					for(int i=0;i<9;i++)
					buffer[4+i]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_LLJZ_YCBL01+2*i);
					
					PrintFlowPara(buffer);//打印流量参数
				}
				else if(ps->Button==0x000f)  //按键值处理
				{
				  // to do 启动
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
	函数名：void Button_WHJZ_WHXZ_LLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-密码设置-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：	1. 2020-05-25,修改超时处理, modify by alfred.
				2. 2020-05-27，修改蓝牙录入触点地址，修复维护密码不能保存的问题，修改蓝牙开关flash地址

*/
void Button_WHJZ_WHXZ_MMSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
//	static u8 uTempKeymode; 

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //外围按键处理			
		  break;
		}
		case 1:    //二极菜单的处理（数据输入）
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		{
			if(ps->Key.bOutsideKeyboard != true)  //屏幕输入
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)RcvBuffer_GetData()); 
					if(ps->Key.Mode<=3)
					{
					ScreenConfirmEnterDisBuffer(ps,DisBuff,(ps->Key.Mode-1),65535,0);
					}
					else if(ps->Key.Mode==4)   //打印蓝牙密码
					{
					ScreenConfirmEnter(ps,FLASHIS_AdrXTWHLYPDM,1,65535,0);
					}
					else if(ps->Key.Mode==5)  //APP蓝牙密码
					{								
					ScreenConfirmEnter(ps,FLASHIS_AdrXTWHAPPLYPDM,1,65535,0);
					}
					else if(ps->Key.Mode==6) //E28地址
					{	
					 ScreenConfirmEnterDisBuffer(ps,DisBuff,(ps->Key.Mode-1),65535,0);
					 Slave_GetData()->E28.ADRRset=(u16)DisBuff[5];
					 ps->g_E28_Set_StateMachine=E28_SET_STATE_MACHINE_INIT;	 //启动写E28线程
					 ps->uE28CtlState=1;
					 ps->Warning=6;	
					}
					else if(ps->Key.Mode==7) //E28信道
					{	
					 ScreenConfirmEnterDisBuffer(ps,DisBuff,(ps->Key.Mode-1),255,0);	
					 Slave_GetData()->E28.CHset=(u8)DisBuff[6];
					 ps->g_E28_Set_StateMachine=E28_SET_STATE_MACHINE_INIT;	 //启动写E28线程
					 ps->uE28CtlState=1;
					 ps->Warning=6;	
					}
					
					ps->sRcvType = 0;
					ScreenBackLastOpt(ps);	
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}			
			}		
			else                                //外围按键输入
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
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
                    else if(ps->Key.Mode==6) //E28地址
					{	
					 ScreenConfirmEnterDisBuffer(ps,DisBuff,(ps->Key.Mode-1),65535,0);
					 Slave_GetData()->E28.ADRRset=(u16)DisBuff[5];
					 ps->g_E28_Set_StateMachine=E28_SET_STATE_MACHINE_INIT;	 //启动写E28线程
					 ps->uE28CtlState=1;
					 ps->Warning=6;	
						
					}
					else if(ps->Key.Mode==7) //E28信道
					{	
					 ScreenConfirmEnterDisBuffer(ps,DisBuff,(ps->Key.Mode-1),255,0);	
					 Slave_GetData()->E28.CHset=(u8)DisBuff[6];
					 ps->g_E28_Set_StateMachine=E28_SET_STATE_MACHINE_INIT;	  //启动写E28线程
					 ps->uE28CtlState=1;
					 ps->Warning=6;	
					}
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,2,6); //数值输入
			}
		 break;			
		}					
	    }
		
	    { //屏幕按键处理一级菜单时
		        ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)  //返回上一页
				{
						ScreenBackLastPag(ps,0x8100);
			
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x00003)  //按键值处理
				{
 						if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);      
						ScreenTowOptChose(ps,ps->Button);	
					
				}
			    else if(ps->Button==0x0004)  //按键值处理
				{
					//to do 确认密码修改  	
					ps->Key.Opt=ps->Button;		
					ps->Key.Mode=0;ps->RunModeStat=1;
//					u16 ttt = pf->Data[FLASHIS_AdrXTWHMM];
					if((ps->DisValue[0]==pf->Data[FLASHIS_AdrXTWHMM])&&(ps->DisValue[1]==ps->DisValue[2]))
					{
						pf->Data[FLASHIS_AdrXTWHMM] = ps->DisValue[1];	//2020-05-26, 保存修改后的密码, modify by alfred.
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
				else if(ps->Button==0x0005)  //按键值处理
				{   //打开蓝牙与否
					if(pf->Data[FLASHIS_AdrXTSZBOX]&(1<<2))	//保存地址改为FLASHIS_AdrXTSZBOX, 2020-05-27, by Alfred
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
					ps->RunModeStat=1;//刷新数据
			  		ps->Key.Opt=ps->Button;	 
				}
	
				else if(ps->Button==0x0006)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA0+4);  
					// to do 确定
					ScreenTowOptChose(ps,4);	
			  		ps->Key.Opt=ps->Button;	 
				} 				
				else if((ps->Button==0x0007)&&(ps->uTempState!= true))  //按键处理
				{
					// to do 蓝牙初始化化
					if(pf->Data[FLASHIS_AdrXTSZBOX]&(1<<2)) //蓝牙开关打开时才初始化
					{
						ps->Key.Opt=ps->Button;		
						ps->Key.Mode=0;ps->RunModeStat=1;
						ps->Key.Opt=ps->Button;
						ps->uTempState = true; //蓝牙初始化，启动线程
						ps->Warning = 6;
					}
					else
					{
						ps->Warning = 3;	//如果蓝牙开关没打开，在屏幕上显示提示信息。
					}
				}
				else if(ps->Button==0x0008)  //按键处理
				{  //APP蓝牙选择
					if(pf->Data[FLASHIS_AdrXTWHLYPDMKG]&0x02)
					{
						 pf->Data[FLASHIS_AdrXTWHLYPDMKG]&=~(0x02);
					}else
					{
						pf->Data[FLASHIS_AdrXTWHLYPDMKG]|=(0x02);
					}
					pf->Flag=1;
					ps->Key.Mode=0;
					ps->RunModeStat=1;//刷新数据
			  		ps->Key.Opt=ps->Button;	
				} 				
				else if(ps->Button==0x0009)  //按键值处理
				{ //APP蓝牙输入
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+5);  
				       // 
					   ScreenTowOptChose(ps,5);	
			  		   ps->Key.Opt=ps->Button;	 
				} 
				else if(ps->Button==0x000A)  //按键值返回
				{   // to do APP蓝牙初始化化 ?????????????????
					ps->Key.Mode=0;ps->RunModeStat=1;
			  		ps->Key.Opt=ps->Button;	 
				}				
				else if(ps->Button==0x000B)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+6);  
				       //
					   ScreenTowOptChose(ps,6);	
			  		   ps->Key.Opt=ps->Button;	 
				} 
				else if(ps->Button==0x000C)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
						Screen_WriteRegisterOneData(0x4f,0xA0+7);  
				       // to do 确定
					   ScreenTowOptChose(ps,7);	
			  		   ps->Key.Opt=ps->Button;	 
				} 
             
	ScreenChangeDataInit(ps);
	} 
}
//----2020-05-18
/*
	函数名：void Button_WHJZ_WHXZ_NDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-浓度校准-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录： 2020-05-25, 修改接收超时处理, modify by alfred.
*/
void Button_WHJZ_WHXZ_NDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
		default :    //已进入二级菜单
		{
			if(ps->Key.bOutsideKeyboard != true)//屏幕按键
			{
				ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType != 0)
				{
					Screen_ResetKeyDataBuffer();//清除按键值
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
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}											
			}	
			else                                //外围键盘处理 
			{								
				if(ps->Key.Mode<=14)  //零点与倍率的输入
				{        
					ScreenTowOpButton(ps,1,0,0);   //二级菜单按键处理
					
					
					if(ps->Button==0x0Aff) //确认
					{
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_NDJZ_LDO2+(ps->Key.Mode-1)*2,2,2000.0,0);

						Slave_ReadFlashZOData(&Slave_GetData()->O2Concent,FLASHIS_AdrWHJZ_NDJZ_LDO2,SLAVE_BaseGas);
                        Slave_ReadFlashZOData(&Slave_GetData()->SO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDSO2,SLAVE_BaseGas);
                        Slave_ReadFlashZOData(&Slave_GetData()->NOConcent,FLASHIS_AdrWHJZ_NDJZ_LDNO,SLAVE_BaseGas);
                        Slave_ReadFlashZOData(&Slave_GetData()->NO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDNO2,SLAVE_BaseGas);
                        Slave_ReadFlashZOData(&Slave_GetData()->COConcent,FLASHIS_AdrWHJZ_NDJZ_LDCO,SLAVE_BaseGas);
                        Slave_ReadFlashZOData(&Slave_GetData()->CO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDCO2,SLAVE_BaseGas);
                        Slave_ReadFlashZOData(&Slave_GetData()->H2SConcent,FLASHIS_AdrWHJZ_NDJZ_LDH2S,SLAVE_BaseGas);

						if(ps->Key.Mode<=7)  //当零点确认后，转到倍率
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
					ScreenBackFirstOpt(ps);	       //退出检测
					ScreenEnterNumberText(ps,2,6); //数值输入
				}
			}
			break;			
		}					
	}
		
	 { //屏幕按键处理(包含外按键)
		        ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt) //退出
				{
					 ScreenBackLastPag(ps,0x8100);
					 ps->Function=0;
					 Flux_StopSampling('C');
			
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x00007)  //外按键值与屏幕按键处理
				{
					 if(ps->Key.bOutsideKeyboard != true)
					 Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘       
					 ScreenTowOptChose(ps,ps->Button);	 //进入二级与处理
				}	
				else if(ps->Button>=0x0021&&ps->Button<=0x00027)  //屏幕按键值处理(外按键不会有这几个值)
				{
                     
                      ps->Key.Opt=ps->Button&0x000f;//	 倍率选择
                      ps->RunMode=0x8141;	
			          ps->gzdselect=0;	  //进去默认不做选择
                      ps->SubCnt=	ps->Key.Opt-1;	
                      ps->DisValue[0]=100;
					  ps->DisValue[1]=1000;//标准气源
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
			  else if(ps->Button==0x0008)  //按键值处理
				{
					//to do 参数确认	 打印
					ps->Key.Opt=ps->Button;		
					ps->Key.Mode=0;ps->RunModeStat=1;
					float buffer[30];
					buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
					
					for(int i=0;i<7;i++ )
					{
						 buffer[1+i*4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_NDJZ_LDO2+2*i);  //零点
						 buffer[2+i*4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_NDJZ_BLO2_1+2*i); //中
						 buffer[3+i*4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_NDJZ_BLO2_2+2*i); //低
						 buffer[4+i*4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_NDJZ_BLO2_3+2*i); //高
					}
					PrintConcentraPara(buffer);//打印浓度数据
				}
				else if(ps->Button==0x0009)  //按键值处理
				{
				  // to do 启动
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
				else if(ps->Button==0x000A)  //按键值处理
				{
				  // to do 校零
				 Slave_Zero_QT();
				 ps->Key.Opt=ps->Button;		
				 ps->Key.Mode=0;ps->RunModeStat=1;
				} 	

	ScreenChangeDataInit(ps);
	} 
}
//----2020-05-19
/*
	函数名：void Button_WHJZ_WHXZ_NDJZ_BLSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-浓度校准-倍率设置-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录： 2020-05-25, 修改接收超时处理, modify by alfred.
*/
void Button_WHJZ_WHXZ_NDJZ_BLSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
  Slave_DataStructure* Sp=Slave_GetData();

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理	
			
		  break;
		}
		default :
		{
			if(ps->Key.bOutsideKeyboard != true)  //屏幕按键
			{
				ScreenTowOpButton_KeyControl(ps,CALC_HAVE_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType != 0)
				{
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)RcvBuffer_GetData()); 									
					if(ps->Key.Mode<=3) //确认,输入倍率值后处理
					{
						u16 Addr=	FLASHIS_AdrWHJZ_NDJZ_BLO2_1+ps->SubCnt*2+(ps->Key.Mode-1)*14;	
						ScreenConfirmEnter(ps,Addr,2,2.0,0.0);
					}
					else if(ps->Key.Mode<=6)//输入倍率值后处理
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
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}									
			}	
			else  //外按键
			{	               
				ScreenTowOpButton(ps,1,0,0);							
				if(ps->Button==0x0Aff)
				{              					
					if(ps->Key.Mode>=0x21&&ps->Key.Mode<=0x23) //确认
					{
						u16 Addr=	FLASHIS_AdrWHJZ_NDJZ_BLO2_1+ps->SubCnt*2+(ps->Key.Mode-0x21)*14;	
						ScreenConfirmEnter(ps,Addr,2,2.0,0.0);
						
						ScreenTowOptChose(ps,ps->Key.Mode+3);  //进入下一列
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
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,2,6); //数值输入
			}						
		 break;			
	  }				
	}
		
	 { //屏幕按键处理
		        ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)   //返回
				{
					 ScreenBackLastPag(ps,0x8140);
			         ps->ZjTimer=0;
					 ps->staflg=0;
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x00003)  //按键值处理
				{
                    if(ps->Button==ps->gzdselect)
					{  //再按已选择打钩,再按OK键
						 ScreenTowOptChose(ps,ps->Button+0x20);	 //进入二级与处理
					     ps->Key.Opt=ps->Key.Mode-0x20;
					}
					else
					{

					ps->Key.Opt=ps->Button;	ps->Key.Mode=0;
					ps->RunModeStat=1;
					ps->gzdselect=ps->Button;
						
					}

					
				}	
				else if(ps->Button>=0x0021&&ps->Button<=0x00026)  //按键值处理
				{
						 if(ps->Key.bOutsideKeyboard != true)
						 Screen_WriteRegisterOneData(0x4f,0xA0+(ps->Button&0x000f));//弹出软件键盘					       
						 ScreenTowOptChose(ps,(ps->Button&0x000f));
                         ps->Key.Opt=ps->Button&0x000f;//	 倍率选择
                         if(ps->Key.Opt>3) ps->Key.Opt-=3;			
					
				}
			  else if(ps->Button==0x0004)  //按键值处理
				{
					//to do 启动
					ps->Key.Opt=ps->Button;		
				  ps->Key.Mode=0;
					ps->RunModeStat=1;
					if(ps->PauseOrStop==B0)
					{ ps->ZjTimer=0;
					  ps->staflg=0;
					  ps->PauseOrStop=0;
					  Flux_StopSampling('C');//停止气泵
					  ps->TimerSec=0;
					}else
					{  ps->ZjTimer=0;
					   ps->staflg=1;
					   ps->PauseOrStop=B0;
					   Flux_StarSampling('C',1);//开始气泵 以 1L/Min采集
					   ps->TimerSec=10;
					}
        			 
				}
				else if(ps->Button==0x0005)  //按键值处理
				{ Slave_TPActualStructure * BeforGasp=&Sp->O2Concent;//倍率前
				  // to do 自动计算
				if(ps->gzdselect==1)   //选择中浓度
				{
					BeforGasp[ps->SubCnt].K=CalculationFormula_ZdK(ps->DisValue[0],ps->DisValue[3],BeforGasp[ps->SubCnt].B,BeforGasp[ps->SubCnt].K);

				  
				  SaveFlaotToFlash(BeforGasp[ps->SubCnt].K,FLASHIS_AdrWHJZ_NDJZ_BLO2_1+ps->SubCnt*2); 
				  
				}
				else if(ps->gzdselect==2)  //选择低浓度
				{
					
				 // BeforGasp[ps->SubCnt].K1=CalculationFormula_ZdK(ps->DisValue[1],ps->DisValue[4],BeforGasp[ps->SubCnt].B,BeforGasp[ps->SubCnt].K1);
				  if(ps->DisValue[3]!=0)
				  BeforGasp[ps->SubCnt].K1=ps->DisValue[1]/ps->DisValue[3];
				  else 
				  BeforGasp[ps->SubCnt].K1=1;
				  
				  SaveFlaotToFlash(BeforGasp[ps->SubCnt].K1,FLASHIS_AdrWHJZ_NDJZ_BLO2_2+ps->SubCnt*2);
				}
				else if(ps->gzdselect==3)  //选择高浓度。
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
	函数名：void Button_WHJZ_WHXZ_NDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-压力校准-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25,修改超时处理, modify by alfred.
*/
void Button_WHJZ_WHXZ_YLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
  Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
//	static u8 uTempKeymode = 0;

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
					Screen_ResetKeyDataBuffer();//清除按键值
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
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}		
			}
			else  //外围按键
			{					
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
				{
		
					if((ps->Key.Mode==2)||(ps->Key.Mode==3))
					{
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_YLJZ_DQY+(ps->Key.Mode-1)*2,2,600,400);	
					}
					else	
					{
						
					ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_YLJZ_DQY+(ps->Key.Mode-1)*2,2,600,400);	 //处理视觉零点
						
					}
					
					 Slave_ReadFlashZOData(&Slave_GetData()->DQPress,FLASHIS_AdrWHJZ_YLJZ_DQY,SLAVE_BaseCPress); //处理实制零点
                     Slave_ReadFlashZOData(&Slave_GetData()->ACPress,FLASHIS_AdrWHJZ_YLJZ_DY,SLAVE_BaseCPress);
                     Slave_ReadFlashZOData(&Slave_GetData()->AGPress,FLASHIS_AdrWHJZ_YLJZ_JY,SLAVE_BaseCPress);
                     Slave_ReadFlashZOData(&Slave_GetData()->BCPress,FLASHIS_AdrWHJZ_YLJZ_CLY,SLAVE_BaseCPress);
                     Slave_ReadFlashZOData(&Slave_GetData()->BGPress,FLASHIS_AdrWHJZ_YLJZ_JJY,SLAVE_BaseCPress);
                     Slave_ReadFlashZOData(&Slave_GetData()->CCPress,FLASHIS_AdrWHJZ_YLJZ_QLY,SLAVE_BaseCPress);

				}             							
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,2,6); //数值输入
			}
			break;			
		}
	}
		
	 { //屏幕按键处理
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x8100);
			
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x00006)  //按键值处理
				{
							if(ps->Key.bOutsideKeyboard != true)
							Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘         
							ScreenTowOptChose(ps,ps->Button);	
					
				}	
			  else if(ps->Button==0x0007)  //按键值处理
				{
					//to do 参数确认	 打印
					ps->Key.Opt=ps->Button;		
				 ps->Key.Mode=0;ps->RunModeStat=1;
					float buffer[10];
					buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
					for(int i=0;i<6;i++)
					{
					  buffer[i+1]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_YLJZ_DQY+i*2);//装载零点数据
					}
					PrintPressurePara(buffer);//打印压力参数
        			 
				}
				else if(ps->Button==0x0008)  //按键值处理
				{
				  // to do 校零
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
	函数名：void Button_WHJZ_YQCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-烟气传感器选择-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WHJZ_YQCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
   Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
	
	}
		
	 { //屏幕按键处理
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x8100);
			
				}
				else if(ps->Button>=0x0001&&ps->Button<=0x00007)  //按键值处理
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
	函数名：void Button_WHJZ_WDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-温度校准-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：	1. 2020-05-25,修改超时处理, modify by alfred.
				2. 修改菜单1~4触控显示时改变底色, 2020-05-29, modify by alfred.
				3. 修正烟温倍率B值错误保存到计温结构体里面的问题。
*/
void Button_WHJZ_WDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	//ywTempPointY1:采样点，ywTempPointY2：理论值点；
	//static u8 uMenuPosition;	
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
					Screen_ResetKeyDataBuffer();//清除按键值
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
							
						Slave_ReadFlashBLData(&Slave_GetData()->GQTem,FLASHIS_AdrWHJZ_WDJZ_BLGQ); //干球倍率 20200526
						Slave_ReadFlashBLData(&Slave_GetData()->YWTem,FLASHIS_AdrWHJZ_WDJZ_BLYW);//烟温倍率  //2020-05-30, modify by alfred.
						Slave_ReadFlashBLData(&Slave_GetData()->SQTem,FLASHIS_AdrWHJZ_WDJZ_BLSQ);//湿球倍率
					}											
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}		
			}
			else
			{				
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
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
						Slave_ReadFlashBLData(&Slave_GetData()->GQTem,FLASHIS_AdrWHJZ_WDJZ_BLGQ); //干球倍率  //20200526
						Slave_ReadFlashBLData(&Slave_GetData()->YWTem,FLASHIS_AdrWHJZ_WDJZ_BLYW);//烟温倍率	//2020-05-30, modify by alfred.
						Slave_ReadFlashBLData(&Slave_GetData()->SQTem,FLASHIS_AdrWHJZ_WDJZ_BLSQ);//湿球倍率	
					}	

				}						
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,2,6); //数值输入
			}
			break;			
		}			
	}
		
	{ //屏幕按键处理
		ps->OldMode=ps->RunMode;
		if(ps->Button==0x0AFe||ps->Button==Max_Opt)
		{
			ScreenBackLastPag(ps,0x8100);
			ps->Warning = 0;	//警告清零
			ps->uMenuPosition = 0; //光标位置清零
		}
		else if(ps->Button>=0x0001&&ps->Button<=0x0004)  //按键值处理
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
		else if(ps->Button>=0x0021&&ps->Button<=0x00024)  //按键值处理
		{			
			if(ps->Key.bOutsideKeyboard != true)
				Screen_WriteRegisterOneData(0x4f,0xA0+(ps->Button&0x000f));//弹出软件键盘 
			ScreenTowOptChose(ps,ps->Button&0x000f);
			ps->Warning = 0;
			ps->uMenuPosition = ps->Key.Opt;
			
		}	else if(ps->Button>=0x0031&&ps->Button<=0x00032)  //按键值处理
		{
			if(ps->Key.bOutsideKeyboard != true)
				Screen_WriteRegisterOneData(0x4f,0xA0+(ps->Button&0x000f)+4);//弹出软件键盘              
			ScreenTowOptChose(ps,(ps->Button&0x000f)+4);	
			ps->Key.Opt=ps->Button&0x000f;
			ps->Warning = 0;
			ps->uMenuPosition = ps->Key.Opt;
		}
		else if(ps->Button==0x0005)  //按键值处理
		{
			//to do 参数	 打印
			ps->Key.Opt=ps->Button;		
			ps->Key.Mode=0;ps->RunModeStat=1;
			float buffer[10];
			buffer[0]=GetU32dataFormFlash2Word(FLASHIS_AdrCJWHYQBH);
			buffer[1]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_WDJZ_LDGQ);
			buffer[2]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_WDJZ_BLGQ);
			buffer[3]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_WDJZ_LDSQ);
			buffer[4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_WDJZ_BLSQ);
			buffer[5]=GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_WDJZ_LDJW);
			PrintTemperaturePara(buffer);//打印压力参数			 
		}
		else if(ps->Button==0x0006)  //按键值处理
		{
						  // to do 确定
			ps->Key.Opt=ps->Button;		
			ps->Key.Mode=0;ps->RunModeStat=1;
			if(ps->Key.LastOpt == 0x01) ps->uMenuPosition = 1;

			TemperatureCaribration(&ps->Warning,ps->uMenuPosition);
			if(ps->Warning == 0xAF) 	//标定完成
			{	
				pf->Flag = 1;
				ps->RunModeStat=1;  //更新静态数据, 当按了确认或返回时ps->RunModeStat=1更新页面数据。 
				ps->Button=0XffFF;
			}
		} 									
	ScreenChangeDataInit(ps);
	} 
}





//----2020-05-20
/*
	函数名：void Button_WHJZ_DCBD_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-电池标定-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25,修改超时处理, modify by alfred.
*/
void Button_WHJZ_DCBD_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 
					ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_DCJZ_BL+(ps->Key.Mode-1)*2,2,2.0,0.2);
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
					Battery_ReadFlashData();  //20200528
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
					
				}		
			}
			else
			{				
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
				{
						ScreenConfirmEnter(ps,FLASHIS_AdrWHJZ_DCJZ_BL+(ps->Key.Mode-1)*2,2,2.0,0.2);
					    Battery_ReadFlashData();  //20200528
				}						
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,2,6); //数值输入
			}
		 break;			
		}		
	}
		
	 { //屏幕按键处理
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,0x8100);
			
				}
				else if(ps->Button==0x0001)  //按键值处理
				{
							if(ps->Key.bOutsideKeyboard != true)
										Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘         
						 ScreenTowOptChose(ps,ps->Button);	
					
				}
				
	ScreenChangeDataInit(ps);
	} 
}
//----2020-05-21
/*
	函数名：void Button_WHJZ_CJWH_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-超级维护-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25,修改超时处理, modify by alfred.
*/
void Button_WHJZ_CJWH_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
     Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();


	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
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
				if(ps->Button==0x0Aff) //确认
				{
				ScreenConfirmEnter(ps,FLASHIS_AdrCJWHYQBH+(ps->Key.Mode-1)*2,3,9999999999,1);
				}						
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,0,10); //数值输入
			}
		 break;			
		}			
	}
		
	 { //屏幕按键处理
		     ps->OldMode=ps->RunMode;
				if(ps->Button==0x0AFe||ps->Button==Max_Opt)
				{
					 ScreenBackLastPag(ps,52);
			
				}
				else if(ps->Button==0x0001)  //按键值处理
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘        
					ScreenTowOptChose(ps,ps->Button);	
					
				}else if(ps->Button==0x0002)  //系统备份
				{
					ps->Key.Opt=ps->Button;
					Flashisp_Backup();         //系统备份
					ps->Function=1;
					
				}else if(ps->Button==0x0003)  //转到气体传感器配置
				{      
					ps->RunMode=0x8210;	
					ps->Key.Opt=ps->Button;
					ps->LastChoice=0xffff;
				}	
	      ScreenChangeDataInit(ps);
	} 
}


/******************  系统设置页面写入日期  ********************/
void SystemSetPageWriteDate(Screen_DataStructure *ps)
{
//						ps->Key.Mode=2;
						ps->Key.TwoOpt=0;
						u8 date[15];
						Screen_ResetKeyDataBuffer();//清除按键值
						strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer);
				    ps->Clock.Clock.tm_year=Calculate_CharToU16((&ps->Key.Buffer[0]),4)-SCREEN_YearBase; //修改日期
				    if(ps->Clock.Clock.tm_year>138) ps->Clock.Clock.tm_year = 138;
				    ps->Clock.Clock.tm_mon=Calculate_CharToU16((&ps->Key.Buffer[4]),2)-1;
				    if(ps->Clock.Clock.tm_mon >11)ps->Clock.Clock.tm_mon = 11;
				    ps->Clock.Clock.tm_mday=Calculate_CharToU16((&ps->Key.Buffer[6]),2);
				   if(ps->Clock.Clock.tm_mday >31)ps->Clock.Clock.tm_mday = 31;
				    Screen_SetClock(&(ps->Clock.Clock));
				    Screen_ResetKeyDataBuffer();//清除按键值
				    sprintf((char*)date,"%02d:%02d:%02d",ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
						strcpy((char*)ps->Key.Buffer,(char*)date);
				    
//						ps->Button = 0x0002; //设置完日期后模拟点击了时间输入框，弹出软键盘继续设置时间。
						ps->Key.Mode=0;
						ps->RunModeStat=1;//刷新数据
						ps->Button=0xffff;
						ps->Timer=1000;				
					 
}

/******************  系统设置页面写入时间  ********************/
void SystemSetPageWriteTime(Screen_DataStructure *ps)
{
						Screen_ResetKeyDataBuffer();//清除按键值
						strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer);
						ps->Clock.Clock.tm_hour=Calculate_CharToU16((&ps->Key.Buffer[0]),2); //修改时间
						ps->Clock.Clock.tm_min=Calculate_CharToU16((&ps->Key.Buffer[2]),2);
						ps->Clock.Clock.tm_sec=Calculate_CharToU16((&ps->Key.Buffer[4]),2);
						ps->Clock.Clock.tm_mon-=1;
						if(ps->Clock.Clock.tm_mon >11)ps->Clock.Clock.tm_mon = 11;
						Screen_SetClock(&(ps->Clock.Clock));
					 
						ps->Timer=1000;
						ps->Key.Mode=0;
						ps->RunModeStat=1;//刷新数据
						ps->Button=0xffff;
}



//----
/*
	函数名：void Button_WHJZ_CJWH_QTCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-超级维护-气体传感器选择-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：2020-05-25,修改超时处理, modify by alfred.
*/
void Button_WHJZ_CJWH_QTCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
  FourBytes four;
	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
		   //进入了安装日期
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:		
			  //进入了量程
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:		

		{//二级对象日期设置	
			if(ps->Key.bOutsideKeyboard != true)
			{  //屏幕按键(包含接收屏幕键盘)
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ScreenBackLastOpt(ps);
				}
				if(ps->sRcvType !=0)
				{
					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)uScreenRcvBuffer); 

					if(ps->Key.Mode<=7)  //输入的数据是日期
					{
					pf->Data[FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2+1]=Calculate_CharToU16((&ps->Key.Buffer[0]),4);
					pf->Data[FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2]=(Calculate_CharToU16((&ps->Key.Buffer[4]),2)<<8)+Calculate_CharToU16((&ps->Key.Buffer[6]),2);
					}
					else                //输入的数据是量程
					{
						ScreenConfirmEnter(ps,FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2,3,20000,0);
					}
					ScreenBackLastOpt(ps);
					ps->sRcvType = 0;
					pf->Flag = 1;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}	
			}
			else
			{  	//先获取键盘按键值
				if(ps->Key.Mode<=7)  //在不同的二级菜单里,数据显示可能不同
				{
				ScreenTowOpButton(ps,1,9,1);  //可能用方向键，所以要加下面几个语句
				if(ps->Key.TwoOpt==4&&ps->Key.KeyDir==2) ps->Key.TwoOpt=5;
				else if(ps->Key.TwoOpt==4) ps->Key.TwoOpt=3;
					
				if(ps->Key.TwoOpt==7&&ps->Key.KeyDir==2) ps->Key.TwoOpt=8;
				else if(ps->Key.TwoOpt==7) ps->Key.TwoOpt=6;
				}
				else
				{
						ScreenTowOpButton(ps,1,9,0);
				}
				
				if(ps->Button==0x0Aff) //确认按键值处理
				{
						if(ps->Key.Mode<=7)  //输入的数据是日期
					{
					pf->Data[FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2+1]=Calculate_CharToU16((&ps->Key.Buffer[0]),4);
					pf->Data[FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2]=(Calculate_CharToU16((&ps->Key.Buffer[5]),2)<<8)+Calculate_CharToU16((&ps->Key.Buffer[8]),2);
				  }
					else
					{
						ScreenConfirmEnter(ps,FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2,3,20000,0);
					}
				}										
				ScreenBackFirstOpt(ps);	       //退出按键值处理
				if(ps->Key.number>=0x30&&ps->Key.number<=0x39) //数字按键值处理
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
	{ //屏幕按键处理
				ps->OldMode=ps->RunMode;
			switch(ps->Button)
			{		
				case 0x0AfE:
				case 0x008:   //返回
					{
					ScreenBackLastPag(ps,0x8200);
							break;
					 }
				case 0x0009:    //日期   2020-09-03  LIN
				case 0x000a:
				case 0x000b:
				case 0x000c:
				case 0x000d:
				case 0x000e:
				case 0x000f:
				{
					if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button-8);//弹出软件键盘 					
					ScreenTowOptChose(ps,(ps->Button-8));  //并进入二级菜单:1~7
					ps->Key.Opt=ps->Button;  //焦点是9~f
					sprintf((char*)ps->Key.Buffer,"%02d-%02d-%02d",pf->Data[FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2+1],pf->Data[FLASHIS_AdrO2AZSJ+(ps->Key.Mode-1)*2]>>8,pf->Data[FLASHIS_AdrO2AZSJ+(ps->Key.Mode)*2]&0x00FF);			
					break;
				}
				case 0x0001:    //选择
				case 0x0002: 
                case 0x0003: 
                case 0x0004: 	
			    case 0x0005: 	
                case 0x0006: 
                case 0x0007: 					
				{
		      	 	ps->Key.Opt=ps->Button; //焦点是1~7
				      ps->Key.Mode=0;  //一级菜单,无二级菜单
					    ps->RunModeStat=1;
					    u8 Chose=(pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ])>>8;
					    if((Chose&(1<<(ps->Key.Opt-1)))==0)
							{
							  Chose|=(1<<(ps->Key.Opt-1));
								//打钩时顺便进入二级菜单
								ScreenTowOptChose(ps,(ps->Button-1+8));  //二级菜单是8~14
								ps->Key.Opt=ps->Button; //焦点是1~7
							}else
							{
							 Chose&=~(1<<(ps->Key.Opt-1));
							}
							pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ]=(Chose<<8)+(pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ]&0x00ff);
					break;
				}
				case 0x0010:    //量程输入
				case 0x0011: 
                case 0x0012: 
                case 0x0013: 	
				case 0x0014: 	
                case 0x0015: 
        case 0x0016: 	
        {
	        if(ps->Key.bOutsideKeyboard != true)
					Screen_WriteRegisterOneData(0x4f,(0xA0+(ps->Button-0x10)+8));//弹出软件键盘 					
					ScreenTowOptChose(ps,(ps->Button-0x10+8));  //二级菜单是8~14
                    ps->Key.Opt=ps->Button-0x10+1;   //焦点是1~7
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
	函数名：void Button_KJ_MMSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	描述：	该函数为开机-密码输入-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录： 2020-05-25, 修改接收超时处理, modify by alfred.
*/
void Button_KJ_MMSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
{
   	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	u16 PassWord;

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  	ScreenFirstOpButton(ps,Max_Opt,isNumberChose); //底层处理			
			break;
		}
		
     	case 1:	
			if(ps->Key.bOutsideKeyboard != true)
			{
				ScreenTowOpButton_KeyControl(ps,CALC_NO_POINT,CALC_NO_SIGNS);
			
				if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
				{     
					ps->Key.Mode=0;
					ps->RunModeStat=1;//刷新数据
					ps->Button=0xffff;
				}
			
				if(ps->sRcvType !=0)
				{
					PassWord = (u16)atof((char*)&(uScreenRcvBuffer[0]));	
					ScreenBackLastOpt(ps);
					if(PassWord==GetKjMm())
					{
					  	ps->Button=3; //密码通过
					}
					else
					{
					   	ps->Button=4; //密码错误
					}
					ps->sRcvType = 0;
				}
				else if(ps->Button==0x0Aff)
				{
					ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
					ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
				}				
			}
			else
			{
				ScreenTowOpButton(ps,1,0,0);  
				if(ps->Button==0x0Aff) //确认
				{
					PassWord=(u16)atoi((char*)&(ps->Key.Buffer[0]));
					ScreenBackFirstOpt(ps);	       //退出检测
					if(PassWord==GetKjMm())
					{
					  	ps->Button=3; //密码通过
					}
					else
					{
					   	ps->Button=4; //密码错误
					}
					break;
				} 
				ScreenBackFirstOpt(ps);	       //退出检测
				ScreenEnterNumberText(ps,0,5); //数值输入
			}
			break;
	}
		
	{ //屏幕按键处理
		ps->OldMode=ps->RunMode;
		if(ps->Button==0x0AFe||ps->Button==Max_Opt)
		{
			 ScreenBackLastPag(ps,101);
		}
		else if(ps->Button==0x0001)  //按键值处理
		{
			if(ps->Key.bOutsideKeyboard != true)
				Screen_WriteRegisterOneData(0x4f,0xA0+ps->Button);//弹出软件键盘 键盘控值分别为0XA1~0XA5						   
		   	ScreenTowOptChose(ps,ps->Button);			
		}
		else if(ps->Button==0x0003)  //按键值处理
		{
			ps->RunMode=14;	//进入主界面
			//顺便写STM8 标记位为0
			Slave_GetData()->Stm8Data.rw_ctrl=1;
			Slave_GetData()->Stm8Data.Data=0;
		}
		else if(ps->Button==0x0004)  //按键值处理
		{
		   	ps->Warning=1;	
		}
		ScreenChangeDataInit(ps);
	} 
}


