#ifndef __pt6312_h__
#define __pt6312_h__
#include "STC12C5A.h"
#include <intrins.h>
/*-----------------------------------------------------------------------
数字0-9定义;
1=0x06;7=0x07;逗号=0x10;下划线=0x20;圆点=0x40;
-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------
函数功能: PT6312/μpD16312/CS16312/SC16312示范程序
调用文件: 无
调用函数: 
调用参数: 无
使用说明: 
-----------------------------------------------------------------------*/
#define uchar unsigned char
#define uint unsigned int
//-----------------------------------------------------------------------
sbit DIN =    P0^6;              // 数据输入线
sbit CLK =    P0^4;               // 时钟信号线
sbit STB =    P0^5;              // 片选信号线
//-----------------------------------------------------------------------
#define MODE    0x06              // VFD 段位设置( 10位*13段)   
#define LIGHT   0x07              // VFD 亮度设置(0x00-0x07)   
#define ON      0x08              // 开显示   
#define OFF     0x00              // 关显示  

uchar dplay[]={0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce,0xd0,0xd2};
uchar writeshu[]={0xaf,0x06,0x9b,0x1f,0x36,0x3d,0xbd,0x07,0xbf,0x3f,0xb7,0xbf,0xa9,0xaf,0xb9,0xb1};
void delay1us(void)   //误差 0us
{
    _nop_();  //if Keil,require use intrins.h
}
void delay500ms(void)   //误差 -0.000000000028us
{
    unsigned char a,b,c;
    for(c=254;c>0;c--)
        for(b=96;b>0;b--)
            for(a=60;a>0;a--);
}                                             
//-----------------------------------------------------------------------
/*-----------------------------------------------------------------------
函数功能: 写数据
入口参数: 无 
出口参数: 无 
函数说明: 
-----------------------------------------------------------------------*/ 
void Write_PT6312(uchar dat)
{   
  uchar i;
  for(i=0;i<8;i++) 
   { 
     CLK=0;
     DIN=dat&0x01;
     dat=dat>>1;
     CLK=1;
	 DIN=1;
   }
}
/*-----------------------------------------------------------------------
函数功能: 初始化
入口参数: 无 
出口参数: 无 
函数说明: 
-----------------------------------------------------------------------*/
void VFD_Init(void)
{
  STB=0;
  Write_PT6312(MODE);                //  段位设置
  delay1us();
  STB=1;
  
  STB=0;
  Write_PT6312(0x44);                //  设定数据
  delay1us();
  STB=1;
  
  STB=0;
  Write_PT6312(0xc0);                //  设定地址
  delay1us();
  STB=1;
  
  STB=0;
  Write_PT6312(0x80|ON|LIGHT);       // 显示控制
  delay1us();
  STB=1;
}
void clear_vfd()
{
	unsigned char i;
    for (i=0;i<10;i++)
	{ 
	STB=0;
	Write_PT6312(dplay[i]);//ADDRESS SETTING COMMANDS  ""
	Write_PT6312(0);
	STB=1;         
	}    
}
void test_display()
{
	uchar i;
	
	for (i=0;i<10;i++)
	{ 
	STB=0;
	Write_PT6312(dplay[i]);//ADDRESS SETTING COMMANDS  ""
	delay1us();
	Write_PT6312(0xff);
	STB=1;         
	}
	    
	delay500ms();
	delay500ms();
	clear_vfd();
}

#endif