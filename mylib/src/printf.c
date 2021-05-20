                        //��ģ��Ϊ�����ڹ���printf()����

/*
	�汾��		V1.0
	��д���ڣ�	
	�༭�ˣ�	
	��ģ�����ã���ģ������printf�����ӡ�����л�������������ʹ��ѭ���б�ʽ
	ʹ�÷�����	1��ʹ��printf��������ǣ���Ҫ�Ƚ�Target Options��Target���õ�Use MicroLIBѡ��㹴
				2����ʼ����������紮�ڣ��ĸ�������������������жϹ���
				3���ڴ�ӡ����������紮��������ķ����жϣ��紮�ڵķ����жϣ�����Printf_Send()����
				4����Ҫ��printf.h�ļ��ĵ�7�����(//#define OPEN_PRINTF)ע��ȥ��
				5��Printf_Data.Printf_Mode��ʼ��Ϊ1��Ĭ�ϵ��Դ�ӡ���ڣ���ʹ�������ڣ�һ��Ҫ��ʹ������ѿ���Printf_Data.Printf_Mode����ָ��ֵ������Ҫ�ָ�=1��
	�޸����ڣ�
	�޸�˵����
*/
#include "printf.h"

Printf_DataStructure  Printf_Data; 

Printf_DataStructure * Printf_GetData(void)
{
	return &Printf_Data;
}

//��ʼ��
void Printf_Init(void)
{
	Printf_Data.Printf_Mode=1;
}

/*
	����ԭ���ƣ� int fputc(int ch, FILE *f)
	˵����	�ú�����Printf����������ã�����Ҫ��ӡ����������洢����ӡ��������
	���룺	int ch;
	�����	int ch
	���أ�	int ch
*/
PUTCHAR_PROTOTYPE
{
	/* ARMJISHU ��ȴ���Printf���ڴ�ӡ����������
     д��Buffer����������(Bufferδ��ʱ) */
 switch(Printf_Data.Printf_Mode)
 {
	 case 1:   //��ʼ��Ĭ��Ϊ 1��ӡ�˿�
	 {
	#ifdef	OPEN_PRINTF    
	USARTx_StoreBufferData(USART1,Get_USART1Buffer(),(unsigned char)ch);		//�Ӵ���1�����ݷ��ͳ�ȥ
	#endif
		 break;
	 }
	 case 2:break;
	 case 3:break;
	 case 4:
	 {
		 while((UART4->SR&0X40)==0) //ѭ������,ֱ���������   ��ӡ
		 { ;}
	   UART4->DR = (u8) ch; 
    break;		 
	 }
	 case 5:
	 {
		 while((UART5->SR&0X40)==0) //ѭ������,ֱ���������  �ֻ�APP 
		 { ;}
	   UART5->DR = (u8) ch; 
		 break;
	 }
	 default:break;
 }
	return ch;
}


	//�������� void myprintf(char * usart,const char * __restrict /*format*/, ...) __attribute__((__nonnull__(1)))
/*	������	 ����Ҫ��ӡ�����ݴ����ӡ������������FIFOԭ���ӡ����ÿ�ε��ú���Ҫ��
			 �������жϣ���������ֲʱ�����޸����һ����䣬�����������жϼ���
	�����	 ��
	���룺	 (unsigned char ch)��Ҫ����ӡ������
	���أ�	 ��
*/
	
