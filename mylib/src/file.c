#include "stm32f4xx.h"
#include "sys.h"  
#include "file.h"
#include "ff.h"
#include "exfuns.h"
#include "string.h"
#include "usart.h"
#include "malloc.h" 
//#include "delay.h"
// SD ���ϱ���ĸ�·��,������
//u32 recont_cnt;

const u8 ROOT_DIR[]="0:YLB3330";            //��Ŀ¼
//const u8 ROOT_DIR[]="0:YLB";            //��Ŀ¼

const u8 FOLDER_OF_Dust[]="0:/YLB3330/Dust";   //�̳��ļ���
const u8 FOLDER_OF_Dust00[]="0:/YLB3330/Dust/Dust00";   //�̳������ļ��ļ���
const u8 LINK_OF_Dust[]="0:/YLB3330/Dust/link.txt"; //�̳������ļ�

const u8 FOLDER_OF_Gas[]="0:/YLB3330/Gas";   //�����ļ���
const u8 FOLDER_OF_Gas00[]="0:/YLB3330/Gas/Gas00";   //���������ļ��ļ���
const u8 LINK_OF_Gas[]="0:/YLB3330/Gas/link.txt"; //���������ļ�
const u8 TEMP_OF_Gas[]="0:/YLB3330/Gas/temp0.txt"; //���������ļ�

const u8 FOLDER_OF_Work[]="0:/YLB3330/Work";   //�����ļ���
const u8 FOLDER_OF_Work00[]="0:/YLB3330/Work/Work00";   //���������ļ��ļ���
const u8 LINK_OF_Work[]="0:/YLB3330/Work/link.txt"; //���������ļ�
/*@lian add 20290302 star*/
const u8 FOLDER_OF_Flue[]="0:/YLB3330/Flue";   //�̵��ļ���
const u8 FOLDER_OF_Flue00[]="0:/YLB3330/Flue/Flue00";   //�̵������ļ��ļ���
const u8 LINK_OF_Flue[]="0:/YLB3330/Flue/link.txt"; //�̵������ļ�
/*@lian End*/
//�����ǹ̶����ļ�
//�����ļ�����
const char Work_line[Work_car_length][Other_length+1]={
"ƽ����ѹ:              Pa   \r\n",//0 
"ƽ����ѹ:              kPa  \r\n",//1 
"ƽ��ȫѹ:              kPa  \r\n",//2 
"�� �� ѹ:              kPa  \r\n",//3 
"�̵�����:              m2   \r\n",//4 
"�� �� ��:              mm   \r\n",//5 
"ƽ������:              ��   \r\n",//6 
"ƽ������:              m/s  \r\n",//7 
"��������:              m3/h \r\n",//8 
"�������:              m3/h \r\n",//9 
"�� ʪ ��:                   \r\n",//10 
"����ϵ��:                   \r\n",//11 
"����ϵ��:                   \r\n",//12 
"Ƥ �� ��:                   \r\n"//13
};

//�̳��ļ�����
const char Dust_line[Dust_car_length][Other_length+1]={
"ƽ����ѹ:              Pa   \r\n",//0
"ƽ����ѹ:              kPa  \r\n",//1
"ƽ��ȫѹ:              kPa  \r\n",//2
"ƽ������:              m/s  \r\n",//3
"ƽ������:              ��   \r\n",//4
"�� �� ѹ:              kPa  \r\n",//5
"�̵�����:              m2   \r\n",//6
"�� ʪ ��:                   \r\n",//7
"Ƥ��ϵ��:                   \r\n",//8
"����ϵ��:                   \r\n",//9
"����ϵ��:                   \r\n",//10
"��������:              m3/h \r\n",//11
"�������:              m3/h \r\n",//12
"�� �� ��:                   \r\n",//13
"����ֱ��:              mm   \r\n",//14
"ƽ����ѹ:              Kpa  \r\n",//15
"ƽ������:              ��   \r\n",//16
"�������:              L    \r\n",//17
"������:              L    \r\n",//18
"�̳�����:              g    \r\n",//19
"�� �� ��:              %    \r\n",//20
"��������:              mg/m3\r\n",//21
"һ������:              mg/m3\r\n",//22
"��������:              mg/m3\r\n",//23
"һ����̼:              mg/m3\r\n",//24
"�� �� ��:              mg/m3\r\n",//25
"������̼:              %    \r\n",//26
"�˿�����:                   \r\n",//27  ��16����ʼ��¼
"��Ʒ���:                   \r\n",//28
"�ۼƲ�ʱ:              s    \r\n",//29
"�� �� ��:                   \r\n",//30
"��������:                   \r\n",//31
"��ҺŨ��:                   \r\n",//32
"�������:                   \r\n",//33
"��ͷ��Ŀ:                   \r\n", //34
"��׼����:              %    \r\n", //35
"��ǰ�¶�:              ��   \r\n", //35
"��ǰѹ��:              kPa  \r\n", //37
"��ʣϵ��:                   \r\n", //38
};

//�����ļ�����
const char Gas_line[Gas_car_length][Other_length+1]={
"ƽ����ѹ:              Pa   \r\n",   //0
"ƽ����ѹ:              kPa  \r\n",   //1
"ƽ��ȫѹ:              kPa  \r\n",   //2
"ƽ������:              m/s  \r\n",   //3
"ƽ������:              ��   \r\n",   //4
"�� �� ѹ:              kPa  \r\n",   //5
"�̵�����:              m2   \r\n",   //6
"�� ʪ ��:                   \r\n",   //7
"Ƥ��ϵ��:                   \r\n",   //8
"����ϵ��:                   \r\n",   //9
"����ϵ��:                   \r\n",   //10
"��������:              m3/h \r\n",   //11
"�������:              m3/h \r\n",   //12
"�� �� ��:              %    \r\n",   //13
"��������:              mg/m3\r\n",   //14
"һ������:              mg/m3\r\n",   //15
"��������:              mg/m3\r\n",   //16
"һ����̼:              mg/m3\r\n",   //17
"�� �� ��:              mg/m3\r\n",   //18
"������̼:              %    \r\n",   //19
"�˿�����:                   \r\n",   //20  ��16����ʼ��¼
"����ʱ��:                   \r\n",   //21
"��ǰ�¶�:              ��   \r\n",   //22
"��ʣϵ��:                   \r\n",   //23
"��׼����:              %    \r\n",   //24
};

const char Gas_line_record[GasRecord_length+1]={"                   ,          %        ,          mg/m3    ,          mg/m3    ,          mg/m3    ,          mg/m3    ,          mg/m3    ,          %        ,          %        ,          %       \r\n"};

/*@lian add 20290302 star*/
//�̵��ļ�����20200527
const char Flue_line[Flue_car_length][Other_length+1]={
"�̵����:              m2   \r\n",   //0
"�̵�����:                   \r\n",   //1
"������:                   \r\n",   //2
"�������:                   \r\n",   //3
"���  01:              m    \r\n",   //4
"���  02:              m    \r\n",   //5
"���  03:              m    \r\n",   //6
"���  04:              m    \r\n",   //7
"���  05:              m    \r\n",   //9
"���  06:              m    \r\n",   //10
"���  07:              m    \r\n",   //11
"���  08:              m    \r\n",   //12
"���  09:              m    \r\n",   //13
"���  10:              m    \r\n",   //14
"�ھ�D   :              m    \r\n",   //15
"�׹�L   :              m    \r\n",   //16
"����    :                   \r\n",   //17
"��A     :              m    \r\n",   //18
"��B     :              m    \r\n",   //19
"���ײ��:                   \r\n",   //20
};

const u8 Flue_YXSY[5]={15,0,16,17,2};    //Բ������:�ھ�D ,������׹�L ,����,����
const u8 Flue_JXSY[6]={18,19,0,16,2,20}; //��������:��A ,��B,���,�׹�L ,����,������
const u8 Flue_QTSY[2]={0,3};              //��������

/*@lian End*/
SdFile_DataStruct  SdFile_Data;




/*
	��������SdFile_DataStruct* SdFile_GetData(void)
	��������ȡ����File.c�ļ�ȫ�ֱ����ṹ��
*/
SdFile_DataStruct* SdFile_GetData(void)
{
	return &SdFile_Data;
}

/*
	��������SdFile_Timer(void)
	������SD����Ķ�ʱ��
*/
void SdFile_Timer(void)
{
	if(SdFile_Data.Timer!=0)
	SdFile_Data.Timer--;
}


/*******************************************************************************
	��������makelinkfile_record
	��  ��: NumFile���ļ���
	        ��д����������ȡ������
	��  ��: &SdFile_Data.file_link_buf.stru.rowdata[0]
	����˵���������ļ��ţ�����������¼

	*/
void SdFile_makelinkfile_record(u32 NumFile)
{	
	 u8 i,j;
	 char pathbuf[202],charNumFile[12];   //�������ļ�·��

	 //�����������¼
	   //��ʽ��������
	 for(i=0;i<10;i++)
	 { 
		 for(j=0;j<9;j++)
		 SdFile_Data.file_link_buf.stru.rowdata[i][j]=' ';
		 SdFile_Data.file_link_buf.stru.rowdata[i][9]=',';
	 }
	 SdFile_Data.file_link_buf.stru.rowdata[9][8]='\r';
	 SdFile_Data.file_link_buf.stru.rowdata[9][9]='\n';

	 //��֯����
	   //�ļ���
	 SPutl(0x6,(u8 *)&charNumFile[0],NumFile); //���ļ���ת����6Ϊ���ַ���
	 charNumFile[6]=0;
	 strcpy((char *)&pathbuf[0],"   ");
	 strcat((char *)&pathbuf[0],(char *)&charNumFile[0]);
	 strncpy((char *)&SdFile_Data.file_link_buf.stru.rowdata[row1_Filenumber][0],(char *)&pathbuf[0],9);
	   //������ ,���������ɻ�ȡʱ��
	 strcpy((char *)&pathbuf[0]," ");
	 strncpy((char *)&charNumFile[0],(char *)&SdFile_Data.InterFace_data.dat[0],4); //��
	 charNumFile[4]=0;
	 strcat((char *)&pathbuf[0],(char *)&charNumFile[0]);   
	 strncpy((char *)&charNumFile[0],(char *)&SdFile_Data.InterFace_data.dat[5],2); //��
	 charNumFile[2]=0;
	 strcat((char *)&pathbuf[0],(char *)&charNumFile[0]);                           //
	 strncpy((char *)&charNumFile[0],(char *)&SdFile_Data.InterFace_data.dat[8],2); //��
	 charNumFile[2]=0;
	 strcat((char *)&pathbuf[0],(char *)&charNumFile[0]);                           //
	 strncpy((char *)&SdFile_Data.file_link_buf.stru.rowdata[row2_YMD][0],(char *)&pathbuf[0],9);
   //��ʱ��
	 strcpy((char *)&pathbuf[0],"   ");
	 strncpy((char *)&charNumFile[0],(char *)&SdFile_Data.InterFace_data.dat[11],2);  //��
	 charNumFile[2]=0;
	 strcat((char *)&pathbuf[0],(char *)&charNumFile[0]);   
	 strncpy((char *)&charNumFile[0],(char *)&SdFile_Data.InterFace_data.dat[14],2); //ʱ
	 charNumFile[2]=0;
	 strcat((char *)&pathbuf[0],(char *)&charNumFile[0]);                           //
	 strncpy((char *)&charNumFile[0],(char *)&SdFile_Data.InterFace_data.dat[17],2); //��
	 charNumFile[2]=0;
	 strcat((char *)&pathbuf[0],(char *)&charNumFile[0]);                           //
	 strncpy((char *)&SdFile_Data.file_link_buf.stru.rowdata[row3_DHM][0],(char *)&pathbuf[0],9);
	 
}
/*******************************************************************************
	��������FileNumberToDir(u32 NumFile)
	��  ��:  NumFile:�ļ����
           path:�����ļ���Ŀ¼
	��  ��: 
	����˵������ȡ��������
	*/
