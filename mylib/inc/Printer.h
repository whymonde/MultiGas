
#ifndef __PRINTER_H
#define __PRINTER_H

#include "stm32f4xx.h"
//inclde "file.h"  //������file.h�ṹ


typedef struct
{
	char databuf[30][11];  //50��,10���ֽ�ʹ��
	
	/***************������******
	0: 02Ũ��
	1: S02Ũ��
	2: NOŨ��
	3: N2OŨ��
	4: COŨ��
	5: H2SŨ��
	6: CO2Ũ��
	7:
	8:
	******************************/
	/***************��������******
	0:�����¶�
	1:����ѹ
	2:1.0L����
	3:20 L����
	4:30 L����
	5:40 L����
	6:50 L����
	7:70 L����
	8:90 L����
	******************************/
	
	/***************ѹ������******
	0:��ѹ���
	1:��ѹ���
	2:��ѹ���
	3:��ѹ���
	4:����ѹ���
	5:����ѹ���
	******************************/
	/***************�¶Ȳ���******
	0:����/�������
	1:����/������
	2:ʪ�����
	3:ʪ����
	4:�������
	******************************/
	/***************Ũ�Ȳ���******
	0:O2���
	1: O2 �Ͷ˱���
	2: O2 �ж˱���
	3: O2 �߶˱���
	4:SO2     ���
	5:SO2 �Ͷ˱���
	6:SO2 �ж˱���
	7:SO2 �߶˱���
	8: NO     ���
	9: NO �Ͷ˱���
	10: NO �ж˱���
	11: NO �߶˱���
	12:NO2     ���
	13:NO2 �Ͷ˱���
	14:NO2 �ж˱���
	15:NO2 �߶˱���
	16: CO     ���
	17: CO �Ͷ˱���
	18: CO �ж˱���
	19: CO �߶˱���
	20:H2S     ���
	21:H2S �Ͷ˱���
	22:H2S �ж˱���
	23:H2S �߶˱���
	24:CO2     ���
	25:CO2 �Ͷ˱���
	26:CO2 �ж˱���
	27:CO2 �߶˱���
	******************************/
	
}Para_Str;  //��ӡѹ�����ݽṹ

typedef struct
{
	u8 RunMode,RunMode_high;
	u16 Timer_high,Timer;
	u8 NextMode,NextMode_high;
	u8 Flag,Flag_high;              //������ӡ���־     
  //u8 ErrFlg,ErrFlg_high;           //���ϱ�־
		
	u16 line;             //�����߳��д�ӡ�ڼ���,��1��ʼ
	u16 LineMax;          //���´�ӡ��������//��ӡ���1��ӡ���� 2 ��ӡ�̳� 3,��ӡ��������ӡ�����������ݣ�4����ӡ���ڲ���������
	// 5 ��ӡѹ��У׼���ݣ�6 ��ӡ�¶�У׼���ݣ�7��ӡ����У׼��8 ��ӡŨ��У����ͬ���ͬ

  u8 pselect[33]; //��ӡѡ�У�1ѡ�У�0����ѡ��
	//  ������ӡѡ��
	// 0:O2Ũ�� , 1:��ʣ/����,2:�̵����� ,3:ƽ����ѹ         
	// 4:SO2Ũ��, 5:SO2����  ,6: SO2�ŷ� ,7:ƽ����ѹ
	// 8:NOxŨ��, 9:NOx����  ,10:NOx�ŷ� ,11:ƽ������      
	// 12:NOŨ��, 13:NO����  ,14:NO �ŷ� ,15:ƽ������   
	// 16:NO2Ũ��,17:NO2���� ,18:NO2�ŷ� ,19:�� ʪ ��
	// 20:COŨ��, 21:CO����  ,22:CO�ŷ�  ,23:��������             
	// 24:H2SŨ��,25:H2S���� ,26:H2S�ŷ� ,27:�������
	// 28:CO2Ũ��,29:CO2���� ,30:CO2�ŷ� ,31:����ϵ��      
	// 

  //  �̳���ӡѡ��
	// 0:������   ,1:�̵�����,2:�������,3:O2           
	// 4:������   ,5:ƽ����ѹ,6:������,7:SO2
	// 8:Ƥ�й�Kp ,9:ƽ����ѹ,10:�̳�����,11:NOx      
	//12:����ֱ��,13:ƽ��ȫѹ,14:�̳�Ũ��,15:NO
	//16:ƽ������,17:ƽ������,18:�̳�����,19:NO2
	//20:ƽ����ѹ 21:ƽ������,22:�̳��ŷ�,23:CO             
	//24:�ۼƲ�ʱ,25:�� ʪ��,26:��������,27:H2S
	//28:�� �� ѹ,29:����ϵ��,30:�������,31:CO2      
	//32:��ʣ/����,
	
	
	u8 bianhao[9];  //���8���ַ���
	u8 dat[17];     //�����ļ�����
	u8 address[51]; //������ַ
	u8 installselect[8];//��װѡ��
	//[0]: 02Ũ�� ,1����װ��
	//[1]: S02Ũ��
	//[2]: NOŨ��
  //[3]: N2OŨ��
	//[4]: COŨ��
	//[5]: H2SŨ��
	//[6]: CO2Ũ��
	u32 print_start_file;
	u32 print_end_file;
	u32 print_cnt_file;  //��print_start_file��ʼ,��
	u32 print_FileNomber;  //��ӡ���Ӽ�¼ʱ���µ��ļ���
	int64_t PrintSelect; //��������-��ӡѡ��
	u8  record_Cnt;       //��ӡѡ��
	Para_Str Para_StrData; //�ⲿ������������

}Printer_Structure; 

