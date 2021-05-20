/*
  该文件主要用于处理屏幕需要重复调用的函数
*/

#include "ScreenDwFunction.h"
#include "ScreenDisCalculate.h"
#include "button.h"
#include "screenButton.h"

/*
	函数名：GetU16dataFormFlash2Word(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP)     
	描述：	去取4字节数据（主要作用减少代码行数）
*/
void GetU16dataFormFlash2Word(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP)               
{
	four_p->U16Data[0] = fP->Data[addr];
	four_p->U16Data[1] = fP->Data[addr+1];
	
}
/*
	float GetFlaotdataFormFlash2Word(u32 addr)     
	描述：	去取4字节浮点数据（主要作用减少代码行数）
*/
float GetFlaotdataFormFlash2Word(u32 addr)               
{
	FourBytes four;
	four.U16Data[0] = Flashisp_GetData()->Data[addr];
	four.U16Data[1] = Flashisp_GetData()->Data[addr+1];
	return four.FloatData;
	
}
/*
	u32 GetU32dataFormFlash2Word(u32 addr)     
	描述：	去取4字节浮点数据（主要作用减少代码行数）
*/
u32 GetU32dataFormFlash2Word(u32 addr)               
{
	FourBytes four;
	four.U16Data[0] = Flashisp_GetData()->Data[addr];
	four.U16Data[1] = Flashisp_GetData()->Data[addr+1];
	return four.U32Data;
}
//---
/*
	函数名void SaveFlaotToFlash(float data,u32 addr) 
	描述：	保存浮点数据到FLASE里面（主要作用减少代码行数）
*/
void SaveFlaotToFlash(float data,u32 addr)               
{
		FourBytes four;
		four.FloatData=data;
		Flashisp_DataStructure* pf=Flashisp_GetData();
		pf->Data[addr]	=four.U16Data[0]  ;
		pf->Data[addr+1]	=four.U16Data[1]  ;
		pf->Flag=1;//保存数据
	
}
/*
	函数名SaveU32ToFlash(u32 data,u32 addr) 
	描述：	保存U32数据到FLASE里面（主要作用减少代码行数）
*/
void SaveU32ToFlash(u32 data,u32 addr)               
{
		FourBytes four;
		four.U32Data=data;
		Flashisp_DataStructure* pf=Flashisp_GetData();
		pf->Data[addr]	=four.U16Data[0]  ;
		pf->Data[addr+1]	=four.U16Data[1]  ;
		pf->Flag=1;//保存数据
	
}
/*
	函数名SaveU16ToFlash(u32 data,u32 addr) 
	描述：	保存U16数据到FLASE里面（主要作用减少代码行数）
*/
void SaveU16ToFlash(u16 data,u32 addr)               
{
		Flashisp_DataStructure* pf=Flashisp_GetData();
		pf->Data[addr]	=data;
		pf->Flag=1;//保存数据
	
}
//---
/*
	函数名oid UpdataSensorFixDate(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP,Screen_DataStructure *p)               
	描述：	更新传感器安装时间（主要作用减少代码行数）
  type:   0~7 气体寄存器的种类
*/
void UpdataSensorFixDate(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP,Screen_DataStructure *p,u8 type)               
{
		            GetU16dataFormFlash2Word(four_p,addr,fP);
								Screen_ResetDataBuffer();
								if(((p->LastChoice>>type)&0x01)==0)
								{
								  sprintf((char*)p->Buffer,"未安装");
							
								}else
								{
								  sprintf((char*)p->Buffer,"%02d-%02d-%02d",four_p->U16Data[1],(four_p->U16Data[0]>>8),(four_p->U16Data[0]&0xff));
								}
								Screen_RefreshDataMulti(type*0x20+0x0420,p->Buffer,12*2);
		
}
//---
/*
	函数名： StrinGalignment(* str,u8 len)
	描述：	用于字符串填充至固定长度
*/
void StrinGalignment(u8* str,u8 len)
{
   	u8 NowLen=strlen((const char* )str);
	  if(NowLen<len)
		{
		   for(;NowLen<len;NowLen++)
			{
			  *(str+NowLen)=' ';
			}
			*(str+NowLen)='\0';
		}
}
//---
/*
	函数名： void ScrrenSetJitu(Screen_DataStructure *p)
	描述：	用于设置界面处理基图
  tpye:   基图的类型 0-7
*/
void ScreenSetJitu(Screen_DataStructure *p,u8 type)
{
     switch(type)
		 {
			 case 0:
				 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&(1<<0))==0) //设置-大气压
			{
			    Dis_CheckBox(0x0A20,1,(253<<16)+108);
			}else
			{
			    Dis_CheckBox(0x0A20,1,(353<<16)+108);
			}break;
			 case 9:
			 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&(1<<4))==0) //设置-采样类型
			{
			    Dis_CheckBox(0x0AE0,1,(250<<16)+305);
			}
			else
			{
			   Dis_CheckBox(0x0AE0,1,(378<<16)+305);
			}
			break;
			 case 2:	
				 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&(1<<1))==0)   //设置-防倒吸
			{
			   Dis_CheckBox(0x0A60,0,(253<<16)+159);
			}
			else
			{
			  Dis_CheckBox(0x0A60,1,(253<<16)+159);
			}break;
			 case 3:				
				 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&(1<<2))==0) //设置-蓝牙开关
			{
			   Dis_CheckBox(0x0A80,0,(648<<16)+159);
			}
			else
			{
			  Dis_CheckBox(0x0A80,1,(648<<16)+159);
			}
			break;
			 case 4:
				 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&(1<<3))==0) //设置-自动排水开关
			{
			 Dis_CheckBox(0x0AA0,0,(252<<16)+250);
			}
			else
			{
			  Dis_CheckBox(0x0AA0,1,(252<<16)+250);
			}
			case 5:    //设置基图切换
			{
				if(p->Key.LastOpt!=p->Key.Opt)
				{
					p->Key.LastOpt=p->Key.Opt;
				  switch(p->Key.Opt)
					{
						case 1: Screen_PastePicture(0x0800,0x000B,(55<<16)+50,(230<<16)+95,(55<<16)+50);break; //时间
						case 2: Screen_PastePicture(0x0800,0x000B,(55<<16)+98,(230<<16)+144,(55<<16)+98);break; //大气
						case 3: Screen_PastePicture(0x0800,0x000B,(55<<16)+147,(230<<16)+192,(55<<16)+147);break; //防倒吸
						case 4: Screen_PastePicture(0x0800,0x000B,(448<<16)+147,(621<<16)+190,(448<<16)+147);break;//蓝牙开关
						case 5: Screen_PastePicture(0x0800,0x000B,(55<<16)+196,(230<<16)+240,(55<<16)+196);break;//地址
						case 6: Screen_PastePicture(0x0800,0x000B,(55<<16)+243,(230<<16)+286,(55<<16)+243);break;//自动排水开光
						case 7: Screen_PastePicture(0x0800,0x000B,(328<<16)+245,(427<<16)+287,(328<<16)+245);break;//排水间隔
						case 8: Screen_PastePicture(0x0800,0x000B,(558<<16)+245,(655<<16)+287,(558<<16)+245);break;//排水时长
						case 9: Screen_PastePicture(0x0800,0x000B,(55<<16)+291,(230<<16)+335,(55<<16)+291);break;//采样类型
						case 10: Screen_PastePicture(0x0800,0x000B,(469<<16)+291,(618<<16)+335,(469<<16)+291);break;//测含湿量
						case 11: Screen_PastePicture(0x0800,0x000B,(55<<16)+339,(230<<16)+380,(55<<16)+339);break;//烟尘采样流量
						case 12: Screen_PastePicture(0x0800,0x000B,(719<<16)+392,(795<<16)+456,(719<<16)+392);break;//返回
						default : break;

					}
				}
				      
			break;
			}
