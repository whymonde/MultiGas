/*
  ���ļ���Ҫ���ڴ�����Ļ��Ҫ��ʾ���ݵ���ؼ���
	�������ļ��������߼�
*/
#include <stdio.h>
#include <stdlib.h>
#include "ScreenDisCalculate.h"
#include "CalculationFormula.h"
#include "ScreenDwFunction.h"


float gMinuteData[60][8];	//��������,������ʾ�ݴ�
u8 gSystemTime[60][10];		//��������ʱ��

BLUETOOTH_INIT_STATE_MACHINE g_BlueToothInitStateMachine;


/*
  Բ���̵�������ϵ��
*/
 const float CircularMeasurePoint[6][10]=
 {
		{0.5,0,0,0,0,0,0,0,0,0},
		{0.146,0.854,0,0,0,0,0,0,0,0},
		{0.067,0.25,0.75,0.933,0,0,0,0,0,0},
		{0.044,0.146,0.296,0.704,0.854,0.956,0,0,0,0},
		{0.033,0.105,0.194,0.323,0.677,0.806,0.895,0.967,0,0},
		{0.026,0.082,0.146,0.226,0.342,0.658,0.774,0.854,0.918,0.974},	
 };
/* date 2020-02-28
	��������void CalculationCircularMeasurePoint(u8 flag,float d,float L,u16 *h,u16 *SingleNumber,float *MeasurePoint)
	������	Բ�β����㹫ʽ
	flag�Ƿ�ͨ������õ����� ����ֱ��������L  
	�������H �� SingleNumbe���������� �������MeasurePoint 
	�޸ļ�¼�� 1. �޸���Բ�λ������ò�����������, modify by alfred, 2020.07.03
*/
void CalculationCircularMeasurePoint(u8 flag,float d,float L,u16 *h,u16 *SingleNumber,float *MeasurePoint)
{
	/*���㻷��*/
	if(flag!=0) //��Ҫ����
	{
		/*
		if(d<0.3f) *h=0;    //
		else if(d<0.6f)     //h=1~2
		{
	        if(*h<1)*h=1;
			else if(*h>2)*h=2;
		}
		else if(d<1.0f)    //h=2~3
		{
			if(*h<2)*h=2;
			else if(*h>3)*h=3;
		}
		else if(d<2.0f)   //h=3~4
		{
			if(*h<3)*h=3;
			else if(*h>4)*h=4;
		}
		else if(d<4.0f)  //h=4~5
		{
			if(*h<4)*h=4;
			else if(*h>5)*h=5;
		}
		else
		*h=5;
		*/
		if(d<0.3f) *h=0;    //
		else if(d<0.6f) *h=1;
		else if(d<1.0f) *h=2;
		else if(d<2.0f) *h=3;
		else if(d<4.0f) *h=4;
		else 
		*h=5;
	}


	
	/*���ݻ���������*/
	
	if(*h!=0)
	{
		*SingleNumber=2*(*h);
	}
	for(int i=0;i<*SingleNumber;i++)
	 {
	   MeasurePoint[i]=CircularMeasurePoint[*h][i]*d+L;				
	 }
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 FourBytes four;
	for(int i=0;i<10;i++)
	{
			four.FloatData=MeasurePoint[i];
			pf->Data[FLASHIS_AdrYDBDCDJL1+i*2]=four.U16Data[0];
			pf->Data[FLASHIS_AdrYDBDCDJL1+1+i*2]=four.U16Data[1];
			
	}
	 if(*h!=0)
	 {
	   *SingleNumber=2*(*h);
	 }
}
/* date 2020-02-28
	��������void SquareCalculationMeasurePoint(float d,int L,u8 *h,float *MeasurePoint)
	������	���β����㹫ʽ 
     1.A/����=С�����α߳�
      

  flag�Ƿ�ͨ������õ����׵��� ����B�߳��ȣ�����L  
  ������߲����X �Ͳ����MeasurePoint ����
*/

/*
void SquareCalculationMeasurePoint(u8 flag,float B,float L,u16 *x,float *MeasurePoint)
{
	//���㵥�ײ����
	if(flag!=0){
   if(B<1.5f) *x=1;
	 else if(B<2.5f) *x=2;
	 else if(B<3.5f) *x=3;
	 else if(B<4.5f) *x=4;
	 else if(B<5.5f) *x=5;
	 else if(B<6.5f) *x=6;
	 else if(B<7.5f) *x=7;
	 else if(B<8.5f) *x=8;
	 else if(B<9.5f) *x=9;
	 else *x=10;
	}
	//���ݵ��ײ����������
	for(int i=0;i<*x;i++)
	 { 
		 int N=i+1;
	   MeasurePoint[i]=(2*N-1)*(B/(2*(*x)))+L;
	 }
	  
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 FourBytes four;
	for(int i=0;i<10;i++)
	{
			four.FloatData=MeasurePoint[i];
			pf->Data[FLASHIS_AdrYDBDCDJL1+i*2]=four.U16Data[0];
			pf->Data[FLASHIS_AdrYDBDCDJL1+1+i*2]=four.U16Data[1];
			
	}
}
*/
void SquareCalculationMeasurePoint(u8 Ks,float B,float L,float A,float *MeasurePoint)
{ 
	float buf1,buf2;
	u16 N1;

	if (Ks==0)
		Ks=1;
	buf1=A/Ks;  //�õ�ÿС��߳�
	buf2=B/buf1;// �ɱ߳�B�õ���С�ֱ߳�ΪA/Ks ��С������
	N1=(u16)(buf2+0.5f);  //
	if(N1<1)N1=1;  //��Сһ����
	
	for(int i=0;i<N1;i++)
	 { 
	   MeasurePoint[i]=(2*i+1)*(B/(2*N1))+L;
	 }
	  
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 FourBytes four;
	for(int i=0;i<10;i++)
	{
			four.FloatData=MeasurePoint[i];
			pf->Data[FLASHIS_AdrYDBDCDJL1+i*2]=four.U16Data[0];
			pf->Data[FLASHIS_AdrYDBDCDJL1+1+i*2]=four.U16Data[1];
			
	}

}
/* date 2021-01-27
	��������u16 SquareCalculationMeasureHold(float A)
	������	��������������򵥵�����
            ���ؿ���
*/

u16 SquareCalculationMeasureHold(float MJ,float A,float B)
{  // u16 buf;
	if(MJ<0.1f)    //���С��0.1
	{
        return 0x0101;
	}
	else if(MJ<0.5f)         // 2*2
	{
		return 0x0202;
	}
	else if(MJ<1.0f)
	{
		if(A>B)
		return 0x0302;
		else 
		return 0x0203;
		
	}
	else if(MJ<4.0f)
	{
		return 0x0303;
	}
	else if(MJ<=9.0f)
	{
		return 0x0404;
	}
	else
	{
		if(A>B)
		return 0x0504;
		else 
		return 0x0405;
	}
	
	
	
	
	
	
	
	
	
	/*
	
	if(MJ<0.1f)
		buf= (u16)((A/0.32)+0.67);
	else if(MJ<0.5f)
		buf= (u16)((A/0.35)+0.67);
	else if(MJ<1.0f)
		buf= (u16)((A/0.50)+0.67);
	else if(MJ<4.0f)
		buf= (u16)((A/0.67)+0.67);
	else if(MJ<=9.0f)
		buf= (u16)((A/0.75)+0.67);
	else
	   buf= (u16)((A/1.0f)+0.67);
	
	if(buf==0)
	buf=1;
	return buf;
	*/
}


/* date 2020-02-28
	��������SquareCalculationMeasurePointN(u16 *N,u8 Ks,float B,float L,float A,float *MeasurePoint)
	������	���β����㹫ʽ 
     1.*N Ϊ���׼�����ָ��
     2.Ks ����
     3.B �߳�
     4.L�׹ܳ�
     5.A �߳�
     6.MeasurePoint�����ַָ��
  flag�Ƿ�ͨ������õ����׵��� ����B�߳��ȣ�����L  
  ������߲����X �Ͳ����MeasurePoint ����
*/
void SquareCalculationMeasurePointN(u16 *N,u8 Ks,float B,float L,float A,float *MeasurePoint)
{ 
	//float buf1,buf2;

	for(int i=0;i<(*N);i++)  //��ÿ�����
	 { 
	   MeasurePoint[i]=(2*i+1)*(B/(2*(*N)))+L;
	 }
	  
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 FourBytes four;
	for(int i=0;i<10;i++)
	{
	four.FloatData=MeasurePoint[i];
	pf->Data[FLASHIS_AdrYDBDCDJL1+i*2]=four.U16Data[0];
	pf->Data[FLASHIS_AdrYDBDCDJL1+1+i*2]=four.U16Data[1];
			
	}
}

/* date 2020-03-02
20200527
	��������void SaveFlueData(float *SaveData)
	������	�����̵�����
  ���룺SaveData[14] �ο�File.c �ļ�const char Flue_line��14����30��
*/
void SaveFlueData(float *SaveData)
{
   SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	u32 FileNumber=01;
	SdFile_Serch_MaxofFlueLinkFile();//�ȶ�ȡ����ļ���
	FileNumber=SdFp->FlueLinkFileMax+1;
	
	u8 buf[31]={0};
	//װ������ʱ��
	SdFile_ramlodair((char *)&SdFp->InterFace_data.dat[0],Interf_dat_length);  //�����ո�
    sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
    strncpy((char *)&SdFp->InterFace_data.dat[0],(char *)buf,Interf_dat_length);  
  //װ�ص�ַ
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Adress[0],Adress_length-9-2);  //�����ո�49+/r/n
	 sprintf((char *)&buf[0],"%-30s",(char *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));  //
	 strncpy((char *)&SdFp->InterFace_data.Adress[0],(char *)&buf[0],30);   //flash��ֻ��30����ַ����
	 printf("%-30s",(char *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));
	//���װ��
    SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[0],Interf_other_length);  //�����ո�
	sprintf((char*)SdFp->InterFace_data.Other[0],"     %-8.4f",SaveData[0]);
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[1],Interf_other_length);  //�����ո�
	if(SaveData[1]==1)
	{
	  sprintf((char*)SdFp->InterFace_data.Other[1],"  1.Բ���̵� ");
	}else  if(SaveData[1]==2)
	{
	  sprintf((char*)SdFp->InterFace_data.Other[1],"  2.�����̵� ");
	}else
	{
	   sprintf((char*)SdFp->InterFace_data.Other[1],"  3.�������̵� ");
	}
		//װ�ؿ���
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[2],Interf_other_length);  //�����ո�
	sprintf((char*)SdFp->InterFace_data.Other[2],"     %-8d",(int32_t)SaveData[2]);
		//װ�ز������
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[3],Interf_other_length);  //�����ո�
	sprintf((char*)SdFp->InterFace_data.Other[3],"     %-8d",(int32_t)SaveData[3]);
	for(int i=4;i<=14;i++)//������
	{
		SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[i],Interf_other_length);  //�����ո�
	  sprintf((char*)SdFp->InterFace_data.Other[i],"     %-8.3f",SaveData[i]);
	}
	//װ���ھ�D
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[15],Interf_other_length);  //�����ո�
	sprintf((char*)SdFp->InterFace_data.Other[15],"     %-8.2f",SaveData[15]);
	
	//װ���׹�L
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[16],Interf_other_length);  //�����ո�
	sprintf((char*)SdFp->InterFace_data.Other[16],"     %-8.2f",SaveData[16]);
	
	//����
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[17],Interf_other_length);  //�����ո�
	sprintf((char*)SdFp->InterFace_data.Other[17],"     %-8d",(int32_t)SaveData[17]);
	
	//��A
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[18],Interf_other_length);  //�����ո�
	sprintf((char*)SdFp->InterFace_data.Other[18],"     %-8.2f",SaveData[18]);
	
	//��B
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[19],Interf_other_length);  //�����ո�
	sprintf((char*)SdFp->InterFace_data.Other[19],"     %-8.2f",SaveData[19]);
	
	//���ײ��
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[20],Interf_other_length);  //�����ո�
	sprintf((char*)SdFp->InterFace_data.Other[20],"     %-8d",(int32_t)SaveData[20]);

	SdFile_WriteFlueFile(&SdFp->InterFace_data,FileNumber);//д���̵�����

}
/* date 2020-03-02
	��������ReadFlueData(u32 FileNum)
	������	��ȡ�̵�����
  ���룺�ļ���
*/
void ReadFlueData(u32 FileNum)//�����ļ��� ��ȡ�̵���Ϣ
{ s16 i;
  SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	SdFile_ReadFlueFile(FileNum);
	
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //�ļ���
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);
	strncpy((char *)ps->Buffer,(char *)&(SdFp->FlueFile.Other[0][13]),8);//���
	Screen_RefreshDataMulti(0x0120,ps->Buffer,15*2);
	
	strncpy((char *)ps->Buffer,(char *)&(SdFp->FlueFile.Other[1][11]),8);//����
	Screen_RefreshDataMulti(0x0140,ps->Buffer,15*2); //�̵�����
	strncpy((char *)ps->Buffer,(char *)&(SdFp->FlueFile.Other[2][13]),8);//����
	Screen_RefreshDataMulti(0x0160,ps->Buffer,15*2);
	strncpy((char *)ps->Buffer,(char *)&(SdFp->FlueFile.Other[3][13]),8);//�������
	Screen_RefreshDataMulti(0x0180,ps->Buffer,15*2);
	//��ʾ��ַ��Ϣ
	
		
	strncpy((char*)ps->Buffer,(char *)&(SdFp->FlueFile.Adress[9]),30);
	for(i=29;i>=0;i--)               //�ѿո�ת��Ϊ0
     {
		 if((ps->Buffer[i]==0x20)||(ps->Buffer[i]==00))
		 {
			 ps->Buffer[i]=00;
		 }
		 else
		 {
			 break;
		 }
	 }		 
				
	char uCursor[]={0xff,0xff,0x00}; //��������0XFFʹ���λ����ȷ��ʾ��
	strcat((char*)ps->Buffer, uCursor);
	Screen_RefreshDataMulti(0x01A0,ps->Buffer,15*2);
	
	//��ʾ������
	for(int i=0;i<10;i++)
	{
		 strncpy((char *)ps->Buffer,(char *)&(SdFp->FlueFile.Other[i+4][13]),8);//�������
		if(i<=1) 
		{
			Screen_RefreshDataMulti(0x01C0+(0x20)*(i),ps->Buffer,15*2);
		}
		else      
		{
			Screen_RefreshDataMulti(0x0400+(0x20)*(i-2),ps->Buffer,15*2);
		}
	}    

}

