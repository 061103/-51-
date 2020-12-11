#ifndef __STC12CEEPROM_H__
#define __STC12CEEPROM_H__
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
//定义Flash 操作等待时间及允许IAP/ISP/EEPROM 操作的常数
//#define ENABLE_ISP 0X80//晶振小于30M
//#define ENABLE_ISP 0X81//晶振小于24M
#define ENABLE_ISP 0X82//晶振小于20M
//#define ENABLE_ISP 0X83//晶振小于12M
//#define ENABLE_ISP 0X84//晶振小于6M
//#define ENABLE_ISP 0X85//晶振小于3M
//用于STC12C2052
//sfr  IAP_DATA   =0XE2;
//sfr  IAP_ADDRH  =0XE3;
//sfr  IAP_ADDRL  =0XE4;
//sfr  IAP_CMD    =0XE5;
//sfr  IAP_TRIG   =0XE6;
//sfr  IAP_CONTR  =0XE7;
//#define  WD1  0X46
//#define  WD2  0XB9
//用于STC12C5A60S3
//sfr  IAP_DATA   =0XC2;
//sfr  IAP_ADDRH  =0Xc3;
//sfr  IAP_ADDRL  =0Xc4;
//sfr  IAP_CMD    =0Xc5;
//sfr  IAP_TRIG   =0Xc6;
//sfr  IAP_CONTR  =0Xc7;
#define  WD1  0X5A
#define  WD2  0XA5

uchar Byte_read(uint add);//读一字节 调用前打开IAP
void Byte_write(uint add,uchar ch);//字节编程
void sector_Erase(uint add);//擦除扇区
void IAP_Disable();//关闭IAP
//读一字节 调用前打开IAP
uchar Byte_read(uint add)
{
        IAP_DATA=0X00;
        IAP_CONTR=ENABLE_ISP;
        IAP_CMD =0X01;
        IAP_ADDRH=add/256;
        IAP_ADDRL =add%256;
        
        IAP_TRIG  =WD1;
        IAP_TRIG  =WD2;
        _nop_();_nop_();
        IAP_Disable();
        return (IAP_DATA);
}
//字节编程
void Byte_write(uint add,uchar ch)
{
        IAP_CONTR  =ENABLE_ISP;
        IAP_CMD =0X02;
        IAP_ADDRH=add/256;
        IAP_ADDRL =add%256;
        IAP_DATA=ch;
        IAP_TRIG  =WD1;
        IAP_TRIG  =WD2;
        _nop_();_nop_();
        IAP_Disable();
}
//擦除扇区
void sector_Erase(uint add)
{
        IAP_CONTR =ENABLE_ISP;
        IAP_CMD =0X03;
        IAP_ADDRH=add/256;
        IAP_ADDRL =add%256;
        IAP_TRIG  =WD1;
        IAP_TRIG  =WD2;
        _nop_();_nop_();
        IAP_Disable();
}
//关闭IAP
void IAP_Disable()
{
        IAP_CONTR =0;
        IAP_CMD =0;
        IAP_TRIG  =0;
        IAP_ADDRH=0;
        IAP_ADDRL =0;        
}
#endif