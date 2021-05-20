#ifndef SCREENDISCALCULATE__H
#define SCREENDISCALCULATE__H

#include "stm32f4xx.h"

#define Res_Degree0 832   //0��ʱ����ֵ, �����¶�У׼
#define Res_Degree195 1501	//195��ʱ��ֵ

#define CRes_Degree0 100   //0��ʱ����ֵ, �����¶�У׼
#define CRes_Degree195 174	//195��ʱ��ֵ

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
	��������void void CalculationCircularMeasurePoint(float d,float L,u16 *h,float *MeasurePoint)
	������	Բ�β����㹫ʽ
  ����ֱ��������L  
  �������H �Ͳ����MeasurePoint ����
*/
void  CalculationCircularMeasurePoint(u8 flag,float d,float L,u16 *h,u16 *SingleNumber,float *MeasurePoint);

/* date 2020-02-28
	��������void SquareCalculationMeasurePoint(u8 flag,float B,float L,u16 *x,float *MeasurePoint)
	������	���β����㹫ʽ
  ����B�߳��ȣ�����L  
  ������߲����X �Ͳ����MeasurePoint ����
*/
//void SquareCalculationMeasurePoint(u8 flag,float B,float L,u16 *x,float *MeasurePoint);


/* date 2020-02-28
	��������void SquareCalculationMeasurePoint(u8 flag,float B,float L,u16 *x,float *MeasurePoint)
	������	���β����㹫ʽ
  ����B�߳��ȣ�����L  
  ������߲����X �Ͳ����MeasurePoint ����
*/
void SquareCalculationMeasurePoint(u8 Ks,float B,float L,float A,float *MeasurePoint);
void SquareCalculationMeasurePointN(u16 *N,u8 Ks,float B,float L,float A,float *MeasurePoint);
u16 SquareCalculationMeasureHold(float MJ,float A,float B);

/* date 2020-03-02
	��������void SaveFlueData(float *SaveData)
	������	�����̵�����
  ���룺SaveData[14] �ο�File.c �ļ�const char Flue_line��14����30��
*/
void SaveFlueData(float *SaveData);
/* date 2020-03-02
	��������ReadFlueData(u32 FileNum)
	������	��ȡ�̵�����
  ���룺�ļ���
*/
void ReadFlueData(u32 FileNum);//�����ļ��� ��ȡ�̵���Ϣ��
/* date 2020-03-02
	��������void ApplicationFlueFileData (void)
	������	�����ļ�����
  ���룺void
*/
void ApplicationFlueFileData (void);//���õ�ǰ����


/*@lin add 20200310 star*/
void SaveWorkData(float *SaveData,u32 NumFile);
/* date 2020-03-10
	��������ReadFlueData(u32 FileNum)
	������	���湤���ļ�
  ���룺
	
*/

void SaveGasData(float *SaveData,u32 NumFile,u8 attr);

/* date 20200513
	��������SaveGasAverageRecordData(float *SaveData,u32 NumFile)
	������	����ƽ��ֵ����ת��
  ���룺
	
*/
void SaveGasAverageRecordData(float *SaveData,u32 NumFile);  //20200515

//---
 /* date 20200518
	��������SaveGasData(float *SaveData,u32 NumFile,u8 attr)
	������	���������Ļ�׼������,����ϵ������ʣϵ��
  ���룺
	
*/
void SaveGas_Z_J_F_RecordData(void);

/* date 2020-03-10
	��������ReadFlueData(u32 FileNum)
	������	���������ļ�
  ���룺
	
*/

void SaveDustData(float *SaveData,u32 NumFile);
/* date 2020-03-10
	��������ReadFlueData(u32 FileNum)
	������	�����̳��ļ�
  ���룺
	
*/
/*@lin add 20200310 end
*/
/* date 2020-03-18
	��������ReadWorkData(u32 FileNum)
	������ ��ȡ�����ļ�
  ���룺
*/         
void ReadWorkData(u32 FileNum);//�����ļ��� ��ȡ������Ϣ
 /* date 2020-03-10
	��������ReadFlueData(u32 FileNum)
	������	������������װ��
  ���룺
	
*/
 void SaveGasDataLoad(float *SaveData,float *Gas);
 /* date 2020-03-30
	��������ReadGasFirstData(u32 FileNum)
	������ ��ȡ�����ļ� --��һҳ
  ���룺
*/         
void ReadGasFirstData(u32 FileNum);
 /* date 2020-03-30
	��������void GetGasSecondData (void)
	������	��ȡ��ǰ�������� -�ڶ�ҳ
  ���룺void
*/
void GetGasSecondData (u32 FileNum);//��ȡ��ǰ��������

//---
 /* date 20200518
	��������void GetGasSecondData (u32 FileNum)
	������	��ȡ��ǰ����Ũ������
  ���룺void
*/
void GetGasLongDuData(u32 FileNum,float *DisValue);//��ȡ��ǰ����Ũ������

 /* date 2020-03-10
	�������SaveGasINputDataLoad(float *SaveData)
	������	����������������װ��
  ���룺
	
*/

 void SaveGasINputDataLoad(float *SaveData);
 void  GetGasFileRecodData(u32 FileNum,u32 Recod,float * GetData,u8 isDisplay);//��ȡ��ǰ�����¼����
 
  /* date 2020-03-10
	������?void SaveDustDataLoad(float *SaveData)
	������	�����̳�����װ��
  ���룺 
	
*/
 void SaveDustDataLoad(float *SaveData);
  
