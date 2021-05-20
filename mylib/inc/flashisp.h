/*
	flashisp.c库函数
*/

#ifndef __FLASHISP_H
#define __FLASHISP_H

#include "stm32f4xx.h"

#define FLASH_SIZE 									1024//所选MCU的FLASH容量大小(单位为K)
#if FLASH_SIZE<256
  #define SECTOR_SIZE           		1024    //字节
#else 
  #define SECTOR_SIZE           		2048    //字节
#endif
//--------------------------------------
//常量宏定义
#define FLASH_USE_FM24CL64
//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//扇区0起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//扇区1起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//扇区2起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//扇区4起始地址, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//扇区5起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//扇区6起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//扇区7起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//扇区8起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//扇区10起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//扇区11起始地址,128 Kbytes  

#define FLASHIS_FACTORY								0XAAAA	//回复出厂设置码


//存储基址
#define FLASHIS_AdrStoreStart 				0x080E0000+1024*10//FLASH_BASE+500*SECTOR_SIZE			//起始基址
//存储基址
//#define FLASHIS_AdrStoreFluxA 				FLASH_BASE+501*SECTOR_SIZE	
//#define FLASHIS_AdrStoreFluxB 				FLASH_BASE+502*SECTOR_SIZE
//#define FLASHIS_AdrStoreFluxT 				FLASH_BASE+503*SECTOR_SIZE


//地址，偏移地址，一个单位为两个字节
#define FLASHIS_AdrSTART							(u16)0x0000
//系统设置
#define FLASHIS_AdrXTSZHJWD						FLASHIS_AdrSTART				//系统设置环境温度
#define FLASHIS_AdrXTSZHJWDKG					FLASHIS_AdrXTSZHJWD+2		//系统设置环境温度开关
#define FLASHIS_AdrXTSZDQY						FLASHIS_AdrXTSZHJWDKG+1	//系统设置大气压
#define FLASHIS_AdrXTSZCYDZ						FLASHIS_AdrXTSZDQY+2	  //系统设置采样地址 //15个字
#define FLASHIS_AdrXTSZZDPSJG					FLASHIS_AdrXTSZCYDZ+15	//系统设置排水间隔
#define FLASHIS_AdrXTSZZDPSSC					FLASHIS_AdrXTSZZDPSJG+1	//系统设置排水时长
#define FLASHIS_AdrXTSZYQCLSJ					FLASHIS_AdrXTSZZDPSSC+1	//系统设烟气测量时间
#define FLASHIS_AdrXTSZYQCLCS					FLASHIS_AdrXTSZYQCLSJ+1	//系统设烟气测量次数
#define FLASHIS_AdrXTSZCHSLSJ					FLASHIS_AdrXTSZYQCLCS+1	//系统设测含湿量时间
#define FLASHIS_AdrXTSZBOX  					FLASHIS_AdrXTSZCHSLSJ+1	//  //0位 大气压 1位防倒吸 2位蓝牙开关 3位自动排水 4采样类型 5 烟气单位 6烟尘采样类型
//烟道布点设置
#define FLASHIS_AdrYDBDLX							FLASHIS_AdrXTSZBOX+1		//烟道布点类型  1=圆形  2=方形 3=其他类型

#define FLASHIS_AdrYDBDYXNJ						FLASHIS_AdrYDBDLX+1		     //烟道布点圆形内径D
#define FLASHIS_AdrYDBD_MJ						FLASHIS_AdrYDBDYXNJ+2		   //烟道布点圆形面积
#define FLASHIS_AdrYDBD_GT						FLASHIS_AdrYDBD_MJ+2		   //烟道布点圆形管套
#define FLASHIS_AdrYDBDYXHS						FLASHIS_AdrYDBD_GT+2		   //烟道布点圆形环数
#define FLASHIS_AdrYDBDYXKS						FLASHIS_AdrYDBDYXHS+1		   //烟道布点圆形孔数
#define FLASHIS_AdrYDBD_CYDS					FLASHIS_AdrYDBDYXKS+1		   //烟道布点圆形采样点数

