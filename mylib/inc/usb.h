
#ifndef __USB_H__
#define	__USB_H__	1

#include "sys.h"  
#define DF_FILE_HEAD_LILE	1
#define DF_FILE_CONTENT		2
#define DF_FILE_CLOSED		3



//������USB����״̬
#define DF_USB_IDLE					0   //����
#define DF_USB_INIT					1   //��ʼ��
#define DF_UDISK_ERR				2   //U���д�
#define DF_USB_CONNECT				3 //USB ����
#define DF_USB_CONNECT_ERR			4 //USB ���Ӵ���
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
	u8 step;   //��ǰ����
	u8 NextStep;  //��һ��
	u8 count;
	u16 Timer;
	};

typedef struct 
{
	
	u16 total;
	u8 upstatus;  //�ϴ�״̬��USB ������=1����״̬ҳ=2����ʾ�ļ���=3����ʾ���ҳ=4���ָ���ǰҳ=5��6:�����U����ʾ��0������
	u8 FILE_DATA_BUF[712];
	u32 File_cnt;                //�ļ�������
	u32 Record_cnt;              //��¼������
	u8  usb_out_file_step;   //0Ϊ���У������ж�
	u32 usb_out_start_file;
	u32 usb_out_end_file;

	struct USB_TASK USB_OUT_Gas,USB_OUT_Dust;
	u8 bianhao[9];  //���8���ַ���
	u8 dat[9];      //�����ļ�����
	u8 testflg; //=0 USBæ�� =1 usb����
}Usb_DataStructure;

Usb_DataStructure* Usb_GetData(void);
void Usb_Init(void);
void Usb_Timer(void);

u8 USB_mdir(u8 FileType);

void Usb_OutGasFileInf(u32 start_file,u32 end_file,char *sysDate, char *macNumber);  //�ⲿ���ýӿڣ����������ļ�����
void Usb_OutDustFileInf(u32 start_file,u32 end_file,char *sysDate, char *macNumber);  //�ⲿ���ýӿڣ������̳��ļ�����

void Usb_GasFileOut_TXT(u32 start_file,u32 end_file);  //�������������ļ�����
void Usb_DustFileOut_TXT(u32 start_file,u32 end_file); //���������̳��ļ�����

void Usb_OutGasFileTask(void);
void Usb_OutDustFileTask(void);
void Usb_Main(void);



//void SampleFileOut_CSV(u32 start_file,u32 end_file);
//void PowerFileOut_CSV(u32 start_file,u32 end_file);
//u8 ShunshiFileOut_CSV(u32 start_file,u32 end_file);
void usb_out_sample_file_task(void);
void usb_out_power_file_task(void);
void usb_out_shunshi_file_task(void);

void Usb_DebugASCII(u8 *p,u8 len); //���Ժ���

	

#endif

