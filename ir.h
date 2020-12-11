#ifndef __IR_h__
#define __IR_h__

#include "STC12C5A.h"
#define uint unsigned int
#define uchar unsigned char
sbit IR=P3^3;  //����ӿڱ�־
unsigned char  irtime;//������ȫ�ֱ���

bit irpro_ok,irok;
unsigned char IRcord[4];
unsigned char irdata[33];

unsigned char TempData[10];
/*------------------------------------------------
                  ��������
------------------------------------------------*/
void Ir_work(void);
void Ircordpro(void);

/*------------------------------------------------
                  ��ʱ��0�жϴ���
------------------------------------------------*/

void tim0_isr (void) interrupt 1 using 1
{
  irtime++;  //���ڼ���2���½���֮���ʱ��
}
/*------------------------------------------------
                ��ʱ��0��ʼ��
------------------------------------------------*/
void TIM0init(void)//��ʱ��0��ʼ��
{

  TMOD=0x02;//��ʱ��0������ʽ2��TH0����װֵ��TL0�ǳ�ֵ
  TH0=0x00; //����ֵ
  TL0=0x00; //��ʼ��ֵ
  ET0=1;    //���ж�
  TR0=1;    
}
/*------------------------------------------------
                  �ⲿ�ж�1��ʼ��
------------------------------------------------*/
void EX1init(void)
{
 IT1 = 1;   //ָ���ⲿ�ж�1�½��ش�����INT1 (P3.3)
 EX1 = 1;   //ʹ���ⲿ�ж�
 EA = 1;    //�����ж�
}
/*------------------------------------------------
                  �ⲿ�ж�1�жϴ���
------------------------------------------------*/
void EX1_ISR (void) interrupt 2 //�ⲿ�ж�1������
{
  static unsigned char  i;             //���պ����źŴ���
  static bit startflag;                //�Ƿ�ʼ�����־λ
if(startflag)                         
   {
    if(irtime<63&&irtime>=33)//������ TC9012��ͷ�룬9ms+4.5ms
            i=0;
    		irdata[i]=irtime;//�洢ÿ����ƽ�ĳ���ʱ�䣬�����Ժ��ж���0����1
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
                 ������ֵ����
------------------------------------------------*/
void Ircordpro(void)//������ֵ������
{ 
  unsigned char i, j, k;
  unsigned char cord,value;

  k=1;
  for(i=0;i<4;i++)      //����4���ֽ�
     {
      for(j=1;j<=8;j++) //����1���ֽ�8λ
         {
          cord=irdata[k];
          if(cord>7)//����ĳֵΪ1������;����о��Թ�ϵ������ʹ��12M���㣬��ֵ������һ�����
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
	 irpro_ok=1;//������ϱ�־λ��
	 irok=0;    //�ж���ɱ�־
}

#endif