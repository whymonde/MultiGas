#include "stm32f4xx.h"
#include "sys.h"  
#include "file.h"
#include "ff.h"
#include "exfuns.h"
#include "string.h"
#include "usart.h"
#include "malloc.h" 
//#include "delay.h"
// SD 卡上保存的个路径,与内容
//u32 recont_cnt;

const u8 ROOT_DIR[]="0:YLB3330";            //根目录
//const u8 ROOT_DIR[]="0:YLB";            //根目录

const u8 FOLDER_OF_Dust[]="0:/YLB3330/Dust";   //烟尘文件夹
const u8 FOLDER_OF_Dust00[]="0:/YLB3330/Dust/Dust00";   //烟尘具体文件文件夹
const u8 LINK_OF_Dust[]="0:/YLB3330/Dust/link.txt"; //烟尘索引文件

const u8 FOLDER_OF_Gas[]="0:/YLB3330/Gas";   //烟气文件夹
const u8 FOLDER_OF_Gas00[]="0:/YLB3330/Gas/Gas00";   //烟气具体文件文件夹
const u8 LINK_OF_Gas[]="0:/YLB3330/Gas/link.txt"; //烟气索引文件
const u8 TEMP_OF_Gas[]="0:/YLB3330/Gas/temp0.txt"; //烟气索引文件

const u8 FOLDER_OF_Work[]="0:/YLB3330/Work";   //工况文件夹
const u8 FOLDER_OF_Work00[]="0:/YLB3330/Work/Work00";   //工况具体文件文件夹
const u8 LINK_OF_Work[]="0:/YLB3330/Work/link.txt"; //工况索引文件
/*@lian add 20290302 star*/
const u8 FOLDER_OF_Flue[]="0:/YLB3330/Flue";   //烟道文件夹
const u8 FOLDER_OF_Flue00[]="0:/YLB3330/Flue/Flue00";   //烟道具体文件文件夹
const u8 LINK_OF_Flue[]="0:/YLB3330/Flue/link.txt"; //烟道索引文件
/*@lian End*/
//下面是固定的文件
//工况文件内容
const char Work_line[Work_car_length][Other_length+1]={
"平均动压:              Pa   \r\n",//0 
"平均静压:              kPa  \r\n",//1 
"平均全压:              kPa  \r\n",//2 
"大 气 压:              kPa  \r\n",//3 
"烟道截面:              m2   \r\n",//4 
"采 样 嘴:              mm   \r\n",//5 
"平均烟温:              ℃   \r\n",//6 
"平均流速:              m/s  \r\n",//7 
"烟气流量:              m3/h \r\n",//8 
"标杆流量:              m3/h \r\n",//9 
"含 湿 量:                   \r\n",//10 
"折算系数:                   \r\n",//11 
"负荷系数:                   \r\n",//12 
"皮 托 管:                   \r\n"//13
};

//烟尘文件内容
const char Dust_line[Dust_car_length][Other_length+1]={
"平均动压:              Pa   \r\n",//0
"平均静压:              kPa  \r\n",//1
"平均全压:              kPa  \r\n",//2
"平均流速:              m/s  \r\n",//3
"平均烟温:              ℃   \r\n",//4
"大 气 压:              kPa  \r\n",//5
"烟道截面:              m2   \r\n",//6
"含 湿 量:                   \r\n",//7
"皮管系数:                   \r\n",//8
"折算系数:                   \r\n",//9
"负荷系数:                   \r\n",//10
"烟气流量:              m3/h \r\n",//11
"标杆流量:              m3/h \r\n",//12
"跟 踪 率:                   \r\n",//13
"采样直径:              mm   \r\n",//14
"平均计压:              Kpa  \r\n",//15
"平均计温:              ℃   \r\n",//16
"采样体积:              L    \r\n",//17
"标况体积:              L    \r\n",//18
"烟尘静重:              g    \r\n",//19
"含 氧 量:              %    \r\n",//20
"二氧化硫:              mg/m3\r\n",//21
"一氧化氮:              mg/m3\r\n",//22
"二氧化氮:              mg/m3\r\n",//23
"一氧化碳:              mg/m3\r\n",//24
"硫 化 氢:              mg/m3\r\n",//25
"二氧化碳:              %    \r\n",//26
"端口配置:                   \r\n",//27  【16】开始记录
"样品编号:                   \r\n",//28
"累计采时:              s    \r\n",//29
"气 密 性:                   \r\n",//30
"采样类型:                   \r\n",//31
"溶液浓度:                   \r\n",//32
"定容体积:                   \r\n",//33
"灶头数目:                   \r\n", //34
"基准含氧:              %    \r\n", //35
"计前温度:              ℃   \r\n", //35
"计前压力:              kPa  \r\n", //37
"过剩系数:                   \r\n", //38
};

//烟气文件内容
const char Gas_line[Gas_car_length][Other_length+1]={
"平均动压:              Pa   \r\n",   //0
"平均静压:              kPa  \r\n",   //1
"平均全压:              kPa  \r\n",   //2
"平均流速:              m/s  \r\n",   //3
"平均烟温:              ℃   \r\n",   //4
"大 气 压:              kPa  \r\n",   //5
"烟道截面:              m2   \r\n",   //6
"含 湿 量:                   \r\n",   //7
"皮管系数:                   \r\n",   //8
"折算系数:                   \r\n",   //9
"负荷系数:                   \r\n",   //10
"烟气流量:              m3/h \r\n",   //11
"标杆流量:              m3/h \r\n",   //12
"含 氧 量:              %    \r\n",   //13
"二氧化硫:              mg/m3\r\n",   //14
"一氧化氮:              mg/m3\r\n",   //15
"二氧化氮:              mg/m3\r\n",   //16
"一氧化碳:              mg/m3\r\n",   //17
"硫 化 氢:              mg/m3\r\n",   //18
"二氧化碳:              %    \r\n",   //19
"端口配置:                   \r\n",   //20  【16】开始记录
"采样时长:                   \r\n",   //21
"计前温度:              ℃   \r\n",   //22
"过剩系数:                   \r\n",   //23
"基准含氧:              %    \r\n",   //24
};

const char Gas_line_record[GasRecord_length+1]={"                   ,          %        ,          mg/m3    ,          mg/m3    ,          mg/m3    ,          mg/m3    ,          mg/m3    ,          %        ,          %        ,          %       \r\n"};

/*@lian add 20290302 star*/
//烟道文件内容20200527
const char Flue_line[Flue_car_length][Other_length+1]={
"烟道面积:              m2   \r\n",   //0
"烟道类型:                   \r\n",   //1
"测点孔数:                   \r\n",   //2
"测点总数:                   \r\n",   //3
"测点  01:              m    \r\n",   //4
"测点  02:              m    \r\n",   //5
"测点  03:              m    \r\n",   //6
"测点  04:              m    \r\n",   //7
"测点  05:              m    \r\n",   //9
"测点  06:              m    \r\n",   //10
"测点  07:              m    \r\n",   //11
"测点  08:              m    \r\n",   //12
"测点  09:              m    \r\n",   //13
"测点  10:              m    \r\n",   //14
"内径D   :              m    \r\n",   //15
"套管L   :              m    \r\n",   //16
"环数    :                   \r\n",   //17
"边A     :              m    \r\n",   //18
"边B     :              m    \r\n",   //19
"单孔测点:                   \r\n",   //20
};

const u8 Flue_YXSY[5]={15,0,16,17,2};    //圆形索引:内径D ,面积，套管L ,环数,孔数
const u8 Flue_JXSY[6]={18,19,0,16,2,20}; //矩形索引:边A ,边B,面积,套管L ,孔数,单点测点
const u8 Flue_QTSY[2]={0,3};              //其他索引

/*@lian End*/
SdFile_DataStruct  SdFile_Data;




/*
	函数名：SdFile_DataStruct* SdFile_GetData(void)
	描述：获取返回File.c文件全局变量结构体
*/
SdFile_DataStruct* SdFile_GetData(void)
{
	return &SdFile_Data;
}

/*
	函数名：SdFile_Timer(void)
	描述：SD卡类的定时器
*/
void SdFile_Timer(void)
{
	if(SdFile_Data.Timer!=0)
	SdFile_Data.Timer--;
}


/*******************************************************************************
	函数名：makelinkfile_record
	输  入: NumFile：文件号
	        在写缓冲区中提取年月日
	输  出: &SdFile_Data.file_link_buf.stru.rowdata[0]
	功能说明：根据文件号，制作索引记录

	*/
void SdFile_makelinkfile_record(u32 NumFile)
{	
	 u8 i,j;
	 char pathbuf[202],charNumFile[12];   //放完整文件路径

	 //先添加索引记录
	   //格式化缓冲区
	 for(i=0;i<10;i++)
	 { 
		 for(j=0;j<9;j++)
		 SdFile_Data.file_link_buf.stru.rowdata[i][j]=' ';
		 SdFile_Data.file_link_buf.stru.rowdata[i][9]=',';
	 }
	 SdFile_Data.file_link_buf.stru.rowdata[9][8]='\r';
	 SdFile_Data.file_link_buf.stru.rowdata[9][9]='\n';

	 //组织数据
	   //文件号
	 SPutl(0x6,(u8 *)&charNumFile[0],NumFile); //把文件号转换成6为的字符串
	 charNumFile[6]=0;
	 strcpy((char *)&pathbuf[0],"   ");
	 strcat((char *)&pathbuf[0],(char *)&charNumFile[0]);
	 strncpy((char *)&SdFile_Data.file_link_buf.stru.rowdata[row1_Filenumber][0],(char *)&pathbuf[0],9);
	   //年月日 ,在数据区可获取时间
	 strcpy((char *)&pathbuf[0]," ");
	 strncpy((char *)&charNumFile[0],(char *)&SdFile_Data.InterFace_data.dat[0],4); //年
	 charNumFile[4]=0;
	 strcat((char *)&pathbuf[0],(char *)&charNumFile[0]);   
	 strncpy((char *)&charNumFile[0],(char *)&SdFile_Data.InterFace_data.dat[5],2); //月
	 charNumFile[2]=0;
	 strcat((char *)&pathbuf[0],(char *)&charNumFile[0]);                           //
	 strncpy((char *)&charNumFile[0],(char *)&SdFile_Data.InterFace_data.dat[8],2); //日
	 charNumFile[2]=0;
	 strcat((char *)&pathbuf[0],(char *)&charNumFile[0]);                           //
	 strncpy((char *)&SdFile_Data.file_link_buf.stru.rowdata[row2_YMD][0],(char *)&pathbuf[0],9);
   //日时分
	 strcpy((char *)&pathbuf[0],"   ");
	 strncpy((char *)&charNumFile[0],(char *)&SdFile_Data.InterFace_data.dat[11],2);  //日
	 charNumFile[2]=0;
	 strcat((char *)&pathbuf[0],(char *)&charNumFile[0]);   
	 strncpy((char *)&charNumFile[0],(char *)&SdFile_Data.InterFace_data.dat[14],2); //时
	 charNumFile[2]=0;
	 strcat((char *)&pathbuf[0],(char *)&charNumFile[0]);                           //
	 strncpy((char *)&charNumFile[0],(char *)&SdFile_Data.InterFace_data.dat[17],2); //秒
	 charNumFile[2]=0;
	 strcat((char *)&pathbuf[0],(char *)&charNumFile[0]);                           //
	 strncpy((char *)&SdFile_Data.file_link_buf.stru.rowdata[row3_DHM][0],(char *)&pathbuf[0],9);
	 
}
/*******************************************************************************
	函数名：FileNumberToDir(u32 NumFile)
	输  入:  NumFile:文件编号
           path:索引文件的目录
	输  出: 
	功能说明：读取采样数据
	*/
