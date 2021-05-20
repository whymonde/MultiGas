//#include "sys.h"
//#include "usart.h"
//#include "delay.h"
//#include "stdlib.h" 
//#include "string.h"
//#include "stdio.h"
#include "Printer.h"
#define linenumber 33

const char mechine_type[linenumber] = "           YLB-2700型           ";
const char mechine_name[linenumber] = "   环境空气颗粒物综合采样器     ";
const char instrument_number[linenumber] = "仪器编号：";
const char line_change[linenumber] = "  ";
const char divider[linenumber] = "--------------------------------";
const char comp_name[linenumber] = "   佛山宇隆博环保科技有限公司   ";
const char comp_web[linenumber] =  "     公司官网:www.wwwww.cn      ";
const char comp_phone[linenumber] ="     客服热线:0757-22207829     ";

const char sample_data[linenumber] = "------------采样数据------------";
const char sample_name[11][16] = { "文件号: "," 路采样数据","采样日期: ","采样时长: ","采样流量: ","累计体积: ","参比体积: ","计前温度: ","计前压力: ","环境温度: ","大 气 压: "};

char shunshi_data[linenumber] = "------------瞬时数据------------";
const char shunshi_name[12][16] = {"文件号: ","瞬时编号: "," 路采样数据","采样日期: ","采样时长: ","采样流量: ","累计体积: ","参比体积: ","计前温度: ","计前压力: ","环境温度: ","大 气 压: "};

char spower_data[linenumber] = "------------掉电记录------------";
const char spower_name[4][16] = {"文件号: ","掉电时间: ","来电时间: ","掉电时长: "};