#define FLASHIS_AdrYDBDFXBA					FLASHIS_AdrYDBD_CYDS+1		     //烟道布点方形一边A
#define FLASHIS_AdrYDBDFXBB					FLASHIS_AdrYDBDFXBA+2		   //烟道布点方形一边B
//#define FLASHIS_AdrYDBDFXMJ					FLASHIS_AdrYDBDFXBB+2		   //烟道布点方形面积
//#define FLASHIS_AdrYDBDFXGT					FLASHIS_AdrYDBDFXMJ+2		   //烟道布点方形管套
#define FLASHIS_AdrYDBDFXKS					FLASHIS_AdrYDBDFXBB+2		   //烟道布点方形孔数
#define FLASHIS_AdrYDBDFXDKCD				FLASHIS_AdrYDBDFXKS+1		   //烟道布点方形单孔测点
//#define FLASHIS_AdrYDBDFXCYDS				FLASHIS_AdrYDBDFXDKCD+1		   //烟道布点方形采样点数
#define FLASHIS_AdrYDBDCDJL1				FLASHIS_AdrYDBDFXDKCD+1		   //烟道布点方形单孔测点距离1
#define FLASHIS_AdrYDBDCDJL2				FLASHIS_AdrYDBDCDJL1+2		   //烟道布点方形单孔测点距离2
#define FLASHIS_AdrYDBDCDJL3				FLASHIS_AdrYDBDCDJL2+2		   //烟道布点方形单孔测点距离3
#define FLASHIS_AdrYDBDCDJL4				FLASHIS_AdrYDBDCDJL3+2	   //烟道布点方形单孔测点距离4
#define FLASHIS_AdrYDBDCDJL5				FLASHIS_AdrYDBDCDJL4+2		   //烟道布点方形单孔测点距离5
#define FLASHIS_AdrYDBDCDJL6				FLASHIS_AdrYDBDCDJL5+2		   //烟道布点方形单孔测点距离6
#define FLASHIS_AdrYDBDCDJL7				FLASHIS_AdrYDBDCDJL6+2		   //烟道布点方形单孔测点距离7
#define FLASHIS_AdrYDBDCDJL8				FLASHIS_AdrYDBDCDJL7+2		   //烟道布点方形单孔测点距离8
#define FLASHIS_AdrYDBDCDJL9				FLASHIS_AdrYDBDCDJL8+2		   //烟道布点方形单孔测点距离9
#define FLASHIS_AdrYDBDCDJL10				FLASHIS_AdrYDBDCDJL9+2		   //烟道布点方形单孔测点距离10
//#define FLASHIS_AdrYDBDQTMJ				FLASHIS_AdrYDBDFXCYDS+1		   //烟道布点其他方式面积
//#define FLASHIS_AdrYDBDQTCYDS				FLASHIS_AdrYDBDQTMJ+2		   //烟道布点采样点数
//
//工况预测  压力调0
#define FLASHIS_AdrGKYC_YLTL_YQDY				    FLASHIS_AdrYDBDCDJL10+1		      //压力调零，烟气动压
#define FLASHIS_AdrGKYC_YLTL_YQJY				    FLASHIS_AdrGKYC_YLTL_YQDY+1		   //压力调零，烟气静压
#define FLASHIS_AdrGKYC_YLTL_JQYL				    FLASHIS_AdrGKYC_YLTL_YQJY+2		   //压力调零，计前压力
#define FLASHIS_AdrGKYC_YLTL_CLY				    FLASHIS_AdrGKYC_YLTL_JQYL+2		   //压力调零，尘流压压
#define FLASHIS_AdrGKYC_YLTL_QLY				    FLASHIS_AdrGKYC_YLTL_CLY+1		   //压力调零，气流压压
//工况预测 烟温测量
#define FLASHIS_AdrGKYC_YWCL_FS				        FLASHIS_AdrGKYC_YLTL_QLY+1		   //烟温方式 0 测量 1输入
#define FLASHIS_AdrGKYC_YWCL_SRYW				    FLASHIS_AdrGKYC_YWCL_FS+1		   //输入烟温

////工况预测 烟气流速

#define FLASHIS_AdrGKYC_YQLS_YQLS				      FLASHIS_AdrGKYC_YWCL_SRYW+2		   //平均流速  //20200601

#define FLASHIS_AdrGKYC_CPJZ_PJDY				      FLASHIS_AdrGKYC_YQLS_YQLS+2		   //平均动压      ----
#define FLASHIS_AdrGKYC_CPJZ_PJJY				      FLASHIS_AdrGKYC_CPJZ_PJDY+2		   //平均静压      ----
#define FLASHIS_AdrGKYC_CPJZ_PJQY				      FLASHIS_AdrGKYC_CPJZ_PJJY+2		   //平均全压      ----
#define FLASHIS_AdrGKYC_CPJZ_PJYW				      FLASHIS_AdrGKYC_CPJZ_PJQY+2		   //平均烟温      ----
#define FLASHIS_AdrGKYC_CPJZ_YGLL				      FLASHIS_AdrGKYC_CPJZ_PJYW+2		   //烟气流量      ----
#define FLASHIS_AdrGKYC_CPJZ_BGLL				      FLASHIS_AdrGKYC_CPJZ_YGLL+2		   //标杆流量
//#define FLASHIS_AdrGKYC_CPJZ_DXLL                   FLASHIS_AdrGKYC_CPJZ_BGLL+2        //等速流量    ，计算采样嘴用

