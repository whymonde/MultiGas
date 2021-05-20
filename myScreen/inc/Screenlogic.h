#ifndef SCREENLOGIC__H
#define SCREENLOGIC__H

#include "stm32f4xx.h"



/*
	��������void Screen_programUpdata(Screen_DataStructure *p)
	������	�����������洦����
*/
void Screen_programUpdata(Screen_DataStructure *p);
/*
	��������void Screen_programUpdata(Screen_DataStructure *p)
	������	�����������洦����
*/
void Screen_AutoCheck(Screen_DataStructure *p);
/* date 2019-12-04
	��������void Screen_programUpdata(Screen_DataStructure *p)
	������	���������洦����
*/
void Screen_MainLayer(Screen_DataStructure *p);
/*
	�������� StrinGalignment(* str,u8 len)
	������	�����ַ���������̶�����
*/
void StrinGalignment(u8* str,u8 len);
/* date 2019-12-06
	��������void Screen_PowerOffPag(Screen_DataStructure *p)
	������	���������洦����
*/
void Screen_PowerOffPag(Screen_DataStructure *p);
/* date 2019-12-12
	��������void Screen_SystemSetPag(Screen_DataStructure *p)
	������	ϵͳ���ý����߼�����
*/
void Screen_SystemSetPag(Screen_DataStructure *p);

/* date 2019-12-16
	��������void Screen_FluePag(Screen_DataStructure *p)
	������	�̵��������ý����߼�����
*/

void Screen_FluePag(Screen_DataStructure *p);
/* date 2019-12-16
	��������void Screen_FlueCircularPag(Screen_DataStructure *p)
	������	�̵�����Բ�����ý����߼�����
*/

void Screen_FlueCircularPag(Screen_DataStructure *p);
/* date 2019-12-16
	��������void Screen_FlueSquarePag(Screen_DataStructure *p)
	������	�̵����㷽�����ý����߼�����  
*/
void Screen_FlueSquarePag(Screen_DataStructure *p);
/* date 2019-12-16
	��������void Screen_FlueOtherPag(Screen_DataStructure *p)
	������	�̵�����������״���ý����߼�����  
*/
void Screen_FlueOtherPag(Screen_DataStructure *p);

/* date 2019-12-19
	��������void Screen_WCP_SetPag(Screen_DataStructure *p)
	������	����Ԥ��
*/
void Screen_WCP_SetPag(Screen_DataStructure *p);
/* date 2019-12-19
	��������void Screen_WCP_PressureZeroPag(Screen_DataStructure *p)
	������	����Ԥ��-ѹ������
*/
void Screen_WCP_PressureZeroPag(Screen_DataStructure *p);
/* date 2019-12-19
	��������void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p)
	������	����Ԥ��-���²���
*/
void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p);
/* date 2019-12-19
	��������void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p)
	������	����Ԥ��-Ԥ������
*/
void Screen_WCP_PredictedVelocityPag(Screen_DataStructure *p);
/* date 2019-12-19
	��������void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p)
	������	����Ԥ��-��ƽ��ֵ
*/
void Screen_WCP_CheckAveragePag(Screen_DataStructure *p);
/* date 2019-12-25
	��������void Screen_CHSL_SetPag(Screen_DataStructure *p)
	������	�⺬ʪ������
*/
void Screen_CHSL_SetPag(Screen_DataStructure *p);
/* date 2019-12-25
	��������void Screen_CHSL_SetPag(Screen_DataStructure *p)
	������	�⺬ʪ����ʪ��
*/
void Screen_CHSL_GSQF_Pag(Screen_DataStructure *p);
/* date 2019-12-25
	��������void Screen_CHSL_HSLCX_Pag(Screen_DataStructure *p)
	������	��ʪ����ѯ
*/
void Screen_CHSL_HSLCX_Pag(Screen_DataStructure *p);
/* date 2019-12-25
	��������void Screen_CHSL_ZRF_Pag(Screen_DataStructure *p)
	������	�⺬ʪ�����ݷ�
*/
void Screen_CHSL_ZRF_Pag(Screen_DataStructure *p);

/* date 2019-12-26
	��������void Screen_CLYQ_SetPag(Screen_DataStructure *p)
	������	��������
*/
void Screen_CLYQ_SetPag(Screen_DataStructure *p);
/* date 2019-12-26
	��������void Screen_CLYQ_YQJZ_Pag(Screen_DataStructure *p)
	������	��������-����У׼
*/
void Screen_CLYQ_YQJZ_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	��������void Screen_CLYQ_KSJC_Pag(Screen_DataStructure *p)
	������	��������-��ʼ���
*/
void Screen_CLYQ_KSJC_Pag(Screen_DataStructure *p);
//---
/* date 2019-12-26
	��������void Screen_CLYQ_YQCX_Pag(Screen_DataStructure *p)
	������	��������-������ѯ
*/
void Screen_CLYQ_YQCX_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	��������void Screen_CLYQ_CLJS_Pag(Screen_DataStructure *p)
	������	��������-��������
*/
void Screen_CLYQ_CLJS_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	��������void Screen_CLYQ_YQAX_Pag(Screen_DataStructure *p)
	������	��������-��������-������ϴ
*/
void Screen_CLYQ_YQAX_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	��������void Screen_CLYQ_YQCX_JS_Pag(Screen_DataStructure *p)
	������	��������-�ļ���ѯ-����
*/
void Screen_CLYQ_YQCX_JS_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	��������void Screen_CLYQ_YQCX_JS_YQSR_Pag(Screen_DataStructure *p)
	������	��������-�ļ���ѯ-����
*/
void Screen_CLYQ_YQCX_JS_YQSR_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	��������void Screen_CLYQ_YQCX_WJGL_Pag(Screen_DataStructure *p)
	������	��������-�ļ���ѯ-����
*/
void Screen_CLYQ_YQCX_WJGL_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	��������void Screen_CLYQ_YQCX_DYXX_Pag(Screen_DataStructure *p)
	������	��������-�ļ���ѯ-��ӡѡ��
*/
void Screen_CLYQ_YQCX_DYXX_Pag(Screen_DataStructure *p);
/* date 2019-12-27
	��������void Screen_YCCY_CYSZ_Pag(Screen_DataStructure *p)
	������	�̳�����-��������
*/
void Screen_YCCY_CYSZ_Pag(Screen_DataStructure *p);
/* date 2019-12-12
	��������void Screen_Sys_CYSZPage(Screen_DataStructure *p)
	������	ϵͳ����-��������-�����߼�����
*/
void Screen_Sys_CYSZPage(Screen_DataStructure *p);
/* date 2019-12-18
	��������void Screen_FlueCircularPag(Screen_DataStructure *p)
	������	�̵�����-�̵���ѯ�����߼�����
*/

void Screen_FlueSeekPag(Screen_DataStructure *p);
/* date 2019-12-25
	��������void Screen_YCCY_QMJC_Pag(Screen_DataStructure *p)
	������	�̳�����-���ܼ��
*/
void Screen_YCCY_QMJC_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	��������void Screen_YCCY_KSJC_Pag(Screen_DataStructure *p)
	������	�̳�����-��ʼ���
*/
void Screen_YCCY_KSJC_Pag(Screen_DataStructure *p);
/* date 2020-01-14
	��������void Screen_CLYQ_CLJS_Pag(Screen_DataStructure *p)
	������	�̳�����-��������
*/
void Screen_YCCY_CLJS_Pag(Screen_DataStructure *p);
/* date 2020-01-14
	��������void Screen_CLYQ_YQAX_Pag(Screen_DataStructure *p)
	������	�̳�����-��������-�̳�������
*/
void Screen_YCCY_YCFDX_Pag(Screen_DataStructure *p);
/* date 2020-01-14
	��������void Screen_CLYQ_YQCX_Pag(Screen_DataStructure *p)
	������	��������-�̳��ļ���ѯ
*/
void Screen_YCCY_YCCX_Pag(Screen_DataStructure *p);
/* date 2020-01-14
	��������void Screen_YCCY_YCCX_JS_YY_Pag(Screen_DataStructure *p)
������	��������-�ļ���ѯ-����-����
*/
void Screen_YCCY_YCCX_JS_YY_Pag(Screen_DataStructure *p);
/* date 2020-01-14
	��������void Screen_YCCY_YCCX_JS_YC_Pag(Screen_DataStructure *p)
	������	��������-�ļ���ѯ-����
*/
void Screen_YCCY_YCCX_JS_YC_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	��������void Screen_YCCY_YCCX_WJGL_Pag(Screen_DataStructure *p)
	������	�̳�����-�ļ���ѯ-�ļ�����
