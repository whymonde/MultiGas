/*
	Project�⺯��
*/

#ifndef __PROJECT_H
#define __PROJECT_H

#include "stm32f4xx.h"

#define  YLB3330_SOFE_VERSION           "V1.0"   //����汾
#define  YLB3330_HEAT_VERSION           "V1.0"   //Ӳ���汾
#define  COMPANY_TELEPHON  "0757-22629008"               //�绰����
#define  COMPANY_WEBSITE   "http://www.zcqk88.com"       //��վ  

//���
#define PRO_LED1(a)\
					if (a==1)\
						GPIO_SetBits(PRO_LED1_GPIO,PRO_LED1_PIN);\
					else if (a==0)\
						GPIO_ResetBits(PRO_LED1_GPIO,PRO_LED1_PIN);\
					else if(a==2)\
						PRO_LED1_GPIO->ODR ^= PRO_LED1_PIN;
#define PRO_LED2(a)\
					if (a==1)\
						GPIO_SetBits(PRO_LED2_GPIO,PRO_LED2_PIN);\
					else if (a==0)\
						GPIO_ResetBits(PRO_LED2_GPIO,PRO_LED2_PIN);\
					else if(a==2)\
						PRO_LED2_GPIO->ODR ^= PRO_LED2_PIN;

#define PROJECT_BUZZER(a)\
					if (a==1)\
						GPIO_ResetBits(PROJECT_BUZZER_PORT,PROJECT_BUZZER_PIN);\
					else if (a==0)\
						GPIO_SetBits(PROJECT_BUZZER_PORT,PROJECT_BUZZER_PIN);\
					else if(a==2)\
						PROJECT_BUZZER_PORT->ODR ^= PROJECT_BUZZER_PIN;	
//����
					
/*
	ϵͳ�ṹ��
*/
//typedef struct 
//{
//	float D_Tem;
//	float MA_Press;
//	float MA_Tem;
//	float MB_Press;
//	float MB_Tem;
//	float MC_Press;
//	float MC_Tem;
//	float SA_Press;
//	float SA_Tem;
//	float SB_Press;
//	float SB_Tem;
//	float SC_Press;
//	float SC_Tem;
//	float SD_Press;
//	float SD_Tem;
//	float A_Flux;
//	float B_Flux;
//	float C_Flux;
//}Project_BufferStructure;


/*
	ϵͳ�ṹ��
*/
typedef struct 
{
	u8 Address;			//������ַ
	u8 RunMode;			//����ģʽ
	u32 Timer;			//ϵͳ�ۼ�����ʱ�䣬��λ��
	u16 TimerPro;		
	u16 DelayTimre;	//������ʱ��ʱ���ã�����Ŀ��ʱ��������Ϊ0��--��ֱ����Ϊ0Ϊֹ������ͬʱʹ��
	
}Project_SystemStructure;


//�⺯��
void Project_Init(void);				//������Ϊ������ʼ��
void Project_Timer(void);				//ϵͳ��ʱ��
void Project_SecTimer(void);
void Porject_Debug(u8 *p);			//�ú���Ϊ��ϵͳ��Ŀ��صĵ��Ժ���
void Project_ClearProjectSysBuffer(void);//�ú������ڸ�λProjec_SysBuffer���е�����
Project_SystemStructure *Project_GetData(void); //����ϵͳ�����ṹ��ĵ�ַ
void Project_TimerInit(void);		//��ʼ������Ŀ�Ķ�ʱ��	
void Project_InitLED(void);
void Project_IWDGInit(u32 TimeoutFreMs);

//Project_BufferStructure *Project_GetBuffer(void);
	

void Project_DebugASCII(u8 *p,u8 len);//
void Project_DebugRTU(u8 *p,u8 len);//

#endif



