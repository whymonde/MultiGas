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
#include "RTU.h"

RTU_DataStructure RTUData;
//---
/*
	��������void RTU_main(void)
	������	�ú���ΪRTU.c������
*/
void RTU_main(void)
{
	/*
	switch(RTUData.RunMode)
	{
		case 1:
		{
			Flux_CommSendData(0xAA);
			RTUData.Error = 0xFF;
			RTUData.Timer = 100;
			RTUData.RunMode = 2;
			break;
		}
		case 2:
		{
			if(RTUData.Error == 0)
			{
//				printf("ͨѶ�ɹ�\r\n");
				RTUData.ErrorCnt = 0;
				RTUData.Timer = 2;
				RTUData.NewMode = 1;
				RTUData.RunMode = 251;
			}
			else if(RTUData.Timer == 0)
			{//�涨ʱ����û�н��յ����ݣ���Ϊ����
				if(RTUData.ErrorCnt < 20) 
				{
					RTUData.ErrorCnt++;//ͨѶ����ͳ��
				}
				else
				{//����
					Flux_GetData()->HJTem.Actual 					 = FLUX_ColFAULT; 	//�����¶�
					Slave_GetData()->AGPress.Value = FLUX_ColFAULT;		//A·��ѹ����ѹ��
					Slave_GetData()->AGTem.Value 	 = FLUX_ColFAULT;		//A·���£����£�
					Flux_GetData()->ChannelA.CTem.Actual 	 = FLUX_ColFAULT;		//A·���£����£�
					Slave_GetData()->ACPress.Value = FLUX_ColFAULT;		//A·��ѹ����ѹ��
					Slave_GetData()->BGPress.Value = FLUX_ColFAULT;		//B·��ѹ����ѹ��
					Slave_GetData()->BGTem.Value 	 = FLUX_ColFAULT;		//B·���£����£�
					Slave_GetData()->BCPress.Value = FLUX_ColFAULT;		//B·��ѹ����ѹ��
					Flux_GetData()->ChannelB.CTem.Actual 	 = FLUX_ColFAULT;		//B·���£����£�
					Slave_GetData()->TGPress.Value = FLUX_ColFAULT;		//TSP·��ѹ����ѹ��
					Flux_GetData()->ChannelT.GTem.Actual 	 = FLUX_ColFAULT;		//TSP·���£����£�
					Slave_GetData()->TCPress.Value = FLUX_ColFAULT;		//TSP·��ѹ����ѹ��
					Flux_GetData()->ChannelT.CTem.Actual 	 = FLUX_ColFAULT;		//TSP·���£����£�
					Flux_GetData()->DQPress.Actual 				 = FLUX_ColFAULT;		//����ѹ
					Flux_GetData()->DQTem.Actual 					 = FLUX_ColFAULT;		//������
//					printf("����ͨѶ����\r\n");
				}
				RTUData.Timer = 2;
				RTUData.NewMode = 1;
				RTUData.RunMode = 251;
			}
			break;
		}
		case 251:
		{
			if(RTUData.Timer == 0) RTUData.RunMode = RTUData.NewMode;
			break;
		}
	}
	*/
}
//---
/*
	��������void RTU_Timer(void)
	������RTU.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void RTU_Timer(void)
{
//	if(RTUData.Timer != 0) RTUData.Timer--;
}
//---
/*
	��������void RTU_Init(void)
	������	�ú���ΪRUT.c�ļ���ʼ��
*/
void RTU_Init(void)
{
//	RTUData.RunMode = 1;
}
//---
/*
	��������	void Master_ReceiveData(u8 *p,u8 len)
	������		�ú���Ϊ���������������͵�����
	���ã�		�ú�����ͨѶ�˿ڣ����ڣ�����
	ͨѶ��ʽ��
	��λ������λ������ʼ�롢��ַ�롢�����롢���ݳ����롢����1����������n��У���롢ֹͣ��
	��λ������λ������ʼ�롢������ַ�롢�����롢�����롢ִ�н�������롢���ݳ���λ������1����������n��У���롢ֹͣ��

*/
void RTU_CommunicateReceive(u8 *p,u8 len)
{
	switch(*(p))//�鿴���ĸ�������
	{
		case 0x10:
		{
			Project_DebugRTU(p+1,len-1);
			break;
		}
		default:
		{//����ع�����
			printf("RTU�յ���Ч������%u\r\n",*(p));
			break;
		}
	}
}
//---
/*
	��������void RTU_ReceiveSlave(u8 *p,u8 len)
	������	���յ��ӻ��Ĵ���������
*//*
void RTU_ReceiveSlave(u8 *p,u8 len)
{
	switch(p[0])
	{
		case 0x01:
		{
			if(len == 63) 
			{
				RTUData.Error = 0;
				Flux_CommReceiveData(&p[1]);
			}
			break;
		}
	}
}*/
//---
/*
	��������void RTU_ReadSlaveBase(void)
	������	�ú���Ϊ��ȡ�ӻ��Ļ�������
*//*
void RTU_ReadSlaveBase(u8 address)
{
	u8 bf[10];
	u8 cnt = 0,i;
	TwoBytes two;
	bf[cnt++] = 0x01;
	two.U16Data = CRC16(bf,cnt);
	bf[cnt++] = two.U16Data>>8;
	bf[cnt++] = two.U16Data;
	RTU_SendBufferData(RTU_UsartComputerPort,RTU_UsartComputerBuffer,address);
	RTU_SendBufferData(RTU_UsartComputerPort,RTU_UsartComputerBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		RTU_SendBufferData(RTU_UsartComputerPort,RTU_UsartComputerBuffer,bf[i]);
	}
}*/























//---
