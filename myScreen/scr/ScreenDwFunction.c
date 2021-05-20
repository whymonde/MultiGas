/*
  ���ļ���Ҫ���ڴ�����Ļ��Ҫ�ظ����õĺ���
*/

#include "ScreenDwFunction.h"
#include "ScreenDisCalculate.h"
#include "button.h"
#include "screenButton.h"

/*
	��������GetU16dataFormFlash2Word(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP)     
	������	ȥȡ4�ֽ����ݣ���Ҫ���ü��ٴ���������
*/
void GetU16dataFormFlash2Word(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP)               
{
	four_p->U16Data[0] = fP->Data[addr];
	four_p->U16Data[1] = fP->Data[addr+1];
	
}
/*
	float GetFlaotdataFormFlash2Word(u32 addr)     
	������	ȥȡ4�ֽڸ������ݣ���Ҫ���ü��ٴ���������
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
	������	ȥȡ4�ֽڸ������ݣ���Ҫ���ü��ٴ���������
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
	�������Svoid SaveFlaotToFlash(float data,u32 addr) 
	������	���渡�����ݵ�FLASE���棨��Ҫ���ü��ٴ���������
*/
void SaveFlaotToFlash(float data,u32 addr)               
{
		FourBytes four;
		four.FloatData=data;
		Flashisp_DataStructure* pf=Flashisp_GetData();
		pf->Data[addr]	=four.U16Data[0]  ;
		pf->Data[addr+1]	=four.U16Data[1]  ;
		pf->Flag=1;//��������
	
}
/*
	�������SSaveU32ToFlash(u32 data,u32 addr) 
	������	����U32���ݵ�FLASE���棨��Ҫ���ü��ٴ���������
*/
void SaveU32ToFlash(u32 data,u32 addr)               
{
		FourBytes four;
		four.U32Data=data;
		Flashisp_DataStructure* pf=Flashisp_GetData();
		pf->Data[addr]	=four.U16Data[0]  ;
		pf->Data[addr+1]	=four.U16Data[1]  ;
		pf->Flag=1;//��������
	
}
/*
	�������SSaveU16ToFlash(u32 data,u32 addr) 
	������	����U16���ݵ�FLASE���棨��Ҫ���ü��ٴ���������
*/
void SaveU16ToFlash(u16 data,u32 addr)               
{
		Flashisp_DataStructure* pf=Flashisp_GetData();
		pf->Data[addr]	=data;
		pf->Flag=1;//��������
	
}
//---
/*
	�������void UpdataSensorFixDate(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP,Screen_DataStructure *p)               
	������	���´�������װʱ�䣨��Ҫ���ü��ٴ���������
  type:   0~7 ����Ĵ���������
*/
void UpdataSensorFixDate(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP,Screen_DataStructure *p,u8 type)               
{
		            GetU16dataFormFlash2Word(four_p,addr,fP);
								Screen_ResetDataBuffer();
								if(((p->LastChoice>>type)&0x01)==0)
								{
								  sprintf((char*)p->Buffer,"δ��װ");
							
								}else
								{
								  sprintf((char*)p->Buffer,"%02d-%02d-%02d",four_p->U16Data[1],(four_p->U16Data[0]>>8),(four_p->U16Data[0]&0xff));
								}
								Screen_RefreshDataMulti(type*0x20+0x0420,p->Buffer,12*2);
		
}
//---
/*
	�������� StrinGalignment(* str,u8 len)
	������	�����ַ���������̶�����
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
	�������� void ScrrenSetJitu(Screen_DataStructure *p)
	������	�������ý��洦���ͼ
  tpye:   ��ͼ������ 0-7
*/
void ScreenSetJitu(Screen_DataStructure *p,u8 type)
{
     switch(type)
		 {
			 case 0:
				 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&(1<<0))==0) //����-����ѹ
			{
			    Dis_CheckBox(0x0A20,1,(253<<16)+108);
			}else
			{
			    Dis_CheckBox(0x0A20,1,(353<<16)+108);
			}break;
			 case 9:
			 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&(1<<4))==0) //����-��������
			{
			    Dis_CheckBox(0x0AE0,1,(250<<16)+305);
			}
			else
			{
			   Dis_CheckBox(0x0AE0,1,(378<<16)+305);
			}
			break;
			 case 2:	
				 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&(1<<1))==0)   //����-������
			{
			   Dis_CheckBox(0x0A60,0,(253<<16)+159);
			}
			else
			{
			  Dis_CheckBox(0x0A60,1,(253<<16)+159);
			}break;
			 case 3:				
				 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&(1<<2))==0) //����-��������
			{
			   Dis_CheckBox(0x0A80,0,(648<<16)+159);
			}
			else
			{
			  Dis_CheckBox(0x0A80,1,(648<<16)+159);
			}
			break;
			 case 4:
				 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&(1<<3))==0) //����-�Զ���ˮ����
			{
			 Dis_CheckBox(0x0AA0,0,(252<<16)+250);
			}
			else
			{
			  Dis_CheckBox(0x0AA0,1,(252<<16)+250);
			}
			case 5:    //���û�ͼ�л�
			{
				if(p->Key.LastOpt!=p->Key.Opt)
				{
					p->Key.LastOpt=p->Key.Opt;
				  switch(p->Key.Opt)
					{
						case 1: Screen_PastePicture(0x0800,0x000B,(55<<16)+50,(230<<16)+95,(55<<16)+50);break; //ʱ��
						case 2: Screen_PastePicture(0x0800,0x000B,(55<<16)+98,(230<<16)+144,(55<<16)+98);break; //����
						case 3: Screen_PastePicture(0x0800,0x000B,(55<<16)+147,(230<<16)+192,(55<<16)+147);break; //������
						case 4: Screen_PastePicture(0x0800,0x000B,(448<<16)+147,(621<<16)+190,(448<<16)+147);break;//��������
						case 5: Screen_PastePicture(0x0800,0x000B,(55<<16)+196,(230<<16)+240,(55<<16)+196);break;//��ַ
						case 6: Screen_PastePicture(0x0800,0x000B,(55<<16)+243,(230<<16)+286,(55<<16)+243);break;//�Զ���ˮ����
						case 7: Screen_PastePicture(0x0800,0x000B,(328<<16)+245,(427<<16)+287,(328<<16)+245);break;//��ˮ���
						case 8: Screen_PastePicture(0x0800,0x000B,(558<<16)+245,(655<<16)+287,(558<<16)+245);break;//��ˮʱ��
						case 9: Screen_PastePicture(0x0800,0x000B,(55<<16)+291,(230<<16)+335,(55<<16)+291);break;//��������
						case 10: Screen_PastePicture(0x0800,0x000B,(469<<16)+291,(618<<16)+335,(469<<16)+291);break;//�⺬ʪ��
						case 11: Screen_PastePicture(0x0800,0x000B,(55<<16)+339,(230<<16)+380,(55<<16)+339);break;//�̳���������
						case 12: Screen_PastePicture(0x0800,0x000B,(719<<16)+392,(795<<16)+456,(719<<16)+392);break;//����
						default : break;

					}
				}
				      
			break;
			}
