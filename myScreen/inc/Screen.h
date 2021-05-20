/*
	screen�⺯��
*/

#ifndef __SCREEN_H
#define __SCREEN_H

#include "stm32f4xx.h"
#include <stdbool.h>

//У׼����ѡ��
#define SCREEN_MAINTAIN			//ע�͸ú궨���Ϊ��У׼
//#define SCREEN_PointDebug		//����ʱ��ʾС����λ��

#define SCREE_CRC										//��ĻͨѶ����CRCУ��	
//�����궨��
#define SCREEN_BFLEN								64			//�ṹ��Screen_DataStructure�����Ļ����С
//���̳�������ֵ�궨��
#define SCREEN_KEY_0								0x0030
#define SCREEN_KEY_1								0x0031
#define SCREEN_KEY_2								0x0032
#define SCREEN_KEY_3								0x0033
#define SCREEN_KEY_4								0x0034
#define SCREEN_KEY_5								0x0035
#define SCREEN_KEY_6								0x0036
#define SCREEN_KEY_7								0x0037
#define SCREEN_KEY_8								0x0038
#define SCREEN_KEY_9								0x0039
#define SCREEN_KEY_Point							0x002E														
#define SCREEN_KEY_A								0x0041
#define SCREEN_KEY_B								0x0042
#define SCREEN_KEY_C								0x0043
#define SCREEN_KEY_D								0x0044
#define SCREEN_KEY_E								0x0045
#define SCREEN_KEY_F								0x0046
#define SCREEN_KEY_G								0x0047
#define SCREEN_KEY_H								0x0048
#define SCREEN_KEY_I								0x0049
#define SCREEN_KEY_J								0x004A
#define SCREEN_KEY_K								0x004B
#define SCREEN_KEY_L								0x004C
#define SCREEN_KEY_M								0x004D
#define SCREEN_KEY_N								0x004E
#define SCREEN_KEY_O								0x004F
#define SCREEN_KEY_P								0x0050
#define SCREEN_KEY_Q								0x0051
#define SCREEN_KEY_R								0x0052
#define SCREEN_KEY_S								0x0053
#define SCREEN_KEY_T								0x0054
#define SCREEN_KEY_U								0x0055
#define SCREEN_KEY_V								0x0056
#define SCREEN_KEY_W								0x0057
#define SCREEN_KEY_X								0x0058
#define SCREEN_KEY_Y								0x0059
#define SCREEN_KEY_Z								0x005A
#define SCREEN_KEY_OK								0x00F1
#define SCREEN_KEY_ESC								0x00F0
#define SCREEN_KEY_Back								0x00F2
#define SCREEN_KEY_Blank							0x0020					

//������غ궨��
#define SCREEN_FACTORY							0XAA		//�ظ�����������
#define SCREEN_KEYADD							0X0001	//��ֵ������ַ
#define SCREEN_SAMPLE_ADRESS					0X0200	//������ַ������ַ

#define SCREEN_KEYADD_ESC						0X0002	//������ESC��������ֵ��ַ



//����ֵ�궨��
#define SCREEN_NONE_KEY							0//��
#define SCREEN_UP_KEY							10//����
#define SCREEN_R_KEY							11//����
#define SCREEN_SW_KEY							12//�л�
#define SCREEN_ESC_KEY							13//�˳�
#define SCREEN_OK_KEY							14//ȷ��

/********************************************    ��������ʾ�ؼ��ķ��ص�ַ   ***************************************************************/
/********************************** 0-��������ҳ ********************************************/
#define SCREEN_ADD_KJMM  						0x9001			//����	2020-05-20 hjl add	

