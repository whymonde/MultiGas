
#ifndef __SCREENBUTTON_H
#define __SCREENBUTTON_H
#include "stm32f4xx.h"

#define YLB3330D_CJMM 8088  //超级密码
//typedef enum __tagTEMP_CARIBRATION_STATE_MACHINE
//{
//	TEMP_CARIBRATION_STATE_MACHINE_IDLE = 0,
//	TEMP_CARIBRATION_STATE_MACHINE_READ_YWADC = 1,
//	TEMP_CARIBRATION_STATE_MACHINE2_WAIT_YW100OHM_CONFIRM = 2,

//}TEMP_CARIBRATION_STATE_MACHINE;



/*
	函数名：void Buttom_MainPage(void)
	描述：	该函数为升级面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Buttom_UpdataProgramPage(void);
/*
	函数名：void Buttom_MainPage(void)
	描述：	该函数为主界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Buttom_MainPage(void);
/*
	函数名：void Buttom_MainPage(void)
	描述：	该函数为掉电面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Buttom_PowerOffPage(void);
/*
	函数名：void Button_SysSetPage(Button_DataStructure *p)
	描述：	该函数为系统设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_SysSetPage(void);
/*
	函数名：void Button_Sys_CYSZPage(void)
	描述：	该函数为系统设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/

void Button_Sys_CYSZPage(void);
/*
	函数名：void Button_SysSetPage(void)
	描述：	该函数为烟道布点设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/

void Button_FlueSetPage(void);
/*
	函数名：void Button_FlueCircularPage(void)
	描述：	该函数为烟道布点设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/

void Button_FlueCircularPage(void);
/*
	函数名：void Button_FlueSquarePage(void)
	描述：	该函数为烟道布点设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/

void Button_FlueSquarePage(void);
/*
	函数名：void Button_FlueOtherPage(void)
	描述：	该函数为烟道布点设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/

void Button_FlueOtherPage(void);

/*
	函数名：void void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	描述：	该函数为工况预测设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_WCP_PressureZeroPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)

	描述：	该函数为工况预测调零设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WCP_PressureZeroPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_WCP_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	描述：	该函数为工况预测烟温设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WCP_FumeTemperaturPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_WCP_PredictedVelocityPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	描述：	该函数为工况预测预测流速设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WCP_PredictedVelocityPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_WCP_CheckAveragePag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	描述：	该函数为工况预测查询平均值设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WCP_CheckAveragePag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_CHSL_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	描述：	该函数为测含湿量设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CHSL_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_CHSL_GSQF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	描述：	该函数为测含湿量-干湿球法设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CHSL_GSQF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_CHSL_ZRF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	描述：	该函数为测含湿量-阻容法设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CHSL_ZRF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*	函数名：void Button_CHSL_ZRF_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测含湿量-含湿量查询-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CHSL_HSLCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_CLYQ_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	描述：	该函数为测量烟气设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/

void Button_CLYQ_SetPag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_CLYQ_YQJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	描述：	该函数为测量烟-烟气校准设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CLYQ_YQJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_CLYQ_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	描述：	该函数为测量烟气-开始测量设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CLYQ_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_CLYQ_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	描述：	该函数为测量烟气-开始测量-结束测量设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CLYQ_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_CLYQ_YQQX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	描述：	该函数为测量烟气-开始测量-结束测量-清洗设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/

void Button_CLYQ_YQQX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_CLYQ_YQCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	描述：	该函数为测量烟气-烟气查询设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CLYQ_YQCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	描述：	该函数为测量烟气-烟气查询-计算设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_CLYQ_YQCX_JS_YQSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);,float *DisBuffer)
	描述：	该函数为测量烟气-烟气查询-计算-烟气输入设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CLYQ_YQCX_JS_YQSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_CLYQ_YQCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);,float *DisBuffer)
	描述：	该函数为测量烟气-烟气查询-文件管理-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CLYQ_YQCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
//---
/*
	函数名：void Button_CLYQ_YQCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);,float *DisBuffer)
	描述：	该函数为测量烟气-烟气查询-打印选项-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CLYQ_YQCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_CLYQ_YQCX_FZSJ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);)
	描述：	该函数为测量烟气-烟气查询-分钟数据设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_CLYQ_YQCX_FZSJ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);

/*
	函数名：void Button_FlueSeekPage(void)
	描述：	该函数为烟道布点-查找-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/

void Button_FlueSeekPage(float *FileCode);
/*
	函数名：void Button_YCCY_CYSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为烟尘采样-采样设置设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_YCCY_CYSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_YCCY_QMJC_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为烟尘采样-气密性检测界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_YCCY_QMJC_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_YCCY_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测量烟气-开始测量设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_YCCY_KSCL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_YCCY_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为烟尘采集-开始测量-结束测量设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_YCCY_CLJS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_YCCY_YCFDX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测量烟气-开始测量-结束测量-烟尘防倒吸-置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_YCCY_YCFDX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测量烟气-烟气查询-计算设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_YCCY_YCCX_JS_YC_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_CLYQ_YQCX_JS_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
  描述：	该函数为测量烟气-烟气查询-计算设置界面按键处理---油烟
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_YCCY_YCCX_JS_YY_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_YCCY_YCCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为烟气采样-烟尘查询设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_YCCY_YCCX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_YCCY_YCCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	描述：	该函数为烟尘采样-烟尘查询-文件管理-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_YCCY_YCCX_WJGL_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_YCCY_YCCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	描述：	该函数为烟尘采样-烟尘查询-打印选项-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_YCCY_YCCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_YCCY_YCCX_DYXX_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	描述：	该函数为矫正维护-密码输入-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WHJZ_MMSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_WHJZ_WHXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	描述：	该函数为矫正维护-密码输入-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WHJZ_WHXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);
/*
	函数名：void Button_WHJZ_WHXZ_STSZ_CO_SO2_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-系统设置-设置界面-CO-SO2校准按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WHJZ_WHXZ_STSZ_CO_SO2_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_WHJZ_WHXZ_LLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-流量校准-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WHJZ_WHXZ_LLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_WHJZ_WHXZ_STSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-系统设置-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WHJZ_WHXZ_STSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_WHJZ_WHXZ_LLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-密码设置-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WHJZ_WHXZ_MMSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_WHJZ_WHXZ_NDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-浓度校准-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WHJZ_WHXZ_NDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_WHJZ_WHXZ_NDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-压力校准-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WHJZ_WHXZ_YLJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_WHJZ_YQCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-烟气传感器选择-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WHJZ_YQCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_WHJZ_WDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-温度校准-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WHJZ_WDJZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_WHJZ_DCBD_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-电池标定-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WHJZ_DCBD_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_WHJZ_CJWH_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-超级维护-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WHJZ_CJWH_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_WHJZ_CJWH_QTCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-超级维护-气体传感器选择-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WHJZ_CJWH_QTCGQXZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);
/*
	函数名：void Button_WHJZ_WHXZ_NDJZ_BLSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为矫正维护-维护选择-浓度校准-倍率设置-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_WHJZ_WHXZ_NDJZ_BLSZ_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);

/******************  系统设置页面写入日期  ********************/
void SystemSetPageWriteDate(Screen_DataStructure *ps);

/******************  系统设置页面写入时间  ********************/
void SystemSetPageWriteTime(Screen_DataStructure *ps);

/*  date 2020-5-13 hjl add
	函数名：void Button_KJ_MMSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer)
	描述：	该函数为开机-密码输入-设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
*/
void Button_KJ_MMSR_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuffer);

//---2020-06-05
/*
	函数名：Button_CLYQ_JIPINGJUN_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff)
	描述：	该函数为测含湿量设置界面按键处理
	输入：	按键结构体变量
	影响：	屏幕结构体相关变量
	修改记录：
*/
void Button_CLYQ_JIPINGJUN_Pag(u8 Max_Opt,u8 isNumberChose,float *DisBuff);

#endif
