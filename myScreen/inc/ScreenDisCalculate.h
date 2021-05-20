#ifndef SCREENDISCALCULATE__H
#define SCREENDISCALCULATE__H

#include "stm32f4xx.h"

#define Res_Degree0 832   //0度时的阻值, 用于温度校准
#define Res_Degree195 1501	//195度时的值

#define CRes_Degree0 100   //0度时的阻值, 用于温度校准
#define CRes_Degree195 174	//195度时的值

typedef enum __tagBLUETOOTH_INIT_STATE_MACHINE
{
	BLUETOOTH_INIT_STATE_MACHINE_INPUT_PASSWORD = 0,
	BLUETOOTH_INIT_STATE_MACHINE_PASSWORD_WAITOK = 1,
	BLUETOOTH_INIT_STATE_MACHINE_SET_MODE = 2,
	BLUETOOTH_INIT_STATE_MACHINE_SET_MODE_WAITOK = 3,
	BLUETOOTH_INIT_STATE_MACHINE_RESET_DEVICE = 4,
	BLUETOOTH_INIT_STATE_MACHINE_RESET_WAITOK = 5,
	BLUETOOTH_INIT_STATE_MACHINE_WAIT_FINISH = 6,
}BLUETOOTH_INIT_STATE_MACHINE;



typedef struct
{
	float A;
	float B;
	
}TEMP_POINT;


/* date 2020-02-28
	函数名：void void CalculationCircularMeasurePoint(float d,float L,u16 *h,float *MeasurePoint)
	描述：	圆形测点计算公式
  输入直径，管套L  
  输出环数H 和测点数MeasurePoint 其中
*/
void  CalculationCircularMeasurePoint(u8 flag,float d,float L,u16 *h,u16 *SingleNumber,float *MeasurePoint);

/* date 2020-02-28
	函数名：void SquareCalculationMeasurePoint(u8 flag,float B,float L,u16 *x,float *MeasurePoint)
	描述：	矩形测点计算公式
  输入B边长度，管套L  
  输出单边测点数X 和测点数MeasurePoint 其中
*/
//void SquareCalculationMeasurePoint(u8 flag,float B,float L,u16 *x,float *MeasurePoint);


/* date 2020-02-28
	函数名：void SquareCalculationMeasurePoint(u8 flag,float B,float L,u16 *x,float *MeasurePoint)
	描述：	矩形测点计算公式
  输入B边长度，管套L  
  输出单边测点数X 和测点数MeasurePoint 其中
*/
void SquareCalculationMeasurePoint(u8 Ks,float B,float L,float A,float *MeasurePoint);
void SquareCalculationMeasurePointN(u16 *N,u8 Ks,float B,float L,float A,float *MeasurePoint);
u16 SquareCalculationMeasureHold(float MJ,float A,float B);

/* date 2020-03-02
	函数名：void SaveFlueData(float *SaveData)
	描述：	保存烟道数据
  输入：SaveData[14] 参考File.c 文件const char Flue_line【14】【30】
*/
void SaveFlueData(float *SaveData);
/* date 2020-03-02
	函数名：ReadFlueData(u32 FileNum)
	描述：	读取烟道数据
  输入：文件号
*/
void ReadFlueData(u32 FileNum);//根据文件号 读取烟道信息；
/* date 2020-03-02
	函数名：void ApplicationFlueFileData (void)
	描述：	采样文件数据
  输入：void
*/
void ApplicationFlueFileData (void);//采用当前数据


/*@lin add 20200310 star*/
void SaveWorkData(float *SaveData,u32 NumFile);
/* date 2020-03-10
	函数名：ReadFlueData(u32 FileNum)
	描述：	保存工况文件
  输入：
	
*/

void SaveGasData(float *SaveData,u32 NumFile,u8 attr);

