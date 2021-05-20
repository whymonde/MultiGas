#ifndef __FILE_H__
#define __FILE_H__
#include         "sys.h"
#define         LinkFile_Record_Length     100           //�����ļ���¼����
#define         LinkFile_row_length  10                  //���������ļ�ÿ����10�ֽ�
#define         Record_row_length  20                    //ÿ��20���ֽ�
#define         row1_Filenumber             0            //���������ļ���һ�����ļ���
#define         row2_YMD                    1            //���������ļ��ڶ�����������
#define         row3_DHM                    2            //���������ļ��ڶ�������ʱ��

#define Adress_length 60  //�����ַ�г��ȴ��س�����
#define Other_length 30   //���������г���30
#define GasRecord_length 200  //�����������Ӽ�¼������200

#define MAX_car_length 39   //�ļ�����ַ��������������������Ϊ׼
#define Work_car_length 14  //�����ļ�����ַ������14��30�ֽ�
#define Dust_car_length 39  //�̳��ļ�����ַ������39��30�ֽ�
#define Gas_car_length 25  //���ļ�����ַ������25��30�ֽ�
#define Flue_car_length 21  //���ļ�����ַ������21��30�ֽ�   20200527

#define Interf_dat_length 19  //����ӿڵ�ʱ�䳤��
#define Interf_other_length 13  //����ӿڵ����������ݳ���


//    ......��ʱֻ��3��

#define         SampleFile_Record_Length   93                            //�����ļ���¼���� ����SD��
#define         PowerFile_Record_Length    55                            //SD����USB����
#define         ShunshiFile_Record_Length  99                            //�����ļ���¼���ȣ�������USB��
//extern u32 recont_cnt;
typedef struct    //�ļ����ӱ�ṹ
{
	char rowdata[10][LinkFile_row_length];           //�����ά����
}Sfile_link;

typedef union {
	Sfile_link stru;
	char arr[LinkFile_Record_Length]; //û�н�����0��
}Ufile_link;                   //����һ���ļ������ַ����ṹ

typedef struct    //�ļ����ӱ�ṹ
{
	char rowdata[10][Record_row_length];           //�����ά����10x20
}record_str;

typedef union {
	record_str stru;
	char arr[GasRecord_length]; //û�н�����0��
}u_record_str;                   //����һ���ļ������ַ����ṹ

typedef struct    //�����ļ��Ĺ����ļ��ṹ
{ char dat[Other_length];                     //[30]
	char Adress[Adress_length];                 //[60]
	char Other[MAX_car_length][Other_length];   //[39][30]
	u_record_str G_record;                      //�����������ݼ�¼�Ľṹ
}strWorkFile;
/*@lian add 20290302 star*/
typedef struct    //�ļ����ӱ�ṹ
{
	char rowdata[10][5];           //�����ά���� 10*5  10��5λ��
}record_str_Flue;

typedef union {
	record_str_Flue stru;
	char measure_point[GasRecord_length]; //û�н�����0��
}u_record_str_Flue;                   //����һ���ļ������ַ����ṹ

typedef struct    //�����ļ��Ĺ����ļ��ṹ
{ char dat[Other_length];                     //[30]
	char Adress[Adress_length];                 //[60]
	char Other[Flue_car_length][Other_length];  //[21][30]
	u_record_str_Flue FlueMeasurePoint_record;                      //�����������ݼ�¼�Ľṹ
}strFlueFile;
/*@lian add 20290302 end*/
typedef struct    //���������ļ��Ľӿڽṹ
{   char dat[Interf_dat_length];                     //ƽ�������Ŀ�ʼʱ�� 16�ֽ��ַ���
	char T[4];                                     //����ʱ��
	char Adress[Adress_length-9];                 //51�ֽ��ַ���
	char Other[MAX_car_length][Interf_other_length];   //[15][13]  //13���ֽ��ַ���
	char recorddat[Record_row_length-1];   //��¼ʱ��,19���ֽ��ַ���
	char record[9][Record_row_length-10];   //��¼����,10���ֽ��ַ���
}strInterface;
/*add Flue �������*/