void SdFile_NumberToDir(char *path,char *pathbuf,u32 NumFile)
{
	 char charNumFile[12];
	              
	 strcpy(pathbuf,&path[0]);                    //�Ȱ�·��copy��������
	 SPutl(0x6,(u8 *)&charNumFile[0],NumFile); //���ļ���ת����6λ���ַ���
	 charNumFile[6]=0;
	 strcat(pathbuf,(char *)"/");
	 strcat(pathbuf,&charNumFile[0]);    //
	 strcat(pathbuf,".txt");             //�����ļ�·��
	 strcpy(pathbuf,&pathbuf[0]);
}

/**************
 ��������clr_chaxun(void)
 ����˵������ջ�����
***************/
void clr_file_chaxun(void)
{
	
}


/*******************************************************************************
	��������u8 ReadLinkRecord(void)
	��  ��:  path �ļ�·��
           Filenumber ,�ļ���(>=1,�������ļ��Ǵ��㿪ʼ) 
	��  ��: SdFile_Data.file_link_buf.arr[0]
	����˵���������ļ����Ѷ�Ӧʱ��
	*/
u8 SdFile_ReadLinkRecord(char *path,u32 Filenumber)
{    u8 res;
	   u32 Ptr;
	 //  strcpy(SdFile_Data.GENG_FILEPATH,(TCHAR *)&path[0]); //�� ·������GENG_FILEPATH,�����ļ�
	   //�ȼ����¼�Ķ�λָ��
	   Ptr=(Filenumber-1)*LinkFile_Record_Length; //���������ļ���¼��ͷ���㷨�������ļ���������ķ���
	
	   res=SdFile_read_record(path,(char *)&SdFile_Data.file_link_buf.arr[0],Ptr,LinkFile_Record_Length); //��Ptrλ�õļ�¼������ 
		
	// res=read_n_record(SdFile_Data.GENG_FILEPATH,(char *)&SdFile_Data.file_link.arr[0],Ptr,LinkFile_Record_Length); //�ѵ�NumFile����¼������ 
	   return res;
}

/*******************************************************************************
	��������u8 ReadLinkRecord(void)
	��  ��:  path �ļ�·��
           Filenumber ,�ļ���(>=1,�������ļ��Ǵ��㿪ʼ) 
           SdFile_Data.file_link_buf.arr
	��  ��: SdFile_Data.file_link_buf.arr[0]
	����˵���������ļ����Ѷ�Ӧʱ��
	*/
u8 SdFile_WriteLinkRecord(char *path,u32 Filenumber)
{    u8 res;
	   u32 Ptr;
	   char dbuf[LinkFile_Record_Length+1];
	 //  strcpy(SdFile_Data.GENG_FILEPATH,(TCHAR *)&path[0]); //�� ·������GENG_FILEPATH,�����ļ�
	   //�ȼ����¼�Ķ�λָ��
	   Ptr=(Filenumber-1)*LinkFile_Record_Length; //���������ļ���¼��ͷ���㷨�������ļ���������ķ���
	   strncpy((char *)&dbuf[0],(char *)&SdFile_Data.file_link_buf.arr[0],LinkFile_Record_Length);
	   dbuf[LinkFile_Record_Length]=0;
	   res=SdFile_write_record(path,(char *)&dbuf[0],Ptr,LinkFile_Record_Length); //��Ptrλ�õļ�¼������ 
		
	// res=read_n_record(SdFile_Data.GENG_FILEPATH,(char *)&SdFile_Data.file_link.arr[0],Ptr,LinkFile_Record_Length); //�ѵ�NumFile����¼������ 
	   return res;
}


/*******************************************************************************
	��������DeleteWorkFiles(void)
	��  ��:  
	��  ��: 
	����˵����ɾ�������ļ�
	�޸ļ�¼��1. ���ӷ���ɾ���ļ�״̬, modify by alfred, 2020.07.13
	*/
