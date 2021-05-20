/*
	���ļ�Ϊ����Ŀ��PAC1934оƬ��غ���
	���庯������
*/

#include "PAC193X.h"

PAC193X_DataStructure PAC193XData;
/*
	��������void PAC193X_Main(void)
	������	��Ļ��ʾ������
*/
void PAC193X_Main(void)
{
	//���ݲɼ�
	if((PAC193XData.CollectTimer == 0) && (PAC193XData.WorkMode == 0))
	{
		u8 i;
		PAC193XData.CollectTimer = 50;
		PAC193X_DelayTime(2);
		PAC193XData.WorkMode = 1;
		for(i=0;i<4;i++)
		{
			TwoBytes two;
			two.U8Data[1] = PAC193XData.VBuffer[i*2];
			two.U8Data[0] = PAC193XData.VBuffer[i*2+1];
			PAC193XData.Channel[i] = two.U16Data;
			PAC193XData.Voltage[i] = 32.0*PAC193XData.Channel[i] / 65535.0;
		}
		PAC193X_GetData()->Facility[0].ADValue = PAC193XData.Channel[1];
		PAC193X_GetData()->Facility[1].ADValue = PAC193XData.Channel[0];
		PAC193X_GetData()->Facility[2].ADValue = PAC193XData.Channel[2];
//		PAC193XData.ADValue[3] = PAC193XData.Channel[3];
		PAC193X_CalMapValue(&PAC193X_GetData()->Facility[0]);
		PAC193X_CalMapValue(&PAC193X_GetData()->Facility[1]);
		PAC193X_CalMapValue(&PAC193X_GetData()->Facility[2]);
	}
}
//---
/*
	��������void PAC193X_Work(void)
	�������ú���Ϊ����ϵͳ��PAC1934оƬͨѶ
*/
void PAC193X_Work(void)
{
	switch(PAC193XData.WorkMode)
	{
		case 1:
			{//������ʼ�ź�
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 201;
			PAC193XData.OldWorkMode = 2;
			break;
		}
		case 2:
		{//���͵�ַ��д
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer = PAC193X_ADDRESS;
			PAC193XData.WorkMode = 231;
			PAC193XData.OldWorkMode = 11;
			PAC193X_DelayTime(2);
			break;
		}
		case 3:
		{//���͵�ַ����
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer = PAC193X_ADDRESS+1;
			PAC193XData.WorkMode = 231;
			PAC193XData.OldWorkMode = 0;
			PAC193X_DelayTime(2);
			break;
		}
		//REFRESH����0x00
		case 11:
		{
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer = 0x00;
			PAC193XData.WorkMode = 231;
			PAC193XData.OldWorkMode = 12;
			PAC193X_DelayTime(2);
			break;
		}
		//REFRESH_G����0x1F
		case 12:
		{
			PAC193XData.WorkMode = 211;
			PAC193XData.OldWorkMode = 13;
			PAC193X_DelayTime(2);
			break;
		}
		case 13:
		{
			PAC193XData.WorkMode = 51;
			PAC193X_DelayTime(5000);
			break;
		}
		//������ȡ4ͨ����ѹ����
		case 51:
		{
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 201;
			PAC193XData.OldWorkMode = 52;
			break;
		}
		case 52:
		{
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer = PAC193X_ADDRESS;
			PAC193XData.WorkMode = 231;
			PAC193XData.OldWorkMode = 53;
			PAC193X_DelayTime(2);
			break;
		}
		case 53:
		{
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer = 0x0F;//��ַ
			PAC193XData.WorkMode = 231;
			PAC193XData.OldWorkMode = 54;
			PAC193X_DelayTime(2);
			break;
		}
		case 54:
		{//������ʼ�ź�
			PAC193X_SCL(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 201;
			PAC193XData.OldWorkMode = 55;
			break;
		}
		case 55:
		{
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer = PAC193X_ADDRESS+1;
			PAC193X_DelayTime(2);
			PAC193XData.CntByte = 0;
			PAC193XData.WorkMode = 231;
			PAC193XData.OldWorkMode = 56;
			break;
		}
		case 56:
		{//��ʼ��ȡ����
			PAC193XData.CntBit=0;
			PAC193XData.RTxBuffer=0;
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 241;
			PAC193XData.OldWorkMode = 57;
			break;
		}
		case 57:
		{
			PAC193XData.VBuffer[PAC193XData.CntByte] = PAC193XData.RTxBuffer;
			PAC193XData.CntByte++;
			if(PAC193XData.CntByte < 8) 
			{
				PAC193XData.WorkMode = 243;
				PAC193XData.OldWorkMode = 56;
			}
			else 
			{
				PAC193XData.WorkMode = 247;
				PAC193XData.OldWorkMode = 58;
			}
			PAC193X_DelayTime(2);
			break;
		}
		case 58:
		{
			PAC193XData.WorkMode = 211;
			PAC193XData.OldWorkMode = 0;
			PAC193X_DelayTime(2);
			break;
		}
		//���������źţ���֮ǰ����֤SCL=1��SDA=1���˳�ʱSCL=0��SDA=0
		case 201:
		{
			PAC193X_SDA(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 202;
			break;
		}
		case 202:
		{
			PAC193X_SCL(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = PAC193XData.OldWorkMode;
			break;
		}
		//���ͽ����ź�,��֮ǰ����֤SCL=0��SDA����Ҫ���˳�ʱSCL=1��SDA=1
		case 211:
		{
			PAC193X_SDA(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 212;
			break;
		}
		case 212:
		{
			PAC193X_SCL(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 213;
			break;
		}
		case 213:
		{
			PAC193X_SDA(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = PAC193XData.OldWorkMode;
			break;
		}
		//����Ӧ���źţ���֮ǰ��֤SCL=0��SDA����Ҫ���˳�ʱSCL=0��SDA=0
		case 221:
		{
			PAC193X_SDA(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 222;
			break;
		}
		case 222:
		{
			PAC193X_SCL(1)
			PAC193X_DelayTime(3);
			PAC193XData.WorkMode = 223;
			break;
		}
		case 223:
		{
			PAC193X_SCL(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 224;
			break;
		}
		case 224:
		{
			PAC193X_SDA(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = PAC193XData.OldWorkMode;
			break;
		}
		//����һ���ֽ����ݣ���֮ǰ��֤SCL=0��SDA����Ҫ���˳�ʱSCL=0,SDA=1,AC193XData.CntBit=0
		//Ҫ���͵����ݻ�����ΪAC193XData.RTxBuffer
		case 231:
		{
			if(PAC193XData.RTxBuffer & B7) 
			{
				PAC193X_SDA(1)
			}
			else
			{
				PAC193X_SDA(0)
			}
			PAC193XData.RTxBuffer = PAC193XData.RTxBuffer<<1;	
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 232;	
			break;
		}
		case 232:
		{
			PAC193X_SCL(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 233;
			break;
		}
		case 233:
		{
			PAC193X_SCL(0)
			PAC193XData.CntBit++;
			PAC193X_DelayTime(2);
			if(PAC193XData.CntBit<8) PAC193XData.WorkMode = 231;
			else PAC193XData.WorkMode = 234;
			break;
		}
		case 234:
		{
			PAC193X_SDA(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 235;
			break;
		}
		case 235:
		{//�ж�Ӧ��
			PAC193X_SCL(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 236;
			break;
		}
		case 236:
		{
			PAC193X_SCL(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = PAC193XData.OldWorkMode;
			break;
		}
		//����һ���ֽ����ݣ���֮ǰ��֤SCL=0��SDA=1����Ҫ�󣬳�ʱSCL=0,SDA=1,PAC193XData.CntBit=0,PAC193XData.RTxBuffer=0
		//Ҫ���յ����ݻ�����ΪPAC193XData.RTxBuffer
		case 241:
		{
			PAC193X_SCL(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 242;
			break;
		}
		case 242:
		{
			if(PAC193X_SDAIN()) PAC193XData.RTxBuffer |= B0;
			PAC193X_SCL(0)
			PAC193XData.CntBit++;
			PAC193X_DelayTime(2);
			if(PAC193XData.CntBit<8)
			{
				PAC193XData.WorkMode = 241;
				PAC193XData.RTxBuffer = PAC193XData.RTxBuffer<<1;
			}
			else PAC193XData.WorkMode = PAC193XData.OldWorkMode;
			break;
		}
		case 243:
		{//Ӧ��0
			PAC193X_SDA(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 244;
			break;
		}
		case 244:
		{
			PAC193X_SCL(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 245;
			break;
		}
		case 245:
		{
			PAC193X_SCL(0)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 246;
			break;
		}
		case 246:
		{
//			printf("���յ�����Ϊ��0x%X\r\n",PAC193XData.RTxBuffer);
			PAC193X_SDA(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = PAC193XData.OldWorkMode;
			break;
		}
		case 247:
		{//Ӧ��1
			PAC193X_SDA(1)
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 244;
			break;
		}
		
		
		case 250:
		{
			PAC193XData.WorkMode = 211;
			PAC193XData.OldWorkMode = 0;
			PAC193X_DelayTime(2);
			break;
		}
	}
}
//---
/*
	��������void PAC193X_Operation(u16 mode,u8 *p,u8 len)
	�������ú���ΪPAC193X.c�����ݲ���
*/
void PAC193X_Operation(u16 mode,u8 *p,u8 len)
{
	switch(mode)
	{
		//AD1������ȡ
		case 2:
		{//��AD1ԭ��ֵ
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD1Orgin,p,2);
			break;
		}
		case 3:
		{//��AD1�˵�ֵ
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD1Endpoint,p,2);
			break;
		}
		case 4:
		{//��AD1�ο�ֵ
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD1Refer,p,2);
			break;
		}
		case 5:
		{//��AD1�豸ӳ��
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD1Map,p,1);
			break;
		}
		//AD2������ȡ
		case 22:
		{//��AD2ԭ��ֵ
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD2Orgin,p,2);
			break;
		}
		case 23:
		{//��AD2�˵�ֵ
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD2Endpoint,p,2);
			break;
		}
		case 24:
		{//��AD1�ο�ֵ
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD2Refer,p,2);
			break;
		}
		case 25:
		{//��AD2�豸ӳ��
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD2Map,p,1);
			break;
		}
		//AD3������ȡ
		case 42:
		{//��AD3ԭ��ֵ
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD3Orgin,p,2);
			break;
		}
		case 43:
		{//��AD3�˵�ֵ
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD3Endpoint,p,2);
			break;
		}
		case 44:
		{//��AD3�ο�ֵ
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD3Refer,p,2);
			break;
		}
		case 45:
		{//��AD3�豸ӳ��
			PAC193X_ReadStoreData(PAC193X_AdrStoreAD3Map,p,1);
			break;
		}
		//AD1����д��
		case 102:
		{//д��AD1ԭ��ֵ
			PAC193X_StoreData(PAC193X_AdrStoreAD1Orgin,p,2);
			break;
		}
		case 103:
		{//д��AD1�˵�ֵ
			PAC193X_StoreData(PAC193X_AdrStoreAD1Endpoint,p,2);
			break;
		}
		case 104:
		{//д��AD1�ο�ֵ
			PAC193X_StoreData(PAC193X_AdrStoreAD1Refer,p,2);
			break;
		}
		case 105:
		{//д��AD1�豸ӳ��
			PAC193X_StoreData(PAC193X_AdrStoreAD1Map,p,1);
			break;
		}
		//AD2����д��
		case 122:
		{//д��AD2ԭ��ֵ
			PAC193X_StoreData(PAC193X_AdrStoreAD2Orgin,p,2);
			break;
		}
		case 123:
		{//д��AD2�˵�ֵ
			PAC193X_StoreData(PAC193X_AdrStoreAD2Endpoint,p,2);
			break;
		}
		case 124:
		{//д��AD2�ο�ֵ
			PAC193X_StoreData(PAC193X_AdrStoreAD2Refer,p,2);
			break;
		}
		case 125:
		{//д��AD2�豸ӳ��
			PAC193X_StoreData(PAC193X_AdrStoreAD2Map,p,1);
			break;
		}
		//AD3����д��
		case 142:
		{//д��AD3ԭ��ֵ
			PAC193X_StoreData(PAC193X_AdrStoreAD3Orgin,p,2);
			break;
		}
		case 143:
		{//д��AD3�˵�ֵ
			PAC193X_StoreData(PAC193X_AdrStoreAD3Endpoint,p,2);
			break;
		}
		case 144:
		{//д��AD3�ο�ֵ
			PAC193X_StoreData(PAC193X_AdrStoreAD3Refer,p,2);
			break;
		}
		case 145:
		{//д��AD3�豸ӳ��
			PAC193X_StoreData(PAC193X_AdrStoreAD3Map,p,1);
			break;
		}
	}
}
//---
/*
	��������void PAC193X_Operation(u16 mode,u8 *p,u8 len)
	������PAC193X.c�ļ������ݲ���
*/
//---
/*
	��������void PAC193X_Timer(void)
	������screen.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void PAC193X_Timer(void)
{
	if(PAC193XData.Timer != 0)PAC193XData.Timer--;
	if(PAC193XData.CollectTimer != 0)PAC193XData.CollectTimer--;
}
//---
/*
	��������void PAC193X_Init(void)
	������	screen.c�ļ���ʼ������
*/
void PAC193X_Init(void)
{
	PAC193X_InitGPIO();
	PAC193X_TimerInit();
	PAC193X_InitData();
	//�����ɼ�
	PAC193X_DelayTime(2);
	PAC193XData.WorkMode = 1;
	PAC193XData.Timer = 100;
	while(PAC193XData.Timer);
}
//---
/*
	��������void PAC193X_InitData(void)
*/
void PAC193X_InitData(void)
{
	u8 bf[PAC193X_AdrStoreEND];
	u16 b,c;
	TwoBytes two;
	//AD1ͨ�����ݳ�ʼ��
	PAC193X_ReadStoreData(PAC193X_AdrStoreStart,bf,PAC193X_AdrStoreEND);
	two.U8Data[0] = bf[PAC193X_AdrStoreAD1Orgin];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD1Orgin+1];
	PAC193XData.Facility[0].Orgin = two.U16Data;
	two.U8Data[0] = bf[PAC193X_AdrStoreAD1Endpoint];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD1Endpoint+1];
	b = two.U16Data;
	two.U8Data[0] = bf[PAC193X_AdrStoreAD1Refer];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD1Refer+1];
	c = two.U16Data;
	PAC193XData.Facility[0].Rate = (float)c/((s32)b-PAC193XData.Facility[0].Orgin);
	if((bf[PAC193X_AdrStoreAD1Map] == 1) || (bf[PAC193X_AdrStoreAD1Map] == 2) || (bf[PAC193X_AdrStoreAD1Map] == 3))
		PAC193XData.Facility[0].Map = bf[PAC193X_AdrStoreAD1Map];
	else 
	{
		two.U8Data[0] = 1;
		PAC193X_Operation(105,two.U8Data,1);
		PAC193XData.Facility[0].Map = 1;
	}
	//AD2ͨ�����ݳ�ʼ��
	two.U8Data[0] = bf[PAC193X_AdrStoreAD2Orgin];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD2Orgin+1];
	PAC193XData.Facility[1].Orgin = two.U16Data;
	two.U8Data[0] = bf[PAC193X_AdrStoreAD2Endpoint];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD2Endpoint+1];
	b = two.U16Data;
	two.U8Data[0] = bf[PAC193X_AdrStoreAD2Refer];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD2Refer+1];
	c = two.U16Data;
	PAC193XData.Facility[1].Rate = (float)c/((s32)b-PAC193XData.Facility[1].Orgin);
	if((bf[PAC193X_AdrStoreAD2Map] == 1) || (bf[PAC193X_AdrStoreAD2Map] == 2) || (bf[PAC193X_AdrStoreAD2Map] == 3))
		PAC193XData.Facility[1].Map = bf[PAC193X_AdrStoreAD2Map];
	else 
	{
		two.U8Data[0] = 1;
		PAC193X_Operation(125,two.U8Data,1);
		PAC193XData.Facility[1].Map = 1;
	}
	//AD3ͨ�����ݳ�ʼ��
	two.U8Data[0] = bf[PAC193X_AdrStoreAD3Orgin];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD3Orgin+1];
	PAC193XData.Facility[2].Orgin = two.U16Data;
	two.U8Data[0] = bf[PAC193X_AdrStoreAD3Endpoint];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD3Endpoint+1];
	b = two.U16Data;
	two.U8Data[0] = bf[PAC193X_AdrStoreAD3Refer];
	two.U8Data[1] = bf[PAC193X_AdrStoreAD3Refer+1];
	c = two.U16Data;
	PAC193XData.Facility[2].Rate = (float)c/((s32)b-PAC193XData.Facility[2].Orgin);
	if((bf[PAC193X_AdrStoreAD3Map] == 1) || (bf[PAC193X_AdrStoreAD3Map] == 2) || (bf[PAC193X_AdrStoreAD3Map] == 3))
		PAC193XData.Facility[2].Map = bf[PAC193X_AdrStoreAD3Map];
	else 
	{
		two.U8Data[0] = 1;
		PAC193X_Operation(145,two.U8Data,1);
		PAC193XData.Facility[2].Map = 1;
	}
	PAC193XData.Address = PAC193X_ADDRESS;
	
	printf("\r\n");
	for(b=0;b<3;b++)
	{
		printf("AD%uͨ��ԭ��ֵ%u,����%f,ӳ��",b+1,PAC193XData.Facility[b].Orgin,PAC193XData.Facility[b].Rate);
		if(PAC193XData.Facility[b].Map == 1)	printf("��ת���ݲɼ�\r\n");
		else if(PAC193XData.Facility[b].Map == 2)	printf("������ݲɼ�\r\n");
		else if(PAC193XData.Facility[b].Map == 3)	printf("�������ݲɼ�\r\n");
	}
}
//---
/*
	��������void PAC193X_CalMapValue(PAC193X_TowerStructure *p)
	����������AD�ɼ���ӳ����󣬼���ӳ��ֵ
*/
void PAC193X_CalMapValue(PAC193X_TowerStructure *p)
{
	if(p->Map == 1)
	{//�����ת
		p->MapValue = (p->ADValue - p->Orgin) * p->Rate;
		while(p->MapValue > 360)
			p->MapValue -=360;
		while(p->MapValue < 0)
			p->MapValue +=360;
		Tower_GetSlewingData()->Rotation = p->MapValue;
	}
	else if(p->Map == 2)
	{//������
		p->MapValue = (p->ADValue - p->Orgin) * p->Rate;
//		while(p->MapValue > 360)
//			p->MapValue -=360;
//		while(p->MapValue < 0)
//			p->MapValue +=360;
		Tower_GetTrolleyingData()->Range = p->MapValue*1000;
	}
	else if(p->Map == 3)
	{//�������
		p->MapValue = (p->ADValue - p->Orgin) * p->Rate;
//		while(p->MapValue > 360)
//			p->MapValue -=360;
//		while(p->MapValue < 0)
//			p->MapValue +=360;
		Tower_GetHoistingData()->HookHeight =  p->MapValue*1000;
	}
	
//	PAC193XData.Facility[0].MapValue = ((s32)PAC193XData.ADValue[0] - PAC193XData.Facility[0].Orgin)* PAC193XData.Facility[0].Rate;
//	PAC193XData.Facility[1].MapValue = ((s32)PAC193XData.ADValue[1] - PAC193XData.Facility[1].Orgin)* PAC193XData.Facility[1].Rate;
//	PAC193XData.Facility[2].MapValue = ((s32)PAC193XData.ADValue[2] - PAC193XData.Facility[2].Orgin)* PAC193XData.Facility[2].Rate;
//	//��ת
//	if(PAC193XData.Facility[0].Map == 1)
//		Tower_GetSlewingData()->Rotation = PAC193XData.Facility[0].MapValue;
//	else if(PAC193XData.Facility[1].Map == 1)
//		Tower_GetSlewingData()->Rotation = PAC193XData.Facility[0].MapValue;
//	else if(PAC193XData.Facility[2].Map == 1)
//		Tower_GetSlewingData()->Rotation = PAC193XData.Facility[0].MapValue;
//	//���
//	if(PAC193XData.Facility[0].Map == 2)
//		Tower_GetTrolleyingData()->Range = PAC193XData.Facility[1].MapValue;
//	else if(PAC193XData.Facility[1].Map == 2)
//		Tower_GetTrolleyingData()->Range = PAC193XData.Facility[1].MapValue;
//	else if(PAC193XData.Facility[2].Map == 2)
//		Tower_GetTrolleyingData()->Range = PAC193XData.Facility[1].MapValue;
//	//����
//	if(PAC193XData.Facility[0].Map == 3)
//		Tower_GetHoistingData()->HookHeight = PAC193XData.Facility[2].MapValue;
//	else if(PAC193XData.Facility[1].Map == 3)
//		Tower_GetHoistingData()->HookHeight = PAC193XData.Facility[2].MapValue;
//	else if(PAC193XData.Facility[2].Map == 3)
//		Tower_GetHoistingData()->HookHeight = PAC193XData.Facility[2].MapValue;
		
}
//---
/*
	��������void PAC193X_Cal_Rotation(PAC193X_TowerStructure *p)
*/
void PAC193X_Cal_Rotation(PAC193X_TowerStructure *p)
{
	if(p->Map == 1)
	{//�����ת
		p->MapValue = (p->ADValue - p->Orgin) * p->Rate;
		while(p->MapValue > 360)
			p->MapValue -=360;
		while(p->MapValue < 0)
			p->MapValue +=360;
		
		
		
	
	}
}
//---
/*
	��������void PAC193X_InitGPIO(void)
	������PAC193XͨѶ���ƹܽų�ʼ��
*/
void PAC193X_InitGPIO(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(PAC193X_SCL_RCC,ENABLE);
  RCC_APB2PeriphClockCmd(PAC193X_SDA_RCC,ENABLE);
	RCC_APB2PeriphClockCmd(PAC193X_SCLIN_RCC,ENABLE);
  RCC_APB2PeriphClockCmd(PAC193X_SDAIN_RCC,ENABLE);
  RCC_APB2PeriphClockCmd(PAC193X_PWRDN_RCC,ENABLE);
  RCC_APB2PeriphClockCmd(PAC193X_SLAL_RCC,ENABLE);
	//�������
	GPIO_InitStructure.GPIO_Pin = PAC193X_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_Init(PAC193X_SCL_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = PAC193X_SDA_PIN;
	GPIO_Init(PAC193X_SDA_PORT, &GPIO_InitStructure);
	//��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_InitStructure.GPIO_Pin = PAC193X_SCLIN_PIN;
	GPIO_Init(PAC193X_SCLIN_PORT, &GPIO_InitStructure);//�����¸���1����ܽ�
	GPIO_InitStructure.GPIO_Pin = PAC193X_SDAIN_PIN;
	GPIO_Init(PAC193X_SDAIN_PORT, &GPIO_InitStructure);//�����¸���2����ܽ�
	//PWRDN/SLAL
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin =  PAC193X_PWRDN_PIN;
  GPIO_Init(PAC193X_PWRDN_PORT, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin =  PAC193X_SLAL_PIN;
  GPIO_Init(PAC193X_SLAL_PORT, &GPIO_InitStructure);
	PAC193X_SCL(1);
	PAC193X_SDA(1);
	PAC193X_PWRDN(1);
	PAC193X_SLAL(0);
}
//---
/*
	��������void PAC193X_TimerInit(void)
	������	�ú���Ϊ��DS18B20��ʱ�����г�ʼ��
	˵����	����ʱ����ʼ��Ϊ0.5us��ʹ�õ���ʱ��2
*/
void PAC193X_TimerInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 1000-1;				//����ֵ1000   
	TIM_TimeBaseStructure.TIM_Prescaler = 18-1;    		//Ԥ��Ƶ,��ֵ+1Ϊ��Ƶ�ĳ���,72/36=4MHz.0.25us
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	//����ʱ�����жϵȼ���Ϊ���
	NVIC_InitStructure.NVIC_IRQChannel= TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE); 
//	TIM_Cmd(TIM2, ENABLE); 
}
//---
/*
	��������void DS18B20_DelayTime(u16 tim)
	������	�ú���Ϊ�������ö�ʱ����ʱֵ����������ʱ��
	���룺	u16 tim��ʱֵ����λ0.25us
*/
void PAC193X_DelayTime(u16 tim)
{
	TIM2->ARR = tim; 
	TIM2->CNT = 0;
	TIM2->CR1  |= B0;//������ʱ��
}
//---
/*
	ͨ�ö�ʱ��3�жϣ�����ϵͳ��ʱ��
*/
void TIM2_IRQHandler(void)
{
	if(TIM2->SR & 0X0001)	//����ж�
	{
		TIM2->CR1  &= ~B0;//�رն�ʱ��
		PAC193X_Work();
		TIM2->SR &= ~(1<<0);	//����жϱ�־λ
	}
}
//---
/*
	��������PAC193X_DataStructure* PAC193X_GetData(void)
	��������ȡ����screen.c�ļ�ȫ�ֱ����ṹ��
*/
PAC193X_DataStructure* PAC193X_GetData(void)
{
	return &PAC193XData;
}
//---
//-------------------------���ݶ�ȡ���洢----------------------------
//---
/*
	��������void PAC193X_ReadStoreData(u8 address,u8 *p,u8 len)
	������	��ȡ����
*/
void PAC193X_ReadStoreData(u8 address,u8 *p,u8 len)
{
	W25Xxx_ReadData(p,PAC193X_AdrStoreBase+address,len);
}
//---
/*
	��������void PAC193X_StoreData(u8 address,u8 *p,u8 len)
	�������ú���Ϊ�洢PAC193X���ò�������
	˵������ȡ����
				��������
				�޸�����
				��������
*/
void PAC193X_StoreData(u8 address,u8 *p,u8 len)
{
	u8 buffer[PAC193X_AdrStoreEND],cnt;
	W25Xxx_ReadData(buffer,PAC193X_AdrStoreBase,PAC193X_AdrStoreEND);//���������ò���
	W25Xxx_EraseSector(PAC193X_AdrStoreBase);//��������
	for(cnt=0;cnt<len;cnt++)
	{
		buffer[address++] = *p;
		p++;
	}
	W25Xxx_PageWrite(buffer,PAC193X_AdrStoreBase,PAC193X_AdrStoreEND);//��������
	
	PAC193X_InitData();
}
//---
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void PAC193X_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪPAC193X�ļ���ASCII�����ļ�
*/
void PAC193X_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1://:000|88|001
		{//��ȡ�������ò���
			u8 bf[PAC193X_AdrStoreEND];
			TwoBytes two;
			PAC193X_ReadStoreData(PAC193X_AdrStoreStart,bf,PAC193X_AdrStoreEND);
			//AD1
			two.U8Data[0] = bf[PAC193X_AdrStoreAD1Orgin];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD1Orgin+1];
			printf("AD1ԭ��ֵ=%u\r\n",two.U16Data);
			two.U8Data[0] = bf[PAC193X_AdrStoreAD1Endpoint];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD1Endpoint+1];
			printf("AD1�˵�ֵ=%u\r\n",two.U16Data);
			two.U8Data[0] = bf[PAC193X_AdrStoreAD1Refer];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD1Refer+1];
			printf("AD1����ֵ=%u\r\nAD1ӳ��",two.U16Data);
			if(bf[PAC193X_AdrStoreAD1Map] == 1)	printf("��ת���ݲɼ�\r\n");
			else if(bf[PAC193X_AdrStoreAD1Map] == 2)	printf("������ݲɼ�\r\n");
			else if(bf[PAC193X_AdrStoreAD1Map] == 3)	printf("�������ݲɼ�\r\n");
			else  
			{
				two.U8Data[0] = 1;
				PAC193X_Operation(105,two.U8Data,1);
				printf("��ת���ݲɼ�\r\n");
			}
			//AD2
			two.U8Data[0] = bf[PAC193X_AdrStoreAD2Orgin];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD2Orgin+1];
			printf("AD2ԭ��ֵ=%u\r\n",two.U16Data);
			two.U8Data[0] = bf[PAC193X_AdrStoreAD2Endpoint];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD2Endpoint+1];
			printf("AD2�˵�ֵ=%u\r\n",two.U16Data);
			two.U8Data[0] = bf[PAC193X_AdrStoreAD2Refer];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD2Refer+1];
			printf("AD2����ֵ=%u\r\nAD2ӳ��",two.U16Data);
			if(bf[PAC193X_AdrStoreAD2Map] == 1)	printf("��ת���ݲɼ�\r\n");
			else if(bf[PAC193X_AdrStoreAD2Map] == 2)	printf("������ݲɼ�\r\n");
			else if(bf[PAC193X_AdrStoreAD2Map] == 3)	printf("�������ݲɼ�\r\n");
			else  
			{
				two.U8Data[0] = 1;
				PAC193X_Operation(125,two.U8Data,1);
				printf("��ת���ݲɼ�\r\n");
			}
			//AD3
			two.U8Data[0] = bf[PAC193X_AdrStoreAD3Orgin];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD3Orgin+1];
			printf("AD3ԭ��ֵ=%u\r\n",two.U16Data);
			two.U8Data[0] = bf[PAC193X_AdrStoreAD3Endpoint];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD3Endpoint+1];
			printf("AD3�˵�ֵ=%u\r\n",two.U16Data);
			two.U8Data[0] = bf[PAC193X_AdrStoreAD3Refer];
			two.U8Data[1] = bf[PAC193X_AdrStoreAD3Refer+1];
			printf("AD3����ֵ=%u\r\nAD3ӳ��",two.U16Data);
			if(bf[PAC193X_AdrStoreAD3Map] == 1)	printf("��ת���ݲɼ�\r\n");
			else if(bf[PAC193X_AdrStoreAD3Map] == 2)	printf("������ݲɼ�\r\n");
			else if(bf[PAC193X_AdrStoreAD3Map] == 3)	printf("�������ݲɼ�\r\n");
			else  
			{
				two.U8Data[0] = 1;
				PAC193X_Operation(145,two.U8Data,1);
				printf("��ת���ݲɼ�\r\n");
			}
			break;
		}
		case 2:
		{
			break;
		}
		case 101:
		{
			break;
		}
		case 501:
		{//����ʼ�ź�
			printf("����ʼ�ź�\r\n");
			PAC193X_SCL(1)
			PAC193X_SDA(1)
			PAC193XData.WorkMode = 201;
			PAC193XData.OldWorkMode = 250;
			PAC193X_DelayTime(2);//��ʼ��������500us
			break;
		}
		case 502:
		{//��ֹͣ�ź�
			printf("�������ź�\r\n");
			PAC193X_SCL(0)
			PAC193XData.WorkMode = 211;
			PAC193XData.OldWorkMode = 250;
			PAC193X_DelayTime(2);//��ʼ��������500us
			break;
		}
		case 503:
		{//����Ӧ���ź�
			printf("��Ӧ���ź�\r\n");
			PAC193X_SCL(0)
			PAC193XData.WorkMode = 221;
			PAC193XData.OldWorkMode = 250;
			PAC193X_DelayTime(2);//��ʼ��������500usak;
		}
		case 504:
		{//����һ���ֽ�����
			printf("����һ���ֽ�����\r\n");
			PAC193X_SCL(0)
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer = 0xAA;
			PAC193XData.WorkMode = 231;
			PAC193XData.OldWorkMode = 250;
			PAC193X_DelayTime(2);//��ʼ��������500usak;
			break;
		}
		case 505:
		{//��ȡһ���ֽ�����
			printf("��ȡһ���ֽ�����\r\n");
			PAC193X_SCL(0)
			PAC193XData.CntBit = 0;
			PAC193XData.RTxBuffer= 0;
			PAC193XData.RTxBuffer = 0xAA;
			PAC193XData.WorkMode = 241;
			PAC193XData.OldWorkMode = 250;
			PAC193X_DelayTime(2);//��ʼ��������500usak;
			break;
		}
		case 506:
		{//��ȡ��ѹ����
			printf("��ȡ��ѹ����\r\n");
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 51;
			break;
		}
		case 507:
		{//ˢ�µ�ѹ����
			printf("ˢ�µ�ѹ����\r\n");
			PAC193X_DelayTime(2);
			PAC193XData.WorkMode = 1;
			break;
		}
		case 520:
		{//���ص�ѹ����
			u8 i;
			for(i=0;i<4;i++)
			{
				printf("ͨ��%u��ѹΪ��%fV\r\n",i+1,PAC193XData.Voltage[i]);
			}
			break;
		}
	}
}
//---
/*
	�������� 	void PAC193X_DebugRTU((u8 *p)
	������		�ú���ΪPAC193X�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*/
void PAC193X_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//�鿴���ĸ�������
	{
		case 0x01:
		{
			break;
		}
	}
}
//---





































