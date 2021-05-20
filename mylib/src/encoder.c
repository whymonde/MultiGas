/*
	���ļ�Ϊ����Ŀ�ı�������غ���
	���庯������

*/

#include "encoder.h"

Encoder_DataStructure EncoderData;//Encoder.c�ṹ�����
/*
	��������void Encoder_Init(void)
	�������ú���Ϊencoder.c�ļ���ʼ������
*/
void Encoder_Init(void)
{
//	Encoder_GpioInit();
	Encoder_EncoderDataInit();
}
//---
/*
	��������void Encoder_Timer(void)
	������	Encoder.c��ʱ����1ms����ϵͳ��ʱ������
*/
void Encoder_Timer(void)
{
	
}
//---
/*
	��������void Encoder_GpioInit(void)
	�������ú���ΪEncoder.c���GPIO�ڳ�ʼ��
	˵�����ú�����ʼ����ת����������߱��������ùܽŵĳ�ʼ��
	�ܽţ�PA9		��ת���ƣ�0=�������ƣ�1=�رտ���
				PA10	������ƣ�0=�������ƣ�1=�رտ���
				PA11	���߿��ƣ�0=�������ƣ�1=�رտ���
*/
void Encoder_GpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(ENCODER_Slewing_RCC, ENABLE); //��ʱ��
	RCC_APB2PeriphClockCmd(ENCODER_Trolleying_RCC, ENABLE); 
	RCC_APB2PeriphClockCmd(ENCODER_Hoisting_RCC, ENABLE); 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_InitStructure.GPIO_Pin = ENCODER_Slewing_PIN;
	GPIO_Init(ENCODER_Slewing_PORT, &GPIO_InitStructure);//����DS18B20��Դ�˿�
	GPIO_InitStructure.GPIO_Pin = ENCODER_Trolleying_PIN;
	GPIO_Init(ENCODER_Trolleying_PORT, &GPIO_InitStructure);//����DS18B20���Ͷ˿�
	GPIO_InitStructure.GPIO_Pin = ENCODER_Hoisting_PIN;
	GPIO_Init(ENCODER_Hoisting_PORT, &GPIO_InitStructure);//����DS18B20���Ͷ˿�
	ENCODER_SlewingRESET();
	ENCODER_TrolleyingRESET();
	ENCODER_HoistingRESET();
}
//---
/*
	��������void Encoder_EncoderDataInit(void)
	������	�ú���Ϊ��ת�ɼ���������ʼ������
*/
void Encoder_EncoderDataInit(void)
{
	EncoderData.Slewing.Address = ENCODER_SlewingAddress;
	EncoderData.Trolleying.Address = ENCODER_TrolleyingAddress;
	EncoderData.Hoisting.Address = ENCODER_HoistingAddress;
	EncoderData.CollectMode = 0;
}
//---
/*
	��������Encoder_DataStructure* Encoder_GetData(void)
	������	�ú������ڻ�ȡEncoderData�ṹ�������ַ
*/
Encoder_DataStructure* Encoder_GetData(void)
{
	return &EncoderData;
}
/*
	��������Encoder_EncoderStructure Encoder_GetSlewingData(void)
	������	�ú���Ϊ��ȡEncoderSlewing������ַ
*/
Encoder_EncoderStructure* Encoder_GetSlewingData(void)
{
	return &EncoderData.Slewing;
}
//---
/*
	��������Encoder_EncoderStructure Encoder_GetTrolleyingData(void)
	������	�ú���Ϊ��ȡEncoderTrolleying������ַ
*/
Encoder_EncoderStructure* Encoder_GetTrolleyingData(void)
{
	return &EncoderData.Trolleying;
}
//---
/*
	��������Encoder_EncoderStructure Encoder_GetHoistingData(void)
	������	�ú���Ϊ��ȡEncoderHoisting������ַ
*/
Encoder_EncoderStructure* Encoder_GetHoistinggData(void)
{
	return &EncoderData.Hoisting;
}
//---
/*
	��������void Encoder_CollectValue(Encoder_EncoderStructure *p)
	������	�ú���Ϊ�������Ͳɼ�������ֵ����
	���룺	Encoder_EncoderStructure *p��Ҫ�ɼ��ı������ṹ���ַ
*/
void Encoder_CollectValue(Encoder_EncoderStructure *p)
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
	Encoder_SetComNoneVerify();//����Ϊ��У��ģʽ
	for(crc=0;crc<cnt;crc++)
	{
		Encoder_SendBufferData(Encoder_UsartPort,Encoder_UsartBuffer,buffer[crc]);//��������
	}
	if(p == &EncoderData.Slewing)
	{
		EncoderData.CollectMode = ENCODER_SlewingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdReadData);
	}
	else if(p == &EncoderData.Trolleying)
	{
		EncoderData.CollectMode = ENCODER_TrolleyingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdReadData);
	}
	else if(p == &EncoderData.Hoisting)
	{
		EncoderData.CollectMode = ENCODER_HoistingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdReadData);
	}
}
//---
/*
	��������void Encoder_SetAddress(Encoder_EncoderStructure *p,u8 adr)
	������	�ú���Ϊ���ñ������ĵ�ַ
	���룺	Encoder_EncoderStructure *p��Ҫ���õı������ṹ���ַ
					u8 adr���õĵ�ַ�����з�ΧΪ0x01~0x0F
*/
void Encoder_SetAddress(Encoder_EncoderStructure *p,u8 adr)
{
	if(p == &EncoderData.Slewing)
	{//��ת
		ENCODER_SlewingSET();
		EncoderData.CollectMode = ENCODER_SlewingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdSetAddress);
	}
	else if(p == &EncoderData.Trolleying)
	{//���
		ENCODER_TrolleyingSET();
		EncoderData.CollectMode = ENCODER_TrolleyingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdSetAddress);
	}
	else if(p == &EncoderData.Hoisting)
	{//����
		ENCODER_HoistingSET();
		EncoderData.CollectMode = ENCODER_HoistingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdSetAddress);
	}
	if((adr > 0x0F) || (adr == 0))
	{
		adr = 0x01;
	}
	Encoder_SetComNoneVerify();//����Ϊ��У��ģʽ
	Encoder_SendBufferData(Encoder_UsartPort,Encoder_UsartBuffer,0xA0+adr);//��������
}
//---
/*
	��������void Encoder_SetCycle(Encoder_EncoderStructure *p,u8 cycle)
	������	�ú���Ϊ���ñ������ĵ�ַ
	���룺	Encoder_EncoderStructure *p��Ҫ���õı������ṹ���ַ
					u8 cycle���õ�Ȧ�������з�ΧΪ0x01~0x0F
*/
void Encoder_SetCycle(Encoder_EncoderStructure *p,u8 cycle)
{
	if(p == &EncoderData.Slewing)
	{//��ת
		ENCODER_SlewingSET();
		EncoderData.CollectMode = ENCODER_SlewingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdSetCircle);
	}
	else if(p == &EncoderData.Trolleying)
	{//���
		ENCODER_TrolleyingSET();
		EncoderData.CollectMode = ENCODER_TrolleyingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdSetCircle);
	}
	else if(p == &EncoderData.Hoisting)
	{//����
		ENCODER_HoistingSET();
		EncoderData.CollectMode = ENCODER_HoistingRunModeBasic + Encoder_SetRunMode(ENCODER_CmdSetCircle);
	}
	if((cycle > 0x0F) || (cycle == 0))
	{
		cycle = 0x01;
	}
	Encoder_SetComNoneVerify();//����Ϊ��У��ģʽ
	Encoder_SendBufferData(Encoder_UsartPort,Encoder_UsartBuffer,0xA0+cycle);//��������
}
//---
/*
	��������void Encoder_SetCommand(Encoder_EncoderStructure *p,u8 cmd)
	�������ú���Ϊ���ñ���������
*/
void Encoder_SetCommand(Encoder_EncoderStructure *p,u8 cmd)
{
	if(p == &EncoderData.Slewing)
	{//��ת
		ENCODER_SlewingSET();
		EncoderData.CollectMode = ENCODER_SlewingRunModeBasic + Encoder_SetRunMode(cmd);
	}
	else if(p == &EncoderData.Trolleying)
	{//���
		ENCODER_TrolleyingSET();
		EncoderData.CollectMode = ENCODER_TrolleyingRunModeBasic + Encoder_SetRunMode(cmd);
	}
	else if(p == &EncoderData.Hoisting)
	{//����
		ENCODER_HoistingSET();
		EncoderData.CollectMode = ENCODER_HoistingRunModeBasic + Encoder_SetRunMode(cmd);
	}
	Encoder_SetComNoneVerify();//����Ϊ��У��ģʽ
	Encoder_SendBufferData(Encoder_UsartPort,Encoder_UsartBuffer,cmd);//��������
}
//---
/*
	��������u8 Encoder_SetRunMode(u8 cmd)
	������	���ò�ͬ�����ͬ��CollectModeֵ
*/
u8 Encoder_SetRunMode(u8 cmd)
{
	u8 buffer=0;
	switch(cmd)
	{
		case ENCODER_CmdReadData:
		{//���ر�������ֵ
			buffer = 1;
			break;
		}
		case ENCODER_CmdSetAddress:
		{//���õ�ַ
			buffer = 2;
			break;
		}
		case ENCODER_CmdBaudRate4800:
		case ENCODER_CmdBaudRate9600:
		case ENCODER_CmdBaudRate19200:
		case ENCODER_CmdBaudRate57600:
		case ENCODER_CmdBaudRate115200:
		{//���ò�����
			buffer = 3;
			break;
		}
		case ENCODER_CmdReturnSet:
		{//��ȡ�趨ֵ
			buffer = 4;
			break;
		}
		case ENCODER_CmdReturnValue:
		{//��ȡ��ǰ��������ֵ
			buffer = 5;
			break;
		}
		case ENCODER_CmdRetrunBit:
		{//��ȡ��Ȧ���Ȧλ����
			buffer = 6;
			break;
		}
		case ENCODER_CmdSet2047Circle:
		{//���õ�ǰȦ��Ϊ2047Ȧ
			buffer = 7;
			break;
		}
		case ENCODER_CmdSetCircle:
		{//�趨��ǰȦ��0~15Ȧ
			buffer = 8;
			break;
		}
		case ENCODER_CmdClockwiseAdd:
		{//����Ϊ˳ʱ��+����
			buffer = 9;
			break;
		}
		case ENCODER_CmdAnticlockwiseAdd:
		{//����Ϊ��ʱ��+����
			buffer = 10;
			break;
		}
	}
	return buffer;
}
//---
//-------------------------------------���Ժ���----------------------------------------
//---
/*
	�������� void Encoder_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪEncoder.c�ļ���ASCII�����ļ�
	:000|57|XXX������������
*/
void Encoder_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		//------------------------------------------------------------------------
		case 1://
		{//:000|57|001
			Encoder_CollectValue(&EncoderData.Slewing);
			printf("���Ͳɼ���ת����������!\r\n");
			break;
		}
		case 2://:000|57|002|10
		{//���ñ�������ַ����
			u8 adr = Calculate_CharToU16(p+4,2);//��������
			if(adr>0x0F || adr==0)
			{
				printf("�������%u�������������Ϊ01~15\r\n",adr);
			}
			else 
			{
				Encoder_SetAddress(&EncoderData.Slewing,adr);
				printf("���û�ת��������ַΪ%u\r\n",adr);
			}
			break;
		}
		case 3://:000|57|003|1
		{//���ñ�����������
			u8 adr = Calculate_CharToU16(p+4,1);//��������
			switch(adr)
			{
				case 1:
				{//4800
					Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdBaudRate4800);
					printf("���û�ת������������Ϊ4800\r\n");
					break;
				}
				case 2:
				{//9600
					Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdBaudRate9600);
					printf("���û�ת������������Ϊ9600\r\n");
					break;
				}
				case 3:
				{//19200
					Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdBaudRate19200);
					printf("���û�ת������������Ϊ19200\r\n");
					break;
				}
				case 4:
				{//57600
					Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdBaudRate57600);
					printf("���û�ת������������Ϊ57600\r\n");
					break;
				}
				case 5:
				{//115200
					Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdBaudRate115200);
					printf("���û�ת������������Ϊ115200\r\n");
					break;
				}
				default:
				{
					printf("����������󣬲�����Χ1~5\r\n");
					break;
				}
			}
			break;
		}
		case 4:
		{//��ȡ�趨ֵ�����ص�ַ�������ʡ��������
			Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdReturnSet);
			printf("��ȡSlewing���趨ֵ�����ص�ַ�������ʡ��������\r\n");
			break;
		}
		case 5:
		{//��ȡ��������ǰ��Ȧ�뵥Ȧ����
			Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdReturnValue);
			printf("��ȡSlewing��������ǰ��Ȧ�뵥Ȧ����\r\n");
			break;
		}
		case 6:
		{//��ȡ��������ǰ��Ȧ�뵥Ȧ����
			Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdRetrunBit);
			printf("��ȡSlewing��������Ȧ�뵥Ȧλ��\r\n");
			break;
		}
		case 7:
		{//��ȡ��������ǰ��Ȧ�뵥Ȧ����
			Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdSet2047Circle);
			printf("�趨Slewing��������ǰȦ��Ϊ2047Ȧ\r\n");
			break;
		}
		case 8://:000|57|208|11
		{//���ñ�����Ȧ��,�������Ϊ00~15
			u8 cycle = Calculate_CharToU16(p+4,2);//��������
			if(cycle>0x0F)
			{
				printf("�������%u�������������Ϊ00~15\r\n",cycle);
			}
			else 
			{
				Encoder_SetCycle(&EncoderData.Slewing,cycle);
				printf("����Slewing��������ǰȦ��Ϊ%u\r\n",cycle);
			}
			break;
		}
		case 9:
		{//�趨Slewing��������˳ʱ��Ӽ�������ʱ����������������Ϊ0��Ϊ˳ʱ�룬��0Ϊ��ʱ��
			if(*(p+4) == '0')
			{
				Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdClockwiseAdd);
				printf("�趨Slewing������˳ʱ��Ӽ���\r\n");
			}
			else 
			{
				Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdAnticlockwiseAdd);
				printf("�趨Slewing��������ʱ��Ӽ���\r\n");
			}
			break;
		}
		//------------------------------------------------------------------------
		case 101://
		{//:000|57|101
			Encoder_CollectValue(&EncoderData.Trolleying);
			printf("���Ͳɼ��������������!\r\n");
			break;
		}
		case 102://:000|57|202|11
		{//���ñ�������ַ����
			u8 adr = Calculate_CharToU16(p+4,2);//��������
			if(adr>0x0F || adr==0)
			{
				printf("�������%u�������������Ϊ00~15\r\n",adr);
			}
			else 
			{
				Encoder_SetAddress(&EncoderData.Trolleying,adr);
				printf("���ñ����������ַΪ%u\r\n",adr);
			}
			break;
		}
		case 103://:000|57|003|1
		{//���ñ�����������
			u8 adr = Calculate_CharToU16(p+4,1);//��������
			switch(adr)
			{
				case 1:
				{//4800
					Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdBaudRate4800);
					printf("���ñ��������������Ϊ4800\r\n");
					break;
				}
				case 2:
				{//9600
					Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdBaudRate9600);
					printf("���ñ��������������Ϊ9600\r\n");
					break;
				}
				case 3:
				{//19200
					Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdBaudRate19200);
					printf("���ñ��������������Ϊ19200\r\n");
					break;
				}
				case 4:
				{//57600
					Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdBaudRate57600);
					printf("���ñ��������������Ϊ57600\r\n");
					break;
				}
				case 5:
				{//115200
					Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdBaudRate115200);
					printf("���ñ��������������Ϊ115200\r\n");
					break;
				}
				default:
				{
					printf("����������󣬲�����Χ1~5\r\n");
					break;
				}
			}
			break;
		}
		case 104:
		{//��ȡ�趨ֵ�����ص�ַ�������ʡ��������
			Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdReturnSet);
			printf("��ȡTrolleying���趨ֵ�����ص�ַ�������ʡ��������\r\n");
			break;
		}
		case 105:
		{//��ȡ��������ǰ��Ȧ�뵥Ȧ����
			Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdReturnValue);
			printf("��ȡTrolleying��������ǰ��Ȧ�뵥Ȧ����\r\n");
			break;
		}
		case 106:
		{//��ȡ��������ǰ��Ȧ�뵥Ȧ����
			Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdRetrunBit);
			printf("��ȡTrolleying��������Ȧ�뵥Ȧλ��\r\n");
			break;
		}	
		case 107:
		{//��ȡ��������ǰ��Ȧ�뵥Ȧ����
			Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdSet2047Circle);
			printf("�趨Trolleying��������ǰȦ��Ϊ2047Ȧ\r\n");
			break;
		}
		case 108://:000|57|208|11
		{//���ñ�����Ȧ��,�������Ϊ00~15
			u8 cycle = Calculate_CharToU16(p+4,2);//��������
			if(cycle>0x0F)
			{
				printf("�������%u�������������Ϊ00~15\r\n",cycle);
			}
			else 
			{
				Encoder_SetCycle(&EncoderData.Trolleying,cycle);
				printf("����Trolleying��������ǰȦ��Ϊ%u\r\n",cycle);
			}
			break;
		}
		case 109:
		{//�趨Trolleying��������˳ʱ��Ӽ�������ʱ����������������Ϊ0��Ϊ˳ʱ�룬��0Ϊ��ʱ��
			if(*(p+4) == '0')
			{
				Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdClockwiseAdd);
				printf("�趨Trolleying������˳ʱ��Ӽ���\r\n");
			}
			else 
			{
				Encoder_SetCommand(&EncoderData.Trolleying,ENCODER_CmdAnticlockwiseAdd);
				printf("�趨Trolleying��������ʱ��Ӽ���\r\n");
			}
			break;
		}
		//------------------------------------------------------------------------
		case 201://
		{//:000|57|201
			Encoder_CollectValue(&EncoderData.Hoisting);
			printf("���Ͳɼ����߱���������!\r\n");
			break;
		}
		case 202://:000|57|202|11
		{//���ñ�������ַ����
			u8 adr = Calculate_CharToU16(p+4,2);//��������
			if(adr>0x0F || adr==0)
			{
				printf("�������%u�������������Ϊ00~15\r\n",adr);
			}
			else 
			{
				Encoder_SetAddress(&EncoderData.Hoisting,adr);
				printf("���õ��߱�������ַΪ%u\r\n",adr);
			}
			break;
		}
		case 203://:000|57|203|1
		{//���ñ�����������
			u8 adr = Calculate_CharToU16(p+4,1);//��������
			switch(adr)
			{
				case 1:
				{//4800
					Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdBaudRate4800);
					printf("���õ��߱�����������Ϊ4800\r\n");
					break;
				}
				case 2:
				{//9600
					Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdBaudRate9600);
					printf("���õ��߱�����������Ϊ9600\r\n");
					break;
				}
				case 3:
				{//19200
					Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdBaudRate19200);
					printf("���õ��߱�����������Ϊ19200\r\n");
					break;
				}
				case 4:
				{//57600
					Encoder_SetCommand(&EncoderData.Slewing,ENCODER_CmdBaudRate57600);
					printf("���õ��߱�����������Ϊ57600\r\n");
					break;
				}
				case 5:
				{//115200
					Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdBaudRate115200);
					printf("���õ��߱�����������Ϊ115200\r\n");
					break;
				}
				default:
				{
					printf("����������󣬲�����Χ1~5\r\n");
					break;
				}
			}
			break;
		}
		case 204:
		{//��ȡ�趨ֵ�����ص�ַ�������ʡ��������
			Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdReturnSet);
			printf("��ȡHoisting���趨ֵ�����ص�ַ�������ʡ��������\r\n");
			break;
		}
		case 205:
		{//��ȡ��������ǰ��Ȧ�뵥Ȧ����
			Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdReturnValue);
			printf("��ȡHoisting��������ǰ��Ȧ�뵥Ȧ����\r\n");
			break;
		}
		case 206:
		{//��ȡ��������ǰ��Ȧ�뵥Ȧ����
			Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdRetrunBit);
			printf("��ȡHoisting��������Ȧ�뵥Ȧλ��\r\n");
			break;
		}
		case 207:
		{//��ȡ��������ǰ��Ȧ�뵥Ȧ����
			Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdSet2047Circle);
			printf("�趨Hoisting��������ǰȦ��Ϊ2047Ȧ\r\n");
			break;
		}
		case 208://:000|57|208|11
		{//���ñ�����Ȧ��,�������Ϊ00~15
			u8 cycle = Calculate_CharToU16(p+4,2);//��������
			if(cycle>0x0F)
			{
				printf("�������%u�������������Ϊ00~15\r\n",cycle);
			}
			else 
			{
				Encoder_SetCycle(&EncoderData.Hoisting,cycle);
				printf("����Hoisting��������ǰȦ��Ϊ%u\r\n",cycle);
			}
			break;
		}
		case 209:
		{//�趨Hoisting��������˳ʱ��Ӽ�������ʱ����������������Ϊ0��Ϊ˳ʱ�룬��0Ϊ��ʱ��
			if(*(p+4) == '0')
			{
				Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdClockwiseAdd);
				printf("�趨Hoisting������˳ʱ��Ӽ���\r\n");
			}
			else 
			{
				Encoder_SetCommand(&EncoderData.Hoisting,ENCODER_CmdAnticlockwiseAdd);
				printf("�趨Hoisting��������ʱ��Ӽ���\r\n");
			}
			break;
		}
		//
		default:
		{
			printf("Encoder.c���յ���Ч����ָ��%u!\r\n",cmd);
			break;
		}
	}
}
//---
/*
	�������� void Encoder_DebugRTU((u8 *p)
	������		�ú���ΪEncoder.c�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*/
