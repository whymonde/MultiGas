/*
	该文件创建于2014年11月19日
	该文件为用于存储芯片W25X16/32/64的驱动和应用接口
	说明：文件包括存储芯片的硬件层、驱动层和接口层（应用层）
	硬件层：该层为与单片机有关，其为驱动层提供物理接口，主要包括IO口的初始化，外
					设的初始化，以及给驱动层提供的接口，根据不同的单片机，只需要修改该层
					即可，方便移植。
	驱动层：该层为与芯片W25Xxx芯片有关，该层包括所有驱动芯片的最小控制指令以及算法
					包括发送命令，写入数据，存储数据等所有的操作
	应用层：该层为为外部应用提供标准接口，方便使用，使使用者不需要了解驱动层的所有过程，
					只需按照接口写入数据即可
	W25Xxx芯片简单介绍：W25X16/32/64为华邦电子生产的SPI接口flash存储芯片，容量分别是16M-bit,
					32M-bit,64M-bit,该系列芯片支持最大75MHz的时钟总线。
					
	
*/
#ifndef __W25XXX_H
#define __W25XXX_H
#include "stm32f10x.h"
//命令表
#define W25XXX_WriteEnable                0x06 		//写使能命令
#define W25XXX_WriteDisable               0x04 		//写禁能命令
#define W25XXX_ReadStatusReg              0x05 		//读状态寄存器命令
#define W25XXX_WriteStatusReg             0x01 		//写状态寄存器命令
#define W25XXX_ReadData                   0x03 		//读数据命令
#define W25XXX_FastReadData               0x0B 		//快读指令
#define W25XXX_FastReadDual               0x3B 		//快读双输出指令
#define W25XXX_PageProgram                0x02 		//页编程指令
#define W25XXX_SectorErase                0x20 		//扇区擦除指令
#define W25XXX_BlockErase                 0xD8 		//块擦除指令
#define W25XXX_ChipErase                  0xC7 		//芯片擦除指令
#define W25XXX_PowerDown                  0xB9 		//掉电指令
#define W25XXX_ReleasePowerDown           0xAB 		//释放掉电/器件ID指令
#define W25XXX_DeviceID                   0xAB 		//释放掉电/器件ID指令
#define W25XXX_ManufactDeviceID           0x90 		//读制造/器件号指令
#define W25XXX_JedecDeviceID              0x9F 		//JEDEC ID指令

#define W25XXX_PageSize              			256			//页长度
#define W25XXX_PerWritePageSize      			256			//页编程长度

#define W25XXX_FlagBUSY                   0x01  	/* Write In Progress (WIP) flag */
#define W25XXX_DummyByte                  0xAA		//虚假数据，用于读取数据

#define W25XXX_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)	//片选引脚/CS拉低
#define W25XXX_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)  //片选引脚/CS拉高


//存储地址（每一组基于一个扇区）
#define W25XXX_Adr_CraneBase							0x000000		//塔机存储基址
#define W25XXX_Adr_Slewing								0x1000


#define W25XXX_AdrTowerBase								0x1000*0		//塔机参数存储基址
#define W25XXX_AdrMCH58Base								0x1000*1		//编码器存储基址
#define W25XXX_AdrTHBSS01Base							0x1000*2		//称重存储基址
#define W25XXX_AdrOHRD23Base							0x1000*3		//电量存储基址
#define W25XXX_AdrYCFS260Base							0x1000*4		//风速存储基址
#define W25XXX_AdrDS18B20Base							0x1000*5		//温度存储基址
#define W25XXX_AdrSystemBase							0x1000*6		//系统存储基址
#define W25XXX_AdrPAC193XmBase						0x1000*7		//AC193X存储基址




//库函数
void W25Xxx_Init(void);

//硬件层
void W25Xxx_GPIOInit(void);

void W25Xxx_ReadData(u8 *p,u32 Addr,u16 Len);
void W25Xxx_PageWrite(u8 *p,u32 Addr,u16 Len);
void W25Xxx_EraseSector(u32 Addr);
void W25Xxx_EraseBlock(u32 Addr);
void W25Xxx_EraseChip(void);

void W25Xxx_WriteEnable(void);
void W25Xxx_WriteDisable(void);
u8 W25Xxx_ReadStatusRegister(void);
void W25Xxx_WaitForWriteEnd(void);
u8 W25Xxx_ReadDeviceID(void);
u32 W25Xxx_ReadJEDECID(void);
u8 W25Xxx_SendByte(u8 buffer);
void W25Xxx_PowerDown(void);
void W25Xxx_WakeUp(void);

//调试函数
void W25Xxx_DebugASCII(u8 *p,u8 len);
void W25Xxx_DebugRTU(u8 *p,u8 len);
#endif


