//--------include------------
#include "sys.h"  
//#include "sample.h"
//#include "process.h"
#include <string.h>
#include <stdio.h>
#include "usb.h"
#include "file.h"
#include "timer.h"
#include "malloc.h" 
//#include "YLB001_CONFIG.h" 
#include "process.h"
#include "ScreenDwFunction.h"
//#include "sdio_sdcard.h"   

#include "ff.h"              
#include "file.h"
          
#include "exfuns.h" 
#include "usbh_core.h"
#include "usbh_usr.h"
USBH_HOST  USB_Host;
USB_OTG_CORE_HANDLE  USB_OTG_Core;

u8 USH_User_App(void)
{ 
	//u32 total,free;
	u8 res=0;
	USH_User_App_flag=DF_USH_USER_APP_OK;
//	printf("�豸���ӳɹ�\r\n");
	return res;
} 

//#include "ZR3500_CONFIG.h"
#define DEF_SECTOR_SIZE  512

const u8 Usb_FILE_DIR[]="2:/YLB-3330A";  //�ļ�Ŀ¼
const u8 Usb_FILE_OF_Gas[]="GasFile";   //�����ļ�·��
const u8 Usb_FILE_OF_Dust[]="DustFile";  //�����ļ�·��


Usb_DataStructure Usb_Data;


/*
	��������SdFile_DataStruct* SdFile_GetData(void)
	��������ȡ����File.c�ļ�ȫ�ֱ����ṹ��
*/
Usb_DataStructure* Usb_GetData(void)
{
	return &Usb_Data;
}
/*
	��������UsbInit(void)
	��������ʼ��Usb�ṹ����
*/
void Usb_Init(void)
{ 
	//u32 buf,i;
	//---------------------------------------------------
	
	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);   //USB��ʼ��
	//---------------------------------------------------
  Usb_Data.testflg=1;
	
}

/****************************************************************************
	��������Usb_Timer
	��  ��:
        
	��  ��:
	����˵�����û�USB���ö�ʱ��

*/
void Usb_Timer(void)
{
	if(Usb_Data.USB_OUT_Gas.Timer!=0)
		Usb_Data.USB_OUT_Gas.Timer--;
	if(Usb_Data.USB_OUT_Dust.Timer!=0)
		Usb_Data.USB_OUT_Dust.Timer--;
}

/****************************************************************************
	��������Usb_Main(void)
	��  ��:
        
	��  ��:
	����˵����USB ��س���

*/
void Usb_Main(void)
{
	USH_User_App_flag = DF_USH_USER_APP_INVALID;
	USBH_Process(&USB_OTG_Core, &USB_Host);
	Usb_OutGasFileTask();
	Usb_OutDustFileTask();
}
/****************************************************************************
	��������mFlushBufferToDisk
	��  ��:force=0����⻺�����Ƿ����������Ǿ�д�ļ�
         force=1������黺����,ֱ��д�ļ�
	��  ��:
	����˵�����ѻ�����������ֱ��д�ļ�

*/

void mFlushBufferToDisk(u8 force)
// force = 0 ���Զ�ˢ��(��黺�����е����ݳ���,����д��,��������ʱ���ڻ�������), force != 0 ��ǿ��ˢ��(���ܻ������е������ж��ٶ�д��,ͨ����ϵͳ�ػ�ǰӦ��ǿ��д��)
{
	if(force != 0) 
	{  // ǿ��ˢ��
		f_write(file_usb,Usb_Data.FILE_DATA_BUF,Usb_Data.total,&bw_usb);
		Usb_Data.total = 0;       //д�껺����λ�ü���Ҫ��0
		f_sync(file_usb);         //�ļ�ͬ������,���ִ򿪡�
	}
	else if(Usb_Data.total >= DEF_SECTOR_SIZE) 
	{  // �������е����ݿ�Ҫ����,����Ӧ���Ƚ��������е�ԭ������д��U��
		f_write(file_usb,Usb_Data.FILE_DATA_BUF,Usb_Data.total,&bw_usb);
		Usb_Data.total = 0;   //д�껺����λ�ü���Ҫ��
		f_sync(file_usb);     //�ļ�ͬ������,���ִ�
	}
}
/****************************************************************************
	��������my_sprintf
	��  ��:*array ����������ǰ��ַ
         *pstr���ַ�����ǰ��ַ
         flag���Ƿ����ӻس������ַ�,��=1��
	��  ��:
	����˵������pstr�ַ�����ӵ��ļ�������array��ֱ��512�ֽ�

*/

