#include "ScreenDrive.h"
#include "Screenlogic.h"  
	 
extern Screen_DataStructure ScreenData;
//---
/*
	函数名：void Screen_ShowPage(u16 page)
	描述：	显示指定页面5A A5 06 80 03 0005 CRCH CRCL
*/
void Screen_ShowPage(u16 page)
{
	u8 bf[10];
	u8 cnt=0;
	u8 i;
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	ScreenData.OldPage = ScreenData.Page;
	ScreenData.Page = page;
	bf[cnt++] = 0x80;
	bf[cnt++] = 0x03;
	bf[cnt++] = page>>8;
	bf[cnt++] = page;
	#ifdef SCREE_CRC
	crc = CRC16(bf,cnt);
	bf[cnt++] = crc>>8;
	bf[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,bf[i]);
	}
	//---------------
//	ScreenData.Key.OldOpt = 0;//更新界面后，重新对焦
	//---------------
}
//---
/*
	函数名：void Screen_RefreshDataMulti(u16 address,u8 *p,u8 len)
	描述：向屏幕变量存储器发送多个数据
	输入：address 地址
				p数据地址
				len 数据长度，长度大小为小于等于195个字符
*/
void Screen_RefreshDataMulti(u16 address,u8 *p,u8 len)
{
	u8 buffer[80];
	u8 cnt=0;
	u8 i;
	StrinGalignment(p,len);
	len=strlen((const char* )p);
	
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x82;
	buffer[cnt++] = address>>8;
	buffer[cnt++] = address;
	for(i=0;i<len;i++)
	{
		buffer[cnt++] = *p;
		p++;
	}
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
	Screen_ResetDataBuffer();
}
//---
/*
	函数名：void Screen_RefreshVarData(u16 address,u16 data)
	描述：向屏幕变量存储器发送位图选项
	输入：address 地址
				data数据
			
*/
void Screen_RefreshVarData(u16 address,u16 data)
{
	u8 buffer[20];
	u8 cnt=0;
	u8 i;

	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x82;
	buffer[cnt++] = address>>8;
	buffer[cnt++] = address;
	buffer[cnt++] = data>>8;
	buffer[cnt++] = data;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
}
//---
/*
	函数名：void Screen_Reset(void)
	描述：复位屏幕5A A5 04 80 EE 5A A5
*/
void Screen_Reset(void)
{
	u8 buffer[10];
	u8 cnt=0;
	u8 i;
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x80;
	buffer[cnt++] = 0xEE;
	buffer[cnt++] = 0x5A;
	buffer[cnt++] = 0xA5;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	#ifdef SCREE_CRC
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x06);
	#else
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,4);
	#endif
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
}
//---
/*
	函数名：void Screen_ResetDataBuffer(void)
	描述：复位ScreenData.Bfffer数组
*/
void Screen_ResetDataBuffer(void)
{
	for(ScreenData.Cnt=0;ScreenData.Cnt<SCREEN_BFLEN;ScreenData.Cnt++)
		ScreenData.Buffer[ScreenData.Cnt]=0;
	ScreenData.Cnt = 0;
	ScreenData.Con = 0;
}
//---
/*
	函数名：void Screen_ResetKeyDataBuffer(void)
	描述：复位ScreenData.Bfffer数组
*/
void Screen_ResetKeyDataBuffer(void)
{
	u8 i;
	for(i=0; i<30;i++) ScreenData.Key.Buffer[i] = 0;
}
//---
/*
	函数名：void Screen_WriteRegister(u8 address,u8 len,u8 *p)
	描述：	向屏幕寄存器写数据
	输入：	address：写入寄存器起始地址
					len：写入数据长度，长度小于等于195
					*p：被写入数据指针首地址
*/
void Screen_WriteRegister(u8 address,u8 len,u8 *p)
{
	u8 buffer[60]; //修改与20200310 修改200
	u8 cnt=0;
	u8 i;
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x80;
	buffer[cnt++] = address;
	for(i=0;i<len;i++)
	{
		buffer[cnt++] = p[i];
	}
	buffer[cnt++] = len;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
}
//---
/*
	函数名：void Screen_WriteRegisterOneData(u8 address,u8 data)
	描述：	向屏幕寄存器写一个字节数据
	输入：	address：写入寄存器起始地址
					data：被写入的数据
*/
void Screen_WriteRegisterOneData(u8 address,u8 data)
{
	u8 buffer[20];
	u8 cnt=0;
	u8 i;
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x80;
	buffer[cnt++] = address;
	buffer[cnt++] = data;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
	
	if(address==0x4f)   //特别处理发弹出菜单时
	{
		Screen_GetData()->Scan_Stimer=100;  //100 ms后开始访问
 		Screen_GetData()->Scan_status=Screen_GetData()->Scan_status|0x02;  //启动检测
		Screen_GetData()->Scan_status=Screen_GetData()->Scan_status|0x01;  
	}
}
//---
/*
	函数名：void Screen_ReadRegister(u8 address,u8 len)
描述：	向屏幕发送读取寄存器数据命令
	输入：	address：读寄存器地址
					len：读数据长度
*/
void Screen_ReadRegister(u8 address,u8 len)
{
	u8 buffer[20];
	u8 cnt=0;
	u8 i;
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x81;
	buffer[cnt++] = address;
	buffer[cnt++] = len;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
}
//---
/*
	函数名：void Screen_PastePicture(u16 address,u16 pid,u32 ctlc,u32 clrc,u32 ptlc)
	描述：	该函数为将pid页的图片区域为左上角ctlc至右下角clrc的区域复制到当前页面的左上角为ptlc位置
					对应迪文屏幕的“基本图形显示”控件的“图片区域剪切、黏贴"指令
	输入：	address：基本图形显示变量变量地址
					pid：被复制图片页面ID(Picture ID)
					ctlc：被复制图片页面左上角XY坐标，高字为X坐标，低字为Y坐标(copy top left corner)
					clrc：被复制图片页面右下角XY坐标，高字为X坐标，低字为Y坐标(copy lower right corner)
					ptlc：黏贴当前页面左上角XY坐标，高字为X坐标，低字为Y坐标(paste top left corner)
*/
void Screen_PastePicture(u16 address,u16 pid,u32 ctlc,u32 clrc,u32 ptlc)
{
	u8 buffer[30];
	u8 cnt=0;
	u8 i;
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x82;
	buffer[cnt++] = address>>8;
	buffer[cnt++] = address;
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x06;
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x01;
	buffer[cnt++] = pid>>8;
	buffer[cnt++] = pid;
	buffer[cnt++] = ctlc>>24;
	buffer[cnt++] = ctlc>>16;
	buffer[cnt++] = ctlc>>8;
	buffer[cnt++] = ctlc;
	buffer[cnt++] = clrc>>24;
	buffer[cnt++] = clrc>>16;
	buffer[cnt++] = clrc>>8;
	buffer[cnt++] = clrc;
	buffer[cnt++] = ptlc>>24;
	buffer[cnt++] = ptlc>>16;
	buffer[cnt++] = ptlc>>8;
	buffer[cnt++] = ptlc;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
}
//---
/*
	函数名：void Screen_RectangleFilling(u16 address,u32 ctlc,u32 clrc,u16 colour)
	描述：	该函数为将address地址“基本图形变量”控件进行矩形填充
	输入：	address：基本图形显示变量变量地址
					ctlc：填充位置的左上角XY坐标，高字为X坐标，低字为Y坐标(copy top left corner)
					clrc：填充位置的右下角XY坐标，高字为X坐标，低字为Y坐标(copy lower right corner)
					colour：填充的颜色
*/
void Screen_RectangleFilling(u16 address,u32 ctlc,u32 clrc,u16 colour)
{
	u8 buffer[30];
	u8 cnt=0;
	u8 i;
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x82;
	buffer[cnt++] = address>>8;
	buffer[cnt++] = address;
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x04;//矩形域填充命令
	buffer[cnt++] = 0x00;
	buffer[cnt++] = 0x01;//数目包数
	buffer[cnt++] = ctlc>>24;
	buffer[cnt++] = ctlc>>16;
	buffer[cnt++] = ctlc>>8;
	buffer[cnt++] = ctlc;
	buffer[cnt++] = clrc>>24;
	buffer[cnt++] = clrc>>16;
	buffer[cnt++] = clrc>>8;
	buffer[cnt++] = clrc;
	buffer[cnt++] = colour>>8;
	buffer[cnt++] = colour;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
}
//---
/*
	函数名：void Screen_SetClock(u16 year,u8 month,u8 day,u8 hour,u8 minute,u8 sec)
	描述：	该函数为设置时间
	输入：	年月日时分秒
					年：2000~2099
					月：1~12
					日：1~31
					时：0~23
					分：0~59
					秒：0~59
	说明：	无需转换成屏幕的时间格式
	命令样式：	5A A5 0A 80 1F 5A 16 01 25 00 17 08 20（2016年1月25日17:08:20）
	说明：	
	输入：	struct tm *sTime 被设置时间指针
*/
void Screen_SetClock(struct tm *sTime)//u16 year,u8 month,u8 day,u8 hour,u8 minute,u8 sec)
{
	u8 buffer[20];
	u8 cnt=0;
	u8 i;
	u8 bf = sTime->tm_year - SCREEN_YearScreen;	//(year - SCREEN_YearBase) - SCREEN_YearScreen;//转换为迪文屏幕年
	#ifdef SCREE_CRC
	u16 crc;
	#endif
	buffer[cnt++] = 0x80;
	buffer[cnt++] = 0x1F;
	buffer[cnt++] = 0x5A;
	buffer[cnt++] = (bf/10)*0x10+bf%10;
	buffer[cnt++] = ((sTime->tm_mon + 1) / 10) * 0x10 + (sTime->tm_mon + 1) % 10;
	buffer[cnt++] = (sTime->tm_mday / 10) * 0x10 + sTime->tm_mday % 10;
	buffer[cnt++] = 0;
	buffer[cnt++] = (sTime->tm_hour / 10) * 0x10 + sTime->tm_hour % 10;
	buffer[cnt++] = (sTime->tm_min / 10) * 0x10 + sTime->tm_min % 10;
	buffer[cnt++] = (sTime->tm_sec /10) * 0x10 + sTime->tm_sec % 10;
	#ifdef SCREE_CRC
	crc = CRC16(buffer,cnt);
	buffer[cnt++] = crc>>8;
	buffer[cnt++] = crc;
	#endif
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0x5A);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,0xA5);
	SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,cnt);
	for(i=0;i<cnt;i++)
	{
		SCREEN_SendBufferData(SCREEN_UsartPort,SCREEN_UsartBuffer,buffer[i]);
	}
}


