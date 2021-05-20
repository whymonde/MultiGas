/*
	本文件为该项目的屏幕相关函数
	具体函数如下
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
	函数名：void Screen_Main(void)
	描述：	屏幕显示主函数
*/
void Screen_Main(void)
{

	Screen_SecClock();//读取屏幕时钟
	Screen_DealWihtReceiveData();//获取屏幕数据
	Screen_BacklightDW();//背光处理
	Screen_changPowerState();  //监控充电充满
	Screen_ShowScreenLockState();//按键锁处理
	Screen_WaterPumServo();  //水泵监控  20200528
	Screen_ShowWifi();       //E28图标
	Screen_ScanSchack();     //检测屏幕数据录入状态寄存器
	//---
	if(ScreenData.RunMode < 0x1000)//主界面
		Screen_MainPage(&ScreenData);
	else if(ScreenData.RunMode < 0x2000)//系统设置
	{
	  Screen_SystemSet(&ScreenData);
	}	
	else if(ScreenData.RunMode < 0x3000)//烟道布置
	{ 
	   Screen_FlueSet(&ScreenData);
	}	
	else if(ScreenData.RunMode < 0x4000)//工况预测
	{
	  Screen_WorkingConditionPrediction(&ScreenData);
	}	
	else if(ScreenData.RunMode < 0x5000)//测含湿量
	{
	   Screen_MeasureHumidity(&ScreenData);
	}	
	else if(ScreenData.RunMode < 0x6000)//测量烟气
	{
	   Screen_MeasureSomke(&ScreenData);
	}	
	else if(ScreenData.RunMode < 0x7000)//烟尘采集
	{
	   Screen_SmokeSampling(&ScreenData);
	}
	else if(ScreenData.RunMode < 0x8000)//文件查询
	{
	  Screen_FileSeek(&ScreenData);
	}
	else if(ScreenData.RunMode < 0x9000)//校准维护
	{
	  Screen_CalibrationFix(&ScreenData);
	}
	else if(ScreenData.RunMode < 0xA000)
	{
		Screen_Backlight(&ScreenData);//进入背光后处理
	}
	else if(ScreenData.RunMode < 0XB000)
	{
		Screen_PowerDowm(&ScreenData);//关机
	}
	
	 ScreenButtonReset(&ScreenData);
	//---
}
//---
/*
	函数名：void Screen_Timer(void)
	描述：screen.c文件定时器函数，由系统1ms定时器调用
*/
void Screen_Timer(void)
{   if(ScreenData.Scan_Stimer!=0)ScreenData.Scan_Stimer--;
	if(ScreenData.Timer != 0) ScreenData.Timer--;	
	if(ScreenData.ChangTimer != 0) ScreenData.ChangTimer--;	
	if((ScreenData.sReveiveDataTimer != 0)&&(ScreenData.uEnableRecDataTimer == 1)) ScreenData.sReveiveDataTimer--;	//接收屏幕数据超时处理定时器
	if(ScreenData.BatflshTimer>0)ScreenData.BatflshTimer--;
	if(ScreenData.Clock.Msec<500) ScreenData.Clock.Msec++;//读屏幕时钟
 
	
	if(ScreenData.JiPingJun.Timer != 0) ScreenData.JiPingJun.Timer--;	
//	if(ScreenData.uCounter1ms%1000==0) ScreenData.uCounter1S++;	//秒数加1；
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
	函数名：void Screen_SecTimer(void)
	描述：该函数为Screen.c文件的秒函数,由系统秒函数调用
    执行时间更新、图标更新等
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
	函数名：void Screen_SecTimer(void)
	描述：	该函数为Screen.c文件的秒函数，由系统秒函数调用
  执行时间更新、图标更新等
*/
void Screen_SecTimer(void)
{
	if(ScreenData.TimerSec>0) ScreenData.TimerSec--;
	 if(ScreenData.WaterPumTimer != 0) ScreenData.WaterPumTimer--; 
	
	if(ScreenData.staflg)                 //启动自加定时器标志
	{
		if(ScreenData.ZjTimer<10000)  ScreenData.ZjTimer++;
	}

	if(ScreenData.BacklightSecTimer < 30*60) ScreenData.BacklightSecTimer++;
    Screen_ShowDynamicData(); 
//YC_SampleControl();//20200604
}
//---
/*
	函数名：void Screen_Init(void)
	描述：	screen.c文件初始化函数
*/
void Screen_Init(void)
{
	Screen_InitData();
	Screen_ColckInit();
	Screen_WaterPumOpenServo();  //这函数要在读出EEPROM数据后
}
//---
/*
	函数名：Screen_DataStructure* Screen_GetData(void)
	描述：获取返回screen.c文件全局变量结构体
*/
Screen_DataStructure* Screen_GetData(void)
{
	return &ScreenData;
}
//---
/*20200529
	函数名：void Screen_MainPage(Screen_DataStructure *p)
	描述：	显示主界面相关,包含开机、开机界面、主界面
	RunMode范围：1~99
*/

void Screen_MainPage(Screen_DataStructure *p)
{ //u32 buf;
	switch(p->RunMode)
	{
	  
		case 1: 
		{//等待屏幕上电稳定
			p->Timer = 1000;
			p->RunMode = 2;	//2020-05-19 hjl 增加开机密码显示，RunMode = 100。 //2020-05-22调时时暂时改为51, by Alfred.
			p->RunModeStat=1;
			break;
		}
		case 2:
		{//显示开机
			if(p->Timer<=0)
			{
			Screen_WriteRegisterOneData(0x02,50);//设置蜂鸣器 50*10ms
			Screen_ShowPage(0);
			Screen_WriteRegisterOneData(0x01,0x40);//设置亮度 0x00 --0x 40
			p->Timer = 2000;
			p->RunMode = 3;
			}
			break;
		}case 3:
		{//显示动画
			if(p->Timer<=0)
			{			  
			Screen_ShowPage(1);			
			p->Timer = 3000;
			p->RunMode = 4;
			}
			break;
		}
		case 4:
		{//等按键响应
			if((p->Button == 0) || (p->Timer == 0)) 
			{p->RunMode = 5;
			}
			break;
		}
		case 5:
		{//检测程序升级文件 
		//	if(1)   //检测升级文件 有文件返回1
		//	{
		//	  p->RunMode = 6;
		//	  Screen_ShowPage(2);
		////	  p->Key.Opt=2;		
		//	}else  //否则跳转至文件检测
			{
			  p->RunMode = 11;
			}
			break;
		}
		case 6:
		{//程序升级界面处理
			
			//	Screen_programUpdata(p);
			
			break;
		}
		case 11:
		{//显示自检界面
			Screen_ShowPage(4);
			p->RunMode = 12;
			p->Key.Opt=1;
			break;
		}
		case 12:
		{//自检界面逻辑处理
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
			if((Flashisp_GetData()->Data[FLASHIS_AdrCJMOD]>=2)&&(Flashisp_GetData()->Data[FLASHIS_AdrCJMOD]<=4))  //判断是否采样过程中掉电
			{   p->RunMode =31;
				p->Key.Opt=1;   //预先选择退出选项
				p->Key.LastOpt=0;
				p->Key.Mode=0; //一级菜单
				p->RunModeStat=1; //静态显示
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
		{//进入主界面
			Screen_ShowPage(8);
			p->Key.OldOpt=0; //旧选项
			p->Key.Opt=1; 	//新选项
			p->RunMode = 52;
			
			Power_SetFlg(); //启动按键检测
			break;
		}
		case 52:
		{//静态数据显示
			Screen_MainLayer(p);
			//p->RunMode = 52;
			break;
		}

		case 100:
			//2020-05-19 hjl add
			//判断是否连接气体检测板
		{
			if(p->Timer <= 0)
			{
				if(Slave_GetData()->Stm8Data.Data == 0xFE)
				{
					p->RunMode = 101;
					Power_SetFlg(); //启动按键检测
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
			//开机密码输入界面
			
			Screen_KJ_MMSR_Pag(p);
			break;
        }
		}
}


//---
/*
	函数名：void Screen_SysSet(Screen_DataStructure *p)
	描述：	系统设置相关
	RunMode范围：0x1000-0x1fff
	修改记录：1. 删除系统设置页, modify by alfred, 2020.06.13
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
	函数名：void Screen_YanDaoBuDianSet(Screen_DataStructure *p)
	描述：	烟道布点设置界面
	RunMode范围：0x2000-0x2fff
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
			Screen_FlueCircularPag(p); //圆形
			break;
		} 
		case 0x2200:
		{
			Screen_FlueSquarePag(p);  //方形
			break;
		} 
		case 0x2300:
		{
			Screen_FlueOtherPag(p);   //其他
			break;
		}
	    case 0x2400:
		{
			Screen_FlueSeekPag(p);   //烟道查询
			break;
		} 		
		
	}
}
//---
/*
	函数名：void Screen_WorkingConditionPrediction(Screen_DataStructure *p)
	描述：	工况预测设置界面 
	RunMode范围：0x3000-0x3fff
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
			Screen_WCP_PressureZeroPag(p); //压力调零
			break;
		} 
		case 0x3200:
		{
			Screen_WCP_FumeTemperaturPag(p);  //烟温测量
			break;
		} 
		case 0x3300:
		{
			Screen_WCP_PredictedVelocityPag(p);   //预测流速
			break;
		}
		case 0x3400:
		{
			Screen_WCP_CheckAveragePag(p);   //查平均值
			break;
		}		
	}
}
//---

/*
	函数名：void Screen_WorkingConditionPrediction(Screen_DataStructure *p)
	描述：	测含湿量设置界面 
	RunMode范围：0x3000-0x3fff
*/
void Screen_MeasureHumidity(Screen_DataStructure *p)
{
  	switch(p->RunMode)
	{
		case 0x4000:
		{
			Screen_CHSL_SetPag(p); //选择
			break;
		} 
		case 0x4100:
		{
			Screen_CHSL_GSQF_Pag(p); //干湿球发
			break;
		} 
		case 0x4200:
		{
			Screen_CHSL_ZRF_Pag(p);  //阻容法测量
			break;
		} 
//			case 0x4400:	//
//		{
//			Screen_CHSL_HSLCX_Pag(p);  //含湿量查询
//			break;
//		}
		
	
	}
}
//---
/*
	函数名：void Screen_WorkingConditionPrediction(Screen_DataStructure *p)
	描述：	烟气测量设置界面 
	RunMode范围：0x5000-0x5fff
*/
void Screen_MeasureSomke(Screen_DataStructure *p)
{
	
	if(p->JiPingJun.cIsCalcAverage)	//如果正在计平均，直接跳到计平均页面.
		p->RunMode = 0x5230;
	
  	switch(p->RunMode)
	{
		case 0x5000:
		{
			 Screen_CLYQ_SetPag(p);//选择
			break;
		} 
		case 0x5100:
		{
		 Screen_CLYQ_YQJZ_Pag(p);//烟气校准
			break;
		} 
	
		case 0x5200:
		{
		 Screen_CLYQ_KSJC_Pag(p); //开始测量
			break;
		}		
		case 0x5210:
		{
		 Screen_CLYQ_CLJS_Pag(p);//烟气测量结束
			break;
		}
			case 0x5220:
		{
		 Screen_CLYQ_YQAX_Pag(p);//烟枪清洗
			break;
		}
			case 0x5230:
		{
			Screen_CLYQ_JIPINGJUN_Pag(p);//计平均
			break;
		}		
	 
			case 0x5300:
		{
			  Screen_CLYQ_YQCX_Pag(p);//烟气查询
			break;
		}
		case 0x5310:
		{
			  Screen_CLYQ_YQCX_JS_Pag(p);//烟气查询-计算
			break;
		} 		
	  	case 0x5311:
		{
			  Screen_CLYQ_YQCX_JS_YQSR_Pag(p);//烟气查询-计算-烟气输入
			break;
		}
			case 0x5320:
		{
			  Screen_CLYQ_YQCX_WJGL_Pag(p);//烟气查询-文件管理
			break;
		} 
		case 0x5330:
			{
			  Screen_CLYQ_YQCX_DYXX_Pag(p);//烟气查询-打印选项
			break;
		} 
		case 0x5340:
		{
			Screen_CLYQ_YQCX_FZSJ_Pag(p);//烟气查询-分钟数据
			break;
		} 
		
	}
}
//---
/*
	函数名：void Screen_SmokeSampling(Screen_DataStructure *p)
	描述：	烟尘采样设置界面 
	RunMode范围：0x6000-0x6fff
*/
void Screen_SmokeSampling(Screen_DataStructure *p)
{
  	switch(p->RunMode)
	{
		case 0x6000:
		{
			Screen_YCCY_CYSZ_Pag(p); //设置
			break;
		} 
		case 0x6100:
		{
			Screen_YCCY_QMJC_Pag(p);//气密性检测
			break;
		} 
		case 0x6200:
		{
			Screen_YCCY_KSJC_Pag(p);//开始采样

			break;
		}
	  case 0x6210:
		{
			Screen_YCCY_CLJS_Pag(p);//结束采样
			break;
		}
   case 0x6220:
		{
			Screen_YCCY_YCFDX_Pag(p);//烟尘防倒吸
			break;
		}  		

	}
}
//---
/*
	函数名：void Screen_FileSeek(Screen_DataStructure *p)
	描述：	文件查询设置界面 
	RunMode范围：0x7000-0x7fff
*/
void Screen_FileSeek(Screen_DataStructure *p)
{
  	switch(p->RunMode)
	{
		case 0x7000:
		{
			Screen_YCCY_YCCX_Pag(p);//文件选择
			break;
		} 
		case 0x7100:
		{
		    Screen_YCCY_YCCX_JS_YC_Pag(p);//烟尘计算
			break;
		} 
		case 0x7200:
		{
		    Screen_YCCY_YCCX_WJGL_Pag(p);//文件管理
			break;
		} 
			case 0x7300:
		{
			Screen_YCCY_YCCX_DYXX_Pag(p);//打印选项
			break;
		}
		case 0x7400:
		{
			Screen_YCCY_YCCX_JS_YY_Pag(p);//油烟计算
			break;
		} 		
	
	}
}
//---
/*
	函数名：void Screen_FileSeek(Screen_DataStructure *p)
	描述：	校准维护设置界面 
	RunMode范围：0x8000-0x8fff
*/
void Screen_CalibrationFix(Screen_DataStructure *p)
{
  	switch(p->RunMode)
	{
		case 0x8000:
		{
			Screen_JZWH_MMSR_Pag(p);//密码输入选择
			break;
		} 
		case 0x8100:
		{
		 Screen_JZWH_WHXZ_Pag(p);//矫正维护选择
			break;
		} 
		case 0x8110:
		{
		  Screen_WHJZ_WHXZ_STSZ_Pag(p);//系统设置
			break;
		} 
			case 0x8111:
		{
		  Screen_WHJZ_WHXZ_STSZ_CO_SO2_Pag(p);//CO 对SO2校准
			break;
		} 
		
			case 0x8120:
		{
			  Screen_WHJZ_WHXZ_LLJZ_Pag(p);//流量校准
			break;
		} 
			case 0x8130:
		{
			  Screen_WHJZ_WHXZ_MMSZ_Pag(p);//密码管理
			break;
		} 
	  	case 0x8140:
		{
			  Screen_WHJZ_WHXZ_NDJZ_Pag(p);//浓度校准
			break;
		}
		  	case 0x8141:
		{
			  Screen_WHJZ_WHXZ_NDJZ_BLSZ_Pag(p);//浓度倍率设置校准
			break;
		}

			case 0x8150:
		{
			  Screen_WHJZ_WHXZ_YLJZ_Pag(p);//压力校准
			break;
		}
	case 0x8160:
		{
			  Screen_WHJZ_YQCGQXZ_Pag(p);//传感器选择
			break;
		} 
	case 0x8170:
		{
			  Screen_WHJZ_WHXZ_WDJZ_Pag(p);//温度校准
			break;
		} 
  	case 0x8180:
		{
			  Screen_WHJZ_WHXZ_DCBD_Pag(p);//电池校准
			break;
		}
		 	case 0x8200:
		{
			  Screen_JZWH_CJWH_Pag(p);//超级维护
			break;
		}
		case 0x8210:
		{
			  Screen_WHJZ_CJWH_QTCGQPZ_Pag(p);//超级维护-气体传感器选择
			break;
		}
		
 		
	}
}
//---
/*
	函数名：void Screen_Backlight(Screen_DataStructure *p)
	描述：	该函数为调按屏幕背光相关处理
*/
void Screen_Backlight(Screen_DataStructure *p)
{
	switch(p->RunMode)
	{
		case 0X9001:
		{//数字键盘界面显示
		//	Screen_ShowPage(205);
			p->RunMode = 0X9002;
			break;
		}
		case 0X9002:
		{//静态数据显示
			p->RunMode = 0X9003;
			break;
		}
		case 0X9003:
		{//动态数据显示
			p->RunMode = 0X9090;
			break;
		}
		case 0X9090:
		{//按键检测控制
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
	函数名：Screen_PowerDowm(Screen_DataStructure *p)
	描述：	关机
*/
void Screen_PowerDowm(Screen_DataStructure *p)
    {
	 switch(p->RunMode)
	   {
		case 0XA100:
			//显示黑屏页
		    Screen_ShowPage(162);
		    //关硬件
		    CTRbat_OUT(1);  //关电源
			CTRexPower_OUT(1);
		    //关副板
		    Flux_StopSampling('B');
		    Flux_StopSampling('C');
		    Slave_CloseWaterPump();//关闭抽水泵
		
		    //关风扇
		    Fan_Close();
		    p->RunMode=0XA101;
			break;
		case 0XA101:
			
			break;
	   }
    }

/*
	函数名：void Screen_BacklightDW(void)
	描述：	背光处理函数
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
	函数名：void Screen_ShowDynamicData(void)
  界面底部时间、大气压检测处理、图标处理
*/
void Screen_ShowDynamicData(void)
{   FourBytes four;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	Screen_DataStructure* p = &ScreenData;
	struct tm *pt = &ScreenData.Clock.Clock;
	char * buf[50]={0};
	Screen_ResetDataBuffer();
	sprintf((char*)p->Buffer,"%02d:%02d:%02d",pt->tm_hour,pt->tm_min,pt->tm_sec);//时间
	Screen_RefreshDataMulti(0x0320,p->Buffer,15*2);
	Screen_ResetDataBuffer();
	
	if(Power_GetData()->starFlg ==1)
	{
		sprintf((char*)p->Buffer,"%02d秒后关闭电源",(3-(u8)Power_GetData()->Tcnt/10));
		
		if((3-(u8)Power_GetData()->Tcnt/10)<=0)
		{
			p->RunMode=0XA100;
		}
	}
	else
	{
	if(Flux_GetData()->ChannelB.RunMode==2)
	{
		sprintf((char*)&buf,"烟尘采样中");
	}
	else if(Flux_GetData()->ChannelB.RunMode==3)
	{
		sprintf((char*)&buf,"烟尘暂停  ");
	}	
	else if(Flux_GetData()->ChannelB.RunMode==4)
	{
		sprintf((char*)&buf,"烟尘防倒吸");
	}
	else if(Flux_GetData()->ChannelB.Sampling.FdxFlg)
	{
		sprintf((char*)&buf,"烟尘未保存");
	}
	else
	{
		
					if((pf->Data[FLASHIS_AdrXTSZBOX]&(1>>0))==0x0000)//大气压测量值
						{//大气压
						
					    if(Slave_GetData()->DQPress.Value == FLUX_ColFAULT) 
						sprintf((char*)&buf,"故障");
					    else 
						sprintf((char*)&buf,"%0.1fkPa",Slave_GetData()->DQPress.Value);
				        }
				        else
				        {//大气压输入值
					     four.FloatData=GetFlaotdataFormFlash2Word(FLASHIS_AdrXTSZDQY);
					     sprintf((char*)&buf,"%0.1fkPa",four.FloatData);
				        }
		
		
		
	}
	strcpy((char*)p->Buffer,(char*)buf); 
	if(Flux_GetData()->ChannelC.RunMode==2)
	{
		sprintf((char*)&buf," 烟气采样中");
	}
	else if(Flux_GetData()->ChannelC.RunMode==3)
	{
		sprintf((char*)&buf," 烟气暂停  ");
	}
	else if(Flux_GetData()->ChannelC.RunMode==4)
	{
		sprintf((char*)&buf," 烟气清洗  ");
	}
	else if(Flux_GetData()->ChannelC.Sampling.EndFlg)
	{
		sprintf((char*)&buf," 烟气未保存");
	}
	else
	{
		sprintf((char*)&buf,"           ");
	}
	
	strcat((char*)p->Buffer,(char*)buf);
    }
	
	Screen_RefreshDataMulti(0x0340,p->Buffer,15*2);
	
	//电源检测部分
	Screen_ShowPowerState();
/*	if(p->Warning==1)//警告
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
	if(AT_GetData()->Mode==0)//蓝牙透传模式
	{
	  Screen_RefreshVarData(0x0060,5);
	}else
	{
	   Screen_RefreshVarData(0x0060,0);
	}
	
}

//---
/*
	函数名：void Screen_changPowerState(void)
	描述：	监控充电充满
*/
void Screen_changPowerState(void)
{
	u16 buf;
	
	if(LTC2944_GetData()->flag==1)  //上电时去判断电池容量
	{  LTC2944_GetData()->flag=2;
	   if((LTC2944_GetData()->PTC<1)&&(Battery_GetData()->Battery.ActualV>=BATTERY_Vcmp))  //判断为拔掉电池然后重插上，恢复电量。
       {
		 buf=LTC2944_Cal_ACC(Battery_GetData()->Battery.ActualV);  
		 LTC2944_WriteTwoByte(AccumulatCharge_adrr,buf);
	   }		   
	}
	if(ScreenData.ChangTimer == 0)
	{ ScreenData.ChangTimer=2000;    //两秒监控一次
	if(TestPower_GetData()->Changflg)//有充电器时
	{

		if((LTC2944_GetData()->I<0.1f)&&(LTC2944_GetData()->I>=0.0f)&&(Battery_GetData()->Battery.ActualV>=BATTERY_VMAX-0.7f))  //用电流,电压去改写满电量
		{
			LTC2944_WriteTwoByte(AccumulatCharge_adrr,LTC2944_ACCMax);
		}

	}
 }
}
//---
/*
	函数名：void Screen_SecClock(void)
	描述：	该函数为屏幕的秒钟计时器，用作系统的精确秒时钟源
	说明：	该函数为用于计算分钟使用
*/
void Screen_SecClock(void)
{
	if(ScreenData.Clock.Msec >= 500)
	{
		ScreenData.Clock.Msec = 0;
		Screen_ReadRegister(0x20,0x07);//读屏幕时钟
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
	函数名：void Screen_ShowNetworkState(void)
	描述：	显示网络图标
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
	函数名：void Screen_ShowNetworkState(void)
	描述：	显示屏幕锁图标
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
	   if(ScreenData.ScreenButtonLock==2) //屏幕按键失效
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
	函数名：void Screen_ShowPowerState(void)
	描述：	显示电池及电源图标
*/
void Screen_ShowPowerState(void)
{ char bufdata[10];
	TwoBytes buf;
	if(Power_GetData()->Flg&0x02)//if(TestPower_GetData()->flag)//显示外部电源图标  PowerData.Flg
	{
		Screen_RefreshVarData(0x0080,6);
	}
	else
	{
	 Screen_RefreshVarData(0x0080,0);
	}
	//电源电量显示
	
	
	if(TestPower_GetData()->Changflg==1)   //有充电器
	{
	 if(ScreenData.BatflshTimer==0)
		{  ScreenData.BatflshTimer=250;
			 ScreenData.Batflshcnt++;
			 if(ScreenData.Batflshcnt>4)
				ScreenData.Batflshcnt=1;
			 
			  buf.U8Data[0]=0;
	         buf.U8Data[1]=ScreenData.Batflshcnt+7;   //图标
			 if(LTC2944_GetData()->PTC>=100)//图标
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
	//显示电池百分比
	if(LTC2944_GetData()->PTC<100)
	sprintf((char *)&bufdata[0]," %02u%%",(u8)LTC2944_GetData()->PTC);//故障信息
	else
	sprintf((char *)&bufdata[0],"%u%%",(u8)LTC2944_GetData()->PTC);//故障信息
	Screen_RefreshDataMulti(0x0360,(u8 *)&bufdata[0],4*2);
	
}

//---
/*
	函数名：Screen_ShowWifi(void)
	描述：	显示WiFi图标
*/
void Screen_ShowWifi(void)
{
   if(Slave_GetData()->E28.STATERD24G&0x01) //E28连上
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
	函数名：Screen_ShowWifi(void)
	描述：	检测数据录入的状态
*/
void Screen_ScanSchack(void)   //检测屏幕录入的状态
{
	if(ScreenData.Scan_status&0x02)   //允许定时检测
	{
		if(ScreenData.Scan_Stimer==0)
		{
		   ScreenData.Scan_Stimer=1000; //1秒检测一次
			//发命令
           Screen_ReadRegister(0xE9,0x01);  //发命令读取0xe9命令
			
		}
		//检测标准位
		if(ScreenData.Scan_Stimer<900)  //100毫秒缓冲才检测有没有
		{
		if(ScreenData.Scan_status&0x01)  //在数字录入状态
		{
			
		}
		else                            //退出了要处理
		{
			ScreenData.Scan_status=ScreenData.Scan_status&0xFD;
			ScreenData.Key.Mode=0;
		}
		
	    }
		
	}
}

//---
//---
//-------------------------初始化相关----------------------------
//---
/*
	函数名：void Screen_InitData(void)
	描述：	screen.c文件初始化函数
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
	ScreenData.Warning=0;		//2020-05-19 hjl 修改初始值为0
	ScreenData.uEnableRecDataTimer = 0; //默认不启动接收数据超时定时器。//2020-05-23 by Alfred.
	ScreenData.JiPingJun.ucCurrentPage = 0xff; //初始化计平均当前页的值。
	ScreenData.gSysConfigWaringBits.m_byData = 0x00;	//初始化标志位。
	ScreenData.QimiStat=0;                      //进入机器未检测气密性
}
//---
/*
	函数名：void Screen_ColckInit(void)
	描述：	该函数为初始化时钟相关
*/
void Screen_ColckInit(void)
{
	ScreenData.Clock.Flag = 0;
	ScreenData.Clock.ClockSec = 0;
	ScreenData.Clock.Msec = 0;
//	Screen_ReadRegister(0x20,0x07);//读屏幕时钟
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
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Screen_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Screen文件的ASCII调试文件
	格式：		:000|02|002
*/
void Screen_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
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
		{//读取RunMode
			printf("p->RunMode=%u\r\n",ScreenData.RunMode);
			printf("p->RunModeStat=%u\r\n",ScreenData.RunModeStat);
			printf("p->Page=%u\r\n", ScreenData.Page);
			break;
		}
		case 102:
		{//读取RunMode
			u16 bf = Calculate_CharToU16(p+4,5);//计算命令
			ScreenData.Button = bf;
			printf("接收到按键=0x%X\r\n",ScreenData.Button);
			break;
		}
		case 103:
		{//设置年
			struct tm pt = ScreenData.Clock.Clock;
			pt.tm_year = Calculate_CharToU16(p+4,2) + SCREEN_YearScreen;//计算命令;
			if(pt.tm_year>138) pt.tm_year = 138;
			printf("接收到年=%u\r\n",pt.tm_year);
			Screen_SetClock(&pt);
			break;
		}
		case 104:
		{//设置月
			struct tm pt = ScreenData.Clock.Clock;
			pt.tm_mon = Calculate_CharToU16(p+4,2) - 1;//计算命令;
			if(pt.tm_mon>11) pt.tm_mon = 11;
			printf("接收到月=%u\r\n",pt.tm_mon);
			Screen_SetClock(&pt);
			break;
		}
		case 105:
		{//设置日
			struct tm pt = ScreenData.Clock.Clock;
			pt.tm_mday = Calculate_CharToU16(p+4,2);//计算命令;
			if(pt.tm_mday>31) pt.tm_mday = 31;
			printf("接收到日=%u\r\n",pt.tm_mday);
			Screen_SetClock(&pt);
			break;
		}
		case 106:
		{//设置时
			struct tm pt = ScreenData.Clock.Clock;
			pt.tm_hour = Calculate_CharToU16(p+4,2);//计算命令;
			if(pt.tm_hour>23) pt.tm_hour = 23;
			printf("接收到时=%u\r\n",pt.tm_hour);
			Screen_SetClock(&pt);
			break;
		}
		case 107:
		{//设置分
			struct tm pt = ScreenData.Clock.Clock;
			pt.tm_min = Calculate_CharToU16(p+4,2);//计算命令;
			if(pt.tm_min>59) pt.tm_min = 59;
			printf("接收到分=%u\r\n",pt.tm_min);
			Screen_SetClock(&pt);
			break;
		}
		case 108:
		{//设置分
			struct tm pt = ScreenData.Clock.Clock;
			pt.tm_sec = Calculate_CharToU16(p+4,2);//计算命令;
			if(pt.tm_sec>59) pt.tm_sec = 59;
			printf("接收到秒=%u\r\n",pt.tm_sec);
			Screen_SetClock(&pt);
			break;
		}
		case 601:
		{//设置ScreenData.Opt
			ScreenData.Key.Opt = Calculate_CharToU16(p+4,3);
			printf("设置的新Opt值为：%u\r\n",ScreenData.Key.Opt);
			break;
		}
		case 701:
		{//读取当前时钟及掉电时钟
			FourBytes four;
			struct tm tf;
			four.U32Data = mktime(&Screen_GetData()->Clock.Clock);//当前时间
			printf("当前信息：%u\r\n",four.U32Data);
			tf = ScreenData.Clock.Clock;
			printf("当前时间时间：%u年%02u月%02u日 %02u时%02u分%02u秒\r\n",tf.tm_year+SCREEN_YearBase,tf.tm_mon,tf.tm_mday,tf.tm_hour,tf.tm_min,tf.tm_sec);
			four.U16Data[0] = Flashisp_GetData()->Data[FLASHIS_AdrDDTime];
			four.U16Data[1] = Flashisp_GetData()->Data[FLASHIS_AdrDDTime+1];
			printf("掉电信息：%u\r\n",four.U32Data);
			tf = *localtime(&four.U32Data);//掉电时间
			printf("掉电时间：%u年%02u月%02u日 %02u时%02u分%02u秒\r\n",tf.tm_year+SCREEN_YearBase,tf.tm_mon,tf.tm_mday,tf.tm_hour,tf.tm_min,tf.tm_sec);
			
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
	函数名：void Screen_SavePowerDownMsg(void)
	描述：	该函数为保存掉电信息到SD卡中
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
//	Flashisp_GetData()->Data[FLASHIS_AdrDDYear]//年
//	Flashisp_GetData()->Data[FLASHIS_AdrDDMonth]//月
//	Flashisp_GetData()->Data[FLASHIS_AdrDDDate]//日
//	Flashisp_GetData()->Data[FLASHIS_AdrDDHour]//时
//	Flashisp_GetData()->Data[FLASHIS_AdrDDMinute]//分
//	Flashisp_GetData()->Data[FLASHIS_AdrDDSec]//秒
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
	//	printf("保存掉电记录");
		//以下求时间差
		four.U16Data[0] = Flashisp_GetData()->Data[FLASHIS_AdrDDTime];
		four.U16Data[1] = Flashisp_GetData()->Data[FLASHIS_AdrDDTime+1];
		tf = *localtime(&four.U32Data);//掉电时间数据tm结构
		A_LostPowerTime = mktime(&ScreenData.Clock.Clock); //当前时钟数据
		A_LostPowerTime = (u32)difftime(A_LostPowerTime,four.U32Data);
		hh = A_LostPowerTime/3600;
		mm = (A_LostPowerTime%3600)/60;
		ss = A_LostPowerTime%60;

//		Serch_MaxPowerFile(); //找出最大记录
		nomber=SdFile_GetData()->PowerFileMax+1; //得到最新的行号
		
		SPutl(0x4,&temp[0],nomber);
		temp[4]=0;
		strcpy((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],",");

		SPutl(0x2,&temp[0],tf.tm_year+SCREEN_YearBase); //掉电年
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],"-");
		
		SPutl(0x2,&temp[0],tf.tm_mon+1); //掉电月
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],"-");
		
		SPutl(0x2,&temp[0],tf.tm_mday); //掉电日
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);

		strcat((char *)&dbuf[0]," ");
		
		SPutl(0x2,&temp[0],tf.tm_hour); //掉电时
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],":");
		
		SPutl(0x2,&temp[0],tf.tm_min); //掉电分
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],":");
		
		SPutl(0x2,&temp[0],tf.tm_sec); //掉电秒
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		
		strcat((char *)&dbuf[0],",");
		
		SPutl(0x2,&temp[0],Screen_GetData()->Clock.Clock.tm_year); //来电年
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],"-");
		
		SPutl(0x2,&temp[0],Screen_GetData()->Clock.Clock.tm_mon+1); //来电月
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],"-");
		
		SPutl(0x2,&temp[0],Screen_GetData()->Clock.Clock.tm_mday); //来电日
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);

		strcat((char *)&dbuf[0]," ");
		
		SPutl(0x2,&temp[0],Screen_GetData()->Clock.Clock.tm_hour); //来电时
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],":");
		
		SPutl(0x2,&temp[0],Screen_GetData()->Clock.Clock.tm_min); //来电分
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],":");
		
		SPutl(0x2,&temp[0],Screen_GetData()->Clock.Clock.tm_sec); //来电秒
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);

		strcat((char *)&dbuf[0],",");
		
		SPutl(0x2,&temp[0],hh); //来电时
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],"h");
		
		SPutl(0x2,&temp[0],mm); //来电分
		temp[2]=0;
		strcat((char *)&dbuf[0],(char *)&temp[0]);
		strcat((char *)&dbuf[0],"min");
		
		SPutl(0x2,&temp[0],ss); //来电秒
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
函数名：void Screen_DealWihtReceiveData(void)
描述：处理接收到来自屏幕的数据
说明：已经去掉帧头
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
		if(AnalysisCRC(p))  //CRC校正
		{
		
				switch(p[2])
					{
						case 0x00:
						{//返回版本号04 81 00 01 71
	//						ScreenData.Button = (*(p+2)<<8) + *(p+4);
							break;
						}
						case 0x20:
						{//返回时钟值(5A A5 0A 81 20 07 19 01 23 03 16 38 57)YY:MM:DD:WW:HH:MM:SS
							ScreenData.Clock.Clock.tm_year = (p[4] % 0x10 + (p[4] / 0x10) * 10) + SCREEN_YearScreen;//年
							ScreenData.Clock.Clock.tm_mon  = (p[5] % 0x10 + (p[5] / 0x10) * 10); //- 1;								//月
							ScreenData.Clock.Clock.tm_mday = p[6] % 0x10 + (p[6] / 0x10) * 10;											//日
							ScreenData.Clock.Clock.tm_wday = (p[7] % 0x10 + (p[7] / 0x10) * 10) ;//- 1;								//星期
							ScreenData.Clock.Clock.tm_hour = p[8] % 0x10 + (p[8] / 0x10) * 10;											//时
							ScreenData.Clock.Clock.tm_min  = p[9] % 0x10 + (p[9] / 0x10) * 10;											//分
							ScreenData.Clock.Clock.tm_sec  = p[10] % 0x10 + (p[10] / 0x10) * 10;										//秒
							ScreenData.Clock.Flag |= B1;//标志时间已更新
							if(ScreenData.Clock.Flag & B2)
							{//实时保存时钟
								FourBytes four;
								four.U32Data = mktime(&ScreenData.Clock.Clock);
								Flashisp_GetData()->Data[FLASHIS_AdrDDTime] = four.U16Data[0];
								Flashisp_GetData()->Data[FLASHIS_AdrDDTime+1] = four.U16Data[1];
								Flashisp_GetData()->Flag = 1;
							}
							break;
						}
						case 0XE9:  //返回数据录入状态
						{//返回版本号04 81 E9 01 71
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
		if(AnalysisCRC(&Get_USART3Buffer()->ProgressBuffer2[0]))  //CRC校正
		{
			u8 *p=&Get_USART3Buffer()->ProgressBuffer2[0];
			u8 ucAdressDataLength = *p; //
			{//读变量存储器指令指令
				u16 address = (*(p+2)<<8) + *(p+3);//变量地址
				Flashisp_DataStructure *pf = Flashisp_GetData();//Flash指针
				Screen_DataStructure *ps = Screen_GetData();
				switch(address)
				{
					case SCREEN_KEYADD:
					{//接收到按键指令
						ScreenData.BacklightSecTimer = 0;
						ScreenData.Button = (*(p+5)<<8) + *(p+6);
						printf("接收到按键=0x%X\r\n",ScreenData.Button);
						break;
					}

					case SCREEN_KEYADD_ESC:
					{//接收到按键指令
						ScreenData.sValScreenKeyEsc = (*(p+5)<<8) + *(p+6);
						printf("接收到按键=0x%X\r\n",ScreenData.sValScreenKeyEsc);
						break;
					}

					case SCREEN_SAMPLE_ADRESS:
					{
					//							AdressData tempAdressdData;
						for(u8 i=0; i<15;i++)  //清空15个字
						{
							pf->Data[FLASHIS_AdrXTSZCYDZ+i] = 0;
						}						
						//2020-05-20 hjl 修改限定15个字，避免超出后影响其他参数
						//例如文本框录入1后，收到的返回数据如下：0x0A, 0x83, 0x02, 0x00, 0x02, 0x31, 0x30, 0xFF, 0xFF, 0x5D, 0x3D,
//						for(u8 i=0;i<((ucAdressDataLength-6)/2 > 15 ? 15 : (ucAdressDataLength-6)/2);i++)    //去除长度，指令码，CRC校验等位总共6个字节，剩下的是有效数据位(0x31, 0x30, 0xFF, 0xFF)
//						{
//							pf->Data[FLASHIS_AdrXTSZCYDZ+i] = *(u16*)(p+5+i*2);
//						}
						for(u8 i=0;i<((ucAdressDataLength-8)/2 > 15 ? 15 : (ucAdressDataLength-8)/2);i++)    //去除长度，指令码, 两个0XFF，CRC校验等位总共8个字节，剩下的是有效数据位(0x31, 0x30)
						{
							pf->Data[FLASHIS_AdrXTSZCYDZ+i] = *(u16*)(p+5+i*2);
						}						
							pf->Flag = 1;
						    ps->Key.Mode=0;   //这是专门针对地址输入
						    ScreenBackLastOpt(ps);
							break;
					}
					

					/*************************************     1-开机密码页变量值返回     *******************************************/	
					//开机密码 2020-05-20 hjl add
					case SCREEN_ADD_KJMM:			
					{					
						uTemp = *(u32*)(p+5);
						uTemp = reversebytes_uint32t(uTemp);						
						memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
						sprintf((char*)uScreenRcvBuffer,"%04d",uTemp);
						ps->sRcvType = SCREEN_ADD_KJMM;
						break;
					}
					
					/*************************************     1-系统设置页变量值返回     *******************************************/	
					/*************************************     系统设置页变量值返回     *******************************************/							

					//接收日期
					case SCREEN_ADD_RIQI:			
					{					
						uTemp = *(u32*)(p+5);
						uTemp = reversebytes_uint32t(uTemp);						
						memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
						sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
						ps->sRcvType = SCREEN_ADD_RIQI;
						break;
					}
					
					//接收时间
					case SCREEN_ADD_SHIJIAN:	
					{					
						uTemp = *(u32*)(p+5);
						uTemp = reversebytes_uint32t(uTemp);						
						memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
						sprintf((char*)uScreenRcvBuffer,"%06d",uTemp);	//2020-05-20 hjl 修改输入不足6位补0，避免小时为一位数显示错误
						ps->sRcvType = SCREEN_ADD_SHIJIAN;
						break;
					}

					//大气压
					case SCREEN_ADD_DAQIYA:		
					{					
						uTemp = *(u32*)(p+5);
						uTemp = reversebytes_uint32t(uTemp);		
						fTemp  = (float)uTemp/100;          //因迪文屏此处控件设置了两位小数，返回值自动乘了100，所以此处相应除100；
						memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
						sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
						ps->sRcvType = SCREEN_ADD_DAQIYA;
						break;
					}	
					
					//排水间隔
					case SCREEN_ADD_PSJG:			
					{
						uTemp = *(u32*)(p+5);
						uTemp = reversebytes_uint32t(uTemp);						
						memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
						sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
						ps->sRcvType = SCREEN_ADD_PSJG;
						break;
					}	
					
					//排水时长
					case SCREEN_ADD_PSSS:			
					{				
						uTemp = *(u32*)(p+5);
						uTemp = reversebytes_uint32t(uTemp);						
						memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
						sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
						ps->sRcvType = SCREEN_ADD_PSSS;
						break;
					}


//					//含湿量
//					case SCREEN_ADD_YC_MIN:	
//											{				
//												uTemp = *(u32*)(p+5);
//												uTemp = reversebytes_uint32t(uTemp);						
//												memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
//												sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
//												ps->sRcvType = SCREEN_ADD_YC_MIN;
//												break;
//											}
//					//烟气次数
//					case SCREEN_ADD_YC_TIMES:
//											{				
//												uTemp = *(u32*)(p+5);
//												uTemp = reversebytes_uint32t(uTemp);						
//												memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
//												sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
//												ps->sRcvType = SCREEN_ADD_YC_TIMES;
//												break;
//											}						
					//含湿量
					case SCREEN_ADD_HSL_MIN:	
											{				
												uTemp = *(u32*)(p+5);
												uTemp = reversebytes_uint32t(uTemp);						
												memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
												sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
												ps->sRcvType = SCREEN_ADD_HSL_MIN;
												break;
											}	
						
					/************************************     2-烟道布点变量值返回     **************************************************************************/	
					/*************************************     烟道布点--圆形--矩形--其它--查询	     *****************************************************/
					//内径,套管--2位小数
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
																			
					//面积--4位小数
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
					//环数，孔数---整数
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
					//查找文件号--7位整数，不够补零						
					case SCREEN_ADD_FLUE_FIND:				
												{				
													uTemp = *(u32*)(p+5);
													uTemp = reversebytes_uint32t(uTemp);						
													memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
													sprintf((char*)uScreenRcvBuffer,"%7d",uTemp);
													ps->sRcvType = address;
													break;
												}

					/*************************************     3-工况预测变量值返回     ***********************************************************************************************/	
					/*************************************     工况预测--烟温测量--查询平均	     *************************************************/
					//烟温--1位小数
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
					//平均查询文件号
					case SCREEN_ADD_FILE_CHECK:				
											{				
												uTemp = *(u32*)(p+5);
												uTemp = reversebytes_uint32t(uTemp);						
												memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
												sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
												ps->sRcvType = address;
												break;
											}
						
					/*************************************     4-测含湿量变量值返回     **********************************************************************/	
					/*************************************     测含湿量--含湿量输入	     *************************************************/
					//1-含湿量输入-2020-05-21						
					case SCREEN_ADD_MOISTURE:				
											{	//--两位小数				
												uTemp = *(u32*)(p+5);
												uTemp = reversebytes_uint32t(uTemp);		
												fTemp  = (float)uTemp/100;          
												memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
												sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
												ps->sRcvType = address;
												break;
											}
						
					/*************************************     5-测量烟气变量值返回     *****************************************************************/	
					/*************************************     测量烟气--计时	     *************************************************/
					//1-计时时长-2020-05-22																							
					case SCREEN_ADD_YC_MIN:
					case SCREEN_ADD_YC_TIMES:	
					case SCREEN_ADD_GAS_FIND:
					case SCREEN_ADD_START_NUMBER:
					case SCREEN_ADD_END_NUMBER:
												{	//--整数				
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
											{	//--一位小数				
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
											{	//--两位小数				
												uTemp = *(u32*)(p+5);
												uTemp = reversebytes_uint32t(uTemp);		
												fTemp  = (float)uTemp/100;          
												memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
												sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
												ps->sRcvType = address;
												break;
											}						
																							
					/*************************************     6-烟尘采样     ***********************************************************************************************/	
					/*************************************     烟尘采样--采样设置	     *************************************************/
					//采样方式恒流，采嘴直径						
					case SCREEN_ADD_CC_SAMPLE:			  		
					case SCREEN_ADD_MOUTH_DIAMETER:	  
													{	//--一位小数				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/10;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.1f",fTemp);
														ps->sRcvType = address;
														break;
													}
					//滤筒号，单点采时，总采点						
					case SCREEN_ADD_CARTRIDGE_NUMBER: 
					case SCREEN_ADD_TIME_MINUTE:			
					case SCREEN_ADD_TIME_SECOND:
					case SCREEN_ADD_TOTAL_SAMPLE:	
													{	//整数			
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);						
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
														ps->sRcvType = address;
														break;
													}

					/*************************************     7-文件查询     ***********************************************************************************************/	
					/*************************************     文件查询--计算--文件管理	     *************************************************/
					//1-文件查询-2020-05-22																						
					case SCREEN_ADD_OILGAS_FIND:
					case SCREEN_ADD_RANGES_NUMBER:
					case SCREEN_ADD_START_NUMBER2:
					case SCREEN_ADD_END_NUMBER2:
					case SCREEN_ADD_DUST_FILE_NUM:
													{	//--整数				
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
													{	//--一位小数				
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
													{	//--两位小数				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/100;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
														ps->sRcvType = address;
														break;
													}
					case SCREEN_ADD_DUST_WEIGHT:	
													{	//--四位小数				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/100000;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.4f",fTemp);
														ps->sRcvType = address;
														break;
													}														
																							
					/*************************************     9-校准维护变量值返回     ***********************************************************************************************/	
					/*************************************     校准维护--密码输入--各种校准	     *************************************************/
					//密码---整数
					case SCREEN_ADD_PASSWORD_INPUT:		
													{				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);						
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
														ps->sRcvType = address;
														break;
													}
					//1-系统配置-2020-05-17						
					case SCREEN_ADD_CALIB_NOx:
					case SCREEN_ADD_CO_SO2:
					case SCREEN_ADD_NO2_SO2:
					case SCREEN_ADD_PTG_Kp:
					case SCREEN_ADD_SMOKE_DENSITY:
					case SCREEN_ADD_CALC_RATIO:
					case SCREEN_ADD_OXYGEN_REF:				
													{	//--两位小数				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/100;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
														ps->sRcvType = address;
														break;
													}			
					//1-系统配置--CO对SO2修正-2020-05-18
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
													{	//--两位小数				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/100;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
														ps->sRcvType = address;
														break;
													}
					//2-流量校准============================================================================================//																							
						
						
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
						                          {	//--4位小数				
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
													{	//--两位小数				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/100;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
														ps->sRcvType = address;
														break;
													}																							
					//3-密码设置============================================================================================//
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
					//4-浓度校准																							
					case SCREEN_ADD_O2_TEXT01:	
					case SCREEN_ADD_SO2_TEXT02:	
					case SCREEN_ADD_NO_TEXT03:
					case SCREEN_ADD_NO2_TEXT04:	
					case SCREEN_ADD_CO_TEXT05:	
					case SCREEN_ADD_H2S_TEXT06:	
					case SCREEN_ADD_CO2_TEXT07:		
												   {	//--两位小数				
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
													{	//--三位小数				
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
													{	//--1位小数				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/10;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.1f",fTemp);
														ps->sRcvType = address;
														break;
													}
					//5-压力校准																							
					case SCREEN_ADD_AIR_PRESSURE:			
					case SCREEN_ADD_CALC_PRESSURE:		
					case SCREEN_ADD_STATIC_PRESSURE:	
					case SCREEN_ADD_DYNAMIC_PRESSURE:
					case SCREEN_ADD_DUST_PRESSURE:	 
					case SCREEN_ADD_FLOW_PRESSURE:		
													{	//--两位小数				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/100;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
														ps->sRcvType = address;
														break;
													}
					//7-烟温校准																							
					case SCREEN_ADD_DRY_BALL:		
					case SCREEN_ADD_WET_BALL:		
					case SCREEN_ADD_CALC_TEMP:	
					case SCREEN_ADD_CIRCLE_TEMP:
					case SCREEN_ADD_DRY_RATIO:	
					case SCREEN_ADD_WET_RATIO:				
													{	//--两位小数				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/100;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.2f",fTemp);
														ps->sRcvType = address;
														break;
													}
					//8-电池标定																							
					case SCREEN_ADD_BATTERY_STD:				
													{	//--两位小数				
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		
														fTemp  = (float)uTemp/10000;          
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%.4f",fTemp);
														ps->sRcvType = address;
														break;
													}																							
					//超级维护->仪器编号																							
					case SCREEN_ADD_INSTRUMENT_NUM:
					case SCREEN_ADD_O2_DATE:	
					case SCREEN_ADD_SO2_DATE:
					case SCREEN_ADD_NO_DATE:	
					case SCREEN_ADD_NO2_DATE:
					case SCREEN_ADD_CO_DATE:	
					case SCREEN_ADD_H2S_DATE:
					case SCREEN_ADD_CO2_DATE:							
													{	//--整数				
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
													{	//--整数	
														uTemp = *(u32*)(p+5);
														uTemp = reversebytes_uint32t(uTemp);		         
														memset(uScreenRcvBuffer,0,sizeof(uScreenRcvBuffer));
														sprintf((char*)uScreenRcvBuffer,"%d",uTemp);
														ps->sRcvType = address;														
														break;
                          }														
													
						
					default:
								{
									printf("接收到屏幕未知命令=0x%X\r\n",*(p+1));
									break;
								}
					}
				
				}
		}
	}
//	else printf("错误1\r\n");
}
//---