//				 case 6:
//				 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&(1<<5))==0) //设置-烟气单位
//			{
//			 Dis_CheckBox(0x0AC0,1,(224<<16)+138);
//			}
//			else
//			{
//			  Dis_CheckBox(0x0AC0,1,(378<<16)+138);
//			}
//			break;
	   		 case 7:
				 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&(1<<6))==0) //设置-烟尘采样流量
			{
			 Dis_CheckBox(0x0B20,1,(251<<16)+350);
			}
			else
			{
			  Dis_CheckBox(0x0B20,1,(490<<16)+350);
			}	
//			case 8:    //采样设置基图切换
//			{
//				if(p->Key.LastOpt!=p->Key.Opt)
//				{
//					p->Key.LastOpt=p->Key.Opt;
//				  switch(p->Key.Opt)
//					{
//						case 1: Screen_PastePicture(0x0800,139,(42<<16)+70,(210<<16)+117,(42<<16)+70);    break; //类型
//						case 2: Screen_PastePicture(0x0800,139,(42<<16)+120,(210<<16)+167,(42<<16)+120);  break; //单位
//						case 3: Screen_PastePicture(0x0800,139,(42<<16)+170,(210<<16)+217,(42<<16)+170);  break; //烟气类型
//						case 4: Screen_PastePicture(0x0800,139,(42<<16)+220,(210<<16)+267,(42<<16)+220);  break; //含湿量
//						case 5: Screen_PastePicture(0x0800,139,(42<<16)+270,(210<<16)+317,(42<<16)+270);  break;//烟尘
//						case 6: Screen_PastePicture(0x0800,139,(600<<16)+370,(710<<16)+440,(600<<16)+370);break;//主页
//						case 7: Screen_PastePicture(0x0800,139,(720<<16)+370,(790<<16)+440,(720<<16)+370);break;//返回	
//						default : break;

