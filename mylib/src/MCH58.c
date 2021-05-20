/*
	���ļ�Ϊ����Ŀ��MCH58��������غ���
	���庯������
*/

#include "MCH58.h"

MCH58_DataStructure MCH58Data;
/*
	��������void MCH58_Main(void)
	������	��Ļ��ʾ������
*/
void MCH58_Main(void)
{
	switch(MCH58Data.RunMode)
	{
		//������1
		case 1:
		{
			MCH58_CollectValue(&MCH58Data.Mch58[0]);
			MCH58Data.Timer = 200;
			MCH58Data.RunMode = 2;
			break;
		}
		case 2:
		{
			if(MCH58Data.CollectMode == 0)
			{
				MCH58Data.RunMode = 1;
			}
			else if(MCH58Data.Timer == 0)
			{
				MCH58Data.RunMode = 1;
			}
			break;
		}
		//������2
		case 11:
		{
			MCH58_CollectValue(&MCH58Data.Mch58[1]);
			MCH58Data.Timer = 200;
			MCH58Data.RunMode = 12;
			break;
		}
		case 12:
		{
			if(MCH58Data.CollectMode == 0)
			{
				MCH58Data.RunMode = 11;
			}
			else if(MCH58Data.Timer == 0)
			{
				MCH58Data.RunMode = 11;
			}
			break;
		}
		//������3
		case 21:
		{
			MCH58_CollectValue(&MCH58Data.Mch58[2]);
			MCH58Data.Timer = 200;
			MCH58Data.RunMode = 22;
			break;
		}
		case 22:
		{
			if(MCH58Data.CollectMode == 0)
			{
				MCH58Data.RunMode = 21;
			}
			else if(MCH58Data.Timer == 0)
			{
				MCH58Data.RunMode = 21;
			}
			break;
		}
		case 31:
		{//�������1��������
			MCH58_UartModeSet(MCH58_UsartPort,9600,'n','1');
			MCH58_Con1_ON();
			MCH58Data.Timer = 20;
			MCH58Data.RunMode = 32;
			break;
		}
		case 32:
		{
			if(MCH58Data.Timer == 0)
			{
				u8 bf;
				MCH58_Operation(4,&bf,1);
				if(bf == 2)
				{
					MCH58_SendBufferData(MCH58_UsartPort,MCH58_UsartBuffer(),0xF2);
					printf("��ʱ��Ӽ���\r\n");
				}
				else
				{
					MCH58_SendBufferData(MCH58_UsartPort,MCH58_UsartBuffer(),0xF1);
					printf("˳ʱ��Ӽ���\r\n");
				}
				MCH58Data.Timer = 300;
				MCH58Data.RunMode = 33;
			}
			break;
		}
		case 33:
		{
			if(MCH58Data.Timer == 0)
			{
				MCH58_PWR_OFF();
				MCH58Data.Timer = 20;
				MCH58Data.RunMode = 34;
			}
			break;
		}
		case 34:
		{
			if(MCH58Data.Timer == 0)
			{
				MCH58_PWR_ON();
				MCH58_Con1_OFF();
				printf("�������\r\n");
				MCH58Data.RunMode = 0;
			}
			break;
		}
	}
}
//---
/*
	��������void MCH58_Init(void)
	������	MCH58.c�ļ���ʼ������
*/
void MCH58_Init(void)
{
	MCH58_GpioInit();
	MCH58_DataInit();
}
//---
/*
	��������void MCH58_DataInit(void)
	������MCH58���������ݳ�ʼ��
*/
void MCH58_DataInit(void)
{
	TwoBytes two;
	//������1
	//��ַ
	MCH58_Operation(6,two.U8Data,1);
	MCH58Data.Mch58[0].Address = two.U8Data[0];
	//������
	MCH58_Operation(7,two.U8Data,2);
	MCH58Data.Mch58[0].BaudRate = two.U16Data;
	//ӳ��
	MCH58_Operation(5,two.U8Data,1);
	MCH58Data.Mch58[0].Map = two.U8Data[0];
	//���ֵ
	MCH58_Operation(2,two.U8Data,2);
	MCH58Data.Mch58[0].ZeroValue = two.U16Data * 4096;
	MCH58_Operation(3,two.U8Data,2);
	MCH58Data.Mch58[0].ZeroValue += two.U16Data;
	//������2
	//��ַ
	MCH58_Operation(106,two.U8Data,1);
	MCH58Data.Mch58[1].Address = two.U8Data[0];
	//������
	MCH58_Operation(107,two.U8Data,2);
	MCH58Data.Mch58[1].BaudRate = two.U16Data;
	//ӳ��
	MCH58_Operation(105,two.U8Data,1);
	MCH58Data.Mch58[1].Map = two.U8Data[0];
	//���ֵ
	MCH58_Operation(102,two.U8Data,2);
	MCH58Data.Mch58[1].ZeroValue = two.U16Data * 4096;
	MCH58_Operation(103,two.U8Data,2);
	MCH58Data.Mch58[1].ZeroValue += two.U16Data;
	//������3
	//��ַ
	MCH58_Operation(206,two.U8Data,1);
	MCH58Data.Mch58[2].Address = two.U8Data[0];
	//������
	MCH58_Operation(207,two.U8Data,2);
	MCH58Data.Mch58[2].BaudRate = two.U16Data;
	//ӳ��
	MCH58_Operation(205,two.U8Data,1);
	MCH58Data.Mch58[2].Map = two.U8Data[0];
	//���ֵ
	MCH58_Operation(202,two.U8Data,2);
	MCH58Data.Mch58[2].ZeroValue = two.U16Data * 4096;
	MCH58_Operation(203,two.U8Data,2);
	MCH58Data.Mch58[2].ZeroValue += two.U16Data;
}
//---
/*
	��������void MCH58_GpioInit(void)
	������	MCH58���б��������GPIO�ܽų�ʼ��
*/
void MCH58_GpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(PwrV24_PWR_RCC, ENABLE); //��ʱ��
	RCC_APB2PeriphClockCmd(MCH58_Con1_RCC, ENABLE); 
	RCC_APB2PeriphClockCmd(MCH58_Con2_RCC, ENABLE); 
	RCC_APB2PeriphClockCmd(MCH58_Con3_RCC, ENABLE); 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure.GPIO_Pin = PwrV24_PWR_PIN;
	GPIO_Init(PwrV24_PWR_PORT, &GPIO_InitStructure);//24V��Դ
	GPIO_InitStructure.GPIO_Pin = MCH58_Con1_PIN;
	GPIO_Init(MCH58_Con1_PORT, &GPIO_InitStructure);//������1���ƶ˿�
	GPIO_InitStructure.GPIO_Pin = MCH58_Con2_PIN;
	GPIO_Init(MCH58_Con2_PORT, &GPIO_InitStructure);//������2���ƶ˿�
	GPIO_InitStructure.GPIO_Pin = MCH58_Con3_PIN;
	GPIO_Init(MCH58_Con3_PORT, &GPIO_InitStructure);//������3���ƶ˿�
	MCH58_PWR_ON();
	MCH58_Con1_OFF();
	MCH58_Con2_OFF();
	MCH58_Con3_OFF();
}
//---
/*
	��������void MCH58_Timer(void)
	������MCH58.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void MCH58_Timer(void)
{
	if(MCH58Data.Timer != 0)MCH58Data.Timer--;
}
//---
/*
	��������MCH58_DataStructure* MCH58_GetMCH58Data(void)
	��������ȡ����MCH58.c�ļ�ȫ�ֱ����ṹ��
*/
MCH58_DataStructure* MCH58_GetMCH58Data(void)
{
	return &MCH58Data;
}
//---
/*
	��������void MCH58_CollectValue(MCH58_Structure *p)
	������	�ú���Ϊ�������Ͳɼ�������ֵ����
	���룺	MCH58_Structure *p��Ҫ�ɼ��ı������ṹ���ַ
*/
void MCH58_CollectValue(MCH58_Structure *p)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = p->Address;
	buffer[cnt++] = 0x03;//��ȡ����
	buffer[cnt++] = 0x00;//��ʼ��ַ��2�ֽ�
	buffer[cnt++] = 0x02;
	buffer[cnt++] = 0x00;//���ݳ��ȣ�2�ֽ�
	buffer[cnt++] = 0x02;
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	MCH58_UsartModeSet(p);//���ö˿�ģʽ
	for(crc=0;crc<cnt;crc++)
	{
		MCH58_SendBufferData(MCH58_UsartPort,MCH58_UsartBuffer(),buffer[crc]);//��������
	}
	if(p == &MCH58Data.Mch58[0])
	{
		MCH58Data.CollectMode = MCH58_Con1RunModeBasic + MCH58_SetCollectMode(MCH58_CmdReadData);
	}
	else if(p == &MCH58Data.Mch58[1])
	{
		MCH58Data.CollectMode = MCH58_Con2RunModeBasic + MCH58_SetCollectMode(MCH58_CmdReadData);
	}
	else if(p == &MCH58Data.Mch58[2])
	{
		MCH58Data.CollectMode = MCH58_Con3RunModeBasic + MCH58_SetCollectMode(MCH58_CmdReadData);
	}
}
//---
/*
	��������u8 MCH58_SetCollectMode(u8 cmd)
	������	���ò�ͬ�����ͬ��CollectModeֵ
*/
u8 MCH58_SetCollectMode(u8 cmd)
{
	u8 buffer=0;
	switch(cmd)
	{
		case MCH58_CmdReadData:
		{//���ر�������ֵ
			buffer = 1;
			break;
		}
		case MCH58_CmdSetAddress:
		{//���õ�ַ
			buffer = 2;
			break;
		}
		case MCH58_CmdBaudRate4800:
		case MCH58_CmdBaudRate9600:
		case MCH58_CmdBaudRate19200:
		case MCH58_CmdBaudRate57600:
		case MCH58_CmdBaudRate115200:
		{//���ò�����
			buffer = 3;
			break;
		}
		case MCH58_CmdReturnSet:
		{//��ȡ�趨ֵ
			buffer = 4;
			break;
		}
		case MCH58_CmdReturnValue:
		{//��ȡ��ǰ��������ֵ
			buffer = 5;
			break;
		}
		case MCH58_CmdRetrunBit:
		{//��ȡ��Ȧ���Ȧλ����
			buffer = 6;
			break;
		}
		case MCH58_CmdSet2047Circle:
		{//���õ�ǰȦ��Ϊ2047Ȧ
			buffer = 7;
			break;
		}
		case MCH58_CmdSetCircle:
		{//�趨��ǰȦ��0~15Ȧ
			buffer = 8;
			break;
		}
		case MCH58_CmdClockwiseAdd:
		{//����Ϊ˳ʱ��+����
			buffer = 9;
			break;
		}
		case MCH58_CmdAnticlockwiseAdd:
		{//����Ϊ��ʱ��+����
			buffer = 10;
			break;
		}
	}
	return buffer;
}
//---
/*
	��������void MCH58_Operation(u16 mode,u8 *p,u8 len)
	���������ݲ���
*/
void MCH58_Operation(u16 mode,u8 *p,u8 len)
{
	switch(mode)
	{
		//---------------------------
		//��������1���ò���
		case 1:
		{//��������1���в���
			MCH58_ReadStoreData(MCH58_AdrStoreBM1Base,p,len);
			break;
		}
		case 2:
		{//��������1����Ȧֵ
			MCH58_ReadStoreData(MCH58_AdrStoreBM1ZeroH,p,len);
			break;
		}
		case 3:
		{//��������1��㵥Ȧֵ
			MCH58_ReadStoreData(MCH58_AdrStoreBM1ZeroL,p,len);
			break;
		}
		case 4:
		{//��������1�Ӽ�������
			MCH58_ReadStoreData(MCH58_AdrStoreBM1CD,p,len);
			break;
		}
		case 5:
		{//��������1�豸ӳ��
			MCH58_ReadStoreData(MCH58_AdrStoreBM1YSSB,p,len);
			break;
		}
		case 6:
		{//��������1�豸��ַ
			MCH58_ReadStoreData(MCH58_AdrStoreBM1ADR,p,len);
			break;
		}
		case 7:
		{//��������1ͨѶ������
			MCH58_ReadStoreData(MCH58_AdrStoreBM1BD,p,len);
			break;
		}
		//�޸ı�����1���ò���
		case 51:
		{//�޸ı�����1���в���
			MCH58_StoreData(MCH58_AdrStoreBM1Base,p,len);
			break;
		}
		case 52:
		{//�޸ı�����1����Ȧֵ
			MCH58_StoreData(MCH58_AdrStoreBM1ZeroH,p,len);
			break;
		}
		case 53:
		{//�޸ı�����1��㵥Ȧֵ
			MCH58_StoreData(MCH58_AdrStoreBM1ZeroL,p,len);
			break;
		}
		case 54:
		{//�޸ı�����1�Ӽ�������
			MCH58_StoreData(MCH58_AdrStoreBM1CD,p,len);
			break;
		}
		case 55:
		{//�޸ı�����1�豸ӳ��
			MCH58_StoreData(MCH58_AdrStoreBM1YSSB,p,len);
			break;
		}
		case 56:
		{//�޸ı�����1�豸��ַ
			MCH58_StoreData(MCH58_AdrStoreBM1ADR,p,len);
			break;
		}
		case 57:
		{//�޸ı�����1ͨѶ������
			MCH58_StoreData(MCH58_AdrStoreBM1BD,p,len);
			break;
		}
		//---------------------------
		//��������2���ò���
		case 101:
		{//��������2���в���
			MCH58_ReadStoreData(MCH58_AdrStoreBM2Base,p,len);
			break;
		}
		case 102:
		{//��������2����Ȧֵ
			MCH58_ReadStoreData(MCH58_AdrStoreBM2ZeroH,p,len);
			break;
		}
		case 103:
		{//��������2��㵥Ȧֵ
			MCH58_ReadStoreData(MCH58_AdrStoreBM2ZeroL,p,len);
			break;
		}
		case 104:
		{//��������2�Ӽ�������
			MCH58_ReadStoreData(MCH58_AdrStoreBM2CD,p,len);
			break;
		}
		case 105:
		{//��������2�豸ӳ��
			MCH58_ReadStoreData(MCH58_AdrStoreBM2YSSB,p,len);
			break;
		}
		case 106:
		{//��������2�豸��ַ
			MCH58_ReadStoreData(MCH58_AdrStoreBM2ADR,p,len);
			break;
		}
		case 107:
		{//��������2ͨѶ������
			MCH58_ReadStoreData(MCH58_AdrStoreBM2BD,p,len);
			break;
		}
		//�޸ı�����2���ò���
		case 151:
		{//�޸ı�����2���в���
			MCH58_StoreData(MCH58_AdrStoreBM2Base,p,len);
			break;
		}
		case 152:
		{//�޸ı�����2����Ȧֵ
			MCH58_StoreData(MCH58_AdrStoreBM2ZeroH,p,len);
			break;
		}
		case 153:
		{//�޸ı�����2��㵥Ȧֵ
			MCH58_StoreData(MCH58_AdrStoreBM2ZeroL,p,len);
			break;
		}
		case 154:
		{//�޸ı�����2�Ӽ�������
			MCH58_StoreData(MCH58_AdrStoreBM2CD,p,len);
			break;
		}
		case 155:
		{//�޸ı�����2�豸ӳ��
			MCH58_StoreData(MCH58_AdrStoreBM2YSSB,p,len);
			break;
		}
		case 156:
		{//�޸ı�����2�豸��ַ
			MCH58_StoreData(MCH58_AdrStoreBM2ADR,p,len);
			break;
		}
		case 157:
		{//�޸ı�����2ͨѶ������
			MCH58_StoreData(MCH58_AdrStoreBM2BD,p,len);
			break;
		}
		//---------------------------
		//��������3���ò���
		case 201:
		{//��������3���в���
			MCH58_ReadStoreData(MCH58_AdrStoreBM3Base,p,len);
			break;
		}
		case 202:
		{//��������3����Ȧֵ
			MCH58_ReadStoreData(MCH58_AdrStoreBM3ZeroH,p,len);
			break;
		}
		case 203:
		{//��������3��㵥Ȧֵ
			MCH58_ReadStoreData(MCH58_AdrStoreBM3ZeroL,p,len);
			break;
		}
		case 204:
		{//��������3�Ӽ�������
			MCH58_ReadStoreData(MCH58_AdrStoreBM3CD,p,len);
			break;
		}
		case 205:
		{//��������3�豸ӳ��
			MCH58_ReadStoreData(MCH58_AdrStoreBM3YSSB,p,len);
			break;
		}
		case 206:
		{//��������3�豸��ַ
			MCH58_ReadStoreData(MCH58_AdrStoreBM3ADR,p,len);
			break;
		}
		case 207:
		{//��������3ͨѶ������
			MCH58_ReadStoreData(MCH58_AdrStoreBM3BD,p,len);
			break;
		}
		//�޸ı�����3���ò���
		case 251:
		{//�޸ı�����3���в���
			MCH58_StoreData(MCH58_AdrStoreBM3Base,p,len);
			break;
		}
		case 252:
		{//�޸ı�����3����Ȧֵ
			MCH58_StoreData(MCH58_AdrStoreBM3ZeroH,p,len);
			break;
		}
		case 253:
		{//�޸ı�����3��㵥Ȧֵ
			MCH58_StoreData(MCH58_AdrStoreBM3ZeroL,p,len);
			break;
		}
		case 254:
		{//�޸ı�����3�Ӽ�������
			MCH58_StoreData(MCH58_AdrStoreBM3CD,p,len);
			break;
		}
		case 255:
		{//�޸ı�����3�豸ӳ��
			MCH58_StoreData(MCH58_AdrStoreBM3YSSB,p,len);
			break;
		}
		case 256:
		{//�޸ı�����3�豸��ַ
			MCH58_StoreData(MCH58_AdrStoreBM3ADR,p,len);
			break;
		}
		case 257:
		{//�޸ı�����3ͨѶ������
			MCH58_StoreData(MCH58_AdrStoreBM3BD,p,len);
			break;
		}
	}
}
//---
/*
	v��������void MCH58_UsartModeSet(MCH58_Structure *p)
	������ΪMCH58���ô���ͨѶģʽ
*/
void MCH58_UsartModeSet(MCH58_Structure *p)
{
	MCH58_UartModeSet(MCH58_UsartPort,p->BaudRate,'n','1');
}
//---
//-------------------------���ݶ�ȡ���洢----------------------------
//---
/*
	��������void MCH58_ReadStoreData(u8 address,u8 *p,u8 len)
	������	��ȡ����
*/
void MCH58_ReadStoreData(u8 address,u8 *p,u8 len)
{
	W25Xxx_ReadData(p,MCH58_AdrStoreBase+address,len);
}
//---
/*
	��������void MCH58_StoreData(u8 address,u8 *p,u8 len)
	�������ú���Ϊ�洢MCH58���ò�������
	˵������ȡ����
				��������
				�޸�����
				��������
*/
void MCH58_StoreData(u8 address,u8 *p,u8 len)
{
	u8 buffer[MCH58_AdrStoreEnd],cnt;
	W25Xxx_ReadData(buffer,MCH58_AdrStoreBase,MCH58_AdrStoreEnd);//���������ò���
	W25Xxx_EraseSector(MCH58_AdrStoreBase);//��������
	for(cnt=0;cnt<len;cnt++)
	{
		buffer[address++] = *p;
		p++;
	}
	W25Xxx_PageWrite(buffer,MCH58_AdrStoreBase,MCH58_AdrStoreEnd);//��������
}
//---
/*
	��������void MCH58_ChangeToMap(MCH58_Structure *p)
	�������ú���Ϊ��MCH58�ı���ֵת��Ϊ��Ӧ��Ŀ������
*/
void MCH58_ChangeToMap(MCH58_Structure *p)
{
//	s32 bf= p->CodedValue - p->ZeroValue;
//	if(p->Map == 1)
//		Tower_CalSlewing(bf);
//	else if(p->Map == 2)
//	{
//		Tower_CalTrolleying(bf);
//	}
//	else if(p->Map == 3)
//	{
//		Tower_CalHoisting(bf);
//	}
}

