/*
	flashisp.c�⺯��
*/

#ifndef __FLASHISP_H
#define __FLASHISP_H

#include "stm32f4xx.h"

#define FLASH_SIZE 									1024//��ѡMCU��FLASH������С(��λΪK)
#if FLASH_SIZE<256
  #define SECTOR_SIZE           		1024    //�ֽ�
#else 
  #define SECTOR_SIZE           		2048    //�ֽ�
#endif
//--------------------------------------
//�����궨��
#define FLASH_USE_FM24CL64
//FLASH ��������ʼ��ַ
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//����0��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//����1��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//����2��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//����3��ʼ��ַ, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//����4��ʼ��ַ, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//����5��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//����6��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//����7��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//����8��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//����9��ʼ��ַ, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//����10��ʼ��ַ,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//����11��ʼ��ַ,128 Kbytes  

#define FLASHIS_FACTORY								0XAAAA	//�ظ�����������


//�洢��ַ
#define FLASHIS_AdrStoreStart 				0x080E0000+1024*10//FLASH_BASE+500*SECTOR_SIZE			//��ʼ��ַ
//�洢��ַ
//#define FLASHIS_AdrStoreFluxA 				FLASH_BASE+501*SECTOR_SIZE	
//#define FLASHIS_AdrStoreFluxB 				FLASH_BASE+502*SECTOR_SIZE
//#define FLASHIS_AdrStoreFluxT 				FLASH_BASE+503*SECTOR_SIZE


//��ַ��ƫ�Ƶ�ַ��һ����λΪ�����ֽ�
#define FLASHIS_AdrSTART							(u16)0x0000
//ϵͳ����
#define FLASHIS_AdrXTSZHJWD						FLASHIS_AdrSTART				//ϵͳ���û����¶�
#define FLASHIS_AdrXTSZHJWDKG					FLASHIS_AdrXTSZHJWD+2		//ϵͳ���û����¶ȿ���
#define FLASHIS_AdrXTSZDQY						FLASHIS_AdrXTSZHJWDKG+1	//ϵͳ���ô���ѹ
#define FLASHIS_AdrXTSZCYDZ						FLASHIS_AdrXTSZDQY+2	  //ϵͳ���ò�����ַ //15����
#define FLASHIS_AdrXTSZZDPSJG					FLASHIS_AdrXTSZCYDZ+15	//ϵͳ������ˮ���
#define FLASHIS_AdrXTSZZDPSSC					FLASHIS_AdrXTSZZDPSJG+1	//ϵͳ������ˮʱ��
#define FLASHIS_AdrXTSZYQCLSJ					FLASHIS_AdrXTSZZDPSSC+1	//ϵͳ����������ʱ��
#define FLASHIS_AdrXTSZYQCLCS					FLASHIS_AdrXTSZYQCLSJ+1	//ϵͳ��������������
#define FLASHIS_AdrXTSZCHSLSJ					FLASHIS_AdrXTSZYQCLCS+1	//ϵͳ��⺬ʪ��ʱ��
#define FLASHIS_AdrXTSZBOX  					FLASHIS_AdrXTSZCHSLSJ+1	//  //0λ ����ѹ 1λ������ 2λ�������� 3λ�Զ���ˮ 4�������� 5 ������λ 6�̳���������
//�̵���������
#define FLASHIS_AdrYDBDLX							FLASHIS_AdrXTSZBOX+1		//�̵���������  1=Բ��  2=���� 3=��������

#define FLASHIS_AdrYDBDYXNJ						FLASHIS_AdrYDBDLX+1		     //�̵�����Բ���ھ�D
#define FLASHIS_AdrYDBD_MJ						FLASHIS_AdrYDBDYXNJ+2		   //�̵�����Բ�����
#define FLASHIS_AdrYDBD_GT						FLASHIS_AdrYDBD_MJ+2		   //�̵�����Բ�ι���
#define FLASHIS_AdrYDBDYXHS						FLASHIS_AdrYDBD_GT+2		   //�̵�����Բ�λ���
#define FLASHIS_AdrYDBDYXKS						FLASHIS_AdrYDBDYXHS+1		   //�̵�����Բ�ο���
#define FLASHIS_AdrYDBD_CYDS					FLASHIS_AdrYDBDYXKS+1		   //�̵�����Բ�β�������

#define FLASHIS_AdrYDBDFXBA					FLASHIS_AdrYDBD_CYDS+1		     //�̵����㷽��һ��A
#define FLASHIS_AdrYDBDFXBB					FLASHIS_AdrYDBDFXBA+2		   //�̵����㷽��һ��B
//#define FLASHIS_AdrYDBDFXMJ					FLASHIS_AdrYDBDFXBB+2		   //�̵����㷽�����
//#define FLASHIS_AdrYDBDFXGT					FLASHIS_AdrYDBDFXMJ+2		   //�̵����㷽�ι���
#define FLASHIS_AdrYDBDFXKS					FLASHIS_AdrYDBDFXBB+2		   //�̵����㷽�ο���
#define FLASHIS_AdrYDBDFXDKCD				FLASHIS_AdrYDBDFXKS+1		   //�̵����㷽�ε��ײ��
//#define FLASHIS_AdrYDBDFXCYDS				FLASHIS_AdrYDBDFXDKCD+1		   //�̵����㷽�β�������
#define FLASHIS_AdrYDBDCDJL1				FLASHIS_AdrYDBDFXDKCD+1		   //�̵����㷽�ε��ײ�����1
#define FLASHIS_AdrYDBDCDJL2				FLASHIS_AdrYDBDCDJL1+2		   //�̵����㷽�ε��ײ�����2
#define FLASHIS_AdrYDBDCDJL3				FLASHIS_AdrYDBDCDJL2+2		   //�̵����㷽�ε��ײ�����3
#define FLASHIS_AdrYDBDCDJL4				FLASHIS_AdrYDBDCDJL3+2	   //�̵����㷽�ε��ײ�����4
#define FLASHIS_AdrYDBDCDJL5				FLASHIS_AdrYDBDCDJL4+2		   //�̵����㷽�ε��ײ�����5
#define FLASHIS_AdrYDBDCDJL6				FLASHIS_AdrYDBDCDJL5+2		   //�̵����㷽�ε��ײ�����6
#define FLASHIS_AdrYDBDCDJL7				FLASHIS_AdrYDBDCDJL6+2		   //�̵����㷽�ε��ײ�����7
#define FLASHIS_AdrYDBDCDJL8				FLASHIS_AdrYDBDCDJL7+2		   //�̵����㷽�ε��ײ�����8
#define FLASHIS_AdrYDBDCDJL9				FLASHIS_AdrYDBDCDJL8+2		   //�̵����㷽�ε��ײ�����9
#define FLASHIS_AdrYDBDCDJL10				FLASHIS_AdrYDBDCDJL9+2		   //�̵����㷽�ε��ײ�����10
//#define FLASHIS_AdrYDBDQTMJ				FLASHIS_AdrYDBDFXCYDS+1		   //�̵�����������ʽ���
//#define FLASHIS_AdrYDBDQTCYDS				FLASHIS_AdrYDBDQTMJ+2		   //�̵������������
//
//����Ԥ��  ѹ����0
#define FLASHIS_AdrGKYC_YLTL_YQDY				    FLASHIS_AdrYDBDCDJL10+1		      //ѹ�����㣬������ѹ
#define FLASHIS_AdrGKYC_YLTL_YQJY				    FLASHIS_AdrGKYC_YLTL_YQDY+1		   //ѹ�����㣬������ѹ
#define FLASHIS_AdrGKYC_YLTL_JQYL				    FLASHIS_AdrGKYC_YLTL_YQJY+2		   //ѹ�����㣬��ǰѹ��
#define FLASHIS_AdrGKYC_YLTL_CLY				    FLASHIS_AdrGKYC_YLTL_JQYL+2		   //ѹ�����㣬����ѹѹ
#define FLASHIS_AdrGKYC_YLTL_QLY				    FLASHIS_AdrGKYC_YLTL_CLY+1		   //ѹ�����㣬����ѹѹ
//����Ԥ�� ���²���
#define FLASHIS_AdrGKYC_YWCL_FS				        FLASHIS_AdrGKYC_YLTL_QLY+1		   //���·�ʽ 0 ���� 1����
#define FLASHIS_AdrGKYC_YWCL_SRYW				    FLASHIS_AdrGKYC_YWCL_FS+1		   //��������

////����Ԥ�� ��������

#define FLASHIS_AdrGKYC_YQLS_YQLS				      FLASHIS_AdrGKYC_YWCL_SRYW+2		   //ƽ������  //20200601

#define FLASHIS_AdrGKYC_CPJZ_PJDY				      FLASHIS_AdrGKYC_YQLS_YQLS+2		   //ƽ����ѹ      ----
#define FLASHIS_AdrGKYC_CPJZ_PJJY				      FLASHIS_AdrGKYC_CPJZ_PJDY+2		   //ƽ����ѹ      ----
#define FLASHIS_AdrGKYC_CPJZ_PJQY				      FLASHIS_AdrGKYC_CPJZ_PJJY+2		   //ƽ��ȫѹ      ----
#define FLASHIS_AdrGKYC_CPJZ_PJYW				      FLASHIS_AdrGKYC_CPJZ_PJQY+2		   //ƽ������      ----
#define FLASHIS_AdrGKYC_CPJZ_YGLL				      FLASHIS_AdrGKYC_CPJZ_PJYW+2		   //��������      ----
#define FLASHIS_AdrGKYC_CPJZ_BGLL				      FLASHIS_AdrGKYC_CPJZ_YGLL+2		   //�������
//#define FLASHIS_AdrGKYC_CPJZ_DXLL                   FLASHIS_AdrGKYC_CPJZ_BGLL+2        //��������    �������������

//��ʪ��
//#define FLASHIS_AdrCHSL_LX				      FLASHIS_AdrGKYC_YQLS_YQLL+2		   //�⺬ʪ������
//#define FLASHIS_AdrCHSL_CLZ				      FLASHIS_AdrCHSL_LX+1		         //������ʪ��
#define FLASHIS_AdrCHSL_HSL				          FLASHIS_AdrGKYC_CPJZ_BGLL+2		      //��ʪ��
//��������
#define FLASHIS_AdrCLYQ_CLFS				      FLASHIS_AdrCHSL_HSL+2		         //������ʽ 0�ֶ� 1��ʱ
#define FLASHIS_AdrCLYQ_JSSC				      FLASHIS_AdrCLYQ_CLFS+1		         //��ʱʱ��
//�������� -����У׼
//#define FLASHIS_AdrCLYQ_CJO2				      FLASHIS_AdrCLYQ_JSSC+1		         //O2У׼ֵ
//#define FLASHIS_AdrCLYQ_CJNO				      FLASHIS_AdrCLYQ_CJO2+2		         //NOУ׼ֵ
//#define FLASHIS_AdrCLYQ_CJNO2				      FLASHIS_AdrCLYQ_CJNO+2		         //NO2У׼ֵ
//#define FLASHIS_AdrCLYQ_CJNOX				      FLASHIS_AdrCLYQ_CJNO2+2		         //NOXУ׼ֵ
//#define FLASHIS_AdrCLYQ_CJSO2				      FLASHIS_AdrCLYQ_CJNOX+2		         //SO2У׼ֵ
//#define FLASHIS_AdrCLYQ_CJCO				      FLASHIS_AdrCLYQ_CJSO2+2		         //COУ׼ֵ
//#define FLASHIS_AdrCLYQ_CJH2S				      FLASHIS_AdrCLYQ_CJCO+2		         //H2SУ׼ֵ
//#define FLASHIS_AdrCLYQ_CJCO2				      FLASHIS_AdrCLYQ_CJH2S+2		         //CO2У׼ֵ
//�������� -�����е�λ����
#define FLASHIS_AdrCLYQ_DWLX				      FLASHIS_AdrCLYQ_JSSC+1		         //��λ����
//�������� -�ļ���ѯ - ����ϵ��
#define FLASHIS_AdrCLYQ_WJCX_JS_ZSXS				      FLASHIS_AdrCLYQ_DWLX+1		         //����ϵ��
#define FLASHIS_AdrCLYQ_WJCX_JS_JZYHL				      FLASHIS_AdrCLYQ_WJCX_JS_ZSXS+2		 //��׼��������
#define FLASHIS_AdrCLYQ_WJCX_JS_FHXS				      FLASHIS_AdrCLYQ_WJCX_JS_JZYHL+2		 //����ϵ��
//�������� -�ļ���ѯ - ��ӡѡ��
#define FLASHIS_AdrCLYQ_WJCX_DYXX				      FLASHIS_AdrCLYQ_WJCX_JS_FHXS+2		    //��ӡѡ��
//�������� -�ļ���ѯ - ��������
#define FLASHIS_AdrCLYQ_WJCX_FZSJ_PJJS			      FLASHIS_AdrCLYQ_WJCX_DYXX+2		    //����ƽ������
//�̳������� -��������
#define FLASHIS_AdrYCCJ_CYSE_CZZJ				          FLASHIS_AdrCLYQ_WJCX_FZSJ_PJJS+1		   //����ֱ�� �빤��Ԥ��Ĳ���ֱ��һ��
#define FLASHIS_AdrYCCJ_CYSE_CYFS				          FLASHIS_AdrYCCJ_CYSE_CZZJ+2		   //������ʽ 0����  1����
#define FLASHIS_AdrYCCJ_CYSE_CYLS				          FLASHIS_AdrYCCJ_CYSE_CYFS+1		   //��������
#define FLASHIS_AdrYCCJ_CYSE_LTH				          FLASHIS_AdrYCCJ_CYSE_CYLS+2		   //��Ͱ��
#define FLASHIS_AdrYCCJ_CYSE_DDCS				          FLASHIS_AdrYCCJ_CYSE_LTH+2		   //�����ʱ
//�ļ���ѯ -�̳���ӡѡ��
#define FLASHIS_AdrYCCJ_YCWJCX_DYXX				      FLASHIS_AdrYCCJ_CYSE_DDCS+1		    //��ӡѡ��
//�ļ���ѯ - ����/����
#define FFLASHIS_AdrYCCJ_YCWJCX_JSRYND            FLASHIS_AdrYCCJ_YCWJCX_DYXX+3        //�ļ���ѯ�� ���� ��ҺŨ��
#define FFLASHIS_AdrYCCJ_YCWJCX_JSDRTJ            FFLASHIS_AdrYCCJ_YCWJCX_JSRYND+2     //�ļ���ѯ�� ���� �������
#define FFLASHIS_AdrYCCJ_YCWJCX_JSZTSM            FFLASHIS_AdrYCCJ_YCWJCX_JSDRTJ+2     //�ļ���ѯ�� ���� ��ͷ��Ŀ

