/*
	���ļ�������2014��11��19��
	���ļ�Ϊ���ڴ洢оƬW25X16/32/64��������Ӧ�ýӿ�
	˵�����ļ������洢оƬ��Ӳ���㡢������ͽӿڲ㣨Ӧ�ò㣩
	Ӳ���㣺�ò�Ϊ�뵥Ƭ���йأ���Ϊ�������ṩ����ӿڣ���Ҫ����IO�ڵĳ�ʼ������
					��ĳ�ʼ�����Լ����������ṩ�Ľӿڣ����ݲ�ͬ�ĵ�Ƭ����ֻ��Ҫ�޸ĸò�
					���ɣ�������ֲ��
	�����㣺�ò�Ϊ��оƬW25XxxоƬ�йأ��ò������������оƬ����С����ָ���Լ��㷨
					�����������д�����ݣ��洢���ݵ����еĲ���
	Ӧ�ò㣺�ò�ΪΪ�ⲿӦ���ṩ��׼�ӿڣ�����ʹ�ã�ʹʹ���߲���Ҫ�˽�����������й��̣�
					ֻ�谴�սӿ�д�����ݼ���
	W25XxxоƬ�򵥽��ܣ�W25X16/32/64Ϊ�������������SPI�ӿ�flash�洢оƬ�������ֱ���16M-bit,
					32M-bit,64M-bit,��ϵ��оƬ֧�����75MHz��ʱ�����ߡ�
					
	
*/
#ifndef __W25XXX_H
#define __W25XXX_H
#include "stm32f10x.h"
//�����
#define W25XXX_WriteEnable                0x06 		//дʹ������
#define W25XXX_WriteDisable               0x04 		//д��������
#define W25XXX_ReadStatusReg              0x05 		//��״̬�Ĵ�������
#define W25XXX_WriteStatusReg             0x01 		//д״̬�Ĵ�������
#define W25XXX_ReadData                   0x03 		//����������
#define W25XXX_FastReadData               0x0B 		//���ָ��
#define W25XXX_FastReadDual               0x3B 		//���˫���ָ��
#define W25XXX_PageProgram                0x02 		//ҳ���ָ��
#define W25XXX_SectorErase                0x20 		//��������ָ��
#define W25XXX_BlockErase                 0xD8 		//�����ָ��
#define W25XXX_ChipErase                  0xC7 		//оƬ����ָ��
#define W25XXX_PowerDown                  0xB9 		//����ָ��
#define W25XXX_ReleasePowerDown           0xAB 		//�ͷŵ���/����IDָ��
#define W25XXX_DeviceID                   0xAB 		//�ͷŵ���/����IDָ��
#define W25XXX_ManufactDeviceID           0x90 		//������/������ָ��
#define W25XXX_JedecDeviceID              0x9F 		//JEDEC IDָ��

#define W25XXX_PageSize              			256			//ҳ����
#define W25XXX_PerWritePageSize      			256			//ҳ��̳���

#define W25XXX_FlagBUSY                   0x01  	/* Write In Progress (WIP) flag */
#define W25XXX_DummyByte                  0xAA		//������ݣ����ڶ�ȡ����

#define W25XXX_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)	//Ƭѡ����/CS����
#define W25XXX_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)  //Ƭѡ����/CS����


//�洢��ַ��ÿһ�����һ��������
#define W25XXX_Adr_CraneBase							0x000000		//�����洢��ַ
#define W25XXX_Adr_Slewing								0x1000


#define W25XXX_AdrTowerBase								0x1000*0		//���������洢��ַ
#define W25XXX_AdrMCH58Base								0x1000*1		//�������洢��ַ
#define W25XXX_AdrTHBSS01Base							0x1000*2		//���ش洢��ַ
#define W25XXX_AdrOHRD23Base							0x1000*3		//�����洢��ַ
#define W25XXX_AdrYCFS260Base							0x1000*4		//���ٴ洢��ַ
#define W25XXX_AdrDS18B20Base							0x1000*5		//�¶ȴ洢��ַ
#define W25XXX_AdrSystemBase							0x1000*6		//ϵͳ�洢��ַ
#define W25XXX_AdrPAC193XmBase						0x1000*7		//AC193X�洢��ַ




//�⺯��
void W25Xxx_Init(void);

//Ӳ����
void W25Xxx_GPIOInit(void);

void W25Xxx_ReadData(u8 *p,u32 Addr,u16 Len);
void W25Xxx_PageWrite(u8 *p,u32 Addr,u16 Len);
void W25Xxx_EraseSector(u32 Addr);
void W25Xxx_EraseBlock(u32 Addr);
void W25Xxx_EraseChip(void);

void W25Xxx_WriteEnable(void);
void W25Xxx_WriteDisable(void);
u8 W25Xxx_ReadStatusRegister(void);
void W25Xxx_WaitForWriteEnd(void);
u8 W25Xxx_ReadDeviceID(void);
u32 W25Xxx_ReadJEDECID(void);
u8 W25Xxx_SendByte(u8 buffer);
void W25Xxx_PowerDown(void);
void W25Xxx_WakeUp(void);

//���Ժ���
void W25Xxx_DebugASCII(u8 *p,u8 len);
void W25Xxx_DebugRTU(u8 *p,u8 len);
#endif


