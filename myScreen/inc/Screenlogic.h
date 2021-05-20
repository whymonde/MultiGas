#ifndef SCREENLOGIC__H
#define SCREENLOGIC__H

#include "stm32f4xx.h"



/*
	函数名：void Screen_programUpdata(Screen_DataStructure *p)
	描述：	程序升级界面处理函数
*/
void Screen_programUpdata(Screen_DataStructure *p);
/*
	函数名：void Screen_programUpdata(Screen_DataStructure *p)
	描述：	程序升级界面处理函数
*/
void Screen_AutoCheck(Screen_DataStructure *p);
/* date 2019-12-04
	函数名：void Screen_programUpdata(Screen_DataStructure *p)
	描述：	程序主界面处理函数
*/
void Screen_MainLayer(Screen_DataStructure *p);
/*
	函数名： StrinGalignment(* str,u8 len)
	描述：	用于字符串填充至固定长度
*/
void StrinGalignment(u8* str,u8 len);
/* date 2019-12-06
	函数名：void Screen_PowerOffPag(Screen_DataStructure *p)
	描述：	掉电主界面处理函数
*/
void Screen_PowerOffPag(Screen_DataStructure *p);
/* date 2019-12-12
	函数名：void Screen_SystemSetPag(Screen_DataStructure *p)
	描述：	系统设置界面逻辑处理
*/
void Screen_SystemSetPag(Screen_DataStructure *p);

/* date 2019-12-16
	函数名：void Screen_FluePag(Screen_DataStructure *p)
	描述：	烟道布点设置界面逻辑处理
*/

void Screen_FluePag(Screen_DataStructure *p);
/* date 2019-12-16
	函数名：void Screen_FlueCircularPag(Screen_DataStructure *p)
	描述：	烟道布点圆形设置界面逻辑处理
*/

void Screen_FlueCircularPag(Screen_DataStructure *p);
/* date 2019-12-16
	函数名：void Screen_FlueSquarePag(Screen_DataStructure *p)
	描述：	烟道布点方形设置界面逻辑处理  
*/
void Screen_FlueSquarePag(Screen_DataStructure *p);
/* date 2019-12-16
	函数名：void Screen_FlueOtherPag(Screen_DataStructure *p)
	描述：	烟道布点其他形状设置界面逻辑处理  
*/
void Screen_FlueOtherPag(Screen_DataStructure *p);

/* date 2019-12-19
	函数名：void Screen_WCP_SetPag(Screen_DataStructure *p)
	描述：	工况预测
*/
void Screen_WCP_SetPag(Screen_DataStructure *p);
/* date 2019-12-19
	函数名：void Screen_WCP_PressureZeroPag(Screen_DataStructure *p)
	描述：	工况预测-压力调零
*/
void Screen_WCP_PressureZeroPag(Screen_DataStructure *p);
/* date 2019-12-19
	函数名：void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p)
	描述：	工况预测-烟温测量
*/
void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p);
/* date 2019-12-19
	函数名：void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p)
	描述：	工况预测-预测流速
*/
void Screen_WCP_PredictedVelocityPag(Screen_DataStructure *p);
/* date 2019-12-19
	函数名：void Screen_WCP_FumeTemperaturPag(Screen_DataStructure *p)
	描述：	工况预测-查平均值
*/
void Screen_WCP_CheckAveragePag(Screen_DataStructure *p);
/* date 2019-12-25
	函数名：void Screen_CHSL_SetPag(Screen_DataStructure *p)
	描述：	测含湿量类型
*/
void Screen_CHSL_SetPag(Screen_DataStructure *p);
/* date 2019-12-25
	函数名：void Screen_CHSL_SetPag(Screen_DataStructure *p)
	描述：	测含湿量干湿球法
*/
void Screen_CHSL_GSQF_Pag(Screen_DataStructure *p);
/* date 2019-12-25
	函数名：void Screen_CHSL_HSLCX_Pag(Screen_DataStructure *p)
	描述：	含湿量查询
*/
void Screen_CHSL_HSLCX_Pag(Screen_DataStructure *p);
/* date 2019-12-25
	函数名：void Screen_CHSL_ZRF_Pag(Screen_DataStructure *p)
	描述：	测含湿量阻容法
*/
void Screen_CHSL_ZRF_Pag(Screen_DataStructure *p);

