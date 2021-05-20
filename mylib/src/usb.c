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
//	printf("设备链接成功\r\n");
	return res;
} 

//#include "ZR3500_CONFIG.h"
#define DEF_SECTOR_SIZE  512

const u8 Usb_FILE_DIR[]="2:/YLB-3330A";  //文件目录
const u8 Usb_FILE_OF_Gas[]="GasFile";   //采样文件路径
const u8 Usb_FILE_OF_Dust[]="DustFile";  //丢掉文件路径


Usb_DataStructure Usb_Data;


/*
	函数名：SdFile_DataStruct* SdFile_GetData(void)
	描述：获取返回File.c文件全局变量结构体
*/
Usb_DataStructure* Usb_GetData(void)
{
	return &Usb_Data;
}
/*
	函数名：UsbInit(void)
	描述：初始化Usb结构变量
*/
void Usb_Init(void)
{ 
	//u32 buf,i;
	//---------------------------------------------------
	
	USBH_Init(&USB_OTG_Core,USB_OTG_FS_CORE_ID,&USB_Host,&USBH_MSC_cb,&USR_Callbacks);   //USB初始化
	//---------------------------------------------------
  Usb_Data.testflg=1;
	
}

/****************************************************************************
	函数名：Usb_Timer
	输  入:
        
	输  出:
	功能说明：用户USB自用定时器

*/
void Usb_Timer(void)
{
	if(Usb_Data.USB_OUT_Gas.Timer!=0)
		Usb_Data.USB_OUT_Gas.Timer--;
	if(Usb_Data.USB_OUT_Dust.Timer!=0)
		Usb_Data.USB_OUT_Dust.Timer--;
}

/****************************************************************************
	函数名：Usb_Main(void)
	输  入:
        
	输  出:
	功能说明：USB 监控程序

*/
void Usb_Main(void)
{
	USH_User_App_flag = DF_USH_USER_APP_INVALID;
	USBH_Process(&USB_OTG_Core, &USB_Host);
	Usb_OutGasFileTask();
	Usb_OutDustFileTask();
}
/****************************************************************************
	函数名：mFlushBufferToDisk
	输  入:force=0；检测缓冲区是否已满，若是就写文件
         force=1：不检查缓冲区,直接写文件
	输  出:
	功能说明：把缓冲区的数据直接写文件

*/

void mFlushBufferToDisk(u8 force)
// force = 0 则自动刷新(检查缓冲区中的数据长度,满则写盘,不满则暂时放在缓冲区中), force != 0 则强制刷新(不管缓冲区中的数据有多少都写盘,通常在系统关机前应该强制写盘)
{
	if(force != 0) 
	{  // 强制刷新
		f_write(file_usb,Usb_Data.FILE_DATA_BUF,Usb_Data.total,&bw_usb);
		Usb_Data.total = 0;       //写完缓冲区位置计数要清0
		f_sync(file_usb);         //文件同步更新,保持打开。
	}
	else if(Usb_Data.total >= DEF_SECTOR_SIZE) 
	{  // 缓冲区中的数据快要满了,所以应该先将缓冲区中的原有数据写入U盘
		f_write(file_usb,Usb_Data.FILE_DATA_BUF,Usb_Data.total,&bw_usb);
		Usb_Data.total = 0;   //写完缓冲区位置计数要清
		f_sync(file_usb);     //文件同步更新,保持打开
	}
}
/****************************************************************************
	函数名：my_sprintf
	输  入:*array ：缓冲区当前地址
         *pstr：字符串当前地址
         flag：是否增加回车换行字符,是=1。
	输  出:
	功能说明：将pstr字符串添加到文件缓冲区array，直到512字节

*/

u16 my_sprintf(u8 *array,u8 *pstr,u16 flag)
{
    u16 i = 0;
    while(*pstr)          //字符串还没有达到EOF
    {
        *(array + i) = (*pstr);  //把字符复制到缓冲区当前位置
        i++;
        pstr += 1;                //字符串移到下一位
    }
    if(flag == 1)   //如果参数flag==1,则加上回车；换行
		{
        *(array + i) = '\xd';	   //回车
        *(array + i + 1) = '\xa';	   //换行
        i += 2;
    }
    else
    {
			
    }
    return i;
}

