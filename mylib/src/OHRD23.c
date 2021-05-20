/*
���ļ�Ϊ����OHRD23ϵ������Һ�����ܵ�����������غ���
	���庯������
*/

#include "OHRD23.h"

Ohrd23_DataStructure Ohrd23Data;
/*
	��������void Ohrd23_Main(void)
	������	OHRD23.c������
*/
void Ohrd23_Main(void)
{
	if(Ohrd23Data.RunMode != 0)
	switch(Ohrd23Data.RunMode)
	{
		case 201:
		{
			Ohrd23Data.Flag |= B0;
			Ohrd23Data.Conf.ADR = 1;
			Ohrd23Data.Conf.BR = 4;
			Ohrd23Data.RunMode = 202;
			break;
		}
		case 202:
		{
			Ohrd23_ReadConf();
			printf("BR=%u,ADR=%u\r\n",Ohrd23Data.Conf.BR,Ohrd23Data.Conf.ADR);
			Ohrd23Data.RunMode = 203;
			break;
		}
		case 203:
		{
			if(Ohrd23Data.CollectMode == 0)
			{
				if((Ohrd23Data.Flag & B0) ==0)
				{
					printf("������ϣ��ҵ�OHRD23\r\n");
					Ohrd23Data.RunMode = 0;
				}
				else
				{
					Ohrd23Data.Conf.ADR++;
					if(Ohrd23Data.Conf.ADR < 254)
					{
						Ohrd23Data.RunMode = 202;
					}
					else
					{
						Ohrd23Data.Conf.ADR = 1;
						Ohrd23Data.Conf.BR--;
						if(Ohrd23Data.Conf.BR==0)
						{
							Ohrd23Data.RunMode = 204;
						}
						else
						{
							Ohrd23Data.RunMode = 202;
						}
					}
				}
			}
			break;
		}
		case 204:
		{
			Ohrd23_ReadConf();
			printf("BR=%u,ADR=%u\r\n",Ohrd23Data.Conf.BR,Ohrd23Data.Conf.ADR);
			Ohrd23Data.RunMode = 205;
			break;
		}
		case 205:
		{
			if(Ohrd23Data.CollectMode == 0)
			{
				if((Ohrd23Data.Flag & B0) ==0)
				{
					printf("������ϣ��ҵ�OHRD23\r\n");
					Ohrd23Data.RunMode = 0;
				}
				else
				{
					Ohrd23Data.Conf.ADR++;
					if(Ohrd23Data.Conf.ADR < 254)
					{
						Ohrd23Data.RunMode = 0;
						printf("������ϣ�û���ҵ�OHRD23\r\n");
					}
					else
					{
						Ohrd23Data.RunMode = 204;
					}
				}
			}
			break;
		}
	}
	
	//---
	if(Ohrd23Data.CollectMode != 0)
	{//��ǰ������ͨѶ
		if(Ohrd23Data.Timer == 0)
		{//��ʱδ���յ�����
//			printf("��ʱδ���յ�����\r\n");
			Ohrd23Data.CollectMode = 0;
			Ohrd23Data.ErrorCnt++;
			Ohrd23Data.Flag |= B1;//ͨѶ����
			if(Ohrd23Data.ErrorCnt == 10)
			{//����10��û���յ��ظ�����Ϊ����
				Ohrd23Data.ErrorCnt = 0;
				Ohrd23Data.Flag |= B0;//ͨѶ����
			}
		}
	}
}
//---
/*
	��������void Ohrd23_Collect(void)
	
*/
void Ohrd23_Collect(void)
{
	switch(Ohrd23Data.RunMode)
	{
		//�ɼ�������������
		case 1:
		{//�ɼ�������������
			Ohrd23_ReadBase();
			Ohrd23Data.Timer = 200;
			Ohrd23Data.RunMode = 2;
			break;
		}
		case 2:
		{
			if(Ohrd23Data.CollectMode == 0)
			{
				Ohrd23Data.RunMode = 1;
			}
			else if(Ohrd23Data.Timer == 0)
			{
				Ohrd23Data.RunMode = 1;
			}
			break;
		}
		//�ɼ���������
		case 11:
		{
			Ohrd23_ReadEnergy();
			Ohrd23Data.Timer = 200;
			Ohrd23Data.RunMode = 12;
			break;
		}
		case 12:
		{
			if(Ohrd23Data.CollectMode == 0)
			{
//				printf("OK");
				Ohrd23Data.RunMode = 11;
			}
			else if(Ohrd23Data.Timer == 0)
			{
//				printf("BUG");
				Ohrd23Data.RunMode = 11;
			}
			break;
		}
	}
}
//---
/*
	��������void Ohrd23_Init(void)
	������	OHRD23.c�ļ���ʼ������
*/
void Ohrd23_Init(void)
{
	u8* pdata = (u8*)&Ohrd23Data.Conf;
	Ohrd23_ReadStoreData(OHRD23_AdrConfBase,pdata,OHRD23_AdrStoreEnd);//��ȡ���ò���
	Ohrd23Data.CollectCommand = 0;
	Ohrd23Data.Conf.ADR = 4;
	Ohrd23Data.Conf.BR = 4;
}
//---
/*
	��������void Ohrd23_Timer(void)
	������OHRD23.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void Ohrd23_Timer(void)
{
	if(Ohrd23Data.Timer != 0)	Ohrd23Data.Timer--;
}
//---
/*
	��������Ohrd23_DataStructure* Ohrd23_GetOhrd23Data(void)
	��������ȡ����OHRD23.c�ļ�ȫ�ֱ����ṹ��
*/
Ohrd23_DataStructure* Ohrd23_GetOhrd23Data(void)
{
	return &Ohrd23Data;
}
//---
/*
��������void Ohrd23_ReadBase(void)
��������������������
˵�����׵�ַOHRD23_AdrBase�����ݳ���52
*/
void Ohrd23_ReadBase(void)
{
	Ohrd23_ReadData(OHRD23_AdrBase,52);
	Ohrd23Data.CollectMode = 0x01;
}
//---
/*
��������void Ohrd23_ReadBase(void)
�����������е�������
˵�����׵�ַOHRD23_AdrEnergy�����ݳ���12
*/
void Ohrd23_ReadEnergy(void)
{
	Ohrd23_ReadData(OHRD23_AdrEnergy,12);
	Ohrd23Data.CollectMode = 0x02;
}
//---
/*
��������void Ohrd23_ReadConf(void)
�������������ò���
˵�����׵�ַOHRD23_AdrConf�����ݳ���5
*/
void Ohrd23_ReadConf(void)
{
	Ohrd23_ReadData(OHRD23_AdrConf,5);
	Ohrd23Data.CollectMode = 0x03;
}
//---
/*
��������void Ohrd23_SetVT(s16 rate)
������	���õ�ѹ���ʣ��������뷶Χ1-1200
*/
void Ohrd23_SetVT(s16 rate)
{
 	TwoBytes buffer;
	buffer.S16Data = rate;
	Ohrd23_WriteOneWord(OHRD23_AdrConfVT,buffer.U16Data);
	Ohrd23Data.CollectMode = 0x81;
}
//---
/*
��������void Ohrd23_SetIT(s16 rate)
������	���õ������ʣ��������뷶Χ1-1200
*/
void Ohrd23_SetIT(s16 rate)
{
 	TwoBytes buffer;
	buffer.S16Data = rate;
	Ohrd23_WriteOneWord(OHRD23_AdrConfIT,buffer.U16Data);
	Ohrd23Data.CollectMode = 0x82;
}
//---
/*
��������void Ohrd23_SetLM(u8 mode)
������	���ý���ģʽ���������뷶Χ0=�������ߣ�1=�������ߣ�2=3V3A
*/
void Ohrd23_SetLM(u8 mode)
{
	Ohrd23_WriteOneWord(OHRD23_AdrConfLM,mode);
	Ohrd23Data.CollectMode = 0x83;
}
//---
/*
��������void Ohrd23_SetAddress(u8 address)
������	���õ�ַ���������뷶Χ1-253
*/
void Ohrd23_SetAddress(u8 address)
{
	Ohrd23_WriteOneWord(OHRD23_AdrConfADR,address);
	Ohrd23Data.CollectMode = 0x84;
}
//---
/*
��������void Ohrd23_SetAddress(u8 address)
������	���õ�ַ���������뷶Χ0-4�ֱ��Ӧ1200~19200
*/
void Ohrd23_SetBaudRate(u8 baudrate)
{
	Ohrd23_WriteOneWord(OHRD23_AdrConfBR,baudrate);
	Ohrd23Data.CollectMode = 0x85;
}
//---
/*
	��������void Ohrd23_SetConfiguration(OHRD23_ConfigurationStructure *p)
	����������OHRD23�����ò���
*/
void Ohrd23_SetConfiguration(OHRD23_ConfigurationStructure *p)
{
	TwoBytes change;
	u16 buffer[5];
	u8 cnt = 0;
	change.S16Data = p->VT;
	buffer[cnt++] = change.U16Data;
	change.S16Data = p->IT;
	buffer[cnt++] = change.U16Data;
	change.S16Data = p->LM;
	buffer[cnt++] = change.U16Data;
	change.S16Data = p->ADR;
	buffer[cnt++] = change.U16Data;
	change.S16Data = p->BR;
	buffer[cnt++] = change.U16Data;
	Ohrd23_WriteMultiWord(OHRD23_AdrConf,buffer,5);
	Ohrd23Data.CollectMode = 0x86;
}
//---
/*
��������void Ohrd23_ReadData(u16 address,u8 amount)
���������Ͷ�ȡ��������
���룺u16 address��ȡ���ݵ���ʼ��ַ
			u8 amount��ȡ���ݵĸ���������Ϊ��λ
*/
void Ohrd23_ReadData(u16 address,u16 amount)
{
	u8 buffer[8],cnt=0;
	TwoBytes change;
	buffer[cnt++] = Ohrd23Data.Conf.ADR;
	buffer[cnt++] = 0x03;//��ȡ����
	change.U16Data = address;
//	printf("ͨѶ��ַΪ=%d\r\n",address);
	buffer[cnt++] = change.U8Data[1];//��ʼ��ַ0x0100��2�ֽ�
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = amount;
	buffer[cnt++] = change.U8Data[1];//���ݳ���0x0034(52��������)��2�ֽ�
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = CRC16(buffer,cnt);
	buffer[cnt++] = change.U8Data[1];//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = change.U8Data[0];
	Ohrd23_UsartModeSet();
	for(change.U16Data=0;change.U16Data<cnt;change.U16Data++)
	{
		Ohrd23_SendBufferData(Ohrd23_UsartPort,Ohrd23_UsartBuffer(),buffer[change.U16Data]);//��������
	}
	Ohrd23Data.Timer = 200;//������ʱ
}
//---
/*
	��������void Ohrd23_WriteOneWord(u16 address,u16 data)
	������	�ú���Ϊ��Ŀ��Ĵ���д��һ��������
*/
void Ohrd23_WriteOneWord(u16 address,u16 data)
{
	u8 buffer[8],cnt=0;
	TwoBytes change;
	buffer[cnt++] = (u8)Ohrd23Data.Conf.ADR;
	buffer[cnt++] = 0x06;					//д��һ��������
	change.U16Data = address;
	buffer[cnt++] = change.U8Data[1];//��ַ0x0100��2�ֽ�
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = data;
	buffer[cnt++] = change.U8Data[1];//д������ݣ�2�ֽ�
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = CRC16(buffer,cnt);
	buffer[cnt++] = change.U8Data[1];//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = change.U8Data[0];
	Ohrd23_UsartModeSet();
	for(change.U16Data=0;change.U16Data<cnt;change.U16Data++)
	{
		Ohrd23_SendBufferData(Ohrd23_UsartPort,Ohrd23_UsartBuffer(),buffer[change.U16Data]);//��������
	}
	Ohrd23Data.Timer = 200;//������ʱ
}
//---
/*
	��������void Ohrd23_WriteMultiWord(u16 address,u16 *p,u16 len)
	�������ú���Ϊ��Ŀ��Ĵ���д��д�����
*/
void Ohrd23_WriteMultiWord(u16 address,u16 *p,u16 len)
{
	u8 buffer[200],cnt=0,i=0;
	TwoBytes change;
	buffer[cnt++] = (u8)Ohrd23Data.Conf.ADR;
	buffer[cnt++] = 0x10;//д����������
	change.U16Data = address;
	buffer[cnt++] = change.U8Data[1];//��ַ
	buffer[cnt++] = change.U8Data[0];
	change.U16Data = len;
	buffer[cnt++] = change.U8Data[1];//�����ֳ���
	buffer[cnt++] = change.U8Data[0];
	buffer[cnt++] = len*2;					//�ֽ���
	for(i=0;i<len;i++)
	{
		change.U16Data = *p;
		buffer[cnt++] = change.U8Data[1];//��ַ
		buffer[cnt++] = change.U8Data[0];
		p++;
	}
	change.U16Data = CRC16(buffer,cnt);
	buffer[cnt++] = change.U8Data[1];//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = change.U8Data[0];
	Ohrd23_UsartModeSet();
	for(change.U16Data=0;change.U16Data<cnt;change.U16Data++)
	{
		Ohrd23_SendBufferData(Ohrd23_UsartPort,Ohrd23_UsartBuffer(),buffer[change.U16Data]);//��������
	}
	Ohrd23Data.Timer = 200;//������ʱ
}
//---
/*
��������void Ohrd23_BaseChange(u8 *p,OHRD23_BaseStructure* Base)
������	�ú�����һ֡104���ֽڣ���26�������������ݣ��Ļ�����������ת��ΪOhrd23������������
				�������ѹ���ߵ�ѹ����������й����ʣ��޹����ʣ����ڹ��ʣ�����������Ƶ��
����:		u8 *p���������������׵�ַ
				OHRD23_BaseStructure* Base�洢OHRD23�������������׵�ַ	
˵����	OHRD23�������Ǵ��ģʽ
				STM32��������С��ģʽ
				��������ת������Ϊ�߶Եͣ��ͶԸ�
*/
void Ohrd23_BaseChange(u8 *p,OHRD23_BaseStructure* Base)
{
	u8 i;
	FourBytes four;
	s32 *pS32 = &(Base->VA);
	float *pFlaot = &(Base->PAY);
	for(i=0;i<9;i++)
	{//�������ѹ���ߵ�ѹ�������
		four.U8Data[0] = *(p+3);
		four.U8Data[1] = *(p+2);
		four.U8Data[2] = *(p+1);
		four.U8Data[3] = *(p+0);
		*pS32 = four.S32Data;
		pS32++;
		p = p+4;
	}
	for(i=0;i<12;i++)
	{//�����й����ʣ��޹����ʣ����ڹ���
		four.U8Data[0] = *(p+3);
		four.U8Data[1] = *(p+2);
		four.U8Data[2] = *(p+1);
		four.U8Data[3] = *(p+0);
		*pFlaot = four.FloatData;
		pFlaot++;
		p = p+4;
	}
	pS32 = &(Base->FA);
	for(i=0;i<5;i++)
	{//�������ѹ���ߵ�ѹ�������
		four.U8Data[0] = *(p+3);
		four.U8Data[1] = *(p+2);
		four.U8Data[2] = *(p+1);
		four.U8Data[3] = *(p+0);
		*pS32 = four.S32Data;
		pS32++;
		p = p+4;
	}
	/*
	printf("�з��ų����ͣ�����%d��ʵ��ֵ%d��100=%f\r\n",Base->VA,Base->VA,Base->VA/100.0);
	printf("�з��ų����ͣ�����%d��ʵ��ֵ%d��100=%f\r\n",Base->VB,Base->VB,Base->VB/100.0);
	printf("�з��ų����ͣ�����%d��ʵ��ֵ%d��100=%f\r\n",Base->VC,Base->VC,Base->VC/100.0);
	printf("�з��ų����ͣ�����%d��ʵ��ֵ%d��100=%f\r\n",Base->VAB,Base->VAB,Base->VAB/100.0);
	printf("�з��ų����ͣ�����%d��ʵ��ֵ%d��100=%f\r\n",Base->VBC,Base->VBC,Base->VBC/100.0);
	printf("�з��ų����ͣ�����%d��ʵ��ֵ%d��100=%f\r\n",Base->VCA,Base->VCA,Base->VCA/100.0);
	printf("�з��ų����ͣ�����%d��ʵ��ֵ%d��1000=%f\r\n",Base->IA,Base->IA,Base->IA/1000.0);
	printf("�з��ų����ͣ�����%d��ʵ��ֵ%d��1000=%f\r\n",Base->IB,Base->IB,Base->IB/1000.0);
	printf("�з��ų����ͣ�����%d��ʵ��ֵ%d��1000=%f\r\n",Base->IC,Base->IC,Base->IC/1000.0);
	printf("�����ͣ�����%f��ʵ��ֵ%f��100=%f\r\n",Base->PAY,Base->PAY,Base->PAY/10.0);
	printf("�����ͣ�����%f��ʵ��ֵ%f��100=%f\r\n",Base->PBY,Base->PBY,Base->PBY/10.0);
	printf("�����ͣ�����%f��ʵ��ֵ%f��100=%f\r\n",Base->PCY,Base->PCY,Base->PCY/10.0);
	printf("�����ͣ�����%f��ʵ��ֵ%f��100=%f\r\n",Base->PZY,Base->PZY,Base->PZY/10.0);
	printf("�����ͣ�����%f��ʵ��ֵ%f��100=%f\r\n",Base->PAW,Base->PAW,Base->PAW/10.0);
	printf("�����ͣ�����%f��ʵ��ֵ%f��100=%f\r\n",Base->PBW,Base->PBW,Base->PBW/10.0);
	printf("�����ͣ�����%f��ʵ��ֵ%f��100=%f\r\n",Base->PCW,Base->PCW,Base->PCW/10.0);
	printf("�����ͣ�����%f��ʵ��ֵ%f��100=%f\r\n",Base->PZW,Base->PZW,Base->PZW/10.0);
	printf("�����ͣ�����%f��ʵ��ֵ%f��100=%f\r\n",Base->PAS,Base->PAS,Base->PAS/10.0);
	printf("�����ͣ�����%f��ʵ��ֵ%f��100=%f\r\n",Base->PBS,Base->PBS,Base->PBS/10.0);
	printf("�����ͣ�����%f��ʵ��ֵ%f��100=%f\r\n",Base->PCS,Base->PCS,Base->PCS/10.0);
	printf("�����ͣ�����%f��ʵ��ֵ%f��100=%f\r\n",Base->PZS,Base->PZS,Base->PZS/10.0);
	printf("�з��ų����ͣ�����%d��ʵ��ֵ%d��100=%f\r\n",Base->FA,Base->FA,Base->FA/1000.0);
	printf("�з��ų����ͣ�����%d��ʵ��ֵ%d��100=%f\r\n",Base->FB,Base->FB,Base->FB/1000.0);
	printf("�з��ų����ͣ�����%d��ʵ��ֵ%d��100=%f\r\n",Base->FC,Base->FC,Base->FC/1000.0);
	printf("�з��ų����ͣ�����%d��ʵ��ֵ%d��100=%f\r\n",Base->FZ,Base->FZ,Base->FZ/1000.0);
	printf("�з��ų����ͣ�����%d��ʵ��ֵ%d��100=%f\r\n",Base->Frequency,Base->Frequency,Base->Frequency/1000.0);
	*/
	
//	printf("A�����ѹ=%d\r\n",Base->VA);
//	printf("B�����ѹ=%d\r\n",Base->VB);
//	printf("C�����ѹ=%d\r\n",Base->VC);
//	printf("A-B�ߵ�ѹ=%d\r\n",Base->VAB);
//	printf("B-C�ߵ�ѹ=%d\r\n",Base->VBC);
//	printf("C-A�ߵ�ѹ=%d\r\n",Base->VCA);
//	printf("A�����=%d\r\n",Base->IA);
//	printf("B�����=%d\r\n",Base->IB);
//	printf("C�����=%d\r\n",Base->IC);
//	printf("A���й�����=%f\r\n",Base->PAY);
//	printf("B���й�����=%f\r\n",Base->PBY);
//	printf("C���й�����=%f\r\n",Base->PCY);
//	printf("�����й�����=%f\r\n",Base->PZY);
//	printf("A���޹�����=%f\r\n",Base->PAW);
//	printf("B���޹�����=%f\r\n",Base->PBW);
//	printf("C���޹�����=%f\r\n",Base->PCW);
//	printf("�����޹�����=%f\r\n",Base->PZW);
//	printf("A���ӹ�����=%f\r\n",Base->PAS);
//	printf("B���ӹ�����=%f\r\n",Base->PBS);
//	printf("C���ӹ�����=%f\r\n",Base->PCS);
//	printf("�����ӹ�����=%f\r\n",Base->PZS);
//	printf("A�๦������=%d\r\n",Base->FA);
//	printf("B�๦������=%d\r\n",Base->FB);
//	printf("C�๦������=%d\r\n",Base->FC);
//	printf("���๦������=%d\r\n",Base->FZ);
//	printf("Ƶ��=%d\r\n",Base->Frequency);
}
//---
/*
��������void Ohrd23_EnergyChange(u8 *p,OHRD23_EnergyStructure* Energy)
������	�ú�����һ֡24���ֽڣ���6�����ܲ������ĵ�������ת��ΪOhrd23�ĵ�������
����:		u8 *p���������������׵�ַ
				OHRD23_EnergyStructure* Energy�洢OHRD23���������׵�ַ	
˵����	OHRD23�������Ǵ��ģʽ
				STM32��������С��ģʽ
				��������ת������Ϊ�߶Եͣ��ͶԸ�	
*/
void Ohrd23_EnergyChange(u8 *p,OHRD23_EnergyStructure* Energy)
{
	u8 i;
	FourBytes buffer;
	s32 *pS32 = &(Energy->WHP);
	u32 *pU32 = &(Energy->SWHP);
	for(i=0;i<4;i++)
	{//���������й����ܣ������й����ܣ������޹����ܣ������޹�����
		buffer.U8Data[0] = *(p+3);
		buffer.U8Data[1] = *(p+2);
		buffer.U8Data[2] = *(p+1);
		buffer.U8Data[3] = *(p+0);
		*pS32 = buffer.S32Data;
		pS32++;
		p = p+4;
	}
	for(i=0;i<2;i++)
	{//�������й����ܣ����޹�����
		buffer.U8Data[0] = *(p+3);
		buffer.U8Data[1] = *(p+2);
		buffer.U8Data[2] = *(p+1);
		buffer.U8Data[3] = *(p+0);
		*pU32 = buffer.U32Data;
		pU32++;
		p = p+4;
	}
	
//	printf("�����й�����=%f\r\n",Energy->WHP/100.0);
//	printf("�����й�����=%f\r\n",Energy->WHN/100.0);
//	printf("�����޹�����=%f\r\n",Energy->VARHP/100.0);
//	printf("�����޹�����=%f\r\n",Energy->VARHN/100.0);
//	printf("���й�����=%f\r\n",Energy->SWHP/100.0);
//	printf("���޹�����=%f\r\n",Energy->SWHN/100.0);
	
	
	
	
	
}
//---
/*
	��������void Ohrd23_ConfigurationChange(u8 *p,OHRD23_ConfigurationStructure *conf)
	������
*/
void Ohrd23_ConfigurationChange(u8 *p,OHRD23_ConfigurationStructure *conf)
{
	u8 i;
	TwoBytes buffer;
	s16 *pS16 = &conf->VT;
	for(i=0;i<5;i++)
	{
		buffer.U8Data[0] = *(p+1);
		buffer.U8Data[1] = *(p);
		*pS16 = buffer.S16Data;
		pS16++;
		p = p+2;
	}
//	printf("��ѹ����=%d\r\n",conf->VT);
//	printf("��������=%d\r\n",conf->IT);
//	printf("����ģʽ=%d\r\n",conf->LM);
//	printf("ͨѶ��ַ=%d\r\n",conf->ADR);
//	printf("ͨѶ������=%d\r\n",conf->BR);
}
//---
/*
	��������void Ohrd23_SearchOhrd23(void)
	�������ú���Ϊ���������ϵ�Ohrd23������
	˵������Ҫ���������Ĺ����е��øú��������øú����󣬸ú�����ֱ��ռ��ϵͳ��֪���������
*/
void Ohrd23_SearchOhrd23(void)
{
	u8 i,j;
	Ohrd23Data.Flag |= B0;//ͨѶ����
	for(i=5;i>0;i--)
	{
		printf("\r\n%u\r\n",i-1);
//		Ohrd23Data.Conf.BR = i;
		for(j=1;j<253;j++)
		{
//			Ohrd23Data.Conf.ADR = j;
//			Ohrd23_ReadConf();
//			printf("A ");
//			Ohrd23Data.CollectMode = 1;
//			Ohrd23Data.Timer = 1000;//������ʱ
//			while(Ohrd23Data.CollectMode != 0);
			Ohrd23Data.Timer = 10;//������ʱ
			Ohrd23Data.CollectMode = 1;
			printf("%u ",j);
			while(Ohrd23Data.CollectMode != 0);
//			if(Ohrd23Data.Flag & B0) break;
		}
//		if(Ohrd23Data.Flag & B0) break;
	}
}
//---
/*
	void Ohrd23_UsartModeSet(void)
	������ΪOhrd23���ô���ͨѶģʽ
*/
void Ohrd23_UsartModeSet(void)
{
	u32 BaudRate=(u32)(1200*pow(2,Ohrd23Data.Conf.BR));
//	printf("ͨѶ������=%d\r\n",BaudRate);
	Ohrd23_UartModeSet(Ohrd23_UsartPort,BaudRate,'n','1');
}
//---
/*
	��������void Ohrd23_Operation(u16 mode,u8 *p,u8 len)
	�������ú���ΪOhrd23.c�����ݲ���
*/
void Ohrd23_Operation(u16 mode,u8 *p,u8 len)
{
	switch(mode)
	{
		//��ȡ
		case 1:
		{//��ȡ�������ò���
			break;
		}
		case 2:
		{//����ѹ����
			Ohrd23_ReadStoreData(OHRD23_AdrStoreVT,p,len);
			break;
		}
		case 3:
		{//����������
			Ohrd23_ReadStoreData(OHRD23_AdrStoreIT,p,len);
			break;
		}
		case 4:
		{//�����߷�ʽ
			Ohrd23_ReadStoreData(OHRD23_AdrStoreLM,p,len);
			break;
		}
		case 5:
		{//��ͨѶ��ַ
			Ohrd23_ReadStoreData(OHRD23_AdrStoreADR,p,len);
			break;
		}
		case 6:
		{//��ͨѶ������
			Ohrd23_ReadStoreData(OHRD23_AdrStoreBR,p,len);
			break;
		}
		//�޸�
		case 101:
		{//�޸��������ò���
			break;
		}
		case 102:
		{//�޸ĵ�ѹ����
			Ohrd23_StoreData(OHRD23_AdrStoreVT,p,len);
			break;
		}
		case 103:
		{//�޸ĵ�������
			Ohrd23_StoreData(OHRD23_AdrStoreIT,p,len);
			break;
		}
		case 104:
		{//�޸Ľ��߷�ʽ
			Ohrd23_StoreData(OHRD23_AdrStoreLM,p,len);
			break;
		}
		case 105:
		{//�޸�ͨѶ��ַ
			Ohrd23_StoreData(OHRD23_AdrStoreADR,p,len);
			break;
		}
		case 106:
		{//�޸�ͨѶ������
			Ohrd23_StoreData(OHRD23_AdrStoreBR,p,len);
			break;
		}
	}
}
//-------------------------���ݶ�ȡ���洢----------------------------
//---
/*
	��������void Ohrd23_ReadStoreData(u8 address,u8 *p,u8 len)
	������	��ȡ����
*/
void Ohrd23_ReadStoreData(u8 address,u8 *p,u8 len)
{
	W25Xxx_ReadData(p,OHRD23_AdrStoreBase+address,len);
}
//---
/*
	��������void Ohrd23_StoreData(u8 address,u8 *p,u8 len)
	�������ú���Ϊ�洢OHRD23���ò�������
	˵������ȡ����
				��������
				�޸�����
				��������
*/
void Ohrd23_StoreData(u8 address,u8 *p,u8 len)
{
	u8 buffer[OHRD23_AdrStoreEnd],cnt;
	W25Xxx_ReadData(buffer,OHRD23_AdrStoreBase,OHRD23_AdrStoreEnd);//���������ò���
	W25Xxx_EraseSector(OHRD23_AdrStoreBase);//��������
	for(cnt=0;cnt<len;cnt++)
	{
		buffer[address++] = *p;
		p++;
	}
	W25Xxx_PageWrite(buffer,OHRD23_AdrStoreBase,OHRD23_AdrStoreEnd);//��������
}
/*
	��������void Ohrd23_RestoreFactorySetting(void)
	�������ָ��������ã�������ѹ���ʣ��������ʣ����߷�ʽ��ͨѶ��ַ��ͨѶ������
*/
void Ohrd23_RestoreFactorySetting(void)
{
	u8 buffer[OHRD23_AdrStoreEnd],cnt=0;
	TwoBytes change;
	W25Xxx_EraseSector(OHRD23_AdrStoreBase);//��������
	Ohrd23Data.Conf.VT = 1;
	Ohrd23Data.Conf.IT = 40;
	Ohrd23Data.Conf.LM = 0;
	Ohrd23Data.Conf.ADR = 0x01;
	Ohrd23Data.Conf.BR = 3;
	change.S16Data = Ohrd23Data.Conf.VT;
	buffer[cnt++] = change.U8Data[0];
	buffer[cnt++] = change.U8Data[1];
	change.S16Data = Ohrd23Data.Conf.IT;
	buffer[cnt++] = change.U8Data[0];
	buffer[cnt++] = change.U8Data[1];
	change.S16Data = Ohrd23Data.Conf.LM;
	buffer[cnt++] = change.U8Data[0];
	buffer[cnt++] = change.U8Data[1];
	change.S16Data = Ohrd23Data.Conf.ADR;
	buffer[cnt++] = change.U8Data[0];
	buffer[cnt++] = change.U8Data[1];
	change.S16Data = Ohrd23Data.Conf.BR;
	buffer[cnt++] = change.U8Data[0];
	buffer[cnt++] = change.U8Data[1];
	W25Xxx_PageWrite(buffer,OHRD23_AdrStoreBase,cnt);//��������
}
//---
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Ohrd23_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪOHRD23�ļ���ASCII�����ļ�
:000|85|001
*/
void Ohrd23_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:
		{//��������������
			Ohrd23_ReadBase();
			printf("��������������\r\n");
			break;
		}
		case 2:
		{//����������
			Ohrd23_ReadEnergy();
			printf("����������\r\n");
			break;
		}
		case 3:
		{//����������
			Ohrd23_ReadConf();
			printf("�����ò���\r\n");
			break;
		}
		//---
		case 101://:000|85|101|0001
		{//���õ�ѹ����
			u16 rate = Calculate_CharToU16(p+4,4);//��������
			if(rate<=1200)
			{
				Ohrd23_SetVT(rate);
				printf("���õ�ѹ����Ϊ%u\r\n",rate);
			}
			else
			{
				printf("����������õ�ѹ���ʷ�ΧΪ0~1200\r\n");
			}
			break;
		}
		case 102://:000|85|102|0040
		{//���õ�������
			u16 rate = Calculate_CharToU16(p+4,4);//��������
			if(rate<=1200)
			{
				Ohrd23_SetIT(rate);
				printf("���õ�������Ϊ%u\r\n",rate);
			}
			else
			{
				printf("����������õ������ʷ�ΧΪ0~1200\r\n");
			}
			break;
		}
		case 103:
		{//���ý���ģʽ
			if(*(p+4) == '0')
			{
				Ohrd23_SetLM(0);
				printf("����ģʽ������Ϊ0=��������\r\n");
			}
			else if(*(p+4) == '1')
			{
				Ohrd23_SetLM(1);
				printf("����ģʽ������Ϊ1=��������\r\n");
			}
			else if(*(p+4) == '2')
			{
				Ohrd23_SetLM(2);
				printf("����ģʽ2=3V3A\r\n");
			}
			else
			{
				printf("������󣬽���ģʽ������Ϊ0=�������ߣ�1=�������ߣ�2=3V3A\r\n");
			}
			break;
		}
		case 104:
		{//���õ�ַ
			u8 address = Calculate_CharToU16(p+4,3);//��������
			if((address>0) && (address<253))
			{
				Ohrd23_SetAddress(address);
				printf("ͨѶ��ַ����Ϊ%u\r\n",address);
			}
			else
			{
				printf("�������ͨѶ��ַ���÷�ΧΪ1~253\r\n");
			}
			break;
		}
		case 105:
		{//���ò�����
			u8 baudrate = Calculate_CharToU16(p+4,1);//��������
			if(baudrate<5)
			{
				Ohrd23_SetBaudRate(baudrate);
				printf("ͨѶ����������Ϊ%u\r\n",(u16)(1200*pow(2,baudrate)));
			}
			else
			{
				printf("�������ͨѶ���������÷�ΧΪ0-4\r\n");
			}
			break;
		}
		case 106:
		{//�������ò��� :000|85|106|1000|1000|0|004|3
			OHRD23_ConfigurationStructure buffer;
			buffer.VT = Calculate_CharToU16(p+4,4);
			buffer.IT = Calculate_CharToU16(p+9,4);
			buffer.LM = Calculate_CharToU16(p+14,1);
			buffer.ADR = Calculate_CharToU16(p+16,3);
			buffer.BR = Calculate_CharToU16(p+20,1);
			if(buffer.VT<1201 && buffer.IT<1201 && buffer.LM<3 && buffer.ADR>1 && buffer.ADR<253 && buffer.BR<5)
			{
				Ohrd23_SetConfiguration(&buffer);
				Ohrd23Data.Conf = buffer;
				printf("���õ�ѹ����Ϊ%d,",buffer.VT);
				printf("��������Ϊ%d,",buffer.IT);
				printf("���߷�ʽΪ%d,",buffer.LM);
				printf("ͨѶ��ַΪ%d,",buffer.ADR);
				printf("ͨѶ������Ϊ%d\r\n",buffer.BR);
			}
			else
			{
				printf("�������\r\n");
				printf("��ѹ���ʷ�ΧΪ0~1200,");
				printf("�������ʷ�ΧΪ0~1200,");
				printf("���߷�ʽ��ΧΪ0~2,");
				printf("ͨѶ��ַ��ΧΪ1~253,");
				printf("ͨѶ�����ʷ�ΧΪ0~4\r\n");
			}
			break;
		}
		case 201:
		{//��ȡ�洢��EEPROM�����ò���
//			OHRD23_ConfigurationStructure buffer;
//			u8 *pdata = (u8*)&buffer;	
//			Ohrd23_ReadStoreData(OHRD23_AdrConfBase,pdata,OHRD23_AdrStoreEnd);
//			printf("��ѹ����Ϊ%d,",buffer.VT);
//			printf("��������Ϊ%d,",buffer.IT);
//			printf("���߷�ʽΪ%d,",buffer.LM);
//			printf("ͨѶ��ַΪ%d,",buffer.ADR);
//			printf("ͨѶ������Ϊ%d\r\n",buffer.BR);
			break;
		}
		case 202:
		{//��ȡ�洢��EEPROM�ĵ�ѹ����
			TwoBytes two;
			Ohrd23_Operation(2,two.U8Data,2);
			printf("������EEPROM�ĵ�ѹ����Ϊ%u\r\n",two.U16Data);
			break;
		}
		case 203:
		{//��ȡ�洢��EEPROM�ĵ�������
			TwoBytes two;
			Ohrd23_Operation(3,two.U8Data,2);
			printf("������EEPROM�ĵ�������Ϊ%d\r\n",two.U16Data);
			break;
		}
		case 204:
		{//��ȡ�洢��EEPROM�Ľ��߷�ʽ
			u8 bf;
			Ohrd23_Operation(4,&bf,1);
			printf("������EEPROM�Ľ��߷�ʽΪ%d\r\n",bf);
			break;
		}
		case 205:
		{//��ȡ�洢��EEPROM��ͨѶ��ַ
			u8 bf;
			Ohrd23_Operation(5,&bf,1);
			printf("������EEPROM��ͨѶ��ַΪ%d\r\n",bf);
			break;
		}
		case 206:
		{//��ȡ�洢��EEPROM��ͨѶ������
			TwoBytes two;
			Ohrd23_Operation(6,two.U8Data,2);
			printf("������EEPROM��ͨѶ������Ϊ%d\r\n",two.U16Data);
			break;
		}
		case 301:
		{//���ô洢��EEPROM�����ò���
			OHRD23_ConfigurationStructure buffer;
			buffer.VT = Calculate_CharToU16(p+4,4);
			buffer.IT = Calculate_CharToU16(p+9,4);
			buffer.LM = Calculate_CharToU16(p+14,1);
			buffer.ADR = Calculate_CharToU16(p+16,3);
			buffer.BR = Calculate_CharToU16(p+20,1);
			if(buffer.VT<1201 && buffer.IT<1201 && buffer.LM<3 && buffer.ADR>1 && buffer.ADR<253 && buffer.BR<5)
			{
				u8 *pdata = (u8*)&buffer;
				Ohrd23_StoreData(OHRD23_AdrConfBase,pdata,OHRD23_AdrStoreEnd);//�洢����	
				printf("���õ�ѹ����Ϊ%d,",buffer.VT);
				printf("��������Ϊ%d,",buffer.IT);
				printf("���߷�ʽΪ%d,",buffer.LM);
				printf("ͨѶ��ַΪ%d,",buffer.ADR);
				printf("ͨѶ������Ϊ%d\r\n",buffer.BR);
			}
			else
			{
				printf("�������\r\n");
				printf("��ѹ���ʷ�ΧΪ0~1200,");
				printf("�������ʷ�ΧΪ0~1200,");
				printf("���߷�ʽ��ΧΪ0~2,");
				printf("ͨѶ��ַ��ΧΪ1~253,");
				printf("ͨѶ�����ʷ�ΧΪ0~4\r\n");
			}
			break;
		}
		case 302:
		{//���ô洢��EEPROM�ĵ�ѹ����
			TwoBytes two;
			two.U16Data = Calculate_CharToU16(p+4,4);//��������
			if(two.U16Data<=1200)
			{
				Ohrd23_Operation(102,two.U8Data,2);
				printf("���õ�ѹ����Ϊ%u\r\n",two.U16Data);
			}
			else
			{
				printf("����������õ�ѹ���ʷ�ΧΪ0~1200\r\n");
			}
			break;
		}
		case 303:
		{//���ô洢��EEPROM�ĵ�������
			TwoBytes two;
			two.U16Data = Calculate_CharToU16(p+4,4);//��������
			if(two.U16Data<=1200)
			{
				Ohrd23_Operation(103,two.U8Data,2);
				printf("���õ�������Ϊ%u\r\n",two.U16Data);
			}
			else
			{
				printf("����������õ������ʷ�ΧΪ0~1200\r\n");
			}
			break;
		}
		case 304:
		{//���ô洢��EEPROM�Ľ��߷�ʽ
			u8 bf = Calculate_CharToU16(p+4,1);//��������
			if(bf<3)
			{
				Ohrd23_Operation(104,&bf,1);
				printf("���ý���ģʽΪ%u\r\n",bf);
			}
			else
			{
				printf("������󣬽���ģʽ������Ϊ0=�������ߣ�1=�������ߣ�2=3V3A\r\n");
			}
			break;
		}
		case 305:
		{//���ô洢��EEPROM��ͨѶ��ַ
			u8 bf = Calculate_CharToU16(p+4,3);//��������
			if((bf>0) && (bf<253))
			{
				Ohrd23_Operation(105,&bf,1);
				printf("ͨѶ��ַ����Ϊ%u\r\n",bf);
			}
			else
			{
				printf("�������ͨѶ��ַ���÷�ΧΪ1~253\r\n");
			}
			break;
		}
		case 306:
		{//���ô洢��EEPROM�Ĳ�����
			TwoBytes two;
			two.U16Data = Calculate_CharToU16(p+4,5);//��������
			if((two.U16Data==1200) || (two.U16Data==2400) || (two.U16Data==4800) || (two.U16Data==9600) || (two.U16Data==19200))
			{
				Ohrd23_Operation(106,two.U8Data,2);
				printf("ͨѶ����������Ϊ%u\r\n",two.U16Data);
			}
			else
			{
				printf("�������ͨѶ���������÷�ΧΪ1200,2400,4800,9600,19200\r\n");
			}
			break;
		}
		case 500:
		{
			Ohrd23Data.RunMode = 201;
			break;
		}
		case 900:
		{
			printf("����OHRD23\r\n");
			Ohrd23_SearchOhrd23();
			break;
		}
		default:
		{
			printf("����OHRD23�������\r\n");
			break;
		}
	}
}
//---
/*
	�������� 	void Ohrd23_DebugRTU((u8 *p)
	������		�ú���ΪOHRD23�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*/
