//#include "sys.h"
//#include "usart.h"
//#include "delay.h"
//#include "stdlib.h" 
//#include "string.h"
//#include "stdio.h"
#include "Printer.h"
#define linenumber 33

const char mechine_type[linenumber] = "           YLB-2700��           ";
const char mechine_name[linenumber] = "   ���������������ۺϲ�����     ";
const char instrument_number[linenumber] = "������ţ�";
const char line_change[linenumber] = "  ";
const char divider[linenumber] = "--------------------------------";
const char comp_name[linenumber] = "   ��ɽ��¡�������Ƽ����޹�˾   ";
const char comp_web[linenumber] =  "     ��˾����:www.wwwww.cn      ";
const char comp_phone[linenumber] ="     �ͷ�����:0757-22207829     ";

const char sample_data[linenumber] = "------------��������------------";
const char sample_name[11][16] = { "�ļ���: "," ·��������","��������: ","����ʱ��: ","��������: ","�ۼ����: ","�α����: ","��ǰ�¶�: ","��ǰѹ��: ","�����¶�: ","�� �� ѹ: "};

char shunshi_data[linenumber] = "------------˲ʱ����------------";
const char shunshi_name[12][16] = {"�ļ���: ","˲ʱ���: "," ·��������","��������: ","����ʱ��: ","��������: ","�ۼ����: ","�α����: ","��ǰ�¶�: ","��ǰѹ��: ","�����¶�: ","�� �� ѹ: "};

char spower_data[linenumber] = "------------�����¼------------";
const char spower_name[4][16] = {"�ļ���: ","����ʱ��: ","����ʱ��: ","����ʱ��: "};

const char danwei[4][16] = {"       L",
                            "       ��C",
                            "      kPa",
                            "        L/min"
                           };

char Line1data[linenumber]={0};
char Line2data[linenumber]={0};
char Line3data[linenumber]={0};
char Line4data[linenumber]={0};
char Line5data[linenumber]={0};
char Line6data[linenumber]={0};
char Line7data[linenumber]={0};
char Line8data[linenumber]={0};
char Line9data[linenumber]={0};
char Line10data[linenumber]={0};
char Line11data[linenumber]={0};
char Line12data[linenumber]={0};
char Line13data[linenumber]={0};
char Line14data[linenumber]={0};
char Line15data[linenumber]={0};
char Line16data[linenumber]={0};
char Line17data[linenumber]={0};
char Line18data[linenumber]={0};
char Line19data[linenumber]={0};
char Line20data[linenumber]={0};
char Line21data[linenumber]={0};


Printer_Structure Printer_Data;


Printer_Structure *Printer_GetData(void){    //����Printer_Data�ṹ���ַ
	return &Printer_Data;
}