/* date 2020-03-02
20200527
	��������void ApplicationFlueFileData (void)
	������	�����ļ�����
  ���룺void
*/
void ApplicationFlueFileData (void)//���õ�ǰ����
{   s16 i;
    SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	char buff[61]={0};
	FourBytes four;
	strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[0][13]),8);//���
	four.FloatData=atof(buff);
	SaveFlaotToFlash(four.FloatData,FLASHIS_AdrYDBD_MJ);
  printf("%s,%.4f\r\n",buff,four.FloatData);
	//װ���̵�����
  if(SdFp->FlueFile.Other[1][11]=='1')
  {
	  pf->Data[FLASHIS_AdrYDBDLX]=1;
	
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[15][13]),8);//�ھ�
	  buff[8]=0;
	  four.FloatData=atof(buff);
	  SaveFlaotToFlash(four.FloatData,FLASHIS_AdrYDBDYXNJ);  
	  
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[16][13]),8);//�׹�
	  buff[8]=0;
	  four.FloatData=atof(buff);
	  SaveFlaotToFlash(four.FloatData,FLASHIS_AdrYDBD_GT);  //
	  
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[17][13]),8);//����
	  buff[8]=0;
	  four.U16Data[0]=(u16)atoi(buff);
	  SaveU16ToFlash(four.U16Data[0],FLASHIS_AdrYDBDYXHS);  //
	  
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[2][13]),8);//����
	  buff[8]=0;
	  four.U16Data[0]=(u16)atoi(buff);
	  SaveU16ToFlash(four.U16Data[0],FLASHIS_AdrYDBDYXKS);  //
	  
	  
  }
  else if(SdFp->FlueFile.Other[1][11]=='2') 
  {
	  pf->Data[FLASHIS_AdrYDBDLX]=2;
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[18][13]),8);//��A
	  buff[8]=0;
	  four.FloatData=atof(buff);
	  SaveFlaotToFlash(four.FloatData,FLASHIS_AdrYDBDFXBA);  
	  
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[19][13]),8);//��B
	  buff[8]=0;
	  four.FloatData=atof(buff);
	  SaveFlaotToFlash(four.FloatData,FLASHIS_AdrYDBDFXBB);  //  
	  
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[16][13]),8);//�׹�
	  buff[8]=0;
	  four.FloatData=atof(buff);
	  SaveFlaotToFlash(four.FloatData,FLASHIS_AdrYDBD_GT);  //

	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[2][13]),8);//����
	  buff[8]=0;
	  four.U16Data[0]=(u16)atoi(buff);
	  SaveU16ToFlash(four.U16Data[0],FLASHIS_AdrYDBDYXKS);  //
	  SaveU16ToFlash(four.U16Data[0],FLASHIS_AdrYDBDFXKS);  //
	  
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[20][13]),8);//������
	  buff[8]=0;
	  four.U16Data[0]=(u16)atoi(buff);
	  SaveU16ToFlash(four.U16Data[0],FLASHIS_AdrYDBDFXDKCD);  //

  }
	else if(SdFp->FlueFile.Other[1][11]=='3')
	{	
	  pf->Data[FLASHIS_AdrYDBDLX]=3;	
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[3][13]),8);//������
	  buff[8]=0;
	  four.U16Data[0]=(u16)atoi(buff);
	  SaveU16ToFlash(four.U16Data[0],FLASHIS_AdrYDBD_CYDS);  //

	}
	printf("%s,%.4f\r\n",&SdFp->FlueFile.Other[1][9],four.FloatData);

  for(int i=4;i<14;i++)
	{
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[i][13]),8);//��������
	  four.FloatData=atof(buff);
		SaveFlaotToFlash(four.FloatData,FLASHIS_AdrYDBDCDJL1+2*(i-4));
		printf("%s,%.3f\r\n",buff,four.FloatData);
	}
	Screen_RefreshDataMulti(0x0180,ps->Buffer,15*2);
	//��ʾ��ַ��Ϣ
	{
    strncpy((char *)buff,(char *)&(SdFp->FlueFile.Adress[9]),30);//��ַ
	for(i=29;i>=0;i--)
	{
		if((buff[i]==0x20)||(buff[i]==0x00))  //�ո�ת��0
        {
			buff[i]=0x00;
	    }
        else
	    {
			break;
	    }		
	}
		
	strncpy((char *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]),(char *)buff,30);
		
	printf("%s,%s\r\n",buff,(char *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));
	}				

  

}
//---
/* date 20200505
	��������ReadWorkData(u32 FileNum)
	������ ��ȡ�����ļ�
  ���룺
*/         
void ReadWorkData(u32 FileNum)//�����ļ��� ��ȡ������Ϣ
{
  SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	SdFile_ReadWorkFile(FileNum);
	
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //�ļ���
	Screen_RefreshDataMulti(0x0440,ps->Buffer,15*2);
	for(int i=0;i<2;i++)
	{
		
		strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[i][12]),10);//��ʾ����
		
		Screen_RefreshDataMulti(0x0120+i*0x20,ps->Buffer,15*2);
	}
	for(int i=3;i<11;i++)                                                       //i=10Ϊ��ʪ��
	{   
		strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[i][12]),10);//��ʾ����//
		Screen_RefreshDataMulti(0x0120+(i-1)*0x20,ps->Buffer,15*2);             //
	} 

	
	//��ʾʱ����Ϣ
	{		
		strncpy((char*)ps->Buffer,(char *)&(SdFp->rdWorkFile.dat[0]),16);
	}				
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);

}
//---
/* date 2020-03-10
	��������ReadFlueData(u32 FileNum)
	������	���湤���ļ�
  ���룺
*/

