/*
	���ļ�Ϊ���ּ��㺯��
	���庯���������£�
	
*/
#include "calculate.h"

//CRC��λ�ֽڱ� /* Table of CRC values for high�Corder byte */
const unsigned char auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
};
//CRC��λ�ֽڱ� /* Table of CRC values for low�Corder byte */
const char  auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
};
//��׼CRC�㷨
unsigned short CRC16(unsigned char *puchMsg,unsigned short usDataLen)
{
	unsigned char uchCRCHi = 0xFF;		//��ʼ�����ֽ�
	unsigned char uchCRCLo = 0xFF;		//��ʼ�����ֽ�
	unsigned short uIndex;		//��CRC��
	while(usDataLen--)
	{
		uIndex=uchCRCHi ^ *puchMsg++;	//����CRC
		uchCRCHi=uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo=auchCRCLo[uIndex];
	}
	return (uchCRCHi<<8 |uchCRCLo);
}

/*******************************************
	��������AnalysisCRC(u8 *pstr)
����������CRC :���ȣ����ݣ�CRC16
�Ƚ� ������CRC
*******************************************/
u8 AnalysisCRC(u8 *pstr)
   { u16 crc,dmtcrc;
		 
		 u8 len=pstr[0];
		 pstr++;
		 if(len>=3)
		 {
		    crc=CRC16(pstr,len-2);
			  dmtcrc=pstr[len-2];
			  dmtcrc=dmtcrc<<8;
			  dmtcrc+=pstr[len-1];
			 if(dmtcrc==crc)
				 return 1;
			  else 
				 return 0;
		 }
		 else
			 return 0;
	 }
/*
	�������� u16 charTOu16(u8 *p,u8 len)
	������ 	 ���ַ���ת����16λ���޷�����������
	���룺	 *pΪ�ַ����׵�ַ��lenΪ�ַ�������,len<=5
*/
u16 Calculate_CharToU16(u8 *p,u8 len)
{
	char srt[6]={0,0,0,0,0,0};
	u8 i;
	for(i=0;i<len;i++)
	{
		srt[i]=(char)*p++;
	}
	return atoi(srt);
}
//---
/*
	�������� u32 charTOu32(u8 *p,u8 len)
	������ 	 ���ַ���ת����32λ���޷�����������
	���룺	 *pΪ�ַ����׵�ַ��lenΪ�ַ�������,len<11
*/
u32 Calculate_CharToU32(u8 *p,u8 len)
{
	char str[10]={0,0,0,0,0,0,0,0,0,0};
	u8 i;
	long long a;
	for(i=0;i<len;i++)
	{
		str[i]=(char)*p++;
	}
	a = atoll(str);
	if(a>0xFFFFFFFF)
		a = 0xFFFFFFFF;
	return (u32)a;
}
//---
/*
	��������u16 Calculate_HexStrToU16(u8 *p,u8 len)
	�������ú���Ϊ��ʮ�������ַ���ת��Ϊ�޷���16������
	���룺*pΪ�ַ����׵�ַ��lenΪ�ַ�������,len<=4
*/
//u16 Calculate_HexStrToU16(u8 *p,u8 len)
//{
//	u8 i;
//	for(i=0;i<4;i++)
//	{
//		if(0x2F < *p && *p < 0x3A)
//		{
//			
//		}
//	}
//}
//---
/*
	��������u8 BeforeToBafter(u8 buffer)
	������	 �ú������ڽ�һ��8λ���޷������ݵ�ÿһλǰ��Ի���b0-b7,b1-b6����������
	���룺	 ��Ҫ���Ի�������
	���أ�	 �Ի��������
*/
u8 BeforeToBafter(u8 buffer)
{
	u8 a=0;
	if(buffer & 0x01) a |= 0x80;//������һλ
	if(buffer & 0x02) a |= 0x40;//�����ڶ�λ
	if(buffer & 0x04) a |= 0x20;//��������λ
	if(buffer & 0x08) a |= 0x10;//��������λ
	if(buffer & 0x10) a |= 0x08;//��������λ
	if(buffer & 0x20) a |= 0x04;//��������λ
	if(buffer & 0x40) a |= 0x02;//��������λ
	if(buffer & 0x80) a |= 0x01;//�����ڰ�λ
	return a;
}

