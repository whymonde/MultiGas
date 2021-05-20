//#include "sys.h"
//#include "usart.h"
//#include "delay.h"
//#include "stdlib.h" 
//#include "string.h"
//#include "stdio.h"
#include "Printer.h"
#define linenumber 33
#include "CalculationFormula.h"

//单位
const char danwei[14][9] ={"m3/h    ",//0
                           "%       ",//1
                           "mg/m3   ",//2
                           "kg/h    ",//3
	                         "m2      ",//4
	                         "Pa      ",//5
	                         "kPa     ",//6
	                         "m/s     ",//7
	                         "mm      ",//8
	                         "℃      ",//9
	                         "L       ",//10
	                         "m3/h    ",//11
	                         "g       ",//12
	                         "s       ",//13
                           };

//**********************3330********************************
const char p_hand[linenumber]="       YLB-3330 采样报表        ";
const char p_gas_hand1[linenumber] ="            烟气报表            ";
const char p_dust_hand1[linenumber]="            烟尘报表            ";
const char p_Work_hand1[linenumber]="            工况报表            ";	
const char p_gasminu_hand1[linenumber] ="  YLB-3330型烟气分钟数据        ";
const char p_gasrealt_hand1[linenumber] ="  YLB-3330型实时烟气数据        ";		
													 
const char p_PressurePara_hand[linenumber]="     YLB-3330 压力参数报表      ";	
const char p_TemperaturePara_hand[linenumber]="     YLB-3330 温度参数报表      ";		
const char p_FlowPara_hand[linenumber]="     YLB-3330 流量参数报表      ";	
const char p_ConcentraPara_hand[linenumber]="     YLB-3330 浓度参数报表      ";														 
													 
//工况，烟尘，烟气打印项													 
const char p_item_name[53][10]={
  "仪器编号:",//0
  "文 件 号:",//1
  "采样时间:",//2
  "采样地点:",//3
  "样品编号:",//4
  "气 密 性:",//5
  "平均动压:",//6
  "平均静压:",//7
	"平均全压:",//8
	"平均流速:",//9	
	"平均烟温:",//10
	"大 气 压:",//11
	"烟道截面:",//12
	"含 湿 量:",//13
	"皮托管Kp:",//14
	"过剩系数:",//15
	"折算系数:",//16
	"负荷系数:",//17
	"跟 踪 率:",//18
	"采嘴直径:",//19
  "平均计压:",//20
  "平均计温:",//21
	"累计采时:",//22
	"工况体积:",//23
	"标况体积:",//24
	"烟气流量:",//25
  "标杆流量:",//26
	" O2 浓度:",//27
	"SO2 浓度:",//28
	"SO2 折算:",//29
	"SO2 排放:",//30
	" NO 浓度:",//31
	" NO 折算:",//32
	" NO 排放:",//33
	"NO2 浓度:",//34
	"NO2 折算:",//35
	"NO2 排放:",//36
	"NOx 浓度:",//37
	"NOx 折算:",//38
	"NOx 排放:",//39
	"CO  浓度:",//40
	"CO  折算:",//41
	"CO  排放:",//42
	"NOx 浓度:",//43
	"NOx 折算:",//44
	"NOx 排放:",//45
	"CO2 浓度:",//46
	"CO2 折算:",//47
	"CO2 排放:",//48
	"烟尘净重:",//49
	"烟尘浓度:",//50
	"烟尘折算:",//51
	"烟尘排放:",//52
};
//参数打印项
const char p_para_item_name[51][15]={
	"烟温/干球零点:",//0
	"烟温/干球倍率:",//1
	"仪器编号:",//2
	"时    间:",//3
	"环境温度:",//4
	"大 气 压:",//5
	"1.0L倍率:",//6
  "20 L倍率:",//7
	"30 L倍率:",//8
	"40 L倍率:",//9
	"50 L倍率:",//10
	"70 L倍率:",//11
	"90 L倍率:",//12
  "气压零点:",//13
	"计压零点:",//14
	"静压零点:",//15
	"动压零点:",//16
	"尘流压零点:",//17
	"湿球零点:",//18
	"湿球倍率:",//19
	"计温零点:",//20
	" O2     零点:",//21
	" O2 低端倍率:",//22
	" O2 中端倍率:",//23
	" O2 高端倍率:",//24
	"SO2     零点:",//25
	"SO2 低端倍率:",//26
	"SO2 中端倍率:",//27
	"SO2 高端倍率:",//28
	" NO     零点:",//29
	" NO 低端倍率:",//30
	" NO 中端倍率:",//31
	" NO 高端倍率:",//32
	"NO2     零点:",//33
	"NO2 低端倍率:",//34
	"NO2 中端倍率:",//35
	"NO2 高端倍率:",//36
	" CO     零点:",//37
	" CO 低端倍率:",//38
	" CO 中端倍率:",//39
	" CO 高端倍率:",//40
	"H2S     零点:",//41
	"H2S 低端倍率:",//42
	"H2S 中端倍率:",//43
	"H2S 高端倍率:",//44
	"CO2     零点:",//45
	"CO2 低端倍率:",//46
	"CO2 中端倍率:",//47
	"CO2 高端倍率:",//48
  "气流压零点:",//49
	"100L倍率:",//50
};



char LineData[55][linenumber]={0};  //定义50行,从LineData[1] 开始计，好记

Printer_Structure Printer_Data;
/****************************************************************************
	函数名：Printer_Structure *Printer_GetData(void)
	输  入: &Printer_Data
         
	输  出:
	功能说明：返回类Printer_Data 类指针

*/

