/* date 2019-12-12
	��������void dome(Screen_DataStructure *p)
	������	ϵͳ���ý����߼�����
*/
void dome(Screen_DataStructure *p)
{
	 FourBytes four;
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 //��������
	
	 //���ݸ���
    switch(p->RunModeStat)
		{
		   case 0x01: //��̬������ʾ			
			 {
			 Screen_ShowPage(10); 	
			 Screen_PastePicture(0x0800,0x000B,(54<<16)+66,(433<<16)+111,(54<<16)+66); //����
			
			}
			break;
			case 0x02: //��̬������ʾ
			{			
					
			}
			break;
		}
		//ѡ���ͼ����
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
	��������void Button_SysSetPage(void)
	������	�ú���Ϊϵͳ���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/

void Button_Page(void)
{
	Screen_DataStructure *ps = Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	//Button_DataStructure *p = Button_GetData();
	

	//����������
	switch(ps->Key.Mode)//�鿴��������
	{
		
		case 0:
		{//û��ѡ�ж�������
		  ScreenFirstOpButton(ps,1,10); //�ײ㴦��			
			break;
		}
		case 1:
	
		 if(ps->Button==0x0Afe||ps->Button==0x0Aff) //�˻���һ��
			{
			  ps->Key.Mode=0;
				ps->RunModeStat=1;//ˢ������
			}//ȡ��ֵ
			if(ps->Key.number>=0x30&&ps->Key.number<=0x39)
			{
			  ps->Key.Buffer[ps->Key.TwoOpt]=ps->Key.number;
			}
			break;
		}
		
	 { //��Ļ��������
				switch(ps->Button)
			{
				case 0x0A00:   //����
					{
						ps->Key.Opt=ps->Key.OldOpt;
						ps->Key.LastOpt=0;
						ps->RunMode =52; //������ҳ��
						ps->RunModeStat=1;//���¾�̬����
							break;
					 }
				case 0x0A02:    //����
				{
								
					break;
				}
				case 0x0A0C:    //ʱ��
				{
				
					break;
				}
				case 0x0A04:    //����ѹ			
				{
				
					break;
				}
				case 0x0A06:    //����	
				{
					
					break;
				}	
		}
	} 
	ps->Button=0xffff;
  ps->Key.number=0xff;
}
//---
//����ı������
Screen_ResetDataBuffer();
sprintf((char*)p->Buffer,"%02d:%02d:%02d",p->Clock.Clock.tm_hour,p->Clock.Clock.tm_min,p->Clock.Clock.tm_sec);
Screen_RefreshDataMulti(0x0120,p->Buffer,15*2);//��
				