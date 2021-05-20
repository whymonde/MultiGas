/*
	���ļ�Ϊ����Ŀ�����ͨѶ����غ����ļ�
	���庯������
*/
#include "computer.h"

/*
	��������void Computer_main(void)
	������	�ú���Ϊcomputer.c�ļ�����������ϵͳ����������
*/
void Computer_main(void)
{
	
}
//---
/*
	��������void Computer_Init(void)
	������	�ú���Ϊcomputer.c�ļ���ʼ����������ϵͳ��ʼ����������
*/
void Computer_Init(void)
{
	
}
//---
/*
	��������void Computer_Timer(void)
	������	�ú���Ϊcomputer.c�ļ���ʱ������ϵͳ1ms��ʱ����������
*/
void Computer_Timer(void)
{
	
}
//---
/*
	��������void Computer_DebugRTU(u8 *p,u8 len)
	������	�ú���Ϊ�������Ե��Է��͵�����
*/
void Computer_DebugRTU(u8 *p,u8 len)
{
	switch(*(p+1))
	{
		case 0x00:
		{//��ҳ������ʾ
			switch(*(p+2))
			{//��ҳ��������ʾ
				case 0x01:
				{//��ѯ��ǰʵʱ����
					break;
				}
			}
			break;
		}
		case 0x01:
		{//�������
			switch(*(p+2))
			{
				case 0x01:
				{//������������Ϣ
					break;
				}
				case 0x21:
				{//����ת����
					break;
				}
				case 0x41:
				{//����ת����
					break;
				}
				case 0x61:
				{//����ת����
					break;
				}
			}
			break;
		}
		case 0x02:
		{//���������
			switch(*(p+2))
			{
				case 1:
				{//��ѯ������1ʵʱֵ
					
					break;
				}
				case 2:
				{//��ѯ������1���ֵ
					break;
				}
				case 0x21:
				{//��ѯ������2ʵʱֵ
					break;
				}
				case 0x22:
				{//��ѯ������2���ֵ
					break;
				}
				case 0x41:
				{//��ѯ������3ʵʱֵ
					break;
				}
				case 0x42:
				{//��ѯ������3���ֵ
					break;
				}
				
			}
			break;
		}
		case 0x03:
		{//�������
			switch(*(p+2))
			{//�������
				case 0x01:
				{//����ǰ����ʵʱֵ
					break;
				}
				case 0x81:
				{//д�����ֵ
					break; 
				}
				case 0x82:
				{//д������ֵ
					break; 
				}
				case 0x83:
				{//д����������
					break; 
				}
				case 0x84:
				{//ȥƤ
					break; 
				}
				case 0x85:
				{//ȡ��ȥƤ
					break; 
				}
			}
			break;
		}
		case 0x04:
		{//�������
			switch(*(p+2))
			{
				case 0x01:
				{//��ȡ������������
					break;
				}
				case 0x02:
				{//��ȡ���ܲ���
					break;
				}
				case 0x03:
				{//��ȡ���ò���
					break;
				}
				case 0x81:
				{//���õ�ѹ����
					break;
				}
				case 0x82:
				{//���õ�������
					break;
				}
				case 0x83:
				{//���ý��߷�ʽ
					break;
				}
				case 0x84:
				{//����ͨѶ��ַ
					break;
				}
				case 0x85:
				{//����ͨѶ������
					break;
				}
				case 0x12:
				{//��ȡ���ܲ���
					break;
				}
			}
			break;
		}
		case 0x05:
		{//���ٴ�����
			switch(*(p+2))
			{
				case 0x01:
				{//������ֵ
					break;
				}
				case 0x02:
				{//�����ò���
					break;
				}
				case 0x81:
				{//���õ�ַ
					break;
				}
				case 0x82:
				{//���ò�����
					break;
				}
			}
			break;
		}
		case 0x06:
		{//�¶ȴ�����
			break;
		}
		case 0x07:
		{//ϵͳ���
			break;
		}
	}
}
//---
void Computer_Send(u8 *p,u8 len)
{
	u8 buffer[30];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = 0xF1;//��ַ
	for(crc=0;crc<len;crc++)
	{
		buffer[cnt++]= *p;
		p++;
	}
	crc = Calculate_CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	for(crc=0;crc<cnt;crc++)
	{
		Server_SendBufferData(RTU_UsartComputerPort,RTU_UsartComputerBuffer,buffer[crc]);//��������
	}
}
//---











