/*
	tower库函数
*/

#ifndef __TOWER_H
#define __TOWER_H

#include "stm32f10x.h"


#define TOWER_PI 				3.1415926
#define TOWER_EnDiv 		4096




//存储地址
//塔机参数存储地址
#define TOWER_AdrStoreBase					W25XXX_AdrTowerBase
#define TOWER_AdrStoreTJBase				0
#define TOWER_AdrStoreTJBH					TOWER_AdrStoreTJBase			//塔机编号
#define TOWER_AdrStoreTJXH					TOWER_AdrStoreTJBH+4			//塔机型号
#define TOWER_AdrStoreTJZZS					TOWER_AdrStoreTJXH+21			//塔机制造商
#define TOWER_AdrStoreTJBC					TOWER_AdrStoreTJZZS+21		//塔机臂长
#define TOWER_AdrStoreTJGD					TOWER_AdrStoreTJBC+2			//塔机高度
#define TOWER_AdrStoreTJEDZZ				TOWER_AdrStoreTJGD+2			//额定载重
#define TOWER_AdrStoreTJSDZZ				TOWER_AdrStoreTJEDZZ+4			//首端载重
#define TOWER_AdrStoreTJMDZZ				TOWER_AdrStoreTJSDZZ+4			//末端载重
#define TOWER_AdrStoreTJCCRQ				TOWER_AdrStoreTJMDZZ+4			//塔机出厂时间年月日
#define TOWER_AdrStoreTJSBGG				TOWER_AdrStoreTJCCRQ+4			//设备规格
#define TOWER_AdrStoreTJSBLX				TOWER_AdrStoreTJSBGG+10			//设备类型
#define TOWER_AdrStoreTJEnd					TOWER_AdrStoreTJSBLX+10			//塔机参数末端

//回转参数存储地址
#define TOWER_AdrStoreHZBase				TOWER_AdrStoreTJEnd					//回转参数存储基址
#define TOWER_AdrStoreHZDCLS				TOWER_AdrStoreHZBase				//回转大齿轮数
#define TOWER_AdrStoreHZXCLS				TOWER_AdrStoreHZDCLS+2			//回转小齿轮数
#define TOWER_AdrStoreHZEnd					TOWER_AdrStoreHZXCLS+2			//回转参数末端
//变幅参数存储地址
#define TOWER_AdrStoreBFBase				TOWER_AdrStoreHZEnd//70//TOWER_AdrStoreHZEnd					//变幅参数存储基址
#define TOWER_AdrStoreBFDCLS				TOWER_AdrStoreBFBase+0			//变幅大齿轮数
#define TOWER_AdrStoreBFXCLS				TOWER_AdrStoreBFDCLS+2			//变幅小齿轮数
#define TOWER_AdrStoreBFJTDBZJ			TOWER_AdrStoreBFXCLS+2			//变幅卷筒底部直径
#define TOWER_AdrStoreBFJTYXCD			TOWER_AdrStoreBFJTDBZJ+2		//变幅卷筒有效长度
#define TOWER_AdrStoreBFLJ					TOWER_AdrStoreBFJTYXCD+2		//变幅卷筒螺距
#define TOWER_AdrStoreBFGSSZJ				TOWER_AdrStoreBFLJ+2				//变幅钢丝绳直径
#define TOWER_AdrStoreBFGSSCS				TOWER_AdrStoreBFGSSZJ+2			//变幅钢丝绳层数
#define TOWER_AdrStoreBFYXZS				TOWER_AdrStoreBFGSSCS+1			//变幅钢丝绳有效转数
#define TOWER_AdrStoreBFEnd					TOWER_AdrStoreBFYXZS+2			//变幅参数末端
//吊高参数存储地址
#define TOWER_AdrStoreDGBase				TOWER_AdrStoreBFEnd					//吊高参数存储基址
#define TOWER_AdrStoreDGDCLS				TOWER_AdrStoreDGBase+0			//吊高大齿轮数
#define TOWER_AdrStoreDGXCLS				TOWER_AdrStoreDGDCLS+2			//吊高小齿轮数
#define TOWER_AdrStoreDGJTDBZJ			TOWER_AdrStoreDGXCLS+2			//吊高卷筒底部直径
#define TOWER_AdrStoreDGJTYXCD			TOWER_AdrStoreDGJTDBZJ+2		//吊高卷筒有效长度
#define TOWER_AdrStoreDGLJ					TOWER_AdrStoreDGJTYXCD+2		//吊高卷筒螺距
#define TOWER_AdrStoreDGGSSZJ				TOWER_AdrStoreDGLJ+2				//吊高钢丝绳直径
#define TOWER_AdrStoreDGGSSCS				TOWER_AdrStoreDGGSSZJ+2			//吊高钢丝绳层数
#define TOWER_AdrStoreDGYXZS				TOWER_AdrStoreDGGSSCS+1			//吊高钢丝绳有效转数
#define TOWER_AdrStoreDGGSSBL				TOWER_AdrStoreDGYXZS+2			//吊高钢丝绳倍率
#define TOWER_AdrStoreDGGSSRL				TOWER_AdrStoreDGGSSBL+1			//吊高钢丝绳容量
#define TOWER_AdrStoreDGEnd					TOWER_AdrStoreDGGSSRL+2			//吊高参数末端