*/
void Screen_YCCY_YCCX_WJGL_Pag(Screen_DataStructure *p);
/* date 2020-01-15
	��������void Screen_YCCY_YCCX_DYXX_Pag(Screen_DataStructure *p)
	������	�̳�����-�ļ���ѯ-��ӡѡ��
*/
void Screen_YCCY_YCCX_DYXX_Pag(Screen_DataStructure *p);
/* date 2020-1-15
	��������void Screen_JZWH_WHXZ_Pag(Screen_DataStructure *p)
	������	У׼ά��-ά��ѡ��-�����߼�����
*/
void Screen_JZWH_WHXZ_Pag(Screen_DataStructure *p);
/* date 2020-1-15
	��������void Screen_JZWH_Pag(Screen_DataStructure *p)
	������	У׼ά��-��������-�����߼�����
*/
void Screen_JZWH_MMSR_Pag(Screen_DataStructure *p);/* date 2020-1-16
	��������void Screen_WHJZ_WHXZ_STSZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-ϵͳ����
*/
void Screen_WHJZ_WHXZ_STSZ_Pag(Screen_DataStructure *p);
/* date 2020-1-16
	��������void Screen_WHJZ_WHXZ_LLJZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-����У׼
*/
void Screen_WHJZ_WHXZ_LLJZ_Pag(Screen_DataStructure *p);
/* date 2020-03-05
	��������void Screen_WHJZ_WHXZ_STSZ_CO_SO2_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-ϵͳ����-CO��SO2����
*/
void Screen_WHJZ_WHXZ_STSZ_CO_SO2_Pag(Screen_DataStructure *p);
/* date 2020-1-16
	��������void Screen_WHJZ_WHXZ_MMSZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-��������
*/
void Screen_WHJZ_WHXZ_MMSZ_Pag(Screen_DataStructure *p);
/* date 2020-1-16
	��������void Screen_WHJZ_WHXZ_NDJZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-Ũ��У׼
*/
void Screen_WHJZ_WHXZ_NDJZ_Pag(Screen_DataStructure *p);
/* date 2020-1-16
	��������void Screen_WHJZ_WHXZ_NDJZ_BLSZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-Ũ��У׼-��������
*/

void Screen_WHJZ_WHXZ_NDJZ_BLSZ_Pag(Screen_DataStructure *p);
/* date 2020-1-16
	��������void Screen_WHJZ_WHXZ_YLJZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-ѹ��У׼
*/

void Screen_WHJZ_WHXZ_YLJZ_Pag(Screen_DataStructure *p);
/* date 2020-1-17
	��������void Screen_WHJZ_YQCGQXZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-����������ѡ��
*/
void Screen_WHJZ_YQCGQXZ_Pag(Screen_DataStructure *p);
/* date 2020-1-17
	��������void Screen_WHJZ_WHXZ_WDJZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-�¶�У׼
*/
void Screen_WHJZ_WHXZ_WDJZ_Pag(Screen_DataStructure *p);
/* date 2020-1-17
	��������void Screen_WHJZ_WHXZ_DCBD_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-ά��ѡ��-��ر궨
*/
void Screen_WHJZ_WHXZ_DCBD_Pag(Screen_DataStructure *p);
/* date 2020-03-05
	��������void Screen_JZWH_CJWH_Pag(Screen_DataStructure *p)
	������	У׼ά��-����ά��-�����߼�����
*/
void Screen_JZWH_CJWH_Pag(Screen_DataStructure *p);
/* date 2020-0305-17
	��������void Screen_WHJZ_CJWH_QTCGQPZ_Pag(Screen_DataStructure *p)
	������	�ú���Ϊ����ά��-����ά��-����������ѡ��
*/
void Screen_WHJZ_CJWH_QTCGQPZ_Pag(Screen_DataStructure *p);

/* date 2020-5-13 hjl add
	��������void Screen_KJ_MMSR_Pag(Screen_DataStructure *p)
	������	����-��������-�����߼�����
*/
void Screen_KJ_MMSR_Pag(Screen_DataStructure *p);

/* date 2020.06.05
	��������void Screen_CLYQ_JIPINGJUN_Pag(Screen_DataStructure *p)
	������	��������-��ƽ��
*/
void Screen_CLYQ_JIPINGJUN_Pag(Screen_DataStructure *p);



#endif
