#ifndef _1DS18B20_H_
#define _1DS18B20_H_
#include "STC12C5A.h"

sbit DQ1=P0^1;

/*****************************1us延时函数******************************/
void delay_us1(unsigned char a)//12MHz时钟时的1微秒延时函数
{
while (a--); 
}

/*****************************1ms延时函数******************************/
void delay_ms1(unsigned int a)//12MHz时钟时的1毫秒延时函数
{
unsigned int b;
while(a--)
{
  for(b=0;b<600;b++);
}
}

/******************************复位函数*******************************/
void DS18B201_Reset()
{
CY = 1;
while (CY)
{
  DQ1=0;//送出低电平复位信号
  delay_us1(255);//延时510us
  delay_us1(255);
  DQ1=1;//释放数据线
  delay_us1(60);//等待60us
  CY=DQ1;//检测存在脉冲
  delay_us1(200);//等待设备释放数据线
  delay_us1(150);
}
}

/******************************读字节函数*******************************/
unsigned char DS18B201_ReadByte()
{
unsigned char i;
unsigned char dat = 0;
for(i=0;i<8;i++)//8位计数器
  {
   dat>>=1;
   DQ1=0;//开始时间片
   delay_us1(1);//延时等待
   DQ1=1;//准备接收
   delay_us1(2);//接收延时
   if (DQ1)
    dat|= 0x80;//读取数据
   delay_us1(42);//等待时间片结束
  }
return dat;
}

/****************************写字节函数*********************************/
void DS18B201_WriteByte(unsigned char dat)
{
unsigned char i;
for(i=0;i<8;i++)//8位计数器
  {
   DQ1=0;//开始时间片
   delay_us1(12);//延时等待
   dat>>=1;//送出数据
   DQ1=CY;
   delay_us1(35);//等待时间片结束
   DQ1=1;//恢复数据线
   delay_us1(0);//恢复延时
  }
}
/*******************************获取温度函数******************************/
int get_temper1()
{
unsigned int temp;
unsigned int wendu;//温度
unsigned char TPH;//存放温度值的高字节
unsigned char TPL;//存放温度值的低字节
DS18B201_Reset();//设备复位
DS18B201_WriteByte(0xCC);//跳过ROM命令
DS18B201_WriteByte(0x44);//开始转换命令
delay_ms1(800);//等待800ms
DS18B201_Reset();//设备复位
DS18B201_WriteByte(0xCC);//跳过ROM命令
DS18B201_WriteByte(0xBE);//读暂存存储器命令
TPL=DS18B201_ReadByte();//读温度低字节
TPH=DS18B201_ReadByte();//读温度高字节    
temp=(TPH<<8)|TPL;    //合并
if(TPH>200)//如果是负温度 
  temp=~temp+1;//取反,加一
wendu=temp*0.625;//转换成十进制
if(TPH>200)//如果是负温度
  wendu=-wendu;//转换成负数
return(wendu);//返回温度
}

#endif