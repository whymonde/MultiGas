/*
	server�⺯��
*/

#ifndef __SERVER_H
#define __SERVER_H

#include "stm32f10x.h"
//Ӳ���궨��
//#define SERVERPWR_APP					RCC_APB2Periph_GPIOA	//ServerPWR��Դ�ܽŵ�ʱ����
//#define SERVERPWR_PORT				GPIOA									//ServerPWR��Դ�ܽŶ˿�
//#define SERVERPWR_PIN					GPIO_Pin_10						//ServerPWR��Դ�ܽ�
//#define SERVERPWR_OFF					GPIO_SetBits(SERVERPWR_PORT, SERVERPWR_PIN)		//ServerPWR��Դ�ر�
//#define SERVERPWR_OPEN				GPIO_ResetBits(SERVERPWR_PORT, SERVERPWR_PIN)	//ServerPWR��Դ��

//���ݺ궨��
#define SERVER_DefaultAddress					0x08		//A1ͨѶģ��Ĭ�ϵ�ַ

//����ģʽ
#define SERVER_RunModeNone						0				//ʲôģʽҲû������
#define SERVER_RunModeWrit30to44			0x10		//������д30~44ģʽ
#define SERVER_RunModeWrit45to59			0x11		//������д45~59ģʽ
#define SERVER_RunModeWrit60to74			0x12		//������д60~74ģʽ
#define SERVER_RunModeWrit75to76			0x13		//������д75~76ģʽ
#define SERVER_RunModeRead0to6				0x14		//�����ڶ�0~6ģʽ
#define SERVER_RunModeWritNewModule		0x15		//д��ģ��

//---------------------------------------------------------------
//Ӳ���˿ں궨��
#define Server_SendBufferData	 			USARTx_StoreBufferData			//RTU���ݷ��ͺ���
//A1�˿�(RS485)
#define Server_UsartServerPort			USART3											//��Ӧ��������˿�
#define Server_UsartServerBuffer		Get_USART3Buffer()					//��Ӧ���ڽṹ��
#define Server_SetComEvenVerify			USART_SetEvenVerify					//���ö˿ں�ΪżУ��ģʽ


/*
	������������������ݽṹ��ṹ��
*/
typedef struct 
{
	u8 Flag;						//����״̬��־λ��b0=1(ϵͳ��������b1=1(ϵͳ����)
	u8 Address;					//A1ͨѶģ���ַ
	u8 RunMode;					//����ģʽ
	u32 LocalNumber;		//A1ģ�����к�
	u16 Timer;					//ϵͳ�ۼ�����ʱ�䣬��λ��
	u32 SysTime;
	u16 Manufacturer;		//���������̣����������̴��룬��ï�����Ĵ���Ϊ22861��
	u16 Version;				//�����ͺţ������ͺţ�����STT553����������Ϊ553��
	u32 SerialNumber;		//������ţ��������кţ����к���32λ�޷���������ʾ�����֧��10λ���֣�
	u16	Height;					//�����߶ȣ�������ǰ��װ�߶ȣ���λΪ�ף�
	u16 Length;					//���۳��ȣ�������ǰ�۳�����λΪ�ף�
	u16 RatedLoad;			//�����(�����Ķ������������λΪ��)
	s16 OutdoorsTem;		//�����¶�(������ʻ�Ҹ������£���λΪ0.1���϶ȣ������軧���¶�Ϊ24.5�ȣ���Ϊ245)
	u16 WindSpeed;			//���٣��������ڵصĵ�ǰ���٣���λΪ0.1��/�룩
	u16 UpHour;					//����ʱ�䣺Сʱ�������Կ����󾭹���ʱ���Сʱ���֣���ʱ�����ڹػ������㣩
	u16 UpMinutes;			//����ʱ�䣺����
	u16 UpSec;					//����ʱ�䣺��
	u16 WorkingHour;		//����ʱ�䣺Сʱ�������Կ�������Ч������ʱ���Сʱ���֣���ʱ�����ڹػ������㣩
	u16 WorkingMinutes;	//����ʱ�䣺����
	u16 WorkingSec;			//����ʱ�䣺��
	u16 Rotation;				//��ת�Ƕȣ�������ת���ݣ���λΪ�ȣ�ȡֵ��Χ0��359������ָ������ʱ���Ϊ0�ȣ�
	u16 Range;					//���������������ݣ���λΪ0.01�ף�
	u16 Raising;				//�����������������ݣ���λΪ0.01�ף�
	u16 HookHeight;			//�����߶ȣ�����Ŀǰ�����߶ȣ���λΪ0.01�ף��Ե���λ�ڵ���Ϊ0�ף�
	u16 Hoisting;				//��ǰ���������������ǰ�������������λΪ0.01�֣�����������Ϊ2583ǧ�ˣ���Ϊ258��
	u16 CurrentPower;		//��ǰ���ʣ���ǰ�������ܹ������ʣ���λΪ0.1ǧ�ߣ�
	u16 Momental;				//���أ���ǰ�����������شﵽ������صİٷֱȣ�ȡֵ��ΧΪ0��100��
	u16 TaErrorCode; 		//����������루����ϵͳ������룬�������ݺ���ȷ�ϣ�
	u16 BianErrorCode;	//��Ƶ��������루������Ƶ��������룬�������ݺ���ȷ�ϣ�
	u32 PlcInState;			//PLC����״̬��PLC�������ź�״̬�����λBit��ӦX0���������ơ�0Ϊ���źţ�1Ϊ���źţ�
	u32 PlcOutState;		//PLC���״̬��PLC�������ź�״̬��ͬ�ϣ�
}Sever_TowerCraneStructure;



//�⺯��
void Server_Init(void);//�ú���Ϊserver.c��ʼ������
void Server_Timer(void);//�ú���Ϊserver.c�ļ���ʱ�Ӻ�����ʱ�ӵ�λΪ1ms

void Server_PwrIoInt(void);//A1��Դ��ʼ���˿�

void Server_InitTowerCraneData(void);//��TowerCraneData�ṹ��������г�ʼ��
Sever_TowerCraneStructure *Server_GetTowerCraneData(void);//�ú���Ϊ���ڻ�ȡServer.c�ṹ�����TowerCraneData�ĵ�ַ

void Server_Writ30to44(void);
void Server_Writ45to59(void);
void Server_Writ60to74(void);
void Server_Writ75to76(void);
void Server_Read0to6(void);
void Server_WritNewModule(void);
void Server_UsartModeSet(void);
//���ԡ�ͨѶ����
void Server_DebugASCII(u8 *p,u8 len);//�ú���ΪServer.c�ļ���ASCII�����ļ�
void Server_DebugRTU(u8 *p,u8 len);//�ú���ΪServer.c�ļ���RTU�����ļ�


#endif




















