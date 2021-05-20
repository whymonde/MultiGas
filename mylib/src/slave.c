/*
	���ļ�Ϊ����Ŀ����ӻ�������غ���
	���庯������
*/

#include "slave.h"
#include "CalculationFormula.h"
Slave_DataStructure SlaveData;
/*
	��������void Slave_Main(void)
	������	heat.c������
*/
void Slave_Main(void)
{
	
	Slave_Communicate();  //�·����ݣ����������(�ɼ����ݹ���)
	Slave_GetSensorData();//�ѵõ������ݽ�������ת��
	//Slave_MotorRun(&SlaveData.AMotor,Flux_GetData()->ChannelA.ShowFlux);
	Slave_MotorRun(&SlaveData.DustMotor,Flux_GetData()->ChannelB.ShowFlux);
    Slave_MotorRun(&SlaveData.GasMotor,Flux_GetData()->ChannelC.ShowFlux);
}
//---
/*
	��������void Slave_Init(void)
	������	slave.c�ļ���ʼ������
*/
void Slave_Init(void)
{
	Slave_InitComm();
	Slave_InitRawData();

	//��ʼ������
	Slave_InitActualData(&SlaveData.HWTem);
	Slave_InitActualData(&SlaveData.GQTem);
	Slave_InitActualData(&SlaveData.SQTem);
	Slave_InitActualData(&SlaveData.YWTem);
	Slave_InitActualData(&SlaveData.JWTem);
	
	Slave_InitActualData(&SlaveData.DQPress);
	Slave_InitActualData(&SlaveData.ACPress);
	Slave_InitActualData(&SlaveData.AGPress);
	Slave_InitActualData(&SlaveData.BCPress);
	Slave_InitActualData(&SlaveData.BGPress);
	Slave_InitActualData(&SlaveData.CCPress);
	
	Slave_InitActualData(&SlaveData.O2Concent);
	Slave_InitActualData(&SlaveData.SO2Concent);
	Slave_InitActualData(&SlaveData.NOConcent);
	Slave_InitActualData(&SlaveData.NO2Concent);
	Slave_InitActualData(&SlaveData.COConcent);
	Slave_InitActualData(&SlaveData.H2SConcent);
	Slave_InitActualData(&SlaveData.CO2Concent);
	
	
	Slave_InitActualData(&SlaveData.BeforO2Concent);
	Slave_InitActualData(&SlaveData.BeforSO2Concent);
	Slave_InitActualData(&SlaveData.BeforNOConcent);
	Slave_InitActualData(&SlaveData.BeforNO2Concent);
	Slave_InitActualData(&SlaveData.BeforCOConcent);
	Slave_InitActualData(&SlaveData.BeforH2SConcent);
	Slave_InitActualData(&SlaveData.BeforCO2Concent);
	
	

	//����flash����,��ȡ�������
	Slave_ReadFlashData();
	
	
	//�����س�ʼ��
	
	SlaveData.WaterMotor.rame='W';
	SlaveData.SlvdMotor.rame='S';
	SlaveData.GasMotor.rame='G';    //����
	SlaveData.DustMotor.rame='D';   //�̳�
	
	Slave_MotorInit(&SlaveData.WaterMotor);
	Slave_MotorInit(&SlaveData.SlvdMotor);
	Slave_MotorInit(&SlaveData.GasMotor);
	Slave_MotorInit(&SlaveData.DustMotor);

	Slave_MotorPidDataInit(&SlaveData.WaterMotor);            //��PID������ʼ��
	Slave_MotorPidDataInit(&SlaveData.SlvdMotor);
	Slave_MotorPidDataInit(&SlaveData.GasMotor);
	Slave_MotorPidDataInit(&SlaveData.DustMotor);
	
	//E28 2.4Gģ�����ݼĴ�����ʼ��
	SlaveData.E28.STATE=0;
	SlaveData.E28.ADRRRD24G=0;
	SlaveData.E28.CHRD24G=0;
	SlaveData.E28.ADRRset=0;
	SlaveData.E28.CHset=0;
}
//---
/*
	��������void Slave_Timer(void)
	������slave.c�ļ���ʱ����������ϵͳ1ms��ʱ������
*/
void Slave_Timer(void)
{
	if(SlaveData.Comm.Timer != 0)SlaveData.Comm.Timer--;	//ͨѶ��ʱ��
	if(SlaveData.GetTimer != 0)SlaveData.GetTimer--;			//���ݲɼ���ʱ��
	if(SlaveData.GasMotor.Timer!= 0)SlaveData.GasMotor.Timer--;			//���ݲɼ���ʱ��
	if(SlaveData.DustMotor.Timer!= 0)SlaveData.DustMotor.Timer--;			//���ݲɼ���ʱ��

	/*
	//���㶨ʱ��
	if(SlaveData.DQPress.Timer != 0)SlaveData.DQPress.Timer--;  //����ѹ
	if(SlaveData.ACPress.Timer != 0)SlaveData.ACPress.Timer--;
	if(SlaveData.AGPress.Timer != 0)SlaveData.AGPress.Timer--;
	if(SlaveData.BCPress.Timer != 0)SlaveData.BCPress.Timer--;
	if(SlaveData.BGPress.Timer != 0)SlaveData.BGPress.Timer--;
	if(SlaveData.CCPress.Timer != 0)SlaveData.CCPress.Timer--;
	*/
	
	SlaveData.WaterMotor.Pid.Timer++;
	SlaveData.SlvdMotor.Pid.Timer++;
	SlaveData.GasMotor.Pid.Timer++;
	SlaveData.DustMotor.Pid.Timer++;
	
}
//---
/*
	��������Slave_DataStructure* Slave_GetData(void)
	��������ȡ����slave.c�ļ�ȫ�ֱ����ṹ�����
*/
Slave_DataStructure* Slave_GetData(void)
{
	return &SlaveData;
}
//---
/*
	��������void Slave_InitComm(void)
	������	�ú���ΪͨѶ��ʼ��
*/
void Slave_InitComm(void)
{
	SlaveData.Comm.RunMode = 1;
	SlaveData.Comm.ErrorCnt = 20;
}
//---
/*
	��������void Slave_InitRawData(void)
	������	�ú���Ϊ��ز�����ʼ��
*/
void Slave_InitRawData(void)
{
	u8 i;
	for(i=0;i<SLAVE_DataMax;i++)                //��ʼ������������
	{
		SlaveData.Data[i].Filter.Cnt = 0;
		SlaveData.Data[i].Filter.Flag = 0;   //��ʼΪ0����û�б�������������ʱ״̬
		SlaveData.Data[i].Filter.Sum = 0;
		SlaveData.Data[i].Filter.CntMiddle= 0;
		SlaveData.Data[i].Raw = SLAVE_ColFAULT;
	}
}
//---
/*
	��������void Slave_InitActualData(Slave_TPActualStructure *p)
	������	�ú���Ϊ��ʼ�������¶���ز���
*/
#include <stm32f4xx.h>
void Slave_InitActualData(Slave_TPActualStructure *p)
{
	p->Value = SLAVE_ColFAULT;
	p->Mode.All = 0;
	p->Cnt = 0;
	p->Sum = 0;
	p->Max = 0;
	p->Min = 0;
	p->Timer = 0;
	p->OldRaw = 0;
	p->K=1;   //
	p->B=0;   //
}
//---
/*
	��������void Slave_ReadFlashData(void)
	������	�ú���Ϊ��ȡFlash���ݹؼ���ȡ������ݵ����ݽṹ��B
*/
void Slave_ReadFlashData(void)  //�ؼ��ǻ�ȡ�������
{
 //���
  Slave_ReadFlashZOData(&SlaveData.HWTem,FLASHIS_AdrWHJZ_WDJZ_LDHW,SLAVE_BaseTem);
  Slave_ReadFlashZOData(&SlaveData.GQTem,FLASHIS_AdrWHJZ_WDJZ_LDGQ,SLAVE_BaseTem);
  Slave_ReadFlashZOData(&SlaveData.SQTem,FLASHIS_AdrWHJZ_WDJZ_LDSQ,SLAVE_BaseTem);
  Slave_ReadFlashZOData(&SlaveData.YWTem,FLASHIS_AdrWHJZ_WDJZ_LDYW,SLAVE_BaseTem);
  Slave_ReadFlashZOData(&SlaveData.JWTem,FLASHIS_AdrWHJZ_WDJZ_LDJW,SLAVE_BaseTem);
	
  Slave_ReadFlashZOData(&SlaveData.DQPress,FLASHIS_AdrWHJZ_YLJZ_DQY,SLAVE_BaseCPress);
  Slave_ReadFlashZOData(&SlaveData.ACPress,FLASHIS_AdrWHJZ_YLJZ_DY,SLAVE_BaseCPress);
  Slave_ReadFlashZOData(&SlaveData.AGPress,FLASHIS_AdrWHJZ_YLJZ_JY,SLAVE_BaseCPress);
  Slave_ReadFlashZOData(&SlaveData.BCPress,FLASHIS_AdrWHJZ_YLJZ_CLY,SLAVE_BaseCPress);
  Slave_ReadFlashZOData(&SlaveData.BGPress,FLASHIS_AdrWHJZ_YLJZ_JJY,SLAVE_BaseCPress);
  Slave_ReadFlashZOData(&SlaveData.CCPress,FLASHIS_AdrWHJZ_YLJZ_QLY,SLAVE_BaseCPress);
	
  Slave_ReadFlashZOData(&SlaveData.O2Concent,FLASHIS_AdrWHJZ_NDJZ_LDO2,SLAVE_BaseGas);
  Slave_ReadFlashZOData(&SlaveData.SO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDSO2,SLAVE_BaseGas);
  Slave_ReadFlashZOData(&SlaveData.NOConcent,FLASHIS_AdrWHJZ_NDJZ_LDNO,SLAVE_BaseGas);
  Slave_ReadFlashZOData(&SlaveData.NO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDNO2,SLAVE_BaseGas);
  Slave_ReadFlashZOData(&SlaveData.COConcent,FLASHIS_AdrWHJZ_NDJZ_LDCO,SLAVE_BaseGas);
  Slave_ReadFlashZOData(&SlaveData.CO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDCO2,SLAVE_BaseGas);
  Slave_ReadFlashZOData(&SlaveData.H2SConcent,FLASHIS_AdrWHJZ_NDJZ_LDH2S,SLAVE_BaseGas);
  //����
  Slave_ReadFlashBLData(&SlaveData.GQTem,FLASHIS_AdrWHJZ_WDJZ_BLGQ); //������
  Slave_ReadFlashBLData(&SlaveData.JWTem,FLASHIS_AdrWHJZ_WDJZ_BLYW);//���±���
  Slave_ReadFlashBLData(&SlaveData.SQTem,FLASHIS_AdrWHJZ_WDJZ_BLSQ);//ʪ����
  
  Slave_ReadFlashBLData(&SlaveData.O2Concent,FLASHIS_AdrWHJZ_NDJZ_BLO2_1);   //��������������K
  Slave_ReadFlashBLDatak1(&SlaveData.O2Concent,FLASHIS_AdrWHJZ_NDJZ_BLO2_2);  //K1
  Slave_ReadFlashBLDatak2(&SlaveData.O2Concent,FLASHIS_AdrWHJZ_NDJZ_BLO2_3);  //K2  
   
  Slave_ReadFlashBLData(&SlaveData.SO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLSO2_1);   //SO2����������K
  Slave_ReadFlashBLDatak1(&SlaveData.SO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLSO2_2);  //K1
  Slave_ReadFlashBLDatak2(&SlaveData.SO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLSO2_3);  //K2
  
  Slave_ReadFlashBLData(&SlaveData.NOConcent,FLASHIS_AdrWHJZ_NDJZ_BLNO_1);   //NO����������K
  Slave_ReadFlashBLDatak1(&SlaveData.NOConcent,FLASHIS_AdrWHJZ_NDJZ_BLNO_2);  //K1
  Slave_ReadFlashBLDatak2(&SlaveData.NOConcent,FLASHIS_AdrWHJZ_NDJZ_BLNO_3);  //K2 
  
  Slave_ReadFlashBLData(&SlaveData.NO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLNO2_1);   //NO����������K
  Slave_ReadFlashBLDatak1(&SlaveData.NO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLNO2_2);  //K1
  Slave_ReadFlashBLDatak2(&SlaveData.NO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLNO2_3);  //K2 
  
  Slave_ReadFlashBLData(&SlaveData.COConcent,FLASHIS_AdrWHJZ_NDJZ_BLCO_1);   //NO����������K
  Slave_ReadFlashBLDatak1(&SlaveData.COConcent,FLASHIS_AdrWHJZ_NDJZ_BLCO_2);  //K1
  Slave_ReadFlashBLDatak2(&SlaveData.COConcent,FLASHIS_AdrWHJZ_NDJZ_BLCO_3);  //K2 
  
  Slave_ReadFlashBLData(&SlaveData.H2SConcent,FLASHIS_AdrWHJZ_NDJZ_BLH2S_1);   //NO����������K
  Slave_ReadFlashBLDatak1(&SlaveData.H2SConcent,FLASHIS_AdrWHJZ_NDJZ_BLH2S_2);  //K1
  Slave_ReadFlashBLDatak2(&SlaveData.H2SConcent,FLASHIS_AdrWHJZ_NDJZ_BLH2S_3);  //K2 
  
  Slave_ReadFlashBLData(&SlaveData.CO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLCO2_1);   //NO����������K
  Slave_ReadFlashBLDatak1(&SlaveData.CO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLCO2_2);  //K1
  Slave_ReadFlashBLDatak2(&SlaveData.CO2Concent,FLASHIS_AdrWHJZ_NDJZ_BLCO2_3);  //K2 
  
  //
}


