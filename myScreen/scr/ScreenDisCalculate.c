/*
  该文件主要用于处理屏幕需要显示数据的相关计算
	及部分文件操作的逻辑
*/
#include <stdio.h>
#include <stdlib.h>
#include "ScreenDisCalculate.h"
#include "CalculationFormula.h"
#include "ScreenDwFunction.h"


float gMinuteData[60][8];	//分钟数据,用于显示暂存
u8 gSystemTime[60][10];		//分钟数据时间

BLUETOOTH_INIT_STATE_MACHINE g_BlueToothInitStateMachine;


/*
  圆形烟道测点距离系数
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
	函数名：void CalculationCircularMeasurePoint(u8 flag,float d,float L,u16 *h,u16 *SingleNumber,float *MeasurePoint)
	描述：	圆形测点计算公式
	flag是否通过计算得到环数 输入直径，管套L  
	输出环数H 、 SingleNumbe单点测点总数 、测点数MeasurePoint 
	修改记录： 1. 修复了圆形环数设置不正常的问题, modify by alfred, 2020.07.03
*/
void CalculationCircularMeasurePoint(u8 flag,float d,float L,u16 *h,u16 *SingleNumber,float *MeasurePoint)
{
	/*计算环数*/
	if(flag!=0) //需要计算
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


	
	/*根据环数计算测点*/
	
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
	函数名：void SquareCalculationMeasurePoint(float d,int L,u8 *h,float *MeasurePoint)
	描述：	矩形测点计算公式 
     1.A/孔数=小长方形边长
      

  flag是否通过计算得到单孔点数 输入B边长度，管套L  
  输出单边测点数X 和测点数MeasurePoint 其中
*/

/*
void SquareCalculationMeasurePoint(u8 flag,float B,float L,u16 *x,float *MeasurePoint)
{
	//计算单孔测点数
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
	//根据单孔测点数计算测点
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
	buf1=A/Ks;  //得到每小块边长
	buf2=B/buf1;// 由边长B得到多小分边长为A/Ks 的小正方形
	N1=(u16)(buf2+0.5f);  //
	if(N1<1)N1=1;  //最小一个点
	
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
	函数名：u16 SquareCalculationMeasureHold(float A)
	描述：	由面积决定孔数或单点测孔数
            返回孔数
*/

u16 SquareCalculationMeasureHold(float MJ,float A,float B)
{  // u16 buf;
	if(MJ<0.1f)    //面积小于0.1
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
	函数名：SquareCalculationMeasurePointN(u16 *N,u8 Ks,float B,float L,float A,float *MeasurePoint)
	描述：	矩形测点计算公式 
     1.*N 为单孔检测点数指针
     2.Ks 孔数
     3.B 边长
     4.L套管长
     5.A 边长
     6.MeasurePoint保存地址指针
  flag是否通过计算得到单孔点数 输入B边长度，管套L  
  输出单边测点数X 和测点数MeasurePoint 其中
*/
void SquareCalculationMeasurePointN(u16 *N,u8 Ks,float B,float L,float A,float *MeasurePoint)
{ 
	//float buf1,buf2;

	for(int i=0;i<(*N);i++)  //求每个点的
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
	函数名：void SaveFlueData(float *SaveData)
	描述：	保存烟道数据
  输入：SaveData[14] 参考File.c 文件const char Flue_line【14】【30】
*/
void SaveFlueData(float *SaveData)
{
   SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	u32 FileNumber=01;
	SdFile_Serch_MaxofFlueLinkFile();//先读取最大文件号
	FileNumber=SdFp->FlueLinkFileMax+1;
	
	u8 buf[31]={0};
	//装载日期时间
	SdFile_ramlodair((char *)&SdFp->InterFace_data.dat[0],Interf_dat_length);  //先填充空格
    sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
    strncpy((char *)&SdFp->InterFace_data.dat[0],(char *)buf,Interf_dat_length);  
  //装载地址
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Adress[0],Adress_length-9-2);  //先填充空格49+/r/n
	 sprintf((char *)&buf[0],"%-30s",(char *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));  //
	 strncpy((char *)&SdFp->InterFace_data.Adress[0],(char *)&buf[0],30);   //flash里只存30个地址数据
	 printf("%-30s",(char *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));
	//面积装载
    SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[0],Interf_other_length);  //先填充空格
	sprintf((char*)SdFp->InterFace_data.Other[0],"     %-8.4f",SaveData[0]);
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[1],Interf_other_length);  //先填充空格
	if(SaveData[1]==1)
	{
	  sprintf((char*)SdFp->InterFace_data.Other[1],"  1.圆形烟道 ");
	}else  if(SaveData[1]==2)
	{
	  sprintf((char*)SdFp->InterFace_data.Other[1],"  2.方形烟道 ");
	}else
	{
	   sprintf((char*)SdFp->InterFace_data.Other[1],"  3.其他形烟道 ");
	}
		//装载孔数
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[2],Interf_other_length);  //先填充空格
	sprintf((char*)SdFp->InterFace_data.Other[2],"     %-8d",(int32_t)SaveData[2]);
		//装载测点总数
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[3],Interf_other_length);  //先填充空格
	sprintf((char*)SdFp->InterFace_data.Other[3],"     %-8d",(int32_t)SaveData[3]);
	for(int i=4;i<=14;i++)//测点距离
	{
		SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[i],Interf_other_length);  //先填充空格
	  sprintf((char*)SdFp->InterFace_data.Other[i],"     %-8.3f",SaveData[i]);
	}
	//装载内径D
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[15],Interf_other_length);  //先填充空格
	sprintf((char*)SdFp->InterFace_data.Other[15],"     %-8.2f",SaveData[15]);
	
	//装载套管L
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[16],Interf_other_length);  //先填充空格
	sprintf((char*)SdFp->InterFace_data.Other[16],"     %-8.2f",SaveData[16]);
	
	//环数
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[17],Interf_other_length);  //先填充空格
	sprintf((char*)SdFp->InterFace_data.Other[17],"     %-8d",(int32_t)SaveData[17]);
	
	//边A
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[18],Interf_other_length);  //先填充空格
	sprintf((char*)SdFp->InterFace_data.Other[18],"     %-8.2f",SaveData[18]);
	
	//边B
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[19],Interf_other_length);  //先填充空格
	sprintf((char*)SdFp->InterFace_data.Other[19],"     %-8.2f",SaveData[19]);
	
	//单孔测点
	SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[20],Interf_other_length);  //先填充空格
	sprintf((char*)SdFp->InterFace_data.Other[20],"     %-8d",(int32_t)SaveData[20]);

	SdFile_WriteFlueFile(&SdFp->InterFace_data,FileNumber);//写入烟道数据

}
/* date 2020-03-02
	函数名：ReadFlueData(u32 FileNum)
	描述：	读取烟道数据
  输入：文件号
*/
void ReadFlueData(u32 FileNum)//根据文件号 读取烟道信息
{ s16 i;
  SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	SdFile_ReadFlueFile(FileNum);
	
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //文件号
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);
	strncpy((char *)ps->Buffer,(char *)&(SdFp->FlueFile.Other[0][13]),8);//面积
	Screen_RefreshDataMulti(0x0120,ps->Buffer,15*2);
	
	strncpy((char *)ps->Buffer,(char *)&(SdFp->FlueFile.Other[1][11]),8);//类型
	Screen_RefreshDataMulti(0x0140,ps->Buffer,15*2); //烟道类型
	strncpy((char *)ps->Buffer,(char *)&(SdFp->FlueFile.Other[2][13]),8);//孔数
	Screen_RefreshDataMulti(0x0160,ps->Buffer,15*2);
	strncpy((char *)ps->Buffer,(char *)&(SdFp->FlueFile.Other[3][13]),8);//测点总数
	Screen_RefreshDataMulti(0x0180,ps->Buffer,15*2);
	//显示地址信息
	
		
	strncpy((char*)ps->Buffer,(char *)&(SdFp->FlueFile.Adress[9]),30);
	for(i=29;i>=0;i--)               //把空格转换为0
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
				
	char uCursor[]={0xff,0xff,0x00}; //补上两个0XFF使光标位置正确显示。
	strcat((char*)ps->Buffer, uCursor);
	Screen_RefreshDataMulti(0x01A0,ps->Buffer,15*2);
	
	//显示测点距离
	for(int i=0;i<10;i++)
	{
		 strncpy((char *)ps->Buffer,(char *)&(SdFp->FlueFile.Other[i+4][13]),8);//测点总数
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
	函数名：void ApplicationFlueFileData (void)
	描述：	采样文件数据
  输入：void
*/
void ApplicationFlueFileData (void)//采用当前数据
{   s16 i;
    SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	char buff[61]={0};
	FourBytes four;
	strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[0][13]),8);//面积
	four.FloatData=atof(buff);
	SaveFlaotToFlash(four.FloatData,FLASHIS_AdrYDBD_MJ);
  printf("%s,%.4f\r\n",buff,four.FloatData);
	//装载烟道类型
  if(SdFp->FlueFile.Other[1][11]=='1')
  {
	  pf->Data[FLASHIS_AdrYDBDLX]=1;
	
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[15][13]),8);//内径
	  buff[8]=0;
	  four.FloatData=atof(buff);
	  SaveFlaotToFlash(four.FloatData,FLASHIS_AdrYDBDYXNJ);  
	  
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[16][13]),8);//套管
	  buff[8]=0;
	  four.FloatData=atof(buff);
	  SaveFlaotToFlash(four.FloatData,FLASHIS_AdrYDBD_GT);  //
	  
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[17][13]),8);//环数
	  buff[8]=0;
	  four.U16Data[0]=(u16)atoi(buff);
	  SaveU16ToFlash(four.U16Data[0],FLASHIS_AdrYDBDYXHS);  //
	  
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[2][13]),8);//孔数
	  buff[8]=0;
	  four.U16Data[0]=(u16)atoi(buff);
	  SaveU16ToFlash(four.U16Data[0],FLASHIS_AdrYDBDYXKS);  //
	  
	  
  }
  else if(SdFp->FlueFile.Other[1][11]=='2') 
  {
	  pf->Data[FLASHIS_AdrYDBDLX]=2;
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[18][13]),8);//边A
	  buff[8]=0;
	  four.FloatData=atof(buff);
	  SaveFlaotToFlash(four.FloatData,FLASHIS_AdrYDBDFXBA);  
	  
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[19][13]),8);//边B
	  buff[8]=0;
	  four.FloatData=atof(buff);
	  SaveFlaotToFlash(four.FloatData,FLASHIS_AdrYDBDFXBB);  //  
	  
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[16][13]),8);//套管
	  buff[8]=0;
	  four.FloatData=atof(buff);
	  SaveFlaotToFlash(four.FloatData,FLASHIS_AdrYDBD_GT);  //

	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[2][13]),8);//孔数
	  buff[8]=0;
	  four.U16Data[0]=(u16)atoi(buff);
	  SaveU16ToFlash(four.U16Data[0],FLASHIS_AdrYDBDYXKS);  //
	  SaveU16ToFlash(four.U16Data[0],FLASHIS_AdrYDBDFXKS);  //
	  
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[20][13]),8);//单点测孔
	  buff[8]=0;
	  four.U16Data[0]=(u16)atoi(buff);
	  SaveU16ToFlash(four.U16Data[0],FLASHIS_AdrYDBDFXDKCD);  //

  }
	else if(SdFp->FlueFile.Other[1][11]=='3')
	{	
	  pf->Data[FLASHIS_AdrYDBDLX]=3;	
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[3][13]),8);//单点测孔
	  buff[8]=0;
	  four.U16Data[0]=(u16)atoi(buff);
	  SaveU16ToFlash(four.U16Data[0],FLASHIS_AdrYDBD_CYDS);  //

	}
	printf("%s,%.4f\r\n",&SdFp->FlueFile.Other[1][9],four.FloatData);

  for(int i=4;i<14;i++)
	{
	  strncpy((char *)buff,(char *)&(SdFp->FlueFile.Other[i][13]),8);//测点距离数
	  four.FloatData=atof(buff);
		SaveFlaotToFlash(four.FloatData,FLASHIS_AdrYDBDCDJL1+2*(i-4));
		printf("%s,%.3f\r\n",buff,four.FloatData);
	}
	Screen_RefreshDataMulti(0x0180,ps->Buffer,15*2);
	//显示地址信息
	{
    strncpy((char *)buff,(char *)&(SdFp->FlueFile.Adress[9]),30);//地址
	for(i=29;i>=0;i--)
	{
		if((buff[i]==0x20)||(buff[i]==0x00))  //空格转回0
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
	函数名：ReadWorkData(u32 FileNum)
	描述： 读取工况文件
  输入：
*/         
void ReadWorkData(u32 FileNum)//根据文件号 读取工况信息
{
  SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	SdFile_ReadWorkFile(FileNum);
	
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //文件号
	Screen_RefreshDataMulti(0x0440,ps->Buffer,15*2);
	for(int i=0;i<2;i++)
	{
		
		strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[i][12]),10);//显示参数
		
		Screen_RefreshDataMulti(0x0120+i*0x20,ps->Buffer,15*2);
	}
	for(int i=3;i<11;i++)                                                       //i=10为含湿量
	{   
		strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[i][12]),10);//显示参数//
		Screen_RefreshDataMulti(0x0120+(i-1)*0x20,ps->Buffer,15*2);             //
	} 

	
	//显示时间信息
	{		
		strncpy((char*)ps->Buffer,(char *)&(SdFp->rdWorkFile.dat[0]),16);
	}				
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);

}
//---
/* date 2020-03-10
	函数名：ReadFlueData(u32 FileNum)
	描述：	保存工况文件
  输入：
*/