/* date 20200520
	��������ReadDustFirstData(u32 FileNum)
	������ ��ȡ�̳��ļ�-��һҳ
  ���룺
*/         
void ReadDustData(u32 FileNum);//�����ļ��� ��ȡ�̳���Ϣ
/* date 2020-04-07
	��������ReadDustFirstData(u32 FileNum)
	������ ��ȡ�̳��ļ� --��һҳ
  ���룺
*/        
void ReadDustFirstData(u32 FileNum);
 /* date 2020-04-07
	��������void GetDustSecondData (void)
	������	��ȡ��ǰ�������� -�ڶ�ҳ
  ���룺void
*/

/* date 2020-04-07
	��������ReadLampBlackFirstData(u32 FileNum)
	������ ��ȡ�����ļ� --��һҳ
  ���룺
*/        
void ReadLampBlackFirstData(u32 FileNum);

void GetDustSecondData (u32 FileNum);//��ȡ��ǰ��������
/* date 20200520
	��������u8 GetDustJS_INpuData (u32 FileNum)
	������	��ȡ��ǰ���㵼������
  ���룺FileNum��ǰ�̳��ļ���
	      Screen_Data.DRWJH ������ļ���
*/
u8 GetDustJS_INpuData(u32 FileNum);
 /* date 2020-03-10
	�������SaveGasJS_InputDataLoad(float *SaveData)
	������	���������������
  ���룺
	
*/
 void SaveGasJS_InputDataLoad(float *SaveData);
 //-------------------------------------------------����Ϊ��ӡ�ӿ�---------------------------------------------

/* date 2020-03-25
	��������PrintPressurePara(float *PrintData)
	������	��ӡѹ������
  ���룺
	
*/
void PrintPara(float *PrintData,u8 max);


/* date 2020-03-25
	��������PrintPressurePara(float *PrintData)
	������	��ӡѹ������
  ���룺
	
*/
void PrintPressurePara(float *PrintData);

/* date 2020-03-25
	��������PrintTemperaturePara(float *PrintData)
	������	��ӡ�¶Ȳ���
  ���룺
	
*/
void PrintTemperaturePara(float *PrintData);

/* date 2020-03-25
	��������PrintFlowPara(float *PrintData)
	������	��ӡ��������
  ���룺
	
*/
void PrintFlowPara(float *PrintData);

/* date 2020-03-25
	��������PrintConcentraPara(float *PrintData)
	������	��ӡŨ�Ȳ���
  ���룺
	
*/
void PrintConcentraPara(float *PrintData);

/* date 2020-03-25
	��������PrintCurTest(float *PrintData)
	������	��ӡ���ڲ��Բ���
  ���룺
	
*/
void PrintCurTest(float *PrintData,u8 *Ps,u8 *Is);

/* date 2020-03-25
	�������PPrintWorkFile(float *PrintData)
	������	��ӡ�ļ�����
  ���룺
	
*/
void PrintFile_com(u8 *Ps,float *PrintData);

/* date 2020-03-25
	�������PPrintWorkFile(float *PrintData)
	������	��ӡ�����ļ�
  ���룺
	
*/
void PrintWorkFile(u8 *Ps,float *PrintData);

/* date 2020-03-25
	�������PPrintGasFile(float *PrintData)
	������	��ӡ�����ļ�
  ���룺
	
*/
void PrintGasFile(u8 *Ps,float *PrintData);

/* date 2020-03-25
	�������PPrintDustFile(float *PrintData)
	������	��ӡ�����ļ�
  ���룺
	
*/
void PrintDustFile(u8 *Ps,float *PrintData);

/* date 2020-03-25
	�������PPrintGasRecordFile
	������	��ӡ������¼�ļ�
  ���룺
	
*/
void PrintGasRecordFile(u8 *Ps,float *PrintData);

/* date 2020-03-25
	�������PScreeDisCalculate_DebugASCII(u8 *p,u8 len);
	������	���Ժ���?
  ���룺
	
*/

void ScreeDisCalculate_DebugASCII(u8 *p,u8 len);


/* 	date 2020-05-13 hjl add
	��������void GetKjMm (void)
	������	��ȡ��������
  	���룺void
*/
u16 GetKjMm(void);

/* 	date 2020-05-14 hjl add
	��������void GetRandomNum (void)
	������	��ȡ4λ�����֤��
  	���룺void
*/
u16 GetRandomNum(void);

/*@lin add 20200310 end
*/


/* 	date 20200528
	��������Screen_WaterPumServo(void)
	������	��ˮ�õļ���ŷ�
  	���룺void
*/
void Screen_WaterPumServo(void);

/* 	date 20200528
	��������Screen_WaterPumOpenServo(void); 
	������	����ˮ���߳�
  	���룺void
*/
void Screen_WaterPumOpenServo(void);  //����ˮ���߳�


/* 	date 20200528
	��������Screen_WaterPumCloseServo(void)
	������	�ر�ˮ���߳�
  	���룺void
*/
void Screen_WaterPumCloseServo(void);//�ر�ˮ���߳�
u8 ProcessBlueToothInitial(u32 lPassword);	//����������ʼ��
u8 ProcessE28Initial(u16 adr,u8 ch); //����E28���õ�ַ���ŵ�
u8 TemperatureCaribration(u8 *pWarning, u8 MenuPos); //�����¶�У׼��
void ProcessGetMinuteData(void);// �����¼��������
#endif

