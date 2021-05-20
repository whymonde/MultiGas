/*
	FM24CL64.c�⺯��
*/

#ifndef __FM24CL64_H
#define __FM24CL64_H

#include "stm32f4xx.h"

//
#define FM24CL64_CMD_WRITE 			0xA0		//д����
#define FM24CL64_CMD_READ				0xA1		//������


//�ṹ��
typedef struct
{
	u8 RunMode;					//����ģʽ
	u16 Timer;					//��ʱ��
}FM24CL64_DataStructure;





//----------------------------------------------------
void FM24CL64_Main(void);
void FM24CL64_Timer(void);
void FM24CL64_Init(void);

FM24CL64_DataStructure* FM24CL64_GetData(void);

void FM24CL64_WriteData(void);
void FM24CL64_ReadData(void);


void FM24CL64_TestReadData(void);
void FM24CL64_TestWriteData(void);

void FM24CL64_DebugASCII(u8 *p,u8 len);
//void FM24CL64_DebugRTU(u8 *p,u8 len);


#endif