void SaveWorkData(float *SaveData,u32 NumFile)
 { 
	 SdFile_DataStruct* SdFp=SdFile_GetData();
   Screen_DataStructure* ps=Screen_GetData();
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 u8 buf[80]={0};
	 //װ������ʱ��
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.dat[0],Interf_dat_length);  //�����ո�
   sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
   strncpy((char *)&SdFp->InterFace_data.dat[0],(char *)&buf[0],Interf_dat_length);  
    //װ�ص�ַ
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Adress[0],Adress_length-9-2);  //�����ո�49+/r/n
	 sprintf((char *)&buf[0]," %-30s",(char *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));  //
	 strncpy((char *)&SdFp->InterFace_data.Adress[0],(char *)&buf[0],30);   //flash��ֻ��30����ַ����

 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[0][0],Interf_other_length);     //�����ո�
	 sprintf((char*)&buf[0],"   %10d",(int32_t)SaveData[0]);    //ƽ����ѹλ��
	 strncpy((char *)&SdFp->InterFace_data.Other[0][0],(char *)&buf[0],Interf_other_length); 
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[1][0],Interf_other_length);     //�����ո�
	 sprintf((char *)&buf[0],"   %10.2f",SaveData[1]);          //ƽ����ѹλ��
	 strncpy((char *)&SdFp->InterFace_data.Other[1][0],(char *)&buf[0],Interf_other_length); 
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[2][0],Interf_other_length);     //�����ո�
	 sprintf((char *)&buf[0],"   %10.2f",SaveData[2]);          //ƽ��ȫѹλ��
	 strncpy((char *)&SdFp->InterFace_data.Other[2][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[3][0],Interf_other_length);     //�����ո�
	 sprintf((char *)&buf[0],"   %10.2f",SaveData[3]);          //����ѹλ��
	 strncpy((char *)&SdFp->InterFace_data.Other[3][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[4][0],Interf_other_length);     //�����ո�
	 sprintf((char *)&buf[0],"   %10.4f",SaveData[4]);          //�̵�����
	 strncpy((char *)&SdFp->InterFace_data.Other[4][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[5][0],Interf_other_length);     //�����ո�
	 sprintf((char *)&buf[0],"   %10.4f",SaveData[5]);          //������d
	 strncpy((char *)&SdFp->InterFace_data.Other[5][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[6][0],Interf_other_length);     //�����ո�
	 sprintf((char *)&buf[0],"   %10.1f",SaveData[6]);          //ƽ������
	 strncpy((char *)&SdFp->InterFace_data.Other[6][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[7][0],Interf_other_length);     //�����ո�
	 sprintf((char *)&buf[0],"   %10.1f",SaveData[7]);          //ƽ������
	 strncpy((char *)&SdFp->InterFace_data.Other[7][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[8][0],Interf_other_length);     //�����ո�
	 sprintf((char *)&buf[0],"   %10d",(int32_t)SaveData[8]);    //��������
	 strncpy((char *)&SdFp->InterFace_data.Other[8][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[9][0],Interf_other_length);     //�����ո�
	 sprintf((char *)&buf[0],"   %10d",(int32_t)SaveData[9]);    //�������
	 strncpy((char *)&SdFp->InterFace_data.Other[9][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[10][0],Interf_other_length);     //�����ո�
	 sprintf((char *)&buf[0],"   %10.2f",SaveData[10]);          //��ʪ��
	 strncpy((char *)&SdFp->InterFace_data.Other[10][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[11][0],Interf_other_length);     //�����ո�
	 sprintf((char *)&buf[0],"   %10.2f",SaveData[11]);          //����ϵ��
	 strncpy((char *)&SdFp->InterFace_data.Other[11][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[12][0],Interf_other_length);     //�����ո�
	 sprintf((char *)&buf[0],"   %10.2f",SaveData[12]);          //����ϵ��
	 strncpy((char *)&SdFp->InterFace_data.Other[12][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[13][0],Interf_other_length);     //�����ո�
   sprintf((char *)&buf[0],"   %10.2f",SaveData[13]);          //Ƥ �� ��
	 strncpy((char *)&SdFp->InterFace_data.Other[13][0],(char *)&buf[0],Interf_other_length);
  
 }





 //---

  /* date 20200504
	������ void SaveGasDataLoad(float *SaveData)
	������	������������װ��
  ���룺 
 
    Gas:��������ָ��
 	Gas[0]=Sp->O2Concent.Value;
	Gas[1]=Sp->SO2Concent.Value;
	Gas[2]=Sp->NOConcent.Value;
	Gas[3]=Sp->NO2Concent.Value;
	Gas[4]=Sp->COConcent.Value;
	Gas[5]=Sp->H2SConcent.Value;
	Gas[6]=Sp->CO2Concent.Value;
	
*/
 void SaveGasDataLoad(float *SaveData ,float *Gas)
{  u8 i;
	Flashisp_DataStructure* pf=Flashisp_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Calculation_FormulaStructure * Cp=CalculationFormula_GetData();
	Slave_DataStructure*  Sp=Slave_GetData();

	SaveData[0]=Cp->Pd;//��ѹ
	SaveData[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJDY);//ƽ����ѹ

	SaveData[1]=Cp->Ps;	//��ѹ	
	SaveData[1]=GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJJY);//ƽ����ѹ

	SaveData[2]=Cp->Pt;//����ȫѹ
	SaveData[2]=GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJQY);//ƽ��ȫѹ
	
	SaveData[4]=Cp->Ts ;         //����
	SaveData[4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJYW);//ƽ������
	
	SaveData[5]=Cp->Ba;	//����ѹ	
	
	/*
    float Pvalue;
	if((pf->Data[FLASHIS_AdrXTSZBOX]&0x40)==0)	
	Pvalue=CalculationFormula_p(SaveData[4],SaveData[5],SaveData[1]);//���������ܶ�
	else
    Pvalue=CalculationFormula_p(Cp->Th,SaveData[5],0.0);//���������ܶ�
	SaveData[3]=CalculationFormula_Vs(Cp->Kp,SaveData[0],Pvalue);	//��������
	*/
	SaveData[3]=Cp->Vs;	//��������
	SaveData[3]=GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_YQLS_YQLS);//ƽ������
	
	SaveData[6]=Cp->F;//��ȡ�̵����
	SaveData[7]=Cp->Xsw*100;//��ʪ��
	SaveData[8]=Cp->Kp;	//Ƥ�й�ϵ��
	SaveData[9]=GetFlaotdataFormFlash2Word(FLASHIS_AdrCLYQ_WJCX_JS_ZSXS);//����ϵ��
	SaveData[10]=GetFlaotdataFormFlash2Word(FLASHIS_AdrCLYQ_WJCX_JS_FHXS);//����ϵ��
	
	//SaveData[11] =CalculationFormula_Qs(SaveData[3],SaveData[6]);//��������
	SaveData[11] =Cp->Qs;//��������
	SaveData[11] =GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_YGLL);//ƽ����������
	
	SaveData[12] =Cp->Qsnd;//�������	
	SaveData[12] =GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_BGLL);//�������
	
	
	SaveData[13]=Sp->O2Concent.Value;
	SaveData[14]=Sp->SO2Concent.Value;
	SaveData[15]=Sp->NOConcent.Value;
	SaveData[16]=Sp->NO2Concent.Value;
	SaveData[17]=Sp->COConcent.Value;
	SaveData[18]=Sp->H2SConcent.Value;
	SaveData[19]=Sp->CO2Concent.Value;
	
	for(i=13;i<=19;i++)
	{
		if(SaveData[i]<=0)
			SaveData[i]=0;
	
	}
	SaveData[20]=0;
	
	for(int i=0;i<7;i++)
	{
		if(pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ]&(1<<i))
		{
			SaveData[20]+=9;
		}
		SaveData[20]*=10;
	}
	
	SaveData[20]/=10;
	SaveData[21]= Flux_GetData()->ChannelC.Sampling.SecTimer; // ����ʱ��, modify by alfred   2020.06.27
	SaveData[22]=Sp->JWTem.Value; //��ǰ�¶� 
	
	
	
	if(Sp->O2Concent.Value!=21)
	SaveData[23]= 21.0f/(21-Sp->O2Concent.Value);//��ʣϵ��
	else
	SaveData[23]=SLAVE_ColFAULT;  //�ɼ����ݳ�����ΪSp->O2Concent.Value=21
	
	SaveData[24]=GetFlaotdataFormFlash2Word(FLASHIS_AdrCLYQ_WJCX_JS_JZYHL); //��׼��������
	
	
	SaveData[25]=Gas[0];
	SaveData[26]=Gas[1];
	SaveData[27]=Gas[2];
	SaveData[28]=Gas[3];
	
	SaveData[29]=Gas[4];
	SaveData[30]=Gas[5];
	SaveData[31]=Gas[6]; 
	
	for(i=25;i<=31;i++)
	{
		if(SaveData[i]<=0)
		SaveData[i]=0;
	
	}

}
//--
 /* date 2020-03-10
	�������SaveGasINputDataLoad(float *SaveData)
	������	����������������װ��
  ���룺
	
*/
 void SaveGasINputDataLoad(float *SaveData)
{

	Screen_DataStructure* ps=Screen_GetData();
	SdFile_DataStruct* SdFp=SdFile_GetData();
	char buff[100];
	FourBytes four;
	for(int i=0;i<25;i++)
	{
		strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[i][12]),10);//ԭʼ����װ��
		four.FloatData=atof(buff);
		SaveData[i]=four.FloatData;
	}
	   //װ��������������		
	for(int i=0;i<7;i++)
			{
				SaveData[13+i]=ps->DisValue[i];
				if(SaveData[13+i]<0)
				SaveData[13+i]=0;
			}
		

}
//---
 /* date 20200518
	��������SaveGasData(float *SaveData,u32 NumFile,u8 attr)
	������	���������ļ�
  ���룺
	
*/
void SaveGasAverageRecordData(float *SaveData,u32 NumFile)
 {
	 
	 SdFile_DataStruct* SdFp=SdFile_GetData();
	 Screen_DataStructure* ps=Screen_GetData();
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 u8 buf[80]={0};
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[13],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[0]);          //������
	 strncpy((char *)&SdFp->InterFace_data.Other[13][0],(char *)&buf[0],Interf_other_length);
	 
	 ps->GSXS=SaveData[0];
	 ps->GSXS=21.0f-ps->GSXS;
	 if(ps->GSXS==0)
	 ps->GSXS=SLAVE_ColFAULT;                               //��ʣϵ������
	 else
	 ps->GSXS=21.0f/ ps->GSXS;                               //������Ũ�ȼ����ʣϵ��
	 
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[14],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[1]);          //��������
	 strncpy((char *)&SdFp->InterFace_data.Other[14][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[15],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[2]);          //һ������
	 strncpy((char *)&SdFp->InterFace_data.Other[15][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[16],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[3]);          //��������
	 strncpy((char *)&SdFp->InterFace_data.Other[16][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[17],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[4]);          //һ����̼
	 strncpy((char *)&SdFp->InterFace_data.Other[17][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[18],Interf_other_length);  //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[5]);          //������
	 strncpy((char *)&SdFp->InterFace_data.Other[18][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[19],Interf_other_length);  //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[6]);          //������̼
	 strncpy((char *)&SdFp->InterFace_data.Other[19][0],(char *)&buf[0],Interf_other_length);
	 
 }
 
 //---
 /* date 20200518
	��������SaveGasData(float *SaveData,u32 NumFile,u8 attr)
	������	���������Ļ�׼������,����ϵ������ʣϵ��
  ���룺
	
*/
void SaveGas_Z_J_F_RecordData(void)
 {
	 
	 SdFile_DataStruct* SdFp=SdFile_GetData();
	 Screen_DataStructure* ps=Screen_GetData();
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 u8 buf[80]={0};
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[9],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",ps->ZSXS);          //����ϵ��
	 strncpy((char *)&SdFp->InterFace_data.Other[9][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[10],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",ps->FHXS);          //����ϵ��
	 strncpy((char *)&SdFp->InterFace_data.Other[10][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[23],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",ps->GSXS);          //��ʣϵ��
	 strncpy((char *)&SdFp->InterFace_data.Other[23][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[24],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",ps->JZHYL);          //��׼������
	 strncpy((char *)&SdFp->InterFace_data.Other[24][0],(char *)&buf[0],Interf_other_length);

 }

//---
 /* date 2020-03-10
	��������ReadFlueData(u32 FileNum)
	������	���������ļ�
  ���룺
	
*/
void SaveGasData(float *SaveData,u32 NumFile,u8 attr)
 {
	 u8 i;
	 SdFile_DataStruct* SdFp=SdFile_GetData();
	 Screen_DataStructure* ps=Screen_GetData();
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 u8 buf[80]={0};
	
	//װ������ʱ��
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.dat[0],Interf_dat_length);  //�����ո�
     strncpy((char *)&SdFp->InterFace_data.dat[0],(char *)&ps->recorstarddat[0],Interf_dat_length); 
	 
   //װ�ص�ַ
	 
	 sprintf((char *)&buf[0],"%02d",(pf->Data[FLASHIS_AdrCLYQ_JSSC]));  //
	 strncpy((char *)&SdFp->InterFace_data.T[0],(char *)&buf[0],2);     //���Ӳ���ʱ��ת�����ַ���
	 SdFp->InterFace_data.T[2]=0;
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Adress[0],Adress_length-9-2);  //�����ո�49+/r/n
	 sprintf((char *)&buf[0]," %-30s",(char *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));  //
	 strncpy((char *)&SdFp->InterFace_data.Adress[0],(char *)&buf[0],30);   //flash��ֻ��30����ַ����

	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[0][0],Interf_other_length);      //�����ո�
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[0]);                             //ƽ����ѹλ��
	 strncpy((char *)&SdFp->InterFace_data.Other[0][0],(char *)&buf[0],Interf_other_length);   //flash��ֻ��30����ַ����
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[1],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[1]);           //ƽ����ѹλ��
	 strncpy((char *)&SdFp->InterFace_data.Other[1][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[2],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[2]);           //ƽ��ȫѹλ��
	 strncpy((char *)&SdFp->InterFace_data.Other[2][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[3],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[3]);           //ƽ������
	 strncpy((char *)&SdFp->InterFace_data.Other[3][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[4],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[4]);           //ƽ������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[4][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[5],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[5]);           //����ѹ
	 	 strncpy((char *)&SdFp->InterFace_data.Other[5][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[6],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.4f",SaveData[6]);           //�̵�����
	 	 strncpy((char *)&SdFp->InterFace_data.Other[6][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[7],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[7]);           //�� ʪ ��
	 	 strncpy((char *)&SdFp->InterFace_data.Other[7][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[8],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[8]);           //Ƥ��ϵ��
	 	 strncpy((char *)&SdFp->InterFace_data.Other[8][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[9],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[9]);           //����ϵ��
	 	 strncpy((char *)&SdFp->InterFace_data.Other[9][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[10],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[10]);           //����ϵ��
	 	 strncpy((char *)&SdFp->InterFace_data.Other[10][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[11],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[11]);   //��������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[11][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[12],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[12]);   //�������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[12][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[13],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[13]);          //������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[13][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[14],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[14]);          //��������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[14][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[15],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[15]);          //һ������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[15][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[16],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[16]);          //��������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[16][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[17],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[17]);          //һ����̼
	 	 strncpy((char *)&SdFp->InterFace_data.Other[17][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[18],Interf_other_length);  //�����ո�
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[18]);          //������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[18][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[19],Interf_other_length);  //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[19]);          //������̼
	 strncpy((char *)&SdFp->InterFace_data.Other[19][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[20],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %10d",(int32_t)SaveData[20]);   //�˿�����
	 strncpy((char *)&SdFp->InterFace_data.Other[20][0],(char *)&buf[0],Interf_other_length);

	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[21],Interf_other_length);  //�����ո�
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[21]);          //����ʱ��
	 strncpy((char *)&SdFp->InterFace_data.Other[21],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[22],Interf_other_length);  //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[22]);          //��ǰ�¶�
	 strncpy((char *)&SdFp->InterFace_data.Other[22][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[23],Interf_other_length);  //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[23]);          //��ʣϵ��
	 strncpy((char *)&SdFp->InterFace_data.Other[23][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[24],Interf_other_length);  //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[24]);          //��׼��������
	 strncpy((char *)&SdFp->InterFace_data.Other[24][0],(char *)&buf[0],Interf_other_length);

	 SdFile_ramlodair((char *)&SdFp->InterFace_data.recorddat[0],Record_row_length);  //�����ո�20-1=19
	 

	 strncpy((char *)&SdFp->InterFace_data.recorddat[0],(char *)&ps->recorddat[0],Record_row_length-1);  //copyʱ���

	 for(i=0;i<9;i++)
	 {
		SdFile_ramlodair((char *)&SdFp->InterFace_data.record[i][0],(Record_row_length-10));    //�����ո�
		if((i==0)&&(i==7))
		sprintf((char*)&buf[0],"%10.1f",SaveData[25+i]);
		else
		sprintf((char*)&buf[0],"%10.0f",SaveData[25+i]);
		strncpy((char *)&SdFp->InterFace_data.record[i][0],(char *)&buf[0],Record_row_length-10);
	 }
	
 }
   //---
 /* date 20200518
	��������ReadGasFirstData(u32 FileNum)
	������ ��ȡ�����ļ�-��һҳ
  ���룺
*/         
void ReadGasFirstData(u32 FileNum)//�����ļ��� ��ȡ������Ϣ
{   float buf;
    SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	SdFile_ReadGasFile(FileNum,0x01);
	Screen_ResetDataBuffer();
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //�ļ���
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.dat[0]),10);//����        
	Screen_RefreshDataMulti(0x0140,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.dat[11]),5);//ʱ��       
	Screen_RefreshDataMulti(0x0120,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[21][12]),10);//�����ܳ�
	
	//strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[21][12]),10);
	buf=atof((char *)&ps->Buffer[0]);
	Screen_ResetDataBuffer();
	sprintf((char *)&ps->Buffer[0],"%02d:%02d:%02d",((u32)buf/3600),((u32)buf%3600)/60,(((u32)buf%3600)%60));
	Screen_RefreshDataMulti(0x0160,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[7][12]),10);//��ʪ��
	Screen_RefreshDataMulti(0x0180,ps->Buffer,15*2);
  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[6][12]),10);//�����
	Screen_RefreshDataMulti(0x01A0,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[4][12]),10);//�����¶�
	Screen_RefreshDataMulti(0x01C0,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[0][12]),10);//ƽ����ѹ
	Screen_RefreshDataMulti(0x01E0,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[1][12]),10);//ƽ����ѹ
	Screen_RefreshDataMulti(0x0400,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[5][12]),10);//����ѹ
	Screen_RefreshDataMulti(0x0540,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[22][12]),10);//��ǰ�¶�
	Screen_RefreshDataMulti(0x0420,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[3][12]),10);//ƽ������
	Screen_RefreshDataMulti(0x0440,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[24][12]),10);//��׼������
	ps->JZHYL=atof((char *)&ps->Buffer[0]);   //�ַ���ת���ɸ�����
	Screen_RefreshDataMulti(0x0460,ps->Buffer,15*2);
	
	//��������ϵ��
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[9][12]),10);//
	ps->ZSXS=atof((char *)&ps->Buffer[0]);   //�ַ���ת���ɸ�����

	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[13][12]),10);//������
	Screen_RefreshDataMulti(0x0480,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[23][12]),10);//��ʣϵͳ
	ps->GSXS=atof((char *)&ps->Buffer[0]);   //�ַ���ת���ɸ�����
	if(ps->GSXS==1000000)  //����ı�־
	strcpy((char *)ps->Buffer,"----------");
	Screen_RefreshDataMulti(0x04A0,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[10][12]),10);//����ϵ��
	ps->FHXS=atof((char *)&ps->Buffer[0]);   //�ַ���ת���ɸ�����
	Screen_RefreshDataMulti(0x04C0,ps->Buffer,15*2);
	
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[11][12]),10);//��������
	Screen_RefreshDataMulti(0x0500,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[12][12]),10);//�������
	Screen_RefreshDataMulti(0x0520,ps->Buffer,15*2);
	
	//��ʾ��ַ��Ϣ
	{
	Screen_ResetDataBuffer();
	strncpy((char*)ps->Buffer,(char *)&(SdFp->rdWorkFile.Adress[9]),30);
	}				
	 Screen_RefreshDataMulti(0x0560,ps->Buffer,15*2);
	
	
	 SdFile_Serch_MaxofGasRecord(FileNum);
	 Screen_ResetDataBuffer();	
	 if(SdFp->GasRecordMax)
	 sprintf((char *)ps->Buffer,"��������:%d",SdFp->GasRecordMax); //
	 else
	 sprintf((char *)ps->Buffer,"��������:�޷�������!"); //
	// strcpy((char *)ps->Buffer,"��������");//����ʱ��
	 Screen_RefreshDataMulti(0x2E20,ps->Buffer,15*2);
 
}
 /* date 2020-03-30
	��������void GetGasSecondData (u32 FileNum)
	������	��ȡ��ǰ�������� -�ڶ�ҳ
  ���룺void
*/
void GetGasSecondData(u32 FileNum)//��ȡ��ǰ��������
{
	SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	char buff[61]={0};
	SdFile_ReadGasFile(FileNum,0x01);
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //�ļ���
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);
	FourBytes four,four1,four2,four3;
	for(int i=0;i<5;i++)  //SO2,NO,NO2,CO,H2S
	{
		strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[14+i][12]),10);//����Ũ��
		four.FloatData=atof(buff);
		ps->DisValue[i]=four.FloatData;
	}
	strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[9][12]),10);//����ϵ��
	four.FloatData=atof(buff);
	ps->DisValue[7]=four.FloatData;//����ϵ��
	
	strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[13][12]),10);//����Ũ��
	four.FloatData=atof(buff);
	CalculationFormula_a(four.FloatData); //���������ʣϵ��������C_F_Structure.a��CalculationFormula_CaALL�����õ�
	
	strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[12][12]),10);//�������
	four1.FloatData=atof(buff);
	
	strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[10][12]),10);//���ɣ�������ϵ��
	four2.FloatData=atof(buff);
	
	CalculationFormula_CaALL(&ps->DisValue[0],ps->DisValue[7],four1.FloatData,four2.FloatData); //���̳��������ͼ���
	CalculationFormula_GetData();
	
		if(ps->DisValue[0]!=1000000)
		{
			if(ps->GSXS==1000000)
			sprintf((char*)ps->Buffer,"%8.0f       ------  %10.1f",ps->DisValue[0],CalculationFormula_GetData()->GaofSO2); //ԭֵ������Ũ�ȣ��ŷ���
		    else
		    sprintf((char*)ps->Buffer,"%8.0f %10.0f  %10.1f",ps->DisValue[0],CalculationFormula_GetData()->CaofSO2,CalculationFormula_GetData()->GaofSO2); //ԭֵ������Ũ
		}
		else
		{
			sprintf((char*)ps->Buffer,(char *)"  ------       ------       ------  ");
		}
	   Screen_RefreshDataMulti(0x0120,ps->Buffer,20*2);
	
		if(ps->DisValue[1]!=1000000)
		{
			if(ps->GSXS==1000000)
			sprintf((char*)ps->Buffer,"%8.0f       ------  %10.1f",ps->DisValue[1],CalculationFormula_GetData()->GaofNO); //ԭֵ������Ũ�ȣ��ŷ���
		    else
			sprintf((char*)ps->Buffer,"%8.0f %10.0f  %10.1f",ps->DisValue[1],CalculationFormula_GetData()->CaofNO,CalculationFormula_GetData()->GaofNO); //ԭֵ������Ũ�ȣ��ŷ���
		}
		else
		{
			sprintf((char*)ps->Buffer,(char *)"  ------       ------       ------  ");
		}
	   Screen_RefreshDataMulti(0x0160,ps->Buffer,20*2);
		
		if(ps->DisValue[2]!=1000000)
		{
			if(ps->GSXS==1000000)
			sprintf((char*)ps->Buffer,"%8.0f       ------  %10.1f",ps->DisValue[2],CalculationFormula_GetData()->GaofNO2); //ԭֵ������Ũ�ȣ��ŷ���
		    else
			sprintf((char*)ps->Buffer,"%8.0f %10.0f  %10.1f",ps->DisValue[2],CalculationFormula_GetData()->CaofNO2,CalculationFormula_GetData()->GaofNO2); //ԭֵ������Ũ�ȣ��ŷ���
		}
		else
		{
			sprintf((char*)ps->Buffer,(char *)"  ------       ------       ------  ");
		}
	   Screen_RefreshDataMulti(0x01A0,ps->Buffer,20*2);
		
		if(ps->DisValue[3]!=1000000)
		{
			if(ps->GSXS==1000000)
			sprintf((char*)ps->Buffer,"%8.0f       ------  %10.1f",ps->DisValue[3],CalculationFormula_GetData()->GaofCO); //ԭֵ������Ũ�ȣ��ŷ���
		    else
			sprintf((char*)ps->Buffer,"%8.0f %10.0f  %10.1f",ps->DisValue[3],CalculationFormula_GetData()->CaofCO,CalculationFormula_GetData()->GaofCO); //ԭֵ������Ũ�ȣ��ŷ���
		}
		else
		{
			sprintf((char*)ps->Buffer,(char *)"  ------       ------       ------  ");
		}
	   Screen_RefreshDataMulti(0x0440,ps->Buffer,20*2);

		if(ps->DisValue[4]!=1000000)
		{
			if(ps->GSXS==1000000)
			sprintf((char*)ps->Buffer,"%8.0f       ------  %10.1f",ps->DisValue[4],CalculationFormula_GetData()->GaofH2S); //ԭֵ������Ũ�ȣ��ŷ���
		    else
			sprintf((char*)ps->Buffer,"%8.0f %10.0f  %10.1f",ps->DisValue[4],CalculationFormula_GetData()->CaofH2S,CalculationFormula_GetData()->GaofH2S); //ԭֵ������Ũ�ȣ��ŷ���
		}
		else
		{
			sprintf((char*)ps->Buffer,(char *)"  ------       ------       ------  ");
		}
	   Screen_RefreshDataMulti(0x0480,ps->Buffer,20*2);
		
          //NOX Ũ��
		if(ps->DisValue [1]==1000000||ps->DisValue[2]==1000000)
		{
			ps->DisValue[8]=1000000;
		}
		else
		{  

			ps->DisValue[8]= CalculationFormula_NOX(ps->DisValue [1],ps->DisValue [2]);
			four.FloatData=CalculationFormula_Ca(ps->DisValue[8],CalculationFormula_GetData()->a,ps->DisValue[7],four2.FloatData);
			four3.FloatData=CalculationFormula_G(ps->DisValue[8],four1.FloatData);
		} 
	
		if(ps->DisValue[8]!=1000000)
		{   if(ps->GSXS==1000000)
			sprintf((char*)ps->Buffer,"%8.0f       ------  %10.1f",ps->DisValue[8],four3.FloatData); //ԭֵ������Ũ�ȣ��ŷ���
		    else
			sprintf((char*)ps->Buffer,"%8.0f %10.0f  %10.1f",ps->DisValue[8],four.FloatData,four3.FloatData);
		}
		else
		{
			sprintf((char*)ps->Buffer,"  ------       ------       ------  ");
		}
		Screen_RefreshDataMulti(0x0400,ps->Buffer,20*2);
		
		strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[19][12]),10);//CO2
	    four.FloatData=atof(buff);
		//CO2
		if(four.FloatData!=1000000)
		{
			sprintf((char*)ps->Buffer,"%8.0f ",four.FloatData);
		}
		else
		{
			sprintf((char*)ps->Buffer,"  ------       ------       ------  ");
		}
		
       	Screen_RefreshDataMulti(0x04C0,ps->Buffer,20*2);
}
//---
 /* date 20200518
	��������void GetGasSecondData (u32 FileNum)
	������	��ȡ��ǰ����Ũ������
  ���룺void
*/
void GetGasLongDuData(u32 FileNum ,float *DisValue)//��ȡ��ǰ����Ũ������
{
	SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	char buff[61]={0};
	SdFile_ReadGasFile(FileNum,0x01);
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //�ļ���
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);
	FourBytes four;
	for(int i=0;i<7;i++)
	{
		strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[13+i][12]),10);//����Ũ��
		four.FloatData=atof(buff);
		DisValue[i]=four.FloatData;
	}
}

/* date 20200514
	�������void GetGasFileRecodData(u32 FileNum,u32 Recod)
	������	��ȡ��ǰ�����¼����
  ���룺isDisplay =0;����ʾ  ==1 ��ʾ
*/
	char DisBuff[8][30]={
		{"O2 Ũ��:    ------%"},
		{"SO2Ũ��:    ------mg/m3"},
		{"NO Ũ��:    ------mg/m3"},
		{"NO2Ũ��:    ------mg/m3"},
		{"NOXŨ��:    ------mg/m3"},
		{"CO Ũ��:    ------mg/m3"},
		{"H2SŨ��:    ------mg/m3"},
		{"CO2Ũ��:    ------%"}};