//含湿量
//#define FLASHIS_AdrCHSL_LX				      FLASHIS_AdrGKYC_YQLS_YQLL+2		   //测含湿量类型
//#define FLASHIS_AdrCHSL_CLZ				      FLASHIS_AdrCHSL_LX+1		         //测量含湿量
#define FLASHIS_AdrCHSL_HSL				          FLASHIS_AdrGKYC_CPJZ_BGLL+2		      //含湿量
//测量烟气
#define FLASHIS_AdrCLYQ_CLFS				      FLASHIS_AdrCHSL_HSL+2		         //测量方式 0手动 1计时
#define FLASHIS_AdrCLYQ_JSSC				      FLASHIS_AdrCLYQ_CLFS+1		         //计时时长
//测量烟气 -烟气校准
//#define FLASHIS_AdrCLYQ_CJO2				      FLASHIS_AdrCLYQ_JSSC+1		         //O2校准值
//#define FLASHIS_AdrCLYQ_CJNO				      FLASHIS_AdrCLYQ_CJO2+2		         //NO校准值
//#define FLASHIS_AdrCLYQ_CJNO2				      FLASHIS_AdrCLYQ_CJNO+2		         //NO2校准值
//#define FLASHIS_AdrCLYQ_CJNOX				      FLASHIS_AdrCLYQ_CJNO2+2		         //NOX校准值
//#define FLASHIS_AdrCLYQ_CJSO2				      FLASHIS_AdrCLYQ_CJNOX+2		         //SO2校准值
//#define FLASHIS_AdrCLYQ_CJCO				      FLASHIS_AdrCLYQ_CJSO2+2		         //CO校准值
//#define FLASHIS_AdrCLYQ_CJH2S				      FLASHIS_AdrCLYQ_CJCO+2		         //H2S校准值
//#define FLASHIS_AdrCLYQ_CJCO2				      FLASHIS_AdrCLYQ_CJH2S+2		         //CO2校准值
//测量烟气 -测量中单位类型
#define FLASHIS_AdrCLYQ_DWLX				      FLASHIS_AdrCLYQ_JSSC+1		         //单位类型
//测量烟气 -文件查询 - 计算系数
#define FLASHIS_AdrCLYQ_WJCX_JS_ZSXS				      FLASHIS_AdrCLYQ_DWLX+1		         //折算系数
#define FLASHIS_AdrCLYQ_WJCX_JS_JZYHL				      FLASHIS_AdrCLYQ_WJCX_JS_ZSXS+2		 //基准氧气含量
#define FLASHIS_AdrCLYQ_WJCX_JS_FHXS				      FLASHIS_AdrCLYQ_WJCX_JS_JZYHL+2		 //负荷系数
//测量烟气 -文件查询 - 打印选项
#define FLASHIS_AdrCLYQ_WJCX_DYXX				      FLASHIS_AdrCLYQ_WJCX_JS_FHXS+2		    //打印选项
//测量烟气 -文件查询 - 分钟数据
#define FLASHIS_AdrCLYQ_WJCX_FZSJ_PJJS			      FLASHIS_AdrCLYQ_WJCX_DYXX+2		    //参与平均计算
//烟尘采样吧 -采样设置
#define FLASHIS_AdrYCCJ_CYSE_CZZJ				          FLASHIS_AdrCLYQ_WJCX_FZSJ_PJJS+1		   //采嘴直径 与工况预测的采嘴直径一样
#define FLASHIS_AdrYCCJ_CYSE_CYFS				          FLASHIS_AdrYCCJ_CYSE_CZZJ+2		   //采样方式 0恒流  1等速
#define FLASHIS_AdrYCCJ_CYSE_CYLS				          FLASHIS_AdrYCCJ_CYSE_CYFS+1		   //采样流量
#define FLASHIS_AdrYCCJ_CYSE_LTH				          FLASHIS_AdrYCCJ_CYSE_CYLS+2		   //滤桶号
#define FLASHIS_AdrYCCJ_CYSE_DDCS				          FLASHIS_AdrYCCJ_CYSE_LTH+2		   //单点采时
//文件查询 -烟尘打印选项
#define FLASHIS_AdrYCCJ_YCWJCX_DYXX				      FLASHIS_AdrYCCJ_CYSE_DDCS+1		    //打印选项
//文件查询 - 计算/油烟
#define FFLASHIS_AdrYCCJ_YCWJCX_JSRYND            FLASHIS_AdrYCCJ_YCWJCX_DYXX+3        //文件查询下 计算 溶液浓度
#define FFLASHIS_AdrYCCJ_YCWJCX_JSDRTJ            FFLASHIS_AdrYCCJ_YCWJCX_JSRYND+2     //文件查询下 计算 定溶体积
#define FFLASHIS_AdrYCCJ_YCWJCX_JSZTSM            FFLASHIS_AdrYCCJ_YCWJCX_JSDRTJ+2     //文件查询下 计算 灶头数目

