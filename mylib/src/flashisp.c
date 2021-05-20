/*
	本文件为该项目的STM32-Flash内部存储数据相关函数
	具体函数如下
*/

#include "flashisp.h"
#include "FM24CL64.h"

Flashisp_DataStructure FlashispData;
//---
/*
	函数名：void Flashisp_main(void)
	描述：	flashisp.c文件主函数
*/
void Flashisp_main(void)
{
	switch(FlashispData.RunMode)
	{
		case 0:
		{
			if((FlashispData.Flag)) //判断保存数据
			{//修改Flish区域的数据
				#ifdef FLASH_USE_FM24CL64
				FM24CL64_WriteData();
				FlashispData.Flag=0;
     				
				#else
				Flashisp_StoreData();
				#endif
				
				printf("\r\n存Flash成功\r\n");
			}
			break;
		}
		case 1:
		{//格式化数据
				{
				#ifndef FLASH_USE_FM24CL64
				FLASH_Unlock();  //解锁FLASH编程擦除控制器
				FLASH_DataCacheCmd(DISABLE);
				FLASH_EraseSector(STMFLASH_GetFlashSector(FLASHIS_AdrStoreStart),VoltageRange_3);//VCC=2.7~3.6V之间!!
				FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
				FLASH_Lock();    //锁定FLASH编程擦除控制器
				FlashispData.Flag = 1;
				#endif
				FlashispData.RunMode = 0;
			}
			break;
		}
	}
}
/*
	函数名：void Flashisp_Init(void)
	描述：	flashisp.c文件初始化函数
*/
void Flashisp_Init(void)
{
#ifdef FLASH_USE_FM24CL64
	// FM24CL64_Init();
	 FM24CL64_ReadData();//读取数据
	if(FlashispData.Data[FLASHIS_AdrStoreFactory] != FLASHIS_FACTORY)
	{
		Flashisp_FactoryReset(1);//恢复出厂设置
		Flashisp_DataInit();
	}	
	else
	{
	FlashispData.Flag = 0;  //没数据更新
	FlashispData.RunMode = 0;
	}
	#else 
	if(*(u16*)(FLASHIS_AdrStoreStart + ((u16)FLASHIS_AdrStoreFactory)*2) != FLASHIS_FACTORY)
	Flashisp_FactoryReset();//恢复出厂设置
	
	Flashisp_DataInit();
	#endif
}
//---
/*
	函数名：void Flashisp_DataInit(void)
	描述：	该函数为Flaships.c文件的数据初始化
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
	函数名：Flashisp_DataStructure* Flashisp_GetData(void)
	描述：	该函数为获取FlashispData结构体变量首地址
*/