//					}
//				}
//				      
//			break;
//			}
			default :break;
		}
}
//---2020-05-19
/*
	函数名：void ScreenFirstOpButton(Screen_DataStructure *ps,u8 OpMax,u8 isNumberChose)
	描述：	 一级菜单按键处理
  OpMax:   一级按键菜单最大值
  ButtonAddr：  对应界面按键起始值
  isNumberChose： 是否对应快捷键数据
*/
void ScreenFirstOpButton(Screen_DataStructure *ps,u8 OpMax,u8 isNumberChose)
{
	    ps->Key.bOutsideKeyboard = false;
		switch(Button_GetData()->KeyData)  //先处理外围按键
			{
				case BUTTON_LEFT:    //左
				if(ps->RunMode==0x8111)   
				    {
						if(ps->Key.Opt>2&&ps->Key.Opt<13)
							ps->Key.Opt+=10;
						else if(ps->Key.Opt>12&&ps->Key.Opt<23)
							ps->Key.Opt-=10;break;
					}
				case BUTTON_UP: //上
				{
					ps->Key.Opt--;
					if(ps->RunMode==0x8111)   
				    {
					if(ps->Key.Opt ==1) ps->Key.Opt = OpMax;
					}
					else
					{
					if(ps->Key.Opt ==0) ps->Key.Opt = OpMax;	
					}
					break;
				}
				case BUTTON_RIGHT: //右
				if(ps->RunMode==0x8111)
				{
					if(ps->Key.Opt>2&&ps->Key.Opt<13)
						ps->Key.Opt+=10;
					else if(ps->Key.Opt>12&&ps->Key.Opt<23)
						ps->Key.Opt-=10;break;
				}
				case BUTTON_DOWN: //下
				{
					ps->Key.Opt++;
					if(ps->RunMode==0x8111)  
					{
					if(ps->Key.Opt == OpMax+1) ps->Key.Opt = 2;
					}
					else
					{
					if(ps->Key.Opt == OpMax+1) ps->Key.Opt = 1;	
					}
					
					break;
				}
				case BUTTON_BACK: //返回
				{
					ps->Button=0x0AFe;
					break;
				}
				case BUTTON_OK:   //第一次按OK
				{
					 ps->Key.bOutsideKeyboard = true;
					 if(ps->Key.Opt<=OpMax&&ps->Key.Opt>0) 
						{
							if(ps->RunMode==0x1000) //设置界面数值快捷键
							{
							  if(ps->Key.Opt>=2)
								{
								  ps->Button=ps->Key.Opt+1;   //返回BUTTUNKEY
								}else
								{
								 ps->Button=ps->Key.Opt;
								}
							}
//							else 	if(ps->RunMode==0x1100) //设置界面数值快捷键
//							{
//							  if(ps->Key.Opt>=4)
//								{
//								  ps->Button=ps->Key.Opt+1;
//								}else
//								{
//								 ps->Button=ps->Key.Opt;
//								}
//							}
							else if(ps->RunMode==0x5000)
							{
								if(ps->Key.Opt>=5)
								{ 
									 ps->Button=ps->Key.Opt+1;
								}
								else
								{
									ps->Button=ps->Key.Opt;
								}

							}
							else if(ps->RunMode==0x6000) //设置界面数值快捷键
							{
								
							  if(ps->Key.Opt<=4)
								{ 
									 ps->Button=ps->Key.Opt;
								} 
								else if(ps->Key.Opt>=5)
								{ 
									 ps->Button=ps->Key.Opt+1;
								}

							}
							else 	if(ps->RunMode==0x8210) //气体传感器选择 
							{
							 ps->Button=ps->Key.Opt;
							}else
							{
							 ps->Button=ps->Key.Opt;
							}

						}
				}		
				break;
				case BUTTON_1:  ps->Key.KeyData=1; break;
				case BUTTON_2:  ps->Key.KeyData=2; break;
				case BUTTON_3:  ps->Key.KeyData=3; break;
				case BUTTON_4:  ps->Key.KeyData=4; break;
				case BUTTON_5:  ps->Key.KeyData=5; break;
				case BUTTON_6:  ps->Key.KeyData=6; break;
				case BUTTON_7:  ps->Key.KeyData=7; break;
				case BUTTON_8:  ps->Key.KeyData=8; break;
				case BUTTON_9:  ps->Key.KeyData=9; break;
				case BUTTON_F1:  ps->Key.KeyData=10; break;
				case BUTTON_F2:  ps->Key.KeyData=11; break;	
				case BUTTON_F3:  ps->Key.KeyData=12; break;	
				case BUTTON_F4:  ps->Key.KeyData=13; break;					
			}
				if(isNumberChose==1) //菜单数字快捷键
				{
					if(ps->Key.KeyData<=OpMax)  //按键值<=屏幕按键触控
					{
						ps->Key.bOutsideKeyboard = true;
					}
					
					if(ps->RunMode==0x1000) //设置界面数值快捷键
					{
					  if(ps->Key.KeyData==1)
						{ 
						   ps->Button=ps->Key.KeyData;
						}
					  else if(ps->Key.KeyData>=2&&ps->Key.KeyData<=6)
						{
						  ps->Button=ps->Key.KeyData+1;
						}
					  else if(ps->Key.KeyData>=7&&ps->Key.KeyData<=9)
						{
						  ps->Button=ps->Key.KeyData+3;
						}

					}
					else 	if(ps->RunMode==0x1100) //设置界面数值快捷键
					{
					  if(ps->Key.KeyData<=3)
						{ 
						   ps->Button=ps->Key.KeyData;
						}
						else if(ps->Key.KeyData<OpMax&&ps->Key.KeyData>=4)
						{
						  ps->Button=ps->Key.KeyData+1;
						}
					}
					else 	if(ps->RunMode==0x6000) //设置界面数值快捷键
					{
						
					  if(ps->Key.KeyData<=1)
						{ 
						   ps->Button=ps->Key.KeyData;
						} 
					  else if(ps->Key.KeyData<=3)
						{ 
						   ps->Button=ps->Key.KeyData+1;
						}
					  else if(ps->Key.KeyData<OpMax&&ps->Key.KeyData>=4)
						{
						  ps->Button=ps->Key.KeyData+2;
						}
						
					}
					else 	if(ps->RunMode==0x6200) //设置界面数值快捷键
					{
					  if(ps->Key.KeyData<=1)
						{ 
						   ps->Button=ps->Key.KeyData;
						} else if(ps->Key.KeyData==3)
						{ 
						   ps->Button=2;
						}
						else if(ps->Key.KeyData==2)
						{
						  ps->Button=3;
						}
					}
					else 	if(ps->RunMode==0x8120) //流量校准
					{
					  if(ps->Key.KeyData<=0x09)
						{ 
						   ps->Button=ps->Key.KeyData;
						}else if(ps->Key.KeyData<=0x0b)	
						{
						  ps->Button=ps->Key.KeyData+4;
						}else if(ps->Key.KeyData<=0x0d)	
						{
						  ps->Button=ps->Key.KeyData-2;
						}
					}
					else if(ps->RunMode==0x8130) //密码设置
					{
						if((ps->Key.KeyData>=8)&&(ps->Key.KeyData<=10))
						{
						 ps->Button=ps->Key.KeyData+2;
						}
						else if((ps->Key.KeyData>=6)&&(ps->Key.KeyData<=7))
						{ 
						   ps->Button=ps->Key.KeyData+1;
						}
						else if(ps->Key.KeyData<=5)
						{
							ps->Button=ps->Key.KeyData; 
						}	

					}					
					else if(ps->RunMode==0x8140) //浓度校准
					{
					  if(ps->Key.KeyData<=0x07)
						{ 
						   ps->Button=ps->Key.KeyData;
						}
						else if(ps->Key.KeyData>=0x0a&&ps->Key.KeyData<=0x0C)	
						{
						  ps->Button=ps->Key.KeyData-2;
						}
					}
					else if(ps->RunMode==0x8150) //压力校准
					{
					  if(ps->Key.KeyData<=0x06)
						{ 
								ps->Button=ps->Key.KeyData;
						}else if(ps->Key.KeyData>=0x0a&&ps->Key.KeyData<=0x0b)	
						{
						  ps->Button=ps->Key.KeyData-3;
						}
					}
					else 	if(ps->RunMode==0x8170) //温度校准
					{
					  if(ps->Key.KeyData==1)
						{ 
						   ps->Button=0x0021;
						}else if(ps->Key.KeyData==2)
						{ 
						   ps->Button=0x0031;
						}
						 else if(ps->Key.KeyData==3)
						{ 
						   ps->Button=0x0022;
						} else if(ps->Key.KeyData==4)
						{ 
						   ps->Button=0x0032;
						}else  if(ps->Key.KeyData==5)
						{ 
						   ps->Button=0x0023;
						} else if(ps->Key.KeyData==7)
						{ 
						   ps->Button=0x0024;
						}else if(ps->Key.KeyData>=0x0a&&ps->Key.KeyData<=0x0b)	
						{
						  ps->Button=ps->Key.KeyData-5;
						}
					
					}
					else 	if(ps->RunMode==0x8210) //气体传感器选择 
					{
					  if(ps->Key.KeyData<=7)
						{ 
						   ps->Button=ps->Key.KeyData;  //  ps->Key.KeyData*2    -2020-09-03
						}
					}
					else if(ps->RunMode==101)		//2020-05-20 hjl add开机密码界面处理
					{
						if(ps->Key.KeyData==1)
							{ 
								ps->Button=ps->Key.KeyData;
							}
						if(ps->Key.KeyData != 0xFF)
							{
									ps->Key.bOutsideKeyboard = true;
							}
					}					
					else
					{
				  if(ps->Key.KeyData>0&&ps->Key.KeyData<OpMax)
					{
						ps->Key.bOutsideKeyboard = true;
					  ps->Button=ps->Key.KeyData;
						ps->Key.KeyData=0xff;
					}
				}
				}
		   Button_GetData()->KeyData=BUTTON_NONE;
	  
}



