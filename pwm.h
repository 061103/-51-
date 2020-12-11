#ifndef _PWM_H_
#define _PWM_H_
#include "STC12C5A.h"
void init_PWM() 
{     
CMOD=0X08;     //0X00：以系统时钟/12 为时钟源，0X02：系统时钟/2，0x08：系统时钟
CH=0;    	//PCA的16位计数器高八位
CL=0;       //PCA的16位计数器低八位
/**********************P1.3************************************/
CCAPM0=0X42;   //0X42：8位PWM P1.3输出,无中断；0X53：8位PWM输出，下降沿产生中断；0X63：上升沿产生中断；0X73：跳变沿产生中断
CCAP0H=0x00;  //  占空比控制 
CCAP0L=0x00;    
/**********************P1.4************************************/
CCAPM1=0x42; //PWM1设置PCA工作方式为PWM方式（使用时删除//）  
CCAP1L=0x00; //设置PWM1初始值与CCAP0H相同   
CCAP1H=0x00; // PWM1初始时为0
CR=1;          //计时器开始工作 
}   

void PWM0_set(unsigned char empty)  //直接设置占空比 {       CCAP0L=empty;    CCAP0H=empty; }
	{       
	CCAP0L=empty;    
	CCAP0H=empty; 
	}

void PWM1_set(unsigned char empty)  //直接设置占空比 {       CCAP0L=empty;    CCAP0H=empty; }
	{       
	CCAP1L=empty;    
	CCAP1H=empty; 
	}
#endif