void Encoder_DebugRTU(u8 *p,u8 len)
{
	switch(EncoderData.CollectMode)
	{
		//------------------------------------------------------------------
		//��ת������
		case ENCODER_SlewingRunModeBasic+1:
		{//�յ�Slewing��������
//			u32 bite=0;
			float buffer=0;
			EncoderData.Slewing.ValCircle = *(p+3)<<8 | *(p+4);//Ȧ��
			EncoderData.Slewing.ValAngle = *(p+5)<<8 | *(p+6);//����
//			bite = EncoderData.Slewing.ValCircle*4096+EncoderData.Slewing.ValAngle;
//			buffer = Tower_CalSlewing(bite);
			Server_GetTowerCraneData()->Rotation = (u16)(buffer+0.5);
//			printf("��ǰ�Ƕ�Ϊ��%u\r\n",Server_GetTowerCraneData()->Rotation);
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+2:
		{//���õ�ַ
			printf("�յ�ENCODER_Slewing���õ�ַ�ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+3:
		{//���ò�����
			printf("�յ�ENCODER_Slewing���ò����ʻظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+4:
		{//��ȡ�趨ֵ
			printf("�յ�ENCODER_Slewing��ȡ�趨ֵ�ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+5:
		{//��ȡ��ǰ��������ֵ
			printf("�յ�ENCODER_Slewing��ȡ��ǰ��������ֵ�ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+6:
		{//��ȡ��Ȧ���Ȧλ����
			printf("�յ�ENCODER_Slewing��ȡ��Ȧ���Ȧλ���Ȼظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+7:
		{//���õ�ǰȦ��Ϊ2047Ȧ
			printf("�յ�ENCODER_Slewing���õ�ǰȦ��Ϊ2047Ȧ�ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+8:
		{//�趨��ǰȦ��0~15Ȧ
			printf("�յ�ENCODER_Slewing�趨��ǰȦ��0~15Ȧ�ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+9:
		{//����Ϊ˳ʱ��+����
			printf("�յ�ENCODER_Slewing����Ϊ˳ʱ��+�����ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_SlewingRunModeBasic+10:
		{//����Ϊ��ʱ��-����
			printf("�յ�ENCODER_Slewing����Ϊ��ʱ��-�����ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		//------------------------------------------------------------------
		//���������
		case ENCODER_TrolleyingRunModeBasic+1:
		{//�յ�Trolleying��������
//			u32 bite=0;
			float buffer=0;
			EncoderData.Trolleying.ValCircle = *(p+3)<<8 | *(p+4);//Ȧ��
			EncoderData.Trolleying.ValAngle = *(p+5)<<8 | *(p+6);//����
//			bite = EncoderData.Trolleying.ValCircle*4096+EncoderData.Trolleying.ValAngle;
//			buffer = Tower_CalTrolleying(bite);
			Server_GetTowerCraneData()->Range = (u16)(buffer/10+0.5);
//			printf("��ǰ���Ϊ��%f��\r\n",(float)Server_GetTowerCraneData()->Range/100);
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+2:
		{//���õ�ַ
			printf("�յ�ENCODER_Trolleying���õ�ַ�ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+3:
		{//���ò�����
			printf("�յ�ENCODER_Trolleying���ò����ʻظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+4:
		{//��ȡ�趨ֵ
			printf("�յ�ENCODER_Trolleying��ȡ�趨ֵ�ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+5:
		{//��ȡ��ǰ��������ֵ
			printf("�յ�ENCODER_Trolleying��ȡ��ǰ��������ֵ�ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+6:
		{//��ȡ��Ȧ���Ȧλ����
			printf("�յ�ENCODER_Trolleying��ȡ��Ȧ���Ȧλ���Ȼظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+7:
		{//���õ�ǰȦ��Ϊ2047Ȧ
			printf("�յ�ENCODER_Trolleying���õ�ǰȦ��Ϊ2047Ȧ�ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+8:
		{//�趨��ǰȦ��0~15Ȧ
			printf("�յ�ENCODER_Trolleying�趨��ǰȦ��0~15Ȧ�ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+9:
		{//����Ϊ˳ʱ��+����
			printf("�յ�ENCODER_Trolleying����Ϊ˳ʱ��+�����ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_TrolleyingRunModeBasic+10:
		{//����Ϊ��ʱ��-����
			printf("�յ�ENCODER_Trolleying����Ϊ��ʱ��-�����ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		//------------------------------------------------------------------
		//���߱�����
		case ENCODER_HoistingRunModeBasic+1:
		{//�յ�Slewing��������
//			u32 bite=0;
			float buffer=0;
			EncoderData.Hoisting.ValCircle = *(p+3)<<8 | *(p+4);//Ȧ��
			EncoderData.Hoisting.ValAngle = *(p+5)<<8 | *(p+6);//����
//			bite = EncoderData.Hoisting.ValCircle*4096+EncoderData.Hoisting.ValAngle;
//			buffer = Tower_CalHoisting(bite);
			Server_GetTowerCraneData()->HookHeight = (u16)(buffer/10+0.5);
//			printf("��ǰ�����߶�Ϊ��%f��\r\n",(float)Server_GetTowerCraneData()->HookHeight/100);
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+2:
		{//���õ�ַ
			printf("�յ�ENCODER_Hoisting���õ�ַ�ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+3:
		{//���ò�����
			printf("�յ�ENCODER_Hoisting���ò����ʻظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+4:
		{//��ȡ�趨ֵ
			printf("�յ�ENCODER_Hoisting��ȡ�趨ֵ�ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+5:
		{//��ȡ��ǰ��������ֵ
			printf("�յ�ENCODER_Hoisting��ȡ��ǰ��������ֵ�ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+6:
		{//��ȡ��Ȧ���Ȧλ����
			printf("�յ�ENCODER_Hoisting��ȡ��Ȧ���Ȧλ���Ȼظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+7:
		{//���õ�ǰȦ��Ϊ2047Ȧ
			printf("�յ�ENCODER_Hoisting���õ�ǰȦ��Ϊ2047Ȧ�ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+8:
		{//�趨��ǰȦ��0~15Ȧ
			printf("�յ�ENCODER_Hoisting�趨��ǰȦ��0~15Ȧ�ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+9:
		{//����Ϊ˳ʱ��+����
			printf("�յ�ENCODER_Hoisting����Ϊ˳ʱ��+�����ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
		case ENCODER_HoistingRunModeBasic+10:
		{//����Ϊ��ʱ��-����
			printf("�յ�ENCODER_Hoisting����Ϊ˳ʱ��+-�����ظ���\r\n");
			EncoderData.CollectMode = 0;
			break;
		}
	}
	if(EncoderData.CollectMode == 0)
	{
		ENCODER_SlewingRESET();
		ENCODER_TrolleyingRESET();
		ENCODER_HoistingRESET();
	}
}
//---












