/*
	���ļ�Ϊ����Ŀ��STM32-Flash�ڲ��洢������غ���
	���庯������
*/

#include "flashisp.h"
#include "FM24CL64.h"

Flashisp_DataStructure FlashispData;
//---
/*
	��������void Flashisp_main(void)
	������	flashisp.c�ļ�������
*/
void Flashisp_main(void)
{
	switch(FlashispData.RunMode)
	{
		case 0:
		{
			if((FlashispData.Flag)) //�жϱ�������
			{//�޸�Flish���������
				#ifdef FLASH_USE_FM24CL64
				FM24CL64_WriteData();
				FlashispData.Flag=0;
     				
				#else
				Flashisp_StoreData();
				#endif
				
				printf("\r\n��Flash�ɹ�\r\n");
			}
			break;
		}
		case 1:
		{//��ʽ������
				{
				#ifndef FLASH_USE_FM24CL64
				FLASH_Unlock();  //����FLASH��̲���������
				FLASH_DataCacheCmd(DISABLE);
				FLASH_EraseSector(STMFLASH_GetFlashSector(FLASHIS_AdrStoreStart),VoltageRange_3);//VCC=2.7~3.6V֮��!!
				FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
				FLASH_Lock();    //����FLASH��̲���������
				FlashispData.Flag = 1;
				#endif
				FlashispData.RunMode = 0;
			}
			break;
		}
	}
}
/*
	��������void Flashisp_Init(void)
	������	flashisp.c�ļ���ʼ������
*/
void Flashisp_Init(void)
{
#ifdef FLASH_USE_FM24CL64
	// FM24CL64_Init();
	 FM24CL64_ReadData();//��ȡ����
	if(FlashispData.Data[FLASHIS_AdrStoreFactory] != FLASHIS_FACTORY)
	{
		Flashisp_FactoryReset(1);//�ָ���������
		Flashisp_DataInit();
	}	
	else
	{
	FlashispData.Flag = 0;  //û���ݸ���
	FlashispData.RunMode = 0;
	}
	#else 
	if(*(u16*)(FLASHIS_AdrStoreStart + ((u16)FLASHIS_AdrStoreFactory)*2) != FLASHIS_FACTORY)
	Flashisp_FactoryReset();//�ָ���������
	
	Flashisp_DataInit();
	#endif
}
//---
/*
	��������void Flashisp_DataInit(void)
	������	�ú���ΪFlaships.c�ļ������ݳ�ʼ��
*/
void Flashisp_DataInit(void)
{
	#ifdef FLASH_USE_FM24CL64
	FM24CL64_ReadData();
	#else 
	u16 i;
	u16 *p = (u16*)(FLASHIS_AdrStoreStart);
	for(i=0;i<FLASHIS_AdrEnd;i++)
	{
		FlashispData.Data[i] = *p;
		p++;
	}
	#endif
	FlashispData.Flag = 0;
	FlashispData.RunMode = 1;
}
//---
/*
	��������Flashisp_DataStructure* Flashisp_GetData(void)
	������	�ú���Ϊ��ȡFlashispData�ṹ������׵�ַ
*/

