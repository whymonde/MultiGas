/*

*/

#ifndef __SCREENDWFUNCTION_H
#define __SCREENDWFUNCTION_H
#include "stm32f4xx.h"

//计算器的类型
typedef enum _tagCALCTYPE
{
	CALC_NO_POINT = 0,						//无小数点
	CALC_HAVE_POINT = 1,				//有小数点
	CALC_NO_SIGNS = 3,						//无正负符号
	CALC_HAVE_SIGNS = 4,					//有正负符号
}CALCTYPE;

//格式化变量的类型
typedef enum _tagVALTYPE
{
	TWO_DIGIT_INT = 0,						//整型数
	TWO_POINT_FLOAT = 1,				//两位小数点的浮点数类型
	EIGHT_POINT_DATA = 3,					//日期
	EIGHT_POINT_TIME = 4,					//时间
}VALTYPE;


/*
	函数名：GetU16dataFormFlash2Word(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP)     
	描述：	去取4字节数据（主要作用减少代码行数）
*/
void GetU16dataFormFlash2Word(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP);
/*
	float GetFlaotdataFormFlash2Word(u32 addr)     
	描述：	去取4字节数据（主要作用减少代码行数）
*/
float GetFlaotdataFormFlash2Word(u32 addr) ;
/*
	u32 GetU32dataFormFlash2Word(u32 addr)     
	描述：	去取4字节浮点数据（主要作用减少代码行数）
*/
u32 GetU32dataFormFlash2Word(u32 addr);
/*
	函数名SaveU32ToFlash(u32 data,u32 addr) 
	描述：	保存U32数据到FLASE里面（主要作用减少代码行数）
*/
void SaveU32ToFlash(u32 data,u32 addr);
/*
	函数名SaveU16ToFlash(u32 data,u32 addr) 
	描述：	保存U16数据到FLASE里面（主要作用减少代码行数）
*/
void SaveU16ToFlash(u16 data,u32 addr) ;

/*
	函数名void SaveFlaotToFlash(float data,u32 addr) 
	描述：	保存浮点数据到FLASE里面（主要作用减少代码行数）
*/
void SaveFlaotToFlash(float data,u32 addr);

/*
	函数名oid UpdataSensorFixDate(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP,Screen_DataStructure *p)               
	描述：	更新传感器安装时间（主要作用减少代码行数）
  type:   0~7 气体寄存器的种类
*/