void SaveWorkData(float *SaveData,u32 NumFile)
 { 
	 SdFile_DataStruct* SdFp=SdFile_GetData();
   Screen_DataStructure* ps=Screen_GetData();
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 u8 buf[80]={0};
	 //装载日期时间
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.dat[0],Interf_dat_length);  //先填充空格
   sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
   strncpy((char *)&SdFp->InterFace_data.dat[0],(char *)&buf[0],Interf_dat_length);  
    //装载地址
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Adress[0],Adress_length-9-2);  //先填充空格49+/r/n
	 sprintf((char *)&buf[0]," %-30s",(char *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));  //
	 strncpy((char *)&SdFp->InterFace_data.Adress[0],(char *)&buf[0],30);   //flash里只存30个地址数据

 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[0][0],Interf_other_length);     //先填充空格
	 sprintf((char*)&buf[0],"   %10d",(int32_t)SaveData[0]);    //平均动压位置
	 strncpy((char *)&SdFp->InterFace_data.Other[0][0],(char *)&buf[0],Interf_other_length); 
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[1][0],Interf_other_length);     //先填充空格
	 sprintf((char *)&buf[0],"   %10.2f",SaveData[1]);          //平均静压位置
	 strncpy((char *)&SdFp->InterFace_data.Other[1][0],(char *)&buf[0],Interf_other_length); 
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[2][0],Interf_other_length);     //先填充空格
	 sprintf((char *)&buf[0],"   %10.2f",SaveData[2]);          //平均全压位置
	 strncpy((char *)&SdFp->InterFace_data.Other[2][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[3][0],Interf_other_length);     //先填充空格
	 sprintf((char *)&buf[0],"   %10.2f",SaveData[3]);          //大气压位置
	 strncpy((char *)&SdFp->InterFace_data.Other[3][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[4][0],Interf_other_length);     //先填充空格
	 sprintf((char *)&buf[0],"   %10.4f",SaveData[4]);          //烟道截面
	 strncpy((char *)&SdFp->InterFace_data.Other[4][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[5][0],Interf_other_length);     //先填充空格
	 sprintf((char *)&buf[0],"   %10.4f",SaveData[5]);          //采样嘴d
	 strncpy((char *)&SdFp->InterFace_data.Other[5][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[6][0],Interf_other_length);     //先填充空格
	 sprintf((char *)&buf[0],"   %10.1f",SaveData[6]);          //平均烟温
	 strncpy((char *)&SdFp->InterFace_data.Other[6][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[7][0],Interf_other_length);     //先填充空格
	 sprintf((char *)&buf[0],"   %10.1f",SaveData[7]);          //平均流速
	 strncpy((char *)&SdFp->InterFace_data.Other[7][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[8][0],Interf_other_length);     //先填充空格
	 sprintf((char *)&buf[0],"   %10d",(int32_t)SaveData[8]);    //烟气流量
	 strncpy((char *)&SdFp->InterFace_data.Other[8][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[9][0],Interf_other_length);     //先填充空格
	 sprintf((char *)&buf[0],"   %10d",(int32_t)SaveData[9]);    //标干流量
	 strncpy((char *)&SdFp->InterFace_data.Other[9][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[10][0],Interf_other_length);     //先填充空格
	 sprintf((char *)&buf[0],"   %10.2f",SaveData[10]);          //含湿量
	 strncpy((char *)&SdFp->InterFace_data.Other[10][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[11][0],Interf_other_length);     //先填充空格
	 sprintf((char *)&buf[0],"   %10.2f",SaveData[11]);          //折算系数
	 strncpy((char *)&SdFp->InterFace_data.Other[11][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[12][0],Interf_other_length);     //先填充空格
	 sprintf((char *)&buf[0],"   %10.2f",SaveData[12]);          //负荷系数
	 strncpy((char *)&SdFp->InterFace_data.Other[12][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[13][0],Interf_other_length);     //先填充空格
   sprintf((char *)&buf[0],"   %10.2f",SaveData[13]);          //皮 托 管
	 strncpy((char *)&SdFp->InterFace_data.Other[13][0],(char *)&buf[0],Interf_other_length);
  
 }





 //---

  /* date 20200504
	函数名 void SaveGasDataLoad(float *SaveData)
	描述：	保存烟气数据装载
  输入： 
 
    Gas:气体数据指针
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

	SaveData[0]=Cp->Pd;//动压
	SaveData[0]=GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJDY);//平均动压

	SaveData[1]=Cp->Ps;	//静压	
	SaveData[1]=GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJJY);//平均静压

	SaveData[2]=Cp->Pt;//计算全压
	SaveData[2]=GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJQY);//平均全压
	
	SaveData[4]=Cp->Ts ;         //烟温
	SaveData[4]=GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJYW);//平均烟温
	
	SaveData[5]=Cp->Ba;	//大气压	
	
	/*
    float Pvalue;
	if((pf->Data[FLASHIS_AdrXTSZBOX]&0x40)==0)	
	Pvalue=CalculationFormula_p(SaveData[4],SaveData[5],SaveData[1]);//计算烟气密度
	else
    Pvalue=CalculationFormula_p(Cp->Th,SaveData[5],0.0);//计算烟气密度
	SaveData[3]=CalculationFormula_Vs(Cp->Kp,SaveData[0],Pvalue);	//计算流速
	*/
	SaveData[3]=Cp->Vs;	//计算流速
	SaveData[3]=GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_YQLS_YQLS);//平均流速
	
	SaveData[6]=Cp->F;//获取烟道面积
	SaveData[7]=Cp->Xsw*100;//含湿量
	SaveData[8]=Cp->Kp;	//皮托管系数
	SaveData[9]=GetFlaotdataFormFlash2Word(FLASHIS_AdrCLYQ_WJCX_JS_ZSXS);//折算系数
	SaveData[10]=GetFlaotdataFormFlash2Word(FLASHIS_AdrCLYQ_WJCX_JS_FHXS);//负荷系数
	
	//SaveData[11] =CalculationFormula_Qs(SaveData[3],SaveData[6]);//烟气流量
	SaveData[11] =Cp->Qs;//烟气流量
	SaveData[11] =GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_YGLL);//平均烟气流量
	
	SaveData[12] =Cp->Qsnd;//标杆流量	
	SaveData[12] =GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_BGLL);//标杆流量
	
	
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
	SaveData[21]= Flux_GetData()->ChannelC.Sampling.SecTimer; // 采样时长, modify by alfred   2020.06.27
	SaveData[22]=Sp->JWTem.Value; //计前温度 
	
	
	
	if(Sp->O2Concent.Value!=21)
	SaveData[23]= 21.0f/(21-Sp->O2Concent.Value);//过剩系数
	else
	SaveData[23]=SLAVE_ColFAULT;  //采集数据出错不能为Sp->O2Concent.Value=21
	
	SaveData[24]=GetFlaotdataFormFlash2Word(FLASHIS_AdrCLYQ_WJCX_JS_JZYHL); //基准氧气含量
	
	
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
	函数名aveGasINputDataLoad(float *SaveData)
	描述：	保存输入烟气数据装载
  输入：
	
*/
 void SaveGasINputDataLoad(float *SaveData)
{

	Screen_DataStructure* ps=Screen_GetData();
	SdFile_DataStruct* SdFp=SdFile_GetData();
	char buff[100];
	FourBytes four;
	for(int i=0;i<25;i++)
	{
		strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[i][12]),10);//原始数据装载
		four.FloatData=atof(buff);
		SaveData[i]=four.FloatData;
	}
	   //装载输入烟气数据		
	for(int i=0;i<7;i++)
			{
				SaveData[13+i]=ps->DisValue[i];
				if(SaveData[13+i]<0)
				SaveData[13+i]=0;
			}
		

}
//---
 /* date 20200518
	函数名：SaveGasData(float *SaveData,u32 NumFile,u8 attr)
	描述：	保存烟气文件
  输入：
	
*/
void SaveGasAverageRecordData(float *SaveData,u32 NumFile)
 {
	 
	 SdFile_DataStruct* SdFp=SdFile_GetData();
	 Screen_DataStructure* ps=Screen_GetData();
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 u8 buf[80]={0};
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[13],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[0]);          //含氧量
	 strncpy((char *)&SdFp->InterFace_data.Other[13][0],(char *)&buf[0],Interf_other_length);
	 
	 ps->GSXS=SaveData[0];
	 ps->GSXS=21.0f-ps->GSXS;
	 if(ps->GSXS==0)
	 ps->GSXS=SLAVE_ColFAULT;                               //过剩系数错误
	 else
	 ps->GSXS=21.0f/ ps->GSXS;                               //由氧气浓度计算过剩系数
	 
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[14],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[1]);          //二氧化硫
	 strncpy((char *)&SdFp->InterFace_data.Other[14][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[15],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[2]);          //一氧化氮
	 strncpy((char *)&SdFp->InterFace_data.Other[15][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[16],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[3]);          //二氧化氮
	 strncpy((char *)&SdFp->InterFace_data.Other[16][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[17],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[4]);          //一氧化碳
	 strncpy((char *)&SdFp->InterFace_data.Other[17][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[18],Interf_other_length);  //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[5]);          //流化氢
	 strncpy((char *)&SdFp->InterFace_data.Other[18][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[19],Interf_other_length);  //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[6]);          //二氧化碳
	 strncpy((char *)&SdFp->InterFace_data.Other[19][0],(char *)&buf[0],Interf_other_length);
	 
 }
 
 //---
 /* date 20200518
	函数名：SaveGasData(float *SaveData,u32 NumFile,u8 attr)
	描述：	保存烟气的基准含氧量,出力系数，过剩系数
  输入：
	
*/
void SaveGas_Z_J_F_RecordData(void)
 {
	 
	 SdFile_DataStruct* SdFp=SdFile_GetData();
	 Screen_DataStructure* ps=Screen_GetData();
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 u8 buf[80]={0};
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[9],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",ps->ZSXS);          //折算系数
	 strncpy((char *)&SdFp->InterFace_data.Other[9][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[10],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",ps->FHXS);          //负荷系数
	 strncpy((char *)&SdFp->InterFace_data.Other[10][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[23],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",ps->GSXS);          //过剩系数
	 strncpy((char *)&SdFp->InterFace_data.Other[23][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[24],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",ps->JZHYL);          //基准含氧量
	 strncpy((char *)&SdFp->InterFace_data.Other[24][0],(char *)&buf[0],Interf_other_length);

 }

//---
 /* date 2020-03-10
	函数名：ReadFlueData(u32 FileNum)
	描述：	保存烟气文件
  输入：
	
*/
void SaveGasData(float *SaveData,u32 NumFile,u8 attr)
 {
	 u8 i;
	 SdFile_DataStruct* SdFp=SdFile_GetData();
	 Screen_DataStructure* ps=Screen_GetData();
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 u8 buf[80]={0};
	
	//装载日期时间
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.dat[0],Interf_dat_length);  //先填充空格
     strncpy((char *)&SdFp->InterFace_data.dat[0],(char *)&ps->recorstarddat[0],Interf_dat_length); 
	 
   //装载地址
	 
	 sprintf((char *)&buf[0],"%02d",(pf->Data[FLASHIS_AdrCLYQ_JSSC]));  //
	 strncpy((char *)&SdFp->InterFace_data.T[0],(char *)&buf[0],2);     //分钟采样时长转换成字符串
	 SdFp->InterFace_data.T[2]=0;
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Adress[0],Adress_length-9-2);  //先填充空格49+/r/n
	 sprintf((char *)&buf[0]," %-30s",(char *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));  //
	 strncpy((char *)&SdFp->InterFace_data.Adress[0],(char *)&buf[0],30);   //flash里只存30个地址数据

	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[0][0],Interf_other_length);      //先填充空格
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[0]);                             //平均动压位置
	 strncpy((char *)&SdFp->InterFace_data.Other[0][0],(char *)&buf[0],Interf_other_length);   //flash里只存30个地址数据
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[1],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[1]);           //平均静压位置
	 strncpy((char *)&SdFp->InterFace_data.Other[1][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[2],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[2]);           //平均全压位置
	 strncpy((char *)&SdFp->InterFace_data.Other[2][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[3],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[3]);           //平均流速
	 strncpy((char *)&SdFp->InterFace_data.Other[3][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[4],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[4]);           //平均烟温
	 	 strncpy((char *)&SdFp->InterFace_data.Other[4][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[5],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[5]);           //大气压
	 	 strncpy((char *)&SdFp->InterFace_data.Other[5][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[6],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.4f",SaveData[6]);           //烟道截面
	 	 strncpy((char *)&SdFp->InterFace_data.Other[6][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[7],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[7]);           //含 湿 量
	 	 strncpy((char *)&SdFp->InterFace_data.Other[7][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[8],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[8]);           //皮管系数
	 	 strncpy((char *)&SdFp->InterFace_data.Other[8][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[9],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[9]);           //折算系数
	 	 strncpy((char *)&SdFp->InterFace_data.Other[9][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[10],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[10]);           //负荷系数
	 	 strncpy((char *)&SdFp->InterFace_data.Other[10][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[11],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[11]);   //烟气流量
	 	 strncpy((char *)&SdFp->InterFace_data.Other[11][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[12],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[12]);   //标干流量
	 	 strncpy((char *)&SdFp->InterFace_data.Other[12][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[13],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[13]);          //含氧量
	 	 strncpy((char *)&SdFp->InterFace_data.Other[13][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[14],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[14]);          //二氧化硫
	 	 strncpy((char *)&SdFp->InterFace_data.Other[14][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[15],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[15]);          //一氧化氮
	 	 strncpy((char *)&SdFp->InterFace_data.Other[15][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[16],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[16]);          //二氧化氮
	 	 strncpy((char *)&SdFp->InterFace_data.Other[16][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[17],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[17]);          //一氧化碳
	 	 strncpy((char *)&SdFp->InterFace_data.Other[17][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[18],Interf_other_length);  //先填充空格
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[18]);          //流化氢
	 	 strncpy((char *)&SdFp->InterFace_data.Other[18][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[19],Interf_other_length);  //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[19]);          //二氧化碳
	 strncpy((char *)&SdFp->InterFace_data.Other[19][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[20],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %10d",(int32_t)SaveData[20]);   //端口配置
	 strncpy((char *)&SdFp->InterFace_data.Other[20][0],(char *)&buf[0],Interf_other_length);

	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[21],Interf_other_length);  //先填充空格
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[21]);          //采样时长
	 strncpy((char *)&SdFp->InterFace_data.Other[21],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[22],Interf_other_length);  //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[22]);          //计前温度
	 strncpy((char *)&SdFp->InterFace_data.Other[22][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[23],Interf_other_length);  //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[23]);          //过剩系数
	 strncpy((char *)&SdFp->InterFace_data.Other[23][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[24],Interf_other_length);  //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[24]);          //基准氧气含量
	 strncpy((char *)&SdFp->InterFace_data.Other[24][0],(char *)&buf[0],Interf_other_length);

	 SdFile_ramlodair((char *)&SdFp->InterFace_data.recorddat[0],Record_row_length);  //先填充空格20-1=19
	 

	 strncpy((char *)&SdFp->InterFace_data.recorddat[0],(char *)&ps->recorddat[0],Record_row_length-1);  //copy时间的

	 for(i=0;i<9;i++)
	 {
		SdFile_ramlodair((char *)&SdFp->InterFace_data.record[i][0],(Record_row_length-10));    //先填充空格
		if((i==0)&&(i==7))
		sprintf((char*)&buf[0],"%10.1f",SaveData[25+i]);
		else
		sprintf((char*)&buf[0],"%10.0f",SaveData[25+i]);
		strncpy((char *)&SdFp->InterFace_data.record[i][0],(char *)&buf[0],Record_row_length-10);
	 }
	
 }
   //---
 /* date 20200518
	函数名：ReadGasFirstData(u32 FileNum)
	描述： 读取烟气文件-第一页
  输入：
*/         
void ReadGasFirstData(u32 FileNum)//根据文件号 读取烟气信息
{   float buf;
    SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	SdFile_ReadGasFile(FileNum,0x01);
	Screen_ResetDataBuffer();
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //文件号
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.dat[0]),10);//日期        
	Screen_RefreshDataMulti(0x0140,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.dat[11]),5);//时间       
	Screen_RefreshDataMulti(0x0120,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[21][12]),10);//采样总长
	
	//strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[21][12]),10);
	buf=atof((char *)&ps->Buffer[0]);
	Screen_ResetDataBuffer();
	sprintf((char *)&ps->Buffer[0],"%02d:%02d:%02d",((u32)buf/3600),((u32)buf%3600)/60,(((u32)buf%3600)%60));
	Screen_RefreshDataMulti(0x0160,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[7][12]),10);//含湿量
	Screen_RefreshDataMulti(0x0180,ps->Buffer,15*2);
  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[6][12]),10);//截面积
	Screen_RefreshDataMulti(0x01A0,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[4][12]),10);//烟气温度
	Screen_RefreshDataMulti(0x01C0,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[0][12]),10);//平均动压
	Screen_RefreshDataMulti(0x01E0,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[1][12]),10);//平均静压
	Screen_RefreshDataMulti(0x0400,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[5][12]),10);//大气压
	Screen_RefreshDataMulti(0x0540,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[22][12]),10);//计前温度
	Screen_RefreshDataMulti(0x0420,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[3][12]),10);//平均流速
	Screen_RefreshDataMulti(0x0440,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[24][12]),10);//基准含氧量
	ps->JZHYL=atof((char *)&ps->Buffer[0]);   //字符串转换成浮点数
	Screen_RefreshDataMulti(0x0460,ps->Buffer,15*2);
	
	//计算折算系数
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[9][12]),10);//
	ps->ZSXS=atof((char *)&ps->Buffer[0]);   //字符串转换成浮点数

	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[13][12]),10);//含氧量
	Screen_RefreshDataMulti(0x0480,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[23][12]),10);//过剩系统
	ps->GSXS=atof((char *)&ps->Buffer[0]);   //字符串转换成浮点数
	if(ps->GSXS==1000000)  //错误的标志
	strcpy((char *)ps->Buffer,"----------");
	Screen_RefreshDataMulti(0x04A0,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[10][12]),10);//出力系数
	ps->FHXS=atof((char *)&ps->Buffer[0]);   //字符串转换成浮点数
	Screen_RefreshDataMulti(0x04C0,ps->Buffer,15*2);
	
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[11][12]),10);//烟气流量
	Screen_RefreshDataMulti(0x0500,ps->Buffer,15*2);
	
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[12][12]),10);//标杆流量
	Screen_RefreshDataMulti(0x0520,ps->Buffer,15*2);
	
	//显示地址信息
	{
	Screen_ResetDataBuffer();
	strncpy((char*)ps->Buffer,(char *)&(SdFp->rdWorkFile.Adress[9]),30);
	}				
	 Screen_RefreshDataMulti(0x0560,ps->Buffer,15*2);
	
	
	 SdFile_Serch_MaxofGasRecord(FileNum);
	 Screen_ResetDataBuffer();	
	 if(SdFp->GasRecordMax)
	 sprintf((char *)ps->Buffer,"分钟数据:%d",SdFp->GasRecordMax); //
	 else
	 sprintf((char *)ps->Buffer,"分钟数据:无分钟数据!"); //
	// strcpy((char *)ps->Buffer,"分钟数据");//采样时长
	 Screen_RefreshDataMulti(0x2E20,ps->Buffer,15*2);
 
}
 /* date 2020-03-30
	函数名：void GetGasSecondData (u32 FileNum)
	描述：	获取当前气体数据 -第二页
  输入：void
*/
void GetGasSecondData(u32 FileNum)//获取当前气体数据
{
	SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	char buff[61]={0};
	SdFile_ReadGasFile(FileNum,0x01);
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //文件号
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);
	FourBytes four,four1,four2,four3;
	for(int i=0;i<5;i++)  //SO2,NO,NO2,CO,H2S
	{
		strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[14+i][12]),10);//气体浓度
		four.FloatData=atof(buff);
		ps->DisValue[i]=four.FloatData;
	}
	strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[9][12]),10);//折算系数
	four.FloatData=atof(buff);
	ps->DisValue[7]=four.FloatData;//折算系数
	
	strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[13][12]),10);//氧气浓度
	four.FloatData=atof(buff);
	CalculationFormula_a(four.FloatData); //计算空气过剩系数，放在C_F_Structure.a，CalculationFormula_CaALL函数用到
	
	strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[12][12]),10);//标杆流量
	four1.FloatData=atof(buff);
	
	strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[10][12]),10);//负荷（出力）系数
	four2.FloatData=atof(buff);
	
	CalculationFormula_CaALL(&ps->DisValue[0],ps->DisValue[7],four1.FloatData,four2.FloatData); //用烟尘采样流型计算
	CalculationFormula_GetData();
	
		if(ps->DisValue[0]!=1000000)
		{
			if(ps->GSXS==1000000)
			sprintf((char*)ps->Buffer,"%8.0f       ------  %10.1f",ps->DisValue[0],CalculationFormula_GetData()->GaofSO2); //原值，折算浓度，排放量
		    else
		    sprintf((char*)ps->Buffer,"%8.0f %10.0f  %10.1f",ps->DisValue[0],CalculationFormula_GetData()->CaofSO2,CalculationFormula_GetData()->GaofSO2); //原值，折算浓
		}
		else
		{
			sprintf((char*)ps->Buffer,(char *)"  ------       ------       ------  ");
		}
	   Screen_RefreshDataMulti(0x0120,ps->Buffer,20*2);
	
		if(ps->DisValue[1]!=1000000)
		{
			if(ps->GSXS==1000000)
			sprintf((char*)ps->Buffer,"%8.0f       ------  %10.1f",ps->DisValue[1],CalculationFormula_GetData()->GaofNO); //原值，折算浓度，排放量
		    else
			sprintf((char*)ps->Buffer,"%8.0f %10.0f  %10.1f",ps->DisValue[1],CalculationFormula_GetData()->CaofNO,CalculationFormula_GetData()->GaofNO); //原值，折算浓度，排放量
		}
		else
		{
			sprintf((char*)ps->Buffer,(char *)"  ------       ------       ------  ");
		}
	   Screen_RefreshDataMulti(0x0160,ps->Buffer,20*2);
		
		if(ps->DisValue[2]!=1000000)
		{
			if(ps->GSXS==1000000)
			sprintf((char*)ps->Buffer,"%8.0f       ------  %10.1f",ps->DisValue[2],CalculationFormula_GetData()->GaofNO2); //原值，折算浓度，排放量
		    else
			sprintf((char*)ps->Buffer,"%8.0f %10.0f  %10.1f",ps->DisValue[2],CalculationFormula_GetData()->CaofNO2,CalculationFormula_GetData()->GaofNO2); //原值，折算浓度，排放量
		}
		else
		{
			sprintf((char*)ps->Buffer,(char *)"  ------       ------       ------  ");
		}
	   Screen_RefreshDataMulti(0x01A0,ps->Buffer,20*2);
		
		if(ps->DisValue[3]!=1000000)
		{
			if(ps->GSXS==1000000)
			sprintf((char*)ps->Buffer,"%8.0f       ------  %10.1f",ps->DisValue[3],CalculationFormula_GetData()->GaofCO); //原值，折算浓度，排放量
		    else
			sprintf((char*)ps->Buffer,"%8.0f %10.0f  %10.1f",ps->DisValue[3],CalculationFormula_GetData()->CaofCO,CalculationFormula_GetData()->GaofCO); //原值，折算浓度，排放量
		}
		else
		{
			sprintf((char*)ps->Buffer,(char *)"  ------       ------       ------  ");
		}
	   Screen_RefreshDataMulti(0x0440,ps->Buffer,20*2);

		if(ps->DisValue[4]!=1000000)
		{
			if(ps->GSXS==1000000)
			sprintf((char*)ps->Buffer,"%8.0f       ------  %10.1f",ps->DisValue[4],CalculationFormula_GetData()->GaofH2S); //原值，折算浓度，排放量
		    else
			sprintf((char*)ps->Buffer,"%8.0f %10.0f  %10.1f",ps->DisValue[4],CalculationFormula_GetData()->CaofH2S,CalculationFormula_GetData()->GaofH2S); //原值，折算浓度，排放量
		}
		else
		{
			sprintf((char*)ps->Buffer,(char *)"  ------       ------       ------  ");
		}
	   Screen_RefreshDataMulti(0x0480,ps->Buffer,20*2);
		
          //NOX 浓度
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
			sprintf((char*)ps->Buffer,"%8.0f       ------  %10.1f",ps->DisValue[8],four3.FloatData); //原值，折算浓度，排放量
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
	函数名：void GetGasSecondData (u32 FileNum)
	描述：	获取当前气体浓度数据
  输入：void