void PrintAPI_Ssample(Ssample_file *sample) {

	char linedata[linenumber]={0};
	char data[linenumber]={0};

	{
			strncpy((char *)&linedata[0],(char *)&mechine_type[0],linenumber-1);
			strncpy((char *)&Line1data[0],(char *)&linedata[0],linenumber-1);
			
		}
	
		{
			strncpy((char *)&linedata[0],(char *)&mechine_name[0],linenumber-1);
			strncpy((char *)&Line2data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&sample_data[0],linenumber-1);
			strncpy((char *)&Line3data[0],(char *)&linedata[0],linenumber-1);
			
		}
		{
			strcpy((char *)&linedata[0],(char *)&instrument_number[0]); //�������
			strcat((char *)&linedata[0],(char *)&Printer_Data.yiqibianhao[0]);
			strncpy((char *)&Line4data[0],(char *)&linedata[0],linenumber-1);
		}
		
		{
			strncpy((char *)&linedata[0],(char *)&sample_name[0][0],strlen((char *)&sample_name[0][0])+1);
			strncpy((char *)&data[0],(char *)&sample->filenumber[0],6);
			data[6]=0;
			strcat((char *)&linedata[0],(char *)&data[0]); 
			strncpy((char *)&Line5data[0],(char *)&linedata[0],linenumber-1);
			
		}
		{
			strncpy((char *)&linedata[0],(char *)&sample->sampletype[0],1);    // A ·��������
			linedata[1]=0;
			strcat((char *)&linedata[0],(char *)&sample_name[1][0]);
			strncpy((char *)&Line6data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&sample_name[2][0],strlen((char *)&sample_name[2][0])+1); //��������
			strncpy((char *)&data[0],(char *)&sample->sampletime[0],17);
			data[17]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strncpy((char *)&Line7data[0],(char *)&linedata[0],linenumber-1);
		
		}
		
		{
      strncpy((char *)&linedata[0],(char *)&sample_name[3][0],strlen((char *)&sample_name[3][0])+1);
			strncpy((char *)&data[0],(char *)&sample->running_time[0],11);
			data[11]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strncpy((char *)&Line8data[0],(char *)&linedata[0],linenumber-1);
			
		}
		{
			strncpy((char *)&linedata[0],(char *)&sample_name[4][0],strlen((char *)&sample_name[4][0])+1);
			strncpy((char *)&data,(char *)&sample->sample_flow_set[0],5);
			data[5]=0;
			strcat((char *)&linedata[0], (char *)&data[0]);
			strcat((char *)&linedata[0], (char *)&danwei[3][0]);
			strncpy((char *)&Line9data[0],(char *)&linedata[0],linenumber-1);
		
		}
		
		{
			strncpy((char *)&linedata[0],(char *)&sample_name[5][0],strlen((char *)&sample_name[5][0])+1);
			strncpy((char *)&data[0],(char *)&sample->volume_leiji[0],9);
			data[9]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],(char *)&danwei[0][0]);
			strncpy((char *)&Line10data[0],(char *)&linedata[0],linenumber-1);
		
		}

		{
			strncpy((char *)&linedata[0],(char *)&sample_name[6][0],strlen((char *)&sample_name[6][0])+1);
			strncpy((char *)&data[0],(char *)&sample->volume_biaokuang[0],9);
			data[9]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],(char *)&danwei[0][0]);
			strncpy((char *)&Line11data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
		  strncpy((char *)&linedata[0],(char *)&sample_name[7][0],strlen((char *)&sample_name[7][0])+1);
			strncpy((char *)&data[0],(char *)&sample->temperature[0],5);
			data[5]=0;
			strcat((char *)&linedata[0], (char *)&data[0]);
			strcat((char *)&linedata[0],(char *)& danwei[1][0]);
			strncpy((char *)&Line12data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&sample_name[8][0],strlen((char *)&sample_name[8][0])+1);
			strncpy((char *)&data[0],(char *)&sample->jy[0],6);
			data[6]=0;
			strcat((char *)&linedata[0], (char *)&data[0]);
			strcat((char *)&linedata[0],(char *)&danwei[2][0]);
			strncpy((char *)&Line13data[0],(char *)&linedata[0],linenumber-1);
			
		}
	 {
			strncpy((char *)&linedata[0],(char *)&sample_name[9][0],strlen((char *)&sample_name[9][0])+1);
			strncpy((char *)&data[0],(char *)&sample->environment_temperature[0],5);
		  data[5]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],(char *)&danwei[1][0]);
			strncpy((char *)&Line14data[0],(char *)&linedata[0],linenumber-1);
		
		}
	{
			strncpy((char *)&linedata[0],(char *)&sample_name[5][0],strlen((char *)&sample_name[5][0])+1);
			strncpy((char *)&data[0],(char *)&sample->barometric[0],6);
		  data[6]=0;
			strcat((char *)&linedata[0], (char *)&data[0]);
			strcat((char *)&linedata[0],(char *)&danwei[2][0]);
			strncpy((char *)&Line15data[0],(char *)&linedata[0],linenumber-1);
			
		}
		{
			strncpy((char *)&linedata[0],(char *)&line_change[0],linenumber-1);
			strncpy(&Line16data[0],&linedata[0],linenumber-1);
		
		}
	{
			strncpy((char *)&linedata[0],(char *)&divider[0],linenumber-1);
			strncpy(&Line17data[0],&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&comp_name[0],linenumber-1);
			strncpy(&Line18data[0],&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&comp_web[0],linenumber-1);
			strncpy(&Line19data[0],&linedata[0],linenumber-1);
		
		}
	{
			strncpy((char *)&linedata[0],(char *)&comp_phone[0],linenumber-1);
			strncpy(&Line20data[0],&linedata[0],linenumber-1);
		
		}

}