//文件查询 - 计算/烟尘
#define FFLASHIS_AdrYCCJ_YCWJCX_JSYCZL            FFLASHIS_AdrYCCJ_YCWJCX_JSZTSM+1        //文件查询下 计算 烟尘重量
#define FFLASHIS_AdrYCCJ_YCWJCX_JSHYL             FFLASHIS_AdrYCCJ_YCWJCX_JSYCZL+2         //文件查询下 计算 氧气含量
#define FFLASHIS_AdrYCCJ_YCWJCX_JSFHXS            FFLASHIS_AdrYCCJ_YCWJCX_JSHYL+2         //文件查询下 计算 负荷系数
#define FFLASHIS_AdrYCCJ_YCWJCX_JSZSXS            FFLASHIS_AdrYCCJ_YCWJCX_JSFHXS+2        //文件查询下 计算 折算系数
#define FFLASHIS_AdrYCCJ_YCWJCX_JSJZYHL           FFLASHIS_AdrYCCJ_YCWJCX_JSZSXS+2       //文件查询下 计算 基准氧气含量
//-----------------------------------------------------------------------------------
//20200601
//掉电相关设计
#define FLASHIS_AdrCJKSSK				      FFLASHIS_AdrYCCJ_YCWJCX_JSJZYHL+2	//采集开始时刻	单位秒，time_t格式
#define FLASHIS_AdrCJLJSH				      FLASHIS_AdrCJKSSK+2         //掉电前采样计时 u32
#define FLASHIS_AdrCJGKTJ				      FLASHIS_AdrCJLJSH+2         //掉电前工况体积
#define FLASHIS_AdrCJBQTJ				      FLASHIS_AdrCJGKTJ+2         //掉电前标况体积
#define FLASHIS_AdrCJCNT		      	      FLASHIS_AdrCJBQTJ+2         //掉电前采集到第几个点
#define FLASHIS_AdrCJMOD			      	  FLASHIS_AdrCJCNT+1         //掉电前运行状态启动,暂停
#define FLASHIS_AdrCJWJH                      FLASHIS_AdrCJMOD+1         //烟尘丢电前文件号
//系统维护相关 ----------
#define FLASHIS_AdrBDStart						        FLASHIS_AdrCJWJH+2//标定起始值（用于备份用）
#define FLASHIS_AdrWHJZ_XTSZ_NO2_NOX			      	FLASHIS_AdrCJWJH+2                     //校准维护 系统设置 NO2/NOx
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2			      	  FLASHIS_AdrWHJZ_XTSZ_NO2_NOX+2            //校准维护 系统设置 CO对SO2修正
#define FLASHIS_AdrWHJZ_XTSZ_NO2_SO2			      	FLASHIS_AdrWHJZ_XTSZ_CO_SO2+2          //校准维护 系统设置 NO2对SO2修正
#define FLASHIS_AdrWHJZ_XTSZ_PTG_KP			      	  FLASHIS_AdrWHJZ_XTSZ_NO2_SO2+2            //校准维护 系统设置 皮托管
#define FLASHIS_AdrWHJZ_XTSZ_YQMD			      	    FLASHIS_AdrWHJZ_XTSZ_PTG_KP+2              //校准维护 系统设置 烟气密度
#define FLASHIS_AdrWHJZ_XTSZ_ZSXS			      	    FLASHIS_AdrWHJZ_XTSZ_YQMD+2                //校准维护 系统设置 折算系数
#define FLASHIS_AdrWHJZ_XTSZ_JZYHL			      	  FLASHIS_AdrWHJZ_XTSZ_ZSXS+2                  //校准维护 基准痒含量
#define FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ			      	FLASHIS_AdrWHJZ_XTSZ_JZYHL+2               //校准维护 SO2 修正选择  0bit CO-SO2 bit1 NO-SO2
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1			    FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ+1            //校准维护 系统设置 CO对SO2修正 CO浓度1
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND2			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1+1            //校准维护 系统设置 CO对SO2修正 CO浓度2
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND3			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND2+1            //校准维护 系统设置 CO对SO2修正 CO浓度3
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND4			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND3+1            //校准维护 系统设置 CO对SO2修正 CO浓度4
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND5			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND4+1            //校准维护 系统设置 CO对SO2修正 CO浓度5
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND6			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND5+1            //校准维护 系统设置 CO对SO2修正 CO浓度6
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND7		      FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND6+1               //校准维护 系统设置 CO对SO2修正 CO浓度7
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND8			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND7+1             //校准维护 系统设置 CO对SO2修正 CO浓度8
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND9			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND8+1              //校准维护 系统设置 CO对SO2修正 CO浓度9
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND10			  FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND9+1            //校准维护 系统设置 CO对SO2修正 CO浓度10
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ1			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND10+1            //校准维护 系统设置 CO对SO2修正系数1
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ2			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ1+2            //校准维护 系统设置 CO对SO2修正系数2
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ3			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ2+2            //校准维护 系统设置 CO对SO2修正系数3
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ4			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ3+2            //校准维护 系统设置 CO对SO2修正系数4
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ5			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ4+2            //校准维护 系统设置 CO对SO2修正系数5
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ6			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ5+2            //校准维护 系统设置 CO对SO2修正系数6
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ7			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ6+2            //校准维护 系统设置 CO对SO2修正系数7
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ8			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ7+2            //校准维护 系统设置 CO对SO2修正系数8
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ9			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ8+2            //校准维护 系统设置 CO对SO2修正系数9
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ10			  FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ9+2            //校准维护 系统设置 CO对SO2修正系数10



