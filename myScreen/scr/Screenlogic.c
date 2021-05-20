#include "Screen.h"
#include "Button.h"
#include "ScreenDwFunction.h"
#include "CalculationFormula.h"
#include "ScreenButton.h"
#include "ScreenDisCalculate.h"
#include "Screenlogic.h"
#define BACK_BUTTON Screen_PastePicture(0x0800,19,(718<<16)+370,(787<<16)+440,(718<<16)+370);
static float FileValue=00000001;//文件号 用于文件查询

extern float gMinuteData[60][8];	//分钟数据,用于显示暂存
extern u8 gSystemTime[60][10];		//分钟数据时间


//---
/*
	函数名：void Screen_programUpdata(Screen_DataStructure *p)
	描述：	程序升级界面处理函数
*/
void Screen_programUpdata(Screen_DataStructure *p)
{
   if(p->Page==2) //
	 {
	    ScreenFirstOpButton(p,2,1);//获取按键
		 if(p->Button==0x0002&p->Key.Mode==0) //否按键
		 {
			 p->Key.Opt=2;
		     p->RunMode=11; 	//退出升级界面
		 }
		 else if(p->Button==0x0001&&p->Key.Mode==0) //是按键
		 {
			 p->Key.Opt=1;
			 Screen_ResetDataBuffer();
			 sprintf((char*)p->Buffer,"Star programUpdata");//软件版本
			 Screen_RefreshDataMulti(0x0100,p->Buffer,strlen((const char*)p->Buffer));
			 p->Key.Mode=1; 
		 }
		 if(p->Key.LastOpt!=p->Key.Opt) //选择更新
		 {
		   
			  switch(p->Key.Opt)
				{
					case 01: Screen_PastePicture(0x0800,0x0003,0x00D900f5,0x01480137,0x00D900f5);break;
					case 02: Screen_PastePicture(0x0800,0x0003,0x01e000f5,0x02530137,0x01e000f5);break;
					default: p->Key.Opt=2;Screen_PastePicture(0x0800,0x0003,0x01e000f5,0x02530137,0x01e000f5); break;
				}
				p->Key.LastOpt =p->Key.Opt;
		 }
		 if(p->Button!=0xffff) //按键状态复位
		 {
		    p->Button=0xffff;
		 }
		 if(p->Key.Mode==1)//开始升级
		 {
		    //进入程序升级程序
		 }
	 }
}
//---

/*20200529
	函数名：void Screen_programUpdata(Screen_DataStructure *p)
	描述：	程序升级界面处理函数
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
				        {//软件版本
								Screen_ResetDataBuffer();
								GetU16dataFormFlash2Word(&four,FLASHIS_AdrCJWHRJBB,fP);
								sprintf((char*)p->Buffer,"V%.2f        ",four.FloatData);//软件版本
								Screen_RefreshDataMulti(0x0400,p->Buffer,12*2);
								//仪器编号
								Screen_ResetDataBuffer();
								GetU16dataFormFlash2Word(&four,FLASHIS_AdrCJWHYQBH,fP);
					     	    sprintf((char*)p->Buffer,"%u           ",four.U32Data);//仪器编号
								Screen_RefreshDataMulti(0x0410,p->Buffer,12*2);
								 //仪器运行时间
								Screen_ResetDataBuffer();
								GetU16dataFormFlash2Word(&four,FLASHIS_AdrYQYXSJ,fP);
								Screen_ResetDataBuffer();
								sprintf((char*)p->Buffer,"仪器运行:%uh%umin          ",four.U32Data/3600,four.U32Data%3600/60);//仪器编号
								Screen_RefreshDataMulti(0x0500,p->Buffer,12*2);
				         //尘泵运行时间
								GetU16dataFormFlash2Word(&four,FLASHIS_AdrCBYXSJ,fP);
								sprintf((char*)p->Buffer,"尘泵运行:%uh%umin",four.U32Data/3600,four.U32Data%3600/60);//仪器编号
								Screen_RefreshDataMulti(0x0520,p->Buffer,12*2);
							}				
							{		//气体传感器安装时间
							  	   p->LastChoice=fP->Data[FLASHIS_AdrWHJZ_QTCGQXZ];
										for(int i=0;i<7;i++)
										{
										 UpdataSensorFixDate(&four,FLASHIS_AdrO2AZSJ+i*2,fP,p,i);//O2
										}		
										
							}
							//先清空显示区域, modify by alfred, 2020.06.16
                             for(u8 i = 0; i<7; i++)
                             {
                             Screen_RefreshDataMulti(0x0120+i*0x20,(u8 *)"                              ",12*2);
                             }
							p->Key.Opt=2;
							p->Timer=100;
					      break;
				case 2:   //主从板通信
				     {        
					     if(Slave_GetData()->Comm.ErrorCnt == 0)//自检返回 成功1  失败0
				        {
                               sprintf((char*)p->Buffer,"自检项目%d：主从通信正常",p->Key.Opt-1); 
						}else
						{
								  sprintf((char*)p->Buffer,"自检项目%d：主从通信异常",p->Key.Opt-1); 
						}
								Screen_RefreshDataMulti(0x0120+(p->Key.Opt-2)*0x20,p->Buffer,12*2);
								p->Key.Opt=3;
							  p->Timer=1000;
				
							  break;
					}
				case 3:  //检测SD卡 
					   {        
					     if(SD_GetData()->flag != 0)//自检返回 成功1  失败0
				        {
                                sprintf((char*)p->Buffer,"自检项目%d：SD卡正常",p->Key.Opt-1); 
						}else
						{
								  sprintf((char*)p->Buffer,"自检项目%d：SD卡异常",p->Key.Opt-1); 
						}
								Screen_RefreshDataMulti(0x0120+(p->Key.Opt-2)*0x20,p->Buffer,12*2);
								p->Key.Opt=4;
							    p->Timer=1000;
				
							  break;
							}
				case 4:  //打印蓝牙  
					{        
					     if(AT_GetData()->Mode!=2)//自检返回 成功1  失败0
				        {
                              sprintf((char*)p->Buffer,"自检项目%d：打印蓝牙正常",p->Key.Opt-1); 
						}else
						{
								sprintf((char*)p->Buffer,"自检项目%d：打印蓝牙异常",p->Key.Opt-1); 
						}
								Screen_RefreshDataMulti(0x0120+(p->Key.Opt-2)*0x20,p->Buffer,12*2);
								p->Key.Opt=9;  //5
							    p->Timer=1000;
				         //根据设置状态打开/关闭蓝牙状态
						 if(fP->Data[FLASHIS_AdrXTSZBOX]&0x04)
						 {
							 //打开打印蓝牙
							  AT_Power_On();
						 }
						 else
						 {
							  //关闭打印蓝牙
							  AT_Power_Off();
						 }
							  break;
							}
					     
				case 5:  //APP蓝牙 
					{        
					    if(AT_GetData()->Mode!=2)//自检返回 成功1  失败0 AT_GetData()->Mode!=2
				        {
                               sprintf((char*)p->Buffer,"自检项目%d：APP蓝牙正常",p->Key.Opt-1); 
						}else
						{
								  sprintf((char*)p->Buffer,"自检项目%d：APP蓝牙异常",p->Key.Opt-1); 
						}
								Screen_RefreshDataMulti(0x0120+(p->Key.Opt-2)*0x20,p->Buffer,12*2);
								p->Key.Opt=6;
							    p->Timer=1000;
				
							  break;
							}
				case 6:  //4G模块
					{        
					     if(1)//自检返回 成功1  失败0
				        {
                               sprintf((char*)p->Buffer,(const char*)("自检项目%d：4G正常"),p->Key.Opt-1); 
						}else
						{
								  sprintf((char*)p->Buffer,"自检项目%d：4G异常",p->Key.Opt-1); 
						}
								Screen_RefreshDataMulti(0x0120+(p->Key.Opt-2)*0x20,p->Buffer,12*2);
								p->Key.Opt=7;
							    p->Timer=1000;
				
							  break;
							}
				case 7:   
					{        
					     if(0)//自检返回 成功1  失败0
				        {
                                 sprintf((char*)p->Buffer,"自检项目%d：正常",p->Key.Opt-1); 
						}else
						{
								  sprintf((char*)p->Buffer,"自检项目%d：异常",p->Key.Opt-1); 
						}
								Screen_RefreshDataMulti(0x0120+(p->Key.Opt-2)*0x20,p->Buffer,12*2);
								p->Key.Opt=8;
							    p->Timer=1000;
				
							  break;
							}
					case 8:   
						{        
					     if(0)//自检返回 成功1  失败0
				        {
                                  sprintf((char*)p->Buffer,"自检项目%d：正常",p->Key.Opt-1); 
						}else
						{
								  sprintf((char*)p->Buffer,"自检项目%d：异常",p->Key.Opt-1); 
						}
								Screen_RefreshDataMulti(0x0120+(p->Key.Opt-2)*0x20,p->Buffer,12*2);
								p->Key.Opt=9;
							    p->Timer=1000;
				
							  break;
							}
				case 9:   	
							{ //检测完成
								p->RunMode=13;//自检完成后进入主界面
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
	函数名：void Screen_programUpdata(Screen_DataStructure *p)
	描述：	程序主界面处理函数
*/
void Screen_MainLayer(Screen_DataStructure *p)
{
 
	//按键检测处理
	Buttom_MainPage();
		 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
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
    		//按键逻辑处理
	p->OldMode=p->RunMode;
	if(p->Button>0&&p->Button<=8)
	{
		p->RunMode=p->Button*0x1000;
	}
	if(p->Button==0x0002) p->Key.Opt=Flashisp_GetData()->Data[FLASHIS_AdrYDBDLX];
	
	if(p->Button==0x0005) //烟气采样,如果是在采样中，直接进入采样界面
	{ 
		
		if((Flux_GetData()->ChannelC.RunMode==2)||(Flux_GetData()->ChannelC.RunMode==3))//采样暂停中或采样中
		{ //烟气采样页面
		  p->RunMode=0x5200;
		}
		else if((Flux_GetData()->ChannelC.RunMode==4)&&(Flux_GetData()->ChannelC.Sampling.EndFlg==0))//调到清洗页面
		{
			ScreenBackLastPag(p,0X5220);
		}
		else if(Flux_GetData()->ChannelC.Sampling.EndFlg==1)
		{Flux_GetData()->ChannelC.Sampling.EndFlg=0;
			//跳到保存界面
			p->Key.Opt=5;
			p->RunModeStat=1;		 
			p->RunMode=0x5210;//提示是否结束
		}
		else
		{
			p->YC_Pause=0;
		}

	}
	
	if(p->Button==0x0006) //烟尘采样，如果是在采样中，直接进入采样界面
	{ 
		if((Flux_GetData()->ChannelB.RunMode==2)||(Flux_GetData()->ChannelB.RunMode==3))//采样暂停中//采样中
		{
			p->RunMode=0x6200; //开始采样
		    p->Function=0;
		    p->Key.Opt=7;	
		}
	
		else if(Flux_GetData()->ChannelB.RunMode==4)//防倒吸中,跳到放倒吸页面
		{
			 p->RunMode=0x6220;
			 p->YC_Stat=0;
			 ScreenChangeDataInit(p);//界面调整
		}
		else if(Flux_GetData()->ChannelB.Sampling.FdxFlg)//防倒吸完成,去到保存
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
	//界面处理	 
	if(p->Key.LastOpt!=p->Key.Opt) 
	{
		p->Key.LastOpt=p->Key.Opt;
		switch(p->Key.Opt)
		 {
			case 1:  //对象选择
				{//设置
					Screen_PastePicture(0x0800,0x0009,0x04B006B,0x0D600F9,0x04B006B);
					break;
				}
				case 2:	
				{//烟道布管
					Screen_PastePicture(0x0800,0x0009,0x0F60069,0x18100F7,0x0F60069);
					break;
				}
				case 3:	
				{//工况预测
					Screen_PastePicture(0x0800,0x0009,0x1A2006B,0x022D00F9,0x1A2006B);
					break;
				}
				case 4:
				{//测含湿量
					Screen_PastePicture(0x0800,0x0009,0x24D0068,0x2D800F6,0x24D0068);
					break;
				}
				case 5:	
				{//测量烟气
					Screen_PastePicture(0x0800,0x0009,0x0490106,0x0D40194,0x0490106);
					break;
				}
				case 6:	
				{//烟尘采样
					Screen_PastePicture(0x0800,0x0009,0x0F40107,0x017F0195,0x0F40107);
					break;
				}
					case 7:	
				{//文件查询
					Screen_PastePicture(0x0800,0x0009,0x1A10107,0x022C0195,0x1A10107);
					break;
				}
					case 8:	
				{//校准维护
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
	函数名：void Screen_PowerOffPag(Screen_DataStructure *p)
	描述：	掉电主界面处理函数
*/
void Screen_PowerOffPag(Screen_DataStructure *p)
{
    FourBytes four;
	   
	  //按键检测处理
    	Buttom_PowerOffPage();
	   	if(p->RunMode!=31) return;  //按键处理后不是本页马上退出
	 //基图显示
	
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

	//文本静态动态显示
	  switch(p->RunModeStat)
		{
			case 1:
			 { //显示静态数据 处理

				Screen_ShowPage(6);
				Flux_GetData()->ChannelB.RunMode=0;
				Flux_GetData()->ChannelB.Sampling.Cnt=Flashisp_GetData()->Data[FLASHIS_AdrCJCNT]; //第正在采样的第几个采样点

				Flux_GetData()->ChannelB.Sampling.DinSiTime=(u32)Flashisp_GetData()->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]; //单点采样时间
				Flux_GetData()->ChannelB.Sampling.Count=Flashisp_GetData()->Data[FLASHIS_AdrYDBD_CYDS]; //总采样点
				Flux_GetData()->ChannelB.Sampling.SamTime=(Flashisp_GetData()->Data[FLASHIS_AdrYCCJ_CYSE_DDCS])*Flux_GetData()->ChannelB.Sampling.Count;//总时长
	            four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrCJGKTJ);	//工况体积
				four.FloatData =GetFlaotdataFormFlash2Word(FLASHIS_AdrCJWJH);                     //文件号
				Flux_GetData()->ChannelB.SaveFileNumber=four.FloatData;
				 
				Flux_GetData()->ChannelB.Sampling.YiciFlg=0;
				Flux_GetData()->ChannelB.Sampling.EndFlg=0;
				Flux_GetData()->ChannelB.Sampling.FdxFlg=0; //防倒吸完成标志
				Flux_GetData()->ChannelB.FluxRK.Sum = 0;
		        Flux_GetData()->ChannelB.FluxRK.Cnt = 0;
		        Flux_GetData()->ChannelB.FluxKD.Sum = 0;
		        Flux_GetData()->ChannelB.FluxKD.Cnt = 0;
		        Flux_GetData()->ChannelB.FluxCB.Sum = 0;
		        Flux_GetData()->ChannelB.FluxCB.Cnt = 0;

				four.U32Data=GetU32dataFormFlash2Word(FLASHIS_AdrCJKSSK); //开始采集时间
				struct tm *myTm=localtime(&four.U32Data);
				sprintf((char*)p->Buffer,"%u-%02u-%02u %02u:%02u:%02u",myTm->tm_year+SCREEN_YearBase,myTm->tm_mon,myTm->tm_mday,myTm->tm_hour,myTm->tm_min,myTm->tm_sec);//软件版本
				Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);
				 
				four.U32Data=GetU32dataFormFlash2Word(FLASHIS_AdrCJLJSH);	//累计采集时间
				sprintf((char*)p->Buffer,"%dH %dMin %dS",four.U32Data/3600,four.U32Data%3600/60,(four.U32Data%3600)%60);//
				Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);
				  
				Flux_GetData()->ChannelB.Sampling.SecTimer=four.U32Data;  //获取记忆时间
				Flux_GetData()->ChannelB.Sampling.IntTime=Flux_GetData()->ChannelB.Sampling.SecTimer-Flux_GetData()->ChannelB.Sampling.DinSiTime*(Flux_GetData()->ChannelB.Sampling.Cnt-1);  //从零开始
				 	
				four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrCJGKTJ);	//工况体积
				sprintf((char*)p->Buffer,"%10.2fL",four.FloatData);
				Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);
				Flux_GetData()->ChannelB.Sampling.SumVolume=four.FloatData;  //累计体积
				
				four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrCJBQTJ);	//标况体积
				sprintf((char*)p->Buffer,"%10.2fL",four.FloatData);
				Screen_RefreshDataMulti(0x0160,p->Buffer,15*2);
				Flux_GetData()->ChannelB.Sampling.RefVolume=four.FloatData;  //累计体积
				 
				four.U32Data=GetU32dataFormFlash2Word(FLASHIS_AdrDDTime); //丢电时间
				myTm=localtime(&four.U32Data);
				sprintf((char*)p->Buffer,"掉电时间:%04u-%02u-%02u %02u:%02u:%02u",myTm->tm_year+SCREEN_YearBase,myTm->tm_mon,myTm->tm_mday,myTm->tm_hour,myTm->tm_min,myTm->tm_sec);//软件版本
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
	函数名：void Screen_SystemSetPag(Screen_DataStructure *p)
	描述：	系统设置界面逻辑处理
	修改记录：1. 迪文屏修改了本页显示内容，相应修改了代码，增加和减少一些显示, modify by alfred, 2020.06.12.
*/
void Screen_SystemSetPag(Screen_DataStructure *p)
{
	FourBytes four;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//按键处理
	Button_SysSetPage();
	if(p->RunMode!=0x1000) return;
	//数据更新
	switch(p->RunModeStat)
	{
		case 0x01: //静态数据显示			
		{
			Screen_ShowPage(10); 	
			// Screen_PastePicture(0x0800,0x000B,(54<<16)+66,(433<<16)+111,(54<<16)+66); //日期
			for(int i=0;i<10;i++)
			{
				ScreenSetJitu(p,i);
			}
			//日期
			sprintf((char*)p->Buffer,"%02d-%02d-%02d",p->Clock.Clock.tm_year+SCREEN_YearBase,p->Clock.Clock.tm_mon,p->Clock.Clock.tm_mday);
			Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);//年
			//时间
			sprintf((char*)p->Buffer,"%02d:%02d:%02d",p->Clock.Clock.tm_hour,p->Clock.Clock.tm_min,p->Clock.Clock.tm_sec);
			Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);//年					
			if(((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX])&(1>>0))!=0x0000)//大气压 手动输入
			{//输入
				four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrXTSZDQY);
				sprintf((char*)p->Buffer,"%.2f",four.FloatData);
				Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);//大气压
			}
			//显示地址信息
			{
				const char uCursor[]={0xff,0xff,0x00}; //补上两个0XFF使光标位置正确显示。
				sprintf((char*)p->Buffer,"%s",(u8 *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));
				strcat((char*)p->Buffer, uCursor);
				
			}				
			Screen_RefreshDataMulti(0x0200,p->Buffer,15*2);
			//排水间隔
			Screen_ResetDataBuffer();
			sprintf((char*)p->Buffer,"%.2d",Flashisp_GetData()->Data[FLASHIS_AdrXTSZZDPSJG]);
			Screen_RefreshDataMulti(0x0180,p->Buffer,15*2);
			//排水时长
			Screen_ResetDataBuffer();
			sprintf((char*)p->Buffer,"%.2d",Flashisp_GetData()->Data[FLASHIS_AdrXTSZZDPSSC]);
			Screen_RefreshDataMulti(0x01A0,p->Buffer,15*2);
			
			//含湿量时间
			Screen_ResetDataBuffer();
			sprintf((char*)p->Buffer,"%.2d",Flashisp_GetData()->Data[FLASHIS_AdrXTSZCHSLSJ]);
			Screen_RefreshDataMulti(0x01C0,p->Buffer,15*2);	
		
			
			p->RunModeStat=2;
			p->Timer=50;
			break;
		}
			
		case 0x02: //动态数据显示
		{
			   	if(p->Timer==0)
					{
					  p->Timer=500;
						Screen_ResetDataBuffer();
						if(p->Key.Mode == 1) //日期
						{
							if(p->Key.bOutsideKeyboard != false)
								ScreenEnterNumberTwinkle(p,0x0100);
						}
						else
						{
							sprintf((char*)p->Buffer,"%02d-%02d-%02d",p->Clock.Clock.tm_year+SCREEN_YearBase,p->Clock.Clock.tm_mon,p->Clock.Clock.tm_mday);
							Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);//年
							//strcpy((char*)p->Key.Buffer,(char*)p->Buffer);
						}
						Screen_ResetDataBuffer();
						if(p->Key.Mode == 2) //时间
						{
							if(p->Key.bOutsideKeyboard != false)
								ScreenEnterNumberTwinkle(p,0x0120);
						}
						else
						{
							sprintf((char*)p->Buffer,"%02d:%02d:%02d",p->Clock.Clock.tm_hour,p->Clock.Clock.tm_min,p->Clock.Clock.tm_sec);
							Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);//年
							//strcpy((char*)p->Key.Buffer,(char*)p->Buffer);
						}

				        if((p->Key.Mode != 0) && (p->Key.bOutsideKeyboard != false))	//2020-05-20 hjl modify
				       {
					    switch(p->Key.Mode)
					   {
						case 3:
						{//大气压输入闪烁
							if((pf->Data[FLASHIS_AdrXTSZBOX]&(1>>0))!=0x0000)
							{
								ScreenEnterNumberTwinkle(p,0x0140);
								break;
							}
						}
						
						case 7:
						{//排水间隔闪烁
							ScreenEnterNumberTwinkle(p,0x0180);
							break;
						
						}
						
						case 8:
						{//排水时长闪烁
							ScreenEnterNumberTwinkle(p,0x01A0);
							break;
						}
						
						case 10:
						{//含湿量时间
							ScreenEnterNumberTwinkle(p,0x01C0);
							break;
						
						}
					  }	
				    }
					else
					{
						
						
						if((pf->Data[FLASHIS_AdrXTSZBOX]&(1>>0))==0x0000)//大气压测量值
						{//大气压
						Screen_ResetDataBuffer();
					    if(Slave_GetData()->DQPress.Value == FLUX_ColFAULT) 
						sprintf((char*)p->Buffer,"故障");
					    else 
						sprintf((char*)p->Buffer,"%.2f",Slave_GetData()->DQPress.Value);//Slave_GetData()->DQPress.Value
					    Screen_RefreshDataMulti(0x0140,p->Buffer,6*2);//大气压
				        }
				        else
				        {//大气压输入值
					     four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrXTSZDQY);
					     sprintf((char*)p->Buffer,"%.2f",four.FloatData);
					     Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);//大气压
				        }
					}
			}
		}	
	}
	//选择基图更新
	ScreenSetJitu(p,5);
}