void PrintAPI_Sshunshi(Ssample_file *shunshi){
	
	char linedata[linenumber]={0};
	char data[linenumber]={0};
	  {
			strncpy((char *)&linedata[0],(char *)&mechine_type[0],linenumber-1);  //�ͺ�
			strncpy((char *)&Line1data[0],(char *)&linedata[0],linenumber-1);
			
		}
	  {
			strncpy((char *)&linedata[0],(char *)&mechine_name[0],linenumber-1);  //��������
			strncpy((char *)&Line2data[0],(char *)&linedata[0],linenumber-1);
		
		}
	  {
			strncpy((char *)&linedata[0],(char *)&shunshi_data[0],linenumber-1);   //
			strncpy((char *)&Line3data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strcpy((char *)&linedata[0],(char *)&instrument_number[0]); //�������
			strcat((char *)&linedata[0],(char *)&Printer_Data.yiqibianhao[0]);  //��ȡ���
			strncpy((char *)&Line4data[0],(char *)&linedata[0],linenumber-1);
		
		}
	  {
			strncpy((char *)&linedata[0],(char *)&shunshi_name[0][0],strlen((char *)&shunshi_name[0][0])+1);  //�ļ���
			strncpy((char *)&data[0],(char *)&shunshi->filenumber[0],6);
			data[6]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strncpy((char *)&Line5data[0],(char *)&linedata[0],linenumber-1);
		}
		
		{
			strncpy((char *)&linedata[0],(char *)&shunshi_name[1][0],strlen((char *)&shunshi_name[1][0])+1);  //˲ʱ��ţ�
			sprintf((char*)&data[0],"%06u",Printer_Data.ShunshiCur);//��ǰ����
			data[6]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],"/");
			sprintf((char*)&data[0],"%06u",Printer_Data.ShunshiFileNum);//�ļ���
			data[6]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
  
			strncpy((char *)&Line6data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&shunshi->sampletype[0],1);
		  linedata[1]=0;
		
			strcat((char *)&linedata[0],(char *)&shunshi_name[2][0]);
			strncpy((char *)&Line7data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&shunshi_name[3][0],strlen((char *)&shunshi_name[3][0])+1);
			strncpy((char *)&data[0],(char *)&shunshi->sampletime[0],17);
			data[17]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strncpy((char *)&Line8data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&shunshi_name[4][0],strlen((char *)&shunshi_name[4][0])+1);
			strncpy((char *)&data[0],(char *)&shunshi->running_time[0],11);
			data[11]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strncpy((char *)&Line9data[0],(char *)&linedata[0],linenumber-1);
			
		}
		{
			strncpy((char *)&linedata[0],(char *)&shunshi_name[5][0],strlen((char *)&shunshi_name[5][0])+1);
			strncpy((char *)&data[0],(char *)&shunshi->sample_flow_set[0],5);
			data[5]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],(char *)&danwei[3][0]);
			strncpy((char *)&Line10data[0],(char *)&linedata[0],linenumber-1);
			
		}
		{
			strncpy((char *)&linedata[0],(char *)&shunshi_name[6][0],strlen((char *)&shunshi_name[6][0])+1);
			strncpy((char *)&data[0],(char *)&shunshi->volume_leiji[0],9);
			data[9]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],(char *)&danwei[0][0]);
			strncpy((char *)&Line11data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&shunshi_name[7][0],strlen((char *)&shunshi_name[7][0])+1);
			strncpy((char *)&data[0],(char *)&shunshi->volume_biaokuang[0],9);
			data[9]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],(char *)&danwei[0][0]);
			strncpy((char *)&Line12data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&shunshi_name[8][0],strlen((char *)&shunshi_name[8][0])+1);
			strncpy((char *)&data[0],(char *)&shunshi->temperature[0],5);
			data[5]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],(char *)&danwei[1][0]);
			strncpy((char *)&Line13data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&shunshi_name[9][0],strlen((char *)&shunshi_name[9][0])+1);
			strncpy((char *)&data[0],(char *)&shunshi->jy[0],6);
			data[6]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],(char *)&danwei[2][0]);
			strncpy((char *)&Line14data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&sample_name[9][0],strlen((char *)&sample_name[9][0])+1);
			strncpy((char *)&data[0],(char *)&shunshi->environment_temperature[0],5);
			data[5]=0;
			strcat((char *)&linedata[0],(char *)& data[0]);
			strcat((char *)&linedata[0],(char *)&danwei[1][0]);
			strncpy((char *)&Line15data[0],(char *)&linedata[0],linenumber-1);
			
		}
		{
			strncpy((char *)&linedata[0],(char *)&shunshi_name[11][0],strlen((char *)&shunshi_name[11][0])+1);
			strncpy((char *)&data[0],(char *)&shunshi->barometric[0],6);
			data[6]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],(char *)&danwei[2][0]);
			strncpy((char *)&Line16data[0],(char *)&linedata[0],linenumber-1);
			
		}
		{
			strncpy((char *)&linedata[0],(char *)&line_change[0],linenumber-1);
			strncpy((char *)&Line17data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&divider[0],linenumber-1);
			strncpy((char *)&Line18data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&comp_name[0],linenumber-1);
			strncpy((char *)&Line19data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&comp_web[0],linenumber-1);
			strncpy((char *)&Line20data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&comp_phone[0],linenumber-1);
			strncpy((char *)&Line21data[0],(char *)&linedata[0],linenumber-1);
		
		}
		
}