*/
void GetGasLongDuData(u32 FileNum ,float *DisValue)//获取当前气体浓度数据
{
	SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	char buff[61]={0};
	SdFile_ReadGasFile(FileNum,0x01);
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //文件号
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);
	FourBytes four;
	for(int i=0;i<7;i++)
	{
		strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[13+i][12]),10);//气体浓度
		four.FloatData=atof(buff);
		DisValue[i]=four.FloatData;
	}
}

/* date 20200514
	函数名oid GetGasFileRecodData(u32 FileNum,u32 Recod)
	描述：	获取当前气体记录数据
  输入：isDisplay =0;不显示  ==1 显示
*/
	char DisBuff[8][30]={
		{"O2 浓度:    ------%"},
		{"SO2浓度:    ------mg/m3"},
		{"NO 浓度:    ------mg/m3"},
		{"NO2浓度:    ------mg/m3"},
		{"NOX浓度:    ------mg/m3"},
		{"CO 浓度:    ------mg/m3"},
		{"H2S浓度:    ------mg/m3"},
		{"CO2浓度:    ------%"}};
void GetGasFileRecodData(u32 FileNum,u32 Recod,float * GetData,u8 isDisplay)//获取当前气体记录数据
{
  SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
  
	char buff[61]={0};
	
	Screen_ResetDataBuffer();
	sprintf((char *)ps->Buffer,"%04d-",Recod);                   //20200514
	sprintf((char *)&buff[0],"%04d",SdFp->GasRecordMax);
	strcat((char *)ps->Buffer,(char *)&buff[0]);
	Screen_RefreshDataMulti(0x100,ps->Buffer,15*2);              //显示第几条信息
	
	SdFile_ReadGasRecord(FileNum,Recod);
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.G_record.arr[0]),16);//时间             20200514
	Screen_RefreshDataMulti(0x2E20,ps->Buffer,15*2);
	
	if(SdFp->rdWorkFile.G_record.arr[18]=='9')                                   //获取参与计算标志
		GetData[8]=1;
	else
		GetData[8]=0;
	
	FourBytes four;
	for(int i=0;i<7;i++)
	{
		strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.G_record.stru.rowdata[i+1][0]),10);//气体浓度
		four.FloatData=atof(buff);
		if(i<4)
		GetData[i]=four.FloatData;
		else
		GetData[i+1]=four.FloatData;	
	}
	//计算NOX
	if(GetData[2]==1000000||GetData[3]==1000000)
	{
	 GetData[4]=1000000;//NOX 浓度
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
	函数名?void SaveGasDataLoad(float *SaveData)
	描述：	保存烟气数据装载
  输入： 
	
*/
 void SaveDustDataLoad(float *SaveData)
{       u8 i;
	    Flashisp_DataStructure* pf=Flashisp_GetData();
	    Screen_DataStructure* ps=Screen_GetData();
	    Calculation_FormulaStructure * Cp=CalculationFormula_GetData();
	    Slave_DataStructure*  Sp=Slave_GetData();
			SaveData[0]  = Cp->Pd;//动压
	        SaveData[0]  = GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJDY); //平均动压
	
			SaveData[1]  = Cp->Ps;	//静压	
	        SaveData[1]  = GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJJY); //平均静压
	
			SaveData[2]  = Cp->Pt;//计算全压
	        SaveData[2]  = GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJQY); //平均全压
	
			SaveData[4]  = Cp->Ts ;         //烟温
	        SaveData[4]  = GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_PJYW); //平均烟温
	
			SaveData[5]  = Cp->Ba;	//大气压	
	
	        /*
	        float Pvalue;
	        if((pf->Data[FLASHIS_AdrXTSZBOX]&0x40)==0)
			Pvalue = CalculationFormula_p(SaveData[4],SaveData[5],SaveData[1]);//计算烟气密度
			else
			Pvalue = CalculationFormula_p(Cp->Th,SaveData[5],0.0);//计算烟气密度
	        
			
			SaveData[3]  = CalculationFormula_Vs(Cp->Kp,SaveData[0],Pvalue);	//计算流速
	        */
	        SaveData[3]  =Cp->Vs; //流速
			SaveData[3]  =GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_YQLS_YQLS); //平均流速
			
			SaveData[6]  = Cp->F;//获取烟道面积
			SaveData[7]  = Cp->Xsw*100;//含湿量
			SaveData[8]  = Cp->Kp ;	//皮托管系数
			SaveData[9]  = GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSZSXS);//折算系数
			SaveData[10] = GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSFHXS);//负荷系数
			//SaveData[11] = CalculationFormula_Qs(SaveData[3],SaveData[6]);//烟气流量
			SaveData[11] = Cp->Qs; //流量
			SaveData[11] = GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_YGLL);//平均烟气流量
			
			SaveData[12] = Cp->Qsnd;//标杆流量	
			SaveData[12] = GetFlaotdataFormFlash2Word(FLASHIS_AdrGKYC_CPJZ_BGLL);//平均标杆流量	
	     
	        SaveData[13] = ps->YC_FollowRate;//跟踪率
	        SaveData[14] = GetFlaotdataFormFlash2Word(FLASHIS_AdrYCCJ_CYSE_CZZJ);//采嘴直径
		    SaveData[15]=Cp->Pcs;//平均计压
			SaveData[16]=Cp->Tj;//平均计温
			SaveData[17]=Flux_GetData()->ChannelB.Sampling.SumVolume;        //工况体积
		    SaveData[18]=Flux_GetData()->ChannelB.Sampling.RefVolume;        //标况体积
			SaveData[19]=GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSYCZL);//重量		
            
		
			SaveData[20]=Sp->O2Concent.Value;
			
			SaveData[21]=Sp->SO2Concent.Value;

			SaveData[22]=Sp->NOConcent.Value;

			SaveData[23]=Sp->NO2Concent.Value;

			SaveData[24]=Sp->COConcent.Value;

			if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x01))==1)  //
			{
			SaveData[21]=SaveData[21]-SaveData[24]*GetFlaotdataFormFlash2Word(FLASHIS_AdrWHJZ_XTSZ_CO_SO2); //修正后SO2
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
			SaveData[28]=002; //样品编号
			//SaveData[29]= ps->YC_SampleAllTime;//总采样时长
			SaveData[29]= Flux_GetData()->ChannelB.Sampling.SecTimer;//总采样时长
			SaveData[30]=ps->QimiStat;
			SaveData[31]=(pf->Data[FLASHIS_AdrXTSZBOX]&(1<<4))==0x0000?0:1;
			SaveData[32]=GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSRYND);// 溶液浓度
			SaveData[33]=GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSDRTJ); // 定溶体积
			SaveData[34]=pf->Data[FFLASHIS_AdrYCCJ_YCWJCX_JSZTSM];                   //灶头数目
			SaveData[35]=GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSJZYHL); //基准氧气含量
            SaveData[36]=Sp->JWTem.Value; //计前温度 
			SaveData[37]=Cp->Pcs;//计前压	
			
			if(Sp->O2Concent.Value!=21)
            SaveData[38]= 21.0f/(21-Sp->O2Concent.Value);//过剩系数
			else
			SaveData[38]=SLAVE_ColFAULT;  //错误的数据，设为最多
}
//------
/* date 20200519
	函数名：ReadFlueData(u32 FileNum)
	描述：	保存烟尘文件
  输入：
	
*/
void SaveDustData(float *SaveData,u32 NumFile)
{

	 SdFile_DataStruct* SdFp=SdFile_GetData();
	 Screen_DataStructure* ps=Screen_GetData();
	 Flashisp_DataStructure* pf=Flashisp_GetData();
	 u8 buf[80]={0};
	 //装载日期时间
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.dat[0],Interf_dat_length);  //先填充空格
   sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min,ps->Clock.Clock.tm_sec);
   strncpy((char *)&SdFp->InterFace_data.dat[0],(char *)buf,Interf_dat_length);  
   //装载地址
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Adress[0],Adress_length-9-2);  //先填充空格49+/r/n
	 sprintf((char *)&buf[0]," %-30s",(char *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));  //
	 strncpy((char *)&SdFp->InterFace_data.Adress[0],(char *)&buf[0],30);   //flash里只存30个地址数据
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[0],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[0]);    //平均动压位置
	 strncpy((char *)&SdFp->InterFace_data.Other[0][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[1],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[1]);           //平均静压位置
	 	 strncpy((char *)&SdFp->InterFace_data.Other[1][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[2],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[2]);           //平均全压位置
	 	 strncpy((char *)&SdFp->InterFace_data.Other[2][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[3],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[3]);           //平均流速
	 	 strncpy((char *)&SdFp->InterFace_data.Other[3][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[4],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[4]);           //平均烟温
	 	 strncpy((char *)&SdFp->InterFace_data.Other[4][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[5],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[5]);           //大气压
	 	 strncpy((char *)&SdFp->InterFace_data.Other[5][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[6],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.4f",SaveData[6]);           //烟道截面
	 	 strncpy((char *)&SdFp->InterFace_data.Other[6][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[7],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[7]);           //含 湿 量
	 	 strncpy((char *)&SdFp->InterFace_data.Other[7][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[8],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[8]);           //皮管系数
	 	 strncpy((char *)&SdFp->InterFace_data.Other[8][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[9],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[9]);           //折算系数
	 	 strncpy((char *)&SdFp->InterFace_data.Other[9][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[10],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[10]);           //负荷系数
	 	 strncpy((char *)&SdFp->InterFace_data.Other[10][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[11],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[11]);   //烟气流量
	 	 strncpy((char *)&SdFp->InterFace_data.Other[11][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[12],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[12]);   //标干流量
	 	 strncpy((char *)&SdFp->InterFace_data.Other[12][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[13],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[13]);          //跟 踪 率
	 	 strncpy((char *)&SdFp->InterFace_data.Other[13][0],(char *)&buf[0],Interf_other_length);

	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[14],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[14]);          //采嘴直径
	 	 strncpy((char *)&SdFp->InterFace_data.Other[14][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[15],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[15]);          //平均计压
	 	 strncpy((char *)&SdFp->InterFace_data.Other[15][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[16],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.2f",SaveData[16]);          //平均计温
	 	 strncpy((char *)&SdFp->InterFace_data.Other[16][0],(char *)&buf[0],Interf_other_length);
	
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[17],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[17]);          //采样体积
	 	 strncpy((char *)&SdFp->InterFace_data.Other[17][0],(char *)&buf[0],Interf_other_length);
	
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[18],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[18]);          //标况体积
	 	 strncpy((char *)&SdFp->InterFace_data.Other[18][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[19],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.5f",SaveData[19]);            //烟尘净重20200519
	 	 strncpy((char *)&SdFp->InterFace_data.Other[19][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[20],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[20]);          //含 氧 量
	 	 strncpy((char *)&SdFp->InterFace_data.Other[20][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[21],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[21]);          //二氧化硫 
	 	 strncpy((char *)&SdFp->InterFace_data.Other[21][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[22],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[22]);          //一氧化氮
	 	 strncpy((char *)&SdFp->InterFace_data.Other[22][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[23],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[23]);          //二氧化氮 
	 	 strncpy((char *)&SdFp->InterFace_data.Other[23][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[24],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[24]);          //一氧化碳 
	 	 strncpy((char *)&SdFp->InterFace_data.Other[24][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[25],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.0f",SaveData[25]);          //硫 化 氢 
	 	 strncpy((char *)&SdFp->InterFace_data.Other[25][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[26],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[26]);          //二氧化碳
	 	 strncpy((char *)&SdFp->InterFace_data.Other[26][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[27],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %10d",(int32_t)SaveData[27]); //端口配置
	 	 strncpy((char *)&SdFp->InterFace_data.Other[27][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[28],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[28]);   //样品编号
	 	 strncpy((char *)&SdFp->InterFace_data.Other[28][0],(char *)&buf[0],Interf_other_length);

	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[29],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[29]);   //累计采时
	 	 strncpy((char *)&SdFp->InterFace_data.Other[29][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[30],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[30]);   //气 密 性
	 	 strncpy((char *)&SdFp->InterFace_data.Other[30][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[31],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[31]);   //采样类型
	 	 strncpy((char *)&SdFp->InterFace_data.Other[31][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[32],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[32]);          //溶液浓度
	 strncpy((char *)&SdFp->InterFace_data.Other[32][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[33],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[33]);          //定容体积
	 strncpy((char *)&SdFp->InterFace_data.Other[33][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[34],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10d",(int32_t)SaveData[34]);   //灶头数目
	 strncpy((char *)&SdFp->InterFace_data.Other[34][0],(char *)&buf[0],Interf_other_length);
	 
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[35],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[35]);          //基准含氧
	 strncpy((char *)&SdFp->InterFace_data.Other[35][0],(char *)&buf[0],Interf_other_length);
	
	 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[36],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[36]);          //计前温度
	 strncpy((char *)&SdFp->InterFace_data.Other[36][0],(char *)&buf[0],Interf_other_length);

 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[37],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[37]);          //计前压力
	 strncpy((char *)&SdFp->InterFace_data.Other[37][0],(char *)&buf[0],Interf_other_length);

 SdFile_ramlodair((char *)&SdFp->InterFace_data.Other[38],Interf_other_length);    //先填充空格
	 sprintf((char *)&buf[0],"   %-10.1f",SaveData[38]);          //过剩系数:
	 strncpy((char *)&SdFp->InterFace_data.Other[38][0],(char *)&buf[0],Interf_other_length);


}
/* date 20200520
	函数名：ReadDustFirstData(u32 FileNum)
	描述： 读取烟尘文件-第一页
  输入：
*/         
void ReadDustData(u32 FileNum)//根据文件号 读取烟尘信息
{
  SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	SdFile_ReadDustFile(FileNum);

//平均动压0:   0          Pa   
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[0][12]),10);
	ps->DisValue[0]=atof((char *)&ps->Buffer[0]);
//平均静压1:   -0.05      kPa  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[1][12]),10);
	ps->DisValue[1]=atof((char *)&ps->Buffer[0]);

//平均全压2:   -0.05      kPa  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[2][12]),10);
	ps->DisValue[2]=atof((char *)&ps->Buffer[0]);
//平均流速3:   0.0        m/s  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[3][12]),10);
	ps->DisValue[3]=atof((char *)&ps->Buffer[0]);
//平均烟温4:   206.0      ℃   
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[4][12]),10);
	ps->DisValue[4]=atof((char *)&ps->Buffer[0]);
//大 气 压5:   103.50     kPa  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[5][12]),10);
	ps->DisValue[5]=atof((char *)&ps->Buffer[0]);
//烟道截面6:   0.7854     m2   
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[6][12]),10);
	ps->DisValue[6]=atof((char *)&ps->Buffer[0]);
//含 湿 量7:   0.50       
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[7][12]),10);
	ps->DisValue[7]=atof((char *)&ps->Buffer[0]);
//皮管系数8:   0.84      
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[8][12]),10);
	ps->DisValue[8]=atof((char *)&ps->Buffer[0]);
//折算系数9:   1.00            
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[9][12]),10);
	ps->DisValue[9]=atof((char *)&ps->Buffer[0]);
//负荷系数10:   1.00            
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[10][12]),10);
	ps->DisValue[10]=atof((char *)&ps->Buffer[0]);
//烟气流量11:   0          m3/h 
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[11][12]),10);
	ps->DisValue[11]=atof((char *)&ps->Buffer[0]);
//标杆流量12:   0          m3/h 
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[12][12]),10);
	ps->DisValue[12]=atof((char *)&ps->Buffer[0]);
//跟 踪 率13:   0.00         
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[13][12]),10);
	ps->DisValue[13]=atof((char *)&ps->Buffer[0]);
//采样直径14:   0.8        mm  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[14][12]),10);
	ps->DisValue[14]=atof((char *)&ps->Buffer[0]);
//平均计压15:   -0.01      Kpa  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[15][12]),10);
	ps->DisValue[15]=atof((char *)&ps->Buffer[0]);
//平均计温16:   30.50      ℃   
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[16][12]),10);
	ps->DisValue[16]=atof((char *)&ps->Buffer[0]);
//采样体积17:   0.0        L   
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[17][12]),10);
	ps->DisValue[17]=atof((char *)&ps->Buffer[0]);
//标况体积18:   0.0        NL  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[18][12]),10);
	ps->DisValue[18]=atof((char *)&ps->Buffer[0]);
//烟尘静重19: 1.00000      g    
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[19][12]),10);
	ps->DisValue[19]=atof((char *)&ps->Buffer[0]);
//含 氧 量20:   64.41      %    
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[20][12]),10);
	ps->DisValue[20]=atof((char *)&ps->Buffer[0]);
//二氧化硫21:   34178.55   mg/m3
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[21][12]),10);
	ps->DisValue[21]=atof((char *)&ps->Buffer[0]);