/* date 2019-12-12
	函数名：void Screen_Sys_CYSZPage(Screen_DataStructure *p)
	描述：	系统设置界面采样设置逻辑处理
*/
/*
void Screen_Sys_CYSZPage(Screen_DataStructure *p)
{

	Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	Button_Sys_CYSZPage();
	if(p->RunMode!=0x1100) return;
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
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
			case 0x02: //动态数据显示
			{
			   	if(p->Timer==0)
					{
					  p->Timer=500;
						if(p->Key.Mode >= 3&&p->Key.Mode <= 5) //时间
						{
							if(p->Key.bOutsideKeyboard != false)
								ScreenEnterNumberTwinkle(p,0x0100+0x20*(p->Key.Mode-3));  //此处地址对应迪文屏的地址，因为迪文屏的触控键值为3，4，5相对应的文本框地址为0X0100，0X0120，0X0140，所要对应加上0x20
						}
						
					
			}
		}	
  }
		//选择基图更新
	  ScreenSetJitu(p,8);
}
*/
//---
/* date 20200513
	函数名：void Screen_FluePag(Screen_DataStructure *p)
	描述：	烟道布点设置界面逻辑处理
*/
void Screen_FluePag(Screen_DataStructure *p)
{
//	FourBytes four;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  static u16 LastChoice=0;
	 //按键处理
	  Button_FlueSetPage();
	  if(p->RunMode!=0x2000) return;
			//选择基图更新
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
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				Screen_ShowPage(12); 
				LastChoice=0;	                      //20200513
				p->RunModeStat=2;				 
			 }
			break;
			case 0x02: //动态数据显示
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
	函数名：void Screen_FlueCircularPag(Screen_DataStructure *p)
	描述：	烟道布点-圆形设置界面逻辑处理
*/

void Screen_FlueCircularPag(Screen_DataStructure *p)
{

	  Flashisp_DataStructure* pf=Flashisp_GetData();
	u16 Single_hole_Number=1;
	 //按键处理
	  Button_FlueCircularPage();
	  if(p->RunMode!=0x2100) return;
				//选择基图更新
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
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				 Screen_ShowPage(14);
				 //显示数据装载
				  p->DisValue[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDYXNJ); 
				  p->DisValue[1]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_MJ); 
				 
				 
				 if(p->Key.LastMode==2)   //一起进入静态处理，有谁决定谁（直径-面积）计算
				 {
					p->DisValue[1]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_MJ); 
					p->DisValue[0]=2.0f*sqrt(p->DisValue[1]/PI); //由面积计算半径
					SaveFlaotToFlash(p->DisValue[0],FLASHIS_AdrYDBDYXNJ);//保存直径
			        CalculationCircularMeasurePoint(1,p->DisValue[0],GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT),&(pf->Data[FLASHIS_AdrYDBDYXHS]),&Single_hole_Number,&(p->DisValue[6]));
				 }
				 else if(p->Key.LastMode==1) //由半径计算面积
				 {
					 p->DisValue[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDYXNJ); 
					 p->DisValue[1]= pow(p->DisValue[0]/2,2)*PI;//计算园的面积
					 SaveFlaotToFlash(p->DisValue[1],FLASHIS_AdrYDBD_MJ);//保存面积
					 CalculationCircularMeasurePoint(1,p->DisValue[0],GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT),&(pf->Data[FLASHIS_AdrYDBDYXHS]),&Single_hole_Number,&(p->DisValue[6]));
				 }	
                 else				 
				 CalculationCircularMeasurePoint(0,p->DisValue[0],GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT),&(pf->Data[FLASHIS_AdrYDBDYXHS]),&Single_hole_Number,&(p->DisValue[6]));
				 p->DisValue[2]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT);//管套
		
				 p->DisValue[3]= pf->Data[FLASHIS_AdrYDBDYXHS]; //环数
				 p->DisValue[4]= pf->Data[FLASHIS_AdrYDBDYXKS]; //孔数
				 p->DisValue[5]= Single_hole_Number* p->DisValue[4]; 
				 pf->Data[FLASHIS_AdrYDBD_CYDS]=p->DisValue[5];//总采样点数
				 pf->Flag=1;//保存
				//显示数据
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
				//显示地址信息
				{
				 char uCursor[]={0xff,0xff,0x00}; //补上两个0XFF使光标位置正确显示。
				 sprintf((char*)p->Buffer,"%s",(u8 *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));
				 strcat((char*)p->Buffer, uCursor);
				//  sprintf((char*)p->Buffer,"%s",(u8 *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));
				}				
				Screen_RefreshDataMulti(0x01A0,p->Buffer,15*2);
				//显示测点距离
				for(int i=0;i<10;i++)
				{
					Screen_ResetDataBuffer();
					if(i<(Single_hole_Number)){
					  
						sprintf((char*)p->Buffer,"%.3f",p->DisValue[i+6]);
					}
				  if(i%2==0) //偶数
					{
					  Screen_RefreshDataMulti(0x0A00+(0x10)*(i/2+1),p->Buffer,15*2);//年
					}else      //计数
					{
					   Screen_RefreshDataMulti(0x0A00+(0x10)*(i/2+1)+8,p->Buffer,15*2);//年
					}
				}
                sprintf((char*)p->Buffer,"%d",(u32)p->DisValue[5]);		
				Screen_RefreshDataMulti(0x01C0,p->Buffer,15*2); 
				p->RunModeStat=2;				 
			 }
			break;
			case 0x02: //动态数据显示
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
	函数名：void Screen_FlueCircularPag(Screen_DataStructure *p)
	描述：	烟道布点-烟道查询界面逻辑处理
*/
void Screen_FlueSeekPag(Screen_DataStructure *p)
{

	  
	  Flashisp_DataStructure* pf=Flashisp_GetData();
		//按键处理
	  Button_FlueSeekPage(&p->FileNumber);
	  if(p->RunMode!=0x2400) return;
		//选择基图更新
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
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				 Screen_ShowPage(136);
				 
				//清空显示区域
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
				 
				 SdFile_Serch_MaxofFlueLinkFile();//先读取最大文件号
				 ReadFlueData(p->FileNumber);
				 //显示数据装载 
				//显示数据
					 p->RunModeStat=2;				 
			 }
			break;
			case 0x02: //动态数据显示
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
	函数名：void Screen_FlueOtherPag(Screen_DataStructure *p)
	描述：	烟道布点-其他形状设置界面逻辑处理  
*/
void Screen_FlueOtherPag(Screen_DataStructure *p)
{
    FourBytes four;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	  Button_FlueOtherPage();
	  if(p->RunMode!=0x2300) return;
		//选择基图更新
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
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				Screen_ShowPage(18);
		
				four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_MJ);//面积
				sprintf((char*)p->Buffer,"%.4f",four.FloatData);
				Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);
				sprintf((char*)p->Buffer,"%d",pf->Data[FLASHIS_AdrYDBD_CYDS]);//采样总点数
				Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);    
				p->RunModeStat=2;	
		//显示地址信息
				{
									 char uCursor[]={0xff,0xff,0x00}; //补上两个0XFF使光标位置正确显示。
				 sprintf((char*)p->Buffer,"%s",(u8 *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));
				 strcat((char*)p->Buffer, uCursor);
				 // sprintf((char*)p->Buffer,"%s",(u8 *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));
				}				
				Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);				 
			 }
			break;
			case 0x02: //动态数据显示
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
	函数名：void Screen_FlueSquarePag(Screen_DataStructure *p)
	描述：	烟道布点方-形设置界面逻辑处理  
*/
void Screen_FlueSquarePag(Screen_DataStructure *p)
{
 
	  Flashisp_DataStructure* pf=Flashisp_GetData();
      u16 buf;
	 //按键处理
	  Button_FlueSquarePage();
	  if(p->RunMode!=0x2200) return;
		//选择基图更新
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
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {       
				Screen_ShowPage(16);
			  //显示数据装载

				 if(p->Key.LastMode==3) //由面积计算B边
				 {
	
					 p->DisValue[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBA); //A 边
					 p->DisValue[2] =GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_MJ); //面积
					 p->DisValue[1]=p->DisValue[2]/p->DisValue[0];
					 SaveFlaotToFlash(p->DisValue[1],FLASHIS_AdrYDBDFXBB); //保存B边
				 }else //由AB计算 面积
				 {
		
					 p->DisValue[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBA); //A 边
					 p->DisValue[1]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBDFXBB); //B 边
					 p->DisValue[2]=p->DisValue[1]*p->DisValue[0];
					 SaveFlaotToFlash(p->DisValue[2],FLASHIS_AdrYDBD_MJ); //保存面积
				
				 }
				 p->DisValue[3]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYDBD_GT); //管套
				 p->DisValue[4]= pf->Data[FLASHIS_AdrYDBDFXKS];
                 p->DisValue[5]= pf->Data[FLASHIS_AdrYDBDFXDKCD];
				 
				  if((p->Key.LastMode>=1)&&(p->Key.LastMode<=3))
				 { 
                     buf=SquareCalculationMeasureHold(p->DisValue[2],p->DisValue[0],p->DisValue[1]); //计算孔数，单孔测点
				
                     p->DisValue[4]=(float)((buf&0xff00)>>8);
					 SaveU16ToFlash(((buf&0xff00)>>8),FLASHIS_AdrYDBDFXKS);
					 p->DisValue[5]=(float)(buf&0x00ff);
					 SaveU16ToFlash(buf&0x00ff,FLASHIS_AdrYDBDFXDKCD);
					 
				 }
	
					 //计算采样点位置
					SquareCalculationMeasurePointN(&(pf->Data[FLASHIS_AdrYDBDFXDKCD]),p->DisValue[4],p->DisValue[1],p->DisValue[3],p->DisValue[0],&(p->DisValue[7])); 
			
				

				 p->DisValue[6]=p->DisValue[4]*p->DisValue[5];
				 pf->Data[FLASHIS_AdrYDBD_CYDS]=p->DisValue[6];
				 pf->Flag=1;
				//显示数据
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
				//显示地址信息
				{
				 char uCursor[]={0xff,0xff,0x00}; //补上两个0XFF使光标位置正确显示。
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
				  if(i%2==0) //偶数
					{
					  Screen_RefreshDataMulti(0x0A00+(0x10)*(i/2+1),p->Buffer,15*2);//年
					}else      //计数
					{
					   Screen_RefreshDataMulti(0x0A00+(0x10)*(i/2+1)+8,p->Buffer,15*2);//年
					}
				}  
				 sprintf((char*)p->Buffer,"%d",(u32)p->DisValue[6]);		
				Screen_RefreshDataMulti(0x01E0,p->Buffer,15*2); 
			 p->RunModeStat=2;				 
			 }
			break;
	
			case 0x02: //动态数据显示
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
	函数名：void Screen_WCP_SetPag(Screen_DataStructure *p)
	描述：	工况预测