/*
	函数名： void ScreenTowOpButton(Screen_DataStructure *ps,u8 type,u8 OpMax,u8 isOpChang)
	描述：	二级菜单按键处理
  tpye:   二级按键菜单类型
*/
void ScreenTowOpButton_KeyControl(Screen_DataStructure *ps, u8 isHavePoint,u8 isHaveSign)
{

		 switch(Button_GetData()->KeyData)
			{
				case BUTTON_LEFT:
				case BUTTON_RIGHT:
				case BUTTON_UP:
				case BUTTON_DOWN:
				case BUTTON_BACK:
				{
					ps->Button=0x0AFE;
					Screen_WriteRegisterOneData(0x4f,0xf0);
					break;
				}
				case BUTTON_OK:
				{
				   ps->Button=0x0AFF;
					Screen_WriteRegisterOneData(0x4f,0xf1);break;
				}
				case BUTTON_DELETE:  Screen_WriteRegisterOneData(0x4f,0xf2);break;
				case BUTTON_1:	Screen_WriteRegisterOneData(0x4f,1);break;
				case BUTTON_2:	Screen_WriteRegisterOneData(0x4f,2);break;
				case BUTTON_3:  Screen_WriteRegisterOneData(0x4f,3);break;
				case BUTTON_4:	Screen_WriteRegisterOneData(0x4f,4);break;
				case BUTTON_5:	Screen_WriteRegisterOneData(0x4f,5);break;
				case BUTTON_6: 	Screen_WriteRegisterOneData(0x4f,6);break;
				case BUTTON_7:	Screen_WriteRegisterOneData(0x4f,7);break;
				case BUTTON_8:	Screen_WriteRegisterOneData(0x4f,8);break;
				case BUTTON_9: 	Screen_WriteRegisterOneData(0x4f,9);break;
				case BUTTON_0: 	Screen_WriteRegisterOneData(0x4f,0x30);break;
				
				case BUTTON_POINT:			{
																	if(isHavePoint == CALC_HAVE_POINT)
																	Screen_WriteRegisterOneData(0x4f,0x2e);
																	break;
											}
				
				case BUTTON_PLUS_MINUS: {
																	if(isHavePoint == CALC_HAVE_SIGNS)
																		Screen_WriteRegisterOneData(0x4f,0x2d);
																	break;
										}

				}
			
		 Button_GetData()->KeyData=BUTTON_NONE;
	  
}

/*
	函数名： void ScreenTowOpButton(Screen_DataStructure *ps,u8 type,u8 OpMax,u8 isOpChang)
	描述：	二级菜单按键处理针对输入地址的子程序
    tpye:   二级按键菜单类型
*/
void ScreenTowOpButton_KeyControl_adr(Screen_DataStructure *ps)
{

		 switch(Button_GetData()->KeyData)
			{
				case BUTTON_LEFT:
				case BUTTON_RIGHT:
				case BUTTON_UP:
				case BUTTON_DOWN:
				case BUTTON_BACK:
				{
					ps->Button=0xAFe;
					Screen_WriteRegisterOneData(0x4f,0xf0);
					break;
				}
				case BUTTON_OK:
				{
				   ps->Button=0xFFFF;
				   Screen_WriteRegisterOneData(0x4f,0xf1);break;
				}
			

			}
		    Button_GetData()->KeyData=BUTTON_NONE;
}