//				 case 6:
//				 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&(1<<5))==0) //����-������λ
//			{
//			 Dis_CheckBox(0x0AC0,1,(224<<16)+138);
//			}
//			else
//			{
//			  Dis_CheckBox(0x0AC0,1,(378<<16)+138);
//			}
//			break;
	   		 case 7:
				 if((Flashisp_GetData()->Data[FLASHIS_AdrXTSZBOX]&(1<<6))==0) //����-�̳���������
			{
			 Dis_CheckBox(0x0B20,1,(251<<16)+350);
			}
			else
			{
			  Dis_CheckBox(0x0B20,1,(490<<16)+350);
			}	
//			case 8:    //�������û�ͼ�л�
//			{
//				if(p->Key.LastOpt!=p->Key.Opt)
//				{
//					p->Key.LastOpt=p->Key.Opt;
//				  switch(p->Key.Opt)
//					{
//						case 1: Screen_PastePicture(0x0800,139,(42<<16)+70,(210<<16)+117,(42<<16)+70);    break; //����
//						case 2: Screen_PastePicture(0x0800,139,(42<<16)+120,(210<<16)+167,(42<<16)+120);  break; //��λ
//						case 3: Screen_PastePicture(0x0800,139,(42<<16)+170,(210<<16)+217,(42<<16)+170);  break; //��������
//						case 4: Screen_PastePicture(0x0800,139,(42<<16)+220,(210<<16)+267,(42<<16)+220);  break; //��ʪ��
//						case 5: Screen_PastePicture(0x0800,139,(42<<16)+270,(210<<16)+317,(42<<16)+270);  break;//�̳�
//						case 6: Screen_PastePicture(0x0800,139,(600<<16)+370,(710<<16)+440,(600<<16)+370);break;//��ҳ
//						case 7: Screen_PastePicture(0x0800,139,(720<<16)+370,(790<<16)+440,(720<<16)+370);break;//����	
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
	��������void ScreenFirstOpButton(Screen_DataStructure *ps,u8 OpMax,u8 isNumberChose)
	������	 һ���˵���������
  OpMax:   һ�������˵����ֵ
  ButtonAddr��  ��Ӧ���水����ʼֵ
  isNumberChose�� �Ƿ��Ӧ��ݼ�����
*/
void ScreenFirstOpButton(Screen_DataStructure *ps,u8 OpMax,u8 isNumberChose)
{
	    ps->Key.bOutsideKeyboard = false;
		switch(Button_GetData()->KeyData)  //�ȴ�����Χ����
			{
				case BUTTON_LEFT:    //��
				if(ps->RunMode==0x8111)   
				    {
						if(ps->Key.Opt>2&&ps->Key.Opt<13)
							ps->Key.Opt+=10;
						else if(ps->Key.Opt>12&&ps->Key.Opt<23)
							ps->Key.Opt-=10;break;
					}
				case BUTTON_UP: //��
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
				case BUTTON_RIGHT: //��
				if(ps->RunMode==0x8111)
				{
					if(ps->Key.Opt>2&&ps->Key.Opt<13)
						ps->Key.Opt+=10;
					else if(ps->Key.Opt>12&&ps->Key.Opt<23)
						ps->Key.Opt-=10;break;
				}
				case BUTTON_DOWN: //��
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
				case BUTTON_BACK: //����
				{
					ps->Button=0x0AFe;
					break;
				}
				case BUTTON_OK:   //��һ�ΰ�OK
				{
					 ps->Key.bOutsideKeyboard = true;
					 if(ps->Key.Opt<=OpMax&&ps->Key.Opt>0) 
						{
							if(ps->RunMode==0x1000) //���ý�����ֵ��ݼ�
							{
							  if(ps->Key.Opt>=2)
								{
								  ps->Button=ps->Key.Opt+1;   //����BUTTUNKEY
								}else
								{
								 ps->Button=ps->Key.Opt;
								}
							}
//							else 	if(ps->RunMode==0x1100) //���ý�����ֵ��ݼ�
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
							else if(ps->RunMode==0x6000) //���ý�����ֵ��ݼ�
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
							else 	if(ps->RunMode==0x8210) //���崫����ѡ�� 
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
				if(isNumberChose==1) //�˵����ֿ�ݼ�
				{
					if(ps->Key.KeyData<=OpMax)  //����ֵ<=��Ļ��������
					{
						ps->Key.bOutsideKeyboard = true;
					}
					
					if(ps->RunMode==0x1000) //���ý�����ֵ��ݼ�
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
					else 	if(ps->RunMode==0x1100) //���ý�����ֵ��ݼ�
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
					else 	if(ps->RunMode==0x6000) //���ý�����ֵ��ݼ�
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
					else 	if(ps->RunMode==0x6200) //���ý�����ֵ��ݼ�
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
					else 	if(ps->RunMode==0x8120) //����У׼
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
					else if(ps->RunMode==0x8130) //��������
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
					else if(ps->RunMode==0x8140) //Ũ��У׼
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
					else if(ps->RunMode==0x8150) //ѹ��У׼
					{
					  if(ps->Key.KeyData<=0x06)
						{ 
								ps->Button=ps->Key.KeyData;
						}else if(ps->Key.KeyData>=0x0a&&ps->Key.KeyData<=0x0b)	
						{
						  ps->Button=ps->Key.KeyData-3;
						}
					}
					else 	if(ps->RunMode==0x8170) //�¶�У׼
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
					else 	if(ps->RunMode==0x8210) //���崫����ѡ�� 
					{
					  if(ps->Key.KeyData<=7)
						{ 
						   ps->Button=ps->Key.KeyData;  //  ps->Key.KeyData*2    -2020-09-03
						}
					}
					else if(ps->RunMode==101)		//2020-05-20 hjl add����������洦��
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
	�������� void ScreenTowOpButton(Screen_DataStructure *ps,u8 type,u8 OpMax,u8 isOpChang)
	������	�����˵���������
  tpye:   ���������˵�����
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
	�������� void ScreenTowOpButton(Screen_DataStructure *ps,u8 type,u8 OpMax,u8 isOpChang)
	������	�����˵�����������������ַ���ӳ���
    tpye:   ���������˵�����
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
	�������� void ScreenTowOpButton(Screen_DataStructure *ps,u8 type,u8 OpMax,u8 isOpChang)
	������	�����˵���������
  tpye:   ���������˵�����
*/
void ScreenTowOpButton(Screen_DataStructure *ps,u8 type,u8 OpMax,u8 isOpChang)
{

	   	if(isOpChang)  //�������Ҽ���Ч����λ��
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
	�������� void ScreenTowOptChose(Screen_DataStructure *ps,u8 opt)
	������	�����˵�����ѡ��Ԥ�ȴ���
  tpye:   ���������˵�����
*/
void ScreenTowOptChose(Screen_DataStructure *ps,u8 opt)
{	
	
	          Screen_ResetKeyDataBuffer();//�������ֵ
              ps->Key.Opt=opt;
			  ps->Key.Mode=opt;
			  ps->Key.TwoOpt=0;
	          ps->RunModeStat=1;//20200310
	          ps->Timer=0;
	          
}
//-----
/*
	�������� ScreenBackLastPag(Screen_DataStructure *ps,u16 lastMode)
	������	�����ϼ�����
*/
void ScreenBackLastPag(Screen_DataStructure *ps,u16 lastMode)
{
  	                    ps->Key.Opt=ps->Key.OldOpt;
						ps->Key.LastOpt=0;
						ps->RunMode =lastMode; //������ҳ��
						ps->RunModeStat=1;//���¾�̬����
	                    ps->Key.Mode=0;//����ѡ����0
}



// 2020-05-25 modify by alfred.
/*
	�������� ScreenBackLastPag(Screen_DataStructure *ps)
	������	������һ���˵�
*/
void ScreenBackLastOpt(Screen_DataStructure *ps)
{
						ps->Key.Mode=0;     //ȷ�Ϻ�����һ������
						ps->Key.LastOpt=0;   ////ȷ�Ϻ�����һ������
						ps->RunModeStat=1;  //���¾�̬����, ������ȷ�ϻ򷵻�ʱps->RunModeStat=1����ҳ�����ݡ� 
						ps->Button=0XffFF;
						ps->uEnableRecDataTimer = 0;
						ps->sReveiveDataTimer = 0;			//�˳������˵�ʱ�رն�ʱ��

}
/*
	�������� ScreenBackLastPag(Screen_DataStructure *ps)
	������	����һ���˵�
*/
void ScreenBackFirstOpt(Screen_DataStructure *ps)
{
	if(ps->Button==0x0Aff||ps->Button==0x0Afe)
	{     
		ps->Key.Mode=0;     //ȷ�Ϻ�����һ������
		ps->Key.LastOpt=0;   ////ȷ�Ϻ�����һ������
		ps->RunModeStat=1;  //���¾�̬����, ������ȷ�ϻ򷵻�ʱps->RunModeStat=1����ҳ�����ݡ� 
		ps->Button=0XffFF;
	}
}
//---
/*
	��������void ScreenConfirmEnter(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min)
	������	������ֵȷ��  
  �洢����Ϊ16λ��32λ������
  //type 1 U16  2  float  3 u32
*/
void ScreenConfirmEnter(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min)
{
	     Flashisp_DataStructure* pf=Flashisp_GetData();
	     
  	     if(type==2)
				 {				 
					FourBytes four;
					four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));
					if(four.FloatData<Min) four.FloatData=Min; //���� ����50Kp
					if(four.FloatData>Max) four.FloatData=Max; //���� С����130Kp
					pf->Data[Addr]=four.U16Data[0]; //����ֵ
					pf->Data[Addr+1]=four.U16Data[1];
					
				 } if(type==3)
				 {				 
					FourBytes four;
					four.U32Data = (u32)atoi((char*)&(ps->Key.Buffer[0]));	//2020-05-19 hjl �޸�ǿ��ת������Ϊu32 
					if(four.U32Data<Min) four.U32Data=Min; //���� ����50Kp
					if(four.U32Data>Max) four.U32Data=Max; //���� С����130Kp
					pf->Data[Addr]=four.U16Data[0]; //����ֵ
					pf->Data[Addr+1]=four.U16Data[1];
					
				 }else   if(type==1)
				 {
					u16 Datai=atoi((char*)&(ps->Key.Buffer[0]));
					if(Datai<Min) Datai=Min; //���� ����50Kp
					if(Datai>Max) Datai=Max; //���� С����130Kp
					pf->Data[Addr]=Datai; //����ֵ
				 }
				 pf->Flag=1;
}
//---
/*
	��������void ScreenConfirmEnterDisBuffer(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min)
	������	������ֵȷ��----ȷ����ʾ���� 
  �洢����Ϊfloat ����
  //position �����ݵ�λ��
*/
void ScreenConfirmEnterDisBuffer(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min)
{
					DisBuffer[position] = (float)atof((char*)&(ps->Key.Buffer[0]));
					if(DisBuffer[position]<Min) DisBuffer[position]=Min; //���� ����
					if(DisBuffer[position]>Max) DisBuffer[position]=Max; //���� С����

}
//---
/*
	��������void ScreenEnterNumberText(Screen_DataStructure *ps,u8 type)
	������	��ֵ  
  ��ֵ���ͣ�type 0  �޷�������
            type 1  �з�������
            type 2  �޷��Ÿ�����
            type 3  �з��Ÿ�����
  �ı����ȣ�len
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
					ps->Key.TwoOpt=len;//�������볤��
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
						ps->Key.TwoOpt=len;//�������볤��
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
				ps->Key.TwoOpt=len;//�������볤��
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
						ps->Key.TwoOpt=len;//�������볤��
						ps->Key.Buffer[ps->Key.TwoOpt]=0x00;
					}
				}
			}
		}
	}
  	if(ps->Button==0x0AfC) //ɾ��
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

//��ʱ�����ã�����淶ʱɾ����
void ScreenTextTwinkle(Screen_DataStructure *ps,u16 TextAddr, u16 FlashAddress, u8 valType)
{
    
			if(ps->Key.State == 0)	//State����������˸
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
	��������void ScreenEnterNumberTwinkle(Screen_DataStructure *ps,u16 TextAddr)
	������	��ֵ������˸  

*/
void ScreenEnterNumberTwinkle(Screen_DataStructure *ps,u16 TextAddr)
{
    
			strcpy((char*)ps->Buffer,(char*)ps->Key.Buffer);
			if(ps->Key.State == 0)	//State����������˸
			{
				if(((ps->RunMode==0x1000)&&((ps->Key.Mode==1)||(ps->Key.Mode==2)))||((ps->RunMode==0x8210)&&(ps->Key.Mode<=7)))  //����ĳЩ����ҳ��
					;
				else
					ps->Buffer[ps->Key.TwoOpt] = '|';
				  ps->Key.State = 1;
			}
			else {ps->Key.State = 0;
					ps->Buffer[ps->Key.TwoOpt] = ' ';
			}
			Screen_RefreshDataMulti(TextAddr,ps->Buffer,15*2);//�ϴ����������ݵ�������
}
//-----
/*20200521
	��������void ScreenChangeDataInit(Screen_DataStructure *ps)
	������	ҳ���л����ݳ�ʼ��

*/
void ScreenChangeDataInit(Screen_DataStructure *ps)
{	
	
	if(ps->OldMode!=ps->RunMode)
	{
		ps->Key.OldOpt=ps->Key.Opt;
		ps->Key.LastOpt=0;
		ps->RunModeStat=1;//���¾�̬����	
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
		if((ps->RunMode!=0x8141)&&(ps->RunMode!=0x8140))   //����ҳ�洦��
		ps->PauseOrStop=0;	
		ps->StarCun=1;		
		ps->LoadFlag=1;				
	}
	}
//---
/*
	��������void ScreenButtonReset(Screen_DataStructure *ps)
	������	����������λ

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
	������	�����̵���������
*/
u8* GetYDBD_Describe(u8 Type)
{	
	 	Screen_DataStructure *ps = Screen_GetData();
   Flashisp_DataStructure* pf=Flashisp_GetData();
	 if(Type==1)
	 {
	    sprintf((char*)ps->Buffer,"Բ���̵�");
	 }
	 else if(Type==2)
	 {
	    sprintf((char*)ps->Buffer,"�����̵�");
	 }
	 else if(Type==3)
	 {
	    sprintf((char*)ps->Buffer,"�������̵�");
	 } else 
	 {
	    sprintf((char*)ps->Buffer,"δ֪�̵�");
	 }
	 return ps->Buffer;
}
//---
/*
	��������void Dis_Tips(u8* Type)
	������	��ʾ��ʾ
*/
void Dis_Tips(Screen_DataStructure *p,u8* Tips,u16 TextAddr)
{	
	
  sprintf((char*)p->Buffer,"%s",Tips);
  Screen_RefreshDataMulti(TextAddr,p->Buffer,15*2);
}
//---

/*
	��������u16 Dis_CheckBox(u16 addr,u8 Stat,u32 DisXY)
	������	��ѡ
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
	��������u16 Set_SysCheckBox(u16 addr,u16 ByteNumber)
	������	����-ϵͳ���ù�ѡ��/��ӡѡ��ѡ������
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
	������	��ȡ�̵����
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
	������	��ȡ�̵����
*/
void DisQTND (u32 addr,float *DisBuffer,u8 len)
{	
	 Flashisp_DataStructure* pf=Flashisp_GetData();
}

//---

/*
	float YC_SampleControl (void)
	������	�̳��������ƴ���  ��Screen_SecTimer() �е���
*/
void YC_SampleControl(void)
{	 u32 cal;
	 float buf;
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 Screen_DataStructure *ps = Screen_GetData();
	 if(Flux_GetData()->ChannelB.RunMode==2) //�̳�������
	 {    
		   cal=Flux_GetData()->ChannelB.Sampling.DinSiTime-Flux_GetData()->ChannelB.Sampling.IntTime;
			if(cal<10&&(cal%2))//����С��10s �������뱨��һ��
			{
			  Speak_Buzzer(200);//��500ms
			}
			if(pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]==1&&(cal%10==0)) //���ٸ���
				{
					 //to do ���ٸ���
					//Flux_ContinueSampling('B',	Flux_GetData()->ChannelA.ShowFlux);
					buf=CalculationFormula_Qrs(GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ),CalculationFormula_GetData()->Vs,GetFlaotdataFormFlash2Word(FLASHIS_AdrCHSL_HSL)/100);//���ݲ���ֱ����ʵ�����٣���ʪ��
					Flux_ContinueSampling('B',buf);
					ps->YC_TrageFlux=buf;
					if(ps->YC_TrageFlux<5) ps->YC_TrageFlux=5; //���ƹ�С
				} 
			else if(pf->Data[FLASHIS_AdrYCCJ_CYSE_CYFS]==0)
				{
				   ps->YC_TrageFlux=GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CYLS);//Ŀ������
				}
			  ps->YC_FollowRate= (ps->YC_FollowRate+Flux_GetData()->ChannelB.ShowFlux/ps->YC_TrageFlux)/2;	

	 }
}


/*
	��������void ScreenConfirmEnter(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min)
	������	�����������ֵȷ��  
  �洢����Ϊ16λ��32λ������
  //type 1 U16  2  float  3 u32
*/
void SoftKeyboardConfirmEnter(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min)
{
	     Flashisp_DataStructure* pf=Flashisp_GetData();
	     
				Screen_ResetKeyDataBuffer();//�������ֵ
				strcpy((char*)ps->Key.Buffer,(char*)RcvBuffer_GetData()); 
	
  	     if(type==2)
				 {				 
					FourBytes four;
					four.FloatData = (float)atof((char*)&(ps->Key.Buffer[0]));
					if(four.FloatData<Min) four.FloatData=Min; //���� ����50Kp
					if(four.FloatData>Max) four.FloatData=Max; //���� С����130Kp
					pf->Data[Addr]=four.U16Data[0]; //����ֵ
					pf->Data[Addr+1]=four.U16Data[1];
					
				 } if(type==3)
				 {				 
					FourBytes four;
					four.U32Data = (float)atoi((char*)&(ps->Key.Buffer[0]));
					if(four.U32Data<Min) four.U32Data=Min; //���� ����50Kp
					if(four.U32Data>Max) four.U32Data=Max; //���� С����130Kp
					pf->Data[Addr]=four.U16Data[0]; //����ֵ
					pf->Data[Addr+1]=four.U16Data[1];
					
				 }else   if(type==1)
				 {
					u16 Datai=atoi((char*)&(ps->Key.Buffer[0]));
					if(Datai<Min) Datai=Min; //���� ����50Kp
					if(Datai>Max) Datai=Max; //���� С����130Kp
					pf->Data[Addr]=Datai; //����ֵ
				 }
				 
				 
				 pf->Flag=1;
				 ps->sRcvType = 0;
				 ps->Key.LastMode= ps->Key.Mode;
				 
				 ScreenBackLastOpt(ps);

}

/*
	��������ProcessSoftKeyboardInput(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min,u8 isHavePoint,u8 isHaveSign)
	������	����������� 
  �洢����Ϊ16λ��32λ������
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
		ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
		ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������	
	}				
}


/*
	��������void SoftKeyboardConfirmEnterDisBuffer(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min)
	������	�����������ֵȷ��----ȷ����ʾ���� 
  �洢����Ϊfloat ����
  //position �����ݵ�λ��
*/
void SoftKeyboardConfirmEnterDisBuffer(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min)
{

					Screen_ResetKeyDataBuffer();//�������ֵ
					strcpy((char*)ps->Key.Buffer,(char*)RcvBuffer_GetData()); 
  
					DisBuffer[position] = (float)atof((char*)&(ps->Key.Buffer[0]));
					if(DisBuffer[position]<Min) DisBuffer[position]=Min; //���� ����
					if(DisBuffer[position]>Max) DisBuffer[position]=Max; //���� С����

					ps->sRcvType = 0;
//					ScreenBackLastOpt(ps);
}

/*
	��������ProcessSoftKeyboardInput(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min,u8 isHavePoint,u8 isHaveSign)
	������	�����������(�����ļ���ѯʱ�ļ��ŵ�¼�룩 
  �洢����Ϊ16λ��32λ������
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
		ps->uEnableRecDataTimer = 1; //�������OK��,������ʱ��	
		ps->sReveiveDataTimer = TIM_OVERTIME_CHECK;//��ʱ����鳬ʱʱ������					
	}				
}

//---



			