//�ļ���ѯ - ����/�̳�
#define FFLASHIS_AdrYCCJ_YCWJCX_JSYCZL            FFLASHIS_AdrYCCJ_YCWJCX_JSZTSM+1        //�ļ���ѯ�� ���� �̳�����
#define FFLASHIS_AdrYCCJ_YCWJCX_JSHYL             FFLASHIS_AdrYCCJ_YCWJCX_JSYCZL+2         //�ļ���ѯ�� ���� ��������
#define FFLASHIS_AdrYCCJ_YCWJCX_JSFHXS            FFLASHIS_AdrYCCJ_YCWJCX_JSHYL+2         //�ļ���ѯ�� ���� ����ϵ��
#define FFLASHIS_AdrYCCJ_YCWJCX_JSZSXS            FFLASHIS_AdrYCCJ_YCWJCX_JSFHXS+2        //�ļ���ѯ�� ���� ����ϵ��
#define FFLASHIS_AdrYCCJ_YCWJCX_JSJZYHL           FFLASHIS_AdrYCCJ_YCWJCX_JSZSXS+2       //�ļ���ѯ�� ���� ��׼��������
//-----------------------------------------------------------------------------------
//20200601
//����������
#define FLASHIS_AdrCJKSSK				      FFLASHIS_AdrYCCJ_YCWJCX_JSJZYHL+2	//�ɼ���ʼʱ��	��λ�룬time_t��ʽ
#define FLASHIS_AdrCJLJSH				      FLASHIS_AdrCJKSSK+2         //����ǰ������ʱ u32
#define FLASHIS_AdrCJGKTJ				      FLASHIS_AdrCJLJSH+2         //����ǰ�������
#define FLASHIS_AdrCJBQTJ				      FLASHIS_AdrCJGKTJ+2         //����ǰ������
#define FLASHIS_AdrCJCNT		      	      FLASHIS_AdrCJBQTJ+2         //����ǰ�ɼ����ڼ�����
#define FLASHIS_AdrCJMOD			      	  FLASHIS_AdrCJCNT+1         //����ǰ����״̬����,��ͣ
#define FLASHIS_AdrCJWJH                      FLASHIS_AdrCJMOD+1         //�̳�����ǰ�ļ���
//ϵͳά����� ----------
#define FLASHIS_AdrBDStart						        FLASHIS_AdrCJWJH+2//�궨��ʼֵ�����ڱ����ã�
#define FLASHIS_AdrWHJZ_XTSZ_NO2_NOX			      	FLASHIS_AdrCJWJH+2                     //У׼ά�� ϵͳ���� NO2/NOx
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2			      	  FLASHIS_AdrWHJZ_XTSZ_NO2_NOX+2            //У׼ά�� ϵͳ���� CO��SO2����
#define FLASHIS_AdrWHJZ_XTSZ_NO2_SO2			      	FLASHIS_AdrWHJZ_XTSZ_CO_SO2+2          //У׼ά�� ϵͳ���� NO2��SO2����
#define FLASHIS_AdrWHJZ_XTSZ_PTG_KP			      	  FLASHIS_AdrWHJZ_XTSZ_NO2_SO2+2            //У׼ά�� ϵͳ���� Ƥ�й�
#define FLASHIS_AdrWHJZ_XTSZ_YQMD			      	    FLASHIS_AdrWHJZ_XTSZ_PTG_KP+2              //У׼ά�� ϵͳ���� �����ܶ�
#define FLASHIS_AdrWHJZ_XTSZ_ZSXS			      	    FLASHIS_AdrWHJZ_XTSZ_YQMD+2                //У׼ά�� ϵͳ���� ����ϵ��
#define FLASHIS_AdrWHJZ_XTSZ_JZYHL			      	  FLASHIS_AdrWHJZ_XTSZ_ZSXS+2                  //У׼ά�� ��׼������
#define FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ			      	FLASHIS_AdrWHJZ_XTSZ_JZYHL+2               //У׼ά�� SO2 ����ѡ��  0bit CO-SO2 bit1 NO-SO2
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1			    FLASHIS_AdrWHJZ_XTSZ_SO2_XZXZ+1            //У׼ά�� ϵͳ���� CO��SO2���� COŨ��1
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND2			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND1+1            //У׼ά�� ϵͳ���� CO��SO2���� COŨ��2
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND3			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND2+1            //У׼ά�� ϵͳ���� CO��SO2���� COŨ��3
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND4			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND3+1            //У׼ά�� ϵͳ���� CO��SO2���� COŨ��4
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND5			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND4+1            //У׼ά�� ϵͳ���� CO��SO2���� COŨ��5
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND6			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND5+1            //У׼ά�� ϵͳ���� CO��SO2���� COŨ��6
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND7		      FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND6+1               //У׼ά�� ϵͳ���� CO��SO2���� COŨ��7
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND8			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND7+1             //У׼ά�� ϵͳ���� CO��SO2���� COŨ��8
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND9			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND8+1              //У׼ά�� ϵͳ���� CO��SO2���� COŨ��9
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND10			  FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND9+1            //У׼ά�� ϵͳ���� CO��SO2���� COŨ��10
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ1			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COND10+1            //У׼ά�� ϵͳ���� CO��SO2����ϵ��1
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ2			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ1+2            //У׼ά�� ϵͳ���� CO��SO2����ϵ��2
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ3			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ2+2            //У׼ά�� ϵͳ���� CO��SO2����ϵ��3
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ4			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ3+2            //У׼ά�� ϵͳ���� CO��SO2����ϵ��4
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ5			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ4+2            //У׼ά�� ϵͳ���� CO��SO2����ϵ��5
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ6			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ5+2            //У׼ά�� ϵͳ���� CO��SO2����ϵ��6
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ7			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ6+2            //У׼ά�� ϵͳ���� CO��SO2����ϵ��7
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ8			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ7+2            //У׼ά�� ϵͳ���� CO��SO2����ϵ��8
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ9			    FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ8+2            //У׼ά�� ϵͳ���� CO��SO2����ϵ��9
#define FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ10			  FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ9+2            //У׼ά�� ϵͳ���� CO��SO2����ϵ��10