//一氧化氮22:   5913.55    mg/m3
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[22][12]),10);
	ps->DisValue[22]=atof((char *)&ps->Buffer[0]);
//二氧化氮23:   595.61     mg/m3
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[23][12]),10);
	ps->DisValue[23]=atof((char *)&ps->Buffer[0]);
//一氧化碳24:   49793.92   mg/m3
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[24][12]),10);
	ps->DisValue[24]=atof((char *)&ps->Buffer[0]);
//硫 化 氢25:   1000000.00 mg/m3
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[25][12]),10);
	ps->DisValue[25]=atof((char *)&ps->Buffer[0]);
//二氧化碳26:   1000000.00 %    
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[26][12]),10);
	ps->DisValue[26]=atof((char *)&ps->Buffer[0]);
//端口配置27:      9999900      
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[27][12]),10);
	ps->DisValue[27]=atof((char *)&ps->Buffer[0]);
//样品编号28:   2              
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[28][12]),10);
	ps->DisValue[28]=atof((char *)&ps->Buffer[0]);
//累计采时29:   10         s    
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[29][12]),10);
	ps->DisValue[29]=atof((char *)&ps->Buffer[0]);
//气 密 性30:   0    
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[30][12]),10);
	ps->DisValue[30]=atof((char *)&ps->Buffer[0]);