Flashisp_DataStructure* Flashisp_GetData(void)
{
	return &FlashispData;
}
//---
/*
	��������void Flashisp_FactoryReset(void)
	������	�ú���Ϊ������ϵͳ�洢���ݻָ���������
*/
void Flashisp_FactoryReset(u8 select)
{
	u16 i;
	FourBytes four;
	struct tm myTm;
    u8 *addr=(u8*)"��ɽ��¡�������Ƽ����޹�˾";
	
//	EightBytes eight;
	if(select)
	for(i=0;i<FLASHIS_AdrEnd;i++) FlashispData.Data[i] = 0;
	//ϵͳ����
	four.FloatData = 25.5;			//�����¶�
	FlashispData.Data[FLASHIS_AdrXTSZHJWD] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrXTSZHJWD+1] = four.U16Data[1];
	FlashispData.Data[FLASHIS_AdrXTSZHJWDKG] = 0;	//�����¶Ȳ���/����-�ر�
	four.FloatData = 103.5;			//����ѹ
	FlashispData.Data[FLASHIS_AdrXTSZDQY] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrXTSZDQY+1] = four.U16Data[1];

	for(i=0;i<15;i++) //ϵͳ������ַ
	FlashispData.Data[FLASHIS_AdrXTSZCYDZ+i]=(addr[2*i+1]<<8)+addr[2*i];;
	FlashispData.Data[FLASHIS_AdrXTSZBOX] = 0;	//Boxѡ�񿪹�
	FlashispData.Data[FLASHIS_AdrXTSZZDPSJG] = 30;	//��ˮ���
	FlashispData.Data[FLASHIS_AdrXTSZZDPSSC] = 10;	//��ˮʱ��
  FlashispData.Data[FLASHIS_AdrXTSZYQCLSJ] = 10;//ϵͳ����������ʱ��
	FlashispData.Data[FLASHIS_AdrXTSZYQCLCS] = 11;//ϵͳ��������������
	FlashispData.Data[FLASHIS_AdrXTSZCHSLSJ] = 12;//ϵͳ��⺬ʪ��ʱ��

	//�̵���������
	
	//�̵���������
	
    FlashispData.Data[FLASHIS_AdrYDBDLX]=1;		//�̵���������  1=Բ��  2=���� 3=��������
    four.FloatData=1.0;
		FlashispData.Data[FLASHIS_AdrYDBDYXNJ]=four.U16Data[0];				     //�̵�����Բ���ھ�D
		FlashispData.Data[FLASHIS_AdrYDBDYXNJ+1]=four.U16Data[1];				     //�̵�����Բ���ھ�D
		four.FloatData=65.358;
		FlashispData.Data[FLASHIS_AdrYDBD_MJ]=four.U16Data[0];							//�̵�����Բ�����
		FlashispData.Data[FLASHIS_AdrYDBD_MJ+1]=four.U16Data[1];	
    four.FloatData=1.00;		
		FlashispData.Data[FLASHIS_AdrYDBD_GT]=four.U16Data[0];	;						//�̵�����Բ�ι���
		FlashispData.Data[FLASHIS_AdrYDBD_GT+1]=four.U16Data[1];	;						//�̵�����Բ�ι���
		FlashispData.Data[FLASHIS_AdrYDBDYXHS]=0;						//�̵�����Բ�λ���
		FlashispData.Data[FLASHIS_AdrYDBDYXKS]=1;						//�̵�����Բ�ο���
		FlashispData.Data[FLASHIS_AdrYDBD_CYDS]=1;					//�̵�����Բ�β�������
    
		four.FloatData=1.00;
		FlashispData.Data[FLASHIS_AdrYDBDFXBA]=four.U16Data[0];					//�̵����㷽��һ��A
		FlashispData.Data[FLASHIS_AdrYDBDFXBA+1]=four.U16Data[1];					//�̵����㷽��һ��A
		four.FloatData=1.00;
		FlashispData.Data[FLASHIS_AdrYDBDFXBB]=four.U16Data[0];					//�̵����㷽��һ��B
		FlashispData.Data[FLASHIS_AdrYDBDFXBB+1]=four.U16Data[1];					//�̵����㷽��һ��B	
		FlashispData.Data[FLASHIS_AdrYDBDFXKS]=1;					//�̵����㷽�ο���
		FlashispData.Data[FLASHIS_AdrYDBDFXDKCD]=1;				//�̵����㷽�ε��ײ��
     four.FloatData=0.00;
		 for(i=0;i<10;i++)
		{
		FlashispData.Data[FLASHIS_AdrYDBDCDJL1+2*i]=four.U16Data[0];					//�̵����㷽��һ��A
		FlashispData.Data[FLASHIS_AdrYDBDCDJL1+1+2*i]=four.U16Data[1];					//�̵����㷽��һ��A
		
		}

    //����Ԥ�� ѹ������
		FlashispData.Data[FLASHIS_AdrGKYC_YLTL_YQDY]=1;		                       //ѹ�����㣬������ѹ
		four.FloatData=123.01;
		FlashispData.Data[FLASHIS_AdrGKYC_YLTL_YQJY]=four.U16Data[0];					//ѹ�����㣬������ѹ
		FlashispData.Data[FLASHIS_AdrGKYC_YLTL_YQJY+1]=four.U16Data[1];					//ѹ�����㣬������ѹ
		four.FloatData=122.01;
		FlashispData.Data[FLASHIS_AdrGKYC_YLTL_JQYL]=four.U16Data[0];					//ѹ�����㣬��ǰѹ��
		FlashispData.Data[FLASHIS_AdrGKYC_YLTL_JQYL+1]=four.U16Data[1];					//ѹ�����㣬��ǰѹ��
		FlashispData.Data[FLASHIS_AdrGKYC_YLTL_CLY]=20;					//ѹ�����㣬����ѹѹ
		FlashispData.Data[FLASHIS_AdrGKYC_YLTL_QLY]=21;				//ѹ�����㣬����ѹѹ

		//����Ԥ�� ���²���
		FlashispData.Data[FLASHIS_AdrGKYC_YWCL_FS]=0;				//���·�ʽ 0���� 1����
		four.FloatData=89.1;
		FlashispData.Data[FLASHIS_AdrGKYC_YWCL_SRYW]=four.U16Data[0];					//��������
		FlashispData.Data[FLASHIS_AdrGKYC_YWCL_SRYW+1]=four.U16Data[1];
		
		
