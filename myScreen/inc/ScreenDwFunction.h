/*

*/

#ifndef __SCREENDWFUNCTION_H
#define __SCREENDWFUNCTION_H
#include "stm32f4xx.h"

//������������
typedef enum _tagCALCTYPE
{
	CALC_NO_POINT = 0,						//��С����
	CALC_HAVE_POINT = 1,				//��С����
	CALC_NO_SIGNS = 3,						//����������
	CALC_HAVE_SIGNS = 4,					//����������
}CALCTYPE;

//��ʽ������������
typedef enum _tagVALTYPE
{
	TWO_DIGIT_INT = 0,						//������
	TWO_POINT_FLOAT = 1,				//��λС����ĸ���������
	EIGHT_POINT_DATA = 3,					//����
	EIGHT_POINT_TIME = 4,					//ʱ��
}VALTYPE;


/*
	��������GetU16dataFormFlash2Word(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP)     
	������	ȥȡ4�ֽ����ݣ���Ҫ���ü��ٴ���������
*/
void GetU16dataFormFlash2Word(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP);
/*
	float GetFlaotdataFormFlash2Word(u32 addr)     
	������	ȥȡ4�ֽ����ݣ���Ҫ���ü��ٴ���������
*/
float GetFlaotdataFormFlash2Word(u32 addr) ;
/*
	u32 GetU32dataFormFlash2Word(u32 addr)     
	������	ȥȡ4�ֽڸ������ݣ���Ҫ���ü��ٴ���������
*/
u32 GetU32dataFormFlash2Word(u32 addr);
/*
	�������SSaveU32ToFlash(u32 data,u32 addr) 
	������	����U32���ݵ�FLASE���棨��Ҫ���ü��ٴ���������
*/
void SaveU32ToFlash(u32 data,u32 addr);
/*
	�������SSaveU16ToFlash(u32 data,u32 addr) 
	������	����U16���ݵ�FLASE���棨��Ҫ���ü��ٴ���������
*/
void SaveU16ToFlash(u16 data,u32 addr) ;

/*
	�������Svoid SaveFlaotToFlash(float data,u32 addr) 
	������	���渡�����ݵ�FLASE���棨��Ҫ���ü��ٴ���������
*/
void SaveFlaotToFlash(float data,u32 addr);

/*
	�������void UpdataSensorFixDate(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP,Screen_DataStructure *p)               
	������	���´�������װʱ�䣨��Ҫ���ü��ٴ���������
  type:   0~7 ����Ĵ���������
*/