//----
/*
	函数名： void ScreenTowOpButton(Screen_DataStructure *ps,u8 type,u8 OpMax,u8 isOpChang)
	描述：	二级菜单按键处理
  tpye:   二级按键菜单类型
*/
void ScreenTowOpButton(Screen_DataStructure *ps,u8 type,u8 OpMax,u8 isOpChang)
{

	   	if(isOpChang)  //上下左右键生效（移位）
				{
							switch(Button_GetData()->KeyData)
							{
								case BUTTON_LEFT:
								case BUTTON_UP:
								{
									if(ps->Key.TwoOpt > 0)
										ps->Key.TwoOpt--;
										ps->Key.KeyDir=1;
									 break;
								}
								case BUTTON_RIGHT:
								case BUTTON_DOWN:
								{
									if(ps->Key.TwoOpt < OpMax) ps->Key.TwoOpt ++;
									 ps->Key.KeyDir=2;
									break;
								}
							}
					}
		 switch(Button_GetData()->KeyData)
			{
				case BUTTON_BACK:
				{
					ps->Button=0x0AFE;
					break;
				}
				case BUTTON_OK:
				{
				   ps->Button=0x0AFF;
				   break;
				}
				case BUTTON_DELETE:  ps->Button=0x0AFc;break;
				case BUTTON_1:ps->Key.number=0x31;break;
				case BUTTON_2:ps->Key.number=0x32;break;
				case BUTTON_3:ps->Key.number=0x33;break;
				case BUTTON_4:ps->Key.number=0x34;break;
				case BUTTON_5:ps->Key.number=0x35;break;
				case BUTTON_6:ps->Key.number=0x36;break;
				case BUTTON_7:ps->Key.number=0x37;break;
				case BUTTON_8:ps->Key.number=0x38;break;
				case BUTTON_9:ps->Key.number=0x39;break;
				case BUTTON_0: ps->Key.number=0x30;break;
				case BUTTON_POINT: ps->Key.number=0x2e;break;
				case BUTTON_PLUS_MINUS: ps->Key.number=0x2b;break;
				}
			
		 Button_GetData()->KeyData=BUTTON_NONE;
	  
}

//----
/*
	函数名： void ScreenTowOptChose(Screen_DataStructure *ps,u8 opt)
	描述：	二级菜单按键选择预先处理
  tpye:   二级按键菜单类型
*/
void ScreenTowOptChose(Screen_DataStructure *ps,u8 opt)
{	
	
	          Screen_ResetKeyDataBuffer();//清除按键值
              ps->Key.Opt=opt;
			  ps->Key.Mode=opt;
			  ps->Key.TwoOpt=0;
	          ps->RunModeStat=1;//20200310
	          ps->Timer=0;
	          
}
//-----
/*
	函数名： ScreenBackLastPag(Screen_DataStructure *ps,u16 lastMode)
	描述：	返回上级界面
*/
void ScreenBackLastPag(Screen_DataStructure *ps,u16 lastMode)
{
  	                    ps->Key.Opt=ps->Key.OldOpt;
						ps->Key.LastOpt=0;
						ps->RunMode =lastMode; //返回主页面
						ps->RunModeStat=1;//更新静态数据
	                    ps->Key.Mode=0;//二级选项清0
}