/********************************** 1-ϵͳ����ҳ ********************************************/
#define SCREEN_ADD_RIQI							0x9111			//����						*/																																	
#define SCREEN_ADD_SHIJIAN  					0x9112			//ʱ��						*/
#define SCREEN_ADD_DAQIYA  						0x9113			//����ѹ					*/
#define SCREEN_ADD_PSJG  						0x9118			//��ˮ���					*/
#define SCREEN_ADD_PSSS  						0x9119			//��ˮʱ��					*/
//#define SCREEN_ADD_YC_MIN						0x9121			//������������				*/
//#define SCREEN_ADD_YC_TIMES					0x9122			//��������					*/
#define SCREEN_ADD_HSL_MIN						0x911A			//��ʪ������				*/
/********************************************************************************************/
/********************************** 2-�̵����� **********************************************/
#define SCREEN_ADD_CIRCLE_DIAMETER  			0x9211			//1-Բ��					*/																																	
#define SCREEN_ADD_CIRCLE_AREA  				0x9212			//							*/
#define SCREEN_ADD_CIRCLE_PIPE		 			0x9213			//							*/
#define SCREEN_ADD_CIRCLE_RING  				0x9214			//							*/
#define SCREEN_ADD_CIRCLE_HOLE  				0x9215			//							*/
#define SCREEN_ADD_RECT_A  						0x9221			//2-����					*/																																	
#define SCREEN_ADD_RECT_B  						0x9222			//							*/
#define SCREEN_ADD_RECT_AREA		 			0x9223			//							*/
#define SCREEN_ADD_RECT_PIPE  					0x9224			//							*/
#define SCREEN_ADD_RECT_HOLE  					0x9225			//							*/
#define SCREEN_ADD_RECT_TESTPOINT  				0x9226			//							*/
#define SCREEN_ADD_CROSS_SECTION				0x9231			//3-�������̵�����			*/
#define SCREEN_ADD_SAMPLES  					0x9232			//							*/
#define SCREEN_ADD_FLUE_FIND  					0x9241			//							*/
/********************************************************************************************/
/********************************** 3-����Ԥ�� **********************************************/
#define SCREEN_ADD_YANWENG				  		0x9321			//2-���²���				*/																																	
#define SCREEN_ADD_FILE_CHECK				  	0x9341			//4-��ѯ�ļ���				*/
/********************************************************************************************/
/********************************** 4-�⺬ʪ�� **********************************************/
#define SCREEN_ADD_MOISTURE				  		0x9411			//1-��ʪ������				*/																																	
/********************************************************************************************/
/********************************** 5-�������� **********************************************/
#define SCREEN_ADD_YC_MIN					  	0x9501			//1-��ƽ��ʱ��				*/
#define SCREEN_ADD_YC_TIMES					  	0x9502			//1-��ƽ������				*/
#define	SCREEN_ADD_GAS_FIND						0x9521			//1-������ѯ				*/
#define	SCREEN_ADD_CALC_FACTOR					0x9561			//1-����ϵ��				*/
#define	SCREEN_ADD_OXYGEN_BASE					0x9562			//2-��׼����				*/
#define	SCREEN_ADD_LOAD_FACTOR					0x9563			//3-����ϵ��				*/
#define	SCREEN_ADD_O2_INPUT						0x95A1			//1-O2						*/
#define	SCREEN_ADD_SO2_INPUT					0x95A2			//2-SO2						*/
#define	SCREEN_ADD_NO_INPUT						0x95A3			//3-NO						*/
#define	SCREEN_ADD_NO2_INPUT					0x95A4			//4-NO2						*/
#define	SCREEN_ADD_CO_INPUT						0x95A5			//5-CO						*/
#define	SCREEN_ADD_H2S_INPUT					0x95A6			//6-H2S						*/
#define	SCREEN_ADD_CO2_INPUT					0x95A7			//7-CO2						*/
#define	SCREEN_ADD_START_NUMBER					0x9571			//1-��ʼ�ļ���				*/
#define	SCREEN_ADD_END_NUMBER					0x9572			//2-�����ļ���				*/
/********************************************************************************************/
/********************************** 6-�̳����� **********************************************/
#define SCREEN_ADD_CC_SAMPLE			  		0x9611			//1-��������				*/
#define SCREEN_ADD_CARTRIDGE_NUMBER  			0x9612			//2-��Ͳ��					*/
#define SCREEN_ADD_TIME_MINUTE			 		0x9613			//3-�����ʱ����			*/
#define SCREEN_ADD_TIME_SECOND			 		0x9614			//4-�����ʱ����			*/
#define SCREEN_ADD_MOUTH_DIAMETER	  			0x9615			//5-����ֱ��				*/
#define SCREEN_ADD_TOTAL_SAMPLE			  		0x9616			//6-�ܲɵ�					*/
/********************************************************************************************/
/********************************** 7-�ļ���ѯ **********************************************/
#define	SCREEN_ADD_OILGAS_FIND					0x9721			//1-���̲�ѯ				*/
#define	SCREEN_ADD_CONCENTRATIO					0x9761			//1-��ҺŨ��				*/
#define	SCREEN_ADD_VOLUME						0x9762			//2-�������				*/
#define	SCREEN_ADD_RANGES_NUMBER				0x9763			//3-��ͷ��Ŀ				*/
#define	SCREEN_ADD_START_NUMBER2				0x9771			//1-��ʼ�ļ���				*/
#define	SCREEN_ADD_END_NUMBER2					0x9772			//2-�����ļ���				*/
#define	SCREEN_ADD_DUST_WEIGHT					0x97A1			//1-�̳�����				*/
#define	SCREEN_ADD_DUST_OXYGEN					0x97A2			//2-������					*/
#define	SCREEN_ADD_DUST_LOAD_FACTOR				0x97A3			//3-����ϵ��				*/
#define	SCREEN_ADD_DUST_CALC_FACTOR				0x97A4			//4-����ϵ��				*/
#define	SCREEN_ADD_DUST_OXYGEN_BASE				0x97A5			//5-��׼������				*/
#define	SCREEN_ADD_DUST_FILE_NUM				0x97A6			//6-�����ļ���				*/
/********************************************************************************************/
/********************************** 9-У׼ά�� **********************************************/
#define SCREEN_ADD_PASSWORD_INPUT				0x9801			//��������					*/
//1-ϵͳ����																				*/
#define SCREEN_ADD_CALIB_NOx				  	0x9811			//NO2/NOX					*/
#define SCREEN_ADD_CO_SO2						0x9812			//COvsSO2					*/
#define SCREEN_ADD_NO2_SO2					  	0x9813			//NO2vsSO2					*/
#define SCREEN_ADD_PTG_Kp						0x9814			//Ƥ�й�Kp					*/
#define SCREEN_ADD_SMOKE_DENSITY		  		0x9815			//�����ܶ�					*/
#define SCREEN_ADD_CALC_RATIO				  	0x9816			//����ϵ��					*/
#define SCREEN_ADD_OXYGEN_REF				  	0x9817			//��׼������				*/
//1-ϵͳ����-CO vs SO2																		*/
#define SCREEN_ADD_COSO2_TEXT01				  	0x98A1			// 							*/
#define SCREEN_ADD_COSO2_TEXT02				  	0x98A2			// 							*/
#define SCREEN_ADD_COSO2_TEXT03				  	0x98A3			// 							*/
#define SCREEN_ADD_COSO2_TEXT04				  	0x98A4			// 							*/
#define SCREEN_ADD_COSO2_TEXT05				  	0x98A5			// 							*/
#define SCREEN_ADD_COSO2_TEXT06				  	0x98A6			// 							*/
#define SCREEN_ADD_COSO2_TEXT07				  	0x98A7			// 							*/
#define SCREEN_ADD_COSO2_TEXT08				  	0x98A8			// 							*/
#define SCREEN_ADD_COSO2_TEXT09				  	0x98A9			// 							*/
#define SCREEN_ADD_COSO2_TEXT0A				  	0x98AA			// 							*/
#define SCREEN_ADD_COSO2_TEXT0B				  	0x98AB			// 							*/
#define SCREEN_ADD_COSO2_TEXT0C				  	0x98AC			// 							*/
#define SCREEN_ADD_COSO2_TEXT0D				  	0x98AD			// 							*/
#define SCREEN_ADD_COSO2_TEXT0E				  	0x98AE			// 							*/
#define SCREEN_ADD_COSO2_TEXT0F				  	0x98AF			// 							*/
#define SCREEN_ADD_COSO2_TEXT10				  	0x98B0			// 							*/
#define SCREEN_ADD_COSO2_TEXT11				  	0x98B1			// 							*/
#define SCREEN_ADD_COSO2_TEXT12				  	0x98B2			// 							*/
#define SCREEN_ADD_COSO2_TEXT13				  	0x98B3			// 							*/
#define SCREEN_ADD_COSO2_TEXT14				  	0x98B4			// 							*/
//2-����У׼																				*/
#define SCREEN_ADD_STD_FLOW						0x9821			//��׼����					*/
#define SCREEN_ADD_RATIO_TEXT02				  	0x9822			// 20-����					*/
#define SCREEN_ADD_RATIO_TEXT03				  	0x9823			// 30-����					*/
#define SCREEN_ADD_RATIO_TEXT04				  	0x9824			// 							*/
#define SCREEN_ADD_RATIO_TEXT05				  	0x9825			// 							*/
#define SCREEN_ADD_RATIO_TEXT06				  	0x9826			// 							*/
#define SCREEN_ADD_RATIO_TEXT07				  	0x9827			// 							*/
#define SCREEN_ADD_RATIO_TEXT08				  	0x9828			// 							*/
#define SCREEN_ADD_RATIO_TEXT09				  	0x9829			// 							*/
#define SCREEN_ADD_RATIO_TEXT0A				  	0x982A			// 							*/
#define SCREEN_ADD_RATIO_TEXT0B				  	0x982B			// 							*/
#define SCREEN_ADD_RATIO_TEXT0C				  	0x982C			//�¶�						*/
#define SCREEN_ADD_RATIO_TEXT0D				  	0x982D			//����ѹ					*/
//3-��������																				*/
#define SCREEN_ADD_ORIG_PASS					0x9831			//ԭ����					*/
#define SCREEN_ADD_NEW_PASS				  		0x9832			//������					*/
#define SCREEN_ADD_CONFIRM_PASS				  	0x9833			//ȷ������					*/
#define SCREEN_ADD_PRINT_PASS				  	0x9834			//��ӡ�������� 				*/
#define SCREEN_ADD_APP_PASS				  		0x9835			//APP��������				*/
#define SCREEN_ADD_E28ADR_PASS                  0x9836          //E28��ַ
#define SCREEN_ADD_E28CH_PASS                   0x9837          //E28�ŵ�
//4-Ũ��У׼																				*/
#define SCREEN_ADD_O2_TEXT01					0x9841			//O2���					*/
#define SCREEN_ADD_SO2_TEXT02				  	0x9842			//SO2���					*/
#define SCREEN_ADD_NO_TEXT03				  	0x9843			//NO���					*/
#define SCREEN_ADD_NO2_TEXT04				  	0x9844			// 							*/
#define SCREEN_ADD_CO_TEXT05				  	0x9845			//							*/
#define SCREEN_ADD_H2S_TEXT06				  	0x9846			// 							*/
#define SCREEN_ADD_CO2_TEXT07				  	0x9847			// 							*/
//4-Ũ��У׼->��������																		*/
#define SCREEN_ADD_MID_RATIO				  	0x984A			//�ж˱���K 				*/
#define SCREEN_ADD_LOW_RATIO				  	0x984B			//�Ͷ˱���K1				*/
#define SCREEN_ADD_HIGH_RATIO				  	0x984C			//�߶˱���K2				*/
#define SCREEN_ADD_MID_STD				  		0x984D			//�ж˱���Ũ��				*/
#define SCREEN_ADD_LOW_STD				  		0x984E			//�Ͷ˱���Ũ��				*/
#define SCREEN_ADD_HIGH_STD				  		0x984F			//�߶˱���Ũ��				*/
//5-ѹ��У׼																				*/
#define SCREEN_ADD_AIR_PRESSURE			  		0x9851			//1-����ѹ	 				*/
#define SCREEN_ADD_CALC_PRESSURE		  		0x9852			//2-��ѹ					*/
#define SCREEN_ADD_STATIC_PRESSURE	  			0x9853			//3-��ѹ					*/
#define SCREEN_ADD_DYNAMIC_PRESSURE  			0x9854			//4-��ѹ					*/
#define SCREEN_ADD_DUST_PRESSURE	  			0x9855			//5-��ѹ					*/
#define SCREEN_ADD_FLOW_PRESSURE	  			0x9856			//6-����ѹ					*/
//7-�¶�У׼																				*/
#define SCREEN_ADD_DRY_BALL			  			0x9871			//1-���¸������	 		*/
#define SCREEN_ADD_WET_BALL		  				0x9872			//2-ʪ�����				*/
#define SCREEN_ADD_CALC_TEMP	  				0x9873			//3-�������				*/
#define SCREEN_ADD_CIRCLE_TEMP  				0x9874			//4-�������				*/
#define SCREEN_ADD_DRY_RATIO	  				0x9875			//5-������				*/
#define SCREEN_ADD_WET_RATIO	  				0x9876			//6-ʪ����				*/
//7-��ر궨																				*/
#define SCREEN_ADD_BATTERY_STD			  		0x9881			//1-��ر궨				*/
//����ά��																					*/																								*/
#define SCREEN_ADD_INSTRUMENT_NUM		  		0x9A11			//1-�������				*/
#define SCREEN_ADD_O2_DATE				  		0x9A21			//1-O2��װ����				*/
#define SCREEN_ADD_SO2_DATE		  				0x9A22			//2-SO2��װ����				*/
#define SCREEN_ADD_NO_DATE				  		0x9A23			//3-NO��װ����				*/
#define SCREEN_ADD_NO2_DATE		  				0x9A24			//4-NO2��װ����				*/
#define SCREEN_ADD_CO_DATE				  		0x9A25			//5-CO��װ����				*/
#define SCREEN_ADD_H2S_DATE		  				0x9A26			//6-H2S��װ����				*/
#define SCREEN_ADD_CO2_DATE		  				0x9A27			//7-CO2��װ����				*/