void PrintAPI_Spower(sPOWER_FILE *spower){
	char linedata[linenumber+1]={0};
	char data[linenumber]={0};
		{
			strncpy((char *)&linedata[0],(char *)&mechine_type[0],linenumber-1);  //�ͺ�
			strncpy((char *)&Line1data[0],(char *)&linedata[0],linenumber-1);
		
		}
	  {
			strncpy((char *)&linedata[0],(char *)&mechine_name[0],linenumber-1);   //��������
			strncpy((char *)&Line2data[0],(char *)&linedata[0],linenumber-1);
		
		}
	  {
			strncpy((char *)&linedata[0],(char *)&spower_data[0],linenumber-1);   //��ӡ����
			strncpy((char *)&Line3data[0],(char *)&linedata[0],linenumber-1);
		
		}
	  {

			strcpy((char *)&linedata[0],(char *)&instrument_number[0]); //�������
			strcat((char *)&linedata[0],(char *)&Printer_Data.yiqibianhao[0]);
			strncpy((char *)&Line4data[0],(char *)&linedata[0],linenumber-1);
			
		}
	  {
			strncpy((char *)&linedata[0],(char *)&spower_name[0][0],strlen((char *)&spower_name[0][0])+1);
			strncpy((char *)&data[0],(char *)&spower->filenumber[0],4);
		  data[4]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strncpy((char *)&Line5data[0],(char *)&linedata[0],linenumber-1);
			
		}
	{
			strncpy((char *)&linedata[0],(char *)&spower_name[1][0],strlen((char *)&spower_name[1][0])+1);
			strncpy((char *)&data[0],(char *)&spower->lostpowertime[0],17);
		  data[17]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strncpy((char *)&Line6data[0],(char *)&linedata[0],linenumber-1);
		
		}
	{
			strncpy((char *)&linedata[0],(char *)&spower_name[2][0],strlen((char *)&spower_name[2][0])+1);
			strncpy((char *)&data[0],(char *)&spower->powertime[0],17);
		  data[17]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strncpy((char *)&Line7data[0],(char *)&linedata[0],linenumber-1);
		
		}
	{
			strncpy((char *)&linedata[0],(char *)&spower_name[3][0],strlen((char *)&spower_name[3][0])+1);
			strncpy((char *)&data[0],(char *)&spower->running_time[0],11);
		  data[11]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strncpy((char *)&Line8data[0],(char *)&linedata[0],linenumber-1);
		
		}
	{
			strncpy((char *)&linedata[0],(char *)&line_change[0],linenumber-1);
			strncpy((char *)&Line9data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&divider[0],linenumber-1);
			strncpy((char *)&Line10data[0],(char *)&linedata[0],linenumber-1);
			
		}
		{
			strncpy((char *)&linedata[0],(char *)&comp_name[0],linenumber-1);
			strncpy((char *)&Line11data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&comp_web[0],linenumber-1);
			strncpy((char *)&Line12data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&comp_phone[0],linenumber-1);
			strncpy((char *)&Line13data[0],(char *)&linedata[0],linenumber-1);
		
		}
	
}