void GetGasFileRecodData(u32 FileNum,u32 Recod,float * GetData,u8 isDisplay)//��ȡ��ǰ�����¼����
{
  SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
  
	char buff[61]={0};
	
	Screen_ResetDataBuffer();
	sprintf((char *)ps->Buffer,"%04d-",Recod);                   //20200514
	sprintf((char *)&buff[0],"%04d",SdFp->GasRecordMax);
	strcat((char *)ps->Buffer,(char *)&buff[0]);
	Screen_RefreshDataMulti(0x100,ps->Buffer,15*2);              //��ʾ�ڼ�����Ϣ
	
	SdFile_ReadGasRecord(FileNum,Recod);
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.G_record.arr[0]),16);//ʱ��             20200514
	Screen_RefreshDataMulti(0x2E20,ps->Buffer,15*2);
	
	if(SdFp->rdWorkFile.G_record.arr[18]=='9')                                   //��ȡ��������־
		GetData[8]=1;
	else
		GetData[8]=0;
	
	FourBytes four;
	for(int i=0;i<7;i++)
	{
		strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.G_record.stru.rowdata[i+1][0]),10);//����Ũ��
		four.FloatData=atof(buff);
		if(i<4)
		GetData[i]=four.FloatData;
		else
		GetData[i+1]=four.FloatData;	
	}
	//����NOX
	if(GetData[2]==1000000||GetData[3]==1000000)
	{
	 GetData[4]=1000000;//NOX Ũ��
	}else

	GetData[4]=CalculationFormula_NOX(GetData[2],GetData[3]);
	if(isDisplay==1)
	{
		for(int i=0;i<7;i++)
		{
			if(ps->DisValue[i]!=1000000)
			{   if(i==0)
				sprintf((char*)buff,"%10.1f",GetData[i]);
				else
				sprintf((char*)buff,"%10.0f",GetData[i]);
				
				strncpy((char *)&DisBuff[i][8],(char *)buff,10);  
			}
				strncpy((char *)&ps->Buffer,(char *)DisBuff[i],30);	
				Screen_RefreshDataMulti(0x0120+i*0x20,ps->Buffer,15*2);

		}	

		
		if(ps->DisValue[7]!=1000000)
		{
			sprintf((char*)buff,"%10.1f",GetData[7]);
			strncpy((char *)&DisBuff[7][8],(char *)buff,10);

		}
		strncpy((char *)&ps->Buffer,(char *)DisBuff[7],30);	
		Screen_RefreshDataMulti(0x0401,ps->Buffer,15*2);
}

}
//---


  





 /* date 2020-03-10
	������?void SaveGasDataLoad(float *SaveData)
	������	������������װ��
  ���룺 
	
*/
 void SaveDustDataLoad(float *SaveData)
{       u8 i;
	    Flashisp_DataStructure* pf=Flashisp_GetData();
	    Screen_DataStructure* ps=Screen_GetData();
	    Calculation_FormulaStructure * Cp=CalculationFormula_GetData();
	    Slave_DataStructure*  Sp=Slave_GetData();
			SaveData[0]  = Cp->Pd;//��ѹ
	        SaveData[0]  = GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJDY); //ƽ����ѹ
	
			SaveData[1]  = Cp->Ps;	//��ѹ	
	        SaveData[1]  = GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJJY); //ƽ����ѹ
	
			SaveData[2]  = Cp->Pt;//����ȫѹ
	        SaveData[2]  = GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJQY); //ƽ��ȫѹ
	
			SaveData[4]  = Cp->Ts ;         //����
	        SaveData[4]  = GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJYW); //ƽ������
	
			SaveData[5]  = Cp->Ba;	//����ѹ	
	
	        /*
	        float Pvalue;
	        if((pf->Data[FLASHIS_AdrXTSZBOX]&0x40)==0)
			Pvalue = CalculationFormula_p(SaveData[4],SaveData[5],SaveData[1]);//���������ܶ�
			else
			Pvalue = CalculationFormula_p(Cp->Th,SaveData[5],0.0);//���������ܶ�
	        
			
			SaveData[3]  = CalculationFormula_Vs(Cp->Kp,SaveData[0],Pvalue);	//��������
	        */
	        SaveData[3]  =Cp->Vs; //����
			SaveData[3]  =GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_YQLS_YQLS); //ƽ������
			
			SaveData[6]  = Cp->F;//��ȡ�̵����
			SaveData[7]  = Cp->Xsw*100;//��ʪ��
			SaveData[8]  = Cp->Kp ;	//Ƥ�й�ϵ��
			SaveData[9]  = GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSZSXS);//����ϵ��
			SaveData[10] = GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSFHXS);//����ϵ��
			//SaveData[11] = CalculationFormula_Qs(SaveData[3],SaveData[6]);//��������
			SaveData[11] = Cp->Qs; //����
			SaveData[11] = GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_YGLL);//ƽ����������
			
			SaveData[12] = Cp->Qsnd;//�������	
			SaveData[12] = GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_BGLL);//ƽ���������	
	     
	        SaveData[13] = ps->YC_FollowRate;//������
	        SaveData[14] = GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ);//����ֱ��
		    SaveData[15]=Cp->Pcs;//ƽ����ѹ
			SaveData[16]=Cp->Tj;//ƽ������
			SaveData[17]=Flux_GetData()->ChannelB.Sampling.SumVolume;        //�������
		    SaveData[18]=Flux_GetData()->ChannelB.Sampling.RefVolume;        //������
			SaveData[19]=GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSYCZL);//����		
            
		
			SaveData[20]=Sp->O2Concent.Value;
			
			SaveData[21]=Sp->SO2Concent.Value;

			SaveData[22]=Sp->NOConcent.Value;

			SaveData[23]=Sp->NO2Concent.Value;

			SaveData[24]=Sp->COConcent.Value;

			if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x01))==1)  //
			{
			SaveData[21]=SaveData[21]-SaveData[24]*GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_XTSZ_CO_SO2); //������SO2
            if(SaveData[21]<0)SaveData[21]=0;
			}
			
			
			SaveData[25]=Sp->H2SConcent.Value;
			SaveData[26]=Sp->CO2Concent.Value; 
	
		
			for(i=20;i<=26;i++)
	         {
		     if(SaveData[i]<=0)
			 SaveData[i]=0;
	        }
			
			SaveData[27]=0;
              for(int i=0;i<7;i++)
             {
				if(pf->Data[FLASHIS_AdrWHJZ_QTCGQXZ]&(1<<i))
				{
					  SaveData[27]+=9;
				}
			SaveData[27]*=10;
			}
			SaveData[27]/=10;		
			SaveData[28]=002; //��Ʒ���
			//SaveData[29]= ps->YC_SampleAllTime;//�ܲ���ʱ��
			SaveData[29]= Flux_GetData()->ChannelB.Sampling.SecTimer;//�ܲ���ʱ��
			SaveData[30]=ps->QimiStat;
			SaveData[31]=(pf->Data[FLASHIS_AdrXTSZBOX]&(1<<4))==0x0000?0:1;
			SaveData[32]=GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSRYND);// ��ҺŨ��
			SaveData[33]=GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSDRTJ); // �������
			SaveData[34]=pf->Data[FFLASHIS_AdrYCCJ_YCWJCX_JSZTSM];                   //��ͷ��Ŀ
			SaveData[35]=GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSJZYHL); //��׼��������
            SaveData[36]=Sp->JWTem.Value; //��ǰ�¶� 
			SaveData[37]=Cp->Pcs;//��ǰѹ	
			
			if(Sp->O2Concent.Value!=21)
            SaveData[38]= 21.0f/(21-Sp->O2Concent.Value);//��ʣϵ��
			else
			SaveData[38]=SLAVE_ColFAULT;  //��������ݣ���Ϊ���
}
//------
/* date 20200519
	��������ReadFlueData(u32 FileNum)
	������	�����̳��ļ�
  ���룺
	
*/
void SaveDustData(float *SaveData,u32 NumFile)
{

	 SdFile_DataStruct* SdFp=SdFile_GetData();
	 Screen_DataStructure* ps=Screen_GetData();
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 u8 buf[80]={0};
	 //װ������ʱ��
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.dat[0],Interf_dat_length);  //�����ո�
   sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
   strncpy((char *)&SdFp->InterFace_data.dat[0],(char *)buf,Interf_dat_length);  
   //װ�ص�ַ
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Adress[0],Adress_length-9-2);  //�����ո�49+/r/n
	 sprintf((char *)&buf[0]," %-30s",(char *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));  //
	 strncpy((char *)&SdFp->InterFace_data.Adress[0],(char *)&buf[0],30);   //flash��ֻ��30����ַ����
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[0],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[0]);    //ƽ����ѹλ��
	 strncpy((char *)&SdFp->InterFace_data.Other[0][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[1],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[1]);           //ƽ����ѹλ��
	 	 strncpy((char *)&SdFp->InterFace_data.Other[1][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[2],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[2]);           //ƽ��ȫѹλ��
	 	 strncpy((char *)&SdFp->InterFace_data.Other[2][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[3],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[3]);           //ƽ������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[3][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[4],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[4]);           //ƽ������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[4][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[5],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[5]);           //����ѹ
	 	 strncpy((char *)&SdFp->InterFace_data.Other[5][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[6],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.4f",SaveData[6]);           //�̵�����
	 	 strncpy((char *)&SdFp->InterFace_data.Other[6][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[7],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[7]);           //�� ʪ ��
	 	 strncpy((char *)&SdFp->InterFace_data.Other[7][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[8],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[8]);           //Ƥ��ϵ��
	 	 strncpy((char *)&SdFp->InterFace_data.Other[8][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[9],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[9]);           //����ϵ��
	 	 strncpy((char *)&SdFp->InterFace_data.Other[9][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[10],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[10]);           //����ϵ��
	 	 strncpy((char *)&SdFp->InterFace_data.Other[10][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[11],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[11]);   //��������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[11][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[12],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[12]);   //�������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[12][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[13],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[13]);          //�� �� ��
	 	 strncpy((char *)&SdFp->InterFace_data.Other[13][0],(char *)&buf[0],Interf_other_length);

	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[14],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[14]);          //����ֱ��
	 	 strncpy((char *)&SdFp->InterFace_data.Other[14][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[15],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[15]);          //ƽ����ѹ
	 	 strncpy((char *)&SdFp->InterFace_data.Other[15][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[16],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[16]);          //ƽ������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[16][0],(char *)&buf[0],Interf_other_length);
	
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[17],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[17]);          //�������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[17][0],(char *)&buf[0],Interf_other_length);
	
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[18],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[18]);          //������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[18][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[19],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.5f",SaveData[19]);            //�̳�����20200519
	 	 strncpy((char *)&SdFp->InterFace_data.Other[19][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[20],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[20]);          //�� �� ��
	 	 strncpy((char *)&SdFp->InterFace_data.Other[20][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[21],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[21]);          //�������� 
	 	 strncpy((char *)&SdFp->InterFace_data.Other[21][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[22],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[22]);          //һ������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[22][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[23],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[23]);          //�������� 
	 	 strncpy((char *)&SdFp->InterFace_data.Other[23][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[24],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[24]);          //һ����̼ 
	 	 strncpy((char *)&SdFp->InterFace_data.Other[24][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[25],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[25]);          //�� �� �� 
	 	 strncpy((char *)&SdFp->InterFace_data.Other[25][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[26],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[26]);          //������̼
	 	 strncpy((char *)&SdFp->InterFace_data.Other[26][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[27],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %10d",(int32_t)SaveData[27]); //�˿�����
	 	 strncpy((char *)&SdFp->InterFace_data.Other[27][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[28],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[28]);   //��Ʒ���
	 	 strncpy((char *)&SdFp->InterFace_data.Other[28][0],(char *)&buf[0],Interf_other_length);

	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[29],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[29]);   //�ۼƲ�ʱ
	 	 strncpy((char *)&SdFp->InterFace_data.Other[29][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[30],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[30]);   //�� �� ��
	 	 strncpy((char *)&SdFp->InterFace_data.Other[30][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[31],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[31]);   //��������
	 	 strncpy((char *)&SdFp->InterFace_data.Other[31][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[32],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[32]);          //��ҺŨ��
	 strncpy((char *)&SdFp->InterFace_data.Other[32][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[33],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[33]);          //�������
	 strncpy((char *)&SdFp->InterFace_data.Other[33][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[34],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[34]);   //��ͷ��Ŀ
	 strncpy((char *)&SdFp->InterFace_data.Other[34][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[35],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[35]);          //��׼����
	 strncpy((char *)&SdFp->InterFace_data.Other[35][0],(char *)&buf[0],Interf_other_length);
	
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[36],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[36]);          //��ǰ�¶�
	 strncpy((char *)&SdFp->InterFace_data.Other[36][0],(char *)&buf[0],Interf_other_length);

 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[37],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[37]);          //��ǰѹ��
	 strncpy((char *)&SdFp->InterFace_data.Other[37][0],(char *)&buf[0],Interf_other_length);

 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[38],Interf_other_length);    //�����ո�
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[38]);          //��ʣϵ��:
	 strncpy((char *)&SdFp->InterFace_data.Other[38][0],(char *)&buf[0],Interf_other_length);


}
/* date 20200520
	��������ReadDustFirstData(u32 FileNum)
	������ ��ȡ�̳��ļ�-��һҳ
  ���룺
*/         
void ReadDustData(u32 FileNum)//�����ļ��� ��ȡ�̳���Ϣ
{
  SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	SdFile_ReadDustFile(FileNum);

//ƽ����ѹ0:   0          Pa   
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[0][12]),10);
	ps->DisValue[0]=atof((char *)&ps->Buffer[0]);
//ƽ����ѹ1:   -0.05      kPa  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[1][12]),10);
	ps->DisValue[1]=atof((char *)&ps->Buffer[0]);

//ƽ��ȫѹ2:   -0.05      kPa  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[2][12]),10);
	ps->DisValue[2]=atof((char *)&ps->Buffer[0]);
//ƽ������3:   0.0        m/s  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[3][12]),10);
	ps->DisValue[3]=atof((char *)&ps->Buffer[0]);
//ƽ������4:   206.0      ��   
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[4][12]),10);
	ps->DisValue[4]=atof((char *)&ps->Buffer[0]);
//�� �� ѹ5:   103.50     kPa  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[5][12]),10);
	ps->DisValue[5]=atof((char *)&ps->Buffer[0]);
//�̵�����6:   0.7854     m2   
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[6][12]),10);
	ps->DisValue[6]=atof((char *)&ps->Buffer[0]);
//�� ʪ ��7:   0.50       
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[7][12]),10);
	ps->DisValue[7]=atof((char *)&ps->Buffer[0]);
//Ƥ��ϵ��8:   0.84      
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[8][12]),10);
	ps->DisValue[8]=atof((char *)&ps->Buffer[0]);
//����ϵ��9:   1.00            
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[9][12]),10);
	ps->DisValue[9]=atof((char *)&ps->Buffer[0]);
//����ϵ��10:   1.00            
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[10][12]),10);
	ps->DisValue[10]=atof((char *)&ps->Buffer[0]);
//��������11:   0          m3/h 
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[11][12]),10);
	ps->DisValue[11]=atof((char *)&ps->Buffer[0]);
//�������12:   0          m3/h 
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[12][12]),10);
	ps->DisValue[12]=atof((char *)&ps->Buffer[0]);
//�� �� ��13:   0.00         
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[13][12]),10);
	ps->DisValue[13]=atof((char *)&ps->Buffer[0]);
//����ֱ��14:   0.8        mm  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[14][12]),10);
	ps->DisValue[14]=atof((char *)&ps->Buffer[0]);
//ƽ����ѹ15:   -0.01      Kpa  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[15][12]),10);
	ps->DisValue[15]=atof((char *)&ps->Buffer[0]);
//ƽ������16:   30.50      ��   
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[16][12]),10);
	ps->DisValue[16]=atof((char *)&ps->Buffer[0]);
//�������17:   0.0        L   
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[17][12]),10);
	ps->DisValue[17]=atof((char *)&ps->Buffer[0]);
//������18:   0.0        NL  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[18][12]),10);
	ps->DisValue[18]=atof((char *)&ps->Buffer[0]);
//�̳�����19: 1.00000      g    
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[19][12]),10);
	ps->DisValue[19]=atof((char *)&ps->Buffer[0]);
//�� �� ��20:   64.41      %    
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[20][12]),10);
	ps->DisValue[20]=atof((char *)&ps->Buffer[0]);
//��������21:   34178.55   mg/m3
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[21][12]),10);
	ps->DisValue[21]=atof((char *)&ps->Buffer[0]);
//һ������22:   5913.55    mg/m3
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[22][12]),10);
	ps->DisValue[22]=atof((char *)&ps->Buffer[0]);
//��������23:   595.61     mg/m3
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[23][12]),10);
	ps->DisValue[23]=atof((char *)&ps->Buffer[0]);