//	//����Ԥ�� ��ѯƽ��ֵ
//		 four.U32Data=1;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_WJH]=four.U16Data[0];					//�ļ���
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_WJH+1]=four.U16Data[1];					
//			four.U32Data=120;
//			
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJDY]=four.U16Data[0];					//ƽ����ѹ
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJDY+1]=four.U16Data[1];					//ƽ����ѹ
//    four.FloatData=121.11;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJJY]=four.U16Data[0];					//ƽ����ѹ
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJJY+1]=four.U16Data[1];					//ƽ����ѹ
//     four.FloatData=121.21;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_DQY]=four.U16Data[0];					//����ѹ
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_DQY+1]=four.U16Data[1];					//����ѹ
//     four.FloatData=121.3131;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_YDJMJ]=four.U16Data[0];					//�̵������
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_YDJMJ+1]=four.U16Data[1];					//�̵��ؽ���
//      four.FloatData=121.4;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_CZZJ]=four.U16Data[0];					//����ֱ��
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_CZZJ+1]=four.U16Data[1];					//����ֱ��

//      four.FloatData=121.5;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJYW]=four.U16Data[0];					//ƽ�������¶�
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJYW+1]=four.U16Data[1];					//ƽ��������
//		
//		 four.FloatData=121.7;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJLS]=four.U16Data[0];					//ƽ������
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJLS+1]=four.U16Data[1];					//ƽ������
//		
//		 four.U32Data=124;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_YGLL]=four.U16Data[0];					//��������
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_YGLL+1]=four.U16Data[1];					//��������
//		four.U32Data=125;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_BGLL]=four.U16Data[0];					//�������
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_BGLL+1]=four.U16Data[1];					
    //�⺬ʪ��
	 // FlashispData.Data[FLASHIS_AdrCHSL_LX]=1;  	//�⺬ʪ������ 
		four.FloatData=5;
//		FlashispData.Data[FLASHIS_AdrCHSL_CLZ]=four.U16Data[0];					//����ֵ
////		FlashispData.Data[FLASHIS_AdrCHSL_CLZ+1]=four.U16Data[1];				
////		four.FloatData=0.2;
		FlashispData.Data[FLASHIS_AdrCHSL_HSL]=four.U16Data[0];					//����ֵ
		FlashispData.Data[FLASHIS_AdrCHSL_HSL+1]=four.U16Data[1];	
//    //��������
		FlashispData.Data[FLASHIS_AdrCLYQ_CLFS] = 0;	//����������ʽ
    FlashispData.Data[FLASHIS_AdrCLYQ_JSSC] = 10;	//��ʱʱ��
//		//�������� ����У׼
//		for(i=0;i<8;i++)
//		{
//		 four.FloatData=0.0;
//		 FlashispData.Data[FLASHIS_AdrCLYQ_CJO2+i*2]=four.U16Data[0];					//O2 У��
//	   FlashispData.Data[FLASHIS_AdrCLYQ_CJO2+i*2+1]=four.U16Data[1];
//		}
   //�������� -�����е�λ����

	FlashispData.Data[FLASHIS_AdrCLYQ_DWLX] =	0;	
	//�������� -�ļ���ѯ - ����ϵ��
	four.FloatData=0.1;
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_JS_ZSXS]=four.U16Data[0];					//����ϵ��
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_JS_ZSXS+1]=four.U16Data[1];	
	four.FloatData=0.2;
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_JS_JZYHL]=four.U16Data[0];					//��׼��������
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_JS_JZYHL+1]=four.U16Data[1];	
	four.FloatData=0.3;
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_JS_FHXS]=four.U16Data[0];					//����ϵ��
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_JS_FHXS+1]=four.U16Data[1];	
	//�������� -�ļ���ѯ - ��ӡ����
	four.U32Data=0xFFFFffff;
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_DYXX]=four.U16Data[0];					//��ӡѡ��
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_DYXX+1]=four.U16Data[1];	
			//�̳��ɼ� -�ļ���ѯ - ��ӡ����
	four.U32Data=0xFFFFffff;
	FlashispData.Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX]=four.U16Data[0];					//��ӡѡ��
	FlashispData.Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+1]=four.U16Data[1];	
		//�������� -�ļ���ѯ - ��������
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_FZSJ_PJJS]=0;   //�Ƿ����ƽ������
	//�̳������� -��������
	four.FloatData=10.0;
	FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_CYLS]=four.U16Data[0];					//��ӡѡ��
	FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_CYLS+1]=four.U16Data[1];	
	four.U32Data=01;
	FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_LTH]=four.U16Data[0];					//��ӡѡ��
	FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_LTH+1]=four.U16Data[1];
	four.FloatData=0.84f;
	FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_CZZJ]=four.U16Data[0];					//��ӡѡ��
	FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_CZZJ+1]=four.U16Data[1];
    FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_CYFS]=0;
    FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_DDCS]=600;//8min High8 Min Low8 Sec	
	four.FloatData=0.0;
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSRYND]=four.U16Data[0];					 //�ļ���ѯ�� ���� ��ҺŨ��
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSRYND+1]=four.U16Data[1];	
	four.FloatData=0.0;
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSDRTJ]=four.U16Data[0];					//�ļ���ѯ�� ���� �������
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSDRTJ+1]=four.U16Data[1];	
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSZTSM]=1;//�ļ���ѯ�� ���� ��ͷ��Ŀ
   
	 
	 //�ļ���ѯ - ����/�̳�
	four.FloatData=1.0;
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSYCZL]=four.U16Data[0];					//�ļ���ѯ�� ���� �̳�����
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSYCZL+1]=four.U16Data[1];	
	
	four.FloatData=21.0;
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSHYL]=four.U16Data[0];					//�ļ���ѯ�� ���� ��������
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSHYL+1]=four.U16Data[1];	
	
	four.FloatData=1.0;
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSFHXS]=four.U16Data[0];					//�ļ���ѯ�� ���� ����ϵ��
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSFHXS+1]=four.U16Data[1];	
	
	four.FloatData=1.0;
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSZSXS]=four.U16Data[0];					//�ļ���ѯ�� ���� ����ϵ��
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSZSXS+1]=four.U16Data[1];	
	
	four.FloatData=1.0;
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSJZYHL]=four.U16Data[0];					//�ļ���ѯ�� ���� ��׼��������
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSJZYHL+1]=four.U16Data[1];	
	

	//------------------------------------------------------------------------
	//�����������
	FlashispData.Data[FLASHIS_AdrCJKSSK]=123;
	FlashispData.Data[FLASHIS_AdrCJKSSK+1]=456;
	FlashispData.Data[FLASHIS_AdrCJLJSH]=4556;
	FlashispData.Data[FLASHIS_AdrCJLJSH+1]=0;
	FlashispData.Data[FLASHIS_AdrCJGKTJ]=27;
	FlashispData.Data[FLASHIS_AdrCJBQTJ]=21;
	FlashispData.Data[FLASHIS_AdrCJCNT]=10;
	FlashispData.Data[FLASHIS_AdrCJMOD]=0;
	//------------------------------------------------------------------------
	
	//ϵͳά�����
	//ϵͳά����� ----------ϵͳ����
	four.FloatData = 5.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_NO2_NOX] = four.U16Data[0]; //У׼ά�� ϵͳ���� NO2/NOx  50/10.0/100.0=5.0%
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_NO2_NOX+1] = four.U16Data[1];
	four.FloatData = 0.6;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2] = four.U16Data[0]; //У׼ά�� ϵͳ���� CO��SO2����  60/100.0/100.0=0.60%
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2+1] = four.U16Data[1];
	four.FloatData = 3.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_NO2_SO2] = four.U16Data[0];//У׼ά�� ϵͳ���� NO2��SO2����  -300/100.0=-3.00
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_NO2_SO2+1] = four.U16Data[1];
	four.FloatData = 0.84;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_PTG_KP] = four.U16Data[0];//У׼ά�� ϵͳ���� Ƥ�й�   84/100.0=0.84
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_PTG_KP+1] = four.U16Data[1];
	four.FloatData = 1.34;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_YQMD] = four.U16Data[0];//У׼ά�� ϵͳ���� �����ܶ�  134/100.0=1.34kg/m3
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_YQMD+1] = four.U16Data[1];

	four.FloatData = 1.75;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_ZSXS] = four.U16Data[0];//У׼ά�� ϵͳ���� ����ϵ��  175/100.0=1.75
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_ZSXS+1] = four.U16Data[1];
	four.FloatData = 9.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_JZYHL] = four.U16Data[0];//У׼ά�� ϵͳ����  ��׼������  90/10.0=9.0
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_JZYHL+1] = four.U16Data[1];
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ] = 0x03;        //У׼ά�� SO2 ����ѡ��  1 CO 2 NO2

    FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1]=62;    //У׼ά�� ϵͳ���� CO��SO2���� COŨ��1
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND2]=125;    //У׼ά�� ϵͳ���� CO��SO2���� COŨ��2
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND3]=375;    //У׼ά�� ϵͳ���� CO��SO2���� COŨ��3
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND4]=625;    //У׼ά�� ϵͳ���� CO��SO2���� COŨ��4
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND5]=1250;    //У׼ά�� ϵͳ���� CO��SO2���� COŨ��5
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND6]=1875;    //У׼ά�� ϵͳ���� CO��SO2���� COŨ��6
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND7]=2500;    //У׼ά�� ϵͳ���� CO��SO2���� COŨ��7
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND8]=3750;    //У׼ά�� ϵͳ���� CO��SO2���� COŨ��8
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND9]=5000;    //У׼ά�� ϵͳ���� CO��SO2���� COŨ��9
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND10]=10000;    //У׼ά�� ϵͳ���� CO��SO2���� COŨ��10
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ] = 0x03;        //У׼ά�� SO2 ����ѡ��  1 CO 2 NO2

	four.FloatData = 0.5;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ1] = four.U16Data[0];///У׼ά�� ϵͳ���� CO��SO2����ϵ��1 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ1+1] = four.U16Data[1];
	four.FloatData = 0.5;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ2] = four.U16Data[0];///У׼ά�� ϵͳ���� CO��SO2����ϵ��2 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ2+1] = four.U16Data[1];
	four.FloatData = 0.5;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ3] = four.U16Data[0];///У׼ά�� ϵͳ���� CO��SO2����ϵ��3 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ3+1] = four.U16Data[1];
	four.FloatData = 0.5;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ4] = four.U16Data[0];///У׼ά�� ϵͳ���� CO��SO2����ϵ��4 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ4+1] = four.U16Data[1];
	four.FloatData = 0.5;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ5] = four.U16Data[0];///У׼ά�� ϵͳ���� CO��SO2����ϵ��5 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ5+1] = four.U16Data[1];
	four.FloatData = 0.6;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ6] = four.U16Data[0];///У׼ά�� ϵͳ���� CO��SO2����ϵ��6 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ6+1] = four.U16Data[1];
	four.FloatData = 0.6;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ7] = four.U16Data[0];///У׼ά�� ϵͳ���� CO��SO2����ϵ��7 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ7+1] = four.U16Data[1];
	four.FloatData = 0.6;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ8] = four.U16Data[0];///У׼ά�� ϵͳ���� CO��SO2����ϵ��8 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ8+1] = four.U16Data[1];
	four.FloatData = 0.6;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ9] = four.U16Data[0];///У׼ά�� ϵͳ���� CO��SO2����ϵ��9 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ9+1] = four.U16Data[1];
	four.FloatData = 0.6;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ10] = four.U16Data[0];///У׼ά�� ϵͳ���� CO��SO2����ϵ��10 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ10+1] = four.U16Data[1];
	 //ϵͳά��-->����У׼
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ] = 0x01;        ////�̳�����ѡ��
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_BZLL] = four.U16Data[0]; //��׼����
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_BZLL+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL01] = four.U16Data[0]; //�̳��궨����1
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL01+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL02] = four.U16Data[0];//�̳��궨����2
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL02+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL03] = four.U16Data[0];//�̳��궨����3
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL03+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL04] = four.U16Data[0];//�̳��궨����4
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL04+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL05] = four.U16Data[0];//�̳��궨����5
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL05+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL06] = four.U16Data[0];//�̳��궨����6
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL06+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL07] = four.U16Data[0];//�̳��궨����7
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL07+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL08] = four.U16Data[0];//�̳��궨����8
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL08+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL09] = four.U16Data[0];//�̳��궨����9
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL09+1] = four.U16Data[1];

