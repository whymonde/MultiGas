/* date 2019-12-12
	函数名：void dome(Screen_DataStructure *p)
	描述：	系统设置界面逻辑处理
*/
void dome(Screen_DataStructure *p)
{
	 FourBytes four;
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 //按键处理
	
	 //数据更新
    switch(p->RunModeStat)
		{
		   case 0x01: //静态数据显示			
			 {
			 Screen_ShowPage(10); 	
			 Screen_PastePicture(0x0800,0x000B,(54<<16)+66,(433<<16)+111,(54<<16)+66); //日期
			
			}
			break;
			case 0x02: //动态数据显示
			{			
					
			}
			break;
		}
		//选择基图更新
	  if(p->Key.LastOpt!=p->Key.Opt)
		{ 
		  p->Key.LastOpt=p->Key.Opt; 
			switch(p->Key.Opt)
			{
			  case 0x01:break;
				default:break;
			}
		}
}
//-----
//---
/*
	函数名：void Button_SysSetPage(void)
	描述：	该函数为系统设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/

void Button_Page(void)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	

	//二级对象处理
	switch(ps->Key.Mode)//查看二级对象
	{
		
		case 0:
		{//没有选中二级对象
		  ScreenFirstOpButton(ps,1,10); //底层处理			
			break;
		}
		case 1:
	
		 if(ps->Button==0x0Afe||ps->Button==0x0Aff) //退回上一级
			{
			  ps->Key.Mode=0;
				ps->RunModeStat=1;//刷新数据
			}//取数值
			if(ps->Key.number>=0x30&&ps->Key.number<=0x39)
			{
			  ps->Key.Buffer[ps->Key.TwoOpt]=ps->Key.number;
			}
			break;
		}
		
	 { //屏幕按键处理
				switch(ps->Button)
			{
				case 0x0A00:   //返回
					{
						ps->Key.Opt=ps->Key.OldOpt;
						ps->Key.LastOpt=0;
						ps->RunMode =52; //返回主页面
						ps->RunModeStat=1;//更新静态数据
							break;
					 }
				case 0x0A02:    //日期
				{
								
					break;
				}
				case 0x0A0C:    //时间
				{
				
					break;
				}
				case 0x0A04:    //大气压			
				{
				
					break;
				}
				case 0x0A06:    //类型	
				{
					
					break;
				}	
		}
	} 
	ps->Button=0xffff;
  ps->Key.number=0xff;
}
//---
//更形谋韭数据
Screen_ResetDataBuffer();
sprintf((char*)p->Buffer,"%02d:%02d:%02d",p->Clock.Clock.tm_hour,p->Clock.Clock.tm_min,p->Clock.Clock.tm_sec);
Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);//年
				