//ά��У׼-->����У׼
#define FLASHIS_AdrWHJZ_LLJZ_YCBLXZ					FLASHIS_AdrWHJZ_XTSZ_CO_SO2_COXZ10+2//�̳�����ѡ��
#define FLASHIS_AdrWHJZ_LLJZ_BZLL					FLASHIS_AdrWHJZ_LLJZ_YCBLXZ+1   //��׼����
#define FLASHIS_AdrWHJZ_LLJZ_YCBL01					FLASHIS_AdrWHJZ_LLJZ_BZLL+2 //�̳��궨����1
#define FLASHIS_AdrWHJZ_LLJZ_YCBL02					FLASHIS_AdrWHJZ_LLJZ_YCBL01+2//�̳��궨����2
#define FLASHIS_AdrWHJZ_LLJZ_YCBL03					FLASHIS_AdrWHJZ_LLJZ_YCBL02+2//�̳��궨����3
#define FLASHIS_AdrWHJZ_LLJZ_YCBL04					FLASHIS_AdrWHJZ_LLJZ_YCBL03+2//�̳��궨����4
#define FLASHIS_AdrWHJZ_LLJZ_YCBL05					FLASHIS_AdrWHJZ_LLJZ_YCBL04+2//�̳��궨����5
#define FLASHIS_AdrWHJZ_LLJZ_YCBL06					FLASHIS_AdrWHJZ_LLJZ_YCBL05+2//�̳��궨����6
#define FLASHIS_AdrWHJZ_LLJZ_YCBL07					FLASHIS_AdrWHJZ_LLJZ_YCBL06+2//�̳��궨����7
#define FLASHIS_AdrWHJZ_LLJZ_YCBL08					FLASHIS_AdrWHJZ_LLJZ_YCBL07+2//�̳��궨����8
#define FLASHIS_AdrWHJZ_LLJZ_YCBL09					FLASHIS_AdrWHJZ_LLJZ_YCBL08+2//�̳��궨����9
//#define FLASHIS_AdrWHJZ_LLJZ_YCBL10					FLASHIS_AdrWHJZ_LLJZ_YCBL09+2//�̳��궨����10
#define FLASHIS_AdrWHJZ_LLJZ_YQBL01					FLASHIS_AdrWHJZ_LLJZ_YCBL09+2//�����궨����1
//ά��У׼--->Ũ�Ƚ���
#define FLASHIS_AdrWHJZ_NDJZ_LDO2				      FLASHIS_AdrWHJZ_LLJZ_YQBL01+2		         //O2���ֵ
#define FLASHIS_AdrWHJZ_NDJZ_LDSO2				      FLASHIS_AdrWHJZ_NDJZ_LDO2+2		         //SO2���ֵ
#define FLASHIS_AdrWHJZ_NDJZ_LDNO				      FLASHIS_AdrWHJZ_NDJZ_LDSO2+2		         //NO���ֵ
#define FLASHIS_AdrWHJZ_NDJZ_LDNO2				      FLASHIS_AdrWHJZ_NDJZ_LDNO+2		         //NO2���ֵ
#define FLASHIS_AdrWHJZ_NDJZ_LDCO				      FLASHIS_AdrWHJZ_NDJZ_LDNO2+2		         //CO���ֵ
#define FLASHIS_AdrWHJZ_NDJZ_LDH2S				      FLASHIS_AdrWHJZ_NDJZ_LDCO+2		         //H2S���ֵ
#define FLASHIS_AdrWHJZ_NDJZ_LDCO2				      FLASHIS_AdrWHJZ_NDJZ_LDH2S+2		         //CO2���ֵ

#define FLASHIS_AdrWHJZ_NDJZ_BLO2_1				      FLASHIS_AdrWHJZ_NDJZ_LDCO2+2		             //O2����ֵ1    ��
#define FLASHIS_AdrWHJZ_NDJZ_BLSO2_1				      FLASHIS_AdrWHJZ_NDJZ_BLO2_1+2		         //SO2����ֵ1
#define FLASHIS_AdrWHJZ_NDJZ_BLNO_1				      FLASHIS_AdrWHJZ_NDJZ_BLSO2_1+2		         //NO����ֵ1
#define FLASHIS_AdrWHJZ_NDJZ_BLNO2_1				      FLASHIS_AdrWHJZ_NDJZ_BLNO_1+2		         //NO2����ֵ1

#define FLASHIS_AdrWHJZ_NDJZ_BLCO_1				      FLASHIS_AdrWHJZ_NDJZ_BLNO2_1+2		         //CO����ֵ1
#define FLASHIS_AdrWHJZ_NDJZ_BLH2S_1				      FLASHIS_AdrWHJZ_NDJZ_BLCO_1+2		         //H2S����ֵ1
#define FLASHIS_AdrWHJZ_NDJZ_BLCO2_1				      FLASHIS_AdrWHJZ_NDJZ_BLH2S_1+2		     //CO2����ֵ1

#define FLASHIS_AdrWHJZ_NDJZ_BLO2_2				      FLASHIS_AdrWHJZ_NDJZ_BLCO2_1+2		         //O2����ֵ2   ��
#define FLASHIS_AdrWHJZ_NDJZ_BLSO2_2				      FLASHIS_AdrWHJZ_NDJZ_BLO2_2+2		         //SO2����ֵ2
#define FLASHIS_AdrWHJZ_NDJZ_BLNO_2				      FLASHIS_AdrWHJZ_NDJZ_BLSO2_2+2		         //NO����ֵ2
#define FLASHIS_AdrWHJZ_NDJZ_BLNO2_2				      FLASHIS_AdrWHJZ_NDJZ_BLNO_2+2		         //NO2����ֵ2