// 2020-05-25 modify by alfred.
/*
	函数名： ScreenBackLastPag(Screen_DataStructure *ps)
	描述：	返回上一级菜单
*/
void ScreenBackLastOpt(Screen_DataStructure *ps)
{
						ps->Key.Mode=0;     //确认后跳回一级对像
						ps->Key.LastOpt=0;   ////确认后跳回一级对像
						ps->RunModeStat=1;  //更新静态数据, 当按了确认或返回时ps->RunModeStat=1更新页面数据。 
						ps->Button=0XffFF;
						ps->uEnableRecDataTimer = 0;
						ps->sReveiveDataTimer = 0;			//退出二级菜单时关闭定时器

}
/*
	函数名： ScreenBackLastPag(Screen_DataStructure *ps)
	描述：	返回一级菜单
*/
void ScreenBackFirstOpt(Screen_DataStructure *ps)
{
	if(ps->Button==0x0Aff||ps->Button==0x0Afe)
	{     
		ps->Key.Mode=0;     //确认后跳回一级对像
		ps->Key.LastOpt=0;   ////确认后跳回一级对像
		ps->RunModeStat=1;  //更新静态数据, 当按了确认或返回时ps->RunModeStat=1更新页面数据。 
		ps->Button=0XffFF;
	}
}
//---
/*
	函数名：void ScreenConfirmEnter(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min)
	描述：	输入数值确认  
  存储类型为16位或32位的数据
  //type 1 U16  2  float  3 u32
*/
void ScreenConfirmEnter(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min)
{
	     Flashisp_DataStructure* pf=Flashisp_GetData();
	     
  	     if(type==2)
				 {				 
					FourBytes four;
					four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));
					if(four.FloatData<Min) four.FloatData=Min; //限制 大于50Kp
					if(four.FloatData>Max) four.FloatData=Max; //限制 小于于130Kp
					pf->Data[Addr]=four.U16Data[0]; //保存值
					pf->Data[Addr+1]=four.U16Data[1];
					
				 } if(type==3)
				 {				 
					FourBytes four;
					four.U32Data = (u32)atoi((char*)&(ps->Key.Buffer[0]));	//2020-05-19 hjl 修改强制转化类型为u32 
					if(four.U32Data<Min) four.U32Data=Min; //限制 大于50Kp
					if(four.U32Data>Max) four.U32Data=Max; //限制 小于于130Kp
					pf->Data[Addr]=four.U16Data[0]; //保存值
					pf->Data[Addr+1]=four.U16Data[1];
					
				 }else   if(type==1)
				 {
					u16 Datai=atoi((char*)&(ps->Key.Buffer[0]));
					if(Datai<Min) Datai=Min; //限制 大于50Kp
					if(Datai>Max) Datai=Max; //限制 小于于130Kp
					pf->Data[Addr]=Datai; //保存值
				 }
				 pf->Flag=1;
}
//---
/*
	函数名：void ScreenConfirmEnterDisBuffer(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min)
	描述：	输入数值确认----确认显示缓存 
  存储类型为float 数据
  //position 该数据的位置
*/
void ScreenConfirmEnterDisBuffer(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min)
{
					DisBuffer[position] = (float)atof((char*)&(ps->Key.Buffer[0]));
					if(DisBuffer[position]<Min) DisBuffer[position]=Min; //限制 大于
					if(DisBuffer[position]>Max) DisBuffer[position]=Max; //限制 小于于

}
//---
/*
	函数名：void ScreenEnterNumberText(Screen_DataStructure *ps,u8 type)
	描述：	数值  
  数值类型：type 0  无符号整形
            type 1  有符号整形
            type 2  无符号浮点型
            type 3  有符号浮点型
  文本长度：len
*/
void ScreenEnterNumberText(Screen_DataStructure *ps,u8 type,u8 len)
{
	if(type==0)
	{
	 	if((ps->Key.number>=0x30&&ps->Key.number<=0x39))//ps->Button==0x0AfC||ps->Key.number==0x2e
			{
				
			  ps->Key.Buffer[ps->Key.TwoOpt++]=ps->Key.number;
				if(ps->Key.TwoOpt>len) 
				{
					ps->Key.TwoOpt=len;//限制输入长度
					ps->Key.Buffer[ps->Key.TwoOpt]=0x00;
				}
				
			}
		}
	else if(type==1)
	{
	 	if((ps->Key.number>=0x30&&ps->Key.number<=0x39)||ps->Key.number==0x2b)//ps->Button==0x0AfC||ps->Key.number==0x2e
		{
			if(ps->Key.TwoOpt==0)
			{ 
				ps->Key.Buffer[ps->Key.TwoOpt++]=' ';
			}
			else
			{
				if(ps->Key.number==0x2b)
				{
					if(ps->Key.Buffer[0]==' ')
					{
						ps->Key.Buffer[0]='-';
					}
					else
					{
						ps->Key.Buffer[0]=' ';
					}
				}
				else
				{
					ps->Key.Buffer[ps->Key.TwoOpt++]=ps->Key.number;
					if(ps->Key.TwoOpt>len)
					{
						ps->Key.TwoOpt=len;//限制输入长度
						ps->Key.Buffer[ps->Key.TwoOpt]=0x00;
					}
				}
			}
		}
	}
	else if(type==2)
	{
	 	if((ps->Key.number>=0x30&&ps->Key.number<=0x39)||ps->Key.number==0x2e)//ps->Button==0x0AfC
		{
		  	ps->Key.Buffer[ps->Key.TwoOpt++]=ps->Key.number;
			if(ps->Key.TwoOpt>len) 
			{
				ps->Key.TwoOpt=len;//限制输入长度
				ps->Key.Buffer[ps->Key.TwoOpt]=0x00;
			}
		}
	}
	else if(type==3)
	{
	 	if((ps->Key.number>=0x30&&ps->Key.number<=0x39)||ps->Key.number==0x2b||ps->Key.number==0x2e)//ps->Button==0x0AfC||ps->Key.number==0x2e
		{
			if(ps->Key.TwoOpt==0)
			{ 
				ps->Key.Buffer[ps->Key.TwoOpt++]=' ';				
			}
			else
			{
				if(ps->Key.number==0x2b)
				{
				  	if(ps->Key.Buffer[0]==' ')
					{
					  ps->Key.Buffer[0]='-';
					}
					else
					{
					   ps->Key.Buffer[0]=' ';
					}
				}
				else
				{
					ps->Key.Buffer[ps->Key.TwoOpt++]=ps->Key.number;
					if(ps->Key.TwoOpt>len) 
					{
						ps->Key.TwoOpt=len;//限制输入长度
						ps->Key.Buffer[ps->Key.TwoOpt]=0x00;
					}
				}
			}
		}
	}
  	if(ps->Button==0x0AfC) //删除
	{
	  	if(ps->Key.TwoOpt>0)
		{
		  ps->Key.Buffer[--ps->Key.TwoOpt]='\0';
		}
		ps->Button=0xffff;
	}
	ps->Key.number=0xff;
}
//---

