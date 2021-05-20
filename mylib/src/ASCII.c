/*
	���ļ�����2017��7��28��
	���ļ�Ϊ���ڱ�ϵͳ������ϵͳ֮���RTUͨѶ
ͨѶ��ʽ
	��λ������λ������ʼ�롢��ַ�롢�����롢�����롢���ݳ����롢����1����������n��У����ߡ�У����͡�ֹͣ��
	��λ������λ������ʼ�롢������ַ�롢�����롢�����롢ִ�н�������롢���ݳ���λ������1����������n��У���롢ֹͣ��
λ�룺
	��ʼ�룺0XAA
	��ַ�룺0X01~0XFF��0X00Ϊ�����ַ��
	�����룺ָʾ��ǰ�Ĺ���
	���ݳ����룺���п��ޣ�0X00~0XFF
	У���룺
	ֹͣ�룺0X55
*/
#include "ASCII.h"
#include "Button.h"


/*
	�������� void ASCII_DebugCmd(u8 *p,u8 len)
	������	 ����ϵͳ�����豸
	���룺	 ָ�����׵�ַ��˵��
					 0~2��Ϊָ���룬��3��Ϊ�ָ���"|"��4~5��Ϊ���ݳ��ȣ���6��Ϊ�ָ���"|",7~7+n��Ϊ����
	��ʽ��	 :000|01|������������
*/
void ASCII_CommunicateReceive(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,2);//��������
	switch(cmd)
	{
		case 1:
		{
			Flashisp_DebugASCII(p+3,len-3);
			break;
		}
		case 2:
		{
			Screen_DebugASCII(p+3,len-3);
			break;
		}
		case 3:
		{
			File_DebugASCII(p+3,len-3);
			break;
		}
		case 4:
		{
			Flux_DebugASCII(p+3,len-3);
			break;
		}
		case 5:
		{
			Print_DebugASCII(p+3,len-3);
			break;
		}
		case 6:
		{
			Button_DebugASCII(p+3,len-3);
			break;
		}
		case 7:
		{
			Battery_DebugASCII(p+3,len-3);
			break;
		}
		case 8:
		{
			Slave_DebugASCII(p+3,len-3);
			break;
		}
		case 9:
		{
			Usb_DebugASCII(p+3,len-3);
			break;
		}		
		case 90:
		{
			Project_DebugASCII(p+3,len-3);
			break;
		}
			case 10:
		{
			Flux_DebugASCII(p+3,len-3);
			break;
		}
		case 11:
		{
			AT_DebugASCII(p+3,len-3);
			break;
		}
		case 12:
		{
			APP_DebugASCII(p+3,len-3);
			break;
		}
		default:
		{
			printf("ASCII.c���յ���Ч����ָ��%u!\r\n",cmd);
			break;
		}
	}
}
//---