//维护校准-->流量校准
#define FLASHIS_AdrWHJZ_LLJZ_YCBLXZ					FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ10+2//烟尘倍率选择
#define FLASHIS_AdrWHJZ_LLJZ_BZLL					FLASHIS_AdrWHJZ_LLJZ_YCBLXZ+1   //标准流量
#define FLASHIS_AdrWHJZ_LLJZ_YCBL01					FLASHIS_AdrWHJZ_LLJZ_BZLL+2 //烟尘标定倍率1
#define FLASHIS_AdrWHJZ_LLJZ_YCBL02					FLASHIS_AdrWHJZ_LLJZ_YCBL01+2//烟尘标定倍率2
#define FLASHIS_AdrWHJZ_LLJZ_YCBL03					FLASHIS_AdrWHJZ_LLJZ_YCBL02+2//烟尘标定倍率3
#define FLASHIS_AdrWHJZ_LLJZ_YCBL04					FLASHIS_AdrWHJZ_LLJZ_YCBL03+2//烟尘标定倍率4
#define FLASHIS_AdrWHJZ_LLJZ_YCBL05					FLASHIS_AdrWHJZ_LLJZ_YCBL04+2//烟尘标定倍率5
#define FLASHIS_AdrWHJZ_LLJZ_YCBL06					FLASHIS_AdrWHJZ_LLJZ_YCBL05+2//烟尘标定倍率6
#define FLASHIS_AdrWHJZ_LLJZ_YCBL07					FLASHIS_AdrWHJZ_LLJZ_YCBL06+2//烟尘标定倍率7
#define FLASHIS_AdrWHJZ_LLJZ_YCBL08					FLASHIS_AdrWHJZ_LLJZ_YCBL07+2//烟尘标定倍率8
#define FLASHIS_AdrWHJZ_LLJZ_YCBL09					FLASHIS_AdrWHJZ_LLJZ_YCBL08+2//烟尘标定倍率9
//#define FLASHIS_AdrWHJZ_LLJZ_YCBL10					FLASHIS_AdrWHJZ_LLJZ_YCBL09+2//烟尘标定倍率10
#define FLASHIS_AdrWHJZ_LLJZ_YQBL01					FLASHIS_AdrWHJZ_LLJZ_YCBL09+2//烟气标定倍率1
//维护校准--->浓度矫正
#define FLASHIS_AdrWHJZ_NDJZ_LDO2				      FLASHIS_AdrWHJZ_LLJZ_YQBL01+2		         //O2零点值
#define FLASHIS_AdrWHJZ_NDJZ_LDSO2				      FLASHIS_AdrWHJZ_NDJZ_LDO2+2		         //SO2零点值
#define FLASHIS_AdrWHJZ_NDJZ_LDNO				      FLASHIS_AdrWHJZ_NDJZ_LDSO2+2		         //NO零点值
#define FLASHIS_AdrWHJZ_NDJZ_LDNO2				      FLASHIS_AdrWHJZ_NDJZ_LDNO+2		         //NO2零点值
#define FLASHIS_AdrWHJZ_NDJZ_LDCO				      FLASHIS_AdrWHJZ_NDJZ_LDNO2+2		         //CO零点值
#define FLASHIS_AdrWHJZ_NDJZ_LDH2S				      FLASHIS_AdrWHJZ_NDJZ_LDCO+2		         //H2S零点值
#define FLASHIS_AdrWHJZ_NDJZ_LDCO2				      FLASHIS_AdrWHJZ_NDJZ_LDH2S+2		         //CO2零点值

#define FLASHIS_AdrWHJZ_NDJZ_BLO2_1				      FLASHIS_AdrWHJZ_NDJZ_LDCO2+2		             //O2倍率值1    中
#define FLASHIS_AdrWHJZ_NDJZ_BLSO2_1				      FLASHIS_AdrWHJZ_NDJZ_BLO2_1+2		         //SO2倍率值1
#define FLASHIS_AdrWHJZ_NDJZ_BLNO_1				      FLASHIS_AdrWHJZ_NDJZ_BLSO2_1+2		         //NO倍率值1
#define FLASHIS_AdrWHJZ_NDJZ_BLNO2_1				      FLASHIS_AdrWHJZ_NDJZ_BLNO_1+2		         //NO2倍率值1

#define FLASHIS_AdrWHJZ_NDJZ_BLCO_1				      FLASHIS_AdrWHJZ_NDJZ_BLNO2_1+2		         //CO倍率值1
#define FLASHIS_AdrWHJZ_NDJZ_BLH2S_1				      FLASHIS_AdrWHJZ_NDJZ_BLCO_1+2		         //H2S倍率值1
#define FLASHIS_AdrWHJZ_NDJZ_BLCO2_1				      FLASHIS_AdrWHJZ_NDJZ_BLH2S_1+2		     //CO2倍率值1

#define FLASHIS_AdrWHJZ_NDJZ_BLO2_2				      FLASHIS_AdrWHJZ_NDJZ_BLCO2_1+2		         //O2倍率值2   低
#define FLASHIS_AdrWHJZ_NDJZ_BLSO2_2				      FLASHIS_AdrWHJZ_NDJZ_BLO2_2+2		         //SO2倍率值2
#define FLASHIS_AdrWHJZ_NDJZ_BLNO_2				      FLASHIS_AdrWHJZ_NDJZ_BLSO2_2+2		         //NO倍率值2
#define FLASHIS_AdrWHJZ_NDJZ_BLNO2_2				      FLASHIS_AdrWHJZ_NDJZ_BLNO_2+2		         //NO2倍率值2

#define FLASHIS_AdrWHJZ_NDJZ_BLCO_2				      FLASHIS_AdrWHJZ_NDJZ_BLNO2_2+2		         //CO倍率值2
#define FLASHIS_AdrWHJZ_NDJZ_BLH2S_2				      FLASHIS_AdrWHJZ_NDJZ_BLCO_2+2		         //H2S倍率值2
#define FLASHIS_AdrWHJZ_NDJZ_BLCO2_2				      FLASHIS_AdrWHJZ_NDJZ_BLH2S_2+2		     //CO2倍率值2

