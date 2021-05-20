/*
	���ļ�Ϊ����Ŀ�ĵ�����غ���
	���庯������

*/

#include "electric.h"

Electric_DataStructure ElectricData;//�����ṹ�����


/*
	��������void Electric_Init(void)
	������	�ú���ΪElectric.c��ʼ���ļ�
*/
void Electric_Init(void)
{
	Electric_DataInit();
}
//---
/*
	��������void Electric_DataInit(void)
	������	�ú���ΪElectricData���ݳ�ʼ��
*/
void Electric_DataInit(void)
{
//	ElectricData.Flag = 0;
	ElectricData.Address = ELECTRIC_DefaultAddress;
	ElectricData.RunMode = ELECTRIC_RunModeNone;//������ģʽ
}
//---
/*
	��������Electric_DataStructure* Electric_GetData(void)
	������	�ú���Ϊ��ȡElectricData������ַ
*/
Electric_DataStructure* Electric_GetData(void)
{
	return &ElectricData;
}
//---
/*
	��������void Electric_CollectBasicData(void)
	������	�ú���Ϊ���ڷ��Ͳɼ�OHR-D23���������������26��������
	01 03 01 00 00 34 45 E1
*/
void Electric_CollectBasicData(void)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = ElectricData.Address;
	buffer[cnt++] = ELECTRIC_ReadWords;//��ȡ����
	buffer[cnt++] = 0x01;//��ʼ��ַ0x0100��2�ֽ�
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x00;//���ݳ���0x0034(52��������)��2�ֽ�
	buffer[cnt++] = 0x34;
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	for(crc=0;crc<cnt;crc++)
	{
		Electric_SendBufferData(Electric_UsartPort,Electric_UsartBuffer,buffer[crc]);//��������
	}
	ElectricData.RunMode = ELECTRIC_RunModeBasicData;
}
//---
/*
	��������void Electric_SendReadCmd(u16 address,u16 len)
	������	�ú���Ϊ���Ͷ�ȡ����
	���룺	address����ʼ��ַ
					len����ȡ���ݵĳ��ȣ�����Ϊ��λ
*/
void Electric_SendReadCmd(u16 address,u16 len)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = ElectricData.Address;
	buffer[cnt++] = ELECTRIC_ReadWords;//��ȡ����
	buffer[cnt++] = address>>8;//��ʼ��ַ���ֽڣ�0x0100��2�ֽ�
	buffer[cnt++] = address;
	buffer[cnt++] = len>>8;//���ݳ���0x0034(52��������)��2�ֽ�
	buffer[cnt++] = len;
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	for(crc=0;crc<cnt;crc++)
	{
		Electric_SendBufferData(Electric_UsartPort,Electric_UsartBuffer,buffer[crc]);//��������
	}
	switch(address)
	{
		case ELECTRIC_AddBasicData:
		{//��ǰ��ȡ������������
			ElectricData.RunMode = ELECTRIC_RunModeBasicData;
			break;
		}
		case ELECTRIC_AddEnergyData:
		{//��ǰ��ȡ���ܲ���
			ElectricData.RunMode = ELECTRIC_RunModeEnergyData;
			break;
		}
		case ELECTRIC_AddParameterData:
		{//��ǰ��ȡ���ò���
			ElectricData.RunMode = ELECTRIC_RunModeParameterData;
			break;
		}
	}
}
//---
/*
	��������void Electric_WriteOneWord(u16 address,u16 data)
	������	�ú���Ϊ��Ŀ��Ĵ���д��һ��������
*/
void Electric_WriteOneWord(u16 address,u16 data)
{
	u8 buffer[8];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = ElectricData.Address;
	buffer[cnt++] = ELECTRIC_WriteOneWord;//��ȡ����
	buffer[cnt++] = address>>8;//��ʼ��ַ���ֽڣ�0x0100��2�ֽ�
	buffer[cnt++] = address;
	buffer[cnt++] = data>>8;//���ݳ���0x0034(52��������)��2�ֽ�
	buffer[cnt++] = data;
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	for(crc=0;crc<cnt;crc++)
	{
		Electric_SendBufferData(Electric_UsartPort,Electric_UsartBuffer,buffer[crc]);//��������
	}
	switch(address)
	{
		case ELECTRIC_AddVoltageRatio:
		{//��ѹ����
			ElectricData.RunMode = ELECTRIC_RunModeVoltageRatio;
			break;
		}
		case ELECTRIC_AddCurrentRatio:
		{//��������
			ElectricData.RunMode = ELECTRIC_RunModeCurrentRatio;
			break;
		}
		case ELECTRIC_AddWiringWay:
		{//���߷�ʽ
			ElectricData.RunMode = ELECTRIC_RunModeWiringWay;
			break;
		}
		case ELECTRIC_AddCommAddress:
		{//ͨѶ��ַ
			ElectricData.RunMode = ELECTRIC_RunModeCommAddress;
			break;
		}
		case ELECTRIC_AddBaudRate:
		{//������
			ElectricData.RunMode = ELECTRIC_RunModeBaudRate;
			break;
		}
	}
}
//---
//-------------------------------------���Ժ���----------------------------------------
//---
/*
	�������� void Electric_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪElectricc�ļ���ASCII�����ļ�
	:000|60|XXX������������
*/
void Electric_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1://
		{//:000|60|001
			Electric_SetComNoneVerify();//��У��
			Electric_CollectBasicData();			//��������
			printf("���Ͷ�ȡ����������������!\r\n");
			break;
		}
		case 2:
		{//:000|60|002
			Electric_SetComNoneVerify();//��У��
			Electric_SendReadCmd(ELECTRIC_AddBasicData,ELECTRIC_LenBasicData);//
			printf("���Ͷ�ȡ����������������!\r\n");
			break;
		}
		case 3:
		{//:000|60|003
			Electric_SetComNoneVerify();//��У��
			Electric_SendReadCmd(ELECTRIC_AddEnergyData,ELECTRIC_LenEnergyData);//
			printf("���Ͷ�ȡ������������!\r\n");
			break;
		}
		case 4:
		{//:000|60|004
			Electric_SetComNoneVerify();//��У��
			Electric_SendReadCmd(ELECTRIC_AddParameterData,ELECTRIC_LenParameterData);//
			printf("���Ͷ�ȡ�������ò�����������!\r\n");
			break;
		}
		case 30:
		{//�޸ĵ�ѹ���ʣ����÷�Χ0~1200	:000|60|030|1200\r\n
			u16 buffer = Calculate_CharToU16(p+4,4);
			Electric_SetComNoneVerify();//��У��
			Electric_WriteOneWord(ELECTRIC_AddVoltageRatio,buffer);//
			printf("�޸ĵ�ѹ����Ϊ��%u\r\n",buffer);
			break;
		}
		case 31:
		{//�޸ĵ������ʣ����÷�Χ0~1200	:000|60|031|1200\r\n
			u16 buffer = Calculate_CharToU16(p+4,4);
			Electric_SetComNoneVerify();//��У��
			Electric_WriteOneWord(ELECTRIC_AddCurrentRatio,buffer);//
			printf("�޸ĵ�������Ϊ��%u\r\n",buffer);
			break;
		}
		case 32:
		{//�޸Ľ��߷�ʽ��0Ϊ�������ߣ�1Ϊ�������ߣ�2Ϊ3V3A��	:000|60|032|0\r\n
			u16 buffer = Calculate_CharToU16(p+4,1);
			Electric_SetComNoneVerify();//��У��
			Electric_WriteOneWord(ELECTRIC_AddWiringWay,buffer);//
			printf("�޸Ľ��߷�ʽ��%u\r\n",buffer);
			break;
		}
		case 33:
		{//���Ǳ�ͨѶ��ַ�����÷�Χ1~253		:000|60|033|001\r\n
			u16 buffer = Calculate_CharToU16(p+4,3);
			Electric_SetComNoneVerify();//��У��
			Electric_WriteOneWord(ELECTRIC_AddCommAddress,buffer);//
			printf("�޸�ͨ�ŵ�ַΪ��%u\r\n",buffer);
			break;
		}
		case 34:
		{//���Ǳ�ͨѶ�����ʣ����÷�Χ0~4��0Ϊ1200,1Ϊ2400,2Ϊ4800,3Ϊ9600,4Ϊ19200��		
			//:000|60|034|3\r\n
			u16 buffer = Calculate_CharToU16(p+4,1);
			Electric_SetComNoneVerify();//��У��
			Electric_WriteOneWord(ELECTRIC_AddBaudRate,buffer);//
			printf("�޸�ͨѶ������Ϊ��%u\r\n",buffer);
			break;
		}
		default:
		{
			printf("Electric.c���յ���Ч����ָ��%u!\r\n",cmd);
			break;
		}
	}
}
//---
/*
	�������� void Electric_DebugRTU((u8 *p)
	������	�ú���ΪElectric.c�ļ���RTU�����ļ�
	˵����	���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*/
