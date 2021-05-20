/*
	���ļ�Ϊ����Ŀ����غ���
	���庯������

*/

#include "collect.h"

Collect_DataStructure CollectData;
/*
	��������void Collect_main(void)
	������	�ú���Ϊ�ɼ�������������������������
*/
void Collect_main(void)
{
	switch(CollectData.RunMode)
	{
		//����A1�����
		case 1:
		{
			if(CollectData.Timer == 0)
			{
				Server_WritNewModule();
				CollectData.Timer = 300;
				CollectData.TimerTwo = 2000;
				CollectData.RunMode = 2;
			}
			break;
		}
		case 2:
		{
			if(Server_GetTowerCraneData()->RunMode == 0)
			{
				CollectData.Timer = 30;
				CollectData.RunMode = 11;
			}
			else if(CollectData.Timer == 0)
			{
				printf("A1��ģ�����\r\n");
				CollectData.RunMode = 11;
			}
			break;
		}
		//-----������������-----
		case 11:
		{
			if(CollectData.Timer==0)
			{
				Ohrd23_ReadBase();
				CollectData.Timer = 300;
				CollectData.RunMode = 12;
			}
			break;
		}
		case 12:
		{
			if(Ohrd23_GetOhrd23Data()->CollectMode  == 0)
			{
				if(Ohrd23_GetOhrd23Data()->CollectCommand != 0)
				{
					CollectData.Timer = 30;
					CollectData.RunMode = 13;
				}
				else
				{
					CollectData.RunMode = 21;
				}
			}
			else if(CollectData.Timer == 0)
			{
				printf("��������������\r\n");
				CollectData.RunMode = 21;
			}
			break;
		}
		case 13:
		{
			CollectData.RunMode = 21;
			break;
		}
		//-----�����ٴ�����------
		case 21:
		{
			if(CollectData.Timer == 0)
			{
				YCFS260_CollectWindSpeed();
				CollectData.Timer = 300;
				CollectData.RunMode = 22;
			}
			break;
		}
		case 22:
		{
			if(Wind_GetWindData()->RunMode == 0)
			{
				CollectData.Timer = 30;
				CollectData.RunMode = 31;
			}
			else if(CollectData.Timer == 0)
			{
				printf("���ٴ���������\r\n");
				CollectData.RunMode = 31;
			}
			break;
		}
		//-----�����ش�����-----
		case 31:
		{
			if(CollectData.Timer == 0)
			{
				Thbss01_Operation(2,0,0);
				CollectData.Timer = 400;
				CollectData.RunMode = 32;
			}
			break;
		}
		case 32:
		{
			if(Thbss01_GetThbss01Data()->CollectMode == 0)
			{
				CollectData.Timer = 30;
				CollectData.RunMode = 41;
			}
			else if(CollectData.Timer == 0)
			{
				printf("���ش���������\r\n");
				CollectData.RunMode = 200;
			}
			break;
		}
		//-----��������1����ֵ------
		case 41:
		{
			if(CollectData.Timer == 0)
			{
				MCH58_CollectValue(&MCH58_GetMCH58Data()->Mch58[0]);
				CollectData.Timer = 300;
				CollectData.RunMode = 42;
			}
			break;
		}
		case 42:
		{
			if(MCH58_GetMCH58Data()->CollectMode == 0)
			{
				CollectData.Timer = 30;
				CollectData.RunMode = 51;
			}
			else if(CollectData.Timer == 0)
			{
				printf("������1����\r\n");
				CollectData.RunMode = 51;
			}
			break;
		}
		//-----��������2����ֵ-----
		case 51:
		{
			if(CollectData.Timer == 0)
			{
				MCH58_CollectValue(&MCH58_GetMCH58Data()->Mch58[1]);
				CollectData.Timer = 300;
				CollectData.RunMode = 52;
			}
			break;
		}
		case 52:
		{
			if(MCH58_GetMCH58Data()->CollectMode == 0)
			{
				CollectData.Timer = 30;
				CollectData.RunMode = 61;
			}
			else if(CollectData.Timer == 0)
			{
				printf("������2����\r\n");
				CollectData.RunMode = 61;
			}
			break;
		}
		//-----��������3����ֵ-----
		case 61:
		{
			if(CollectData.Timer == 0)
			{
				MCH58_CollectValue(&MCH58_GetMCH58Data()->Mch58[2]);
				CollectData.Timer = 300;
				CollectData.RunMode = 62;
			}
			break;
		}
		case 62:
		{
			if(MCH58_GetMCH58Data()->CollectMode == 0)
			{
				CollectData.RunMode = 200;
			}
			else if(CollectData.Timer == 0)
			{
				printf("������3����\r\n");
				CollectData.RunMode = 200;
			}
			break;
		}
		
		case 200:
		{
			if(CollectData.TimerTwo == 0)
			{
				CollectData.RunMode = 1;
			}
			else
			{
				CollectData.Timer = 30;
				CollectData.RunMode = 11;
			}
			break;
		}
	}
}
//---
/*
	��������void Collect_Init(void)
	������	�ú���Ϊcollect.c�ļ���ʼ������
*/
void Collect_Init(void)
{
	Collect_CollectDataInit();
}
//---
/*
	��������void Collect_Timer(void)
	������	�ú���Ϊcollect.c�ļ���ʱ��������1ms����ϵͳ1ms��ʱ������
*/
void Collect_Timer(void)
{
	if(CollectData.Timer != 0)CollectData.Timer--;
	if(CollectData.TimerTwo != 0)CollectData.TimerTwo--;
}
//---
/*
	��������Collect_DataStructure* Collect_GetCollectData(void)
	˵����	��ȡCollectData���ݵ�ַָ��
*/
Collect_DataStructure* Collect_GetCollectData(void)
{
	return &CollectData;
}
//---
/*
	��������void Collect_CollectDataInit(void)
	������	��ʼ��CollectData����
*/
void Collect_CollectDataInit(void)
{
	CollectData.RunMode = 1;
}