#define SCREEN_ADD_O2_LQ				  		0x9A28			//1-O2��װ����				*/
#define SCREEN_ADD_SO2_LQ		  				0x9A29			//2-SO2��װ����				*/
#define SCREEN_ADD_NO_LQ				  		0x9A2A			//3-NO��װ����    		*/
#define SCREEN_ADD_NO2_LQ		  				0x9A2B			//4-NO2��װ����   		*/
#define SCREEN_ADD_CO_LQ				  		0x9A2C			//5-CO��װ����	    	*/
#define SCREEN_ADD_H2S_LQ		  				0x9A2D			//6-H2S��װ���� 			*/
#define SCREEN_ADD_CO2_LQ		  				0x9A2E			//7-CO2��װ����  			*/




/********************************************************************************************/



//ʱ����غ궨��
#define SCREEN_YearBase							1900//���ֵ��1900�꣬��ʾʱ����Ҫ���Ӹ�ֵ
#define SCREEN_TimeZone							8		//ʱ����������
#define SCREEN_YearScreen						100 //��Ļʱ������time.c��ת����ֵ


//Ӳ���˿ں궨��
#define SCREEN_SendBufferData	 			USARTx_StoreBufferData			//RTU���ݷ��ͺ���
#define SCREEN_UsartPort						USART3											//��Ӧ��������˿�
#define SCREEN_UsartBuffer					Get_USART3Buffer()					//��Ӧ���ڽṹ��
//
//��Ļ����ʱ��
typedef struct
{
	struct tm Clock;
	u8 Flag;				//b0�������ʱ���ã�B1=1��ʾ�ѻ�ȡ��ʱ�����ݣ�B2=1��ʾ�ϴε��������ѱ��棬���Դ��»�ȡ��ʱ��
	u8 ClockSec;		//�������ʱ��
	u16 Msec;				//��ʱ�Ӷ�ʱ������λ������
}Screen_TimeStructure;