void SdFile_NumberToDir(char *path,char *pathbuf,u32 NumFile)
{
	 char charNumFile[12];
	              
	 strcpy(pathbuf,&path[0]);                    //先把路径copy到缓冲区
	 SPutl(0x6,(u8 *)&charNumFile[0],NumFile); //把文件号转换成6位的字符串
	 charNumFile[6]=0;
	 strcat(pathbuf,(char *)"/");
	 strcat(pathbuf,&charNumFile[0]);    //
	 strcat(pathbuf,".txt");             //整合文件路径
	 strcpy(pathbuf,&pathbuf[0]);
}

/**************
 函数名：clr_chaxun(void)
 功能说明：清空缓冲区
***************/
void clr_file_chaxun(void)
{
	
}


/*******************************************************************************
	函数名：u8 ReadLinkRecord(void)
	输  入:  path 文件路径
           Filenumber ,文件号(>=1,但搜索文件是从零开始) 
	输  出: SdFile_Data.file_link_buf.arr[0]
	功能说明：根据文件号搜对应时间
	*/
u8 SdFile_ReadLinkRecord(char *path,u32 Filenumber)
{    u8 res;
	   u32 Ptr;
	 //  strcpy(SdFile_Data.GENG_FILEPATH,(TCHAR *)&path[0]); //把 路径付给GENG_FILEPATH,链接文件
	   //先计算记录的定位指向
	   Ptr=(Filenumber-1)*LinkFile_Record_Length; //这是链接文件记录开头的算法，其他文件有他另外的方法
	
	   res=SdFile_read_record(path,(char *)&SdFile_Data.file_link_buf.arr[0],Ptr,LinkFile_Record_Length); //把Ptr位置的记录读出来 
		
	// res=read_n_record(SdFile_Data.GENG_FILEPATH,(char *)&SdFile_Data.file_link.arr[0],Ptr,LinkFile_Record_Length); //把第NumFile条记录读出来 
	   return res;
}

/*******************************************************************************
	函数名：u8 ReadLinkRecord(void)
	输  入:  path 文件路径
           Filenumber ,文件号(>=1,但搜索文件是从零开始) 
           SdFile_Data.file_link_buf.arr
	输  出: SdFile_Data.file_link_buf.arr[0]
	功能说明：根据文件号搜对应时间
	*/
u8 SdFile_WriteLinkRecord(char *path,u32 Filenumber)
{    u8 res;
	   u32 Ptr;
	   char dbuf[LinkFile_Record_Length+1];
	 //  strcpy(SdFile_Data.GENG_FILEPATH,(TCHAR *)&path[0]); //把 路径付给GENG_FILEPATH,链接文件
	   //先计算记录的定位指向
	   Ptr=(Filenumber-1)*LinkFile_Record_Length; //这是链接文件记录开头的算法，其他文件有他另外的方法
	   strncpy((char *)&dbuf[0],(char *)&SdFile_Data.file_link_buf.arr[0],LinkFile_Record_Length);
	   dbuf[LinkFile_Record_Length]=0;
	   res=SdFile_write_record(path,(char *)&dbuf[0],Ptr,LinkFile_Record_Length); //把Ptr位置的记录读出来 
		
	// res=read_n_record(SdFile_Data.GENG_FILEPATH,(char *)&SdFile_Data.file_link.arr[0],Ptr,LinkFile_Record_Length); //把第NumFile条记录读出来 
	   return res;
}


/*******************************************************************************
	函数名：DeleteWorkFiles(void)
	输  入:  
	输  出: 
	功能说明：删除工况文件
	修改记录：1. 增加返回删除文件状态, modify by alfred, 2020.07.13
	*/