//	four.FloatData = 1.0;
//	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL10] = four.U16Data[0];//�̳��궨����10
//	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL10+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YQBL01] = four.U16Data[0];//�����궨����1
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YQBL01+1] = four.U16Data[1];

//ϵͳά��-->Ũ��У׼
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDO2] = four.U16Data[0]; //O2���ֵ1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDO2+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDSO2] = four.U16Data[0]; //SO2���ֵ1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDSO2+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDNO] = four.U16Data[0];//NO���ֵ1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDNO+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDNO2] = four.U16Data[0];//NO2���ֵ1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDNO2+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDCO] = four.U16Data[0];//CO���ֵ1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDCO+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDH2S] = four.U16Data[0];//H2S���ֵ1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDH2S+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDCO2] = four.U16Data[0];//CO2���ֵ1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDCO2+1] = four.U16Data[1];

	//---
		four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLO2_1] = four.U16Data[0]; //O2����ֵ1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLO2_1+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLSO2_1] = four.U16Data[0]; //SO2����ֵ1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLSO2_1+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO_1] = four.U16Data[0];//NO����ֵ1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO_1+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO2_1] = four.U16Data[0];//NO2����ֵ1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO2_1+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO_1] = four.U16Data[0];//CO����ֵ1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO_1+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLH2S_1] = four.U16Data[0];//H2S����ֵ1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLH2S_1+1] = four.U16Data[1];
	four.FloatData = 1.2;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO2_1] = four.U16Data[0];//CO2����ֵ1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO2_1+1] = four.U16Data[1];
	//---
		four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLO2_2] = four.U16Data[0]; //O2����ֵ2
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLO2_2+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLSO2_2] = four.U16Data[0]; //SO2����ֵ2
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLSO2_2+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO_2] = four.U16Data[0];//NO����ֵ2
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO_2+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO2_2] = four.U16Data[0];//NO2����ֵ2
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO2_2+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO_2] = four.U16Data[0];//CO����ֵ2
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO_2+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLH2S_2] = four.U16Data[0];//H2S����ֵ2
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLH2S_2+1] = four.U16Data[1];
	four.FloatData = 1.2;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO2_2] = four.U16Data[0];//CO2����ֵ2
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO2_2+1] = four.U16Data[1];
		//---
		four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLO2_3] = four.U16Data[0]; //O2����ֵ3
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLO2_3+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLSO2_3] = four.U16Data[0]; //SO2����ֵ3
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLSO2_3+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO_3] = four.U16Data[0];//NO����ֵ3
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO_3+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO2_3] = four.U16Data[0];//NO2����ֵ3
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO2_3+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO_3] = four.U16Data[0];//CO����ֵ3
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO_3+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLH2S_3] = four.U16Data[0];//H2S����ֵ3
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLH2S_3+1] = four.U16Data[1];
	four.FloatData = 1.2;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO2_3] = four.U16Data[0];//CO2����ֵ3
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO2_3+1] = four.U16Data[1];
  //ά��У׼--->ѹ��У׼, modify by alfred, 2020.07.13
		four.FloatData = 500.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_DQY] = four.U16Data[0];//����ѹ���ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_DQY+1] = four.U16Data[1];
			four.FloatData = 500.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_JJY] = four.U16Data[0];//��ѹ���ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_JJY+1] = four.U16Data[1];
			four.FloatData = 500.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_JY] = four.U16Data[0];//��ѹ���ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_JY+1] = four.U16Data[1];
			four.FloatData = 500.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_DY] = four.U16Data[0];//��ѹ���ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_DY+1] = four.U16Data[1];
			four.FloatData = 500.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_CLY] = four.U16Data[0];//����ѹ���ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_CLY+1] = four.U16Data[1];
			four.FloatData = 500.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_QLY] = four.U16Data[0];//����ѹ���ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_QLY+1] = four.U16Data[1];
  //ά��У׼--->����������ѡ��
  FlashispData.Data[FLASHIS_AdrWHJZ_QTCGQXZ]=0x1f1f;
	//ά��У׼--->�¶�У׼, modify by alfred, 2020.07.13
	four.FloatData = 50.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDGQ] = four.U16Data[0];//�������ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDGQ+1] = four.U16Data[1];	
	four.FloatData = 50.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDYW] = four.U16Data[0];//�������ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDYW+1] = four.U16Data[1];		
	four.FloatData = 50.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDSQ] = four.U16Data[0];//ʪ�����ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDSQ+1] = four.U16Data[1];
	four.FloatData = 50.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDJW] = four.U16Data[0];//�������ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDJW+1] = four.U16Data[1];
	four.FloatData = 50.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDHW] = four.U16Data[0];//��ѹ���ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDHW+1] = four.U16Data[1];
	
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLGQ] = four.U16Data[0];//������ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLGQ+1] = four.U16Data[1];	
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLYW] = four.U16Data[0];//���±���ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLYW+1] = four.U16Data[1];	
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLSQ] = four.U16Data[0];//ʪ����ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLSQ+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLJW] = four.U16Data[0];//���±���ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLJW+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLHW] = four.U16Data[0];//��ѹ����ֵ
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLHW+1] = four.U16Data[1];

  	//ά��У׼--->�¶�У׼
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_DCJZ_BL] = four.U16Data[0];//��ر���
	FlashispData.Data[FLASHIS_AdrWHJZ_DCJZ_BL+1] = four.U16Data[1];	  
	//ϵͳά������
	FlashispData.Data[FLASHIS_AdrXTWHMM] = 2012;
	//�������
	FlashispData.Data[FLASHIS_AdrXTWHLYPDM] = 2218;//���������
	FlashispData.Data[FLASHIS_AdrXTWHAPPLYPDM] = 2218;//���������
	//������ʾ����
	FlashispData.Data[FLASHIS_AdrXTWHLLXS] = 'R';
	//��ѹ����
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrXTWHDYBL] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrXTWHDYBL+1] = four.U16Data[1];
	//������������
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrCYSZALCYLL] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrCYSZALCYLL+1] = four.U16Data[1];
  FlashispData.Data[FLASHIS_AdrCYSZBLCYLL] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrCYSZBLCYLL+1] = four.U16Data[1];
	FlashispData.Data[FLASHIS_AdrCYSZCLCYLL] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrCYSZCLCYLL+1] = four.U16Data[1];
	//���ȿ���
	FlashispData.Data[FLASHIS_AdrXTWHFSKG] = 0;
	//��������
	four.U32Data = 1234567;
	FlashispData.Data[FLASHIS_AdrXTWHBJHMQD] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrXTWHBJHMQD+1] = four.U16Data[1];
	four.U32Data = 123456;
	FlashispData.Data[FLASHIS_AdrXTWHBJHMHD] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrXTWHBJHMHD+1] = four.U16Data[1];
	//------------------------------------------------------------------------
	//ϵͳ����ʱ�䡢��������װʱ�����
	//------------------------------------------------------------------------
	//����ʱ����Ϣ
	four.U32Data = 120;
	FlashispData.Data[FLASHIS_AdrYQYXSJ] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrYQYXSJ+1] = four.U16Data[1];
	four.U32Data = 5678;
	FlashispData.Data[FLASHIS_AdrCBYXSJ] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrCBYXSJ+1] = four.U16Data[1];
