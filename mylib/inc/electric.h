/*
	Electric�⺯��
*/

#ifndef __ELECTRIC_H
#define __ELECTRIC_H

#include "stm32f10x.h"


#define ELECTRIC_DefaultAddress			0x01		//���ٴ�����Ĭ�ϵ�ַ
//ͨѶ����
#define ELECTRIC_ReadWords					0x03		//��ȡ�Ĵ�������������
#define ELECTRIC_WriteOneWord				0x06		//��Ŀ��Ĵ���д��һ��������

//OHR-D23�Ĵ�����ַ
#define ELECTRIC_AddBasicData				0x0100		//���������Ĵ����׵�ַ
#define ELECTRIC_AddEnergyData			0x0600		//���������յ�ַ
#define ELECTRIC_AddParameterData		0x0903		//�����׵�ַ
#define ELECTRIC_AddVoltageRatio		0x0903		//��ѹ���ʵ�ַ
#define ELECTRIC_AddCurrentRatio		0x0904		//�������ʵ�ַ
#define ELECTRIC_AddWiringWay				0x0905		//���߷�ʽ��ַ
#define ELECTRIC_AddCommAddress			0x0906		//ͨѶ��ַ��ַ
#define ELECTRIC_AddBaudRate				0x0907		//�����ʵ�ַ

#define ELECTRIC_LenBasicData				0x0034	//���������Ĵ�������������26��
#define ELECTRIC_LenEnergyData			0x000C	//�������ݵ�ַ,6��
#define ELECTRIC_LenParameterData		0x0005		//�����׵�ַ

//����ģʽ
#define ELECTRIC_RunModeNone						0				//ʲôģʽҲ��������
#define ELECTRIC_RunModeBasicData				0x10		//��ȡ�������ܲ���ģʽ
#define ELECTRIC_RunModeEnergyData			0x11		//��ȡ���ܲ���ģʽ
#define ELECTRIC_RunModeParameterData		0x12		//��ȡ���ò���ģʽ
#define ELECTRIC_RunModeVoltageRatio		0x20		//д���ѹ���ʲ���ģʽ
#define ELECTRIC_RunModeCurrentRatio		0x21		//д��������ʲ���ģʽ
#define ELECTRIC_RunModeWiringWay				0x22		//д����߷�ʽ����ģʽ
#define ELECTRIC_RunModeCommAddress			0x23		//д��ͨѶ��ַ����ģʽ
#define ELECTRIC_RunModeBaudRate				0x24		//д��ͨѶ�����ʲ���ģʽ	


//---------------------------------------------------------------
//Ӳ���˿ں궨��
#define Electric_SendBufferData	 			USARTx_StoreBufferData			//RTU���ݷ��ͺ���
//�ɼ��˿�(RS485)
#define Electric_UsartPort						USART3											//��Ӧ��������˿�
#define Electric_UsartBuffer					Get_USART3Buffer()					//��Ӧ���ڽṹ��
#define Electric_SetComNoneVerify			USART_SetNoneVerify				//���ö˿ں�Ϊ��У��ģʽ



/*
	�����շ��ṹ��
*/
typedef struct
{
//	u8 Flag;						//��ر�־λ
	u8 Address;					//���ճ���
	u8 RunMode;					//����ģʽ�������жϵ�ǰ��ȡ����Щ���ݣ�����д����Щ����
}Electric_DataStructure;


//�⺯��
void Electric_Init(void);
void Electric_DataInit(void);
Electric_DataStructure* Electric_GetData(void);
void Electric_CollectBasicData(void);
void Electric_SendReadCmd(u16 address,u16 len);
void Electric_WriteOneWord(u16 address,u16 data);
void Electric_DebugASCII(u8 *p,u8 len);
void Electric_DebugRTU(u8 *p,u8 len);

#endif