Flashisp_DataStructure* Flashisp_GetData(void)
{
	return &FlashispData;
}
//---
/*
	函数名：void Flashisp_FactoryReset(void)
	描述：	该函数为将整个系统存储数据恢复出厂设置
*/
void Flashisp_FactoryReset(u8 select)
{
	u16 i;
	FourBytes four;
	struct tm myTm;
    u8 *addr=(u8*)"佛山宇隆博环保科技有限公司";
	
//	EightBytes eight;
	if(select)
	for(i=0;i<FLASHIS_AdrEnd;i++) FlashispData.Data[i] = 0;
	//系统设置
	four.FloatData = 25.5;			//环境温度
	FlashispData.Data[FLASHIS_AdrXTSZHJWD] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrXTSZHJWD+1] = four.U16Data[1];
	FlashispData.Data[FLASHIS_AdrXTSZHJWDKG] = 0;	//环境温度测量/输入-关闭
	four.FloatData = 103.5;			//大气压
	FlashispData.Data[FLASHIS_AdrXTSZDQY] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrXTSZDQY+1] = four.U16Data[1];

	for(i=0;i<15;i++) //系统采样地址
	FlashispData.Data[FLASHIS_AdrXTSZCYDZ+i]=(addr[2*i+1]<<8)+addr[2*i];;
	FlashispData.Data[FLASHIS_AdrXTSZBOX] = 0;	//Box选择开关
	FlashispData.Data[FLASHIS_AdrXTSZZDPSJG] = 30;	//排水间隔
	FlashispData.Data[FLASHIS_AdrXTSZZDPSSC] = 10;	//排水时长
  FlashispData.Data[FLASHIS_AdrXTSZYQCLSJ] = 10;//系统设烟气测量时间
	FlashispData.Data[FLASHIS_AdrXTSZYQCLCS] = 11;//系统设烟气测量次数
	FlashispData.Data[FLASHIS_AdrXTSZCHSLSJ] = 12;//系统设测含湿量时间

	//烟道布管配置
	
	//烟道布点设置
	
    FlashispData.Data[FLASHIS_AdrYDBDLX]=1;		//烟道布点类型  1=圆形  2=方形 3=其他类型
    four.FloatData=1.0;
		FlashispData.Data[FLASHIS_AdrYDBDYXNJ]=four.U16Data[0];				     //烟道布点圆形内径D
		FlashispData.Data[FLASHIS_AdrYDBDYXNJ+1]=four.U16Data[1];				     //烟道布点圆形内径D
		four.FloatData=65.358;
		FlashispData.Data[FLASHIS_AdrYDBD_MJ]=four.U16Data[0];							//烟道布点圆形面积
		FlashispData.Data[FLASHIS_AdrYDBD_MJ+1]=four.U16Data[1];	
    four.FloatData=1.00;		
		FlashispData.Data[FLASHIS_AdrYDBD_GT]=four.U16Data[0];	;						//烟道布点圆形管套
		FlashispData.Data[FLASHIS_AdrYDBD_GT+1]=four.U16Data[1];	;						//烟道布点圆形管套
		FlashispData.Data[FLASHIS_AdrYDBDYXHS]=0;						//烟道布点圆形环数
		FlashispData.Data[FLASHIS_AdrYDBDYXKS]=1;						//烟道布点圆形孔数
		FlashispData.Data[FLASHIS_AdrYDBD_CYDS]=1;					//烟道布点圆形采样点数
    
		four.FloatData=1.00;
		FlashispData.Data[FLASHIS_AdrYDBDFXBA]=four.U16Data[0];					//烟道布点方形一边A
		FlashispData.Data[FLASHIS_AdrYDBDFXBA+1]=four.U16Data[1];					//烟道布点方形一边A
		four.FloatData=1.00;
		FlashispData.Data[FLASHIS_AdrYDBDFXBB]=four.U16Data[0];					//烟道布点方形一边B
		FlashispData.Data[FLASHIS_AdrYDBDFXBB+1]=four.U16Data[1];					//烟道布点方形一边B	
		FlashispData.Data[FLASHIS_AdrYDBDFXKS]=1;					//烟道布点方形孔数
		FlashispData.Data[FLASHIS_AdrYDBDFXDKCD]=1;				//烟道布点方形单孔测点
     four.FloatData=0.00;
		 for(i=0;i<10;i++)
		{
		FlashispData.Data[FLASHIS_AdrYDBDCDJL1+2*i]=four.U16Data[0];					//烟道布点方形一边A
		FlashispData.Data[FLASHIS_AdrYDBDCDJL1+1+2*i]=four.U16Data[1];					//烟道布点方形一边A
		
		}

    //工况预测 压力调零
		FlashispData.Data[FLASHIS_AdrGKYC_YLTL_YQDY]=1;		                       //压力调零，烟气动压
		four.FloatData=123.01;
		FlashispData.Data[FLASHIS_AdrGKYC_YLTL_YQJY]=four.U16Data[0];					//压力调零，烟气静压
		FlashispData.Data[FLASHIS_AdrGKYC_YLTL_YQJY+1]=four.U16Data[1];					//压力调零，烟气静压
		four.FloatData=122.01;
		FlashispData.Data[FLASHIS_AdrGKYC_YLTL_JQYL]=four.U16Data[0];					//压力调零，计前压力
		FlashispData.Data[FLASHIS_AdrGKYC_YLTL_JQYL+1]=four.U16Data[1];					//压力调零，计前压力
		FlashispData.Data[FLASHIS_AdrGKYC_YLTL_CLY]=20;					//压力调零，尘流压压
		FlashispData.Data[FLASHIS_AdrGKYC_YLTL_QLY]=21;				//压力调零，气流压压

		//工况预测 烟温测量
		FlashispData.Data[FLASHIS_AdrGKYC_YWCL_FS]=0;				//烟温方式 0测量 1输入
		four.FloatData=89.1;
		FlashispData.Data[FLASHIS_AdrGKYC_YWCL_SRYW]=four.U16Data[0];					//烟温输入
		FlashispData.Data[FLASHIS_AdrGKYC_YWCL_SRYW+1]=four.U16Data[1];
		
		