typedef enum
{
	E28_SET_STATE_MACHINE_SLEEP=0,
	E28_SET_STATE_MACHINE_INIT=1,
	E28_SET_STATE_MACHINE_WAITOK=2,
	E28_SET_STATE_MACHINE_FINISH=3,
}E28_SET_STATE_MACHINE;

typedef struct
{
	u32 uCounter1S;					//��ʱ��
	u32 uCounter1ms;				//1ms������
	u32 uCounter100ms;				//100ms������
	bool bTime100ms;				//100ms������־λ
	bool bTime1S;
	u8 ucCurrentPage;
	u8 uMinDataPage;				//��������ҳ
	u8 cIsCalcAverage;				//���ڼ�ƽ����
	u8 cAverageCount;				//ƽ���������㣻
	u32 Timer;
	double JpjSum[8];               //�ۼӼ�ƽ��
	u16    JpjSuncnt;            //�ۼӴ���
    u8 JpjEndFlg;                //ʱ����ɱ��
}Screen_JPGStructure;

//������ؽṹ��
typedef struct
{
	u8 KeyData;					//ʵ�尴��ֵ
	u8 Opt;							//��ǰҳ��ѡ�ж���(һ�������о������)
	u8 LastOpt;					//��ǰҳ����һ�ζ���(һ�������о������)
	u8 OldOpt;					//��һ����ѡ�ж���
	u8 TwoOpt;					//��������
	u8 Mode;						//0:һ������>=1:��������ľ������
	u8 LastMode;				//0:һ������>=1:��������ľ������
	u8 State;						//����������˸
	u8 Buffer[32];			//������
	u8 number;          //���水������
	u8 KeyDir;             //�����Ӽ�����
	u8 bOutsideKeyboard;	//�ⲿ����
}Screen_keyStructure;

