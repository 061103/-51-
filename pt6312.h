#ifndef __pt6312_h__
#define __pt6312_h__
#include "STC12C5A.h"
#include <intrins.h>
/*-----------------------------------------------------------------------
����0-9����;
1=0x06;7=0x07;����=0x10;�»���=0x20;Բ��=0x40;
-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------
��������: PT6312/��pD16312/CS16312/SC16312ʾ������
�����ļ�: ��
���ú���: 
���ò���: ��
ʹ��˵��: 
-----------------------------------------------------------------------*/
#define uchar unsigned char
#define uint unsigned int
//-----------------------------------------------------------------------
sbit DIN =    P0^6;              // ����������
sbit CLK =    P0^4;               // ʱ���ź���
sbit STB =    P0^5;              // Ƭѡ�ź���
//-----------------------------------------------------------------------
#define MODE    0x06              // VFD ��λ����( 10λ*13��)   
#define LIGHT   0x07              // VFD ��������(0x00-0x07)   
#define ON      0x08              // ����ʾ   
#define OFF     0x00              // ����ʾ  

uchar dplay[]={0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce,0xd0,0xd2};
uchar writeshu[]={0xaf,0x06,0x9b,0x1f,0x36,0x3d,0xbd,0x07,0xbf,0x3f,0xb7,0xbf,0xa9,0xaf,0xb9,0xb1};
void delay1us(void)   //��� 0us
{
    _nop_();  //if Keil,require use intrins.h
}
void delay500ms(void)   //��� -0.000000000028us
{
    unsigned char a,b,c;
    for(c=254;c>0;c--)
        for(b=96;b>0;b--)
            for(a=60;a>0;a--);
}                                             
//-----------------------------------------------------------------------
/*-----------------------------------------------------------------------
��������: д����
��ڲ���: �� 
���ڲ���: �� 
����˵��: 
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
��������: ��ʼ��
��ڲ���: �� 
���ڲ���: �� 
����˵��: 
-----------------------------------------------------------------------*/
void VFD_Init(void)
{
  STB=0;
  Write_PT6312(MODE);                //  ��λ����
  delay1us();
  STB=1;
  
  STB=0;
  Write_PT6312(0x44);                //  �趨����
  delay1us();
  STB=1;
  
  STB=0;
  Write_PT6312(0xc0);                //  �趨��ַ
  delay1us();
  STB=1;
  
  STB=0;
  Write_PT6312(0x80|ON|LIGHT);       // ��ʾ����
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