//---
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void MCH58_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪMCH58�ļ���ASCII�����ļ�
*/
void MCH58_DebugASCII(u8 *p,u8 len)
{
	TwoBytes two;
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		//������1
		case 2:
		{//��������1����Ȧֵ
			MCH58_Operation(2,two.U8Data,2);
			printf("������1����ȦֵΪ��%u\r\n",two.U16Data);
			break;
		}
		case 3:
		{//��������1��㵥Ȧֵ
			MCH58_Operation(3,two.U8Data,2);
			printf("������1��㵥ȦֵΪ��%u\r\n",two.U16Data);
			break;
		}
		case 4:
		{//��������1�Ӽ�������
			MCH58_Operation(4,two.U8Data,1);
			if(two.U8Data[0] == 1)
				printf("˳ʱ��Ӽ���\r\n");
			else if(two.U8Data[0] == 2)
				printf("��ʱ��Ӽ���\r\n");
			else
				printf("����\r\n");
			break;
		}
		case 5:
		{//��������1�豸ӳ��
			MCH58_Operation(5,two.U8Data,1);
			if(two.U8Data[0] == 1)
				printf("��ת���ݲɼ�\r\n");
			else if(two.U8Data[0] == 2)
				printf("������ݲɼ�\r\n");
			else if(two.U8Data[0] == 3)
				printf("�������ݲɼ�\r\n");
			else
				printf("����\r\n");
			break;
		}
		case 6:
		{//��������1ͨѶ��ַ
			MCH58_Operation(6,two.U8Data,1);
			printf("������1ͨѶ��ַΪ��%u\r\n",two.U8Data[0]);
			break;
		}
		case 7:
		{//��������1ͨѶ������
			MCH58_Operation(7,two.U8Data,2);
			printf("������1ͨѶ������Ϊ��%u\r\n",two.U16Data);
			break;
		}
		case 52:
		{//�޸ı�����1����Ȧֵ
			two.U16Data = Calculate_CharToU16(p+4,4);//��������
			if(two.U16Data <4097)
			{
				MCH58_Operation(52,two.U8Data,2);
				printf("�޸ı�����1����ȦֵΪ��%u\r\n",two.U16Data);
			}
			else
				printf("��������������뷶ΧΪ0~4096\r\n");
			break;
		}
		case 53:
		{//��������1��㵥Ȧֵ
			two.U16Data = Calculate_CharToU16(p+4,4);//��������
			if(two.U16Data <4097)
			{
				MCH58_Operation(53,two.U8Data,2);
				printf("�޸ı�����1��㵥ȦֵΪ��%u\r\n",two.U16Data);
			}
			else
				printf("��������������뷶ΧΪ0~4096\r\n");
			break;
		}
		case 54:
		{//��������1�Ӽ�������
			if(p[4] == '1')
			{
				two.U8Data[0] = 1;
				printf("����Ϊ˳ʱ��Ӽ���\r\n");
			}
			else
			{
				two.U8Data[0] = 2;
				printf("����Ϊ��ʱ��Ӽ���\r\n");
			}
			MCH58_Operation(54,two.U8Data,1);
			break;
		}
		case 55:
		{//��������1�豸ӳ��
			if(p[4] == '2')
			{
				two.U8Data[0] = 2;
				printf("����Ϊ������ݲɼ�\r\n");
			}
			else if(p[4] == '3')
			{
				two.U8Data[0] = 3;
				printf("����Ϊ�������ݲɼ�\r\n");
			}
			else
			{
				two.U8Data[0] = 1;
				printf("����Ϊ��ת���ݲɼ�\r\n");
			}
			MCH58_Operation(55,two.U8Data,1);
			break;
		}
		case 56:
		{//��������1ͨѶ��ַ
			u8 bf = Calculate_CharToU16(p+4,3);//��������
			if(bf>0 && bf<251)
			{
				MCH58_Operation(56,&bf,1);
				printf("������1ͨѶ��ַΪ��%u\r\n",bf);
			}
			else
				printf("�����������������1~250\r\n");
			break;
		}
		case 57:
		{//��������1ͨѶ������
			two.U16Data = Calculate_CharToU16(p+4,5);//��������
			if((two.U16Data==4800) || (two.U16Data==9600) || (two.U16Data==19200) ||(two.U16Data==57600))
			{
				MCH58_Operation(57,two.U8Data,2);
				printf("������1ͨѶ������Ϊ��%u\r\n",two.U16Data);
			}
			else
				printf("�����������������4800/9600/19200/57600\r\n");
			break;
		}
		//������2
		case 102:
		{//��������2����Ȧֵ
			MCH58_Operation(102,two.U8Data,2);
			printf("������2����ȦֵΪ��%u\r\n",two.U16Data);
			break;
		}
		case 103:
		{//��������2��㵥Ȧֵ
			MCH58_Operation(103,two.U8Data,2);
			printf("������2��㵥ȦֵΪ��%u\r\n",two.U16Data);
			break;
		}
		case 104:
		{//��������2�Ӽ�������
			MCH58_Operation(104,two.U8Data,1);
			if(two.U8Data[0] == 1)
				printf("˳ʱ��Ӽ���\r\n");
			else if(two.U8Data[0] == 2)
				printf("��ʱ��Ӽ���\r\n");
			else
				printf("����\r\n");
			break;
		}
		case 105:
		{//��������2�豸ӳ��
			MCH58_Operation(105,two.U8Data,1);
			if(two.U8Data[0] == 1)
				printf("��ת���ݲɼ�\r\n");
			else if(two.U8Data[0] == 2)
				printf("������ݲɼ�\r\n");
			else if(two.U8Data[0] == 3)
				printf("�������ݲɼ�\r\n");
			else
				printf("����\r\n");
			break;
		}
		case 106:
		{//��������2ͨѶ��ַ
			MCH58_Operation(106,two.U8Data,1);
			printf("������2ͨѶ��ַΪ��%u\r\n",two.U8Data[0]);
			break;
		}
		case 107:
		{//��������2ͨѶ������
			MCH58_Operation(107,two.U8Data,2);
			printf("������2ͨѶ������Ϊ��%u\r\n",two.U16Data);
			break;
		}
		case 152:
		{//�޸ı�����2����Ȧֵ
			two.U16Data = Calculate_CharToU16(p+4,4);//��������
			if(two.U16Data <4097)
			{
				MCH58_Operation(152,two.U8Data,2);
				printf("�޸ı�����2����ȦֵΪ��%u\r\n",two.U16Data);
			}
			else
				printf("��������������뷶ΧΪ0~4096\r\n");
			break;
		}
		case 153:
		{//��������2��㵥Ȧֵ
			two.U16Data = Calculate_CharToU16(p+4,4);//��������
			if(two.U16Data <4097)
			{
				MCH58_Operation(153,two.U8Data,2);
				printf("�޸ı�����2��㵥ȦֵΪ��%u\r\n",two.U16Data);
			}
			else
				printf("��������������뷶ΧΪ0~4096\r\n");
			break;
		}
		case 154:
		{//��������2�Ӽ�������
			if(p[4] == '1')
			{
				two.U8Data[0] = 1;
				printf("����Ϊ˳ʱ��Ӽ���\r\n");
			}
			else
			{
				two.U8Data[0] = 2;
				printf("����Ϊ��ʱ��Ӽ���\r\n");
			}
			MCH58_Operation(154,two.U8Data,1);
			break;
		}
		case 155:
		{//��������2�豸ӳ��
			if(p[4] == '1')
			{
				two.U8Data[0] = 1;
				printf("����Ϊ��ת���ݲɼ�\r\n");
			}
			else if(p[4] == '3')
			{
				two.U8Data[0] = 3;
				printf("����Ϊ�������ݲɼ�\r\n");
			}
			else
			{
				two.U8Data[0] = 2;
				printf("����Ϊ������ݲɼ�\r\n");
			}
			MCH58_Operation(155,two.U8Data,1);
			break;
		}
		case 156:
		{//��������2ͨѶ��ַ
			u8 bf = Calculate_CharToU16(p+4,3);//��������
			if(bf>0 && bf<251)
			{
				MCH58_Operation(156,&bf,1);
				printf("������2ͨѶ��ַΪ��%u\r\n",bf);
			}
			else
				printf("�����������������1~250\r\n");
			break;
		}
		case 157:
		{//��������2ͨѶ������
			two.U16Data = Calculate_CharToU16(p+4,5);//��������
			if((two.U16Data==4800) || (two.U16Data==9600) || (two.U16Data==19200) ||(two.U16Data==57600))
			{
				MCH58_Operation(157,two.U8Data,2);
				printf("������2ͨѶ������Ϊ��%u\r\n",two.U16Data);
			}
			else
				printf("�����������������4800/9600/19200/57600\r\n");
			break;
		}
		//������3
		case 202:
		{//��������3����Ȧֵ
			MCH58_Operation(202,two.U8Data,2);
			printf("������3����ȦֵΪ��%u\r\n",two.U16Data);
			break;
		}
		case 203:
		{//��������3��㵥Ȧֵ
			MCH58_Operation(203,two.U8Data,2);
			printf("������3��㵥ȦֵΪ��%u\r\n",two.U16Data);
			break;
		}
		case 204:
		{//��������3�Ӽ�������
			MCH58_Operation(204,two.U8Data,1);
			if(two.U8Data[0] == 1)
				printf("˳ʱ��Ӽ���\r\n");
			else if(two.U8Data[0] == 2)
				printf("��ʱ��Ӽ���\r\n");
			else
				printf("����\r\n");
			break;
		}
		case 205:
		{//��������3�豸ӳ��
			MCH58_Operation(205,two.U8Data,1);
			if(two.U8Data[0] == 1)
				printf("��ת���ݲɼ�\r\n");
			else if(two.U8Data[0] == 2)
				printf("������ݲɼ�\r\n");
			else if(two.U8Data[0] == 3)
				printf("�������ݲɼ�\r\n");
			else
				printf("����\r\n");
			break;
		}
		case 206:
		{//��������3ͨѶ��ַ
			MCH58_Operation(206,two.U8Data,1);
			printf("������3ͨѶ��ַΪ��%u\r\n",two.U8Data[0]);
			break;
		}
		case 207:
		{//��������3ͨѶ������
			MCH58_Operation(207,two.U8Data,2);
			printf("������3ͨѶ������Ϊ��%u\r\n",two.U16Data);
			break;
		}
		case 252:
		{//�޸ı�����3����Ȧֵ
			two.U16Data = Calculate_CharToU16(p+4,4);//��������
			if(two.U16Data <4097)
			{
				MCH58_Operation(252,two.U8Data,2);
				printf("�޸ı�����3����ȦֵΪ��%u\r\n",two.U16Data);
			}
			else
				printf("��������������뷶ΧΪ0~4096\r\n");
			break;
		}
		case 253:
		{//��������3��㵥Ȧֵ
			two.U16Data = Calculate_CharToU16(p+4,4);//��������
			if(two.U16Data <4097)
			{
				MCH58_Operation(253,two.U8Data,2);
				printf("�޸ı�����3��㵥ȦֵΪ��%u\r\n",two.U16Data);
			}
			else
				printf("��������������뷶ΧΪ0~4096\r\n");
			break;
		}
		case 254:
		{//��������3�Ӽ�������
			if(p[4] == '1')
			{
				two.U8Data[0] = 1;
				printf("����Ϊ˳ʱ��Ӽ���\r\n");
			}
			else
			{
				two.U8Data[0] = 2;
				printf("����Ϊ��ʱ��Ӽ���\r\n");
			}
			MCH58_Operation(254,two.U8Data,1);
			break;
		}
		case 255:
		{//��������3�豸ӳ��
			if(p[4] == '2')
			{
				two.U8Data[0] = 2;
				printf("����Ϊ������ݲɼ�\r\n");
			}
			else if(p[4] == '1')
			{
				two.U8Data[0] = 1;
				printf("����Ϊ��ת���ݲɼ�\r\n");
			}
			else
			{
				two.U8Data[0] = 3;
				printf("����Ϊ�������ݲɼ�\r\n");
			}
			MCH58_Operation(255,two.U8Data,1);
			break;
		}
		case 256:
		{//��������3ͨѶ��ַ
			u8 bf = Calculate_CharToU16(p+4,3);//��������
			if(bf>0 && bf<251)
			{
				MCH58_Operation(256,&bf,1);
				printf("������3ͨѶ��ַΪ��%u\r\n",bf);
			}
			else
				printf("�����������������1~250\r\n");
			break;
		}
		case 257:
		{//��������3ͨѶ������
			two.U16Data = Calculate_CharToU16(p+4,5);//��������
			if((two.U16Data==4800) || (two.U16Data==9600) || (two.U16Data==19200) ||(two.U16Data==57600))
			{
				MCH58_Operation(257,two.U8Data,2);
				printf("������3ͨѶ������Ϊ��%u\r\n",two.U16Data);
			}
			else
				printf("�����������������4800/9600/19200/57600\r\n");
			break;
		}
		case 801://:000|87|801|1
		{//��/�ص�Դ
			if(p[4] == '0')
			{
				MCH58_PWR_OFF();
				printf("�ر�VCC_24V���\r\n");
			}
			else
			{
				MCH58_PWR_ON();
				printf("��VCC_24V���\r\n");
			}
			break;
		}
		case 802:
		{//��/��CON1
			if(p[4] == '0')
			{
				MCH58_Con1_OFF();
				printf("24V_OUT1=0\r\n");
			}
			else
			{
				MCH58_Con1_ON();
				printf("24V_OUT1=1\r\n");
			}
			break;
		}
		case 803:
		{//��/��CON2
			if(p[4] == '0')
			{
				MCH58_Con2_OFF();
				printf("24V_OUT2=0\r\n");
			}
			else
			{
				MCH58_Con2_ON();
				printf("24V_OUT2=1\r\n");
			}
			break;
		}
		case 804:
		{//��/��CON3
			if(p[4] == '0')
			{
				MCH58_Con3_OFF();
				printf("24V_OUT3=0\r\n");
			}
			else
			{
				MCH58_Con3_ON();
				printf("24V_OUT3=1\r\n");
			}
			break;
		}
	}
}
//---
/*
	�������� 	void MCH58_DebugRTU((u8 *p)
	������		�ú���ΪMCH58�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*/