typedef struct
{ u32 Printwho;                         //��ӡ�����ļ�1:����; 2:˲ʱ;3:����
	u32 PDatashunshi;                    //���˲ʱ�ļ����ļ���
	u32 PDataFrom,PDataTo;               //��ӡ���ļ���ʼ������  ,���
  u32 PDatacurrent;	                   //���ڴ�ӡ�ĺ���
	u32 SampTimeFrom,SampTimeTo;          //�����ļ���ʼʱ��,����ʱ��
	u32 SampNumberFrom,SampNumberTo;      //��ʼ�ļ��ţ������ļ���
	u32 SunshiFileNumber;                 //˲ʱ�ļ���
	u32 SunShiNumberFrom,SunShiNumberTo;  //˲ʱ��ʼ���кţ�˲ʱ�������к�
	u32 LosePowerNumberFrom,LosePowerNumberTo;  //���翪ʼ�ļ��ţ���������ļ���
}Screen_Page58Structure;

typedef union __tagSDFileStateBit
{
	struct
	{
		unsigned int m_uDustFile		:1;		//
		unsigned int m_uGasFile			:1;		//
		unsigned int m_uWorkFile		:1;		//
		unsigned int m_uAllFile			:1;
		unsigned int m_data0			:1;		
		unsigned int m_factorySet		:1;		//��������
	}BitField;
	u8 m_byData;
}SysConfigWaringBits;