*/
void Screen_WCP_SetPag(Screen_DataStructure *p)
{
  //	FourBytes four;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	  Button_WCP_SetPag(5,1,p->DisValue);
	  if(p->RunMode!=0x3000) return;
			//选择基图更新
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
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
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
	函数名：void Screen_WCP_PressureZeroPag(Screen_DataStructure *p)
	描述：	工况预测-压力调零
*/
void Screen_WCP_PressureZeroPag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  Slave_DataStructure* Sp=Slave_GetData();
		 //按键处理
	  Button_WCP_PressureZeroPag(2,1,p->DisValue);
	  if(p->RunMode!=0x3100) return;
		//选择基图更新
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
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
       
				Screen_ShowPage(22);
                p->Timer=0;
				p->TimerSec=10;
				p->Function=2;
				p->RunModeStat=2;				 
			 }
			break;
	
			case 0x02: //动态数据显示
			{	
				      if(p->Timer==0)
							{

							Screen_ResetDataBuffer();
							sprintf((char*)p->Buffer,"%d",(u32)Sp->ACPress.Value);//烟气动压
							Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);//
							sprintf((char*)p->Buffer,"%.2f",Sp->AGPress.Value);//烟气静压
							Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);//
						    sprintf((char*)p->Buffer,"%.2f",Sp->BGPress.Value);//计前压力
							Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);//
                            sprintf((char*)p->Buffer,"%d",(u32)Sp->BCPress.Value);//尘流压		
							Screen_RefreshDataMulti(0x0160,p->Buffer,15*2);//
	                        sprintf((char*)p->Buffer,"%d",(u32)Sp->CCPress.Value);//气流压		
							Screen_RefreshDataMulti(0x0180,p->Buffer,15*2);// 气流压PA
						    p->Timer=500;
							if(p->TimerSec==0)
							{
							  //to 确认零点值
								if(p->Function==2)
                                {									
								Slave_Zero_Press();
								Screen_RefreshDataMulti(0x01A0,p->Buffer,15*2);//倒计时
								sprintf((char*)p->Buffer,"调零完成!");
								Screen_RefreshDataMulti(0x01A0,p->Buffer,15*2);//倒计时	
								Speak_Buzzer(300);	
								}
								p->Function=0;

							}else
							{
							  sprintf((char*)p->Buffer,"%2dS",p->TimerSec);
							  Screen_RefreshDataMulti(0x01A0,p->Buffer,15*2);//倒计时
							}
						}
			}
			break;
		}
}
//--
/* date 2019-12-19
	函数名：void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p)
	描述：	工况预测-烟温测量
*/
void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p)
{

	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  Slave_DataStructure* Sp=Slave_GetData();
		 //按键处理
	  Button_WCP_FumeTemperaturPag(2,1,p->DisValue);
	  if(p->RunMode!=0x3200) return;
		//选择基图更新
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
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				Screen_ShowPage(24);
                p->Timer=0;
				p->TimerSec=10;
				p->RunModeStat=2;				 
			 }
			break;	
			case 0x02: //动态数据显示
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
											
						       	  sprintf((char*)p->Buffer,"%.1f", GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_YWCL_SRYW)); //输入烟温
										 }   
							     Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);//倒计时
								}
															
						  p->Timer=500;
						}
			}
			break;
		}
}
//--
/* date 2019-12-19
	函数名：void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p)
	描述：	工况预测-预测流速
*/
void Screen_WCP_PredictedVelocityPag(Screen_DataStructure *p)
{
//    FourBytes four;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	Slave_DataStructure* Sp=Slave_GetData();
//	FourBytes four;
	Calculation_FormulaStructure * Cp=CalculationFormula_GetData();
	 //按键处理
	Button_WCP_PredictedVelocityPag(3,1,p->DisValue);
	if(p->RunMode!=0x3300) return;
	//选择基图更新
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
	 //数据更新
    switch(p->RunModeStat)
	{
		case 0x01: //静态数据显示			
		{
			Screen_ShowPage(26);
			p->Timer=0;
			p->RunModeStat=2;
			
			Screen_RefreshDataMulti(0x0420,(u8*)"                              ",15*2);// 烟道类型	
			Screen_ResetDataBuffer(); 
			sprintf((char*)p->Buffer,"%s",GetYDBD_Describe(pf->Data[FLASHIS_AdrYDBDLX]));
			Screen_RefreshDataMulti(0x0460,p->Buffer,15*2);// 烟道类型		
			Dis_Tips(p,(u8*)"正在测试烟气流速",0x0401);//显示提示	
			Screen_ResetDataBuffer(); 
			sprintf((char*)p->Buffer,"%2d",p->StarCun); //显示点数
			Screen_RefreshDataMulti(0x0440,p->Buffer,15*2);// 烟道类型
			Screen_ResetDataBuffer(); 
			sprintf((char*)p->Buffer,"共%2d点",pf->Data[FLASHIS_AdrYDBD_CYDS]); //显示总共点数
			Screen_RefreshDataMulti(0x0480,p->Buffer,15*2);// 烟道类型
			
            //清除p->DisValue	
         

		}
		break;
		case 0x02: //动态数据显示
		{	
			if(p->Timer==0)
			{		
				p->DisValue[0]=Cp->Pd;//动压
				p->DisValue[1]=Cp->Pt;//计算全压
				p->DisValue[2]=Cp->Ps;	//静压	
				p->DisValue[3]=Cp->Ba;	//大气压
				p->DisValue[5]=Cp->Ts ; //烟温
				p->DisValue[4]=Cp->Vs; //烟气流速
				p->DisValue[6]=Cp->F;//获取烟道面积
                p->DisValue[7]=Cp->Qs;//烟气流量	
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
	函数名：void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p)
	描述：	工况预测-查平均值
*/
void Screen_WCP_CheckAveragePag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
		 //按键处理
	  Button_WCP_CheckAveragePag(5,1,&p->FileNumber);
	  if(p->RunMode!=0x3400) return;
		//选择基图更新
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
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
					Screen_ShowPage(28);
					p->Timer=0;
					p->RunModeStat=2;
					//显示数据装载
                    if(p->LoadFlag==1)
					{
					  p->LoadFlag=0;
						SdFile_Serch_MaxofWorkLinkFile();//先读取最大文件号
						p->FileNumber=SdFile_GetData()->WorkLinkFileMax;
					}
					ReadWorkData(p->FileNumber);
                    Dis_Tips(p,(u8*)"文件查询成功",0x3B20);//显示提示							
			 }
			break;
			case 0x02: //动态数据显示
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
	函数名：void Screen_CHSL_SetPag(Screen_DataStructure *p)
	描述：	测含湿量类型
*/
void Screen_CHSL_SetPag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	  Button_CHSL_SetPag(4,1,&FileValue);
	  if(p->RunMode!=0x4000) return;
		//选择基图更新
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
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				 Screen_ShowPage(30); 	
				sprintf((char*)p->Buffer,"%.2f",GetFlaotdataFormFlash2Word(FLASHIS_AdrCHSL_HSL));//含湿量
				Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);		
				 p->RunModeStat=2;				 
			 }
			break;
      case 0x02: //动态态数据显示			
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
	函数名：void Screen_CHSL_SetPag(Screen_DataStructure *p)
	描述：	测含湿量干湿球法
*/
void Screen_CHSL_GSQF_Pag(Screen_DataStructure *p)
{
   Flashisp_DataStructure* pf=Flashisp_GetData();
	 Calculation_FormulaStructure * Cp=CalculationFormula_GetData();
	 //按键处理
	  Button_CHSL_GSQF_Pag(2,1,p->DisValue);
	  if(p->RunMode!=0x4100) return;
		//选择基图更新
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
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				 Screen_ShowPage(32);
				 p->TimerSec=pf->Data[FLASHIS_AdrXTSZCHSLSJ]*60;
				  p->mystatus=0;
				 if( p->TimerSec==0)
				 	p->mystatus=1;
                 Flux_StarSampling('B',15.0f);			//样机用25L/MIN  ,GB用15L/min	 
				 p->RunModeStat=2;	
       // Dis_Tips(p,"开始测量",0x01e0);			 
			 }
			break;
      case 0x02: //动态态数据显示			
			 {
				 if(p->Timer==0)
							{	
								 p->DisValue[0]=	CalculationFormula_Xsw(CalculationFormula_GetData()->Phv,(CalculationFormula_GetData()->Tc)-(CalculationFormula_GetData()->Tb),
								(CalculationFormula_GetData()->Ba+CalculationFormula_GetData()->Pb));
                                 p->DisValue[1]=Flux_GetData()->ChannelB.ShowFlux;
								 p->DisValue[2]=Cp->Tc;
								 p->DisValue[3]=Cp->Tb;
								 p->DisValue[4]=Slave_GetData()->DustMotor.Ratio*100/Slave_GetData()->DustMotor.Pid.PWMcycle;//计算功率
								 p->DisValue[5]=Cp->Pcs;
								 p->DisValue[6]=Cp->Tj;
								 if(p->mystatus==0)
								 sprintf((char*)p->Buffer,"倒计时：%d S",p->TimerSec);
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
	函数名：void Screen_CHSL_ZRF_Pag(Screen_DataStructure *p)
	描述：	测含湿量阻容法
*/
void Screen_CHSL_ZRF_Pag(Screen_DataStructure *p)
{
    float DisData={11.01};//0含湿量 1流量 2干球温度 3湿球温度 4功率 5计压 6计温
	  Flashisp_DataStructure* pf=Flashisp_GetData();
		//数据初始化

	 //按键处理
	  Button_CHSL_ZRF_Pag(2,1,&FileValue);
	  if(p->RunMode!=0x4200) return;
		//选择基图更新
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
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				 Screen_ShowPage(118); 
                 Dis_Tips(p,(u8*)"通讯异常",0x0120);						 
				 p->RunModeStat=2;				 
			 }
			break;
           case 0x02: //动态态数据显示			
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
	函数名：void Screen_CHSL_HSLCX_Pag(Screen_DataStructure *p)
	描述：	含湿量查询
*/
void Screen_CHSL_HSLCX_Pag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	  Button_CHSL_HSLCX_Pag(5,1,&p->FileNumber);
	  if(p->RunMode!=0x4400) return;
		//选择基图更新
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
	 //数据更新
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
					Screen_ShowPage(50);
					p->Timer=0;
					p->RunModeStat=2;
					//显示数据装载
          
				  //数据显示 
					sprintf((char*)p->Buffer,"%07d",(u32)FileValue);
					Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);// 文件号						 
					sprintf((char*)p->Buffer,"2019-12-24");
					Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);// 日期时间	
					sprintf((char*)p->Buffer,"16:10");
					Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);// 日期时间					 
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
			case 0x02: //动态数据显示
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
	函数名：void Screen_CLYQ_SetPag(Screen_DataStructure *p)
	描述：	测量烟气
	修改记录：1. 迪文屏修改了本页显示内容，相应修改了代码，去掉采样时长，增加计平均时长和计平均次数的设置, modify by alfred, 2020.06.13.
*/
void Screen_CLYQ_SetPag(Screen_DataStructure *p)
{
//	static u8 LastChoice=11;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//按键处理
	Button_CLYQ_SetPag(5,1,&FileValue);
	if(p->RunMode!=0x5000) return;
	//选择基图更新
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
			

	//数据更新
	switch(p->RunModeStat)
	{
		case 0x01: //静态数据显示			
		{
			Screen_ShowPage(34); 	
//			Screen_ResetDataBuffer();		
//			sprintf((char*)p->Buffer,"%2d",pf->Data[FLASHIS_AdrCLYQ_JSSC]);
//			Screen_RefreshDataMulti(0x0101,p->Buffer,15*2);	
			
			Screen_ResetDataBuffer();		
			sprintf((char*)p->Buffer,"%2d",pf->Data[FLASHIS_AdrXTSZYQCLSJ]);	//计平均时间
			Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);	
			Screen_ResetDataBuffer();		
			sprintf((char*)p->Buffer,"%2d",pf->Data[FLASHIS_AdrXTSZYQCLCS]);	//计平均次数
			Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);	
			
			p->RunModeStat=2;				 
		}
		break;
		case 0x02: //动态态数据显示			
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
	函数名：void Screen_CLYQ_YQJZ_Pag(Screen_DataStructure *p)
	描述：	测量烟气-烟气校准