//采样类型31:   0     
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[31][12]),10);
	ps->DisValue[31]=atof((char *)&ps->Buffer[0]);
//溶液浓度32:   0.0          
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[32][12]),10);
	ps->DisValue[32]=atof((char *)&ps->Buffer[0]);
//定容体积33:   0.0      
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[33][12]),10);
	ps->DisValue[33]=atof((char *)&ps->Buffer[0]);
//灶头数目34:   1               
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[34][12]),10);
	ps->DisValue[34]=atof((char *)&ps->Buffer[0]);
//基准含氧35:   1.0        %    
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[35][12]),10);
	ps->DisValue[35]=atof((char *)&ps->Buffer[0]);
//计前温度36:   30.5       ℃   
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[36][12]),10);
	ps->DisValue[36]=atof((char *)&ps->Buffer[0]);
//计前压力37:   -0.0       kPa  
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[37][12]),10);
	ps->DisValue[37]=atof((char *)&ps->Buffer[0]);
//过剩系数38:   -0.5            
   ps->DisValue[38]=CalculationFormula_a(ps->DisValue[20]);
}

/* date 2020-09-07  bye linzequan
	函数名：ReadLampBlackFirstData(u32 FileNum)
	描述： 读取油烟文件 --第一页
  输入：
*/        
void ReadLampBlackFirstData(u32 FileNum)
{
	SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	SdFile_ReadDustFile(FileNum);
	Screen_ResetDataBuffer();
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //文件号
	FourBytes four;
	char buf[61]={0};
	strncpy((char *)buf,(char *)&(SdFp->rdWorkFile.Other[31][12]),10);//烟气类型
	four.U32Data=atoi(buf);
	ps->YC_Type=four.U32Data;//烟气类型
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.dat[0]),10);//日期                 //
	Screen_RefreshDataMulti(0x0120,ps->Buffer,15*2);
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.dat[11]),5);//时间
	Screen_RefreshDataMulti(0x0140,ps->Buffer,15*2);
	
//----------------------	
	//sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[29]);//累计采样时长
	sprintf((char *)&buf[0],"%02d:%02d:%02d",(int32_t)ps->DisValue[29]/3600,((int32_t)ps->DisValue[29]%3600)/60,((int32_t)ps->DisValue[29]%3600)%60);

	Screen_RefreshDataMulti(0x0160,(u8 *)&buf[0],15*2);
	
	sprintf((char *)&buf[0],"%-10.4f",ps->DisValue[6]);           //烟道截面
	Screen_RefreshDataMulti(0x0180,(u8 *)&buf[0],15*2);
	
	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[4]);//烟温
	Screen_RefreshDataMulti(0x01A0,(u8 *)&buf[0],15*2);
	
	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[11]);//烟气流量
	Screen_RefreshDataMulti(0x01c0,(u8 *)&buf[0],15*2);
  /*
	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[35]);//基准痒含量
	ps->JZHYL=ps->DisValue[35];   //字符串转换成浮点数
	Screen_RefreshDataMulti(0x01C0,(u8 *)&buf[0],15*2);
	*/

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[7]);   //含湿量
	ps->HYL=ps->DisValue[7];   //字符串转换成浮点数
	Screen_RefreshDataMulti(0x01E0,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[1]);  //平均净压
	Screen_RefreshDataMulti(0x0400,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[0]);//平均动压
	Screen_RefreshDataMulti(0x0420,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[2]);//平均全压
	Screen_RefreshDataMulti(0x0440,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[5]);//大气压
	Screen_RefreshDataMulti(0x0460,(u8 *)&buf[0],15*2);

    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[16]);//计温
	Screen_RefreshDataMulti(0x0480,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[3]);//平均流速
	Screen_RefreshDataMulti(0x04A0,(u8 *)&buf[0],15*2);
	
	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[37]);//计压
	Screen_RefreshDataMulti(0x04C0,(u8 *)&buf[0],15*2);

  /*
	 sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[20]);//含氧量
	Screen_RefreshDataMulti(0x04C0,(u8 *)&buf[0],15*2);
	*/
	
	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[13]);//跟踪率
	Screen_RefreshDataMulti(0x0500,(u8 *)&buf[0],15*2);
	
	/*
    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[38]);//过剩系数
	ps->GSXS=ps->DisValue[38];   //字符串转换成浮点数
	if(ps->GSXS==1000000)  //错误的标志
	strcpy((char *)buf,"----------");
	Screen_RefreshDataMulti(0x0500,(u8 *)&buf[0],15*2);
	*/

    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[14]); //采嘴直径
	Screen_RefreshDataMulti(0x0520,(u8 *)&buf[0],15*2);

    sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[9]);//折算系数
	ps->ZSXS=atof((char *)&ps->Buffer[0]);   //字符串转换成浮点数
	
    /*
	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[10]);//出力系数
	ps->FHXS=ps->DisValue[10];   //字符串转换成浮点数
	Screen_RefreshDataMulti(0x0520,(u8 *)&buf[0],15*2);
	*/
                   //油烟浓度
	four.FloatData=CalculationFormula_Cc(ps->DisValue[32],ps->DisValue[33],ps->DisValue[18]/1000); //(溶液浓度，)			   
	sprintf((char *)&buf[0],"%-10.2f",four.FloatData);//标杆流量
	Screen_RefreshDataMulti(0x0540,(u8 *)&buf[0],15*2);
	 //折算浓度  0x760
	sprintf((char *)&buf[0],"%-10.2f",CalculationFormula_Yyzs(four.FloatData,ps->DisValue[12],ps->DisValue[34]));//计算油烟折算 
	Screen_RefreshDataMulti(0x0760,(u8 *)&buf[0],15*2);
	
	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[12]);//标杆流量
	Screen_RefreshDataMulti(0x0580,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[28]);//滤筒号
	Screen_RefreshDataMulti(0x05A0,(u8 *)&buf[0],15*2);

    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[17]); //工况体积
	Screen_RefreshDataMulti(0x05C0,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[18]); //标况体积
	Screen_RefreshDataMulti(0x05E0,(u8 *)&buf[0],15*2);


    /*
	sprintf((char *)&buf[0],"%-10.5f",ps->DisValue[19]);//烟尘净重
	ps->YCJZ=ps->DisValue[19];   //字符串转换成浮点数
	Screen_RefreshDataMulti(0x0760,(u8 *)&buf[0],15*2);
     */
   
	

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[32]);//溶液浓度
	ps->RYLD=ps->DisValue[32];   //字符串转换成浮点数
	Screen_RefreshDataMulti(0x0700,(u8 *)&buf[0],15*2);
	
	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[33]);//定容体积
	ps->DRTZ=ps->DisValue[33];   //字符串转换成浮点数
    Screen_RefreshDataMulti(0x0720,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[34]);//灶头数目
	ps->ZTSM=ps->DisValue[34];   //字符串转换成浮点数
	Screen_RefreshDataMulti(0x0740,(u8 *)&buf[0],15*2);
	
		
    SdFile_Serch_MaxofGasLinkFile();//查找烟气最大文件号
    ps->DRWJH= SdFp-> GasLinkFileMax;

	Screen_ResetDataBuffer();
	strcpy((char *)ps->Buffer,"        ");//清提示20200519
	Screen_RefreshDataMulti(0x2E20,ps->Buffer,15*2);

	//显示地址信息
	{
		
	strncpy((char*)ps->Buffer,(char *)&(SdFp->rdWorkFile.Adress[9]),30);
	}				
	 Screen_RefreshDataMulti(0x0560,ps->Buffer,15*2);
}
/* date 20200519
	函数名：ReadDustFirstData(u32 FileNum)
	描述： 读取烟尘文件-第一页
  输入：
*/         
void ReadDustFirstData(u32 FileNum)//根据文件号 读取烟尘信息
{
  SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	SdFile_ReadDustFile(FileNum);

	
	FourBytes four;
	char buf[61]={0};
	strncpy((char *)buf,(char *)&(SdFp->rdWorkFile.Other[31][12]),10);//烟气类型
	four.U32Data=atoi(buf);
	ps->YC_Type=four.U32Data;//烟气类型
	
	Screen_ResetDataBuffer();
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //文件号
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.dat[0]),10);//日期                 //
	Screen_RefreshDataMulti(0x0120,ps->Buffer,15*2);
	Screen_ResetDataBuffer();
	strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.dat[11]),5);//时间
	Screen_RefreshDataMulti(0x0140,ps->Buffer,15*2);
	
