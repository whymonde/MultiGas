
#ifndef __PRINTER_H
#define __PRINTER_H

#include "stm32f4xx.h"
//inclde "file.h"  //引用了file.h结构


typedef struct
{
	char databuf[30][11];  //50行,10个字节使用
	
	/***************测量中******
	0: 02浓度
	1: S02浓度
	2: NO浓度
	3: N2O浓度
	4: CO浓度
	5: H2S浓度
	6: CO2浓度
	7:
	8:
	******************************/
	/***************流量参数******
	0:环境温度
	1:大气压
	2:1.0L倍率
	3:20 L倍率
	4:30 L倍率
	5:40 L倍率
	6:50 L倍率
	7:70 L倍率
	8:90 L倍率
	******************************/
	
	/***************压力参数******
	0:气压零点
	1:计压零点
	2:静压零点
	3:动压零点
	4:尘流压零点
	5:气流压零点
	******************************/
	/***************温度参数******
	0:烟温/干球零点
	1:烟温/干球倍率
	2:湿球零点
	3:湿球倍率
	4:计温零点
	******************************/
	/***************浓度参数******
	0:O2零点
	1: O2 低端倍率
	2: O2 中端倍率
	3: O2 高端倍率
	4:SO2     零点
	5:SO2 低端倍率
	6:SO2 中端倍率
	7:SO2 高端倍率
	8: NO     零点
	9: NO 低端倍率
	10: NO 中端倍率
	11: NO 高端倍率
	12:NO2     零点
	13:NO2 低端倍率
	14:NO2 中端倍率
	15:NO2 高端倍率
	16: CO     零点
	17: CO 低端倍率
	18: CO 中端倍率
	19: CO 高端倍率
	20:H2S     零点
	21:H2S 低端倍率
	22:H2S 中端倍率
	23:H2S 高端倍率
	24:CO2     零点
	25:CO2 低端倍率
	26:CO2 中端倍率
	27:CO2 高端倍率
	******************************/
	
}Para_Str;  //打印压力数据结构

typedef struct
{
	u8 RunMode,RunMode_high;
	u16 Timer_high,Timer;
	u8 NextMode,NextMode_high;
	u8 Flag,Flag_high;              //整个打印完标志     
  //u8 ErrFlg,ErrFlg_high;           //故障标志
		
	u16 line;             //记下线程中打印第几行,从1开始
	u16 LineMax;          //记下打印总行数，//打印类别：1打印工况 2 打印烟尘 3,打印烟气，打印烟气分钟数据，4，打印正在测量的数据
	// 5 打印压力校准数据，6 打印温度校准数据，7打印流量校准，8 打印浓度校，不同类别不同

  u8 pselect[33]; //打印选中，1选中，0：不选中
	//  烟气打印选项
	// 0:O2浓度 , 1:过剩/折算,2:烟道截面 ,3:平均动压         
	// 4:SO2浓度, 5:SO2折算  ,6: SO2排放 ,7:平均静压
	// 8:NOx浓度, 9:NOx折算  ,10:NOx排放 ,11:平均流速      
	// 12:NO浓度, 13:NO折算  ,14:NO 排放 ,15:平均烟温   
	// 16:NO2浓度,17:NO2折算 ,18:NO2排放 ,19:含 湿 量
	// 20:CO浓度, 21:CO折算  ,22:CO排放  ,23:烟气流量             
	// 24:H2S浓度,25:H2S折算 ,26:H2S排放 ,27:标杆流量
	// 28:CO2浓度,29:CO2折算 ,30:CO2排放 ,31:负荷系数      
	// 

  //  烟尘打印选项
	// 0:气密性   ,1:烟道截面,2:工况体积,3:O2           
	// 4:跟踪率   ,5:平均动压,6:标况体积,7:SO2
	// 8:皮托管Kp ,9:平均静压,10:烟尘净重,11:NOx      
	//12:采嘴直径,13:平均全压,14:烟尘浓度,15:NO
	//16:平均计温,17:平均流速,18:烟尘折算,19:NO2
	//20:平均计压 21:平均烟温,22:烟尘排放,23:CO             
	//24:累计采时,25:含 湿量,26:烟气流量,27:H2S
	//28:大 气 压,29:负荷系数,30:标杆流量,31:CO2      
	//32:过剩/折算,
	
	
	u8 bianhao[9];  //编号8个字符串
	u8 dat[17];     //导出文件日期
	u8 address[51]; //采样地址
	u8 installselect[8];//安装选择
	//[0]: 02浓度 ,1代表安装了
	//[1]: S02浓度
	//[2]: NO浓度
  //[3]: N2O浓度
	//[4]: CO浓度
	//[5]: H2S浓度
	//[6]: CO2浓度
	u32 print_start_file;
	u32 print_end_file;
	u32 print_cnt_file;  //从print_start_file开始,或
	u32 print_FileNomber;  //打印分钟记录时记下的文件号
	int64_t PrintSelect; //分钟数据-打印选中
	u8  record_Cnt;       //打印选中
	Para_Str Para_StrData; //外部传进来的数据

}Printer_Structure; 

//打印命令组

void Print_Init_Com(void);
void Print_Printorder_Com(void);
void Print_ChangLine_Com(void);
void Print_Chk_Com(void);


Printer_Structure* Printer_GetData(void);
void PrintLine(u16 line);



void PrintAPI_WorkFile(Printer_Structure *P,strWorkFile *Psour);  ////打印工况数据转换到打印缓冲
void PrintAPI_GasFile(Printer_Structure *P,strWorkFile *Psour);  //打印烟尘数据转换到打印缓冲
void PrintAPI_GasRecordFile(Printer_Structure *P,strWorkFile *Psour);//打印烟气记录数据转换到打印缓冲
void PrintAPI_DustFile(Printer_Structure *P,strWorkFile *Psour);   //打印烟尘数据转换到打印缓冲
                //打印正在测试数据转换到打印缓冲
void PrintAPI_PressureParaOrder(Printer_Structure *P);        //打印压力参数数据转换到打印缓冲
void PrintAPI_TemperatureParaOrder(Printer_Structure *P);     //打印温度参数数据转换到打印缓冲
void PrintAPI_FlowParaOrder(Printer_Structure *P);            //打印流量参数数据转换到打印缓冲
void PrintAPI_ConcentraParaOrder(Printer_Structure *P);        //打印浓度参数数据转换到打印缓冲
void PrintAPI_CurTest(Printer_Structure *P);  


//以下为对外接口
void Print_WorkFileOrder(Printer_Structure *P);  //打印工况文件
void Print_GasFileOrder(Printer_Structure *P);    //打印烟气文件
void Print_DustFileOrder(Printer_Structure *P);    //打印烟尘文件
void Print_GasRecordFileOrder(Printer_Structure *P); //打印烟气记录文件
               
void Print_PressureParaOrder(Printer_Structure *P);             //打印压力参数
void Print_TemperatureParaOrder(Printer_Structure *P);          //打印温度参数
void Print_FlowParaOrder(Printer_Structure *P);                 //打印流量参数
void Print_ConcentraParaOrder(Printer_Structure *P);        //打印浓度参数
void Print_CurTestOrder(Printer_Structure *P);   //打印正在测试

void Print_Init(void);
void PrintTimer(void);
void Print_Main(void);
void Print_DebugASCII(u8 *p,u8 len);

#endif

