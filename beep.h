#ifndef _BEEP_H_
#define _BEEP_H_

#include "STC12C5A.h"

sbit Beep=P0^2 ;//beep
/*******************************************************************************
* ������         : Delay()
* ��������		   : ��ʱ����
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void delay1ms(void)   //��� 0us
{
    unsigned char a,b;
    for(b=199;b>0;b--)
        for(a=1;a>0;a--);
}
void Bp()
{	unsigned int x;
	for (x;x<600;x++)
	{
	Beep=0;
	delay1ms();
	Beep=1;
	delay1ms();
	}	
	
}


#endif