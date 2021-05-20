/*
	S18B20�⺯��
*/

#ifndef __S18B20_H
#define __S18B20_H

#include "stm32f10x.h"


//���
#define S18B20_TX(a)\
					if (a==1)\
						GPIO_SetBits(S18B20_GPIO_PORT,S18B20_GPIO_PIN);\
					else if (a==0)\
						GPIO_ResetBits(S18B20_GPIO_PORT,S18B20_GPIO_PIN);\
					else if(a==2)\
						S18B20_GPIO_PORT->ODR ^= S18B20_GPIO_PIN;
#define S18B20_RX() 				GPIO_ReadInputDataBit(S18B20_GPIO_PORT, S18B20_GPIO_PIN)
#define S18B20_IO_IN()  		{GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8<<0;}
#define S18B20_IO_OUT() 		{GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3<<0;}


/*
	�ṹ��
*/
typedef struct
{
//	u8 Flag;						//��־λ
	u8 RunMode;					//����ģʽ
	u8 WorkMode;				//����ģʽ
	u8 CntOne;					//������1
	u8 CntTwo;					//������2
	u16 Timer;					//��ʱ��
}S18B20_DataStructure;





//----------------------------------------------------
void S18B20_Main(void);
void S18B20_Work(void);
void S18B20_Init(void);
void S18B20_Timer(void);
void S18B20_TimerInit(void);
void S18B20_DelayTime(u16 tim);
void S18B20_InitGPIO(void);

S18B20_DataStructure* S18B20_GetData(void);

void S18B20_DebugASCII(u8 *p,u8 len);
void S18B20_DebugRTU(u8 *p,u8 len);


//--------------------------------------------------------

#define DS18B20_PIN                         GPIO_Pin_0
#define DS18B20_GPIO_PORT                   GPIOA
#define DS18B20_GPIO_CLK                    RCC_APB2Periph_GPIOA  

#define DS18B20_DATA_IN  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define DS18B20_OUT_HIGH GPIO_SetBits(GPIOA, GPIO_Pin_0);
#define DS18B20_OUT_LOW  GPIO_ResetBits(GPIOA, GPIO_Pin_0);
#define DS18B20_IO_IN()  {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=8<<0;}
#define DS18B20_IO_OUT() {GPIOA->CRL&=0XFFFFFFF0;GPIOA->CRL|=3<<0;}
   	
uint8_t DS18B20_Init(void);//��ʼ��DS18B20
short DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Write_Byte(uint8_t dat);//д��һ���ֽ�
uint8_t DS18B20_Read_Byte(void);//����һ���ֽ�
uint8_t DS18B20_Read_Bit(void);//����һ��λ
uint8_t DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Reset(void);       //��λDS18B20

uint8_t DS18B20_Get_Serial(uint8_t *serial);
uint8_t GetCRC(uint8_t *str, uint32_t length);

//--------------------------------------------------------
void SZ_STM32_SysTickInit(uint32_t HzPreSecond);
void TimingDelay_Decrement(void);
void Delay_us(__IO u32 nTime);
void Delay_ms(__IO u32 nTime);
void Delay_s(__IO u32 nTime);
//--------------------------------------------------------

#endif