//һ����̼24:   49793.92   mg/m3
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[24][12]),10);
	ps->DisValue[24]=atof((char *)&ps->Buffer[0]);
//�� �� ��25:   1000000.00 mg/m3
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[25][12]),10);
	ps->DisValue[25]=atof((char *)&ps->Buffer[0]);
//������̼26:   1000000.00 %    
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[26][12]),10);
	ps->DisValue[26]=atof((char *)&ps->Buffer[0]);
//�˿�����27:      9999900      
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[27][12]),10);
	ps->DisValue[27]=atof((char *)&ps->Buffer[0]);
//��Ʒ���28:   2              
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[28][12]),10);
	ps->DisValue[28]=atof((char *)&ps->Buffer[0]);
//�ۼƲ�ʱ29:   10         s    
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[29][12]),10);
	ps->DisValue[29]=atof((char *)&ps->Buffer[0]);
//�� �� ��30:   0    
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[30][12]),10);
	ps->DisValue[30]=atof((char *)&ps->Buffer[0]);
//��������31:   0     
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[31][12]),10);
	ps->DisValue[31]=atof((char *)&ps->Buffer[0]);
//��ҺŨ��32:   0.0          
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[32][12]),10);
	ps->DisValue[32]=atof((char *)&ps->Buffer[0]);
//�������33:   0.0      
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[33][12]),10);
	ps->DisValue[33]=atof((char *)&ps->Buffer[0]);
//��ͷ��Ŀ34:   1               
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[34][12]),10);
	ps->DisValue[34]=atof((char *)&ps->Buffer[0]);
//��׼����35:   1.0        %    
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[35][12]),10);
	ps->DisValue[35]=atof((char *)&ps->Buffer[0]);
//��ǰ�¶�36:   30.5       ��   
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[36][12]),10);
	ps->DisValue[36]=atof((char *)&ps->Buffer[0]);
//��ǰѹ��37:   -0.0       kPa  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[37][12]),10);
	ps->DisValue[37]=atof((char *)&ps->Buffer[0]);
//��ʣϵ��38:   -0.5            
   ps->DisValue[38]=CalculationFormula_a(ps->DisValue[20]);
}

/* date 2020-09-07  bye linzequan
	��������ReadLampBlackFirstData(u32 FileNum)
	������ ��ȡ�����ļ� --��һҳ
  ���룺
*/        
void ReadLampBlackFirstData(u32 FileNum)
{
	SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	SdFile_ReadDustFile(FileNum);
	Screen_ResetDataBuffer();
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //�ļ���
	FourBytes four;
	char buf[61]={0};
	strncpy((char *)buf,(char *)&(SdFp->rdWorkFile.Other[31][12]),10);//��������
	four.U32Data=atoi(buf);
	ps->YC_Type=four.U32Data;//��������
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.dat[0]),10);//����                 //
	Screen_RefreshDataMulti(0x0120,ps->Buffer,15*2);
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.dat[11]),5);//ʱ��
	Screen_RefreshDataMulti(0x0140,ps->Buffer,15*2);
	
//----------------------	
	//sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[29]);//�ۼƲ���ʱ��
	sprintf((char *)&buf[0],"%02d:%02d:%02d",(int32_t)ps->DisValue[29]/3600,((int32_t)ps->DisValue[29]%3600)/60,((int32_t)ps->DisValue[29]%3600)%60);

	Screen_RefreshDataMulti(0x0160,(u8 *)&buf[0],15*2);
	
	sprintf((char *)&buf[0],"%-10.4f",ps->DisValue[6]);           //�̵�����
	Screen_RefreshDataMulti(0x0180,(u8 *)&buf[0],15*2);
	
	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[4]);//����
	Screen_RefreshDataMulti(0x01A0,(u8 *)&buf[0],15*2);
	
	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[11]);//��������
	Screen_RefreshDataMulti(0x01c0,(u8 *)&buf[0],15*2);
  /*
	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[35]);//��׼������
	ps->JZHYL=ps->DisValue[35];   //�ַ���ת���ɸ�����
	Screen_RefreshDataMulti(0x01C0,(u8 *)&buf[0],15*2);
	*/

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[7]);   //��ʪ��
	ps->HYL=ps->DisValue[7];   //�ַ���ת���ɸ�����
	Screen_RefreshDataMulti(0x01E0,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[1]);  //ƽ����ѹ
	Screen_RefreshDataMulti(0x0400,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[0]);//ƽ����ѹ
	Screen_RefreshDataMulti(0x0420,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[2]);//ƽ��ȫѹ
	Screen_RefreshDataMulti(0x0440,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[5]);//����ѹ
	Screen_RefreshDataMulti(0x0460,(u8 *)&buf[0],15*2);

    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[16]);//����
	Screen_RefreshDataMulti(0x0480,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[3]);//ƽ������
	Screen_RefreshDataMulti(0x04A0,(u8 *)&buf[0],15*2);
	
	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[37]);//��ѹ
	Screen_RefreshDataMulti(0x04C0,(u8 *)&buf[0],15*2);

  /*
	 sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[20]);//������
	Screen_RefreshDataMulti(0x04C0,(u8 *)&buf[0],15*2);
	*/
	
	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[13]);//������
	Screen_RefreshDataMulti(0x0500,(u8 *)&buf[0],15*2);
	
	/*
    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[38]);//��ʣϵ��
	ps->GSXS=ps->DisValue[38];   //�ַ���ת���ɸ�����
	if(ps->GSXS==1000000)  //����ı�־
	strcpy((char *)buf,"----------");
	Screen_RefreshDataMulti(0x0500,(u8 *)&buf[0],15*2);
	*/

    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[14]); //����ֱ��
	Screen_RefreshDataMulti(0x0520,(u8 *)&buf[0],15*2);

    sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[9]);//����ϵ��
	ps->ZSXS=atof((char *)&ps->Buffer[0]);   //�ַ���ת���ɸ�����
	
    /*
	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[10]);//����ϵ��
	ps->FHXS=ps->DisValue[10];   //�ַ���ת���ɸ�����
	Screen_RefreshDataMulti(0x0520,(u8 *)&buf[0],15*2);
	*/
                   //����Ũ��
	four.FloatData=CalculationFormula_Cc(ps->DisValue[32],ps->DisValue[33],ps->DisValue[18]/1000); //(��ҺŨ�ȣ�)			   
	sprintf((char *)&buf[0],"%-10.2f",four.FloatData);//�������
	Screen_RefreshDataMulti(0x0540,(u8 *)&buf[0],15*2);
	 //����Ũ��  0x760
	sprintf((char *)&buf[0],"%-10.2f",CalculationFormula_Yyzs(four.FloatData,ps->DisValue[12],ps->DisValue[34]));//������������ 
	Screen_RefreshDataMulti(0x0760,(u8 *)&buf[0],15*2);
	
	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[12]);//�������
	Screen_RefreshDataMulti(0x0580,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[28]);//��Ͳ��
	Screen_RefreshDataMulti(0x05A0,(u8 *)&buf[0],15*2);

    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[17]); //�������
	Screen_RefreshDataMulti(0x05C0,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[18]); //������
	Screen_RefreshDataMulti(0x05E0,(u8 *)&buf[0],15*2);


    /*
	sprintf((char *)&buf[0],"%-10.5f",ps->DisValue[19]);//�̳�����
	ps->YCJZ=ps->DisValue[19];   //�ַ���ת���ɸ�����
	Screen_RefreshDataMulti(0x0760,(u8 *)&buf[0],15*2);
     */
   
	

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[32]);//��ҺŨ��
	ps->RYLD=ps->DisValue[32];   //�ַ���ת���ɸ�����
	Screen_RefreshDataMulti(0x0700,(u8 *)&buf[0],15*2);
	
	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[33]);//�������
	ps->DRTZ=ps->DisValue[33];   //�ַ���ת���ɸ�����
    Screen_RefreshDataMulti(0x0720,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[34]);//��ͷ��Ŀ
	ps->ZTSM=ps->DisValue[34];   //�ַ���ת���ɸ�����
	Screen_RefreshDataMulti(0x0740,(u8 *)&buf[0],15*2);
	
		
    SdFile_Serch_MaxofGasLinkFile();//������������ļ���
    ps->DRWJH= SdFp-> GasLinkFileMax;

	Screen_ResetDataBuffer();
	strcpy((char *)ps->Buffer,"        ");//����ʾ20200519
	Screen_RefreshDataMulti(0x2E20,ps->Buffer,15*2);

	//��ʾ��ַ��Ϣ
	{
		
	strncpy((char*)ps->Buffer,(char *)&(SdFp->rdWorkFile.Adress[9]),30);
	}				
	 Screen_RefreshDataMulti(0x0560,ps->Buffer,15*2);
}
/* date 20200519
	��������ReadDustFirstData(u32 FileNum)
	������ ��ȡ�̳��ļ�-��һҳ
  ���룺
*/         
void ReadDustFirstData(u32 FileNum)//�����ļ��� ��ȡ�̳���Ϣ
{
  SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	SdFile_ReadDustFile(FileNum);

	
	FourBytes four;
	char buf[61]={0};
	strncpy((char *)buf,(char *)&(SdFp->rdWorkFile.Other[31][12]),10);//��������
	four.U32Data=atoi(buf);
	ps->YC_Type=four.U32Data;//��������
	
	Screen_ResetDataBuffer();
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //�ļ���
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.dat[0]),10);//����                 //
	Screen_RefreshDataMulti(0x0120,ps->Buffer,15*2);
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.dat[11]),5);//ʱ��
	Screen_RefreshDataMulti(0x0140,ps->Buffer,15*2);
	
//----------------------	
	//sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[29]);//�ۼƲ���ʱ��
	
	sprintf((char *)&buf[0],"%2d:%2d:%2d",(int32_t)ps->DisValue[29]/3600,((int32_t)ps->DisValue[29]%3600)/60,((int32_t)ps->DisValue[29]%3600)%60);//�ۼƲ���ʱ��
	
	Screen_RefreshDataMulti(0x0160,(u8 *)&buf[0],15*2);
	
	sprintf((char *)&buf[0],"%-10.4f",ps->DisValue[6]);           //�̵�����
	Screen_RefreshDataMulti(0x0180,(u8 *)&buf[0],15*2);
	
  
	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[4]);//����
	Screen_RefreshDataMulti(0x01A0,(u8 *)&buf[0],15*2);
	

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[35]);//��׼������
	ps->JZHYL=ps->DisValue[35];   //�ַ���ת���ɸ�����
	Screen_RefreshDataMulti(0x01C0,(u8 *)&buf[0],15*2);
	

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[7]);   //��ʪ��
	ps->HYL=ps->DisValue[7];   //�ַ���ת���ɸ�����
	Screen_RefreshDataMulti(0x01E0,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[1]);  //ƽ����ѹ
	Screen_RefreshDataMulti(0x0400,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[0]);//ƽ����ѹ
	Screen_RefreshDataMulti(0x0420,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[2]);//ƽ��ȫѹ
	Screen_RefreshDataMulti(0x0440,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[5]);//����ѹ
	Screen_RefreshDataMulti(0x0460,(u8 *)&buf[0],15*2);

    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[16]);//����
	Screen_RefreshDataMulti(0x0480,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[3]);//ƽ������
	Screen_RefreshDataMulti(0x04A0,(u8 *)&buf[0],15*2);

	 sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[20]);//������
	Screen_RefreshDataMulti(0x04C0,(u8 *)&buf[0],15*2);
	
	
    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[38]);//��ʣϵ��
	ps->GSXS=ps->DisValue[38];   //�ַ���ת���ɸ�����
	if(ps->GSXS==1000000)  //����ı�־
	strcpy((char *)buf,"----------");
	Screen_RefreshDataMulti(0x0500,(u8 *)&buf[0],15*2);
	
    sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[9]);//����ϵ��
	ps->ZSXS=atof((char *)&ps->Buffer[0]);   //�ַ���ת���ɸ�����
	
  
	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[10]);//����ϵ��
	ps->FHXS=ps->DisValue[10];   //�ַ���ת���ɸ�����
	Screen_RefreshDataMulti(0x0520,(u8 *)&buf[0],15*2);
	

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[11]);//��������
	Screen_RefreshDataMulti(0x0540,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[12]);//�������
	Screen_RefreshDataMulti(0x0580,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[28]);//��Ͳ��
	Screen_RefreshDataMulti(0x05A0,(u8 *)&buf[0],15*2);

    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[17]); //�������
	Screen_RefreshDataMulti(0x05C0,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[18]); //������
	Screen_RefreshDataMulti(0x05E0,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[13]);//������
	Screen_RefreshDataMulti(0x0700,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[37]);//��ѹ
	Screen_RefreshDataMulti(0x0720,(u8 *)&buf[0],15*2);

    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[14]); //����ֱ��
	Screen_RefreshDataMulti(0x0740,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.5f",ps->DisValue[19]);//�̳�����
	ps->YCJZ=ps->DisValue[19];   //�ַ���ת���ɸ�����
	Screen_RefreshDataMulti(0x0760,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[32]);//��ҺŨ��
	ps->RYLD=ps->DisValue[32];   //�ַ���ת���ɸ�����
	
	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[33]);//�������
	ps->DRTZ=ps->DisValue[33];   //�ַ���ת���ɸ�����

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[34]);//��ͷ��Ŀ
	ps->ZTSM=ps->DisValue[34];   //�ַ���ת���ɸ�����
		
  SdFile_Serch_MaxofGasLinkFile();//������������ļ���
  ps->DRWJH= SdFp-> GasLinkFileMax;

	Screen_ResetDataBuffer();
	strcpy((char *)ps->Buffer,"                              ");//����ʾ20200519
	Screen_RefreshDataMulti(0x2E20,ps->Buffer,15*2);

	//��ʾ��ַ��Ϣ
	{
		
	strncpy((char*)ps->Buffer,(char *)&(SdFp->rdWorkFile.Adress[9]),30);
	}				
	 Screen_RefreshDataMulti(0x0560,ps->Buffer,15*2);
}
//---
/* date 20200520
	��������void GetDustSecondData (u32 FileNum)
	������	��ȡ��ǰ�������� -�ڶ�ҳ
  ���룺void
*/
void GetDustSecondData(u32 FileNum)//��ȡ��ǰ��������
{
  SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	float dbf,dbf1;

	u8 buff[61]={0};
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //�ļ���
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);
	
	dbf=ps->DisValue[19];//�̳�����g
	if(ps->DisValue[17]!=0)                         //���L
	dbf=dbf*1000/ps->DisValue[17]*1000;  //                g/L =mg/m^3
	else
	dbf=0.0f;
	sprintf((char *)&ps->Buffer[0],"%10.2f",dbf); //�̳�Ũ��
	Screen_RefreshDataMulti(0x0120,ps->Buffer,15*2);
	          //�̳�����
	dbf1=dbf*ps->DisValue[38]/ps->DisValue[9]*ps->DisValue[10];  //����=Ũ��*��ʣϵ��/����ϵ��*����ϵ��
	sprintf((char *)&buff[0],"%10.2f",dbf1); //
	Screen_RefreshDataMulti(0x0540,(u8 *)&buff[0],15*2);
	//�̳��ŷ�
	dbf1=dbf*ps->DisValue[12]*0.000001f;  //����=Ũ��*��ʣϵ��/����ϵ��*����ϵ��
	sprintf((char *)&buff[0],"%10.2f",dbf1); //
	Screen_RefreshDataMulti(0x0520,(u8 *)&buff[0],15*2);