*/
void Screen_CLYQ_YQJZ_Pag(Screen_DataStructure *p)
{
 	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  Slave_DataStructure*  Sp=Slave_GetData();
	 //按键处理
	  Button_CLYQ_YQJZ_Pag(2,1,&FileValue);
	  if(p->RunMode!=0x5100) return;
		//选择基图更新
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
		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				 Screen_ShowPage(36); 
				 p->TimerSecUp=0;
				 Flux_StarSampling('C',1.0);//打开气泵			 
				 p->RunModeStat=2;	
				 p->Function=0;	
				 p->TimerSec=0;				 
			 }
			break;
      case 0x02: //动态态数据显示			
			 {
				 if(p->Timer==0)
							{		
								p->TimerSecUp++;								
								Screen_ResetDataBuffer(); //时间信息		
								sprintf((char*)p->Buffer,"%2dM%2dS",p->TimerSecUp/120,p->TimerSecUp/2%60);
								Screen_RefreshDataMulti(0x0420,p->Buffer,15*2);										
								//时间倒计时
								if(p->TimerSec==0)
								{
									 if(p->Function==1)
									{
									  ScreenBackLastPag(p,0X5000);
										p->Function=0;
										ScreenChangeDataInit(p);
										Flux_StopSampling('C');//关闭气泵	
				             
									}
									sprintf((char*)p->Buffer," ");
									Screen_RefreshDataMulti(0x0400,p->Buffer,15*2);	
								}
								else
								{
									sprintf((char*)p->Buffer,"%2dS后完成校准！",p->TimerSec);
									Screen_RefreshDataMulti(0x0400,p->Buffer,15*2);	
								}
								Screen_ResetDataBuffer(); //时间倒计时
								sprintf((char*)p->Buffer,"%.1f",Flux_GetData()->ChannelC.ShowFlux);//流量值
								Screen_RefreshDataMulti(0x0440,p->Buffer,15*2);
								sprintf((char*)p->Buffer,"%.1f %%",Slave_GetData()->GasMotor.Ratio*100/Slave_GetData()->GasMotor.Pid.PWMcycle);//流量值, modify by alfred, 2020.07.04
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
								  sprintf((char*)p->Buffer,"------");//气体值
								}
								else
								{
									//if(p->DisValue[i]>0)
									{   if((i==0)||(i==7))
										sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);//瞬时 
										else
										sprintf((char*)p->Buffer,"%.0f",p->DisValue[i]);//瞬时 	
									}
									/*
									else
									{			
										 if((i==0)||(i==7))
										sprintf((char*)p->Buffer,"%.1f",0.0f);	//	去掉负数显示，modify by alfred, 2020.07.15
										 else
										sprintf((char*)p->Buffer,"%.0f",0.0f);	//	去掉负数显示，modify by alfred, 2020.07.15
									}
									*/
								}
								Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);	
								}
								if(p->Function==1)
								{
								    //to do 校准逻辑
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
	函数名：void Screen_CLYQ_JIPINGJUN_Pag(Screen_DataStructure *p)
	描述：	测量烟气-计平均
*/
void Screen_CLYQ_JIPINGJUN_Pag(Screen_DataStructure *p)
{
	Flashisp_DataStructure* pf=Flashisp_GetData();
	Slave_DataStructure*  Sp=Slave_GetData();
	

	u8 uDataDisplayBuf[60], uTimeDisplayBuf[10], uTempBuf[8]; //定义用于显示区的组组：日期显示，时间显示，共用临时数组
	u8 uDisRow;	//显示的行数
	float buf[8]={1,Ppmtomgm3_SO2,Ppmtomgm3_NO,Ppmtomgm3_NO2,Ppmtomgm3_CO,1,Ppmtomgm3_H2S,1};
	//按键处理
	Button_CLYQ_JIPINGJUN_Pag(5,1,&FileValue);
	if(p->RunMode!=0x5230) return;
	//选择基图更新

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
		//数据更新
    switch(p->RunModeStat)
	{  
		case 0x01: //静态数据显示			
		{   
			Screen_ShowPage(140); 		 
			p->RunModeStat=2;

			//显示单位
			if(pf->Data[FLASHIS_AdrCLYQ_DWLX]==1)//单位切换
			{
			
				Screen_RefreshDataMulti(0x0120,(u8 *)"ppm    ",10);
			}
			else
			{
			
				Screen_RefreshDataMulti(0x0120,(u8 *)"mg/m^3 ",10);
			}
						
			//当翻页时启动定时器，定时未够时不显示当前页面
			if(p->JiPingJun.Timer==0)
				p->JiPingJun.uMinDataPage = (p->JiPingJun.cAverageCount-1)/TABLE_ROW;
			
			//如果显示下一页或上一页，在新页中清空显示区域

				p->JiPingJun.ucCurrentPage = p->JiPingJun.uMinDataPage;
				for(int i=0;i<8;i++)
				{
					Screen_RefreshDataMulti(0x0400+0x80*i,(u8 *)"                                                            ",60);
					Screen_RefreshDataMulti(0x0900+0x10*i,(u8 *)"          ",10);
				}

			
			memset(uTempBuf,0,sizeof(uTempBuf));
			sprintf((char*)uTempBuf,"Page: %d",p->JiPingJun.uMinDataPage+1);	//,p->JiPingJun.uMinDataPage为零的时候显示第一页
			Dis_Tips(p,uTempBuf,0x0130); //刷新页码
			if(p->JiPingJun.cAverageCount>0)
			{
				if(p->JiPingJun.uMinDataPage!= (p->JiPingJun.cAverageCount -1)/TABLE_ROW)	//当分钟数据条数=行数,即已满屏,当前显示行uDisRow = TABLE_ROW-1总行数
							//页码不等于当前记数页，则当前显示行uDisRow = TABLE_ROW总行数,用于翻页时数据显示
				{
					uDisRow = TABLE_ROW-1;
				}
				else uDisRow = (p->JiPingJun.cAverageCount-1)%TABLE_ROW;	//当前行数 = 当前分钟数据条数%行数
								
				for(u8 i=0+p->JiPingJun.uMinDataPage*TABLE_ROW; i<=uDisRow+p->JiPingJun.uMinDataPage*TABLE_ROW;i++)	//每页共TABLE_ROW行,默认8行
				{//显示分钟数据
					memset(uDataDisplayBuf,0,sizeof(uDataDisplayBuf));
					memset(uTimeDisplayBuf,0,sizeof(uTimeDisplayBuf));
					
					for(u8 k=0;k<8;k++)	//取一行数据
					{				
						memset(uTempBuf,0,sizeof(uTempBuf));

						if(gMinuteData[i][k] == SLAVE_ColFAULT) 
						sprintf((char*)uTempBuf,"%-7s","-----");//如果气体值不正常，显示-----
						else
                        {
                        if(pf->Data[FLASHIS_AdrCLYQ_DWLX]==1)
						{ if((k==0)||(k==5))
							sprintf((char*)uTempBuf,"%-7.1f",gMinuteData[i][k]/buf[k]);	//根据单位设定乘以相应系数显示
						  else
						    sprintf((char*)uTempBuf,"%-7.0f",gMinuteData[i][k]/buf[k]);	//根据单位设定乘以相应系数显示	  
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
		case 0x02: //动态态数据显示			
		{
			if(p->Timer==0)
			{							 

				Screen_ResetDataBuffer();
				sprintf((char*)p->Buffer,"%.1f",Flux_GetData()->ChannelC.ShowFlux);//流量值
				Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);	
							
				Screen_ResetDataBuffer();
				sprintf((char*)p->Buffer,"%02d:%02d",p->JiPingJun.uCounter1S/60,p->JiPingJun.uCounter1S%60);//平均时间显示20200505
				Screen_RefreshDataMulti(0x0110,p->Buffer,15*2);
							
				p->DisValue[0]=Sp->O2Concent.Value;
				p->DisValue[1]=Sp->SO2Concent.Value;
				p->DisValue[2]=Sp->NOConcent.Value;
				p->DisValue[3]=Sp->NO2Concent.Value;
				p->DisValue[4]=Sp->COConcent.Value;
				p->DisValue[5]=Sp->CO2Concent.Value;
				p->DisValue[6]=Sp->H2SConcent.Value;
				p->DisValue[7]=Sp->NOxConcent.Value;
				
				if(pf->Data[FLASHIS_AdrCLYQ_DWLX]==1)//单位切换
				{
					for(int i=0;i<8;i++) 
					{
					if(p->DisValue[i]!=SLAVE_ColFAULT)
					 p->DisValue[i]=p->DisValue[i]/buf[i];	//瞬时值
				    }
				}
				for(int i=0;i<8;i++)
				{ 
					Screen_ResetDataBuffer();
					if(p->DisValue[i]==SLAVE_ColFAULT)
					{
						sprintf((char*)p->Buffer,"-----");//气体值
					}
					else
					{
						if(p->DisValue[i]>0)
						{if((i==0)||(i==5))
						sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);//瞬时 
						else
						sprintf((char*)p->Buffer,"%.0f",p->DisValue[i]);//瞬时 	
                        }
						else
                        {if((i==0)||(i==5))
						  sprintf((char*)p->Buffer,"%.1f",0.0f);//瞬时 
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
	函数名：void Screen_CLYQ_KSJC_Pag(Screen_DataStructure *p)
	描述：	测量烟气-开始检测
*/
void Screen_CLYQ_KSJC_Pag(Screen_DataStructure *p)
{
	static u8 LastChoiceDw=11,LastChoiceJx=11;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	Slave_DataStructure*  Sp=Slave_GetData();
	 SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure *ps = Screen_GetData();
	//按键处理
	Button_CLYQ_KSCL_Pag(5,1,&FileValue);	//去掉“单位”菜单，modify by alfred, 2020.07.11
	if(p->RunMode!=0x5200) return;
	//选择基图更新
	
	if(LastChoiceJx!=p->PauseOrStop)	
	{
		LastChoiceJx=p->PauseOrStop;
		if(p->PauseOrStop==B0) //暂停
		{
			Screen_PastePicture(0x26A0,39,(568<<16)+278,(678<<16)+340,(377<<16)+373);	//2020.07.15 修正显示错误
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
			case 0x02: if(p->PauseOrStop==B0) //暂停
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
		
		//数据更新
    switch(p->RunModeStat)
	{
		case 0x01: //静态数据显示			
		{  FourBytes four;
			u8 buf[80]={0};			
			float buffer[40];
			float gas[8];
			Screen_ShowPage(38); 
			p->TimerSecUp=0;	 
			p->RunModeStat=2;	
			if(Flux_GetData()->ChannelC.RunMode==0)  //
			{
				
				//先拷贝折算与基准含氧量,负荷系数1.0
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_ZSXS]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_ZSXS];
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_ZSXS+1]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_ZSXS+1];
				
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_JZYHL]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_JZYHL];
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_JZYHL+1]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_JZYHL+1];
                four.FloatData=1.0f;
				SaveFlaotToFlash(four.FloatData,FLASHIS_AdrCLYQ_WJCX_JS_FHXS); //先默认负荷系数为1
				
				Flux_StarSampling('C',1.0);//打开气泵		
				Flux_GetData()->ChannelC.RunMode=1;

				mf_copy((u8*)"0:/YLB3330/Gas/link.txt",(u8*)"0:/YLB3330/Gas/temp0.txt",1);	//备份索引文件, add by alfred, 2020.07.06					 
				//先保存开始时间
				SdFile_ramlodair((char *)&SdFp->InterFace_data.dat[0],Interf_dat_length);  //先填充空格
				sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);

				//strncpy((char *)&SdFp->InterFace_data.dat[0],(char *)&buf[0],Interf_dat_length); 
				strncpy((char *)&ps->recorstarddat[0],(char *)&buf[0],Interf_dat_length); 

				SdFile_Serch_MaxofGasLinkFile();                                     
				p->FileNumber=SdFile_GetData()->GasLinkFileMax+1;     //搜索最大文件号
				Flux_GetData()->ChannelC.SaveFileNumber=p->FileNumber;                

				
				gas[0]=Sp->O2Concent.Value;
	            gas[1]=Sp->SO2Concent.Value;
				if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x01))==1)  //
				{
					
				//p->DisValue[4]=p->DisValue[4]-p->DisValue[5]*GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_XTSZ_CO_SO2); //修正后
                //if(p->DisValue[4]<0)p->DisValue[4]=0;
				 gas[1]=CalculationFormula_GetData()->SO2FromCO;//修正后
                }

	            gas[2]=Sp->NOConcent.Value;
	            gas[3]=Sp->NO2Concent.Value;
	            gas[4]=Sp->COConcent.Value;
	            gas[5]=Sp->H2SConcent.Value;
	            gas[6]=Sp->CO2Concent.Value;
				SaveGasDataLoad(buffer,gas);
				SaveGasData(buffer,p->FileNumber,1);
				SdFile_WriteGasFile(&SdFile_GetData()->InterFace_data,p->FileNumber,0,1);	//建立索引文件与文件常项	
					 
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
			sprintf((char*)p->Buffer,"%d",p->AgvTimer);//平均时间
			Screen_RefreshDataMulti(0x0420,p->Buffer,15*2);	
					
			Screen_ResetDataBuffer();
			sprintf((char*)p->Buffer,"%d",p->AgvTimes);//平均次数
			Screen_RefreshDataMulti(0x0460,p->Buffer,15*2);
   
			 }
			break;
      case 0x02: //动态态数据显示			
		{
			if(p->Timer==0)
			{							 

				Screen_ResetDataBuffer();
				sprintf((char*)p->Buffer,"%.1f",Flux_GetData()->ChannelC.ShowFlux);//流量值
				Screen_RefreshDataMulti(0x0400,p->Buffer,15*2);	

                Screen_ResetDataBuffer();
				sprintf((char*)p->Buffer,"%02d:%02d",Flux_GetData()->ChannelC.Sampling.SecTimer/60,(Flux_GetData()->ChannelC.Sampling.SecTimer)%60);//平均时间显示20200505
				Screen_RefreshDataMulti(0x0440,p->Buffer,15*2);

				p->DisValue[0]=Sp->O2Concent.Value;
				p->DisValue[1]=Sp->NOConcent.Value;
				p->DisValue[2]=Sp->NO2Concent.Value;
				p->DisValue[3]=Sp->NOxConcent.Value;
				
				p->DisValue[4]=Sp->SO2Concent.Value;
				
				p->DisValue[5]=Sp->COConcent.Value;
				
				if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x01))==1)  //
				{
				//p->DisValue[4]=p->DisValue[4]-p->DisValue[5]*GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_XTSZ_CO_SO2); //修正后
                //if(p->DisValue[4]<0)p->DisValue[4]=0;
				 p->DisValue[4]=CalculationFormula_GetData()->SO2FromCO;//修正后
					
                }
				
				p->DisValue[6]=Sp->H2SConcent.Value;
				p->DisValue[7]=Sp->CO2Concent.Value;
				if(pf->Data[FLASHIS_AdrCLYQ_DWLX]==1)//单位切换
				{
					for(int i=0;i<5;i++) 
					 p->DisValue[1+i]*=0.17f;
				}
				for(int i=0;i<8;i++)
				{ 
					Screen_ResetDataBuffer();
					if(p->DisValue[i]==SLAVE_ColFAULT)
					{
						sprintf((char*)p->Buffer,"------");//气体值
					}
					else
					{
						if(p->DisValue[i]>0)
						{
						if((i==0)||(i==7))	
						sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);//瞬时 
						else
						sprintf((char*)p->Buffer,"%.0f",p->DisValue[i]);//瞬时 	
						}
						else  //把负数的显示零
						{	
						if((i==0)||(i==7))	
						sprintf((char*)p->Buffer,"%.1f",0.0f);//瞬时 
						else
						sprintf((char*)p->Buffer,"%.0f",0.0f);
						}
					}
					Screen_RefreshDataMulti(0x0100+0x20*i,p->Buffer,15*2);
					
					Screen_ResetDataBuffer();
					if( (p->DisValue[10+i]==-1)|| (p->DisValue[10+i]==SLAVE_ColFAULT))
					{
						sprintf((char*)p->Buffer,"-------");//分钟
					}else
					{					
                        if(p->DisValue[i+10]>0)
						{
						if((i==0)||(i==7))	
						sprintf((char*)p->Buffer,"%.1f",p->DisValue[i+10]);//瞬时 
						else
						sprintf((char*)p->Buffer,"%.0f",p->DisValue[i+10]);//瞬时 	
						}
						else 
						{	
						if((i==0)||(i==7))	
						sprintf((char*)p->Buffer,"%.1f",0.0f);//瞬时 
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
	函数名：void Screen_CLYQ_CLJS_Pag(Screen_DataStructure *p)
	描述：	测量烟气-测量结束
*/
void Screen_CLYQ_CLJS_Pag(Screen_DataStructure *p)
{
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//按键处理
	  Button_CLYQ_CLJS_Pag(3,1,&FileValue);
	  if(p->RunMode!=0x5210) return;
		//选择基图更新
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
		
		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				 Screen_ShowPage(40); 
				 p->FileNumber=Flux_GetData()->ChannelC.SaveFileNumber;  //取出文件号
				 p->RunModeStat=2;				 
			 }
			break;
			}
}
//---
//---
/* date 2019-12-26
	函数名：void Screen_CLYQ_YQAX_Pag(Screen_DataStructure *p)
	描述：	测量烟气-测量结束-烟气清洗
*/
void Screen_CLYQ_YQAX_Pag(Screen_DataStructure *p)
{
		  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	  Button_CLYQ_YQQX_Pag(1,0,&FileValue);
	  if(p->RunMode!=0x5220) return;
		//选择基图更新
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{  
						case 0x01: Screen_PastePicture(0x0800,43,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;			
				   default:break;

			}
		}
		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
			 Screen_ShowPage(42); 				 
			 p->RunModeStat=2;	
             p->TimerSec=240;//5Min				 
			 }
			break;
			   case 0x02: //动态态数据显示			
			 {
				 if(p->Timer==0)
							{							 
								//时间倒计时
							sprintf((char*)p->Buffer,"%02d分%02d秒",Flux_GetData()->ChannelC.Sampling.DXTime/60,Flux_GetData()->ChannelC.Sampling.DXTime%60);//
                            Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);	
						    p->Timer=500;
						    }			 
			 }
			 break;
			}
}
//---2020-05-22
/* date 2019-12-26
	函数名：void Screen_CLYQ_YQCX_Pag(Screen_DataStructure *p)
	描述：	测量烟气-烟气查询
*/
void Screen_CLYQ_YQCX_Pag(Screen_DataStructure *p)
{
		  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	  Button_CLYQ_YQCX_Pag(11,1,&p->FileNumber);
	  if(p->RunMode!=0x5300) return;
		//选择基图更新
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
	  		


		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {	
				/*数据读取*/				
				 if(p->StarCun==1){ //第一次进入界面	
					SdFile_Serch_MaxofGasLinkFile();
					p->FileNumber=SdFile_GetData()->GasLinkFileMax;
					p->StarCun=0; 
					 p->Function=0;	
				 }
				 if(p->Function==0)
				 {
					 Screen_ShowPage(46); 
				 ReadGasFirstData(p->FileNumber); //第一页数据
				 }
				else
				{
					Screen_ShowPage(48);  
					GetGasSecondData(p->FileNumber); //第二页数据					
				}
								 
				p->RunModeStat=2;	
				//翻页指示		 
			 }
			break;
      case 0x02: //动态态数据显示			
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
	函数名：void Screen_CLYQ_YQCX_JS_Pag(Screen_DataStructure *p)
	描述：	测量烟气-文件查询-计算
*/
void Screen_CLYQ_YQCX_JS_Pag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	  Button_CLYQ_YQCX_JS_Pag(4,1,&FileValue);
	  if(p->RunMode!=0x5310) return;
		//选择基图更新
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
		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				 Screen_ShowPage(54); 	
				 
				 Screen_ResetDataBuffer();
				 sprintf((char*)p->Buffer,"%.2f",p->ZSXS);             //折算系数
				 Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);
				 
				 Screen_ResetDataBuffer();
				 sprintf((char*)p->Buffer,"%.2f",p->JZHYL);           //基准含氧量
				 Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);
				 
				 Screen_ResetDataBuffer();
				 sprintf((char*)p->Buffer,"%.2f",p->FHXS);           //负荷系数
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
      case 0x02: //动态态数据显示			
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
	函数名：void Screen_CLYQ_YQCX_JS_YQSR_Pag(Screen_DataStructure *p)
	描述：	测量烟气-文件查询-计算-烟气输入
