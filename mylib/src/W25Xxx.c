/*
	���ļ�������2014��11��19��
	���ļ�Ϊ���ڴ洢оƬW25X16/32/64��������Ӧ�ýӿ�
	˵�����ļ������洢оƬ��Ӳ���㡢������ͽӿڲ㣨Ӧ�ò㣩
	Ӳ���㣺�ò�Ϊ�뵥Ƭ���йأ���Ϊ�������ṩ����ӿڣ���Ҫ����IO�ڵĳ�ʼ������
					��ĳ�ʼ�����Լ����������ṩ�Ľӿڣ����ݲ�ͬ�ĵ�Ƭ����ֻ��Ҫ�޸ĸò�
					���ɣ�������ֲ��
	�����㣺�ò�Ϊ��оƬW25XxxоƬ�йأ��ò������������оƬ����С����ָ���Լ��㷨
					�����������д�����ݣ��洢���ݵ����еĲ���
	Ӧ�ò㣺�ò�ΪΪ�ⲿӦ���ṩ��׼�ӿڣ�����ʹ�ã�ʹʹ���߲���Ҫ�˽�����������й��̣�
					ֻ�谴�սӿ�д�����ݼ���
	W25XxxоƬ�򵥽��ܣ�W25X16/32/64Ϊ�������������SPI�ӿ�flash�洢оƬ�������ֱ���16M-bit,
					32M-bit,64M-bit,��ϵ��оƬ֧�����75MHz��ʱ�����ߡ�
*/
#include "W25Xxx.h"
//-------------------------------------------------------------
/*
	��������void W25Xxx_Init(void)
	������	�ú���ΪW25Xxx�ļ���ʼ��
*/
void W25Xxx_Init(void)
{
	W25Xxx_GPIOInit();
}

//-------------------------------------------------------------
/*
	��һ�㣺Ӳ����
	�ò��Ӳ����·������STM32F103��Ƭ����SPI1���п�
	SIP�˿�����Ϊ�����ܽ�����
		SCK,���츴�����
		MOSI,���츴�����
		MISO,������������������
		NSS,�����������
	��ǰ����ʹ�õ�STM32��SPI2,�ܽ�����
		PB12	SPI2_NSS		FLASH_NSS
		PB13	SPI2_SCK		FLASH_SCK
		PB14	SPI2_MISO		FLASH_MISO
		PB15	SPI2_MOSI		FLASH_MOSI
*/
//---
/*
	��������void W25Xxx_GPIOInit(void)
	�������ú���ΪW25Xxx��ؿ��ƹܽų�ʼ��
*/
void W25Xxx_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	//ʹ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	//��ʼ���ܽ�
	//�ܽ����ó�SPI��ģʽ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//SPI2_NSS���ó��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;//SPI2_SCK��SPI2_MOSI���ó����츴�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//SPI2_MISO���ó���������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	W25XXX_CS_HIGH();//ʧ��Ƭѡ
	//SPI�ھ�������
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;	//SPI��Ƶֵ����Ƶ���ֵΪSCK��ʱ��Ƶ��
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ѡ��SPIͨ��ʱ���Ǹ�λ������ǰ���ǵ�λ������ǰ
  SPI_InitStructure.SPI_CRCPolynomial = 7;	//��CRCУ���й�
	SPI_Init(SPI2, &SPI_InitStructure);
	/*ʹ��SPI1 */
  SPI_Cmd(SPI2, ENABLE);
	W25Xxx_WakeUp();//����W25Xxx
}