#define TOWER_AdrStoreInit					TOWER_AdrStoreDGEnd				//初始化参数状态
#define TOWER_AdrStoreEnd						TOWER_AdrStoreInit+1			//


#define TOWER_AdrStoreADR						THBSS01_AdrConfBase
#define TOWER_AdrStoreBR						THBSS01_AdrStoreADR+1
//#define TOWER_AdrStoreEnd						THBSS01_AdrStoreBR+2


//基本数据机构提
//回转结构体
typedef struct
{
	u16 Manufacturer;		//塔机制造商
	u16 Version;				//塔机型号
	u32 SerialNumber;		//塔机编号
	float Length;				//塔机臂长，单位mm0
	float Height;				//塔机高度，单位mm
	float RatedLoad;		//额定载重，单位Kg
	float HeadLoad;			//首端载重，单位Kg
	float EndLoad;			//末端载重，单位Kg
	float Momental;			//力矩
	u16 UpHour;					//开机时间：小时（塔机自开机后经过的时间的小时部分，此时间须在关机后清零）
	u16 UpMinutes;			//开机时间：分钟
	u16 UpSec;					//开机时间：秒
	u16 WorkingHour;		//工作时间：小时（塔机自开机后有效工作的时间的小时部分，此时间须在关机后清零）
	u16 WorkingMinutes;	//工作时间：分钟
	u16 WorkingSec;			//工作时间：秒
}TOWER_BaseStructure;

//回转结构体
typedef struct
{
	u16 BigGear;//大齿轮
	u16 LittleGear;//小齿轮
	float Rotation;//角度，单位°
}TOWER_SlewingStructure;

//变幅结构体
typedef struct
{
	u16 BigGear;//大齿轮
	u16 LittleGear;//小齿轮
	u8 RopeLayer;//钢丝绳层数
	float CoilingDiameter;//卷筒直径,单位mm
	float RopeDiameter;//钢丝绳直径,单位mm
	float Range;//变幅,单位mm
}TOWER_TrolleyingStructure;

//吊高结构体
typedef struct
{
	u16 BigGear;//大齿轮
	u16 LittleGear;//小齿轮
	u8 RopeLayer;//钢丝绳层数
//	u8 LayerCnt;//钢丝绳层数
	u8 RopeRate;//钢丝绳倍率
	float CoilingDiameter;//卷筒直径
	float RopeDiameter;//钢丝绳直径
	float HookHeight;//吊高，单位mm
}TOWER_HoistingStructure;

//吊高结构体
typedef struct
{
	TOWER_BaseStructure Base;
	TOWER_SlewingStructure Slewing;
	TOWER_TrolleyingStructure Trolleying;
	TOWER_HoistingStructure Hoisting;
}TOWER_DataStructure;



//库函数
void Tower_Init(void);
void Tower_MachineInit(void);
void Tower_BaseInit(void);
void Tower_SlewingInit(void);
void Tower_TrolleyingInit(void);
void Tower_HoistingInit(void);
TOWER_BaseStructure* Tower_GetBaseData(void);
TOWER_SlewingStructure* Tower_GetSlewingData(void);
TOWER_TrolleyingStructure* Tower_GetTrolleyingData(void);
TOWER_HoistingStructure* Tower_GetHoistingData(void);

void Tower_Operation(u16 mode,u8 *p,u8 len);


void Tower_CalSlewing(s32 buffer);
void Tower_CalTrolleying(s32 buffer);
void Tower_CalHoisting(s32 buffer);
void Tower_CalMomental(void);

//存储相关
void Tower_ReadStoreData(u8 address,u8 *p,u8 len);
void Tower_StoreData(u8 address,u8 *p,u8 len);
void Tower_FactoryReset(void);
//调试函数
void Tower_DebugASCII(u8 *p,u8 len);
void Tower_DebugRTU(u8 *p,u8 len);
















#endif