//---
/*
	��������void Slave_ReadFlashZOData()
	������	�ú���Ϊ��ȡH2S������Flash����
*/
void Slave_ReadFlashZOData(Slave_TPActualStructure *P,u32 adrr,float offset)
{
	P->B = Slave_ReadTPFlashData(adrr) - offset;
}

//---
/* 20200524
	��������Slave_ReadFlashBLData()
	������	�ú���Ϊ��ȡ�������Flash����
*/
void Slave_ReadFlashBLData(Slave_TPActualStructure *P,u32 adrr)
{
	P->K = Slave_ReadTPFlashData(adrr);
}
void Slave_ReadFlashBLDatak1(Slave_TPActualStructure *P,u32 adrr)
{
	P->K1 = Slave_ReadTPFlashData(adrr);
}
void Slave_ReadFlashBLDatak2(Slave_TPActualStructure *P,u32 adrr)
{
	P->K2 = Slave_ReadTPFlashData(adrr);
}

//---
/*
	��������float Slave_ReadTPFlashData(u32 addr)
	���������ú���Ϊ��ȡ�¶ȡ�ѹ��У׼���ݵ�Flash�����ֽڵĸ���������
	���룺	u32 addr��flash�ڴ��ַ
*/
float Slave_ReadTPFlashData(u32 addr)
{ 
	FourBytes Cfour;
	u16* p= Flashisp_GetData()->Data;
	Cfour.U16Data[0] = p[addr];
	Cfour.U16Data[1] = p[addr+1];
	return(Cfour.FloatData);
	
}
//---
/*
	��������void Slave_GetSensorData(void)
	������	�ú���Ϊ��ȡ����������
*/
void Slave_GetSensorData(void)
{
	if(SlaveData.GetTimer == 0) 
	{
		SlaveData.GetTimer = SLAVE_SensorRefreshTime;
	//Slave_DataDealWith(&SlaveData.Data[10],DS18B20_GetData()->Tem);//����DS18B20
		{//����ӳ��
			
			Slave_DataMaping(&SlaveData.JWTem,SlaveData.Data[SLAVE_MAP_JWTem].Raw);	   //����   //�����DS18B20
			Slave_DataMaping(&SlaveData.HWTem,SlaveData.Data[SLAVE_MAP_HWTem].Raw);	   //�����¶ȸ���5805���¶�
 			//Slave_DataMaping(&SlaveData.HWTem,DS18B20_GetData()->Tem);	   //�����¶������ϱ���λ�� *************
			if(SlaveData.E28.STATERD24G&0x01)  //�����ж�
			{
				
			//Slave_DataMaping(&SlaveData.YWTem,SlaveData.Data[SLAVE_MAP_E28YW].Raw);//�����¶�  20200525
				
			SlaveData.YWTem.Value=	SlaveData.Data[SLAVE_MAP_E28YW].Raw;
			Slave_DataMaping(&SlaveData.ACPress,SlaveData.Data[SLAVE_MAP_E28DY].Raw*1000);//A· ��ѹ ������ת����PA
			Slave_DataMaping(&SlaveData.AGPress,SlaveData.Data[SLAVE_MAP_E28JY].Raw);//A·     ȫѹKpa
				
			}
			else
			{
			//Slave_DataMaping(&SlaveData.YWTem,(SlaveData.Data[SLAVE_MAP_YWTem].Raw-AdMin)*PT100_K+PT100_B);//�����¶�  20200525
			Slave_DataMaping(&SlaveData.YWTem,CalcAdcToTemperature(SlaveData.Data[SLAVE_MAP_YWTem].Raw));//�����¶� 20200525				
			Slave_DataMaping(&SlaveData.ACPress,SlaveData.Data[SLAVE_MAP_ACPress].Raw*1000);//A· ��ѹ ������ת����PA
			Slave_DataMaping(&SlaveData.AGPress,SlaveData.Data[SLAVE_MAP_AGPress].Raw);//A·      ȫѹKPA
			}

			//Slave_DataMaping(&SlaveData.GQTem,(SlaveData.Data[SLAVE_MAP_GQTem].Raw-AdMin)*PT100_K+PT100_B);//�����¶�  20200525
			//Slave_DataMaping(&SlaveData.SQTem,(SlaveData.Data[SLAVE_MAP_SQTem].Raw-AdMin)*PT100_K+PT100_B);//ʪ���¶�  20200525
			Slave_DataMaping(&SlaveData.GQTem,CalcAdcToTemperature(SlaveData.Data[SLAVE_MAP_GQTem].Raw));//�����¶�  20200525

			Slave_DataMaping(&SlaveData.SQTem,CalcAdcToTemperature(SlaveData.Data[SLAVE_MAP_SQTem].Raw));//ʪ���¶�  20200525
		
			Slave_DataMaping(&SlaveData.DQPress,SlaveData.Data[SLAVE_MAP_DQPress].Raw);//����ѹ

			Slave_DataMaping(&SlaveData.BCPress,SlaveData.Data[SLAVE_MAP_BCPress].Raw*1000);//����ѹ   ������ת����PA
			
			Slave_DataMaping(&SlaveData.BGPress,SlaveData.Data[SLAVE_MAP_BGPress].Raw);//d·��ѹ����ѹ��
			Slave_DataMaping(&SlaveData.CCPress,SlaveData.Data[SLAVE_MAP_CCPress].Raw*1000);//e·����ѹ     ������ת����PA
			
	
		}
		Flux_CalculateFlux('A');
		Flux_CalculateFlux('B');
		Flux_CalculateFlux('C');
		//�������
		CalculationGas();
		//����������Ҫ������
		CalculationFormula_All();
	}
}
//---
/*
	��������void Slave_DataDealWith(Flux_FluxStructure *p,float value)
	������	�ú���Ϊ���¶ȡ�ѹ�����ݽ��д��������˲�	
	���룺	*p����������¶�/ѹ��/��������
					value������ֵ
*/
void Slave_DataDealWith(Slave_TPRawStructure *p,float value,u8 flag)
{   float buf;
	if(value == SLAVE_ColFAULT)
	{	p->Raw = SLAVE_ColFAULT;
	}
	else 
	{  buf=Slave_DataMiddleFilter(&p->Filter,value);
		if(flag)
		{
         p->Raw = Slave_DataFilter(&p->Filter,buf);
		}
		else
		{
		 p->Raw =buf;	
		}
	}
}
//---
/*
	��������float Slave_DataFilter(Slave_FilterStructure *p,float value)
	������	�ú���Ϊ�����ݽ���ѭ���ж��˲��������㲢�����˲�ֵ
	���룺	p�����˲��ṹ��
					data���²���ֵ
	���أ�	�����˲�ֵ
*/
float Slave_DataFilter(Slave_FilterStructure *p,float value)
{
	if(p->Flag & B0)   //���ַ�����Сʱ�俪��
	{//����ģʽ
		p->Sum += value;             //������ֵ
		p->Sum -= p->Buffer[p->Cnt]; //��ȥ�ɶ�Ӧλ��ֵ 
		p->Buffer[p->Cnt++] = value;
		if(p->Cnt == SLAVE_FILTER_DEPTH) p->Cnt = 0;
		value = p->Sum/SLAVE_FILTER_DEPTH;
	}
	else
	{//�ϵ��ʼģʽ
		p->Buffer[p->Cnt++] = value;
		p->Sum += value;
		if(p->Cnt == SLAVE_FILTER_DEPTH) 
		{
			p->Cnt = 0;
			p->Flag |= B0;
			value = p->Sum/SLAVE_FILTER_DEPTH;
		}
	}
	return value;
}