typedef struct   
{    u32 recont_cnt;  //�ļ���¼������
	   u16 Timer;
	   Ufile_link file_link_work,file_link_gas,file_link_dust,file_link_buf,file_link_flue;   //����3�������ļ������ݽṹ 
     strWorkFile rdWorkFile,wrWorkFile;   //�����ļ����ݽṹ 
	   strFlueFile FlueFile;
     strInterface InterFace_data;         //�ӿڵ�����	
     u32 GasLinkFileMax,WorkLinkFileMax,DustLinkFileMax,FlueLinkFileMax;  //���弸�������ļ�������ļ�����
	   u32 GasRecordMax;//��������ĳ���ļ���¼����

	   u32 LinkFileMAX;
	   u32 PowerFileMax;
	   u32 SampFileMAX;
	   u32 SampFileNumber;
	   u32 SampFileTime;
	   u32 LosePowerFileNumber;
	   u32 FileTimeMin,FileTimeMax;
	
     char GENG_FILEPATH[80];   //�����ļ���¼������
}SdFile_DataStruct;


SdFile_DataStruct* SdFile_GetData(void);  //
void SdFile_Timer(void);

u8 SdFile_write_record(char *path,char * buff,u32 adrr,u32 length);  //дһ����¼,�Իس����н���
u8 SdFile_read_record(char *path,char * buff,u32 adrr,u32 length); //��һ����¼,�Իس����н���

void SdFile_init_SD_dir(void);
void SdFile_Init(void);

u8 SdFile_ReadLinkRecord(char *path,u32 Filenumber);    //д�����ļ���¼
u8 SdFile_WriteLinkRecord(char *path,u32 Filenumber);   //�������ļ���¼
void SdFile_NumberToDir(char *path,char *pathbuf,u32 NumFile);


void SdFile_makelinkfile_record(u32 NumFile);  //??????????????xxxxxxx

u8 SdFile_ReadWorkFile(u32 NumFile);   //��ȡ�����ļ���¼
u8 SdFile_WriteWorkFile(strInterface *p,u32 NumFile);  //д�빤���ļ���¼

u8 SdFile_ReadGasFile(u32 NumFile,u32 NumRecord);   //��ȡ�����ļ���¼
u8 SdFile_ReadGasRecord(u32 NumFile,u32 NumRecord);  //��ȡ�����ļ���¼
u8 SdFile_WriteGasFile(strInterface *p,u32 NumFile,u8 attr,u8 WriteLink);  //׷�������ļ���¼

u8 SdFile_WriteGasFiletxt(u32 NumFile,u32 record,char txt);  //���ļ�ĳ����ַд���ֽ�20200515
u8 SdFile_ReadGasFiletxt(u32 NumFile,u32 record,char * txt);  //���ļ�ĳ����ַд���ֽ�20200515

u8 SdFile_WriteGasFileAverageRecord(strInterface *p,u32 NumFile);  //��дһ������ƽ��ֵ20200515
u8 SdFile_WriteGasFile_Z_J_F_Record(strInterface *p,u32 NumFile);  //��дһ������ƽ��ֵ20200518
u8 SdFile_ReadDustFile(u32 NumFile);   //��ȡ�̳��ļ���¼
u8 SdFile_WriteDustFile(strInterface *p,u32 NumFile);  //д���̳��ļ���¼

u8 SdFile_ReadFlueFile(u32 NumFile);  //��ȡ�̵��ļ���¼
u8 SdFile_WriteFlueFile(strInterface *p,u32 NumFile);  //д���̵��ļ���¼
u8 SdFile_WriteDustRecord(strInterface *p,u32 NumFile);  //д���̳��ļ���¼(���ļ��ţ�ʱ�䣬�ص��⣩20200523
u8 SdFile_Serch_MaxofFlueLinkFile(void);
u8 SdFile_Serch_MaxOfFile(char *path,u32 * ptr,u32 length);
u8 SdFile_Serch_MaxofGasLinkFile(void);//�������������ļ�����ļ���
u8 SdFile_Serch_MaxofDustLinkFile(void);//�����̳������ļ�����ļ���
u8 SdFile_Serch_MaxofWorkLinkFile(void);   //�������������ļ�����ļ���
u8 SdFile_Serch_MaxofGasRecord(u32 NumFile);  //�����ڼ��������ļ�����¼��
FRESULT SdFile_DeleteDustFiles(void);  //ɾ���̳��ļ�
FRESULT SdFile_DeleteGasFiles(void);  //ɾ�������ļ�
FRESULT SdFile_DeleteWorkFiles(void);  //ɾ�������ļ�
FRESULT SdFile_DeleteFlueFiles(void);  //ɾ�������ļ�
void  SdFile_ramlodair(char *p,u8 length);//���������ո�
void  SdFile_ImportGasData(strInterface *p);
void  SdFile_ImportDustData(strInterface *p);
void  SdFile_ImportWorkData(strInterface *p);
void  SdFile_ImportFlueData(void);

u8 mf_copy(u8 *psrc,u8 *pdst,u8 fwmode);//�ļ�����

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