u16 my_sprintf(u8 *array,u8 *pstr,u16 flag)
{
    u16 i = 0;
    while(*pstr)          //�ַ�����û�дﵽEOF
    {
        *(array + i) = (*pstr);  //���ַ����Ƶ���������ǰλ��
        i++;
        pstr += 1;                //�ַ����Ƶ���һλ
    }
    if(flag == 1)   //�������flag==1,����ϻس�������
		{
        *(array + i) = '\xd';	   //�س�
        *(array + i + 1) = '\xa';	   //����
        i += 2;
    }
    else
    {
			
    }
    return i;
}

/****************************************************************************
	��������Usb_OutGasFileInf(u32 start_file,u32 end_file, u8* sysDate, u8* macNumber)
	��  ��:start_file ����ʼ�ļ���
         end_file��   �����ļ���       
	��  ��:
	����˵��:  �ⲿ���ýӿڣ����������ļ�����
	�޸ļ�¼��	1. ����ʱʹ��ϵͳ���ں��豸�������, modify by alfre, 2020.06.04.
*/
void Usb_OutGasFileInf(u32 start_file,u32 end_file, char *sysDate, char *macNumber)  //�ⲿ���ýӿڣ����������ļ�����
{
	strncpy((char *)&Usb_Data.dat[0],(char *)sysDate,8);   //���ϵ�������
	strncpy((char *)&Usb_Data.bianhao[0],(char *)macNumber,8);   //���ϵ������
	Usb_Data.usb_out_start_file=start_file;
	Usb_Data.usb_out_end_file=end_file;
	Usb_Data.USB_OUT_Gas.step=DF_USB_INIT;
}