//---
/*
	Slave_DataMiddleFilter(Flux_FilterStructure *p,float value)
    �С�ֵ�˲���
*/

float Slave_DataMiddleFilter(Slave_FilterStructure *p,float value)
{   u8 i;
	float temp,temp1;
	float buf[SLAVE_MIDDLEFILTER_DEPTH];
	if(p->Flag & B1)
	{//���Ǳ��淽ʽ
		p->BufferMiddle[p->CntMiddle++] = value;
		if(p->CntMiddle >= SLAVE_MIDDLEFILTER_DEPTH) p->CntMiddle = 0;
	}
	else
	{//δ�����淽ʽ
		p->BufferMiddle[p->CntMiddle++] = value;
		if(p->CntMiddle >=SLAVE_MIDDLEFILTER_DEPTH) 
		{
			p->CntMiddle = 0;
			p->Flag |= B1;
		
		}
	}
	//
	for(i=0;i<SLAVE_MIDDLEFILTER_DEPTH;i++)
	buf[i]=p->BufferMiddle[i];
	//ȥ�����ֵ
	for(i=0;i<SLAVE_MIDDLEFILTER_DEPTH-1;i++)
	 {
		 if(buf[i]>buf[i+1])
		 {
			 temp1=buf[i+1];
			 buf[i+1]=buf[i];
			 buf[i]=temp1;
		 }
	 }
	//ȥ����Сֵ
	for(i=0;i<SLAVE_MIDDLEFILTER_DEPTH-2;i++)
	 {
		  if(buf[i]<buf[i+1])
		 {
			 temp1=buf[i+1];
			 buf[i+1]=buf[i];
			 buf[i]=temp1;
		 }
	 }
	

	temp=0;
	for(i=0;i<(SLAVE_MIDDLEFILTER_DEPTH-2);i++)
	temp=temp+buf[i];
	temp=temp/(SLAVE_MIDDLEFILTER_DEPTH-2);    //ȥ�������С���ƽ��ֵ
	temp1=temp;
	temp=temp+p->OldMiddle;  //����ƽ��
	temp=temp/2;
    p->OldMiddle=temp1;
   return temp;
}