FRESULT SdFile_DeleteWorkFiles(void)
{    FRESULT res;
	   char buffloder[100];
	   strcpy((char *)&buffloder[0],(TCHAR *)&FOLDER_OF_Work[0]);
	   res=FS_DeleteFolderOrFile((char *)&buffloder[0]);
     if(res==FR_OK)
		 {  printf("ɾ���ļ��гɹ�");
			  //���ؽ��ļ����������ļ�
			res=f_mkdir((TCHAR *)&FOLDER_OF_Work[0]);
		    if(res==FR_OK)
		    printf("�����ļ��У�YLB3330/Work �ɹ�\r\n"); 
				
			res=f_mkdir((TCHAR *)&FOLDER_OF_Work00[0]);
		    if(res==FR_OK)
		    printf("�����ļ��У�YLB3330/Work/Work00 �ɹ�\r\n"); 
				
			res=f_open(file_sd,(TCHAR *)&LINK_OF_Work[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //�������������ļ�
			if(res==FR_OK)
			{
				printf("����Work-link.txt�ļ��ɹ�\r\n"); 
				res = f_lseek(file_sd,0);
				if(res==FR_OK)
				{
					printf("��λWork-link.txt�ļ���ͷ�ɹ�\r\n"); 
				}				 
				f_close(file_sd);
			}			
			else
				printf("����Work-link.txt�ļ�ʧ��\r\n"); 
		 }
		 else
		 {
			 printf("ɾ���ļ��д���");
		 }
		 return res;
}

/*******************************************************************************
	��������DeleteGasFiles(void)
	��  ��:  
	��  ��: 
	����˵����ɾ�������ļ�
	�޸ļ�¼��1. ���ӷ���ɾ���ļ�״̬, modify by alfred, 2020.07.13
	*/
FRESULT SdFile_DeleteGasFiles(void)
{     FRESULT res;
	    char buffloder[100];
	   strcpy((char *)&buffloder[0],(TCHAR *)&FOLDER_OF_Gas[0]);
	
	   res=FS_DeleteFolderOrFile((char *)&buffloder[0]);
     if(res==FR_OK)
		 {  printf("ɾ���ļ��гɹ�");
			  //���ؽ��ļ����������ļ�
			  res=f_mkdir((TCHAR *)&FOLDER_OF_Gas[0]);
		    if(res==FR_OK)
		    printf("�����ļ��У�YLB3330/Gas �ɹ�\r\n"); 
				
				res=f_mkdir((TCHAR *)&FOLDER_OF_Gas00[0]);
		    if(res==FR_OK)
		    printf("�����ļ��У�YLB3330/Gas/Gas00 �ɹ�\r\n"); 
				
				res=f_open(file_sd,(TCHAR *)&LINK_OF_Gas[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //�������������ļ�
			if(res==FR_OK)
			{
		   printf("����Gas-link.txt�ļ��ɹ�\r\n"); 
			 res = f_lseek(file_sd,0);
       if(res==FR_OK)
 			 {
				  printf("��λGas-link.txt�ļ���ͷ�ɹ�\r\n"); 
			 }				 
				f_close(file_sd);
			}			
      else
			 printf("����Gas-link.txt�ļ�ʧ��\r\n"); 
		 }
		 else
		 {
			 printf("ɾ���ļ��д���");
		 }
		 return res;
}

/*******************************************************************************
	��������DeleteDustFiles(void)
	��  ��:  
	��  ��: 
	����˵����ɾ���̳��ļ�
	�޸ļ�¼��1. ���ӷ���ɾ���ļ�״̬, modify by alfred, 2020.07.13
	*/
FRESULT SdFile_DeleteDustFiles(void)
{    FRESULT res;
      char buffloder[100];
	   strcpy((char *)&buffloder[0],(TCHAR *)&FOLDER_OF_Dust[0]);
	
	  	res=FS_DeleteFolderOrFile((char *)&buffloder[0]);
     if(res==FR_OK)
		 {  printf("ɾ���ļ��гɹ�");
			  //���ؽ��ļ����������ļ�
			  res=f_mkdir((TCHAR *)&FOLDER_OF_Dust[0]);
		    if(res==FR_OK)
		    printf("�����ļ��У�YLB3330/Dust �ɹ�\r\n"); 
				
				res=f_mkdir((TCHAR *)&FOLDER_OF_Dust00[0]);
		    if(res==FR_OK)
		    printf("�����ļ��У�YLB3330/Dust/Dust00 �ɹ�\r\n"); 
				
				res=f_open(file_sd,(TCHAR *)&LINK_OF_Dust[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //�������������ļ�
			if(res==FR_OK)
			{
		   printf("����Dust-link.txt�ļ��ɹ�\r\n"); 
			 res = f_lseek(file_sd,0);
       if(res==FR_OK)
 			 {
				  printf("��λDust-link.txt�ļ���ͷ�ɹ�\r\n"); 
			 }				 
				f_close(file_sd);
			}			
      else
			 printf("����Dust-link.txt�ļ�ʧ��\r\n"); 
		 }
		 else
		 {
			 printf("ɾ���ļ��д���");
		 }
		 return res;
}
/*******************************************************************************
	��������SdFile_DeleteFlueFiles(void)
	��  ��:  
	��  ��: 
	����˵����ɾ�������ļ�
	�޸ļ�¼��1. ���ӷ���ɾ���ļ�״̬, modify by alfred, 2020.07.13
	*/
FRESULT SdFile_DeleteFlueFiles(void)  //ɾ�������ļ�
{
	 FRESULT res;
      char buffloder[100];
	   strcpy((char *)&buffloder[0],(TCHAR *)&FOLDER_OF_Flue[0]);
	
	  	res=FS_DeleteFolderOrFile((char *)&buffloder[0]);
     if(res==FR_OK)
		 {  printf("ɾ���ļ��гɹ�");
			  //���ؽ��ļ����������ļ�
			  res=f_mkdir((TCHAR *)&FOLDER_OF_Flue[0]);
		    if(res==FR_OK)
		    printf("�����ļ��У�YLB3330/flue �ɹ�\r\n"); 
				
				res=f_mkdir((TCHAR *)&FOLDER_OF_Flue00[0]);
		    if(res==FR_OK)
		    printf("�����ļ��У�YLB3330/Flue/Flue00 �ɹ�\r\n"); 
				
				res=f_open(file_sd,(TCHAR *)&LINK_OF_Flue[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //�������������ļ�
			if(res==FR_OK)
			{
		   printf("����Flue-link.txt�ļ��ɹ�\r\n"); 
			 res = f_lseek(file_sd,0);
       if(res==FR_OK)
 			 {
				  printf("��λFlue-link.txt�ļ���ͷ�ɹ�\r\n"); 
			 }				 
				f_close(file_sd);
			}			
      else
			 printf("����Flue-link.txt�ļ�ʧ��\r\n"); 
		 }
		 else
		 {
			 printf("ɾ���ļ��д���");
		 }
		 return res;
	
}

//ɾ���ļ�
u8  DeleteFile(TCHAR* FileName)
{
	u8 res;
	res=f_unlink(FileName);
	return res;
}

//====================================================================================================  
//�� �� �� : f_deldir  
//�������� : �Ƴ�һ���ļ��У������䱾��������ļ��У����ļ�  
//��    �� : const TCHAR *path---ָ��Ҫ�Ƴ��Ŀս�β�ַ��������ָ��  
//��    �� : ��  
//�� �� ֵ : FR_OK(0)��           �����ɹ�   
//           FR_NO_FILE��         �޷��ҵ��ļ���Ŀ¼   
//           FR_NO_PATH��         �޷��ҵ�·��   
//           FR_INVALID_NAME��    �ļ����Ƿ�   
//           FR_INVALID_DRIVE��   �������ŷǷ�   
//           FR_DENIED��          ������������ԭ�򱻾ܾ���   
//               ��������Ϊֻ����  
//               Ŀ¼�·ǿգ�  
//               ��ǰĿ¼��  
//           FR_NOT_READY��       �����������޷�������������������û��ý�������ԭ��   
//           FR_WRITE_PROTECTED�� ý��д����   
//           FR_DISK_ERR��        ����ʧ�����ڴ������е�һ������   
//           FR_INT_ERR��         ����ʧ������һ������� FAT �ṹ���ڲ�����   
//           FR_NOT_ENABLED��     �߼�������û�й�����   
//           FR_NO_FILESYSTEM��   ��������û�кϷ��� FAT ��   
//           FR_LOCKED��          �������������ļ�������ƣ�_FS_SHARE��   
//��    ע : f_deldir ���������Ƴ�һ���ļ��м������ļ��С����ļ����������Ƴ��Ѿ��򿪵Ķ���   
//====================================================================================================  
FRESULT f_deldir(const TCHAR *path_sure)  
{  
    FRESULT res;  
    DIR   dir;     /* �ļ��ж��� */ //36  bytes  
    FILINFO fno;   /* �ļ����� */   //32  bytes  
    TCHAR lfile[_MAX_LFN + 2] = {0};  
#if _USE_LFN  
    TCHAR lname[_MAX_LFN + 2] = {0};  
#endif  
      
#if _USE_LFN  
    fno.lfsize = _MAX_LFN;  
    fno.lfname = lname;    //���븳��ֵ  
#endif  
    //���ļ���  
		
    res = f_opendir(&dir, path_sure);  
    //������ȡ�ļ�������  
   while((res == FR_OK) && (FR_OK == f_readdir(&dir, &fno)))  
    { 
        //����"."��".."�ļ��У�����  
        if(0 == strlen(fno.fname))          break;      //���������ļ���Ϊ��  
        if(0 == strcmp(fno.fname, "."))     continue;   //���������ļ���Ϊ��ǰ�ļ���  
        if(0 == strcmp(fno.fname, ".."))    continue;   //���������ļ���Ϊ��һ���ļ���  
          
        memset(lfile, 0, sizeof(lfile));  
#if _USE_LFN  
        sprintf((char*)lfile, "%s/%s",path_sure, (*fno.lfname) ? fno.lfname : fno.fname);  
#else  
        sprintf((char*)lfile, "%s/%s", path, fno.fname);  
#endif  
        if (fno.fattrib & AM_DIR)  
        {//�����ļ��У��ݹ�ɾ��  
            res = f_deldir(lfile); 
        }  
        else  
        {//�����ļ���ֱ��ɾ��  
            res = f_unlink((char*)lfile); 
				
					printf("\r\n%s",lfile);	
        }  
    }  
      
    //ɾ������  
    if(res == FR_OK)    res = f_unlink(path_sure);  
 
    return res;  
} 



/**************************************************
**************************************************/

FRESULT DeleteAllFiles(void)
{ FRESULT Res;
	char buffloder[100];
	strcpy((char *)&buffloder[0],(char *)&ROOT_DIR[0]);
	Res=FS_DeleteFolderOrFile((char *)&buffloder[0]);
//	Res=f_unlink((char *)"0:/YLB3330/Dust/Dust00/000001.txt");
//	Res=f_unlink((char *)"0:/YLB3330/Dust/Dust00/000002.txt");
//	Res=f_unlink((char *)"0:/YLB3330/Dust/Dust00/000003.txt");
 // Res=f_deldir((char *)&ROOT_DIR[0]);
	printf("\r\nɾ����Ŀ¼");
	if(Res== FR_OK)
	{
		printf("\r\nɾ����Ŀ¼�ɹ�");
	}
	else
	{
		printf("\r\nɾ����Ŀ¼ʧ��");
	}
	return(Res);
}

/*******************************************************************************
	��������SdFile_Init(void)
	��  ��: ΪSD��������������
          
           
	��  ��: 
	����˵���������ļ����Ѷ�Ӧʱ��
	*/

void SdFile_Init(void)
{ u8 i;

	
	my_mem_init(SRAMIN);		              //��ʼ���ڲ��ڴ��
	
	printf("sd test star....\r\n");

	for(i=0;i<=10;i++)
	{
		if(SD_Initialize())
			break;
		delay_ms(10);
	}
	
	delay_ms(500);
 
  exfuns_init();							//ΪFATFS ��ر��������ڴ�	

  f_mount(fs[0],"0:",1);       //����SD��
	//f_mount(fs[1],"1:",1);       //
	f_mount(fs[2],"2:",1); 

//����Ϊ�����ļ�����
	if(SD_GetData()->flag)   //���������SD
	{
		
 	SdFile_init_SD_dir();            //ΪSD����Ŀ¼,Ŀ¼���ھ��˳�
	
	}
	
	
}

void test_file_write(void)
{

}
/*******************************************************************************
	��������serch_c(char *path,u16 NumFile)
	��  ��: path �ļ�·��
          NumFile �ļ���
	��  ��:
	����˵�������ļ����ѳ��ļ��ż�¼
*/



/****************************************************************************
	��������SdFile_read_record(char *path,char * buff,u16 NumFile,u16 length)
��  ��: path �ļ�·��ָ��,buff:��¼�洢λ��ָ��,adrr:��ʼ��ַ
        length ��¼�ĳ���
	��  ��:
	����˵�������ļ����ѳ��ļ��ż�¼
*/
u8 SdFile_read_record(char *path,char * buff,u32 adrr,u32 length)
{  FRESULT res;
	 u8 bak;
//	 u32 i,j;

	 unsigned char read_buffer[250];                  //
	 res=f_open(file_sd,(TCHAR*)path,FA_READ);//�ȴ��ļ�
	 if (res==FR_OK)  
	 {
		 printf("\r\n open file OK\r\n");
		 res = f_lseek(file_sd,adrr);               //��λ�ļ�ͷ
     if(res == FR_OK)
		 {
			 f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
			 if(file_sd->fptr-adrr==length)                            //��ȷ��ȡһ����¼
			 {
				 strncpy(buff,(char *)&read_buffer[0],length);
				 bak=0;                                                 //��ȡ��ȷ
			 }
			 else
			 {
				 printf("read data error!\r\n");
			   bak=3;                                                //��ȡ����
  		 }
     }
	   else
		 {
      printf("f_lseek error!2\r\n");
			bak=2;                                                   //��λ��¼����
      
     }

	   f_close(file_sd);         //�ر��ļ�
		 return bak;             //��λ�ļ�����
	 }
	 else
	 { f_close(file_sd);         //�ر��ļ�
		  printf("\r\n open file ERROR\r\n");
		  bak=1;
		  return bak;              //���ļ�����
	 }

}


/****************************************************************************
	��������SdFile_write_record(char *path,u16 NumFile)
	��  ��: path �ļ�·��
          NumFile �ļ���
	��  ��:
	����˵�������ļ�β׷�Ӽ�¼
*/
u8 SdFile_write_record(char *path,char * buff,u32 adrr,u32 length)
{ 
	FRESULT res;
	u8 bak;
	
	 res=f_open(file_sd,(TCHAR*)path,FA_READ | FA_WRITE | FA_OPEN_ALWAYS);//����ļ����ڣ��ʹ򿪣����򴴽�
	 if (res==FR_OK)
	 {
		 printf("\r\n open file OK\r\n");
		res = f_lseek(file_sd,adrr);  		 //��λ��ĳ��¼��ͷ
		if (res==FR_OK)
	  {
		 if(f_puts(buff,file_sd)!=EOF)                                     //��鵽�س����� 
		 {
		  printf("WRITE FILE successful!\r\n");
		  bak=0;
		 }
		 else
		 {
 		  printf("WRITE FILE REEOR!\r\n");
		  bak=3; 
		 }
	 }
  else
		{
      printf("f_lseek error!2\r\n");
			bak=2;                                                   //��λ��¼����
      
    }

		f_close(file_sd);         //�ر��ļ�
	//	res=f_open(file_sd,(TCHAR*)path,FA_READ | FA_WRITE | FA_OPEN_ALWAYS);//����ļ����ڣ��ʹ򿪣����򴴽�

	  SdFile_Data.Timer=10;
		while(SdFile_Data.Timer);  //��ʱ10ms
					
	//	f_close(file_sd);       //�ر��ļ�			
		return bak;             //��λ�ļ�����  
	 }
	 else
	 {
		  printf("\r\n open file ERROR\r\n");
		  bak=1;
		  f_close(file_sd);       //�ر��ļ�		
		  return bak;              //���ļ�����
	 }
}
/****************************************************************************
	��������init_SD_dir(void)
	��  ��:
          
	��  ��:
	����˵������ʼ����SD�����ļ�·��
*/
void SdFile_init_SD_dir(void)
{
	// FATFS fs; /*  */
 //  DIR dir; /*  */
 //  FILINFO fno; /**/
   FRESULT res; /* FatFs */
	 if(f_opendir(&dir_sd,(TCHAR *)&ROOT_DIR[0])==FR_OK) //�˳�
	 { 
		 f_closedir(&dir_sd);
		 printf("Ŀ¼����\r\n");
	 }
	 else //����Ϊ�µ�SD�������������ļ�Ŀ¼
	 {
		 res=f_mkdir((TCHAR *)&ROOT_DIR[0]);
		 if(res==FR_OK)
		 printf("������Ŀ¼YLB3330�ɹ�\r\n"); 
		 
     res=f_mkdir((TCHAR *)&FOLDER_OF_Dust[0]);
		 if(res==FR_OK)
		 printf("�����ļ��У�YLB3330/Dust �ɹ�\r\n"); 
			
		 res=f_mkdir((TCHAR *)&FOLDER_OF_Gas[0]);
		 if(res==FR_OK)
		 printf("�����ļ��У�YLB3330/Gas �ɹ�\r\n"); 
			
		 res=f_mkdir((TCHAR *)&FOLDER_OF_Work[0]);
		 if(res==FR_OK)
		 printf("�����ļ��У�YLB3330/Work �ɹ�\r\n"); 
		 
		 res=f_mkdir((TCHAR *)&FOLDER_OF_Flue[0]);
		 if(res==FR_OK)
		 printf("�����ļ��У�YLB3330/Flue �ɹ�\r\n");
		 
		 res=f_mkdir((TCHAR *)&FOLDER_OF_Dust00[0]);
		 if(res==FR_OK)
		 printf("�����ļ��У�YLB3330/Dust/Dust00 �ɹ�\r\n"); 
			
		 res=f_mkdir((TCHAR *)&FOLDER_OF_Gas00[0]);
		 if(res==FR_OK)
		 printf("�����ļ��У�YLB3330/Gas/Gas00 �ɹ�\r\n"); 
			
		 res=f_mkdir((TCHAR *)&FOLDER_OF_Work00[0]);
		 if(res==FR_OK)
		 printf("�����ļ��У�YLB3330/Work/Work00 �ɹ�\r\n"); 
     	 res=f_mkdir((TCHAR *)&FOLDER_OF_Flue00[0]);
		 if(res==FR_OK)
		 printf("�����ļ��У�YLB3330/Work/Flue00 �ɹ�\r\n"); 
     
			res=f_open(file_sd,(TCHAR *)&LINK_OF_Dust[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //�������������ļ�
			if(res==FR_OK)
			{
		   printf("����Dust-link.txt�ļ��ɹ�\r\n"); 
			 res = f_lseek(file_sd,0);
       if(res==FR_OK)
 			 {
				  printf("��λDust-link.txt�ļ���ͷ�ɹ�\r\n"); 
			 }				 
				f_close(file_sd);
			}			
      else
			 printf("����Dust-link.txt�ļ�ʧ��\r\n"); 
			
			res=f_open(file_sd,(TCHAR *)&LINK_OF_Gas[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //�������������ļ�
			if(res==FR_OK)
			{
		   printf("����Gas-link.txt�ɹ�\r\n"); 
			 res = f_lseek(file_sd,0);
       if(res==FR_OK)
 			 {
				  printf("��λGas-link.txt�ļ���ͷ�ɹ�\r\n"); 
			 }				 
				f_close(file_sd);
			}			
      else
			 printf("����Gas-link.txt�ļ�ʧ��\r\n"); 
			
			res=f_open(file_sd,(TCHAR *)&LINK_OF_Work[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //�������������ļ�
			if(res==FR_OK)
			{
		   printf("����Work-link.txt�ļ��ɹ�\r\n"); 
			 res = f_lseek(file_sd,0);
       if(res==FR_OK)
 			 {
				  printf("��λWork-link.txt�ļ���ͷ�ɹ�\r\n"); 
			 }				 
				f_close(file_sd);
			}			
      else
			 printf("����Work-link.txt�ļ�ʧ��\r\n"); 
			
	    	res=f_open(file_sd,(TCHAR *)&LINK_OF_Flue[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //�������������ļ�
			if(res==FR_OK)
			{
		   printf("����Flue-link.txt�ļ��ɹ�\r\n"); 
			 res = f_lseek(file_sd,0);
       if(res==FR_OK)
 			 {
				  printf("��λFlue-link.txt�ļ���ͷ�ɹ�\r\n"); 
			 }				 
				f_close(file_sd);
			}			
      else
			 printf("����Work-link.txt�ļ�ʧ��\r\n"); 
			
	 }
	
}

/****************************************************************************
	��������u8 Serch_MaxOfFile(char *path,u32 * ptr,u32 length)
��  ��:path:�ļ�·��
       ptr����������ļ���
       length����¼����+1
	��  ��:
	����˵�������������ļ�����ļ���
*/

u8 SdFile_Serch_MaxOfFile(char *path,u32 * ptr,u32 length)
{
  FRESULT res;
	 u8 bak;

	 u32 i;
	         
	 res=f_open(file_sd,(TCHAR*)path,FA_READ);//�ȴ��ļ�
	 if (res==FR_OK)  
	 {
		 printf("\r\n open file OK\r\n");
		 res = f_lseek(file_sd,0);       //��λ�ļ�ͷ
     if(res == FR_OK)
		 {
			printf("f_lseek successful!\r\n");
			if( file_sd->fsize==0)
			{* ptr=0;}
			else	
			{
				i= file_sd->fsize;   //�õ�ʣ�µ��ֽ���
				i=i/(length);
				* ptr=i; 
			}
			bak=0;
     }
	   else
		 {
			printf("f_lseek error!2\r\n");
			bak=2;                                                   //��λ0����
     }
	   f_close(file_sd);         //�ر��ļ�
	 }
	 else
	 {
		  printf("\r\n open file ERROR\r\n");
		  bak=1;
	 }
	 return bak;              //���ļ����� 
}
/****************************************************************************
	��������Serch_MaxofGasLinkFile()
	��  ��:
          
	��  ��:
	����˵�����������������ļ�����ļ���
*/

u8 SdFile_Serch_MaxofGasLinkFile(void)
{
	u8 res;
	res=SdFile_Serch_MaxOfFile((TCHAR *)&LINK_OF_Gas[0],&SdFile_Data.GasLinkFileMax,LinkFile_Record_Length);
	return res;
}
/****************************************************************************
	SdFile_Serch_MaxofFlueLinkFile()
	��  ��:
          
	��  ��:
	����˵���������̵������ļ�����ļ���
*/

u8 SdFile_Serch_MaxofFlueLinkFile(void)
{
	u8 res;
	res=SdFile_Serch_MaxOfFile((TCHAR *)&LINK_OF_Flue[0],&SdFile_Data.FlueLinkFileMax,LinkFile_Record_Length);
	return res;
}
/****************************************************************************
	��������Serch_MaxLinkFile(char *path)
	��  ��:
          
	��  ��:
	����˵���������̳������ļ�����ļ���
*/
u8 SdFile_Serch_MaxofDustLinkFile(void)
{
	u8 res;
	res=SdFile_Serch_MaxOfFile((TCHAR *)&LINK_OF_Dust[0],&SdFile_Data.DustLinkFileMax,LinkFile_Record_Length);
	return res;
}
/****************************************************************************
	��������Serch_MaxLinkFile(char *path)
	��  ��:
          
	��  ��:
	����˵�����������������ļ�����ļ���
*/
u8 SdFile_Serch_MaxofWorkLinkFile(void)
{
	u8 res;
	res=SdFile_Serch_MaxOfFile((TCHAR *)&LINK_OF_Work[0],&SdFile_Data.WorkLinkFileMax,LinkFile_Record_Length);
	return res;
}

/****************************************************************************
	��������Serch_MaxofGasRecord(u32 NumFile)
	��  ��:
          
	��  ��:
	����˵���������ڼ��������ļ�����¼��
*/

u8 SdFile_Serch_MaxofGasRecord(u32 NumFile)
{
	FRESULT res;
	u8 bak;
	char pathbuf[202];   //�������ļ�·��
	u32 Ptr;
	u32 i;
		 
	SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������
	res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ);  //�������ļ�

	if (res==FR_OK)  
	{
		printf("\r\n open file OK\r\n");
		Ptr=(Gas_car_length+1)*Other_length+Adress_length;
		res = f_lseek(file_sd,Ptr);       //����̨ͷ
		if(res == FR_OK)
		{
			printf("f_lseek successful!\r\n");
			if( file_sd->fsize>=Ptr)
			{
				i= file_sd->fsize-Ptr;   //�õ�ʣ�µ��ֽ���
				i=i/(GasRecord_length);
				SdFile_Data.GasRecordMax=i;	
			}
			else
			{
				SdFile_Data.GasRecordMax=0;
			}
			bak=0;
		}
		else
		{
			printf("f_lseek error!2\r\n");
			bak=2;                                                   //��λ0����
		}
		f_close(file_sd);         //�ر��ļ�
	}
	else
	{
	printf("\r\n open file ERROR\r\n");
	bak=1;
	}
	return bak;              //���ļ����� 
}



/****************************************************************************
	��������ReadWorkFile(u32 NumFile)
	��  ��: FileNumber �ļ����
          
	��  ��:  SdFile_Data.rdWorkFile

	����˵������ȡ�����ļ�
***************************************************************************/

u8 SdFile_ReadWorkFile(u32 NumFile)  //path �ļ���·��
{
	 FRESULT res;
	 u16 recordcnt;
	 DWORD ptr;
	 char pathbuf[50];   //�������ļ�·��
	 char read_buffer[250];      //
	 SdFile_NumberToDir((char *)&FOLDER_OF_Work00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ);//�ȴ��ļ�
	 if (res==FR_OK)  
	 {
		 printf("\r\n open file OK\r\n");
		 res = f_lseek(file_sd,0);               //��λ�ļ�ͷ
		 ptr=0;
     if(res == FR_OK)
		 {
			  printf("\r\n ��λ�ļ�ͷ��ȷ\r\n");
			  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
			  if(file_sd->fptr-ptr==Other_length)                      //��һ����¼ 30
				{
					strncpy((char *)&SdFile_Data.rdWorkFile.dat[0],(char *)&read_buffer[0],Other_length);
				  ptr=file_sd->fptr;
					res=FR_OK; 
				}
				else
				{ printf("\r\n ����¼����1\r\n");
					res=FR_DISK_ERR;
				}
				
				if(res==FR_OK)
				{
					 f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
			     if(file_sd->fptr-ptr==Adress_length)                      //�ڶ�����¼ 60
					 {
						 strncpy((char *)&SdFile_Data.rdWorkFile.Adress[0],(char *)&read_buffer[0],Adress_length);
				     ptr=file_sd->fptr;
					   res=FR_OK; 
					 }
					 else
					 { printf("\r\n ����¼����2\r\n");
						 res=FR_DISK_ERR;
					 }
					
				}
				
				if(res==FR_OK)
				{
					for(recordcnt=0;recordcnt<Work_car_length;recordcnt++)
					 { 
						  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
			        if(file_sd->fptr-ptr==Other_length)                      //��3����¼ ��
							{
								strncpy((char *)&SdFile_Data.rdWorkFile.Other[recordcnt][0],(char *)&read_buffer[0],Other_length);
								ptr=file_sd->fptr;
								res=FR_OK; 
							}
							else
							{
								printf("\r\n ����¼����3\r\n");
								res=FR_DISK_ERR;
								break;
								
							}
						 
					 }
					
				}
			
		 }
		 else
		 {
			 printf("\r\n ��λ�ļ�ͷ����\r\n");
			 res=FR_DISK_ERR;
		 }
		 
		f_close(file_sd);         //�ر��ļ�
	 }
	 else
	 {
		printf("\r\n open file err\r\n");
		res=FR_DISK_ERR;
	 }

   return(res);
}
/****************************************************************************
	��������WriteWorkFile(u32 NumFile)
	��  ��: FileNumber �ļ����
          SdFile_Data.wrWorkFile 
	��  ��:
	����˵����
***************************************************************************/

u8 SdFile_WriteWorkFile(strInterface *p,u32 NumFile)  //д�빤���ļ���¼
{
	 FRESULT res;
	 u8 i;

	 char pathbuf[202];   //�������ļ�·��
   SdFile_ImportWorkData(p);  //��������
	 SdFile_makelinkfile_record(NumFile);       //����������¼
	 strcpy(&pathbuf[0],(char *)&LINK_OF_Work[0]);
   SdFile_WriteLinkRecord((char *)&pathbuf[0],NumFile);   //�������ļ���¼
	
	 SdFile_NumberToDir((char *)&FOLDER_OF_Work00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������
	 
	 
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//�����ļ�
   if(res==FR_OK)  //�����ļ��ɹ�
	 {
		   //�������д�ļ�
		res = f_lseek(file_sd,0);               //��λ�ļ�ͷ
		 
		if (res==FR_OK)  //����дʱ�� 
	  {
		 strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.dat[0],Other_length);
		 pathbuf[Other_length]=0;
		 if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //дʱ��                                //��鵽�س����� 
		 {
		  printf("WRITE FILE successful!\r\n");
		  res=FR_OK;
		 }
		 else
		 {
 		  printf("WRITE FILE REEOR!\r\n");
		  res=FR_DISK_ERR; 
		 }
	  }
		 
    if(res==FR_OK)  //����д��ַ
		{  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Adress[0],Adress_length);
		   pathbuf[Adress_length]=0;
			if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		 {
		  printf("WRITE FILE successful!\r\n");
		  res=FR_OK;
		 }
		 else
		 {
 		  printf("WRITE FILE REEOR!\r\n");
		  res=FR_DISK_ERR; 
		 }
		}
		
   if(res==FR_OK) //����дOTHER��Ŀ
		{  
			 for(i=0;i<Work_car_length;i++)
			  {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Other[i][0],Other_length);
		       pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		       {
		         printf("WRITE FILE successful!\r\n");
		         res=FR_OK;
		        }
		       else
		       {
 		        printf("WRITE FILE REEOR!\r\n");
		        res=FR_DISK_ERR; 
						break;
		       }
				}
		}

			f_close(file_sd);         //�ر��ļ�
	 }
	 return res;

}


/****************************************************************************
	��������ReadDustFile(u32 NumFile)
	��  ��: FileNumber �ļ����
          
	��  ��:  SdFile_Data.rdWorkFile

	����˵������ȡ�����ļ�
***************************************************************************/

u8 SdFile_ReadDustFile(u32 NumFile)  //path �ļ���·��
{
	 FRESULT res;
	 u16 recordcnt;
	 DWORD ptr;
	 char pathbuf[50];   //�������ļ�·��
	 char read_buffer[250];      //
	 SdFile_NumberToDir((char *)&FOLDER_OF_Dust00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ);//�ȴ��ļ�
	 if (res==FR_OK)  
	 {
		 printf("\r\n open file OK\r\n");
		 res = f_lseek(file_sd,0);               //��λ�ļ�ͷ
		 ptr=0;
     if(res == FR_OK)
		 {
			  printf("\r\n ��λ�ļ�ͷ��ȷ\r\n");
			  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
			  if(file_sd->fptr-ptr==Other_length)                      //��һ����¼ 30
				{
					strncpy((char *)&SdFile_Data.rdWorkFile.dat[0],(char *)&read_buffer[0],Other_length);
				  ptr=file_sd->fptr;
					res=FR_OK; 
				}
				else
				{ printf("\r\n ����¼����1\r\n");
					res=FR_DISK_ERR;
				}
				
				if(res==FR_OK)
				{
					 f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
			     if(file_sd->fptr-ptr==Adress_length)                      //�ڶ�����¼ 60
					 {
						 strncpy((char *)&SdFile_Data.rdWorkFile.Adress[0],(char *)&read_buffer[0],Adress_length);
				     ptr=file_sd->fptr;
					   res=FR_OK; 
					 }
					 else
					 { printf("\r\n ����¼����2\r\n");
						 res=FR_DISK_ERR;
					 }
					
				}
				
				if(res==FR_OK)
				{
					for(recordcnt=0;recordcnt<Dust_car_length;recordcnt++)
					 { 
						  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
			        if(file_sd->fptr-ptr==Other_length)                      //��3����¼ ��
							{
								strncpy((char *)&SdFile_Data.rdWorkFile.Other[recordcnt][0],(char *)&read_buffer[0],Other_length);
				        ptr=file_sd->fptr;
					      res=FR_OK; 
							}
							else
							{
								printf("\r\n ����¼����3\r\n");
								res=FR_DISK_ERR;
								break;
								
							}
						 
					 }
					
				}
			
		 }
		 else
		 {
			 printf("\r\n ��λ�ļ�ͷ����\r\n");
			 res=FR_DISK_ERR;
		 }
		 
		f_close(file_sd);         //�ر��ļ�
	 }
	 else
	 {
		printf("\r\n open file err\r\n");
		res=FR_DISK_ERR;
	 }
   return(res);
}
/****************************************************************************
	��������WriteWorkFile(u32 NumFile)
	��  ��: FileNumber �ļ����
          SdFile_Data.wrWorkFile 
	��  ��:
	����˵����
***************************************************************************/

u8 SdFile_WriteDustFile(strInterface *p,u32 NumFile)  //д�빤���ļ���¼
{
	 FRESULT res;
	 u8 i;

	 char pathbuf[202];   //�������ļ�·��

	SdFile_ImportDustData(p);      //��������
	
  SdFile_makelinkfile_record(NumFile);       //����������¼
	 
   SdFile_WriteLinkRecord((char *)&LINK_OF_Dust[0],NumFile);   //д�����ļ���¼
	
	 SdFile_NumberToDir((char *)&FOLDER_OF_Dust00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������
	 
	 
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//�����ļ�
   if(res==FR_OK)  //�����ļ��ɹ�
	 {
		   //�������д�ļ�
		res = f_lseek(file_sd,0);               //��λ�ļ�ͷ
		 
		if (res==FR_OK)  //����дʱ�� 
	  {
		 strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.dat[0],Other_length);
		 pathbuf[Other_length]=0;
		 if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //дʱ��                                //��鵽�س����� 
		 {
		  printf("WRITE FILE successful!\r\n");
		  res=FR_OK;
		 }
		 else
		 {
 		  printf("WRITE FILE REEOR!\r\n");
		  res=FR_DISK_ERR; 
		 }
	  }
		 
    if(res==FR_OK)  //����д��ַ
		{  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Adress[0],Adress_length);
		   pathbuf[Adress_length]=0;
			if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		 {
		  printf("WRITE FILE successful!\r\n");
		  res=FR_OK;
		 }
		 else
		 {
 		  printf("WRITE FILE REEOR!\r\n");
		  res=FR_DISK_ERR; 
		 }
		}
		
   if(res==FR_OK) //����дOTHER��Ŀ
		{  
			 for(i=0;i<Dust_car_length;i++)
			  {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Other[i][0],Other_length);
		       pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		       {
		         printf("WRITE FILE successful!\r\n");
		         res=FR_OK;
		        }
		       else
		       {
 		        printf("WRITE FILE REEOR!\r\n");
		        res=FR_DISK_ERR; 
						break;
		       }
				}
		}
			f_close(file_sd);         //�ر��ļ�
	 }
	 return res;

}

/****************************************************************************
20200523
��������SdFile_WriteDustRecord(strInterface *p,u32 NumFile)  //д���̳��ļ��ļ�¼
	��  ��: FileNumber �ļ����
          SdFile_Data.wrWorkFile 
	��  ��:
	����˵����
***************************************************************************/

u8 SdFile_WriteDustRecord(strInterface *p,u32 NumFile)  //д�빤���ļ���¼
{
	 FRESULT res;
	 u8 i;
	 char pathbuf[202];   //�������ļ�·��
	SdFile_ImportDustData(p);      //��������
	SdFile_NumberToDir((char *)&FOLDER_OF_Dust00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//�����ļ�
   if(res==FR_OK)  //�����ļ��ɹ�
	 {
		   //�������д�ļ�
		res = f_lseek(file_sd,0+Other_length+Adress_length);               //��λ�ļ�ͷ
	
   if(res==FR_OK) //����дOTHER��Ŀ
		{  
			 for(i=0;i<Dust_car_length;i++)
			  {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Other[i][0],Other_length);
		       pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		       {
		         printf("WRITE FILE successful!\r\n");
		         res=FR_OK;
		        }
		       else
		       {
 		        printf("WRITE FILE REEOR!\r\n");
		        res=FR_DISK_ERR; 
						break;
		       }
				}
		}
			f_close(file_sd);         //�ر��ļ�
	 }
	 return res;

}
/****************************************************************************
	��������SdFile_ReadGasRecord(u32 NumFile,u32 NumRecord)  //��ȡ�����ļ���¼
	��  ��: NumFile �ļ����
          NumRecord �ڼ�����¼����1��ʼ

	��  ��:  SdFile_Data.rdWorkFile

	����˵������ȡ�����ļ�
***************************************************************************/

u8 SdFile_ReadGasRecord(u32 NumFile,u32 NumRecord)  //��ȡ�����ļ���¼
{
	 FRESULT res;
	
	 u8 i;
	 DWORD ptr;
	 char pathbuf[50];   //�������ļ�·��
	 char read_buffer[250];      //
	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ);//�ȴ��ļ�
	 if (res==FR_OK)  
	 {
		      printf("\r\n open file OK\r\n");
		
					if(NumRecord<=SdFile_Data.GasRecordMax)  //�ж���û�м�¼������¼����ʼ���ļ�ʱ����
					{
					 ptr=(Gas_car_length+1)*Other_length+Adress_length+(NumRecord-1)*GasRecord_length;  //��λ�ڼ�����¼λ��
					 res = f_lseek(file_sd,ptr);               //��λ�ļ�ͷ
					 f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
			     if(file_sd->fptr-ptr==GasRecord_length)                      // 200
					 {
						 strncpy((char *)&SdFile_Data.rdWorkFile.G_record.arr[0],(char *)&read_buffer[0],GasRecord_length);
				     ptr=file_sd->fptr;
					   res=FR_OK; 
					 }
					 else
					 { printf("\r\n ����¼����2\r\n");
						 res=FR_DISK_ERR;
					 }
					}
					else  //�޼�¼���ո� 
					{
						for(i=0;i<GasRecord_length;i++)
						SdFile_Data.rdWorkFile.G_record.arr[i]=' ';
						
					}
			f_close(file_sd);         //�ر��ļ�
		 }
		 else
		 {
			 printf("\r\n ��λ�ļ�ͷ����\r\n");
			 res=FR_DISK_ERR;
		 }

   return(res);
}

/****************************************************************************
	��������ReadDustFile(u32 NumFile)
	��  ��: NumFile �ļ����
          NumRecord �ڼ�����¼����1��

	��  ��:  SdFile_Data.rdWorkFile

	����˵������ȡ�����ļ�
***************************************************************************/

u8 SdFile_ReadGasFile(u32 NumFile,u32 NumRecord)  //��ȡ�����ļ���¼
{
	 FRESULT res;
	 u16 recordcnt;
	 u8 i;
	 DWORD ptr;
	 char pathbuf[50];   //�������ļ�·��
	 char read_buffer[250];      //
	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ);//�ȴ��ļ�
	 if (res==FR_OK)  
	 {
		 printf("\r\n open file OK\r\n");
		 res = f_lseek(file_sd,0);               //��λ�ļ�ͷ
		 ptr=0;
     if(res == FR_OK)
		 {
			  printf("\r\n ��λ�ļ�ͷ��ȷ\r\n");
			  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
			  if(file_sd->fptr-ptr==Other_length)                      //��һ����¼ 30
				{
					strncpy((char *)&SdFile_Data.rdWorkFile.dat[0],(char *)&read_buffer[0],Other_length);
					ptr=file_sd->fptr;
					res=FR_OK; 
				}
				else
				{ printf("\r\n ����¼����1\r\n");
					res=FR_DISK_ERR;
				}
				
				if(res==FR_OK)
				{
					 f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
					if(file_sd->fptr-ptr==Adress_length)                      //�ڶ�����¼ 60
					 {
						strncpy((char *)&SdFile_Data.rdWorkFile.Adress[0],(char *)&read_buffer[0],Adress_length);
						ptr=file_sd->fptr;
						res=FR_OK; 
					 }
					 else
					 { printf("\r\n ����¼����2\r\n");
						 res=FR_DISK_ERR;
					 }
					
				}
				
				if(res==FR_OK)
				{
					for(recordcnt=0;recordcnt<Gas_car_length;recordcnt++)
					 { 
						  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
			        if(file_sd->fptr-ptr==Other_length)                      //��3����¼ ��
							{
								strncpy((char *)&SdFile_Data.rdWorkFile.Other[recordcnt][0],(char *)&read_buffer[0],Other_length);
				        ptr=file_sd->fptr;
					      res=FR_OK; 
							}
							else
							{
								printf("\r\n ����¼����3\r\n");
								res=FR_DISK_ERR;
								break;

							}
						 
					 }
					
				}
				if(res==FR_OK)
				{ 
					if(NumRecord<=SdFile_Data.GasRecordMax)  //�ж���û�м�¼������¼����ʼ���ļ�ʱ����
					{
						ptr=file_sd->fptr+(NumRecord-1)*GasRecord_length;  //��λ�ڼ�����¼λ��

						res = f_lseek(file_sd,ptr);               //��λ�ļ�ͷ
						f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
						if(file_sd->fptr-ptr==GasRecord_length)                      // 200
						{
							strncpy((char *)&SdFile_Data.rdWorkFile.G_record.arr[0],(char *)&read_buffer[0],GasRecord_length);
							ptr=file_sd->fptr;
							res=FR_OK; 
						}
						else
						{ 
							printf("\r\n ����¼����2\r\n");
							res=FR_DISK_ERR;
						}
					}
					else  //�޼�¼���ո� 
					{
						for(i=0;i<GasRecord_length;i++)
						SdFile_Data.rdWorkFile.G_record.arr[i]=' ';
					}
					
				}
			
		 }
		 else
		 {
			 printf("\r\n ��λ�ļ�ͷ����\r\n");
			 res=FR_DISK_ERR;
		 }
		 
		f_close(file_sd);         //�ر��ļ�
	 }
	 else
	 {
		printf("\r\n open file err\r\n");
		res=FR_DISK_ERR;
	 }
   return(res);
}
/****************************************************************************
	��������SdFile_WriteGasFile(strInterface *p,u32 NumFile,u8 attr,u8 WriteLink)
	��  ��: FileNumber �ļ����
          SdFile_Data.wrWorkFile
          attr:>0ʱ׷�Ӽ�¼��=0ʱ��׷��
	��  ��:
	����˵����
***************************************************************************/
unsigned int writeWorkfileEnable = 1;
u8 SdFile_WriteGasFile(strInterface *p,u32 NumFile,u8 attr,u8 WriteLink)  //д�������ļ���¼
{
	FRESULT res;
	u8 i;
	DWORD ptr;
	char pathbuf[202];   //�������ļ�·��

	SdFile_ImportGasData(p);      //��������
	if(WriteLink==1)
	{
		 SdFile_makelinkfile_record(NumFile);       //����������¼
		 SdFile_WriteLinkRecord((char *)&LINK_OF_Gas[0],NumFile);   //д�����ļ���¼
	}
	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������

	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//�����ļ�
     if(res==FR_OK)  //�����ļ��ɹ�
	  {
		   //�������д�ļ�
		 
			if(attr) //>0ʱ׷�Ӽ�¼��=0ʱ��׷��
			{
				//�ȶ�Ϊ�����
				
				ptr=(DWORD)(Other_length*(Gas_car_length+1)+Adress_length+(SdFile_Data.GasRecordMax)*GasRecord_length);  //��λ�ڼ�����¼λ��
                res = f_lseek(file_sd,ptr);  
				strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.G_record.arr[0],GasRecord_length);
				pathbuf[GasRecord_length]=0;
           //��λ�ļ�ͷ
			  if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)        //��鵽�س����� 
		     {
				  printf("WRITE FILE successful...........!\r\n");
				  res=FR_OK;
		     }
		    else
		     {
				  printf("WRITE FILE REEOR!\r\n");
				  res=FR_DISK_ERR; 
		     }
			}
			else
			{
			res = f_lseek(file_sd,0);               //��λ�ļ�ͷ
		 
		     if (res==FR_OK)  //����дʱ�� 
	           {
		       strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.dat[0],Other_length);
		        pathbuf[Other_length]=0;
		     if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //дʱ��                                //��鵽�س����� 
		      {
		        printf("WRITE FILE successful!\r\n");
		        res=FR_OK;
		       }
		       else
		      {
 		        printf("WRITE FILE REEOR!\r\n");
		        res=FR_DISK_ERR; 
		       }
	           }
		 
               if(res==FR_OK)  //����д��ַ
		       {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Adress[0],Adress_length);
		          pathbuf[Adress_length]=0;
			      if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		           {
		            printf("WRITE FILE successful!\r\n");
		          res=FR_OK;
		           }
		           else
		            {
 		            printf("WRITE FILE REEOR!\r\n");
		            res=FR_DISK_ERR; 
		            }
		         }
		
                if(res==FR_OK) //����дOTHER��Ŀ
		         {  
			      for(i=0;i<Gas_car_length;i++)
			      {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Other[i][0],Other_length);
		            pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		           {
		            printf("WRITE FILE successful!\r\n");
		            res=FR_OK;
		           }
		          else
		           {
 		             printf("WRITE FILE REEOR!\r\n");
		             res=FR_DISK_ERR; 
					break;
		            }
				}
		        }
			}
			f_close(file_sd);         //�ر��ļ�

		}

	 return res;

}
/*

u8 SdFile_WriteGasFile(strInterface *p,u32 NumFile,u8 attr,u8 WriteLink)  //д�빤���ļ���¼
{
	 FRESULT res;
	 u8 i;

	 char pathbuf[202];   //�������ļ�·��

	 SdFile_ImportGasData(p);      //��������
	if(WriteLink==1){
   SdFile_makelinkfile_record(NumFile);       //����������¼
	 
   SdFile_WriteLinkRecord((char *)&LINK_OF_Gas[0],NumFile);   //д�����ļ���¼
	}
	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������
	 
	
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//�����ļ�
   if(res==FR_OK)  //�����ļ��ɹ�
	 {
		   //�������д�ļ�
		res = f_lseek(file_sd,0);               //��λ�ļ�ͷ
		 
		if (res==FR_OK)  //����дʱ�� 
	  {
		 strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.dat[0],Other_length);
		 pathbuf[Other_length]=0;
		 if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //дʱ��                                //��鵽�س����� 
		 {
		  printf("WRITE FILE successful!\r\n");
		  res=FR_OK;
		 }
		 else
		 {
 		  printf("WRITE FILE REEOR!\r\n");
		  res=FR_DISK_ERR; 
		 }
	  }
		 
    if(res==FR_OK)  //����д��ַ
		{  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Adress[0],Adress_length);
		   pathbuf[Adress_length]=0;
			if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		 {
		  printf("WRITE FILE successful!\r\n");
		  res=FR_OK;
		 }
		 else
		 {
 		  printf("WRITE FILE REEOR!\r\n");
		  res=FR_DISK_ERR; 
		 }
		}
		
   if(res==FR_OK) //����дOTHER��Ŀ
		{  
			 for(i=0;i<Gas_car_length;i++)
			  {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Other[i][0],Other_length);
		       pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		       {
		         printf("WRITE FILE successful!\r\n");
		         res=FR_OK;
		        }
		       else
		       {
 		        printf("WRITE FILE REEOR!\r\n");
		        res=FR_DISK_ERR; 
						break;
		       }
				}
		}
		
		if(res==FR_OK) //����д��Ӽ�¼
		{
			if(attr) //>0ʱ׷�Ӽ�¼��=0ʱ��׷��
			{
				//�ȶ�Ϊ�����
				file_sd->fptr=Other_length*(Gas_car_length+1)+Adress_length+(SdFile_Data.GasRecordMax)*GasRecord_length;  //��λ�ڼ�����¼λ��
				strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.G_record.arr[0],GasRecord_length);
				pathbuf[GasRecord_length]=0;
				res = f_lseek(file_sd,file_sd->fptr);             //��λ�ļ�ͷ
			  if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		     {
				  printf("WRITE FILE successful...........!\r\n");
				  res=FR_OK;
		     }
		    else
		     {
				  printf("WRITE FILE REEOR!\r\n");
				  res=FR_DISK_ERR; 
		     }
				
			}
			
		}
		
			f_close(file_sd);         //�ر��ļ�
	 }
	 return res;

}
*/
/****************************************************************************
20200515
	��������SdFile_WriteGasFileAverageRecord(strInterface *p,u32 NumFile)
	��  ��: FileNumber �ļ����
          pָ����Ҫд�������Դ
	��  ��:
	����˵����
***************************************************************************/
u8 SdFile_WriteGasFileAverageRecord(strInterface *p,u32 NumFile)  //��дһ������ƽ��ֵ
{
	 FRESULT res;
	 u8 i;

	 char pathbuf[202];   //�������ļ�·��

	 SdFile_ImportGasData(p);      //��������

	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������

	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//�����ļ�
   if(res==FR_OK)  //�����ļ��ɹ�
	 {
		   //�������д�ļ�
		file_sd->fptr=Other_length*(14)+Adress_length;  //��λ�ڼ�����¼д���λ��
		res = f_lseek(file_sd,file_sd->fptr);               //��λ�ļ�ͷ
	
   if(res==FR_OK) //����дOTHER��Ŀ
		{  
			 for(i=13;i<20;i++)   //д��13~19������
			  {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Other[i][0],Other_length);
		       pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		       {
		         printf("WRITE FILE successful!\r\n");
		         res=FR_OK;
		        }
		       else
		       {
 		        printf("WRITE FILE REEOR!\r\n");
		        res=FR_DISK_ERR; 
						break;
		       }
				}
		}
	
			f_close(file_sd);         //�ر��ļ�
	 }
	 return res;
}
/****************************************************************************
20200518
	��������SdFile_WriteGasFileAverageRecord(strInterface *p,u32 NumFile)
	��  ��: FileNumber �ļ����
          pָ����Ҫд�������Դ
	��  ��:
	����˵����
***************************************************************************/
u8 SdFile_WriteGasFile_Z_J_F_Record(strInterface *p,u32 NumFile)  //��дһ������ƽ��ֵ
{
	 FRESULT res;
	 u8 i;
   u8 ip[12]={9,10,13,14,15,16,17,18,19,23,24};
	 char pathbuf[202];   //�������ļ�·��

	 SdFile_ImportGasData(p);      //��������

	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������

	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//�����ļ�
   if(res==FR_OK)  //�����ļ��ɹ�
	 {
		   //�������д�ļ�

	
 
			 for(i=0;i<12;i++)   //д��13~19������
			  {
          file_sd->fptr=Other_length*(ip[i]+1)+Adress_length;  //��λ�ڼ�����¼д���λ��
		      res = f_lseek(file_sd,file_sd->fptr);               //��λ�ļ�ͷ
					if(res==FR_OK)  //�����ļ��ɹ�
	         {
					strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Other[ip[i]][0],Other_length);
		       pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		       {
		         printf("WRITE FILE successful!\r\n");
		         res=FR_OK;
		        }
		       else
		       {
 		        printf("WRITE FILE REEOR!\r\n");
		        res=FR_DISK_ERR; 
						break;
		       }
				 }
				}

	
			f_close(file_sd);         //�ر��ļ�
	 }
	 return res;
}
/****************************************************************************
20200515	
��������SdFile_WriteGasFiletxt
	��  ��: FileNumber �ļ����
          record �ڼ�����¼
          txt ��д��Ĳ�������ı���ַ�
	��  ��:
	����˵����
***************************************************************************/

u8 SdFile_WriteGasFiletxt(u32 NumFile,u32 record,char txt)  //���ļ�ĳ����ַд���ֽ�
{
	 FRESULT res;
	

	 char pathbuf[202];   //�������ļ�·��

	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������
	 	
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//�����ļ�
   if(res==FR_OK)  //�����ļ��ɹ�
	 {
		   //�������д�ļ�
	
				//�ȶ�Ϊ�����
				file_sd->fptr=Other_length*(Gas_car_length+1)+Adress_length+(record-1)*GasRecord_length+18;  //��λ�ڼ�����¼д���λ��
		    pathbuf[0]=txt;
				
		    pathbuf[1]=0;
				res = f_lseek(file_sd,file_sd->fptr);             //��λ�ļ�ͷ
			  if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		     {
		      printf("WRITE FILE successful...........!\r\n");
		      res=FR_OK;
		     }
		    else
		     {
 		      printf("WRITE FILE REEOR!\r\n");
		      res=FR_DISK_ERR; 
		     }
	
			f_close(file_sd);         //�ر��ļ�
	 }
	 return res;

}

/****************************************************************************
20200515
��������SdFile_WriteGasFiletxt
	��  ��: FileNumber �ļ����
          record �ڼ�����¼
          txt ��д��Ĳ�������ı���ַ�
	��  ��:
	����˵����
***************************************************************************/

u8 SdFile_ReadGasFiletxt(u32 NumFile,u32 record,char *txt)  //���ļ�ĳ����ַд���ֽ�
{
	 FRESULT res;

	 char pathbuf[50];   //�������ļ�·��
	 char read_buffer[250];      //
	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ);//�ȴ��ļ�
   if(res==FR_OK)  //�����ļ��ɹ�
	 {
		   //�������д�ļ�
	
				//�ȶ�Ϊ�����
				file_sd->fptr=Other_length*(Gas_car_length+1)+Adress_length+(record-1)*GasRecord_length;  //��λ�ڼ�����¼д���λ��
		    f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
				txt[0]=read_buffer[18];  //��ȡ�Ǹ�����ַ�
				res = f_lseek(file_sd,file_sd->fptr);             //��λ�ļ�ͷ
			  if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		     {
		      printf("WRITE FILE successful...........!\r\n");
		      res=FR_OK;
		     }
		    else
		     {
 		      printf("WRITE FILE REEOR!\r\n");
		      res=FR_DISK_ERR; 
		     }
	
			f_close(file_sd);         //�ر��ļ�
	 }
	 return res;

}

//--
/****************************************************************************
	��������SdFile_ReadFlueFile(u32 NumFile)  
	��  ��: NumFile �ļ����
  @lian 20200302
	��  ��:  SdFile_Data.FlueFile

	����˵������ȡ�̵��ļ�
***************************************************************************/

u8 SdFile_ReadFlueFile(u32 NumFile)  //��ȡ�̵��ļ���¼
{
	 FRESULT res;
	 u16 recordcnt;

	 DWORD ptr;
	 char pathbuf[50];   //�������ļ�·��
	 char read_buffer[250];      //
	 SdFile_NumberToDir((char *)&FOLDER_OF_Flue00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ);//�ȴ��ļ�
	 if (res==FR_OK)  
	 {
		 printf("\r\n open file OK\r\n");
		 res = f_lseek(file_sd,0);               //��λ�ļ�ͷ
		 ptr=0;
     if(res == FR_OK)
		 {
			  printf("\r\n ��λ�ļ�ͷ��ȷ\r\n");
			  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
			  if(file_sd->fptr-ptr==Other_length)                      //��һ����¼ 30
				{
					strncpy((char *)&SdFile_Data.FlueFile.dat[0],(char *)&read_buffer[0],Other_length);
				  ptr=file_sd->fptr;
					res=FR_OK; 
				}
				else
				{ printf("\r\n ����¼����1\r\n");
					res=FR_DISK_ERR;
				}
				
				if(res==FR_OK)
				{
					 f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
			     if(file_sd->fptr-ptr==Adress_length)                      //�ڶ�����¼ 60
					 {
						 strncpy((char *)&SdFile_Data.FlueFile.Adress[0],(char *)&read_buffer[0],Adress_length);
				     ptr=file_sd->fptr;
					   res=FR_OK; 
					 }
					 else
					 { printf("\r\n ����¼����2\r\n");
						 res=FR_DISK_ERR;
					 }
					
				}
				
				if(res==FR_OK)
				{
					for(recordcnt=0;recordcnt<Flue_car_length;recordcnt++)
					 { 
						  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//�Իس�Ϊ����ַ���	
			        if(file_sd->fptr-ptr==Other_length)                      //��3����¼ ��
							{
								strncpy((char *)&SdFile_Data.FlueFile.Other[recordcnt][0],(char *)&read_buffer[0],Other_length);
				        ptr=file_sd->fptr;
					      res=FR_OK; 
							}
							else
							{
								printf("\r\n ����¼����3\r\n");
								res=FR_DISK_ERR;
								break;

							}
						 
					 }
					
				}
			
			
		 }
		 else
		 {
			 printf("\r\n ��λ�ļ�ͷ����\r\n");
			 res=FR_DISK_ERR;
		 }
		 
		f_close(file_sd);         //�ر��ļ�
	 }
	 else
	 {
		printf("\r\n open file err\r\n");
		res=FR_DISK_ERR;
	 }
   return(res);
}
/****************************************************************************
	��������SdFile_WriteFlueFile(strInterface *p,u32 NumFile)
	��  ��: FileNumber �ļ����
          SdFile_Data.wrWorkFile
         
  @lian 20200203
	��  ��:
	����˵����
***************************************************************************/

u8 SdFile_WriteFlueFile(strInterface *p,u32 NumFile)  //д���̵��ļ���¼
{
	 FRESULT res;
	 u8 i;

	 char pathbuf[202];   //�������ļ�·��

	 SdFile_ImportFlueData();      //��������
	
   SdFile_makelinkfile_record(NumFile);       //����������¼
	 
   SdFile_WriteLinkRecord((char *)&LINK_OF_Flue[0],NumFile);   //д�����ļ���¼
	
	 SdFile_NumberToDir((char *)&FOLDER_OF_Flue00[0],(char *)&pathbuf[0],NumFile); //���ļ���·���ŵ�·��������
	 
	 
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//�����ļ�
   if(res==FR_OK)  //�����ļ��ɹ�
	 {
		   //�������д�ļ�
		res = f_lseek(file_sd,0);               //��λ�ļ�ͷ
		 
		if (res==FR_OK)  //����дʱ�� 
	  {
		 strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.FlueFile.dat[0],Other_length);
		 pathbuf[Other_length]=0;
		 if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //дʱ��                                //��鵽�س����� 
		 {
		  printf("WRITE FILE successful!\r\n");
		  res=FR_OK;
		 }
		 else
		 {
 		  printf("WRITE FILE REEOR!\r\n");
		  res=FR_DISK_ERR; 
		 }
	  }
		 
    if(res==FR_OK)  //����д��ַ
		{  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.FlueFile.Adress[0],Adress_length);
		   pathbuf[Adress_length]=0;
			if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		 {
		  printf("WRITE FILE successful!\r\n");
		  res=FR_OK;
		 }
		 else
		 {
 		  printf("WRITE FILE REEOR!\r\n");
		  res=FR_DISK_ERR; 
		 }
		}
		
   if(res==FR_OK) //����дOTHER��Ŀ
		{  
			 for(i=0;i<Flue_car_length;i++)
			  {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.FlueFile.Other[i][0],Other_length);
		       pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //��ַ                              //��鵽�س����� 
		       {
		         printf("WRITE FILE successful!\r\n");
		         res=FR_OK;
		        }
		       else
		       {
 		        printf("WRITE FILE REEOR!\r\n");
		        res=FR_DISK_ERR; 
						break;
		       }
				}
		}
		
			f_close(file_sd);         //�ر��ļ�
	 }
	 return res;

}
//--
/****************************************************************************
	��������SdFile_ramlodair(char *p,u8 length)//���������ո�
	��  ��: 
          
	��  ��:
	����˵����
***************************************************************************/