/* date 20200513
	函数名：SaveGasAverageRecordData(float *SaveData,u32 NumFile)
	描述：	烟气平均值数据转换
  输入：
	
*/
void SaveGasAverageRecordData(float *SaveData,u32 NumFile);  //20200515

//---
 /* date 20200518
	函数名：SaveGasData(float *SaveData,u32 NumFile,u8 attr)
	描述：	保存烟气的基准含氧量,出力系数，过剩系数
  输入：
	
*/
void SaveGas_Z_J_F_RecordData(void);

/* date 2020-03-10
	函数名：ReadFlueData(u32 FileNum)
	描述：	保存烟气文件
  输入：
	
*/

void SaveDustData(float *SaveData,u32 NumFile);
/* date 2020-03-10
	函数名：ReadFlueData(u32 FileNum)
	描述：	保存烟尘文件
  输入：
	
*/
/*@lin add 20200310 end
*/
/* date 2020-03-18
	函数名：ReadWorkData(u32 FileNum)
	描述： 读取工况文件
  输入：
*/         
void ReadWorkData(u32 FileNum);//根据文件号 读取工况信息
 /* date 2020-03-10
	函数名：ReadFlueData(u32 FileNum)
	描述：	保存烟气数据装载
  输入：
	
*/
 void SaveGasDataLoad(float *SaveData,float *Gas);
 /* date 2020-03-30
	函数名：ReadGasFirstData(u32 FileNum)
	描述： 读取烟气文件 --第一页
  输入：
*/         
void ReadGasFirstData(u32 FileNum);
 /* date 2020-03-30
	函数名：void GetGasSecondData (void)
	描述：	获取当前气体数据 -第二页
  输入：void
*/
void GetGasSecondData (u32 FileNum);//获取当前气体数据

//---
 /* date 20200518
	函数名：void GetGasSecondData (u32 FileNum)
	描述：	获取当前气体浓度数据
  输入：void
*/
void GetGasLongDuData(u32 FileNum,float *DisValue);//获取当前气体浓度数据

 /* date 2020-03-10
	函数名aveGasINputDataLoad(float *SaveData)
	描述：	保存输入烟气数据装载
  输入：
	
*/

 void SaveGasINputDataLoad(float *SaveData);
 void  GetGasFileRecodData(u32 FileNum,u32 Recod,float * GetData,u8 isDisplay);//获取当前气体记录数据
 
  /* date 2020-03-10
	函数名?void SaveDustDataLoad(float *SaveData)
	描述：	保存烟尘数据装载
  输入： 
	
*/
 void SaveDustDataLoad(float *SaveData);
  
/* date 20200520
	函数名：ReadDustFirstData(u32 FileNum)
	描述： 读取烟尘文件-第一页
  输入：
*/         
void ReadDustData(u32 FileNum);//根据文件号 读取烟尘信息
/* date 2020-04-07
	函数名：ReadDustFirstData(u32 FileNum)
	描述： 读取烟尘文件 --第一页
  输入：
*/        
void ReadDustFirstData(u32 FileNum);
 /* date 2020-04-07
	函数名：void GetDustSecondData (void)
	描述：	获取当前尘体数据 -第二页
  输入：void
*/

/* date 2020-04-07
	函数名：ReadLampBlackFirstData(u32 FileNum)
	描述： 读取油烟文件 --第一页
  输入：
*/        
void ReadLampBlackFirstData(u32 FileNum);

void GetDustSecondData (u32 FileNum);//获取当前气体数据
/* date 20200520
	函数名：u8 GetDustJS_INpuData (u32 FileNum)
	描述：	获取当前计算导入数据
  输入：FileNum当前烟尘文件号
	      Screen_Data.DRWJH 导入的文件号
*/
u8 GetDustJS_INpuData(u32 FileNum);
 /* date 2020-03-10
	函数名aveGasJS_InputDataLoad(float *SaveData)
	描述：	保存输入计算数据
  输入：
	
*/
 void SaveGasJS_InputDataLoad(float *SaveData);
 //-------------------------------------------------以下为打印接口---------------------------------------------