#define FLASHIS_AdrWHJZ_NDJZ_BLCO_2				      FLASHIS_AdrWHJZ_NDJZ_BLNO2_2+2		         //CO����ֵ2
#define FLASHIS_AdrWHJZ_NDJZ_BLH2S_2				      FLASHIS_AdrWHJZ_NDJZ_BLCO_2+2		         //H2S����ֵ2
#define FLASHIS_AdrWHJZ_NDJZ_BLCO2_2				      FLASHIS_AdrWHJZ_NDJZ_BLH2S_2+2		     //CO2����ֵ2

#define FLASHIS_AdrWHJZ_NDJZ_BLO2_3				      FLASHIS_AdrWHJZ_NDJZ_BLCO2_2+2		         //O2����ֵ3   ��

#define FLASHIS_AdrWHJZ_NDJZ_BLSO2_3				      FLASHIS_AdrWHJZ_NDJZ_BLO2_3+2		         //SO2����ֵ3
#define FLASHIS_AdrWHJZ_NDJZ_BLNO_3				      FLASHIS_AdrWHJZ_NDJZ_BLSO2_3+2		         //NO����ֵ3
#define FLASHIS_AdrWHJZ_NDJZ_BLNO2_3				      FLASHIS_AdrWHJZ_NDJZ_BLNO_3+2		         //NO2����ֵ3

#define FLASHIS_AdrWHJZ_NDJZ_BLCO_3				      FLASHIS_AdrWHJZ_NDJZ_BLNO2_3+2		         //CO����ֵ3
#define FLASHIS_AdrWHJZ_NDJZ_BLH2S_3				      FLASHIS_AdrWHJZ_NDJZ_BLCO_3+2		         //H2S����ֵ3
#define FLASHIS_AdrWHJZ_NDJZ_BLCO2_3				      FLASHIS_AdrWHJZ_NDJZ_BLH2S_3+2		         //CO2����ֵ3

  //--------------------------------------------------------------20200519
#define FLASHIS_AdrWHJZ_NDJZ_BQLDO2_1				      FLASHIS_AdrWHJZ_NDJZ_BLCO2_3+2		         //O2����Ũ��ֵ1      ��
#define FLASHIS_AdrWHJZ_NDJZ_BQLDSO2_1				      FLASHIS_AdrWHJZ_NDJZ_BQLDO2_1+2		         //SO2����Ũ��ֵ1
#define FLASHIS_AdrWHJZ_NDJZ_BQLDNO_1				      FLASHIS_AdrWHJZ_NDJZ_BQLDSO2_1+2		         //NO����Ũ��ֵ1
#define FLASHIS_AdrWHJZ_NDJZ_BQLDNO2_1				      FLASHIS_AdrWHJZ_NDJZ_BQLDNO_1+2		         //NO2����Ũ��ֵ1

#define FLASHIS_AdrWHJZ_NDJZ_BQLDCO_1				      FLASHIS_AdrWHJZ_NDJZ_BQLDNO2_1+2		         //CO����Ũ��ֵ1
#define FLASHIS_AdrWHJZ_NDJZ_BQLDH2S_1				      FLASHIS_AdrWHJZ_NDJZ_BQLDCO_1+2		         //H2S����Ũ��ֵ1
#define FLASHIS_AdrWHJZ_NDJZ_BQLDCO2_1				      FLASHIS_AdrWHJZ_NDJZ_BQLDH2S_1+2		         //CO2����Ũ��ֵ1

#define FLASHIS_AdrWHJZ_NDJZ_BQLDO2_2				      FLASHIS_AdrWHJZ_NDJZ_BQLDCO2_1+2		         //O2����Ũ��ֵ2     ��
#define FLASHIS_AdrWHJZ_NDJZ_BQLDSO2_2				      FLASHIS_AdrWHJZ_NDJZ_BQLDO2_2+2		         //SO2����Ũ��ֵ2
#define FLASHIS_AdrWHJZ_NDJZ_BQLDNO_2				      FLASHIS_AdrWHJZ_NDJZ_BQLDSO2_2+2		         //NO����Ũ��ֵ2
#define FLASHIS_AdrWHJZ_NDJZ_BQLDNO2_2				      FLASHIS_AdrWHJZ_NDJZ_BQLDNO_2+2		         //NO2����Ũ��ֵ2

#define FLASHIS_AdrWHJZ_NDJZ_BQLDCO_2				      FLASHIS_AdrWHJZ_NDJZ_BQLDNO2_2+2		         //CO����Ũ��ֵ2
#define FLASHIS_AdrWHJZ_NDJZ_BQLDH2S_2				      FLASHIS_AdrWHJZ_NDJZ_BQLDCO_2+2		         //H2S����Ũ��ֵ2
#define FLASHIS_AdrWHJZ_NDJZ_BQLDCO2_2				      FLASHIS_AdrWHJZ_NDJZ_BQLDH2S_2+2		         //CO2����Ũ��ֵ2

#define FLASHIS_AdrWHJZ_NDJZ_BQLDO2_3				      FLASHIS_AdrWHJZ_NDJZ_BQLDCO2_2+2		         //O2����Ũ��ֵ3      ��

#define FLASHIS_AdrWHJZ_NDJZ_BQLDSO2_3				      FLASHIS_AdrWHJZ_NDJZ_BQLDO2_3+2		         //SO2����Ũ��ֵ3
#define FLASHIS_AdrWHJZ_NDJZ_BQLDNO_3				      FLASHIS_AdrWHJZ_NDJZ_BQLDSO2_3+2		         //NO����Ũ��ֵ3
#define FLASHIS_AdrWHJZ_NDJZ_BQLDNO2_3				      FLASHIS_AdrWHJZ_NDJZ_BQLDNO_3+2		         //NO2����Ũ��ֵ3

#define FLASHIS_AdrWHJZ_NDJZ_BQLDCO_3				      FLASHIS_AdrWHJZ_NDJZ_BQLDNO2_3+2		         //CO����Ũ��ֵ3
#define FLASHIS_AdrWHJZ_NDJZ_BQLDH2S_3				      FLASHIS_AdrWHJZ_NDJZ_BQLDCO_3+2		         //H2S����Ũ��ֵ3
#define FLASHIS_AdrWHJZ_NDJZ_BQLDCO2_3				      FLASHIS_AdrWHJZ_NDJZ_BQLDH2S_3+2		         //CO2����Ũ��ֵ3


//ά��У׼--->ѹ��У׼
#define FLASHIS_AdrWHJZ_YLJZ_DQY				      FLASHIS_AdrWHJZ_NDJZ_BQLDCO2_3+2		         //����ѹ���ֵ
#define FLASHIS_AdrWHJZ_YLJZ_JJY				      FLASHIS_AdrWHJZ_YLJZ_DQY+2		         //��ѹ���ֵ
#define FLASHIS_AdrWHJZ_YLJZ_JY				      FLASHIS_AdrWHJZ_YLJZ_JJY+2		         //��ѹ���ֵ
#define FLASHIS_AdrWHJZ_YLJZ_DY				      FLASHIS_AdrWHJZ_YLJZ_JY+2		         //��ѹ���ֵ
#define FLASHIS_AdrWHJZ_YLJZ_CLY				      FLASHIS_AdrWHJZ_YLJZ_DY+2		         //����ѹ���ֵ
#define FLASHIS_AdrWHJZ_YLJZ_QLY				      FLASHIS_AdrWHJZ_YLJZ_CLY+2		         //����ѹ���ֵ
//ά��У׼--->����������ѡ��
#define FLASHIS_AdrWHJZ_QTCGQXZ			      FLASHIS_AdrWHJZ_YLJZ_QLY+2		         //
//ά��У׼--->�¶Ƚ���
#define FLASHIS_AdrWHJZ_WDJZ_LDGQ				      FLASHIS_AdrWHJZ_QTCGQXZ+1		           //�������ֵ
#define FLASHIS_AdrWHJZ_WDJZ_LDYW             FLASHIS_AdrWHJZ_WDJZ_LDGQ+2		         //�������ֵ       //mark2
#define FLASHIS_AdrWHJZ_WDJZ_LDSQ				      FLASHIS_AdrWHJZ_WDJZ_LDYW+2		         //ʪ�����ֵ
#define FLASHIS_AdrWHJZ_WDJZ_LDJW				      FLASHIS_AdrWHJZ_WDJZ_LDSQ+2		         //�������ֵ
#define FLASHIS_AdrWHJZ_WDJZ_LDHW				      FLASHIS_AdrWHJZ_WDJZ_LDJW+2		         //�������ֵ