//	//工况预测 查询平均值
//		 four.U32Data=1;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_WJH]=four.U16Data[0];					//文件号
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_WJH+1]=four.U16Data[1];					
//			four.U32Data=120;
//			
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJDY]=four.U16Data[0];					//平均动压
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJDY+1]=four.U16Data[1];					//平均动压
//    four.FloatData=121.11;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJJY]=four.U16Data[0];					//平均静压
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJJY+1]=four.U16Data[1];					//平均静压
//     four.FloatData=121.21;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_DQY]=four.U16Data[0];					//大气压
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_DQY+1]=four.U16Data[1];					//大气压
//     four.FloatData=121.3131;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_YDJMJ]=four.U16Data[0];					//烟道截面积
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_YDJMJ+1]=four.U16Data[1];					//烟道截界面
//      four.FloatData=121.4;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_CZZJ]=four.U16Data[0];					//采嘴直径
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_CZZJ+1]=four.U16Data[1];					//采嘴直径

//      four.FloatData=121.5;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJYW]=four.U16Data[0];					//平均烟气温度
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJYW+1]=four.U16Data[1];					//平均烟气温
//		
//		 four.FloatData=121.7;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJLS]=four.U16Data[0];					//平均流速
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_PJLS+1]=four.U16Data[1];					//平均流速
//		
//		 four.U32Data=124;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_YGLL]=four.U16Data[0];					//烟气流量
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_YGLL+1]=four.U16Data[1];					//烟气流量
//		four.U32Data=125;
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_BGLL]=four.U16Data[0];					//标杆流量
//		FlashispData.Data[FLASHIS_AdrGKYC_CPJZ_BGLL+1]=four.U16Data[1];					
    //测含湿量
	 // FlashispData.Data[FLASHIS_AdrCHSL_LX]=1;  	//测含湿量类型 
		four.FloatData=5;
//		FlashispData.Data[FLASHIS_AdrCHSL_CLZ]=four.U16Data[0];					//测量值
////		FlashispData.Data[FLASHIS_AdrCHSL_CLZ+1]=four.U16Data[1];				
////		four.FloatData=0.2;
		FlashispData.Data[FLASHIS_AdrCHSL_HSL]=four.U16Data[0];					//输入值
		FlashispData.Data[FLASHIS_AdrCHSL_HSL+1]=four.U16Data[1];	
//    //测量烟气
		FlashispData.Data[FLASHIS_AdrCLYQ_CLFS] = 0;	//烟气测量方式
    FlashispData.Data[FLASHIS_AdrCLYQ_JSSC] = 10;	//计时时长
