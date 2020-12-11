#ifndef _DS18B20_H_
#define _DS18B20_H_
#include "STC12C5A.h"

sbit DQ0=P0^0;

/*****************************1us延时函数******************************/
void delay_us(unsigned char a)//12MHz时钟时的1微秒延时函数
{
while (a--); 
}

/*****************************1ms延时函数******************************/
void delay_ms(unsigned int a)//12MHz时钟时的1毫秒延时函数
{
unsigned int b;
while(a--)
{
  for(b=0;b<600;b++);
}
}

/******************************复位函数*******************************/
void DS18B20_Reset()
{
CY = 1;
while (CY)
{
  DQ0=0;//送出低电平复位信号
  delay_us(255);//延时510us
  delay_us(255);
  DQ0=1;//释放数据线
  delay_us(60);//等待60us
  CY=DQ0;//检测存在脉冲
  delay_us(200);//等待设备释放数据线
  delay_us(150);
}
}

/******************************读字节函数*******************************/
unsigned char DS18B20_ReadByte()
{
unsigned char i;
unsigned char dat = 0;
for(i=0;i<8;i++)//8位计数器
  {
   dat>>=1;
   DQ0=0;//开始时间片
   delay_us(1);//延时等待
   DQ0=1;//准备接收
   delay_us(2);//接收延时
   if (DQ0)
    dat|= 0x80;//读取数据
   delay_us(42);//等待时间片结束
  }
return dat;
}

/****************************写字节函数*********************************/
void DS18B20_WriteByte(unsigned char dat)
{
unsigned char i;
for(i=0;i<8;i++)//8位计数器
  {
   DQ0=0;//开始时间片
   delay_us(12);//延时等待
   dat>>=1;//送出数据
   DQ0=CY;
   delay_us(35);//等待时间片结束
   DQ0=1;//恢复数据线
   delay_us(0);//恢复延时
  }
}
/*******************************获取温度函数******************************/
int get_temper()
{
unsigned int temp;
unsigned int wendu;//温度
unsigned char TPH;//存放温度值的高字节
unsigned char TPL;//存放温度值的低字节
DS18B20_Reset();//设备复位
DS18B20_WriteByte(0xCC);//跳过ROM命令
DS18B20_WriteByte(0x44);//开始转换命令
delay_ms(800);//等待800ms
DS18B20_Reset();//设备复位
DS18B20_WriteByte(0xCC);//跳过ROM命令
DS18B20_WriteByte(0xBE);//读暂存存储器命令
TPL=DS18B20_ReadByte();//读温度低字节
TPH=DS18B20_ReadByte();//读温度高字节    
temp=(TPH<<8)|TPL;    //合并
if(TPH>200)//如果是负温度 
  temp=~temp+1;//取反,加一
wendu=temp*0.625;//转换成十进制
if(TPH>200)//如果是负温度
  wendu=-wendu;//转换成负数
return(wendu);//返回温度
}

#endif