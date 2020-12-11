#ifndef _PWM_H_
#define _PWM_H_
#include "STC12C5A.h"
void init_PWM() 
{     
CMOD=0X08;     //0X00����ϵͳʱ��/12 Ϊʱ��Դ��0X02��ϵͳʱ��/2��0x08��ϵͳʱ��
CH=0;    	//PCA��16λ�������߰�λ
CL=0;       //PCA��16λ�������Ͱ�λ
/**********************P1.3************************************/
CCAPM0=0X42;   //0X42��8λPWM P1.3���,���жϣ�0X53��8λPWM������½��ز����жϣ�0X63�������ز����жϣ�0X73�������ز����ж�
CCAP0H=0x00;  //  ռ�ձȿ��� 
CCAP0L=0x00;    
/**********************P1.4************************************/
CCAPM1=0x42; //PWM1����PCA������ʽΪPWM��ʽ��ʹ��ʱɾ��//��  
CCAP1L=0x00; //����PWM1��ʼֵ��CCAP0H��ͬ   
CCAP1H=0x00; // PWM1��ʼʱΪ0
CR=1;          //��ʱ����ʼ���� 
}   

void PWM0_set(unsigned char empty)  //ֱ������ռ�ձ� {       CCAP0L=empty;    CCAP0H=empty; }
	{       
	CCAP0L=empty;    
	CCAP0H=empty; 
	}

void PWM1_set(unsigned char empty)  //ֱ������ռ�ձ� {       CCAP0L=empty;    CCAP0H=empty; }
	{       
	CCAP1L=empty;    
	CCAP1H=empty; 
	}
#endif