#ifndef _DS18B20_H_
#define _DS18B20_H_
#include "STC12C5A.h"

sbit DQ0=P0^0;

/*****************************1us��ʱ����******************************/
void delay_us(unsigned char a)//12MHzʱ��ʱ��1΢����ʱ����
{
while (a--); 
}

/*****************************1ms��ʱ����******************************/
void delay_ms(unsigned int a)//12MHzʱ��ʱ��1������ʱ����
{
unsigned int b;
while(a--)
{
  for(b=0;b<600;b++);
}
}

/******************************��λ����*******************************/
void DS18B20_Reset()
{
CY = 1;
while (CY)
{
  DQ0=0;//�ͳ��͵�ƽ��λ�ź�
  delay_us(255);//��ʱ510us
  delay_us(255);
  DQ0=1;//�ͷ�������
  delay_us(60);//�ȴ�60us
  CY=DQ0;//����������
  delay_us(200);//�ȴ��豸�ͷ�������
  delay_us(150);
}
}

/******************************���ֽں���*******************************/
unsigned char DS18B20_ReadByte()
{
unsigned char i;
unsigned char dat = 0;
for(i=0;i<8;i++)//8λ������
  {
   dat>>=1;
   DQ0=0;//��ʼʱ��Ƭ
   delay_us(1);//��ʱ�ȴ�
   DQ0=1;//׼������
   delay_us(2);//������ʱ
   if (DQ0)
    dat|= 0x80;//��ȡ����
   delay_us(42);//�ȴ�ʱ��Ƭ����
  }
return dat;
}

/****************************д�ֽں���*********************************/
void DS18B20_WriteByte(unsigned char dat)
{
unsigned char i;
for(i=0;i<8;i++)//8λ������
  {
   DQ0=0;//��ʼʱ��Ƭ
   delay_us(12);//��ʱ�ȴ�
   dat>>=1;//�ͳ�����
   DQ0=CY;
   delay_us(35);//�ȴ�ʱ��Ƭ����
   DQ0=1;//�ָ�������
   delay_us(0);//�ָ���ʱ
  }
}
/*******************************��ȡ�¶Ⱥ���******************************/
int get_temper()
{
unsigned int temp;
unsigned int wendu;//�¶�
unsigned char TPH;//����¶�ֵ�ĸ��ֽ�
unsigned char TPL;//����¶�ֵ�ĵ��ֽ�
DS18B20_Reset();//�豸��λ
DS18B20_WriteByte(0xCC);//����ROM����
DS18B20_WriteByte(0x44);//��ʼת������
delay_ms(800);//�ȴ�800ms
DS18B20_Reset();//�豸��λ
DS18B20_WriteByte(0xCC);//����ROM����
DS18B20_WriteByte(0xBE);//���ݴ�洢������
TPL=DS18B20_ReadByte();//���¶ȵ��ֽ�
TPH=DS18B20_ReadByte();//���¶ȸ��ֽ�    
temp=(TPH<<8)|TPL;    //�ϲ�
if(TPH>200)//����Ǹ��¶� 
  temp=~temp+1;//ȡ��,��һ
wendu=temp*0.625;//ת����ʮ����
if(TPH>200)//����Ǹ��¶�
  wendu=-wendu;//ת���ɸ���
return(wendu);//�����¶�
}

#endif