/****************************************************************************
	函数名：Usb_OutGasFileInf(u32 start_file,u32 end_file, u8* sysDate, u8* macNumber)
	输  入:start_file ：开始文件号
         end_file：   结束文件号       
	输  出:
	功能说明:  外部调用接口，导出烟气文件命令
	修改记录：	1. 导出时使用系统日期和设备仪器编号, modify by alfre, 2020.06.04.
*/
void Usb_OutGasFileInf(u32 start_file,u32 end_file, char *sysDate, char *macNumber)  //外部调用接口，导出烟气文件命令
{
	strncpy((char *)&Usb_Data.dat[0],(char *)sysDate,8);   //填上导出日期
	strncpy((char *)&Usb_Data.bianhao[0],(char *)macNumber,8);   //填上导出编号
	Usb_Data.usb_out_start_file=start_file;
	Usb_Data.usb_out_end_file=end_file;
	Usb_Data.USB_OUT_Gas.step=DF_USB_INIT;
}

/****************************************************************************
	函数名：Usb_OutDustFileInf(u32 start_file,u32 end_file, u8* sysDate, u8* macNumber) 
	输  入:start_file ：开始文件号
         end_file：   结束文件号         
	输  出:
	功能说明:  外部调用接口，导出烟尘文件命令
	修改记录：	1. 导出时使用系统日期和设备仪器编号, modify by alfre, 2020.06.04.
*/
void Usb_OutDustFileInf(u32 start_file,u32 end_file,char *sysDate, char *macNumber)  //外部调用接口，导出烟尘文件命令
{
	
	strncpy((char *)&Usb_Data.dat[0],(char *)sysDate,8);   //填上导出日期
	strncpy((char *)&Usb_Data.bianhao[0],(char *)macNumber,8);   //填上导出编号
	Usb_Data.usb_out_start_file=start_file;
	Usb_Data.usb_out_end_file=end_file;
	Usb_Data.USB_OUT_Dust.step=DF_USB_INIT;
	
}
/****************************************************************************
	函数名：Usb_GasFileOut_TXT(u32 start_file,u32 end_file) 
	输  入:start_file ：开始文件号
         end_file：   结束文件号
         usb_out_start_file,usb_out_end_file
	输  出:
	功能说明: //导出具体烟气文件内容

*/
void Usb_GasFileOut_TXT(u32 start_file,u32 end_file)  //导出具体烟气文件内容
{
	u8 i;
  u8 strbuf[100];
	u8 bufdata[50];
	//先在SD卡读取文件
  SdFile_Serch_MaxofGasRecord(start_file);    //读取最大记录先,读之前要确定最大值
	SdFile_ReadGasFile(start_file,0x01);        //读取1号文件文件头与1条记录
  Usb_Data.usb_out_file_step = DF_FILE_HEAD_LILE;
	if(Usb_Data.usb_out_file_step == DF_FILE_HEAD_LILE)           //文件头部字符
	{
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"  YLB-3330型低浓度自动烟尘烟气综合测试仪采样报表",1); //增加回车换行
		mFlushBufferToDisk( 0 );  // 自动刷新缓冲区,检查缓冲区是否已满,满则写盘
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"---------烟气采样文件---------",1);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"软件版本号:   ",0);
		mFlushBufferToDisk( 0 );
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)YLB3330_SOFE_VERSION,1);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"仪器编号  :3330A",0);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.bianhao[8]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&Usb_Data.bianhao[0],1);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"文 件 号  :       ",0);
		mFlushBufferToDisk( 0 );
		SPutl(0x06,(u8*)&strbuf[0],Usb_Data.usb_out_start_file); //把文件名转换成6为数字
		strbuf[6]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);
		mFlushBufferToDisk( 0 );
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"--------------------------------",1);
		mFlushBufferToDisk( 0 );
		Usb_Data.usb_out_file_step = DF_FILE_CONTENT;
		
	}
	if(Usb_Data.usb_out_file_step == DF_FILE_CONTENT)//文件中间
	{
		 //传递采样时间
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"采样时间：",0);   
		mFlushBufferToDisk( 0 ); 
   	strncpy((char  *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.dat[0],Other_length);   //提取读取的时间放到接口数据结构的时间项
		strbuf[Other_length]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],0);   
		mFlushBufferToDisk( 0 );  
		 //传递采样地址
		strncpy((char  *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Adress[0],Adress_length);
		strbuf[Adress_length]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],0);   
		mFlushBufferToDisk( 0 ); 
		//以下是根据"端口配置"来列举
		   //下面是浓度
		if(SdFile_GetData()->rdWorkFile.Other[20][15]=='9')  //有O2数据
		{
			strcpy((char *)&strbuf[0],(char*)" O2 浓度:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[13][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][16]=='9') //有SO2数据
		{
			
			strcpy((char *)&strbuf[0],(char*)"SO2 浓度:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[14][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][17]=='9') //有NO数据
		{
			strcpy((char *)&strbuf[0],(char*)" NO 浓度:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[15][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][18]=='9') //有NO2数据,就有NOX
		{
			strcpy((char *)&strbuf[0],(char*)"NO2 浓度:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[16][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 

			
		}

		if(SdFile_GetData()->rdWorkFile.Other[20][19]=='9') //有CO数据
		{
			strcpy((char *)&strbuf[0],(char*)" CO 浓度:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[17][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][20]=='9') //有H2S数据
		{
			strcpy((char *)&strbuf[0],(char*)"H2S 浓度:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[18][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][21]=='9') //有CO2数据
		{
			strcpy((char *)&strbuf[0],(char*)"CO2 浓度:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[19][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		
		  //下面是折算
		
		if(SdFile_GetData()->rdWorkFile.Other[20][16]=='9') //有SO2数据
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][17]=='9') //有NO数据
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][18]=='9') //有NO2数据,就有NOX
		{
			
		}

		if(SdFile_GetData()->rdWorkFile.Other[20][19]=='9') //有CO数据
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][20]=='9') //有H2S数据
		{
			
		}

		 //下面是排放
	
		if(SdFile_GetData()->rdWorkFile.Other[20][16]=='9') //有SO2数据
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][17]=='9') //有NO数据
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][18]=='9') //有NO2数据,就有NOX
		{
			
		}

		if(SdFile_GetData()->rdWorkFile.Other[20][19]=='9') //有CO数据
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][20]=='9') //有H2S数据
		{
			
		}

		//过剩系数
		//折数系数
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[9][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//负荷系数
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[10][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//平均动压
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[0][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//平均静压
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[1][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//平均流速
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[3][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//平均烟温
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[4][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//含湿量
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[7][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//烟道截面
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[6][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//烟气流量
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[11][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//标杆流量
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[12][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//栏
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"--------------------------------",1);
		mFlushBufferToDisk( 0 );
		//网站
		strcpy((char *)&strbuf[0],"------ ");
		strcat((char *)&strbuf[0],(char *)COMPANY_WEBSITE);
		strcat((char *)&strbuf[0]," ---");
    Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//电话
		strcpy((char *)&strbuf[0],"------ Tel: ");
		strcat((char *)&strbuf[0],(char *)COMPANY_TELEPHON);
		strcat((char *)&strbuf[0]," ------");
    Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//空两行
		strcpy((char *)&strbuf[0]," ");
    Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		strcpy((char *)&strbuf[0]," ");
    Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		
	//读分钟数据	*****************************
  for(i=0;i<SdFile_GetData()->GasRecordMax;i++)   
   {
		 
		 SdFile_ReadGasRecord(start_file,i+1);   //读第i+1条记录数据	

		//分钟数据
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"分钟数据:  ",0);
		 mFlushBufferToDisk( 0 );
		 sprintf((char *)&strbuf[0],"%02u",(i+1));
		 strbuf[2]=0;
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);
		 mFlushBufferToDisk( 0 );
		//日期
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.G_record.stru.rowdata[0][0],(Record_row_length-4));
		strbuf[Record_row_length-4]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
	 //下面是分钟数据
		if(SdFile_GetData()->rdWorkFile.Other[20][15]=='9')  //有O2数据
		{
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"O2  :",0);
		 mFlushBufferToDisk( 0 );
		 strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.G_record.stru.rowdata[1][0],(Record_row_length));
		 strbuf[Record_row_length]=0;
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		 mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][16]=='9') //有SO2数据
		{
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"SO2 :",0);
		 mFlushBufferToDisk( 0 );
			strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.G_record.stru.rowdata[2][0],(Record_row_length));
		 strbuf[Record_row_length]=0;
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		 mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][17]=='9') //有NO数据
		{
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"NO  :",0);
		 mFlushBufferToDisk( 0 );
		 strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.G_record.stru.rowdata[3][0],(Record_row_length));
		 strbuf[Record_row_length]=0;
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		 mFlushBufferToDisk( 0 ); 	
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][18]=='9') //有NO2数据,
		{
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"NO2 :",0);
		 mFlushBufferToDisk( 0 );
			strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.G_record.stru.rowdata[4][0],(Record_row_length));
		 strbuf[Record_row_length]=0;
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		 mFlushBufferToDisk( 0 ); 
			
		}

		if(SdFile_GetData()->rdWorkFile.Other[20][19]=='9') //有CO数据
		{
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"CO  :",0);
		 mFlushBufferToDisk( 0 );
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.G_record.stru.rowdata[5][0],(Record_row_length));
		 strbuf[Record_row_length]=0;
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		 mFlushBufferToDisk( 0 ); 	
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][20]=='9') //有H2S数据
		{
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"H2O :",0);
		 mFlushBufferToDisk( 0 );
			strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.G_record.stru.rowdata[6][0],(Record_row_length));
		 strbuf[Record_row_length]=0;
		 Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		 mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[20][21]=='9') //有CO2数据
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
	函数名：Usb_DustFileOut_TXT(u32 start_file,u32 end_file)
	输  入:start_file ：开始文件号
         end_file：   结束文件号
         usb_out_start_file,usb_out_end_file
	输  出:
	功能说明: //导出具体烟尘文件内容