//------------------------------------------------------------------------------------
//��������װʱ��
   myTm.tm_year=2019;
	 myTm.tm_mon=11;
	 myTm.tm_mday=28;
   four.U16Data[0] = ((myTm.tm_mon)<<8)+myTm.tm_mday;
	 four.U16Data[1] = myTm.tm_year;
	 FlashispData.Data[FLASHIS_AdrO2AZSJ] = four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrO2AZSJ+1] = four.U16Data[1];
	 FlashispData.Data[FLASHIS_AdrSOANSJ] = four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrSOANSJ+1] = four.U16Data[1];
	 FlashispData.Data[FLASHIS_AdrNOANSJ] = four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrNOANSJ+1] = four.U16Data[1];
	 FlashispData.Data[FLASHIS_AdrNO2ANSJ] = four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrNO2ANSJ+1] = four.U16Data[1];
	 FlashispData.Data[FLASHIS_AdrCOANSJ] = four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrCOANSJ+1] = four.U16Data[1];
	 FlashispData.Data[FLASHIS_AdrH2SNSJ] = four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrH2SNSJ+1] = four.U16Data[1];
	 FlashispData.Data[FLASHIS_AdrCO2ANSJ] = four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrCO2ANSJ+1] = four.U16Data[1];
// ���̳�ʼ��
  //����0~24
	 four.U32Data=24;
	 FlashispData.Data[FLASHIS_AdrO2LQ]=four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrO2LQ+1]=four.U16Data[1];
	 //�������� 0~5700
	 four.U32Data=5700;
	 FlashispData.Data[FLASHIS_AdrSO2LQ]=four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrSO2LQ+1]=four.U16Data[1];
	  //һ������0~1300
	 four.U32Data=1300;
	 FlashispData.Data[FLASHIS_AdrNOLQ]=four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrNOLQ+1]=four.U16Data[1];
	 //�������� 0~200
	 four.U32Data=200;
	 FlashispData.Data[FLASHIS_AdrNO2LQ]=four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrNO2LQ+1]=four.U16Data[1];
	 
	 //һ����̼ 0~5000
	 four.U32Data=5000;
	 FlashispData.Data[FLASHIS_AdrCOLQ]=four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrCOLQ+1]=four.U16Data[1];
	 
	  //����0~200
	 four.U32Data=200;
	 FlashispData.Data[FLASHIS_AdrH2SLQ]=four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrH2SLQ+2]=four.U16Data[1];
	 
	 //������̼
	 four.U32Data=20;
	 FlashispData.Data[FLASHIS_AdrCO2LQ]=four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrCO2LQ+1]=four.U16Data[1];

	 
	//����ά�����
	//����ά������
	FlashispData.Data[FLASHIS_AdrCJWHMM] = 8088;
	//�������
	four.U32Data = 12345678;
	FlashispData.Data[FLASHIS_AdrCJWHYQBH] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrCJWHYQBH+1] = four.U16Data[1];
	//����汾��
	four.FloatData = 1.02;
	FlashispData.Data[FLASHIS_AdrCJWHRJBB] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrCJWHRJBB+1] = four.U16Data[1];

	
	//���������ʼ��
	if(select)
	Flashisp_Backup();
	
	//�������ñ�־λ
	FlashispData.Data[FLASHIS_AdrStoreFactory] = FLASHIS_FACTORY;
	#ifdef FLASH_USE_FM24CL64
	FM24CL64_WriteData();
	#else
	Flashisp_StoreData();//�޸�����
	#endif

	Screen_GetData()->gSysConfigWaringBits.BitField.m_factorySet = 1;
	printf("ϵͳ�ѻָ���������\r\n");
}
//---
/*
	��������void Flashisp_Backup(void)
	������	�ú���Ϊ������Ҫ���ݵ��������
*/
void Flashisp_Backup(void)
{   u16 bs=FLASHIS_AdrBackupStart;

	u16 dbs=FLASHIS_AdrBDStart;
	u16 i,len;
	len =FLASHIS_AdrBDEnd-FLASHIS_AdrBDStart;
	
	for(i=0;i<len;i++)
	{
		FlashispData.Data[bs+i] = FlashispData.Data[dbs+i];
	}
	FlashispData.Flag = 1;
}
//---
/*
	��������void Flashisp_Restore(void)
	������	�ú���Ϊ��ԭ���ݵ����ݵ��������
*/
void Flashisp_Restore(void)
{
	u16 bs=FLASHIS_AdrBackupStart;

	u16 dbs=FLASHIS_AdrBDStart;
	u16 i,len;
	len =FLASHIS_AdrBDEnd-FLASHIS_AdrBDStart;
	for(i=0;i<len;i++)
	{
		FlashispData.Data[dbs+i] = FlashispData.Data[bs+i];
	}
	FlashispData.Flag = 1;
	Slave_ReadFlashData();//����ʵʱ����
}
//---
//-------------------------���ݶ�ȡ���洢----------------------------
//---
/*
	��������void Flashisp_StoreData(void)
	�������ú���Ϊ�洢��ϵͳ����Flash����
	˵��������
				��ȡ����
				��������
				�޸�����
				��������
				����
*/
void Flashisp_StoreData(void)
{
	u16 i;
	u32 adr = FLASHIS_AdrStoreStart;//�׵�ַ
	FLASH_Unlock();  //����FLASH��̲���������
	FLASH_DataCacheCmd(DISABLE);
	FLASH_EraseSector(STMFLASH_GetFlashSector(FLASHIS_AdrStoreStart),VoltageRange_3);//VCC=2.7~3.6V֮��!!
	for(i=0;i<FLASHIS_AdrEnd;i++)
	{
		FLASH_ProgramHalfWord(adr,FlashispData.Data[i]);
		adr += 2;
	}
	FLASH_DataCacheCmd(ENABLE);	//FLASH��������,�������ݻ���
	FLASH_Lock();    //����FLASH��̲���������
	FlashispData.Flag = 0;
//	printf("�޸�����\r\n");
}
//---
//��ȡĳ����ַ���ڵ�flash����
//addr:flash��ַ
//����ֵ:0~11,��addr���ڵ�����
u16 STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
	return FLASH_Sector_11;	
}
//---
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Flashisp_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪFlashisp�ļ���ASCII�����ļ�
						:000|01|xxx