void  SdFile_ramlodair(char *p,u8 length)//���������ո�
{
	    u8 i;
	    for(i=0;i<length;i++)
	    {
				*p=' ';
				p++;
	    }
}
/****************************************************************************
	��������SdFile_ImportGasData(strInterface *p)
	��  ��: strInterface *p
          
	��  ��:
	����˵����������������
***************************************************************************/

void  SdFile_ImportGasData(strInterface *p)
{     u8 i;
	    SdFile_ramlodair((char *)&SdFile_Data.wrWorkFile.dat[0],Other_length);  //�����ո�
	    SdFile_Data.wrWorkFile.dat[Other_length-2]='\r';
	    SdFile_Data.wrWorkFile.dat[Other_length-1]='\n';
	    strncpy((char *)&SdFile_Data.wrWorkFile.dat[0],(char *)&SdFile_Data.InterFace_data.dat[0],Interf_dat_length);  //����ʱ��
	  //MARK20210320
	    strncpy((char *)&SdFile_Data.wrWorkFile.dat[Interf_dat_length+2],(char *)&SdFile_Data.InterFace_data.T[0],2);  //���� ����ʱ��  
	
	   
	    SdFile_ramlodair((char *)&SdFile_Data.wrWorkFile.Adress[0],Adress_length);  //�����ո�
	    
	
	    strncpy((char *)&SdFile_Data.wrWorkFile.Adress[0],(char *)"������ַ:",9);
	    strncpy((char *)&SdFile_Data.wrWorkFile.Adress[9],(char *)&SdFile_Data.InterFace_data.Adress[0],(Adress_length-9));  //������ַ
	    SdFile_Data.wrWorkFile.Adress[Adress_length-2]='\r';
	    SdFile_Data.wrWorkFile.Adress[Adress_length-1]='\n';
	    for(i=0;i<Gas_car_length;i++)
	    {
			strncpy((char *)&SdFile_Data.wrWorkFile.Other[i][0],(char *)&Gas_line[i][0],Other_length);    //����ԭʼ���ݸ�ʽ
		}
		 for(i=0;i<Gas_car_length;i++)
	    {
			strncpy((char *)&SdFile_Data.wrWorkFile.Other[i][9],(char *)&SdFile_Data.InterFace_data.Other[i][0],Interf_other_length);    //�����ӿ�����
		}
	  
			//�����ǿ�����¼
			strncpy((char *)&SdFile_Data.wrWorkFile.G_record.arr[0],(char *)&Gas_line_record[0],GasRecord_length); //������¼ԭʼ���ݸ�ʽ
			strncpy((char *)&SdFile_Data.wrWorkFile.G_record.stru.rowdata[0][0],(char *)&SdFile_Data.InterFace_data.recorddat[0],(Record_row_length-1)); //������¼ʱ�䣬�����򶺺�
			for(i=0;i<9;i++)
			{
				strncpy((char *)&SdFile_Data.wrWorkFile.G_record.stru.rowdata[i+1][0],(char *)&SdFile_Data.InterFace_data.record[i][0],(Record_row_length-10));
			}
}
/****************************************************************************
	��������SdFile_ImportDustData(strInterface *p)
	��  ��: strInterface *p
          
	��  ��:
	����˵���������̳�����
***************************************************************************/
void  SdFile_ImportDustData(strInterface *p)
{
	     u8 i;
	    SdFile_ramlodair((char *)&SdFile_Data.wrWorkFile.dat[0],Other_length);  //�����ո�
	    SdFile_Data.wrWorkFile.dat[Other_length-2]='\r';
	    SdFile_Data.wrWorkFile.dat[Other_length-1]='\n';
	    strncpy((char *)&SdFile_Data.wrWorkFile.dat[0],(char *)&SdFile_Data.InterFace_data.dat[0],Interf_dat_length);  //����ʱ��
	    
	    SdFile_ramlodair((char *)&SdFile_Data.wrWorkFile.Adress[0],Adress_length);  //�����ո�
	  
	    strncpy((char *)&SdFile_Data.wrWorkFile.Adress[0],(char *)"������ַ:",9);
	    strncpy((char *)&SdFile_Data.wrWorkFile.Adress[9],(char *)&SdFile_Data.InterFace_data.Adress[0],(Adress_length-9));  //������ַ
	    SdFile_Data.wrWorkFile.Adress[Adress_length-2]='\r';
	    SdFile_Data.wrWorkFile.Adress[Adress_length-1]='\n';
	
	    for(i=0;i<Dust_car_length;i++)
	    {
				strncpy((char *)&SdFile_Data.wrWorkFile.Other[i][0],(char *)&Dust_line[i][0],Other_length);    //����ԭʼ���ݸ�ʽ
			}
			 for(i=0;i<Dust_car_length;i++)
	    {
				strncpy((char *)&SdFile_Data.wrWorkFile.Other[i][9],(char *)&SdFile_Data.InterFace_data.Other[i][0],Interf_other_length);    //�����ӿ�����
			}
}
/****************************************************************************
	��������SdFile_ImportWorkData(strInterface *p)
	��  ��: strInterface *p
          
	��  ��:
	����˵�������빤������
***************************************************************************/
void  SdFile_ImportWorkData(strInterface *p)
{
	    u8 i;
	    SdFile_ramlodair((char *)&SdFile_Data.wrWorkFile.dat[0],Other_length);  //�����ո�
	    SdFile_Data.wrWorkFile.dat[Other_length-2]='\r';
	    SdFile_Data.wrWorkFile.dat[Other_length-1]='\n';
	    strncpy((char *)&SdFile_Data.wrWorkFile.dat[0],(char *)&SdFile_Data.InterFace_data.dat[0],Interf_dat_length);  //����ʱ��
	    
	    SdFile_ramlodair((char *)&SdFile_Data.wrWorkFile.Adress[0],Adress_length);  //�����ո�
	    SdFile_Data.wrWorkFile.Adress[Adress_length-2]='\r';
	    SdFile_Data.wrWorkFile.Adress[Adress_length-1]='\n';
	
	    strncpy((char *)&SdFile_Data.wrWorkFile.Adress[0],(char *)"������ַ:",9);
	    strncpy((char *)&SdFile_Data.wrWorkFile.Adress[9],(char *)&SdFile_Data.InterFace_data.Adress[0],(Adress_length-9-2));  //������ַ
	    for(i=0;i<Work_car_length;i++)
	    {
			strncpy((char *)&SdFile_Data.wrWorkFile.Other[i][0],(char *)&Work_line[i][0],Other_length);    //����ԭʼ���ݸ�ʽ
		}
		 for(i=0;i<Work_car_length;i++)
		{
			strncpy((char *)&SdFile_Data.wrWorkFile.Other[i][9],(char *)&SdFile_Data.InterFace_data.Other[i][0],Interf_other_length);    //�����ӿ�����
		}
}
//--
/****************************************************************************
	SdFile_ImportFlueData(void)
	��  ��: void
  @lian 20200302        
	��  ��:
	����˵���������̵�����
***************************************************************************/
void  SdFile_ImportFlueData(void)
{
	    u8 i;
	    SdFile_ramlodair((char *)&SdFile_Data.FlueFile.dat[0],Other_length);  //�����ո�
	    SdFile_Data.FlueFile.dat[Other_length-2]='\r';
	    SdFile_Data.FlueFile.dat[Other_length-1]='\n';
	    strncpy((char *)&SdFile_Data.FlueFile.dat[0],(char *)&SdFile_Data.InterFace_data.dat[0],Interf_dat_length);  //����ʱ��
	    
	    SdFile_ramlodair((char *)&SdFile_Data.FlueFile.Adress[0],Adress_length);  //�����ո�
	   
	
	    strncpy((char *)&SdFile_Data.FlueFile.Adress[0],(char *)"������ַ:",9);
	    strncpy((char *)&SdFile_Data.FlueFile.Adress[9],(char *)&SdFile_Data.InterFace_data.Adress[0],(Adress_length-9));  //������ַ
	    SdFile_Data.FlueFile.Adress[Adress_length-2]='\r';
	    SdFile_Data.FlueFile.Adress[Adress_length-1]='\n';
	   for(i=0;i<Flue_car_length;i++)
	    {
				strncpy((char *)&SdFile_Data.FlueFile.Other[i][0],(char *)&Flue_line[i][0],Other_length);    //����ԭʼ���ݸ�ʽ
			}
			 for(i=0;i<Flue_car_length;i++)
	    {
				strncpy((char *)&SdFile_Data.FlueFile.Other[i][9],(char *)&SdFile_Data.InterFace_data.Other[i][0],Interf_other_length);    //�����ӿ�����
			}
			
	
}