/* date 2020-03-25
	函数名：PrintPressurePara(float *PrintData)
	描述：	打印压力参数
  输入：
	
*/
void PrintPara(float *PrintData,u8 max);


/* date 2020-03-25
	函数名：PrintPressurePara(float *PrintData)
	描述：	打印压力参数
  输入：
	
*/
void PrintPressurePara(float *PrintData);

/* date 2020-03-25
	函数名：PrintTemperaturePara(float *PrintData)
	描述：	打印温度参数
  输入：
	
*/
void PrintTemperaturePara(float *PrintData);

/* date 2020-03-25
	函数名：PrintFlowPara(float *PrintData)
	描述：	打印流量参数
  输入：
	
*/
void PrintFlowPara(float *PrintData);

/* date 2020-03-25
	函数名：PrintConcentraPara(float *PrintData)
	描述：	打印浓度参数
  输入：
	
*/
void PrintConcentraPara(float *PrintData);

/* date 2020-03-25
	函数名：PrintCurTest(float *PrintData)
	描述：	打印正在测试参数
  输入：
	
*/
void PrintCurTest(float *PrintData,u8 *Ps,u8 *Is);

/* date 2020-03-25
	函数名PrintWorkFile(float *PrintData)
	描述：	打印文件公共
  输入：
	
*/
void PrintFile_com(u8 *Ps,float *PrintData);

/* date 2020-03-25
	函数名PrintWorkFile(float *PrintData)
	描述：	打印工况文件
  输入：
	
*/
void PrintWorkFile(u8 *Ps,float *PrintData);

/* date 2020-03-25
	函数名PrintGasFile(float *PrintData)
	描述：	打印烟气文件
  输入：
	
*/
void PrintGasFile(u8 *Ps,float *PrintData);

/* date 2020-03-25
	函数名PrintDustFile(float *PrintData)
	描述：	打印烟气文件
  输入：
	
*/
void PrintDustFile(u8 *Ps,float *PrintData);

/* date 2020-03-25
	函数名PrintGasRecordFile
	描述：	打印烟气记录文件
  输入：
	
*/
void PrintGasRecordFile(u8 *Ps,float *PrintData);

/* date 2020-03-25
	函数名ScreeDisCalculate_DebugASCII(u8 *p,u8 len);
	描述：	调试函数?
  输入：
	
*/

void ScreeDisCalculate_DebugASCII(u8 *p,u8 len);


/* 	date 2020-05-13 hjl add
	函数名：void GetKjMm (void)
	描述：	获取开机密码
  	输入：void
*/
u16 GetKjMm(void);

/* 	date 2020-05-14 hjl add
	函数名：void GetRandomNum (void)
	描述：	获取4位随机验证码
  	输入：void
*/
u16 GetRandomNum(void);

/*@lin add 20200310 end
*/


/* 	date 20200528
	函数名：Screen_WaterPumServo(void)
	描述：	抽水泵的监控伺服
  	输入：void
*/
void Screen_WaterPumServo(void);

/* 	date 20200528
	函数名：Screen_WaterPumOpenServo(void); 
	描述：	启动水泵线程
  	输入：void
*/
void Screen_WaterPumOpenServo(void);  //启动水泵线程


/* 	date 20200528
	函数名：Screen_WaterPumCloseServo(void)
	描述：	关闭水泵线程
  	输入：void
*/
void Screen_WaterPumCloseServo(void);//关闭水泵线程
u8 ProcessBlueToothInitial(u32 lPassword);	//处理蓝牙初始化
u8 ProcessE28Initial(u16 adr,u8 ch); //处理E28设置地址与信道
u8 TemperatureCaribration(u8 *pWarning, u8 MenuPos); //处理温度校准。
void ProcessGetMinuteData(void);// 处理记录分钟数据
#endif