FRESULT SdFile_DeleteWorkFiles(void)
{    FRESULT res;
	   char buffloder[100];
	   strcpy((char *)&buffloder[0],(TCHAR *)&FOLDER_OF_Work[0]);
	   res=FS_DeleteFolderOrFile((char *)&buffloder[0]);
     if(res==FR_OK)
		 {  printf("删除文件夹成功");
			  //再重建文件夹与索引文件
			res=f_mkdir((TCHAR *)&FOLDER_OF_Work[0]);
		    if(res==FR_OK)
		    printf("创建文件夹：YLB3330/Work 成功\r\n"); 
				
			res=f_mkdir((TCHAR *)&FOLDER_OF_Work00[0]);
		    if(res==FR_OK)
		    printf("创建文件夹：YLB3330/Work/Work00 成功\r\n"); 
				
			res=f_open(file_sd,(TCHAR *)&LINK_OF_Work[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //建立，打开链接文件
			if(res==FR_OK)
			{
				printf("创建Work-link.txt文件成功\r\n"); 
				res = f_lseek(file_sd,0);
				if(res==FR_OK)
				{
					printf("定位Work-link.txt文件开头成功\r\n"); 
				}				 
				f_close(file_sd);
			}			
			else
				printf("创建Work-link.txt文件失败\r\n"); 
		 }
		 else
		 {
			 printf("删除文件夹错误");
		 }
		 return res;
}

/*******************************************************************************
	函数名：DeleteGasFiles(void)
	输  入:  
	输  出: 
	功能说明：删除烟气文件
	修改记录：1. 增加返回删除文件状态, modify by alfred, 2020.07.13
	*/
FRESULT SdFile_DeleteGasFiles(void)
{     FRESULT res;
	    char buffloder[100];
	   strcpy((char *)&buffloder[0],(TCHAR *)&FOLDER_OF_Gas[0]);
	
	   res=FS_DeleteFolderOrFile((char *)&buffloder[0]);
     if(res==FR_OK)
		 {  printf("删除文件夹成功");
			  //再重建文件夹与索引文件
			  res=f_mkdir((TCHAR *)&FOLDER_OF_Gas[0]);
		    if(res==FR_OK)
		    printf("创建文件夹：YLB3330/Gas 成功\r\n"); 
				
				res=f_mkdir((TCHAR *)&FOLDER_OF_Gas00[0]);
		    if(res==FR_OK)
		    printf("创建文件夹：YLB3330/Gas/Gas00 成功\r\n"); 
				
				res=f_open(file_sd,(TCHAR *)&LINK_OF_Gas[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //建立，打开链接文件
			if(res==FR_OK)
			{
		   printf("创建Gas-link.txt文件成功\r\n"); 
			 res = f_lseek(file_sd,0);
       if(res==FR_OK)
 			 {
				  printf("定位Gas-link.txt文件开头成功\r\n"); 
			 }				 
				f_close(file_sd);
			}			
      else
			 printf("创建Gas-link.txt文件失败\r\n"); 
		 }
		 else
		 {
			 printf("删除文件夹错误");
		 }
		 return res;
}

/*******************************************************************************
	函数名：DeleteDustFiles(void)
	输  入:  
	输  出: 
	功能说明：删除烟尘文件
	修改记录：1. 增加返回删除文件状态, modify by alfred, 2020.07.13
	*/
FRESULT SdFile_DeleteDustFiles(void)
{    FRESULT res;
      char buffloder[100];
	   strcpy((char *)&buffloder[0],(TCHAR *)&FOLDER_OF_Dust[0]);
	
	  	res=FS_DeleteFolderOrFile((char *)&buffloder[0]);
     if(res==FR_OK)
		 {  printf("删除文件夹成功");
			  //再重建文件夹与索引文件
			  res=f_mkdir((TCHAR *)&FOLDER_OF_Dust[0]);
		    if(res==FR_OK)
		    printf("创建文件夹：YLB3330/Dust 成功\r\n"); 
				
				res=f_mkdir((TCHAR *)&FOLDER_OF_Dust00[0]);
		    if(res==FR_OK)
		    printf("创建文件夹：YLB3330/Dust/Dust00 成功\r\n"); 
				
				res=f_open(file_sd,(TCHAR *)&LINK_OF_Dust[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //建立，打开链接文件
			if(res==FR_OK)
			{
		   printf("创建Dust-link.txt文件成功\r\n"); 
			 res = f_lseek(file_sd,0);
       if(res==FR_OK)
 			 {
				  printf("定位Dust-link.txt文件开头成功\r\n"); 
			 }				 
				f_close(file_sd);
			}			
      else
			 printf("创建Dust-link.txt文件失败\r\n"); 
		 }
		 else
		 {
			 printf("删除文件夹错误");
		 }
		 return res;
}
/*******************************************************************************
	函数名：SdFile_DeleteFlueFiles(void)
	输  入:  
	输  出: 
	功能说明：删除布点文件
	修改记录：1. 增加返回删除文件状态, modify by alfred, 2020.07.13
	*/
FRESULT SdFile_DeleteFlueFiles(void)  //删除布点文件
{
	 FRESULT res;
      char buffloder[100];
	   strcpy((char *)&buffloder[0],(TCHAR *)&FOLDER_OF_Flue[0]);
	
	  	res=FS_DeleteFolderOrFile((char *)&buffloder[0]);
     if(res==FR_OK)
		 {  printf("删除文件夹成功");
			  //再重建文件夹与索引文件
			  res=f_mkdir((TCHAR *)&FOLDER_OF_Flue[0]);
		    if(res==FR_OK)
		    printf("创建文件夹：YLB3330/flue 成功\r\n"); 
				
				res=f_mkdir((TCHAR *)&FOLDER_OF_Flue00[0]);
		    if(res==FR_OK)
		    printf("创建文件夹：YLB3330/Flue/Flue00 成功\r\n"); 
				
				res=f_open(file_sd,(TCHAR *)&LINK_OF_Flue[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //建立，打开链接文件
			if(res==FR_OK)
			{
		   printf("创建Flue-link.txt文件成功\r\n"); 
			 res = f_lseek(file_sd,0);
       if(res==FR_OK)
 			 {
				  printf("定位Flue-link.txt文件开头成功\r\n"); 
			 }				 
				f_close(file_sd);
			}			
      else
			 printf("创建Flue-link.txt文件失败\r\n"); 
		 }
		 else
		 {
			 printf("删除文件夹错误");
		 }
		 return res;
	
}

//删除文件
u8  DeleteFile(TCHAR* FileName)
{
	u8 res;
	res=f_unlink(FileName);
	return res;
}

//====================================================================================================  
//函 数 名 : f_deldir  
//函数功能 : 移除一个文件夹，包括其本身和其子文件夹，子文件  
//输    入 : const TCHAR *path---指向要移除的空结尾字符串对象的指针  
//输    出 : 无  
//返 回 值 : FR_OK(0)：           函数成功   
//           FR_NO_FILE：         无法找到文件或目录   
//           FR_NO_PATH：         无法找到路径   
//           FR_INVALID_NAME：    文件名非法   
//           FR_INVALID_DRIVE：   驱动器号非法   
//           FR_DENIED：          函数由于以下原因被拒绝：   
//               对象属性为只读；  
//               目录下非空；  
//               当前目录。  
//           FR_NOT_READY：       磁盘驱动器无法工作，由于驱动器中没有媒体或其他原因   
//           FR_WRITE_PROTECTED： 媒体写保护   
//           FR_DISK_ERR：        函数失败由于磁盘运行的一个错误   
//           FR_INT_ERR：         函数失败由于一个错误的 FAT 结构或内部错误   
//           FR_NOT_ENABLED：     逻辑驱动器没有工作区   
//           FR_NO_FILESYSTEM：   驱动器上没有合法的 FAT 卷   
//           FR_LOCKED：          函数被拒由于文件共享机制（_FS_SHARE）   
//备    注 : f_deldir 函数用来移除一个文件夹及其子文件夹、子文件，但不能移除已经打开的对象。   
//====================================================================================================  
FRESULT f_deldir(const TCHAR *path_sure)  
{  
    FRESULT res;  
    DIR   dir;     /* 文件夹对象 */ //36  bytes  
    FILINFO fno;   /* 文件属性 */   //32  bytes  
    TCHAR lfile[_MAX_LFN + 2] = {0};  
#if _USE_LFN  
    TCHAR lname[_MAX_LFN + 2] = {0};  
#endif  
      
#if _USE_LFN  
    fno.lfsize = _MAX_LFN;  
    fno.lfname = lname;    //必须赋初值  
#endif  
    //打开文件夹  
		
    res = f_opendir(&dir, path_sure);  
    //持续读取文件夹内容  
   while((res == FR_OK) && (FR_OK == f_readdir(&dir, &fno)))  
    { 
        //若是"."或".."文件夹，跳过  
        if(0 == strlen(fno.fname))          break;      //若读到的文件名为空  
        if(0 == strcmp(fno.fname, "."))     continue;   //若读到的文件名为当前文件夹  
        if(0 == strcmp(fno.fname, ".."))    continue;   //若读到的文件名为上一级文件夹  
          
        memset(lfile, 0, sizeof(lfile));  
#if _USE_LFN  
        sprintf((char*)lfile, "%s/%s",path_sure, (*fno.lfname) ? fno.lfname : fno.fname);  
#else  
        sprintf((char*)lfile, "%s/%s", path, fno.fname);  
#endif  
        if (fno.fattrib & AM_DIR)  
        {//若是文件夹，递归删除  
            res = f_deldir(lfile); 
        }  
        else  
        {//若是文件，直接删除  
            res = f_unlink((char*)lfile); 
				
					printf("\r\n%s",lfile);	
        }  
    }  
      
    //删除本身  
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
	printf("\r\n删除根目录");
	if(Res== FR_OK)
	{
		printf("\r\n删除根目录成功");
	}
	else
	{
		printf("\r\n删除根目录失败");
	}
	return(Res);
}

/*******************************************************************************
	函数名：SdFile_Init(void)
	输  入: 为SD卡创建工作环境
          
           
	输  出: 
	功能说明：根据文件号搜对应时间
	*/

void SdFile_Init(void)
{ u8 i;

	
	my_mem_init(SRAMIN);		              //初始化内部内存池
	
	printf("sd test star....\r\n");

	for(i=0;i<=10;i++)
	{
		if(SD_Initialize())
			break;
		delay_ms(10);
	}
	
	delay_ms(500);
 
  exfuns_init();							//为FATFS 相关变量申请内传	

  f_mount(fs[0],"0:",1);       //挂载SD卡
	//f_mount(fs[1],"1:",1);       //
	f_mount(fs[2],"2:",1); 

//以下为保持文件例子
	if(SD_GetData()->flag)   //如果已链接SD
	{
		
 	SdFile_init_SD_dir();            //为SD卡键目录,目录存在就退出
	
	}
	
	
}

void test_file_write(void)
{

}
/*******************************************************************************
	函数名：serch_c(char *path,u16 NumFile)
	输  入: path 文件路径
          NumFile 文件号
	输  出:
	功能说明：从文件中搜出文件号记录
*/



/****************************************************************************
	函数名：SdFile_read_record(char *path,char * buff,u16 NumFile,u16 length)
输  入: path 文件路径指针,buff:记录存储位置指针,adrr:开始地址
        length 记录的长度
	输  出:
	功能说明：从文件中搜出文件号记录
*/
u8 SdFile_read_record(char *path,char * buff,u32 adrr,u32 length)
{  FRESULT res;
	 u8 bak;
//	 u32 i,j;

	 unsigned char read_buffer[250];                  //
	 res=f_open(file_sd,(TCHAR*)path,FA_READ);//先打开文件
	 if (res==FR_OK)  
	 {
		 printf("\r\n open file OK\r\n");
		 res = f_lseek(file_sd,adrr);               //定位文件头
     if(res == FR_OK)
		 {
			 f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
			 if(file_sd->fptr-adrr==length)                            //正确读取一条记录
			 {
				 strncpy(buff,(char *)&read_buffer[0],length);
				 bak=0;                                                 //读取正确
			 }
			 else
			 {
				 printf("read data error!\r\n");
			   bak=3;                                                //读取错误
  		 }
     }
	   else
		 {
      printf("f_lseek error!2\r\n");
			bak=2;                                                   //定位记录错误
      
     }

	   f_close(file_sd);         //关闭文件
		 return bak;             //定位文件错误
	 }
	 else
	 { f_close(file_sd);         //关闭文件
		  printf("\r\n open file ERROR\r\n");
		  bak=1;
		  return bak;              //打开文件错误
	 }

}


/****************************************************************************
	函数名：SdFile_write_record(char *path,u16 NumFile)
	输  入: path 文件路径
          NumFile 文件号
	输  出:
	功能说明：在文件尾追加记录
*/
u8 SdFile_write_record(char *path,char * buff,u32 adrr,u32 length)
{ 
	FRESULT res;
	u8 bak;
	
	 res=f_open(file_sd,(TCHAR*)path,FA_READ | FA_WRITE | FA_OPEN_ALWAYS);//如果文件存在，就打开，否则创建
	 if (res==FR_OK)
	 {
		 printf("\r\n open file OK\r\n");
		res = f_lseek(file_sd,adrr);  		 //定位到某记录开头
		if (res==FR_OK)
	  {
		 if(f_puts(buff,file_sd)!=EOF)                                     //检查到回车换行 
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
			bak=2;                                                   //定位记录错误
      
    }

		f_close(file_sd);         //关闭文件
	//	res=f_open(file_sd,(TCHAR*)path,FA_READ | FA_WRITE | FA_OPEN_ALWAYS);//如果文件存在，就打开，否则创建

	  SdFile_Data.Timer=10;
		while(SdFile_Data.Timer);  //延时10ms
					
	//	f_close(file_sd);       //关闭文件			
		return bak;             //定位文件错误  
	 }
	 else
	 {
		  printf("\r\n open file ERROR\r\n");
		  bak=1;
		  f_close(file_sd);       //关闭文件		
		  return bak;              //打开文件错误
	 }
}
/****************************************************************************
	函数名：init_SD_dir(void)
	输  入:
          
	输  出:
	功能说明：初始化新SD卡的文件路径
*/
void SdFile_init_SD_dir(void)
{
	// FATFS fs; /*  */
 //  DIR dir; /*  */
 //  FILINFO fno; /**/
   FRESULT res; /* FatFs */
	 if(f_opendir(&dir_sd,(TCHAR *)&ROOT_DIR[0])==FR_OK) //退出
	 { 
		 f_closedir(&dir_sd);
		 printf("目录存在\r\n");
	 }
	 else //以下为新的SD卡创建基本的文件目录
	 {
		 res=f_mkdir((TCHAR *)&ROOT_DIR[0]);
		 if(res==FR_OK)
		 printf("创建根目录YLB3330成功\r\n"); 
		 
     res=f_mkdir((TCHAR *)&FOLDER_OF_Dust[0]);
		 if(res==FR_OK)
		 printf("创建文件夹：YLB3330/Dust 成功\r\n"); 
			
		 res=f_mkdir((TCHAR *)&FOLDER_OF_Gas[0]);
		 if(res==FR_OK)
		 printf("创建文件夹：YLB3330/Gas 成功\r\n"); 
			
		 res=f_mkdir((TCHAR *)&FOLDER_OF_Work[0]);
		 if(res==FR_OK)
		 printf("创建文件夹：YLB3330/Work 成功\r\n"); 
		 
		 res=f_mkdir((TCHAR *)&FOLDER_OF_Flue[0]);
		 if(res==FR_OK)
		 printf("创建文件夹：YLB3330/Flue 成功\r\n");
		 
		 res=f_mkdir((TCHAR *)&FOLDER_OF_Dust00[0]);
		 if(res==FR_OK)
		 printf("创建文件夹：YLB3330/Dust/Dust00 成功\r\n"); 
			
		 res=f_mkdir((TCHAR *)&FOLDER_OF_Gas00[0]);
		 if(res==FR_OK)
		 printf("创建文件夹：YLB3330/Gas/Gas00 成功\r\n"); 
			
		 res=f_mkdir((TCHAR *)&FOLDER_OF_Work00[0]);
		 if(res==FR_OK)
		 printf("创建文件夹：YLB3330/Work/Work00 成功\r\n"); 
     	 res=f_mkdir((TCHAR *)&FOLDER_OF_Flue00[0]);
		 if(res==FR_OK)
		 printf("创建文件夹：YLB3330/Work/Flue00 成功\r\n"); 
     
			res=f_open(file_sd,(TCHAR *)&LINK_OF_Dust[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //建立，打开链接文件
			if(res==FR_OK)
			{
		   printf("创建Dust-link.txt文件成功\r\n"); 
			 res = f_lseek(file_sd,0);
       if(res==FR_OK)
 			 {
				  printf("定位Dust-link.txt文件开头成功\r\n"); 
			 }				 
				f_close(file_sd);
			}			
      else
			 printf("创建Dust-link.txt文件失败\r\n"); 
			
			res=f_open(file_sd,(TCHAR *)&LINK_OF_Gas[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //建立，打开链接文件
			if(res==FR_OK)
			{
		   printf("创建Gas-link.txt成功\r\n"); 
			 res = f_lseek(file_sd,0);
       if(res==FR_OK)
 			 {
				  printf("定位Gas-link.txt文件开头成功\r\n"); 
			 }				 
				f_close(file_sd);
			}			
      else
			 printf("创建Gas-link.txt文件失败\r\n"); 
			
			res=f_open(file_sd,(TCHAR *)&LINK_OF_Work[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //建立，打开链接文件
			if(res==FR_OK)
			{
		   printf("创建Work-link.txt文件成功\r\n"); 
			 res = f_lseek(file_sd,0);
       if(res==FR_OK)
 			 {
				  printf("定位Work-link.txt文件开头成功\r\n"); 
			 }				 
				f_close(file_sd);
			}			
      else
			 printf("创建Work-link.txt文件失败\r\n"); 
			
	    	res=f_open(file_sd,(TCHAR *)&LINK_OF_Flue[0],FA_CREATE_NEW | FA_READ | FA_WRITE | FA_OPEN_ALWAYS);   //建立，打开链接文件
			if(res==FR_OK)
			{
		   printf("创建Flue-link.txt文件成功\r\n"); 
			 res = f_lseek(file_sd,0);
       if(res==FR_OK)
 			 {
				  printf("定位Flue-link.txt文件开头成功\r\n"); 
			 }				 
				f_close(file_sd);
			}			
      else
			 printf("创建Work-link.txt文件失败\r\n"); 
			
	 }
	
}

/****************************************************************************
	函数名：u8 Serch_MaxOfFile(char *path,u32 * ptr,u32 length)
输  入:path:文件路径
       ptr：保存最大文件号
       length：记录长度+1
	输  出:
	功能说明：搜索链接文件最大文件号
*/

u8 SdFile_Serch_MaxOfFile(char *path,u32 * ptr,u32 length)
{
  FRESULT res;
	 u8 bak;

	 u32 i;
	         
	 res=f_open(file_sd,(TCHAR*)path,FA_READ);//先打开文件
	 if (res==FR_OK)  
	 {
		 printf("\r\n open file OK\r\n");
		 res = f_lseek(file_sd,0);       //定位文件头
     if(res == FR_OK)
		 {
			printf("f_lseek successful!\r\n");
			if( file_sd->fsize==0)
			{* ptr=0;}
			else	
			{
				i= file_sd->fsize;   //得到剩下的字节数
				i=i/(length);
				* ptr=i; 
			}
			bak=0;
     }
	   else
		 {
			printf("f_lseek error!2\r\n");
			bak=2;                                                   //定位0错误
     }
	   f_close(file_sd);         //关闭文件
	 }
	 else
	 {
		  printf("\r\n open file ERROR\r\n");
		  bak=1;
	 }
	 return bak;              //打开文件错误 
}
/****************************************************************************
	函数名：Serch_MaxofGasLinkFile()
	输  入:
          
	输  出:
	功能说明：搜索烟气链接文件最大文件号
*/

u8 SdFile_Serch_MaxofGasLinkFile(void)
{
	u8 res;
	res=SdFile_Serch_MaxOfFile((TCHAR *)&LINK_OF_Gas[0],&SdFile_Data.GasLinkFileMax,LinkFile_Record_Length);
	return res;
}
/****************************************************************************
	SdFile_Serch_MaxofFlueLinkFile()
	输  入:
          
	输  出:
	功能说明：搜索烟道链接文件最大文件号
*/

u8 SdFile_Serch_MaxofFlueLinkFile(void)
{
	u8 res;
	res=SdFile_Serch_MaxOfFile((TCHAR *)&LINK_OF_Flue[0],&SdFile_Data.FlueLinkFileMax,LinkFile_Record_Length);
	return res;
}
/****************************************************************************
	函数名：Serch_MaxLinkFile(char *path)
	输  入:
          
	输  出:
	功能说明：搜索烟尘链接文件最大文件号
*/
u8 SdFile_Serch_MaxofDustLinkFile(void)
{
	u8 res;
	res=SdFile_Serch_MaxOfFile((TCHAR *)&LINK_OF_Dust[0],&SdFile_Data.DustLinkFileMax,LinkFile_Record_Length);
	return res;
}
/****************************************************************************
	函数名：Serch_MaxLinkFile(char *path)
	输  入:
          
	输  出:
	功能说明：搜索工况链接文件最大文件号
*/
u8 SdFile_Serch_MaxofWorkLinkFile(void)
{
	u8 res;
	res=SdFile_Serch_MaxOfFile((TCHAR *)&LINK_OF_Work[0],&SdFile_Data.WorkLinkFileMax,LinkFile_Record_Length);
	return res;
}

/****************************************************************************
	函数名：Serch_MaxofGasRecord(u32 NumFile)
	输  入:
          
	输  出:
	功能说明：搜索第几个烟气文件最大记录数
*/

u8 SdFile_Serch_MaxofGasRecord(u32 NumFile)
{
	FRESULT res;
	u8 bak;
	char pathbuf[202];   //放完整文件路径
	u32 Ptr;
	u32 i;
		 
	SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区
	res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ);  //打开烟气文件

	if (res==FR_OK)  
	{
		printf("\r\n open file OK\r\n");
		Ptr=(Gas_car_length+1)*Other_length+Adress_length;
		res = f_lseek(file_sd,Ptr);       //跳过台头
		if(res == FR_OK)
		{
			printf("f_lseek successful!\r\n");
			if( file_sd->fsize>=Ptr)
			{
				i= file_sd->fsize-Ptr;   //得到剩下的字节数
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
			bak=2;                                                   //定位0错误
		}
		f_close(file_sd);         //关闭文件
	}
	else
	{
	printf("\r\n open file ERROR\r\n");
	bak=1;
	}
	return bak;              //打开文件错误 
}



/****************************************************************************
	函数名：ReadWorkFile(u32 NumFile)
	输  入: FileNumber 文件编号
          
	输  出:  SdFile_Data.rdWorkFile

	功能说明：读取工况文件
***************************************************************************/

u8 SdFile_ReadWorkFile(u32 NumFile)  //path 文件夹路径
{
	 FRESULT res;
	 u16 recordcnt;
	 DWORD ptr;
	 char pathbuf[50];   //放完整文件路径
	 char read_buffer[250];      //
	 SdFile_NumberToDir((char *)&FOLDER_OF_Work00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ);//先打开文件
	 if (res==FR_OK)  
	 {
		 printf("\r\n open file OK\r\n");
		 res = f_lseek(file_sd,0);               //定位文件头
		 ptr=0;
     if(res == FR_OK)
		 {
			  printf("\r\n 定位文件头正确\r\n");
			  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
			  if(file_sd->fptr-ptr==Other_length)                      //第一条记录 30
				{
					strncpy((char *)&SdFile_Data.rdWorkFile.dat[0],(char *)&read_buffer[0],Other_length);
				  ptr=file_sd->fptr;
					res=FR_OK; 
				}
				else
				{ printf("\r\n 读记录错误1\r\n");
					res=FR_DISK_ERR;
				}
				
				if(res==FR_OK)
				{
					 f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
			     if(file_sd->fptr-ptr==Adress_length)                      //第二条记录 60
					 {
						 strncpy((char *)&SdFile_Data.rdWorkFile.Adress[0],(char *)&read_buffer[0],Adress_length);
				     ptr=file_sd->fptr;
					   res=FR_OK; 
					 }
					 else
					 { printf("\r\n 读记录错误2\r\n");
						 res=FR_DISK_ERR;
					 }
					
				}
				
				if(res==FR_OK)
				{
					for(recordcnt=0;recordcnt<Work_car_length;recordcnt++)
					 { 
						  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
			        if(file_sd->fptr-ptr==Other_length)                      //第3条记录 后
							{
								strncpy((char *)&SdFile_Data.rdWorkFile.Other[recordcnt][0],(char *)&read_buffer[0],Other_length);
								ptr=file_sd->fptr;
								res=FR_OK; 
							}
							else
							{
								printf("\r\n 读记录错误3\r\n");
								res=FR_DISK_ERR;
								break;
								
							}
						 
					 }
					
				}
			
		 }
		 else
		 {
			 printf("\r\n 定位文件头错误\r\n");
			 res=FR_DISK_ERR;
		 }
		 
		f_close(file_sd);         //关闭文件
	 }
	 else
	 {
		printf("\r\n open file err\r\n");
		res=FR_DISK_ERR;
	 }

   return(res);
}
/****************************************************************************
	函数名：WriteWorkFile(u32 NumFile)
	输  入: FileNumber 文件编号
          SdFile_Data.wrWorkFile 
	输  出:
	功能说明：
***************************************************************************/

u8 SdFile_WriteWorkFile(strInterface *p,u32 NumFile)  //写入工况文件记录
{
	 FRESULT res;
	 u8 i;

	 char pathbuf[202];   //放完整文件路径
   SdFile_ImportWorkData(p);  //导入数据
	 SdFile_makelinkfile_record(NumFile);       //制作索引记录
	 strcpy(&pathbuf[0],(char *)&LINK_OF_Work[0]);
   SdFile_WriteLinkRecord((char *)&pathbuf[0],NumFile);   //读链接文件记录
	
	 SdFile_NumberToDir((char *)&FOLDER_OF_Work00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区
	 
	 
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//创建文件
   if(res==FR_OK)  //创建文件成功
	 {
		   //下面就是写文件
		res = f_lseek(file_sd,0);               //定位文件头
		 
		if (res==FR_OK)  //以下写时间 
	  {
		 strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.dat[0],Other_length);
		 pathbuf[Other_length]=0;
		 if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //写时间                                //检查到回车换行 
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
		 
    if(res==FR_OK)  //以下写地址
		{  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Adress[0],Adress_length);
		   pathbuf[Adress_length]=0;
			if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
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
		
   if(res==FR_OK) //以下写OTHER项目
		{  
			 for(i=0;i<Work_car_length;i++)
			  {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Other[i][0],Other_length);
		       pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
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

			f_close(file_sd);         //关闭文件
	 }
	 return res;

}


/****************************************************************************
	函数名：ReadDustFile(u32 NumFile)
	输  入: FileNumber 文件编号
          
	输  出:  SdFile_Data.rdWorkFile

	功能说明：读取工况文件
***************************************************************************/

u8 SdFile_ReadDustFile(u32 NumFile)  //path 文件夹路径
{
	 FRESULT res;
	 u16 recordcnt;
	 DWORD ptr;
	 char pathbuf[50];   //放完整文件路径
	 char read_buffer[250];      //
	 SdFile_NumberToDir((char *)&FOLDER_OF_Dust00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ);//先打开文件
	 if (res==FR_OK)  
	 {
		 printf("\r\n open file OK\r\n");
		 res = f_lseek(file_sd,0);               //定位文件头
		 ptr=0;
     if(res == FR_OK)
		 {
			  printf("\r\n 定位文件头正确\r\n");
			  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
			  if(file_sd->fptr-ptr==Other_length)                      //第一条记录 30
				{
					strncpy((char *)&SdFile_Data.rdWorkFile.dat[0],(char *)&read_buffer[0],Other_length);
				  ptr=file_sd->fptr;
					res=FR_OK; 
				}
				else
				{ printf("\r\n 读记录错误1\r\n");
					res=FR_DISK_ERR;
				}
				
				if(res==FR_OK)
				{
					 f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
			     if(file_sd->fptr-ptr==Adress_length)                      //第二条记录 60
					 {
						 strncpy((char *)&SdFile_Data.rdWorkFile.Adress[0],(char *)&read_buffer[0],Adress_length);
				     ptr=file_sd->fptr;
					   res=FR_OK; 
					 }
					 else
					 { printf("\r\n 读记录错误2\r\n");
						 res=FR_DISK_ERR;
					 }
					
				}
				
				if(res==FR_OK)
				{
					for(recordcnt=0;recordcnt<Dust_car_length;recordcnt++)
					 { 
						  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
			        if(file_sd->fptr-ptr==Other_length)                      //第3条记录 后
							{
								strncpy((char *)&SdFile_Data.rdWorkFile.Other[recordcnt][0],(char *)&read_buffer[0],Other_length);
				        ptr=file_sd->fptr;
					      res=FR_OK; 
							}
							else
							{
								printf("\r\n 读记录错误3\r\n");
								res=FR_DISK_ERR;
								break;
								
							}
						 
					 }
					
				}
			
		 }
		 else
		 {
			 printf("\r\n 定位文件头错误\r\n");
			 res=FR_DISK_ERR;
		 }
		 
		f_close(file_sd);         //关闭文件
	 }
	 else
	 {
		printf("\r\n open file err\r\n");
		res=FR_DISK_ERR;
	 }
   return(res);
}
/****************************************************************************
	函数名：WriteWorkFile(u32 NumFile)
	输  入: FileNumber 文件编号
          SdFile_Data.wrWorkFile 
	输  出:
	功能说明：
***************************************************************************/

u8 SdFile_WriteDustFile(strInterface *p,u32 NumFile)  //写入工况文件记录
{
	 FRESULT res;
	 u8 i;

	 char pathbuf[202];   //放完整文件路径

	SdFile_ImportDustData(p);      //导入数据
	
  SdFile_makelinkfile_record(NumFile);       //制作索引记录
	 
   SdFile_WriteLinkRecord((char *)&LINK_OF_Dust[0],NumFile);   //写链接文件记录
	
	 SdFile_NumberToDir((char *)&FOLDER_OF_Dust00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区
	 
	 
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//创建文件
   if(res==FR_OK)  //创建文件成功
	 {
		   //下面就是写文件
		res = f_lseek(file_sd,0);               //定位文件头
		 
		if (res==FR_OK)  //以下写时间 
	  {
		 strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.dat[0],Other_length);
		 pathbuf[Other_length]=0;
		 if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //写时间                                //检查到回车换行 
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
		 
    if(res==FR_OK)  //以下写地址
		{  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Adress[0],Adress_length);
		   pathbuf[Adress_length]=0;
			if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
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
		
   if(res==FR_OK) //以下写OTHER项目
		{  
			 for(i=0;i<Dust_car_length;i++)
			  {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Other[i][0],Other_length);
		       pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
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
			f_close(file_sd);         //关闭文件
	 }
	 return res;

}

/****************************************************************************
20200523
函数名：SdFile_WriteDustRecord(strInterface *p,u32 NumFile)  //写入烟尘文件的记录
	输  入: FileNumber 文件编号
          SdFile_Data.wrWorkFile 
	输  出:
	功能说明：
***************************************************************************/

u8 SdFile_WriteDustRecord(strInterface *p,u32 NumFile)  //写入工况文件记录
{
	 FRESULT res;
	 u8 i;
	 char pathbuf[202];   //放完整文件路径
	SdFile_ImportDustData(p);      //导入数据
	SdFile_NumberToDir((char *)&FOLDER_OF_Dust00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//创建文件
   if(res==FR_OK)  //创建文件成功
	 {
		   //下面就是写文件
		res = f_lseek(file_sd,0+Other_length+Adress_length);               //定位文件头
	
   if(res==FR_OK) //以下写OTHER项目
		{  
			 for(i=0;i<Dust_car_length;i++)
			  {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Other[i][0],Other_length);
		       pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
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
			f_close(file_sd);         //关闭文件
	 }
	 return res;

}
/****************************************************************************
	函数名：SdFile_ReadGasRecord(u32 NumFile,u32 NumRecord)  //读取烟气文件记录
	输  入: NumFile 文件编号
          NumRecord 第几条记录，从1开始

	输  出:  SdFile_Data.rdWorkFile

	功能说明：读取烟气文件
***************************************************************************/

u8 SdFile_ReadGasRecord(u32 NumFile,u32 NumRecord)  //读取烟气文件记录
{
	 FRESULT res;
	
	 u8 i;
	 DWORD ptr;
	 char pathbuf[50];   //放完整文件路径
	 char read_buffer[250];      //
	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ);//先打开文件
	 if (res==FR_OK)  
	 {
		      printf("\r\n open file OK\r\n");
		
					if(NumRecord<=SdFile_Data.GasRecordMax)  //判断有没有记录，最大记录，开始打开文件时计算
					{
					 ptr=(Gas_car_length+1)*Other_length+Adress_length+(NumRecord-1)*GasRecord_length;  //定位第几条记录位置
					 res = f_lseek(file_sd,ptr);               //定位文件头
					 f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
			     if(file_sd->fptr-ptr==GasRecord_length)                      // 200
					 {
						 strncpy((char *)&SdFile_Data.rdWorkFile.G_record.arr[0],(char *)&read_buffer[0],GasRecord_length);
				     ptr=file_sd->fptr;
					   res=FR_OK; 
					 }
					 else
					 { printf("\r\n 读记录错误2\r\n");
						 res=FR_DISK_ERR;
					 }
					}
					else  //无记录填充空格 
					{
						for(i=0;i<GasRecord_length;i++)
						SdFile_Data.rdWorkFile.G_record.arr[i]=' ';
						
					}
			f_close(file_sd);         //关闭文件
		 }
		 else
		 {
			 printf("\r\n 定位文件头错误\r\n");
			 res=FR_DISK_ERR;
		 }

   return(res);
}

/****************************************************************************
	函数名：ReadDustFile(u32 NumFile)
	输  入: NumFile 文件编号
          NumRecord 第几条记录，从1器

	输  出:  SdFile_Data.rdWorkFile

	功能说明：读取烟气文件
***************************************************************************/

u8 SdFile_ReadGasFile(u32 NumFile,u32 NumRecord)  //读取烟气文件记录
{
	 FRESULT res;
	 u16 recordcnt;
	 u8 i;
	 DWORD ptr;
	 char pathbuf[50];   //放完整文件路径
	 char read_buffer[250];      //
	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ);//先打开文件
	 if (res==FR_OK)  
	 {
		 printf("\r\n open file OK\r\n");
		 res = f_lseek(file_sd,0);               //定位文件头
		 ptr=0;
     if(res == FR_OK)
		 {
			  printf("\r\n 定位文件头正确\r\n");
			  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
			  if(file_sd->fptr-ptr==Other_length)                      //第一条记录 30
				{
					strncpy((char *)&SdFile_Data.rdWorkFile.dat[0],(char *)&read_buffer[0],Other_length);
					ptr=file_sd->fptr;
					res=FR_OK; 
				}
				else
				{ printf("\r\n 读记录错误1\r\n");
					res=FR_DISK_ERR;
				}
				
				if(res==FR_OK)
				{
					 f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
					if(file_sd->fptr-ptr==Adress_length)                      //第二条记录 60
					 {
						strncpy((char *)&SdFile_Data.rdWorkFile.Adress[0],(char *)&read_buffer[0],Adress_length);
						ptr=file_sd->fptr;
						res=FR_OK; 
					 }
					 else
					 { printf("\r\n 读记录错误2\r\n");
						 res=FR_DISK_ERR;
					 }
					
				}
				
				if(res==FR_OK)
				{
					for(recordcnt=0;recordcnt<Gas_car_length;recordcnt++)
					 { 
						  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
			        if(file_sd->fptr-ptr==Other_length)                      //第3条记录 后
							{
								strncpy((char *)&SdFile_Data.rdWorkFile.Other[recordcnt][0],(char *)&read_buffer[0],Other_length);
				        ptr=file_sd->fptr;
					      res=FR_OK; 
							}
							else
							{
								printf("\r\n 读记录错误3\r\n");
								res=FR_DISK_ERR;
								break;

							}
						 
					 }
					
				}
				if(res==FR_OK)
				{ 
					if(NumRecord<=SdFile_Data.GasRecordMax)  //判断有没有记录，最大记录，开始打开文件时计算
					{
						ptr=file_sd->fptr+(NumRecord-1)*GasRecord_length;  //定位第几条记录位置

						res = f_lseek(file_sd,ptr);               //定位文件头
						f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
						if(file_sd->fptr-ptr==GasRecord_length)                      // 200
						{
							strncpy((char *)&SdFile_Data.rdWorkFile.G_record.arr[0],(char *)&read_buffer[0],GasRecord_length);
							ptr=file_sd->fptr;
							res=FR_OK; 
						}
						else
						{ 
							printf("\r\n 读记录错误2\r\n");
							res=FR_DISK_ERR;
						}
					}
					else  //无记录填充空格 
					{
						for(i=0;i<GasRecord_length;i++)
						SdFile_Data.rdWorkFile.G_record.arr[i]=' ';
					}
					
				}
			
		 }
		 else
		 {
			 printf("\r\n 定位文件头错误\r\n");
			 res=FR_DISK_ERR;
		 }
		 
		f_close(file_sd);         //关闭文件
	 }
	 else
	 {
		printf("\r\n open file err\r\n");
		res=FR_DISK_ERR;
	 }
   return(res);
}
/****************************************************************************
	函数名：SdFile_WriteGasFile(strInterface *p,u32 NumFile,u8 attr,u8 WriteLink)
	输  入: FileNumber 文件编号
          SdFile_Data.wrWorkFile
          attr:>0时追加记录，=0时不追加
	输  出:
	功能说明：
***************************************************************************/
unsigned int writeWorkfileEnable = 1;
u8 SdFile_WriteGasFile(strInterface *p,u32 NumFile,u8 attr,u8 WriteLink)  //写入气体文件记录
{
	FRESULT res;
	u8 i;
	DWORD ptr;
	char pathbuf[202];   //放完整文件路径

	SdFile_ImportGasData(p);      //导入数据
	if(WriteLink==1)
	{
		 SdFile_makelinkfile_record(NumFile);       //制作索引记录
		 SdFile_WriteLinkRecord((char *)&LINK_OF_Gas[0],NumFile);   //写链接文件记录
	}
	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区

	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//创建文件
     if(res==FR_OK)  //创建文件成功
	  {
		   //下面就是写文件
		 
			if(attr) //>0时追加记录，=0时不追加
			{
				//先定为到最大
				
				ptr=(DWORD)(Other_length*(Gas_car_length+1)+Adress_length+(SdFile_Data.GasRecordMax)*GasRecord_length);  //定位第几条记录位置
                res = f_lseek(file_sd,ptr);  
				strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.G_record.arr[0],GasRecord_length);
				pathbuf[GasRecord_length]=0;
           //定位文件头
			  if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)        //检查到回车换行 
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
			res = f_lseek(file_sd,0);               //定位文件头
		 
		     if (res==FR_OK)  //以下写时间 
	           {
		       strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.dat[0],Other_length);
		        pathbuf[Other_length]=0;
		     if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //写时间                                //检查到回车换行 
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
		 
               if(res==FR_OK)  //以下写地址
		       {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Adress[0],Adress_length);
		          pathbuf[Adress_length]=0;
			      if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
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
		
                if(res==FR_OK) //以下写OTHER项目
		         {  
			      for(i=0;i<Gas_car_length;i++)
			      {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Other[i][0],Other_length);
		            pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
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
			f_close(file_sd);         //关闭文件

		}

	 return res;

}
/*

u8 SdFile_WriteGasFile(strInterface *p,u32 NumFile,u8 attr,u8 WriteLink)  //写入工况文件记录
{
	 FRESULT res;
	 u8 i;

	 char pathbuf[202];   //放完整文件路径

	 SdFile_ImportGasData(p);      //导入数据
	if(WriteLink==1){
   SdFile_makelinkfile_record(NumFile);       //制作索引记录
	 
   SdFile_WriteLinkRecord((char *)&LINK_OF_Gas[0],NumFile);   //写链接文件记录
	}
	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区
	 
	
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//创建文件
   if(res==FR_OK)  //创建文件成功
	 {
		   //下面就是写文件
		res = f_lseek(file_sd,0);               //定位文件头
		 
		if (res==FR_OK)  //以下写时间 
	  {
		 strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.dat[0],Other_length);
		 pathbuf[Other_length]=0;
		 if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //写时间                                //检查到回车换行 
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
		 
    if(res==FR_OK)  //以下写地址
		{  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Adress[0],Adress_length);
		   pathbuf[Adress_length]=0;
			if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
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
		
   if(res==FR_OK) //以下写OTHER项目
		{  
			 for(i=0;i<Gas_car_length;i++)
			  {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Other[i][0],Other_length);
		       pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
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
		
		if(res==FR_OK) //以下写添加记录
		{
			if(attr) //>0时追加记录，=0时不追加
			{
				//先定为到最大
				file_sd->fptr=Other_length*(Gas_car_length+1)+Adress_length+(SdFile_Data.GasRecordMax)*GasRecord_length;  //定位第几条记录位置
				strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.G_record.arr[0],GasRecord_length);
				pathbuf[GasRecord_length]=0;
				res = f_lseek(file_sd,file_sd->fptr);             //定位文件头
			  if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
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
		
			f_close(file_sd);         //关闭文件
	 }
	 return res;

}
*/
/****************************************************************************
20200515
	函数名：SdFile_WriteGasFileAverageRecord(strInterface *p,u32 NumFile)
	输  入: FileNumber 文件编号
          p指向所要写入的数据源
	输  出:
	功能说明：
***************************************************************************/
u8 SdFile_WriteGasFileAverageRecord(strInterface *p,u32 NumFile)  //再写一次烟气平均值
{
	 FRESULT res;
	 u8 i;

	 char pathbuf[202];   //放完整文件路径

	 SdFile_ImportGasData(p);      //导入数据

	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区

	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//创建文件
   if(res==FR_OK)  //创建文件成功
	 {
		   //下面就是写文件
		file_sd->fptr=Other_length*(14)+Adress_length;  //定位第几条记录写入的位置
		res = f_lseek(file_sd,file_sd->fptr);               //定位文件头
	
   if(res==FR_OK) //以下写OTHER项目
		{  
			 for(i=13;i<20;i++)   //写入13~19行数据
			  {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Other[i][0],Other_length);
		       pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
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
	
			f_close(file_sd);         //关闭文件
	 }
	 return res;
}
/****************************************************************************
20200518
	函数名：SdFile_WriteGasFileAverageRecord(strInterface *p,u32 NumFile)
	输  入: FileNumber 文件编号
          p指向所要写入的数据源
	输  出:
	功能说明：
***************************************************************************/
u8 SdFile_WriteGasFile_Z_J_F_Record(strInterface *p,u32 NumFile)  //再写一次烟气平均值
{
	 FRESULT res;
	 u8 i;
   u8 ip[12]={9,10,13,14,15,16,17,18,19,23,24};
	 char pathbuf[202];   //放完整文件路径

	 SdFile_ImportGasData(p);      //导入数据

	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区

	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//创建文件
   if(res==FR_OK)  //创建文件成功
	 {
		   //下面就是写文件

	
 
			 for(i=0;i<12;i++)   //写入13~19行数据
			  {
          file_sd->fptr=Other_length*(ip[i]+1)+Adress_length;  //定位第几条记录写入的位置
		      res = f_lseek(file_sd,file_sd->fptr);               //定位文件头
					if(res==FR_OK)  //创建文件成功
	         {
					strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.wrWorkFile.Other[ip[i]][0],Other_length);
		       pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
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

	
			f_close(file_sd);         //关闭文件
	 }
	 return res;
}
/****************************************************************************
20200515	
函数名：SdFile_WriteGasFiletxt
	输  入: FileNumber 文件编号
          record 第几个记录
          txt 被写入的参与运算的标记字符
	输  出:
	功能说明：
***************************************************************************/

u8 SdFile_WriteGasFiletxt(u32 NumFile,u32 record,char txt)  //向文件某个地址写入字节
{
	 FRESULT res;
	

	 char pathbuf[202];   //放完整文件路径

	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区
	 	
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//创建文件
   if(res==FR_OK)  //创建文件成功
	 {
		   //下面就是写文件
	
				//先定为到最大
				file_sd->fptr=Other_length*(Gas_car_length+1)+Adress_length+(record-1)*GasRecord_length+18;  //定位第几条记录写入的位置
		    pathbuf[0]=txt;
				
		    pathbuf[1]=0;
				res = f_lseek(file_sd,file_sd->fptr);             //定位文件头
			  if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
		     {
		      printf("WRITE FILE successful...........!\r\n");
		      res=FR_OK;
		     }
		    else
		     {
 		      printf("WRITE FILE REEOR!\r\n");
		      res=FR_DISK_ERR; 
		     }
	
			f_close(file_sd);         //关闭文件
	 }
	 return res;

}

/****************************************************************************
20200515
函数名：SdFile_WriteGasFiletxt
	输  入: FileNumber 文件编号
          record 第几个记录
          txt 被写入的参与运算的标记字符
	输  出:
	功能说明：
***************************************************************************/

u8 SdFile_ReadGasFiletxt(u32 NumFile,u32 record,char *txt)  //向文件某个地址写入字节
{
	 FRESULT res;

	 char pathbuf[50];   //放完整文件路径
	 char read_buffer[250];      //
	 SdFile_NumberToDir((char *)&FOLDER_OF_Gas00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ);//先打开文件
   if(res==FR_OK)  //创建文件成功
	 {
		   //下面就是写文件
	
				//先定为到最大
				file_sd->fptr=Other_length*(Gas_car_length+1)+Adress_length+(record-1)*GasRecord_length;  //定位第几条记录写入的位置
		    f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
				txt[0]=read_buffer[18];  //获取那个标记字符
				res = f_lseek(file_sd,file_sd->fptr);             //定位文件头
			  if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
		     {
		      printf("WRITE FILE successful...........!\r\n");
		      res=FR_OK;
		     }
		    else
		     {
 		      printf("WRITE FILE REEOR!\r\n");
		      res=FR_DISK_ERR; 
		     }
	
			f_close(file_sd);         //关闭文件
	 }
	 return res;

}

//--
/****************************************************************************
	函数名：SdFile_ReadFlueFile(u32 NumFile)  
	输  入: NumFile 文件编号
  @lian 20200302
	输  出:  SdFile_Data.FlueFile

	功能说明：读取烟道文件
***************************************************************************/

u8 SdFile_ReadFlueFile(u32 NumFile)  //读取烟道文件记录
{
	 FRESULT res;
	 u16 recordcnt;

	 DWORD ptr;
	 char pathbuf[50];   //放完整文件路径
	 char read_buffer[250];      //
	 SdFile_NumberToDir((char *)&FOLDER_OF_Flue00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ);//先打开文件
	 if (res==FR_OK)  
	 {
		 printf("\r\n open file OK\r\n");
		 res = f_lseek(file_sd,0);               //定位文件头
		 ptr=0;
     if(res == FR_OK)
		 {
			  printf("\r\n 定位文件头正确\r\n");
			  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
			  if(file_sd->fptr-ptr==Other_length)                      //第一条记录 30
				{
					strncpy((char *)&SdFile_Data.FlueFile.dat[0],(char *)&read_buffer[0],Other_length);
				  ptr=file_sd->fptr;
					res=FR_OK; 
				}
				else
				{ printf("\r\n 读记录错误1\r\n");
					res=FR_DISK_ERR;
				}
				
				if(res==FR_OK)
				{
					 f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
			     if(file_sd->fptr-ptr==Adress_length)                      //第二条记录 60
					 {
						 strncpy((char *)&SdFile_Data.FlueFile.Adress[0],(char *)&read_buffer[0],Adress_length);
				     ptr=file_sd->fptr;
					   res=FR_OK; 
					 }
					 else
					 { printf("\r\n 读记录错误2\r\n");
						 res=FR_DISK_ERR;
					 }
					
				}
				
				if(res==FR_OK)
				{
					for(recordcnt=0;recordcnt<Flue_car_length;recordcnt++)
					 { 
						  f_gets ((TCHAR*)read_buffer,sizeof(read_buffer),file_sd);//以回车为界读字符串	
			        if(file_sd->fptr-ptr==Other_length)                      //第3条记录 后
							{
								strncpy((char *)&SdFile_Data.FlueFile.Other[recordcnt][0],(char *)&read_buffer[0],Other_length);
				        ptr=file_sd->fptr;
					      res=FR_OK; 
							}
							else
							{
								printf("\r\n 读记录错误3\r\n");
								res=FR_DISK_ERR;
								break;

							}
						 
					 }
					
				}
			
			
		 }
		 else
		 {
			 printf("\r\n 定位文件头错误\r\n");
			 res=FR_DISK_ERR;
		 }
		 
		f_close(file_sd);         //关闭文件
	 }
	 else
	 {
		printf("\r\n open file err\r\n");
		res=FR_DISK_ERR;
	 }
   return(res);
}
/****************************************************************************
	函数名：SdFile_WriteFlueFile(strInterface *p,u32 NumFile)
	输  入: FileNumber 文件编号
          SdFile_Data.wrWorkFile
         
  @lian 20200203
	输  出:
	功能说明：
***************************************************************************/

u8 SdFile_WriteFlueFile(strInterface *p,u32 NumFile)  //写入烟道文件记录
{
	 FRESULT res;
	 u8 i;

	 char pathbuf[202];   //放完整文件路径

	 SdFile_ImportFlueData();      //导入数据
	
   SdFile_makelinkfile_record(NumFile);       //制作索引记录
	 
   SdFile_WriteLinkRecord((char *)&LINK_OF_Flue[0],NumFile);   //写链接文件记录
	
	 SdFile_NumberToDir((char *)&FOLDER_OF_Flue00[0],(char *)&pathbuf[0],NumFile); //把文件的路径放到路径缓冲区
	 
	 
	 res=f_open(file_sd,(TCHAR*)&pathbuf[0],FA_READ| FA_WRITE | FA_OPEN_ALWAYS);//创建文件
   if(res==FR_OK)  //创建文件成功
	 {
		   //下面就是写文件
		res = f_lseek(file_sd,0);               //定位文件头
		 
		if (res==FR_OK)  //以下写时间 
	  {
		 strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.FlueFile.dat[0],Other_length);
		 pathbuf[Other_length]=0;
		 if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //写时间                                //检查到回车换行 
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
		 
    if(res==FR_OK)  //以下写地址
		{  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.FlueFile.Adress[0],Adress_length);
		   pathbuf[Adress_length]=0;
			if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
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
		
   if(res==FR_OK) //以下写OTHER项目
		{  
			 for(i=0;i<Flue_car_length;i++)
			  {  strncpy((char *)&pathbuf[0],(char *)&SdFile_Data.FlueFile.Other[i][0],Other_length);
		       pathbuf[Other_length]=0;
					if(f_puts((char *)&pathbuf[0],file_sd)!=EOF)     //地址                              //检查到回车换行 
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
		
			f_close(file_sd);         //关闭文件
	 }
	 return res;

}
//--
/****************************************************************************
	函数名：SdFile_ramlodair(char *p,u8 length)//缓冲区填充空格
	输  入: 
          
	输  出:
	功能说明：
***************************************************************************/

void  SdFile_ramlodair(char *p,u8 length)//缓冲区填充空格
{
	    u8 i;
	    for(i=0;i<length;i++)
	    {
				*p=' ';
				p++;
	    }
}
/****************************************************************************
	函数名：SdFile_ImportGasData(strInterface *p)
	输  入: strInterface *p
          
	输  出:
	功能说明：导入烟气数据
***************************************************************************/

void  SdFile_ImportGasData(strInterface *p)
{     u8 i;
	    SdFile_ramlodair((char *)&SdFile_Data.wrWorkFile.dat[0],Other_length);  //先填充空格
	    SdFile_Data.wrWorkFile.dat[Other_length-2]='\r';
	    SdFile_Data.wrWorkFile.dat[Other_length-1]='\n';
	    strncpy((char *)&SdFile_Data.wrWorkFile.dat[0],(char *)&SdFile_Data.InterFace_data.dat[0],Interf_dat_length);  //拷贝时间
	  //MARK20210320
	    strncpy((char *)&SdFile_Data.wrWorkFile.dat[Interf_dat_length+2],(char *)&SdFile_Data.InterFace_data.T[0],2);  //拷贝 采样时长  
	
	   
	    SdFile_ramlodair((char *)&SdFile_Data.wrWorkFile.Adress[0],Adress_length);  //先填充空格
	    
	
	    strncpy((char *)&SdFile_Data.wrWorkFile.Adress[0],(char *)"采样地址:",9);
	    strncpy((char *)&SdFile_Data.wrWorkFile.Adress[9],(char *)&SdFile_Data.InterFace_data.Adress[0],(Adress_length-9));  //拷贝地址
	    SdFile_Data.wrWorkFile.Adress[Adress_length-2]='\r';
	    SdFile_Data.wrWorkFile.Adress[Adress_length-1]='\n';
	    for(i=0;i<Gas_car_length;i++)
	    {
			strncpy((char *)&SdFile_Data.wrWorkFile.Other[i][0],(char *)&Gas_line[i][0],Other_length);    //拷贝原始数据格式
		}
		 for(i=0;i<Gas_car_length;i++)
	    {
			strncpy((char *)&SdFile_Data.wrWorkFile.Other[i][9],(char *)&SdFile_Data.InterFace_data.Other[i][0],Interf_other_length);    //拷贝接口数据
		}
	  
			//以下是拷贝记录
			strncpy((char *)&SdFile_Data.wrWorkFile.G_record.arr[0],(char *)&Gas_line_record[0],GasRecord_length); //拷贝记录原始数据格式
			strncpy((char *)&SdFile_Data.wrWorkFile.G_record.stru.rowdata[0][0],(char *)&SdFile_Data.InterFace_data.recorddat[0],(Record_row_length-1)); //拷贝记录时间，不保或逗号
			for(i=0;i<9;i++)
			{
				strncpy((char *)&SdFile_Data.wrWorkFile.G_record.stru.rowdata[i+1][0],(char *)&SdFile_Data.InterFace_data.record[i][0],(Record_row_length-10));
			}
}
/****************************************************************************
	函数名：SdFile_ImportDustData(strInterface *p)
	输  入: strInterface *p
          
	输  出:
	功能说明：导入烟尘数据
***************************************************************************/
void  SdFile_ImportDustData(strInterface *p)
{
	     u8 i;
	    SdFile_ramlodair((char *)&SdFile_Data.wrWorkFile.dat[0],Other_length);  //先填充空格
	    SdFile_Data.wrWorkFile.dat[Other_length-2]='\r';
	    SdFile_Data.wrWorkFile.dat[Other_length-1]='\n';
	    strncpy((char *)&SdFile_Data.wrWorkFile.dat[0],(char *)&SdFile_Data.InterFace_data.dat[0],Interf_dat_length);  //拷贝时间
	    
	    SdFile_ramlodair((char *)&SdFile_Data.wrWorkFile.Adress[0],Adress_length);  //先填充空格
	  
	    strncpy((char *)&SdFile_Data.wrWorkFile.Adress[0],(char *)"采样地址:",9);
	    strncpy((char *)&SdFile_Data.wrWorkFile.Adress[9],(char *)&SdFile_Data.InterFace_data.Adress[0],(Adress_length-9));  //拷贝地址
	    SdFile_Data.wrWorkFile.Adress[Adress_length-2]='\r';
	    SdFile_Data.wrWorkFile.Adress[Adress_length-1]='\n';
	
	    for(i=0;i<Dust_car_length;i++)
	    {
				strncpy((char *)&SdFile_Data.wrWorkFile.Other[i][0],(char *)&Dust_line[i][0],Other_length);    //拷贝原始数据格式
			}
			 for(i=0;i<Dust_car_length;i++)
	    {
				strncpy((char *)&SdFile_Data.wrWorkFile.Other[i][9],(char *)&SdFile_Data.InterFace_data.Other[i][0],Interf_other_length);    //拷贝接口数据
			}
}
/****************************************************************************
	函数名：SdFile_ImportWorkData(strInterface *p)
	输  入: strInterface *p
          
	输  出:
	功能说明：导入工况数据
***************************************************************************/
void  SdFile_ImportWorkData(strInterface *p)
{
	    u8 i;
	    SdFile_ramlodair((char *)&SdFile_Data.wrWorkFile.dat[0],Other_length);  //先填充空格
	    SdFile_Data.wrWorkFile.dat[Other_length-2]='\r';
	    SdFile_Data.wrWorkFile.dat[Other_length-1]='\n';
	    strncpy((char *)&SdFile_Data.wrWorkFile.dat[0],(char *)&SdFile_Data.InterFace_data.dat[0],Interf_dat_length);  //拷贝时间
	    
	    SdFile_ramlodair((char *)&SdFile_Data.wrWorkFile.Adress[0],Adress_length);  //先填充空格
	    SdFile_Data.wrWorkFile.Adress[Adress_length-2]='\r';
	    SdFile_Data.wrWorkFile.Adress[Adress_length-1]='\n';
	
	    strncpy((char *)&SdFile_Data.wrWorkFile.Adress[0],(char *)"采样地址:",9);
	    strncpy((char *)&SdFile_Data.wrWorkFile.Adress[9],(char *)&SdFile_Data.InterFace_data.Adress[0],(Adress_length-9-2));  //拷贝地址
	    for(i=0;i<Work_car_length;i++)
	    {
			strncpy((char *)&SdFile_Data.wrWorkFile.Other[i][0],(char *)&Work_line[i][0],Other_length);    //拷贝原始数据格式
		}
		 for(i=0;i<Work_car_length;i++)
		{
			strncpy((char *)&SdFile_Data.wrWorkFile.Other[i][9],(char *)&SdFile_Data.InterFace_data.Other[i][0],Interf_other_length);    //拷贝接口数据
		}
}
//--
/****************************************************************************
	SdFile_ImportFlueData(void)
	输  入: void
  @lian 20200302        
	输  出:
	功能说明：导入烟道数据
***************************************************************************/
void  SdFile_ImportFlueData(void)
{
	    u8 i;
	    SdFile_ramlodair((char *)&SdFile_Data.FlueFile.dat[0],Other_length);  //先填充空格
	    SdFile_Data.FlueFile.dat[Other_length-2]='\r';
	    SdFile_Data.FlueFile.dat[Other_length-1]='\n';
	    strncpy((char *)&SdFile_Data.FlueFile.dat[0],(char *)&SdFile_Data.InterFace_data.dat[0],Interf_dat_length);  //拷贝时间
	    
	    SdFile_ramlodair((char *)&SdFile_Data.FlueFile.Adress[0],Adress_length);  //先填充空格
	   
	
	    strncpy((char *)&SdFile_Data.FlueFile.Adress[0],(char *)"采样地址:",9);
	    strncpy((char *)&SdFile_Data.FlueFile.Adress[9],(char *)&SdFile_Data.InterFace_data.Adress[0],(Adress_length-9));  //拷贝地址
	    SdFile_Data.FlueFile.Adress[Adress_length-2]='\r';
	    SdFile_Data.FlueFile.Adress[Adress_length-1]='\n';
	   for(i=0;i<Flue_car_length;i++)
	    {
				strncpy((char *)&SdFile_Data.FlueFile.Other[i][0],(char *)&Flue_line[i][0],Other_length);    //拷贝原始数据格式
			}
			 for(i=0;i<Flue_car_length;i++)
	    {
				strncpy((char *)&SdFile_Data.FlueFile.Other[i][9],(char *)&SdFile_Data.InterFace_data.Other[i][0],Interf_other_length);    //拷贝接口数据
			}
			
	
}



//文件复制
//将psrc文件,copy到pdst.
//psrc,pdst:源文件和目标文件
//fwmode:文件写入模式
//0:不覆盖原有的文件
//1:覆盖原有的文件
u8 mf_copy(u8 *psrc,u8 *pdst,u8 fwmode)
{
	u8 res;
	u16 br=0;
	u16 bw=0;
	FIL *fsrc=0;
	FIL *fdst=0;
	u8 *fbuf=0;
	fsrc=(FIL*)mymalloc(SRAMIN,sizeof(FIL));//申请内存
	fdst=(FIL*)mymalloc(SRAMIN,sizeof(FIL));
	fbuf=(u8*)mymalloc(SRAMIN,512);
	if(fsrc==NULL||fdst==NULL||fbuf==NULL)res=100;//前面的值留给fatfs
	else
	{
		if(fwmode==0)fwmode=FA_CREATE_NEW;//不覆盖
		else fwmode=FA_CREATE_ALWAYS; //覆盖存在的文件
		
		res=f_open(fsrc,(const TCHAR*)psrc,FA_READ|FA_OPEN_EXISTING);//打开只读文件
		if(res==0)res=f_open(fdst,(const TCHAR*)pdst,FA_WRITE|fwmode); //第一个打开成功,才开始打开第二个
		if(res==0)//两个都打开成功了
		{
			while(res==0)//开始复制
			{
				res=f_read(fsrc,fbuf,512,(UINT*)&br);//源头读出512字节
				if(res||br==0)break;
				res=f_write(fdst,fbuf,(UINT)br,(UINT*)&bw);//写入目的文件
				if(res||bw<br) break;
			}
			f_close(fsrc);
			f_close(fdst);
		}
	}
	myfree(SRAMIN,fsrc);//释放内存
	myfree(SRAMIN,fdst);
	myfree(SRAMIN,fbuf);
	return res;
}



//--
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Slave_DebugASCII(u8 *p,u8 len)
	描述：		该函数为slave.c文件的ASCII调试文件
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

	u16 cmd = Calculate_CharToU16(p,3);//计算命令

	switch(cmd)
	{
		case 1:
		{//读取所有传感器的实时采样值
		  printf("\r\n读取工况文件1");
			SdFile_ReadWorkFile(0x01);  //读取1号文件
			
			strncpy(&buf[0],&SdFile_Data.rdWorkFile.dat[0],Other_length);   //提取读取的时间放到接口数据结构的时间项
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
		printf("\r\n写工况文件6");
		SaveWorkData((float *)&input[0],5);
		SdFile_WriteWorkFile(&SdFile_Data.InterFace_data,0x05);  //写进6号文件
			break;
		}
		case 3:
		{
		
			break;
		}
		
		case 4:   //
		{ SdFile_Serch_MaxofGasLinkFile();
			printf("烟气文件数%d",SdFile_Data.GasLinkFileMax);
			break;
		}
		case 5:   //
		{ SdFile_Serch_MaxofDustLinkFile();
			printf("烟尘文件数%d",SdFile_Data.DustLinkFileMax);
			break;
		}
		case 6:   //
		{ SdFile_Serch_MaxofWorkLinkFile();
			printf("工况文件数%d",SdFile_Data.WorkLinkFileMax);
			break;
		}
		case 7:   //
		{
			SdFile_Serch_MaxofGasRecord(0x08);
			printf("8号烟气文件记录数%d",SdFile_Data.GasRecordMax);
			break;
		}

		case 11:
		{//读取所有传感器的实时采样值
		  printf("\r\n读取烟尘文件1");
			SdFile_ReadDustFile(0x01);  //读取1号文件
			
			strncpy(&buf[0],&SdFile_Data.rdWorkFile.dat[0],Other_length);   //提取读取的时间放到接口数据结构的时间项
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
			printf("\r\n写烟尘文件4");
			SaveDustData((float *)&input[0],4);
			SdFile_WriteDustFile(&SdFile_Data.InterFace_data,0x04);  //写进4号文件
			break;
		}
		case 13:
		{
		
			break;
		}
		
		case 21:
		{//读取所有传感器的实时采样值
			
			SdFile_Serch_MaxofGasRecord(0x01);    //读取最大记录先,读之前要确定最大值
			printf("1号烟气文件记录数%d",SdFile_Data.GasRecordMax);
		  printf("\r\n读取烟气文件1");
			SdFile_ReadGasFile(0x01,0x01);  //读取1号文件1条记录
						
			strncpy(&buf[0],&SdFile_Data.rdWorkFile.dat[0],Other_length);   //提取读取的时间放到接口数据结构的时间项
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
		{ SdFile_Serch_MaxofGasRecord(0x01);    //读取最大记录先
			printf("1号烟气文件记录数%d",SdFile_Data.GasRecordMax);
			SdFile_Data.GasRecordMax=0;    //使用要配这参数和查找最大文件号
			printf("\r\n写烟气记录1");
			SaveGasData((float *)&input[0],11,0x01);
			SdFile_WriteGasFile(&SdFile_Data.InterFace_data,11,0x01,0x01);  //写进11号文件
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