//---
/*
	��������void Slave_DataMaping(Slave_TPActualStructure *p,float value)
	������	�ú����ǽ���������ֵӳ��Ϊ����ֵ
*/
void Slave_DataMaping(Slave_TPActualStructure *p,float value)
{
	if(value == SLAVE_ColFAULT) p->Value = SLAVE_ColFAULT;
	else
	{//���ݴ���	
		p->Value = value * p->K + p->B;
	
		switch(p->Mode.Bits.ActualMode)
		{//Actual�˴���
			case 1:
			{//������ѹֵС��1ʱ��ΪֵΪ0
				if(fabs(p->Value*1000) < 1) p->Value = 0;
				break;
			}
			case 2:
			{
				if(fabs(p->Value*1000) < 5) p->Value = 0;
				break;
			}
		}
	}
}

//---
/*
	��������void Slave_StopZero(Slave_TPActualStructure *p)
	������	�ú���Ϊ����
	���룺	Slave_TPActualStructure *p���������
          Addr fla
					
*/


void Slave_Zero(Slave_TPActualStructure *p,u32 Addr,float offset) //�����浽flash
{
	FourBytes four;
	u16* PTR= Flashisp_GetData()->Data;
	p->B=p->B-p->Value;                      //�ı�ƫ�������ϲ���ֵ�õ��µ�ƫ����
	four.FloatData=p->B+offset;              //20200703
	PTR[Addr]=four.U16Data[0];
	PTR[Addr+1]=four.U16Data[1];

}
//---
/*
	��������void Slave_Zero_Press()
	������	�ú���Ϊ����
	���룺	����ѹ��ֵΪ�������
 
*/
void Slave_Zero_Press(void)  //����ѹ��ֵ����
{
//	Slave_Zero(&SlaveData.DQPress,FLASHIS_AdrWHJZ_YLJZ_DQY);
   	Slave_Zero(&SlaveData.ACPress,FLASHIS_AdrWHJZ_YLJZ_DY,SLAVE_BaseCPress);
	Slave_Zero(&SlaveData.AGPress,FLASHIS_AdrWHJZ_YLJZ_JY,SLAVE_BaseCPress);
	Slave_Zero(&SlaveData.BCPress,FLASHIS_AdrWHJZ_YLJZ_CLY,SLAVE_BaseCPress);
	Slave_Zero(&SlaveData.BGPress,FLASHIS_AdrWHJZ_YLJZ_JJY,SLAVE_BaseCPress);
    Slave_Zero(&SlaveData.CCPress,FLASHIS_AdrWHJZ_YLJZ_QLY,SLAVE_BaseCPress);
	Flashisp_GetData()->Flag=1; //��������
}
//---
/*
	��������void Slave_Zero_Tem()
	������	�ú���Ϊ����
	���룺	�����¶�ֵΪ�������
 
*/
void Slave_Zero_Tem(void)  //����ѹ��ֵ����
{
  Slave_Zero(&SlaveData.HWTem,FLASHIS_AdrWHJZ_WDJZ_LDHW,SLAVE_BaseTem);
  Slave_Zero(&SlaveData.GQTem,FLASHIS_AdrWHJZ_WDJZ_LDGQ,SLAVE_BaseTem);
  Slave_Zero(&SlaveData.SQTem,FLASHIS_AdrWHJZ_WDJZ_LDSQ,SLAVE_BaseTem);
  Slave_Zero(&SlaveData.YWTem,FLASHIS_AdrWHJZ_WDJZ_LDYW,SLAVE_BaseTem);
  Slave_Zero(&SlaveData.JWTem,FLASHIS_AdrWHJZ_WDJZ_LDJW,SLAVE_BaseTem);
	
  Flashisp_GetData()->Flag=1; //��������
}
//---