#define FLASHIS_AdrWHJZ_NDJZ_BLO2_3				      FLASHIS_AdrWHJZ_NDJZ_BLCO2_2+2		         //O2倍率值3   高

#define FLASHIS_AdrWHJZ_NDJZ_BLSO2_3				      FLASHIS_AdrWHJZ_NDJZ_BLO2_3+2		         //SO2倍率值3
#define FLASHIS_AdrWHJZ_NDJZ_BLNO_3				      FLASHIS_AdrWHJZ_NDJZ_BLSO2_3+2		         //NO倍率值3
#define FLASHIS_AdrWHJZ_NDJZ_BLNO2_3				      FLASHIS_AdrWHJZ_NDJZ_BLNO_3+2		         //NO2倍率值3

#define FLASHIS_AdrWHJZ_NDJZ_BLCO_3				      FLASHIS_AdrWHJZ_NDJZ_BLNO2_3+2		         //CO倍率值3
#define FLASHIS_AdrWHJZ_NDJZ_BLH2S_3				      FLASHIS_AdrWHJZ_NDJZ_BLCO_3+2		         //H2S倍率值3
#define FLASHIS_AdrWHJZ_NDJZ_BLCO2_3				      FLASHIS_AdrWHJZ_NDJZ_BLH2S_3+2		         //CO2倍率值3

  //--------------------------------------------------------------20200519
#define FLASHIS_AdrWHJZ_NDJZ_BQLDO2_1				      FLASHIS_AdrWHJZ_NDJZ_BLCO2_3+2		         //O2标气浓度值1      中
#define FLASHIS_AdrWHJZ_NDJZ_BQLDSO2_1				      FLASHIS_AdrWHJZ_NDJZ_BQLDO2_1+2		         //SO2标气浓度值1
#define FLASHIS_AdrWHJZ_NDJZ_BQLDNO_1				      FLASHIS_AdrWHJZ_NDJZ_BQLDSO2_1+2		         //NO标气浓度值1
#define FLASHIS_AdrWHJZ_NDJZ_BQLDNO2_1				      FLASHIS_AdrWHJZ_NDJZ_BQLDNO_1+2		         //NO2标气浓度值1

#define FLASHIS_AdrWHJZ_NDJZ_BQLDCO_1				      FLASHIS_AdrWHJZ_NDJZ_BQLDNO2_1+2		         //CO标气浓度值1
#define FLASHIS_AdrWHJZ_NDJZ_BQLDH2S_1				      FLASHIS_AdrWHJZ_NDJZ_BQLDCO_1+2		         //H2S标气浓度值1
#define FLASHIS_AdrWHJZ_NDJZ_BQLDCO2_1				      FLASHIS_AdrWHJZ_NDJZ_BQLDH2S_1+2		         //CO2标气浓度值1

#define FLASHIS_AdrWHJZ_NDJZ_BQLDO2_2				      FLASHIS_AdrWHJZ_NDJZ_BQLDCO2_1+2		         //O2标气浓度值2     低
#define FLASHIS_AdrWHJZ_NDJZ_BQLDSO2_2				      FLASHIS_AdrWHJZ_NDJZ_BQLDO2_2+2		         //SO2标气浓度值2
#define FLASHIS_AdrWHJZ_NDJZ_BQLDNO_2				      FLASHIS_AdrWHJZ_NDJZ_BQLDSO2_2+2		         //NO标气浓度值2
#define FLASHIS_AdrWHJZ_NDJZ_BQLDNO2_2				      FLASHIS_AdrWHJZ_NDJZ_BQLDNO_2+2		         //NO2标气浓度值2

#define FLASHIS_AdrWHJZ_NDJZ_BQLDCO_2				      FLASHIS_AdrWHJZ_NDJZ_BQLDNO2_2+2		         //CO标气浓度值2
#define FLASHIS_AdrWHJZ_NDJZ_BQLDH2S_2				      FLASHIS_AdrWHJZ_NDJZ_BQLDCO_2+2		         //H2S标气浓度值2
#define FLASHIS_AdrWHJZ_NDJZ_BQLDCO2_2				      FLASHIS_AdrWHJZ_NDJZ_BQLDH2S_2+2		         //CO2标气浓度值2

#define FLASHIS_AdrWHJZ_NDJZ_BQLDO2_3				      FLASHIS_AdrWHJZ_NDJZ_BQLDCO2_2+2		         //O2标气浓度值3      高

#define FLASHIS_AdrWHJZ_NDJZ_BQLDSO2_3				      FLASHIS_AdrWHJZ_NDJZ_BQLDO2_3+2		         //SO2标气浓度值3
#define FLASHIS_AdrWHJZ_NDJZ_BQLDNO_3				      FLASHIS_AdrWHJZ_NDJZ_BQLDSO2_3+2		         //NO标气浓度值3
#define FLASHIS_AdrWHJZ_NDJZ_BQLDNO2_3				      FLASHIS_AdrWHJZ_NDJZ_BQLDNO_3+2		         //NO2标气浓度值3