void MCH58_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//�鿴���ĸ�������
	{
		case 0x01:
		{
			break;
		}
		case 0x02:
		{
			break;
		}
	}
}
//---
/*
	��������void MCH58_Receive(u8 *p,u8 len)
	�������������Խ��յ�Thbss01��һ֡����
	˵��������ú���֮ǰ����Ҫ�Ƚ���CRCУ��
*/
void MCH58_Receive(u8 *p,u8 len)
{
	switch(MCH58Data.CollectMode)
	{
		case MCH58_Con1RunModeBasic+1:
		{//�յ�������1��������
			MCH58Data.Mch58[0].CodedValue = (u16)(*(p+3)<<8 | *(p+4))*4096 + (u16)(*(p+5)<<8 | *(p+6));
			MCH58_ChangeToMap(&MCH58Data.Mch58[0]);
			break;
		}
		case MCH58_Con2RunModeBasic+1:
		{//�յ�������2��������
			MCH58Data.Mch58[1].CodedValue = (u16)(*(p+3)<<8 | *(p+4))*4096 + (u16)(*(p+5)<<8 | *(p+6));
			MCH58_ChangeToMap(&MCH58Data.Mch58[1]);
			break;
		}
		case MCH58_Con3RunModeBasic+1:
		{//�յ�������3��������
			MCH58Data.Mch58[2].CodedValue = (u16)(*(p+3)<<8 | *(p+4))*4096 + (u16)(*(p+5)<<8 | *(p+6));
			MCH58_ChangeToMap(&MCH58Data.Mch58[2]);
			break;
		}
		//��������
		case MCH58_Con1RunModeBasic+9:
		case MCH58_Con1RunModeBasic+10:
		{//
			printf("�ɹ����ñ�����1��������\r\n");
			break;
		}
	}
	MCH58Data.CollectMode = 0;
}
//---



