//		//测量烟气 烟气校准
//		for(i=0;i<8;i++)
//		{
//		 four.FloatData=0.0;
//		 FlashispData.Data[FLASHIS_AdrCLYQ_CJO2+i*2]=four.U16Data[0];					//O2 校正
//	   FlashispData.Data[FLASHIS_AdrCLYQ_CJO2+i*2+1]=four.U16Data[1];
//		}
   //测量烟气 -测量中单位类型

	FlashispData.Data[FLASHIS_AdrCLYQ_DWLX] =	0;	
	//测量烟气 -文件查询 - 计算系数
	four.FloatData=0.1;
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_JS_ZSXS]=four.U16Data[0];					//折算系数
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_JS_ZSXS+1]=four.U16Data[1];	
	four.FloatData=0.2;
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_JS_JZYHL]=four.U16Data[0];					//基准氧气含量
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_JS_JZYHL+1]=four.U16Data[1];	
	four.FloatData=0.3;
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_JS_FHXS]=four.U16Data[0];					//负荷系数
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_JS_FHXS+1]=four.U16Data[1];	
	//测量烟气 -文件查询 - 打印设置
	four.U32Data=0xFFFFffff;
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_DYXX]=four.U16Data[0];					//打印选项
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_DYXX+1]=four.U16Data[1];	
			//烟尘采集 -文件查询 - 打印设置
	four.U32Data=0xFFFFffff;
	FlashispData.Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX]=four.U16Data[0];					//打印选项
	FlashispData.Data[FLASHIS_AdrYCCJ_YCWJCX_DYXX+1]=four.U16Data[1];	
		//测量烟气 -文件查询 - 分钟数据
	FlashispData.Data[FLASHIS_AdrCLYQ_WJCX_FZSJ_PJJS]=0;   //是否参与平均计算
	//烟尘采样吧 -采样设置
	four.FloatData=10.0;
	FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_CYLS]=four.U16Data[0];					//打印选项
	FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_CYLS+1]=four.U16Data[1];	
	four.U32Data=01;
	FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_LTH]=four.U16Data[0];					//打印选项
	FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_LTH+1]=four.U16Data[1];
	four.FloatData=0.84f;
	FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_CZZJ]=four.U16Data[0];					//打印选项
	FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_CZZJ+1]=four.U16Data[1];
    FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_CYFS]=0;
    FlashispData.Data[FLASHIS_AdrYCCJ_CYSE_DDCS]=600;//8min High8 Min Low8 Sec	
	four.FloatData=0.0;
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSRYND]=four.U16Data[0];					 //文件查询下 计算 溶液浓度
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSRYND+1]=four.U16Data[1];	
	four.FloatData=0.0;
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSDRTJ]=four.U16Data[0];					//文件查询下 计算 定溶体积
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSDRTJ+1]=four.U16Data[1];	
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSZTSM]=1;//文件查询下 计算 灶头数目
   
	 
	 //文件查询 - 计算/烟尘
	four.FloatData=1.0;
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSYCZL]=four.U16Data[0];					//文件查询下 计算 烟尘重量
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSYCZL+1]=four.U16Data[1];	
	
	four.FloatData=21.0;
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSHYL]=four.U16Data[0];					//文件查询下 计算 氧气含量
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSHYL+1]=four.U16Data[1];	
	
	four.FloatData=1.0;
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSFHXS]=four.U16Data[0];					//文件查询下 计算 负荷系数
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSFHXS+1]=four.U16Data[1];	
	
	four.FloatData=1.0;
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSZSXS]=four.U16Data[0];					//文件查询下 计算 折算系数
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSZSXS+1]=four.U16Data[1];	
	
	four.FloatData=1.0;
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSJZYHL]=four.U16Data[0];					//文件查询下 计算 基准氧气含量
	FlashispData.Data[FFLASHIS_AdrYCCJ_YCWJCX_JSJZYHL+1]=four.U16Data[1];	
	

	//------------------------------------------------------------------------
	//掉电数据相关
	FlashispData.Data[FLASHIS_AdrCJKSSK]=123;
	FlashispData.Data[FLASHIS_AdrCJKSSK+1]=456;
	FlashispData.Data[FLASHIS_AdrCJLJSH]=4556;
	FlashispData.Data[FLASHIS_AdrCJLJSH+1]=0;
	FlashispData.Data[FLASHIS_AdrCJGKTJ]=27;
	FlashispData.Data[FLASHIS_AdrCJBQTJ]=21;
	FlashispData.Data[FLASHIS_AdrCJCNT]=10;
	FlashispData.Data[FLASHIS_AdrCJMOD]=0;
	//------------------------------------------------------------------------
	
	//系统维护相关
	//系统维护相关 ----------系统设置
	four.FloatData = 5.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_NO2_NOX] = four.U16Data[0]; //校准维护 系统设置 NO2/NOx  50/10.0/100.0=5.0%
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_NO2_NOX+1] = four.U16Data[1];
	four.FloatData = 0.6;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2] = four.U16Data[0]; //校准维护 系统设置 CO对SO2修正  60/100.0/100.0=0.60%
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2+1] = four.U16Data[1];
	four.FloatData = 3.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_NO2_SO2] = four.U16Data[0];//校准维护 系统设置 NO2对SO2修正  -300/100.0=-3.00
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_NO2_SO2+1] = four.U16Data[1];
	four.FloatData = 0.84;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_PTG_KP] = four.U16Data[0];//校准维护 系统设置 皮托管   84/100.0=0.84
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_PTG_KP+1] = four.U16Data[1];
	four.FloatData = 1.34;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_YQMD] = four.U16Data[0];//校准维护 系统设置 烟气密度  134/100.0=1.34kg/m3
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_YQMD+1] = four.U16Data[1];

	four.FloatData = 1.75;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_ZSXS] = four.U16Data[0];//校准维护 系统设置 折算系数  175/100.0=1.75
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_ZSXS+1] = four.U16Data[1];
	four.FloatData = 9.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_JZYHL] = four.U16Data[0];//校准维护 系统设置  基准痒含量  90/10.0=9.0
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_JZYHL+1] = four.U16Data[1];
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ] = 0x03;        //校准维护 SO2 修正选择  1 CO 2 NO2

    FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1]=62;    //校准维护 系统设置 CO对SO2修正 CO浓度1
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND2]=125;    //校准维护 系统设置 CO对SO2修正 CO浓度2
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND3]=375;    //校准维护 系统设置 CO对SO2修正 CO浓度3
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND4]=625;    //校准维护 系统设置 CO对SO2修正 CO浓度4
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND5]=1250;    //校准维护 系统设置 CO对SO2修正 CO浓度5
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND6]=1875;    //校准维护 系统设置 CO对SO2修正 CO浓度6
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND7]=2500;    //校准维护 系统设置 CO对SO2修正 CO浓度7
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND8]=3750;    //校准维护 系统设置 CO对SO2修正 CO浓度8
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND9]=5000;    //校准维护 系统设置 CO对SO2修正 CO浓度9
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND10]=10000;    //校准维护 系统设置 CO对SO2修正 CO浓度10
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ] = 0x03;        //校准维护 SO2 修正选择  1 CO 2 NO2

	four.FloatData = 0.5;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ1] = four.U16Data[0];///校准维护 系统设置 CO对SO2修正系数1 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ1+1] = four.U16Data[1];
	four.FloatData = 0.5;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ2] = four.U16Data[0];///校准维护 系统设置 CO对SO2修正系数2 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ2+1] = four.U16Data[1];
	four.FloatData = 0.5;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ3] = four.U16Data[0];///校准维护 系统设置 CO对SO2修正系数3 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ3+1] = four.U16Data[1];
	four.FloatData = 0.5;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ4] = four.U16Data[0];///校准维护 系统设置 CO对SO2修正系数4 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ4+1] = four.U16Data[1];
	four.FloatData = 0.5;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ5] = four.U16Data[0];///校准维护 系统设置 CO对SO2修正系数5 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ5+1] = four.U16Data[1];
	four.FloatData = 0.6;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ6] = four.U16Data[0];///校准维护 系统设置 CO对SO2修正系数6 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ6+1] = four.U16Data[1];
	four.FloatData = 0.6;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ7] = four.U16Data[0];///校准维护 系统设置 CO对SO2修正系数7 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ7+1] = four.U16Data[1];
	four.FloatData = 0.6;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ8] = four.U16Data[0];///校准维护 系统设置 CO对SO2修正系数8 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ8+1] = four.U16Data[1];
	four.FloatData = 0.6;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ9] = four.U16Data[0];///校准维护 系统设置 CO对SO2修正系数9 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ9+1] = four.U16Data[1];
	four.FloatData = 0.6;
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ10] = four.U16Data[0];///校准维护 系统设置 CO对SO2修正系数10 
	FlashispData.Data[FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ10+1] = four.U16Data[1];
	 //系统维护-->流量校准
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBLXZ] = 0x01;        ////烟尘倍率选择
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_BZLL] = four.U16Data[0]; //标准流量
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_BZLL+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL01] = four.U16Data[0]; //烟尘标定倍率1
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL01+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL02] = four.U16Data[0];//烟尘标定倍率2
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL02+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL03] = four.U16Data[0];//烟尘标定倍率3
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL03+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL04] = four.U16Data[0];//烟尘标定倍率4
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL04+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL05] = four.U16Data[0];//烟尘标定倍率5
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL05+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL06] = four.U16Data[0];//烟尘标定倍率6
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL06+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL07] = four.U16Data[0];//烟尘标定倍率7
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL07+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL08] = four.U16Data[0];//烟尘标定倍率8
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL08+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL09] = four.U16Data[0];//烟尘标定倍率9
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL09+1] = four.U16Data[1];