/*
	��������	unsigned int uiCrc16Cal(unsigned char const *pucY, unsigned char ucX)
	���룺		unsigned char const *pucY��������CRC���ݵ��׵�ַ
						unsigned char ucX��������CRC���ݵĳ��ȣ��ֽ�����
	���أ�		һ���޷���16���Ƶ�CRC����
*/
#define PRESET_VALUE 0xFFFF 
#define POLYNOMIAL 0x8408
unsigned int Calculate_CRC16(unsigned char const * pucY, unsigned char ucX) 
{
	unsigned char ucI,ucJ;
	unsigned short int uiCrcValue = PRESET_VALUE; 
	for(ucI = 0; ucI < ucX; ucI++) 
	{
		uiCrcValue = uiCrcValue ^ *(pucY + ucI); 
		for(ucJ = 0; ucJ < 8; ucJ++) 
		{
			if(uiCrcValue & 0x0001) 
			{ 
				uiCrcValue = (uiCrcValue >> 1) ^ POLYNOMIAL; 
			}
			else 
			{
				uiCrcValue = (uiCrcValue >> 1); 
			}
		} 
	}
	return uiCrcValue; 
}
//---
/*
	��������	float Calculate_CharToFloat(u8 *p,u8 len)
	������		�ú������ڽ�һ�������ַ�ת��Ϊ����������
	���룺		*p���ַ����׵�ַ
						len���ַ����ݵĳ���
	���أ�		����Ľ������lenΪ0
*/
float Calculate_CharToFloat(u8 *p,u8 len)
{
	char srt[15]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	u8 i;
	for(i=0;i<len;i++)
	{
		srt[i]=(char)*p++;
	}
	return atof(srt);
}
//---
/*
	��������void Calculate_FourBigEndToSmallEnd(FourBytes *p)
	���������ĸ��ֽڽ��д�С�˶Ի�
*/
void Calculate_FourBigEndToSmallEnd(FourBytes *p)
{
	u8 bf;
	bf = p->U8Data[0];
	p->U8Data[0] = p->U8Data[3];
	p->U8Data[3] = bf;
	bf = p->U8Data[1];
	p->U8Data[1] = p->U8Data[2];
	p->U8Data[2] = bf;
}
//---
/*
	��������	void Calculate_TimeToFrequency(u32 *buffer)
	������	 	�����ڻ����Ƶ��,��ʽ��F=1000,000,000��T��100,000
	���룺  	һ��32λ�޷��ŵ�ʱ��ֵ����λns
	���أ�	 	һ��32λ�޷��ŵ�Ƶ��ֵ����λHz��100,000
*/
void Calculate_TimeToFrequency(u32 *buffer)
{
	*buffer = 1000000000/(float)(*buffer)*100000;
}
//---
/*
	��������void Pid_Calculate(PID_DataStructure* p)
	������	pid����
*/
/*
void Pid_Calculate(PID_DataStructure* p)
{
	float DelEk;//����ƫ�����ϴ�ƫ���֮����ڼ��������
	float ki;
	float kd;
	float Iout;//��ʷ�������
	float Pout;//�������
	float Dout;//΢�����
	float out;
	if (p->Timer < p->T) return;//ʱ��δ�����˳�
	//��������
	p->Ek = p->Sv - p->Pv;//��ǰƫ��ֵ�����ڱ�������
	Pout = p->Kp * p->Ek;//�������
	//���ּ���
	p->SEk += p->Ek;
//	p->SEk += (p->Ek - p->Avg);//��ʷƫ���ܺͣ���ֵ�����������Ϊp->Ek�������и�
	//����
	//---------------------
//	p->Avg = p->SEk / (p->Ti / p->T);
	//---------------------
//	printf("SEk=%f\r\n",p->SEk);
	ki = p->Kp * (p->T / p->Ti);
	Iout = ki * p->SEk;//�������
	//΢�ּ���
	DelEk = p->Ek - p->Ek_1;//�������ƫ��֮��
	kd = p->Kp * (p->Td / p->T);
	Dout = kd * DelEk;			//΢�����
	//������
	out = Pout + Iout + Dout + p->OUT0;//���μ�����
	//�����Χ����
	if (out > p->PWMcycle)out = p->PWMcycle;
	else if (out < 0)out = 0;
	p->OUT = out;	//������
	p->Ek_1 = p->Ek;//����ƫ�����ƫ���Ϊ�´�ƫ��
	p->Timer = 0;
}
*/
//---
/*
	��������void Pid_Calculate(PID_DataStructure* p)
	������	pid����
*/
void Pid_Calculate(PID_DataStructure* p)
{
	//float DelEk;//����ƫ�����ϴ�ƫ���֮����ڼ��������
//	float ki;
//	float kd;
	float Iout;//��ʷ�������
	float Pout;//�������
	float Dout;//΢�����
	float out;
	if (p->Timer < p->T) return;//ʱ��δ�����˳�
	//����ʽ����
	p->Ek_2=p->Ek_1;      //���ϴ�ƫ��=�ϴ�ƫ��
	p->Ek_1=p->Ek;        //�ϴ�ƫ��=���ƫ��
	p->Ek = p->Sv - p->Pv;//��ǰƫ��ֵ�����ڱ�������
	Pout=p->Kp*(p->Ek-p->Ek_1);
	
	Iout=p->Ti*(p->Ek);
	
	Dout=p->Td*(p->Ek-2*p->Ek_1+p->Ek_2);
	
		//������
	out = Pout + Iout + Dout + p->OUT;//���μ�����

	//�����Χ����
	if (out > p->PWMcycle)out = p->PWMcycle;
	else if (out < 0)out = 0;
	p->OUT = out;	//������

	p->Timer = 0;
}