void Ohrd23_DebugRTU(u8 *p,u8 len)
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
	��������void Ohrd23_Receive(u8 *p,u8 len)
	�������������Խ��յ�Ohrd23��һ֡����
	˵��������ú���֮ǰ����Ҫ�Ƚ���CRCУ��
*/
void Ohrd23_Receive(u8 *p,u8 len)
{
	switch(Ohrd23Data.CollectMode)//�鿴���ĸ�������
	{
		case 0x01:
		{//��������������
			if(*(p+1) == 0x03)
			{//��ȡ�ɹ�
				Ohrd23_BaseChange(p+3,&Ohrd23Data.Base);//���ݴ���
			}
			else if(*(p+1) == 0x83)
			{//��ȡ����
				printf("��ȡ�������ݴ���\r\n");
			}
			break;
		}
		case 0x02:
		{//�����ܲ���
			if(*(p+1) == 0x03)
			{//��ȡ�ɹ�
				Ohrd23_EnergyChange(p+3,&Ohrd23Data.Energy);//���ݴ���
			}
			else if(*(p+1) == 0x83)
			{//��ȡ����
				printf("��ȡ�������ݴ���\r\n");
			}
			break;
		}
		case 0x03:
		{//�����ò���
			if(*(p+1) == 0x03)
			{//��ȡ�ɹ�
				Ohrd23_ConfigurationChange(p+3,&Ohrd23Data.Conf);//���ݴ���
			}
			else if(*(p+1) == 0x83)
			{//��ȡ����
				printf("��ȡ�������ݴ���\r\n");
			}
			break;
		}
		case 0x81:
		{//���õ�ѹ����
			if(*(p+1) == 0x06)
			{//���óɹ�
				TwoBytes buffer;
				buffer.U8Data[0] = *(p+5);
				buffer.U8Data[1] = *(p+4);
//				u8 *pdata = (u8*)&(Ohrd23Data.Conf.VT);
				Ohrd23Data.Conf.VT = buffer.S16Data;
				Ohrd23_StoreData(OHRD23_AdrStoreVT,buffer.U8Data,2);//�洢����
				printf("���õ�ѹ����Ϊ%d\r\n",Ohrd23Data.Conf.VT);
			}
			else if(*(p+1) == 0x86)
			{//����ʧ��
				printf("��ѹ����ʧ��\r\n");
			}
			break;
		}
		case 0x82:
		{//���õ�������
			if(*(p+1) == 0x06)
			{//���óɹ�
				TwoBytes buffer;
				buffer.U8Data[0] = *(p+5);
				buffer.U8Data[1] = *(p+4);
//				u8 *pdata = (u8*)&Ohrd23Data.Conf.IT;
				Ohrd23Data.Conf.IT = buffer.S16Data;
				Ohrd23_StoreData(OHRD23_AdrStoreIT,buffer.U8Data,2);//�洢����
				printf("���õ�������Ϊ%d\r\n",Ohrd23Data.Conf.IT);
			}
			else if(*(p+1) == 0x86)
			{//����ʧ��
				printf("���õ�������ʧ��\r\n");
			}
			break;
		}
		case 0x83:
		{//���ý���ģʽ
			if(*(p+1) == 0x06)
			{//���óɹ�
				TwoBytes buffer;
				buffer.U8Data[0] = *(p+5);
				buffer.U8Data[1] = *(p+4);
				Ohrd23Data.Conf.LM = buffer.S16Data;
//				u8 *pdata = (u8*)&Ohrd23Data.Conf.LM;
				Ohrd23_StoreData(OHRD23_AdrStoreLM,buffer.U8Data,2);//�洢����
				printf("���ý���ģʽΪ%d\r\n",Ohrd23Data.Conf.LM);
			}
			else if(*(p+1) == 0x86)
			{//����ʧ��
				printf("���ý���ģʽʧ��\r\n");
			}
			break;
		}
		case 0x84:
		{//���õ�ַ
			if(*(p+1) == 0x06)
			{//���óɹ�
				TwoBytes buffer;
				buffer.U8Data[0] = *(p+5);
				buffer.U8Data[1] = *(p+4);
//				u8 *pdata = (u8*)&Ohrd23Data.Conf.ADR;
				Ohrd23Data.Conf.ADR = buffer.S16Data;
				Ohrd23_StoreData(OHRD23_AdrStoreADR,buffer.U8Data,2);//
				printf("�����µ�ַΪ%d\r\n",Ohrd23Data.Conf.ADR);
			}
			else if(*(p+1) == 0x86)
			{//����ʧ��
				printf("��ַ����ʧ��\r\n");
			}
			break;
		}
		case 0x85:
		{//���ò�����
			if(*(p+1) == 0x06)
			{//���óɹ�
				TwoBytes buffer;
				buffer.U8Data[0] = *(p+5);
				buffer.U8Data[1] = *(p+4);
//				u8 *pdata = (u8*)&Ohrd23Data.Conf.BR;
				Ohrd23Data.Conf.BR = buffer.S16Data;
				Ohrd23_StoreData(OHRD23_AdrStoreBR,buffer.U8Data,2);//
				printf("���ò�����Ϊ%d\r\n",(u16)(1200*pow(2,Ohrd23Data.Conf.BR)));
			}
			else if(*(p+1) == 0x86)
			{//����ʧ��
				printf("��ַ����ʧ��\r\n");
			}
			break;
		}
		case 0x86:
		{//�������ò���
			if(*(p+1) == 0x10)
			{//���óɹ�
				u8 *pdata = (u8*)&Ohrd23Data.Conf.VT;
				Ohrd23_StoreData(OHRD23_AdrConfBase,pdata,OHRD23_AdrStoreEnd);//�洢����
				printf("���óɹ�\r\n");
			}
			else if(*(p+1) == 0x90)
			{//����ʧ�ܣ���ȡ�洢������
//				u8 *pdata = (u8*)&Ohrd23Data.Conf.VT;
				Ohrd23_ReadStoreData(OHRD23_AdrConfBase,(u8*)&Ohrd23Data.Conf.VT,OHRD23_AdrStoreEnd);	
				printf("����ʧ��\r\n");
			}
			break;
		}
	}
	Ohrd23Data.CollectMode = 0;
	Ohrd23Data.Flag &= ~(B0|B1);
	Ohrd23Data.ErrorCnt = 0;
}
//---





































