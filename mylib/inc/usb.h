
#ifndef __USB_H__
#define	__USB_H__	1

#include "sys.h"  
#define DF_FILE_HEAD_LILE	1
#define DF_FILE_CONTENT		2
#define DF_FILE_CLOSED		3



//以下是USB任务状态
#define DF_USB_IDLE					0   //空闲
#define DF_USB_INIT					1   //初始化
#define DF_UDISK_ERR				2   //U盘有错
#define DF_USB_CONNECT				3 //USB 链接
#define DF_USB_CONNECT_ERR			4 //USB 链接错误
#define DF_USB_DISK_STATUS			5
#define DF_USB_DISK_OK				6
#define DF_USB_FREE_CAP				7
#define DF_USB_CREAT_DIR			8
#define DF_USB_CREAT_FILE			9
#define DF_USB_ONE_FILE				10
#define DF_USB_CLOSE_FILE			11
#define DF_USB_OUT_SUCCESS		12



struct USB_TASK
	{
	u8 enable; 
	u8 step;   //当前步骤
	u8 NextStep;  //下一步
	u8 count;
	u16 Timer;
	};

typedef struct 
{
	
	u16 total;
	u8 upstatus;  //上传状态：USB 有问题=1，打开状态页=2，显示文件号=3，显示完成页=4，恢复当前页=5。6:请插入U盘提示；0：空闲
	u8 FILE_DATA_BUF[712];
	u32 File_cnt;                //文件计数器
	u32 Record_cnt;              //记录计算器
	u8  usb_out_file_step;   //0为空闲，用来判断
	u32 usb_out_start_file;
	u32 usb_out_end_file;

	struct USB_TASK USB_OUT_Gas,USB_OUT_Dust;
	u8 bianhao[9];  //编号8个字符串
	u8 dat[9];      //导出文件日期
	u8 testflg; //=0 USB忙， =1 usb空闲
}Usb_DataStructure;

Usb_DataStructure* Usb_GetData(void);
void Usb_Init(void);
void Usb_Timer(void);

u8 USB_mdir(u8 FileType);

void Usb_OutGasFileInf(u32 start_file,u32 end_file,char *sysDate, char *macNumber);  //外部调用接口，导出烟气文件命令
void Usb_OutDustFileInf(u32 start_file,u32 end_file,char *sysDate, char *macNumber);  //外部调用接口，导出烟尘文件命令

void Usb_GasFileOut_TXT(u32 start_file,u32 end_file);  //导出具体烟气文件内容
void Usb_DustFileOut_TXT(u32 start_file,u32 end_file); //导出具体烟尘文件内容

void Usb_OutGasFileTask(void);
void Usb_OutDustFileTask(void);
void Usb_Main(void);



//void SampleFileOut_CSV(u32 start_file,u32 end_file);
//void PowerFileOut_CSV(u32 start_file,u32 end_file);
//u8 ShunshiFileOut_CSV(u32 start_file,u32 end_file);
void usb_out_sample_file_task(void);
void usb_out_power_file_task(void);
void usb_out_shunshi_file_task(void);

void Usb_DebugASCII(u8 *p,u8 len); //调试函数

	

#endif

