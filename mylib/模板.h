/*
	screen.c�⺯��
*/

#ifndef __SCREEN_H
#define __SCREEN_H

#include "stm32f10x.h"

//�ṹ��
typedef struct
{
	u8 RunMode;					//����ģʽ
	u16 Timer;					//��ʱ��
}Screen_DataStructure;





//----------------------------------------------------
void Screen_Main(void);
void Screen_Timer(void);
void Screen_Init(void);
Screen_DataStructure* Screen_GetData(void);

void Screen_ReadData(u8 address,u8 *p,u8 len);
void Screen_StoreData(u8 address,u8 *p,u8 len);
void Screen_DebugASCII(u8 *p,u8 len);


void Screen_DebugASCII(u8 *p,u8 len);
//void Screen_DebugRTU(u8 *p,u8 len);






#endif










