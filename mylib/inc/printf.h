/*
	prinrf�⺯��
*/

#ifndef __PRINTF_H
#define __PRINTF_H
#define OPEN_PRINTF		//�Ƿ����ô�ӡ���ܣ������õĺ궨��OPEN_PRINTF,���򲻶���OPEN_PRINTF=4G����

#include "stm32f4xx.h"


#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)		//���׼�⺯��Printf���
typedef struct 
	{
    u8 Printf_Mode;   //��ӡ���ò���
  }Printf_DataStructure;

Printf_DataStructure *Printf_GetData(void);
void Printf_Init(void);

#endif