//----------------------	
	//sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[29]);//累计采样时长
	
	sprintf((char *)&buf[0],"%2d:%2d:%2d",(int32_t)ps->DisValue[29]/3600,((int32_t)ps->DisValue[29]%3600)/60,((int32_t)ps->DisValue[29]%3600)%60);//累计采样时长
	
	Screen_RefreshDataMulti(0x0160,(u8 *)&buf[0],15*2);
	
	sprintf((char *)&buf[0],"%-10.4f",ps->DisValue[6]);           //烟道截面
	Screen_RefreshDataMulti(0x0180,(u8 *)&buf[0],15*2);
	
  
	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[4]);//烟温
	Screen_RefreshDataMulti(0x01A0,(u8 *)&buf[0],15*2);
	

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[35]);//基准痒含量
	ps->JZHYL=ps->DisValue[35];   //字符串转换成浮点数
	Screen_RefreshDataMulti(0x01C0,(u8 *)&buf[0],15*2);
	

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[7]);   //含湿量
	ps->HYL=ps->DisValue[7];   //字符串转换成浮点数
	Screen_RefreshDataMulti(0x01E0,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[1]);  //平均净压
	Screen_RefreshDataMulti(0x0400,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[0]);//平均动压
	Screen_RefreshDataMulti(0x0420,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[2]);//平均全压
	Screen_RefreshDataMulti(0x0440,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[5]);//大气压
	Screen_RefreshDataMulti(0x0460,(u8 *)&buf[0],15*2);

    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[16]);//计温
	Screen_RefreshDataMulti(0x0480,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[3]);//平均流速
	Screen_RefreshDataMulti(0x04A0,(u8 *)&buf[0],15*2);

	 sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[20]);//含氧量
	Screen_RefreshDataMulti(0x04C0,(u8 *)&buf[0],15*2);
	
	
    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[38]);//过剩系数
	ps->GSXS=ps->DisValue[38];   //字符串转换成浮点数
	if(ps->GSXS==1000000)  //错误的标志
	strcpy((char *)buf,"----------");
	Screen_RefreshDataMulti(0x0500,(u8 *)&buf[0],15*2);
	
    sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[9]);//折算系数
	ps->ZSXS=atof((char *)&ps->Buffer[0]);   //字符串转换成浮点数
	
  
	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[10]);//出力系数
	ps->FHXS=ps->DisValue[10];   //字符串转换成浮点数
	Screen_RefreshDataMulti(0x0520,(u8 *)&buf[0],15*2);
	

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[11]);//烟气流量
	Screen_RefreshDataMulti(0x0540,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[12]);//标杆流量
	Screen_RefreshDataMulti(0x0580,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[28]);//滤筒号
	Screen_RefreshDataMulti(0x05A0,(u8 *)&buf[0],15*2);

    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[17]); //工况体积
	Screen_RefreshDataMulti(0x05C0,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[18]); //标况体积
	Screen_RefreshDataMulti(0x05E0,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.2f",ps->DisValue[13]);//跟踪率
	Screen_RefreshDataMulti(0x0700,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[37]);//计压
	Screen_RefreshDataMulti(0x0720,(u8 *)&buf[0],15*2);

    sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[14]); //采嘴直径
	Screen_RefreshDataMulti(0x0740,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.5f",ps->DisValue[19]);//烟尘净重
	ps->YCJZ=ps->DisValue[19];   //字符串转换成浮点数
	Screen_RefreshDataMulti(0x0760,(u8 *)&buf[0],15*2);

	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[32]);//溶液浓度
	ps->RYLD=ps->DisValue[32];   //字符串转换成浮点数
	
	sprintf((char *)&buf[0],"%-10.1f",ps->DisValue[33]);//定容体积
	ps->DRTZ=ps->DisValue[33];   //字符串转换成浮点数

	sprintf((char *)&buf[0],"%-10d",(int32_t)ps->DisValue[34]);//灶头数目
	ps->ZTSM=ps->DisValue[34];   //字符串转换成浮点数
		
  SdFile_Serch_MaxofGasLinkFile();//查找烟气最大文件号
  ps->DRWJH= SdFp-> GasLinkFileMax;

	Screen_ResetDataBuffer();
	strcpy((char *)ps->Buffer,"                              ");//清提示20200519
	Screen_RefreshDataMulti(0x2E20,ps->Buffer,15*2);

	//显示地址信息
	{
		
	strncpy((char*)ps->Buffer,(char *)&(SdFp->rdWorkFile.Adress[9]),30);
	}				
	 Screen_RefreshDataMulti(0x0560,ps->Buffer,15*2);
}
//---
/* date 20200520
	函数名：void GetDustSecondData (u32 FileNum)
	描述：	获取当前尘体数据 -第二页
  输入：void
*/
void GetDustSecondData(u32 FileNum)//获取当前尘体数据
{
  SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	float dbf,dbf1;

	u8 buff[61]={0};
	sprintf((char*)ps->Buffer,"%07d",(u32)FileNum); //文件号
	Screen_RefreshDataMulti(0x0100,ps->Buffer,15*2);
	
	dbf=ps->DisValue[19];//烟尘净重g
	if(ps->DisValue[17]!=0)                         //体积L
	dbf=dbf*1000/ps->DisValue[17]*1000;  //                g/L =mg/m^3
	else
	dbf=0.0f;
	sprintf((char *)&ps->Buffer[0],"%10.2f",dbf); //烟尘浓度
	Screen_RefreshDataMulti(0x0120,ps->Buffer,15*2);
	          //烟尘折算
	dbf1=dbf*ps->DisValue[38]/ps->DisValue[9]*ps->DisValue[10];  //折算=浓度*过剩系数/折算系数*负荷系数
	sprintf((char *)&buff[0],"%10.2f",dbf1); //
	Screen_RefreshDataMulti(0x0540,(u8 *)&buff[0],15*2);
	//烟尘排放
	dbf1=dbf*ps->DisValue[12]*0.000001f;  //折算=浓度*过剩系数/折算系数*负荷系数
	sprintf((char *)&buff[0],"%10.2f",dbf1); //
	Screen_RefreshDataMulti(0x0520,(u8 *)&buff[0],15*2);

//	SdFile_ReadDustFile(FileNum);
   

	//NOX 浓度
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
	//NOX 折算
	dbf=ps->DisValue[39];
	if((ps->DisValue[39]!=1000000)&&(ps->DisValue[38]!=1000000))
	{
	dbf1=dbf*ps->DisValue[38]/ps->DisValue[9]*ps->DisValue[10];  //折算=浓度*过剩系数/折算系数*负荷系数
    sprintf((char*)ps->Buffer,"%10.2f",dbf1); //
	}
	else
	{
	sprintf((char*)ps->Buffer,"----------");	
	}
	Screen_RefreshDataMulti(0x0480,ps->Buffer,15*2);
	//NOX 排放
    if(ps->DisValue[39]!=1000000)
	{
	dbf1=dbf*ps->DisValue[12]*0.000001f;  //排放=浓度*标杆流量*0.000001
    sprintf((char*)ps->Buffer,"%10.2f",dbf1); //
	}
	else
	{
		sprintf((char*)ps->Buffer,"----------");	
	}
	Screen_RefreshDataMulti(0x05C0,ps->Buffer,15*2);

  //SO2,NO,NO2,CO,H2S  浓度
	for(int i=0;i<5;i++)
	{
		if(ps->DisValue[21+i]!=1000000)  //从氧气开始
	{
	  sprintf((char*)ps->Buffer,"%10.2f ",ps->DisValue[21+i]);
	}else
	{
	  sprintf((char*)ps->Buffer,(char *)"----------");
	}

	if(i<3)   //0，1，2
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

//SO2,NO,NO2,CO,H2S折算	
for(int i=0;i<5;i++)
{
  if((ps->DisValue[21+i]!=1000000)&&(ps->DisValue[38]!=1000000))
	{ 
		dbf1=ps->DisValue[21+i]*ps->DisValue[38]/ps->DisValue[9]*ps->DisValue[10];  //折算=浓度*过剩系数/折算系数*负荷系数
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
	//SO2,NO,NO2,CO,H2S排放
for(int i=0;i<5;i++)
  {
		if(ps->DisValue[21+i]!=1000000)
		{
			sprintf((char*)ps->Buffer,"%10.2f",ps->DisValue[i]*0.000001f*ps->DisValue[12]); //排放量 测量*标杆流量*0.000001;

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
			sprintf((char*)ps->Buffer,"%10.2f",ps->DisValue[21+5]); //浓度

		}else
		{
			sprintf((char*)ps->Buffer,(char *)"----------");
		}
	   Screen_RefreshDataMulti(0x01E0,ps->Buffer,15*2);
}
//---
/* date 20200520
	函数名：void GetDustJS_INpuData (u32 FileNum)
	描述：	获取当前计算导入烟气文件其他数据数据
  输入：FileNum烟气文件号
	      Screen_Data.DRWJH 导入的文件号
  输出：0,导入失败; 1导入成功
*/
u8 GetDustJS_INpuData(u32 FileNum)
{
  SdFile_DataStruct* SdFp=SdFile_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
//	char buff[61]={0};
	
	if(FileNum==0)  //无导入文件，导入失败
		return 0;
	FourBytes four;
	if(SdFile_ReadGasFile(FileNum,0x01)==FR_OK)  //读取数据正常
	{
		for(int i=0;i<7;i++)
		{
	 	Screen_ResetDataBuffer();
	    strncpy((char *)ps->Buffer,(char *)&(SdFp->rdWorkFile.Other[13+i][12]),10);//含氧量
		four.FloatData=atof((char *)ps->Buffer);
		ps->DisValue[20+i]=	four.FloatData;
		}
	  return 1;//导入成功
	}
	else
	{
		return 0;//无导入文件，导入失败
	}
	
}
//---
 /* date 2020-03-10
	函数名aveGasJS_InputDataLoad(float *SaveData)
	描述：	保存输入计算数据
  输入：
	
*/
 void SaveGasJS_InputDataLoad(float *SaveData)
{

	Screen_DataStructure* ps=Screen_GetData();
	SdFile_DataStruct* SdFp=SdFile_GetData();
	char buff[100];
	FourBytes four;
	for(int i=0;i<39;i++)
	{
		strncpy((char *)buff,(char *)&(SdFp->rdWorkFile.Other[i][12]),10);//原始数据装载
		four.FloatData=atof(buff);
		SaveData[i]=four.FloatData;
	}
	   //装载输入烟尘数据		
		

      SaveData[9]  = GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSZSXS);//折算系数
			SaveData[10] = GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSFHXS);//负荷系数
			SaveData[19]=GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSYCZL);//重量				
			SaveData[20]=GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSHYL);//氧含量
			SaveData[35]=GetFlaotdataFormFlash2Word(FFLASHIS_AdrYCCJ_YCWJCX_JSJZYHL); //基准氧气含量
 
		

}
//---
//-------------------------------------------------以下为打印接口----------------------------------

/* date 2020-03-25
	函数名：PrintPara(float *PrintData)
	描述：	打印各个参数公用程序
  输入：
	
*/
void PrintPara(float *PrintData,u8 Max)
{     u8 buf[80]={0};
      u8 i;
      Printer_Structure *Pp=Printer_GetData();
      Screen_DataStructure* ps=Screen_GetData();

      SdFile_ramlodair((char *)&Pp->bianhao[0],9);
      sprintf((char *)&buf[0],"%08d",(int32_t)PrintData[0]);   //仪器编号
      strncpy((char *)&Pp->bianhao[0],(char *)&buf[0],8);	   //编号长度为8
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
	函数名：PrintPressurePara(float *PrintData)
	描述：	打印压力参数
  输入：  仪器编号    PrintData[0]  =8位
          大气压零点  PrintData[1]  ,两位小数
          计压零点    PrintData[2]  ,两位小数  B路静压
          静压零点    PrintData[3]  ,两位小数  A路静压
          动压零点    PrintData[4]  ,无小数  A路动压
          尘流压零点  PrintData[5]  ,无小数  B路动压
          气流压零点  PrintData[6]  ,无小数  B路动压


	
*/
void PrintPressurePara(float *PrintData)
{    
     // PrintPara(PrintData,6);
	    u8 buf[80]={0};
      u8 i;
      Printer_Structure *Pp=Printer_GetData();
      Screen_DataStructure* ps=Screen_GetData();

      SdFile_ramlodair((char *)&Pp->bianhao[0],9);
      sprintf((char *)&buf[0],"%08d",(int32_t)PrintData[0]);   //仪器编号
      strncpy((char *)&Pp->bianhao[0],(char *)&buf[0],8);	   //编号长度为8
		//	strcpy((char *)&Pp->bianhao[0],(char *)"12345678");	
      SdFile_ramlodair((char *)&Pp->dat[0],17);
      sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min);
			strncpy((char *)&Pp->dat[0],(char*)buf,16);
			//strcpy((char *)&Pp->dat[0],(char *)"2019-12-20 14:20");
			
			for(i=0;i<3;i++)  
			{
			SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i],11);
			sprintf((char *)&buf[0],"%10.2f",PrintData[i+1]);   // 两位小数
			strncpy((char *)&Pp->Para_StrData.databuf[i],(char *)&buf[0],10);
			}
			for(i=3;i<6;i++)
			{
			SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i],11);
			sprintf((char *)&buf[0],"%10d",(int32_t)PrintData[i+1]);   //无小数
			strncpy((char *)&Pp->Para_StrData.databuf[i],(char *)&buf[0],10);
			}

     Print_PressureParaOrder(Printer_GetData());
}
/* date 2020-03-25
	函数名：PrintTemperaturePara(float *PrintData)
	描述：	打印温度参数
  输入：
          仪器编号       PrintData[0]  =8位
          干球/烟温零点  PrintData[1] ,两位小数
          干球/烟温倍率  PrintData[2]  ,两位小数
          湿球零点       PrintData[3]   ,两位小数
          湿球 倍率      PrintData[4]   ,两位小数
          计温零点       PrintData[5]   ,两位小数
  
*/
void PrintTemperaturePara(float *PrintData)
{
	  // PrintPara(PrintData,6);
	 	  u8 buf[80]={0};
      u8 i;
      Printer_Structure *Pp=Printer_GetData();
      Screen_DataStructure* ps=Screen_GetData();

      SdFile_ramlodair((char *)&Pp->bianhao[0],9);
      sprintf((char *)&buf[0],"%08d",(int32_t)PrintData[0]);   //仪器编号
      strncpy((char *)&Pp->bianhao[0],(char *)&buf[0],8);	   //编号长度为8
		//	strcpy((char *)&Pp->bianhao[0],(char *)"12345678");	
      SdFile_ramlodair((char *)&Pp->dat[0],17);
      sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min);
			strncpy((char *)&Pp->dat[0],(char*)buf,16);
			//strcpy((char *)&Pp->dat[0],(char *)"2019-12-20 14:20");
			
			for(i=0;i<5;i++)
			{
			SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i],11);
			sprintf((char *)&buf[0],"%10.2f",PrintData[i+1]);   //两位小数
			strncpy((char *)&Pp->Para_StrData.databuf[i],(char *)&buf[0],10);
			}
			
	   Print_TemperatureParaOrder(Printer_GetData());
}