//��ӡ������
void Print_Init_Com(void)
{
	/*��ӡ����ʼ��*/
	  Printf_GetData()->Printf_Mode=4;
		printf("%c",0x1B);
		printf("%c",0x40);
	  Printf_GetData()->Printf_Mode=1;
}

void Print_Printorder_Com(void)
{
	/*��ӡָ��*/
	  Printf_GetData()->Printf_Mode=4;
		printf("%c",0xD);
	  Printf_GetData()->Printf_Mode=1;
}

void Print_ChangLine_Com(void)
{
	/*����ָ��*/
	  Printf_GetData()->Printf_Mode=4;
		printf("%c",0xA);
	  Printf_GetData()->Printf_Mode=1;
}
void Print_Chk_Com(void)
{
	/*��ѯָ��*/
	 Printf_GetData()->Printf_Mode=4;
	 printf("%c",0x1B);
	 printf("%c",0x76);
	 Printf_GetData()->Printf_Mode=1;
}

void UartToBlue(char *data)
{ Printf_GetData()->Printf_Mode=4;
	printf("%s",&data[0]);   //��ӡ����
	Printf_GetData()->Printf_Mode=1;
}
//��ӡĳһ������
void PrintLine(u16 line)
{  
	switch(line)
			{
			case 1:{
				UartToBlue(&Line1data[0]);
				break;
			}
			case 2:{
				UartToBlue(&Line2data[0]);
				break;
			}
			case 3:{
				UartToBlue(&Line3data[0]);
				break;
			}
			case 4:{
				UartToBlue(&Line4data[0]);
				break;
			}
			case 5:{
				UartToBlue(&Line5data[0]);
				break;
			}
			case 6:{
				UartToBlue(&Line6data[0]);
				break;
			}
			case 7:{
				UartToBlue(&Line7data[0]);
				break;
			}
		  case 8:{
				UartToBlue(&Line8data[0]);
				break;
			}
			case 9:{
				UartToBlue(&Line9data[0]);
				break;
			}
			case 10:{
				UartToBlue(&Line10data[0]);
				break;
			}
			case 11:{
				UartToBlue(&Line11data[0]);
				break;
			}
			case 12:{
				UartToBlue(&Line12data[0]);
				break;
			}
			case 13:{
				UartToBlue(&Line13data[0]);
				break;
			}
			case 14:{
				UartToBlue(&Line14data[0]);
				break;
			}
			case 15:{
				UartToBlue(&Line15data[0]);
				break;
			}
			case 16:{
				UartToBlue(&Line16data[0]);
				break;
			}
			case 17:{
				UartToBlue(&Line17data[0]);
				break;
			}
			case 18:{
				UartToBlue(&Line18data[0]);
				break;
			}
			case 19:{
				UartToBlue(&Line19data[0]);
				break;
			}
			case 20:{
				UartToBlue(&Line20data[0]);
				break;
			}
			case 21:{
				UartToBlue(&Line21data[0]);
				break;
			}
			default:break;
		}
			
}

