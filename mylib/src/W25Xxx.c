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
#include "W25Xxx.h"
//-------------------------------------------------------------
/*
	函数名：void W25Xxx_Init(void)
	描述：	该函数为W25Xxx文件初始化
*/
void W25Xxx_Init(void)
{
	W25Xxx_GPIOInit();
}

//-------------------------------------------------------------
/*
	第一层：硬件层
	该层的硬件电路连接是STM32F103单片机的SPI1串行口
	SIP端口配置为主机管脚配置
		SCK,推挽复用输出
		MOSI,推挽复用输出
		MISO,浮空输入或带上拉输入
		NSS,复用推挽输出
	当前工程使用到STM32的SPI2,管脚如下
		PB12	SPI2_NSS		FLASH_NSS
		PB13	SPI2_SCK		FLASH_SCK
		PB14	SPI2_MISO		FLASH_MISO
		PB15	SPI2_MOSI		FLASH_MOSI
*/
//---
/*
	函数名：void W25Xxx_GPIOInit(void)
	描述：该函数为W25Xxx相关控制管脚初始化
*/
void W25Xxx_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	//使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	//初始化管脚
	//管脚配置成SPI主模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//SPI2_NSS配置成推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;//SPI2_SCK、SPI2_MOSI配置成推挽复用输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//SPI2_MISO配置成上拉输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	W25XXX_CS_HIGH();//失能片选
	//SPI口具体配置
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主机
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	//SPI分频值，分频后的值为SCK的时钟频率
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//选择SPI通信时，是高位数据在前还是低位数据在前
  SPI_InitStructure.SPI_CRCPolynomial = 7;	//与CRC校验有关
	SPI_Init(SPI2, &SPI_InitStructure);
	/*使能SPI1 */
  SPI_Cmd(SPI2, ENABLE);
	W25Xxx_WakeUp();//唤醒W25Xxx
}

//---
/*
	函数名：void W25Xxx_ReadData(u8 *p,u32 Addr,u16 Len)
	描述：	该函数为从首地址Addr开始读取Len个数据
	输入：	u8 *p存储数据的首地址
					u32 Addr读取数据的首地址
					u16 Len读取数据的长度
*/
void W25Xxx_ReadData(u8 *p,u32 Addr,u16 Len)
{
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_ReadData);
	W25Xxx_SendByte((Addr & 0xFF0000) >> 16);
	W25Xxx_SendByte((Addr & 0xFF00) >> 8);
	W25Xxx_SendByte(Addr & 0xFF);
	while (Len--)
  {
		*p=W25Xxx_SendByte(W25XXX_DummyByte);
		p++;
	}
	W25XXX_CS_HIGH();
}
//---
/*
	函数名：void W25Xxx_PageWrite(u8 *p,u32 Addr,u16 Len)
	描述：页编程命令
	输入：u8 *p被写数据首地址
				u32 Addr写入的首地址
				u16 cnt写入的字节数
*/
void W25Xxx_PageWrite(u8 *p,u32 Addr,u16 Len)
{
	W25Xxx_WriteEnable();//允许写入
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_PageProgram);
	W25Xxx_SendByte((Addr & 0xFF0000) >> 16);
	W25Xxx_SendByte((Addr & 0xFF00) >> 8);
	W25Xxx_SendByte(Addr & 0xFF);
	if(Len > W25XXX_PerWritePageSize)
	{//若大于页编程长度，则缩短为页编程长度
		Len = W25XXX_PerWritePageSize;
	}
	while (Len--)
  {
		W25Xxx_SendByte(*p);
		p++;
	}
	W25XXX_CS_HIGH();
	W25Xxx_WaitForWriteEnd();//等等芯片操作完成
}
//---
/*
	函数名：void W25Xxx_EraseSector(u32 Addr)
	描述：擦除一个扇区
	输入：u32 Addr要擦除的扇区地址
*/
void W25Xxx_EraseSector(u32 Addr)
{
	W25Xxx_WriteEnable();
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_SectorErase);
	W25Xxx_SendByte((Addr & 0xFF0000) >> 16);
	W25Xxx_SendByte((Addr & 0xFF00) >> 8);
	W25Xxx_SendByte(Addr & 0xFF);
	W25XXX_CS_HIGH();
	W25Xxx_WaitForWriteEnd();//等等芯片操作完成
}
//---
/*
	函数名：void W25Xxx_EraseSector(u32 Addr)
	描述：擦除一个块区
	输入：u32 Addr要擦除的扇区地址
*/
void W25Xxx_EraseBlock(u32 Addr)
{
	W25Xxx_WriteEnable();
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_BlockErase);
	W25Xxx_SendByte((Addr & 0xFF0000) >> 16);
	W25Xxx_SendByte((Addr & 0xFF00) >> 8);
	W25Xxx_SendByte(Addr & 0xFF);
	W25XXX_CS_HIGH();
	W25Xxx_WaitForWriteEnd();//等等芯片操作完成
}
//---
/*
	函数名：void SPI_FLASH_ChipErase(void)
	描述：	芯片擦除
*/
void W25Xxx_EraseChip(void)
{
    W25Xxx_WriteEnable();
    W25XXX_CS_LOW();
    W25Xxx_SendByte(W25XXX_ChipErase);
    W25XXX_CS_HIGH();
    W25Xxx_WaitForWriteEnd();
}
//---
/*
	函数名：void W25Xxx_WriteEnable(void)
	描述：	W25Xxx芯片写使能
*/
void W25Xxx_WriteEnable(void)
{
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_WriteEnable);
	W25XXX_CS_HIGH();
}
//---
/*
	函数名：void W25Xxx_WriteDisable(void)
	描述：	W25Xxx芯片写失能
*/
void W25Xxx_WriteDisable(void)
{
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_WriteDisable);
	W25XXX_CS_HIGH();
}
//---
/*
	函数名：u8 W25Xxx_ReadStatusRegister(void)
	描述：	读状态寄存器
*/
u8 W25Xxx_ReadStatusRegister(void)
{
	u8 byte = 0;
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_ReadStatusReg);
	byte = W25Xxx_SendByte(W25XXX_DummyByte);
	W25XXX_CS_HIGH();
	return byte;
}

