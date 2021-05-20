#ifndef __FILE_H__
#define __FILE_H__
#include         "sys.h"
#define         LinkFile_Record_Length     100           //链接文件记录长度
#define         LinkFile_row_length  10                  //定义链接文件每列是10字节
#define         Record_row_length  20                    //每项20个字节
#define         row1_Filenumber             0            //索引链接文件第一列是文件号
#define         row2_YMD                    1            //索引链接文件第二列是年月日
#define         row3_DHM                    2            //索引链接文件第二列是日时分

#define Adress_length 60  //定义地址行长度带回车换行
#define Other_length 30   //定义其他行长度30
#define GasRecord_length 200  //定义烟气分钟记录长度是200

#define MAX_car_length 39   //文件，地址以下是最大行数，已最大为准
#define Work_car_length 14  //工况文件，地址以下是14行30字节
#define Dust_car_length 39  //烟尘文件，地址以下是39行30字节
#define Gas_car_length 25  //烟文件，地址以下是25行30字节
#define Flue_car_length 21  //烟文件，地址以下是21行30字节   20200527

#define Interf_dat_length 19  //定义接口的时间长度
#define Interf_other_length 13  //定义接口的其他行数据长度


//    ......暂时只有3列

#define         SampleFile_Record_Length   93                            //采样文件记录长度 ，在SD用
#define         PowerFile_Record_Length    55                            //SD卡，USB都用
#define         ShunshiFile_Record_Length  99                            //采样文件记录长度，导出到USB用
//extern u32 recont_cnt;
typedef struct    //文件链接表结构
{
	char rowdata[10][LinkFile_row_length];           //定义二维数组
}Sfile_link;

typedef union {
	Sfile_link stru;
	char arr[LinkFile_Record_Length]; //没有结束‘0’
}Ufile_link;                   //定义一个文件链接字符串结构

typedef struct    //文件链接表结构
{
	char rowdata[10][Record_row_length];           //定义二维数组10x20
}record_str;

typedef union {
	record_str stru;
	char arr[GasRecord_length]; //没有结束‘0’
}u_record_str;                   //定义一个文件链接字符串结构

typedef struct    //三种文件的公共文件结构
{ char dat[Other_length];                     //[30]
	char Adress[Adress_length];                 //[60]
	char Other[MAX_car_length][Other_length];   //[39][30]
	u_record_str G_record;                      //定义烟气数据记录的结构
}strWorkFile;
/*@lian add 20290302 star*/
typedef struct    //文件链接表结构
{
	char rowdata[10][5];           //定义二维数组 10*5  10个5位数
}record_str_Flue;

typedef union {
	record_str_Flue stru;
	char measure_point[GasRecord_length]; //没有结束‘0’
}u_record_str_Flue;                   //定义一个文件链接字符串结构

typedef struct    //三种文件的公共文件结构
{ char dat[Other_length];                     //[30]
	char Adress[Adress_length];                 //[60]
	char Other[Flue_car_length][Other_length];  //[21][30]
	u_record_str_Flue FlueMeasurePoint_record;                      //定义烟气数据记录的结构
}strFlueFile;
/*@lian add 20290302 end*/
typedef struct    //定义三种文件的接口结构
{   char dat[Interf_dat_length];                     //平均采样的开始时间 16字节字符串
	char T[4];                                     //采样时长
	char Adress[Adress_length-9];                 //51字节字符串
	char Other[MAX_car_length][Interf_other_length];   //[15][13]  //13个字节字符串
	char recorddat[Record_row_length-1];   //记录时间,19个字节字符串
	char record[9][Record_row_length-10];   //记录数据,10个字节字符串
}strInterface;
/*add Flue 相关数据*/



typedef struct   
{    u32 recont_cnt;  //文件记录计数器
	   u16 Timer;
	   Ufile_link file_link_work,file_link_gas,file_link_dust,file_link_buf,file_link_flue;   //定义3个链接文件的数据结构 
     strWorkFile rdWorkFile,wrWorkFile;   //工况文件数据结构 
	   strFlueFile FlueFile;
     strInterface InterFace_data;         //接口的数据	
     u32 GasLinkFileMax,WorkLinkFileMax,DustLinkFileMax,FlueLinkFileMax;  //定义几个链接文件的最大文件数量
	   u32 GasRecordMax;//定义烟气某个文件记录条数

	   u32 LinkFileMAX;
	   u32 PowerFileMax;
	   u32 SampFileMAX;
	   u32 SampFileNumber;
	   u32 SampFileTime;
	   u32 LosePowerFileNumber;
	   u32 FileTimeMin,FileTimeMax;
	
     char GENG_FILEPATH[80];   //定义文件记录缓冲区
}SdFile_DataStruct;