//�ļ�����
//��psrc�ļ�,copy��pdst.
//psrc,pdst:Դ�ļ���Ŀ���ļ�
//fwmode:�ļ�д��ģʽ
//0:������ԭ�е��ļ�
//1:����ԭ�е��ļ�
u8 mf_copy(u8 *psrc,u8 *pdst,u8 fwmode)
{
	u8 res;
	u16 br=0;
	u16 bw=0;
	FIL *fsrc=0;
	FIL *fdst=0;
	u8 *fbuf=0;
	fsrc=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//�����ڴ�
	fdst=(FIL*)mymalloc(SRAMIN,sizeof(FIL));
	fbuf=(u8*)mymalloc(SRAMIN,512);
	if(fsrc==NULL||fdst==NULL||fbuf==NULL)res=100;//ǰ���ֵ����fatfs
	else
	{
		if(fwmode==0)fwmode=FA_CREATE_NEW;//������
		else fwmode=FA_CREATE_ALWAYS; //���Ǵ��ڵ��ļ�
		
		res=f_open(fsrc,(const TCHAR*)psrc,FA_READ|FA_OPEN_EXISTING);//��ֻ���ļ�
		if(res==0)res=f_open(fdst,(const TCHAR*)pdst,FA_WRITE|fwmode); //��һ���򿪳ɹ�,�ſ�ʼ�򿪵ڶ���
		if(res==0)//�������򿪳ɹ���
		{
			while(res==0)//��ʼ����
			{
				res=f_read(fsrc,fbuf,512,(UINT*)&br);//Դͷ����512�ֽ�
				if(res||br==0)break;
				res=f_write(fdst,fbuf,(UINT)br,(UINT*)&bw);//д��Ŀ���ļ�
				if(res||bw<br) break;
			}
			f_close(fsrc);
			f_close(fdst);
		}
	}
	myfree(SRAMIN,fsrc);//�ͷ��ڴ�
	myfree(SRAMIN,fdst);
	myfree(SRAMIN,fbuf);
	return res;
}



