#include "sys.h"
#include "mmc_sd.h"			   
#include "spi.h"
#include "usart.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//Mini STM32开发板
//SD卡 驱动代码		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/1/10 
//版本：V2.0
//版权所有，盗版必究。
//Copyright(C) 正点原子 2009-2019
//All rights reserved
//********************************************************************************
//2012/1/10  
//大篇幅修改V1.0代码.参考fatfs代码移植过来.
////////////////////////////////////////////////////////////////////////////////// 
u8  SD_Type=0;//SD卡的类型 
SD_DataStructure SDData;
//---
/*
	函数名：Fan_DataStructure* SD_GetData(void)
	描述：获取返回MMC_SD.c文件全局变量结构体
*/
SD_DataStructure* SD_GetData(void)
 {
	 return &SDData;
 }
 //---


////////////////////////////////////移植修改区///////////////////////////////////
//移植时候的接口
//data:要写入的数据
//返回值:读到的数据
u8 SD_SPI_ReadWriteByte(u8 data)
{
	return SPIx_ReadWriteByte(data);
}
//SD卡初始化的时候,需要低速
void SD_SPI_SpeedLow(void)
{
 	SPIx_SetSpeed(SPI_SPEED_256);//设置到低速模式	
}
//SD卡正常工作的时候,可以高速了
void SD_SPI_SpeedHigh(void)
{
 	SPIx_SetSpeed(SPI_SPEED_4);//设置到高速模式	
}
//SPI硬件层初始化
void SD_SPI_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		//使能GPIOB时钟

	//GPIOB14
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;					//PA4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				//输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//初始化

	SPIx_Init();
 	SD_SPI_SpeedLow();//设置到低速模式	 
	SD_CS=1;   //不选中	
}
///////////////////////////////////////////////////////////////////////////////////
//取消选择,释放SPI总线
void SD_DisSelect(void)
{
	SD_CS=1;
	SD_SPI_ReadWriteByte(0xff);//提供额外的8个时钟
}
//选择sd卡,并且等待卡准备OK
//返回值:0,成功;1,失败;
u8 SD_Select(void)
{
	SD_CS=0;
	if(SD_WaitReady()==0)return 0;//等待成功
	SD_DisSelect();
	return 1;//等待失败
}
//等待卡准备好;状态为发送数据后返回0XFF；
//返回值:0,准备好了;其他,错误代码
u8 SD_WaitReady(void)
{
	u32 t=0;
	do
	{
		if(SD_SPI_ReadWriteByte(0XFF)==0XFF)return 0;//OK
		t++;		  	
	}while(t<0XFFFFFF);//等待 
	return 1;
}
//等待SD卡回应
//Response:要得到的回应值
//返回值:0,成功得到了该回应值
//    其他,得到回应值失败
u8 SD_GetResponse(u8 Response)
{
	u16 Count=0xFFF;//等待次数	   						  
	while ((SD_SPI_ReadWriteByte(0XFF)!=Response)&&Count)Count--;//等待得到准确的回应  	  
	if (Count==0)return MSD_RESPONSE_FAILURE;//得到回应失败   
	else return MSD_RESPONSE_NO_ERROR;//正确回应
}
//从sd卡读取一个数据包的内容
//buf:数据缓存区
//len:要读取的数据长度.
//返回值:0,成功;其他,失败;	
u8 SD_RecvData(u8*buf,u16 len)
{			  	  
	if(SD_GetResponse(0xFE))return 1;//等待SD卡发回数据起始令牌0xFE
    while(len--)//开始接收数据
    {
        *buf=SD_SPI_ReadWriteByte(0xFF);
        buf++;
    }
    //下面是2个伪CRC（dummy CRC）
    SD_SPI_ReadWriteByte(0xFF);
    SD_SPI_ReadWriteByte(0xFF);									  					    
    return 0;//读取成功
}
//向sd卡写入一个数据包的内容 512字节
//buf:数据缓存区
//cmd:指令
//返回值:0,成功;其他,失败;	
u8 SD_SendBlock(u8*buf,u8 cmd)
{	
	u16 t;		  	  
	if(SD_WaitReady())return 1;//等待准备失效
	SD_SPI_ReadWriteByte(cmd);
	if(cmd!=0XFD)//不是结束指令（停止指令）；
	{
		for(t=0;t<512;t++)SD_SPI_ReadWriteByte(buf[t]);//提高速度,减少函数传参时间
	    SD_SPI_ReadWriteByte(0xFF);//忽略crc（写入数据后将返回CRC，这里将其忽略；）
	    SD_SPI_ReadWriteByte(0xFF);
		t=SD_SPI_ReadWriteByte(0xFF);//接收响应
		if((t&0x1F)!=0x05)return 2;//响应错误（取低五位，响应应为0X05；）									  					    
	}						 									  					    
    return 0;//写入成功
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
//向SD卡发送一个命令
//输入: u8 cmd   命令 
//      u32 arg  命令参数
//      u8 crc   crc校验值	   
//返回值:SD卡返回的响应	 0XFF;片选失效 														  
u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc)
{
  u8 r1;	
	u8 Retry=0; 
	SD_DisSelect();             //取消上次片选；
	if(SD_Select())return 0XFF; //片选失效； 
	//发送
    SD_SPI_ReadWriteByte(cmd | 0x40);//分别写入命令；
    SD_SPI_ReadWriteByte(arg >> 24); //发送32为参数；
    SD_SPI_ReadWriteByte(arg >> 16);
    SD_SPI_ReadWriteByte(arg >> 8);
    SD_SPI_ReadWriteByte(arg);	  
    SD_SPI_ReadWriteByte(crc); 		 //最后一个字节包含第1至第5字节的CRC校验和；
	if(cmd==CMD12)SD_SPI_ReadWriteByte(0xff);//Skip a stuff byte when stop reading
    //等待响应，或超时退出；若最高位为0，则说明响应了
	Retry=0X1F;
	do
	{
		r1=SD_SPI_ReadWriteByte(0xFF);
	}while((r1&0X80) && Retry--);	 
	//返回状态值
    return r1;
}		    																			  
//获取SD卡的CID信息，包括制造商信息
//发送命令 CMD2 到每个卡以获取每个卡的唯一标识CID号;
//这个函数主要应用于多卡操作；或者想获取卡的CID的情况
//输入: u8 *cid_data(存放CID的内存，至少16Byte）	  
//返回值:0：NO_ERR
//		 1：错误														   
u8 SD_GetCID(u8 *cid_data)
{
    u8 r1;	   
    //发CMD10命令，读CID
    r1=SD_SendCmd(CMD10,0,0x01);
    if(r1==0x00)
	{
		r1=SD_RecvData(cid_data,16);//接收16个字节的数据	 
  }
	SD_DisSelect();//取消片选
	if(r1)return 1;
	else return 0;
}																				  
//获取SD卡的CSD信息，包括容量和速度信息
//输入:u8 *cid_data(存放CID的内存，至少16Byte)	    
//返回值:0：NO_ERR
//		 1：错误														   
u8 SD_GetCSD(u8 *csd_data)
{
    u8 r1;	 
    r1=SD_SendCmd(CMD9,0,0x01);//发CMD9命令，读CSD
    if(r1==0)
	  {
    	r1=SD_RecvData(csd_data, 16);//接收16个字节的数据，正确情况回应0x00；
    }
	SD_DisSelect();//取消片选
	if(r1)return 1;
	else return 0;
}  
//获取SD卡的总扇区数（扇区数）   
//返回值:0；取容量出错 
//其他:SD卡的容量(扇区数/512字节)
//每扇区的字节数必为512，因为如果不是512，则初始化不能通过.														  
u32 SD_GetSectorCount(void)
{
    u8 csd[16];
    u32 Capacity;  
    u8 n;
	  u16 csize;  					    
	//取CSD信息（容量：速度），如果期间出错，返回非0
    if(SD_GetCSD(csd)!=0) return 0;	    
    //如果为SDHC卡，按照下面方式计算
    if((csd[0]&0xC0)==0x40)	 //V2.00的卡
    {	
		csize = csd[9] + ((u16)csd[8] << 8) + 1;
		Capacity = (u32)csize << 10;//得到扇区数	 		   
    }else//V1.XX的卡
    {	
		n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
		csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
		Capacity= (u32)csize << (n - 9);//得到扇区数   
    }
    return Capacity;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
//初始化SD卡
//返回值:0,正常.
//其他,不正常.
u8 SD_Initialize(void)
{
  u8 r1;      // 存放SD卡的返回值
  u16 retry;  // 用来进行超时计数
  u8 buf[4];  
	u16 i;

	SD_SPI_Init();		//初始化IO
 	SD_SPI_SpeedLow();	//设置到低速模式 
    //for(i=0;i<0xf00;i++);//纯延时，等待SD卡上电完成	  
	for(i=0;i<10;i++)SD_SPI_ReadWriteByte(0XFF);//发送最少74个脉冲
	retry=20;
	do
	{
		r1=SD_SendCmd(CMD0,0,0x95);//进入IDLE状态，若返回0X01则进入idle状态；
	}while((r1!=0X01) && retry--);
 	SD_Type=0;//默认无卡
	if(r1==0X01)
	{
		if(SD_SendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0、、因为V2.0（协议）版本支持CMD8指令将返回0X01，（1；；这里代表空闲）；
		{
			for(i=0;i<4;i++)buf[i]=SD_SPI_ReadWriteByte(0XFF);	//Get trailing return value of R7 resp
			if(buf[2]==0X01&&buf[3]==0XAA)//卡是否支持2.7~3.6V
			{
				retry=0XFFFE;
				do
				{
					SD_SendCmd(CMD55,0,0X01);	//发送CMD55
					r1=SD_SendCmd(CMD41,0x40000000,0X01);//发送CMD41;回应应发送0x00,此时停止重复发送；
				}while(r1&&retry--);
				if(retry&&SD_SendCmd(CMD58,0,0X01)==0)//鉴别SD2.0卡版本开始
				{
					for(i=0;i<4;i++)buf[i]=SD_SPI_ReadWriteByte(0XFF);//得到OCR值
					if(buf[0]&0x40)SD_Type=SD_TYPE_V2HC;    //检查CCS（bit30）CCS=1 SDHC;CCS=0 SD2.0;
					else SD_Type=SD_TYPE_V2;   
				}
			}
		}else//SD V1.x/ MMC	V3//如果不是SD V2.0、、发送CMD8指令将返回0X05（即0X00001001），（前一位R1回应代表非法指令，而后一位1；代表空闲）；
		{
			SD_SendCmd(CMD55,0,0X01);		//发送CMD55
			r1=SD_SendCmd(CMD41,0,0X01);	//发送CMD41
			if(r1<=1)			 //r1<=1说明是SD_TYPE_V1
			{		
				SD_Type=SD_TYPE_V1;
				retry=0XFFFE;
				do //等待退出IDLE模式
				{
					SD_SendCmd(CMD55,0,0X01);	//发送CMD55
					r1=SD_SendCmd(CMD41,0,0X01);//发送CMD41
				}while(r1&&retry--);
			}else	//MMC卡不支持 CMD55+CMD41，识别（即指令非法，应该同样返回0X05，即r1>1；；则为SD_TYPE_MMC；
			{
				SD_Type=SD_TYPE_MMC;//MMC V3
				retry=0XFFFE;
				do //等待退出IDLE模式
				{											    
					r1=SD_SendCmd(CMD1,0,0X01);//发送CMD1
				}while(r1&&retry--);  
			}	 //当SD卡为 V1.x/ MMCV3时，对其进行块大小的设置
			if(retry==0||SD_SendCmd(CMD16,512,0X01)!=0)SD_Type=SD_TYPE_ERR;//错误的卡
		}
	}
	SD_DisSelect();//取消片选
	SD_SPI_SpeedHigh();//高速
	SDData.flag=0;
	if(SD_Type)
		{SDData.flag=1;
	   return 0;
	  }
	else if(r1){ return r1;} 	   
	return 0xaa;//其他错误
}
//读SD卡
//buf:数据缓存区
//sector:扇区
//cnt:扇区数
//返回值:0,ok;其他,失败.
u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt)
{
	u8 r1;
	if(SD_Type!=SD_TYPE_V2HC)
	sector <<= 9;//转换为字节地址；SD_Type被定义为全局变量；这里只对非SD_TYPE_V2HC，进行操作；
	if(cnt==1)
	{
		r1=SD_SendCmd(CMD17,sector,0X01);//读一个块命令
		if(r1==0)//指令发送成功
		{
			r1=SD_RecvData(buf,512);//接收512个字节	   
		}
	}else
	{
		r1=SD_SendCmd(CMD18,sector,0X01);//连续读块命令（读多个块数据的命令）
		do
		{
			r1=SD_RecvData(buf,512);//接收512个字节；正确情况返回0（成功）；	 
			buf+=512;  
		}while(--cnt && r1==0); 	
		SD_SendCmd(CMD12,0,0X01);	//发送停止命令；；在这里停止通信被要求，所以下面的写数据块函数应传输停止块！
	}   
	SD_DisSelect();//取消片选
	return r1;//
}
//写SD卡
//buf:数据缓存区
//sector:起始扇区
//cnt:扇区数，这里不能取0；
//返回值:0,ok;其他,失败.
u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt)
{
	u8 r1;
	if(SD_Type!=SD_TYPE_V2HC)sector *= 512;//转换为字节地址
	if(cnt==1)
	{
		r1=SD_SendCmd(CMD24,sector,0X01);//读命令
		if(r1==0)//指令发送成功
		{
			r1=SD_SendBlock(buf,0xFE);//写512个字节	，这里的0XFE为写整个块数据的起始令牌；
		}
	}else
	{
		if(SD_Type!=SD_TYPE_MMC)
		{
			SD_SendCmd(CMD55,0,0X01);	
			SD_SendCmd(CMD23,cnt,0X01);//发送指令；；SD_TYPE_MMC卡，写CMD23,cnt，，先进行预擦除；；	
		}
 		r1=SD_SendCmd(CMD25,sector,0X01);//连续读块命令
		if(r1==0)
		{
			do
			{
			   r1=SD_SendBlock(buf,0xFC);//接收512个字节；这里的0XFC为多个块数据写起始令牌；	 
			   buf+=512;  
			}while(--cnt && r1==0);
			r1=SD_SendBlock(0,0xFD);//在上面读数据块的函数中，停止通信被要求，故应发送停止块！
		}
	}   
	SD_DisSelect();//取消片选
	return r1;//
}	   
























	  