/****************************************************************************
	��������Usb_OutDustFileInf(u32 start_file,u32 end_file, u8* sysDate, u8* macNumber) 
	��  ��:start_file ����ʼ�ļ���
         end_file��   �����ļ���         
	��  ��:
	����˵��:  �ⲿ���ýӿڣ������̳��ļ�����
	�޸ļ�¼��	1. ����ʱʹ��ϵͳ���ں��豸�������, modify by alfre, 2020.06.04.
*/
void Usb_OutDustFileInf(u32 start_file,u32 end_file,char *sysDate, char *macNumber)  //�ⲿ���ýӿڣ������̳��ļ�����
{
	
	strncpy((char *)&Usb_Data.dat[0],(char *)sysDate,8);   //���ϵ�������
	strncpy((char *)&Usb_Data.bianhao[0],(char *)macNumber,8);   //���ϵ������
	Usb_Data.usb_out_start_file=start_file;
	Usb_Data.usb_out_end_file=end_file;
	Usb_Data.USB_OUT_Dust.step=DF_USB_INIT;
	
}
/****************************************************************************
	��������Usb_GasFileOut_TXT(u32 start_file,u32 end_file) 
	��  ��:start_file ����ʼ�ļ���
         end_file��   �����ļ���
         usb_out_start_file,usb_out_end_file
	��  ��:
	����˵��: //�������������ļ�����

*/
void Usb_GasFileOut_TXT(u32 start_file,u32 end_file)  //�������������ļ�����
{
	u8 i;
  u8 strbuf[100];
	u8 bufdata[50];
	//����SD����ȡ�ļ�
  SdFile_Serch_MaxofGasRecord(start_file);    //��ȡ����¼��,��֮ǰҪȷ�����ֵ
	SdFile_ReadGasFile(start_file,0x01);        //��ȡ1���ļ��ļ�ͷ��1����¼
  Usb_Data.usb_out_file_step = DF_FILE_HEAD_LILE;
	if(Usb_Data.usb_out_file_step == DF_FILE_HEAD_LILE)           //�ļ�ͷ���ַ�
	{
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"  YLB-3330�͵�Ũ���Զ��̳������ۺϲ����ǲ�������",1); //���ӻس�����
		mFlushBufferToDisk( 0 );  // �Զ�ˢ�»�����,��黺�����Ƿ�����,����д��
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"---------���������ļ�---------",1);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"����汾��:   ",0);
		mFlushBufferToDisk( 0 );
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)YLB3330_SOFE_VERSION,1);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"�������  :3330A",0);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.bianhao[8]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&Usb_Data.bianhao[0],1);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"�� �� ��  :       ",0);
		mFlushBufferToDisk( 0 );
		SPutl(0x06,(u8*)&strbuf[0],Usb_Data.usb_out_start_file); //���ļ���ת����6Ϊ����
		strbuf[6]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);
		mFlushBufferToDisk( 0 );
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"--------------------------------",1);
		mFlushBufferToDisk( 0 );
		Usb_Data.usb_out_file_step = DF_FILE_CONTENT;
		
	}
	if(Usb_Data.usb_out_file_step == DF_FILE_CONTENT)//�ļ��м�
	{
		 //���ݲ���ʱ��
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"����ʱ�䣺",0);   
		mFlushBufferToDisk( 0 ); 
   	strncpy((char  *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.dat[0],Other_length);   //��ȡ��ȡ��ʱ��ŵ��ӿ����ݽṹ��ʱ����
		strbuf[Other_length]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],0);   
		mFlushBufferToDisk( 0 );  
		 //���ݲ�����ַ
		strncpy((char  *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Adress[0],Adress_length);
		strbuf[Adress_length]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],0);   
		mFlushBufferToDisk( 0 ); 
		//�����Ǹ���"�˿�����"���о�
		   //������Ũ��
		if(SdFile_GetData()->rdWorkFile.Other[20][15]=='9')  //��O2����
		{
			strcpy((char *)&strbuf[0],(char*)" O2 Ũ��:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[13][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][16]=='9') //��SO2����
		{
			
			strcpy((char *)&strbuf[0],(char*)"SO2 Ũ��:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[14][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][17]=='9') //��NO����
		{
			strcpy((char *)&strbuf[0],(char*)" NO Ũ��:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[15][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][18]=='9') //��NO2����,����NOX
		{
			strcpy((char *)&strbuf[0],(char*)"NO2 Ũ��:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[16][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 

			
		}

		if(SdFile_GetData()->rdWorkFile.Other[20][19]=='9') //��CO����
		{
			strcpy((char *)&strbuf[0],(char*)" CO Ũ��:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[17][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][20]=='9') //��H2S����
		{
			strcpy((char *)&strbuf[0],(char*)"H2S Ũ��:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[18][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][21]=='9') //��CO2����
		{
			strcpy((char *)&strbuf[0],(char*)"CO2 Ũ��:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[19][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		
		  //����������
		
		if(SdFile_GetData()->rdWorkFile.Other[20][16]=='9') //��SO2����
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][17]=='9') //��NO����
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][18]=='9') //��NO2����,����NOX
		{
			
		}

		if(SdFile_GetData()->rdWorkFile.Other[20][19]=='9') //��CO����
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][20]=='9') //��H2S����
		{
			
		}

		 //�������ŷ�
	
		if(SdFile_GetData()->rdWorkFile.Other[20][16]=='9') //��SO2����
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][17]=='9') //��NO����
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][18]=='9') //��NO2����,����NOX
		{
			
		}

		if(SdFile_GetData()->rdWorkFile.Other[20][19]=='9') //��CO����
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][20]=='9') //��H2S����
		{
			
		}

		//��ʣϵ��
		//����ϵ��
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[9][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//����ϵ��
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[10][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//ƽ����ѹ
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[0][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//ƽ����ѹ
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[1][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//ƽ������
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[3][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//ƽ������
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[4][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//��ʪ��
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[7][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//�̵�����
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[6][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//��������
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[11][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//�������
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[12][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//��
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"--------------------------------",1);
		mFlushBufferToDisk( 0 );
		//��վ
		strcpy((char *)&strbuf[0],"------ ");
		strcat((char *)&strbuf[0],(char *)COMPANY_WEBSITE);
		strcat((char *)&strbuf[0]," ---");
    Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//�绰
		strcpy((char *)&strbuf[0],"------ Tel: ");
		strcat((char *)&strbuf[0],(char *)COMPANY_TELEPHON);
		strcat((char *)&strbuf[0]," ------");
    Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//������
		strcpy((char *)&strbuf[0]," ");
    Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		strcpy((char *)&strbuf[0]," ");
    Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		
	//����������	*****************************
  for(i=0;i<SdFile_GetData()->GasRecordMax;i++)   
   {
		 
		 SdFile_ReadGasRecord(start_file,i+1);   //����i+1����¼����	

		//��������
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"��������:  ",0);
		 mFlushBufferToDisk( 0 );
		 sprintf((char *)&strbuf[0],"%02u",(i+1));
		 strbuf[2]=0;
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);
		 mFlushBufferToDisk( 0 );
		//����
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.G_record.stru.rowdata[0][0],(Record_row_length-4));
		strbuf[Record_row_length-4]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
	 //�����Ƿ�������
		if(SdFile_GetData()->rdWorkFile.Other[20][15]=='9')  //��O2����
		{
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"O2  :",0);
		 mFlushBufferToDisk( 0 );
		 strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.G_record.stru.rowdata[1][0],(Record_row_length));
		 strbuf[Record_row_length]=0;
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		 mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][16]=='9') //��SO2����
		{
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"SO2 :",0);
		 mFlushBufferToDisk( 0 );
			strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.G_record.stru.rowdata[2][0],(Record_row_length));
		 strbuf[Record_row_length]=0;
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		 mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][17]=='9') //��NO����
		{
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"NO  :",0);
		 mFlushBufferToDisk( 0 );
		 strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.G_record.stru.rowdata[3][0],(Record_row_length));
		 strbuf[Record_row_length]=0;
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		 mFlushBufferToDisk( 0 ); 	
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][18]=='9') //��NO2����,
		{
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"NO2 :",0);
		 mFlushBufferToDisk( 0 );
			strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.G_record.stru.rowdata[4][0],(Record_row_length));
		 strbuf[Record_row_length]=0;
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		 mFlushBufferToDisk( 0 ); 
			
		}

		if(SdFile_GetData()->rdWorkFile.Other[20][19]=='9') //��CO����
		{
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"CO  :",0);
		 mFlushBufferToDisk( 0 );
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.G_record.stru.rowdata[5][0],(Record_row_length));
		 strbuf[Record_row_length]=0;
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		 mFlushBufferToDisk( 0 ); 	
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][20]=='9') //��H2S����
		{
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"H2O :",0);
		 mFlushBufferToDisk( 0 );
			strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.G_record.stru.rowdata[6][0],(Record_row_length));
		 strbuf[Record_row_length]=0;
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		 mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][21]=='9') //��CO2����
		{
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"CO2 :",0);
		 mFlushBufferToDisk( 0 );
		 strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.G_record.stru.rowdata[7][0],(Record_row_length));
		 strbuf[Record_row_length]=0;
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		 mFlushBufferToDisk( 0 ); 	
		}
		
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"------------------------------",1);
		mFlushBufferToDisk( 1);
		
	 }
	
	
	}
	
}
/****************************************************************************
	��������Usb_DustFileOut_TXT(u32 start_file,u32 end_file)
	��  ��:start_file ����ʼ�ļ���
         end_file��   �����ļ���
         usb_out_start_file,usb_out_end_file
	��  ��:
	����˵��: //���������̳��ļ�����

*/
void Usb_DustFileOut_TXT(u32 start_file,u32 end_file) //���������̳��ļ�����
{
	
  u8 strbuf[100];
	u8 bufdata[50];
	//����SD����ȡ�ļ�
 
	SdFile_ReadDustFile(start_file);        //��ȡ1���ļ��ļ�ͷ��1����¼
  Usb_Data.usb_out_file_step = DF_FILE_HEAD_LILE;
	if(Usb_Data.usb_out_file_step == DF_FILE_HEAD_LILE)           //�ļ�ͷ���ַ�
	{
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"  YLB-3330�͵�Ũ���Զ��̳������ۺϲ����ǲ�������",1); //���ӻس�����
		mFlushBufferToDisk( 0 );  // �Զ�ˢ�»�����,��黺�����Ƿ�����,����д��
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"---------���������ļ�---------",1);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"����汾��:   ",0);
		mFlushBufferToDisk( 0 );
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)YLB3330_SOFE_VERSION,1);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"�������  :3330A",0);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.bianhao[8]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&Usb_Data.bianhao[0],1);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"�� �� ��  :       ",0);
		mFlushBufferToDisk( 0 );
		
		SPutl(0x06,(u8*)&strbuf[0],start_file); //���ļ���ת����6Ϊ����
		strbuf[6]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"--------------------------------",1);
		mFlushBufferToDisk( 0 );
		Usb_Data.usb_out_file_step = DF_FILE_CONTENT;
		
	}
	if(Usb_Data.usb_out_file_step == DF_FILE_CONTENT)//�ļ��м�
	{
		 //���ݲ���ʱ��
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"����ʱ�䣺",0);   
		mFlushBufferToDisk( 0 ); 
   	    strncpy((char  *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.dat[0],Other_length);   //��ȡ��ȡ��ʱ��ŵ��ӿ����ݽṹ��ʱ����
		strbuf[Other_length]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],0);   
		mFlushBufferToDisk( 0 );  
		 //���ݲ�����ַ
		strncpy((char  *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Adress[0],Adress_length);
		strbuf[Adress_length]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],0);   
		mFlushBufferToDisk( 0 ); 
		//��Ʒ���
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[28][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//��
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"--------------------------------",1);
		mFlushBufferToDisk( 0 );
		//������
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[30][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//ƽ����ѹ
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[0][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//ƽ����ѹ
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[1][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//ƽ��ȫѹ
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[2][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//ƽ������
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[3][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//ƽ������
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[4][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//����ѹ
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[5][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//�̵�����
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[6][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//��ʪ��
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[7][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//Ƥ��ϵ��
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[8][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//����ϵ��
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[9][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//����ϵ��
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[10][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//������
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[13][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//����ֱ��
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[14][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//ƽ����ѹ
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[15][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//ƽ������
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[16][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//�ۼƲ�ʱ
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[29][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//�������
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[17][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//������
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[18][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//�̳�����
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[19][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//�̳�Ũ��
		//�̳�����
		//�̳��ŷ�
		
		//��������
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[11][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//�������
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[12][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//Ũ��
		if(SdFile_GetData()->rdWorkFile.Other[27][15]=='9')  //��O2����
		{
			strcpy((char *)&strbuf[0],(char*)" O2 Ũ��:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[20][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][16]=='9') //��SO2����
		{
			
			strcpy((char *)&strbuf[0],(char*)"SO2 Ũ��:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[21][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][17]=='9') //��NO����
		{
			strcpy((char *)&strbuf[0],(char*)" NO Ũ��:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[22][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][18]=='9') //��NO2����,����NOX
		{
			strcpy((char *)&strbuf[0],(char*)"NO2 Ũ��:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[23][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 

		}

		if(SdFile_GetData()->rdWorkFile.Other[27][19]=='9') //��CO����
		{
			strcpy((char *)&strbuf[0],(char*)" CO Ũ��:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[24][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][20]=='9') //��H2S����
		{
			strcpy((char *)&strbuf[0],(char*)"H2S Ũ��:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[25][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][21]=='9') //��CO2����
		{
			strcpy((char *)&strbuf[0],(char*)"CO2 Ũ��:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[26][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		//����
		if(SdFile_GetData()->rdWorkFile.Other[27][15]=='9')  //��O2����
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][16]=='9') //��SO2����
		{
			
		
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][17]=='9') //��NO����
		{
			
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][18]=='9') //��NO2����,����NOX
		{
		

		}

		if(SdFile_GetData()->rdWorkFile.Other[27][19]=='9') //��CO����
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][20]=='9') //��H2S����
		{
			
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][21]=='9') //��CO2����
		{
			
			
		}
		//�ŷ�
		if(SdFile_GetData()->rdWorkFile.Other[27][15]=='9')  //��O2����
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][16]=='9') //��SO2����
		{
			
		
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][17]=='9') //��NO����
		{
			
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][18]=='9') //��NO2����,����NOX
		{
		

		}

		if(SdFile_GetData()->rdWorkFile.Other[27][19]=='9') //��CO����
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][20]=='9') //��H2S����
		{
			
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][21]=='9') //��CO2����
		{
			
			
		}
		
		//��
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"--------------------------------",1);
		mFlushBufferToDisk( 0 );
		//��վ
		strcpy((char *)&strbuf[0],"------ ");
		strcat((char *)&strbuf[0],(char *)COMPANY_WEBSITE);
		strcat((char *)&strbuf[0]," ---");
    Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//�绰
		strcpy((char *)&strbuf[0],"------ Tel: ");
		strcat((char *)&strbuf[0],(char *)COMPANY_TELEPHON);
		strcat((char *)&strbuf[0]," ------");
    Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 1 ); 

	}
	
}

/****************************************************************************
	��������Usb_OutGasFileTask()
	��  ��:
         
         
	��  ��:
	����˵��:���������ļ�����

*/
void Usb_OutGasFileTask(void)
{
	u8 Err;
	u8 cbuf[100];
	u8 databuf[20];
	FRESULT res;
	DIR   dir;     /* �ļ��ж��� */ //36  bytes  
	u32 file_tmp;
	
	switch(Usb_Data.USB_OUT_Gas.step)
	{
		case DF_USB_IDLE:   //usb �������� ���ݲ��裺����
		{ 
			break;            //�����ڿ���״̬�ͷ���
		}
		case DF_USB_INIT:  //�ⲿ������ʼ
		{
			Usb_Data.testflg=0;
			Usb_Data.File_cnt=0;
			Usb_Data.USB_OUT_Gas.step = DF_USB_CONNECT;  //��������״̬
			if(Usb_Data.usb_out_start_file > Usb_Data.usb_out_end_file)//�����ļ���
			   {
				   file_tmp = Usb_Data.usb_out_end_file;
				   Usb_Data.usb_out_end_file = Usb_Data.usb_out_start_file;
				   Usb_Data.usb_out_start_file = file_tmp;
			   }
			printf("DF_USB_CONNECT");
			break;
		 }
	 
		case DF_USB_CONNECT:    //������״̬�£�
		{
		//last_wnd = wnd_struct.current;           ���浱ǰҳ��
			if(USH_User_App_flag != DF_USH_USER_APP_OK)//��ѯ�Ƿ����U��
			{
				Usb_Data.USB_OUT_Gas.step = DF_USB_CONNECT_ERR;  //����û���ӣ�����û���Ӵ���
				
				Usb_Data.upstatus=6;
            //Dis_Tips(Screen_GetData(),(u8 *)"δ����U��",0x3B20);
			//�л���ʾ����U��ͼƬ��2��
			//set_refresh_wnd_num(DF_PIC_INSERT_DISK);
				Usb_Data.USB_OUT_Gas.Timer=2000;//��ʱ2��,��ʼ��USB���붨ʱ��
				
			}
			else      //��鵽��U��
			{
				Usb_Data.USB_OUT_Gas.step = DF_USB_CREAT_FILE;         //��u�̾�ת�������ļ��׶�
				Usb_Data.upstatus=2;

			}
		  break;
		}
		case DF_USB_CREAT_FILE:    //��״̬Ϊ�����ļ��н׶�
		{ 
			printf("DF_USB_CREAT_FILE");
			res = f_opendir(&dir,(const TCHAR*)&Usb_FILE_DIR[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir(( TCHAR*)&Usb_FILE_DIR[0]);     //������·�� 
				delay_ms(20);                                //��ʱ
			}

			strcpy((char *)cbuf,(const char *)&Usb_FILE_DIR[0]);
			strcat((char *)cbuf,(char*)"/"); // ���������ļ���

			Usb_Data.bianhao[8] = 0;
			strcat((char *)cbuf,(char*)&Usb_Data.bianhao[0]); // ���ӱ��

			res = f_opendir(&dir,(const TCHAR*)&cbuf[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir((char *)&cbuf[0]);     //������·�� 
				delay_ms(20);                        //��ʱ
			}


			strcat((char *)cbuf,(char*)"/"); // ���������ļ���
			strcat((char *)cbuf,(char*)&Usb_FILE_OF_Gas[0]); // ���������ļ���"DustFile"

			res = f_opendir(&dir,(const TCHAR*)&cbuf[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir((char *)&cbuf[0]);     //������·�� 
				delay_ms(20);                        //��ʱ
			}

									 
			strcat((char *)cbuf,(const char *)"/");
			Usb_Data.dat[8]=0;
			strcat((char *)cbuf,(char*)&Usb_Data.dat[0]); // ��������,�磺20191013

			res = f_opendir(&dir,(const TCHAR*)&cbuf[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir((char *)&cbuf[0]);     //������·�� 
				delay_ms(20);                        //��ʱ
			}

			printf("mkdir %s ;err is%d",(char *)&cbuf[0],Err);
			delay_ms(20);                                   //��ʱ


			Usb_Data.USB_OUT_Gas.step = DF_USB_ONE_FILE;

			break;
		}
		case DF_USB_ONE_FILE:        //���������ļ��뵼��
		{
			//��ʾ���ڵ������ļ���
			Usb_Data.upstatus=3;  //��ʾ�ļ���
			f_mount(fs[2],"2:",1); 
			/**********************************/
			strcpy((char *)cbuf,(const char *)&Usb_FILE_DIR[0]);
			strcat((char *)cbuf,(char*)"/"); // ���������ļ���

			Usb_Data.bianhao[8] = 0;
			strcat((char *)cbuf,(char*)&Usb_Data.bianhao[0]); // ���ӱ��
			strcat((char *)cbuf,(char*)"/"); // ���������ļ���
			strcat((char *)cbuf,(char*)&Usb_FILE_OF_Gas[0]); // ���������ļ���"GasFile"

			strcat((char *)cbuf,(const char *)"/");
			Usb_Data.dat[8]=0;
			strcat((char *)cbuf,(char*)&Usb_Data.dat[0]); // ��������,�磺20191013

			strcat((char *)cbuf,(const char *)"/");
			SPutl(0x06,(u8*)&databuf[0],Usb_Data.usb_out_start_file); //���ļ���ת����8Ϊ����
			databuf[6]=0;
			strcat((char *)cbuf,(char*)databuf);  
			strcat((char *)cbuf,".txt");            //����������ļ�

			Err = f_open(file_usb,(const TCHAR *)cbuf,FA_CREATE_ALWAYS|FA_WRITE|FA_READ);   //�򿪻򴴽��ļ�
			printf("���彨�����ļ�����%s , Rrr=%d",(char *)cbuf,Err);
			
			/**********************************/
			if(Err != FR_OK)
			{
				Usb_Data.USB_OUT_Gas.step = DF_UDISK_ERR;
				//�л���������ͼƬ��2��
				//set_refresh_wnd_num(DF_PIC_UDISK_ERR);
				Usb_Data.upstatus=1;
				Usb_Data.USB_OUT_Gas.Timer=2000;//��ʱ2��
			}
			else
			{
				f_lseek(file_usb,0);		
				Usb_Data.total = 0;   //д������ָ���ʼ
				Usb_GasFileOut_TXT(Usb_Data.usb_out_start_file,Usb_Data.usb_out_end_file);    //���������ļ�
				Err=f_close(file_usb);
				delay_ms(20); 	
				Usb_Data.usb_out_start_file++;
				if(Usb_Data.usb_out_start_file > Usb_Data.usb_out_end_file)	//������ϻ�γ�U�̻򵼳�����
				{
					Usb_Data.USB_OUT_Gas.step = DF_USB_OUT_SUCCESS;
					Usb_Data.upstatus=4;   //��ʾ�ɹ�ҳ
					Usb_Data.USB_OUT_Gas.Timer=2000;//��ʱ2��
				}
			}
			break;
		}
		
		case DF_USB_OUT_SUCCESS:  //��������ʱ2�����������״̬
		{
			if(Usb_Data.USB_OUT_Gas.Timer== 0)
			{
				Usb_Data.USB_OUT_Gas.step = DF_USB_IDLE;
				Usb_Data.testflg=1;
				Usb_Data.upstatus=5; //�л���ԭʼ����

				//set_refresh_wnd_num(last_wnd);
			}
			break;
		}
		case DF_USB_CONNECT_ERR:
		case DF_UDISK_ERR:	 
		//������
		{
			if(Usb_Data.USB_OUT_Gas.Timer==0)      //������붨ʱ��û��
			{
				Usb_Data.upstatus=5; //�л���ԭʼ����
				Usb_Data.testflg=1;
				Usb_Data.USB_OUT_Gas.step = DF_USB_IDLE;   //ת������״̬
			}
			break;
		}
	}
	
}
/****************************************************************************
	��������Usb_OutDustFileTask()
	��  ��:
         
         
	��  ��:
	����˵��:�����̳��ļ�����

*/
void	Usb_OutDustFileTask(void)
{
	u8 Err;
	u8 cbuf[100];
	u8 databuf[20];
	FRESULT res;
	DIR   dir;     /* �ļ��ж��� */ //36  bytes  
	u32 file_tmp;
	
	switch(Usb_Data.USB_OUT_Dust.step)
	{
		case DF_USB_IDLE:   //usb �������� ���ݲ��裺����
		{ 
			break;            //�����ڿ���״̬�ͷ���
		}
		case DF_USB_INIT:  //�ⲿ������ʼ
		{
			Usb_Data.testflg=0;
			Usb_Data.File_cnt=0;
			Usb_Data.USB_OUT_Dust.step = DF_USB_CONNECT;  //��������״̬
			if(Usb_Data.usb_out_start_file > Usb_Data.usb_out_end_file)//�����ļ���
			{
				file_tmp = Usb_Data.usb_out_end_file;
				Usb_Data.usb_out_end_file = Usb_Data.usb_out_start_file;
				Usb_Data.usb_out_start_file = file_tmp;
			}
			printf("DF_USB_CONNECT");
			break;
		}
	 
		case DF_USB_CONNECT:    //������״̬�£�
		{
			//last_wnd = wnd_struct.current;           ���浱ǰҳ��
			if(USH_User_App_flag != DF_USH_USER_APP_OK)//��ѯ�Ƿ����U��
			{
				Usb_Data.USB_OUT_Dust.step = DF_USB_CONNECT_ERR;  //����û���ӣ�����û���Ӵ���
				
				Usb_Data.upstatus=6;
				//�л���ʾ����U��ͼƬ��2��
				//set_refresh_wnd_num(DF_PIC_INSERT_DISK);
				Usb_Data.USB_OUT_Dust.Timer=2000;//��ʱ2��,��ʼ��USB���붨ʱ��
				
			}
			else      //��鵽��U��
			{
				Usb_Data.USB_OUT_Dust.step = DF_USB_CREAT_FILE;         //��u�̾�ת�������ļ��׶�
				Usb_Data.upstatus=2;

			}
			break;
		}
		case DF_USB_CREAT_FILE:    //��״̬Ϊ�����ļ��н׶�
		{ 
			printf("DF_USB_CREAT_FILE");
			res = f_opendir(&dir,(const TCHAR*)&Usb_FILE_DIR[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir(( TCHAR*)&Usb_FILE_DIR[0]);     //������·�� 
				delay_ms(20);                                //��ʱ
			}

			strcpy((char *)cbuf,(const char *)&Usb_FILE_DIR[0]);
			strcat((char *)cbuf,(char*)"/"); // ���������ļ���


			Usb_Data.bianhao[8] = 0;
			strcat((char *)cbuf,(char*)&Usb_Data.bianhao[0]); // ���ӱ��

			res = f_opendir(&dir,(const TCHAR*)&cbuf[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir((char *)&cbuf[0]);     //������·�� 
				delay_ms(20);                        //��ʱ
			}
				
			
			strcat((char *)cbuf,(char*)"/"); // ���������ļ���
			strcat((char *)cbuf,(char*)&Usb_FILE_OF_Dust[0]); // ���������ļ���"DustFile"

			res = f_opendir(&dir,(const TCHAR*)&cbuf[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir((char *)&cbuf[0]);     //������·�� 
				delay_ms(20);                        //��ʱ
			}
		
										 
			strcat((char *)cbuf,(const char *)"/");
			Usb_Data.dat[8]=0;
			strcat((char *)cbuf,(char*)&Usb_Data.dat[0]); // ��������,�磺20191013

			res = f_opendir(&dir,(const TCHAR*)&cbuf[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir((char *)&cbuf[0]);     //������·�� 
				delay_ms(20);                        //��ʱ
			}
				
			printf("mkdir %s ;err is%d",(char *)&cbuf[0],Err);
			delay_ms(20);                                   //��ʱ

			Usb_Data.USB_OUT_Dust.step = DF_USB_ONE_FILE;

			break;
		}
		case DF_USB_ONE_FILE:        //���������ļ��뵼��
		{
			//��ʾ���ڵ������ļ���
			Usb_Data.upstatus=3;  //��ʾ�ļ���
			f_mount(fs[2],"2:",1); 
			/**********************************/
			strcpy((char *)cbuf,(const char *)&Usb_FILE_DIR[0]);
			strcat((char *)cbuf,(char*)"/"); // ���������ļ���


			Usb_Data.bianhao[8] = 0;
			strcat((char *)cbuf,(char*)&Usb_Data.bianhao[0]); // ���ӱ��
			strcat((char *)cbuf,(char*)"/"); // ���������ļ���
			strcat((char *)cbuf,(char*)&Usb_FILE_OF_Dust[0]); // ���������ļ���"DustFile"

			strcat((char *)cbuf,(const char *)"/");
			Usb_Data.dat[8]=0;
			strcat((char *)cbuf,(char*)&Usb_Data.dat[0]); // ��������,�磺20191013

			strcat((char *)cbuf,(const char *)"/");
			SPutl(0x06,(u8*)&databuf[0],Usb_Data.usb_out_start_file); //���ļ���ת����8Ϊ����
			databuf[6]=0;
			strcat((char *)cbuf,(char*)databuf);  
			strcat((char *)cbuf,".txt");            //����������ļ�

			Err = f_open(file_usb,(const TCHAR *)cbuf,FA_CREATE_ALWAYS|FA_WRITE|FA_READ);   //�򿪻򴴽��ļ�
			printf("���彨�����ļ�����%s , Rrr=%d",(char *)cbuf,Err);
			
			/**********************************/
			if(Err != FR_OK)
			{
				Usb_Data.USB_OUT_Dust.step = DF_UDISK_ERR;
				//�л���������ͼƬ��2��
				//set_refresh_wnd_num(DF_PIC_UDISK_ERR);
				Usb_Data.upstatus=1;
				Usb_Data.USB_OUT_Dust.Timer=2000;//��ʱ2��
			}
			else
			{
				f_lseek(file_usb,0);		
				Usb_Data.total = 0;   //д������ָ���ʼ
				Usb_DustFileOut_TXT(Usb_Data.usb_out_start_file,Usb_Data.usb_out_end_file);    //���������ļ�
				Err=f_close(file_usb);
				delay_ms(20); 	
				Usb_Data.usb_out_start_file++;
				if(Usb_Data.usb_out_start_file > Usb_Data.usb_out_end_file)	//������ϻ�γ�U�̻򵼳�����
				{
					Usb_Data.USB_OUT_Dust.step = DF_USB_OUT_SUCCESS;
					Usb_Data.upstatus=4;   //��ʾ�ɹ�ҳ
					Usb_Data.USB_OUT_Dust.Timer=2000;//��ʱ2��
				}
			}
			break;
		}
		
		case DF_USB_OUT_SUCCESS:  //��������ʱ2�����������״̬
		{
			if(Usb_Data.USB_OUT_Dust.Timer== 0)
			{
				Usb_Data.USB_OUT_Dust.step = DF_USB_IDLE;
				Usb_Data.testflg=1;
				Usb_Data.upstatus=5; //�л���ԭʼ����
				
				//set_refresh_wnd_num(last_wnd);
			}
			break;
		}
		case DF_USB_CONNECT_ERR:
		case DF_UDISK_ERR:	 
	   //������
		{
			if(Usb_Data.USB_OUT_Dust.Timer==0)      //������붨ʱ��û��
			{
				Usb_Data.upstatus=5; //�л���ԭʼ����
				Usb_Data.testflg=1;
				Usb_Data.USB_OUT_Dust.step = DF_USB_IDLE;   //ת������״̬
			}
			break;
		}
	}
	
}


//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Slave_DebugASCII(u8 *p,u8 len)
	������		�ú���Ϊslave.c�ļ���ASCII�����ļ�
						:000|09|001
*/
void Usb_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:
		{//��ȡ���д�������ʵʱ����ֵ
			Usb_OutGasFileInf(0x01,0x09,"20000101","12345678");  //����1��2�����ļ�				
			break;
		}
		
		case 2:
		{
			Usb_OutDustFileInf(0x01,0x02,"20000101","12345678");  //����1��2�����ļ�	
			break;
		}
		default:
		{
			break;
		}
	}
}