//��Ļ�ṹ��2020-05-21
/*
	�޸ļ�¼��2020-05-25 modify by alfred�� ɾ�����õ�uTempKeymode
*/
typedef struct
{
	u16 RunMode;				//����ģʽ
	u8 RunModeStat;		//����ģʽ ״̬
	u16 NextMode;				//����ģʽ
	u16 OldMode;				//��ģʽ
	u16 Button;					//��Ļ����ֵ
	u8 Cnt;							//������
	u8 SubCnt;          //�ֲ�������
	u8 Con;							//��ֵ
	//u8 DataBuffer[32];	//���ݻ��棬ֻ��һ���ӳ�������Ч
	u8 Buffer[SCREEN_BFLEN];	//����
	u8 Page;						//��ǰϵͳ��ʾҳ��
	u8 OldPage;					//��һ����
	u16 Timer;					//��ʱ��
	u8 CT;                     //��ʱ���ļ�����
	u16 AgvTimer;					//ƽ��ʱ��--��������ʹ��
	u16 AgvTimes;					//ƽ������--��������ʹ��
	u16 TimerSecUp;					//��ʱ��
	
	u16 LastTimerSecUp;					//�ϴζ�ʱ��
	u16 LastTimerMinUp;					//�ϴζ�ʱ��
	u16 TimerSec;        //�붨ʱ��
	u16 BacklightSecTimer;//������Ƽ�ʱ��
	Screen_TimeStructure Clock;//ʱ��
	Screen_keyStructure Key;//����
//	Screen_Page58Structure Screen_Page58;
//  screen

	u16 sValScreenKeyEsc; 			//������ESC����ֵ
	u16 sReveiveDataTimer;   //����ȷ�Ϻ�ȴ��յ����ݣ������жϳ�ʱ
	u8 uEnableRecDataTimer;  //������ʱ��
	u16	sRcvType;            //����ȷ�Ϻ���Ļ�յ������ݵķ�������
	u8 uTempState;		//��ʱ״̬ȫ�ֱ���, ���ڲ�ͬ����֮�����ݴ���,ʹ������������ //2020-05-30
	
	u8 uE28CtlState;   //����E28��������־����ɹ���
	u16 uE28Timer;     //����E28�ļ�ض�ʱ�����ж��Ƿ�ʱ
	E28_SET_STATE_MACHINE g_E28_Set_StateMachine;  //E28��״̬���Ĵ���
	
	Screen_JPGStructure JiPingJun;

	u16 BatflshTimer;   //��ض�̬��ʱ��
	u8  Batflshcnt;     //��ض�̬������
	u8 ScreenButtonLock;// ������
	u8 OldScreenButtonLock;// ������
	u8 Warning;          //����
	u8 Function;          //����  00ֹͣ 01��ʼȷ��
	u8 LastWarning;          //�ϴξ���
	u8 LastFunction;          //�ϴι���  00ֹͣ 01��ʼȷ��
	u32 LastChoice;        //�ϴ�ѡ��
	u8 LastCoiceonoff;     //�ϴ�����/ֹͣѡ��
	u8 LoadFlag;             //�������װ������
	u8 StarCun;              //��һ��ʼ���� 
	u16 SampleCun;              //����������ǰ����
	u8 RecordCun;           //Gas ��¼  ����ƽ��ʱ��
	u8 LastRecordCun;           //Gas ��¼
	uint64_t UseGasCal;          //��������ƽ�����㣨��λ��ȷ����
	u8 PauseOrStop;                 //ͨ����ͣ��־ ==B0��ʾ��ͣ ==BO������
	u8 YC_Pause;                 //�̳������ ���ڵ����������ʱ������� ��ͣ��־ ==B0��ʾ��ͣ ==BO������
	u8 YC_Type;                 //�̳���������
	u8 QimiStat;             //����δ���==0  ���ʧ��==1 ���ɹ�==2
	u32 YC_SampleAllTime;    //�̳�������ʱ��
	u8 YC_Stat;        //�̳�״̬ ==1 ��ʾ�ڽ����̳�����
	u16 YC_SampleTimerSec;        //�̳������붨ʱ��
	float YC_FollowRate;        //������
	float YC_TrageFlux;         //Ŀ������
	float FileNumber;
	float DisValue[40];
	u16 RandomNum;		//����� 2020-05-19 hjl add 
	float ZSXS;                //����ϵ��           ��������ѯ����ҳ����
	float JZHYL;               //��׼������
	float FHXS;                //����ϵ��
	float GSXS;                //��ʣϵ��
	float HYL;                 //������
	float YCJZ;                //�̳�����
	u32   DRWJH;               //�����ļ���
	float RYLD;                //��ҺŨ��
	float DRTZ;                //�������
	float ZTSM;                //��ͷ��Ŀ
	u8 WaterPumRunmod;         //��ˮ������ģʽ
	u32 WaterPumTimer;         //��ˮ�ö�ʱ��
	u16 ChangTimer;            //��ض�ʱ��
	u8 staflg;                 //�����ԼӶ�ʱ����־
	u16 ZjTimer;               //�ԼӶ�ʱ��
	u8 gzdselect;              //���е�ѡ��
	u8 Scan_status;            //����¼��״̬b0=0������Ļû¼��״̬,b0=1������¼��״̬�� B1=1,������⣬B1=0,���������
	u16 Scan_Stimer;           //��ʱ��
	SysConfigWaringBits gSysConfigWaringBits;	//�����ļ�ɾ��״̬��ʾ
	u8 mystatus;              //�ֲ���״̬
	char recorddat[20];   //��¼ʱ��,
	char recorstarddat[20];   //��¼��ʼ����ʱ�䣬�ؼ���¼��ƽ����ʱ��
	u8 uMenuPosition;
}Screen_DataStructure;