//	SdFile_ReadDustFile(FileNum);
   

	//NOX Ũ��
	if(ps->DisValue [21+1]==1000000||ps->DisValue[21+2]==1000000)
	{
		ps->DisValue[39]=1000000;
	}else
	{
		ps->DisValue[39]=CalculationFormula_NOX(ps->DisValue [21+1],ps->DisValue[21+2]);
	} 
	
	if((ps->DisValue[39]!=1000000))
	{
	  sprintf((char*)ps->Buffer,"%10.2f",ps->DisValue[39]);
	}else
	{
	  	sprintf((char*)ps->Buffer,"----------");
	}
 	Screen_RefreshDataMulti(0x01A0,ps->Buffer,15*2);
	//NOX ����
	dbf=ps->DisValue[39];
	if((ps->DisValue[39]!=1000000)&&(ps->DisValue[38]!=1000000))
	{
	dbf1=dbf*ps->DisValue[38]/ps->DisValue[9]*ps->DisValue[10];  //����=Ũ��*��ʣϵ��/����ϵ��*����ϵ��
    sprintf((char*)ps->Buffer,"%10.2f",dbf1); //
	}
	else
	{
	sprintf((char*)ps->Buffer,"----------");	
	}
	Screen_RefreshDataMulti(0x0480,ps->Buffer,15*2);
	//NOX �ŷ�
    if(ps->DisValue[39]!=1000000)
	{
	dbf1=dbf*ps->DisValue[12]*0.000001f;  //�ŷ�=Ũ��*�������*0.000001
    sprintf((char*)ps->Buffer,"%10.2f",dbf1); //
	}
	else
	{
		sprintf((char*)ps->Buffer,"----------");	
	}
	Screen_RefreshDataMulti(0x05C0,ps->Buffer,15*2);

  //SO2,NO,NO2,CO,H2S  Ũ��
	for(int i=0;i<5;i++)
	{
		if(ps->DisValue[21+i]!=1000000)  //��������ʼ
	{
	  sprintf((char*)ps->Buffer,"%10.2f ",ps->DisValue[21+i]);
	}else
	{
	  sprintf((char*)ps->Buffer,(char *)"----------");
	}

	if(i<3)   //0��1��2
	{
	 Screen_RefreshDataMulti(0x0140+i*0x20,ps->Buffer,15*2);
	}
	else if(i==3) //CO
	{
	Screen_RefreshDataMulti(0x01C0,ps->Buffer,15*2);	
	}
	else if(i==4) //H2S
	{
	Screen_RefreshDataMulti(0x0400,ps->Buffer,15*2);	
	}

  }

//SO2,NO,NO2,CO,H2S����	
for(int i=0;i<5;i++)
{
  if((ps->DisValue[21+i]!=1000000)&&(ps->DisValue[38]!=1000000))
	{ 
		dbf1=ps->DisValue[21+i]*ps->DisValue[38]/ps->DisValue[9]*ps->DisValue[10];  //����=Ũ��*��ʣϵ��/����ϵ��*����ϵ��
		sprintf((char*)ps->Buffer,"%8.1f",dbf1);
	}else
	{
	  	sprintf((char*)ps->Buffer,(char *)"----------");
	}
	
	if(i<3)
	{
		Screen_RefreshDataMulti(0x0420+i*0x20,ps->Buffer,15*2);
	}
	else if(i==3) //CO
	{
		Screen_RefreshDataMulti(0x04A0,ps->Buffer,15*2);
	}
	else if(i==4) //H2S
	{
		Screen_RefreshDataMulti(0x0500,ps->Buffer,15*2);
	}
}
	//SO2,NO,NO2,CO,H2S�ŷ�
for(int i=0;i<5;i++)
  {
		if(ps->DisValue[21+i]!=1000000)
		{
			sprintf((char*)ps->Buffer,"%10.2f",ps->DisValue[i]*0.000001f*ps->DisValue[12]); //�ŷ��� ����*�������*0.000001;

		}else
		{
			sprintf((char*)ps->Buffer,(char *)"----------");
		}
	if(i<3)
	{
		Screen_RefreshDataMulti(0x0560+i*0x20,ps->Buffer,15*2);
	}
	else if(i==3) //CO
	{
	 Screen_RefreshDataMulti(0x05E0,ps->Buffer,15*2);
	}
	else if(i==4)  //  H2S
	{
	Screen_RefreshDataMulti(0x0620,ps->Buffer,15*2);
	}
 }
	//CO2
 		if(ps->DisValue[21+5]!=1000000)
		{
			sprintf((char*)ps->Buffer,"%10.2f",ps->DisValue[21+5]); //Ũ��

		}else
		{
			sprintf((char*)ps->Buffer,(char *)"----------");
		}
	   Screen_RefreshDataMulti(0x01E0,ps->Buffer,15*2);
}
//---
/* date 20200520
	��������void GetDustJS_INpuData (u32 FileNum)
	������	��ȡ��ǰ���㵼�������ļ�������������
  ���룺FileNum�����ļ���
	      Screen_Data.DRWJH ������ļ���
  �����0,����ʧ��; 1����ɹ�
*/
u8 GetDustJS_INpuData(u32 FileNum)
{
  SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
//	char buff[61]={0};
	
	if(FileNum==0)  //�޵����ļ�������ʧ��
		return 0;
	FourBytes four;
	if(SdFile_ReadGasFile(FileNum,0x01)==FR_OK)  //��ȡ��������
	{
		for(int i=0;i<7;i++)
		{
	 	Screen_ResetDataBuffer();
	    strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[13+i][12]),10);//������
		four.FloatData=atof((char *)ps->Buffer);
		ps->DisValue[20+i]=	four.FloatData;
		}
	  return 1;//����ɹ�
	}
	else
	{
		return 0;//�޵����ļ�������ʧ��
	}
	
}
//---
 /* date 2020-03-10
	�������SaveGasJS_InputDataLoad(float *SaveData)
	������	���������������
  ���룺
	
*/
 void SaveGasJS_InputDataLoad(float *SaveData)
{

	Screen_DataStructure* ps=Screen_GetData();
	SdFile_DataStruct* SdFp=SdFile_GetData();
	char buff[100];
	FourBytes four;
	for(int i=0;i<39;i++)
	{
		strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[i][12]),10);//ԭʼ����װ��
		four.FloatData=atof(buff);
		SaveData[i]=four.FloatData;
	}
	   //װ�������̳�����		
		

      SaveData[9]  = GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSZSXS);//����ϵ��
			SaveData[10] = GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSFHXS);//����ϵ��
			SaveData[19]=GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSYCZL);//����				
			SaveData[20]=GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSHYL);//������
			SaveData[35]=GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSJZYHL); //��׼��������
 
		

}
//---
//-------------------------------------------------����Ϊ��ӡ�ӿ�----------------------------------

/* date 2020-03-25
	��������PrintPara(float *PrintData)
	������	��ӡ�����������ó���
  ���룺
	
*/
void PrintPara(float *PrintData,u8 Max)
{     u8 buf[80]={0};
      u8 i;
      Printer_Structure *Pp=Printer_GetData();
      Screen_DataStructure* ps=Screen_GetData();

      SdFile_ramlodair((char *)&Pp->bianhao[0],9);
      sprintf((char *)&buf[0],"%08d",(int32_t)PrintData[0]);   //�������
      strncpy((char *)&Pp->bianhao[0],(char *)&buf[0],8);	   //��ų���Ϊ8
		//	strcpy((char *)&Pp->bianhao[0],(char *)"12345678");	
      SdFile_ramlodair((char *)&Pp->dat[0],17);
      sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min);
			strncpy((char *)&Pp->dat[0],(char*)buf,16);
			
			
			for(i=0;i<Max;i++)
			{
			SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i],11);
			sprintf((char *)&buf[0],"%10.0f",PrintData[i+1]);   //
			strncpy((char *)&Pp->Para_StrData.databuf[i],(char *)&buf[0],10);
			}

}


/* date 2020-03-25
	��������PrintPressurePara(float *PrintData)
	������	��ӡѹ������
  ���룺  �������    PrintData[0]  =8λ
          ����ѹ���  PrintData[1]  ,��λС��
          ��ѹ���    PrintData[2]  ,��λС��  B·��ѹ
          ��ѹ���    PrintData[3]  ,��λС��  A·��ѹ
          ��ѹ���    PrintData[4]  ,��С��  A·��ѹ
          ����ѹ���  PrintData[5]  ,��С��  B·��ѹ
          ����ѹ���  PrintData[6]  ,��С��  B·��ѹ


	
*/
void PrintPressurePara(float *PrintData)
{    
     // PrintPara(PrintData,6);
	    u8 buf[80]={0};
      u8 i;
      Printer_Structure *Pp=Printer_GetData();
      Screen_DataStructure* ps=Screen_GetData();

      SdFile_ramlodair((char *)&Pp->bianhao[0],9);
      sprintf((char *)&buf[0],"%08d",(int32_t)PrintData[0]);   //�������
      strncpy((char *)&Pp->bianhao[0],(char *)&buf[0],8);	   //��ų���Ϊ8
		//	strcpy((char *)&Pp->bianhao[0],(char *)"12345678");	
      SdFile_ramlodair((char *)&Pp->dat[0],17);
      sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min);
			strncpy((char *)&Pp->dat[0],(char*)buf,16);
			//strcpy((char *)&Pp->dat[0],(char *)"2019-12-20 14:20");
			
			for(i=0;i<3;i++)  
			{
			SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i],11);
			sprintf((char *)&buf[0],"%10.2f",PrintData[i+1]);   // ��λС��
			strncpy((char *)&Pp->Para_StrData.databuf[i],(char *)&buf[0],10);
			}
			for(i=3;i<6;i++)
			{
			SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i],11);
			sprintf((char *)&buf[0],"%10d",(int32_t)PrintData[i+1]);   //��С��
			strncpy((char *)&Pp->Para_StrData.databuf[i],(char *)&buf[0],10);
			}

     Print_PressureParaOrder(Printer_GetData());
}
/* date 2020-03-25
	��������PrintTemperaturePara(float *PrintData)
	������	��ӡ�¶Ȳ���
  ���룺
          �������       PrintData[0]  =8λ
          ����/�������  PrintData[1] ,��λС��
          ����/���±���  PrintData[2]  ,��λС��
          ʪ�����       PrintData[3]   ,��λС��
          ʪ�� ����      PrintData[4]   ,��λС��
          �������       PrintData[5]   ,��λС��
  
*/
void PrintTemperaturePara(float *PrintData)
{
	  // PrintPara(PrintData,6);
	 	  u8 buf[80]={0};
      u8 i;
      Printer_Structure *Pp=Printer_GetData();
      Screen_DataStructure* ps=Screen_GetData();

      SdFile_ramlodair((char *)&Pp->bianhao[0],9);
      sprintf((char *)&buf[0],"%08d",(int32_t)PrintData[0]);   //�������
      strncpy((char *)&Pp->bianhao[0],(char *)&buf[0],8);	   //��ų���Ϊ8
		//	strcpy((char *)&Pp->bianhao[0],(char *)"12345678");	
      SdFile_ramlodair((char *)&Pp->dat[0],17);
      sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min);
			strncpy((char *)&Pp->dat[0],(char*)buf,16);
			//strcpy((char *)&Pp->dat[0],(char *)"2019-12-20 14:20");
			
			for(i=0;i<5;i++)
			{
			SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i],11);
			sprintf((char *)&buf[0],"%10.2f",PrintData[i+1]);   //��λС��
			strncpy((char *)&Pp->Para_StrData.databuf[i],(char *)&buf[0],10);
			}
			
	   Print_TemperatureParaOrder(Printer_GetData());
}