//临时测试用，后面规范时删除掉
void ScreenTextTwinkle(Screen_DataStructure *ps,u16 TextAddr, u16 FlashAddress, u8 valType)
{
    
			if(ps->Key.State == 0)	//State用于数字闪烁
			{
				if(0);					//if((ps->RunMode==0x1000&&(ps->Key.Mode==1|ps->Key.Mode==2))||(ps->RunMode==0x8210)) ;
				else
				{
					Screen_ResetDataBuffer();
					switch(valType)
												{
													case TWO_DIGIT_INT:  sprintf((char*)ps->Buffer,"%.2d",Flashisp_GetData()->Data[FlashAddress]);break;
													
						              case TWO_POINT_FLOAT:		{
																										FourBytes four;
																										four.FloatData=GetFlaotdataFormFlash2Word(FlashAddress);
																										sprintf((char*)ps->Buffer,"%.2f",four.FloatData);
																										break;
																									}
													
													case EIGHT_POINT_DATA: 	sprintf((char*)ps->Buffer,"%02d-%02d-%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday);	break;

													case EIGHT_POINT_TIME:	sprintf((char*)ps->Buffer,"%02d:%02d:%02d",ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);	break;
														
													default :	break;
												}
				}
				  ps->Key.State = 1;
			}
			else 
				{
					ps->Key.State = 0;
					Screen_ResetDataBuffer();
				}
			Screen_RefreshDataMulti(TextAddr,ps->Buffer,15*2);//
}


/*
	函数名：void ScreenEnterNumberTwinkle(Screen_DataStructure *ps,u16 TextAddr)
	描述：	数值输入闪烁  

*/
void ScreenEnterNumberTwinkle(Screen_DataStructure *ps,u16 TextAddr)
{
    
			strcpy((char*)ps->Buffer,(char*)ps->Key.Buffer);
			if(ps->Key.State == 0)	//State用于数字闪烁
			{
				if(((ps->RunMode==0x1000)&&((ps->Key.Mode==1)||(ps->Key.Mode==2)))||((ps->RunMode==0x8210)&&(ps->Key.Mode<=7)))  //屏蔽某些特殊页面
					;
				else
					ps->Buffer[ps->Key.TwoOpt] = '|';
				  ps->Key.State = 1;
			}
			else {ps->Key.State = 0;
					ps->Buffer[ps->Key.TwoOpt] = ' ';
			}
			Screen_RefreshDataMulti(TextAddr,ps->Buffer,15*2);//上传缓冲区数据到迪文屏
}
//-----
/*20200521
	函数名：void ScreenChangeDataInit(Screen_DataStructure *ps)
	描述：	页面切换数据初始化

*/
void ScreenChangeDataInit(Screen_DataStructure *ps)
{	
	
	if(ps->OldMode!=ps->RunMode)
	{
		ps->Key.OldOpt=ps->Key.Opt;
		ps->Key.LastOpt=0;
		ps->RunModeStat=1;//更新静态数据	
		ps->TimerSecUp=0;
		ps->Timer=0;
		ps->Key.Opt=1;	
		if(ps->RunMode==0x8111)
		ps->Key.Opt=2;
		ps->Key.Mode=0;	
		// for(int i=0;i<20;i++)
		// ps->DisValue[i]=0;	
		ps->Warning=0;	
		ps->LastChoice=0xff;
		ps->LastCoiceonoff=0xff;
		if((ps->RunMode!=0x8141)&&(ps->RunMode!=0x8140))   //特殊页面处理
		ps->PauseOrStop=0;	
		ps->StarCun=1;		
		ps->LoadFlag=1;				
	}
	}
//---
/*
	函数名：void ScreenButtonReset(Screen_DataStructure *ps)
	描述：	按键参数复位

*/
void ScreenButtonReset(Screen_DataStructure *ps)
{	
	
   ps->Button=0xffff;
   ps->Key.number=0xff;
   ps->Key.KeyData=0xff;	
}
//---
/*
	u8* GetYDBD_Describe(u8 Type)
	描述：	返回烟道布点描述
*/
u8* GetYDBD_Describe(u8 Type)
{	
	 	Screen_DataStructure *ps = Screen_GetData();
   Flashisp_DataStructure* pf=Flashisp_GetData();
	 if(Type==1)
	 {
	    sprintf((char*)ps->Buffer,"圆形烟道");
	 }
	 else if(Type==2)
	 {
	    sprintf((char*)ps->Buffer,"方形烟道");
	 }
	 else if(Type==3)
	 {
	    sprintf((char*)ps->Buffer,"其他形烟道");
	 } else 
	 {
	    sprintf((char*)ps->Buffer,"未知烟道");
	 }
	 return ps->Buffer;
}
//---
/*
	函数名：void Dis_Tips(u8* Type)
	描述：	提示显示
*/
void Dis_Tips(Screen_DataStructure *p,u8* Tips,u16 TextAddr)
{	
	
  sprintf((char*)p->Buffer,"%s",Tips);
  Screen_RefreshDataMulti(TextAddr,p->Buffer,15*2);
}
//---

/*
	函数名：u16 Dis_CheckBox(u16 addr,u8 Stat,u32 DisXY)
	描述：	勾选
*/
void Dis_CheckBox(u16 addr,u8 Stat,u32 DisXY)
{	
	
  if(Stat==1)Screen_PastePicture(addr,11,(353<<16)+109,(370<<16)+129,DisXY);
	else  if(Stat==0)
	{
	   Screen_PastePicture(addr,10,(353<<16)+109,(370<<16)+129,DisXY);
	}
}
//---
/*
	函数名：u16 Set_SysCheckBox(u16 addr,u16 ByteNumber)
	描述：	设置-系统设置勾选框/打印选择勾选框设置
*/
void Set_SysCheckBox(u16 addr,u16 ByteNumber)
{	
	 Flashisp_DataStructure* pf=Flashisp_GetData();
		if((pf->Data[addr]&(1<<ByteNumber))==0x0000)
		{
			TwoBytesBitFunction(pf->Data[addr],ByteNumber,1);
		 
		}else
		{
			TwoBytesBitFunction(pf->Data[addr],ByteNumber,0);
		
		}

}
//---
/*
	float GetFlueArea (void)
	描述：	获取烟道面积
*/
float GetFlueArea (void)
{	
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 FourBytes four;
	 four.U16Data[0]=pf->Data[FLASHIS_AdrYDBD_MJ];
	 four.U16Data[1]=pf->Data[FLASHIS_AdrYDBD_MJ+1];
	return four.FloatData;

}
//---
/*
	float GetFlueArea (void)
	描述：	获取烟道面积
*/
void DisQTND (u32 addr,float *DisBuffer,u8 len)
{	
	 Flashisp_DataStructure* pf=Flashisp_GetData();
}

//---

/*
	float YC_SampleControl (void)
	描述：	烟尘采样控制处理  在Screen_SecTimer() 中调用
*/
void YC_SampleControl(void)
{	 u32 cal;
	 float buf;
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 Screen_DataStructure *ps = Screen_GetData();
	 if(Flux_GetData()->ChannelB.RunMode==2) //烟尘采样中
	 {    
		   cal=Flux_GetData()->ChannelB.Sampling.DinSiTime-Flux_GetData()->ChannelB.Sampling.IntTime;
			if(cal<10&&(cal%2))//单次小于10s 报警两秒报警一次
			{
			  Speak_Buzzer(200);//响500ms
			}
			if(pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]==1&&(cal%10==0)) //等速更新
				{
					 //to do 等速跟踪
					//Flux_ContinueSampling('B',	Flux_GetData()->ChannelA.ShowFlux);
					buf=CalculationFormula_Qrs(GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ),CalculationFormula_GetData()->Vs,GetFlaotdataFormFlash2Word(FLASHIS_AdrCHSL_HSL)/100);//根据采嘴直径，实测流速，含湿量
					Flux_ContinueSampling('B',buf);
					ps->YC_TrageFlux=buf;
					if(ps->YC_TrageFlux<5) ps->YC_TrageFlux=5; //限制过小
				} 
			else if(pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]==0)
				{
				   ps->YC_TrageFlux=GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CYLS);//目标流量
				}
			  ps->YC_FollowRate= (ps->YC_FollowRate+Flux_GetData()->ChannelB.ShowFlux/ps->YC_TrageFlux)/2;	

	 }
}