//----------------------------------------------------
void Screen_Main(void);
void Screen_Timer(void);
void Screen_SecTimer(void);
void Screen_Init(void);
Screen_DataStructure* Screen_GetData(void);

void Screen_MainPage(Screen_DataStructure *p);
void Screen_SystemSet(Screen_DataStructure *p);
void Screen_FlueSet(Screen_DataStructure *p);
void Screen_SamplingA(Screen_DataStructure *p);
void Screen_SamplingB(Screen_DataStructure *p);
void Screen_SamplingTSP(Screen_DataStructure *p);
void Screen_MaintainPressureSet(Screen_DataStructure *p);
void Screen_MaintainTemSet(Screen_DataStructure *p);
void Screen_MaintainFlowSet(Screen_DataStructure *p);
void Screen_MaintainSystemSet(Screen_DataStructure *p);
void Screen_SuperParameterSet(Screen_DataStructure *p);
void Screen_SuperPIDSet(Screen_DataStructure *p);
void Screen_SuperBackupsSet(Screen_DataStructure *p);
void Screen_SuperNumberSet(Screen_DataStructure *p);
void Screen_SamplingInquire(Screen_DataStructure *p);
void Screen_SamplingPrint(Screen_DataStructure *p);
void Screen_USBFlashExport(Screen_DataStructure *p);
void Screen_NetworkSettings(Screen_DataStructure *p);
void Screen_KeyInput(Screen_DataStructure *p);
void Screen_KeyBoard(Screen_DataStructure *p);
void Screen_Backlight(Screen_DataStructure *p);
void Screen_BacklightDW(void);
void Screen_SearchPrinter(Screen_DataStructure *p);//������ӡ�� 
void Screen_PowerDowm(Screen_DataStructure *p);//�ػ�	
void Screen_changPowerState(void);	
	