//--
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Slave_DebugASCII(u8 *p,u8 len)
	������		�ú���Ϊslave.c�ļ���ASCII�����ļ�
						:000|03|001
*/
const float input[50]={0.000,0.111,0.222,0.333,0.444,0.555,0.666,0.777,0.888,0.999,
	                 1.000,1.111,1.222,1.333,1.444,1.555,1.666,1.777,1.888,1.999,
		               2.000,3601,2.222,2.333,2.444,2.555,2.666,2.777,2.888,2.999,
		               3.000,3.111,3.222,3.333,3.444,3.555,3.666,3.777,3.888,3.999,
		               4.000,4.111,4.222,4.333,4.444,4.555,4.666,1.777,4.888,1.999,
		              };
void File_DebugASCII(u8 *p,u8 len)
{ u8 i;
	char buf[202];

	u16 cmd = Calculate_CharToU16(p,3);//��������

	switch(cmd)
	{
		case 1:
		{//��ȡ���д�������ʵʱ����ֵ
		  printf("\r\n��ȡ�����ļ�1");
			SdFile_ReadWorkFile(0x01);  //��ȡ1���ļ�
			
			strncpy(&buf[0],&SdFile_Data.rdWorkFile.dat[0],Other_length);   //��ȡ��ȡ��ʱ��ŵ��ӿ����ݽṹ��ʱ����
			buf[Other_length]=0;
			printf("\r\n%s",&buf[0]);
			
			strncpy(&SdFile_Data.InterFace_data.dat[0],&SdFile_Data.rdWorkFile.dat[0],Interf_dat_length);
		
			strncpy(&buf[0],&SdFile_Data.rdWorkFile.Adress[0],Adress_length);
			buf[Adress_length]=0;
			printf("%s",&buf[0]);
			
			strncpy(&SdFile_Data.InterFace_data.Adress[0],&SdFile_Data.rdWorkFile.Adress[9],(Adress_length-9));
		
			for(i=0;i<Work_car_length;i++)
			{
			strncpy(&buf[0],&SdFile_Data.rdWorkFile.Other[i][0],Other_length);
			buf[Other_length]=0;
			printf("%s",&buf[0]);
			strncpy(&SdFile_Data.InterFace_data.Other[i][0],&SdFile_Data.rdWorkFile.Other[i][9],Interf_other_length);	
			}
	
			break;
		}
		case 2:
		{	
		printf("\r\nд�����ļ�6");
		SaveWorkData((float *)&input[0],5);
		SdFile_WriteWorkFile(&SdFile_Data.InterFace_data,0x05);  //д��6���ļ�
			break;
		}
		case 3:
		{
		
			break;
		}
		
		case 4:   //
		{ SdFile_Serch_MaxofGasLinkFile();
			printf("�����ļ���%d",SdFile_Data.GasLinkFileMax);
			break;
		}
		case 5:   //
		{ SdFile_Serch_MaxofDustLinkFile();
			printf("�̳��ļ���%d",SdFile_Data.DustLinkFileMax);
			break;
		}
		case 6:   //
		{ SdFile_Serch_MaxofWorkLinkFile();
			printf("�����ļ���%d",SdFile_Data.WorkLinkFileMax);
			break;
		}
		case 7:   //
		{
			SdFile_Serch_MaxofGasRecord(0x08);
			printf("8�������ļ���¼��%d",SdFile_Data.GasRecordMax);
			break;
		}

		case 11:
		{//��ȡ���д�������ʵʱ����ֵ
		  printf("\r\n��ȡ�̳��ļ�1");
			SdFile_ReadDustFile(0x01);  //��ȡ1���ļ�
			
			strncpy(&buf[0],&SdFile_Data.rdWorkFile.dat[0],Other_length);   //��ȡ��ȡ��ʱ��ŵ��ӿ����ݽṹ��ʱ����
			buf[Other_length]=0;
			printf("\r\n%s",&buf[0]);
			
			strncpy(&SdFile_Data.InterFace_data.dat[0],&SdFile_Data.rdWorkFile.dat[0],Interf_dat_length);
		
			strncpy(&buf[0],&SdFile_Data.rdWorkFile.Adress[0],Adress_length);
			buf[Adress_length]=0;
			printf("%s",&buf[0]);
			
			strncpy(&SdFile_Data.InterFace_data.Adress[0],&SdFile_Data.rdWorkFile.Adress[9],(Adress_length-9));
		
			for(i=0;i<Dust_car_length;i++)
			{
			strncpy(&buf[0],&SdFile_Data.rdWorkFile.Other[i][0],Other_length);
			buf[Other_length]=0;
			printf("%s",&buf[0]);
			strncpy(&SdFile_Data.InterFace_data.Other[i][0],&SdFile_Data.rdWorkFile.Other[i][9],Interf_other_length);	
			
			}

			break;
		}
		case 12:
		{
			printf("\r\nд�̳��ļ�4");
			SaveDustData((float *)&input[0],4);
			SdFile_WriteDustFile(&SdFile_Data.InterFace_data,0x04);  //д��4���ļ�
			break;
		}
		case 13:
		{
		
			break;
		}
		
		case 21:
		{//��ȡ���д�������ʵʱ����ֵ
			
			SdFile_Serch_MaxofGasRecord(0x01);    //��ȡ����¼��,��֮ǰҪȷ�����ֵ
			printf("1�������ļ���¼��%d",SdFile_Data.GasRecordMax);
		  printf("\r\n��ȡ�����ļ�1");
			SdFile_ReadGasFile(0x01,0x01);  //��ȡ1���ļ�1����¼
						
			strncpy(&buf[0],&SdFile_Data.rdWorkFile.dat[0],Other_length);   //��ȡ��ȡ��ʱ��ŵ��ӿ����ݽṹ��ʱ����
			buf[Other_length]=0;
			printf("\r\n%s",&buf[0]);
			
			strncpy(&SdFile_Data.InterFace_data.dat[0],&SdFile_Data.rdWorkFile.dat[0],Interf_dat_length);
		
			strncpy(&buf[0],&SdFile_Data.rdWorkFile.Adress[0],Adress_length);
			buf[Adress_length]=0;
			printf("%s",&buf[0]);
			
			strncpy(&SdFile_Data.InterFace_data.Adress[0],&SdFile_Data.rdWorkFile.Adress[9],(Adress_length-9));
			for(i=0;i<Gas_car_length;i++)
			{
			strncpy(&buf[0],&SdFile_Data.rdWorkFile.Other[i][0],Other_length);
			buf[Other_length]=0;
			printf("%s",&buf[0]);
			strncpy(&SdFile_Data.InterFace_data.Other[i][0],&SdFile_Data.rdWorkFile.Other[i][9],Interf_other_length);	
			}
			
      if(SdFile_Data.rdWorkFile.G_record.arr[0]!=' ')
			{
			strncpy(&buf[0],&SdFile_Data.rdWorkFile.G_record.arr[0],GasRecord_length);
			buf[GasRecord_length]=0;
			printf("%s",&buf[0]);	
				
		  strncpy(&SdFile_Data.InterFace_data.recorddat[0],&SdFile_Data.rdWorkFile.G_record.stru.rowdata[0][0],Record_row_length-1);
			for(i=0;i<9;i++)
				{
			strncpy(&SdFile_Data.InterFace_data.record[i][0],&SdFile_Data.rdWorkFile.G_record.stru.rowdata[i+1][0],Record_row_length-10);		
				}
				
			}

			break;
		}
		case 22:
		{ SdFile_Serch_MaxofGasRecord(0x01);    //��ȡ����¼��
			printf("1�������ļ���¼��%d",SdFile_Data.GasRecordMax);
			SdFile_Data.GasRecordMax=0;    //ʹ��Ҫ��������Ͳ�������ļ���
			printf("\r\nд������¼1");
			SaveGasData((float *)&input[0],11,0x01);
			SdFile_WriteGasFile(&SdFile_Data.InterFace_data,11,0x01,0x01);  //д��11���ļ�
			break;
		}
		case 30:
		{
			SdFile_DeleteDustFiles();
			break;
		}
		case 31:
		{
			SdFile_DeleteGasFiles();
			break;
		}
		case 32:
		{
			SdFile_DeleteWorkFiles();
			break;
		}
		case 33:
		{
			DeleteAllFiles();
			break;
		}
		default:
		{
		break;
		}
	}
}
//---


#ifdef fielfief
u16 Serch_RealDataFile()
{

}

u16 Serch_AbNormalFile()
{

}

u16 Serch_QiXiangFile(void)
{

}
#endif