//---
/*
	函数名：void W25Xxx_WaitForWriteEnd(void)
	描述：循环读取SPI FLASH的状态寄存器判断写入是否执行结束
*/
void W25Xxx_WaitForWriteEnd(void)
{
	u8 Status = 0;
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_ReadStatusReg);
	do
  {
		Status = W25Xxx_SendByte(W25XXX_DummyByte);//读状态寄存器值
	}while ((Status & W25XXX_FlagBUSY) == SET);
	W25XXX_CS_HIGH();
}
//---
/*
	函数名：u32 W25Xxx_ReadDeviceID(void)
	描述：	读W25Xxx芯片ID号
@函数名 SPI_FLASH_ReadDeviceID
  * @功能   读取SPI FLASH设备ID
  *         Read one Device ID bytes
  * @参数   无
  * @返回值 一个字节的Device ID
*/
u8 W25Xxx_ReadDeviceID(void)
{
    u32 Temp = 0;
    W25XXX_CS_LOW();
    W25Xxx_SendByte(W25XXX_DeviceID);
    W25Xxx_SendByte(W25XXX_DummyByte);
    W25Xxx_SendByte(W25XXX_DummyByte);
    W25Xxx_SendByte(W25XXX_DummyByte);
    Temp = W25Xxx_SendByte(W25XXX_DummyByte);
    W25XXX_CS_HIGH();
    return Temp;
}
//---
/*
	函数名：u32 W25Xxx_ReadJEDECID(void)
	描述：	该函数是读取JEDEC指定的制造商ID和特定的设备ID
*/
u32 W25Xxx_ReadJEDECID(void)
{
	u32 ID=0;
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_JedecDeviceID);
	ID |= W25Xxx_SendByte(W25XXX_DummyByte)<<16;
	ID |= W25Xxx_SendByte(W25XXX_DummyByte)<<8;
	ID |= W25Xxx_SendByte(W25XXX_DummyByte);
	W25XXX_CS_HIGH();
	return ID;
}
//---
/*
	函数名：u8 W25Xxx_SendByte(u8 buffer)
	描述：该函数为通过SPI总线向W25XXX发送一个字节数据，顺便接收一个字节数据
	参数：要写入的一个字节数据
	返回值：在发送数据时，MISO信号线上接收一个字节数据
*/
u8 W25Xxx_SendByte(u8 buffer)
{
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);//等待发送缓冲区为空
	SPI_I2S_SendData(SPI2, buffer);//发送一个字节数据
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);//等待数据接收完毕
	return SPI_I2S_ReceiveData(SPI2);//返回接收到的数据
}
//---
/*
	函数名：void W25Xxx_PowerDown(void)
	描述：	使W25Xxx进入掉电模式
*/
void W25Xxx_PowerDown(void)
{
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_PowerDown);
	W25XXX_CS_HIGH();
}
//---
/*
	函数名：void W25Xxx_WakeUp(void)
	描述：	该函数为唤醒W25Xxx芯片
*/
void W25Xxx_WakeUp(void)
{
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_ReleasePowerDown);
	W25XXX_CS_HIGH();
}
//---
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------------------------------------------
//-------------------------调试函数----------------------------
//---
/*
	函数名： void W25Xxx_DebugASCII(u8 *p,u8 len)
	描述：		该函数为W25Xxx文件的ASCII调试文件
*/
void W25Xxx_DebugASCII(u8 *p,u8 len)
{//:000|61|001
	u16 cmd = Calculate_CharToU16(p,3);//计算命令
	switch(cmd)
	{
		case 1:
		{
			u8 buffer[256];
			u16 i;
			printf("读取存储芯片0~255数据\r\n");
			printf("\r\n");
			for(i=0;i<=255;i++)
			{
				buffer[i]=i;
				printf("%u ",buffer[i]);
			}
			printf("\r\n");
			W25Xxx_EraseSector(0);
			W25Xxx_PageWrite(buffer,0,256);
			for(i=0;i<=255;i++)
			{
				buffer[i]=0;
			}
			W25Xxx_ReadData(buffer,0,256);
			USART_SetNoneVerify();
			for(i=0;i<=255;i++)
			{
				printf("%u ",buffer[i]);
			}
			printf("\r\n");
			printf("读完毕\r\n");
			break;
		}
		case 2:
		{
			u8 buffer[256];
			u16 i;
			u8 block = 1;//块计算
			u8 sector = 1;//扇计算
			u8 page = 1;
			u8 byte = 0xDF;
			u32 Addr = block * 0x10000 + sector * 0x1000 +  page * 0x100 + byte;//块*扇*字节
			printf("第%u块第%u扇区第%u页的起始地址0x%X,地址:0x%X\r\n",block,sector,page,byte,Addr);
			W25Xxx_ReadData(buffer,Addr,256);
			USART_SetNoneVerify();
			for(i=0;i<=255;i++)
			{
				printf("%X ",buffer[i]);
			}
			printf("\r\n");
			printf("读完毕\r\n");
			printf("\r\n");
			for(i=0;i<256;i++)
			{
				buffer[i] = 0xBB;
			}
			W25Xxx_PageWrite(buffer,Addr,200);
			printf("编写完毕\r\n");
			printf("\r\n");
			W25Xxx_ReadData(buffer,Addr,256);
			USART_SetNoneVerify();
			for(i=0;i<=255;i++)
			{
				printf("%X ",buffer[i]);
			}
			printf("\r\n");
			printf("读完毕\r\n");
			break;
		}
		case 100://W25X16有32个块,W25X32有64个块,W25X64有128个块,每个块有16个扇区,每个扇区有4Kb
		{//扇区擦除		:000|61|100|001|16
			u8 block = Calculate_CharToU16(p+4,3);//计算命令
			u8 sector = Calculate_CharToU16(p+8,2);//计算命令
			u32 Addr = block * 0x10000 + sector * 0x1000;//块*扇*字节 
			printf("擦除第%u块第%u扇区,地址:0x%X\r\n",block,sector,Addr);
			W25Xxx_EraseSector(Addr);
			printf("擦除完毕\r\n");
			break;
		}
		case 101:
		{//执行块擦除		:000|61|102|000
			u8 block = Calculate_CharToU16(p+4,3);//计算命令
			u32 Addr = block * 0x10000;//块*扇*字节 
			printf("擦除第%u块,地址:0x%X\r\n",block,Addr);
			W25Xxx_EraseBlock(Addr);
			printf("擦除完毕\r\n");
			break;
		}
		case 102:
		{//芯片擦除		:000|61|102
			printf("芯片擦除\r\n");
			W25Xxx_EraseChip();
			printf("擦除完毕\r\n");
			break;
		}
		case 103://W25X16有32个块,W25X32有64个块,W25X64有128个块,每个块有16个扇区,每个扇区有4Kb,每个扇区有32页，每页有256个字节
		{//读页个数据		:000|61|103|001|16|21
			u8 block = Calculate_CharToU16(p+4,3);//块计算
			u8 sector = Calculate_CharToU16(p+8,2);//扇计算
			u8 page = Calculate_CharToU16(p+11,2);
			u32 Addr = block * 0x10000 + sector * 0x1000 +  page * 0x100;//块*扇*字节 
			u8 buffer[256];
			u16 i;
			printf("读取第%u块第%u扇区第%u页的256个数据,地址:0x%X\r\n",block,sector,page,Addr);
			W25Xxx_ReadData(buffer,Addr,256);
			for(i=0;i<=255;i++)
			{
				printf("%X ",buffer[i]);
			}
			printf("\r\n");
			printf("读取完毕\r\n");
			break;
		}
		case 104:
		{//页编程		:000|61|104|001|16|21|170
			u8 block = Calculate_CharToU16(p+4,3);//块计算
			u8 sector = Calculate_CharToU16(p+8,2);//扇计算
			u8 page = Calculate_CharToU16(p+11,2);
			u8 byte = Calculate_CharToU16(p+14,3);
			u32 Addr = block * 0x10000 + sector * 0x1000 +  page * 0x100;//块*扇*字节
			u8 buffer[256];
			u16 i;
			for(i=0;i<256;i++)
			{
				buffer[i] = byte;
			}
			printf("编程第%u块第%u扇区第%u页的256个数据写入%X,地址:0x%X\r\n",block,sector,page,byte,Addr);
			W25Xxx_PageWrite(buffer,Addr,256);
			printf("编写完毕\r\n");
			break;
		}
		case 105:
		{//读取硬件ID
			u8 ID = W25Xxx_ReadDeviceID();
			printf("串行存储芯片硬件ID为：0x%X\r\n",ID);
			if(ID == 0X14)
			{
				printf("该芯片为W25X16\r\n");
				printf("W25X16有2MB，共0~31个块，一个块有64KB，每个块有0~15扇区，一个扇区有4KB，每个扇区有0~15个页。\r\n");
			}
			else if(ID == 0X15)
			{
				printf("该芯片为W25X32\r\n");
				printf("W25X16有4MB，共0~63个块，一个块有64KB，每个块有0~15扇区，一个扇区有4KB，每个扇区有0~15个页。\r\n");
			}
			else if(ID == 0X16)
			{
				printf("该芯片为W25X64\r\n");
				printf("W25X16有8MB，共0~127个块，一个块有64KB，每个块有0~15扇区，一个扇区有4KB，每个扇区有0~15个页。\r\n");
			}
			break;
		}
		case 106:
		{//读取JEDEC ID
			u32 ID = W25Xxx_ReadJEDECID();
			printf("串行存储芯片硬件JEDEC ID为：0x%X\r\n",ID);
			break;
		}
		//---
	}
}
//---
/*
	函数名： void W25Xxx_DebugRTU((u8 *p)
	描述：		该函数为W25Xxx文件的RTU调试文件
	说明：		调用该函数之前需要先进行CRC校验
*/
void W25Xxx_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//查看是哪个功能码
	{
		//-----------------------------------------
		case 0x01:
		{//复位系统
			break;
		}
	}
}
//---
//---