/*
   ��ӡ�����ļ�����ӿ�
*/
void Print_SampleFile(Ssample_file *sample)
{
	Printer_Data.RunMode = 1;   //������ӡ����
	Printer_Data.NextMode=3;   //3Ϊ������ӡ��4Ϊ˲ʱ��ӡ��5Ϊ�����ӡ
  Printer_Data.line=0;
	PrintAPI_Ssample(sample);
}

/*
   ��ӡ˲ʱ�ļ�����ӿ�
*/
void Print_ShunshiFile(Ssample_file *shunshi)
{
	Printer_Data.RunMode = 1;   //������ӡ����
	Printer_Data.NextMode=4;   //3Ϊ������ӡ��4Ϊ˲ʱ��ӡ��5Ϊ�����ӡ
  Printer_Data.line=0;
	PrintAPI_Sshunshi(shunshi);
}
/*
   ��ӡ�����ļ�����ӿ�
*/
void Print_PowerFile(sPOWER_FILE *spower)
{
	Printer_Data.RunMode = 1;   //������ӡ����
	Printer_Data.NextMode=5;   //3Ϊ������ӡ��4Ϊ˲ʱ��ӡ��5Ϊ�����ӡ
  Printer_Data.line=0;
	PrintAPI_Spower(spower);
}

/*
��ӡģ���ʼ��
*/

void Print_Init(void)
{
	Printer_Data.line=0;
}

/*
��ӡģ�鶨ʱ��
*/

void PrintTimer(void)
{
	if(Printer_Data.Timer>0)
		Printer_Data.Timer--;
}


//��ӡ����̣߳����������

void Print_Main(void){
	
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
			if(Printer_Data.Timer==0)
			{
				Printer_Data.RunMode = Printer_Data.NextMode;
			}
			break;
		}
		case 3:{
		  Printer_Data.LineMax=20;             //����Ҫ��ӡ����
			
			Printer_Data.RunMode = 6;
			break;
		}
		case 4:{
			Printer_Data.LineMax=21;            //����Ҫ��ӡ����
			
			Printer_Data.RunMode = 6;
			break;
		}
		case 5:{
			Printer_Data.LineMax=13; //����Ҫ��ӡ��С��
			
			Printer_Data.RunMode = 6;
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
			}
			else
			{
		  if(USART_COM4_GetData()->ReceveOkFlg)  //������
			  {
		     if ((USART_COM4_GetData()->USART_RX_BUF[0]&0X0F) == 0x0000) 
					 {  //��ѯ��ȷ
						PrintLine(Printer_Data.line);  //��ӡ�ڼ���
			      Print_Printorder_Com();        //��ӡ����
			      Print_ChangLine_Com();         //�س�����
		
						Printer_Data.line++;
		        if(Printer_Data.line>Printer_Data.LineMax)  //��ӡ���
						{
							Printer_Data.RunMode =0;
							Printer_Data.Flag=1;     //�ĵ���ӡ���־�����ڱ��������ж�
						}
						else
						{
							Printer_Data.RunMode =6;  //��ӡ��һ��
						}
		       }
					 else   //������ѯ
					 {
						 Print_Chk_Com();  //������ѯ״ָ̬��
			       USART_COM4_Receve_com(0,1);  //͸��ģʽҪ�󴮿ڽ���һ���ֽ�
					 }
			  }
			break;
		}
	}
 }
}