#define FLASHIS_AdrWHJZ_NDJZ_BQLDCO_3				      FLASHIS_AdrWHJZ_NDJZ_BQLDNO2_3+2		         //CO标气浓度值3
#define FLASHIS_AdrWHJZ_NDJZ_BQLDH2S_3				      FLASHIS_AdrWHJZ_NDJZ_BQLDCO_3+2		         //H2S标气浓度值3
#define FLASHIS_AdrWHJZ_NDJZ_BQLDCO2_3				      FLASHIS_AdrWHJZ_NDJZ_BQLDH2S_3+2		         //CO2标气浓度值3


//维护校准--->压力校准
#define FLASHIS_AdrWHJZ_YLJZ_DQY				      FLASHIS_AdrWHJZ_NDJZ_BQLDCO2_3+2		         //大气压零点值
#define FLASHIS_AdrWHJZ_YLJZ_JJY				      FLASHIS_AdrWHJZ_YLJZ_DQY+2		         //计压零点值
#define FLASHIS_AdrWHJZ_YLJZ_JY				      FLASHIS_AdrWHJZ_YLJZ_JJY+2		         //静压零点值
#define FLASHIS_AdrWHJZ_YLJZ_DY				      FLASHIS_AdrWHJZ_YLJZ_JY+2		         //动压零点值
#define FLASHIS_AdrWHJZ_YLJZ_CLY				      FLASHIS_AdrWHJZ_YLJZ_DY+2		         //尘流压零点值
#define FLASHIS_AdrWHJZ_YLJZ_QLY				      FLASHIS_AdrWHJZ_YLJZ_CLY+2		         //气流压零点值
//维护校准--->烟气传感器选择
#define FLASHIS_AdrWHJZ_QTCGQXZ			      FLASHIS_AdrWHJZ_YLJZ_QLY+2		         //
//维护校准--->温度矫正
#define FLASHIS_AdrWHJZ_WDJZ_LDGQ				      FLASHIS_AdrWHJZ_QTCGQXZ+1		           //干球零点值
#define FLASHIS_AdrWHJZ_WDJZ_LDYW             FLASHIS_AdrWHJZ_WDJZ_LDGQ+2		         //烟温零点值       //mark2
#define FLASHIS_AdrWHJZ_WDJZ_LDSQ				      FLASHIS_AdrWHJZ_WDJZ_LDYW+2		         //湿球零点值
#define FLASHIS_AdrWHJZ_WDJZ_LDJW				      FLASHIS_AdrWHJZ_WDJZ_LDSQ+2		         //计温零点值
#define FLASHIS_AdrWHJZ_WDJZ_LDHW				      FLASHIS_AdrWHJZ_WDJZ_LDJW+2		         //环温零点值