/*
	��������void Slave_Zero_QT()
	������	�ú���Ϊ����
	���룺	��������ֵΪ�������
 
*/
void Slave_Zero_QT(void)  //����ѹ��ֵ����
{
	
	if((Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>0)&0x01)  //�ж���û�а�װ
	{
    SlaveData.O2Concent.Value-=21;		
	Slave_Zero(&SlaveData.O2Concent,FLASHIS_AdrWHJZ_NDJZ_LDO2,SLAVE_BaseGas);
	//SlaveData.O2Concent.B=	SlaveData.O2Concent.B+21;
		
	}
	if((Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>1)&0x01)  //�ж���û�а�װ
    Slave_Zero(&SlaveData.SO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDSO2,SLAVE_BaseGas);
	
	if((Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>2)&0x01)  //�ж���û�а�װ
    Slave_Zero(&SlaveData.NOConcent,FLASHIS_AdrWHJZ_NDJZ_LDNO,SLAVE_BaseGas);
	
	if((Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>3)&0x01)  //�ж���û�а�װ
    Slave_Zero(&SlaveData.NO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDNO2,SLAVE_BaseGas);
	
	if((Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>4)&0x01)  //�ж���û�а�װ
    Slave_Zero(&SlaveData.COConcent,FLASHIS_AdrWHJZ_NDJZ_LDCO,SLAVE_BaseGas);
	
	if((Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>5)&0x01)  //�ж���û�а�װ
	Slave_Zero(&SlaveData.H2SConcent,FLASHIS_AdrWHJZ_NDJZ_LDH2S,SLAVE_BaseGas);
	
	if((Flashisp_GetData()->Data[FLASHIS_AdrWHJZ_QTCGQXZ]>>6)&0x01)  //�ж���û�а�װ
	{
	SlaveData.CO2Concent.Value=SlaveData.CO2Concent.Value-0.3f;
	Slave_Zero(&SlaveData.CO2Concent,FLASHIS_AdrWHJZ_NDJZ_LDCO2,SLAVE_BaseGas);
	//SlaveData.CO2Concent.B=SlaveData.CO2Concent.B+0.3;
	}
	Flashisp_GetData()->Flag=1; //��������
}