/* date 2020-03-25
	��������PrintFlowPara(float *PrintData)
	������	��ӡ��������
  ���룺 
          �������       PrintData[0]   =8λ
          �����¶�       PrintData[1]   1λС��
          ����ѹ         PrintData[2]   1λС��
          1.0L           PrintData[3]   4λС��
          20L            PrintData[4]   4λС��
          30L            PrintData[5]   4λС��
          40L            PrintData[6]   4λС��
          50L            PrintData[7]   4λС��
          60L            PrintData[8]   4λС��
          70L            PrintData[9]   4λС��
          80L            PrintData[10]  4λС��
          90L            PrintData[11]  4λС��
          100L           PrintData[12]  4λС��
	
*/
void PrintFlowPara(float *PrintData)
{
	// PrintPara(PrintData,12);
	  u8 buf[80]={0};
      u8 i;
      Printer_Structure *Pp=Printer_GetData();
      Screen_DataStructure* ps=Screen_GetData();

      SdFile_ramlodair((char *)&Pp->bianhao[0],9);
      sprintf((char *)&buf[0],"%08d",(int32_t)PrintData[0]);   //�������
      strncpy((char *)&Pp->bianhao[0],(char *)&buf[0],8);	   //��ų���Ϊ8
		//	strcpy((char *)&Pp->bianhao[0],(char *)"12345678");	
      SdFile_ramlodair((char *)&Pp->dat[0],17);
      sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min);
			strncpy((char *)&Pp->dat[0],(char*)buf,16);
			//strcpy((char *)&Pp->dat[0],(char *)"2019-12-20 14:20");
			
			for(i=0;i<2;i++)
			{
			SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i],11);
			sprintf((char *)&buf[0],"%10.1f",PrintData[i+1]);   //��λС��
			strncpy((char *)&Pp->Para_StrData.databuf[i],(char *)&buf[0],10);
			}
			for(i=2;i<12;i++)
			{
			SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i],11);
			sprintf((char *)&buf[0],"%10.4f",PrintData[i+1]);   //4λС��
			strncpy((char *)&Pp->Para_StrData.databuf[i],(char *)&buf[0],10);
			}
	
	    Print_FlowParaOrder(Printer_GetData());
}
/* date 2020-03-25
	��������PrintConcentraPara(float *PrintData)
	������	��ӡŨ�Ȳ���
  ���룺
  �������        PrintData[0]   =8λ
	" O2     ���:",PrintData[1] ,��С��
	" O2 �Ͷ˱���:",PrintData[2] ,4λС��
	" O2 �ж˱���:",PrintData[3] ,4λС��
	" O2 �߶˱���:",PrintData[4] ,4λС��
	"SO2     ���:",PrintData[5] ,��С��
	"SO2 �Ͷ˱���:",PrintData[6] ,4λС��
	"SO2 �ж˱���:",PrintData[7],4λС��
	"SO2 �߶˱���:",PrintData[8],4λС��
	" NO     ���:",PrintData[9]  ,��С��
	" NO �Ͷ˱���:",PrintData[10],4λС��
	" NO �ж˱���:",PrintData[11],4λС��
	" NO �߶˱���:",PrintData[12],4λС��
	"NO2     ���:",PrintData[13],��С��
	"NO2 �Ͷ˱���:",PrintData[14],4λС��
	"NO2 �ж˱���:",PrintData[15],4λС��
	"NO2 �߶˱���:",PrintData[16],4λС��
	" CO     ���:",PrintData[17],��С��
	" CO �Ͷ˱���:",PrintData[18],4λС��
	" CO �ж˱���:",PrintData[19],4λС��
	" CO �߶˱���:",PrintData[20],4λС��
	"H2S     ���:",PrintData[21],��С��
	"H2S �Ͷ˱���:",PrintData[22],4λС��
	"H2S �ж˱���:",PrintData[23],4λС��
	"H2S �߶˱���:",PrintData[24],4λС��
	"CO2     ���:",PrintData[25],��С��
	"CO2 �Ͷ˱���:",PrintData[26],4λС��
	"CO2 �ж˱���:",PrintData[27],4λС��
	"CO2 �߶˱���:",PrintData[28],4λС��
	
*/
void PrintConcentraPara(float *PrintData)
{
	// PrintPara(PrintData,29);
	   u8 buf[80]={0};
      u8 i;
      Printer_Structure *Pp=Printer_GetData();
      Screen_DataStructure* ps=Screen_GetData();

      SdFile_ramlodair((char *)&Pp->bianhao[0],9);
      sprintf((char *)&buf[0],"%08d",(int32_t)PrintData[0]);   //�������
      strncpy((char *)&Pp->bianhao[0],(char *)&buf[0],8);	   //��ų���Ϊ8
		//	strcpy((char *)&Pp->bianhao[0],(char *)"12345678");	
      SdFile_ramlodair((char *)&Pp->dat[0],17);
      sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min);
	  strncpy((char *)&Pp->dat[0],(char*)buf,16);
			//strcpy((char *)&Pp->dat[0],(char *)"2019-12-20 14:20");
			
			for(i=0;i<7;i++)
			{
				
				SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i*4],11);
				
				if((i==0)||(i==6))
				sprintf((char *)&buf[0],"%10.1f",PrintData[i*4+1]);   //��λС��
				else
				sprintf((char *)&buf[0],"%10.0f",PrintData[i*4+1]);   //��λС��
				
				strncpy((char *)&Pp->Para_StrData.databuf[i*4],(char *)&buf[0],10);

				SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i*4+1],11);
				sprintf((char *)&buf[0],"%10.3f",PrintData[i*4+3]);   //4λС��       //���Ͷ�Ũ��ֵ��modify by alfred, 2020.07.09
				strncpy((char *)&Pp->Para_StrData.databuf[i*4+1],(char *)&buf[0],10);	//��ʾ�Ͷ�Ũ��ֵ

				SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i*4+2],11);
				sprintf((char *)&buf[0],"%10.3f",PrintData[i*4+2]);   //4λС��			//���ж�Ũ��ֵ��modify by alfred, 2020.07.09
				strncpy((char *)&Pp->Para_StrData.databuf[i*4+2],(char *)&buf[0],10);	//��ʾ�ж�Ũ��ֵ

				SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i*4+3],11);
				sprintf((char *)&buf[0],"%10.3f",PrintData[i*4+4]);   //4λС��
				strncpy((char *)&Pp->Para_StrData.databuf[i*4+3],(char *)&buf[0],10);
					
			}
		
	
	 Print_ConcentraParaOrder(Printer_GetData());
}
/* date 2020-03-25
	��������PrintCurTest(float *PrintData)
	������	��ӡ���ڲ��Բ���
  ���룺PrintData ��������ָ�룬Ps ��ӡѡ���־ָ�룬Is Ӳ����װ��־ָ��

         �������        PrintData[0]   =8λ
O2Ũ��          :PrintData[1]
SO2Ũ��          :PrintData[2]
NOŨ��           :PrintData[3]
NO2Ũ��          :PrintData[4]
NOXŨ��          :PrintData[5]
COŨ��           :PrintData[6]
H2SŨ��          :PrintData[7]
CO2Ũ��          :PrintData[8]
	
*/
void PrintCurTest(float *PrintData,u8 *Ps,u8 *Is)
{
	u8 buf[80]={0};
	u8 i;
	Printer_Structure *Pp=Printer_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	SdFile_ramlodair((char *)&Pp->bianhao[0],9);
	sprintf((char *)&buf[0],"%8d",(int32_t)PrintData[0]);   //�������
	strncpy((char *)&Pp->bianhao[0],(char *)&buf[0],8);	   //��ų���Ϊ8
	//	strcpy((char *)&Pp->bianhao[0],(char *)"12345678");	
	SdFile_ramlodair((char *)&Pp->dat[0],17);               
	sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min);   //��ǰʱ��
	strncpy((char *)&Pp->dat[0],(char*)buf,16);
	//strcpy((char *)&Pp->dat[0],(char *)"2019-12-20 14:20");

	//װ�ص�ַ
	SdFile_ramlodair((char *)&Pp->address[0],51);  //�����ո�49+/r/n
	sprintf((char *)&buf[0],"%-30s",(char *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));  //
	strncpy((char *)&Pp->address[0],(char *)&buf[0],30);   //flash��ֻ��30����ַ����

	for(i=0;i<8;i++)
	{
		SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i],11);
		if(PrintData[i+1]==1000000)                                             //20210306  
		sprintf((char *)&buf[0],"----------");
		else
        {if(i==0)
		 sprintf((char *)&buf[0],"%10.0f",PrintData[i+1]);   //
         else			
		 sprintf((char *)&buf[0],"%10.1f",PrintData[i+1]);   //
		}
		strncpy((char *)&Pp->Para_StrData.databuf[i],(char *)&buf[0],10);
	}
	
	for(i=0;i<8;i++)
	{
		Pp->installselect[i]=Is[0];
		Is++;
	}
	for(i=0;i<33;i++)
	{
		Pp->pselect[i]=Ps[0];	
		Ps++;
	}
			
	Print_CurTestOrder(Printer_GetData());

}
/* date 2020-03-25
	�������PPrintWorkFile(u32 starfile,u32 endfile,u8 *Ps)
	������	��ӡ�ļ���������
���룺  strfile :��ʼ�ļ���
        endfile :�����ļ���
        ps :��ӡѡ������ָ��
        PrintData[0]: �������
        PrintData[1]: ��ʼ�ĵ�
        PrintData[2]: �����ĵ�
        PrintData[3]: 
*/
void PrintFile_com(u8 *Ps,float *PrintData)
{
	u8 buf[80]={0};
	u8 i;
	u32 file_tmp;
	Printer_Structure *Pp=Printer_GetData();
	Screen_DataStructure* ps=Screen_GetData();

	Pp->print_start_file=(u32)PrintData[1];
	Pp->print_end_file=(u32)PrintData[2];
	Pp->print_FileNomber=(u32)PrintData[3];
	
	if(Pp->print_start_file > Pp->print_end_file)//�����ļ���
	{
		file_tmp = Pp->print_end_file;
		Pp->print_end_file = Pp->print_start_file;
		Pp->print_start_file = file_tmp;
		Pp->print_FileNomber = Pp->print_start_file;
	}

	SdFile_ramlodair((char *)&Pp->bianhao[0],9);
	sprintf((char *)&buf[0],"%08d",(int32_t)PrintData[0]);   //�������
	strncpy((char *)&Pp->bianhao[0],(char *)&buf[0],8);	   //��ų���Ϊ8
	//	strcpy((char *)&Pp->bianhao[0],(char *)"12345678");	
	SdFile_ramlodair((char *)&Pp->dat[0],17);               
	sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min);   //��ǰʱ��
	strncpy((char *)&Pp->dat[0],(char*)buf,16);
	//strcpy((char *)&Pp->dat[0],(char *)"2019-12-20 14:20");


	for(i=0;i<33;i++)
	{
		Pp->pselect[i]=Ps[0];	
		Ps++;
	}
	
}
/* date 2020-03-25
	�������PPrintWorkFile(u32 starfile,u32 endfile,u8 *Ps)
	������	��ӡ�����ļ�
���룺  strfile :��ʼ�ļ���
        endfile :�����ļ���
        ps :��ӡѡ������ָ��
        PrintData[0]: ��ʼ�ļ�
        PrintData[1]: �����ļ�

*/
void PrintWorkFile(u8 *Ps,float *PrintData)
{ 
  Printer_GetData()->PrintSelect=0;	
	PrintFile_com(Ps,PrintData);
	Print_WorkFileOrder(Printer_GetData());
}
/* date 2020-03-25
	�������PPrintGasFile(float *PrintData)
	������	��ӡ�����ļ�

  ���룺  
	
*/
void PrintGasFile(u8 *Ps,float *PrintData)
{
	Printer_GetData()->PrintSelect=0;
	PrintFile_com(Ps,PrintData);
	Print_GasFileOrder(Printer_GetData());
}
/* date 2020-03-25
	�������PPrintDustFile(float *PrintData)
	������	��ӡ�����ļ�
  ���룺
	
*/
void PrintDustFile(u8 *Ps,float *PrintData)
{
	Printer_GetData()->PrintSelect=0;
	PrintFile_com(Ps,PrintData);
	Print_DustFileOrder(Printer_GetData());
}
/* date 2020-03-25
	�������PPrintGasRecordFile
	������	��ӡ������¼�ļ�
  ���룺
	
*/
void PrintGasRecordFile(u8 *Ps,float *PrintData)
{
	PrintFile_com(Ps,PrintData);

	Print_GasRecordFileOrder(Printer_GetData());
}
/* 	date 20200528
	��������Screen_WaterPumOpenServo(void) 
	������	 ����ˮ���߳�
  	���룺void
*/
void Screen_WaterPumOpenServo(void)  //����ˮ���߳�
{
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	if(pf->Data[FLASHIS_AdrXTSZBOX ]&0x08)  //�ж��Ƿ�ѡ���Զ���ˮ
	ps->WaterPumRunmod=1;
}
/* 	date 20200528
	��������Screen_WaterPumCloseServo(void)
	������	�ر�ˮ���߳�
  	���룺void
*/
void Screen_WaterPumCloseServo(void)//�ر�ˮ���߳�
{
	Screen_DataStructure* ps=Screen_GetData();
	ps->WaterPumRunmod=4;
}


/* 	date 20200528
	��������Screen_WaterPumServo(void)
	������	��ˮ�õļ���ŷ�
  	���룺void
*/
void Screen_WaterPumServo(void)
{  
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	switch(ps->WaterPumRunmod)
	{
		case 0:
		{
			break;
		}
		case 1:  //��ʼ��  װ�ض�ʱ��
		{
			ps->WaterPumTimer=pf->Data[FLASHIS_AdrXTSZZDPSJG]*60;
			if(ps->WaterPumTimer!=0)    //���ʱ��Ϊ0����ȫ������
			Slave_CloseWaterPump();       //��ˮ��20200528
			ps->WaterPumRunmod=2;
			break;
		}
		case 2:
		{  if(ps->WaterPumTimer==0)
			{
			ps->WaterPumTimer=pf->Data[FLASHIS_AdrXTSZZDPSSC];
			Slave_OpenWaterPump();       //��ˮ��20200528
			ps->WaterPumRunmod=3;
			}
	
			break;
		}
		case 3:
		{
			if(ps->WaterPumTimer==0)
			{
			ps->WaterPumRunmod=1;
			}
			break;
		}
		case 4:
		{
			Slave_CloseWaterPump();       //��ˮ��20200528
			ps->WaterPumRunmod=0;
			break;
		}
		default:
		{
			break;
		}
		
	}
	
}


//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Print_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪAT.c�ļ���ASCII�����ļ�
						:000|09|001
*/

const float Testdata[50]={1,1,1,0.333,0.444,0.555,0.666,0.777,0.888,0.999,
	                 1.000,1.111,1.222,1.333,1.444,1.555,1.666,1.777,1.888,1.999,
		               2.000,3601,2.222,2.333,2.444,2.555,2.666,2.777,2.888,2.999,
		               3.000,3.111,3.222,3.333,3.444,3.555,3.666,3.777,3.888,3.999,
		               4.000,4.111,4.222,4.333,4.444,4.555,4.666,1.777,4.888,1.999,
		              };
const u8 PN33[33]={1,1,1,1,1,1,1,1,1,1,1,
	                 1,1,1,1,1,1,1,1,1,1,1,
	                 1,1,1,1,1,1,1,1,1,1,1
                 };
const u8 IN8[8]={
	               1,1,1,1,1,1,1,1
                 };


void ScreeDisCalculate_DebugASCII(u8 *p,u8 len)
{ //u8 i;
	//char buf[202];
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{  //���Դ�ӡ�ӿ�
		case 1:
		{//
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
				  PrintPressurePara((float *)&Testdata[0]);
				  printf("���ڴ�ӡѹ������");
			}
			else
			{
					printf("����͸��ģʽ");
			}
			break;
		}
		case 2:
		{//
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
				PrintTemperaturePara((float *)&Testdata[0]);
				printf("���ڴ�ӡ�¶Ȳ���");
			}
			else
			{
					printf("����͸��ģʽ");
			}
			break;
		}
		case 3:
		{//
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
				PrintFlowPara((float *)&Testdata[0]);
				printf("���ڴ�ӡ��������");
			}
			else
			{
					printf("����͸��ģʽ");
			}
			break;
		}
		case 4:
		{//
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
				PrintConcentraPara((float *)&Testdata[0]);
				printf("���ڴ�ӡŨ�Ȳ���");
			}
			else
			{
					printf("����͸��ģʽ");
			}
			break;
		}
		case 5:
		{//
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
				PrintCurTest((float *)&Testdata[0],(u8 *)&PN33[0],(u8 *)&IN8[0]);
				printf("���ڴ�ӡ���ڲ�������");
			}
			else
			{
					printf("����͸��ģʽ");
			}
			break;
		}
		case 6:
		{//
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
				PrintWorkFile((u8 *)&PN33[0],(float *)&Testdata[0]);
				printf("���ڴ�ӡ�����ļ�");
			}
			else
			{
					printf("����͸��ģʽ");
			}
			break;
		}
		case 7:
		{//
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
				PrintGasFile((u8 *)&PN33[0],(float *)&Testdata[0]);
				printf("���ڴ�ӡ�����ļ�");
			}
			else
			{
					printf("����͸��ģʽ");
			}
			break;
		}
		case 8:
		{//
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
				PrintDustFile((u8 *)&PN33[0],(float *)&Testdata[0]);
				printf("���ڴ�ӡ�̳��ļ�");
			}
			else
			{
					printf("����͸��ģʽ");
			}
			break;
		}
		case 9:
		{//
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
				PrintGasRecordFile((u8 *)&PN33[0],(float *)&Testdata[0]);
				printf("���ڴ�ӡ������¼");
			}
			else
			{
				printf("����͸��ģʽ");
			}
			break;
		}
	}
}

/* 	date 2020-05-13 hjl add
	��������void GetKjMm (void)
	������	��ȡ��������
  	���룺void
*/
u16 GetKjMm(void)
{
	int cnt = 0;
	u8 bf[10];
	FourBytes four;
	u16 code;
	Flashisp_DataStructure* pf = Flashisp_GetData();
	Screen_DataStructure* ps = Screen_GetData();

	four.U16Data[0]=pf->Data[FLASHIS_AdrCJWHYQBH];
	four.U16Data[1]=pf->Data[FLASHIS_AdrCJWHYQBH+1];

	bf[cnt++] = four.U16Data[0] & 0xFF;
	bf[cnt++] = (four.U16Data[0] >> 8) & 0xFF;
	bf[cnt++] = four.U16Data[1] & 0xFF;
	bf[cnt++] = (four.U16Data[1] >> 8) & 0xFF;
	bf[cnt++] = ps->RandomNum & 0xFF;
	bf[cnt++] = (ps->RandomNum >> 8) & 0xFF;

	code = CRC16(bf, cnt);
	if(code > 9999)
	{
		code = (code / 10) + (code % 10);
	}
	else if(code < 1000)
	{
		code = code + 1000;
	}
	return code;
}


/* 	date 2020-05-14 hjl add
	��������void GetRandomNum (void)
	������	��ȡ4λ�����֤��
  	���룺void
*/
u16 GetRandomNum(void)
{
    u16 Random;
	u32 seed;
	
	Screen_DataStructure *ps = Screen_GetData();
	struct tm *pt = &ps->Clock.Clock;

	//��ʱ��������֮��Ϊ����
	seed = (pt->tm_sec << 24) + (pt->tm_min << 16) + (pt->tm_hour << 8) + pt->tm_mday + Random;
    srand((unsigned long)seed);
    Random = (u16)(rand() % (9999 - 1000 + 1) + 1000);	//����1000~9999�����
    ps->RandomNum = Random;
    return Random;
}

//----2020-05-27
/*
	��������u8 ProcessBlueToothInitial(u32 lPassword)
	������	�ú������ڴ���������ʼ����������ģ����������趨������ģʽ�趨��������λ�Ĳ�����
	���룺	�����������
	Ӱ�죺	����ģʽ�ṹ���ж�ʱ����ֵ��
	�޸ļ�¼�� 1. 2020-05-25, ����, Add by alfred.
*/
u8 ProcessBlueToothInitial(u32 lPassword)
{  Screen_DataStructure* p = Screen_GetData();
	u8 res = BT_DEFAULT; //resĬ�Ϸ���0�� ��ʱ����1�� ��ʼ���ɹ�����2
	u8 cTempBuffer[6];
	switch(g_BlueToothInitStateMachine)
	{
		case BLUETOOTH_INIT_STATE_MACHINE_INPUT_PASSWORD:
		{
			sprintf((char *)cTempBuffer,"%d",lPassword);
			if(AT_GetData()->Mode==1)
				AT_Password_Com(cTempBuffer); //��������
			g_BlueToothInitStateMachine = BLUETOOTH_INIT_STATE_MACHINE_PASSWORD_WAITOK;
			AT_GetData()->EnableBTtimer = true;
			AT_GetData()->sBTtimer = 3000;					
		}break;
		
		//���������ȴ���ӦOK
		case BLUETOOTH_INIT_STATE_MACHINE_PASSWORD_WAITOK:
		{
			if(AT_GetData()->OkFLG)
			{
				AT_GetData()->sBTtimer = 500;
				g_BlueToothInitStateMachine = BLUETOOTH_INIT_STATE_MACHINE_SET_MODE;
				DEBUG_MESSAGE("Set password ok!");			
			}
			else if(AT_GetData()->sBTtimer == 0)
			{
				AT_GetData()->EnableBTtimer = false;
				AT_GetData()->sBTtimer = 0;
				g_BlueToothInitStateMachine = BLUETOOTH_INIT_STATE_MACHINE_INPUT_PASSWORD;
				res = BT_TIME_OUT;
				DEBUG_MESSAGE("Set password fail!");				
			}

		}break;
		
		//������ģʽ
		case BLUETOOTH_INIT_STATE_MACHINE_SET_MODE:
		{
			if(AT_GetData()->sBTtimer == 0) //��ʱ500ms��ִ����һ������
			{
				if(AT_GetData()->Mode==1)
					AT_Role_Com();	//������ģʽ
				AT_GetData()->sBTtimer = 10000;
				g_BlueToothInitStateMachine = BLUETOOTH_INIT_STATE_MACHINE_SET_MODE_WAITOK;
			}
		}break;	
		
		//��������ģʽ��ȴ���ӦOK
		case BLUETOOTH_INIT_STATE_MACHINE_SET_MODE_WAITOK:
		{
			if(AT_GetData()->OkFLG)
			{
				AT_GetData()->sBTtimer = 10;
				g_BlueToothInitStateMachine = BLUETOOTH_INIT_STATE_MACHINE_RESET_DEVICE;
				DEBUG_MESSAGE("Set mode ok!");				
			}
			else if(AT_GetData()->sBTtimer == 0)
			{
				AT_GetData()->EnableBTtimer = false;
				AT_GetData()->sBTtimer = 0;
				g_BlueToothInitStateMachine = BLUETOOTH_INIT_STATE_MACHINE_INPUT_PASSWORD;
				res = BT_TIME_OUT;
				DEBUG_MESSAGE("Set mode fail!");			
			}
		}break;			
		
		//����ģ�鸴λ
		case BLUETOOTH_INIT_STATE_MACHINE_RESET_DEVICE:
		{
			if(AT_GetData()->sBTtimer == 0) //��ʱ500ms��ִ����һ������
			{
				if(AT_GetData()->Mode==1)
					AT_Reset_Com();	//ģ�鸴λ
				AT_GetData()->sBTtimer = 10000;
				g_BlueToothInitStateMachine = BLUETOOTH_INIT_STATE_MACHINE_WAIT_FINISH;
			}			
		}
		break;
		
		//�ȴ���ӦOK��������г�ʼ����
		case BLUETOOTH_INIT_STATE_MACHINE_WAIT_FINISH:
		{
			if(AT_GetData()->OkFLG)
			{
				AT_GetData()->EnableBTtimer = false;
				AT_GetData()->sBTtimer = 0;
				g_BlueToothInitStateMachine = BLUETOOTH_INIT_STATE_MACHINE_INPUT_PASSWORD;
				DEBUG_MESSAGE("Bluetooth initial OK!");				
				res = BT_OK;
			}
			else if(AT_GetData()->sBTtimer == 0)
			{
				AT_GetData()->EnableBTtimer = false;
				AT_GetData()->sBTtimer = 0;
				g_BlueToothInitStateMachine = BLUETOOTH_INIT_STATE_MACHINE_INPUT_PASSWORD;				
				res = BT_TIME_OUT;
				DEBUG_MESSAGE("Set initial fail!");				
			}
			break;
		}		
		
		default :break;			
	}
	return res;	
}