//---
/*
	��������float Tower_CalAngle(u32 buffer)
	������	����������ת�Ƕ�					
*/
float Collec_CalSlewing(u32 buffer)
{
	s32 bf=0;
	if(MCH58_GetMCH58Data()->Mch58[0].Map == 1)
		bf = MCH58_GetMCH58Data()->Mch58[0].CodedValue - MCH58_GetMCH58Data()->Mch58[0].ZeroValue;
	else if(MCH58_GetMCH58Data()->Mch58[1].Map == 1)
		bf = MCH58_GetMCH58Data()->Mch58[1].CodedValue - MCH58_GetMCH58Data()->Mch58[1].ZeroValue;
	else if(MCH58_GetMCH58Data()->Mch58[2].Map == 1)
		bf = MCH58_GetMCH58Data()->Mch58[2].CodedValue - MCH58_GetMCH58Data()->Mch58[2].ZeroValue;
	Tower_GetSlewingData()->Rotation = (float)((360.0*bf)/(((double)Tower_GetSlewingData()->BigGear / Tower_GetSlewingData()->LittleGear)*TOWER_EnDiv));
	return 0;
}
//---
//---
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Collect_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪCollect�ļ���ASCII�����ļ�
*/
void Collect_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:
		{//ϵͳ��λ
			CollectData.RunMode = 1;
			printf("�����ϴ�����\r\n");
			break;
		}
		case 2:
		{//:000|56|002
			CollectData.RunMode = 0;
			printf("ֹͣ�ϴ�����\r\n");
			break;
		}
	}
}
//---
/*
	�������� void Project_DebugRTU((u8 *p)
	������		�ú���ΪProject�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*/
void Collect_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//�鿴���ĸ�������
	{
		//-----------------------------------------
		case 0x01:
		{//��λϵͳ
			NVIC_SystemReset();		//ϵͳ�����λ
			break;
		}
		case 0x02:
		{//��λProjec_SysBuffer���е�����
			Project_ClearProjectSysBuffer();
			break;
		}
		case 0x03:
		{//
			
			break;
		}
		case 0x04:
		{//��������0x04
			break;
		}
	}
}
//---




