//#include "sys.h"
//#include "usart.h"
//#include "delay.h"
//#include "stdlib.h" 
//#include "string.h"
//#include "stdio.h"
#include "Printer.h"
#define linenumber 33
#include "CalculationFormula.h"

//��λ
const char danwei[14][9] ={"m3/h    ",//0
                           "%       ",//1
                           "mg/m3   ",//2
                           "kg/h    ",//3
	                         "m2      ",//4
	                         "Pa      ",//5
	                         "kPa     ",//6
	                         "m/s     ",//7
	                         "mm      ",//8
	                         "��      ",//9
	                         "L       ",//10
	                         "m3/h    ",//11
	                         "g       ",//12
	                         "s       ",//13
                           };

//**********************3330********************************
const char p_hand[linenumber]="       YLB-3330 ��������        ";
const char p_gas_hand1[linenumber] ="            ��������            ";
const char p_dust_hand1[linenumber]="            �̳�����            ";
const char p_Work_hand1[linenumber]="            ��������            ";	
const char p_gasminu_hand1[linenumber] ="  YLB-3330��������������        ";
const char p_gasrealt_hand1[linenumber] ="  YLB-3330��ʵʱ��������        ";		
													 
const char p_PressurePara_hand[linenumber]="     YLB-3330 ѹ����������      ";	
const char p_TemperaturePara_hand[linenumber]="     YLB-3330 �¶Ȳ�������      ";		
const char p_FlowPara_hand[linenumber]="     YLB-3330 ������������      ";	
const char p_ConcentraPara_hand[linenumber]="     YLB-3330 Ũ�Ȳ�������      ";														 
													 
//�������̳���������ӡ��													 
const char p_item_name[53][10]={
  "�������:",//0
  "�� �� ��:",//1
  "����ʱ��:",//2
  "�����ص�:",//3
  "��Ʒ���:",//4
  "�� �� ��:",//5
  "ƽ����ѹ:",//6
  "ƽ����ѹ:",//7
	"ƽ��ȫѹ:",//8
	"ƽ������:",//9	
	"ƽ������:",//10
	"�� �� ѹ:",//11
	"�̵�����:",//12
	"�� ʪ ��:",//13
	"Ƥ�й�Kp:",//14
	"��ʣϵ��:",//15
	"����ϵ��:",//16
	"����ϵ��:",//17
	"�� �� ��:",//18
	"����ֱ��:",//19
  "ƽ����ѹ:",//20
  "ƽ������:",//21
	"�ۼƲ�ʱ:",//22
	"�������:",//23
	"������:",//24
	"��������:",//25
  "�������:",//26
	" O2 Ũ��:",//27
	"SO2 Ũ��:",//28
	"SO2 ����:",//29
	"SO2 �ŷ�:",//30
	" NO Ũ��:",//31
	" NO ����:",//32
	" NO �ŷ�:",//33
	"NO2 Ũ��:",//34
	"NO2 ����:",//35
	"NO2 �ŷ�:",//36
	"NOx Ũ��:",//37
	"NOx ����:",//38
	"NOx �ŷ�:",//39
	"CO  Ũ��:",//40
	"CO  ����:",//41
	"CO  �ŷ�:",//42
	"NOx Ũ��:",//43
	"NOx ����:",//44
	"NOx �ŷ�:",//45
	"CO2 Ũ��:",//46
	"CO2 ����:",//47
	"CO2 �ŷ�:",//48
	"�̳�����:",//49
	"�̳�Ũ��:",//50
	"�̳�����:",//51
	"�̳��ŷ�:",//52
};
//������ӡ��
const char p_para_item_name[51][15]={
	"����/�������:",//0
	"����/������:",//1
	"�������:",//2
	"ʱ    ��:",//3
	"�����¶�:",//4
	"�� �� ѹ:",//5
	"1.0L����:",//6
  "20 L����:",//7
	"30 L����:",//8
	"40 L����:",//9
	"50 L����:",//10
	"70 L����:",//11
	"90 L����:",//12
  "��ѹ���:",//13
	"��ѹ���:",//14
	"��ѹ���:",//15
	"��ѹ���:",//16
	"����ѹ���:",//17
	"ʪ�����:",//18
	"ʪ����:",//19
	"�������:",//20
	" O2     ���:",//21
	" O2 �Ͷ˱���:",//22
	" O2 �ж˱���:",//23
	" O2 �߶˱���:",//24
	"SO2     ���:",//25
	"SO2 �Ͷ˱���:",//26
	"SO2 �ж˱���:",//27
	"SO2 �߶˱���:",//28
	" NO     ���:",//29
	" NO �Ͷ˱���:",//30
	" NO �ж˱���:",//31
	" NO �߶˱���:",//32
	"NO2     ���:",//33
	"NO2 �Ͷ˱���:",//34
	"NO2 �ж˱���:",//35
	"NO2 �߶˱���:",//36
	" CO     ���:",//37
	" CO �Ͷ˱���:",//38
	" CO �ж˱���:",//39
	" CO �߶˱���:",//40
	"H2S     ���:",//41
	"H2S �Ͷ˱���:",//42
	"H2S �ж˱���:",//43
	"H2S �߶˱���:",//44
	"CO2     ���:",//45
	"CO2 �Ͷ˱���:",//46
	"CO2 �ж˱���:",//47
	"CO2 �߶˱���:",//48
  "����ѹ���:",//49
	"100L����:",//50
};



char LineData[55][linenumber]={0};  //����50��,��LineData[1] ��ʼ�ƣ��ü�

Printer_Structure Printer_Data;
/****************************************************************************
	��������Printer_Structure *Printer_GetData(void)
	��  ��: &Printer_Data
         
	��  ��:
	����˵����������Printer_Data ��ָ��

*/