/*
	函数名：void ScreenConfirmEnter(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min)
	描述：	软键盘输入数值确认  
  存储类型为16位或32位的数据
  //type 1 U16  2  float  3 u32
*/
void SoftKeyboardConfirmEnter(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min)
{
	     Flashisp_DataStructure* pf=Flashisp_GetData();
	     
				Screen_ResetKeyDataBuffer();//清除按键值
				strcpy((char*)ps->Key.Buffer,(char*)RcvBuffer_GetData()); 
	
  	     if(type==2)
				 {				 
					FourBytes four;
					four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));
					if(four.FloatData<Min) four.FloatData=Min; //限制 大于50Kp
					if(four.FloatData>Max) four.FloatData=Max; //限制 小于于130Kp
					pf->Data[Addr]=four.U16Data[0]; //保存值
					pf->Data[Addr+1]=four.U16Data[1];
					
				 } if(type==3)
				 {				 
					FourBytes four;
					four.U32Data = (float)atoi((char*)&(ps->Key.Buffer[0]));
					if(four.U32Data<Min) four.U32Data=Min; //限制 大于50Kp
					if(four.U32Data>Max) four.U32Data=Max; //限制 小于于130Kp
					pf->Data[Addr]=four.U16Data[0]; //保存值
					pf->Data[Addr+1]=four.U16Data[1];
					
				 }else   if(type==1)
				 {
					u16 Datai=atoi((char*)&(ps->Key.Buffer[0]));
					if(Datai<Min) Datai=Min; //限制 大于50Kp
					if(Datai>Max) Datai=Max; //限制 小于于130Kp
					pf->Data[Addr]=Datai; //保存值
				 }
				 
				 
				 pf->Flag=1;
				 ps->sRcvType = 0;
				 ps->Key.LastMode= ps->Key.Mode;
				 
				 ScreenBackLastOpt(ps);

}

/*
	函数名：ProcessSoftKeyboardInput(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min,u8 isHavePoint,u8 isHaveSign)
	描述：	处理键盘输入 
  存储类型为16位或32位的数据
  //type 1 U16  2  float  3 u32
*/
void ProcessSoftKeyboardInput(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min,u8 isHavePoint,u8 isHaveSign)
{
	ScreenTowOpButton_KeyControl(ps,isHavePoint,isHaveSign);
	if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
	{     
		ScreenBackLastOpt(ps);
	}
	if(ps->sRcvType !=0)
	{
		SoftKeyboardConfirmEnter(ps,Addr,type,Max,Min);		
	}
	else if(ps->Button==0x0Aff)
	{
		ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
		ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置	
	}				
}


/*
	函数名：void SoftKeyboardConfirmEnterDisBuffer(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min)
	描述：	软键盘输入数值确认----确认显示缓存 
  存储类型为float 数据
  //position 该数据的位置
*/
void SoftKeyboardConfirmEnterDisBuffer(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min)
{

					Screen_ResetKeyDataBuffer();//清除按键值
					strcpy((char*)ps->Key.Buffer,(char*)RcvBuffer_GetData()); 
  
					DisBuffer[position] = (float)atof((char*)&(ps->Key.Buffer[0]));
					if(DisBuffer[position]<Min) DisBuffer[position]=Min; //限制 大于
					if(DisBuffer[position]>Max) DisBuffer[position]=Max; //限制 小于于

					ps->sRcvType = 0;
//					ScreenBackLastOpt(ps);
}

/*
	函数名：ProcessSoftKeyboardInput(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min,u8 isHavePoint,u8 isHaveSign)
	描述：	处理键盘输入(用于文件查询时文件号的录入） 
  存储类型为16位或32位的数据
  //type 1 U16  2  float  3 u32
*/
void ProcessSoftKeyboardInputFileName(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min,u8 isHavePoint,u8 isHaveSign)
{
	ScreenTowOpButton_KeyControl(ps,isHavePoint,isHaveSign);
	if((ps->Button==0x0Afe)||((ps->uEnableRecDataTimer == 1)&&(ps->sReveiveDataTimer == 0)))
	{     
		ScreenBackLastOpt(ps);
	}
	if(ps->sRcvType !=0)
	{
		SoftKeyboardConfirmEnterDisBuffer(ps,DisBuffer,position,Max,Min);
		ScreenBackLastOpt(ps);	
	}
	else if(ps->Button==0x0Aff)
	{
		ps->uEnableRecDataTimer = 1; //如果按下OK键,启动定时器	
		ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//定时器检查超时时间设置					
	}				
}

//---



			