*/
void Screen_CLYQ_YQCX_JS_YQSR_Pag(Screen_DataStructure *p)
{

	Flashisp_DataStructure* pf=Flashisp_GetData();

	//按键处理
	Button_CLYQ_YQCX_JS_YQSR_Pag(10,1,p->DisValue);
	if(p->RunMode!=0x5311) return;
	//选择基图更新
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
	//数据更新
	switch(p->RunModeStat)
	{
		case 0x01: //静态数据显示			,先读入当前的其他数据
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
		case 0x02: //动态态数据显示			
		{
			if(p->Timer==0)
			{							 
				if((p->Key.Mode<=7)&&p->Key.bOutsideKeyboard != false)
				{
					ScreenEnterNumberTwinkle(p,0x0100+(p->Key.Mode-1)*0x20);
				}
				if(p->Function==1)
				{ 
					sprintf((char*)p->Buffer,"保存路径:%7d",(u32)p->FileNumber);

					p->Timer=1500;
					p->Function=0;
				}else if(p->Function==2)
				{
					sprintf((char*)p->Buffer,"另存为路径:%7d",(u32)SdFile_GetData()->GasLinkFileMax);
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
	函数名：void Screen_CLYQ_YQCX_WJGL_Pag(Screen_DataStructure *p)
	描述：	测量烟气-文件查询-文件管理
*/
void Screen_CLYQ_YQCX_WJGL_Pag(Screen_DataStructure *p)
{

	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  static float DisBuffer[2]={1,1};	//modify by alfred, 2020.06.04
	 //按键处理
	  Button_CLYQ_YQCX_WJGL_Pag(5,1,DisBuffer);
	  if(p->RunMode!=0x5320) return;
		//选择基图更新
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
		//数据更新
    switch(p->RunModeStat)
	{
		case 0x01: //静态数据显示			
		{
			Screen_ShowPage(58); 
			if(p->LoadFlag)
			{
				p->LoadFlag = 0;
				Screen_ResetDataBuffer();		
				sprintf((char*)p->Buffer,"          ");
				Screen_RefreshDataMulti(0x3B20,p->Buffer,15*2);	
				
				//初始化起始文件号为1, 结束文件号为最大文件号, modify by alfred, 2020.07.09
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
		case 0x02: //动态态数据显示			
		{
			if(p->Timer==0)
			{							 
				if((p->Key.Mode<=3)&&(p->Key.bOutsideKeyboard != false))
				{
					ScreenEnterNumberTwinkle(p,0x0100+(p->Key.Mode-1)*0x20);
				}
				
				//增加文件导出时的状态提示, add by alfred, 2020.07.11
				if(Usb_GetData()->upstatus == 6)
				{
					Dis_Tips(p,(u8*)"读取U盘失败，请检楂并插入U盘!",0x3B20);					
				}
				else if(Usb_GetData()->upstatus == 1)
				{
					Dis_Tips(p,(u8*)"导出文件失败!",0x3B20);					
				}
				else if(Usb_GetData()->upstatus == 4)
				{
					Dis_Tips(p,(u8*)"导出文件成功!",0x3B20);					
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
	函数名：void Screen_CLYQ_YQCX_DYXX_Pag(Screen_DataStructure *p)
	描述：	测量烟气-文件查询-打印选项
*/
void Screen_CLYQ_YQCX_DYXX_Pag(Screen_DataStructure *p)
{

	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  u16 BoxChoseY[8]={83,119,157,192,227,263,300,334};
		u16 BoxChoseX[4]={52,239,421,607};
	  float DisBuffer[2]={1,10};
	 //按键处理
	  Button_CLYQ_YQCX_DYXX_Pag(33,0,DisBuffer);
	  if(p->RunMode!=0x5330) return;
		//选择基图更新
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
		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
					Screen_ShowPage(52); 
			 
				 p->RunModeStat=2;				 
			 }
			break;
			}
}
//---
/* date 20200516
	函数名：void Screen_CLYQ_YQCX_FZSJ_Pag(Screen_DataStructure *p)
	描述：	测量烟气-烟气查询-分钟数据
*/
void Screen_CLYQ_YQCX_FZSJ_Pag(Screen_DataStructure *p)
{

	static int64_t LastChose=0;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//按键处理
	Button_CLYQ_YQCX_FZSJ_Pag(7,1,&FileValue);
	if(p->RunMode!=0x5340) return;
	//选择基图更新
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
	//数据更新
	switch(p->RunModeStat)
	{
		case 0x01: //静态数据显示			
		{
			Screen_ShowPage(44); 	
			if(p->LoadFlag==1)
			{
				p->LoadFlag=0;
				p->RecordCun=1;
				p->UseGasCal=0;//计算平均标志
				SdFile_Serch_MaxofGasRecord(p->FileNumber);
				if(SdFile_GetData()->GasRecordMax>64) SdFile_GetData()->GasRecordMax=64; //一次最多计算64条平均数据
			}
			//读取分钟数据
			printf("3p->RecordCun=%d\r\n",p->RecordCun);
			printf("(3p->FileNumber=%d\r\n",(u32)p->FileNumber);
			GetGasFileRecodData(p->FileNumber,p->RecordCun,p->DisValue,1);
			if(p->DisValue[8]==1)
				p->UseGasCal=1;   //取出参与计算标志
			else
				p->UseGasCal=0;   //取出参与计算标志
				p->RunModeStat=2;				 
		}
		break;
		case 0x02: //动态数据显示数据显示			
		{
			break;
		}				 
			 
	}
	if(LastChose!=p->UseGasCal||p->RecordCun!=p->LastRecordCun)		//参与计算标志基图显示
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
	函数名：void Screen_YCCY_CYSZ_Pag(Screen_DataStructure *p)
	描述：	烟尘采样-采样设置
*/
void Screen_YCCY_CYSZ_Pag(Screen_DataStructure *p)
{
	 
    Slave_DataStructure*  Sp=Slave_GetData();
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  Calculation_FormulaStructure * Cp=CalculationFormula_GetData();
	 //按键处理
	  Button_YCCY_CYSZ_Pag(9,1,p->DisValue);
		
		
		//其他数据计算
	  if(p->RunMode!=0x6000) return;
		//选择基图更新
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
		//数据更新
    switch(p->RunModeStat)
		{
		    case 0x01: //静态数据显示			
			 {
				 //显示值计算
			
					p->DisValue[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CYLS);	//装载采样流速
					p->DisValue[1]=GetU32dataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_LTH);//滤筒号
					p->DisValue[2]=pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS];
				    p->DisValue[2]=p->DisValue[2]/60; //单独采样时间min 最大99	     
					p->DisValue[3]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ); //采嘴直径
					p->DisValue[4]=pf->Data[FLASHIS_AdrYDBD_CYDS]; //总采样点
					p->DisValue[5]=(pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS])*p->DisValue[4];//总时长
				 if(pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]==1)
					{	p->DisValue[6]=CalculationFormula_Qrs(GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ),GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_YQLS_YQLS),GetFlaotdataFormFlash2Word(FLASHIS_AdrCHSL_HSL)/100);
						p->DisValue[7]=p->DisValue[0]*(p->DisValue[2])*(p->DisValue[4]); //预计工况体积
						p->DisValue[8]=CalculationFormula_Vnd(Cp->Tj,p->DisValue[7],Cp->Ba ,Cp->Pcs);  
					}else
					{
						p->DisValue[6]=p->DisValue[0];
						p->DisValue[7]=p->DisValue[0]*(p->DisValue[2])*(p->DisValue[4]); //预计工况体积
						p->DisValue[8]=CalculationFormula_Vnd(Cp->Tj,p->DisValue[7],Cp->Ba ,Cp->Pcs);              //预计标况体积
					}
				 Screen_ShowPage(60);// Dis_Tips(p,"信息提示",0x440);
        if(((pf->Data[FLASHIS_AdrXTSZBOX])&(1<<4))==0)	//烟气采样类型
				{
				  sprintf((char*)p->Buffer,"烟尘采样");
				}	else
                {				
					sprintf((char*)p->Buffer,"油烟采样");
				}
				Screen_RefreshDataMulti(0x0460,p->Buffer,15*2);	
				Screen_ResetDataBuffer();	
				GetYDBD_Describe(pf->Data[FLASHIS_AdrYDBDLX]);	//烟道布点			
				Screen_RefreshDataMulti(0x0420,p->Buffer,15*2);	
        //批量显示				
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
				sprintf((char*)p->Buffer,"%2d",pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS]%60);//采样时间S
				Screen_RefreshDataMulti(0x0480,p->Buffer,15*2);
				p->RunModeStat=2;	
				if(p->QimiStat==0)
                Dis_Tips(p,(u8 *)"未检测气密性",0x0440);
                else if(p->QimiStat==1)
				 Dis_Tips(p,(u8 *)"气密性不通过",0x0440);
				else if(p->QimiStat==2)
				 Dis_Tips(p,(u8 *)"气密性通过  ",0x0440);
					
			 }
			break;
			case 0x02: //动态态数据显示			
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
											else if(p->Key.Mode==7)    //总采样点
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
	函数名：void Screen_YCCY_QMJC_Pag(Screen_DataStructure *p)
	描述：	烟尘采样-气密检测
*/
void Screen_YCCY_QMJC_Pag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	   Calculation_FormulaStructure * Cp=CalculationFormula_GetData();
	 //按键处理
	  Button_YCCY_QMJC_Pag(2,1,&FileValue);
	  if(p->RunMode!=0x6100) return;
		//选择基图更新
	 	if(p->LastChoice!=p->PauseOrStop)	
		{
			p->LastChoice=p->PauseOrStop;
		if(p->PauseOrStop==B0) //暂停
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

	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				 Screen_ShowPage(62); 	
         p->TimerSecUp=0;				 
				 p->RunModeStat=2;	

				// p->PauseOrStop=0;
         Dis_Tips(p,(u8 *)"",0x0160);//清空提示	 
			 }
			break;
      case 0x02: //动态态数据显示			
			 {
				 if(p->Timer==0)
							{	
                               if(p->PauseOrStop==B0)
								{ 
								   p->TimerSecUp++;
								   Dis_Tips(p,(u8 *)"开始检测气密",0x0160);	
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
                               if(p->TimerSecUp<60&& p->DisValue[1]<-13.0f) //检测通过,用15L流量抽气，60秒内达不到设定值附近。
                               {
								    p->QimiStat=2;  //气密良好
									p->TimerSecUp=0;
									p->PauseOrStop=0;
						            Flux_StopSampling('B');
									Dis_Tips(p,(u8 *)"气密检测良好",0x0160);	
								}
							   else if(p->TimerSecUp>60)
								{
								   p->QimiStat=1; //漏气
								   p->PauseOrStop=0;
								   p->TimerSecUp=0;
						           Flux_StopSampling('B');
								   Dis_Tips(p,(u8 *)"气密检测漏气",0x0160);	
								}								
						    p->Timer=1000;
								
						}			 
			 }
			break;
			}
}
//---
/* date 2019-12-26
	函数名：void Screen_YCCY_KSJC_Pag(Screen_DataStructure *p)
	描述：	烟尘采样-开始检测
*/
void Screen_YCCY_KSJC_Pag(Screen_DataStructure *p)
{
  	Slave_DataStructure* Sp=Slave_GetData();
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  Calculation_FormulaStructure * Cp=CalculationFormula_GetData();
	 //按键处理
	  Button_YCCY_KSCL_Pag(4,1,&FileValue);
	  if(p->RunMode!=0x6200) return;
		//选择基图更新
	if(p->LastChoice!=p->YC_Pause)	
		{
			p->LastChoice=p->YC_Pause;
		if(p->YC_Pause==B0) //暂停
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
				      case 0x03: if(p->YC_Pause==B0) //暂停
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

		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {FourBytes four;
		        Screen_ShowPage(64); 
				//先拷贝折算与基准含氧量,负荷系数1.0
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_ZSXS]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_ZSXS];
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_ZSXS+1]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_ZSXS+1];
				
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_JZYHL]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_JZYHL];
				pf->Data[FLASHIS_AdrCLYQ_WJCX_JS_JZYHL+1]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_JZYHL+1];
                four.FloatData=1.0f;
				SaveFlaotToFlash(four.FloatData,FLASHIS_AdrCLYQ_WJCX_JS_FHXS); //先默认负荷系数为1		 
				 
                p->YC_SampleAllTime=0;
                p->YC_SampleTimerSec=pf->Data[FLASHIS_AdrYCCJ_CYSE_DDCS];		//单次采样时间		 
		        p->RunModeStat=2;
				 
         if(Flux_GetData()->ChannelB.RunMode==0)
		 {   FourBytes four;
			 SdFile_Serch_MaxofDustLinkFile();
			 p->FileNumber=SdFile_GetData()->DustLinkFileMax+1;	 
			 Flux_GetData()->ChannelB.SaveFileNumber=p->FileNumber;  //获得烟尘最大文件号
			 //保存开始采样时间
			 four.U32Data = mktime(&Screen_GetData()->Clock.Clock);
			 Flashisp_GetData()->Data[FLASHIS_AdrCJKSSK] = four.U16Data[0];
			 Flashisp_GetData()->Data[FLASHIS_AdrCJKSSK+1] = four.U16Data[1];

		 }
		 else
		 {
			p->FileNumber= Flux_GetData()->ChannelB.SaveFileNumber; //提取运行中的文件号
		 }

		 if(Flux_GetData()->ChannelB.RunMode!=3)	
         {			 
         p->YC_Pause=0;	
         Dis_Tips(p,(u8 *)"测量中",0x0520);	 	
		 }

		 p->YC_Stat=1; //开始测量标志				 
                    
         if(pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]==1) //等速
				{   if((Flux_GetData()->ChannelB.RunMode!=2)&&(Flux_GetData()->ChannelB.RunMode!=3))
					{
					Flux_StarSampling('B',CalculationFormula_Qrs(GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ),GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_YQLS_YQLS),GetFlaotdataFormFlash2Word(FLASHIS_AdrCHSL_HSL)/100)); //等速流量是目标流量
					Flux_GetData()->ChannelB.RunMode=1;  //启动线程
					}
				}
				else
				{   if((Flux_GetData()->ChannelB.RunMode!=2)&&(Flux_GetData()->ChannelB.RunMode!=3))
					{
					Flux_StarSampling('B',GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CYLS)); //目标流量
					Flux_GetData()->ChannelB.RunMode=1;  //启动线程
					}
				}  	
     		
			 }
			break;
      case 0x02: //动态态数据显示			
			 { //  FourBytes four;
				 if(p->Timer==0)
							{   p->Timer=250;//1 S 更新一次
                                
                               	p->DisValue[9]=Flux_DataMiddleFilter(&Flux_GetData()->ChannelB.FluxTowData.Filter,Flux_GetData()->ChannelB.ShowFlux);
								if(p->DisValue[9]<Slave_GetData()->DustMotor.Pid.Sv*0.95f)
				                p->DisValue[9]=Flux_GetData()->ChannelB.ShowFlux;//烟尘流量
								
								p->CT++;
								if(p->CT>=2)
								{  p->CT=0;

								u32 cal=Flux_GetData()->ChannelB.Sampling.DinSiTime-Flux_GetData()->ChannelB.Sampling.IntTime;	//20200604					 
								sprintf((char*)p->Buffer,"%02d:%02d", cal/60,cal%60);//倒计时
								Screen_RefreshDataMulti(0x0501,p->Buffer,15*2);	
								sprintf((char*)p->Buffer,"%2d %2d", Flux_GetData()->ChannelB.Sampling.Cnt,pf->Data[FLASHIS_AdrYDBD_CYDS]);//流量值
								Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);	
								//数据装载
								p->DisValue[0]=Sp->DustMotor.Ratio/Sp->DustMotor.Pid.PWMcycle*100; //功率	
								p->DisValue[2]=Flux_GetData()->ChannelB.Sampling.SecTimer;
								
								if(pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]==1) //等速
								{
									if((cal%4==0)&&(Flux_GetData()->ChannelB.Sampling.IntTime!=0))
									{
								      //p->DisValue[3]=Flux_GetData()->ChannelA.ShowFlux;//目标流量
									  p->DisValue[3]=CalculationFormula_Qrs(GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ),CalculationFormula_GetData()->Vs,GetFlaotdataFormFlash2Word(FLASHIS_AdrCHSL_HSL)/100); //等速流量是目标流量
									}
									if(Flux_GetData()->ChannelB.Sampling.IntTime==0)
									{
									 p->DisValue[3]=CalculationFormula_Qrs(GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ),GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_YQLS_YQLS),GetFlaotdataFormFlash2Word(FLASHIS_AdrCHSL_HSL)/100);
									}
								}
								else
								{
								  p->DisValue[3]=GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CYLS);//目标流量
								} 
								//p->DisValue[9]=Flux_GetData()->ChannelB.ShowFlux;//采样流量


								p->DisValue[1]=p->YC_FollowRate;                 //跟踪率       								
								
								p->DisValue[5]=Cp->Pd;                           //烟气动压
								p->DisValue[6]=Cp->Ps;                           //烟气静压
								p->DisValue[7]=Cp->Ts;                           //烟气温度
								
								/*
								
								float Pvalue;
								if((pf->Data[FLASHIS_AdrXTSZBOX]&0x40)==0)
								Pvalue=CalculationFormula_p(p->DisValue[7],Cp->Ba,p->DisValue[6]);//计算烟气密度(烟温，大气压，静压）
				                else
								Pvalue=CalculationFormula_p(Cp->Th,Cp->Ba,0.0f);//计算烟气密度(烟温，大气压，静压）
								
								four.U16Data[0]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_PTG_KP];// 皮托管   84/100.0=0.84
	                            four.U16Data[1]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_PTG_KP+1];
				                p->DisValue[4]=CalculationFormula_Vs(four.FloatData,p->DisValue[5],Pvalue);	//计算流速（皮托管，动压，密度）
				                p->DisValue[8]=CalculationFormula_Qs(p->DisValue[4],Cp->F);//烟气流量	
                                */
								
                                p->DisValue[4]=Cp->Vs;  //计算流速
								p->DisValue[8]=Cp->Qs; //烟气流量

								p->DisValue[10]=Cp->Tj;                          //计前温度
								p->DisValue[11]=Cp->Pcs;                         //计前压力

								p->DisValue[12]=Flux_GetData()->ChannelB.Sampling.SumVolume;  //工况体积20200604
								p->DisValue[13]=Flux_GetData()->ChannelB.Sampling.RefVolume;  //标况体积20200604
								
								p->DisValue[14]=CalculationFormula_GetData()->Qsnd;//标杆流量
								for(int i=0;i<15;i++)
								{
									if(i==2) 	
									sprintf((char*)p->Buffer,"%02d:%02d:%02d",(u32)p->DisValue[i]/3600,(u32)p->DisValue[i]%3600/60,(u32)p->DisValue[i]%60);//流量值
									else if((i==6)||(i==11))
									sprintf((char*)p->Buffer,"%.2f",p->DisValue[i]);//静压，计压
									else if((i==5)||(i==0))
									sprintf((char*)p->Buffer,"%.0f",p->DisValue[i]);//动压,功率
									else
									sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);//流量值
									Screen_RefreshDataMulti(0x0120+0x20*i,p->Buffer,15*2);	
								}				
							 if(p->YC_Pause!=B0)   //20200529 	
					           {
                                  Dis_Tips(p,(u8 *)"测量中",0x0520);	                              //20200529 								   
							   }
						       
								YC_SampleControl();   
						   }
						}		
               //以下为检测电压 20200529
          if(p->YC_Pause!=B0)
					{
						 //暂停
					 if((Battery_GetData()->LostPV.ActualV<LowPOWER)&&(Power_GetData()->Flg&0x04))
					 {	 
						 p->YC_Pause=B0;
						 Dis_Tips(p,(u8 *)"电量过低",0x0520);	                              //20200529 
						 Flux_StopSampling('B');
						 Flux_GetData()->ChannelB.RunMode=3;  //暂停20200604
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
	函数名：void Screen_CLYQ_CLJS_Pag(Screen_DataStructure *p)
	描述：	烟尘采样-测量结束保存
*/
void Screen_YCCY_CLJS_Pag(Screen_DataStructure *p)
{
	Slave_DataStructure* Sp=Slave_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	Calculation_FormulaStructure * Cp=CalculationFormula_GetData();
	//按键处理
	Button_YCCY_CLJS_Pag(3,1,&FileValue);
	if(p->RunMode!=0x6210) return;
	//选择基图更新
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
	//数据装载
	p->FileNumber=Flux_GetData()->ChannelB.SaveFileNumber;
	p->DisValue[0]=p->FileNumber; //文件号
	p->DisValue[1]=GetU32dataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_LTH);//滤筒号
	p->DisValue[2]=Cp->Vs;                           //烟气流速
	p->DisValue[3]=Cp->Pd;                           //烟气动压
	p->DisValue[4]=Cp->Ps;                           //烟气静压
	p->DisValue[5]=Cp->Ts;                           //烟气温度
	p->DisValue[6]=p->YC_FollowRate;                 //跟踪率      实测流量/目标流量   
	p->DisValue[7]=Flux_GetData()->ChannelB.Sampling.SecTimer;//p->YC_SampleAllTime;                           //累计采时		
			
	p->DisValue[8]=Flux_GetData()->ChannelB.Sampling.SumVolume;  //工况体积20200604
	p->DisValue[9]=Flux_GetData()->ChannelB.Sampling.RefVolume;  //标况体积20200604
	p->DisValue[10]=CalculationFormula_GetData()->Qsnd;//标杆流量20200605
	//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
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
								  sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);//流量值
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
	函数名：void Screen_CLYQ_YQAX_Pag(Screen_DataStructure *p)
	描述：	烟尘采样-测量结束-烟尘防倒吸
*/
void Screen_YCCY_YCFDX_Pag(Screen_DataStructure *p)
{
		 Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	  Button_YCCY_YCFDX_Pag(1,0,&FileValue);
	  if(p->RunMode!=0x6220) return;
		//选择基图更新
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{  
						case 0x01: Screen_PastePicture(0x0800,69,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;			
				   default:break;

			}
		}
		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				 Screen_ShowPage(68); 				 
				 p->RunModeStat=2;	
             p->TimerSec=10;//5Min	
        // Flux_ContinueSampling('B',10);//防倒吸  10L/min  10s				 
			 }
			break;
			   case 0x02: //动态态数据显示			
			 {
				 if(p->Timer==0)
				{	p->DisValue[0]=Flux_GetData()->ChannelB.ShowFlux;						
					sprintf((char*)p->Buffer,"%.1f",p->DisValue[0]);//流量值
                    Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);
              	    sprintf((char*)p->Buffer,"%2d",Flux_GetData()->ChannelB.Sampling.DXTime);//倒计时时间
                    Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);									
					p->Timer=500;
				}			 
			 }
			 break;
			}
}
//---
/* date 20200521
	函数名：void Screen_CLYQ_YQCX_Pag(Screen_DataStructure *p)
	描述：	烟气采样-烟尘文件查询
*/
void Screen_YCCY_YCCX_Pag(Screen_DataStructure *p)
{

	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	//按键处理
	Button_YCCY_YCCX_Pag(10,1,&(p->FileNumber));
	if(p->RunMode!=0x7000) return;
	//选择基图更新
	if(p->Key.LastOpt!=p->Key.Opt)
	{ 
		p->Key.LastOpt=p->Key.Opt; 
		switch(p->Key.Opt)
		{
			//更正菜单显示位置, modify by alfred, 2020.07.13
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
	  		
		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {   p->RunModeStat=2;	
				/*数据读取*/				
				 if(p->StarCun==1){ //第一次进入界面
					SdFile_Serch_MaxofDustLinkFile();
					p->FileNumber=SdFile_GetData()->DustLinkFileMax;
					p->StarCun=0; 
					p->Function=0;	
				 }
		
				 if(p->Function==0)                    
				 { 
					
					//根据读出来的数据中数据类型判断要显示的是烟尘文件还是油烟文件20200907
					 Screen_ShowPage(72);   //烟尘页 
					
					 ReadDustData(p->FileNumber);
					 if(p->DisValue[31]==0)
					 {  //基图COPY 烟尘

						Screen_PastePicture(0x4120,72,(6<<16)+108,(661<<16)+436,(6<<16)+108);
						ReadDustFirstData(p->FileNumber); //烟尘第一页数据,
						 
					 }
					 else
					 { //基图COPY 油烟
                        
						Screen_PastePicture(0x4120,112,(6<<16)+108,(661<<16)+436,(6<<16)+108);
					    ReadLampBlackFirstData(p->FileNumber);    //油烟第一页
						 
					 }
					
				 }
				 else
				 {
					Screen_ShowPage(74);  
					GetDustSecondData(p->FileNumber); //第二页数据
				 }

			 	break;
			 }
		
		case 0x02: //动态态数据显示			
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
					 {  //基图COPY 烟尘

						Screen_PastePicture(0x4120,112,(6<<16)+108,(661<<16)+436,(6<<16)+108);//4120
						ReadDustFirstData(p->FileNumber); //烟尘第一页数据,
					 }
					 else
					 { //基图COPY 油烟
						

						 Screen_PastePicture(0x4120,112,(6<<16)+108,(661<<16)+436,(6<<16)+108);
						 ReadLampBlackFirstData(p->FileNumber);                      //油烟第一页
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
	函数名：void Screen_YCCY_YCCX_JS_YC_Pag(Screen_DataStructure *p)
	描述：	测量烟气-文件查询-计算
*/
void Screen_YCCY_YCCX_JS_YC_Pag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	  Button_YCCY_YCCX_JS_YC_Pag(9,1,p->DisValue);
	  if(p->RunMode!=0x7100) return;
		//选择基图更新
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
	
		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				 Screen_ShowPage(80);
				 if(p->LoadFlag==1)
				 {
					p->LoadFlag=0;
					Dis_Tips(p,(u8 *)"        ",0x01C0);
				 }

				 sprintf((char*)p->Buffer,"%.5f",p->DisValue[19]);  //烟尘净重[19]
				 Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);
				 
				 sprintf((char*)p->Buffer,"%.1f",p->DisValue[20]); //含氧量[20]
				 Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);
				 
				 sprintf((char*)p->Buffer,"%.2f",p->DisValue[10]); //负荷系数[10]
				 Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);
				 
				 sprintf((char*)p->Buffer,"%.2f",p->DisValue[9]); //折算系数[9]
				 Screen_RefreshDataMulti(0x0160,p->Buffer,15*2);
				 
				 sprintf((char*)p->Buffer,"%.1f",p->DisValue[35]); //基准含氧量[35]
				 Screen_RefreshDataMulti(0x0180,p->Buffer,15*2);
				 
				 sprintf((char*)p->Buffer,"%07d",(u32)p->DRWJH);
				 Screen_RefreshDataMulti(0x01A0,p->Buffer,15*2);			
				 p->RunModeStat=2;				 
			 }
			break;
      case 0x02: //动态态数据显示			
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
	函数名：void Screen_YCCY_YCCX_JS_YY_Pag(Screen_DataStructure *p)