//��ӡ������

void Print_Init_Com(void);
void Print_Printorder_Com(void);
void Print_ChangLine_Com(void);
void Print_Chk_Com(void);


Printer_Structure* Printer_GetData(void);
void PrintLine(u16 line);



void PrintAPI_WorkFile(Printer_Structure *P,strWorkFile *Psour);  ////��ӡ��������ת������ӡ����
void PrintAPI_GasFile(Printer_Structure *P,strWorkFile *Psour);  //��ӡ�̳�����ת������ӡ����
void PrintAPI_GasRecordFile(Printer_Structure *P,strWorkFile *Psour);//��ӡ������¼����ת������ӡ����
void PrintAPI_DustFile(Printer_Structure *P,strWorkFile *Psour);   //��ӡ�̳�����ת������ӡ����
                //��ӡ���ڲ�������ת������ӡ����
void PrintAPI_PressureParaOrder(Printer_Structure *P);        //��ӡѹ����������ת������ӡ����
void PrintAPI_TemperatureParaOrder(Printer_Structure *P);     //��ӡ�¶Ȳ�������ת������ӡ����
void PrintAPI_FlowParaOrder(Printer_Structure *P);            //��ӡ������������ת������ӡ����
void PrintAPI_ConcentraParaOrder(Printer_Structure *P);        //��ӡŨ�Ȳ�������ת������ӡ����
void PrintAPI_CurTest(Printer_Structure *P);  


//����Ϊ����ӿ�
void Print_WorkFileOrder(Printer_Structure *P);  //��ӡ�����ļ�
void Print_GasFileOrder(Printer_Structure *P);    //��ӡ�����ļ�
void Print_DustFileOrder(Printer_Structure *P);    //��ӡ�̳��ļ�
void Print_GasRecordFileOrder(Printer_Structure *P); //��ӡ������¼�ļ�
               
void Print_PressureParaOrder(Printer_Structure *P);             //��ӡѹ������
void Print_TemperatureParaOrder(Printer_Structure *P);          //��ӡ�¶Ȳ���
void Print_FlowParaOrder(Printer_Structure *P);                 //��ӡ��������
void Print_ConcentraParaOrder(Printer_Structure *P);        //��ӡŨ�Ȳ���
void Print_CurTestOrder(Printer_Structure *P);   //��ӡ���ڲ���

void Print_Init(void);
void PrintTimer(void);
void Print_Main(void);
void Print_DebugASCII(u8 *p,u8 len);

#endif

