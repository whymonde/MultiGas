/*
	TestPower.c�⺯��
*/

#ifndef __TestPower_H
#define __TestPower_H

#include "stm32f4xx_conf.h"

#define TPOWER_LED    PBin(9)
#define TCHANG_LED    PBin(8)

//�ṹ��
typedef struct
{       u8 flag;                                        //��Դ����־
	    u8 RunMode;					//����ģʽ
	    u16 Timer;					//��ʱ��
        u16 Timer10ms;                                  //10ms��ʱ��
        u16 Cnt;                                        //������
        u16 StaCnt;                                     //״̬������
	 
	    u8 Changflg;                                    //��û�в�������־    
	    u16 ChangCnt;                                     //�����״̬������
	
	
}TestPower_DataStructure;





//----------------------------------------------------
void TestPower_Main(void);
void TestPower_Timer(void);
void TestPower_Init(void);
TestPower_DataStructure* TestPower_GetData(void);

void TestPower_DebugASCII(u8 *p,u8 len);


void TestPower_DebugASCII(u8 *p,u8 len);
#endif