*/
void Usb_DustFileOut_TXT(u32 start_file,u32 end_file) //导出具体烟尘文件内容
{
	
  u8 strbuf[100];
	u8 bufdata[50];
	//先在SD卡读取文件
 
	SdFile_ReadDustFile(start_file);        //读取1号文件文件头与1条记录
  Usb_Data.usb_out_file_step = DF_FILE_HEAD_LILE;
	if(Usb_Data.usb_out_file_step == DF_FILE_HEAD_LILE)           //文件头部字符
	{
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"  YLB-3330型低浓度自动烟尘烟气综合测试仪采样报表",1); //增加回车换行
		mFlushBufferToDisk( 0 );  // 自动刷新缓冲区,检查缓冲区是否已满,满则写盘
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"---------烟气采样文件---------",1);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"软件版本号:   ",0);
		mFlushBufferToDisk( 0 );
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)YLB3330_SOFE_VERSION,1);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"仪器编号  :3330A",0);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.bianhao[8]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&Usb_Data.bianhao[0],1);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"文 件 号  :       ",0);
		mFlushBufferToDisk( 0 );
		
		SPutl(0x06,(u8*)&strbuf[0],start_file); //把文件名转换成6为数字
		strbuf[6]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);
		mFlushBufferToDisk( 0 );
		
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"--------------------------------",1);
		mFlushBufferToDisk( 0 );
		Usb_Data.usb_out_file_step = DF_FILE_CONTENT;
		
	}
	if(Usb_Data.usb_out_file_step == DF_FILE_CONTENT)//文件中间
	{
		 //传递采样时间
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"采样时间：",0);   
		mFlushBufferToDisk( 0 ); 
   	    strncpy((char  *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.dat[0],Other_length);   //提取读取的时间放到接口数据结构的时间项
		strbuf[Other_length]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],0);   
		mFlushBufferToDisk( 0 );  
		 //传递采样地址
		strncpy((char  *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Adress[0],Adress_length);
		strbuf[Adress_length]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],0);   
		mFlushBufferToDisk( 0 ); 
		//样品编号
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[28][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//栏
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"--------------------------------",1);
		mFlushBufferToDisk( 0 );
		//气密性
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[30][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//平均动压
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[0][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//平均静压
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[1][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//平均全压
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[2][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//平均流速
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[3][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//平均烟温
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[4][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//大气压
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[5][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//烟道截面
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[6][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//含湿量
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[7][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//皮管系数
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[8][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//折算系数
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[9][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//负荷系数
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[10][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//跟踪率
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[13][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//采嘴直径
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[14][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//平均计压
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[15][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//平均计温
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[16][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//累计采时
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[29][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//采样体积
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[17][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//标况体积
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[18][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//烟尘净重
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[19][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//烟尘浓度
		//烟尘折算
		//烟尘排放
		
		//烟气流量
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[11][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//标杆流量
		strncpy((char *)&strbuf[0],(char *)&SdFile_GetData()->rdWorkFile.Other[12][0],(Other_length-2));
		strbuf[Other_length-2]=0;
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//浓度
		if(SdFile_GetData()->rdWorkFile.Other[27][15]=='9')  //有O2数据
		{
			strcpy((char *)&strbuf[0],(char*)" O2 浓度:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[20][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][16]=='9') //有SO2数据
		{
			
			strcpy((char *)&strbuf[0],(char*)"SO2 浓度:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[21][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][17]=='9') //有NO数据
		{
			strcpy((char *)&strbuf[0],(char*)" NO 浓度:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[22][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][18]=='9') //有NO2数据,就有NOX
		{
			strcpy((char *)&strbuf[0],(char*)"NO2 浓度:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[23][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 

		}

		if(SdFile_GetData()->rdWorkFile.Other[27][19]=='9') //有CO数据
		{
			strcpy((char *)&strbuf[0],(char*)" CO 浓度:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[24][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][20]=='9') //有H2S数据
		{
			strcpy((char *)&strbuf[0],(char*)"H2S 浓度:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[25][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][21]=='9') //有CO2数据
		{
			strcpy((char *)&strbuf[0],(char*)"CO2 浓度:");
			strncpy((char *)&bufdata[0],(char *)&SdFile_GetData()->rdWorkFile.Other[26][9],Other_length-11);
			bufdata[Other_length-11]=0;
			strcat((char *)&strbuf[0],(char *)&bufdata[0]);
			Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		  mFlushBufferToDisk( 0 ); 
			
		}
		//折算
		if(SdFile_GetData()->rdWorkFile.Other[27][15]=='9')  //有O2数据
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][16]=='9') //有SO2数据
		{
			
		
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][17]=='9') //有NO数据
		{
			
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][18]=='9') //有NO2数据,就有NOX
		{
		

		}

		if(SdFile_GetData()->rdWorkFile.Other[27][19]=='9') //有CO数据
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][20]=='9') //有H2S数据
		{
			
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][21]=='9') //有CO2数据
		{
			
			
		}
		//排放
		if(SdFile_GetData()->rdWorkFile.Other[27][15]=='9')  //有O2数据
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][16]=='9') //有SO2数据
		{
			
		
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][17]=='9') //有NO数据
		{
			
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][18]=='9') //有NO2数据,就有NOX
		{
		

		}

		if(SdFile_GetData()->rdWorkFile.Other[27][19]=='9') //有CO数据
		{
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][20]=='9') //有H2S数据
		{
			
			
		}
		if(SdFile_GetData()->rdWorkFile.Other[27][21]=='9') //有CO2数据
		{
			
			
		}
		
		//栏
		Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)"--------------------------------",1);
		mFlushBufferToDisk( 0 );
		//网站
		strcpy((char *)&strbuf[0],"------ ");
		strcat((char *)&strbuf[0],(char *)COMPANY_WEBSITE);
		strcat((char *)&strbuf[0]," ---");
    Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 0 ); 
		//电话
		strcpy((char *)&strbuf[0],"------ Tel: ");
		strcat((char *)&strbuf[0],(char *)COMPANY_TELEPHON);
		strcat((char *)&strbuf[0]," ------");
    Usb_Data.total += my_sprintf(Usb_Data.FILE_DATA_BUF+Usb_Data.total,(u8 *)&strbuf[0],1);   
		mFlushBufferToDisk( 1 ); 

	}
	
}

/****************************************************************************
	函数名：Usb_OutGasFileTask()
	输  入:
         
         
	输  出:
	功能说明:导出烟气文件任务

*/
void Usb_OutGasFileTask(void)
{
	u8 Err;
	u8 cbuf[100];
	u8 databuf[20];
	FRESULT res;
	DIR   dir;     /* 文件夹对象 */ //36  bytes  
	u32 file_tmp;
	
	switch(Usb_Data.USB_OUT_Gas.step)
	{
		case DF_USB_IDLE:   //usb 导出采样 数据步骤：挂起
		{ 
			break;            //任务在空闲状态就返回
		}
		case DF_USB_INIT:  //外部启动初始
		{
			Usb_Data.testflg=0;
			Usb_Data.File_cnt=0;
			Usb_Data.USB_OUT_Gas.step = DF_USB_CONNECT;  //进入链接状态
			if(Usb_Data.usb_out_start_file > Usb_Data.usb_out_end_file)//调换文件号
			   {
				   file_tmp = Usb_Data.usb_out_end_file;
				   Usb_Data.usb_out_end_file = Usb_Data.usb_out_start_file;
				   Usb_Data.usb_out_start_file = file_tmp;
			   }
			printf("DF_USB_CONNECT");
			break;
		 }
	 
		case DF_USB_CONNECT:    //在链接状态下，
		{
		//last_wnd = wnd_struct.current;           保存当前页面
			if(USH_User_App_flag != DF_USH_USER_APP_OK)//查询是否插入U盘
			{
				Usb_Data.USB_OUT_Gas.step = DF_USB_CONNECT_ERR;  //优盘没链接，跳到没链接处理
				
				Usb_Data.upstatus=6;
            //Dis_Tips(Screen_GetData(),(u8 *)"未插入U盘",0x3B20);
			//切换显示插入U盘图片，2秒
			//set_refresh_wnd_num(DF_PIC_INSERT_DISK);
				Usb_Data.USB_OUT_Gas.Timer=2000;//定时2秒,初始化USB两秒定时器
				
			}
			else      //检查到有U盘
			{
				Usb_Data.USB_OUT_Gas.step = DF_USB_CREAT_FILE;         //有u盘就转到建立文件阶段
				Usb_Data.upstatus=2;

			}
		  break;
		}
		case DF_USB_CREAT_FILE:    //本状态为建立文件夹阶段
		{ 
			printf("DF_USB_CREAT_FILE");
			res = f_opendir(&dir,(const TCHAR*)&Usb_FILE_DIR[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir(( TCHAR*)&Usb_FILE_DIR[0]);     //创建根路径 
				delay_ms(20);                                //延时
			}

			strcpy((char *)cbuf,(const char *)&Usb_FILE_DIR[0]);
			strcat((char *)cbuf,(char*)"/"); // 连接烟气文件夹

			Usb_Data.bianhao[8] = 0;
			strcat((char *)cbuf,(char*)&Usb_Data.bianhao[0]); // 连接编号

			res = f_opendir(&dir,(const TCHAR*)&cbuf[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir((char *)&cbuf[0]);     //创建根路径 
				delay_ms(20);                        //延时
			}


			strcat((char *)cbuf,(char*)"/"); // 连接烟气文件夹
			strcat((char *)cbuf,(char*)&Usb_FILE_OF_Gas[0]); // 连接烟气文件夹"DustFile"

			res = f_opendir(&dir,(const TCHAR*)&cbuf[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir((char *)&cbuf[0]);     //创建根路径 
				delay_ms(20);                        //延时
			}

									 
			strcat((char *)cbuf,(const char *)"/");
			Usb_Data.dat[8]=0;
			strcat((char *)cbuf,(char*)&Usb_Data.dat[0]); // 导出日期,如：20191013

			res = f_opendir(&dir,(const TCHAR*)&cbuf[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir((char *)&cbuf[0]);     //创建根路径 
				delay_ms(20);                        //延时
			}

			printf("mkdir %s ;err is%d",(char *)&cbuf[0],Err);
			delay_ms(20);                                   //延时


			Usb_Data.USB_OUT_Gas.step = DF_USB_ONE_FILE;

			break;
		}
		case DF_USB_ONE_FILE:        //创建具体文件与导出
		{
			//显示正在导出的文件号
			Usb_Data.upstatus=3;  //显示文件号
			f_mount(fs[2],"2:",1); 
			/**********************************/
			strcpy((char *)cbuf,(const char *)&Usb_FILE_DIR[0]);
			strcat((char *)cbuf,(char*)"/"); // 连接烟气文件夹

			Usb_Data.bianhao[8] = 0;
			strcat((char *)cbuf,(char*)&Usb_Data.bianhao[0]); // 连接编号
			strcat((char *)cbuf,(char*)"/"); // 连接烟气文件夹
			strcat((char *)cbuf,(char*)&Usb_FILE_OF_Gas[0]); // 连接烟气文件夹"GasFile"

			strcat((char *)cbuf,(const char *)"/");
			Usb_Data.dat[8]=0;
			strcat((char *)cbuf,(char*)&Usb_Data.dat[0]); // 导出日期,如：20191013

			strcat((char *)cbuf,(const char *)"/");
			SPutl(0x06,(u8*)&databuf[0],Usb_Data.usb_out_start_file); //把文件名转换成8为数字
			databuf[6]=0;
			strcat((char *)cbuf,(char*)databuf);  
			strcat((char *)cbuf,".txt");            //产生具体的文件

			Err = f_open(file_usb,(const TCHAR *)cbuf,FA_CREATE_ALWAYS|FA_WRITE|FA_READ);   //打开或创建文件
			printf("具体建立的文件名：%s , Rrr=%d",(char *)cbuf,Err);
			
			/**********************************/
			if(Err != FR_OK)
			{
				Usb_Data.USB_OUT_Gas.step = DF_UDISK_ERR;
				//切换导出错误图片，2秒
				//set_refresh_wnd_num(DF_PIC_UDISK_ERR);
				Usb_Data.upstatus=1;
				Usb_Data.USB_OUT_Gas.Timer=2000;//定时2秒
			}
			else
			{
				f_lseek(file_usb,0);		
				Usb_Data.total = 0;   //写缓冲区指针初始
				Usb_GasFileOut_TXT(Usb_Data.usb_out_start_file,Usb_Data.usb_out_end_file);    //导出采样文件
				Err=f_close(file_usb);
				delay_ms(20); 	
				Usb_Data.usb_out_start_file++;
				if(Usb_Data.usb_out_start_file > Usb_Data.usb_out_end_file)	//导出完毕或拔出U盘或导出错误
				{
					Usb_Data.USB_OUT_Gas.step = DF_USB_OUT_SUCCESS;
					Usb_Data.upstatus=4;   //显示成功页
					Usb_Data.USB_OUT_Gas.Timer=2000;//定时2秒
				}
			}
			break;
		}
		
		case DF_USB_OUT_SUCCESS:  //导出后延时2两秒后进入空闲状态
		{
			if(Usb_Data.USB_OUT_Gas.Timer== 0)
			{
				Usb_Data.USB_OUT_Gas.step = DF_USB_IDLE;
				Usb_Data.testflg=1;
				Usb_Data.upstatus=5; //切换到原始界面

				//set_refresh_wnd_num(last_wnd);
			}
			break;
		}
		case DF_USB_CONNECT_ERR:
		case DF_UDISK_ERR:	 
		//错误处理
		{
			if(Usb_Data.USB_OUT_Gas.Timer==0)      //检查两秒定时到没有
			{
				Usb_Data.upstatus=5; //切换到原始界面
				Usb_Data.testflg=1;
				Usb_Data.USB_OUT_Gas.step = DF_USB_IDLE;   //转到空闲状态
			}
			break;
		}
	}
	
}
/****************************************************************************
	函数名：Usb_OutDustFileTask()
	输  入:
         
         
	输  出:
	功能说明:导出烟尘文件任务

*/
void	Usb_OutDustFileTask(void)
{
	u8 Err;
	u8 cbuf[100];
	u8 databuf[20];
	FRESULT res;
	DIR   dir;     /* 文件夹对象 */ //36  bytes  
	u32 file_tmp;
	
	switch(Usb_Data.USB_OUT_Dust.step)
	{
		case DF_USB_IDLE:   //usb 导出采样 数据步骤：挂起
		{ 
			break;            //任务在空闲状态就返回
		}
		case DF_USB_INIT:  //外部启动初始
		{
			Usb_Data.testflg=0;
			Usb_Data.File_cnt=0;
			Usb_Data.USB_OUT_Dust.step = DF_USB_CONNECT;  //进入链接状态
			if(Usb_Data.usb_out_start_file > Usb_Data.usb_out_end_file)//调换文件号
			{
				file_tmp = Usb_Data.usb_out_end_file;
				Usb_Data.usb_out_end_file = Usb_Data.usb_out_start_file;
				Usb_Data.usb_out_start_file = file_tmp;
			}
			printf("DF_USB_CONNECT");
			break;
		}
	 
		case DF_USB_CONNECT:    //在链接状态下，
		{
			//last_wnd = wnd_struct.current;           保存当前页面
			if(USH_User_App_flag != DF_USH_USER_APP_OK)//查询是否插入U盘
			{
				Usb_Data.USB_OUT_Dust.step = DF_USB_CONNECT_ERR;  //优盘没链接，跳到没链接处理
				
				Usb_Data.upstatus=6;
				//切换显示插入U盘图片，2秒
				//set_refresh_wnd_num(DF_PIC_INSERT_DISK);
				Usb_Data.USB_OUT_Dust.Timer=2000;//定时2秒,初始化USB两秒定时器
				
			}
			else      //检查到有U盘
			{
				Usb_Data.USB_OUT_Dust.step = DF_USB_CREAT_FILE;         //有u盘就转到建立文件阶段
				Usb_Data.upstatus=2;

			}
			break;
		}
		case DF_USB_CREAT_FILE:    //本状态为建立文件夹阶段
		{ 
			printf("DF_USB_CREAT_FILE");
			res = f_opendir(&dir,(const TCHAR*)&Usb_FILE_DIR[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir(( TCHAR*)&Usb_FILE_DIR[0]);     //创建根路径 
				delay_ms(20);                                //延时
			}

			strcpy((char *)cbuf,(const char *)&Usb_FILE_DIR[0]);
			strcat((char *)cbuf,(char*)"/"); // 连接烟气文件夹


			Usb_Data.bianhao[8] = 0;
			strcat((char *)cbuf,(char*)&Usb_Data.bianhao[0]); // 连接编号

			res = f_opendir(&dir,(const TCHAR*)&cbuf[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir((char *)&cbuf[0]);     //创建根路径 
				delay_ms(20);                        //延时
			}
				
			
			strcat((char *)cbuf,(char*)"/"); // 连接烟气文件夹
			strcat((char *)cbuf,(char*)&Usb_FILE_OF_Dust[0]); // 连接烟气文件夹"DustFile"

			res = f_opendir(&dir,(const TCHAR*)&cbuf[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir((char *)&cbuf[0]);     //创建根路径 
				delay_ms(20);                        //延时
			}
		
										 
			strcat((char *)cbuf,(const char *)"/");
			Usb_Data.dat[8]=0;
			strcat((char *)cbuf,(char*)&Usb_Data.dat[0]); // 导出日期,如：20191013

			res = f_opendir(&dir,(const TCHAR*)&cbuf[0]);  
			if(res!=FR_OK)
			{
				f_closedir(&dir);
				Err = f_mkdir((char *)&cbuf[0]);     //创建根路径 
				delay_ms(20);                        //延时
			}
				
			printf("mkdir %s ;err is%d",(char *)&cbuf[0],Err);
			delay_ms(20);                                   //延时

			Usb_Data.USB_OUT_Dust.step = DF_USB_ONE_FILE;

			break;
		}
		case DF_USB_ONE_FILE:        //创建具体文件与导出
		{
			//显示正在导出的文件号
			Usb_Data.upstatus=3;  //显示文件号
			f_mount(fs[2],"2:",1); 
			/**********************************/
			strcpy((char *)cbuf,(const char *)&Usb_FILE_DIR[0]);
			strcat((char *)cbuf,(char*)"/"); // 连接烟气文件夹


			Usb_Data.bianhao[8] = 0;
			strcat((char *)cbuf,(char*)&Usb_Data.bianhao[0]); // 连接编号
			strcat((char *)cbuf,(char*)"/"); // 连接烟气文件夹
			strcat((char *)cbuf,(char*)&Usb_FILE_OF_Dust[0]); // 连接烟气文件夹"DustFile"

			strcat((char *)cbuf,(const char *)"/");
			Usb_Data.dat[8]=0;
			strcat((char *)cbuf,(char*)&Usb_Data.dat[0]); // 导出日期,如：20191013

			strcat((char *)cbuf,(const char *)"/");
			SPutl(0x06,(u8*)&databuf[0],Usb_Data.usb_out_start_file); //把文件名转换成8为数字
			databuf[6]=0;
			strcat((char *)cbuf,(char*)databuf);  
			strcat((char *)cbuf,".txt");            //产生具体的文件

			Err = f_open(file_usb,(const TCHAR *)cbuf,FA_CREATE_ALWAYS|FA_WRITE|FA_READ);   //打开或创建文件
			printf("具体建立的文件名：%s , Rrr=%d",(char *)cbuf,Err);
			
			/**********************************/
			if(Err != FR_OK)
			{
				Usb_Data.USB_OUT_Dust.step = DF_UDISK_ERR;
				//切换导出错误图片，2秒
				//set_refresh_wnd_num(DF_PIC_UDISK_ERR);
				Usb_Data.upstatus=1;
				Usb_Data.USB_OUT_Dust.Timer=2000;//定时2秒
			}
			else
			{
				f_lseek(file_usb,0);		
				Usb_Data.total = 0;   //写缓冲区指针初始
				Usb_DustFileOut_TXT(Usb_Data.usb_out_start_file,Usb_Data.usb_out_end_file);    //导出采样文件
				Err=f_close(file_usb);
				delay_ms(20); 	
				Usb_Data.usb_out_start_file++;
				if(Usb_Data.usb_out_start_file > Usb_Data.usb_out_end_file)	//导出完毕或拔出U盘或导出错误
				{
					Usb_Data.USB_OUT_Dust.step = DF_USB_OUT_SUCCESS;
					Usb_Data.upstatus=4;   //显示成功页
					Usb_Data.USB_OUT_Dust.Timer=2000;//定时2秒
				}
			}
			break;
		}
		
		case DF_USB_OUT_SUCCESS:  //导出后延时2两秒后进入空闲状态
		{
			if(Usb_Data.USB_OUT_Dust.Timer== 0)
			{
				Usb_Data.USB_OUT_Dust.step = DF_USB_IDLE;
				Usb_Data.testflg=1;
				Usb_Data.upstatus=5; //切换到原始界面
				
				//set_refresh_wnd_num(last_wnd);
			}
			break;
		}
		case DF_USB_CONNECT_ERR:
		case DF_UDISK_ERR:	 
	   //错误处理
		{
			if(Usb_Data.USB_OUT_Dust.Timer==0)      //检查两秒定时到没有
			{
				Usb_Data.upstatus=5; //切换到原始界面
				Usb_Data.testflg=1;
				Usb_Data.USB_OUT_Dust.step = DF_USB_IDLE;   //转到空闲状态
			}
			break;
		}
	}
	
}


//-------------------------调试函数----------------------------
//---
/*
	函数名： void Slave_DebugASCII(u8 *p,u8 len)
	描述：		该函数为slave.c文件的ASCII调试文件
						:000|09|001
*/
void Usb_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:
		{//读取所有传感器的实时采样值
			Usb_OutGasFileInf(0x01,0x09,"20000101","12345678");  //导出1到2烟气文件				
			break;
		}
		
		case 2:
		{
			Usb_OutDustFileInf(0x01,0x02,"20000101","12345678");  //导出1到2烟气文件	
			break;
		}
		default:
		{
			break;
		}
	}
}