const char danwei[4][16] = {"       L",
                            "       °C",
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


Printer_Structure *Printer_GetData(void){    //返回Printer_Data结构体地址
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
			strcpy((char *)&linedata[0],(char *)&instrument_number[0]); //仪器编号
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
			strncpy((char *)&linedata[0],(char *)&sample->sampletype[0],1);    // A 路采样数据
			linedata[1]=0;
			strcat((char *)&linedata[0],(char *)&sample_name[1][0]);
			strncpy((char *)&Line6data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strncpy((char *)&linedata[0],(char *)&sample_name[2][0],strlen((char *)&sample_name[2][0])+1); //采样日期
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
			strncpy((char *)&linedata[0],(char *)&mechine_type[0],linenumber-1);  //型号
			strncpy((char *)&Line1data[0],(char *)&linedata[0],linenumber-1);
			
		}
	  {
			strncpy((char *)&linedata[0],(char *)&mechine_name[0],linenumber-1);  //机器名称
			strncpy((char *)&Line2data[0],(char *)&linedata[0],linenumber-1);
		
		}
	  {
			strncpy((char *)&linedata[0],(char *)&shunshi_data[0],linenumber-1);   //
			strncpy((char *)&Line3data[0],(char *)&linedata[0],linenumber-1);
		
		}
		{
			strcpy((char *)&linedata[0],(char *)&instrument_number[0]); //仪器编号
			strcat((char *)&linedata[0],(char *)&Printer_Data.yiqibianhao[0]);  //读取编号
			strncpy((char *)&Line4data[0],(char *)&linedata[0],linenumber-1);
		
		}
	  {
			strncpy((char *)&linedata[0],(char *)&shunshi_name[0][0],strlen((char *)&shunshi_name[0][0])+1);  //文件号
			strncpy((char *)&data[0],(char *)&shunshi->filenumber[0],6);
			data[6]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strncpy((char *)&Line5data[0],(char *)&linedata[0],linenumber-1);
		}
		
		{
			strncpy((char *)&linedata[0],(char *)&shunshi_name[1][0],strlen((char *)&shunshi_name[1][0])+1);  //瞬时编号：
			sprintf((char*)&data[0],"%06u",Printer_Data.ShunshiCur);//当前号码
			data[6]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],"/");
			sprintf((char*)&data[0],"%06u",Printer_Data.ShunshiFileNum);//文件号
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
			strncpy((char *)&linedata[0],(char *)&mechine_type[0],linenumber-1);  //型号
			strncpy((char *)&Line1data[0],(char *)&linedata[0],linenumber-1);
		
		}
	  {
			strncpy((char *)&linedata[0],(char *)&mechine_name[0],linenumber-1);   //机器名称
			strncpy((char *)&Line2data[0],(char *)&linedata[0],linenumber-1);
		
		}
	  {
			strncpy((char *)&linedata[0],(char *)&spower_data[0],linenumber-1);   //打印内容
			strncpy((char *)&Line3data[0],(char *)&linedata[0],linenumber-1);
		
		}
	  {

			strcpy((char *)&linedata[0],(char *)&instrument_number[0]); //仪器编号
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




//打印命令组
void Print_Init_Com(void)
{
	/*打印机初始化*/
	  Printf_GetData()->Printf_Mode=4;
		printf("%c",0x1B);
		printf("%c",0x40);
	  Printf_GetData()->Printf_Mode=1;
}

void Print_Printorder_Com(void)
{
	/*打印指令*/
	  Printf_GetData()->Printf_Mode=4;
		printf("%c",0xD);
	  Printf_GetData()->Printf_Mode=1;
}

void Print_ChangLine_Com(void)
{
	/*换行指令*/
	  Printf_GetData()->Printf_Mode=4;
		printf("%c",0xA);
	  Printf_GetData()->Printf_Mode=1;
}
void Print_Chk_Com(void)
{
	/*查询指令*/
	 Printf_GetData()->Printf_Mode=4;
	 printf("%c",0x1B);
	 printf("%c",0x76);
	 Printf_GetData()->Printf_Mode=1;
}

void UartToBlue(char *data)
{ Printf_GetData()->Printf_Mode=4;
	printf("%s",&data[0]);   //打印数据
	Printf_GetData()->Printf_Mode=1;
}
//打印某一行数据
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
   打印采样文件命令接口
*/
void Print_SampleFile(Ssample_file *sample)
{
	Printer_Data.RunMode = 1;   //启动打印流程
	Printer_Data.NextMode=3;   //3为采样打印，4为瞬时打印，5为掉电打印
  Printer_Data.line=0;
	PrintAPI_Ssample(sample);
}

/*
   打印瞬时文件命令接口
*/
void Print_ShunshiFile(Ssample_file *shunshi)
{
	Printer_Data.RunMode = 1;   //启动打印流程
	Printer_Data.NextMode=4;   //3为采样打印，4为瞬时打印，5为掉电打印
  Printer_Data.line=0;
	PrintAPI_Sshunshi(shunshi);
}
/*
   打印掉电文件命令接口
*/
void Print_PowerFile(sPOWER_FILE *spower)
{
	Printer_Data.RunMode = 1;   //启动打印流程
	Printer_Data.NextMode=5;   //3为采样打印，4为瞬时打印，5为掉电打印
  Printer_Data.line=0;
	PrintAPI_Spower(spower);
}

/*
打印模块初始化
*/

void Print_Init(void)
{
	Printer_Data.line=0;
}

/*
打印模块定时器
*/

void PrintTimer(void)
{
	if(Printer_Data.Timer>0)
		Printer_Data.Timer--;
}


//打印监控线程，主程序调用

void Print_Main(void){
	
	switch(Printer_Data.RunMode)
		{
		case 0:{break;}
		case 1:{
			Print_Init_Com();         //打印机初始化指令
			Printer_Data.Timer=500;   //延时0.5s
			Printer_Data.Flag=0;      //先清空打印完成标志
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
		  Printer_Data.LineMax=20;             //记下要打印行数
			
			Printer_Data.RunMode = 6;
			break;
		}
		case 4:{
			Printer_Data.LineMax=21;            //记下要打印行数
			
			Printer_Data.RunMode = 6;
			break;
		}
		case 5:{
			Printer_Data.LineMax=13; //记下要打印多小行
			
			Printer_Data.RunMode = 6;
			break;
		}
		 //开始打印时序
		case 6:{
			Print_Chk_Com();  //发生查询状态指令
			USART_COM4_Receve_com(0,1);  //透传模式要求串口接收一个字节
			Printer_Data.Timer=3000;   //定时3秒查询状态，根据打印一行所用时间设置
			Printer_Data.RunMode = 7;
			break;
		}
		case 7:
		{
			if(Printer_Data.Timer==0)  //超时退出不打
			{
				Printer_Data.RunMode =0;
			}
			else
			{
		  if(USART_COM4_GetData()->ReceveOkFlg)  //接收完
			  {
		     if ((USART_COM4_GetData()->USART_RX_BUF[0]&0X0F) == 0x0000) 
					 {  //查询正确
						PrintLine(Printer_Data.line);  //打印第几行
			      Print_Printorder_Com();        //打印命令
			      Print_ChangLine_Com();         //回车换行
		
						Printer_Data.line++;
		        if(Printer_Data.line>Printer_Data.LineMax)  //打印完毕
						{
							Printer_Data.RunMode =0;
							Printer_Data.Flag=1;     //文档打印完标志，出口被主程序判断
						}
						else
						{
							Printer_Data.RunMode =6;  //打印下一行
						}
		       }
					 else   //继续查询
					 {
						 Print_Chk_Com();  //发生查询状态指令
			       USART_COM4_Receve_com(0,1);  //透传模式要求串口接收一个字节
					 }
			  }
			break;
		}
	}
 }
}