*/
void Flashisp_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:
		{//��ȡFlash����
			
			break;
		}
		case 101:
		{//��ȡA·��ѹ����ֵ��ʵ��ֵ
//			FourBytes four;
			u16* Fp= Flashisp_GetData()->Data;
//			four.U16Data[0] = Fp[FLASHIS_AdrYLBDALCYCYZ];
//			four.U16Data[1] = Fp[FLASHIS_AdrYLBDALCYCYZ+1];
//			printf("A·��ѹ����ֵΪ��%fPa\r\n",four.FloatData*1000);
//			four.U16Data[0] = Fp[FLASHIS_AdrYLBDALCYSJZ];
//			four.U16Data[1] = Fp[FLASHIS_AdrYLBDALCYSJZ+1];
//			printf("A·��ѹʵ��ֵΪ��%fPa\r\n",four.FloatData*1000);
			break;
		}
		case 201:
		{//��ȡϵͳ������ز���
			FourBytes four;
			printf("ά�����룺%04u\r\n",Flashisp_GetData()->Data[FLASHIS_AdrXTWHMM]);
			printf("��������룺%04u\r\n",Flashisp_GetData()->Data[FLASHIS_AdrXTWHLYPDM]);
			printf("������ʾ��%c\r\n",(char)Flashisp_GetData()->Data[FLASHIS_AdrXTWHLLXS]);
			four.U16Data[0] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHDYBL];
			four.U16Data[1] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHDYBL+1];
			printf("��ѹ���ʣ�%f\r\n",four.FloatData);
			printf("���ȿ��أ�%u\r\n",Flashisp_GetData()->Data[FLASHIS_AdrXTWHFSKG]);
			four.U16Data[0] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHBJHMQD];
			four.U16Data[1] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHBJHMQD+1];
			printf("��������ǰ�Σ�%u\r\n",four.U32Data);
			four.U16Data[0] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHBJHMHD];
			four.U16Data[1] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHBJHMHD+1];
			printf("���������Σ�%u\r\n",four.U32Data);
			break;
		}
		case 202:
		{//��ȡ������Ϣ
			FourBytes four;
			struct tm tf;
			four.U16Data[0] = Flashisp_GetData()->Data[FLASHIS_AdrDDTime];
			four.U16Data[1] = Flashisp_GetData()->Data[FLASHIS_AdrDDTime+1];
			printf("������Ϣ��%u\r\n",four.U32Data);
			tf = *localtime(&four.U32Data);//����ʱ��
			printf("����ʱ�䣺%u��%02u��%02u�� %02uʱ%02u��%02u��\r\n",tf.tm_year+SCREEN_YearBase,tf.tm_mon,tf.tm_mday,tf.tm_hour,tf.tm_min,tf.tm_sec);
			break;
		}
		case 111:
		{//��ȡ�������
			break;
		}
		case 901:
		{//�鿴Flash����������
			printf("Flash���������ȣ�%u\r\n",FLASHIS_AdrEnd);
			printf("FLASHIS_AdrStoreFactory=0x%X\r\n",FlashispData.Data[FLASHIS_AdrStoreFactory]);
			break;
		}
		case 902:
		{//�ָ���������
			Flashisp_FactoryReset(1);
			break;
		}
		case 903:
		{//��ȡ����Flash����
			u16 i;
			u16 *p = (u16*)(FLASHIS_AdrStoreStart);
			for(i=0;i<FLASHIS_AdrEnd;i++)
			{
				printf("%p    0x%X\r\n",p,*p);
				p++;
			}
			printf("\r\n");
			break;
		}
		case 904:
		{//��������
			Flashisp_Backup();
			printf("�����ѱ���\r\n");
			break;
		}
		case 905:
		{//�ָ�����
			Flashisp_Restore();
			printf("�����ѻָ�\r\n");
			break;
		}
		case 906:
		{

			
     
	}
}
}
//---