#define FLASHIS_AdrWHJZ_WDJZ_BLGQ				      FLASHIS_AdrWHJZ_WDJZ_LDHW+2		         //干球倍率值
#define FLASHIS_AdrWHJZ_WDJZ_BLYW				      FLASHIS_AdrWHJZ_WDJZ_BLGQ+2		         //烟温倍率值       //mark2
#define FLASHIS_AdrWHJZ_WDJZ_BLSQ				      FLASHIS_AdrWHJZ_WDJZ_BLYW+2		         //湿球倍率值
#define FLASHIS_AdrWHJZ_WDJZ_BLJW				      FLASHIS_AdrWHJZ_WDJZ_BLSQ+2		         //计温倍率值
#define FLASHIS_AdrWHJZ_WDJZ_BLHW				      FLASHIS_AdrWHJZ_WDJZ_BLJW+2		         //环温倍率值
//维护校准--->电池标定
#define FLASHIS_AdrWHJZ_DCJZ_BL			               FLASHIS_AdrWHJZ_WDJZ_BLHW+2		             //电池倍率
#define FLASHIS_AdrBDEnd					           FLASHIS_AdrWHJZ_DCJZ_BL+2			//标定结束值（用于备份用）
//---------------------------------气泵参数----
#define FLASHIS_AdrCYSZALCYLL					FLASHIS_AdrWHJZ_DCJZ_BL+2		//A路采样设置采样流量  工况
#define FLASHIS_AdrCYSZBLCYLL					FLASHIS_AdrCYSZALCYLL+2		//B路采样设置采样流量    烟尘
#define FLASHIS_AdrCYSZCLCYLL					FLASHIS_AdrCYSZBLCYLL+2		//C路采样设置采样流量    烟气
//维护密码
#define FLASHIS_AdrXTWHMM						FLASHIS_AdrCYSZCLCYLL+2		//系统维护密码
#define FLASHIS_AdrXTWHLYPDM					FLASHIS_AdrXTWHMM+1			//蓝牙配对码
#define FLASHIS_AdrXTWHAPPLYPDM					FLASHIS_AdrXTWHLYPDM+1			//APP蓝牙配对码
#define FLASHIS_AdrXTWHLYPDMKG					FLASHIS_AdrXTWHAPPLYPDM+1			//APP蓝牙配对码开关
#define FLASHIS_AdrXTWHLLXS						FLASHIS_AdrXTWHLYPDMKG+1	//流量显示
#define FLASHIS_AdrXTWHDYBL						FLASHIS_AdrXTWHLLXS+1		//电压倍率
#define FLASHIS_AdrXTWHFSKG						FLASHIS_AdrXTWHDYBL+2		//风扇开关
#define FLASHIS_AdrXTWHBJHMQD					FLASHIS_AdrXTWHFSKG+1		//本机号码前段
#define FLASHIS_AdrXTWHBJHMHD					FLASHIS_AdrXTWHBJHMQD+2	//本机号码后段
//#define FLASHIS_AdrXTWHDD
//掉电信息
#define FLASHIS_AdrDDTime						FLASHIS_AdrXTWHBJHMHD+2 //掉电信息，单位秒，time_t格式
//#define FLASHIS_AdrDDYear							FLASHIS_AdrXTWHBJHMHD+2	//掉电年
//#define FLASHIS_AdrDDMonth						FLASHIS_AdrDDYear+1			//掉电月
//#define FLASHIS_AdrDDDate							FLASHIS_AdrDDMonth+1		//掉电日
//#define FLASHIS_AdrDDHour							FLASHIS_AdrDDDate+1			//掉电时
//#define FLASHIS_AdrDDMinute 					FLASHIS_AdrDDHour+1			//掉电分
//#define FLASHIS_AdrDDSec							FLASHIS_AdrDDMinute+1		//掉电秒
//-----------------------------------------------------------------------------------
//运行时间信息
#define FLASHIS_AdrYQYXSJ						FLASHIS_AdrDDTime+2			//仪器运行时间,单位S
#define FLASHIS_AdrCBYXSJ						FLASHIS_AdrYQYXSJ+2			//尘泵运行时间,单位S
//------------------------------------------------------------------------------------
//传感器安装时间
#define FLASHIS_AdrO2AZSJ						FLASHIS_AdrCBYXSJ+2			//O2传感器安装时间
#define FLASHIS_AdrSOANSJ						  FLASHIS_AdrO2AZSJ+2			//SO传感器安装时间
#define FLASHIS_AdrNOANSJ						  FLASHIS_AdrSOANSJ+2			//NO传感器安装时间
#define FLASHIS_AdrNO2ANSJ						FLASHIS_AdrNOANSJ+2			//NO2传感器安装时间
#define FLASHIS_AdrCOANSJ						  FLASHIS_AdrNO2ANSJ+2		//CO传感器安装时间
#define FLASHIS_AdrH2SNSJ						  FLASHIS_AdrCOANSJ+2			//H2S传感器安装时间
#define FLASHIS_AdrCO2ANSJ						FLASHIS_AdrH2SNSJ+2			//CO2传感器安装时间

//传感器量程
#define FLASHIS_AdrO2LQ                FLASHIS_AdrCO2ANSJ+2    //氧气量程
#define FLASHIS_AdrSO2LQ               FLASHIS_AdrO2LQ+2    //量程
#define FLASHIS_AdrNOLQ                FLASHIS_AdrSO2LQ+2    //量程
#define FLASHIS_AdrNO2LQ               FLASHIS_AdrNOLQ+2    //量程
#define FLASHIS_AdrCOLQ                FLASHIS_AdrNO2LQ+2    //量程
#define FLASHIS_AdrH2SLQ               FLASHIS_AdrCOLQ+2    //量程
#define FLASHIS_AdrCO2LQ               FLASHIS_AdrH2SLQ+2    //量程


//------------------------------------------------------------------------------------
//超级维护相关
#define FLASHIS_AdrCJWHMM					FLASHIS_AdrCO2LQ+2			//超级维护密码，默认值4321
#define FLASHIS_AdrCJWHYQBH					FLASHIS_AdrCJWHMM+1			//仪器编号，2个字，无符号整型32位，默认：1234567890
#define FLASHIS_AdrCJWHRJBB					FLASHIS_AdrCJWHYQBH+2		//软件版本号，默认：V1.00

//备份值
#define FLASHIS_AdrBackupStart				FLASHIS_AdrCJWHRJBB+2		//备份区域起始地址
#define FLASHIS_AdrBackupEnd				FLASHIS_AdrBackupStart+(FLASHIS_AdrBDEnd-FLASHIS_AdrBDStart)

#define FLASHIS_AdrStoreFactory				FLASHIS_AdrBackupEnd+2//恢复出厂设置
#define FLASHIS_AdrEnd						FLASHIS_AdrStoreFactory+1//结束


//结构体
typedef struct
{
	u8 Flag;	//标志位，若=0则没有数据更新，若!=0则有数据更新
	u8 Busy;	//标志位，若=0则保存数据寄存器空闲，若!=0则忙	
	u8 RunMode;
	u16 Data[FLASHIS_AdrEnd];//数据缓存
}Flashisp_DataStructure;



//----------------------------------------------------
void Flashisp_main(void);
void Flashisp_Init(void);
void Flashisp_DataInit(void);
Flashisp_DataStructure* Flashisp_GetData(void);
void Flashisp_FactoryReset(u8 select);
void Flashisp_Backup(void);
void Flashisp_Restore(void);

void Flashisp_StoreData(void);
u16 STMFLASH_GetFlashSector(u32 addr);

void Flashisp_DebugASCII(u8 *p,u8 len);



#endif