//---
//---
//-------------------------������----------------------------
//---
/*
	��������void Slave_MotorRun(Slave_MotorStructure *p,float pv)
	������	�������
	���룺	*p�������Ƶĵ��
			pv�����²���ֵ
*/
void Slave_MotorRun(Slave_MotorStructure *p,float pv)
{   Slave_MotorPidPara(p);   //���ݵ�ǰ������С����PID����
	switch(p->RunMode)
	{
		case 1:
		{
			p->RunMode=2;
			p->Timer=1000;  //�ȶ�2��
			p->Ratio =p->Pid.OUT0;
			p->OnOff = 1;
		}
		case 2:
		{

			if(p->Timer==0)
			{  
				p->RunMode=3;
				p->Timer=1500;  //�ȶ�2��
				p->Pid.OUT0=p->Pid.OUT0+30;
			    p->Ratio =p->Pid.OUT0;
			    p->OnOff = 1;
			}
			break;
		}
		case 3:
			{  
			if(p->Timer==0)
			{
				p->RunMode=4;
				p->Pid.OUT=p->Pid.OUT0;
			}
			break;
		}
		case 4:
		{
			//�ɼ�ģʽ
			if(pv != SLAVE_ColFAULT)
			{//�ǹ���
				p->Pid.Pv = pv;//���²ɼ�����
				Pid_Calculate(&p->Pid);
				p->Ratio =p->Pid.OUT;
				p->OnOff = 1;
			}
			
			break;
		}
		case 101:
		{//�������е������Ҫ���趨ռ�ձ�
			p->OnOff = 1;
			break;
		}
		default:
		{
			p->RunMode = 0;
			p->OnOff = 0;
			p->Ratio = 0;
			break;
		}
	}
}
//---
/*
	��������u8 Slave_StartMotor(char motor,u16 ratio)
	������	��Ŀ��ռ�ձ�ratio����A�õ��/B�õ��/TSP·���
	���룺	motor��������A�����B�����TSP·�����'A'=A�õ����'B'=B�õ����'T'=TSP·���
					ratio��ռ�ձȣ�0~65535
	���أ�	0=�����ɹ�
					255=motor����������������'A'��'B'��'T'֮һ
*/
u8 Slave_StartMotor(Slave_MotorStructure *P,u16 ratio)
{
		P->RunMode = 101;
		P->Ratio = ratio;
		P->OnOff = 1;
	    return 0;
}
//---
/*
	��������u8 Slave_StopMotor(char motor)
	������	ֹͣA�õ����B�õ����TSP�õ��
	���룺	motor��ֹͣ��A�����B�����TSP·�����'A'=A�õ����'B'=B�õ����'T'=TSP·���
	���أ�	0=ֹͣ�ɹ�
					255=motor����������������'A'��'B'��'T'֮һ
*/
u8 Slave_StopMotor(Slave_MotorStructure *P)
{
		P->RunMode = 0;
		P->Ratio = 0;
		P->OnOff = 0;
		return 0;
}
//---
/*
	��������u8 Slave_StartPump(char pump,float flux)
	������	��Ŀ������flux����A·/B·/TSP·��
	���룺	pump�������ö���'A'=A·�ã�'B'=B·�ã�'T'=TSP·��
					flux���趨��Ŀ����������λmin/L
	���أ�	0=�����ɹ�
					255=pumb��������pumb����'A'��'B'��'T'�е�����һ����
					B0λ=1����ѹ����(A/B)��ѹ����(TSP)
					B1λ=1����ѹ����(��ѹ����)	
					B2λ=1�����¹���(A/B)���¹���(TSP)
					B3λ=1������ѹ����(TSP)
*/
u8 Slave_StartPump(Slave_MotorStructure *P,float flux)
{
	u8 bf = 0;
	Slave_MotorStructure *p;
	p=P;
	p->Pid.Sv = flux;//�趨ֵ
	p->Pid.Ek = 0;
	p->Pid.Ek_1 = 0;
	p->Pid.Ek_2 = 0;
	p->Pid.SEk = 0;
	p->Pid.OUT = 0;
	p->Pid.Timer = 1000;
	p->RunMode = 1;
	return(bf);
}
//---
//---
/*
	��������u8 Slave_ContinuePump(char pump,float flux)
	������	��Ŀ������flux����A·/B·/TSP·��
	���룺	pump�������ö���'A'=A·�ã�'B'=B·�ã�'T'=TSP·��
					flux���趨��Ŀ����������λmin/L
	���أ�	0=�����ɹ�
					255=pumb��������pumb����'A'��'B'��'T'�е�����һ����
					B0λ=1����ѹ����(A/B)��ѹ����(TSP)
					B1λ=1����ѹ����(��ѹ����)	
					B2λ=1�����¹���(A/B)���¹���(TSP)
					B3λ=1������ѹ����(TSP)
*/
u8 Slave_ContinuePump(Slave_MotorStructure *P,float flux)
{
	u8 bf = 0;
	Slave_MotorStructure *p;
	p=P;
	p->Pid.Sv = flux;//�趨ֵ
//	p->Pid.Ek = 0;
//	p->Pid.Ek_1 = 0;
//	p->Pid.SEk = 0;
//	p->Pid.Timer = 1000;
//	p->RunMode = 1;
	return(bf);
}
/*
	��������u8 Slave_StopPump(char pump)
	������	ֹͣA��/B��/TSP��
	���룺	pump��ֹͣ�ö���'A'=A·�ã�'B'=B·�ã�'T'=TSP·��
	���أ�	0=�رճɹ�
					255=����������������������'A'��'B'��'T'֮һ
*/
u8 Slave_StopPump(Slave_MotorStructure *pump)
{
	return(Slave_StopMotor(pump));
}
//---
/*
	��������void Slave_MotorInit(Slave_MotorStructure *p)
	������	�ú������ڳ�ʼ���ӻ������ز���
*/
void Slave_MotorInit(Slave_MotorStructure *p)
{
	p->OnOff = 0;
	p->Ratio = 0;
	p->RunMode = 0;
}
//---
/*
	��������u8 Slave_MotorPidDataInit(char pump)
	������	�ú���Ϊ��ʼ���õ�pid����
	���룺	pump����ҪУ׼�ıã�'A'=A�ã�'B'=B�ã�'T'=TSP��
	���أ�	0=�����ɹ�
					255=pumb��������pumb����'A'��'B'��'T'�е�����һ����
*/
u8 Slave_MotorPidDataInit(Slave_MotorStructure *P)
{
PID_DataStructure *p;
	if(P->rame == 'W')
	{
		return 0;
	}
	if(P->rame == 'S')
	{
		return 0;
	}
	if(P->rame == 'G')  //����PID���ݳ�ʼ��
	{
		p = &P->Pid;
		//PID��ʼ��
		p->Kp = 2000;	//����ϵ��100
		p->T = 100;	 //�������ڣ�100ms
		p->Ti = 1000;//��������
		p->Td = 2000;//΢��ʱ��
		p->PWMcycle = 65000;//���ֵ
		p->OUT0 = 2000;
		p->SEk = 0;
		p->Ek = 0;
		p->Ek_1 = 0;
		return 0;
	}
	if(P->rame == 'D')//����PID���ݳ�ʼ��
	{
		p = &P->Pid;
		//PID��ʼ��
		p->Kp =1.2;	//����ϵ�� 25  1.8
		p->T = 100;		//�������ڣ�100ms
		p->Ti =1000;//��������1800  ,Խ��OUT ��ԽС
		p->Td =000;//΢��ʱ��1000
		p->PWMcycle = 6500;//���ֵ,����������6500
		p->OUT0 =120;  //1000              ,150�����20L,
		p->SEk = 0;    
		p->Ek = 0;
		p->Ek_1 = 0;
		return 0;
	}
	else return 255;
}
//---
/*
	��������Slave_MotorPidPara(Slave_MotorStructure *P)
	���������ݳ��õ�������Χ�趨PID����
	���룺	pump����ҪУ׼�ıã�'A'=A�ã�'B'=B�ã�'T'=TSP��
	���أ�
				
*/
void Slave_MotorPidPara(Slave_MotorStructure *P)
{ 
	PID_DataStructure *p;
	if(P->rame == 'D')//����PID���ݳ�ʼ��
	{   p = &P->Pid;
		if(p->Sv<=20)
		{
		p->Kp =1.6;	//����ϵ�� 25
		p->T = 200;		//�������ڣ�100ms
		p->Ti =0.8;//��������1800  ,Խ��OUT ��ԽС
		p->Td = 0;		//΢��ʱ��1000
		p->OUT0 =100;  //1000              ,150�����20L,	
		}
		else if(p->Sv<=30)
		{
		p->Kp =1.6;	//����ϵ�� 25
		p->T = 200;		//�������ڣ�100ms
		p->Ti =1.0;//��������1800  ,Խ��OUT ��ԽС 
		p->Td =0;		//΢��ʱ��1000
		p->OUT0 =110;  //1000       ,150�����20L,	
		}
		else if(p->Sv<=40)
		{
		p->Kp =1.6;	//����ϵ�� 25
		p->T = 200;		//�������ڣ�100ms
		p->Ti =1.8;//��������1800  ,Խ��OUT ��ԽС 
		p->Td =0;		//΢��ʱ��1000
		p->OUT0 =110;  //1000       ,150�����20L,	
		}
		else if(p->Sv<=60)
		{
		p->Kp =1.6;	//����ϵ�� 
		p->T = 200;		//�������ڣ�100ms
		p->Ti =2.5;//��������1800  ,Խ��OUT ��ԽС
		p->Td =0;		//΢��ʱ��1000
		p->OUT0 =110;  //1000              ,150�����20L,	
		}
		else if(p->Sv<=80)
		{
		p->Kp =1.6;	//����ϵ�� 
		p->T = 200;		//�������ڣ�100ms
		p->Ti =2.8;//��������1800  ,Խ��OUT ��ԽС
		p->Td =0;		//΢��ʱ��1000
		p->OUT0 =110;  //1000              ,150�����20L,	
		}
		else
		{
		p->Kp =1.8;	//����ϵ�� 25      1.8
		p->T = 200;	//�������ڣ�   200
		p->Ti =3.0;//  ,3.0
		p->Td =0;		//΢��ʱ��1000
		p->OUT0 =110;  //1000              ,150�����20L,	
		}
	}
}
//---
//-------------------------ͨѶ��غ���----------------------------
//---
/*
	��������void Slave_Communicate(void)
	������	�ú���Ϊ��Ӱ�ͨѶ�õ�����������������Ӱ�֮������ݽ���
*/
void Slave_Communicate(void)
{
	switch(SlaveData.Comm.RunMode)
	{
		case 1:  //��һ���´�����
		{
			Slave_CommSendData(0xAA);    //�������ݸ�����
			SlaveData.Comm.Error = 0xFF;
			SlaveData.Comm.Timer = 100;  //
			Slave_UsartBuffer->BufFlg1=0;
			SlaveData.Comm.RunMode = 2;
			break;
		}
		case 2:
		{
			if(Slave_UsartBuffer->BufFlg1==1) // �յ�����
			{
				Slave_UsartBuffer->BufFlg1=0;
				if(Slave_CommReceiveData(&Slave_UsartBuffer->ProgressBuffer1[0])==0)  //������յ�������
    		 {  //���մ���
				  if(SlaveData.Comm.ErrorCnt < 20) 
				 {
					SlaveData.Comm.ErrorCnt++;//ͨѶ����ͳ��
				  }
				else
				{//����
					u8 i;
					for(i=0;i<SLAVE_DataMax-1;i++)
					{
						SlaveData.Data[i].Raw = SLAVE_ColFAULT;
					}
				 }
				
			  }
				else   //�ɹ�����
				{
				SlaveData.Comm.ErrorCnt = 0;
				}
				SlaveData.Comm.Timer = 200;
				SlaveData.Comm.NewMode = 1;
				SlaveData.Comm.RunMode = 251;
				
			}
			else if(SlaveData.Comm.Timer == 0) //���ճ�ʱ
			{//�涨ʱ����û�н��յ����ݣ���Ϊ����
				if(SlaveData.Comm.ErrorCnt < 20) 
				{
					SlaveData.Comm.ErrorCnt++;//ͨѶ����ͳ��
				}
				else
				{//����
					u8 i;
					for(i=0;i<SLAVE_DataMax-1;i++)
					{
						SlaveData.Data[i].Raw = SLAVE_ColFAULT;
					}
				}
				SlaveData.Comm.Timer = 200;
				SlaveData.Comm.NewMode = 1;
				SlaveData.Comm.RunMode = 251;
			}
			break;
		}
		case 251:
		{
			if(SlaveData.Comm.Timer == 0) SlaveData.Comm.RunMode = SlaveData.Comm.NewMode;
			break;
		}
	}
	
}
//---
/*
	��������void Slave_CommSendData(void)
	������	�ú���Ϊ��ȡ�ӻ��Ļ�������
	���룺	address���ӻ���ַ(0xAA)
0xAA,����,0x01,ˮ�ÿ���,��ŷ����أ����ÿ��أ�����ռ�ձȣ����ÿ��أ�����ռ�ձȣ�CRC16
*/
void Slave_CommSendData(u8 address)
{
	u8 bf[20];
	u8 cnt = 0,i;
	TwoBytes two;
	bf[cnt++] = 0x01;   //1
	//��ˮ��WATER������Ϣ
	bf[cnt++] = SlaveData.WaterMotor.OnOff;  //2
	//��ŷ�SLVD������Ϣ
	bf[cnt++] = SlaveData.SlvdMotor.OnOff;   //3

	//����GAS
	bf[cnt++] = SlaveData.GasMotor.OnOff;   //4
	two.U16Data = SlaveData.GasMotor.Ratio;
	bf[cnt++] = two.U8Data[0];               //5
	bf[cnt++] = two.U8Data[1];               //6
	//����DUST
	bf[cnt++] = SlaveData.DustMotor.OnOff;    //7
	two.U16Data = SlaveData.DustMotor.Ratio;
	bf[cnt++] = two.U8Data[0];        //8
	bf[cnt++] = two.U8Data[1];        //9
	bf[cnt++] = SlaveData.Stm8Data.rw_ctrl;	//10 2020-05-22 hjl add stm8��д����״̬
	if(SlaveData.Stm8Data.rw_ctrl)          //д
	SlaveData.Stm8Data.rw_ctrl=0;           //��ض� 
	bf[cnt++] = SlaveData.E28.STATE;        //E28������   11
	if(SlaveData.E28.STATE==1)
		SlaveData.E28.STATE=0;  //д��ָ���
	bf[cnt++] = SlaveData.E28.CHset;        //E28�����ŵ�  12
	two.U16Data =SlaveData.E28.ADRRset;     //E28���õ�ַ
	bf[cnt++] = two.U8Data[1];        //13 ���͵�8λ
	bf[cnt++] = two.U8Data[0];        //14 ���͵�8λ
	
	two.U16Data = CRC16(bf,cnt);
	bf[cnt++] = two.U16Data>>8;    //15�ȷ����ֽ�
	bf[cnt++] = two.U16Data;       //16 �ٷ���ֱ��
	Slave_SendBufferData(Slave_UsartPort,Slave_UsartBuffer,address);
	Slave_SendBufferData(Slave_UsartPort,Slave_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		Slave_SendBufferData(Slave_UsartPort,Slave_UsartBuffer,bf[i]);
	}
}
/*
	��������u8 Slave_CommReceiveData(u8 *p,u8 len)
	������	�ú������ڽ��մ������Դӻ�������
	���룺	p�����ݵ��׵�ַ��60���ֽ�
	˵����          byte0���ֽ��� byte1=1(����);byte2��ʼ����
                    1�� DS18B20S�¶ȣ������ͣ�4���ֽ�
					2�� MS5805-A�¶ȣ������ͣ�4���ֽ�
					3�� MS5805-Aѹ���������ͣ�4���ֽ�
					4�� SM9541-Aѹ���������ͣ�4���ֽ�
					5��	SM9541-Bѹ���������ͣ�4���ֽ�
					6��	SM9541-Cѹ���������ͣ�4���ֽ�
					7�� SM3904-Dѹ���������ͣ�4���ֽ�	
                    8�� SM3904-Eѹ���������ͣ�4���ֽ�	
*/
u8 Slave_CommReceiveData(u8 *p)
{
	u8 i;
	u8 cnt=2; //�ӵڶ��ֽڿ�ʼ����
	FourBytes four;
   if(AnalysisCRC(p))  //���������Ƿ�����CRCЧ��
		 {
			for(i=0;i<(SLAVE_DataMax);i++) //�������ԴӰ��18������
		    {
			   four.U8Data[0] = p[cnt++];
			   four.U8Data[1] = p[cnt++];
			   four.U8Data[2] = p[cnt++];
			   four.U8Data[3] = p[cnt++];
			   if((i>=10)&&(i<=16))
			   Slave_DataDealWith(&SlaveData.Data[i],four.FloatData,1);  //���㻬��ƽ��ֵ�����浽Raw  ��Щ�Ƿ������
			   else
			   Slave_DataDealWith(&SlaveData.Data[i],four.FloatData,0);  //���㻬��ƽ��ֵ�����浽Raw  ��Щ�Ƿ������	
				//printf("\r\n %d :%f",i,four.FloatData);	
		    }
	    	//2020-05-25 hjl add��״̬���ݽ���
		    cnt++;
	    	//SlaveData.Stm8Data.rw_ctrl = p[cnt++];
	    	 SlaveData.Stm8Data.Data = p[cnt++];
			
			 SlaveData.E28.STATERD24G=p[cnt++];   //E28״̬
			//E28��ѹ
			 four.U8Data[0] = p[cnt++];
			 four.U8Data[1] = p[cnt++];
			 four.U8Data[2] = p[cnt++];
			 four.U8Data[3] = p[cnt++];
			Slave_DataDealWith(&SlaveData.Data[SLAVE_DataMax],four.FloatData,0);  //���㻬��ƽ��ֵ�����浽Raw  ��Щ�Ƿ������
			//E28��ѹ
			 four.U8Data[0] = p[cnt++];
			 four.U8Data[1] = p[cnt++];
			 four.U8Data[2] = p[cnt++];
			 four.U8Data[3] = p[cnt++];
			 Slave_DataDealWith(&SlaveData.Data[SLAVE_DataMax+1],four.FloatData,0);  //���㻬��ƽ��ֵ�����浽Raw  ��Щ�Ƿ������
			//E28����
			 four.U8Data[0] = p[cnt++];
			 four.U8Data[1] = p[cnt++];
			 four.U8Data[2] = p[cnt++];
			 four.U8Data[3] = p[cnt++];
			 Slave_DataDealWith(&SlaveData.Data[SLAVE_DataMax+2],four.FloatData,0);  //���㻬��ƽ��ֵ�����浽Raw  ��Щ�Ƿ������
			//E28��ַ

			 SlaveData.E28.ADRRRD24G=p[cnt++]; //��8λ
			 SlaveData.E28.ADRRRD24G=SlaveData.E28.ADRRRD24G<<8;
			 SlaveData.E28.ADRRRD24G=SlaveData.E28.ADRRRD24G+p[cnt++];
            //E28�ŵ� 
	
			SlaveData.E28.CHRD24G=p[cnt++];
		    return 1;
		  }
			 else
			return 0;
}
/*
	��������u8 Slave_OpenWaterPump(void)
	������	�򿪳�ˮ��
	���룺	��
	˵����	
*/
void Slave_OpenWaterPump(void)       //��ˮ��20200528
{
	SlaveData.WaterMotor.OnOff=1;  //���Դ�ˮ��
}
/*
	��������Slave_CloseWaterPump(void)
	������	�رճ�ˮ��
	���룺	��
	˵����	
*/
void Slave_CloseWaterPump(void)      //�ر�ˮ��20200528
{
	SlaveData.WaterMotor.OnOff=0;  //���Թر�ˮ��
}
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Slave_DebugASCII(u8 *p,u8 len)
	������		�ú���Ϊslave.c�ļ���ASCII�����ļ�
						:000|08|001