void Electric_DebugRTU(u8 *p,u8 len)
{
	switch(ElectricData.RunMode)//�鿴���ĸ�������
	{
		case ELECTRIC_RunModeBasicData:
		{//�����������ݴ���
			
				
//			u32 energy;

//			u8 buffer[10];
//			u8 cnt = 0;
//			u8 *pdata = p+3+12*4;
//			float PAY = 0;
//			pdata = (u8*)&PAY;
//			buffer[cnt++] = *pdata++;		
//			buffer[cnt++] = *pdata++;			
//			buffer[cnt++] = *pdata++;			
//			buffer[cnt++] = *pdata;	
//			energy = PAY;
//			Server_GetTowerCraneData()->CurrentPower = (u16)energy;
//			
//			printf("���ʣ�%x,%x,%x,%x\r\n",*(p+3+12*4),*(p+4+12*4),*(p+5+12*4),*(p+6+12*4));
//			
//			printf("���ʣ�%f\r\n",PAY);
//			energy = *(p+3+12*4)<<24 | *(p+4+12*4)<<16 | *(p+5+12*4)<<8 | *(p+6+12*4);//�������й�����
//			Server_GetTowerCraneData()->CurrentPower = (u16)((float)energy / 10000000.0);//��ǰ���ʣ�����Ϊ0.1KW
//			printf("��ǰ�ܹ���Ϊ%u\r\n",energy);
//			printf("��ǰ�ܹ���Ϊ%fw\r\n",((float)Server_GetTowerCraneData()->CurrentPower/10.0));
			break;
		}
		case ELECTRIC_RunModeEnergyData:
		{
			printf("��ȡ��������\r\n");
			break;
		}
		case ELECTRIC_RunModeParameterData:
		{
			printf("��ȡ�������ò���\r\n");
			break;
		}
		//д��
		case ELECTRIC_RunModeVoltageRatio:
		{
			printf("д���ѹ����\r\n");
			break;
		}
		case ELECTRIC_RunModeCurrentRatio:
		{
			printf("д���������\r\n");
			break;
		}
		case ELECTRIC_RunModeWiringWay:
		{
			printf("д����߷�ʽ\r\n");
			break;
		}
		case ELECTRIC_RunModeCommAddress:
		{
			printf("д��ͨѶ��ַ\r\n");
			break;
		}
		case ELECTRIC_RunModeBaudRate:
		{
			printf("д��ͨѶ������\r\n");
			break;
		}
		
	}
	ElectricData.RunMode = ELECTRIC_RunModeNone;//������ģʽ
}
//---





