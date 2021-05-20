/*
	���ļ�Ϊcalculate.c���ļ�
	���ļ��������º���
	u16 charTOu16(u8 *p,u8 len)	//���ַ���ת����16λ���޷�����������
	
*/
#ifndef __CALCULATE_H
#define __CALCULATE_H
#include "stm32f4xx.h"



unsigned short CRC16(unsigned char *puchMsg,unsigned short usDataLen);	//��׼CRC�㷨
u8 AnalysisCRC(u8 *pstr);//�Ƚ�������CRC�Ƿ����
unsigned int Calculate_CRC16(unsigned char const * pucY, unsigned char ucX);//��׼CRC�㷨�������ֽڱ�
u16 Calculate_CharToU16(u8 *p,u8 len);	//���ַ���ת����16λ���޷�����������
u32 Calculate_CharToU32(u8 *p,u8 len);	//���ַ���ת����32λ���޷�����������
//u16 Calculate_HexStrToU16(u8 *p,u8 len);//�ú���Ϊ��ʮ�������ַ���ת��Ϊ�޷���16������
u8 BeforeToBafter(u8 buffer);	//�ú������ڽ�һ��8λ���޷������ݵ�ÿһλǰ��Ի���b0-b7,b1-b6����������
float Calculate_CharToFloat(u8 *p,u8 len);//�ú������ڽ�һ�������ַ�ת��Ϊ����������
void Calculate_TimeToFrequency(u32 *buffer);//�����ڻ����Ƶ��
void Calculate_FourBigEndToSmallEnd(FourBytes *p);//���ֽڿռ�Ĵ�С��ת��
void Pid_Calculate(PID_DataStructure* p);
void Calculate_DebugASCII(u8 *p,u8 len);	//�ú���ΪCalculate.c�ļ��ĵ��Ժ�������ϵͳ���Ժ�������
u32  JiCai_Own(u8 year,u8 month,u8 day,u8 hour,u8 min,u8 sec);  //������絽�ϵ�ʱ���
float  Calc_O2_K(float biaodinzhi,u32 ad);   //���������ĳ���K;
float  Calc_O2(float K,u32 ad);   //��������Ũ��;
float  Calc_SO2_K(float biaodinzhi,u32 ad);   //���������ĳ���K;
float  Calc_SO2(float K,u32 ad);   //��������Ũ��;

#endif

