/*
该文件为塔机文件，处理与塔机相关的数据
*/
#include "tower.h"

TOWER_DataStructure TowerData;//Tower结构体变量
/*
	函数名：void Tower_Init(void)
	说明Tower.c初始化文件
*/
void Tower_Init(void)
{
	Tower_FactoryReset();
	Tower_MachineInit();
	Tower_BaseInit();
	Tower_SlewingInit();
	Tower_TrolleyingInit();
	Tower_HoistingInit();
}
//---
/*
	函数名：void Tower_MachineInit(void)
*/
void Tower_MachineInit(void)
{
	
}
//---
/*
	函数名：void Tower_BaseInit(void)
	描述：	Slewing相关初始化
*/
void Tower_BaseInit(void)
{
	TwoBytes two;
	FourBytes four;
	//塔机臂长
	Tower_Operation(5,two.U8Data,2);
	TowerData.Base.Length = two.U16Data * 100;//转换为mm
	//塔机高度
	Tower_Operation(6,two.U8Data,2);
	TowerData.Base.Height = two.U16Data * 100;//转换为mm
	//塔机制造商
//	Tower_Operation(4,ScreenData.Buffer,20);
	TowerData.Base.Manufacturer = 22861;
	//塔机型号
	TowerData.Base.Version = 553;
	//塔机编号
	Tower_Operation(2,four.U8Data,4);//读塔机编号数据
	TowerData.Base.SerialNumber = four.U32Data;
	//额定载重
	Tower_Operation(7,four.U8Data,4);
	TowerData.Base.RatedLoad = four.FloatData;
	//首端载重
	Tower_Operation(8,four.U8Data,4);
	TowerData.Base.HeadLoad = four.FloatData;
	//末端载重
	Tower_Operation(9,four.U8Data,4);
	TowerData.Base.EndLoad = four.FloatData;
	
	printf("塔机编号%u\r\n",TowerData.Base.SerialNumber);
	printf("塔机制造商代码%u\r\n",TowerData.Base.Manufacturer);
	printf("塔机型号代码%u\r\n",TowerData.Base.Version);
	printf("塔机臂长%.2fmm\r\n",TowerData.Base.Length);
	printf("塔机高度%.2fmm\r\n",TowerData.Base.Height);
	printf("额定载重%.3fKg\r\n",TowerData.Base.RatedLoad);
	printf("首端载重%.3fKg\r\n",TowerData.Base.HeadLoad);
	printf("末端载重%.3fKg\r\n\r\n",TowerData.Base.EndLoad);
}
//---
/*
	函数名：void Tower_SlewingInit(void)
	描述：	Slewing相关初始化
*/
void Tower_SlewingInit(void)
{
	TwoBytes two;
	//大齿轮
	Tower_Operation(22,two.U8Data,2);//读数据
	TowerData.Slewing.BigGear = two.U16Data;//大齿轮
	//小齿轮
	Tower_Operation(23,two.U8Data,2);//读数据
	TowerData.Slewing.LittleGear = two.U16Data;//小齿轮
	printf("回转大齿轮数%u\r\n",TowerData.Slewing.BigGear);
	printf("回转小齿轮数%u\r\n\r\n",TowerData.Slewing.LittleGear);
}
//---
/*
	函数名：void Tower_TrolleyingInit(void)
	描述：	Trolleying相关初始化
*/
void Tower_TrolleyingInit(void)
{
	TwoBytes two;
	//大齿轮
	Tower_Operation(42,two.U8Data,2);
	TowerData.Trolleying.BigGear = two.U16Data;
	//小齿轮
	Tower_Operation(43,two.U8Data,2);
	TowerData.Trolleying.LittleGear = two.U16Data;
	//钢丝绳层数
	Tower_Operation(48,two.U8Data,1);
	TowerData.Trolleying.RopeLayer = two.U8Data[0];
	//钢丝绳直径
	Tower_Operation(47,two.U8Data,2);
	TowerData.Trolleying.RopeDiameter = (float)(two.U16Data/10.0);
	//卷筒直径
	Tower_Operation(44,two.U8Data,2);
	TowerData.Trolleying.CoilingDiameter = (float)(two.U16Data/10.0);
	
	printf("变幅大齿轮数%u\r\n",TowerData.Trolleying.BigGear);
	printf("变幅小齿轮数%u\r\n",TowerData.Trolleying.LittleGear);
	printf("变幅钢丝绳层数%u\r\n",TowerData.Trolleying.RopeLayer);
	printf("变幅钢丝绳直径%.2fmm\r\n",TowerData.Trolleying.RopeDiameter);
	printf("变幅卷筒直径%.2fmm\r\n\r\n",TowerData.Trolleying.CoilingDiameter);
}
//---
/*
	函数名：void Tower_HoistingInit(void)
	描述：	Hoisting相关初始化
*/
void Tower_HoistingInit(void)
{
	TwoBytes two;
	//大齿轮
	Tower_Operation(62,two.U8Data,2);
	TowerData.Hoisting.BigGear = two.U16Data;
	//小齿轮
	Tower_Operation(63,two.U8Data,2);
	TowerData.Hoisting.LittleGear = two.U16Data;
	//钢丝绳层数
	Tower_Operation(68,two.U8Data,1);
	TowerData.Hoisting.RopeLayer = two.U8Data[0];
	//钢丝绳倍率
	Tower_Operation(70,two.U8Data,1);
	TowerData.Hoisting.RopeRate = two.U8Data[0];
	//钢丝绳直径
	Tower_Operation(67,two.U8Data,2);
	TowerData.Hoisting.RopeDiameter = (float)(two.U16Data/10.0);
	//卷筒直径
	Tower_Operation(64,two.U8Data,2);
	TowerData.Hoisting.CoilingDiameter  = (float)(two.U16Data/10.0);
	
	printf("吊高大齿轮数%u\r\n",TowerData.Hoisting.BigGear);
	printf("吊高小齿轮数%u\r\n",TowerData.Hoisting.LittleGear);
	printf("吊高钢丝绳层数%u\r\n",TowerData.Hoisting.RopeLayer);
	printf("吊高钢丝绳倍率%u\r\n",TowerData.Hoisting.RopeRate);
	printf("吊高钢丝绳直径%.2fmm\r\n",TowerData.Hoisting.RopeDiameter);
	printf("吊高卷筒直径%.2fmm\r\n\r\n",TowerData.Hoisting.CoilingDiameter);
}
//---
/*
	函数名：TOWER_BaseStructure* Tower_GetBaseData(void)
	描述：该函数用于获取塔机基本参数结构体地址
*/
TOWER_BaseStructure* Tower_GetBaseData(void)
{
	return &TowerData.Base;
}
//---
/*
	函数名：TOWER_SlewingStructure* Tower_GetSlewingData(void)
	描述：	该函数用于获取SlewingData结构体变量
*/
TOWER_SlewingStructure* Tower_GetSlewingData(void)
{
	return &TowerData.Slewing;
}
//---
/*
	函数名：TOWER_SlewingStructure* Tower_GetTrolleyingData(void)
	描述：	该函数用于获取TrolleyingData结构体变量
*/
TOWER_TrolleyingStructure* Tower_GetTrolleyingData(void)
{
	return &TowerData.Trolleying;
}
//---
/*
	函数名：TOWER_SlewingStructure* Tower_GetHoistingData(void)
	描述：	该函数用于获取HoistingData结构体变量
*/
TOWER_HoistingStructure* Tower_GetHoistingData(void)
{
	return &TowerData.Hoisting;
}
//---
/*
	函数名：void Tower_Operation(u16 mode,u8 *p,u8 len)
	描述：该函数为Tower.c的数据操作
*/
void Tower_Operation(u16 mode,u8 *p,u8 len)
{
	switch(mode)
	{
		case 1:
		{//读所有塔机参数
			Tower_ReadStoreData(TOWER_AdrStoreTJBase,p,len);
			break;
		}
		case 2:
		{//读塔机编号
			Tower_ReadStoreData(TOWER_AdrStoreTJBH,p,len);
			break;
		}
		case 3:
		{//读塔机型号
			Tower_ReadStoreData(TOWER_AdrStoreTJXH,p,len);
			break;
		}
		case 4:
		{//读塔机制造商
			Tower_ReadStoreData(TOWER_AdrStoreTJZZS,p,len);
			break;
		}
		case 5:
		{//读塔机臂长
			Tower_ReadStoreData(TOWER_AdrStoreTJBC,p,len);
			break;
		}
		case 6:
		{//读塔机高度
			Tower_ReadStoreData(TOWER_AdrStoreTJGD,p,len);
			break;
		}
		case 7:
		{//读塔机额定载重
			Tower_ReadStoreData(TOWER_AdrStoreTJEDZZ,p,len);
			break;
		}
		case 8:
		{//读塔机首端载重
			Tower_ReadStoreData(TOWER_AdrStoreTJSDZZ,p,len);
			break;
		}
		case 9:
		{//读塔机末端载重
			Tower_ReadStoreData(TOWER_AdrStoreTJMDZZ,p,len);
			break;
		}
		case 10:
		{//读塔机出厂时间日期
			Tower_ReadStoreData(TOWER_AdrStoreTJCCRQ,p,len);
			break;
		}
		case 21:
		{//读所有回转参数
			Tower_ReadStoreData(TOWER_AdrStoreHZBase,p,len);
			break;
		}
		case 22:
		{//读回转大齿轮数
			Tower_ReadStoreData(TOWER_AdrStoreHZDCLS,p,len);
			break;
		}
		case 23:
		{//读回转小齿轮数
			Tower_ReadStoreData(TOWER_AdrStoreHZXCLS,p,len);
			break;
		}
		case 41:
		{//读所有变幅参数
			Tower_ReadStoreData(TOWER_AdrStoreBFBase,p,len);
			break;
		}
		case 42:
		{//读变幅大齿轮数
			Tower_ReadStoreData(TOWER_AdrStoreBFDCLS,p,len);
			break;
		}
		case 43:
		{//读变幅小齿轮数
			Tower_ReadStoreData(TOWER_AdrStoreBFXCLS,p,len);
			break;
		}
		case 44:
		{//读变幅卷筒底部直径
			Tower_ReadStoreData(TOWER_AdrStoreBFJTDBZJ,p,len);
			break;
		}
		case 45:
		{//读变幅卷筒有效长度
			Tower_ReadStoreData(TOWER_AdrStoreBFJTYXCD,p,len);
			break;
		}
		case 46:
		{//读变幅卷筒螺距
			Tower_ReadStoreData(TOWER_AdrStoreBFLJ,p,len);
			break;
		}
		case 47:
		{//读变幅钢丝绳直径
			Tower_ReadStoreData(TOWER_AdrStoreBFGSSZJ,p,len);
			break;
		}
		case 48:
		{//读变幅钢丝绳层数
			Tower_ReadStoreData(TOWER_AdrStoreBFGSSCS,p,len);
			break;
		}
		case 49:
		{//读变幅钢丝绳有效转数
			Tower_ReadStoreData(TOWER_AdrStoreBFYXZS,p,len);
			break;
		}
		case 61:
		{//读所有吊高参数
			Tower_ReadStoreData(TOWER_AdrStoreDGBase,p,len);
			break;
		}
		case 62:
		{//读吊高大齿轮数
			Tower_ReadStoreData(TOWER_AdrStoreDGDCLS,p,len);
			break;
		}
		case 63:
		{//读吊高小齿轮数
			Tower_ReadStoreData(TOWER_AdrStoreDGXCLS,p,len);
			break;
		}
		case 64:
		{//读吊高卷筒底部直径
			Tower_ReadStoreData(TOWER_AdrStoreDGJTDBZJ,p,len);
			break;
		}
		case 65:
		{//读吊高卷筒有效长度
			Tower_ReadStoreData(TOWER_AdrStoreDGJTYXCD,p,len);
			break;
		}
		case 66:
		{//读吊高卷筒螺距
			Tower_ReadStoreData(TOWER_AdrStoreDGLJ,p,len);
			break;
		}
		case 67:
		{//读吊高钢丝绳直径
			Tower_ReadStoreData(TOWER_AdrStoreDGGSSZJ,p,len);
			break;
		}
		case 68:
		{//读吊高钢丝绳层数
			Tower_ReadStoreData(TOWER_AdrStoreDGGSSCS,p,len);
			break;
		}
		case 69:
		{//读吊高钢丝绳有效转数
			Tower_ReadStoreData(TOWER_AdrStoreDGYXZS,p,len);
			break;
		}
		case 70:
		{//读吊高钢丝绳倍率
			Tower_ReadStoreData(TOWER_AdrStoreDGGSSBL,p,len);
			break;
		}
		case 71:
		{//读吊高钢丝绳容量
			Tower_ReadStoreData(TOWER_AdrStoreDGGSSRL,p,len);
			break;
		}
		case 201:
		{//写所有塔机参数
			Tower_StoreData(TOWER_AdrStoreTJBase,p,len);
			break;
		}
		case 202:
		{//修改塔机编号
			Tower_StoreData(TOWER_AdrStoreTJBH,p,len);
			break;
		}
		case 203:
		{//修改塔机型号
			Tower_StoreData(TOWER_AdrStoreTJXH,p,len);
			break;
		}
		case 204:
		{//修改塔机制造商
			Tower_StoreData(TOWER_AdrStoreTJZZS,p,len);
			break;
		}
		case 205:
		{//修改塔机臂长
			Tower_StoreData(TOWER_AdrStoreTJBC,p,len);
			break;
		}
		case 206:
		{//修改塔机高度
			Tower_StoreData(TOWER_AdrStoreTJGD,p,len);
			break;
		}
		case 207:
		{//修改塔机额定载重
			Tower_StoreData(TOWER_AdrStoreTJEDZZ,p,len);
			break;
		}
		case 208:
		{//修改塔机首端载重
			Tower_StoreData(TOWER_AdrStoreTJSDZZ,p,len);
			break;
		}
		case 209:
		{//修改塔机末端载重
			Tower_StoreData(TOWER_AdrStoreTJMDZZ,p,len);
			break;
		}
		case 210:
		{//修改塔机出厂日期
			Tower_StoreData(TOWER_AdrStoreTJCCRQ,p,len);
			break;
		}
		case 221:
		{//修改所有塔机回转参数
			Tower_StoreData(TOWER_AdrStoreHZBase,p,len);
			break; 
		}
		case 222:
		{//修改塔机回转大齿轮数
			Tower_StoreData(TOWER_AdrStoreHZDCLS,p,len);
			break;
		}
		case 223:
		{//修改塔机回转小齿轮数
			Tower_StoreData(TOWER_AdrStoreHZXCLS,p,len);
			break;
		}
		case 241:
		{//修改所有塔机变幅参数
			Tower_StoreData(TOWER_AdrStoreBFBase,p,len);
			break;
		}
		case 242:
		{//修改塔机变幅大齿轮数
			Tower_StoreData(TOWER_AdrStoreBFDCLS,p,len);
			break;
		}
		case 243:
		{//修改塔机变幅小齿轮数
			Tower_StoreData(TOWER_AdrStoreBFXCLS,p,len);
			break;
		}
		case 244:
		{//修改塔机变幅卷筒底部直径
			Tower_StoreData(TOWER_AdrStoreBFJTDBZJ,p,len);
			break;
		}
		case 245:
		{//修改塔机变幅卷筒有效长度
			Tower_StoreData(TOWER_AdrStoreBFJTYXCD,p,len);
			break;
		}
		case 246:
		{//修改塔机变幅卷筒螺距
			Tower_StoreData(TOWER_AdrStoreBFLJ,p,len);
			break;
		}
		case 247:
		{//修改塔机变幅钢丝绳直径
			Tower_StoreData(TOWER_AdrStoreBFGSSZJ,p,len);
			break;
		}
		case 248:
		{//修改塔机变幅钢丝绳层数
			Tower_StoreData(TOWER_AdrStoreBFGSSCS,p,len);
			break;
		}
		case 249:
		{//修改塔机变幅钢丝绳有效转数
			Tower_StoreData(TOWER_AdrStoreBFYXZS,p,len);
			break;
		}
		case 261:
		{//修改所有塔机吊高参数
			Tower_StoreData(TOWER_AdrStoreDGBase,p,len);
			break;
		}
		case 262:
		{//修改塔机吊高大齿轮数
			Tower_StoreData(TOWER_AdrStoreDGDCLS,p,len);
			break;
		}
		case 263:
		{//修改塔机吊高小齿轮数
			Tower_StoreData(TOWER_AdrStoreDGXCLS,p,len);
			break;
		}
		case 264:
		{//修改塔机吊高卷筒底部直径
			Tower_StoreData(TOWER_AdrStoreDGJTDBZJ,p,len);
			break;
		}
		case 265:
		{//修改塔机吊高卷筒有效长度
			Tower_StoreData(TOWER_AdrStoreDGJTYXCD,p,len);
			break;
		}
		case 266:
		{//修改塔机吊高卷筒螺距
			Tower_StoreData(TOWER_AdrStoreDGLJ,p,len);
			break;
		}
		case 267:
		{//修改塔机吊高钢丝绳直径
			Tower_StoreData(TOWER_AdrStoreDGGSSZJ,p,len);
			break;
		}
		case 268:
		{//修改塔机吊高钢丝绳层数
			Tower_StoreData(TOWER_AdrStoreDGGSSCS,p,1);
			break;
		}
		case 269:
		{//修改塔机吊高钢丝绳有效转数
			Tower_StoreData(TOWER_AdrStoreDGYXZS,p,2);
			break;
		}
		case 270:
		{//修改塔机吊高钢丝绳倍率
			Tower_StoreData(TOWER_AdrStoreDGGSSBL,p,1);
			break;
		}
		case 271:
		{//修改塔机吊高钢丝容量
			Tower_StoreData(TOWER_AdrStoreDGGSSRL,p,2);
			break;
		}
		case 501:
		{//打印所有塔机参数
			FourBytes change4;
			TwoBytes change2;
			u8 *pdata = p+TOWER_AdrStoreTJBH;
			u8 i;
			change4.U8Data[0] = pdata[0];
			change4.U8Data[1] = pdata[1];
			change4.U8Data[2] = pdata[2];
			change4.U8Data[3] = pdata[3];
			printf("塔机编号为：%u\r\n",change4.U32Data);
			pdata = p+TOWER_AdrStoreTJXH;
			printf("塔机型号为：");
			i=0;
			do
			{
				printf("%c",pdata[i]);
				i++;
				if(i>20) break;
			}while(pdata[i] != 0);
			printf("\r\n");
			pdata = p+TOWER_AdrStoreTJZZS;
			printf("制造商为：");
			i=0;
			do
			{
				printf("%c",pdata[i]);
				i++;
				if(i>20) break;
			}while(pdata[i] != 0);
			printf("\r\n");
			pdata = p+TOWER_AdrStoreTJCCRQ;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("设备出厂日期为：%u年",change2.U16Data);
			change2.U8Data[0] = pdata[2];
			change2.U8Data[1] = pdata[3];
			printf("%u月",change2.U8Data[0]);
			printf("%u日\r\n",change2.U8Data[1]);
			
			pdata = p+TOWER_AdrStoreTJBC;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("塔机臂长为：%fm\r\n",change2.U16Data/10.0);
			pdata = p+TOWER_AdrStoreTJGD;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("塔机高度为：%fm\r\n",change2.U16Data/10.0);
			pdata = p+TOWER_AdrStoreTJEDZZ;
			change4.U8Data[0] = pdata[0];
			change4.U8Data[1] = pdata[1];
			change4.U8Data[2] = pdata[2];
			change4.U8Data[3] = pdata[3];
			printf("塔机额定载重为：%fKg\r\n",change4.U32Data/100.0);
			pdata = p+TOWER_AdrStoreTJSDZZ;
			change4.U8Data[0] = pdata[0];
			change4.U8Data[1] = pdata[1];
			change4.U8Data[2] = pdata[2];
			change4.U8Data[3] = pdata[3];
			printf("塔机首端载重为：%fKg\r\n",change4.U32Data/100.0);
			pdata = p+TOWER_AdrStoreTJMDZZ;
			change4.U8Data[0] = pdata[0];
			change4.U8Data[1] = pdata[1];
			change4.U8Data[2] = pdata[2];
			change4.U8Data[3] = pdata[3];
			printf("塔机末端载重为：%fKg\r\n",change4.U32Data/100.0);
			break;
		}
		case 502:
		{//打印塔机编号
			FourBytes change;
			change.U8Data[0] = p[0];
			change.U8Data[1] = p[1];
			change.U8Data[2] = p[2];
			change.U8Data[3] = p[3];
			printf("塔机编号为：%u\r\n",change.U32Data);
			break;
		}
		case 503:
		{//打塔机型号
			u8 i=0;
			printf("塔机型号为：");
			do
			{
				printf("%c",p[i]);
				i++;
				if(i>20) break;
			}while(p[i] != 0);
			printf("\r\n");
			break;
		}
		case 504:
		{//打印塔机制造商
			u8 i=0;
			printf("制造商为：");
			do
			{
				printf("%c",p[i]);
				i++;
				if(i>20) break;
			}while(p[i] != 0);
			printf("\r\n");
			break;
		}
		case 505:
		{//打印塔机臂长
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("塔机臂长为：%fm\r\n",change2.U16Data/100.0);
			break;
		}
		case 506:
		{//打印塔机高度
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("塔机高度为：%fm\r\n",change2.U16Data/100.0);
			break;
		}
		case 507:
		{//打印额定载重
			FourBytes change4;
			change4.U8Data[0] = p[0];
			change4.U8Data[1] = p[1];
			change4.U8Data[2] = p[2];
			change4.U8Data[3] = p[3];
			printf("额定载重为：%fKG\r\n",change4.U32Data/100.0);
			break;
		}
		case 508:
		{//打印首端载重
			FourBytes change4;
			change4.U8Data[0] = p[0];
			change4.U8Data[1] = p[1];
			change4.U8Data[2] = p[2];
			change4.U8Data[3] = p[3];
			printf("首端载重为：%fKG\r\n",change4.U32Data/100.0);
			break;
		}
		case 509:
		{//打印首端载重
			FourBytes change4;
			change4.U8Data[0] = p[0];
			change4.U8Data[1] = p[1];
			change4.U8Data[2] = p[2];
			change4.U8Data[3] = p[3];
			printf("塔机末端载重为：%fKG\r\n",change4.U32Data/100.0);
			break;
		}
		case 510:
		{//打印出厂年月日
			TwoBytes change;
			change.U8Data[0] = p[0];
			change.U8Data[1] = p[1];
			printf("设备出厂日期为：%u年",change.U16Data);
			change.U8Data[0] = p[2];
			change.U8Data[1] = p[3];
			printf("%u月",change.U8Data[0]);
			printf("%u日\r\n",change.U8Data[1]);
			break;
		}
		case 521:
		{//打印所有回转参数
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("回转大齿轮数为：%u\r\n",change2.U16Data);
			change2.U8Data[0] = p[2];
			change2.U8Data[1] = p[3];
			printf("回转小齿轮数为：%u\r\n",change2.U16Data);
			break;
		}
		case 522:
		{//打印回转大齿轮数
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("回转大齿轮数为：%u\r\n",change2.U16Data);
			break;
		}
		case 523:
		{//打印回转小齿轮数
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("回转小齿轮数为：%u\r\n",change2.U16Data);
			break;
		}
		case 541:
		{//打印所有变幅参数
			TwoBytes change2;
			u8 *pdata;
			u8 bf = TOWER_AdrStoreBFBase;
			pdata = p+TOWER_AdrStoreBFDCLS-bf;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("变幅大齿轮数为：%u\r\n",change2.U16Data);
			pdata = p+TOWER_AdrStoreBFXCLS-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("变幅小齿轮数为：%u\r\n",change2.U16Data);
			pdata = p+TOWER_AdrStoreBFJTDBZJ-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("变幅卷筒底部直径：%fmm\r\n",change2.U16Data/10.0);
			pdata = p+TOWER_AdrStoreBFJTYXCD-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("变幅卷筒有效长度：%fmm\r\n",change2.U16Data/10.0);
			pdata = p+TOWER_AdrStoreBFLJ-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("变幅卷筒螺距：%fmm\r\n",change2.U16Data/100.0);
			pdata = p+TOWER_AdrStoreBFGSSZJ-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("变幅钢丝绳直径：%fmm\r\n",change2.U16Data/100.0);
			pdata = p+TOWER_AdrStoreBFGSSCS-bf;
			printf("变幅钢丝绳层数：%u\r\n",*pdata);
			pdata = p+TOWER_AdrStoreBFYXZS-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("变幅钢丝绳有效转数：%f\r\n",change2.U16Data/100.0);
			break;
		}
		case 542:
		{//打印变幅大齿轮数
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("变幅大齿轮数为：%u\r\n",change2.U16Data);
			break;
		}
		case 543:
		{//打印变幅小齿轮数
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("变幅小齿轮数为：%u\r\n",change2.U16Data);
			break;
		}
		case 544:
		{//打印所有变幅参数
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("变幅卷筒底部直径：%fmm\r\n",change2.U16Data/10.0);
			break;
		}
		case 545:
		{//打印所有变幅参数
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("变幅卷筒有效长度：%fmm\r\n",change2.U16Data/10.0);
			break;
		}
		case 546:
		{//打印所有变幅参数
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("变幅卷筒螺距：%fmm\r\n",change2.U16Data/100.0);
			break;
		}
		case 547:
		{//打印所有变幅参数
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("变幅钢丝绳直径：%fmm\r\n",change2.U16Data/100.0);
			break;
		}
		case 548:
		{//打印所有变幅参数
			printf("变幅钢丝绳层数：%u\r\n",*p);
			break;
		}
		case 549:
		{//打印所有变幅参数
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("变幅钢丝绳有效转数：%f\r\n",change2.U16Data/100.0);
			break;
		}
		case 561:
		{//打印所有塔机吊高参数
			TwoBytes change2;
			u8 *pdata;
			u8 bf = TOWER_AdrStoreDGBase;
			pdata = p+TOWER_AdrStoreDGDCLS-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("吊高大齿轮数为：%u\r\n",change2.U16Data);
			pdata = p+TOWER_AdrStoreDGXCLS-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("吊高小齿轮数为：%u\r\n",change2.U16Data);
			pdata = p+TOWER_AdrStoreDGJTDBZJ-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("吊高卷筒底部直径：%fmm\r\n",change2.U16Data/10.0);
			pdata = p+TOWER_AdrStoreDGJTYXCD-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("吊高卷筒有效长度：%fmm\r\n",change2.U16Data/10.0);
			pdata = p+TOWER_AdrStoreDGLJ-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("吊高卷筒螺距：%fmm\r\n",change2.U16Data/100.0);
			pdata = p+TOWER_AdrStoreDGGSSZJ-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("吊高钢丝绳直径：%fmm\r\n",change2.U16Data/100.0);
			pdata = p+TOWER_AdrStoreDGGSSCS-bf;
			printf("吊高钢丝绳层数：%u\r\n",*pdata);
			pdata = p+TOWER_AdrStoreDGYXZS-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("吊高钢丝绳有效转数：%f\r\n",change2.U16Data/10.0);
			pdata = p+TOWER_AdrStoreDGGSSBL-bf;
			printf("吊高钢丝绳倍率：%u\r\n",*pdata);
			pdata = p+TOWER_AdrStoreDGGSSRL-bf;
			change2.U8Data[0] = pdata[0];
			change2.U8Data[1] = pdata[1];
			printf("吊高钢丝绳容量：%f\r\n",change2.U16Data/10.0);
			break;
		}
		case 562:
		{//打印吊高大齿轮数
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("吊高大齿轮数为：%u\r\n",change2.U16Data);
			break;
		}
		case 563:
		{//打印吊高小齿轮数
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("吊高小齿轮数为：%u\r\n",change2.U16Data);
			break;
		}
		case 564:
		{//打印吊高卷筒底部直径
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("吊高卷筒底部直径：%fmm\r\n",change2.U16Data/10.0);
			break;
		}
		case 565:
		{//打印吊高卷筒有效长度
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("吊高卷筒有效长度：%fmm\r\n",change2.U16Data/10.0);
			break;
		}
		case 566:
		{//打印吊高卷筒螺距
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("吊高卷筒螺距：%fmm\r\n",change2.U16Data/100.0);
			break;
		}
		case 567:
		{//打印吊高钢丝绳直径
			TwoBytes change2;
			change2.U8Data[0] = p[0];
			change2.U8Data[1] = p[1];
			printf("吊高钢丝绳直径：%fmm\r\n",change2.U16Data/100.0);
			break;
		}
		case 568:
		{//打印吊高钢丝绳层数
			printf("吊高钢丝绳层数：%u\r\n",*p);
			break;
		}
		case 569:
		{//打印吊高钢丝绳有效转数
			TwoBytes change;
			change.U8Data[0] = p[0];
			change.U8Data[1] = p[1];
			printf("吊高钢丝绳有效转数：%f\r\n",change.U16Data/10.0);
			break;
		}
		case 570:
		{//打印吊高钢丝绳倍率
			printf("吊高钢丝绳倍率：%u\r\n",*p);
			break;
		}
		case 571:
		{//打印吊高钢丝绳容量
			TwoBytes change;
			change.U8Data[0] = p[0];
			change.U8Data[1] = p[1];
			printf("吊高钢丝绳容量：%f\r\n",change.U16Data/10.0);
			break;
		}
	}
}
//---
/*
	函数名：void Tower_CalSlewing(s32 buffer)
	描述：	计算塔机回转角度，计算角度在0~360°之间			
*/
void Tower_CalSlewing(s32 buffer)
{
	TowerData.Slewing.Rotation = (float)buffer/TOWER_EnDiv * \
		(360.0/(TowerData.Slewing.BigGear / TowerData.Slewing.LittleGear));
	while(TowerData.Slewing.Rotation > 360)
		TowerData.Slewing.Rotation -=360;
	while(TowerData.Slewing.Rotation < 0)
		TowerData.Slewing.Rotation +=360;
}
//---
/*
	函数名：float Tower_CalTrolleying(u32 buffer)
	描述：	计算塔机变幅位置			
*/
void Tower_CalTrolleying(s32 buffer)
{
	TowerData.Trolleying.Range = (float)buffer/TOWER_EnDiv * \
		PI*(TowerData.Trolleying.CoilingDiameter +\
		TowerData.Trolleying.RopeDiameter*TowerData.Trolleying.RopeLayer)/ \
		((float)TowerData.Trolleying.BigGear/TowerData.Trolleying.LittleGear);
	Tower_CalMomental();//计算力矩
}
//---
/*
	函数名：float Tower_CalHoisting(u32 buffer)
	描述：	计算塔机变幅位置			
*/
void Tower_CalHoisting(s32 buffer)
{
	TowerData.Hoisting.HookHeight = (float)buffer/TOWER_EnDiv * \
		PI*(TowerData.Hoisting.CoilingDiameter +\
		TowerData.Hoisting.RopeDiameter*TowerData.Hoisting.RopeLayer)/ \
		((float)TowerData.Hoisting.BigGear/TowerData.Hoisting.LittleGear);
}
//---
/*
	函数名：void Tower_CalMomental(void)
	描述：计算力矩
*/
void Tower_CalMomental(void)
{
	TowerData.Base.Momental = (Thbss01_GetThbss01Data()->Hosting/1000.0)\
	/(TowerData.Base.HeadLoad-(TowerData.Base.HeadLoad-TowerData.Base.EndLoad)\
	*TowerData.Trolleying.Range/TowerData.Base.Length);
}
//-------------------------数据读取、存储----------------------------
//---
/*
	函数名：void Tower_ReadStoreData(u8 address,u8 *p,u8 len)
	描述：	读取数据
*/
void Tower_ReadStoreData(u8 address,u8 *p,u8 len)
{
	W25Xxx_ReadData(p,TOWER_AdrStoreBase+address,len);
}
//---
/*
	函数名：void Tower_StoreData(u8 address,u8 *p,u8 len)
	描述：该函数为存储OHRD23配置参数函数
	说明：读取数据
				擦除数据
				修改数据
				保存数据
*/
void Tower_StoreData(u8 address,u8 *p,u8 len)
{
	u8 buffer[TOWER_AdrStoreEnd],cnt;
	W25Xxx_ReadData(buffer,TOWER_AdrStoreBase,TOWER_AdrStoreEnd);//读所有配置参数
	W25Xxx_EraseSector(TOWER_AdrStoreBase);//擦除数据
	for(cnt=0;cnt<len;cnt++)
	{
		buffer[address++] = *p;
		p++;
	}
	W25Xxx_PageWrite(buffer,TOWER_AdrStoreBase,TOWER_AdrStoreEnd);//保存数据
	
	//重新初始化数据
	Tower_BaseInit();
	Tower_SlewingInit();
	Tower_TrolleyingInit();
	Tower_HoistingInit();
}
//---
/*
	函数名：void Tower_FactoryReset(void)
	描述：该函数Tower.c存储数据恢复出厂设置
*/
void Tower_FactoryReset(void)
{
	u8 bf[21];
	FourBytes four;
	TwoBytes two;
	u8 i;
	Tower_ReadStoreData(TOWER_AdrStoreInit,&i,1);
	if(i == 0x55) return;
	i = 0x55;
	Tower_StoreData(TOWER_AdrStoreTJBH,four.U8Data,4);

	four.U32Data = 1234567890;
	Tower_StoreData(TOWER_AdrStoreTJBH,four.U8Data,4);//塔机编号
	for(i=0;i<21;i++)
		bf[i] = 0;
	sprintf((char*)bf,"SST553");
	Tower_StoreData(TOWER_AdrStoreTJXH,bf,21);//塔机型号
	for(i=0;i<21;i++)
		bf[i] = 0;
	sprintf((char*)bf,"大器科技");
	Tower_StoreData(TOWER_AdrStoreTJZZS,bf,21);//塔机制造商
	
	two.U16Data = 1000;
	Tower_StoreData(TOWER_AdrStoreTJGD,two.U8Data,2);//塔机高度
	
	four.U16Data[0] = 2018;
	four.U8Data[2] = 11;
	four.U8Data[3] = 1;
	Tower_StoreData(TOWER_AdrStoreTJCCRQ,four.U8Data,21);//出厂日期
	
	for(i=0;i<21;i++)
		bf[i] = 0;
	sprintf((char*)bf,"大型");
	Tower_StoreData(TOWER_AdrStoreTJSBGG,bf,10);//塔机规格
	for(i=0;i<21;i++)
		bf[i] = 0;
	sprintf((char*)bf,"重型");
	Tower_StoreData(TOWER_AdrStoreTJSBLX,bf,10);//塔机类型
	
//	u16 bf[3]={1111,120,55555};
//	Flashisp_StoreData(FLASHIS_AdrStorePassword,bf,3);
//	Flashisp_ReadStoreData(FLASHIS_AdrStorePassword,bf,3);
//	printf("恢复出厂设置\r\n");
//	printf("维护密码为：%u\r\n",bf[0]);
//	printf("设定温度为：%u\r\n",bf[1]);
}
//---