/* date 2019-12-26
	函数名：void Screen_CLYQ_SetPag(Screen_DataStructure *p)
	描述：	测量烟气
*/
void Screen_CLYQ_SetPag(Screen_DataStructure *p);
/* date 2019-12-26
	函数名：void Screen_CLYQ_YQJZ_Pag(Screen_DataStructure *p)
	描述：	测量烟气-烟气校准
*/
void Screen_CLYQ_YQJZ_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	函数名：void Screen_CLYQ_KSJC_Pag(Screen_DataStructure *p)
	描述：	测量烟气-开始检测
*/
void Screen_CLYQ_KSJC_Pag(Screen_DataStructure *p);
//---
/* date 2019-12-26
	函数名：void Screen_CLYQ_YQCX_Pag(Screen_DataStructure *p)
	描述：	测量烟气-烟气查询
*/
void Screen_CLYQ_YQCX_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	函数名：void Screen_CLYQ_CLJS_Pag(Screen_DataStructure *p)
	描述：	测量烟气-测量结束
*/
void Screen_CLYQ_CLJS_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	函数名：void Screen_CLYQ_YQAX_Pag(Screen_DataStructure *p)
	描述：	测量烟气-测量结束-烟气清洗
*/
void Screen_CLYQ_YQAX_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	函数名：void Screen_CLYQ_YQCX_JS_Pag(Screen_DataStructure *p)
	描述：	测量烟气-文件查询-计算
*/
void Screen_CLYQ_YQCX_JS_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	函数名：void Screen_CLYQ_YQCX_JS_YQSR_Pag(Screen_DataStructure *p)
	描述：	测量烟气-文件查询-计算
*/
void Screen_CLYQ_YQCX_JS_YQSR_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	函数名：void Screen_CLYQ_YQCX_WJGL_Pag(Screen_DataStructure *p)
	描述：	测量烟气-文件查询-计算
*/
void Screen_CLYQ_YQCX_WJGL_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	函数名：void Screen_CLYQ_YQCX_DYXX_Pag(Screen_DataStructure *p)
	描述：	测量烟气-文件查询-打印选项
*/
void Screen_CLYQ_YQCX_DYXX_Pag(Screen_DataStructure *p);
/* date 2019-12-27
	函数名：void Screen_YCCY_CYSZ_Pag(Screen_DataStructure *p)
	描述：	烟尘采样-采样设置
*/
void Screen_YCCY_CYSZ_Pag(Screen_DataStructure *p);
/* date 2019-12-12
	函数名：void Screen_Sys_CYSZPage(Screen_DataStructure *p)
	描述：	系统设置-采样设置-界面逻辑处理
*/
void Screen_Sys_CYSZPage(Screen_DataStructure *p);
/* date 2019-12-18
	函数名：void Screen_FlueCircularPag(Screen_DataStructure *p)
	描述：	烟道布点-烟道查询界面逻辑处理
*/

void Screen_FlueSeekPag(Screen_DataStructure *p);
/* date 2019-12-25
	函数名：void Screen_YCCY_QMJC_Pag(Screen_DataStructure *p)
	描述：	烟尘采样-气密检测
*/
void Screen_YCCY_QMJC_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	函数名：void Screen_YCCY_KSJC_Pag(Screen_DataStructure *p)
	描述：	烟尘采样-开始检测
