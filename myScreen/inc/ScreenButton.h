
#ifndef __SCREENBUTTON_H
#define __SCREENBUTTON_H
#include "stm32f4xx.h"

#define YLB3330D_CJMM 8088  //��������
//typedef enum __tagTEMP_CARIBRATION_STATE_MACHINE
//{
//	TEMP_CARIBRATION_STATE_MACHINE_IDLE = 0,
//	TEMP_CARIBRATION_STATE_MACHINE_READ_YWADC = 1,
//	TEMP_CARIBRATION_STATE_MACHINE2_WAIT_YW100OHM_CONFIRM = 2,

//}TEMP_CARIBRATION_STATE_MACHINE;



/*
	��������void Buttom_MainPage(void)
	������	�ú���Ϊ�����水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Buttom_UpdataProgramPage(void);
/*
	��������void Buttom_MainPage(void)
	������	�ú���Ϊ�����水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Buttom_MainPage(void);
/*
	��������void Buttom_MainPage(void)
	������	�ú���Ϊ�����水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Buttom_PowerOffPage(void);
/*
	��������void Button_SysSetPage(Button_DataStructure *p)
	������	�ú���Ϊϵͳ���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_SysSetPage(void);
/*
	��������void Button_Sys_CYSZPage(void)
	������	�ú���Ϊϵͳ���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/

void Button_Sys_CYSZPage(void);
/*
	��������void Button_SysSetPage(void)
	������	�ú���Ϊ�̵��������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/

void Button_FlueSetPage(void);
/*
	��������void Button_FlueCircularPage(void)
	������	�ú���Ϊ�̵��������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/

void Button_FlueCircularPage(void);
/*
	��������void Button_FlueSquarePage(void)
	������	�ú���Ϊ�̵��������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/

void Button_FlueSquarePage(void);
/*
	��������void Button_FlueOtherPage(void)
	������	�ú���Ϊ�̵��������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/

void Button_FlueOtherPage(void);

/*
	��������void void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	������	�ú���Ϊ����Ԥ�����ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_WCP_PressureZeroPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)

	������	�ú���Ϊ����Ԥ��������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WCP_PressureZeroPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	������	�ú���Ϊ����Ԥ���������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WCP_FumeTemperaturPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_WCP_PredictedVelocityPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	������	�ú���Ϊ����Ԥ��Ԥ���������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WCP_PredictedVelocityPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_WCP_CheckAveragePag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	������	�ú���Ϊ����Ԥ���ѯƽ��ֵ���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WCP_CheckAveragePag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_CHSL_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	������	�ú���Ϊ�⺬ʪ�����ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CHSL_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_CHSL_GSQF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	������	�ú���Ϊ�⺬ʪ��-��ʪ�����ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CHSL_GSQF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_CHSL_ZRF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	������	�ú���Ϊ�⺬ʪ��-���ݷ����ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CHSL_ZRF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*	��������void Button_CHSL_ZRF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ�⺬ʪ��-��ʪ����ѯ-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CHSL_HSLCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_CLYQ_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	������	�ú���Ϊ�����������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/

void Button_CLYQ_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_CLYQ_YQJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	������	�ú���Ϊ������-����У׼���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CLYQ_YQJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_CLYQ_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	������	�ú���Ϊ��������-��ʼ�������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CLYQ_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_CLYQ_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	������	�ú���Ϊ��������-��ʼ����-�����������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CLYQ_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_CLYQ_YQQX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	������	�ú���Ϊ��������-��ʼ����-��������-��ϴ���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/

void Button_CLYQ_YQQX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_CLYQ_YQCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	������	�ú���Ϊ��������-������ѯ���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CLYQ_YQCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	������	�ú���Ϊ��������-������ѯ-�������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_CLYQ_YQCX_JS_YQSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);,float *DisBuffer)
	������	�ú���Ϊ��������-������ѯ-����-�����������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CLYQ_YQCX_JS_YQSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_CLYQ_YQCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);,float *DisBuffer)
	������	�ú���Ϊ��������-������ѯ-�ļ�����-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CLYQ_YQCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
//---
/*
	��������void Button_CLYQ_YQCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);,float *DisBuffer)
	������	�ú���Ϊ��������-������ѯ-��ӡѡ��-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CLYQ_YQCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_CLYQ_YQCX_FZSJ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	������	�ú���Ϊ��������-������ѯ-�����������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_CLYQ_YQCX_FZSJ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);

/*
	��������void Button_FlueSeekPage(void)
	������	�ú���Ϊ�̵�����-����-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/

void Button_FlueSeekPage(float *FileCode);
/*
	��������void Button_YCCY_CYSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ�̳�����-�����������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_YCCY_CYSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_YCCY_QMJC_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ�̳�����-�����Լ����水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_YCCY_QMJC_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_YCCY_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ��������-��ʼ�������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_YCCY_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_YCCY_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ�̳��ɼ�-��ʼ����-�����������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_YCCY_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_YCCY_YCFDX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ��������-��ʼ����-��������-�̳�������-�ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_YCCY_YCFDX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ��������-������ѯ-�������ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_YCCY_YCCX_JS_YC_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
  ������	�ú���Ϊ��������-������ѯ-�������ý��水������---����
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_YCCY_YCCX_JS_YY_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_YCCY_YCCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ��������-�̳���ѯ���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_YCCY_YCCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_YCCY_YCCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	������	�ú���Ϊ�̳�����-�̳���ѯ-�ļ�����-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_YCCY_YCCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_YCCY_YCCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	������	�ú���Ϊ�̳�����-�̳���ѯ-��ӡѡ��-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_YCCY_YCCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_YCCY_YCCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	������	�ú���Ϊ����ά��-��������-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WHJZ_MMSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_WHJZ_WHXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	������	�ú���Ϊ����ά��-��������-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WHJZ_WHXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	��������void Button_WHJZ_WHXZ_STSZ_CO_SO2_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-ϵͳ����-���ý���-CO-SO2У׼��������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WHJZ_WHXZ_STSZ_CO_SO2_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_WHJZ_WHXZ_LLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-����У׼-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WHJZ_WHXZ_LLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_WHJZ_WHXZ_STSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-ϵͳ����-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WHJZ_WHXZ_STSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_WHJZ_WHXZ_LLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-��������-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WHJZ_WHXZ_MMSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_WHJZ_WHXZ_NDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-Ũ��У׼-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WHJZ_WHXZ_NDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_WHJZ_WHXZ_NDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-ѹ��У׼-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WHJZ_WHXZ_YLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_WHJZ_YQCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-����������ѡ��-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WHJZ_YQCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_WHJZ_WDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-�¶�У׼-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WHJZ_WDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_WHJZ_DCBD_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-��ر궨-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WHJZ_DCBD_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_WHJZ_CJWH_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-����ά��-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WHJZ_CJWH_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_WHJZ_CJWH_QTCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-����ά��-���崫����ѡ��-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WHJZ_CJWH_QTCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	��������void Button_WHJZ_WHXZ_NDJZ_BLSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ����ά��-ά��ѡ��-Ũ��У׼-��������-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_WHJZ_WHXZ_NDJZ_BLSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);

/******************  ϵͳ����ҳ��д������  ********************/
void SystemSetPageWriteDate(Screen_DataStructure *ps);

/******************  ϵͳ����ҳ��д��ʱ��  ********************/
void SystemSetPageWriteTime(Screen_DataStructure *ps);

/*  date 2020-5-13 hjl add
	��������void Button_KJ_MMSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	������	�ú���Ϊ����-��������-���ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
*/
void Button_KJ_MMSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);

//---2020-06-05
/*
	��������Button_CLYQ_JIPINGJUN_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	������	�ú���Ϊ�⺬ʪ�����ý��水������
	���룺	�����ṹ�����
	Ӱ�죺	��Ļ�ṹ����ر���
	�޸ļ�¼��
*/
void Button_CLYQ_JIPINGJUN_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);

#endif