//	four.FloatData = 1.0;
//	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL10] = four.U16Data[0];//烟尘标定倍率10
//	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YCBL10+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YQBL01] = four.U16Data[0];//烟气标定倍率1
	FlashispData.Data[FLASHIS_AdrWHJZ_LLJZ_YQBL01+1] = four.U16Data[1];

//系统维护-->浓度校准
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDO2] = four.U16Data[0]; //O2零点值1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDO2+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDSO2] = four.U16Data[0]; //SO2零点值1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDSO2+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDNO] = four.U16Data[0];//NO零点值1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDNO+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDNO2] = four.U16Data[0];//NO2零点值1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDNO2+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDCO] = four.U16Data[0];//CO零点值1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDCO+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDH2S] = four.U16Data[0];//H2S零点值1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDH2S+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDCO2] = four.U16Data[0];//CO2零点值1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_LDCO2+1] = four.U16Data[1];

	//---
		four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLO2_1] = four.U16Data[0]; //O2倍率值1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLO2_1+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLSO2_1] = four.U16Data[0]; //SO2倍率值1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLSO2_1+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO_1] = four.U16Data[0];//NO倍率值1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO_1+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO2_1] = four.U16Data[0];//NO2倍率值1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO2_1+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO_1] = four.U16Data[0];//CO倍率值1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO_1+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLH2S_1] = four.U16Data[0];//H2S倍率值1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLH2S_1+1] = four.U16Data[1];
	four.FloatData = 1.2;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO2_1] = four.U16Data[0];//CO2倍率值1
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO2_1+1] = four.U16Data[1];
	//---
		four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLO2_2] = four.U16Data[0]; //O2倍率值2
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLO2_2+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLSO2_2] = four.U16Data[0]; //SO2倍率值2
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLSO2_2+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO_2] = four.U16Data[0];//NO倍率值2
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO_2+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO2_2] = four.U16Data[0];//NO2倍率值2
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO2_2+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO_2] = four.U16Data[0];//CO倍率值2
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO_2+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLH2S_2] = four.U16Data[0];//H2S倍率值2
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLH2S_2+1] = four.U16Data[1];
	four.FloatData = 1.2;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO2_2] = four.U16Data[0];//CO2倍率值2
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO2_2+1] = four.U16Data[1];
		//---
		four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLO2_3] = four.U16Data[0]; //O2倍率值3
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLO2_3+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLSO2_3] = four.U16Data[0]; //SO2倍率值3
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLSO2_3+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO_3] = four.U16Data[0];//NO倍率值3
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO_3+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO2_3] = four.U16Data[0];//NO2倍率值3
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLNO2_3+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO_3] = four.U16Data[0];//CO倍率值3
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO_3+1] = four.U16Data[1];

	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLH2S_3] = four.U16Data[0];//H2S倍率值3
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLH2S_3+1] = four.U16Data[1];
	four.FloatData = 1.2;
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO2_3] = four.U16Data[0];//CO2倍率值3
	FlashispData.Data[FLASHIS_AdrWHJZ_NDJZ_BLCO2_3+1] = four.U16Data[1];
  //维护校准--->压力校准, modify by alfred, 2020.07.13
		four.FloatData = 500.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_DQY] = four.U16Data[0];//大气压零点值
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_DQY+1] = four.U16Data[1];
			four.FloatData = 500.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_JJY] = four.U16Data[0];//计压零点值
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_JJY+1] = four.U16Data[1];
			four.FloatData = 500.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_JY] = four.U16Data[0];//静压零点值
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_JY+1] = four.U16Data[1];
			four.FloatData = 500.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_DY] = four.U16Data[0];//动压零点值
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_DY+1] = four.U16Data[1];
			four.FloatData = 500.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_CLY] = four.U16Data[0];//尘流压零点值
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_CLY+1] = four.U16Data[1];
			four.FloatData = 500.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_QLY] = four.U16Data[0];//气流压零点值
	FlashispData.Data[FLASHIS_AdrWHJZ_YLJZ_QLY+1] = four.U16Data[1];
  //维护校准--->烟气传感器选择
  FlashispData.Data[FLASHIS_AdrWHJZ_QTCGQXZ]=0x1f1f;
	//维护校准--->温度校准, modify by alfred, 2020.07.13
	four.FloatData = 50.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDGQ] = four.U16Data[0];//干球零点值
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDGQ+1] = four.U16Data[1];	
	four.FloatData = 50.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDYW] = four.U16Data[0];//烟温零点值
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDYW+1] = four.U16Data[1];		
	four.FloatData = 50.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDSQ] = four.U16Data[0];//湿球零点值
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDSQ+1] = four.U16Data[1];
	four.FloatData = 50.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDJW] = four.U16Data[0];//计温零点值
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDJW+1] = four.U16Data[1];
	four.FloatData = 50.0f;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDHW] = four.U16Data[0];//环压零点值
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_LDHW+1] = four.U16Data[1];
	
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLGQ] = four.U16Data[0];//干球倍率值
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLGQ+1] = four.U16Data[1];	
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLYW] = four.U16Data[0];//烟温倍率值
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLYW+1] = four.U16Data[1];	
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLSQ] = four.U16Data[0];//湿球倍率值
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLSQ+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLJW] = four.U16Data[0];//计温倍率值
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLJW+1] = four.U16Data[1];
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLHW] = four.U16Data[0];//环压倍率值
	FlashispData.Data[FLASHIS_AdrWHJZ_WDJZ_BLHW+1] = four.U16Data[1];

  	//维护校准--->温度校准
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrWHJZ_DCJZ_BL] = four.U16Data[0];//电池倍率
	FlashispData.Data[FLASHIS_AdrWHJZ_DCJZ_BL+1] = four.U16Data[1];	  
	//系统维护密码
	FlashispData.Data[FLASHIS_AdrXTWHMM] = 2012;
	//蓝牙相关
	FlashispData.Data[FLASHIS_AdrXTWHLYPDM] = 2218;//蓝牙配对码
	FlashispData.Data[FLASHIS_AdrXTWHAPPLYPDM] = 2218;//蓝牙配对码
	//流量显示设置
	FlashispData.Data[FLASHIS_AdrXTWHLLXS] = 'R';
	//电压倍率
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrXTWHDYBL] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrXTWHDYBL+1] = four.U16Data[1];
	//采样流量设置
	four.FloatData = 1.0;
	FlashispData.Data[FLASHIS_AdrCYSZALCYLL] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrCYSZALCYLL+1] = four.U16Data[1];
  FlashispData.Data[FLASHIS_AdrCYSZBLCYLL] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrCYSZBLCYLL+1] = four.U16Data[1];
	FlashispData.Data[FLASHIS_AdrCYSZCLCYLL] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrCYSZCLCYLL+1] = four.U16Data[1];
	//风扇开关
	FlashispData.Data[FLASHIS_AdrXTWHFSKG] = 0;
	//本机号码
	four.U32Data = 1234567;
	FlashispData.Data[FLASHIS_AdrXTWHBJHMQD] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrXTWHBJHMQD+1] = four.U16Data[1];
	four.U32Data = 123456;
	FlashispData.Data[FLASHIS_AdrXTWHBJHMHD] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrXTWHBJHMHD+1] = four.U16Data[1];
	//------------------------------------------------------------------------
	//系统运行时间、传感器安装时间相关
	//------------------------------------------------------------------------
	//运行时间信息
	four.U32Data = 120;
	FlashispData.Data[FLASHIS_AdrYQYXSJ] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrYQYXSJ+1] = four.U16Data[1];
	four.U32Data = 5678;
	FlashispData.Data[FLASHIS_AdrCBYXSJ] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrCBYXSJ+1] = four.U16Data[1];