void UpdataSensorFixDate(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP,Screen_DataStructure *p,u8 type);
/*
	函数名： StrinGalignment(* str,u8 len)
	描述：	用于字符串填充至固定长度
*/
void StrinGalignment(u8* str,u8 len);
/*
	函数名： void ScreenSetJitu(Flashisp_DataStructure *fP,Screen_DataStructure *p)
	描述：	用于设置界面处理基图
  tpye:   基图的类型 0-5
*/
void ScreenSetJitu(Screen_DataStructure *p,u8 type);
/*
	函数名：void ScreenFirstOpButton(Screen_DataStructure *ps,u8 OpMax,u8 isNumberChose)
	描述：	 一级菜单按键处理
  OpMax:   一级按键菜单最大值
  ButtonAddr：  对应界面按键起始值
  isNumberChose： 是否对应快捷键数据
*/
void ScreenFirstOpButton(Screen_DataStructure *ps,u8 OpMax,u8 isNumberChose);
/*
	函数名： void ScreenTowOpButton(Screen_DataStructure *ps,u8 type,u8 OpMax,u8 isOpChang);
	描述：	二级菜单按键处理
  tpye:   二级按键菜单类型
*/
void ScreenTowOpButton(Screen_DataStructure *ps,u8 type,u8 OpMax,u8 isOpChang);
/*
	函数名： void ScreenTowOptChose(Screen_DataStructure *ps,u8 opt)
	描述：	二级菜单按键选择处理
  tpye:   二级按键菜单类型
*/
void ScreenTowOptChose(Screen_DataStructure *ps,u8 opt);
/*
	函数名： ScreenBackLastPag(Screen_DataStructure *ps,u16 lastPag)
	描述：	返回上级界面
  tpye:   二级按键菜单类型
*/
void ScreenBackLastPag(Screen_DataStructure *ps,u16 lastMode);
/*
	函数名：void ScreenConfirmEnter(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min)
	描述：	输入数值确认  

*/
void ScreenConfirmEnter(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min);
/*
	函数名：void ScreenConfirmEnterDisBuffer(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min)
	描述：	输入数值确认----确认显示缓存 
  存储类型为float 数据
  //position 该数据的位置
*/
void ScreenConfirmEnterDisBuffer(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min);
/*
	函数名： ScreenBackLastPag(Screen_DataStructure *ps)
	描述：	返回一级菜单
*/
void ScreenBackFirstOpt(Screen_DataStructure *ps);
/*
	函数名：ScreenEnterNumberText(Screen_DataStructure *ps,u8 type,u8 len)
	描述：	数值  
  数值类型：type 0  无符号整形
            type 1  有符号整形
            type 2  无符号浮点型
            type 3  有符号浮点型
  文本长度：len
*/
void ScreenEnterNumberText(Screen_DataStructure *ps,u8 type,u8 len);
//---
/*
	函数名：void ScreenEnterNumberTwinkle(Screen_DataStructure *ps,u16 TextAddr)
	描述：	数值输入闪烁  

*/
void ScreenEnterNumberTwinkle(Screen_DataStructure *ps,u16 TextAddr);
/*
	函数名：void ScreenChangeDataInit(Screen_DataStructure *ps)
	描述：	页面切换数据初始化

*/
void ScreenChangeDataInit(Screen_DataStructure *ps);
/*
	函数名：void ScreenButtonReset(Screen_DataStructure *ps)
	描述：	按键参数复位

*/
void ScreenButtonReset(Screen_DataStructure *ps);
/* date 2019-12-26
	函数名：void Screen_CLYQ_YQCX_FZSJ_Pag(Screen_DataStructure *p)
	描述：	测量烟气-烟气查询-分钟数据
*/
void Screen_CLYQ_YQCX_FZSJ_Pag(Screen_DataStructure *p);
/*
	u8* GetYDBD_Describe(u8 Type)
	描述：	返回烟道布点描述
*/
u8* GetYDBD_Describe(u8 Type);
/*
	函数名：u16 Dis_Tips(u8* Type)
	描述：	提示显示
*/
void Dis_Tips(Screen_DataStructure *p,u8* Tips,u16 TextAddr);
/*
	函数名：u16 Dis_CheckBox(u16 addr,u8 Stat,u32 DisXY)
	描述：	勾选
*/
void Dis_CheckBox(u16 addr,u8 Stat,u32 DisXY);
/*
	函数名：u16 Set_SysCheckBox(u16 addr,u16 ByteNumber)
	描述：	设置-系统设置勾选框/打印选择勾选框设置
*/
void Set_SysCheckBox(u16 addr,u16 ByteNumber);
/*
	float GetFlueArea (void)
	描述：	获取烟道面积
*/
float GetFlueArea (void);
/*
	float YC_SampleControl (void)
	描述：	烟尘采样控制处理  在Screen_SecTimer() 中调用
*/
void YC_SampleControl(void);


void ScreenTowOpButton_KeyControl(Screen_DataStructure *ps, u8 isHavePoint,u8 isHaveSign);//屏幕数字录入时外部按键的处理
void ScreenTowOpButton_KeyControl_adr(Screen_DataStructure *ps);//地址录入时外部按键的处理
void ScreenTextTwinkle(Screen_DataStructure *ps,u16 TextAddr, u16 FlashAddress, u8 valType);//临时测试用，后面规范时删除掉
void ScreenBackLastOpt(Screen_DataStructure *ps); //返回上一级菜单
void SoftKeyboardConfirmEnter(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min);//软键盘确认后写入数据
void ProcessSoftKeyboardInput(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min,u8 isHavePoint,u8 isHaveSign);	//处理软键盘输入数据--数值录入
void ProcessSoftKeyboardInputFileName(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min,u8 isHavePoint,u8 isHaveSign);//处理软键盘输入数据--文件号录入
void SoftKeyboardConfirmEnterDisBuffer(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min);//软键盘缓存录入


#endif
