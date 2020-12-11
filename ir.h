#ifndef __IR_h__
#define __IR_h__

#include "STC12C5A.h"
#define uint unsigned int
#define uchar unsigned char
sbit IR=P3^3;  //红外接口标志
unsigned char  irtime;//红外用全局变量

bit irpro_ok,irok;
unsigned char IRcord[4];
unsigned char irdata[33];

unsigned char TempData[10];
/*------------------------------------------------
                  函数声明
------------------------------------------------*/
void Ir_work(void);
void Ircordpro(void);

/*------------------------------------------------
                  定时器0中断处理
------------------------------------------------*/

void tim0_isr (void) interrupt 1 using 1
{
  irtime++;  //用于计数2个下降沿之间的时间
}
/*------------------------------------------------
                定时器0初始化
------------------------------------------------*/
void TIM0init(void)//定时器0初始化
{

  TMOD=0x02;//定时器0工作方式2，TH0是重装值，TL0是初值
  TH0=0x00; //重载值
  TL0=0x00; //初始化值
  ET0=1;    //开中断
  TR0=1;    
}
/*------------------------------------------------
                  外部中断1初始化
------------------------------------------------*/
void EX1init(void)
{
 IT1 = 1;   //指定外部中断1下降沿触发，INT1 (P3.3)
 EX1 = 1;   //使能外部中断
 EA = 1;    //开总中断
}
/*------------------------------------------------
                  外部中断1中断处理
------------------------------------------------*/
void EX1_ISR (void) interrupt 2 //外部中断1服务函数
{
  static unsigned char  i;             //接收红外信号处理
  static bit startflag;                //是否开始处理标志位
if(startflag)                         
   {
    if(irtime<63&&irtime>=33)//引导码 TC9012的头码，9ms+4.5ms
            i=0;
    		irdata[i]=irtime;//存储每个电平的持续时间，用于以后判断是0还是1
    		irtime=0;
    		i++;
   			 if(i==33)
      			{
	  			 irok=1;
				 i=0;
	  			}
          }
  	 else
		{
		irtime=0;
		startflag=1;
		}
}

/*------------------------------------------------
                 红外码值处理
------------------------------------------------*/
void Ircordpro(void)//红外码值处理函数
{ 
  unsigned char i, j, k;
  unsigned char cord,value;

  k=1;
  for(i=0;i<4;i++)      //处理4个字节
     {
      for(j=1;j<=8;j++) //处理1个字节8位
         {
          cord=irdata[k];
          if(cord>7)//大于某值为1，这个和晶振有绝对关系，这里使用12M计算，此值可以有一定误差
             value|=0x80;
          if(j<8)
		    {
			 value>>=1;
			}
           k++;
         }
	     IRcord[i]=value;
	     value=0;     
     } 
	 irpro_ok=1;//处理完毕标志位置
	 irok=0;    //中断完成标志
}

#endif