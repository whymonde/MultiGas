/*
	screen�⺯��
*/

#ifndef __NETWORK_H
#define __NETWORK_H

#include "stm32f4xx.h"

//���
#define POWER_4G_OUT(a)\
					if (a==0)\
						GPIO_ResetBits(POWER_4G_PORT,POWER_4G_PIN);\
					else if (a==1)\
						GPIO_SetBits(POWER_4G_PORT,POWER_4G_PIN);
#define NETLED_4G_IN 	GPIO_ReadInputDataBit(NETLED_4G_PORT,NETLED_4G_PIN)			
					

#define NETWORK_SandDataHanderH 0xaa   //���÷���֡�Ŀ�ʼ�ֽ�Ϊ:0xaa,0x55
#define NETWORK_SandDataHanderL 0x55
#define Network_ReceivLeng  50
#define USART_REC_LEN  50
//������غ궨��
//#define DS18B20_FACTORY					0XAAAA	//�ظ�����������

//�洢��ַ
//�������ݲ����洢��ַ
#define NETWORK_AdrStoreBase					FLASHIS_AdrBaseNetwork
#define NETWORK_AdrIPAdress						0	//IP��ַ��2���֣�4���ֽ�
#define NETWORK_AdrPortNumber					NETWORK_AdrIPAdress+2//�˿ںţ�1����
#define NETWORK_AdrTimeAndOnOff				NETWORK_AdrPortNumber+1//���ͼ��+��ͣ
#define NETWORK_AdrFactory						NETWORK_AdrTimeAndOnOff+1//�������ñ�־λ
#define NETWORK_AdrEND								NETWORK_AdrFactory+1//������־

typedef struct
{ 
	u8 flag;   //�����ݽ��ձ�׼
	u8 DataArr[Network_ReceivLeng]; //������������
}Network_ReceivDataStructure;

//����֡�ṹ��
typedef struct
{ u8 HanderH;   //��ʼ���ֽ�
	u8 HanderL;   //��ʼ���ֽ�
	u8 Length;    //����
	u8 DataArr[255];  // ���ݣ��������ݣ�Ч��
}Network_SendDataStructure;

/*
	Network�ṹ��
*/
typedef struct
{
	u8 Flag;					 //��־λ
	u8 HandOKflag;     //0=��û�����,1=���ֳɹ�,2=����ʧ��
  u8 SEND_OK;//���ͳɹ���ʶ
	u8 Temp;
  u8 USART_RX_BUF[USART_REC_LEN];//
	u16 USART_RX_STA;         		//
  u16 USART_RX_CNT;
	u8 AtMode;          //=0:������ģʽ;1=AT ����ģʽ
	u8 RunMode;					//����ģʽ,0:����ɨ��״̬
	u8 NextMode;        //��һҪ���е�״̬
	u8 SendCnt;         //���ͼ�����,3��
	u16 Timer;					//��ʱ��
	u16 Timer_50ms;     //50���붨ʱ��
	u16 Timer_3s;       //3�붨ʱ��
	u8 Hcnt;            //�ߵ�ƽ������
	u8 NetworkStatus;   //1 ������;0 �Ͽ�,��ר�Ŷ�ʱ����ɨ��������״̬�ڣ����־Ҳ�ṩ��Ļ������ʾ�����־��
	u8 LinkErrFlag;     //���Ӵ�����  =0:��������;  =1:��ʱ =2:���óɹ�
	Network_SendDataStructure  Network_SendData;  //���巢������֡
	Network_ReceivDataStructure Network_ReceivData;//����������ݵ����ݽṹ
}Network_DataStructure;


//----------------------------------------------------
void Network_Main(void);
void Network_Timer(void);
void Network_Init(void);
Network_DataStructure* Network_GetData(void);
void Network_SendNByteData(u8 *p,u8 Nbyte);
void Network_ReadData(u8 address,u8 *p,u8 len);
void Network_StoreData(u8 address,u8 *p,u8 len);
void Network_DebugASCII(u8 *p,u8 len);
void Network_SendOneFrameData(void);
void Network_Test(void);
#endif