/* date 2020-03-25
	函数名：PrintFlowPara(float *PrintData)
	描述：	打印流量参数
  输入： 
          仪器编号       PrintData[0]   =8位
          环境温度       PrintData[1]   1位小数
          大气压         PrintData[2]   1位小数
          1.0L           PrintData[3]   4位小数
          20L            PrintData[4]   4位小数
          30L            PrintData[5]   4位小数
          40L            PrintData[6]   4位小数
          50L            PrintData[7]   4位小数
          60L            PrintData[8]   4位小数
          70L            PrintData[9]   4位小数
          80L            PrintData[10]  4位小数
          90L            PrintData[11]  4位小数
          100L           PrintData[12]  4位小数
	
*/
void PrintFlowPara(float *PrintData)
{
	// PrintPara(PrintData,12);
	  u8 buf[80]={0};
      u8 i;
      Printer_Structure *Pp=Printer_GetData();
      Screen_DataStructure* ps=Screen_GetData();

      SdFile_ramlodair((char *)&Pp->bianhao[0],9);
      sprintf((char *)&buf[0],"%08d",(int32_t)PrintData[0]);   //仪器编号
      strncpy((char *)&Pp->bianhao[0],(char *)&buf[0],8);	   //编号长度为8
		//	strcpy((char *)&Pp->bianhao[0],(char *)"12345678");	
      SdFile_ramlodair((char *)&Pp->dat[0],17);
      sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min);
			strncpy((char *)&Pp->dat[0],(char*)buf,16);
			//strcpy((char *)&Pp->dat[0],(char *)"2019-12-20 14:20");
			
			for(i=0;i<2;i++)
			{
			SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i],11);
			sprintf((char *)&buf[0],"%10.1f",PrintData[i+1]);   //两位小数
			strncpy((char *)&Pp->Para_StrData.databuf[i],(char *)&buf[0],10);
			}
			for(i=2;i<12;i++)
			{
			SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i],11);
			sprintf((char *)&buf[0],"%10.4f",PrintData[i+1]);   //4位小数
			strncpy((char *)&Pp->Para_StrData.databuf[i],(char *)&buf[0],10);
			}
	
	    Print_FlowParaOrder(Printer_GetData());
}
/* date 2020-03-25
	函数名：PrintConcentraPara(float *PrintData)
	描述：	打印浓度参数
  输入：
  仪器编号        PrintData[0]   =8位
	" O2     零点:",PrintData[1] ,无小数
	" O2 低端倍率:",PrintData[2] ,4位小数
	" O2 中端倍率:",PrintData[3] ,4位小数
	" O2 高端倍率:",PrintData[4] ,4位小数
	"SO2     零点:",PrintData[5] ,无小数
	"SO2 低端倍率:",PrintData[6] ,4位小数
	"SO2 中端倍率:",PrintData[7],4位小数
	"SO2 高端倍率:",PrintData[8],4位小数
	" NO     零点:",PrintData[9]  ,无小数
	" NO 低端倍率:",PrintData[10],4位小数
	" NO 中端倍率:",PrintData[11],4位小数
	" NO 高端倍率:",PrintData[12],4位小数
	"NO2     零点:",PrintData[13],无小数
	"NO2 低端倍率:",PrintData[14],4位小数
	"NO2 中端倍率:",PrintData[15],4位小数
	"NO2 高端倍率:",PrintData[16],4位小数
	" CO     零点:",PrintData[17],无小数
	" CO 低端倍率:",PrintData[18],4位小数
	" CO 中端倍率:",PrintData[19],4位小数
	" CO 高端倍率:",PrintData[20],4位小数
	"H2S     零点:",PrintData[21],无小数
	"H2S 低端倍率:",PrintData[22],4位小数
	"H2S 中端倍率:",PrintData[23],4位小数
	"H2S 高端倍率:",PrintData[24],4位小数
	"CO2     零点:",PrintData[25],无小数
	"CO2 低端倍率:",PrintData[26],4位小数
	"CO2 中端倍率:",PrintData[27],4位小数
	"CO2 高端倍率:",PrintData[28],4位小数
	
*/
void PrintConcentraPara(float *PrintData)
{
	// PrintPara(PrintData,29);
	   u8 buf[80]={0};
      u8 i;
      Printer_Structure *Pp=Printer_GetData();
      Screen_DataStructure* ps=Screen_GetData();

      SdFile_ramlodair((char *)&Pp->bianhao[0],9);
      sprintf((char *)&buf[0],"%08d",(int32_t)PrintData[0]);   //仪器编号
      strncpy((char *)&Pp->bianhao[0],(char *)&buf[0],8);	   //编号长度为8
		//	strcpy((char *)&Pp->bianhao[0],(char *)"12345678");	
      SdFile_ramlodair((char *)&Pp->dat[0],17);
      sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min);
	  strncpy((char *)&Pp->dat[0],(char*)buf,16);
			//strcpy((char *)&Pp->dat[0],(char *)"2019-12-20 14:20");
			
			for(i=0;i<7;i++)
			{
				
				SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i*4],11);
				
				if((i==0)||(i==6))
				sprintf((char *)&buf[0],"%10.1f",PrintData[i*4+1]);   //两位小数
				else
				sprintf((char *)&buf[0],"%10.0f",PrintData[i*4+1]);   //两位小数
				
				strncpy((char *)&Pp->Para_StrData.databuf[i*4],(char *)&buf[0],10);

				SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i*4+1],11);
				sprintf((char *)&buf[0],"%10.3f",PrintData[i*4+3]);   //4位小数       //读低端浓渡值，modify by alfred, 2020.07.09
				strncpy((char *)&Pp->Para_StrData.databuf[i*4+1],(char *)&buf[0],10);	//显示低端浓渡值

				SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i*4+2],11);
				sprintf((char *)&buf[0],"%10.3f",PrintData[i*4+2]);   //4位小数			//读中端浓渡值，modify by alfred, 2020.07.09
				strncpy((char *)&Pp->Para_StrData.databuf[i*4+2],(char *)&buf[0],10);	//显示中端浓渡值

				SdFile_ramlodair((char *)&Pp->Para_StrData.databuf[i*4+3],11);
				sprintf((char *)&buf[0],"%10.3f",PrintData[i*4+4]);   //4位小数
				strncpy((char *)&Pp->Para_StrData.databuf[i*4+3],(char *)&buf[0],10);
					
			}
		
	
	 Print_ConcentraParaOrder(Printer_GetData());
}
/* date 2020-03-25
	函数名：PrintCurTest(float *PrintData)
	描述：	打印正在测试参数
  输入：PrintData 浮点数据指针，Ps 打印选择标志指针，Is 硬件安装标志指针

         仪器编号        PrintData[0]   =8位
O2浓度          :PrintData[1]
SO2浓度          :PrintData[2]
NO浓度           :PrintData[3]
NO2浓度          :PrintData[4]
NOX浓度          :PrintData[5]
CO浓度           :PrintData[6]
H2S浓度          :PrintData[7]
CO2浓度          :PrintData[8]
	
*/
void PrintCurTest(float *PrintData,u8 *Ps,u8 *Is)
{
	u8 buf[80]={0};
	u8 i;
	Printer_Structure *Pp=Printer_GetData();
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	SdFile_ramlodair((char *)&Pp->bianhao[0],9);
	sprintf((char *)&buf[0],"%8d",(int32_t)PrintData[0]);   //仪器编号
	strncpy((char *)&Pp->bianhao[0],(char *)&buf[0],8);	   //编号长度为8
	//	strcpy((char *)&Pp->bianhao[0],(char *)"12345678");	
	SdFile_ramlodair((char *)&Pp->dat[0],17);               
	sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min);   //当前时间
	strncpy((char *)&Pp->dat[0],(char*)buf,16);
	//strcpy((char *)&Pp->dat[0],(char *)"2019-12-20 14:20");

	//装载地址
	SdFile_ramlodair((char *)&Pp->address[0],51);  //先填充空格49+/r/n
	sprintf((char *)&buf[0],"%-30s",(char *)&(pf->Data[FLASHIS_AdrXTSZCYDZ]));  //
	strncpy((char *)&Pp->address[0],(char *)&buf[0],30);   //flash里只存30个地址数据

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
	函数名PrintWorkFile(u32 starfile,u32 endfile,u8 *Ps)
	描述：	打印文件公共部分
输入：  strfile :开始文件号
        endfile :结束文件号
        ps :打印选中数组指针
        PrintData[0]: 仪器编号
        PrintData[1]: 开始文档
        PrintData[2]: 结束文档
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
	
	if(Pp->print_start_file > Pp->print_end_file)//调换文件号
	{
		file_tmp = Pp->print_end_file;
		Pp->print_end_file = Pp->print_start_file;
		Pp->print_start_file = file_tmp;
		Pp->print_FileNomber = Pp->print_start_file;
	}

	SdFile_ramlodair((char *)&Pp->bianhao[0],9);
	sprintf((char *)&buf[0],"%08d",(int32_t)PrintData[0]);   //仪器编号
	strncpy((char *)&Pp->bianhao[0],(char *)&buf[0],8);	   //编号长度为8
	//	strcpy((char *)&Pp->bianhao[0],(char *)"12345678");	
	SdFile_ramlodair((char *)&Pp->dat[0],17);               
	sprintf((char*)buf,"%02d-%02d-%02d %02d:%02d",ps->Clock.Clock.tm_year+SCREEN_YearBase,ps->Clock.Clock.tm_mon,ps->Clock.Clock.tm_mday,ps->Clock.Clock.tm_hour,ps->Clock.Clock.tm_min);   //当前时间
	strncpy((char *)&Pp->dat[0],(char*)buf,16);
	//strcpy((char *)&Pp->dat[0],(char *)"2019-12-20 14:20");


	for(i=0;i<33;i++)
	{
		Pp->pselect[i]=Ps[0];	
		Ps++;
	}
	
}
/* date 2020-03-25
	函数名PrintWorkFile(u32 starfile,u32 endfile,u8 *Ps)
	描述：	打印工况文件
输入：  strfile :开始文件号
        endfile :结束文件号
        ps :打印选中数组指针
        PrintData[0]: 开始文件
        PrintData[1]: 结束文件

*/
void PrintWorkFile(u8 *Ps,float *PrintData)
{ 
  Printer_GetData()->PrintSelect=0;	
	PrintFile_com(Ps,PrintData);
	Print_WorkFileOrder(Printer_GetData());
}
/* date 2020-03-25
	函数名PrintGasFile(float *PrintData)
	描述：	打印烟气文件

  输入：  
	
*/
void PrintGasFile(u8 *Ps,float *PrintData)
{
	Printer_GetData()->PrintSelect=0;
	PrintFile_com(Ps,PrintData);
	Print_GasFileOrder(Printer_GetData());
}
/* date 2020-03-25
	函数名PrintDustFile(float *PrintData)
	描述：	打印烟气文件
  输入：
	
*/
void PrintDustFile(u8 *Ps,float *PrintData)
{
	Printer_GetData()->PrintSelect=0;
	PrintFile_com(Ps,PrintData);
	Print_DustFileOrder(Printer_GetData());
}
/* date 2020-03-25
	函数名PrintGasRecordFile
	描述：	打印烟气记录文件
  输入：
	
*/
void PrintGasRecordFile(u8 *Ps,float *PrintData)
{
	PrintFile_com(Ps,PrintData);

	Print_GasRecordFileOrder(Printer_GetData());
}
/* 	date 20200528
	函数名：Screen_WaterPumOpenServo(void) 
	描述：	 启动水泵线程
  	输入：void
*/
void Screen_WaterPumOpenServo(void)  //启动水泵线程
{
	Screen_DataStructure* ps=Screen_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	if(pf->Data[FLASHIS_AdrXTSZBOX ]&0x08)  //判断是否选择自动排水
	ps->WaterPumRunmod=1;
}
/* 	date 20200528
	函数名：Screen_WaterPumCloseServo(void)
	描述：	关闭水泵线程
  	输入：void
*/
void Screen_WaterPumCloseServo(void)//关闭水泵线程
{
	Screen_DataStructure* ps=Screen_GetData();
	ps->WaterPumRunmod=4;
}


/* 	date 20200528
	函数名：Screen_WaterPumServo(void)
	描述：	抽水泵的监控伺服
  	输入：void
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
		case 1:  //初始化  装载定时器
		{
			ps->WaterPumTimer=pf->Data[FLASHIS_AdrXTSZZDPSJG]*60;
			if(ps->WaterPumTimer!=0)    //间隔时间为0可以全速运行
			Slave_CloseWaterPump();       //关水泵20200528
			ps->WaterPumRunmod=2;
			break;
		}
		case 2:
		{  if(ps->WaterPumTimer==0)
			{
			ps->WaterPumTimer=pf->Data[FLASHIS_AdrXTSZZDPSSC];
			Slave_OpenWaterPump();       //打开水泵20200528
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
			Slave_CloseWaterPump();       //关水泵20200528
			ps->WaterPumRunmod=0;
			break;
		}
		default:
		{
			break;
		}
		
	}
	
}


//-------------------------调试函数----------------------------
//---
/*
	函数名： void Print_DebugASCII(u8 *p,u8 len)
	描述：		该函数为AT.c文件的ASCII调试文件
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
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{  //调试打印接口
		case 1:
		{//
			if(AT_GetData()->Mode==0)     //在透传模式
			{
				  PrintPressurePara((float *)&Testdata[0]);
				  printf("正在打印压力参数");
			}
			else
			{
					printf("不在透传模式");
			}
			break;
		}
		case 2:
		{//
			if(AT_GetData()->Mode==0)     //在透传模式
			{
				PrintTemperaturePara((float *)&Testdata[0]);
				printf("正在打印温度参数");
			}
			else
			{
					printf("不在透传模式");
			}
			break;
		}
		case 3:
		{//
			if(AT_GetData()->Mode==0)     //在透传模式
			{
				PrintFlowPara((float *)&Testdata[0]);
				printf("正在打印流量参数");
			}
			else
			{
					printf("不在透传模式");
			}
			break;
		}
		case 4:
		{//
			if(AT_GetData()->Mode==0)     //在透传模式
			{
				PrintConcentraPara((float *)&Testdata[0]);
				printf("正在打印浓度参数");
			}
			else
			{
					printf("不在透传模式");
			}
			break;
		}
		case 5:
		{//
			if(AT_GetData()->Mode==0)     //在透传模式
			{
				PrintCurTest((float *)&Testdata[0],(u8 *)&PN33[0],(u8 *)&IN8[0]);
				printf("正在打印正在测试数据");
			}
			else
			{
					printf("不在透传模式");
			}
			break;
		}
		case 6:
		{//
			if(AT_GetData()->Mode==0)     //在透传模式
			{
				PrintWorkFile((u8 *)&PN33[0],(float *)&Testdata[0]);
				printf("正在打印工况文件");
			}
			else
			{
					printf("不在透传模式");
			}
			break;
		}
		case 7:
		{//
			if(AT_GetData()->Mode==0)     //在透传模式
			{
				PrintGasFile((u8 *)&PN33[0],(float *)&Testdata[0]);
				printf("正在打印烟气文件");
			}
			else
			{
					printf("不在透传模式");
			}
			break;
		}
		case 8:
		{//
			if(AT_GetData()->Mode==0)     //在透传模式
			{
				PrintDustFile((u8 *)&PN33[0],(float *)&Testdata[0]);
				printf("正在打印烟尘文件");
			}
			else
			{
					printf("不在透传模式");
			}
			break;
		}
		case 9:
		{//
			if(AT_GetData()->Mode==0)     //在透传模式
			{
				PrintGasRecordFile((u8 *)&PN33[0],(float *)&Testdata[0]);
				printf("正在打印烟气记录");
			}
			else
			{
				printf("不在透传模式");
			}
			break;
		}
	}
}

/* 	date 2020-05-13 hjl add
	函数名：void GetKjMm (void)
	描述：	获取开机密码
  	输入：void
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
	函数名：void GetRandomNum (void)
	描述：	获取4位随机验证码
  	输入：void
*/
u16 GetRandomNum(void)
{
    u16 Random;
	u32 seed;
	
	Screen_DataStructure *ps = Screen_GetData();
	struct tm *pt = &ps->Clock.Clock;

	//以时间和随机数之和为种子
	seed = (pt->tm_sec << 24) + (pt->tm_min << 16) + (pt->tm_hour << 8) + pt->tm_mday + Random;
    srand((unsigned long)seed);
    Random = (u16)(rand() % (9999 - 1000 + 1) + 1000);	//产生1000~9999随机数
    ps->RandomNum = Random;
    return Random;
}