void UpdataSensorFixDate(FourBytes *four_p,u32 addr,Flashisp_DataStructure *fP,Screen_DataStructure *p,u8 type);
/*
	�������� StrinGalignment(* str,u8 len)
	������	�����ַ���������̶�����
*/
void StrinGalignment(u8* str,u8 len);
/*
	�������� void ScreenSetJitu(Flashisp_DataStructure *fP,Screen_DataStructure *p)
	������	�������ý��洦���ͼ
  tpye:   ��ͼ������ 0-5
*/
void ScreenSetJitu(Screen_DataStructure *p,u8 type);
/*
	��������void ScreenFirstOpButton(Screen_DataStructure *ps,u8 OpMax,u8 isNumberChose)
	������	 һ���˵���������
  OpMax:   һ�������˵����ֵ
  ButtonAddr��  ��Ӧ���水����ʼֵ
  isNumberChose�� �Ƿ��Ӧ��ݼ�����
*/
void ScreenFirstOpButton(Screen_DataStructure *ps,u8 OpMax,u8 isNumberChose);
/*
	�������� void ScreenTowOpButton(Screen_DataStructure *ps,u8 type,u8 OpMax,u8 isOpChang);
	������	�����˵���������
  tpye:   ���������˵�����
*/
void ScreenTowOpButton(Screen_DataStructure *ps,u8 type,u8 OpMax,u8 isOpChang);
/*
	�������� void ScreenTowOptChose(Screen_DataStructure *ps,u8 opt)
	������	�����˵�����ѡ����
  tpye:   ���������˵�����
*/
void ScreenTowOptChose(Screen_DataStructure *ps,u8 opt);
/*
	�������� ScreenBackLastPag(Screen_DataStructure *ps,u16 lastPag)
	������	�����ϼ�����
  tpye:   ���������˵�����
*/
void ScreenBackLastPag(Screen_DataStructure *ps,u16 lastMode);
/*
	��������void ScreenConfirmEnter(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min)
	������	������ֵȷ��  

*/
void ScreenConfirmEnter(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min);
/*
	��������void ScreenConfirmEnterDisBuffer(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min)
	������	������ֵȷ��----ȷ����ʾ���� 
  �洢����Ϊfloat ����
  //position �����ݵ�λ��
*/
void ScreenConfirmEnterDisBuffer(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min);
/*
	�������� ScreenBackLastPag(Screen_DataStructure *ps)
	������	����һ���˵�
*/
void ScreenBackFirstOpt(Screen_DataStructure *ps);
/*
	��������ScreenEnterNumberText(Screen_DataStructure *ps,u8 type,u8 len)
	������	��ֵ  
  ��ֵ���ͣ�type 0  �޷�������
            type 1  �з�������
            type 2  �޷��Ÿ�����
            type 3  �з��Ÿ�����
  �ı����ȣ�len
*/
void ScreenEnterNumberText(Screen_DataStructure *ps,u8 type,u8 len);
//---
/*
	��������void ScreenEnterNumberTwinkle(Screen_DataStructure *ps,u16 TextAddr)
	������	��ֵ������˸  

*/
void ScreenEnterNumberTwinkle(Screen_DataStructure *ps,u16 TextAddr);
/*
	��������void ScreenChangeDataInit(Screen_DataStructure *ps)
	������	ҳ���л����ݳ�ʼ��

*/
void ScreenChangeDataInit(Screen_DataStructure *ps);
/*
	��������void ScreenButtonReset(Screen_DataStructure *ps)
	������	����������λ

*/
void ScreenButtonReset(Screen_DataStructure *ps);
/* date 2019-12-26
	��������void Screen_CLYQ_YQCX_FZSJ_Pag(Screen_DataStructure *p)
	������	��������-������ѯ-��������
*/
void Screen_CLYQ_YQCX_FZSJ_Pag(Screen_DataStructure *p);
/*
	u8* GetYDBD_Describe(u8 Type)
	������	�����̵���������
*/
u8* GetYDBD_Describe(u8 Type);
/*
	��������u16 Dis_Tips(u8* Type)
	������	��ʾ��ʾ
*/
void Dis_Tips(Screen_DataStructure *p,u8* Tips,u16 TextAddr);
/*
	��������u16 Dis_CheckBox(u16 addr,u8 Stat,u32 DisXY)
	������	��ѡ
*/
void Dis_CheckBox(u16 addr,u8 Stat,u32 DisXY);
/*
	��������u16 Set_SysCheckBox(u16 addr,u16 ByteNumber)
	������	����-ϵͳ���ù�ѡ��/��ӡѡ��ѡ������
*/
void Set_SysCheckBox(u16 addr,u16 ByteNumber);
/*
	float GetFlueArea (void)
	������	��ȡ�̵����
*/
float GetFlueArea (void);
/*
	float YC_SampleControl (void)
	������	�̳��������ƴ���  ��Screen_SecTimer() �е���
*/
void YC_SampleControl(void);


void ScreenTowOpButton_KeyControl(Screen_DataStructure *ps, u8 isHavePoint,u8 isHaveSign);//��Ļ����¼��ʱ�ⲿ�����Ĵ���
void ScreenTowOpButton_KeyControl_adr(Screen_DataStructure *ps);//��ַ¼��ʱ�ⲿ�����Ĵ���
void ScreenTextTwinkle(Screen_DataStructure *ps,u16 TextAddr, u16 FlashAddress, u8 valType);//��ʱ�����ã�����淶ʱɾ����
void ScreenBackLastOpt(Screen_DataStructure *ps); //������һ���˵�
void SoftKeyboardConfirmEnter(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min);//�����ȷ�Ϻ�д������
void ProcessSoftKeyboardInput(Screen_DataStructure *ps,u32 Addr,u8 type, float Max,float Min,u8 isHavePoint,u8 isHaveSign);	//�����������������--��ֵ¼��
void ProcessSoftKeyboardInputFileName(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min,u8 isHavePoint,u8 isHaveSign);//�����������������--�ļ���¼��
void SoftKeyboardConfirmEnterDisBuffer(Screen_DataStructure *ps,float *DisBuffer,u8 position ,float Max,float Min);//����̻���¼��


#endif