//---
/*
	��������void W25Xxx_ReadData(u8 *p,u32 Addr,u16 Len)
	������	�ú���Ϊ���׵�ַAddr��ʼ��ȡLen������
	���룺	u8 *p�洢���ݵ��׵�ַ
					u32 Addr��ȡ���ݵ��׵�ַ
					u16 Len��ȡ���ݵĳ���
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
	��������void W25Xxx_PageWrite(u8 *p,u32 Addr,u16 Len)
	������ҳ�������
	���룺u8 *p��д�����׵�ַ
				u32 Addrд����׵�ַ
				u16 cntд����ֽ���
*/
void W25Xxx_PageWrite(u8 *p,u32 Addr,u16 Len)
{
	W25Xxx_WriteEnable();//����д��
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_PageProgram);
	W25Xxx_SendByte((Addr & 0xFF0000) >> 16);
	W25Xxx_SendByte((Addr & 0xFF00) >> 8);
	W25Xxx_SendByte(Addr & 0xFF);
	if(Len > W25XXX_PerWritePageSize)
	{//������ҳ��̳��ȣ�������Ϊҳ��̳���
		Len = W25XXX_PerWritePageSize;
	}
	while (Len--)
  {
		W25Xxx_SendByte(*p);
		p++;
	}
	W25XXX_CS_HIGH();
	W25Xxx_WaitForWriteEnd();//�ȵ�оƬ�������
}
//---
/*
	��������void W25Xxx_EraseSector(u32 Addr)
	����������һ������
	���룺u32 AddrҪ������������ַ
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
	W25Xxx_WaitForWriteEnd();//�ȵ�оƬ�������
}
//---
/*
	��������void W25Xxx_EraseSector(u32 Addr)
	����������һ������
	���룺u32 AddrҪ������������ַ
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
	W25Xxx_WaitForWriteEnd();//�ȵ�оƬ�������
}
//---
/*
	��������void SPI_FLASH_ChipErase(void)
	������	оƬ����
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
	��������void W25Xxx_WriteEnable(void)
	������	W25XxxоƬдʹ��
*/
void W25Xxx_WriteEnable(void)
{
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_WriteEnable);
	W25XXX_CS_HIGH();
}
//---
/*
	��������void W25Xxx_WriteDisable(void)
	������	W25XxxоƬдʧ��
*/
void W25Xxx_WriteDisable(void)
{
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_WriteDisable);
	W25XXX_CS_HIGH();
}
//---
/*
	��������u8 W25Xxx_ReadStatusRegister(void)
	������	��״̬�Ĵ���
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
	��������void W25Xxx_WaitForWriteEnd(void)
	������ѭ����ȡSPI FLASH��״̬�Ĵ����ж�д���Ƿ�ִ�н���
*/
void W25Xxx_WaitForWriteEnd(void)
{
	u8 Status = 0;
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_ReadStatusReg);
	do
  {
		Status = W25Xxx_SendByte(W25XXX_DummyByte);//��״̬�Ĵ���ֵ
	}while ((Status & W25XXX_FlagBUSY) == SET);
	W25XXX_CS_HIGH();
}
//---
/*
	��������u32 W25Xxx_ReadDeviceID(void)
	������	��W25XxxоƬID��
@������ SPI_FLASH_ReadDeviceID
  * @����   ��ȡSPI FLASH�豸ID
  *         Read one Device ID bytes
  * @����   ��
  * @����ֵ һ���ֽڵ�Device ID
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
	��������u32 W25Xxx_ReadJEDECID(void)
	������	�ú����Ƕ�ȡJEDECָ����������ID���ض����豸ID
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
	��������u8 W25Xxx_SendByte(u8 buffer)
	�������ú���Ϊͨ��SPI������W25XXX����һ���ֽ����ݣ�˳�����һ���ֽ�����
	������Ҫд���һ���ֽ�����
	����ֵ���ڷ�������ʱ��MISO�ź����Ͻ���һ���ֽ�����
*/
u8 W25Xxx_SendByte(u8 buffer)
{
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);//�ȴ����ͻ�����Ϊ��
	SPI_I2S_SendData(SPI2, buffer);//����һ���ֽ�����
  while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);//�ȴ����ݽ������
	return SPI_I2S_ReceiveData(SPI2);//���ؽ��յ�������
}
//---
/*
	��������void W25Xxx_PowerDown(void)
	������	ʹW25Xxx�������ģʽ
*/
void W25Xxx_PowerDown(void)
{
	W25XXX_CS_LOW();
	W25Xxx_SendByte(W25XXX_PowerDown);
	W25XXX_CS_HIGH();
}
//---
/*
	��������void W25Xxx_WakeUp(void)
	������	�ú���Ϊ����W25XxxоƬ
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
//-------------------------���Ժ���----------------------------
//---
/*
	�������� void W25Xxx_DebugASCII(u8 *p,u8 len)
	������		�ú���ΪW25Xxx�ļ���ASCII�����ļ�
*/
void W25Xxx_DebugASCII(u8 *p,u8 len)
{//:000|61|001
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:
		{
			u8 buffer[256];
			u16 i;
			printf("��ȡ�洢оƬ0~255����\r\n");
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
			printf("�����\r\n");
			break;
		}
		case 2:
		{
			u8 buffer[256];
			u16 i;
			u8 block = 1;//�����
			u8 sector = 1;//�ȼ���
			u8 page = 1;
			u8 byte = 0xDF;
			u32 Addr = block * 0x10000 + sector * 0x1000 +  page * 0x100 + byte;//��*��*�ֽ�
			printf("��%u���%u������%uҳ����ʼ��ַ0x%X,��ַ:0x%X\r\n",block,sector,page,byte,Addr);
			W25Xxx_ReadData(buffer,Addr,256);
			USART_SetNoneVerify();
			for(i=0;i<=255;i++)
			{
				printf("%X ",buffer[i]);
			}
			printf("\r\n");
			printf("�����\r\n");
			printf("\r\n");
			for(i=0;i<256;i++)
			{
				buffer[i] = 0xBB;
			}
			W25Xxx_PageWrite(buffer,Addr,200);
			printf("��д���\r\n");
			printf("\r\n");
			W25Xxx_ReadData(buffer,Addr,256);
			USART_SetNoneVerify();
			for(i=0;i<=255;i++)
			{
				printf("%X ",buffer[i]);
			}
			printf("\r\n");
			printf("�����\r\n");
			break;
		}
		case 100://W25X16��32����,W25X32��64����,W25X64��128����,ÿ������16������,ÿ��������4Kb
		{//��������		:000|61|100|001|16
			u8 block = Calculate_CharToU16(p+4,3);//��������
			u8 sector = Calculate_CharToU16(p+8,2);//��������
			u32 Addr = block * 0x10000 + sector * 0x1000;//��*��*�ֽ� 
			printf("������%u���%u����,��ַ:0x%X\r\n",block,sector,Addr);
			W25Xxx_EraseSector(Addr);
			printf("�������\r\n");
			break;
		}
		case 101:
		{//ִ�п����		:000|61|102|000
			u8 block = Calculate_CharToU16(p+4,3);//��������
			u32 Addr = block * 0x10000;//��*��*�ֽ� 
			printf("������%u��,��ַ:0x%X\r\n",block,Addr);
			W25Xxx_EraseBlock(Addr);
			printf("�������\r\n");
			break;
		}
		case 102:
		{//оƬ����		:000|61|102
			printf("оƬ����\r\n");
			W25Xxx_EraseChip();
			printf("�������\r\n");
			break;
		}
		case 103://W25X16��32����,W25X32��64����,W25X64��128����,ÿ������16������,ÿ��������4Kb,ÿ��������32ҳ��ÿҳ��256���ֽ�
		{//��ҳ������		:000|61|103|001|16|21
			u8 block = Calculate_CharToU16(p+4,3);//�����
			u8 sector = Calculate_CharToU16(p+8,2);//�ȼ���
			u8 page = Calculate_CharToU16(p+11,2);
			u32 Addr = block * 0x10000 + sector * 0x1000 +  page * 0x100;//��*��*�ֽ� 
			u8 buffer[256];
			u16 i;
			printf("��ȡ��%u���%u������%uҳ��256������,��ַ:0x%X\r\n",block,sector,page,Addr);
			W25Xxx_ReadData(buffer,Addr,256);
			for(i=0;i<=255;i++)
			{
				printf("%X ",buffer[i]);
			}
			printf("\r\n");
			printf("��ȡ���\r\n");
			break;
		}
		case 104:
		{//ҳ���		:000|61|104|001|16|21|170
			u8 block = Calculate_CharToU16(p+4,3);//�����
			u8 sector = Calculate_CharToU16(p+8,2);//�ȼ���
			u8 page = Calculate_CharToU16(p+11,2);
			u8 byte = Calculate_CharToU16(p+14,3);
			u32 Addr = block * 0x10000 + sector * 0x1000 +  page * 0x100;//��*��*�ֽ�
			u8 buffer[256];
			u16 i;
			for(i=0;i<256;i++)
			{
				buffer[i] = byte;
			}
			printf("��̵�%u���%u������%uҳ��256������д��%X,��ַ:0x%X\r\n",block,sector,page,byte,Addr);
			W25Xxx_PageWrite(buffer,Addr,256);
			printf("��д���\r\n");
			break;
		}
		case 105:
		{//��ȡӲ��ID
			u8 ID = W25Xxx_ReadDeviceID();
			printf("���д洢оƬӲ��IDΪ��0x%X\r\n",ID);
			if(ID == 0X14)
			{
				printf("��оƬΪW25X16\r\n");
				printf("W25X16��2MB����0~31���飬һ������64KB��ÿ������0~15������һ��������4KB��ÿ��������0~15��ҳ��\r\n");
			}
			else if(ID == 0X15)
			{
				printf("��оƬΪW25X32\r\n");
				printf("W25X16��4MB����0~63���飬һ������64KB��ÿ������0~15������һ��������4KB��ÿ��������0~15��ҳ��\r\n");
			}
			else if(ID == 0X16)
			{
				printf("��оƬΪW25X64\r\n");
				printf("W25X16��8MB����0~127���飬һ������64KB��ÿ������0~15������һ��������4KB��ÿ��������0~15��ҳ��\r\n");
			}
			break;
		}
		case 106:
		{//��ȡJEDEC ID
			u32 ID = W25Xxx_ReadJEDECID();
			printf("���д洢оƬӲ��JEDEC IDΪ��0x%X\r\n",ID);
			break;
		}
		//---
	}
}
//---
/*
	�������� void W25Xxx_DebugRTU((u8 *p)
	������		�ú���ΪW25Xxx�ļ���RTU�����ļ�
	˵����		���øú���֮ǰ��Ҫ�Ƚ���CRCУ��
*/
void W25Xxx_DebugRTU(u8 *p,u8 len)
{
	switch(*p)//�鿴���ĸ�������
	{
		//-----------------------------------------
		case 0x01:
		{//��λϵͳ
			break;
		}
	}
}
//---
//---