#define FLASHIS_AdrWHJZ_WDJZ_BLGQ				      FLASHIS_AdrWHJZ_WDJZ_LDHW+2		         //������ֵ
#define FLASHIS_AdrWHJZ_WDJZ_BLYW				      FLASHIS_AdrWHJZ_WDJZ_BLGQ+2		         //���±���ֵ       //mark2
#define FLASHIS_AdrWHJZ_WDJZ_BLSQ				      FLASHIS_AdrWHJZ_WDJZ_BLYW+2		         //ʪ����ֵ
#define FLASHIS_AdrWHJZ_WDJZ_BLJW				      FLASHIS_AdrWHJZ_WDJZ_BLSQ+2		         //���±���ֵ
#define FLASHIS_AdrWHJZ_WDJZ_BLHW				      FLASHIS_AdrWHJZ_WDJZ_BLJW+2		         //���±���ֵ
//ά��У׼--->��ر궨
#define FLASHIS_AdrWHJZ_DCJZ_BL			               FLASHIS_AdrWHJZ_WDJZ_BLHW+2		             //��ر���
#define FLASHIS_AdrBDEnd					           FLASHIS_AdrWHJZ_DCJZ_BL+2			//�궨����ֵ�����ڱ����ã�
//---------------------------------���ò���----
#define FLASHIS_AdrCYSZALCYLL					FLASHIS_AdrWHJZ_DCJZ_BL+2		//A·�������ò�������  ����
#define FLASHIS_AdrCYSZBLCYLL					FLASHIS_AdrCYSZALCYLL+2		//B·�������ò�������    �̳�
#define FLASHIS_AdrCYSZCLCYLL					FLASHIS_AdrCYSZBLCYLL+2		//C·�������ò�������    ����
//ά������
#define FLASHIS_AdrXTWHMM						FLASHIS_AdrCYSZCLCYLL+2		//ϵͳά������
#define FLASHIS_AdrXTWHLYPDM					FLASHIS_AdrXTWHMM+1			//���������
#define FLASHIS_AdrXTWHAPPLYPDM					FLASHIS_AdrXTWHLYPDM+1			//APP���������
#define FLASHIS_AdrXTWHLYPDMKG					FLASHIS_AdrXTWHAPPLYPDM+1			//APP��������뿪��
#define FLASHIS_AdrXTWHLLXS						FLASHIS_AdrXTWHLYPDMKG+1	//������ʾ
#define FLASHIS_AdrXTWHDYBL						FLASHIS_AdrXTWHLLXS+1		//��ѹ����
#define FLASHIS_AdrXTWHFSKG						FLASHIS_AdrXTWHDYBL+2		//���ȿ���
#define FLASHIS_AdrXTWHBJHMQD					FLASHIS_AdrXTWHFSKG+1		//��������ǰ��
#define FLASHIS_AdrXTWHBJHMHD					FLASHIS_AdrXTWHBJHMQD+2	//����������
//#define FLASHIS_AdrXTWHDD
//������Ϣ
#define FLASHIS_AdrDDTime						FLASHIS_AdrXTWHBJHMHD+2 //������Ϣ����λ�룬time_t��ʽ
//#define FLASHIS_AdrDDYear							FLASHIS_AdrXTWHBJHMHD+2	//������
//#define FLASHIS_AdrDDMonth						FLASHIS_AdrDDYear+1			//������
//#define FLASHIS_AdrDDDate							FLASHIS_AdrDDMonth+1		//������
//#define FLASHIS_AdrDDHour							FLASHIS_AdrDDDate+1			//����ʱ
//#define FLASHIS_AdrDDMinute 					FLASHIS_AdrDDHour+1			//�����
//#define FLASHIS_AdrDDSec							FLASHIS_AdrDDMinute+1		//������
//-----------------------------------------------------------------------------------
//����ʱ����Ϣ
#define FLASHIS_AdrYQYXSJ						FLASHIS_AdrDDTime+2			//��������ʱ��,��λS
#define FLASHIS_AdrCBYXSJ						FLASHIS_AdrYQYXSJ+2			//��������ʱ��,��λS
//------------------------------------------------------------------------------------
//��������װʱ��
#define FLASHIS_AdrO2AZSJ						FLASHIS_AdrCBYXSJ+2			//O2��������װʱ��
#define FLASHIS_AdrSOANSJ						  FLASHIS_AdrO2AZSJ+2			//SO��������װʱ��
#define FLASHIS_AdrNOANSJ						  FLASHIS_AdrSOANSJ+2			//NO��������װʱ��
#define FLASHIS_AdrNO2ANSJ						FLASHIS_AdrNOANSJ+2			//NO2��������װʱ��
#define FLASHIS_AdrCOANSJ						  FLASHIS_AdrNO2ANSJ+2		//CO��������װʱ��
#define FLASHIS_AdrH2SNSJ						  FLASHIS_AdrCOANSJ+2			//H2S��������װʱ��
#define FLASHIS_AdrCO2ANSJ						FLASHIS_AdrH2SNSJ+2			//CO2��������װʱ��

//����������
#define FLASHIS_AdrO2LQ                FLASHIS_AdrCO2ANSJ+2    //��������
#define FLASHIS_AdrSO2LQ               FLASHIS_AdrO2LQ+2    //����
#define FLASHIS_AdrNOLQ                FLASHIS_AdrSO2LQ+2    //����
#define FLASHIS_AdrNO2LQ               FLASHIS_AdrNOLQ+2    //����
#define FLASHIS_AdrCOLQ                FLASHIS_AdrNO2LQ+2    //����
#define FLASHIS_AdrH2SLQ               FLASHIS_AdrCOLQ+2    //����
#define FLASHIS_AdrCO2LQ               FLASHIS_AdrH2SLQ+2    //����


//------------------------------------------------------------------------------------
//����ά�����
#define FLASHIS_AdrCJWHMM					FLASHIS_AdrCO2LQ+2			//����ά�����룬Ĭ��ֵ4321
#define FLASHIS_AdrCJWHYQBH					FLASHIS_AdrCJWHMM+1			//������ţ�2���֣��޷�������32λ��Ĭ�ϣ�1234567890
#define FLASHIS_AdrCJWHRJBB					FLASHIS_AdrCJWHYQBH+2		//����汾�ţ�Ĭ�ϣ�V1.00

//����ֵ
#define FLASHIS_AdrBackupStart				FLASHIS_AdrCJWHRJBB+2		//����������ʼ��ַ
#define FLASHIS_AdrBackupEnd				FLASHIS_AdrBackupStart+(FLASHIS_AdrBDEnd-FLASHIS_AdrBDStart)

#define FLASHIS_AdrStoreFactory				FLASHIS_AdrBackupEnd+2//�ָ���������
#define FLASHIS_AdrEnd						FLASHIS_AdrStoreFactory+1//����


//�ṹ��
typedef struct
{
	u8 Flag;	//��־λ����=0��û�����ݸ��£���!=0�������ݸ���
	u8 Busy;	//��־λ����=0�򱣴����ݼĴ������У���!=0��æ	
	u8 RunMode;
	u16 Data[FLASHIS_AdrEnd];//���ݻ���
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