Printer_Structure *Printer_GetData(void){    //����Printer_Data�ṹ���ַ
	return &Printer_Data;
}
/****************************************************************************
	��������PrintAPI_WorkFile()
  ��  ��: strWorkFile *P :�ɶ�SD�����ݺ󷵻ص����ݻ�����ָ��
         
	��  ��:
	����˵����/��ӡ��������ת������ӡ���� 

*/
void PrintAPI_WorkFile(Printer_Structure *P,strWorkFile *Psour)  ///
{
  char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //�ƴ�ӡ����
	u8 i;
	// �ͺ�+��ӡ���
	strncpy((char *)&LineData[linecnt][0],(char *)&p_hand[0],linenumber-1);  //
	linecnt++;	
  strncpy((char *)&LineData[linecnt][0],(char *)&p_Work_hand1[0],linenumber-1);  //
	linecnt++;		
	
	//�������
	strcpy((char *)&linedata[0],(char *)&p_item_name[0][0]);  //"�������:"9
	strcat((char *)&linedata[0],(char *)"      ");  //����6
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8�ֽڱ��=23
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//�ļ���
	strcpy((char *)&linedata[0],(char *)&p_item_name[1][0]);  //"�� �� ��:"9�ֽ�
	strcat((char *)&linedata[0],(char *)"        ");  //����8�ֽ�
	SPutl(6,(u8 *)&data[0],P->print_cnt_file);  //��ǰ���ļ��ž���
	data[6]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
  strcat((char *)&linedata[0],(char *)"         "); //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	
	//����ʱ��
	strcpy((char *)&linedata[0],(char *)&p_item_name[2][0]);  //"����ʱ��"9�ֽ�
	strncpy((char *)&data[0],(char *)&Psour->dat[0],16);      //�ڻ�����ȡ��ʱ��  
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);  //����
	strcat((char *)&linedata[0],(char *)"       ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
  //�����ص�
	strncpy((char *)&linedata[0],(char *)&Psour->Adress[0],32);      //�ڻ�����ȡ��ʱ�� 
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
	
	strncpy((char *)&linedata[0],(char *)&Psour->Adress[32],26);      //�ڻ�����ȡ��ʱ�� 
	linedata[26]=0;
	strcat((char *)&linedata[0],(char *)"    ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;

	//ƽ����ѹ
	strncpy((char *)&linedata[0],(char *)&Psour->Other[0],26);      //�ڻ�����ȡ��ʱ�� 
	linedata[26]=0;
	strcat((char *)&linedata[0],(char *)"    ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
	//ƽ����ѹ
	strncpy((char *)&linedata[0],(char *)&Psour->Other[1],26);      //�ڻ�����ȡ��ʱ�� 
	linedata[26]=0;
	strcat((char *)&linedata[0],(char *)"    ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
	//�� �� ѹ
	//�̵�����
	//����ֱ��
	//ƽ������
	//ƽ������
	//��������
	//�������
	for(i=0;i<=6;i++)
	{
	strncpy((char *)&linedata[0],(char *)&Psour->Other[i+3],27);      //�����������������������λ��ʾ��ȫ��modify by alfred, 2020.07.06
	linedata[27]=0;
	strcat((char *)&linedata[0],(char *)"    ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
	}
	
	//��վ
	 strcpy((char *)&linedata[0],(char *)"-----");  //"��վ"
	 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21�ֽ�
	 strcat((char *)&linedata[0],(char *)"------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	//�绰
	 strcpy((char *)&linedata[0],(char *)"---------");  //"�绰"
	 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13�ֽ�
	 strcat((char *)&linedata[0],(char *)"----------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	
	
	 P->LineMax=linecnt;  //�����������
 
}
/****************************************************************************
20200522
��������PrintAPI_GasFile()
  ��  ��: strWorkFile *P :�ɶ�SD�����ݺ󷵻ص����ݻ�����ָ��
         
	��  ��:
	����˵����/��ӡ��������ת������ӡ���� 

*/
void PrintAPI_GasFile(Printer_Structure *P,strWorkFile *Psour)  //��ӡ�̳�����ת������ӡ����
{
	char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //�ƴ�ӡ����
	float fdbuf,fdbuf1; //
	float	ZSXS; //����ϵ��
	float BGLL;  //�������
	float QTLD;  //����Ũ��
	float GSXS;   //��ʣϵ��
	float FHXS;   //����ϵ��������ϵ��
	float NO,NO2,NOX; //��ȡNO,NO2����ֵ
	u32 CYSC; //����ʱ��

    u8 i;
	strncpy((char *)&data[0],(char *)&Psour->Other[9][9],13);   //����ϵ���ַ���
	data[13]=0;
	ZSXS=Calculate_CharToFloat((u8 *)&data[0],13);              //���ַ���ת���ɸ�����
	
	strncpy((char *)&data[0],(char *)&Psour->Other[12][9],13);  //��������ַ���
	data[13]=0;
	BGLL=Calculate_CharToFloat((u8 *)&data[0],13);              //���ַ���ת���ɸ�����

	
	strncpy((char *)&data[0],(char *)&Psour->Other[23][9],13);  //�����ʣϵ��
	data[13]=0;
	GSXS=Calculate_CharToFloat((u8 *)&data[0],13);               //���ַ���ת���ɸ�����
 
	strncpy((char *)&data[0],(char *)&Psour->Other[10][9],13);  //����ϵ��
	data[13]=0;
	FHXS=Calculate_CharToFloat((u8 *)&data[0],13);               //���ַ���ת���ɸ�����
	
	strncpy((char *)&data[0],(char *)&Psour->Other[15][9],13);  //NO
	data[13]=0;
	NO=Calculate_CharToFloat((u8 *)&data[0],13);               //���ַ���ת���ɸ�����
	
	strncpy((char *)&data[0],(char *)&Psour->Other[16][9],13);  //NO2
	data[13]=0;
	NO2=Calculate_CharToFloat((u8 *)&data[0],13);               //���ַ���ת���ɸ�����
	
	strncpy((char *)&data[0],(char *)&Psour->Other[21][9],13);  //����ʱ��
	data[13]=0;
	CYSC=(u32)Calculate_CharToFloat((u8 *)&data[0],13);               //���ַ���ת���ɸ�����
	
	
	
	
	if(NO==1000000)
		NO=0;
	if(NO2==1000000)
		NO2=0;
    NOX=CalculationFormula_NOX(NO,NO2);
	{
		// �ͺ�+��ӡ���
		
		strncpy((char *)&LineData[linecnt][0],(char *)&p_hand[0],linenumber-1);  //
		linecnt++;	
		strncpy((char *)&LineData[linecnt][0],(char *)&p_gas_hand1[0],linenumber-1);  //
		linecnt++;		
		
		//�������
		strcpy((char *)&linedata[0],(char *)&p_item_name[0][0]);  //"�������:"9
		strcat((char *)&linedata[0],(char *)"      ");  //����6
		strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8�ֽڱ��
		data[8]=0;
		strcat((char *)&linedata[0],(char *)&data[0]);
		strcat((char *)&linedata[0],(char *)"         "); //����
		strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		linecnt++;	
		
		//�ļ���
		strcpy((char *)&linedata[0],(char *)&p_item_name[1][0]);  //"�� �� ��:"9�ֽ�
		strcat((char *)&linedata[0],(char *)"        ");  //����8�ֽ�
		SPutl(6,(u8 *)&data[0],P->print_cnt_file);  //��ǰ���ļ��ž���
		data[6]=0;
		strcat((char *)&linedata[0],(char *)&data[0]);
		strcat((char *)&linedata[0],(char *)"         "); //����
		strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		linecnt++;	
		//����ʱ��
		strcpy((char *)&linedata[0],(char *)&p_item_name[2][0]);  //"����ʱ��"9�ֽ�
		strncpy((char *)&data[0],(char *)&Psour->dat[0],16);      //�ڻ�����ȡ��ʱ��  
		data[16]=0;
		strcat((char *)&linedata[0],(char *)&data[0]);  //����
		strcat((char *)&linedata[0],(char *)"       ");  //����
		strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		linecnt++;
		
		//����ʱ��
		
		strcpy((char *)&linedata[0],(char*)"����ʱ��:");      //ȡ������ʱ��
		
		
		sprintf((char *)&data[0],"   %02d:%02d:%02d        ",CYSC/3600,(CYSC%3600)/60,(CYSC%3600)%60);
		//strncpy((char *)&data[0],(char *)&Psour->Other[21][9],19);//19
		data[19]=0;

		strcat((char *)&linedata[0],(char *)&data[0]);
		strcat((char *)&linedata[0],(char *)"    ");  //����
		strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		linecnt++;
		
		
		
		//�����ص�
		strncpy((char *)&linedata[0],(char *)&Psour->Adress[0],32);      //�ڻ�����ȡ��ʱ�� 
		strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		linecnt++;
		
		strncpy((char *)&linedata[0],(char *)&Psour->Adress[32],28);      //�ڻ�����ȡ��ʱ�� 
		linedata[28]=0;
		strcat((char *)&linedata[0],(char *)"    ");  //����
		strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		linecnt++;
	}
	{	
		if(Psour->Other[20][15]=='9')  //��O2����
		{//O2Ũ��,������
			if(P->pselect[0]==1)  //ѡ���ӡ��
			{ 
				strcpy((char *)&linedata[0],(char*)" O2 Ũ��:");      //�ڻ�����ȡ��ʱ�� 9
				strncpy((char *)&data[0],(char *)&Psour->Other[13][9],19);//19
				data[19]=0;

				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)"    ");  //����
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
		}
		if(Psour->Other[20][16]=='9')  //��SO2����
		{
			strncpy((char *)&data[0],(char *)&Psour->Other[14][9],13);
			data[13]=0;
			//���ַ���ת���ɸ�����
			fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
			QTLD=fdbuf;
			//SO2Ũ��
			if(P->pselect[1]==1)  //ѡ���ӡ��
			{ 
				strcpy((char *)&linedata[0],(char*)"SO2 Ũ��:");      //�ڻ�����ȡ��ʱ�� 9
				strncpy((char *)&data[0],(char *)&Psour->Other[14][9],19);//19
				data[19]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)"    ");  //����4
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//SO2����
			if(P->pselect[9]==1)  //ѡ���ӡ��
			{
				 
                fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //����������
				strcpy((char *)&linedata[0],(char*)"SO2 ����:");      //�ڻ�����ȡ��ʱ�� 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf1); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
				
				if(GSXS==1000000)
			      { for(i=12;i<=21;i++)
				        {
					     linedata[i]='-';
				        }
			      }
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;				
			}
			//SO2�ŷ�
			if(P->pselect[17]==1)  //ѡ���ӡ��
			{
				fdbuf=QTLD;         //����Ũ��
				fdbuf=QTLD*BGLL/1000000;   //�ŷ����ļ���

				strcpy((char *)&linedata[0],(char*)"SO2 �ŷ�:");      //�ڻ�����ȡ��ʱ�� 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" kg/h     ");  //����10
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}				
		}
		if(Psour->Other[20][17]=='9')  //��NO����
		{
			strncpy((char *)&data[0],(char *)&Psour->Other[15][9],13);
			data[13]=0;
			//���ַ���ת���ɸ�����
			fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
			QTLD=fdbuf;
		//NOŨ��
		if(P->pselect[3]==1)  //ѡ���ӡ��
		{ 
			strcpy((char *)&linedata[0],(char*)" NO Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&Psour->Other[15][9],19);
			data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],(char *)"    ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//NO����
		if(P->pselect[11]==1)  //ѡ���ӡ��
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //����������

			strcpy((char *)&linedata[0],(char*)" NO ����:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %-10.1f",fdbuf1); //13
			data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
				if(GSXS==1000000)
			      { for(i=12;i<=21;i++)
				        {
					     linedata[i]='-';
				        }
			      }
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//NO�ŷ�
		if(P->pselect[19]==1)  //ѡ���ӡ��
		{
			fdbuf=QTLD;         //����Ũ��
			fdbuf=QTLD*BGLL/1000000;   //�ŷ����ļ���

			strcpy((char *)&linedata[0],(char*)" NO �ŷ�:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
			data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],(char *)" kg/h     ");  //����10
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		}
		if(Psour->Other[20][18]=='9')  //��NO2,NOx����
		{
			strncpy((char *)&data[0],(char *)&Psour->Other[16][9],13);
			data[13]=0;
			//���ַ���ת���ɸ�����
			fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
			QTLD=fdbuf;
			//NO2Ũ��
			if(P->pselect[4]==1)  //ѡ���ӡ��
			{ 
				strcpy((char *)&linedata[0],(char*)"NO2 Ũ��:");      //�ڻ�����ȡ��ʱ�� 
				strncpy((char *)&data[0],(char *)&Psour->Other[16][9],19);
				data[19]=0;
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)"    ");  //����
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//NO2����
			if(P->pselect[12]==1)  //ѡ���ӡ��
			{
				fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //����������
				strcpy((char *)&linedata[0],(char*)"NO2 ����:");      //�ڻ�����ȡ��ʱ�� 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf1); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
				if(GSXS==1000000)
			      { for(i=12;i<=21;i++)
				        {
					     linedata[i]='-';
				        }
			      }
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//NO2�ŷ�
			if(P->pselect[20]==1)  //ѡ���ӡ��
			{
				fdbuf=QTLD;         //����Ũ��
				fdbuf=QTLD*BGLL/1000000;   //�ŷ����ļ���

				strcpy((char *)&linedata[0],(char*)"NO2 �ŷ�:");      //�ڻ�����ȡ��ʱ�� 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" kg/h     ");  //����10
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//NOxŨ��

			fdbuf=NOX;
			QTLD=fdbuf;
			if(P->pselect[2]==1)  //ѡ���ӡ��
			{
				strcpy((char *)&linedata[0],(char*)"NOx Ũ��:");      //�ڻ�����ȡ��ʱ�� 
				sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//NOx����
			if(P->pselect[10]==1)  //ѡ���ӡ��
			{
				fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //����������
				strcpy((char *)&linedata[0],(char*)"NOx ����:");      //�ڻ�����ȡ��ʱ�� 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf1); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
				if(GSXS==1000000)
			      { for(i=12;i<=21;i++)
				        {
					     linedata[i]='-';
				        }
			      }
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++; //����NOX���㲻�ܴ�ӡ, modify by alfred, 2020.07.07
			}
			//NOx�ŷ�
			if(P->pselect[18]==1)  //ѡ���ӡ��
			{
				fdbuf=QTLD;         //����Ũ��
				fdbuf=QTLD*BGLL/1000000;   //�ŷ����ļ���

				strcpy((char *)&linedata[0],(char*)"NOx �ŷ�:");      //�ڻ�����ȡ��ʱ�� 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" kg/h     ");  //����10
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
		}
		if(Psour->Other[20][19]=='9')  //��CO����
		{
			strncpy((char *)&data[0],(char *)&Psour->Other[17][9],13);
			data[13]=0;
			//���ַ���ת���ɸ�����
			fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
			QTLD=fdbuf;
			//COŨ��
			if(P->pselect[5]==1)  //ѡ���ӡ��
			{ 
				strcpy((char *)&linedata[0],(char*)" CO Ũ��:");      //�ڻ�����ȡ��ʱ�� 
				strncpy((char *)&data[0],(char *)&Psour->Other[17][9],19);
				data[19]=0;
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)"    ");  //����
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//CO����
			if(P->pselect[13]==1)  //ѡ���ӡ��
			{
				fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //����������
				strcpy((char *)&linedata[0],(char*)" CO ����:");      //�ڻ�����ȡ��ʱ�� 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf1); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
				if(GSXS==1000000)
			      { for(i=12;i<=21;i++)
				        {
					     linedata[i]='-';
				        }
			      }
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//CO�ŷ�
			if(P->pselect[21]==1)  //ѡ���ӡ��
			{
				fdbuf=QTLD;         //����Ũ��
				fdbuf=QTLD*BGLL/1000000;   //�ŷ����ļ���

				strcpy((char *)&linedata[0],(char*)" CO �ŷ�:");      //�ڻ�����ȡ��ʱ�� 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" kg/h     ");  //����10
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
		}
		if(Psour->Other[20][20]=='9')  //��H2S����
		{
			strncpy((char *)&data[0],(char *)&Psour->Other[18][9],13);
			data[13]=0;
			//���ַ���ת���ɸ�����
			fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
			QTLD=fdbuf; 
			//H2SŨ��
			if(P->pselect[6]==1)  //ѡ���ӡ��
			{ 
				strcpy((char *)&linedata[0],(char*)"H2S Ũ��:");      //�ڻ�����ȡ��ʱ�� 
				strncpy((char *)&data[0],(char *)&Psour->Other[18][9],19);
				data[19]=0;
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)"    ");  //����
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//H2S����
			if(P->pselect[14]==1)  //ѡ���ӡ��
			{
				fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //����������
				strcpy((char *)&linedata[0],(char*)"H2S ����:");      //�ڻ�����ȡ��ʱ�� 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf1); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
				if(GSXS==1000000)
			      { for(i=12;i<=21;i++)
				        {
					     linedata[i]='-';
				        }
			      }
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//H2S�ŷ�
			if(P->pselect[22]==1)  //ѡ���ӡ��
			{
				fdbuf=QTLD;         //����Ũ��
				fdbuf=QTLD*BGLL/1000000;   //�ŷ����ļ���

				strcpy((char *)&linedata[0],(char*)"H2S �ŷ�:");      //�ڻ�����ȡ��ʱ�� 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" kg/h     ");  //����10
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
		}
		if(Psour->Other[20][21]=='9')  //��CO2����
		{
			strncpy((char *)&data[0],(char *)&Psour->Other[19][9],13);
			data[13]=0;
			//���ַ���ת���ɸ�����
			fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
			QTLD=fdbuf; 
			//CO2Ũ��
			if(P->pselect[7]==1)  //ѡ���ӡ��
			{ 
				strcpy((char *)&linedata[0],(char*)"CO2 Ũ��:");      //�ڻ�����ȡ��ʱ�� 
				strncpy((char *)&data[0],(char *)&Psour->Other[19][9],19);
				data[19]=0;
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)"    ");  //����
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//CO2����
			if(P->pselect[15]==1)  //ѡ���ӡ��
			{
				fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //����������
				strcpy((char *)&linedata[0],(char*)"CO2 ����:");      //�ڻ�����ȡ��ʱ�� 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf1); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
				if(GSXS==1000000)
			      { for(i=12;i<=21;i++)
				        {
					     linedata[i]='-';
				        }
			      }
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//CO2�ŷ�
			if(P->pselect[23]==1)  //ѡ���ӡ��
			{
				fdbuf=QTLD;         //����Ũ��
				fdbuf=QTLD*BGLL/1000000;   //�ŷ����ļ���

				strcpy((char *)&linedata[0],(char*)"CO2 �ŷ�:");      //�ڻ�����ȡ��ʱ�� 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" kg/h     ");  //����10
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
		}
		//"ƽ����ѹ://0
		if(P->pselect[24]==1)  //ѡ���ӡ��
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[0][0],28);      //�ڻ�����ȡ��ʱ�� 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"ƽ����ѹ://1
		if(P->pselect[25]==1)  //ѡ���ӡ��
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[1][0],28);      //�ڻ�����ȡ��ʱ�� 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}		
		//"ƽ������://3
		if(P->pselect[26]==1)  //ѡ���ӡ��
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[3][0],28);      //�ڻ�����ȡ��ʱ�� 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"ƽ������://4
		if(P->pselect[27]==1)  //ѡ���ӡ��
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[4][0],28);      //�ڻ�����ȡ��ʱ�� 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"�̵�����://6
		if(P->pselect[16]==1)  //ѡ���ӡ��
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[6][0],28);      //�ڻ�����ȡ��ʱ�� 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"�� ʪ ��://7
		if(P->pselect[28]==1)  //ѡ���ӡ��
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[7][0],28);      //�ڻ�����ȡ��ʱ�� 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//��ʣϵ��
		if(P->pselect[8]==1)  //ѡ���ӡ��
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[23][0],28);      //�ڻ�����ȡ��ʱ�� 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //����
			
			if(GSXS==1000000)
			{ for(i=12;i<=21;i++)
				{
					linedata[i]='-';
				}
			}

			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"����ϵ��://9
		if(P->pselect[8]==1)  //ѡ���ӡ��
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[9][0],28);      //�ڻ�����ȡ��ʱ�� 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"����ϵ��://10
		if(P->pselect[31]==1)  //ѡ���ӡ��
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[10][0],28);      //�ڻ�����ȡ��ʱ�� 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"��������://11
		if(P->pselect[29]==1)  //ѡ���ӡ��
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[11][0],28);      //�ڻ�����ȡ��ʱ�� 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"�������://12
		if(P->pselect[30]==1)  //ѡ���ӡ��
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[12][0],28);      //�ڻ�����ȡ��ʱ��  
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
	}

	//��վ
	strcpy((char *)&linedata[0],(char *)"-----");  //"��վ"
	strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21�ֽ�
	strcat((char *)&linedata[0],(char *)"------");
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//�绰
	strcpy((char *)&linedata[0],(char *)"---------");  //"�绰"
	strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13�ֽ�
	strcat((char *)&linedata[0],(char *)"----------");
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	


	P->LineMax=linecnt;  //�����������

}
/****************************************************************************
��������PrintAPI_GasRecordFile()
��  ��: strWorkFile *P :�ɶ�SD�����ݺ󷵻ص����ݻ�����ָ��
 
��  ��:
����˵����/��ӡ��������ת������ӡ���� 

*/
void PrintAPI_GasRecordFile(Printer_Structure *P,strWorkFile *Psour)//��ӡ������¼����ת������ӡ����
{
	char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //�ƴ�ӡ����
	float NO,NO2,NOX; //��ȡNO,NO2����ֵ

	strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[3][0],13);  //NO
	data[13]=0;
	NO=Calculate_CharToFloat((u8 *)&data[0],13);               //���ַ���ת���ɸ�����
	
	strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[4][0],13);  //NO2
	data[13]=0;
	NO2=Calculate_CharToFloat((u8 *)&data[0],13);               //���ַ���ת���ɸ�����
	
	if(NO==1000000)
		NO=0;
	if(NO2==1000000)
		NO2=0;
    NOX=CalculationFormula_NOX(NO,NO2);
	

	// �ͺ�+��ӡ���
	strncpy((char *)&LineData[linecnt][0],(char *)&p_gasminu_hand1[0],linenumber-1);  //ͷ
	linecnt++;	

	strncpy((char *)&LineData[linecnt][0],(char *)"                                ",linenumber-1);  //����
	linecnt++;		

	//�������
	strcpy((char *)&linedata[0],(char *)&p_item_name[0][0]);  //"�������:"
	strcat((char *)&linedata[0],(char *)"      ");  //����
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8�ֽڱ��
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//����ʱ��
	strcpy((char *)&linedata[0],(char *)"��ʼʱ��:");  //"����ʱ��"9�ֽ�
	strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[0][0],16);      //�ڻ�����ȡ��ʱ��  
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);  //����
	strcat((char *)&linedata[0],(char *)"        ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;
	
	//����ʱ������λ���ӣ�
	strcpy((char *)&linedata[0],(char *)"����ʱ��:");  //"����ʱ��"9�ֽ�
	strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[0][16],2);      //�ڻ�����ȡ��ʱ��  
	data[2]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);  //����
	strcat((char *)&linedata[0],(char *)"����                  ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	
	//�����ص�
	strncpy((char *)&linedata[0],(char *)&Psour->Adress[0],32);      //�ڻ�����ȡ��ʱ�� 
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;

	strncpy((char *)&linedata[0],(char *)&Psour->Adress[32],28);      //�ڻ�����ȡ��ʱ�� 
	linedata[28]=0;
	strcat((char *)&linedata[0],(char *)"    ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;

	if(Psour->Other[20][15]=='9')  //��O2����
	{//O2Ũ��,������
	//	if(P->pselect[0]==1)  //ѡ���ӡ��
		{
			strcpy((char *)&linedata[0],(char*)" O2 Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[1][0],18);      //�ڻ�����ȡ��ʱ��  
			data[18]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //����
			strcat((char *)&linedata[0],(char *)"      ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
	}
	if(Psour->Other[20][16]=='9')  //��SO2����
	{//SO2Ũ��,
	//		if(P->pselect[4]==1)  //ѡ���ӡ��
		{
			strcpy((char *)&linedata[0],(char*)"SO2 Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[2][0],18);      //�ڻ�����ȡ��ʱ��  
			data[18]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //����
			strcat((char *)&linedata[0],(char *)"      ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
	}
	if(Psour->Other[20][17]=='9')  //��NO����
	{//NOŨ��
	//	if(P->pselect[12]==1)  //ѡ���ӡ��
		{
			strcpy((char *)&linedata[0],(char*)" NO Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[3][0],18);      //�ڻ�����ȡ��ʱ��  
			data[18]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //����
			strcat((char *)&linedata[0],(char *)"      ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
	}
	if(Psour->Other[20][18]=='9')  //��NO2����
	{
	 //NO2Ũ��,
	//	if(P->pselect[16]==1)  //ѡ���ӡ��
		{
			strcpy((char *)&linedata[0],(char*)"NO2 Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[4][0],18);      //�ڻ�����ȡ��ʱ��  
			data[18]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //����
			strcat((char *)&linedata[0],(char *)"      ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;			
		}
	 //NOxŨ��,
	//	if(P->pselect[8]==1)  //ѡ���ӡ��
		{
				strcpy((char *)&linedata[0],(char*)"NOx Ũ��:");      //�ڻ�����ȡ��ʱ�� 9
				sprintf((char *)&data[0],"%10.1f",NOX); //10
				data[10]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)"mg/m3        ");  //����
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
		}
	}
	if(Psour->Other[20][19]=='9')  //��C0����
	{//COŨ��,
	//		if(P->pselect[20]==1)  //ѡ���ӡ��
		{
			strcpy((char *)&linedata[0],(char*)" CO Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[5][0],18);      //�ڻ�����ȡ��ʱ��  
			data[18]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //����
			strcat((char *)&linedata[0],(char *)"      ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;			
		}
	}
	if(Psour->Other[20][20]=='9')  //��H2S����
	{//H2SŨ��,
	//	if(P->pselect[24]==1)  //ѡ���ӡ��
	{
			strcpy((char *)&linedata[0],(char*)"H2S Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[6][0],18);      //�ڻ�����ȡ��ʱ��  
			data[18]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //����
			strcat((char *)&linedata[0],(char *)"      ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;			
		}
	}

	if(Psour->Other[20][21]=='9')  //��CO2����
	{//CO2Ũ��,������
	//	if(P->pselect[28]==1)  //ѡ���ӡ��
		{
			strcpy((char *)&linedata[0],(char*)"CO2 Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[7][0],18);      //�ڻ�����ȡ��ʱ��  
			data[18]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //����
			strcat((char *)&linedata[0],(char *)"      ");  //����
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;			
		}
	 }
	//��վ
	 strcpy((char *)&linedata[0],(char *)"-----");  //"��վ"
	 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21�ֽ�
	 strcat((char *)&linedata[0],(char *)"------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	//�绰
	 strcpy((char *)&linedata[0],(char *)"---------");  //"�绰"
	 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13�ֽ�
	 strcat((char *)&linedata[0],(char *)"----------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	
	
	 P->LineMax=linecnt;  //����������� 

}
/****************************************************************************
20200522
��������PrintAPI_DustFile()
  ��  ��: strWorkFile *P :�ɶ�SD�����ݺ󷵻ص����ݻ�����ָ��
         
	��  ��:
	����˵����/��ӡ�̳�����ת������ӡ���� 

*/
void PrintAPI_DustFile(Printer_Structure *P,strWorkFile *Psour)   //��ӡ�̳�����ת������ӡ����
{
	char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //�ƴ�ӡ����
	float fdbuf,fdbuf1; //
	float	ZSXS; //����ϵ��
	float BGLL;  //�������
	float QTLD;  //����Ũ��
	float GSXS;   //��ʣϵ��
	float FHXS;   //����ϵ��������ϵ��
	float NO,NO2,NOX;  //�ȶ���
    u8 i;
	strncpy((char *)&data[0],(char *)&Psour->Other[9][9],13);   //����ϵ���ַ���
	data[13]=0;
	ZSXS=Calculate_CharToFloat((u8 *)&data[0],13);              //���ַ���ת���ɸ�����
	
	strncpy((char *)&data[0],(char *)&Psour->Other[12][9],13);  //��������ַ���
	data[13]=0;
	BGLL=Calculate_CharToFloat((u8 *)&data[0],13);              //���ַ���ת���ɸ�����

	strncpy((char *)&data[0],(char *)&Psour->Other[38][9],13);  //�����ʣϵ��
	data[13]=0;
	GSXS=Calculate_CharToFloat((u8 *)&data[0],13);               //���ַ���ת���ɸ�����
 
	strncpy((char *)&data[0],(char *)&Psour->Other[10][9],13);  //����ϵ��
	data[13]=0;
	FHXS=Calculate_CharToFloat((u8 *)&data[0],13);               //���ַ���ת���ɸ�����
	
	strncpy((char *)&data[0],(char *)&Psour->Other[22][9],13);  //NO
	data[13]=0;
	NO=Calculate_CharToFloat((u8 *)&data[0],13);               //���ַ���ת���ɸ�����
	
	strncpy((char *)&data[0],(char *)&Psour->Other[23][9],13);  //NO2
	data[13]=0;
	NO2=Calculate_CharToFloat((u8 *)&data[0],13);               //���ַ���ת���ɸ�����

	if(NO==1000000)
		NO=0;
	if(NO2==1000000)
		NO2=0;
	NOX=CalculationFormula_NOX(NO,NO2);

	{
	// �ͺ�+��ӡ���
	strncpy((char *)&LineData[linecnt][0],(char *)&p_hand[0],linenumber-1);  //
	linecnt++;	
  strncpy((char *)&LineData[linecnt][0],(char *)&p_dust_hand1[0],linenumber-1);  //
	linecnt++;		
	
	//�������
	strcpy((char *)&linedata[0],(char *)&p_item_name[0][0]);  //"�������:"
	strcat((char *)&linedata[0],(char *)"      ");  //����
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8�ֽڱ��
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	
	//�ļ���
	strcpy((char *)&linedata[0],(char *)&p_item_name[1][0]);  //"�� �� ��:"9�ֽ�
	strcat((char *)&linedata[0],(char *)"        ");  //����8�ֽ�
	SPutl(6,(u8 *)&data[0],P->print_cnt_file);  //��ǰ���ļ��ž���
	data[6]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
    strcat((char *)&linedata[0],(char *)"         "); //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//����ʱ��
	strcpy((char *)&linedata[0],(char *)&p_item_name[2][0]);  //"����ʱ��"9�ֽ�
	strncpy((char *)&data[0],(char *)&Psour->dat[0],16);      //�ڻ�����ȡ��ʱ��  
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);  //����
	strcat((char *)&linedata[0],(char *)"       ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
  //�����ص�
	strncpy((char *)&linedata[0],(char *)&Psour->Adress[0],32);      //�ڻ�����ȡ��ʱ�� 
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
	
	strncpy((char *)&linedata[0],(char *)&Psour->Adress[32],28);      //�ڻ�����ȡ��ʱ�� 
	linedata[28]=0;
	strcat((char *)&linedata[0],(char *)"    ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
}
  {
		//��Ʒ���
		strncpy((char *)&linedata[0],(char *)&Psour->Other[28],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		//������
		if(P->pselect[1]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[30],28);      //�ڻ�����ȡ��ʱ�� 
		if(linedata[12]=='2')   //����
		{
			strcpy((char *)&linedata[0],"�� �� ��:   ͨ��            ");
		}
		else if(linedata[12]=='1') //©��
		{
			strcpy((char *)&linedata[0],"�� �� ��:   ��ͨ��          ");
		}
		else                     //δ���
		{
			strcpy((char *)&linedata[0],"�� �� ��:   δ���          ");
		}
			
			
	    linedata[28]=0;
			
	    strcat((char *)&linedata[0],(char *)"    ");  //����
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
        linecnt++;
		}
		//ƽ����ѹ
		if(P->pselect[10]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[0][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		
		//ƽ����ѹ
		if(P->pselect[11]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[1][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//ƽ��ȫѹ
		if(P->pselect[12]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[2][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//ƽ������
		if(P->pselect[13]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[3][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//ƽ������
		if(P->pselect[14]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[4][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//�� �� ѹ
		if(P->pselect[8]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[5][0],28);      //�ڻ�����ȡ��ʱ�� 
	   linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}		
		//�̵�����
		if(P->pselect[9]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[6][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//�� ʪ��
		if(P->pselect[15]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[7][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//Ƥ�й�Kp
		if(P->pselect[3]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[8][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//��ʣϵ��
		if(P->pselect[0]==1)  //ѡ���ӡ��
		{
    strncpy((char *)&linedata[0],(char *)&Psour->Other[38][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
      if(GSXS==1000000)
		{ for(i=12;i<=21;i++)
			{
			linedata[i]='-';
			}
		}
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		
		//����ϵ��
		if(P->pselect[0]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[9][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//����ϵ��
		if(P->pselect[16]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[10][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//������
		if(P->pselect[2]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[13][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//����ֱ��
		if(P->pselect[4]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[14][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//ƽ����ѹ
		if(P->pselect[6]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[15][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//ƽ������
		if(P->pselect[5]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[16][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//�ۼƲ�ʱ
		if(P->pselect[7]==1)  //ѡ���ӡ��
		{
	  strncpy((char *)&linedata[0],(char *)&Psour->Other[29][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
		//�������
		if(P->pselect[17]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[17][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
		//������
		if(P->pselect[18]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[18][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//��������
		if(P->pselect[23]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[11][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//�������
		if(P->pselect[24]==1)  //ѡ���ӡ��
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[12][0],28);      //�ڻ�����ȡ��ʱ�� 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //����
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
	
	 if(Psour->Other[27][15]=='9')  //��O2����
	 {//O2Ũ��,������
	   if(P->pselect[25]==1)  //ѡ���ӡ��
		{ 
			strcpy((char *)&linedata[0],(char*)" O2 Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&Psour->Other[20][9],19);
	    data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)"    ");  //����
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
			}
	 }
	 if(Psour->Other[27][16]=='9')  //��SO2����
	 {
	  	strncpy((char *)&data[0],(char *)&Psour->Other[21][9],13);
		  data[13]=0;
		  //���ַ���ת���ɸ�����
		  fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
		  QTLD=fdbuf;
	//SO2Ũ��
		 
		 if(P->pselect[26]==1)  //ѡ���ӡ��
		{ 
			strcpy((char *)&linedata[0],(char*)"SO2 Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&Psour->Other[21][9],19);
	    data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)"    ");  //����
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
			}
	//SO2����
		if(P->pselect[26]==1)  //ѡ���ӡ��
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //����������
			
			strcpy((char *)&linedata[0],(char*)"SO2 ����:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf1); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
			if(GSXS==1000000)
			  { for(i=12;i<=21;i++)
				{
				linedata[i]='-';
				}
			  }
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
	//SO2�ŷ�
		if(P->pselect[26]==1)  //ѡ���ӡ��
		{
			fdbuf=QTLD;         //����Ũ��
			fdbuf=QTLD*BGLL/1000000;   //�ŷ����ļ���

			strcpy((char *)&linedata[0],(char*)"SO2 �ŷ�:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	    data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)" kg/h     ");  //����10
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}		
		
	  }
	 if(Psour->Other[27][17]=='9')  //��NO����
	 {
		 	strncpy((char *)&data[0],(char *)&Psour->Other[22][9],13);
		  data[13]=0;
		  //���ַ���ת���ɸ�����
		  fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
		  QTLD=fdbuf;
	//NOŨ��
		if(P->pselect[28]==1)  //ѡ���ӡ��
		{ 
			strcpy((char *)&linedata[0],(char*)" NO Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&Psour->Other[22][9],19);
	    data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)"    ");  //����
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
			}
	//NO����
		if(P->pselect[28]==1)  //ѡ���ӡ��
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //����������
			
			strcpy((char *)&linedata[0],(char*)" NO ����:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf1); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
			if(GSXS==1000000)
			  { for(i=12;i<=21;i++)
				{
				linedata[i]='-';
				}
			  }
	       strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
           linecnt++;
		}
	//NO�ŷ�
		if(P->pselect[28]==1)  //ѡ���ӡ��
		{
			fdbuf=QTLD;         //����Ũ��
			fdbuf=QTLD*BGLL/1000000;   //�ŷ����ļ���

			strcpy((char *)&linedata[0],(char*)" NO �ŷ�:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	    data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)" kg/h     ");  //����10
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
	 }
	if(Psour->Other[27][18]=='9')  //��NO2,NOx����
	 {
		 strncpy((char *)&data[0],(char *)&Psour->Other[23][9],13);
		  data[13]=0;
		  //���ַ���ת���ɸ�����
		  fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
		  QTLD=fdbuf;
	//NO2Ũ��
		if(P->pselect[29]==1)  //ѡ���ӡ��
		{ 
			strcpy((char *)&linedata[0],(char*)"NO2 Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&Psour->Other[23][9],19);
	    data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)"    ");  //����
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
			}
	//NO2����
		if(P->pselect[29]==1)  //ѡ���ӡ��
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //����������
			
			strcpy((char *)&linedata[0],(char*)"NO2 ����:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf1); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
			if(GSXS==1000000)
			  { for(i=12;i<=21;i++)
				{
				linedata[i]='-';
				}
			  }			
	        strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
            linecnt++;
		}
	//NO2�ŷ�
		if(P->pselect[29]==1)  //ѡ���ӡ��
		{
			fdbuf=QTLD;         //����Ũ��
			fdbuf=QTLD*BGLL/1000000;   //�ŷ����ļ���

			strcpy((char *)&linedata[0],(char*)"NO2 �ŷ�:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" kg/h     ");  //����10
	        strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
            linecnt++;
		}
	//NOxŨ��
		  fdbuf=NOX;
		  QTLD=fdbuf;
		
		if(P->pselect[27]==1)  //ѡ���ӡ��
		{
			strcpy((char *)&linedata[0],(char*)"NOX Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
	        strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
            linecnt++;
		}
	//NOx����
		if(P->pselect[27]==1)  //ѡ���ӡ��
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //����������
			strcpy((char *)&linedata[0],(char*)"NOX ����:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf1); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
			if(GSXS==1000000)
			  { for(i=12;i<=21;i++)
				{
				linedata[i]='-';
				}
			  }			
	        strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
            linecnt++;
		}
	//NOx�ŷ�
		if(P->pselect[27]==1)  //ѡ���ӡ��
		{
			fdbuf=QTLD;         //����Ũ��
			fdbuf=QTLD*BGLL/1000000;   //�ŷ����ļ���

			strcpy((char *)&linedata[0],(char*)"NOX �ŷ�:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" kg/h     ");  //����10
	        strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
            linecnt++;
		}
	 }
	 if(Psour->Other[27][19]=='9')  //��CO����
	 {
		 	strncpy((char *)&data[0],(char *)&Psour->Other[24][9],13);
		  data[13]=0;
		  //���ַ���ת���ɸ�����
		  fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
		  QTLD=fdbuf;
	//COŨ��
		if(P->pselect[30]==1)  //ѡ���ӡ��
		{ 
			strcpy((char *)&linedata[0],(char*)" CO Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&Psour->Other[24][9],19);
	    data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)"    ");  //����
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
			}
	//CO����
		if(P->pselect[30]==1)  //ѡ���ӡ��
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //����������
			
			strcpy((char *)&linedata[0],(char*)" CO ����:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf1); //13
	    data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
			if(GSXS==1000000)
			  { for(i=12;i<=21;i++)
				{
				linedata[i]='-';
				}
			  }
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
        linecnt++;
		}
	//CO�ŷ�
		if(P->pselect[30]==1)  //ѡ���ӡ��
		{
			fdbuf=QTLD;         //����Ũ��
			fdbuf=QTLD*BGLL/1000000;   //�ŷ����ļ���

			strcpy((char *)&linedata[0],(char*)" CO �ŷ�:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	    data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)" kg/h     ");  //����10
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
	 }
	 if(Psour->Other[27][20]=='9')  //��H2S����
	 {
		 	strncpy((char *)&data[0],(char *)&Psour->Other[25][9],13);
		  data[13]=0;
		  //���ַ���ת���ɸ�����
		  fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
		  QTLD=fdbuf;
	//H2SŨ��
		if(P->pselect[31]==1)  //ѡ���ӡ��
		{ 
			strcpy((char *)&linedata[0],(char*)"H2S Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&Psour->Other[25][9],19);
	        data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)"    ");  //����

	        strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
            linecnt++;
			}
	//H2S����
		if(P->pselect[31]==1)  //ѡ���ӡ��
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //����������
			strcpy((char *)&linedata[0],(char*)"H2S ����:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf1); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
			if(GSXS==1000000)
			  { for(i=12;i<=21;i++)
				{
				linedata[i]='-';
				}
			  }			
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
	//H2S�ŷ�
		if(P->pselect[31]==1)  //ѡ���ӡ��
		{
			fdbuf=QTLD;         //����Ũ��
			fdbuf=QTLD*BGLL/1000000;   //�ŷ����ļ���
			strcpy((char *)&linedata[0],(char*)"H2S �ŷ�:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	    data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)" kg/h     ");  //����10
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
	 }
	if(Psour->Other[27][21]=='9')  //��CO2����
	 {
		 	strncpy((char *)&data[0],(char *)&Psour->Other[26][9],13);
		  data[13]=0;
		  //���ַ���ת���ɸ�����
		  fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
		  QTLD=fdbuf;
	//CO2Ũ��
   if(P->pselect[32]==1)  //ѡ���ӡ��
		{ 
			strcpy((char *)&linedata[0],(char*)"CO2 Ũ��:");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&Psour->Other[26][9],19);
	    data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)"    ");  //����
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
			}
	//CO2����
		if(P->pselect[32]==1)  //ѡ���ӡ��
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //����������
			strcpy((char *)&linedata[0],(char*)"CO2 ����:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf1); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����
			if(GSXS==1000000)
			  { for(i=12;i<=21;i++)
				{
				linedata[i]='-';
				}
			  }			
	        strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
            linecnt++;
		}
	//CO2�ŷ�
		if(P->pselect[32]==1)  //ѡ���ӡ��
		{
			fdbuf=QTLD;         //����Ũ��
			fdbuf=QTLD*BGLL/1000000;   //�ŷ����ļ���
			strcpy((char *)&linedata[0],(char*)"CO2 �ŷ�:");      //�ڻ�����ȡ��ʱ�� 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	    data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)" kg/h     ");  //����10
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
	 }
 }
	//��վ
	 strcpy((char *)&linedata[0],(char *)"-----");  //"��վ"
	 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21�ֽ�
	 strcat((char *)&linedata[0],(char *)"------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	//�绰
	 strcpy((char *)&linedata[0],(char *)"---------");  //"�绰"
	 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13�ֽ�
	 strcat((char *)&linedata[0],(char *)"----------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	
	
	 P->LineMax=linecnt;  //�����������

}
/****************************************************************************
	��������PrintAPI_CurTest()
  ��  ��: 
         
	��  ��:
	����˵����/��ӡ���ڲ�������ת������ӡ����

*/
void PrintAPI_CurTest(Printer_Structure *P)                  //��ӡ���ڲ�������ת������ӡ����
{
	char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //�ƴ�ӡ����
	
		// �ͺ�+��ӡ���
	strncpy((char *)&LineData[linecnt][0],(char *)&p_gasrealt_hand1[0],linenumber-1);  //
	linecnt++;		
	
	//�������
	strcpy((char *)&linedata[0],(char *)"�������:");  //"�������:"
	strcat((char *)&linedata[0],(char *)"      ");  //����
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8�ֽڱ��
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//��ӡʱ��
	strcpy((char *)&linedata[0],(char *)"����ʱ��:");  //"����ʱ��:"
	strncpy((char *)&data[0],(char *)&P->dat[0],16);  //  16�ֽ�ʱ��
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"       ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//�����ص�
	strcpy((char *)&linedata[0],(char *)"�����ص�: ");  //"�����ص�:"//10�ֽ�
	strncpy((char *)&data[0],(char *)&P->address[0],22);  //  22�ֽ�
	data[22]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	
	strncpy((char *)&linedata[0],(char *)&P->address[22],28);
	linedata[28]=0;
	strcat((char *)&linedata[0],(char *)"    ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	if(P->installselect[0]==1)  //��O2����
	{//O2Ũ��,������
		if(P->pselect[0]==1)  //ѡ���ӡ��
		{
			strcpy((char *)&linedata[0],(char*)" O2 Ũ��:   ");      //�ڻ�����ȡ��ʱ��  12�ֽ�
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[0][0],10);  //  10�ֽ�ʱ��
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" %        ");  //����λ
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
	 }
	 if(P->installselect[1]==1)  //��SO2����
	 {//SO2Ũ��,
		if(P->pselect[4]==1)  //ѡ���ӡ��
		{
			strcpy((char *)&linedata[0],(char*)"SO2 Ũ��:   ");      //�ڻ�����ȡ��ʱ�� 12�ֽ�
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[1][0],10);  //  10�ֽ�ʱ��
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����λ10�ֽ�
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
	 }
	 if(P->installselect[2]==1) //��NO����
	 {//NOŨ��
		if(P->pselect[12]==1)  //ѡ���ӡ��
		{
			strcpy((char *)&linedata[0],(char*)" NO Ũ��:   ");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[2][0],10);  //  10�ֽ�ʱ��
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����λ
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		linecnt++;
		}
	 }
	 if(P->installselect[2]==1)  //��NO2����
	 {
		 //NO2Ũ��,
		if(P->pselect[16]==1)  //ѡ���ӡ��
		{
			strcpy((char *)&linedata[0],(char*)"NO2 Ũ��:   ");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[3][0],10);  //  10�ֽ�ʱ��
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����λ
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;	
		}
		 //NOxŨ��,
		if(P->pselect[8]==1)  //ѡ���ӡ��
		{
			strcpy((char *)&linedata[0],(char*)"NOx Ũ��:   ");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[4][0],10);  //  10�ֽ�ʱ��
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����λ
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;	
		}
	 }
	 if(P->installselect[2]==1)  //��C0����
	 {//COŨ��,
		if(P->pselect[20]==1)  //ѡ���ӡ��
		{
			strcpy((char *)&linedata[0],(char*)" CO Ũ��:   ");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[5][0],10);  //  10�ֽ�ʱ��
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����λ
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;	
		}
	 }
	 if(P->installselect[2]==1)  //��H2S����
	 {//H2SŨ��,
		if(P->pselect[24]==1)  //ѡ���ӡ��
		{
			strcpy((char *)&linedata[0],(char*)"H2S Ũ��:   ");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[6][0],10);  //  10�ֽ�ʱ��
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" mg/m3    ");  //����λ
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;	
		}
	 }
	 
	 if(P->installselect[2]==1)  //��CO2����
	 {//CO2Ũ��,������
		if(P->pselect[28]==1)  //ѡ���ӡ��
		{
			strcpy((char *)&linedata[0],(char*)"CO2 Ũ��:   ");      //�ڻ�����ȡ��ʱ�� 
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[7][0],10);  //  10�ֽ�ʱ��
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" %        ");  //����λ
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
	 }
		//��վ
		 strcpy((char *)&linedata[0],(char *)"-----");  //"��վ"
		 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21�ֽ�
		 strcat((char *)&linedata[0],(char *)"------");
		 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		 linecnt++;	
		//�绰
		 strcpy((char *)&linedata[0],(char *)"---------");  //"�绰"
		 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13�ֽ�
		 strcat((char *)&linedata[0],(char *)"----------");
		 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		 linecnt++;		
	
		P->LineMax=linecnt;  //����������� 
	
}
/****************************************************************************
	��������PrintAPI_PressureParaOrder()
  ��  ��:
         
	��  ��:
	����˵����//��ӡѹ����������ת������ӡ����

*/
void PrintAPI_PressureParaOrder(Printer_Structure *P)        //��ӡѹ����������ת������ӡ����
{
	char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //�ƴ�ӡ����
	u8 i;
	// �ͺ�+��ӡ���
	strncpy((char *)&LineData[linecnt][0],(char *)&p_PressurePara_hand[0],linenumber-1);  //
	linecnt++;		
	
	//�������
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[2][0]);  //"�������:"
	strcat((char *)&linedata[0],(char *)"      ");  //����
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8�ֽڱ��
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//��ӡʱ��
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[3][0]);  //"ʱ    ��:"
	strncpy((char *)&data[0],(char *)&P->dat[0],16);  //  16�ֽ�ʱ��
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"       ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	
  //0:��ѹ���
	for(i=0;i<=3;i++)
	{
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[i+13][0]);  //"��ѹ���:"
	strcat((char *)&linedata[0],(char *)"    ");  //����
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[i][0],10);  //  10�ֽ�ʱ��
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	}

	//4:����ѹ���
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[17][0]);  //"����ѹ���"
	strcat((char *)&linedata[0],(char *)"  ");  //����
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[4][0],10);  //  10�ֽ�ʱ��
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//5:����ѹ���
  strcpy((char *)&linedata[0],(char *)&p_para_item_name[49][0]);  //"����ѹ���:"
	strcat((char *)&linedata[0],(char *)"  ");  //����
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[5][0],10);  //  10�ֽ�ʱ��
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	
	//��վ
	 strcpy((char *)&linedata[0],(char *)"-----");  //"��վ"
	 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21�ֽ�
	 strcat((char *)&linedata[0],(char *)"------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	//�绰
	 strcpy((char *)&linedata[0],(char *)"---------");  //"�绰"
	 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13�ֽ�
	 strcat((char *)&linedata[0],(char *)"----------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	
	 P->LineMax=linecnt;  //�����������
}
/****************************************************************************
	��������PrintAPI_TemperatureParaOrder()
  ��  ��:
         
	��  ��:
	����˵����//��ӡ�¶Ȳ�������ת������ӡ����

*/
void PrintAPI_TemperatureParaOrder(Printer_Structure *P)     //��ӡ�¶Ȳ�������ת������ӡ����
{
	char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //�ƴ�ӡ����
	// �ͺ�+��ӡ���
	strncpy((char *)&LineData[linecnt][0],(char *)&p_TemperaturePara_hand[0],linenumber-1);  //
	linecnt++;		
		//�������
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[2][0]);  //"�������:"
	strcat((char *)&linedata[0],(char *)"      ");  //����
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8�ֽڱ��
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//��ӡʱ��
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[3][0]);  //"ʱ    ��:"
	strncpy((char *)&data[0],(char *)&P->dat[0],16);  //  16�ֽ�ʱ��
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"       ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//0:����/�������
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[0][0]);  //"����/�������:"  14�ֽ�

	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[0][0],10);  //  10�ֽ�ʱ��
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"        ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//1:����/������
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[1][0]);  //"����/������:"  14�ֽ�

	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[1][0],10);  //  10�ֽ�ʱ��
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"        ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//2:ʪ�����
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[18][0]);  //"ʪ�����:"
	strcat((char *)&linedata[0],(char *)"    ");  //����
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[2][0],10);  //  10�ֽ�ʱ��
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
  //3:ʪ����
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[19][0]);  //"ʪ�����:"
	strcat((char *)&linedata[0],(char *)"    ");  //����
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[3][0],10);  //  10�ֽ�ʱ��
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//4:�������
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[20][0]);  //"ʪ�����:"
	strcat((char *)&linedata[0],(char *)"    ");  //����
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[4][0],10);  //  10�ֽ�ʱ��
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
		//��վ
	 strcpy((char *)&linedata[0],(char *)"-----");  //"��վ"
	 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21�ֽ�
	 strcat((char *)&linedata[0],(char *)"------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	//�绰
	 strcpy((char *)&linedata[0],(char *)"---------");  //"�绰"
	 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13�ֽ�
	 strcat((char *)&linedata[0],(char *)"----------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	
	 P->LineMax=linecnt;  //�����������
	
}
/****************************************************************************
	��������PrintAPI_FlowParaOrder()
  ��  ��:
         
	��  ��:
	����˵����//��ӡ������������ת������ӡ����

*/
void PrintAPI_FlowParaOrder(Printer_Structure *P)           //��ӡ������������ת������ӡ����
{
	char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //�ƴ�ӡ����
	u8 i;
	// �ͺ�+��ӡ���
	strncpy((char *)&LineData[linecnt][0],(char *)&p_FlowPara_hand[0],linenumber-1);  //
	linecnt++;		
		//�������
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[2][0]);  //"�������:"
	strcat((char *)&linedata[0],(char *)"      ");  //����
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8�ֽڱ��
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//��ӡʱ��
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[3][0]);  //"ʱ    ��:"
	strncpy((char *)&data[0],(char *)&P->dat[0],16);  //  16�ֽ�ʱ��
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"       ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	
	//0:�����¶�
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[4][0]);  //"�����¶�"
	strcat((char *)&linedata[0],(char *)"    ");  //����
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[0][0],10);  //  10�ֽ�ʱ��
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)" ��       ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//1:����ѹ
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[5][0]);  //"����ѹ"
	strcat((char *)&linedata[0],(char *)"    ");  //����
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[1][0],10);  //  10�ֽ�ʱ��
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)" kPa     ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;		
	//1.0����
	//20����
	//30����
	//40����
	//50����
	//70����
	//90����
	
	for(i=2;i<=8;i++)
	{
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[i+4][0]);  //""
	strcat((char *)&linedata[0],(char *)"    ");  //����
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[i][0],10);  //  10�ֽ�ʱ��
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
  }
	//100����
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[50][0]);  //"100L����"
	strcat((char *)&linedata[0],(char *)"    ");  //����
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[i][0],10);  //  10�ֽ�ʱ��
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	
		//��վ
	 strcpy((char *)&linedata[0],(char *)"-----");  //"��վ"
	 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21�ֽ�
	 strcat((char *)&linedata[0],(char *)"------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	//�绰
	 strcpy((char *)&linedata[0],(char *)"---------");  //"�绰"
	 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13�ֽ�
	 strcat((char *)&linedata[0],(char *)"----------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	
	 P->LineMax=linecnt;  //�����������
}
/****************************************************************************
	��������PrintAPI_ConcentraParaOrder()
  ��  ��:
         
	��  ��:
	����˵����//��ӡŨ�Ȳ�������ת������ӡ����

*/
void PrintAPI_ConcentraParaOrder(Printer_Structure *P)        //��ӡŨ�Ȳ�������ת������ӡ����
{ char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //�ƴ�ӡ����
  u8 i;
	// �ͺ�+��ӡ���
	strncpy((char *)&LineData[linecnt][0],(char *)&p_ConcentraPara_hand[0],linenumber-1);  //
	linecnt++;		
		//�������
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[2][0]);  //"�������:"
	strcat((char *)&linedata[0],(char *)"      ");  //����
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8�ֽڱ��
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//��ӡʱ��
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[3][0]);  //"ʱ    ��:"
	strncpy((char *)&data[0],(char *)&P->dat[0],16);  //  16�ֽ�ʱ��
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"       ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	for(i=0;i<=27;i++)
	{
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[i+21][0]);  // 13�ֽ�
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[i][0],10);  //  10�ֽ�ʱ��
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //����
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	}

		//��վ
	 strcpy((char *)&linedata[0],(char *)"-----");  //"��վ"
	 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21�ֽ�
	 strcat((char *)&linedata[0],(char *)"------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	//�绰
	 strcpy((char *)&linedata[0],(char *)"---------");  //"�绰"
	 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13�ֽ�
	 strcat((char *)&linedata[0],(char *)"----------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	
	 P->LineMax=linecnt;  //�����������
}


//��ӡ������
/****************************************************************************
	��������Print_Init_Com
	��  ��: 
         
	��  ��:
	����˵������ӡ����ʼ��
******************************************************************************/
void Print_Init_Com(void)
{
	
	  Printf_GetData()->Printf_Mode=4;
		printf("%c",0x1B);
		printf("%c",0x40);
	  Printf_GetData()->Printf_Mode=1;
}
/****************************************************************************
	��������Print_Printorder_Com
	��  ��: 
         
	��  ��:
	����˵������ӡָ��
******************************************************************************/
void Print_Printorder_Com(void)
{
	
	  Printf_GetData()->Printf_Mode=4;
		printf("%c",0xD);
	  Printf_GetData()->Printf_Mode=1;
}
/****************************************************************************
	��������Print_ChangLine_Com
	��  ��: 
         
	��  ��:
	����˵��������ָ��
******************************************************************************/
void Print_ChangLine_Com(void)
{
	/*����ָ��*/
	  Printf_GetData()->Printf_Mode=4;
		printf("%c",0xA);
	  Printf_GetData()->Printf_Mode=1;
}
/****************************************************************************
	��������Print_Chk_Com
	��  ��: 
         
	��  ��:
	����˵������ѯָ��
******************************************************************************/
void Print_Chk_Com(void)
{
	
	 Printf_GetData()->Printf_Mode=4;
	 printf("%c",0x1B);
	 printf("%c",0x76);
	 Printf_GetData()->Printf_Mode=1;
}

/****************************************************************************
	��������PrintLine(u16 line)
	��  ��: line������ >=0
         
	��  ��:
	����˵������ӡ�ڼ�������
*******************************************************************************/

void PrintLine(u16 line)
{
	Printf_GetData()->Printf_Mode=4;
	printf("%s",&LineData[line][0]);   //��ӡ�ڼ�������
	Printf_GetData()->Printf_Mode=1;
}


/*******************************************************
                  �����Ǵ�ӡ����ӿ�
********************************************************/

/****************************************************************************
	��������Print_WorkFileOrder(Printer_Structure *P)
  ��  ��:Printer_Structure *P ����ӡ����,������Ԥ�ȴ�������
         
	��  ��:
	����˵������ӡ�����ļ�ָ��,�����
*******************************************************************************/
void Print_WorkFileOrder(Printer_Structure *P)  //��ӡ�����ļ�
{
	P->RunMode_high=11;
	P->print_cnt_file=P->print_start_file;
}
/****************************************************************************
	��������Print_GasFileOrder(Printer_Structure *P)
  ��  ��: Printer_Structure *P ����ӡ����,������Ԥ�ȴ�������
        
	��  ��:
	����˵������ӡ�����ļ�ָ��,�����
*******************************************************************************/
void Print_GasFileOrder(Printer_Structure *P)    //��ӡ�����ļ�
{
	P->RunMode_high=13;
	P->print_cnt_file=P->print_start_file;
}
/****************************************************************************
	��������Print_DustFileOrder(Printer_Structure *P)
  ��  ��: Printer_Structure *P ����ӡ����,������Ԥ�ȴ�������
       
	��  ��:
	����˵������ӡ�̳��ļ�ָ��,�����
*******************************************************************************/
void Print_DustFileOrder(Printer_Structure *P)    //��ӡ�̳��ļ�
{
	P->RunMode_high=15;
	P->print_cnt_file=P->print_start_file;
}
/****************************************************************************
	��������Print_GasRecordOrder(Printer_Structure *P)
  ��  ��: Printer_Structure *P ����ӡ����,������Ԥ�ȴ�������
        
	��  ��:
	����˵������ӡ������¼�ļ�,�����
*******************************************************************************/
void Print_GasRecordFileOrder(Printer_Structure *P) //��ӡ������¼�ļ�
{
	P->RunMode_high=17;
    P->print_cnt_file=P->print_start_file;  //�ӵ�һ��¼��ʼ
}

/****************************************************************************
	��������Print_PressureParaOrder(Printer_Structure *P)
  ��  ��:  Printer_Structure *P ����ӡ����,������Ԥ�ȴ�������
        
	��  ��:
	����˵������ӡѹ������ָ��,�����
*******************************************************************************/
void Print_PressureParaOrder(Printer_Structure *P)             //��ӡѹ������
{
	P->RunMode_high=1;
}
/****************************************************************************
	��������Print_TemperatureOrder(Printer_Structure *P)
  ��  ��: 
        
	��  ��:
	����˵������ӡ�¶��ļ�ָ��,�����
*******************************************************************************/
void Print_TemperatureParaOrder(Printer_Structure *P)          //��ӡ�¶Ȳ���
{
	P->RunMode_high=2;
}
/****************************************************************************
	��������Print_FlowParaOrder()
  ��  ��:
      
	��  ��:
	����˵������ӡ��������ָ��,�����
*******************************************************************************/
void Print_FlowParaOrder(Printer_Structure *P)                 //��ӡ��������
{
	P->RunMode_high=3;
}
/****************************************************************************
	��������Print_ConcentraOrder()
  ��  ��: 
        
	��  ��:
	����˵������ӡũ�Ȳ���ָ��,�����
*******************************************************************************/
void Print_ConcentraParaOrder(Printer_Structure *P)        //��ӡũ�Ȳ���
{
	P->RunMode_high=4;
}
/****************************************************************************
	��������Print_CurTestOrder()
  ��  ��:  Printer_Structure *P ����ӡ����,������Ԥ�ȴ�������
        
	��  ��:
	����˵������ӡ���ڲ���ָ��,�����
*******************************************************************************/
void Print_CurTestOrder(Printer_Structure *P)                  //��ӡ���ڲ���
{
	P->RunMode_high=5;
}
/****************************************************************************
	��������Print_Init(void
	��  ��: 
         
	��  ��:
	����˵������ӡģ���ʼ��
*******************************************************************************/
void Print_Init(void)
{
	Printer_Data.RunMode=0;
	Printer_Data.RunMode_high=0;
}

/****************************************************************************
	��������Print_Init(void
	��  ��: 
         
	��  ��:
	����˵������ӡģ�鶨ʱ��
*******************************************************************************/
void PrintTimer(void)
{
	if(Printer_Data.Timer>0)
		Printer_Data.Timer--;
	
	if(Printer_Data.Timer_high>0)
		Printer_Data.Timer_high--;
}

//
/****************************************************************************
	��������Print_Low_Main(void)
	��  ��: �����Ѿ����浽ָ���Ļ�����
         
	��  ��:
	����˵������ӡ�ĵͲ��߳�ֻ��ӡ������������

*/
void Print_Low_Main(void){
	switch(Printer_Data.RunMode)
		{
		case 0:{break;}
		case 1:{
			Print_Init_Com();         //��ӡ����ʼ��ָ��
			Printer_Data.Timer=500;   //��ʱ0.5s
			Printer_Data.Flag=0;      //����մ�ӡ��ɱ�־
			Printer_Data.RunMode = 2;
			break;
		}
		case 2:{
			if(Printer_Data.Timer==0)  //��ʱ�䵽
			{
				Printer_Data.RunMode = 6;   
			}
			break;
		}
		case 3:{
		
			break;
		}
		case 4:{
	
			break;
		}
		case 5:{
		
			break;
		}
		 //��ʼ��ӡʱ��
		case 6:{
			Print_Chk_Com();  //������ѯ״ָ̬��
			USART_COM4_Receve_com(0,1);  //͸��ģʽҪ�󴮿ڽ���һ���ֽ�
			Printer_Data.Timer=3000;   //��ʱ3���ѯ״̬�����ݴ�ӡһ������ʱ������
			Printer_Data.RunMode = 7;
			break;
		}
		case 7:
		{
			if(Printer_Data.Timer==0)  //��ʱ�˳�����
			{
				Printer_Data.RunMode =0;
				Printer_Data.Flag=2;//ʱ�����
			}
			else
			{
		  if(USART_COM4_GetData()->ReceveOkFlg)  //������
			  {
          if ((USART_COM4_GetData()->USART_RX_BUF[0]&0X0F) == 0x0000) 
					{
						Printer_Data.RunMode =8;
						Printer_Data.line=0;
					}
			  }
				else
				{
					Print_Chk_Com();  //������ѯ״ָ̬��
			    USART_COM4_Receve_com(0,1);  //͸��ģʽҪ�󴮿ڽ���һ���ֽ�
				}
		
		}
			break;	
	}
		case 8:
		{
						PrintLine(Printer_Data.line);  //��ӡ�ڼ���
			      Print_ChangLine_Com();         //�س�����
						Printer_Data.line++;                                 //��������ж��Ƿ񳬹���ӡ��2K����
		        if(Printer_Data.line>=Printer_Data.LineMax)  //��ӡ���
						{
							Printer_Data.RunMode =9;
						}
					
			break;
		}
		case 9:
		{ 
  		 Print_Printorder_Com();    //��ӡ����
			 Printer_Data.Timer=2000;   //��ʱ2s
			 Printer_Data.RunMode = 10;
			break;
		}
		case 10:
		{
			if(Printer_Data.Timer==0)
			{
					Print_Chk_Com();  //������ѯ״ָ̬��
			    USART_COM4_Receve_com(0,1);  //͸��ģʽҪ�󴮿ڽ���һ���ֽ�
				  Printer_Data.RunMode =11;
				  Printer_Data.Timer=2000;   //��ʱ2s
			}
			break;
		}
		case 11:
		{ 
			if(USART_COM4_GetData()->ReceveOkFlg)  //������
			  {
          if ((USART_COM4_GetData()->USART_RX_BUF[0]&0X0F) == 0x0000) 
					{
						Printer_Data.RunMode =0;
						Printer_Data.Flag=1;
					}
			  }
				else
				{
					Print_Chk_Com();  //������ѯ״ָ̬��
			    USART_COM4_Receve_com(0,1);  //͸��ģʽҪ�󴮿ڽ���һ���ֽ�
				}
			if(Printer_Data.Timer==0)  //��ʱ
				{
	        Printer_Data.RunMode =0;
					Printer_Data.Flag=2;//��ʱ����
				}				
			
			break;
		}
 }
}

//��ӡ�ĸ߲���
void Print_High_Main(void)
	{
	switch(Printer_Data.RunMode_high)
		{
		case 0:
		{
			break;
		}
		//��ӡ����
		case 1:  	//��ӡѹ����������,��ת���Ǹ�С�߳�
		{
		  PrintAPI_PressureParaOrder(&Printer_Data); //������ת��
			Printer_Data.RunMode=1;       //�����ײ��ӡ���
			Printer_Data.Flag_high=0;     //��׼����
			Printer_Data.Timer_high=10000; //10����
			Printer_Data.RunMode_high=10;  //������һ���ȴ���ӡ���
			break;
		}
		case 2:  	//��ӡ�¶Ȳ�������,��ת���Ǹ�С�߳�
		{
		  PrintAPI_TemperatureParaOrder(&Printer_Data); //������ת��
			Printer_Data.RunMode=1;       //�����ײ��ӡ���
			Printer_Data.Flag_high=0;     //��׼����
			Printer_Data.Timer_high=10000; //10����
			Printer_Data.RunMode_high=10;  //������һ���ȴ���ӡ���
			break;
		}
		case 3:  	//��ӡ������������,��ת���Ǹ�С�߳�
		{
		  PrintAPI_FlowParaOrder(&Printer_Data); //������ת��
			Printer_Data.RunMode=1;       //�����ײ��ӡ���
			Printer_Data.Flag_high=0;     //��׼����
			Printer_Data.Timer_high=10000; //10����
			Printer_Data.RunMode_high=10;  //������һ���ȴ���ӡ���
			break;
		}
		case 4:  	//��ӡŨ�Ȳ�������,��ת���Ǹ�С�߳�
		{
		  PrintAPI_ConcentraParaOrder(&Printer_Data); //������ת��
			Printer_Data.RunMode=1;       //�����ײ��ӡ���
			Printer_Data.Flag_high=0;     //��׼����
			Printer_Data.Timer_high=10000; //10����
			Printer_Data.RunMode_high=10;  //������һ���ȴ���ӡ���
			break;
		}
			case 5:  	//��ӡŨ�Ȳ�������,��ת���Ǹ�С�߳�
		{
		  PrintAPI_CurTest(&Printer_Data); //������ת��
			Printer_Data.RunMode=1;       //�����ײ��ӡ���
			Printer_Data.Flag_high=0;     //��׼����
			Printer_Data.Timer_high=10000; //10����
			Printer_Data.RunMode_high=10;  //������һ���ȴ���ӡ���
			break;
		}	

		case 10:        //С��������
		{
			if(Printer_Data.Timer_high==0)
			{
				Printer_Data.Flag_high=2;//��ʱ���
				Printer_Data.RunMode_high=0;          //��صȴ�
				printf("��ӡ���");
			}
			else if(Printer_Data.Flag>0)
			{
				Printer_Data.Flag_high=Printer_Data.Flag; //����ȡ�ײ�������
				Printer_Data.RunMode_high=0;          //��صȴ�ģʽ
				if(Printer_Data.Flag==1)
				{
					Printer_Data.Flag_high=1;     //��ӡ���
					printf("��ӡ���");
				}
				else if(Printer_Data.Flag==2)
				{ Printer_Data.Flag_high=2;     //��׼����
					printf("��ӡ���");
				}
			}
			break;
		}
		
		
		case 11:   //��ӡ�����ļ�
		{
			//�ȶ�ȡSD��������,�ȶ�ȡ����ļ���
			SdFile_Serch_MaxofWorkLinkFile();   //�������������ļ�����ļ���
            Printer_Data.Flag_high=0;     //��׼����		
			Printer_Data.RunMode_high=12;
		
			break;
		}
		case 12:
		{
			SdFile_ReadWorkFile(Printer_Data.print_cnt_file); //��ȡ�����ļ���¼
	    PrintAPI_WorkFile(&Printer_Data,&SdFile_GetData()->rdWorkFile);
			Printer_Data.Timer_high=10000; //10����
			Printer_Data.RunMode_high=30;  //������һ���ȴ���ӡ���
			Printer_Data.NextMode_high=12;
			Printer_Data.RunMode=1;       //�����ײ��ӡ���
			
			break;
		}
		
		case 13:  //��ӡ����
		{
			//�ȶ�ȡSD��������,�ȶ�ȡ����ļ���
			SdFile_Serch_MaxofGasLinkFile();   //�������������ļ�����ļ���
			Printer_Data.Flag_high=0;     //��׼����		
			Printer_Data.RunMode_high=14;
			break;
		}
		case 14:
		{
			SdFile_ReadGasFile(Printer_Data.print_cnt_file,0x01); //��ȡ�����ļ���¼
			PrintAPI_GasFile(&Printer_Data,&SdFile_GetData()->rdWorkFile);
			Printer_Data.Timer_high=10000; //10����
			Printer_Data.RunMode_high=30;  //������һ���ȴ���ӡ���
			Printer_Data.NextMode_high=14;
			Printer_Data.RunMode=1;       //�����ײ��ӡ���
			break;
		}
		case 15:  //��ӡ�̳�
		{
			//�ȶ�ȡSD��������,�ȶ�ȡ����ļ���
			SdFile_Serch_MaxofDustLinkFile();   //�������������ļ�����ļ���
      Printer_Data.Flag_high=0;     //��׼����		
			Printer_Data.RunMode_high=16;
      break;
		}
		case 16:
		{
			SdFile_ReadDustFile(Printer_Data.print_cnt_file); //��ȡ�̳��ļ���¼
	        PrintAPI_DustFile(&Printer_Data,&SdFile_GetData()->rdWorkFile);
			Printer_Data.Timer_high=10000; //10����
			Printer_Data.RunMode_high=30;  //������һ���ȴ���ӡ���
			Printer_Data.NextMode_high=16;
			Printer_Data.RunMode=1;       //�����ײ��ӡ���
      break;
		}
		case 17:  //��ӡ�������Ӽ�¼
		{
			SdFile_Serch_MaxofGasLinkFile();   //�������������ļ�����ļ���
			SdFile_Serch_MaxofGasRecord(Printer_Data.print_FileNomber);  //��ǰ�ļ����
			Printer_Data.Flag_high=0;       //��׼����	
			Printer_Data.RunMode_high=18;
			
			break;
		}
		case 18:
		{
			if(Printer_Data.record_Cnt >= SdFile_GetData()->GasRecordMax)
			{
				Printer_Data.RunMode_high=0;          //��صȴ�ģʽ
				Printer_Data.Flag_high=1;     //��ӡ���
				return ;
			}
			if(Printer_Data.PrintSelect==0) //�����ӡ
			{				
				SdFile_ReadGasFile(Printer_Data.print_FileNomber,Printer_Data.print_cnt_file);  //��ȡ�ļ�����
			}
			else
			{
			    u8 reordNum;				
				while(Printer_Data.record_Cnt< SdFile_GetData()->GasRecordMax)	//�����������ݡ���ӡѡ�С����ܲ��ܴ�ӡȫ�����ݵ�����, modify by alfred, 2020.07.07
				{     
					Printer_Data.record_Cnt++;						
					reordNum=Printer_Data.record_Cnt; 
					SdFile_ReadGasFile(Printer_Data.print_FileNomber,reordNum);  //��ȡ�ļ�����
					break;		
				}	
			}
			PrintAPI_GasRecordFile(&Printer_Data,&SdFile_GetData()->rdWorkFile);
			Printer_Data.Timer_high=10000; //10����
			Printer_Data.RunMode_high=30;  //������һ���ȴ���ӡ���
			Printer_Data.NextMode_high=18;
			Printer_Data.RunMode=1;       //�����ײ��ӡ���
      break;

		case 30:
		{
			
			if(Printer_Data.Timer_high==0)
			{
				Printer_Data.Flag_high=2;//��ʱ���
				Printer_Data.RunMode_high=0;          //��صȴ�ģʽ
				printf("��ӡ���");
			}
			else if(Printer_Data.Flag>0)
			{
				Printer_Data.Flag_high=Printer_Data.Flag; //����ȡ�ײ�������
				
				if(Printer_Data.Flag==1)
				{
					printf("/r/n��ӡ����ļ�%d:",Printer_Data.print_cnt_file);
					
					if(Printer_Data.PrintSelect==0) //�����ӡ
					{
						Printer_Data.print_cnt_file++;
						if(Printer_Data.print_cnt_file>Printer_Data.print_end_file)
						{
							Printer_Data.RunMode_high=0;          //��صȴ�ģʽ
							Printer_Data.Flag_high=1;     //��ӡ���
							printf("/r/n�����ļ���ӡ���");
						}
						else
						{
							Printer_Data.RunMode_high=Printer_Data.NextMode_high; //��ӡ��һ���ļ�
							
						}
						
					}else  //��ӡѡ��
					{
					  if(Printer_Data.record_Cnt>63)
						{
							Printer_Data.RunMode_high=0;          //��صȴ�ģʽ
							Printer_Data.Flag_high=1;     //��ӡ���
							printf("/r/n�����ļ���ӡ���");
						}
						else
						{
							Printer_Data.RunMode_high=Printer_Data.NextMode_high; //��ӡ��һ���ļ�
							
						}
					}
				}
				else if(Printer_Data.Flag==2)
				{ Printer_Data.Flag_high=2;     //��׼����
					Printer_Data.RunMode_high=0;          //��صȴ�ģʽ
					printf("��ӡ���");
				}
			}
			break;
		}
		default:
    break;			
		}
  }
}
	//---	
//��ӡ����̣߳����������
void Print_Main(void)
{
	Print_Low_Main();
	Print_High_Main();
}	


//-------------------------���Ժ���----------------------------
//---
/*
	�������� void Print_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪAT.c�ļ���ASCII�����ļ�
						:000|05|001
*/
void Print_DebugASCII(u8 *p,u8 len)
{ u8 i;
	//char buf[202];
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:
		{//
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
			strcpy((char *)&Printer_Data.bianhao[0],(char *)"12345678");	
			strcpy((char *)&Printer_Data.dat[0],(char *)"2019-12-20 14:20");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[0][0],(char *)"0000000000");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[1][0],(char *)"1111111111");
			strcpy((char *)&Printer_Data.Para_StrData.databuf[2][0],(char *)"2222222222");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[3][0],(char *)"3333333333");	
			strcpy((char *)&Printer_Data.Para_StrData.databuf[4][0],(char *)"4444444444");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[5][0],(char *)"5555555555");	
			Print_PressureParaOrder(&Printer_Data);
			}
			else
			{
				printf("����͸��ģʽ");
			}
			break;
		}
		case 2:
		{//
		if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
			strcpy((char *)&Printer_Data.bianhao[0],(char *)"12345678");	
			strcpy((char *)&Printer_Data.dat[0],(char *)"2019-12-20 14:20");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[0][0],(char *)"0000000000");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[1][0],(char *)"1111111111");
			strcpy((char *)&Printer_Data.Para_StrData.databuf[2][0],(char *)"2222222222");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[3][0],(char *)"3333333333");	
			strcpy((char *)&Printer_Data.Para_StrData.databuf[4][0],(char *)"4444444444");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[5][0],(char *)"5555555555");	
			Print_TemperatureParaOrder(&Printer_Data);
			}
			else
			{
				printf("����͸��ģʽ");
			}
			break;
		
		}
		case 3:
		{//
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
			strcpy((char *)&Printer_Data.bianhao[0],(char *)"12345678");	
			strcpy((char *)&Printer_Data.dat[0],(char *)"2019-12-20 14:20");
				
      strcpy((char *)&Printer_Data.Para_StrData.databuf[0][0],(char *)"0000000000");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[1][0],(char *)"1111111111");
			strcpy((char *)&Printer_Data.Para_StrData.databuf[2][0],(char *)"2222222222");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[3][0],(char *)"3333333333");	
			strcpy((char *)&Printer_Data.Para_StrData.databuf[4][0],(char *)"4444444444");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[5][0],(char *)"5555555555");	
			strcpy((char *)&Printer_Data.Para_StrData.databuf[6][0],(char *)"6555555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[7][0],(char *)"7555555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[8][0],(char *)"8555555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[9][0],(char *)"9555555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[10][0],(char *)"5155555555");					
			Print_FlowParaOrder(&Printer_Data);
			}
			else
			{
				printf("����͸��ģʽ");
			}
			break;
		}
		case 4:
		{
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
			strcpy((char *)&Printer_Data.bianhao[0],(char *)"12345678");	
			strcpy((char *)&Printer_Data.dat[0],(char *)"2019-12-20 14:20");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[0][0],(char *)"0000000000");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[1][0],(char *)"1111111111");
			strcpy((char *)&Printer_Data.Para_StrData.databuf[2][0],(char *)"2222222222");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[3][0],(char *)"3333333333");	
			strcpy((char *)&Printer_Data.Para_StrData.databuf[4][0],(char *)"4444444444");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[5][0],(char *)"5555555555");	
			strcpy((char *)&Printer_Data.Para_StrData.databuf[6][0],(char *)"6555555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[7][0],(char *)"7555555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[8][0],(char *)"8555555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[9][0],(char *)"9555555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[10][0],(char *)"5655555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[11][0],(char *)"5755555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[12][0],(char *)"5855555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[13][0],(char *)"5955555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[14][0],(char *)"5515555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[15][0],(char *)"5525555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[16][0],(char *)"5535555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[17][0],(char *)"5545555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[18][0],(char *)"5555555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[19][0],(char *)"5565555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[20][0],(char *)"5575555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[21][0],(char *)"5585555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[22][0],(char *)"5595555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[23][0],(char *)"5551555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[24][0],(char *)"5552555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[25][0],(char *)"5553555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[26][0],(char *)"5554555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[27][0],(char *)"5555555555");	
      strcpy((char *)&Printer_Data.Para_StrData.databuf[28][0],(char *)"5556555555");	
				
			Print_ConcentraParaOrder(&Printer_Data);
			}
			else
			{
				printf("����͸��ģʽ");
			}
			break;
		}
		case 5:
		{
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
			strcpy((char *)&Printer_Data.bianhao[0],(char *)"12345678");	
			strcpy((char *)&Printer_Data.dat[0],(char *)"2019-12-20 14:20");
			strcpy((char *)&Printer_Data.address[0],(char *)"�㶫˳�´���xxxxxxxxXXXXXXXXXXxxxxxxxxxxXXXXXXXXXX");


			strcpy((char *)&Printer_Data.Para_StrData.databuf[0][0],(char *)"0000000000");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[1][0],(char *)"1111111111");
			strcpy((char *)&Printer_Data.Para_StrData.databuf[2][0],(char *)"2222222222");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[3][0],(char *)"3333333333");	
			strcpy((char *)&Printer_Data.Para_StrData.databuf[4][0],(char *)"4444444444");
      strcpy((char *)&Printer_Data.Para_StrData.databuf[5][0],(char *)"5555555555");	
		  strcpy((char *)&Printer_Data.Para_StrData.databuf[6][0],(char *)"6666666666");	

      for(i=0;i<8;i++)
			Printer_Data.installselect[i]=1;
			for(i=0;i<33;i++)
			Printer_Data.pselect[i]=1;	
				
			Print_CurTestOrder(&Printer_Data);
			}
			else
			{
				printf("����͸��ģʽ");
			}
			break;
		}
		case 6:    //��ӡ�����ļ�
		{
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{

			Printer_Data.print_start_file=1;
      Printer_Data.print_end_file=3;
			strcpy((char *)&Printer_Data.bianhao[0],(char *)"12345678");	
			strcpy((char *)&Printer_Data.dat[0],(char *)"2019-12-20 14:20");
			
			for(i=0;i<33;i++)
			Printer_Data.pselect[i]=1;	
			Print_WorkFileOrder(&Printer_Data);
			}
			else
			{
				printf("����͸��ģʽ");
			}
			
			break;
		}
		case 7:
		{
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
			Printer_Data.print_start_file=1;
            Printer_Data.print_end_file=1;
			strcpy((char *)&Printer_Data.bianhao[0],(char *)"12345678");	
			strcpy((char *)&Printer_Data.dat[0],(char *)"2019-12-20 14:20");

			for(i=0;i<33;i++)
			Printer_Data.pselect[i]=1;	

			Print_GasFileOrder(&Printer_Data);
			}
			else
			{
				printf("����͸��ģʽ");
			}
			break;
		}
		case 8:
		{
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
			Printer_Data.print_start_file=1;
            Printer_Data.print_end_file=1;
			strcpy((char *)&Printer_Data.bianhao[0],(char *)"12345678");	
			strcpy((char *)&Printer_Data.dat[0],(char *)"2019-12-20 14:20");
		
			for(i=0;i<33;i++)
			Printer_Data.pselect[i]=1;	

				
			Print_DustFileOrder(&Printer_Data);
			}
			else
			{
				printf("����͸��ģʽ");
			}
			break;
		}
		case 9:
		{
			if(AT_GetData()->Mode==0)     //��͸��ģʽ
			{
			Printer_Data.print_start_file=1;   // //�ӵ�1��¼��ʼ
			Printer_Data.print_end_file=2;    //�����ļ���   //��������Printer_Data.print_start_file����ǰ�����ɴ�ӡ��ǰ
			Printer_Data.print_FileNomber=9;  //9���ļ�	
       
			strcpy((char *)&Printer_Data.bianhao[0],(char *)"12345678");	
			strcpy((char *)&Printer_Data.dat[0],(char *)"2019-12-20 14:20");
			Print_GasRecordFileOrder(&Printer_Data);
			}
			else
			{
				printf("����͸��ģʽ");
			}
			break;
		}
		case 10:
		{
			break;
		}
		
		default:
		{
		break;
		}
	}
}