//------------------------------------------------------------------------------------
//传感器安装时间
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
// 量程初始化
  //氧气0~24
	 four.U32Data=24;
	 FlashispData.Data[FLASHIS_AdrO2LQ]=four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrO2LQ+1]=four.U16Data[1];
	 //二氧化硫 0~5700
	 four.U32Data=5700;
	 FlashispData.Data[FLASHIS_AdrSO2LQ]=four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrSO2LQ+1]=four.U16Data[1];
	  //一氧化氮0~1300
	 four.U32Data=1300;
	 FlashispData.Data[FLASHIS_AdrNOLQ]=four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrNOLQ+1]=four.U16Data[1];
	 //二氧化氮 0~200
	 four.U32Data=200;
	 FlashispData.Data[FLASHIS_AdrNO2LQ]=four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrNO2LQ+1]=four.U16Data[1];
	 
	 //一氧化碳 0~5000
	 four.U32Data=5000;
	 FlashispData.Data[FLASHIS_AdrCOLQ]=four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrCOLQ+1]=four.U16Data[1];
	 
	  //硫化氢0~200
	 four.U32Data=200;
	 FlashispData.Data[FLASHIS_AdrH2SLQ]=four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrH2SLQ+2]=four.U16Data[1];
	 
	 //二氧化碳
	 four.U32Data=20;
	 FlashispData.Data[FLASHIS_AdrCO2LQ]=four.U16Data[0];
	 FlashispData.Data[FLASHIS_AdrCO2LQ+1]=four.U16Data[1];

	 
	//超级维护相关
	//超级维护密码
	FlashispData.Data[FLASHIS_AdrCJWHMM] = 8088;
	//仪器编号
	four.U32Data = 12345678;
	FlashispData.Data[FLASHIS_AdrCJWHYQBH] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrCJWHYQBH+1] = four.U16Data[1];
	//软件版本号
	four.FloatData = 1.02;
	FlashispData.Data[FLASHIS_AdrCJWHRJBB] = four.U16Data[0];
	FlashispData.Data[FLASHIS_AdrCJWHRJBB+1] = four.U16Data[1];

	
	//备份区域初始化
	if(select)
	Flashisp_Backup();
	
	//出厂设置标志位
	FlashispData.Data[FLASHIS_AdrStoreFactory] = FLASHIS_FACTORY;
	#ifdef FLASH_USE_FM24CL64
	FM24CL64_WriteData();
	#else
	Flashisp_StoreData();//修改数据
	#endif

	Screen_GetData()->gSysConfigWaringBits.BitField.m_factorySet = 1;
	printf("系统已恢复出厂设置\r\n");
}
//---
/*
	函数名：void Flashisp_Backup(void)
	描述：	该函数为备份需要备份的相关区域
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
	函数名：void Flashisp_Restore(void)
	描述：	该函数为还原备份的数据到相关区域
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
	Slave_ReadFlashData();//更新实时参数
}
//---
//-------------------------数据读取、存储----------------------------
//---
/*
	函数名：void Flashisp_StoreData(void)
	描述：该函数为存储该系统所有Flash数据
	说明：解锁
				读取数据
				擦除数据
				修改数据
				保存数据
				重锁
*/
void Flashisp_StoreData(void)
{
	u16 i;
	u32 adr = FLASHIS_AdrStoreStart;//首地址
	FLASH_Unlock();  //解锁FLASH编程擦除控制器
	FLASH_DataCacheCmd(DISABLE);
	FLASH_EraseSector(STMFLASH_GetFlashSector(FLASHIS_AdrStoreStart),VoltageRange_3);//VCC=2.7~3.6V之间!!
	for(i=0;i<FLASHIS_AdrEnd;i++)
	{
		FLASH_ProgramHalfWord(adr,FlashispData.Data[i]);
		adr += 2;
	}
	FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	FLASH_Lock();    //锁定FLASH编程擦除控制器
	FlashispData.Flag = 0;
//	printf("修改数据\r\n");
}
//---
//获取某个地址所在的flash扇区
//addr:flash地址
//返回值:0~11,即addr所在的扇区
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
//-------------------------调试函数----------------------------
//---
/*
	函数名： void Flashisp_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Flashisp文件的ASCII调试文件
						:000|01|xxx
*/
void Flashisp_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:
		{//读取Flash表长度
			
			break;
		}
		case 101:
		{//读取A路流压采样值跟实际值
//			FourBytes four;
			u16* Fp= Flashisp_GetData()->Data;
//			four.U16Data[0] = Fp[FLASHIS_AdrYLBDALCYCYZ];
//			four.U16Data[1] = Fp[FLASHIS_AdrYLBDALCYCYZ+1];
//			printf("A路流压采样值为：%fPa\r\n",four.FloatData*1000);
//			four.U16Data[0] = Fp[FLASHIS_AdrYLBDALCYSJZ];
//			four.U16Data[1] = Fp[FLASHIS_AdrYLBDALCYSJZ+1];
//			printf("A路流压实际值为：%fPa\r\n",four.FloatData*1000);
			break;
		}
		case 201:
		{//读取系统设置相关参数
			FourBytes four;
			printf("维护密码：%04u\r\n",Flashisp_GetData()->Data[FLASHIS_AdrXTWHMM]);
			printf("蓝牙配对码：%04u\r\n",Flashisp_GetData()->Data[FLASHIS_AdrXTWHLYPDM]);
			printf("流量显示：%c\r\n",(char)Flashisp_GetData()->Data[FLASHIS_AdrXTWHLLXS]);
			four.U16Data[0] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHDYBL];
			four.U16Data[1] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHDYBL+1];
			printf("电压倍率：%f\r\n",four.FloatData);
			printf("风扇开关：%u\r\n",Flashisp_GetData()->Data[FLASHIS_AdrXTWHFSKG]);
			four.U16Data[0] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHBJHMQD];
			four.U16Data[1] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHBJHMQD+1];
			printf("本机号码前段：%u\r\n",four.U32Data);
			four.U16Data[0] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHBJHMHD];
			four.U16Data[1] = Flashisp_GetData()->Data[FLASHIS_AdrXTWHBJHMHD+1];
			printf("本机号码后段：%u\r\n",four.U32Data);
			break;
		}
		case 202:
		{//读取掉电信息
			FourBytes four;
			struct tm tf;
			four.U16Data[0] = Flashisp_GetData()->Data[FLASHIS_AdrDDTime];
			four.U16Data[1] = Flashisp_GetData()->Data[FLASHIS_AdrDDTime+1];
			printf("掉电信息：%u\r\n",four.U32Data);
			tf = *localtime(&four.U32Data);//掉电时间
			printf("掉电时间：%u年%02u月%02u日 %02u时%02u分%02u秒\r\n",tf.tm_year+SCREEN_YearBase,tf.tm_mon,tf.tm_mday,tf.tm_hour,tf.tm_min,tf.tm_sec);
			break;
		}
		case 111:
		{//读取密码参数
			break;
		}
		case 901:
		{//查看Flash缓冲区长度
			printf("Flash缓冲区长度：%u\r\n",FLASHIS_AdrEnd);
			printf("FLASHIS_AdrStoreFactory=0x%X\r\n",FlashispData.Data[FLASHIS_AdrStoreFactory]);
			break;
		}
		case 902:
		{//恢复出厂设置
			Flashisp_FactoryReset(1);
			break;
		}
		case 903:
		{//读取所有Flash数据
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
		{//备份数据
			Flashisp_Backup();
			printf("数据已备份\r\n");
			break;
		}
		case 905:
		{//恢复数据
			Flashisp_Restore();
			printf("数据已恢复\r\n");
			break;
		}
		case 906:
		{

			
     
	}
}
}
//---