*/
void Slave_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:
		{//��ȡ���д�������ʵʱ����ֵ
			printf("D18B20���£�%f\r\n",SlaveData.JWTem.Value);//����
			printf("5805�¶ȣ�%f\r\n",SlaveData.HWTem.Value);
			
			printf("����ѹ��%f\r\n",SlaveData.DQPress.Value);
			printf("A·��ѹ��%f\r\n",SlaveData.ACPress.Value);
			printf("A·��ѹ��%f\r\n",SlaveData.AGPress.Value);
		    printf("����ѹ��%f\r\n",SlaveData.BCPress.Value);
			printf("����ѹ��%f\r\n",SlaveData.BGPress.Value);
			printf("����ѹ��%f\r\n",SlaveData.CCPress.Value);
	
			printf("���£�%f\r\n",SlaveData.YWTem.Value);  // �����¶�
			printf("�����¶ȣ�%f\r\n",SlaveData.GQTem.Value);  // �����¶�
			printf("ʪ���¶ȣ�%f\r\n",SlaveData.SQTem.Value);  // �����¶�
			
			printf("O2 AD��%f\r\n",SlaveData.O2Concent.Value);
			printf("SO2 AD��%f\r\n",SlaveData.SO2Concent.Value);
			printf("NO AD��%f\r\n",SlaveData.NOConcent.Value);
			printf("NO2 AD��%f\r\n",SlaveData.NO2Concent.Value);
		    printf("CO AD��%f\r\n",SlaveData.COConcent.Value);
			printf("H2S AD��%f\r\n",SlaveData.H2SConcent.Value);
			printf("CO2 AD��%f\r\n",SlaveData.CO2Concent.Value);
			
		//	printf("A·���£�%f\r\n",SlaveData.AGTem.Value);
			
			break;
		}
		case 11:
		{ SlaveData.GasMotor.OnOff=1;
			SlaveData.GasMotor.Ratio+=10;
			if(SlaveData.GasMotor.Ratio>65535)
			SlaveData.GasMotor.Ratio=65535;
			printf("\n����PE5ռ�ձ�+ %d ",SlaveData.GasMotor.Ratio);
			break;
		}
		case 12:
		{ SlaveData.GasMotor.OnOff=1;
			if(SlaveData.GasMotor.Ratio>10)SlaveData.GasMotor.Ratio-=10;
			if(SlaveData.GasMotor.Ratio<=10)SlaveData.GasMotor.Ratio=0;
			printf("\n����PE5ռ�ձ�- %d ",SlaveData.GasMotor.Ratio);
			break;
		}
		case 13:
		{ SlaveData.DustMotor.OnOff=1;
			SlaveData.DustMotor.Ratio+=10;
			if(SlaveData.DustMotor.Ratio>65535)
			SlaveData.DustMotor.Ratio=65535;
			printf("\n����PA5ռ�ձ�+%d ",SlaveData.DustMotor.Ratio);
			break;
		}
		case 14:
		{ SlaveData.DustMotor.OnOff=1;
			if(SlaveData.DustMotor.Ratio>10)SlaveData.DustMotor.Ratio-=10;
			if(SlaveData.DustMotor.Ratio<=10)SlaveData.DustMotor.Ratio=0;
	  	printf("\n����PA5ռ�ձ�+%d ",SlaveData.DustMotor.Ratio);
			break;
		}
		case 21:
		{
			SlaveData.WaterMotor.OnOff=1;  //���Դ�ˮ��
			printf("\n��ˮ��");
			break;
		}
		case 22:
		{ SlaveData.WaterMotor.OnOff=0;  //���Թر�ˮ��
			printf("\n�ر�ˮ��");
			break;
		}
		case 23:
		{
				SlaveData.SlvdMotor.OnOff=1;  //���Դ򿪵�ŷ�
			printf("\n�򿪵�ŷ�");
			break;
		}
		case 24:
		{
			SlaveData.SlvdMotor.OnOff=0;  //���Թص�ŷ�
			printf("\n�رյ�ŷ�");
			break;
		}
		case 2:
		{ //Slave_Zero_QT();  //����ѹ��ֵ����  ����ӿ�
     // Slave_Zero_Tem();  //�����¶ȵ��� ����ӿ�
      Slave_Zero_Press();  //����ѹ��ֵ���� ����ӿ�
			break;
		}
		default:
		{
		  break;
		}
	}
}
//---
/*
	�������� 	void Slave_DebugRTU((u8 *p)
	������		�ú���Ϊslave.c�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*//*
void Slave_DebugRTU(u8 *p,u8 len)
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
}*/
//---