SdFile_DataStruct* SdFile_GetData(void);  //
void SdFile_Timer(void);

u8 SdFile_write_record(char *path,char * buff,u32 adrr,u32 length);  //写一条记录,以回车换行结束
u8 SdFile_read_record(char *path,char * buff,u32 adrr,u32 length); //读一条记录,以回车换行结束

void SdFile_init_SD_dir(void);
void SdFile_Init(void);

u8 SdFile_ReadLinkRecord(char *path,u32 Filenumber);    //写链接文件记录
u8 SdFile_WriteLinkRecord(char *path,u32 Filenumber);   //读链接文件记录
void SdFile_NumberToDir(char *path,char *pathbuf,u32 NumFile);


void SdFile_makelinkfile_record(u32 NumFile);  //??????????????xxxxxxx

u8 SdFile_ReadWorkFile(u32 NumFile);   //读取工况文件记录
u8 SdFile_WriteWorkFile(strInterface *p,u32 NumFile);  //写入工况文件记录

u8 SdFile_ReadGasFile(u32 NumFile,u32 NumRecord);   //读取烟气文件记录
u8 SdFile_ReadGasRecord(u32 NumFile,u32 NumRecord);  //读取烟气文件记录
u8 SdFile_WriteGasFile(strInterface *p,u32 NumFile,u8 attr,u8 WriteLink);  //追加烟气文件记录

u8 SdFile_WriteGasFiletxt(u32 NumFile,u32 record,char txt);  //向文件某个地址写入字节20200515
u8 SdFile_ReadGasFiletxt(u32 NumFile,u32 record,char * txt);  //向文件某个地址写入字节20200515

u8 SdFile_WriteGasFileAverageRecord(strInterface *p,u32 NumFile);  //再写一次烟气平均值20200515
u8 SdFile_WriteGasFile_Z_J_F_Record(strInterface *p,u32 NumFile);  //再写一次烟气平均值20200518
u8 SdFile_ReadDustFile(u32 NumFile);   //读取烟尘文件记录
u8 SdFile_WriteDustFile(strInterface *p,u32 NumFile);  //写入烟尘文件记录

u8 SdFile_ReadFlueFile(u32 NumFile);  //读取烟道文件记录
u8 SdFile_WriteFlueFile(strInterface *p,u32 NumFile);  //写入烟道文件记录
u8 SdFile_WriteDustRecord(strInterface *p,u32 NumFile);  //写入烟尘文件记录(除文件号，时间，地点外）20200523
u8 SdFile_Serch_MaxofFlueLinkFile(void);
u8 SdFile_Serch_MaxOfFile(char *path,u32 * ptr,u32 length);
u8 SdFile_Serch_MaxofGasLinkFile(void);//搜索烟气链接文件最大文件号
u8 SdFile_Serch_MaxofDustLinkFile(void);//搜索烟尘链接文件最大文件号
u8 SdFile_Serch_MaxofWorkLinkFile(void);   //搜索工况链接文件最大文件号
u8 SdFile_Serch_MaxofGasRecord(u32 NumFile);  //搜索第几个烟气文件最大记录数
FRESULT SdFile_DeleteDustFiles(void);  //删除烟尘文件
FRESULT SdFile_DeleteGasFiles(void);  //删除烟气文件
FRESULT SdFile_DeleteWorkFiles(void);  //删除工况文件
FRESULT SdFile_DeleteFlueFiles(void);  //删除布点文件
void  SdFile_ramlodair(char *p,u8 length);//缓冲区填充空格
void  SdFile_ImportGasData(strInterface *p);
void  SdFile_ImportDustData(strInterface *p);
void  SdFile_ImportWorkData(strInterface *p);
void  SdFile_ImportFlueData(void);

u8 mf_copy(u8 *psrc,u8 *pdst,u8 fwmode);//文件复制

////////////////////////////////////////////////////////////////////////////////

void test_file_write(void);
FRESULT DeleteAllFiles(void);
u8  DeleteFile(TCHAR* FileName);


FRESULT f_deldir(const TCHAR *path);


void clr_power_chaxun(void);
void clr_file_chaxun(void);
void File_DebugASCII(u8 *p,u8 len);
 extern const u8 FOLDER_OF_Gas00[];
#endif