//----2020-12-01
/*
	��������u8 ProcessE28Initial(u16 adr,u8 ch)
	������	��������E28��ַ���ŵ�
	���룺	�����������
	Ӱ�죺	����ģʽ�ṹ���ж�ʱ����ֵ��
	�޸ļ�¼�� 1. 2020.12.01, ����, Add by linzequan.
*/

u8 ProcessE28Initial(u16 adr,u8 ch)
  {   Screen_DataStructure* p = Screen_GetData();
	  u8 res = 0; //resĬ�Ϸ���0�� ��ʱ����1�� ��ʼ���ɹ�����2
	  switch(p->g_E28_Set_StateMachine)
	  {
		  case E28_SET_STATE_MACHINE_SLEEP:
			  
			   break;
		  case E28_SET_STATE_MACHINE_INIT:
			   //�򸱰巢������E28����
			   Slave_GetData()->E28.STATE=1;
		       p->uE28Timer=2000;  //������
		       p->g_E28_Set_StateMachine=E28_SET_STATE_MACHINE_WAITOK;
			   break;
		  case E28_SET_STATE_MACHINE_WAITOK:
			   if(p->uE28Timer==0)
			   { //��ʱ��
				   res=1;  //��ʱ
				   p->g_E28_Set_StateMachine=E28_SET_STATE_MACHINE_SLEEP;
			   }
			   else  //ʱ��û�������
			   {
				   if((Slave_GetData()->E28.ADRRset==Slave_GetData()->E28.ADRRRD24G)&&(Slave_GetData()->E28.CHset==Slave_GetData()->E28.CHRD24G))
				   { //�����뷴��һ��
					  res=2; 
					  p->g_E28_Set_StateMachine=E28_SET_STATE_MACHINE_SLEEP;
				   }
			   }
			   break;
		  case E28_SET_STATE_MACHINE_FINISH:
			  
		       break;
	  }
	  
	  return res;
  }

/* 	date: 2020-06-01
	��������u8 TemperatureCaribration(u8 *pWarning, Flashisp_DataStructure *pf, u8 MenuPos)
	������	�ú������ڴ������ⲿ����У׼�¶ȡ�
	���룺	ps->warning, FLASH�ṹ��ָ��,�˵����λ�á�
	Ӱ�죺	��Ļ�ṹ�壬flash�ṹ�塣
	�޸ļ�¼�� 1. 2020-06-01, ����, Add by alfred.
*/
u8 TemperatureCaribration(u8 *pWarning, u8 MenuPos)
{
	u16 sValueADC;	
	Flashisp_DataStructure* pf=Flashisp_GetData();
	static TEMP_POINT ywTempPointY1;	//Y1Ϊʵ�ʲ��ĵ�(���ⲿ����ֱ���100ŷ��174ŷʱ��õ�ʵ���¶�ֵ����Y2Ϊ�����¶�ֵ��

	switch(*pWarning)
	{
		case 0: 
		{
			*pWarning = 0xA1;	//��ʾ����100ŷ����.
			break;
		}
		case 0xA1:
		{
			if(MenuPos == 1) sValueADC = Slave_GetData()->Data[SLAVE_MAP_YWTem].Raw;	//����100ŷʱ���µ�ADCֵ
			else if(MenuPos == 2) sValueADC = Slave_GetData()->Data[SLAVE_MAP_SQTem].Raw; //����100ŷʱʪ���ADCֵ
			
			if((sValueADC>768)&&(sValueADC<855)) //������Χ��
			{
				*pWarning = 0xA3;	//��ʾ����100ŷ����
				
				ywTempPointY1.A=sValueADC;   //
		
			}
			else
			{
				*pWarning = 0xAA;	//100ŷ�����������
			}
			break;
		}
		case 0xA3:
		{
			if(MenuPos == 1) 
			{
				sValueADC = Slave_GetData()->Data[SLAVE_MAP_YWTem].Raw;	//����174ŷʱ���µ�ADCֵ
				
			}
			else if(MenuPos == 2) 
			{
				sValueADC = Slave_GetData()->Data[SLAVE_MAP_SQTem].Raw; //����174ŷʱʪ���ADCֵ
			
			}
			
			ywTempPointY1.B=sValueADC;   //
			
		
			if((sValueADC>1300)&&(sValueADC<1600)) //������Χ��	
			{
				FourBytes fourB, fourK;

				//��Y�������B
				fourB.FloatData=50-CalcAdcToTemperature(ywTempPointY1.A);
				fourK.FloatData=195/(CalcAdcToTemperature(ywTempPointY1.B)-CalcAdcToTemperature(ywTempPointY1.A));

				if(MenuPos == 1)
				{
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_LDGQ] = fourB.U16Data[0] ; //����ֵ
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_LDGQ+1] = fourB.U16Data[1] ; //����ֵ
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_LDYW] = fourB.U16Data[0] ; //����ֵ
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_LDYW+1] = fourB.U16Data[1] ; //����ֵ

					pf->Data[FLASHIS_AdrWHJZ_WDJZ_BLGQ] = fourK.U16Data[0] ;
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_BLGQ+1] = fourK.U16Data[1] ;
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_BLYW] = fourK.U16Data[0] ;
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_BLYW+1] = fourK.U16Data[1] ;																
					
					Slave_ReadFlashZOData(&Slave_GetData()->GQTem,FLASHIS_AdrWHJZ_WDJZ_LDGQ,SLAVE_BaseTem);
					Slave_ReadFlashZOData(&Slave_GetData()->YWTem,FLASHIS_AdrWHJZ_WDJZ_LDYW,SLAVE_BaseTem);							
					Slave_ReadFlashBLData(&Slave_GetData()->GQTem,FLASHIS_AdrWHJZ_WDJZ_BLGQ); //������ 20200526
					Slave_ReadFlashBLData(&Slave_GetData()->YWTem,FLASHIS_AdrWHJZ_WDJZ_BLYW);//���±���  //2020-05-30, modify by alfred.
				}
				else if(MenuPos == 2)
				{
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_LDSQ] = fourB.U16Data[0] ; //ʪ�����
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_LDSQ+1] = fourB.U16Data[1] ; 

					pf->Data[FLASHIS_AdrWHJZ_WDJZ_BLSQ] = fourK.U16Data[0] ;	//ʪ����
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_BLSQ+1] = fourK.U16Data[1] ;
					
					Slave_ReadFlashZOData(&Slave_GetData()->SQTem,FLASHIS_AdrWHJZ_WDJZ_LDSQ,SLAVE_BaseTem);
					Slave_ReadFlashBLData(&Slave_GetData()->SQTem,FLASHIS_AdrWHJZ_WDJZ_BLSQ);//ʪ����					
				}				
				*pWarning = 0xAF; //�궨���				
			}
			else
			{
				*pWarning = 0xAB;	//174ŷ�����������
			}
			break;
		}
		default: break;
	}
	return *pWarning;	
}



/* 	date: 2020.06.09
	��������void ProcessGetMinuteData(void)
	������	�ú������ڴ����ȡ�������ݲ����浽SD��
	���룺	��
	Ӱ�죺	��Ļ�ṹ�壬flash�ṹ�塣
	�޸ļ�¼�� 1. 2020.06.09, ����, Add by alfred.
*/
void ProcessGetMinuteData(void)
{
	Screen_DataStructure *p = Screen_GetData();
	Slave_DataStructure*  Sp=Slave_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	float CoSo2;

	if(p->JiPingJun.bTime1S)  // ÿ�봦�������
	{
		p->JiPingJun.bTime1S = false;
		
		//��������������ʱ��(����,��ƽ��,����ͣ
		if(p->JiPingJun.cIsCalcAverage==1)
		{
		     p->JiPingJun.uCounter1S++;
		  
		     if((p->JiPingJun.uCounter1S%5)==0)   //5��ȡ1�����ۼ�
		     {
			  
			   //������SO2
			    CoSo2=Sp->SO2Concent.Value;
			    if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x01))==1)  //
				{
				CoSo2=CalculationFormula_GetData()->SO2FromCO;//������
                if(CoSo2<0)CoSo2=0;
                }
			  
		  		 p->JiPingJun.JpjSum[0]+= Sp->O2Concent.Value;	
				 p->JiPingJun.JpjSum[1]+= CoSo2;//Sp->SO2Concent.Value;
				 p->JiPingJun.JpjSum[2]+= Sp->NOConcent.Value;
				 p->JiPingJun.JpjSum[3]+= Sp->NO2Concent.Value;
				 p->JiPingJun.JpjSum[4]+= Sp->COConcent.Value;
				 p->JiPingJun.JpjSum[5]+= Sp->CO2Concent.Value;
				 p->JiPingJun.JpjSum[6]+= Sp->H2SConcent.Value;
				 p->JiPingJun.JpjSum[7]+= Sp->NOxConcent.Value;
				 p->JiPingJun.JpjSuncnt++;
				
		   }
		  
		  if( p->JiPingJun.uCounter1S%(p->AgvTimer*60)==0)	//��ʱ������ʱ��
		  {
				gMinuteData[p->JiPingJun.cAverageCount][0] = p->JiPingJun.JpjSum[0]/p->JiPingJun.JpjSuncnt;			
				gMinuteData[p->JiPingJun.cAverageCount][1] = p->JiPingJun.JpjSum[1]/p->JiPingJun.JpjSuncnt;	
				gMinuteData[p->JiPingJun.cAverageCount][2] = p->JiPingJun.JpjSum[2]/p->JiPingJun.JpjSuncnt;	
				gMinuteData[p->JiPingJun.cAverageCount][3] = p->JiPingJun.JpjSum[3]/p->JiPingJun.JpjSuncnt;	
				gMinuteData[p->JiPingJun.cAverageCount][4] = p->JiPingJun.JpjSum[4]/p->JiPingJun.JpjSuncnt;	
				gMinuteData[p->JiPingJun.cAverageCount][5] = p->JiPingJun.JpjSum[5]/p->JiPingJun.JpjSuncnt;	
				gMinuteData[p->JiPingJun.cAverageCount][6] = p->JiPingJun.JpjSum[6]/p->JiPingJun.JpjSuncnt;	
				gMinuteData[p->JiPingJun.cAverageCount][7] = p->JiPingJun.JpjSum[7]/p->JiPingJun.JpjSuncnt;	
			    ScreenJPG_ClearSum(); //���
	
								
				//������ֵ��СֵΪ��
				for(u8 i=0;i<8;i++)
				{
					if(gMinuteData[p->JiPingJun.cAverageCount][i] <0) gMinuteData[p->JiPingJun.cAverageCount][i] = 0.0f	;					
				}

				/*
				sprintf((char*)uTimeBuffer,"%02d:%02d:%02d",p->Clock.Clock.tm_hour,p->Clock.Clock.tm_min,p->Clock.Clock.tm_sec);
				uTimeBuffer[8]=0;	//����λ�ò�0��
				strcpy((char*)gSystemTime[p->JiPingJun.cAverageCount],(char*)uTimeBuffer); 
                */
				strncpy((char*)gSystemTime[p->JiPingJun.cAverageCount],(char*)&p->recorddat[11],5); //�ѿ�ʼʱ��ȡ����ʾ
				gSystemTime[p->JiPingJun.cAverageCount][5]=0;
				
				/*�������ݵ�SD��*/
				float buffer[40];
				float gas[8];
				gas[0]=gMinuteData[p->JiPingJun.cAverageCount][0];
	            gas[1]=gMinuteData[p->JiPingJun.cAverageCount][1];
	            gas[2]=gMinuteData[p->JiPingJun.cAverageCount][2];
	            gas[3]=gMinuteData[p->JiPingJun.cAverageCount][3];
	            gas[4]=gMinuteData[p->JiPingJun.cAverageCount][4];
	            gas[5]=gMinuteData[p->JiPingJun.cAverageCount][5];
	            gas[6]=gMinuteData[p->JiPingJun.cAverageCount][6];

				SaveGasDataLoad(buffer,gas);
				SdFile_Serch_MaxofGasRecord(Flux_GetData()->ChannelC.SaveFileNumber); //��ȡ���ļ�¼��
				SaveGasData(buffer,Flux_GetData()->ChannelC.SaveFileNumber,1);
				SdFile_WriteGasFile(&SdFile_GetData()->InterFace_data,Flux_GetData()->ChannelC.SaveFileNumber,1,0);
 
                //���¼�ʱ���ѿ�ʼʱ�����¼�¼����
                sprintf((char*) p->recorddat,"%02d-%02d-%02d %02d:%02d:%02d",p->Clock.Clock.tm_year+SCREEN_YearBase,p->Clock.Clock.tm_mon,p->Clock.Clock.tm_mday,p->Clock.Clock.tm_hour,p->Clock.Clock.tm_min,p->Clock.Clock.tm_sec);
	            p->recorddat[16]=0;   //��β�ȼӸ���
				u8 buf[5];
				sprintf((char *)&buf[0],"%02d",pf->Data[FLASHIS_AdrXTSZYQCLSJ]);  //
	            strcat((char *)&p->recorddat[0],(char *)&buf[0]);
				strcat((char *)&p->recorddat[0],"9");

				p->JiPingJun.cAverageCount++;	//ƽ��������1
				if(p->JiPingJun.cAverageCount == pf->Data[FLASHIS_AdrXTSZYQCLCS])  //��ƽ�����������
				{
					//to do �����߼�
					p->RunMode=0x5210;//��ʾ�Ƿ����
					p->JiPingJun.cIsCalcAverage = 0; //��ʱ������ƽ����־λ���㣻
					p->JiPingJun.cAverageCount = 0;
					p->Function=0;
					p->JiPingJun.ucCurrentPage = 0xff; //��ʼ����ƽ����ǰҳ��ֵ��
					
					ScreenChangeDataInit(p);	//ҳ���л����ݳ�ʼ��
				}
				p->RunModeStat=1;
				

			}
		  
			
	        if((p->JiPingJun.uCounter1S%(p->AgvTimer*60)>=(p->AgvTimer*60 - 10))&&	//������Ӳ�����ǰ10���������ʾ
			   (p->JiPingJun.uCounter1S%(p->AgvTimer*60)<=(p->AgvTimer*60)))
			      {
				     if(p->JiPingJun.uCounter1S%2==0)
				     Speak_Buzzer(500);
			      }
	
	 }
	}	
	
}

