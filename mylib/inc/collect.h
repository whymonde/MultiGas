/*
	collect�⺯��
*/

#ifndef __COLLECT_H
#define __COLLECT_H

#include "stm32f10x.h"


/*
	�����շ��ṹ��
*/
typedef struct
{
	u8 RunMode;					//����ģʽ
	u16 Timer;						//��ʱ��1
	u16 TimerTwo;					//��ʱ��2
}Collect_DataStructure;



//�⺯��
void Collect_main(void);
void Collect_Init(void);
void Collect_Timer(void);
Collect_DataStructure* Collect_GetCollectData(void);
void Collect_CollectDataInit(void);


void Collect_DebugASCII(u8 *p,u8 len);
void Collect_DebugRTU(u8 *p,u8 len);

#endif