//-------------------------调试函数----------------------------
//---
/*
	函数名： void Tower_DebugASCII(u8 *p,u8 len)
	描述：		该函数为Tower文件的ASCII调试文件
*/
void Tower_DebugASCII(u8 *p,u8 len)
{//:000|63|100
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 201:
		{//写所有塔机参数	:000|63|201|1234567890|
			if(len>TOWER_AdrStoreEnd)
			{
				len = 0;
			}
			break;
		}
		case 202:
		{//修改塔机编号		:000|63|202|1234567890
			FourBytes buffer;
			buffer.U32Data = Calculate_CharToU32(p+4,10);//计算命令
			printf("输入的数据为：%u\r\n",buffer.U32Data);
			Tower_Operation(202,buffer.U8Data,4);//修改数据
			Tower_Operation(2,buffer.U8Data,4);//读数据
			Tower_Operation(502,buffer.U8Data,4);//显示数据
			break;
		}
		case 203:
		{//修改塔机型号
			u8 i,buffer[21];
			for(i=0;i<21;i++)
			{
				buffer[i]= 0;
			}
			i = 0;
			do
			{
				buffer[i] = p[4+i];
				i++;
				if(i>20) break;
				if(p[4+i] == '|') break;
			}while(p[4+i] != '\r');
			buffer[20] = i;
			printf("输入的塔机型号为：");
			for(i=0;i<buffer[20];i++)
			{
				printf("%c",buffer[i]);
			}
			printf("\r\n");
			buffer[20] = 0;
			Tower_Operation(203,buffer,21);//修改数据
			Tower_Operation(3,buffer,21);//读数据
			Tower_Operation(503,buffer,21);//显示数据
			break;
		}
		case 204:
		{//修改塔机制造商
			u8 i,buffer[21];
			for(i=0;i<21;i++)
			{
				buffer[i]= 0;
			}
			i = 0;
			do
			{
				buffer[i] = p[4+i];
				i++;
				if(i>20) break;
				if(p[4+i] == '|') break;
			}while(p[4+i] != '\r');
			buffer[20] = i;
			printf("输入的塔机制造商为：");
			for(i=0;i<buffer[20];i++)
			{
				printf("%c",buffer[i]);
			}
			printf("\r\n");
			buffer[20] = 0;
			Tower_Operation(204,buffer,21);//修改数据
			Tower_Operation(4,buffer,21);//读数据
			Tower_Operation(504,buffer,21);//显示数据
			break;
		}
		case 205:
		{//修改塔机臂长
			TwoBytes buffer;
			float a = Calculate_CharToFloat(p+4,5);//计算命令
			if(a<655.35)
			{
				printf("输入值为：%fm\r\n",a);
				buffer.U16Data = (u16)(a*100);
				Tower_Operation(205,buffer.U8Data,2);//修改数据
				Tower_Operation(5,buffer.U8Data,2);//读数据
				Tower_Operation(505,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("塔机臂长可设置范围为0~655.35m\r\n");
			}
			break;
		}
		case 206:
		{//修改塔机高度
			TwoBytes buffer;
			float a = Calculate_CharToFloat(p+4,5);//计算命令
			if(a<655.35)
			{
				printf("输入值为：%fm\r\n",a);
				buffer.U16Data = (u16)(a*100);
				Tower_Operation(206,buffer.U8Data,2);//修改数据
				Tower_Operation(6,buffer.U8Data,2);//读数据
				Tower_Operation(506,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("塔机高度可设置范围为0~655.35m\r\n");
			}
			break;
		}
		case 207:
		{//修改额定载重
			FourBytes buffer;
			float a = Calculate_CharToFloat(p+4,8);//计算命令
			if(a<99999.99)
			{
				printf("输入值为：%fKG\r\n",a);
				buffer.U32Data = (u32)(a*100);
				Tower_Operation(207,buffer.U8Data,4);//修改数据
				Tower_Operation(7,buffer.U8Data,4);//读数据
				Tower_Operation(507,buffer.U8Data,4);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("塔机额定载重可设置范围为0~99999.99KG\r\n");
			}
			break;
		}
		case 208:
		{//修改首端载重
			FourBytes buffer;
			float a = Calculate_CharToFloat(p+4,8);//计算命令
			if(a<99999.99)
			{
				printf("输入值为：%fKG\r\n",a);
				buffer.U32Data = (u32)(a*100);
				Tower_Operation(208,buffer.U8Data,4);//修改数据
				Tower_Operation(8,buffer.U8Data,4);//读数据
				Tower_Operation(508,buffer.U8Data,4);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("塔机首端载重可设置范围为0~99999.99KG\r\n");
			}
			break;
		}
		case 209:
		{//修改末端载重
			FourBytes buffer;
			float a = Calculate_CharToFloat(p+4,8);//计算命令
			if(a<99999.99)
			{
				printf("输入值为：%fKG\r\n",a);
				buffer.U32Data = (u32)(a*100);
				Tower_Operation(209,buffer.U8Data,4);//修改数据
				Tower_Operation(9,buffer.U8Data,4);//读数据
				Tower_Operation(509,buffer.U8Data,4);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("塔机末端载重可设置范围为0~99999.99KG\r\n");
			}
			break;
		}
		case 210:
		{//修改塔机出厂日期
			u16 year = Calculate_CharToU16(p+4,4);//计算命令
			u8 month	= Calculate_CharToU16(p+9,2);//计算命令
			u8 day	= Calculate_CharToU16(p+12,2);//计算命令
			if(year >1899 && year <2100 && month >0 && month <13 && day>0 && day < 32)
			{
				u8 buffer[4];
				TwoBytes a;
				a.U16Data = year;
				buffer[0] = a.U8Data[0];
				buffer[1] = a.U8Data[1];
				buffer[2] = month;
				buffer[3] = day;
				printf("输入年值为：%u年\r\n",year);
				printf("输入月值为：%u月\r\n",month);
				printf("输入日值为：%u日\r\n",day);
				Tower_Operation(210,buffer,4);//修改数据
				Tower_Operation(10,buffer,4);//读数据
				Tower_Operation(510,buffer,4);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("年参数可输入范围为1900~2099年\r\n");
				printf("月参数可输入范围为1~12月\r\n");
				printf("日参数可输入范围为1~31日\r\n");
			}
			break;
		}
		
		case 221:
		{//修改所有回转参数
			break;
		}
		case 222:
		{//修改回转大齿轮参数
			TwoBytes buffer;
			buffer.U16Data = Calculate_CharToU16(p+4,5);//计算命令
			if(buffer.U16Data<60000)
			{
				printf("输入值为：%u\r\n",buffer.U16Data);
				Tower_Operation(222,buffer.U8Data,2);//修改数据
				Tower_Operation(22,buffer.U8Data,2);//读数据
				Tower_Operation(522,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("回转大齿轮参数可设置范围为0~60000\r\n");
			}
			break;
		}
		case 223:
		{//修改回转小齿轮参数
			TwoBytes buffer;
			buffer.U16Data = Calculate_CharToU16(p+4,5);//计算命令
			if(buffer.U16Data<60000)
			{
				printf("输入值为：%u\r\n",buffer.U16Data);
				Tower_Operation(223,buffer.U8Data,2);//修改数据
				Tower_Operation(23,buffer.U8Data,2);//读数据
				Tower_Operation(523,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("回转小齿轮参数可设置范围为0~60000\r\n");
			}
			break;
		}
		case 241:
		{//修改所有变幅参数
			break;
		}
		case 242:
		{//修改变幅大齿轮数
			TwoBytes buffer;
			buffer.U16Data = Calculate_CharToU16(p+4,5);
			if(buffer.U16Data<60000)
			{
				printf("输入值为：%u\r\n",buffer.U16Data);
				Tower_Operation(242,buffer.U8Data,2);//修改数据
				Tower_Operation(42,buffer.U8Data,2);//读数据
				Tower_Operation(542,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("变幅大齿轮数参数可设置范围为0~60000\r\n");
			}
			break;
		}
		case 243:
		{//修改变幅小齿轮数
			TwoBytes buffer;
			buffer.U16Data = Calculate_CharToU16(p+4,5);
			if(buffer.U16Data<60000)
			{
				printf("输入值为：%u\r\n",buffer.U16Data);
				Tower_Operation(243,buffer.U8Data,2);//修改数据
				Tower_Operation(43,buffer.U8Data,2);//读数据
				Tower_Operation(543,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("变幅大齿轮数参数可设置范围为0~60000\r\n");
			}
			break;
		}
		case 244:
		{//修改变幅卷筒底部直径
			float a = Calculate_CharToFloat(p+4,6);
			if(a<6553.5)
			{
				TwoBytes buffer;
				printf("输入值为：%f\r\n",a);
				buffer.U16Data = (u16)(a*10);
				Tower_Operation(244,buffer.U8Data,2);//修改数据
				Tower_Operation(44,buffer.U8Data,2);//读数据
				Tower_Operation(544,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("变幅卷筒底部直径参数可设置范围为0~6553.5mm\r\n");
			}
			break;
		}
		case 245:
		{//修改变幅卷筒有效长度
			float a = Calculate_CharToFloat(p+4,6);
			if(a<6553.5)
			{
				TwoBytes buffer;
				printf("输入值为：%f\r\n",a);
				buffer.U16Data = (u16)(a*10);
				Tower_Operation(245,buffer.U8Data,2);//修改数据
				Tower_Operation(45,buffer.U8Data,2);//读数据
				Tower_Operation(545,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("变幅卷筒有效长度参数可设置范围为0~6553.5mm\r\n");
			}
			break;
		}
		case 246:
		{//修改变幅卷筒螺距
			float a = Calculate_CharToFloat(p+4,6);
			if(a<655.35)
			{
				TwoBytes buffer;
				printf("输入值为：%f\r\n",a);
				buffer.U16Data = (u16)(a*100);
				Tower_Operation(246,buffer.U8Data,2);//修改数据
				Tower_Operation(46,buffer.U8Data,2);//读数据
				Tower_Operation(546,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("变幅卷筒螺距参数可设置范围为0~655.35mm\r\n");
			}
			break;
		}
		case 247:
		{//修改变幅钢丝绳直径
			float a = Calculate_CharToFloat(p+4,6);
			if(a<655.35)
			{
				TwoBytes buffer;
				printf("输入值为：%f\r\n",a);
				buffer.U16Data = (u16)(a*100);
				Tower_Operation(247,buffer.U8Data,2);//修改数据
				Tower_Operation(47,buffer.U8Data,2);//读数据
				Tower_Operation(547,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("变幅钢丝绳直径参数可设置范围为0~655.35mm\r\n");
			}
			break;
		}
		case 248:
		{//修改变幅钢丝绳层数
			u8 a = Calculate_CharToFloat(p+4,3);
			if(a<250)
			{
				printf("输入值为：%u\r\n",a);
				Tower_Operation(248,&a,1);//修改数据
				Tower_Operation(48,&a,1);//读数据
				Tower_Operation(548,&a,1);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("变幅钢丝绳层数参数可设置范围为0~250\r\n");
			}
			break;
		}
		case 249:
		{//修改变幅钢丝绳有效转数
			float a = Calculate_CharToFloat(p+4,6);
			if(a<655.35)
			{
				TwoBytes buffer;
				printf("输入值为：%f\r\n",a);
				buffer.U16Data = a * 100;
				Tower_Operation(249,buffer.U8Data,2);//修改数据
				Tower_Operation(49,buffer.U8Data,2);//读数据
				Tower_Operation(549,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("变幅钢丝绳有效转数参数可设置范围为0~655.35\r\n");
			}
			break;
		}
		case 261:
		{//修改所有吊高参数
			break;
		}
		case 262:
		{//修改吊高大齿轮数
			TwoBytes buffer;
			buffer.U16Data = Calculate_CharToU16(p+4,5);
			if(buffer.U16Data<60000)
			{
				printf("输入值为：%u\r\n",buffer.U16Data);
				Tower_Operation(262,buffer.U8Data,2);//修改数据
				Tower_Operation(62,buffer.U8Data,2);//读数据
				Tower_Operation(562,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("吊高大齿轮数参数可设置范围为0~60000\r\n");
			}
			break;
		}
		case 263:
		{//修改吊高小齿轮数
			TwoBytes buffer;
			buffer.U16Data = Calculate_CharToU16(p+4,5);
			if(buffer.U16Data<60000)
			{
				printf("输入值为：%u\r\n",buffer.U16Data);
				Tower_Operation(263,buffer.U8Data,2);//修改数据
				Tower_Operation(63,buffer.U8Data,2);//读数据
				Tower_Operation(563,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("吊高小齿轮数参数可设置范围为0~60000\r\n");
			}
			break;
		}
		case 264:
		{//修改吊高卷筒底部直径
			float a = Calculate_CharToFloat(p+4,6);
			if(a<6553.5)
			{
				TwoBytes buffer;
				printf("输入值为：%f\r\n",a);
				buffer.U16Data = (u16)(a*10);
				Tower_Operation(264,buffer.U8Data,2);//修改数据
				Tower_Operation(64,buffer.U8Data,2);//读数据
				Tower_Operation(564,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("吊高卷筒底部直径参数可设置范围为0~6553.5mm\r\n");
			}
			break;
		}
		case 265:
		{//修改吊高卷筒有效长度
			float a = Calculate_CharToFloat(p+4,6);
			if(a<6553.5)
			{
				TwoBytes buffer;
				printf("输入值为：%f\r\n",a);
				buffer.U16Data = (u16)(a*10);
				Tower_Operation(265,buffer.U8Data,2);//修改数据
				Tower_Operation(65,buffer.U8Data,2);//读数据
				Tower_Operation(565,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("吊高卷筒有效长度参数可设置范围为0~6553.5mm\r\n");
			}
			break;
		}
		case 266:
		{//修改吊高卷筒螺距
			float a = Calculate_CharToFloat(p+4,6);
			if(a<655.35)
			{
				TwoBytes buffer;
				printf("输入值为：%f\r\n",a);
				buffer.U16Data = (u16)(a*100);
				Tower_Operation(266,buffer.U8Data,2);//修改数据
				Tower_Operation(66,buffer.U8Data,2);//读数据
				Tower_Operation(566,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("吊高卷筒螺距参数可设置范围为0~655.35mm\r\n");
			}
			break;
		}
		case 267:
		{//修改吊高钢丝绳直径
			float a = Calculate_CharToFloat(p+4,6);
			if(a<655.35)
			{
				TwoBytes buffer;
				printf("输入值为：%f\r\n",a);
				buffer.U16Data = (u16)(a*100);
				Tower_Operation(267,buffer.U8Data,2);//修改数据
				Tower_Operation(67,buffer.U8Data,2);//读数据
				Tower_Operation(567,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("吊高钢丝绳直径参数可设置范围为0~655.35mm\r\n");
			}
			break;
		}
		case 268:
		{//修改吊高钢丝绳层数
			u8 a = Calculate_CharToU16(p+4,3);
			if(a<250)
			{
				printf("输入值为：%u\r\n",a);
				Tower_Operation(268,&a,1);//修改数据
				Tower_Operation(68,&a,1);//读数据
				Tower_Operation(568,&a,1);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("吊高钢丝绳层数参数可设置范围为0~250\r\n");
			}
			break;
		}
		case 269:
		{//修改吊高钢丝绳有效转数
			float a = Calculate_CharToFloat(p+4,6);
			if(a<6553.5)
			{
				TwoBytes buffer;
				printf("输入值为：%f\r\n",a);
				buffer.U16Data = a * 10;
				Tower_Operation(269,buffer.U8Data,2);//修改数据
				Tower_Operation(69,buffer.U8Data,2);//读数据
				Tower_Operation(569,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("吊高钢丝绳有效转数参数可设置范围为0~6553.5\r\n");
			}
			break;
		}
		case 270:
		{//修改吊高钢丝绳倍率
			u8 a = Calculate_CharToU16(p+4,3);
			if(a<250)
			{
				printf("输入值为：%u\r\n",a);
				Tower_Operation(270,&a,1);//修改数据
				Tower_Operation(70,&a,1);//读数据
				Tower_Operation(570,&a,1);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("吊高钢丝绳倍率参数可设置范围为0~250\r\n");
			}
			break;
		}
		case 271:
		{//修改吊高钢丝绳容量
			float a = Calculate_CharToFloat(p+4,6);
			if(a<6553.5)
			{
				TwoBytes buffer;
				buffer.U16Data = a * 10;
				printf("输入值为：%f\r\n",a);
				Tower_Operation(271,buffer.U8Data,2);//修改数据
				Tower_Operation(71,buffer.U8Data,2);//读数据
				Tower_Operation(571,buffer.U8Data,2);//显示数据
			}
			else
			{
				printf("输入错误\r\n");
				printf("吊高钢丝绳容量参数可设置范围为0~6553.5\r\n");
			}
			break;
		}
		
		case 501:
		{//显示所有塔机参数
			u8 buffer[TOWER_AdrStoreEnd];
			Tower_Operation(1,buffer,TOWER_AdrStoreEnd);//读数据
			Tower_Operation(501,buffer,TOWER_AdrStoreEnd);//显示数据
			break;
		}
		case 502:
		{//显示塔机编号
			u8 buffer[4];
			Tower_Operation(2,buffer,4);//读数据
			Tower_Operation(502,buffer,4);//显示数据
			break;
		}
		case 503:
		{//显示塔机型号
			u8 buffer[21];
			Tower_Operation(3,buffer,21);//读数据
			Tower_Operation(503,buffer,21);//显示数据
			break;
		}
		case 504:
		{//显示塔机制造商
			u8 buffer[21];
			Tower_Operation(4,buffer,21);//读数据
			Tower_Operation(504,buffer,21);//显示数据
			break;
		}
		case 505:
		{//显示塔机臂长
			u8 buffer[2];
			Tower_Operation(5,buffer,2);//读数据
			Tower_Operation(505,buffer,2);//显示数据
			break;
		}
		case 506:
		{//显示塔机高度
			u8 buffer[2];
			Tower_Operation(6,buffer,2);//读数据
			Tower_Operation(506,buffer,2);//显示数据
			break;
		}
		case 507:
		{//显示额定载重
			u8 buffer[4];
			Tower_Operation(7,buffer,4);//读数据
			Tower_Operation(507,buffer,4);//显示数据
			break;
		}
		case 508:
		{//显示首端载重
			u8 buffer[4];
			Tower_Operation(8,buffer,4);//读数据
			Tower_Operation(508,buffer,4);//显示数据
			break;
		}
		case 509:
		{//显示首端载重
			u8 buffer[4];
			Tower_Operation(9,buffer,4);//读数据
			Tower_Operation(509,buffer,4);//显示数据
			break;
		}
		case 510:
		{//显示出厂日期
			u8 buffer[4];
			Tower_Operation(10,buffer,4);//读数据
			Tower_Operation(510,buffer,4);//显示数据
			break;
		}
		case 521:
		{//显示所有回转参数
			u8 buffer[TOWER_AdrStoreHZEnd-TOWER_AdrStoreHZBase];
			Tower_Operation(21,buffer,TOWER_AdrStoreHZEnd-TOWER_AdrStoreHZBase);//读数据
			Tower_Operation(521,buffer,TOWER_AdrStoreHZEnd-TOWER_AdrStoreHZBase);//显示数据
			break;
		}
		case 522:
		{//显示回转大齿轮数
			u8 buffer[2];
			Tower_Operation(22,buffer,2);//读数据
			Tower_Operation(522,buffer,2);//显示数据
			break;
		}
		case 523:
		{//显示回转小齿轮数
			u8 buffer[2];
			Tower_Operation(23,buffer,2);//读数据
			Tower_Operation(523,buffer,2);//显示数据
			break;
		}
		case 541:
		{//显示所有变幅参数
			u8 buffer[TOWER_AdrStoreBFEnd-TOWER_AdrStoreBFBase];
			Tower_Operation(41,buffer,TOWER_AdrStoreBFEnd-TOWER_AdrStoreBFBase);//读数据
			Tower_Operation(541,buffer,TOWER_AdrStoreBFEnd-TOWER_AdrStoreBFBase);//显示数据
			break;
		}
		case 542:
		{//显示变幅大齿轮数
			u8 buffer[2];
			Tower_Operation(42,buffer,2);//读数据
			Tower_Operation(542,buffer,2);//显示数据
			break;
		}
		case 543:
		{//显示变幅小齿轮数
			u8 buffer[2];
			Tower_Operation(43,buffer,2);//读数据
			Tower_Operation(543,buffer,2);//显示数据
			break;
		}
		case 544:
		{//显示变幅卷筒底部直径
			u8 buffer[2];
			Tower_Operation(44,buffer,2);//读数据
			Tower_Operation(544,buffer,2);//显示数据
			break;
		}
		case 545:
		{//显示变幅卷筒有效长度
			u8 buffer[2];
			Tower_Operation(45,buffer,2);//读数据
			Tower_Operation(545,buffer,2);//显示数据
			break;
		}
		case 546:
		{//显示变幅卷筒螺距
			u8 buffer[2];
			Tower_Operation(46,buffer,2);//读数据
			Tower_Operation(546,buffer,2);//显示数据
			break;
		}
		case 547:
		{//显示变幅钢丝绳直径
			u8 buffer[2];
			Tower_Operation(47,buffer,2);//读数据
			Tower_Operation(547,buffer,2);//显示数据
			break;
		}
		case 548:
		{//显示变幅钢丝绳层数
			u8 buffer;
			Tower_Operation(48,&buffer,1);//读数据
			Tower_Operation(548,&buffer,1);//显示数据
			break;
		}
		case 549:
		{//显示变幅钢丝绳有效转数
			u8 buffer[2];
			Tower_Operation(49,buffer,2);//读数据
			Tower_Operation(549,buffer,2);//显示数据
			break;
		}
		case 561:
		{//显示所有吊高参数
			u8 buffer[TOWER_AdrStoreDGEnd - TOWER_AdrStoreDGBase];
			Tower_Operation(61,buffer,TOWER_AdrStoreDGEnd - TOWER_AdrStoreDGBase);//读数据
			Tower_Operation(561,buffer,TOWER_AdrStoreDGEnd - TOWER_AdrStoreDGBase);//显示数据
			break;
		}
		case 562:
		{//显示吊高大齿轮数
			u8 buffer[2];
			Tower_Operation(62,buffer,2);//读数据
			Tower_Operation(562,buffer,2);//显示数据
			break;
		}
		case 563:
		{//显示吊高小齿轮数
			u8 buffer[2];
			Tower_Operation(63,buffer,2);//读数据
			Tower_Operation(563,buffer,2);//显示数据
			break;
		}
		case 564:
		{//显示吊高卷筒底部直径
			u8 buffer[2];
			Tower_Operation(64,buffer,2);//读数据
			Tower_Operation(564,buffer,2);//显示数据
			break;
		}
		case 565:
		{//显示吊高卷筒有效长度
			u8 buffer[2];
			Tower_Operation(65,buffer,2);//读数据
			Tower_Operation(565,buffer,2);//显示数据
			break;
		}
		case 566:
		{//显示吊高卷筒螺距
			u8 buffer[2];
			Tower_Operation(66,buffer,2);//读数据
			Tower_Operation(566,buffer,2);//显示数据
			break;
		}
		case 567:
		{//显示吊高钢丝绳直径
			u8 buffer[2];
			Tower_Operation(67,buffer,2);//读数据
			Tower_Operation(567,buffer,2);//显示数据
			break;
		}
		case 568:
		{//显示吊高钢丝绳层数
			u8 buffer;
			Tower_Operation(68,&buffer,1);//读数据
			Tower_Operation(568,&buffer,1);//显示数据
			break;
		}
		case 569:
		{//显示吊高钢丝绳有效转数
			u8 buffer[2];
			Tower_Operation(69,buffer,2);//读数据
			Tower_Operation(569,buffer,2);//显示数据
			break;
		}
		case 570:
		{//显示吊高钢丝绳倍率
			u8 buffer;
			Tower_Operation(70,&buffer,1);//读数据
			Tower_Operation(570,&buffer,1);//显示数据
			break;
		}
		case 571:
		{//显示吊高钢丝绳容量
			u8 buffer;
			Tower_Operation(71,&buffer,2);//读数据
			Tower_Operation(571,&buffer,2);//显示数据
			break;
		}
	}
}
//---
/*
	函数名： void Tower_DebugRTU((u8 *p)
	描述：		该函数为Tower文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*/
void Tower_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//查看是哪个功能码
	{
		//-----------------------------------------
		case 0x01:
		{
			break;
		}
	}  
}
//---





















