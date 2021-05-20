/*
	���ļ�Ϊ����Ŀ����غ���
	���庯������

*/

#include "server.h"


Sever_TowerCraneStructure TowerCraneData;//�����������ݽṹ��
/*
	��������void Server_Init(void);
	������		�ú���Ϊserver.c��ʼ������
*/
void Server_Init(void)
{
	Server_PwrIoInt();
	Server_InitTowerCraneData();
}
//---
/*
	��������void Server_PwrIoInt(void)
	���������ݴ���ģ��A1��ԴIO�ڳ�ʼ��
	˵����ʹ�õ�PA8�˿ڣ�0=�򿪵�Դ��1=�رյ�Դ
*/
void Server_PwrIoInt(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(SERVERPWR_APP, ENABLE);  //RCC_APB2Periph_AFIO
  GPIO_InitStructure.GPIO_Pin = SERVERPWR_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(SERVERPWR_PORT, &GPIO_InitStructure);
	SERVERPWR_OPEN;//��A1��Դ
}
//---
/*
	�������� Sever_TowerCraneStructure Server_GetTowerCraneData(void)
	�������ú���Ϊ���ڻ�ȡServer.c�ṹ�����TowerCraneData�ĵ�ַ
*/
Sever_TowerCraneStructure *Server_GetTowerCraneData(void)
{
	return &TowerCraneData;
}
//---
/*
	��������void Server_InitTowerCraneData(void)
	��������TowerCraneData�ṹ��������г�ʼ��
*/
void Server_InitTowerCraneData(void)
{
	TowerCraneData.Address = 0x08;//A1ͨѶģ���ַ
	TowerCraneData.Address = 0x0C;//A1ͨѶģ���ַ
	TowerCraneData.Flag = 0;
	TowerCraneData.Flag |= B0;//ϵͳ����
	TowerCraneData.RunMode = SERVER_RunModeNone;
	//���ݳ�ʼ��
	TowerCraneData.LocalNumber = 180600011;//A1ģ�����к�
	TowerCraneData.Manufacturer = 22861;//����������
	TowerCraneData.Version = 553;//�����ͺ�
	TowerCraneData.SerialNumber = 180600011;//�������
	TowerCraneData.Height = 100;//�����߶ȣ���λ���ס�
	TowerCraneData.Length = 60;//�����۳�����λ���ס�
	TowerCraneData.RatedLoad = 6;//��������أ���λ���֡�
	TowerCraneData.OutdoorsTem = 286;//�����¶ȣ���λ0.1���϶�
	TowerCraneData.WindSpeed  = 0;//���٣���λ0.1��
	TowerCraneData.UpHour = 0;//����ʱ�䣺Сʱ
	TowerCraneData.UpMinutes = 0;//����ʱ�䣺����
	TowerCraneData.UpSec = 0;//����ʱ�䣺��
	TowerCraneData.WorkingHour = 0;//����ʱ�䣺Сʱ
	TowerCraneData.WorkingMinutes = 0;//����ʱ�䣺����
	TowerCraneData.WorkingSec = 0;//����ʱ�䣺��
	TowerCraneData.Rotation = 100;//������ת���ݣ���λ�ȣ���Χ0~359
	TowerCraneData.Range = 3000;//�����������λ0.01��
	TowerCraneData.Raising = 5000;//������������λ0.01��
	TowerCraneData.HookHeight = 5000;//�����߶ȣ���λ0.01��
	TowerCraneData.Hoisting = 258;//��ǰ���أ���λ0.01��
	TowerCraneData.CurrentPower = 150;//��ǰ���ʣ���λ0.1ǧ��
	TowerCraneData.Momental = 80;//��ǰ���أ���ǰ�����������ص���������صİٷֱȣ�ȡֵ0~100
	TowerCraneData.TaErrorCode = 11111;//��ǰ�����������
	TowerCraneData.BianErrorCode = 22222;//��Ƶ���������
	TowerCraneData.PlcInState = 0xAAAAAAAA;//��ǰPLC����״̬
	TowerCraneData.PlcOutState = 0x55555555;//��ǰPLC���״̬
}
//---
/*
	��������void Server_Timer(void)
	������	�ú���Ϊserver.c�ļ���ʱ�Ӻ�����ʱ�ӵ�λΪ1ms����ϵͳ1ms��ʱ���жϵ���
*/
void Server_Timer(void)
{
	TowerCraneData.Timer++;
	if(TowerCraneData.Timer == 1000)
	{//1S��ʱ
		TowerCraneData.Timer = 0;
		if(TowerCraneData.Flag & b0)
		{//ϵͳ����
			TowerCraneData.UpSec++;//����ʱ����
			if(TowerCraneData.UpSec == 60)
			{//��
				TowerCraneData.UpSec = 0;
				TowerCraneData.UpMinutes++;
				if(TowerCraneData.UpMinutes == 60)
				{//ʱ
					TowerCraneData.UpMinutes = 0;
					TowerCraneData.UpHour++;
				}
			}
		}
		if(TowerCraneData.Flag & b1)
		{//ϵͳ����
			TowerCraneData.WorkingSec++;//����ʱ����
			if(TowerCraneData.WorkingSec == 60)
			{//��
				TowerCraneData.WorkingSec = 0;
				TowerCraneData.WorkingMinutes++;
				if(TowerCraneData.WorkingMinutes == 60)
				{//ʱ
					TowerCraneData.WorkingMinutes = 0;
					TowerCraneData.WorkingHour++;
				}
			}
		}
	}
}
//---
/*
	��������void Server_Writ30to44(void)
	������	�ú���Ϊ��A1ģ��д30~44��ַ�Ĵ���
	08 10 00 1E 00 0F 1E 00 03 00 00 00 00 00 
	00 00 00 00 00 00 00 00 00 00 00 00 00 00 
	00 00 00 00 00 00 00 00 00 5F 12
*/
void Server_Writ30to44(void)
{
	u8 buffer[40];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = TowerCraneData.Address;//��ַ
	buffer[cnt++] = 0x10;//д����
	buffer[cnt++] = 0x00;//��ʼ��ַ0x001E(30)��2���ֽ�
	buffer[cnt++] = 0x1E;
	buffer[cnt++] = 0x00;//�����ݳ���0x000F(15),2���ֽ�
	buffer[cnt++] = 0x0F;
	buffer[cnt++] = 0x1E;//�ֽ����ݳ���0x1E(30),1���ֽ�
	//30�����֣�
	buffer[cnt++] = TowerCraneData.Hoisting>>8;//��ǰ����������ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.Hoisting;//��ǰ����������ֽڣ�2���ֽ�
	//31�����֣�
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
//	
//	buffer[cnt++] = TowerCraneData.Version>>8;//�����ͺŸ��ֽڣ�2���ֽ�
//	buffer[cnt++] = TowerCraneData.Version;//�����ͺŵ��ֽڣ�2���ֽ�
	//32~33��˫�֣�
	buffer[cnt++] = TowerCraneData.SerialNumber>>8;//������Ÿ��ֽڣ�4���ֽ�
	buffer[cnt++] = TowerCraneData.SerialNumber;//������ŵ��ֽڣ�4���ֽ�
	buffer[cnt++] = TowerCraneData.SerialNumber>>24;//�������24~32�ֽڣ�4���ֽ�
	buffer[cnt++] = TowerCraneData.SerialNumber>>16;//������ŵ��ֽڣ�4���ֽ�
	//34�����֣�
	buffer[cnt++] = TowerCraneData.CurrentPower>>8;//��ǰ���ʸ��ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.CurrentPower;//��ǰ���ʵ��ֽڣ�2���ֽ�
	//35�����֣�
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//36�����֣�
	buffer[cnt++] = TowerCraneData.Height>>8;//�����߶ȸ��ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.Height;//�����߶ȵ��ֽڣ�2���ֽ�
	//37�����֣�
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//38�����֣�
	buffer[cnt++] = TowerCraneData.Length>>8;//���۳��ȸ��ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.Length;//���۳��ȵ��ֽڣ�2���ֽ�
	//39�����֣�
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//40�����֣�
	buffer[cnt++] = TowerCraneData.RatedLoad>>8;//����ظ��ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.RatedLoad;//����ص��ֽڣ�2���ֽ�
	//41�����֣�
	buffer[cnt++] = TowerCraneData.OutdoorsTem>>8;//�����¶ȸ��ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.OutdoorsTem;//�����¶ȵ��ֽڣ�2���ֽ�
	//42�����֣�
	buffer[cnt++] = TowerCraneData.WindSpeed>>8;//���ٸ��ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.WindSpeed;//���ٵ��ֽڣ�2���ֽ�
	//43�����֣�
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//44�����֣�
	buffer[cnt++] = TowerCraneData.UpHour>>8;//����ʱ�䣺Сʱ���ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.UpHour;//����ʱ�䣺Сʱ���ֽڣ�2���ֽ�
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	USART_SetEvenVerify();
	for(crc=0;crc<cnt;crc++)
	{
		Server_SendBufferData(Server_UsartServerPort,Server_UsartServerBuffer,buffer[crc]);//��������
	}
	TowerCraneData.RunMode = SERVER_RunModeWrit30to44;
}
//---
/*
	��������void Server_Writ30to44(void)
	������	�ú���Ϊ��A1ģ��д45~59��ַ�Ĵ���
	08 10 00 2D 00 0F 1E 00 00 00 00 00 00 00
	00 00 00 02 BF 00 01 00 00 04 00 00 00 00
	00 00 00 00 00 00 00 00 00 09 EE
*/
void Server_Writ45to59(void)
{
	u8 buffer[40];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = TowerCraneData.Address;//��ַ
	buffer[cnt++] = 0x10;//д����
	buffer[cnt++] = 0x00;//��ʼ��ַ0x001E(30)��2���ֽ�
	buffer[cnt++] = 0x2D;
	buffer[cnt++] = 0x00;//�����ݳ���0x000F(15),2���ֽ�
	buffer[cnt++] = 0x0F;
	buffer[cnt++] = 0x1E;//�ֽ����ݳ���0x1E(30),1���ֽ�
	//45�����֣�
	buffer[cnt++] = 0;	//��
	buffer[cnt++] = 0;
	//46~47��˫�֣�
	buffer[cnt++] = 0;	//��
	buffer[cnt++] = 0;
	buffer[cnt++] = 0;
	buffer[cnt++] = 0;
	//48�����֣�
	buffer[cnt++] = 0;	//��
	buffer[cnt++] = 0;
	//49�����֣�
	buffer[cnt++] = 0;	//��
	buffer[cnt++] = 0;
	//50�����֣�
	buffer[cnt++] = TowerCraneData.UpMinutes>>8;//����ʱ�䣺���Ӹ��ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.UpMinutes;//����ʱ�䣺���ӵ��ֽڣ�2���ֽ�
	//51�����֣�
	buffer[cnt++] = TowerCraneData.UpSec>>8;//����ʱ�䣺����ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.UpSec;//����ʱ�䣺����ֽڣ�2���ֽ�
	//52�����֣�
	buffer[cnt++] = 0;	//��
	buffer[cnt++] = 0;
	//53�����֣�
	buffer[cnt++] = TowerCraneData.Momental>>8;//���ظ��ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.Momental;//���ص��ֽڣ�2���ֽ�
	//54�����֣�
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//55�����֣�
	buffer[cnt++] = TowerCraneData.TaErrorCode>>8;//�������������ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.TaErrorCode;//�������������ֽڣ�2���ֽ�
	//56�����֣�
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//57�����֣�
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//58�����֣�
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//59�����֣�
	buffer[cnt++] = TowerCraneData.BianErrorCode>>8;//��Ƶ��������룬2���ֽ�
	buffer[cnt++] = TowerCraneData.BianErrorCode;//��Ƶ��������룬2���ֽ�
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	USART_SetEvenVerify();
	for(crc=0;crc<cnt;crc++)
	{
		Server_SendBufferData(Server_UsartServerPort,Server_UsartServerBuffer,buffer[crc]);//��������
	}
	TowerCraneData.RunMode = SERVER_RunModeWrit45to59;
}
//---
/*
	��������void Server_Writ30to44(void)
	������	�ú���Ϊ��A1ģ��д45~59��ַ�Ĵ���
	08 10 00 3C 00 0F 1E 00 00 00 00 00 00 00
	00 00 00 00 00 00 00 00 00 00 00 00 00 00
	00 00 00 00 00 00 00 00 00 13 EA
*/
void Server_Writ60to74(void)
{
	u8 buffer[40];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = TowerCraneData.Address;//��ַ
	buffer[cnt++] = 0x10;//д����
	buffer[cnt++] = 0x00;//��ʼ��ַ0x001E(30)��2���ֽ�
	buffer[cnt++] = 0x3C;
	buffer[cnt++] = 0x00;//�����ݳ���0x000F(15),2���ֽ�
	buffer[cnt++] = 0x0F;
	buffer[cnt++] = 0x1E;//�ֽ����ݳ���0x1E(30),1���ֽ�
	//60(���֣�
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//61�����֣�
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//62�����֣�
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//63�����֣�
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//64�����֣�
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//65�����֣�
	buffer[cnt++] = TowerCraneData.Rotation>>8;//������ת�Ǹ��ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.Rotation;//������ת�ǵ��ֽڣ�2���ֽ�
	//66�����֣�
	buffer[cnt++] = TowerCraneData.Range>>8;//����������ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.Range;//����������ֽڣ�2���ֽ�
	//67~68��˫�֣�
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//69�����֣�
	buffer[cnt++] = TowerCraneData.Raising>>8;//�����������ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.Raising;//�����������ֽڣ�2���ֽ�
	//70�����֣�
	buffer[cnt++] = TowerCraneData.HookHeight>>8;//���������߶ȸ��ֽڣ�2���ֽ�
	buffer[cnt++] = TowerCraneData.HookHeight;//���������߶ȵ��ֽڣ�2���ֽ�
	//71~72��˫�֣�
	buffer[cnt++] = TowerCraneData.PlcInState>>8;//PLC����״̬���ֽڣ�4���ֽ�
	buffer[cnt++] = TowerCraneData.PlcInState;//PLC����״̬���ֽڣ�4���ֽ�
	buffer[cnt++] = TowerCraneData.PlcInState>>24;//PLC����״̬���ֽڣ�4���ֽ�
	buffer[cnt++] = TowerCraneData.PlcInState>>16;//PLC����״̬���ֽڣ�4���ֽ�
	//73~74��˫�֣�
	buffer[cnt++] = TowerCraneData.PlcOutState>>8;//PLC���״̬���ֽڣ�4���ֽ�
	buffer[cnt++] = TowerCraneData.PlcOutState;//PLC���״̬���ֽڣ�4���ֽ�
	buffer[cnt++] = TowerCraneData.PlcOutState>>24;//PLC���״̬���ֽڣ�4���ֽ�
	buffer[cnt++] = TowerCraneData.PlcOutState>>16;//PLC���״̬���ֽڣ�4���ֽ�
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	USART_SetEvenVerify();
	for(crc=0;crc<cnt;crc++)
	{
		Server_SendBufferData(Server_UsartServerPort,Server_UsartServerBuffer,buffer[crc]);//��������
	}
	TowerCraneData.RunMode = SERVER_RunModeWrit60to74;
}
//---
/*
	��������void Server_Writ30to44(void)
	������	�ú���Ϊ��A1ģ��д45~59��ַ�Ĵ���
	08 10 00 4B 00 0F 1E D0 13 0A 2C 00 00 00
	00 00 00 00 00 00 00 00 00 00 00 00 00 00
	00 00 00 00 00 00 00 00 00 22 04
*/
void Server_Writ75to76(void)
{
	u8 buffer[40];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = TowerCraneData.Address;//��ַ
	buffer[cnt++] = 0x10;//д����
	buffer[cnt++] = 0x00;//��ʼ��ַ0x001E(30)��2���ֽ�
	buffer[cnt++] = 0x4B;
	buffer[cnt++] = 0x00;//�����ݳ���0x000F(15),2���ֽ�
	buffer[cnt++] = 0x0F;
	buffer[cnt++] = 0x1E;//�ֽ����ݳ���0x1E(30),1���ֽ�
	//75~76��˫�֣�
	buffer[cnt++] = TowerCraneData.LocalNumber>>8;//A1ģ�����кţ�4���ֽ�
	buffer[cnt++] = TowerCraneData.LocalNumber;//A1ģ�����кţ�4���ֽ�
	buffer[cnt++] = TowerCraneData.LocalNumber>>24;//A1ģ�����кţ�4���ֽ�
	buffer[cnt++] = TowerCraneData.LocalNumber>>16;//A1ģ�����кţ�4���ֽ�
	//77
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//78
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//79
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//80
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//81
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//82
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//83
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//84
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//85
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//86
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//87
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//88
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	//89
	buffer[cnt++] = 0;//��
	buffer[cnt++] = 0;
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	USART_SetEvenVerify();
	for(crc=0;crc<cnt;crc++)
	{
		Server_SendBufferData(Server_UsartServerPort,Server_UsartServerBuffer,buffer[crc]);//��������
	}
	TowerCraneData.RunMode = SERVER_RunModeWrit75to76;
}
//---
/*
	��������void Server_Read0to6(void)
	������	�ú���Ϊ��A1ģ��д45~59��ַ�Ĵ���
	08 03 00 00 00 07 04 91
*/
void Server_Read0to6(void)
{
	u8 buffer[10];
	u8 cnt=0;
	u16 crc;
	buffer[cnt++] = TowerCraneData.Address;//��ַ
	buffer[cnt++] = 0x03;//������
	buffer[cnt++] = 0x00;//��ʼ��ַ0x001E(30)��2���ֽ�
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x00;//�����ݳ���0x000F(15),2���ֽ�
	buffer[cnt++] = 0x07;
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	USART_SetEvenVerify();
	for(crc=0;crc<cnt;crc++)
	{
		Server_SendBufferData(Server_UsartServerPort,Server_UsartServerBuffer,buffer[crc]);//��������
	}
	TowerCraneData.RunMode = SERVER_RunModeRead0to6;
}
//---
//---
/*
	��������void Server_WritNewModule(void)
	������	�ú���Ϊ��A1ģ��д30~44��ַ�Ĵ���
	08 10 00 1E 00 0F 1E 00 03 00 00 00 00 00 
	00 00 00 00 00 00 00 00 00 00 00 00 00 00 
	00 00 00 00 00 00 00 00 00 5F 12
*/
void Server_WritNewModule(void)
{
	u8 buffer[200];
	u8 cnt=0;
	u16 crc;
	TwoBytes two;
	FourBytes four;
	buffer[cnt++] = 0x0C;//TowerCraneData.Address;//��ַ
	buffer[cnt++] = 0x10;//д����
	buffer[cnt++] = 0x00;//��ʼ��ַ0x0010(30)��2���ֽ�
	buffer[cnt++] = 0x10;
	buffer[cnt++] = 0x00;//�����ݳ���0x0016(22),2���ֽ�
	buffer[cnt++] = 0x16;
	buffer[cnt++] = 0x2C;//�ֽ����ݳ���0x2C(44),1���ֽ�
	//0x10�����֣����������̣��޷��ţ�
	two.U16Data = Tower_GetBaseData()->Manufacturer;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x11�����֣������ͺţ��޷��ţ�
	two.U16Data = Tower_GetBaseData()->Version;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x12/Ox13��˫�֣�������ţ��޷��ţ�
	four.U32Data = Tower_GetBaseData()->SerialNumber;
	buffer[cnt++] = four.U8Data[3];
	buffer[cnt++] = four.U8Data[2];
	buffer[cnt++] = four.U8Data[1];
	buffer[cnt++] = four.U8Data[0];
	//0x14(���֣������߶ȣ��޷��ţ�
	two.U16Data = (u16)(Tower_GetBaseData()->Height/1000);
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x15(����)�����۳����޷��ţ�
	two.U16Data = (u16)(Tower_GetBaseData()->Length/1000);
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x16(����)���������(�޷��ţ�
	two.U16Data = Tower_GetBaseData()->Length;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x17(����)�����¶ȣ��з��ţ�
	if(DS18B20_GetData()->Map == 1)
		two.S16Data = (s16)(DS18B20_GetData()->Tem1 * 10);
	else
		two.S16Data = (s16)(DS18B20_GetData()->Tem2 * 10);
	two.U16Data = Tower_GetBaseData()->Length;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x18(����)������٣��޷��ţ�
	two.U16Data = Wind_GetWindData()->Speed;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x19(����)����ʱ�䣺ʱ���޷��ţ�
	two.U16Data = TowerCraneData.UpHour;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x1A(����)����ʱ�䣺�֣��޷��ţ�
	two.U16Data = TowerCraneData.UpMinutes;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x1B(����)����ʱ�䣺�루�޷��ţ�
	two.U16Data = TowerCraneData.UpSec;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x1C(����)����ʱ�䣺ʱ���޷��ţ�
	two.U16Data = TowerCraneData.WorkingHour;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x1D(����)����ʱ�䣺�֣��޷��ţ�
	two.U16Data = TowerCraneData.WorkingMinutes;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x1E(����)����ʱ�䣺�루�޷��ţ�
	two.U16Data = TowerCraneData.WorkingSec;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x1F(����)��ת���޷��ţ�
	two.U16Data = (u16)Tower_GetSlewingData()->Rotation;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x20(����)������޷��ţ�
	two.U16Data = (u16)(Tower_GetTrolleyingData()->Range/10);
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x21(����)�������޷��ţ�
	two.U16Data = (u16)(Tower_GetTrolleyingData()->Range/10);
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x22(����)�����߶ȣ��޷��ţ�
	two.U16Data = (u16)(Tower_GetHoistingData()->HookHeight/10);
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x23(����)����������޷��ţ�
	two.U16Data = (u16)((float)Thbss01_GetThbss01Data()->Hosting/1000);;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x24(����)���ʣ��޷��ţ�
	two.U16Data = (u16)(Ohrd23_GetOhrd23Data()->Base.PZY/10);;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	//0x25(����)���أ��޷��ţ�
	two.U16Data = (u16)(Tower_GetBaseData()->Momental*100);;
	buffer[cnt++] = two.U8Data[1];
	buffer[cnt++] = two.U8Data[0];
	crc = CRC16(buffer,cnt);//����CRC����
	buffer[cnt++] = crc>>8;//CRC���ֽڣ�2���ֽ�
	buffer[cnt++] = crc;//CRC���ֽڣ�2���ֽ�
	Server_UsartModeSet();
	for(crc=0;crc<cnt;crc++)
	{
		Server_SendBufferData(Server_UsartServerPort,Server_UsartServerBuffer,buffer[crc]);//��������
	}
	TowerCraneData.RunMode = SERVER_RunModeWritNewModule;
}
void Server_UsartModeSet(void)
{
	MCH58_UartModeSet(Server_UsartServerPort,9600,'E','1');
}
//---
//-------------------------------------���Ժ���----------------------------------------
//---
/*
	�������� void Server_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪServer.c�ļ���ASCII�����ļ�
*/
void Server_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1://
		{//:000|55|001|1|100
			Server_InitTowerCraneData();//��ʼ��TowerCraneData�ṹ���������
			break;
		}
		case 2:
		{
			Server_SetComEvenVerify();
			Server_Writ30to44();
			printf("��A1ģ��д30~44��ַ�Ĵ���\r\n");
			break;
		}
		case 3:
		{
			Server_SetComEvenVerify();
			Server_Writ45to59();
			printf("��A1ģ��д45~59��ַ�Ĵ���\r\n");
			break;
		}
		case 4:
		{
			Server_SetComEvenVerify();
			Server_Writ60to74();
			printf("��A1ģ��д60~74��ַ�Ĵ���\r\n");
			break;
		}
		case 5:
		{
			Server_SetComEvenVerify();
			Server_Writ75to76();
			printf("��A1ģ��д75~76��ַ�Ĵ���\r\n");
			break;
		}
		case 6:
		{
			Server_SetComEvenVerify();
			Server_Read0to6();
			printf("��A1ģ���0~6��ַ�Ĵ���\r\n");
			break;
		}
		//-------------------------------------------------------------------------------
		//�޸Ĳ������������,������Ҫ���뱣�浽EEPROM��
		case 100://�޸����������̴��루00000~65535��
		{//:000|55|100|12345
			TowerCraneData.Manufacturer = Calculate_CharToU16(p+4,5);
			printf("����������%u\r\n",TowerCraneData.PlcOutState);
			break;
		}
		case 101://�޸������ͺţ�00000~65535��
		{//:000|55|100|12345
			TowerCraneData.PlcOutState = Calculate_CharToU16(p+4,5);
			printf("����������%u\r\n",TowerCraneData.Version);
			break;
		}
		case 102://�޸������ͺţ�0000000000~4294967296��
		{//:000|55|100|12345
			TowerCraneData.SerialNumber = Calculate_CharToU16(p+4,10);
			printf("����������%u\r\n",TowerCraneData.Version);
			break;
		}
		//-------------------------------------------------------------------------------
		//�޸ĵ�ǰ����������ݣ�������
		case 200://�޸������ͺţ�0000000000~4294967296��
		{//:000|55|100|12345
			TowerCraneData.SerialNumber = Calculate_CharToU16(p+4,10);
			printf("����������%u\r\n",TowerCraneData.Version);
			break;
		}
		//-------------------------------------------------------------------------------
		//��ȡ��ǰ�����������
		case 300://��ȡ��ǰ�����ͺ�
		{//:000|55|300
			printf("���������̣�%u\r\n",TowerCraneData.Version);
			break;
		}
		case 301://��ȡ��ǰ�����ͺ�
		{//:000|55|301
			printf("�����ͺţ�%u\r\n",TowerCraneData.Version);
			break;
		}
		case 302://��ȡ��ǰ�������
		{//:000|55|302
			printf("������ţ�%u\r\n",TowerCraneData.SerialNumber);
			break;
		}
		case 303://��ȡ��ǰ�����߶�
		{//:000|55|303
			printf("�����߶ȣ�%u\r\n",TowerCraneData.Height);
			break;
		}
		case 304://��ȡ��ǰ�����߶�
		{//:000|55|304
			printf("���۳��ȣ�%u\r\n",TowerCraneData.Length);
			break;
		}
		case 305://��ȡ�����
		{//:000|55|305
			printf("��������أ�%u\r\n",TowerCraneData.RatedLoad);
			break;
		}
		case 306://��ȡ��ǰ�����¶�
		{//:000|55|306
			printf("��ǰ�����¶ȣ�%d\r\n",TowerCraneData.OutdoorsTem);
			break;
		}
		case 307://��ȡ��ǰ�������
		{//:000|55|307
			printf("��ǰ������٣�%u\r\n",TowerCraneData.WindSpeed);
			break;
		}
		case 308://��ȡ��ǰ����ʱ��
		{//:000|55|308
			printf("��ǰ����ʱ�䣺Сʱ��%u\r\n",TowerCraneData.UpHour);
			printf("��ǰ����ʱ�䣺���ӣ�%u\r\n",TowerCraneData.UpHour);
			printf("��ǰ����ʱ�䣺	 �룺%u\r\n",TowerCraneData.UpHour);
			break;
		}
		case 309://��ȡ��ǰ����ʱ��
		{//:000|55|309
			printf("��ǰ����ʱ�䣺Сʱ��%u\r\n",TowerCraneData.WorkingHour);
			printf("��ǰ����ʱ�䣺���ӣ�%u\r\n",TowerCraneData.WorkingHour);
			printf("��ǰ����ʱ�䣺	 �룺%u\r\n",TowerCraneData.WorkingHour);
			break;
		}
		case 310://��ȡ��ǰ��ת�Ƕ�
		{//:000|55|310
			printf("��ǰ������ת�Ƕȣ�%u\r\n",TowerCraneData.Rotation);
			break;
		}
		case 311://��ȡ��ǰ�������
		{//:000|55|311
			printf("��ǰ���������%ucm\r\n",TowerCraneData.Range);
			break;
		}
		case 312://��ȡ��ǰ��������
		{//:000|55|312
			printf("��ǰ����������%ucm\r\n",TowerCraneData.Raising);
			break;
		}
		case 313://��ȡ��ǰ���������߶�
		{//:000|55|313
			printf("��ǰ���������߶ȣ�%ucm\r\n",TowerCraneData.HookHeight);
			break;
		}
		case 314://��ȡ��ǰ�����������
		{//:000|55|314
			printf("��ǰ�������������%u\r\n",TowerCraneData.Hoisting);
			break;
		}
		case 315://��ȡ��ǰ������ǰ����
		{//:000|55|315
			printf("��ǰ�������ʣ�%u\r\n",TowerCraneData.CurrentPower);
			break;
		}
		case 316://��ȡ��ǰ��������
		{//:000|55|316
			printf("��ǰ�������أ�%u\r\n",TowerCraneData.Momental);
			break;
		}
		case 317://��ȡ��ǰ�����������
		{//:000|55|316
			printf("��ǰ����������룺%u\r\n",TowerCraneData.TaErrorCode);
			break;
		}
		case 318://��ȡ��ǰ��Ƶ���������
		{//:000|55|318
			printf("��ǰ��Ƶ��������룺%u\r\n",TowerCraneData.BianErrorCode);
			break;
		}
		case 319://��ȡ��ǰPLC����״̬
		{//:000|55|319
			printf("��ǰPLC����״̬��%u\r\n",TowerCraneData.PlcInState);
			break;
		}
		case 320://��ȡ��ǰPLC���״̬
		{//:000|55|320
			printf("��ǰPLC���״̬��%u\r\n",TowerCraneData.PlcOutState);
			break;
		}
		//-------------------------------------------------------------------------------
		
		
		
		default:
		{
			printf("Server.c���յ���Ч����ָ��%u!\r\n",cmd);
			break;
		}
	}
}
//---
/*
	�������� void Server_DebugRTU((u8 *p)
	������		�ú���ΪServer.c�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*/
void Server_DebugRTU(u8 *p,u8 len)
{
	switch(TowerCraneData.RunMode)//�鿴���ĸ�������
	{
		case SERVER_RunModeWrit30to44:
		{
//			printf("д30~44���\r\n");
			break;
		}
		case SERVER_RunModeWrit45to59:
		{
//			printf("д45~59���\r\n");
			break;
		}
		case SERVER_RunModeWrit60to74:
		{
//			printf("д60~74���\r\n");
			break;
		}
		case SERVER_RunModeWrit75to76:
		{
//			printf("д75~76���\r\n");
			break;
		}
		case SERVER_RunModeRead0to6:
		{
//			printf("��0~6���\r\n");
			break;
		}
		case SERVER_RunModeWritNewModule:
		{
			break;
		}
	}
	TowerCraneData.RunMode = SERVER_RunModeNone;
}
//---










