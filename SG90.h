#ifndef __SG90_h__
#define __SG90_h__
#include "STC12C5A.h"
#define uchar unsigned char
#define uint unsigned int

sbit PWMSG90=P1^0;

uint high,low;
uint cycle=20000;
void delaysg(uint t)
{
	while(t--);
}
void SG90PRO()
{ 
uchar s=20;
	while(s--)
	{
		PWMSG90=1;
		delaysg(high);
		low=cycle-high;
		PWMSG90=0;
		delaysg(low);
		if(s==0) break;
	}
}
#endif