*/
void Screen_YCCY_KSJC_Pag(Screen_DataStructure *p);
/* date 2020-01-14
	函数名：void Screen_CLYQ_CLJS_Pag(Screen_DataStructure *p)
	描述：	烟尘采样-测量结束
*/
void Screen_YCCY_CLJS_Pag(Screen_DataStructure *p);
/* date 2020-01-14
	函数名：void Screen_CLYQ_YQAX_Pag(Screen_DataStructure *p)
	描述：	烟尘采样-测量结束-烟尘防倒吸
*/
void Screen_YCCY_YCFDX_Pag(Screen_DataStructure *p);
/* date 2020-01-14
	函数名：void Screen_CLYQ_YQCX_Pag(Screen_DataStructure *p)
	描述：	烟气采样-烟尘文件查询
*/
void Screen_YCCY_YCCX_Pag(Screen_DataStructure *p);
/* date 2020-01-14
	函数名：void Screen_YCCY_YCCX_JS_YY_Pag(Screen_DataStructure *p)
描述：	测量烟气-文件查询-计算-油烟
*/
void Screen_YCCY_YCCX_JS_YY_Pag(Screen_DataStructure *p);
/* date 2020-01-14
	函数名：void Screen_YCCY_YCCX_JS_YC_Pag(Screen_DataStructure *p)
	描述：	测量烟气-文件查询-计算
*/
void Screen_YCCY_YCCX_JS_YC_Pag(Screen_DataStructure *p);
/* date 2019-12-26
	函数名：void Screen_YCCY_YCCX_WJGL_Pag(Screen_DataStructure *p)
	描述：	烟尘采样-文件查询-文件管理
*/
void Screen_YCCY_YCCX_WJGL_Pag(Screen_DataStructure *p);
/* date 2020-01-15
	函数名：void Screen_YCCY_YCCX_DYXX_Pag(Screen_DataStructure *p)
	描述：	烟尘采样-文件查询-打印选项
*/
void Screen_YCCY_YCCX_DYXX_Pag(Screen_DataStructure *p);
/* date 2020-1-15
	函数名：void Screen_JZWH_WHXZ_Pag(Screen_DataStructure *p)
	描述：	校准维护-维护选择-界面逻辑处理
*/
void Screen_JZWH_WHXZ_Pag(Screen_DataStructure *p);
/* date 2020-1-15
	函数名：void Screen_JZWH_Pag(Screen_DataStructure *p)
	描述：	校准维护-输入密码-界面逻辑处理
*/
void Screen_JZWH_MMSR_Pag(Screen_DataStructure *p);/* date 2020-1-16
	函数名：void Screen_WHJZ_WHXZ_STSZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-系统设置
*/
void Screen_WHJZ_WHXZ_STSZ_Pag(Screen_DataStructure *p);
/* date 2020-1-16
	函数名：void Screen_WHJZ_WHXZ_LLJZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-流量校准
*/
void Screen_WHJZ_WHXZ_LLJZ_Pag(Screen_DataStructure *p);
/* date 2020-03-05
	函数名：void Screen_WHJZ_WHXZ_STSZ_CO_SO2_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-系统设置-CO对SO2修正
*/
void Screen_WHJZ_WHXZ_STSZ_CO_SO2_Pag(Screen_DataStructure *p);
/* date 2020-1-16
	函数名：void Screen_WHJZ_WHXZ_MMSZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-密码设置
*/
void Screen_WHJZ_WHXZ_MMSZ_Pag(Screen_DataStructure *p);
/* date 2020-1-16
	函数名：void Screen_WHJZ_WHXZ_NDJZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-浓度校准
*/
void Screen_WHJZ_WHXZ_NDJZ_Pag(Screen_DataStructure *p);
/* date 2020-1-16
	函数名：void Screen_WHJZ_WHXZ_NDJZ_BLSZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-浓度校准-倍率设置
*/

void Screen_WHJZ_WHXZ_NDJZ_BLSZ_Pag(Screen_DataStructure *p);
/* date 2020-1-16
	函数名：void Screen_WHJZ_WHXZ_YLJZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-压力校准
*/

void Screen_WHJZ_WHXZ_YLJZ_Pag(Screen_DataStructure *p);
/* date 2020-1-17
	函数名：void Screen_WHJZ_YQCGQXZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-烟气传感器选择
*/
void Screen_WHJZ_YQCGQXZ_Pag(Screen_DataStructure *p);
/* date 2020-1-17
	函数名：void Screen_WHJZ_WHXZ_WDJZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-温度校准
*/
void Screen_WHJZ_WHXZ_WDJZ_Pag(Screen_DataStructure *p);
/* date 2020-1-17
	函数名：void Screen_WHJZ_WHXZ_DCBD_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-维护选择-电池标定
*/
void Screen_WHJZ_WHXZ_DCBD_Pag(Screen_DataStructure *p);
/* date 2020-03-05
	函数名：void Screen_JZWH_CJWH_Pag(Screen_DataStructure *p)
	描述：	校准维护-超级维护-界面逻辑处理
*/
void Screen_JZWH_CJWH_Pag(Screen_DataStructure *p);
/* date 2020-0305-17
	函数名：void Screen_WHJZ_CJWH_QTCGQPZ_Pag(Screen_DataStructure *p)
	描述：	该函数为矫正维护-超级维护-烟气传感器选择
*/
void Screen_WHJZ_CJWH_QTCGQPZ_Pag(Screen_DataStructure *p);

/* date 2020-5-13 hjl add
	函数名：void Screen_KJ_MMSR_Pag(Screen_DataStructure *p)
	描述：	开机-输入密码-界面逻辑处理
*/
void Screen_KJ_MMSR_Pag(Screen_DataStructure *p);

/* date 2020.06.05
	函数名：void Screen_CLYQ_JIPINGJUN_Pag(Screen_DataStructure *p)
	描述：	测量烟气-计平均
*/
void Screen_CLYQ_JIPINGJUN_Pag(Screen_DataStructure *p);



#endif