//----2020-05-27
/*
	函数名：u8 ProcessBlueToothInitial(u32 lPassword)
	描述：	该函数用于处理蓝牙初始化，对蓝牙模块进行密码设定，主从模式设定，器件复位的操作。
	输入：	蓝牙配对密码
	影响：	蓝牙模式结构体中定时器的值。
	修改记录： 1. 2020-05-25, 新增, Add by alfred.
*/
u8 ProcessBlueToothInitial(u32 lPassword)
{  Screen_DataStructure* p = Screen_GetData();
	u8 res = BT_DEFAULT; //res默认返回0， 超时返回1， 初始化成功返回2
	u8 cTempBuffer[6];
	switch(g_BlueToothInitStateMachine)
	{
		case BLUETOOTH_INIT_STATE_MACHINE_INPUT_PASSWORD:
		{
			sprintf((char *)cTempBuffer,"%d",lPassword);
			if(AT_GetData()->Mode==1)
				AT_Password_Com(cTempBuffer); //设置密码
			g_BlueToothInitStateMachine = BLUETOOTH_INIT_STATE_MACHINE_PASSWORD_WAITOK;
			AT_GetData()->EnableBTtimer = true;
			AT_GetData()->sBTtimer = 3000;					
		}break;
		
		//设置密码后等待回应OK
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
		
		//设置主模式
		case BLUETOOTH_INIT_STATE_MACHINE_SET_MODE:
		{
			if(AT_GetData()->sBTtimer == 0) //延时500ms再执行下一条命令
			{
				if(AT_GetData()->Mode==1)
					AT_Role_Com();	//设置主模式
				AT_GetData()->sBTtimer = 10000;
				g_BlueToothInitStateMachine = BLUETOOTH_INIT_STATE_MACHINE_SET_MODE_WAITOK;
			}
		}break;	
		
		//设置完主模式后等待回应OK
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
		
		//蓝牙模块复位
		case BLUETOOTH_INIT_STATE_MACHINE_RESET_DEVICE:
		{
			if(AT_GetData()->sBTtimer == 0) //延时500ms再执行下一条命令
			{
				if(AT_GetData()->Mode==1)
					AT_Reset_Com();	//模块复位
				AT_GetData()->sBTtimer = 10000;
				g_BlueToothInitStateMachine = BLUETOOTH_INIT_STATE_MACHINE_WAIT_FINISH;
			}			
		}
		break;
		
		//等待回应OK，完成所有初始化。
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
	函数名：u8 ProcessE28Initial(u16 adr,u8 ch)
	描述：	用于设置E28地址与信道
	输入：	蓝牙配对密码
	影响：	蓝牙模式结构体中定时器的值。
	修改记录： 1. 2020.12.01, 新增, Add by linzequan.
*/

u8 ProcessE28Initial(u16 adr,u8 ch)
  {   Screen_DataStructure* p = Screen_GetData();
	  u8 res = 0; //res默认返回0， 超时返回1， 初始化成功返回2
	  switch(p->g_E28_Set_StateMachine)
	  {
		  case E28_SET_STATE_MACHINE_SLEEP:
			  
			   break;
		  case E28_SET_STATE_MACHINE_INIT:
			   //向副板发送设置E28命令
			   Slave_GetData()->E28.STATE=1;
		       p->uE28Timer=2000;  //两秒监控
		       p->g_E28_Set_StateMachine=E28_SET_STATE_MACHINE_WAITOK;
			   break;
		  case E28_SET_STATE_MACHINE_WAITOK:
			   if(p->uE28Timer==0)
			   { //超时了
				   res=1;  //超时
				   p->g_E28_Set_StateMachine=E28_SET_STATE_MACHINE_SLEEP;
			   }
			   else  //时间没到，检测
			   {
				   if((Slave_GetData()->E28.ADRRset==Slave_GetData()->E28.ADRRRD24G)&&(Slave_GetData()->E28.CHset==Slave_GetData()->E28.CHRD24G))
				   { //发出与反回一致
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
	函数名：u8 TemperatureCaribration(u8 *pWarning, Flashisp_DataStructure *pf, u8 MenuPos)
	描述：	该函数用于处理用外部电阻校准温度。
	输入：	ps->warning, FLASH结构体指针,菜单光标位置。
	影响：	屏幕结构体，flash结构体。
	修改记录： 1. 2020-06-01, 新增, Add by alfred.
*/
u8 TemperatureCaribration(u8 *pWarning, u8 MenuPos)
{
	u16 sValueADC;	
	Flashisp_DataStructure* pf=Flashisp_GetData();
	static TEMP_POINT ywTempPointY1;	//Y1为实际测点的点(即外部电阻分别在100欧和174欧时测得的实际温度值）；Y2为理想温度值。

	switch(*pWarning)
	{
		case 0: 
		{
			*pWarning = 0xA1;	//提示输入100欧电阻.
			break;
		}
		case 0xA1:
		{
			if(MenuPos == 1) sValueADC = Slave_GetData()->Data[SLAVE_MAP_YWTem].Raw;	//读入100欧时烟温的ADC值
			else if(MenuPos == 2) sValueADC = Slave_GetData()->Data[SLAVE_MAP_SQTem].Raw; //读入100欧时湿球的ADC值
			
			if((sValueADC>768)&&(sValueADC<855)) //在允许范围内
			{
				*pWarning = 0xA3;	//提示输入100欧电阻
				
				ywTempPointY1.A=sValueADC;   //
		
			}
			else
			{
				*pWarning = 0xAA;	//100欧电阻输入错误
			}
			break;
		}
		case 0xA3:
		{
			if(MenuPos == 1) 
			{
				sValueADC = Slave_GetData()->Data[SLAVE_MAP_YWTem].Raw;	//读入174欧时烟温的ADC值
				
			}
			else if(MenuPos == 2) 
			{
				sValueADC = Slave_GetData()->Data[SLAVE_MAP_SQTem].Raw; //读入174欧时湿球的ADC值
			
			}
			
			ywTempPointY1.B=sValueADC;   //
			
		
			if((sValueADC>1300)&&(sValueADC<1600)) //在允许范围内	
			{
				FourBytes fourB, fourK;

				//先Y由零点求B
				fourB.FloatData=50-CalcAdcToTemperature(ywTempPointY1.A);
				fourK.FloatData=195/(CalcAdcToTemperature(ywTempPointY1.B)-CalcAdcToTemperature(ywTempPointY1.A));

				if(MenuPos == 1)
				{
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_LDGQ] = fourB.U16Data[0] ; //保存值
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_LDGQ+1] = fourB.U16Data[1] ; //保存值
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_LDYW] = fourB.U16Data[0] ; //保存值
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_LDYW+1] = fourB.U16Data[1] ; //保存值

					pf->Data[FLASHIS_AdrWHJZ_WDJZ_BLGQ] = fourK.U16Data[0] ;
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_BLGQ+1] = fourK.U16Data[1] ;
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_BLYW] = fourK.U16Data[0] ;
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_BLYW+1] = fourK.U16Data[1] ;																
					
					Slave_ReadFlashZOData(&Slave_GetData()->GQTem,FLASHIS_AdrWHJZ_WDJZ_LDGQ,SLAVE_BaseTem);
					Slave_ReadFlashZOData(&Slave_GetData()->YWTem,FLASHIS_AdrWHJZ_WDJZ_LDYW,SLAVE_BaseTem);							
					Slave_ReadFlashBLData(&Slave_GetData()->GQTem,FLASHIS_AdrWHJZ_WDJZ_BLGQ); //干球倍率 20200526
					Slave_ReadFlashBLData(&Slave_GetData()->YWTem,FLASHIS_AdrWHJZ_WDJZ_BLYW);//烟温倍率  //2020-05-30, modify by alfred.
				}
				else if(MenuPos == 2)
				{
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_LDSQ] = fourB.U16Data[0] ; //湿球零点
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_LDSQ+1] = fourB.U16Data[1] ; 

					pf->Data[FLASHIS_AdrWHJZ_WDJZ_BLSQ] = fourK.U16Data[0] ;	//湿球倍率
					pf->Data[FLASHIS_AdrWHJZ_WDJZ_BLSQ+1] = fourK.U16Data[1] ;
					
					Slave_ReadFlashZOData(&Slave_GetData()->SQTem,FLASHIS_AdrWHJZ_WDJZ_LDSQ,SLAVE_BaseTem);
					Slave_ReadFlashBLData(&Slave_GetData()->SQTem,FLASHIS_AdrWHJZ_WDJZ_BLSQ);//湿球倍率					
				}				
				*pWarning = 0xAF; //标定完成				
			}
			else
			{
				*pWarning = 0xAB;	//174欧电阻输入错误
			}
			break;
		}
		default: break;
	}
	return *pWarning;	
}



/* 	date: 2020.06.09
	函数名：void ProcessGetMinuteData(void)
	描述：	该函数用于处理获取分钟数据并保存到SD卡
	输入：	无
	影响：	屏幕结构体，flash结构体。
	修改记录： 1. 2020.06.09, 新增, Add by alfred.
*/
void ProcessGetMinuteData(void)
{
	Screen_DataStructure *p = Screen_GetData();
	Slave_DataStructure*  Sp=Slave_GetData();
	Flashisp_DataStructure* pf=Flashisp_GetData();
	
	float CoSo2;

	if(p->JiPingJun.bTime1S)  // 每秒处理的事情
	{
		p->JiPingJun.bTime1S = false;
		
		//以下是烟气启动时刻(启动,计平均,非暂停
		if(p->JiPingJun.cIsCalcAverage==1)
		{
		     p->JiPingJun.uCounter1S++;
		  
		     if((p->JiPingJun.uCounter1S%5)==0)   //5秒取1次数累加
		     {
			  
			   //先修正SO2
			    CoSo2=Sp->SO2Concent.Value;
			    if((pf->Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ]&(0x01))==1)  //
				{
				CoSo2=CalculationFormula_GetData()->SO2FromCO;//修正后
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
		  
		  if( p->JiPingJun.uCounter1S%(p->AgvTimer*60)==0)	//计时到分钟时间
		  {
				gMinuteData[p->JiPingJun.cAverageCount][0] = p->JiPingJun.JpjSum[0]/p->JiPingJun.JpjSuncnt;			
				gMinuteData[p->JiPingJun.cAverageCount][1] = p->JiPingJun.JpjSum[1]/p->JiPingJun.JpjSuncnt;	
				gMinuteData[p->JiPingJun.cAverageCount][2] = p->JiPingJun.JpjSum[2]/p->JiPingJun.JpjSuncnt;	
				gMinuteData[p->JiPingJun.cAverageCount][3] = p->JiPingJun.JpjSum[3]/p->JiPingJun.JpjSuncnt;	
				gMinuteData[p->JiPingJun.cAverageCount][4] = p->JiPingJun.JpjSum[4]/p->JiPingJun.JpjSuncnt;	
				gMinuteData[p->JiPingJun.cAverageCount][5] = p->JiPingJun.JpjSum[5]/p->JiPingJun.JpjSuncnt;	
				gMinuteData[p->JiPingJun.cAverageCount][6] = p->JiPingJun.JpjSum[6]/p->JiPingJun.JpjSuncnt;	
				gMinuteData[p->JiPingJun.cAverageCount][7] = p->JiPingJun.JpjSum[7]/p->JiPingJun.JpjSuncnt;	
			    ScreenJPG_ClearSum(); //请空
	
								
				//限制数值最小值为零
				for(u8 i=0;i<8;i++)
				{
					if(gMinuteData[p->JiPingJun.cAverageCount][i] <0) gMinuteData[p->JiPingJun.cAverageCount][i] = 0.0f	;					
				}

				/*
				sprintf((char*)uTimeBuffer,"%02d:%02d:%02d",p->Clock.Clock.tm_hour,p->Clock.Clock.tm_min,p->Clock.Clock.tm_sec);
				uTimeBuffer[8]=0;	//结束位置补0；
				strcpy((char*)gSystemTime[p->JiPingJun.cAverageCount],(char*)uTimeBuffer); 
                */
				strncpy((char*)gSystemTime[p->JiPingJun.cAverageCount],(char*)&p->recorddat[11],5); //把开始时间取出显示
				gSystemTime[p->JiPingJun.cAverageCount][5]=0;
				
				/*保存数据到SD卡*/
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
				SdFile_Serch_MaxofGasRecord(Flux_GetData()->ChannelC.SaveFileNumber); //读取最大的记录数
				SaveGasData(buffer,Flux_GetData()->ChannelC.SaveFileNumber,1);
				SdFile_WriteGasFile(&SdFile_GetData()->InterFace_data,Flux_GetData()->ChannelC.SaveFileNumber,1,0);
 
                //重新计时，把开始时间重新记录下来
                sprintf((char*) p->recorddat,"%02d-%02d-%02d %02d:%02d:%02d",p->Clock.Clock.tm_year+SCREEN_YearBase,p->Clock.Clock.tm_mon,p->Clock.Clock.tm_mday,p->Clock.Clock.tm_hour,p->Clock.Clock.tm_min,p->Clock.Clock.tm_sec);
	            p->recorddat[16]=0;   //结尾先加个零
				u8 buf[5];
				sprintf((char *)&buf[0],"%02d",pf->Data[FLASHIS_AdrXTSZYQCLSJ]);  //
	            strcat((char *)&p->recorddat[0],(char *)&buf[0]);
				strcat((char *)&p->recorddat[0],"9");

				p->JiPingJun.cAverageCount++;	//平均次数加1
				if(p->JiPingJun.cAverageCount == pf->Data[FLASHIS_AdrXTSZYQCLCS])  //计平均次数到最大；
				{
					//to do 结束逻辑
					p->RunMode=0x5210;//提示是否结束
					p->JiPingJun.cIsCalcAverage = 0; //计时到，计平均标志位清零；
					p->JiPingJun.cAverageCount = 0;
					p->Function=0;
					p->JiPingJun.ucCurrentPage = 0xff; //初始化计平均当前页的值。
					
					ScreenChangeDataInit(p);	//页面切换数据初始化
				}
				p->RunModeStat=1;
				

			}
		  
			
	        if((p->JiPingJun.uCounter1S%(p->AgvTimer*60)>=(p->AgvTimer*60 - 10))&&	//到达分钟采样点前10秒蜂鸣器提示
			   (p->JiPingJun.uCounter1S%(p->AgvTimer*60)<=(p->AgvTimer*60)))
			      {
				     if(p->JiPingJun.uCounter1S%2==0)
				     Speak_Buzzer(500);
			      }
	
	 }
	}	
	
}