Printer_Structure *Printer_GetData(void){    //返回Printer_Data结构体地址
	return &Printer_Data;
}
/****************************************************************************
	函数名：PrintAPI_WorkFile()
  输  入: strWorkFile *P :由读SD卡数据后返回的数据缓冲区指针
         
	输  出:
	功能说明：/打印工况数据转换到打印缓冲 

*/
void PrintAPI_WorkFile(Printer_Structure *P,strWorkFile *Psour)  ///
{
  char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //计打印行数
	u8 i;
	// 型号+打印类别
	strncpy((char *)&LineData[linecnt][0],(char *)&p_hand[0],linenumber-1);  //
	linecnt++;	
  strncpy((char *)&LineData[linecnt][0],(char *)&p_Work_hand1[0],linenumber-1);  //
	linecnt++;		
	
	//仪器编号
	strcpy((char *)&linedata[0],(char *)&p_item_name[0][0]);  //"仪器编号:"9
	strcat((char *)&linedata[0],(char *)"      ");  //补空6
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8字节编号=23
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//文件号
	strcpy((char *)&linedata[0],(char *)&p_item_name[1][0]);  //"文 件 号:"9字节
	strcat((char *)&linedata[0],(char *)"        ");  //补空8字节
	SPutl(6,(u8 *)&data[0],P->print_cnt_file);  //当前的文件号就是
	data[6]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
  strcat((char *)&linedata[0],(char *)"         "); //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	
	//采样时间
	strcpy((char *)&linedata[0],(char *)&p_item_name[2][0]);  //"采样时间"9字节
	strncpy((char *)&data[0],(char *)&Psour->dat[0],16);      //在缓冲区取出时间  
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);  //补空
	strcat((char *)&linedata[0],(char *)"       ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
  //采样地点
	strncpy((char *)&linedata[0],(char *)&Psour->Adress[0],32);      //在缓冲区取出时间 
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
	
	strncpy((char *)&linedata[0],(char *)&Psour->Adress[32],26);      //在缓冲区取出时间 
	linedata[26]=0;
	strcat((char *)&linedata[0],(char *)"    ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;

	//平均动压
	strncpy((char *)&linedata[0],(char *)&Psour->Other[0],26);      //在缓冲区取出时间 
	linedata[26]=0;
	strcat((char *)&linedata[0],(char *)"    ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
	//平均静压
	strncpy((char *)&linedata[0],(char *)&Psour->Other[1],26);      //在缓冲区取出时间 
	linedata[26]=0;
	strcat((char *)&linedata[0],(char *)"    ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
	//大 气 压
	//烟道截面
	//采嘴直径
	//平均烟温
	//平均流速
	//烟气流量
	//标杆流量
	for(i=0;i<=6;i++)
	{
	strncpy((char *)&linedata[0],(char *)&Psour->Other[i+3],27);      //修正烟气流量，标杆流量单位显示不全，modify by alfred, 2020.07.06
	linedata[27]=0;
	strcat((char *)&linedata[0],(char *)"    ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
	}
	
	//网站
	 strcpy((char *)&linedata[0],(char *)"-----");  //"网站"
	 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21字节
	 strcat((char *)&linedata[0],(char *)"------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	//电话
	 strcpy((char *)&linedata[0],(char *)"---------");  //"电话"
	 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13字节
	 strcat((char *)&linedata[0],(char *)"----------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	
	
	 P->LineMax=linecnt;  //记下最多行数
 
}
/****************************************************************************
20200522
函数名：PrintAPI_GasFile()
  输  入: strWorkFile *P :由读SD卡数据后返回的数据缓冲区指针
         
	输  出:
	功能说明：/打印烟气数据转换到打印缓冲 

*/
void PrintAPI_GasFile(Printer_Structure *P,strWorkFile *Psour)  //打印烟尘数据转换到打印缓冲
{
	char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //计打印行数
	float fdbuf,fdbuf1; //
	float	ZSXS; //折算系数
	float BGLL;  //标杆流量
	float QTLD;  //气体浓度
	float GSXS;   //过剩系数
	float FHXS;   //出力系数，负荷系数
	float NO,NO2,NOX; //读取NO,NO2的数值
	u32 CYSC; //采样时长

    u8 i;
	strncpy((char *)&data[0],(char *)&Psour->Other[9][9],13);   //折算系数字符串
	data[13]=0;
	ZSXS=Calculate_CharToFloat((u8 *)&data[0],13);              //把字符串转换成浮点数
	
	strncpy((char *)&data[0],(char *)&Psour->Other[12][9],13);  //标杆流量字符串
	data[13]=0;
	BGLL=Calculate_CharToFloat((u8 *)&data[0],13);              //把字符串转换成浮点数

	
	strncpy((char *)&data[0],(char *)&Psour->Other[23][9],13);  //计算过剩系数
	data[13]=0;
	GSXS=Calculate_CharToFloat((u8 *)&data[0],13);               //把字符串转换成浮点数
 
	strncpy((char *)&data[0],(char *)&Psour->Other[10][9],13);  //负荷系数
	data[13]=0;
	FHXS=Calculate_CharToFloat((u8 *)&data[0],13);               //把字符串转换成浮点数
	
	strncpy((char *)&data[0],(char *)&Psour->Other[15][9],13);  //NO
	data[13]=0;
	NO=Calculate_CharToFloat((u8 *)&data[0],13);               //把字符串转换成浮点数
	
	strncpy((char *)&data[0],(char *)&Psour->Other[16][9],13);  //NO2
	data[13]=0;
	NO2=Calculate_CharToFloat((u8 *)&data[0],13);               //把字符串转换成浮点数
	
	strncpy((char *)&data[0],(char *)&Psour->Other[21][9],13);  //采样时长
	data[13]=0;
	CYSC=(u32)Calculate_CharToFloat((u8 *)&data[0],13);               //把字符串转换成浮点数
	
	
	
	
	if(NO==1000000)
		NO=0;
	if(NO2==1000000)
		NO2=0;
    NOX=CalculationFormula_NOX(NO,NO2);
	{
		// 型号+打印类别
		
		strncpy((char *)&LineData[linecnt][0],(char *)&p_hand[0],linenumber-1);  //
		linecnt++;	
		strncpy((char *)&LineData[linecnt][0],(char *)&p_gas_hand1[0],linenumber-1);  //
		linecnt++;		
		
		//仪器编号
		strcpy((char *)&linedata[0],(char *)&p_item_name[0][0]);  //"仪器编号:"9
		strcat((char *)&linedata[0],(char *)"      ");  //补空6
		strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8字节编号
		data[8]=0;
		strcat((char *)&linedata[0],(char *)&data[0]);
		strcat((char *)&linedata[0],(char *)"         "); //补空
		strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		linecnt++;	
		
		//文件号
		strcpy((char *)&linedata[0],(char *)&p_item_name[1][0]);  //"文 件 号:"9字节
		strcat((char *)&linedata[0],(char *)"        ");  //补空8字节
		SPutl(6,(u8 *)&data[0],P->print_cnt_file);  //当前的文件号就是
		data[6]=0;
		strcat((char *)&linedata[0],(char *)&data[0]);
		strcat((char *)&linedata[0],(char *)"         "); //补空
		strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		linecnt++;	
		//采样时间
		strcpy((char *)&linedata[0],(char *)&p_item_name[2][0]);  //"采样时间"9字节
		strncpy((char *)&data[0],(char *)&Psour->dat[0],16);      //在缓冲区取出时间  
		data[16]=0;
		strcat((char *)&linedata[0],(char *)&data[0]);  //补空
		strcat((char *)&linedata[0],(char *)"       ");  //补空
		strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		linecnt++;
		
		//采样时长
		
		strcpy((char *)&linedata[0],(char*)"采样时长:");      //取出采样时长
		
		
		sprintf((char *)&data[0],"   %02d:%02d:%02d        ",CYSC/3600,(CYSC%3600)/60,(CYSC%3600)%60);
		//strncpy((char *)&data[0],(char *)&Psour->Other[21][9],19);//19
		data[19]=0;

		strcat((char *)&linedata[0],(char *)&data[0]);
		strcat((char *)&linedata[0],(char *)"    ");  //补空
		strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		linecnt++;
		
		
		
		//采样地点
		strncpy((char *)&linedata[0],(char *)&Psour->Adress[0],32);      //在缓冲区取出时间 
		strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		linecnt++;
		
		strncpy((char *)&linedata[0],(char *)&Psour->Adress[32],28);      //在缓冲区取出时间 
		linedata[28]=0;
		strcat((char *)&linedata[0],(char *)"    ");  //补空
		strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		linecnt++;
	}
	{	
		if(Psour->Other[20][15]=='9')  //有O2数据
		{//O2浓度,含氧量
			if(P->pselect[0]==1)  //选择打印了
			{ 
				strcpy((char *)&linedata[0],(char*)" O2 浓度:");      //在缓冲区取出时间 9
				strncpy((char *)&data[0],(char *)&Psour->Other[13][9],19);//19
				data[19]=0;

				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)"    ");  //补空
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
		}
		if(Psour->Other[20][16]=='9')  //有SO2数据
		{
			strncpy((char *)&data[0],(char *)&Psour->Other[14][9],13);
			data[13]=0;
			//把字符串转换成浮点数
			fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
			QTLD=fdbuf;
			//SO2浓度
			if(P->pselect[1]==1)  //选择打印了
			{ 
				strcpy((char *)&linedata[0],(char*)"SO2 浓度:");      //在缓冲区取出时间 9
				strncpy((char *)&data[0],(char *)&Psour->Other[14][9],19);//19
				data[19]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)"    ");  //补空4
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//SO2折算
			if(P->pselect[9]==1)  //选择打印了
			{
				 
                fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //求得折算后结果
				strcpy((char *)&linedata[0],(char*)"SO2 折算:");      //在缓冲区取出时间 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf1); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
				
				if(GSXS==1000000)
			      { for(i=12;i<=21;i++)
				        {
					     linedata[i]='-';
				        }
			      }
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;				
			}
			//SO2排放
			if(P->pselect[17]==1)  //选择打印了
			{
				fdbuf=QTLD;         //气体浓度
				fdbuf=QTLD*BGLL/1000000;   //排放量的计算

				strcpy((char *)&linedata[0],(char*)"SO2 排放:");      //在缓冲区取出时间 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" kg/h     ");  //补空10
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}				
		}
		if(Psour->Other[20][17]=='9')  //有NO数据
		{
			strncpy((char *)&data[0],(char *)&Psour->Other[15][9],13);
			data[13]=0;
			//把字符串转换成浮点数
			fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
			QTLD=fdbuf;
		//NO浓度
		if(P->pselect[3]==1)  //选择打印了
		{ 
			strcpy((char *)&linedata[0],(char*)" NO 浓度:");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&Psour->Other[15][9],19);
			data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],(char *)"    ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//NO折算
		if(P->pselect[11]==1)  //选择打印了
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //求得折算后结果

			strcpy((char *)&linedata[0],(char*)" NO 折算:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %-10.1f",fdbuf1); //13
			data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
				if(GSXS==1000000)
			      { for(i=12;i<=21;i++)
				        {
					     linedata[i]='-';
				        }
			      }
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//NO排放
		if(P->pselect[19]==1)  //选择打印了
		{
			fdbuf=QTLD;         //气体浓度
			fdbuf=QTLD*BGLL/1000000;   //排放量的计算

			strcpy((char *)&linedata[0],(char*)" NO 排放:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
			data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
			strcat((char *)&linedata[0],(char *)" kg/h     ");  //补空10
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		}
		if(Psour->Other[20][18]=='9')  //有NO2,NOx数据
		{
			strncpy((char *)&data[0],(char *)&Psour->Other[16][9],13);
			data[13]=0;
			//把字符串转换成浮点数
			fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
			QTLD=fdbuf;
			//NO2浓度
			if(P->pselect[4]==1)  //选择打印了
			{ 
				strcpy((char *)&linedata[0],(char*)"NO2 浓度:");      //在缓冲区取出时间 
				strncpy((char *)&data[0],(char *)&Psour->Other[16][9],19);
				data[19]=0;
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)"    ");  //补空
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//NO2折算
			if(P->pselect[12]==1)  //选择打印了
			{
				fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //求得折算后结果
				strcpy((char *)&linedata[0],(char*)"NO2 折算:");      //在缓冲区取出时间 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf1); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
				if(GSXS==1000000)
			      { for(i=12;i<=21;i++)
				        {
					     linedata[i]='-';
				        }
			      }
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//NO2排放
			if(P->pselect[20]==1)  //选择打印了
			{
				fdbuf=QTLD;         //气体浓度
				fdbuf=QTLD*BGLL/1000000;   //排放量的计算

				strcpy((char *)&linedata[0],(char*)"NO2 排放:");      //在缓冲区取出时间 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" kg/h     ");  //补空10
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//NOx浓度

			fdbuf=NOX;
			QTLD=fdbuf;
			if(P->pselect[2]==1)  //选择打印了
			{
				strcpy((char *)&linedata[0],(char*)"NOx 浓度:");      //在缓冲区取出时间 
				sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//NOx折算
			if(P->pselect[10]==1)  //选择打印了
			{
				fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //求得折算后结果
				strcpy((char *)&linedata[0],(char*)"NOx 折算:");      //在缓冲区取出时间 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf1); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
				if(GSXS==1000000)
			      { for(i=12;i<=21;i++)
				        {
					     linedata[i]='-';
				        }
			      }
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++; //修正NOX折算不能打印, modify by alfred, 2020.07.07
			}
			//NOx排放
			if(P->pselect[18]==1)  //选择打印了
			{
				fdbuf=QTLD;         //气体浓度
				fdbuf=QTLD*BGLL/1000000;   //排放量的计算

				strcpy((char *)&linedata[0],(char*)"NOx 排放:");      //在缓冲区取出时间 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" kg/h     ");  //补空10
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
		}
		if(Psour->Other[20][19]=='9')  //有CO数据
		{
			strncpy((char *)&data[0],(char *)&Psour->Other[17][9],13);
			data[13]=0;
			//把字符串转换成浮点数
			fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
			QTLD=fdbuf;
			//CO浓度
			if(P->pselect[5]==1)  //选择打印了
			{ 
				strcpy((char *)&linedata[0],(char*)" CO 浓度:");      //在缓冲区取出时间 
				strncpy((char *)&data[0],(char *)&Psour->Other[17][9],19);
				data[19]=0;
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)"    ");  //补空
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//CO折算
			if(P->pselect[13]==1)  //选择打印了
			{
				fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //求得折算后结果
				strcpy((char *)&linedata[0],(char*)" CO 折算:");      //在缓冲区取出时间 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf1); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
				if(GSXS==1000000)
			      { for(i=12;i<=21;i++)
				        {
					     linedata[i]='-';
				        }
			      }
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//CO排放
			if(P->pselect[21]==1)  //选择打印了
			{
				fdbuf=QTLD;         //气体浓度
				fdbuf=QTLD*BGLL/1000000;   //排放量的计算

				strcpy((char *)&linedata[0],(char*)" CO 排放:");      //在缓冲区取出时间 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" kg/h     ");  //补空10
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
		}
		if(Psour->Other[20][20]=='9')  //有H2S数据
		{
			strncpy((char *)&data[0],(char *)&Psour->Other[18][9],13);
			data[13]=0;
			//把字符串转换成浮点数
			fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
			QTLD=fdbuf; 
			//H2S浓度
			if(P->pselect[6]==1)  //选择打印了
			{ 
				strcpy((char *)&linedata[0],(char*)"H2S 浓度:");      //在缓冲区取出时间 
				strncpy((char *)&data[0],(char *)&Psour->Other[18][9],19);
				data[19]=0;
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)"    ");  //补空
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//H2S折算
			if(P->pselect[14]==1)  //选择打印了
			{
				fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //求得折算后结果
				strcpy((char *)&linedata[0],(char*)"H2S 折算:");      //在缓冲区取出时间 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf1); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
				if(GSXS==1000000)
			      { for(i=12;i<=21;i++)
				        {
					     linedata[i]='-';
				        }
			      }
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//H2S排放
			if(P->pselect[22]==1)  //选择打印了
			{
				fdbuf=QTLD;         //气体浓度
				fdbuf=QTLD*BGLL/1000000;   //排放量的计算

				strcpy((char *)&linedata[0],(char*)"H2S 排放:");      //在缓冲区取出时间 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" kg/h     ");  //补空10
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
		}
		if(Psour->Other[20][21]=='9')  //有CO2数据
		{
			strncpy((char *)&data[0],(char *)&Psour->Other[19][9],13);
			data[13]=0;
			//把字符串转换成浮点数
			fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
			QTLD=fdbuf; 
			//CO2浓度
			if(P->pselect[7]==1)  //选择打印了
			{ 
				strcpy((char *)&linedata[0],(char*)"CO2 浓度:");      //在缓冲区取出时间 
				strncpy((char *)&data[0],(char *)&Psour->Other[19][9],19);
				data[19]=0;
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)"    ");  //补空
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//CO2折算
			if(P->pselect[15]==1)  //选择打印了
			{
				fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //求得折算后结果
				strcpy((char *)&linedata[0],(char*)"CO2 折算:");      //在缓冲区取出时间 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf1); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
				if(GSXS==1000000)
			      { for(i=12;i<=21;i++)
				        {
					     linedata[i]='-';
				        }
			      }
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
			//CO2排放
			if(P->pselect[23]==1)  //选择打印了
			{
				fdbuf=QTLD;         //气体浓度
				fdbuf=QTLD*BGLL/1000000;   //排放量的计算

				strcpy((char *)&linedata[0],(char*)"CO2 排放:");      //在缓冲区取出时间 9
				sprintf((char *)&data[0],"   %-10.1f",fdbuf); //13
				data[13]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)" kg/h     ");  //补空10
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
			}
		}
		//"平均动压://0
		if(P->pselect[24]==1)  //选择打印了
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[0][0],28);      //在缓冲区取出时间 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"平均静压://1
		if(P->pselect[25]==1)  //选择打印了
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[1][0],28);      //在缓冲区取出时间 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}		
		//"平均流速://3
		if(P->pselect[26]==1)  //选择打印了
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[3][0],28);      //在缓冲区取出时间 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"平均烟温://4
		if(P->pselect[27]==1)  //选择打印了
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[4][0],28);      //在缓冲区取出时间 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"烟道截面://6
		if(P->pselect[16]==1)  //选择打印了
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[6][0],28);      //在缓冲区取出时间 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"含 湿 量://7
		if(P->pselect[28]==1)  //选择打印了
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[7][0],28);      //在缓冲区取出时间 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//过剩系数
		if(P->pselect[8]==1)  //选择打印了
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[23][0],28);      //在缓冲区取出时间 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //补空
			
			if(GSXS==1000000)
			{ for(i=12;i<=21;i++)
				{
					linedata[i]='-';
				}
			}

			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"折算系数://9
		if(P->pselect[8]==1)  //选择打印了
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[9][0],28);      //在缓冲区取出时间 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"负荷系数://10
		if(P->pselect[31]==1)  //选择打印了
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[10][0],28);      //在缓冲区取出时间 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"烟气流量://11
		if(P->pselect[29]==1)  //选择打印了
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[11][0],28);      //在缓冲区取出时间 
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
		//"标杆流量://12
		if(P->pselect[30]==1)  //选择打印了
		{
			strncpy((char *)&linedata[0],(char *)&Psour->Other[12][0],28);      //在缓冲区取出时间  
			linedata[28]=0;
			strcat((char *)&linedata[0],(char *)"    ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
	}

	//网站
	strcpy((char *)&linedata[0],(char *)"-----");  //"网站"
	strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21字节
	strcat((char *)&linedata[0],(char *)"------");
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//电话
	strcpy((char *)&linedata[0],(char *)"---------");  //"电话"
	strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13字节
	strcat((char *)&linedata[0],(char *)"----------");
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	


	P->LineMax=linecnt;  //记下最多行数

}
/****************************************************************************
函数名：PrintAPI_GasRecordFile()
输  入: strWorkFile *P :由读SD卡数据后返回的数据缓冲区指针
 
输  出:
功能说明：/打印烟气数据转换到打印缓冲 

*/
void PrintAPI_GasRecordFile(Printer_Structure *P,strWorkFile *Psour)//打印烟气记录数据转换到打印缓冲
{
	char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //计打印行数
	float NO,NO2,NOX; //读取NO,NO2的数值

	strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[3][0],13);  //NO
	data[13]=0;
	NO=Calculate_CharToFloat((u8 *)&data[0],13);               //把字符串转换成浮点数
	
	strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[4][0],13);  //NO2
	data[13]=0;
	NO2=Calculate_CharToFloat((u8 *)&data[0],13);               //把字符串转换成浮点数
	
	if(NO==1000000)
		NO=0;
	if(NO2==1000000)
		NO2=0;
    NOX=CalculationFormula_NOX(NO,NO2);
	

	// 型号+打印类别
	strncpy((char *)&LineData[linecnt][0],(char *)&p_gasminu_hand1[0],linenumber-1);  //头
	linecnt++;	

	strncpy((char *)&LineData[linecnt][0],(char *)"                                ",linenumber-1);  //空行
	linecnt++;		

	//仪器编号
	strcpy((char *)&linedata[0],(char *)&p_item_name[0][0]);  //"仪器编号:"
	strcat((char *)&linedata[0],(char *)"      ");  //补空
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8字节编号
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//测量时间
	strcpy((char *)&linedata[0],(char *)"开始时间:");  //"采样时间"9字节
	strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[0][0],16);      //在缓冲区取出时间  
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);  //补空
	strcat((char *)&linedata[0],(char *)"        ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;
	
	//采样时长（单位分钟）
	strcpy((char *)&linedata[0],(char *)"采样时长:");  //"采样时间"9字节
	strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[0][16],2);      //在缓冲区取出时间  
	data[2]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);  //补空
	strcat((char *)&linedata[0],(char *)"分钟                  ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	
	//采样地点
	strncpy((char *)&linedata[0],(char *)&Psour->Adress[0],32);      //在缓冲区取出时间 
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;

	strncpy((char *)&linedata[0],(char *)&Psour->Adress[32],28);      //在缓冲区取出时间 
	linedata[28]=0;
	strcat((char *)&linedata[0],(char *)"    ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;

	if(Psour->Other[20][15]=='9')  //有O2数据
	{//O2浓度,含氧量
	//	if(P->pselect[0]==1)  //选择打印了
		{
			strcpy((char *)&linedata[0],(char*)" O2 浓度:");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[1][0],18);      //在缓冲区取出时间  
			data[18]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //补空
			strcat((char *)&linedata[0],(char *)"      ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
	}
	if(Psour->Other[20][16]=='9')  //有SO2数据
	{//SO2浓度,
	//		if(P->pselect[4]==1)  //选择打印了
		{
			strcpy((char *)&linedata[0],(char*)"SO2 浓度:");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[2][0],18);      //在缓冲区取出时间  
			data[18]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //补空
			strcat((char *)&linedata[0],(char *)"      ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
	}
	if(Psour->Other[20][17]=='9')  //有NO数据
	{//NO浓度
	//	if(P->pselect[12]==1)  //选择打印了
		{
			strcpy((char *)&linedata[0],(char*)" NO 浓度:");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[3][0],18);      //在缓冲区取出时间  
			data[18]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //补空
			strcat((char *)&linedata[0],(char *)"      ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
	}
	if(Psour->Other[20][18]=='9')  //有NO2数据
	{
	 //NO2浓度,
	//	if(P->pselect[16]==1)  //选择打印了
		{
			strcpy((char *)&linedata[0],(char*)"NO2 浓度:");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[4][0],18);      //在缓冲区取出时间  
			data[18]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //补空
			strcat((char *)&linedata[0],(char *)"      ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;			
		}
	 //NOx浓度,
	//	if(P->pselect[8]==1)  //选择打印了
		{
				strcpy((char *)&linedata[0],(char*)"NOx 浓度:");      //在缓冲区取出时间 9
				sprintf((char *)&data[0],"%10.1f",NOX); //10
				data[10]=0;  
				strcat((char *)&linedata[0],(char *)&data[0]);
				strcat((char *)&linedata[0],(char *)"mg/m3        ");  //补空
				strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
				linecnt++;
		}
	}
	if(Psour->Other[20][19]=='9')  //有C0数据
	{//CO浓度,
	//		if(P->pselect[20]==1)  //选择打印了
		{
			strcpy((char *)&linedata[0],(char*)" CO 浓度:");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[5][0],18);      //在缓冲区取出时间  
			data[18]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //补空
			strcat((char *)&linedata[0],(char *)"      ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;			
		}
	}
	if(Psour->Other[20][20]=='9')  //有H2S数据
	{//H2S浓度,
	//	if(P->pselect[24]==1)  //选择打印了
	{
			strcpy((char *)&linedata[0],(char*)"H2S 浓度:");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[6][0],18);      //在缓冲区取出时间  
			data[18]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //补空
			strcat((char *)&linedata[0],(char *)"      ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;			
		}
	}

	if(Psour->Other[20][21]=='9')  //有CO2数据
	{//CO2浓度,含氧量
	//	if(P->pselect[28]==1)  //选择打印了
		{
			strcpy((char *)&linedata[0],(char*)"CO2 浓度:");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&Psour->G_record.stru.rowdata[7][0],18);      //在缓冲区取出时间  
			data[18]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //补空
			strcat((char *)&linedata[0],(char *)"      ");  //补空
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;			
		}
	 }
	//网站
	 strcpy((char *)&linedata[0],(char *)"-----");  //"网站"
	 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21字节
	 strcat((char *)&linedata[0],(char *)"------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	//电话
	 strcpy((char *)&linedata[0],(char *)"---------");  //"电话"
	 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13字节
	 strcat((char *)&linedata[0],(char *)"----------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	
	
	 P->LineMax=linecnt;  //记下最多行数 

}
/****************************************************************************
20200522
函数名：PrintAPI_DustFile()
  输  入: strWorkFile *P :由读SD卡数据后返回的数据缓冲区指针
         
	输  出:
	功能说明：/打印烟尘数据转换到打印缓冲 

*/
void PrintAPI_DustFile(Printer_Structure *P,strWorkFile *Psour)   //打印烟尘数据转换到打印缓冲
{
	char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //计打印行数
	float fdbuf,fdbuf1; //
	float	ZSXS; //折算系数
	float BGLL;  //标杆流量
	float QTLD;  //气体浓度
	float GSXS;   //过剩系数
	float FHXS;   //出力系数，负荷系数
	float NO,NO2,NOX;  //先读出
    u8 i;
	strncpy((char *)&data[0],(char *)&Psour->Other[9][9],13);   //折算系数字符串
	data[13]=0;
	ZSXS=Calculate_CharToFloat((u8 *)&data[0],13);              //把字符串转换成浮点数
	
	strncpy((char *)&data[0],(char *)&Psour->Other[12][9],13);  //标杆流量字符串
	data[13]=0;
	BGLL=Calculate_CharToFloat((u8 *)&data[0],13);              //把字符串转换成浮点数

	strncpy((char *)&data[0],(char *)&Psour->Other[38][9],13);  //计算过剩系数
	data[13]=0;
	GSXS=Calculate_CharToFloat((u8 *)&data[0],13);               //把字符串转换成浮点数
 
	strncpy((char *)&data[0],(char *)&Psour->Other[10][9],13);  //负荷系数
	data[13]=0;
	FHXS=Calculate_CharToFloat((u8 *)&data[0],13);               //把字符串转换成浮点数
	
	strncpy((char *)&data[0],(char *)&Psour->Other[22][9],13);  //NO
	data[13]=0;
	NO=Calculate_CharToFloat((u8 *)&data[0],13);               //把字符串转换成浮点数
	
	strncpy((char *)&data[0],(char *)&Psour->Other[23][9],13);  //NO2
	data[13]=0;
	NO2=Calculate_CharToFloat((u8 *)&data[0],13);               //把字符串转换成浮点数

	if(NO==1000000)
		NO=0;
	if(NO2==1000000)
		NO2=0;
	NOX=CalculationFormula_NOX(NO,NO2);

	{
	// 型号+打印类别
	strncpy((char *)&LineData[linecnt][0],(char *)&p_hand[0],linenumber-1);  //
	linecnt++;	
  strncpy((char *)&LineData[linecnt][0],(char *)&p_dust_hand1[0],linenumber-1);  //
	linecnt++;		
	
	//仪器编号
	strcpy((char *)&linedata[0],(char *)&p_item_name[0][0]);  //"仪器编号:"
	strcat((char *)&linedata[0],(char *)"      ");  //补空
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8字节编号
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	
	//文件号
	strcpy((char *)&linedata[0],(char *)&p_item_name[1][0]);  //"文 件 号:"9字节
	strcat((char *)&linedata[0],(char *)"        ");  //补空8字节
	SPutl(6,(u8 *)&data[0],P->print_cnt_file);  //当前的文件号就是
	data[6]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
    strcat((char *)&linedata[0],(char *)"         "); //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//采样时间
	strcpy((char *)&linedata[0],(char *)&p_item_name[2][0]);  //"采样时间"9字节
	strncpy((char *)&data[0],(char *)&Psour->dat[0],16);      //在缓冲区取出时间  
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);  //补空
	strcat((char *)&linedata[0],(char *)"       ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
  //采样地点
	strncpy((char *)&linedata[0],(char *)&Psour->Adress[0],32);      //在缓冲区取出时间 
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
	
	strncpy((char *)&linedata[0],(char *)&Psour->Adress[32],28);      //在缓冲区取出时间 
	linedata[28]=0;
	strcat((char *)&linedata[0],(char *)"    ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
  linecnt++;
}
  {
		//样品编号
		strncpy((char *)&linedata[0],(char *)&Psour->Other[28],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		//气密性
		if(P->pselect[1]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[30],28);      //在缓冲区取出时间 
		if(linedata[12]=='2')   //良好
		{
			strcpy((char *)&linedata[0],"气 密 性:   通过            ");
		}
		else if(linedata[12]=='1') //漏气
		{
			strcpy((char *)&linedata[0],"气 密 性:   不通过          ");
		}
		else                     //未检测
		{
			strcpy((char *)&linedata[0],"气 密 性:   未检测          ");
		}
			
			
	    linedata[28]=0;
			
	    strcat((char *)&linedata[0],(char *)"    ");  //补空
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
        linecnt++;
		}
		//平均动压
		if(P->pselect[10]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[0][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		
		//平均静压
		if(P->pselect[11]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[1][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//平均全压
		if(P->pselect[12]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[2][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//平均流速
		if(P->pselect[13]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[3][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//平均烟温
		if(P->pselect[14]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[4][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//大 气 压
		if(P->pselect[8]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[5][0],28);      //在缓冲区取出时间 
	   linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}		
		//烟道截面
		if(P->pselect[9]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[6][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//含 湿量
		if(P->pselect[15]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[7][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//皮托管Kp
		if(P->pselect[3]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[8][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//过剩系数
		if(P->pselect[0]==1)  //选择打印了
		{
    strncpy((char *)&linedata[0],(char *)&Psour->Other[38][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
      if(GSXS==1000000)
		{ for(i=12;i<=21;i++)
			{
			linedata[i]='-';
			}
		}
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		
		//折算系数
		if(P->pselect[0]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[9][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//负荷系数
		if(P->pselect[16]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[10][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//跟踪率
		if(P->pselect[2]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[13][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//采嘴直径
		if(P->pselect[4]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[14][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//平均计压
		if(P->pselect[6]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[15][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//平均计温
		if(P->pselect[5]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[16][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//累计采时
		if(P->pselect[7]==1)  //选择打印了
		{
	  strncpy((char *)&linedata[0],(char *)&Psour->Other[29][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
		//工况体积
		if(P->pselect[17]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[17][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
		//标况体积
		if(P->pselect[18]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[18][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//烟气流量
		if(P->pselect[23]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[11][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
		//标杆流量
		if(P->pselect[24]==1)  //选择打印了
		{
		strncpy((char *)&linedata[0],(char *)&Psour->Other[12][0],28);      //在缓冲区取出时间 
	  linedata[28]=0;
	  strcat((char *)&linedata[0],(char *)"    ");  //补空
	  strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
    linecnt++;
		}
	
	 if(Psour->Other[27][15]=='9')  //有O2数据
	 {//O2浓度,含氧量
	   if(P->pselect[25]==1)  //选择打印了
		{ 
			strcpy((char *)&linedata[0],(char*)" O2 浓度:");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&Psour->Other[20][9],19);
	    data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)"    ");  //补空
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
			}
	 }
	 if(Psour->Other[27][16]=='9')  //有SO2数据
	 {
	  	strncpy((char *)&data[0],(char *)&Psour->Other[21][9],13);
		  data[13]=0;
		  //把字符串转换成浮点数
		  fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
		  QTLD=fdbuf;
	//SO2浓度
		 
		 if(P->pselect[26]==1)  //选择打印了
		{ 
			strcpy((char *)&linedata[0],(char*)"SO2 浓度:");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&Psour->Other[21][9],19);
	    data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)"    ");  //补空
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
			}
	//SO2折算
		if(P->pselect[26]==1)  //选择打印了
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //求得折算后结果
			
			strcpy((char *)&linedata[0],(char*)"SO2 折算:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf1); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
			if(GSXS==1000000)
			  { for(i=12;i<=21;i++)
				{
				linedata[i]='-';
				}
			  }
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
	//SO2排放
		if(P->pselect[26]==1)  //选择打印了
		{
			fdbuf=QTLD;         //气体浓度
			fdbuf=QTLD*BGLL/1000000;   //排放量的计算

			strcpy((char *)&linedata[0],(char*)"SO2 排放:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	    data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)" kg/h     ");  //补空10
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}		
		
	  }
	 if(Psour->Other[27][17]=='9')  //有NO数据
	 {
		 	strncpy((char *)&data[0],(char *)&Psour->Other[22][9],13);
		  data[13]=0;
		  //把字符串转换成浮点数
		  fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
		  QTLD=fdbuf;
	//NO浓度
		if(P->pselect[28]==1)  //选择打印了
		{ 
			strcpy((char *)&linedata[0],(char*)" NO 浓度:");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&Psour->Other[22][9],19);
	    data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)"    ");  //补空
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
			}
	//NO折算
		if(P->pselect[28]==1)  //选择打印了
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //求得折算后结果
			
			strcpy((char *)&linedata[0],(char*)" NO 折算:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf1); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
			if(GSXS==1000000)
			  { for(i=12;i<=21;i++)
				{
				linedata[i]='-';
				}
			  }
	       strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
           linecnt++;
		}
	//NO排放
		if(P->pselect[28]==1)  //选择打印了
		{
			fdbuf=QTLD;         //气体浓度
			fdbuf=QTLD*BGLL/1000000;   //排放量的计算

			strcpy((char *)&linedata[0],(char*)" NO 排放:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	    data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)" kg/h     ");  //补空10
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
	 }
	if(Psour->Other[27][18]=='9')  //有NO2,NOx数据
	 {
		 strncpy((char *)&data[0],(char *)&Psour->Other[23][9],13);
		  data[13]=0;
		  //把字符串转换成浮点数
		  fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
		  QTLD=fdbuf;
	//NO2浓度
		if(P->pselect[29]==1)  //选择打印了
		{ 
			strcpy((char *)&linedata[0],(char*)"NO2 浓度:");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&Psour->Other[23][9],19);
	    data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)"    ");  //补空
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
			}
	//NO2折算
		if(P->pselect[29]==1)  //选择打印了
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //求得折算后结果
			
			strcpy((char *)&linedata[0],(char*)"NO2 折算:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf1); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
			if(GSXS==1000000)
			  { for(i=12;i<=21;i++)
				{
				linedata[i]='-';
				}
			  }			
	        strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
            linecnt++;
		}
	//NO2排放
		if(P->pselect[29]==1)  //选择打印了
		{
			fdbuf=QTLD;         //气体浓度
			fdbuf=QTLD*BGLL/1000000;   //排放量的计算

			strcpy((char *)&linedata[0],(char*)"NO2 排放:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" kg/h     ");  //补空10
	        strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
            linecnt++;
		}
	//NOx浓度
		  fdbuf=NOX;
		  QTLD=fdbuf;
		
		if(P->pselect[27]==1)  //选择打印了
		{
			strcpy((char *)&linedata[0],(char*)"NOX 浓度:");      //在缓冲区取出时间 
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
	        strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
            linecnt++;
		}
	//NOx折算
		if(P->pselect[27]==1)  //选择打印了
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //求得折算后结果
			strcpy((char *)&linedata[0],(char*)"NOX 折算:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf1); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
			if(GSXS==1000000)
			  { for(i=12;i<=21;i++)
				{
				linedata[i]='-';
				}
			  }			
	        strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
            linecnt++;
		}
	//NOx排放
		if(P->pselect[27]==1)  //选择打印了
		{
			fdbuf=QTLD;         //气体浓度
			fdbuf=QTLD*BGLL/1000000;   //排放量的计算

			strcpy((char *)&linedata[0],(char*)"NOX 排放:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" kg/h     ");  //补空10
	        strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
            linecnt++;
		}
	 }
	 if(Psour->Other[27][19]=='9')  //有CO数据
	 {
		 	strncpy((char *)&data[0],(char *)&Psour->Other[24][9],13);
		  data[13]=0;
		  //把字符串转换成浮点数
		  fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
		  QTLD=fdbuf;
	//CO浓度
		if(P->pselect[30]==1)  //选择打印了
		{ 
			strcpy((char *)&linedata[0],(char*)" CO 浓度:");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&Psour->Other[24][9],19);
	    data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)"    ");  //补空
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
			}
	//CO折算
		if(P->pselect[30]==1)  //选择打印了
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //求得折算后结果
			
			strcpy((char *)&linedata[0],(char*)" CO 折算:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf1); //13
	    data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
			if(GSXS==1000000)
			  { for(i=12;i<=21;i++)
				{
				linedata[i]='-';
				}
			  }
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
        linecnt++;
		}
	//CO排放
		if(P->pselect[30]==1)  //选择打印了
		{
			fdbuf=QTLD;         //气体浓度
			fdbuf=QTLD*BGLL/1000000;   //排放量的计算

			strcpy((char *)&linedata[0],(char*)" CO 排放:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	    data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)" kg/h     ");  //补空10
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
	 }
	 if(Psour->Other[27][20]=='9')  //有H2S数据
	 {
		 	strncpy((char *)&data[0],(char *)&Psour->Other[25][9],13);
		  data[13]=0;
		  //把字符串转换成浮点数
		  fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
		  QTLD=fdbuf;
	//H2S浓度
		if(P->pselect[31]==1)  //选择打印了
		{ 
			strcpy((char *)&linedata[0],(char*)"H2S 浓度:");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&Psour->Other[25][9],19);
	        data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)"    ");  //补空

	        strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
            linecnt++;
			}
	//H2S折算
		if(P->pselect[31]==1)  //选择打印了
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //求得折算后结果
			strcpy((char *)&linedata[0],(char*)"H2S 折算:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf1); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
			if(GSXS==1000000)
			  { for(i=12;i<=21;i++)
				{
				linedata[i]='-';
				}
			  }			
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
	//H2S排放
		if(P->pselect[31]==1)  //选择打印了
		{
			fdbuf=QTLD;         //气体浓度
			fdbuf=QTLD*BGLL/1000000;   //排放量的计算
			strcpy((char *)&linedata[0],(char*)"H2S 排放:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	    data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)" kg/h     ");  //补空10
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
	 }
	if(Psour->Other[27][21]=='9')  //有CO2数据
	 {
		 	strncpy((char *)&data[0],(char *)&Psour->Other[26][9],13);
		  data[13]=0;
		  //把字符串转换成浮点数
		  fdbuf=Calculate_CharToFloat((u8 *)&data[0],13);
		  QTLD=fdbuf;
	//CO2浓度
   if(P->pselect[32]==1)  //选择打印了
		{ 
			strcpy((char *)&linedata[0],(char*)"CO2 浓度:");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&Psour->Other[26][9],19);
	    data[19]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)"    ");  //补空
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
			}
	//CO2折算
		if(P->pselect[32]==1)  //选择打印了
		{
			fdbuf1=CalculationFormula_Ca(fdbuf,GSXS,ZSXS,FHXS); //求得折算后结果
			strcpy((char *)&linedata[0],(char*)"CO2 折算:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf1); //13
	        data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	        strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补空
			if(GSXS==1000000)
			  { for(i=12;i<=21;i++)
				{
				linedata[i]='-';
				}
			  }			
	        strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
            linecnt++;
		}
	//CO2排放
		if(P->pselect[32]==1)  //选择打印了
		{
			fdbuf=QTLD;         //气体浓度
			fdbuf=QTLD*BGLL/1000000;   //排放量的计算
			strcpy((char *)&linedata[0],(char*)"CO2 排放:");      //在缓冲区取出时间 9
			sprintf((char *)&data[0],"   %10.1f",fdbuf); //13
	    data[13]=0;  
			strcat((char *)&linedata[0],(char *)&data[0]);
	    strcat((char *)&linedata[0],(char *)" kg/h     ");  //补空10
	    strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
      linecnt++;
		}
	 }
 }
	//网站
	 strcpy((char *)&linedata[0],(char *)"-----");  //"网站"
	 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21字节
	 strcat((char *)&linedata[0],(char *)"------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	//电话
	 strcpy((char *)&linedata[0],(char *)"---------");  //"电话"
	 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13字节
	 strcat((char *)&linedata[0],(char *)"----------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	
	
	 P->LineMax=linecnt;  //记下最多行数

}
/****************************************************************************
	函数名：PrintAPI_CurTest()
  输  入: 
         
	输  出:
	功能说明：/打印正在测试数据转换到打印缓冲

*/
void PrintAPI_CurTest(Printer_Structure *P)                  //打印正在测试数据转换到打印缓冲
{
	char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //计打印行数
	
		// 型号+打印类别
	strncpy((char *)&LineData[linecnt][0],(char *)&p_gasrealt_hand1[0],linenumber-1);  //
	linecnt++;		
	
	//仪器编号
	strcpy((char *)&linedata[0],(char *)"仪器编号:");  //"仪器编号:"
	strcat((char *)&linedata[0],(char *)"      ");  //补空
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8字节编号
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//打印时间
	strcpy((char *)&linedata[0],(char *)"测量时间:");  //"测量时间:"
	strncpy((char *)&data[0],(char *)&P->dat[0],16);  //  16字节时间
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"       ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//采样地点
	strcpy((char *)&linedata[0],(char *)"采样地点: ");  //"采样地点:"//10字节
	strncpy((char *)&data[0],(char *)&P->address[0],22);  //  22字节
	data[22]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	
	strncpy((char *)&linedata[0],(char *)&P->address[22],28);
	linedata[28]=0;
	strcat((char *)&linedata[0],(char *)"    ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	if(P->installselect[0]==1)  //有O2数据
	{//O2浓度,含氧量
		if(P->pselect[0]==1)  //选择打印了
		{
			strcpy((char *)&linedata[0],(char*)" O2 浓度:   ");      //在缓冲区取出时间  12字节
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[0][0],10);  //  10字节时间
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" %        ");  //补单位
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
	 }
	 if(P->installselect[1]==1)  //有SO2数据
	 {//SO2浓度,
		if(P->pselect[4]==1)  //选择打印了
		{
			strcpy((char *)&linedata[0],(char*)"SO2 浓度:   ");      //在缓冲区取出时间 12字节
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[1][0],10);  //  10字节时间
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补单位10字节
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
	 }
	 if(P->installselect[2]==1) //有NO数据
	 {//NO浓度
		if(P->pselect[12]==1)  //选择打印了
		{
			strcpy((char *)&linedata[0],(char*)" NO 浓度:   ");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[2][0],10);  //  10字节时间
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补单位
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		linecnt++;
		}
	 }
	 if(P->installselect[2]==1)  //有NO2数据
	 {
		 //NO2浓度,
		if(P->pselect[16]==1)  //选择打印了
		{
			strcpy((char *)&linedata[0],(char*)"NO2 浓度:   ");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[3][0],10);  //  10字节时间
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补单位
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;	
		}
		 //NOx浓度,
		if(P->pselect[8]==1)  //选择打印了
		{
			strcpy((char *)&linedata[0],(char*)"NOx 浓度:   ");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[4][0],10);  //  10字节时间
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补单位
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;	
		}
	 }
	 if(P->installselect[2]==1)  //有C0数据
	 {//CO浓度,
		if(P->pselect[20]==1)  //选择打印了
		{
			strcpy((char *)&linedata[0],(char*)" CO 浓度:   ");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[5][0],10);  //  10字节时间
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补单位
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;	
		}
	 }
	 if(P->installselect[2]==1)  //有H2S数据
	 {//H2S浓度,
		if(P->pselect[24]==1)  //选择打印了
		{
			strcpy((char *)&linedata[0],(char*)"H2S 浓度:   ");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[6][0],10);  //  10字节时间
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" mg/m3    ");  //补单位
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;	
		}
	 }
	 
	 if(P->installselect[2]==1)  //有CO2数据
	 {//CO2浓度,含氧量
		if(P->pselect[28]==1)  //选择打印了
		{
			strcpy((char *)&linedata[0],(char*)"CO2 浓度:   ");      //在缓冲区取出时间 
			strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[7][0],10);  //  10字节时间
			data[10]=0;
			strcat((char *)&linedata[0],(char *)&data[0]);  //
			strcat((char *)&linedata[0],(char *)" %        ");  //补单位
			strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
			linecnt++;
		}
	 }
		//网站
		 strcpy((char *)&linedata[0],(char *)"-----");  //"网站"
		 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21字节
		 strcat((char *)&linedata[0],(char *)"------");
		 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		 linecnt++;	
		//电话
		 strcpy((char *)&linedata[0],(char *)"---------");  //"电话"
		 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13字节
		 strcat((char *)&linedata[0],(char *)"----------");
		 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
		 linecnt++;		
	
		P->LineMax=linecnt;  //记下最多行数 
	
}
/****************************************************************************
	函数名：PrintAPI_PressureParaOrder()
  输  入:
         
	输  出:
	功能说明：//打印压力参数数据转换到打印缓冲

*/
void PrintAPI_PressureParaOrder(Printer_Structure *P)        //打印压力参数数据转换到打印缓冲
{
	char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //计打印行数
	u8 i;
	// 型号+打印类别
	strncpy((char *)&LineData[linecnt][0],(char *)&p_PressurePara_hand[0],linenumber-1);  //
	linecnt++;		
	
	//仪器编号
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[2][0]);  //"仪器编号:"
	strcat((char *)&linedata[0],(char *)"      ");  //补空
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8字节编号
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//打印时间
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[3][0]);  //"时    间:"
	strncpy((char *)&data[0],(char *)&P->dat[0],16);  //  16字节时间
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"       ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	
  //0:气压零点
	for(i=0;i<=3;i++)
	{
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[i+13][0]);  //"气压零点:"
	strcat((char *)&linedata[0],(char *)"    ");  //补空
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[i][0],10);  //  10字节时间
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	}

	//4:尘流压零点
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[17][0]);  //"尘流压零点"
	strcat((char *)&linedata[0],(char *)"  ");  //补空
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[4][0],10);  //  10字节时间
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//5:气流压零点
  strcpy((char *)&linedata[0],(char *)&p_para_item_name[49][0]);  //"气流压零点:"
	strcat((char *)&linedata[0],(char *)"  ");  //补空
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[5][0],10);  //  10字节时间
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	
	//网站
	 strcpy((char *)&linedata[0],(char *)"-----");  //"网站"
	 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21字节
	 strcat((char *)&linedata[0],(char *)"------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	//电话
	 strcpy((char *)&linedata[0],(char *)"---------");  //"电话"
	 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13字节
	 strcat((char *)&linedata[0],(char *)"----------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	
	 P->LineMax=linecnt;  //记下最多行数
}
/****************************************************************************
	函数名：PrintAPI_TemperatureParaOrder()
  输  入:
         
	输  出:
	功能说明：//打印温度参数数据转换到打印缓冲

*/
void PrintAPI_TemperatureParaOrder(Printer_Structure *P)     //打印温度参数数据转换到打印缓冲
{
	char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //计打印行数
	// 型号+打印类别
	strncpy((char *)&LineData[linecnt][0],(char *)&p_TemperaturePara_hand[0],linenumber-1);  //
	linecnt++;		
		//仪器编号
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[2][0]);  //"仪器编号:"
	strcat((char *)&linedata[0],(char *)"      ");  //补空
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8字节编号
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//打印时间
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[3][0]);  //"时    间:"
	strncpy((char *)&data[0],(char *)&P->dat[0],16);  //  16字节时间
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"       ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//0:烟温/干球零点
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[0][0]);  //"烟温/干球零点:"  14字节

	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[0][0],10);  //  10字节时间
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"        ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//1:烟温/干球倍率
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[1][0]);  //"烟温/干球倍率:"  14字节

	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[1][0],10);  //  10字节时间
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"        ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//2:湿球零点
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[18][0]);  //"湿球零点:"
	strcat((char *)&linedata[0],(char *)"    ");  //补空
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[2][0],10);  //  10字节时间
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
  //3:湿球倍率
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[19][0]);  //"湿球零点:"
	strcat((char *)&linedata[0],(char *)"    ");  //补空
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[3][0],10);  //  10字节时间
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//4:计温零点
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[20][0]);  //"湿球零点:"
	strcat((char *)&linedata[0],(char *)"    ");  //补空
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[4][0],10);  //  10字节时间
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
		//网站
	 strcpy((char *)&linedata[0],(char *)"-----");  //"网站"
	 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21字节
	 strcat((char *)&linedata[0],(char *)"------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	//电话
	 strcpy((char *)&linedata[0],(char *)"---------");  //"电话"
	 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13字节
	 strcat((char *)&linedata[0],(char *)"----------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	
	 P->LineMax=linecnt;  //记下最多行数
	
}
/****************************************************************************
	函数名：PrintAPI_FlowParaOrder()
  输  入:
         
	输  出:
	功能说明：//打印流量参数数据转换到打印缓冲

*/
void PrintAPI_FlowParaOrder(Printer_Structure *P)           //打印流量参数数据转换到打印缓冲
{
	char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //计打印行数
	u8 i;
	// 型号+打印类别
	strncpy((char *)&LineData[linecnt][0],(char *)&p_FlowPara_hand[0],linenumber-1);  //
	linecnt++;		
		//仪器编号
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[2][0]);  //"仪器编号:"
	strcat((char *)&linedata[0],(char *)"      ");  //补空
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8字节编号
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//打印时间
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[3][0]);  //"时    间:"
	strncpy((char *)&data[0],(char *)&P->dat[0],16);  //  16字节时间
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"       ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	
	//0:环境温度
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[4][0]);  //"环境温度"
	strcat((char *)&linedata[0],(char *)"    ");  //补空
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[0][0],10);  //  10字节时间
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)" ℃       ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	//1:大气压
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[5][0]);  //"大气压"
	strcat((char *)&linedata[0],(char *)"    ");  //补空
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[1][0],10);  //  10字节时间
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)" kPa     ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;		
	//1.0倍率
	//20倍率
	//30倍率
	//40倍率
	//50倍率
	//70倍率
	//90倍率
	
	for(i=2;i<=8;i++)
	{
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[i+4][0]);  //""
	strcat((char *)&linedata[0],(char *)"    ");  //补空
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[i][0],10);  //  10字节时间
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
  }
	//100倍率
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[50][0]);  //"100L倍率"
	strcat((char *)&linedata[0],(char *)"    ");  //补空
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[i][0],10);  //  10字节时间
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	
		//网站
	 strcpy((char *)&linedata[0],(char *)"-----");  //"网站"
	 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21字节
	 strcat((char *)&linedata[0],(char *)"------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	//电话
	 strcpy((char *)&linedata[0],(char *)"---------");  //"电话"
	 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13字节
	 strcat((char *)&linedata[0],(char *)"----------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	
	 P->LineMax=linecnt;  //记下最多行数
}
/****************************************************************************
	函数名：PrintAPI_ConcentraParaOrder()
  输  入:
         
	输  出:
	功能说明：//打印浓度参数数据转换到打印缓冲

*/
void PrintAPI_ConcentraParaOrder(Printer_Structure *P)        //打印浓度参数数据转换到打印缓冲
{ char linedata[linenumber+10]={0};
	char data[linenumber]={0};
	u32  linecnt=0;  //计打印行数
  u8 i;
	// 型号+打印类别
	strncpy((char *)&LineData[linecnt][0],(char *)&p_ConcentraPara_hand[0],linenumber-1);  //
	linecnt++;		
		//仪器编号
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[2][0]);  //"仪器编号:"
	strcat((char *)&linedata[0],(char *)"      ");  //补空
	strncpy((char *)&data[0],(char *)&P->bianhao[0],8);  //  8字节编号
	data[8]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         "); //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	//打印时间
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[3][0]);  //"时    间:"
	strncpy((char *)&data[0],(char *)&P->dat[0],16);  //  16字节时间
	data[16]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"       ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	

	for(i=0;i<=27;i++)
	{
	strcpy((char *)&linedata[0],(char *)&p_para_item_name[i+21][0]);  // 13字节
	strncpy((char *)&data[0],(char *)&P->Para_StrData.databuf[i][0],10);  //  10字节时间
	data[10]=0;
	strcat((char *)&linedata[0],(char *)&data[0]);
	strcat((char *)&linedata[0],(char *)"         ");  //补空
	strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	linecnt++;	
	}

		//网站
	 strcpy((char *)&linedata[0],(char *)"-----");  //"网站"
	 strcat((char *)&linedata[0],(char *)COMPANY_WEBSITE);  //21字节
	 strcat((char *)&linedata[0],(char *)"------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	//电话
	 strcpy((char *)&linedata[0],(char *)"---------");  //"电话"
	 strcat((char *)&linedata[0],(char *)COMPANY_TELEPHON);  //13字节
	 strcat((char *)&linedata[0],(char *)"----------");
	 strncpy((char *)&LineData[linecnt][0],(char *)&linedata[0],linenumber-1);  //
	 linecnt++;	
	
	 P->LineMax=linecnt;  //记下最多行数
}


//打印命令组
/****************************************************************************
	函数名：Print_Init_Com
	输  入: 
         
	输  出:
	功能说明：打印机初始化
******************************************************************************/
void Print_Init_Com(void)
{
	
	  Printf_GetData()->Printf_Mode=4;
		printf("%c",0x1B);
		printf("%c",0x40);
	  Printf_GetData()->Printf_Mode=1;
}
/****************************************************************************
	函数名：Print_Printorder_Com
	输  入: 
         
	输  出:
	功能说明：打印指令
******************************************************************************/
void Print_Printorder_Com(void)
{
	
	  Printf_GetData()->Printf_Mode=4;
		printf("%c",0xD);
	  Printf_GetData()->Printf_Mode=1;
}
/****************************************************************************
	函数名：Print_ChangLine_Com
	输  入: 
         
	输  出:
	功能说明：换行指令
******************************************************************************/
void Print_ChangLine_Com(void)
{
	/*换行指令*/
	  Printf_GetData()->Printf_Mode=4;
		printf("%c",0xA);
	  Printf_GetData()->Printf_Mode=1;
}
/****************************************************************************
	函数名：Print_Chk_Com
	输  入: 
         
	输  出:
	功能说明：查询指令
******************************************************************************/
void Print_Chk_Com(void)
{
	
	 Printf_GetData()->Printf_Mode=4;
	 printf("%c",0x1B);
	 printf("%c",0x76);
	 Printf_GetData()->Printf_Mode=1;
}

/****************************************************************************
	函数名：PrintLine(u16 line)
	输  入: line：行数 >=0
         
	输  出:
	功能说明：打印第几行数据
*******************************************************************************/

void PrintLine(u16 line)
{
	Printf_GetData()->Printf_Mode=4;
	printf("%s",&LineData[line][0]);   //打印第几行数据
	Printf_GetData()->Printf_Mode=1;
}


/*******************************************************
                  以下是打印命令接口
********************************************************/

/****************************************************************************
	函数名：Print_WorkFileOrder(Printer_Structure *P)
  输  入:Printer_Structure *P ；打印对象,对象里预先传入数据
         
	输  出:
	功能说明：打印工况文件指令,外调用
*******************************************************************************/
void Print_WorkFileOrder(Printer_Structure *P)  //打印工况文件
{
	P->RunMode_high=11;
	P->print_cnt_file=P->print_start_file;
}
/****************************************************************************
	函数名：Print_GasFileOrder(Printer_Structure *P)
  输  入: Printer_Structure *P ；打印对象,对象里预先传入数据
        
	输  出:
	功能说明：打印烟气文件指令,外调用
*******************************************************************************/
void Print_GasFileOrder(Printer_Structure *P)    //打印烟气文件
{
	P->RunMode_high=13;
	P->print_cnt_file=P->print_start_file;
}
/****************************************************************************
	函数名：Print_DustFileOrder(Printer_Structure *P)
  输  入: Printer_Structure *P ；打印对象,对象里预先传入数据
       
	输  出:
	功能说明：打印烟尘文件指令,外调用
*******************************************************************************/
void Print_DustFileOrder(Printer_Structure *P)    //打印烟尘文件
{
	P->RunMode_high=15;
	P->print_cnt_file=P->print_start_file;
}
/****************************************************************************
	函数名：Print_GasRecordOrder(Printer_Structure *P)
  输  入: Printer_Structure *P ；打印对象,对象里预先传入数据
        
	输  出:
	功能说明：打印烟气记录文件,外调用
*******************************************************************************/
void Print_GasRecordFileOrder(Printer_Structure *P) //打印烟气记录文件
{
	P->RunMode_high=17;
    P->print_cnt_file=P->print_start_file;  //从第一记录开始
}

/****************************************************************************
	函数名：Print_PressureParaOrder(Printer_Structure *P)
  输  入:  Printer_Structure *P ；打印对象,对象里预先传入数据
        
	输  出:
	功能说明：打印压力参数指令,外调用
*******************************************************************************/
void Print_PressureParaOrder(Printer_Structure *P)             //打印压力参数
{
	P->RunMode_high=1;
}
/****************************************************************************
	函数名：Print_TemperatureOrder(Printer_Structure *P)
  输  入: 
        
	输  出:
	功能说明：打印温度文件指令,外调用
*******************************************************************************/
void Print_TemperatureParaOrder(Printer_Structure *P)          //打印温度参数
{
	P->RunMode_high=2;
}
/****************************************************************************
	函数名：Print_FlowParaOrder()
  输  入:
      
	输  出:
	功能说明：打印流量参数指令,外调用
*******************************************************************************/
void Print_FlowParaOrder(Printer_Structure *P)                 //打印流量参数
{
	P->RunMode_high=3;
}
/****************************************************************************
	函数名：Print_ConcentraOrder()
  输  入: 
        
	输  出:
	功能说明：打印农度参数指令,外调用
*******************************************************************************/
void Print_ConcentraParaOrder(Printer_Structure *P)        //打印农度参数
{
	P->RunMode_high=4;
}
/****************************************************************************
	函数名：Print_CurTestOrder()
  输  入:  Printer_Structure *P ；打印对象,对象里预先传入数据
        
	输  出:
	功能说明：打印正在测试指令,外调用
*******************************************************************************/
void Print_CurTestOrder(Printer_Structure *P)                  //打印正在测试
{
	P->RunMode_high=5;
}
/****************************************************************************
	函数名：Print_Init(void
	输  入: 
         
	输  出:
	功能说明：打印模块初始化
*******************************************************************************/
void Print_Init(void)
{
	Printer_Data.RunMode=0;
	Printer_Data.RunMode_high=0;
}

/****************************************************************************
	函数名：Print_Init(void
	输  入: 
         
	输  出:
	功能说明：打印模块定时器
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
	函数名：Print_Low_Main(void)
	输  入: 数据已经保存到指定的缓冲区
         
	输  出:
	功能说明：打印的低层线程只打印缓冲区中内容

*/
void Print_Low_Main(void){
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
			if(Printer_Data.Timer==0)  //等时间到
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
				Printer_Data.Flag=2;//时间溢出
			}
			else
			{
		  if(USART_COM4_GetData()->ReceveOkFlg)  //接收完
			  {
          if ((USART_COM4_GetData()->USART_RX_BUF[0]&0X0F) == 0x0000) 
					{
						Printer_Data.RunMode =8;
						Printer_Data.line=0;
					}
			  }
				else
				{
					Print_Chk_Com();  //发生查询状态指令
			    USART_COM4_Receve_com(0,1);  //透传模式要求串口接收一个字节
				}
		
		}
			break;	
	}
		case 8:
		{
						PrintLine(Printer_Data.line);  //打印第几行
			      Print_ChangLine_Com();         //回车换行
						Printer_Data.line++;                                 //在这里可判断是否超过打印机2K缓存
		        if(Printer_Data.line>=Printer_Data.LineMax)  //打印完毕
						{
							Printer_Data.RunMode =9;
						}
					
			break;
		}
		case 9:
		{ 
  		 Print_Printorder_Com();    //打印命令
			 Printer_Data.Timer=2000;   //延时2s
			 Printer_Data.RunMode = 10;
			break;
		}
		case 10:
		{
			if(Printer_Data.Timer==0)
			{
					Print_Chk_Com();  //发生查询状态指令
			    USART_COM4_Receve_com(0,1);  //透传模式要求串口接收一个字节
				  Printer_Data.RunMode =11;
				  Printer_Data.Timer=2000;   //延时2s
			}
			break;
		}
		case 11:
		{ 
			if(USART_COM4_GetData()->ReceveOkFlg)  //接收完
			  {
          if ((USART_COM4_GetData()->USART_RX_BUF[0]&0X0F) == 0x0000) 
					{
						Printer_Data.RunMode =0;
						Printer_Data.Flag=1;
					}
			  }
				else
				{
					Print_Chk_Com();  //发生查询状态指令
			    USART_COM4_Receve_com(0,1);  //透传模式要求串口接收一个字节
				}
			if(Printer_Data.Timer==0)  //超时
				{
	        Printer_Data.RunMode =0;
					Printer_Data.Flag=2;//超时错误
				}				
			
			break;
		}
 }
}

//打印的高层监控
void Print_High_Main(void)
	{
	switch(Printer_Data.RunMode_high)
		{
		case 0:
		{
			break;
		}
		//打印参数
		case 1:  	//打印压力参数任务,并转到那个小线程
		{
		  PrintAPI_PressureParaOrder(&Printer_Data); //把数据转换
			Printer_Data.RunMode=1;       //启动底层打印监控
			Printer_Data.Flag_high=0;     //标准清零
			Printer_Data.Timer_high=10000; //10秒监控
			Printer_Data.RunMode_high=10;  //进入下一步等待打印完毕
			break;
		}
		case 2:  	//打印温度参数任务,并转到那个小线程
		{
		  PrintAPI_TemperatureParaOrder(&Printer_Data); //把数据转换
			Printer_Data.RunMode=1;       //启动底层打印监控
			Printer_Data.Flag_high=0;     //标准清零
			Printer_Data.Timer_high=10000; //10秒监控
			Printer_Data.RunMode_high=10;  //进入下一步等待打印完毕
			break;
		}
		case 3:  	//打印流量参数任务,并转到那个小线程
		{
		  PrintAPI_FlowParaOrder(&Printer_Data); //把数据转换
			Printer_Data.RunMode=1;       //启动底层打印监控
			Printer_Data.Flag_high=0;     //标准清零
			Printer_Data.Timer_high=10000; //10秒监控
			Printer_Data.RunMode_high=10;  //进入下一步等待打印完毕
			break;
		}
		case 4:  	//打印浓度参数任务,并转到那个小线程
		{
		  PrintAPI_ConcentraParaOrder(&Printer_Data); //把数据转换
			Printer_Data.RunMode=1;       //启动底层打印监控
			Printer_Data.Flag_high=0;     //标准清零
			Printer_Data.Timer_high=10000; //10秒监控
			Printer_Data.RunMode_high=10;  //进入下一步等待打印完毕
			break;
		}
			case 5:  	//打印浓度参数任务,并转到那个小线程
		{
		  PrintAPI_CurTest(&Printer_Data); //把数据转换
			Printer_Data.RunMode=1;       //启动底层打印监控
			Printer_Data.Flag_high=0;     //标准清零
			Printer_Data.Timer_high=10000; //10秒监控
			Printer_Data.RunMode_high=10;  //进入下一步等待打印完毕
			break;
		}	

		case 10:        //小公共部分
		{
			if(Printer_Data.Timer_high==0)
			{
				Printer_Data.Flag_high=2;//超时溢出
				Printer_Data.RunMode_high=0;          //设回等待
				printf("打印溢出");
			}
			else if(Printer_Data.Flag>0)
			{
				Printer_Data.Flag_high=Printer_Data.Flag; //把提取底层任务结果
				Printer_Data.RunMode_high=0;          //设回等待模式
				if(Printer_Data.Flag==1)
				{
					Printer_Data.Flag_high=1;     //打印完成
					printf("打印完成");
				}
				else if(Printer_Data.Flag==2)
				{ Printer_Data.Flag_high=2;     //标准清零
					printf("打印溢出");
				}
			}
			break;
		}
		
		
		case 11:   //打印工况文件
		{
			//先读取SD卡的数据,先读取最大文件数
			SdFile_Serch_MaxofWorkLinkFile();   //搜索工况链接文件最大文件号
            Printer_Data.Flag_high=0;     //标准清零		
			Printer_Data.RunMode_high=12;
		
			break;
		}
		case 12:
		{
			SdFile_ReadWorkFile(Printer_Data.print_cnt_file); //读取工况文件记录
	    PrintAPI_WorkFile(&Printer_Data,&SdFile_GetData()->rdWorkFile);
			Printer_Data.Timer_high=10000; //10秒监控
			Printer_Data.RunMode_high=30;  //进入下一步等待打印完毕
			Printer_Data.NextMode_high=12;
			Printer_Data.RunMode=1;       //启动底层打印监控
			
			break;
		}
		
		case 13:  //打印烟气
		{
			//先读取SD卡的数据,先读取最大文件数
			SdFile_Serch_MaxofGasLinkFile();   //搜索工况链接文件最大文件号
			Printer_Data.Flag_high=0;     //标准清零		
			Printer_Data.RunMode_high=14;
			break;
		}
		case 14:
		{
			SdFile_ReadGasFile(Printer_Data.print_cnt_file,0x01); //读取工况文件记录
			PrintAPI_GasFile(&Printer_Data,&SdFile_GetData()->rdWorkFile);
			Printer_Data.Timer_high=10000; //10秒监控
			Printer_Data.RunMode_high=30;  //进入下一步等待打印完毕
			Printer_Data.NextMode_high=14;
			Printer_Data.RunMode=1;       //启动底层打印监控
			break;
		}
		case 15:  //打印烟尘
		{
			//先读取SD卡的数据,先读取最大文件数
			SdFile_Serch_MaxofDustLinkFile();   //搜索工况链接文件最大文件号
      Printer_Data.Flag_high=0;     //标准清零		
			Printer_Data.RunMode_high=16;
      break;
		}
		case 16:
		{
			SdFile_ReadDustFile(Printer_Data.print_cnt_file); //读取烟尘文件记录
	        PrintAPI_DustFile(&Printer_Data,&SdFile_GetData()->rdWorkFile);
			Printer_Data.Timer_high=10000; //10秒监控
			Printer_Data.RunMode_high=30;  //进入下一步等待打印完毕
			Printer_Data.NextMode_high=16;
			Printer_Data.RunMode=1;       //启动底层打印监控
      break;
		}
		case 17:  //打印烟气分钟记录
		{
			SdFile_Serch_MaxofGasLinkFile();   //搜索工况链接文件最大文件号
			SdFile_Serch_MaxofGasRecord(Printer_Data.print_FileNomber);  //当前文件编号
			Printer_Data.Flag_high=0;       //标准清零	
			Printer_Data.RunMode_high=18;
			
			break;
		}
		case 18:
		{
			if(Printer_Data.record_Cnt >= SdFile_GetData()->GasRecordMax)
			{
				Printer_Data.RunMode_high=0;          //设回等待模式
				Printer_Data.Flag_high=1;     //打印完成
				return ;
			}
			if(Printer_Data.PrintSelect==0) //常规打印
			{				
				SdFile_ReadGasFile(Printer_Data.print_FileNomber,Printer_Data.print_cnt_file);  //读取文件数据
			}
			else
			{
			    u8 reordNum;				
				while(Printer_Data.record_Cnt< SdFile_GetData()->GasRecordMax)	//修正分钟数据“打印选中”功能不能打印全部数据的问题, modify by alfred, 2020.07.07
				{     
					Printer_Data.record_Cnt++;						
					reordNum=Printer_Data.record_Cnt; 
					SdFile_ReadGasFile(Printer_Data.print_FileNomber,reordNum);  //读取文件数据
					break;		
				}	
			}
			PrintAPI_GasRecordFile(&Printer_Data,&SdFile_GetData()->rdWorkFile);
			Printer_Data.Timer_high=10000; //10秒监控
			Printer_Data.RunMode_high=30;  //进入下一步等待打印完毕
			Printer_Data.NextMode_high=18;
			Printer_Data.RunMode=1;       //启动底层打印监控
      break;

		case 30:
		{
			
			if(Printer_Data.Timer_high==0)
			{
				Printer_Data.Flag_high=2;//超时溢出
				Printer_Data.RunMode_high=0;          //设回等待模式
				printf("打印溢出");
			}
			else if(Printer_Data.Flag>0)
			{
				Printer_Data.Flag_high=Printer_Data.Flag; //把提取底层任务结果
				
				if(Printer_Data.Flag==1)
				{
					printf("/r/n打印完成文件%d:",Printer_Data.print_cnt_file);
					
					if(Printer_Data.PrintSelect==0) //常规打印
					{
						Printer_Data.print_cnt_file++;
						if(Printer_Data.print_cnt_file>Printer_Data.print_end_file)
						{
							Printer_Data.RunMode_high=0;          //设回等待模式
							Printer_Data.Flag_high=1;     //打印完成
							printf("/r/n所有文件打印完成");
						}
						else
						{
							Printer_Data.RunMode_high=Printer_Data.NextMode_high; //打印下一个文件
							
						}
						
					}else  //打印选中
					{
					  if(Printer_Data.record_Cnt>63)
						{
							Printer_Data.RunMode_high=0;          //设回等待模式
							Printer_Data.Flag_high=1;     //打印完成
							printf("/r/n所有文件打印完成");
						}
						else
						{
							Printer_Data.RunMode_high=Printer_Data.NextMode_high; //打印下一个文件
							
						}
					}
				}
				else if(Printer_Data.Flag==2)
				{ Printer_Data.Flag_high=2;     //标准清零
					Printer_Data.RunMode_high=0;          //设回等待模式
					printf("打印溢出");
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
//打印监控线程，主程序调用
void Print_Main(void)
{
	Print_Low_Main();
	Print_High_Main();
}	


//-------------------------调试函数----------------------------
//---
/*
	函数名： void Print_DebugASCII(u8 *p,u8 len)
	描述：		该函数为AT.c文件的ASCII调试文件
						:000|05|001
*/
void Print_DebugASCII(u8 *p,u8 len)
{ u8 i;
	//char buf[202];
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:
		{//
			if(AT_GetData()->Mode==0)     //在透传模式
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
				printf("不在透传模式");
			}
			break;
		}
		case 2:
		{//
		if(AT_GetData()->Mode==0)     //在透传模式
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
				printf("不在透传模式");
			}
			break;
		
		}
		case 3:
		{//
			if(AT_GetData()->Mode==0)     //在透传模式
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
				printf("不在透传模式");
			}
			break;
		}
		case 4:
		{
			if(AT_GetData()->Mode==0)     //在透传模式
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
				printf("不在透传模式");
			}
			break;
		}
		case 5:
		{
			if(AT_GetData()->Mode==0)     //在透传模式
			{
			strcpy((char *)&Printer_Data.bianhao[0],(char *)"12345678");	
			strcpy((char *)&Printer_Data.dat[0],(char *)"2019-12-20 14:20");
			strcpy((char *)&Printer_Data.address[0],(char *)"广东顺德大良xxxxxxxxXXXXXXXXXXxxxxxxxxxxXXXXXXXXXX");


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
				printf("不在透传模式");
			}
			break;
		}
		case 6:    //打印工况文件
		{
			if(AT_GetData()->Mode==0)     //在透传模式
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
				printf("不在透传模式");
			}
			
			break;
		}
		case 7:
		{
			if(AT_GetData()->Mode==0)     //在透传模式
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
				printf("不在透传模式");
			}
			break;
		}
		case 8:
		{
			if(AT_GetData()->Mode==0)     //在透传模式
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
				printf("不在透传模式");
			}
			break;
		}
		case 9:
		{
			if(AT_GetData()->Mode==0)     //在透传模式
			{
			Printer_Data.print_start_file=1;   // //从第1记录开始
			Printer_Data.print_end_file=2;    //结束文件号   //把这个设成Printer_Data.print_start_file（当前），可打印当前
			Printer_Data.print_FileNomber=9;  //9号文件	
       
			strcpy((char *)&Printer_Data.bianhao[0],(char *)"12345678");	
			strcpy((char *)&Printer_Data.dat[0],(char *)"2019-12-20 14:20");
			Print_GasRecordFileOrder(&Printer_Data);
			}
			else
			{
				printf("不在透传模式");
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