描述：	测量烟气-文件查询-计算-油烟
*/
void Screen_YCCY_YCCX_JS_YY_Pag(Screen_DataStructure *p)
{
		
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	  Button_YCCY_YCCX_JS_YY_Pag(4,1,p->DisValue);
	  if(p->RunMode!=0x7400) return;
		//选择基图更新
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
	
		//数据更新
    switch(p->RunModeStat)
		{
		  case 0x01: //静态数据显示			
			 {
				 Screen_ShowPage(84);
	             /*
				                   //溶液浓度[32]
					               //定容体积[33]
					               //灶头数目[34]
				 
				*/
                 //Dis_Tips(p,(u8 *)"提示信息：---",0x01C0);	
				 
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
      case 0x02: //动态态数据显示			
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
	函数名：void Screen_YCCY_YCCX_WJGL_Pag(Screen_DataStructure *p)
	描述：	烟尘采样-文件查询-文件管理
*/
void Screen_YCCY_YCCX_WJGL_Pag(Screen_DataStructure *p)
{
	Flashisp_DataStructure* pf=Flashisp_GetData();
	static float DisBuffer[2]={1,1};	//modify by alfred, 2020.06.04
	//按键处理
	Button_YCCY_YCCX_WJGL_Pag(5,1,DisBuffer);
	if(p->RunMode!=0x7200) return;
	//选择基图更新
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
	//数据更新
	switch(p->RunModeStat)
	{
		case 0x01: //静态数据显示			
		{
			Screen_ShowPage(82); 	//更正显示页面, modify by alfred, 2020.06.04
			if(p->LoadFlag)
			{			
				p->LoadFlag = 0;				
				Screen_ResetDataBuffer();		
				sprintf((char*)p->Buffer,"          ");
				Screen_RefreshDataMulti(0x3B20,p->Buffer,15*2);		
				
				
				//初始化起始文件号为1, 结束文件号为最大文件号, modify by alfred, 2020.07.09
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
		case 0x02: //动态态数据显示			
		{
			if(p->Timer==0)
			{							 
				if((p->Key.Mode<=3)&&p->Key.bOutsideKeyboard != false)
				{
					ScreenEnterNumberTwinkle(p,0x0100+(p->Key.Mode-1)*0x20);
				}
				if(Usb_GetData()->upstatus == 6)
				{
					Dis_Tips(p,(u8*)"读取U盘失败，请检楂并插入U盘!",0x3B20);					
				}
				else if(Usb_GetData()->upstatus == 1)
				{
					Dis_Tips(p,(u8*)"导出文件失败!",0x3B20);					
				}
				else if(Usb_GetData()->upstatus == 4)
				{
					Dis_Tips(p,(u8*)"导出文件成功!",0x3B20);					
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
	函数名：void Screen_YCCY_YCCX_DYXX_Pag(Screen_DataStructure *p)
	描述：	烟尘采样-文件查询-打印选项
*/
void Screen_YCCY_YCCX_DYXX_Pag(Screen_DataStructure *p)
{
    u16 BoxChoseY[8]={90,128,164,201,234,270,307,341};
		u16 BoxChoseX[4]={52,239,421,607};
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  static u32 LastChose=0x00;
	 //按键处理
	  Button_YCCY_YCCX_DYXX_Pag(34,0,p->DisValue);
	  if(p->RunMode!=0x7300) return;
		//选择基图更新
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
		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
					Screen_ShowPage(78); 
			 
				 p->RunModeStat=2;				 
			 }
			break;
			}
}
//---
/* date 2020-1-15
	函数名：void Screen_JZWH_Pag(Screen_DataStructure *p)
	描述：	校准维护-输入密码-界面逻辑处理
*/
void Screen_JZWH_MMSR_Pag(Screen_DataStructure *p)
{
	//	FourBytes four;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//按键处理
	Button_WHJZ_MMSR_Pag(2,1,p->DisValue);
	if(p->RunMode!=0x8000) return;
	//选择基图更新
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
	 //数据更新
	switch(p->RunModeStat)
	{
		case 0x01: //静态数据显示			
		{
			Screen_ShowPage(86); 
			sprintf((char*)p->Buffer,"         ");
			Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);				 
			p->RunModeStat=2;
		}
		break;
		case 0x02: //动态数据显示
		{			
			if(p->Timer==0)
			{	
				if(p->Warning==1)
				{
					Dis_Tips(p,(u8 *)"密码输入错误",0x0120);			
					p->Timer=2000;
					p->Warning=0;
				}
				else if(p->Warning==2)
				{
					Dis_Tips(p,(u8 *)"再次输入密码",0x0120);			
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
	函数名：void Screen_JZWH_WHXZ_Pag(Screen_DataStructure *p)
	描述：	校准维护-维护选择-界面逻辑处理
*/
void Screen_JZWH_WHXZ_Pag(Screen_DataStructure *p)
{
//	FourBytes four;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
		  Button_WHJZ_WHXZ_Pag(9,1,p->DisValue);
	  if(p->RunMode!=0x8100) return;
			//选择基图更新
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
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
			 Screen_ShowPage(88); 	
       p->RunModeStat=2;				 
			 }

			break;
		}
	
}
//---
/* date 2020-1-16
	函数名：void Screen_WHJZ_WHXZ_STSZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-系统设置
*/
void Screen_WHJZ_WHXZ_STSZ_Pag(Screen_DataStructure *p)
{

	  static u8 LastChoice=11;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	  Button_WHJZ_WHXZ_STSZ_Pag(13,1,p->DisValue);
	  if(p->RunMode!=0x8110) return;
		//选择基图更新
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
	

		//数据更新
    switch(p->RunModeStat)
	{
	case 0x01: //静态数据显示			
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
	case 0x02: //动态态数据显示			
	{
		if(p->Timer==0)
		{	
			if(p->gSysConfigWaringBits.m_byData!=0)
			{
				if(p->gSysConfigWaringBits.BitField.m_uDustFile == 1)
				{
					Dis_Tips(p,(u8*)"烟尘文件删除成功!",0x01E0);		
				}
				else if(p->gSysConfigWaringBits.BitField.m_uGasFile == 1)
				{
					Dis_Tips(p,(u8*)"烟气文件删除成功!",0x01E0);
				}
				else if(p->gSysConfigWaringBits.BitField.m_uWorkFile == 1)
				{
					Dis_Tips(p,(u8*)"工况文件删除成功!",0x01E0);
				}
				else if(p->gSysConfigWaringBits.BitField.m_uAllFile == 1)
				{
					Dis_Tips(p,(u8*)"全部文件删除成功！",0x01E0);
				}
				else if(p->gSysConfigWaringBits.BitField.m_data0 == 1)
				{
					Dis_Tips(p,(u8*)"文件删除失败！",0x01E0);
				}
				else if(p->gSysConfigWaringBits.BitField.m_factorySet == 1)
				{
					Dis_Tips(p,(u8*)"系统已恢复出厂设置！",0x01E0);
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
	函数名：void Screen_WHJZ_WHXZ_STSZ_CO_SO2_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-系统设置-CO对SO2修正
*/
void Screen_WHJZ_WHXZ_STSZ_CO_SO2_Pag(Screen_DataStructure *p)
{

	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 Slave_DataStructure* Sp=Slave_GetData();   //20200525
	 //按键处理
	  Button_WHJZ_WHXZ_STSZ_CO_SO2_Pag(23,0,p->DisValue);
	  if(p->RunMode!=0x8111) return;
		//选择基图更新
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
			  
//					    case 0x01: if(pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&0x004)//单位切换
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


		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				Screen_ShowPage(114); 
                for(int i=0;i<10;i++) //浓度
				{		

				  p->DisValue[i]=pf->Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1+i];
				  sprintf((char*)p->Buffer,"%d",(u32)p->DisValue[i]);
				  Screen_RefreshDataMulti(0x0100+i*0x20,p->Buffer,15*2);
				  p->DisValue[i+10]=(float)pf->Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ1+i]/100;  //因为不是浮点数，所以要放大与缩小才能保存小数
				  sprintf((char*)p->Buffer,"%.0f",p->DisValue[i+10]);
				  Screen_RefreshDataMulti(0x0400+i*0x20,p->Buffer,15*2);
				}	
				 p->RunModeStat=2;	
               //  Flux_StarSampling('C',1.0);   //进入启动C泵   20200525				
			 }
			break;
      case 0x02: //动态态数据显示			
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
								float  databuf=(float)3000/4096;   //求AD每一级电压单位mV/级
 								DisPlay[0]=Sp->Data[SLAVE_MAP_COConcent].Raw*databuf   ;//CO原始值20200525
								DisPlay[1]=Sp->COConcent.Value;  //CO测量值20200525
								DisPlay[2]=Sp->Data[SLAVE_MAP_SO2Concent].Raw*databuf      ;//SO2原始值 20200525
								DisPlay[3]=Sp->SO2Concent.Value;  //SO2测量值20200525 ,修正前
								DisPlay[4]=Sp->SO2Concent.Value;  //SO2测量值20200525 ,修正前
								if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x01))==1)  //
				                {
								DisPlay[4]=CalculationFormula_GetData()->SO2FromCO; //修正后SO2
                                //if(DisPlay[4]<0)DisPlay[4]=0;   //负数
				                }
								DisPlay[5]=Flux_GetData()->ChannelC.ShowFlux;//流量  20200525
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
	函数名：void Screen_WHJZ_WHXZ_LLJZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-流量校准
*/
void Screen_WHJZ_WHXZ_LLJZ_Pag(Screen_DataStructure *p)
{
      FourBytes four; 
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  Slave_DataStructure* Sp =Slave_GetData();
	 //按键处理
	  Button_WHJZ_WHXZ_LLJZ_Pag(0x10,1,p->DisValue);
	  if(p->RunMode!=0x8120) return;
		//选择基图更新
	 if(p->LastCoiceonoff!=p->PauseOrStop)	
		{
			p->LastCoiceonoff=p->PauseOrStop;
			if(p->PauseOrStop==B0) //停止
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
				case 0x0f: if(p->PauseOrStop==B0) //暂停
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
	  		
		if(p->LastChoice!=pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ])	 //打钩
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
	   if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]<0x0a)	 //烟尘
		 {
		   	p->DisValue[14]=Sp->BCPress.Value;//流压
	    	p->DisValue[15]=Sp->BGPress.Value;//计压
  			//p->DisValue[0]=Flux_GetData()->ChannelB.ShowFlux;//烟尘流量
			p->DisValue[0]=Flux_DataMiddleFilter(&Flux_GetData()->ChannelB.FluxTowData.Filter,Flux_GetData()->ChannelB.ShowFlux);  //加入中值滤波20200529
			if(p->DisValue[0]<Slave_GetData()->DustMotor.Pid.Sv*0.8f)
			p->DisValue[0]=Flux_GetData()->ChannelB.ShowFlux;//烟尘流量
		 }
		 else if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]==0x0a) //烟气
		 {
		   p->DisValue[14]=Sp->CCPress.Value;//C路差压（流压），单位kPaSp->ACPress.Value;//流压
	       p->DisValue[15]=0.02;//Sp->AGPress.Value;//计压
		 //  p->DisValue[0]=Flux_GetData()->ChannelC.ShowFlux;//烟气流量
			p->DisValue[0]=Flux_DataMiddleFilter(&Flux_GetData()->ChannelC.FluxTowData.Filter,Flux_GetData()->ChannelC.ShowFlux);  //加入中值滤波 20200529
		    if(p->DisValue[0]<Slave_GetData()->GasMotor.Pid.Sv*0.8f)
			p->DisValue[0]=Flux_GetData()->ChannelC.ShowFlux;//烟尘流量
		 }
		
		*/		
		p->DisValue[12]=Sp->JWTem.Value;//计温
		if(p->LoadFlag==1)
		{
		  p->DisValue[13]=p->DisValue[12];
		  p->DisValue[16]=Flux_GetDQPress();   //大气压
		  p->LoadFlag=0;
		}
		Flux_LoadJWTem(1, p->DisValue[13]);
		Flux_LoadDQPress(1,p->DisValue[16]);
		
		
		
		//GetU16dataFormFlash2Word(&four,FLASHIS_AdrXTSZHJWD,pf); //环温
//		//p->DisValue[13]=four.FloatData;
//		if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]<0x0a)	
//	//	GetU16dataFormFlash2Word(&four,FLASHIS_AdrXTSZDQY,pf); //大气压
//		p->DisValue[16]=four.FloatData; //大气压

		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
				 Screen_ShowPage(98); 
                for(int i=0;i<17;i++)
				{
				 if((i>=2)&&(i<=11))
				 sprintf((char*)p->Buffer,"%.4f",p->DisValue[i]);
				 else 
				 {
				 if(i==15)
				 sprintf((char*)p->Buffer,"%.2f",p->DisValue[i]);  //计压 2位
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
      case 0x02: //动态态数据显示			
			 {
				 if(p->Timer==0)
							{	
                              	 p->Timer=200;
								
		                       if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]<0x0a)	 //烟尘
								{
								p->DisValue[14]=Sp->BCPress.Value;//流压
								p->DisValue[15]=Sp->BGPress.Value;//计压
								//p->DisValue[0]=Flux_GetData()->ChannelB.ShowFlux;//烟尘流量
								p->DisValue[0]=Flux_DataMiddleFilter(&Flux_GetData()->ChannelB.FluxTowData.Filter,Flux_GetData()->ChannelB.ShowFlux);  //加入中值滤波20200529
								if(p->DisValue[0]<Slave_GetData()->DustMotor.Pid.Sv*0.95f)
								p->DisValue[0]=Flux_GetData()->ChannelB.ShowFlux;//烟尘流量
								}
							   else if(pf->Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ]==0x0a) //烟气
								{
		                        p->DisValue[14]=Sp->CCPress.Value;//C路差压（流压），单位kPaSp->ACPress.Value;//流压
	                            p->DisValue[15]=0.02;//Sp->AGPress.Value;//计压
		                        //  p->DisValue[0]=Flux_GetData()->ChannelC.ShowFlux;//烟气流量
			                    p->DisValue[0]=Flux_DataMiddleFilter(&Flux_GetData()->ChannelC.FluxTowData.Filter,Flux_GetData()->ChannelC.ShowFlux);  //加入中值滤波 20200529
		                        if(p->DisValue[0]<Slave_GetData()->GasMotor.Pid.Sv*0.95f)
			                    p->DisValue[0]=Flux_GetData()->ChannelC.ShowFlux;//烟尘流量
		                        }
								p->CT++;
								if(p->CT>=5)
								{
                                 p->CT=0;
               	                for(int i=0;i<17;i++)
								{
									if(i==1||(i>=2&&i<=11)) continue;
									 if(i==15)
				                     sprintf((char*)p->Buffer,"%.2f",p->DisValue[i]);  //计压 2位
				                     else
				                     sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);

									Screen_RefreshDataMulti(0x0100+i*0x20,p->Buffer,15*2);
								 }
									if(p->Key.bOutsideKeyboard != false)    //外按键
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
							
						
						
                 //以下为检测电压 20200529, 如果调试时不接电池，屏蔽此项, 正式发布时需打开。
                  	if(p->PauseOrStop==B0)
					{
						 //暂停
					 if((Battery_GetData()->LostPV.ActualV<LowPOWER)&&(Power_GetData()->Flg&0x04))	
					 {	 
						 p->PauseOrStop=0;
						 p->LastChoice=0xff;
						 Dis_Tips(p,(u8 *)"电量过低",0x0520);	                              //20200529 
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
	函数名：void Screen_WHJZ_WHXZ_MMSZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-密码设置
*/
void Screen_WHJZ_WHXZ_MMSZ_Pag(Screen_DataStructure *p)
{

	  static u8 LastChoice=255;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	  Button_WHJZ_WHXZ_MMSZ_Pag(0x0D,1,p->DisValue);
	  if(p->RunMode!=0x8130) return;
		//选择基图更新
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
			p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
				case 0x01: Screen_PastePicture(0x0800,93,(54<<16)+48,(514<<16)+95,(54<<16)+48);break;  //原密码
				case 0x02: Screen_PastePicture(0x0800,93,(54<<16)+97,(514<<16)+144,(54<<16)+97);break;//新密码
				case 0x03: Screen_PastePicture(0x0800,93,(54<<16)+146,(514<<16)+193,(54<<16)+146);break;//确认新密码
				case 0x04: Screen_PastePicture(0x0800,93,(580<<16)+94,(772<<16)+141,(580<<16)+94);break;//确认密码修改
				case 0x05: Screen_PastePicture(0x0800,93,(54<<16)+193,(272<<16)+240,(54<<16)+193);break;  //打印蓝牙密码选择
				case 0x06: Screen_PastePicture(0x0800,93,(312<<16)+194,(511<<16)+241,(312<<16)+194);break;//输入打印蓝牙密码
				case 0x07: Screen_PastePicture(0x0800,93,(577<<16)+181,(767<<16)+225,(577<<16)+181);break;//打印蓝牙初始化
				case 0x08: Screen_PastePicture(0x0800,93,(54<<16)+242,(269<<16)+289,(54<<16)+242);break;  //APP蓝牙密码选择
				case 0x09: Screen_PastePicture(0x0800,93,(313<<16)+243,(510<<16)+290,(313<<16)+243);break;	//输入APP蓝牙密码	
				case 0x0A: Screen_PastePicture(0x0800,93,(581<<16)+233,(778<<16)+280,(581<<16)+233);break;  //8 app蓝牙初始化
				case 0x0B: Screen_PastePicture(0x0800,93,(54<<16)+291,(515<<16)+337,(54<<16)+291);break; //e28-2.4g无线地址                                                
				case 0x0C: Screen_PastePicture(0x0800,93,(54<<16)+339,(515<<16)+385,(54<<16)+339);break; //e28-2.4g无线信道
				case 0x0D: Screen_PastePicture(0x0800,93,(718<<16)+375,(787<<16)+440,(718<<16)+375);break;  //这个是返回
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

		//数据更新
	switch(p->RunModeStat)
	{
		case 0x01: //静态数据显示			
		{
			Screen_ShowPage(92); 
           	p->DisValue[3]=pf->Data[FLASHIS_AdrXTWHLYPDM]; //蓝牙配对码
		    p->DisValue[4]=pf->Data[FLASHIS_AdrXTWHAPPLYPDM]; //蓝牙APP

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
		case 0x02: //动态态数据显示			
		{
			if(p->Timer==0)
			{				
				if(p->Warning!=0)
				{
					if(p->Warning==1)
					{
						Dis_Tips(p,(u8*)"密码修改成功",0x01E0);		
					}
					else if(p->Warning==2)
					{
						Dis_Tips(p,(u8*)"密码修改失败---原密码或两次新密码输入不一致",0x01E0);
					}
					else if(p->Warning==3)
					{
						Dis_Tips(p,(u8*)"蓝牙功能未开启，请先开启！",0x01E0);
					}
					else if(p->Warning==4)
					{
						Dis_Tips(p,(u8*)"超时，初始化失败，请重新初始化！",0x01E0);
					}
					else if(p->Warning==5)
					{
						Dis_Tips(p,(u8*)"蓝牙初始化成功！",0x01E0);
					}
					else if(p->Warning==6)
					{
						Dis_Tips(p,(u8*)"正在初始化，请稍后.....",0x01E0);
					}
					
					else if(p->Warning==8)
					{
						Dis_Tips(p,(u8*)"超时,E28无线设置失败,请重新操作！",0x01E0);
					}
					else if(p->Warning==9)
					{
						Dis_Tips(p,(u8*)"E28无线设置成功！",0x01E0);
					}
					if(p->Warning != 6)
						p->Warning=0;	
					p->Timer=2000;									
				}else
				{
					Dis_Tips(p,(u8*)" ",0x01E0);	//清空状态
					if(p->Key.Mode!=0)
					{
						if(p->Key.bOutsideKeyboard != false)
							ScreenEnterNumberTwinkle(p,p->Key.Mode*0x20+0x00e0);  //Key.Mode=1~7
					}
					p->Timer=500;
				 }								
							 
			}
			if(p->uTempState == 1)  //启动监控
			{  //处理蓝牙初始化
				u8 res = ProcessBlueToothInitial((u32)p->DisValue[3]);  //处理线程
				if(res!=0)
				{
					if(res == 1)
						p->Warning = 4; //超时，初始化失败，请重新初始化。
					else if(res == 2)
						p->Warning = 5;	//蓝牙初始化成功。					
					p->uTempState = 0;
				}
			}
			
			if(p->uE28CtlState== 1)  //启动监控
			{   u8 Eres=ProcessE28Initial(Slave_GetData()->E28.ADRRset,Slave_GetData()->E28.CHset);
				if(Eres!=0)
				{
					if(Eres == 1)
						p->Warning = 8; //超时，设置失败，请重新设置。
					else if(Eres== 2)
						p->Warning = 9;	//E28初始化成功。		
					
					    p->uE28CtlState = 0;
				}
				
			}
			
			
		}
		break;
	}
}
//----
/* date 2020-05-18
	函数名：void Screen_WHJZ_WHXZ_NDJZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-浓度校准
*/
void Screen_WHJZ_WHXZ_NDJZ_Pag(Screen_DataStructure *p)
{

	  Flashisp_DataStructure* pf=Flashisp_GetData();
	   Slave_DataStructure* Sp=Slave_GetData();		
	 //按键处理
	  Button_WHJZ_WHXZ_NDJZ_Pag(0x0b,1,p->DisValue);
	
	  if(p->RunMode!=0x8140) return;
		//选择基图更新

	 if(p->PauseOrStop==B0) //启动状态，显示停止
		 { if(p->Key.Opt!=9)
		   Screen_PastePicture(0x40E0,101,(630<<16)+43,(750<<16)+106,(630<<16)+210);
		   
		 }else
		 { if(p->Key.Opt!=9) //停止状态，显示启动
		   Screen_PastePicture(0x40E0,100,(630<<16)+210,(750<<16)+273,(630<<16)+210);
		 }
	  
		switch(p->Key.Opt)
		{
		case 0x09: if(p->PauseOrStop==B0) //启动状态  ,显示停止  暗
						{  Screen_PastePicture(0x0800,101,(630<<16)+410,(750<<16)+475,(630<<16)+210);break;
															//Screen_PastePicture(0x0800,103,(105<<16)+375,(210<<16)+440,(337<<16)+375);break;
						}else //停止状态，显示启动
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
			 
		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
			Screen_ShowPage(100); 
            Screen_PastePicture(0x40E0,100,(220<<16)+375,(220<<16)+375,(337<<16)+375);				 
     		for(int i=0;i<7;i++)
				{
				    FourBytes four;
					
				//	if((pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>i)&0x01)  //判断有没有安装
					{
					GetU16dataFormFlash2Word(&four,FLASHIS_AdrWHJZ_NDJZ_LDO2+i*2,pf);
					if((i==0)||(i==6))
					sprintf((char*)p->Buffer,"%.1f",four.FloatData); //氧气与二氧化碳显示小数点
					else
					sprintf((char*)p->Buffer,"%.0f",four.FloatData);	//改成跟样机一样，不显示小数, modify by alfred, 2020.07.09
					
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
      case 0x02: //动态态数据显示			
			 {float databuf;
				 if(p->Timer==0)
							{	
									if(p->Key.bOutsideKeyboard != false) //外围按键输入数字时的闪动
											{								             							
												if(p->Key.Mode>=1&&p->Key.Mode<=7)
														{
														ScreenEnterNumberTwinkle(p,p->Key.Mode*0x20+0x01e0);  //闪
														}
												else  if(p->Key.Mode>=8&&p->Key.Mode<=14)		
														{
														ScreenEnterNumberTwinkle(p,(p->Key.Mode-7)*0x20+0x04e0);
														}
											}
							 //浓度数据装载
							 {
								 p->DisValue[0]= Sp->O2Concent.Value;
								 p->DisValue[1]= Sp->SO2Concent.Value;
								 p->DisValue[2]= Sp->NOConcent.Value;
								 p->DisValue[3]= Sp->NO2Concent.Value;
								 p->DisValue[4]= Sp->COConcent.Value;
								 p->DisValue[5]= Sp->H2SConcent.Value;
								 p->DisValue[6]= Sp->CO2Concent.Value;
								 
								  databuf=(float)3000/4096;   //求AD每一级电压单位mV/级
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
								if((pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>i)&0x01)  //判断有没有安装
					         {      if((i==0)||(i==6))
									sprintf((char*)p->Buffer,"%.1f",p->DisValue[i]);
								    else
									sprintf((char*)p->Buffer,"%.0f",p->DisValue[i]);  //不显示小数点
									
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
							sprintf((char*)p->Buffer,"%.1f",Flux_GetData()->ChannelC.ShowFlux); //流量
							Screen_RefreshDataMulti(0x04E0,p->Buffer,15*2);
						    p->Timer=500;
						}			 
			 }
			break;
			}
}
//----
/* date 2020-1-16
	函数名：void Screen_WHJZ_WHXZ_NDJZ_BLSZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-浓度校准-倍率设置
*/
  const char strLC[7][30]={ //量程
		"O2量程0%~30%",
		"SO2量程0-5700mg/m3",
		"NO量程0-1300mg/m3",
		"NO2量程0-200mg/m3",
		"CO量程0-5000mg/m3",
		"H2S量程0-300mg/m3",
		"CO2量程0-20%0mg/m3",
		};
		 const char strFDLC[7][3][30]={ //分段量程
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
	 //按键处理
	  Button_WHJZ_WHXZ_NDJZ_BLSZ_Pag(0x06,1,p->DisValue);
	  if(p->RunMode!=0x8141) return;
		//选择基图更新
	
	     //启动停止
	//  if(p->LastChoice!=p->PauseOrStop)	
		{
		//	p->LastChoice=p->PauseOrStop;
		if(p->PauseOrStop==B0) //启动状态，显示停止
		 { if(p->Key.Opt!=4)
		   Screen_PastePicture(0x40E0,103,(220<<16)+375,(330<<16)+440,(337<<16)+375);
		   
		 }else
		 { if(p->Key.Opt!=4) //停止状态，显示启动
		   Screen_PastePicture(0x40E0,102,(337<<16)+375,(452<<16)+440,(337<<16)+375);
		 }
	     }
		switch(p->Key.Opt)
		{
		case 0x04: if(p->PauseOrStop==B0) //启动状态  ,显示停止  暗
									 {  Screen_PastePicture(0x800,103,(102<<16)+375,(216<<16)+440,(337<<16)+375);break;
															//Screen_PastePicture(0x0800,103,(105<<16)+375,(210<<16)+440,(337<<16)+375);break;
									 }else //停止状态，显示启动
									 {  Screen_PastePicture(0x800,103,(337<<16)+375,(452<<16)+440,(337<<16)+375);break;
															//Screen_PastePicture(0x0800,103,(337<<16)+375,(452<<16)+440,(337<<16)+375);break;
									 }
									
		}
		//打钩位置
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
			 
		//数据更新
    switch(p->RunModeStat)
		{
		
		   case 0x01: //静态数据显示			
			 {
					Screen_ShowPage(102); 		
				  if(p->SubCnt==0)   //O2
					{   
					four.U16Data[0]=pf->Data[FLASHIS_AdrO2LQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrO2LQ+1];
					sprintf((char*)p->Buffer,"O2量程:0%%~%d%%",four.U32Data);	
					Screen_RefreshDataMulti(0x02A0,p->Buffer,15*2);	

					//显示各段倍率量程
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
					sprintf((char*)p->Buffer,"SO2量程:0~%d mg/m^3",four.U32Data);	
					Screen_RefreshDataMulti(0x02A0,p->Buffer,15*2);	

					//显示各段倍率量程
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
						sprintf((char*)p->Buffer,"NO量程:0~%d mg/m^3",four.U32Data);	
					Screen_RefreshDataMulti(0x02A0,p->Buffer,15*2);	

					//显示各段倍率量程
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
					sprintf((char*)p->Buffer,"NO2量程:0~%d mg/m^3",four.U32Data);	
					Screen_RefreshDataMulti(0x02A0,p->Buffer,15*2);	

					//显示各段倍率量程
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
						sprintf((char*)p->Buffer,"CO量程:0~%d mg/m^3",four.U32Data);	
					Screen_RefreshDataMulti(0x02A0,p->Buffer,15*2);	

					//显示各段倍率量程
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
						sprintf((char*)p->Buffer,"H2S量程:0~%d mg/m^3",four.U32Data);	
					Screen_RefreshDataMulti(0x02A0,p->Buffer,15*2);	

					//显示各段倍率量程
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
						sprintf((char*)p->Buffer,"CO2量程:0%%~%d%%",four.U32Data);	
					Screen_RefreshDataMulti(0x02A0,p->Buffer,15*2);	

					//显示各段倍率量程
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
					
					//各段气体浓度标定值
					four.U16Data[0]=pf->Data[FLASHIS_AdrWHJZ_NDJZ_BQLDO2_1+p->SubCnt*2];
					four.U16Data[1]=pf->Data[FLASHIS_AdrWHJZ_NDJZ_BQLDO2_1+p->SubCnt*2+1];
					p->DisValue[0]=four.FloatData;  
					if((p->SubCnt==0)||(p->SubCnt==6))
					sprintf((char*)p->Buffer,"%.1f",p->DisValue[0]); //测量值
                    else
					sprintf((char*)p->Buffer,"%.0f",p->DisValue[0]);
					
					Screen_RefreshDataMulti(0x01E0,p->Buffer,15*2);
					
					four.U16Data[0]=pf->Data[FLASHIS_AdrWHJZ_NDJZ_BQLDO2_2+p->SubCnt*2];
					four.U16Data[1]=pf->Data[FLASHIS_AdrWHJZ_NDJZ_BQLDO2_2+p->SubCnt*2+1];
					p->DisValue[1]=four.FloatData;
                    if((p->SubCnt==0)||(p->SubCnt==6))
					sprintf((char*)p->Buffer,"%.1f",p->DisValue[1]); //测量值
                    else
					sprintf((char*)p->Buffer,"%.0f",p->DisValue[1]);
					Screen_RefreshDataMulti(0x0200,p->Buffer,15*2);			


					four.U16Data[0]=pf->Data[FLASHIS_AdrWHJZ_NDJZ_BQLDO2_3+p->SubCnt*2];
					four.U16Data[1]=pf->Data[FLASHIS_AdrWHJZ_NDJZ_BQLDO2_3+p->SubCnt*2+1];
					p->DisValue[2]=four.FloatData;
					
					if((p->SubCnt==0)||(p->SubCnt==6))
					sprintf((char*)p->Buffer,"%.1f",p->DisValue[2]); //测量值
                    else
					sprintf((char*)p->Buffer,"%.0f",p->DisValue[2]);
					Screen_RefreshDataMulti(0x0220,p->Buffer,15*2);								 				 
				 
					//显示倍率
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
      case 0x02: //动态态数据显示			
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

								Slave_TPActualStructure * BeforGasp=&Sp->O2Concent;//倍率前
                                if(i==0)
								{
								p->DisValue[3]=BeforGasp[p->SubCnt].Value0;
								if((p->SubCnt==0)||(p->SubCnt==6))
								sprintf((char*)p->Buffer,"%.1f",BeforGasp[p->SubCnt].Value0); //测量值
                                else								
								sprintf((char*)p->Buffer,"%.0f",BeforGasp[p->SubCnt].Value0); //测量值
									
								}
								else if(i==1)
								{
								p->DisValue[4]=BeforGasp[p->SubCnt].Value1;
								if((p->SubCnt==0)||(p->SubCnt==6))
								sprintf((char*)p->Buffer,"%.1f",BeforGasp[p->SubCnt].Value1); //测量值
                                else
								sprintf((char*)p->Buffer,"%.0f",BeforGasp[p->SubCnt].Value1); //测量值
								}
								else if(i==2)
								{
								p->DisValue[5]=BeforGasp[p->SubCnt].Value2;
									
								if((p->SubCnt==0)||(p->SubCnt==6))
								sprintf((char*)p->Buffer,"%.1f",BeforGasp[p->SubCnt].Value2); //测量值
                                else
								sprintf((char*)p->Buffer,"%.0f",BeforGasp[p->SubCnt].Value2); //测量值
								}
								
								Screen_RefreshDataMulti(0x0240+i*0x20,p->Buffer,15*2);
																
							}	
							sprintf((char*)p->Buffer,"%.1f",Flux_GetData()->ChannelC.ShowFlux); //流量
							Screen_RefreshDataMulti(0x0160,p->Buffer,15*2);

						    p->Timer=500;
							if(p->staflg)
							{
								sprintf((char*)p->Buffer,"%dS",p->ZjTimer); //倒计时S
								Screen_RefreshDataMulti(0x02E0,p->Buffer,15*2);
							}else
							{
								if(p->PauseOrStop==B0)
								{
								   p->Function=0;
						       Flux_StopSampling('C');//停止气泵
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
	函数名：void Screen_WHJZ_WHXZ_YLJZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-压力校准
*/
void Screen_WHJZ_WHXZ_YLJZ_Pag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  	Slave_DataStructure* Sp=Slave_GetData();
	 //按键处理
	  Button_WHJZ_WHXZ_YLJZ_Pag(0x09,1,p->DisValue);
	  if(p->RunMode!=0x8150) return;
		//选择基图更新
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
			 
		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
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
      case 0x02: //动态态数据显示			
			 {
				 if(p->Timer==0)
							{				
             									
								if(p->Key.Mode>=1&&p->Key.Mode<=7)
								{
									if(p->Key.bOutsideKeyboard != false)
									ScreenEnterNumberTwinkle(p,p->Key.Mode*0x20+0x01e0);
								}
          	
								p->DisValue[0]=Sp->DQPress.Value; //大气压
								p->DisValue[1]=Sp->BGPress.Value; //计压(尘计压)
								p->DisValue[2]=Sp->AGPress.Value; //静压
								p->DisValue[3]=Sp->ACPress.Value; //动压
								p->DisValue[4]=Sp->BCPress.Value; //尘流压
								p->DisValue[5]=Sp->CCPress.Value; //气流压
						 
				
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
	函数名：void Screen_WHJZ_YQCGQXZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-烟气传感器选择
*/
void Screen_WHJZ_YQCGQXZ_Pag(Screen_DataStructure *p)
{
   
	  static u8 LastChoice=255;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	  Button_WHJZ_YQCGQXZ_Pag(0x08,1,p->DisValue);
	  if(p->RunMode!=0x8160) return;
		//选择基图更新
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
		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
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
	函数名：void Screen_WHJZ_WHXZ_WDJZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-温度校准
*/
void Screen_WHJZ_WHXZ_WDJZ_Pag(Screen_DataStructure *p)
{
	Flashisp_DataStructure* pf=Flashisp_GetData();
	Slave_DataStructure* Sp=Slave_GetData();  //20200525
	//按键处理
	Button_WHJZ_WDJZ_Pag(0x07,1,p->DisValue);
	if(p->RunMode!=0x8170) return;
	//选择基图更新
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
			 
		//数据更新
    switch(p->RunModeStat)
	{
		case 0x01: //静态数据显示			
		{
			Screen_ShowPage(96);
//			Dis_Tips(p,(u8*)"提示------",0x0480);	
			for(int i=0;i<4;i++)
			{
				sprintf((char*)p->Buffer,"%.2f",GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_WDJZ_LDYW+i*2));	////2020-05-20 因FLASH地址偏移了一个位,此处作修正
				Screen_RefreshDataMulti(0x0100+i*0x20,p->Buffer,15*2);

				sprintf((char*)p->Buffer,"%.2f",GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_WDJZ_BLYW+i*2)); //2020-05-20 因FLASH地址偏移了一个位,此处作修正
				Screen_RefreshDataMulti(0x0500+i*0x20,p->Buffer,15*2);
			}				 
			p->RunModeStat=2;				 
		}
		break;
		case 0x02: //动态态数据显示			
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
				p->DisValue[0]=Sp->GQTem.Value;//干球烟温测量值Sp->YWTem.Value;
				p->DisValue[1]=Sp->SQTem.Value;  //湿球测量值	
				p->DisValue[2]=Sp->JWTem.Value;  //计温测量值
				p->DisValue[3]=Sp->HWTem.Value;  //环温测量值	
				p->DisValue[4]=Sp->Data[SLAVE_MAP_YWTem].Raw;  //mV  原始值  20200525
				p->DisValue[5]=Sp->Data[SLAVE_MAP_SQTem].Raw;  //mV  原始值  20200525

				for(int i=0;i<4;i++)  //测量值
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
						
						Dis_Tips(p,(u8*)"烟温/干球:请接入100R电阻",0x0480);
					}
					else if(p->Warning == 0XA3||p->Warning == 0XA4)
					{
						
						Dis_Tips(p,(u8*)"烟温/干球:请接入174R电阻",0x0480);

					}
					else if(p->Warning == 0xAA)
					{
						Dis_Tips(p,(u8*)"烟温/干球:100欧电阻输入错误！",0x0480);
						p->Warning = 0xA1;				
						p->Timer = 2000;
					}
					else if(p->Warning == 0xAB)
					{
						Dis_Tips(p,(u8*)"烟温/干球:174欧电阻输入错误！",0x0480);
						p->Warning = 0xA3;
						p->Timer = 2000;
					}
					else if(p->Warning == 0xAF)
					{
						Dis_Tips(p,(u8*)"烟温/干球:温度标定完成！",0x0480);
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
						
						Dis_Tips(p,(u8*)"湿球:请接入100R电阻",0x0480);
					}
					else if(p->Warning == 0XA3||p->Warning == 0XA4)
					{
						
						Dis_Tips(p,(u8*)"湿球:请接入174R电阻",0x0480);

					}
					else if(p->Warning == 0xAA)
					{
						Dis_Tips(p,(u8*)"湿球:100欧电阻输入错误！",0x0480);
						p->Warning = 0xA1;				
						p->Timer = 2000;
					}
					else if(p->Warning == 0xAB)
					{
						Dis_Tips(p,(u8*)"湿球:174欧电阻输入错误！",0x0480);
						p->Warning = 0xA3;
						p->Timer = 2000;
					}
					else if(p->Warning == 0xAF)
					{
						Dis_Tips(p,(u8*)"湿球:温度标定完成！",0x0480);
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
	函数名：void Screen_WHJZ_WHXZ_DCBD_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-电池标定
*/
void Screen_WHJZ_WHXZ_DCBD_Pag(Screen_DataStructure *p)
{
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	  Battery_DataStructure* pb=Battery_GetData();
	 //按键处理
	  Button_WHJZ_DCBD_Pag(0x02,1,p->DisValue);
	  if(p->RunMode!=0x8180) return;
		//选择基图更新
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
			 
		//数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
					Screen_ShowPage(116); 		
					Dis_Tips(p,(u8*)"",0x0140);
     
					sprintf((char*)p->Buffer,"%.4f",GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_DCJZ_BL));
				    Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);
				
							 
				 p->RunModeStat=2;				 
			 }
			break;
      case 0x02: //动态态数据显示			
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
	函数名：void Screen_JZWH_CJWH_Pag(Screen_DataStructure *p)
	描述：	校准维护-超级维护-界面逻辑处理
*/
void Screen_JZWH_CJWH_Pag(Screen_DataStructure *p)
{
//	FourBytes four;
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
		  Button_WHJZ_CJWH_Pag(4,1,p->DisValue);
	  if(p->RunMode!=0x8200) return;
			//选择基图更新
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
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
					Screen_ShowPage(106); 
					Dis_Tips(p,(u8*)"",0x0120);    
					FourBytes four;
					four.U16Data[0]=pf->Data[FLASHIS_AdrCJWHYQBH];
					four.U16Data[1]=pf->Data[FLASHIS_AdrCJWHYQBH+1];
					sprintf((char*)p->Buffer,"%08d",four.U32Data);	//改为8位显示，by alfred, 2020.06.04
					Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);		 			 
					p->RunModeStat=2;	
				    p->Function=0;
			 }

			break;
			case 0x02: //动态态数据显示			
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
						Screen_RefreshDataMulti(0x0120,(u8 *)"正在备份",15*2);	
						p->Function=2;
					}
					else if(p->Function==2)
					{
					   Screen_RefreshDataMulti(0x0120,(u8 *)"备份完成",15*2);	
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
	函数名：void Screen_WHJZ_CJWH_QTCGQPZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-超级维护-烟气传感器选择
*/
void Screen_WHJZ_CJWH_QTCGQPZ_Pag(Screen_DataStructure *p)
{
   
	  Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	  Button_WHJZ_CJWH_QTCGQXZ_Pag(0x0F,1,p->DisValue);  //一级有15个0xf
	  if(p->RunMode!=0x8210) return;
		//选择基图更新
		if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)         //一层菜单焦点
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
		if(p->LastChoice!=pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ])	  //选择打钩
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
		//数据更新 TXT
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示	 ,数据有更新时可以置1来更新		
			 {
					Screen_ShowPage(108); 
					FourBytes		four;	
					four.U16Data[0]=pf->Data[FLASHIS_AdrO2LQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrO2LQ+1];
					sprintf((char*)p->Buffer,"0%%~%02d%%",four.U32Data);	//O2量程0~30
					Screen_RefreshDataMulti(0x0100,p->Buffer,15*2);	

					four.U16Data[0]=pf->Data[FLASHIS_AdrSO2LQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrSO2LQ+1];
					sprintf((char*)p->Buffer,"0~%05d",four.U32Data);	//
				  //	sprintf((char*)p->Buffer,"0-5700ppm");//SO2量程
					Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);

					four.U16Data[0]=pf->Data[FLASHIS_AdrNOLQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrNOLQ+1];
					sprintf((char*)p->Buffer,"0~%05d",four.U32Data);	//
					//sprintf((char*)p->Buffer,"0-1300ppm");//NO量程
					Screen_RefreshDataMulti(0x0140,p->Buffer,15*2);
				 
					four.U16Data[0]=pf->Data[FLASHIS_AdrNO2LQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrNO2LQ+1];
					sprintf((char*)p->Buffer,"0~%05d",four.U32Data);	//
					//sprintf((char*)p->Buffer,"0-200ppm");//NO2量程
					Screen_RefreshDataMulti(0x0160,p->Buffer,15*2);
					
					four.U16Data[0]=pf->Data[FLASHIS_AdrCOLQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrCOLQ+1];
					sprintf((char*)p->Buffer,"0~%05d",four.U32Data);	//
					//sprintf((char*)p->Buffer,"0-5000ppm");//cO量程
					Screen_RefreshDataMulti(0x0180,p->Buffer,15*2);
					
					four.U16Data[0]=pf->Data[FLASHIS_AdrH2SLQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrH2SLQ+1];
					sprintf((char*)p->Buffer,"0~%05d",four.U32Data);	//
					//sprintf((char*)p->Buffer,"0-75ppm");//H2S量程
					Screen_RefreshDataMulti(0x01A0,p->Buffer,15*2);
					
					four.U16Data[0]=pf->Data[FLASHIS_AdrCO2LQ];
					four.U16Data[1]=pf->Data[FLASHIS_AdrCO2LQ+1];
					sprintf((char*)p->Buffer,"0%%~%02d%%",four.U32Data);	//
					//sprintf((char*)p->Buffer,"0-200ppm");//CO2量程
					Screen_RefreshDataMulti(0x01C0,p->Buffer,15*2);			
				 
					p->LastChoice=pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ];
					p->LastChoice=p->LastChoice>>8;
                   for(int i=0;i<7;i++)
                   {
					 UpdataSensorFixDate(&four,FLASHIS_AdrO2AZSJ+i*2,pf,p,i);//   显示安装时间
					}				 	 
				 p->RunModeStat=2;				 
			 }
			break;
			 case 0x02: //动态态数据显示			
			 {
				 if(p->Timer==0)
							{										
								if((p->Key.Mode>=1)&&(p->Key.Mode<=7)&&(p->Key.bOutsideKeyboard != false))  //安装输入在外键盘下输入
								{                                                                           //闪烁对应的位字符
										ScreenEnterNumberTwinkle(p,p->Key.Mode*0x20+0x0400);
								}	
								
								if((p->Key.Mode>=8)&&(p->Key.Mode<=14)&&(p->Key.bOutsideKeyboard != false)) //量程输入在外键盘下输入
								{                                                                            //闪烁对应的位字符
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
	函数名：void Screen_KJ_MMSR_Pag(Screen_DataStructure *p)
	描述：	开机-输入密码-界面逻辑处理
*/
void Screen_KJ_MMSR_Pag(Screen_DataStructure *p)
{
	FourBytes four;
	static u8 Flag = 0;

	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	//按键处理
	Button_KJ_MMSR_Pag(2,1,p->DisValue);
	if(p->RunMode!=101) return;
	
	//选择基图更新
	if(p->Key.LastOpt!=p->Key.Opt)
	{ 
	  	p->Key.LastOpt=p->Key.Opt; 
		switch(p->Key.Opt)
		{
			case 0x01: Screen_PastePicture(0x0800,161,(236<<16)+196,(363<<16)+250,(236<<16)+196);break;
			default:break;
		}
	}
	//数据更新
	switch(p->RunModeStat)
	{
		case 0x01: //静态数据显示			
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
					//显示仪器编号
					four.U16Data[0]=pf->Data[FLASHIS_AdrCJWHYQBH];
					four.U16Data[1]=pf->Data[FLASHIS_AdrCJWHYQBH+1];
					sprintf((char*)p->Buffer,"仪器编号:%010d",four.U32Data);
					Screen_RefreshDataMulti(0x0500,p->Buffer,15*2);	
					//显示随机验证码
					sprintf((char*)p->Buffer, "验证码:%04d", GetRandomNum());
					Screen_RefreshDataMulti(0x0400,p->Buffer,15*2);	
				}
			}
		}
		break;
		
		case 0x02: //动态数据显示
		{			
			if(p->Timer==0)
			{	
				if(p->Warning==1)
				{
					Dis_Tips(p,(u8 *)"密码输入错误",0x0120);			
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