void Screen_ShowPage(u16 page);
void Screen_RefreshDataMulti(u16 address,u8 *p,u8 len);
/*
	��������void Screen_RefreshVarData(u16 address,u16 data)
	����������Ļ�����洢������λͼѡ��
	���룺address ��ַ
				data����
			
*/
void Screen_RefreshVarData(u16 address,u16 data);
void Screen_ShowScreenLockState(void);
void Screen_Reset(void);
void Screen_ResetDataBuffer(void);
void Screen_ResetKeyDataBuffer(void);
void Screen_WriteRegister(u8 address,u8 len,u8 *p);
void Screen_WriteRegisterOneData(u8 address,u8 data);
void Screen_ReadRegister(u8 address,u8 len);
void Screen_PastePicture(u16 address,u16 pid,u32 ctlc,u32 clrc,u32 ptlc);
void Screen_RectangleFilling(u16 address,u32 ctlc,u32 clrc,u16 colour);
//void Screen_SetClock(u16 year,u8 month,u8 day,u8 hour,u8 minute,u8 sec);
void Screen_SetClock(struct tm *sTime);
void Screen_ShowDynamicData(void);
void Screen_SecClock(void);
void Screen_ShowNetworkState(void);
void Screen_ShowPowerState(void);
void Screen_ShowWifi(void);
void Screen_ScanSchack(void);   //�����Ļ¼���״̬

void Screen_InitData(void);
void Screen_ColckInit(void);

//void Screen_ReadData(u8 address,u8 *p,u8 len);
//void Screen_StoreData(u8 address,u8 *p,u8 len);
void Screen_DebugASCII(u8 *p,u8 len);
void ScreenReadMemory(u16 address,u8 len);

void Screen_DebugASCII(u8 *p,u8 len);
//void Screen_DebugRTU(u8 *p,u8 len);
void Screen_SavePowerDownMsg(void);
void Screen_DealWihtReceiveData(void);
//void Screen_VariateData(u16 address,u8 *p,u8 len);

/*
	��������void Screen_YanDaoBuDianSet(Screen_DataStructure *p)
	������	�̵��������ý���
	RunMode��Χ��0x2000-0x2fff
*/
void Screen_FlueSet(Screen_DataStructure *p);
/*
	��������void Screen_WorkingConditionPrediction(Screen_DataStructure *p)
	������	����Ԥ�����ý��� 
	RunMode��Χ��0x3000-0x3fff
*/
void Screen_WorkingConditionPrediction(Screen_DataStructure *p);
/*
	��������void Screen_MeasureHumidity(Screen_DataStructure *p)
	������	�⺬ʪ�����ý��� 
	RunMode��Χ��0x4000-0x4fff
*/
void Screen_MeasureHumidity(Screen_DataStructure *p);
/*
	��������void Screen_WorkingConditionPrediction(Screen_DataStructure *p)
	������	�����������ý��� 
	RunMode��Χ��0x5000-0x5fff
*/
void Screen_MeasureSomke(Screen_DataStructure *p);
/*
	��������void Screen_SmokeSampling(Screen_DataStructure *p)
	������	�̳��������ý��� 
	RunMode��Χ��0x6000-0x6fff
*/
void Screen_SmokeSampling(Screen_DataStructure *p);
/*
	��������void Screen_FileSeek(Screen_DataStructure *p)
	������	�ļ���ѯ���ý��� 
	RunMode��Χ��0x7000-0x7fff
*/
void Screen_FileSeek(Screen_DataStructure *p);
/*
	��������void Screen_FileSeek(Screen_DataStructure *p)
	������	У׼ά�����ý��� 
	RunMode��Χ��0x8000-0x8fff
*/
void Screen_CalibrationFix(Screen_DataStructure *p);

/*
	��������u8* RcvBuffer_GetData(void)
	������	 
	RunMode��Χ��
*/
u8* RcvBuffer_GetData(void);
void ScreenJPG_Timer(void);	//��ƽ����ʱ��

void ScreenJPG_ClearSum(void);

#endif