//---
/*
	��������Calc_SO2_K(float biaodinzhi,u32 ad)   //���������ĳ���K;
  ����:   biaodinzhi:�궨ʱ������Ũ�� ��������0.209;
          AD �ǵ�Ƭ��������ԭʼ����0~2048;
  ���:   ����Ũ�ȵĳ���K
	������  ���������ĳ���K;
*/
float  Calc_SO2_K(float biaodinzhi,u32 ad)   //���������ĳ���K;
{ float temp,temp1;
	temp=biaodinzhi;
	
	temp1=3;
	temp1=temp1/4096;
	temp1=(float)ad*temp1;
	temp=temp/temp1;
	
	return temp;
}
//---
/*
	��������Calc_SO2(float K,u32 ad)   //���������ĳ���K;
  ����:   K:����Ũ�ȵĳ���
          AD �ǵ�Ƭ��������ԭʼ����0~2048;
  ���:   ����Ũ��
	������  ��������Ũ��;
*/
float  Calc_SO2(float K,u32 ad)   //��������Ũ��;
{
	float temp;
	temp=3;
	temp=temp/4096;
	temp=(float)ad*temp;
	temp=K*temp;

	return temp;
}

//---
/*
	��������Calc_O2_K(float biaodinzhi,u32 ad)   //���������ĳ���K;
  ����:   biaodinzhi:�궨ʱ������Ũ�� ��������0.209;
          AD �ǵ�Ƭ��������ԭʼ����0~4096;
  ���:   ����Ũ�ȵĳ���K
	������  ���������ĳ���K;
*/
float  Calc_O2_K(float biaodinzhi,u32 ad)   //���������ĳ���K;
{ float temp,temp1;
	temp=biaodinzhi;
	
	temp1=3;
	temp1=temp1/4096;
	temp1=(float)ad*temp1;
	
	temp=1;
	temp=temp/(temp-biaodinzhi);
	temp=logf(temp);
	
	temp=temp1/temp;
	
	return temp;
}
//---
/*
	��������Calc_O2(float K,u32 ad)   //����������Ũ��;
  ����:   K:����Ũ�ȵĳ���
          AD �ǵ�Ƭ��������ԭʼ����0~4096;
  ���:   ����Ũ��
	������  ��������Ũ��;
*/
float  Calc_O2(float K,u32 ad)   //��������Ũ��;
{
	float temp,temp1;
	temp=3;
	temp=temp/4096;
	temp=(float)ad*temp;
	temp=temp/K;
	temp=expf(temp);
	temp1=1;
	temp=temp1/temp;
	temp1=1;
	temp=temp1-temp;
	return temp;
}

//---
/*
	�������� void Calculate_Debug(u8 *p)
	������		�ú���ΪCalculate.c�ļ��ĵ��Ժ���,��ϵͳ���Ժ�������
*/
void Calculate_DebugASCII(u8 *p,u8 len)
{
	u16 cmd = Calculate_CharToU16(p,3);//��������
	switch(cmd)
	{
		case 1:
		{//����CRC16()����
			break;
		}
		case 2:
		{//����u16 charTOu16(u8 *p,u8 len)����
			break;
		}
		case 3:
		{//����u32 charTOu32(u8 *p,u8 len)����
			break;
		}
		case 4:
		{//����u8 BeforeToBafter(u8 buffer)����
			break;
		}
		case 5:
		{//����double Calculate_CharToFloat(u8 *p,u8 len)����
		 //�����ʽ��005|XX|XXXXXXXX,ǰ��2��XX��Ҫת���ĳ��ȣ������XXXXXXXXX����������+С����
			u8 x = Calculate_CharToU16(p+4,2);
			printf("ת���������Ϊ%f\r\n",Calculate_CharToFloat(p+7,x));
			break;
		}
		default:
		{//�������
			printf("Calculate_Debug�޴˵�������%u!\r\n",cmd);
			break;
		}
	}
}






