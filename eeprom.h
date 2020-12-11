#ifndef __STC12CEEPROM_H__
#define __STC12CEEPROM_H__
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
//����Flash �����ȴ�ʱ�估����IAP/ISP/EEPROM �����ĳ���
//#define ENABLE_ISP 0X80//����С��30M
//#define ENABLE_ISP 0X81//����С��24M
#define ENABLE_ISP 0X82//����С��20M
//#define ENABLE_ISP 0X83//����С��12M
//#define ENABLE_ISP 0X84//����С��6M
//#define ENABLE_ISP 0X85//����С��3M
//����STC12C2052
//sfr  IAP_DATA   =0XE2;
//sfr  IAP_ADDRH  =0XE3;
//sfr  IAP_ADDRL  =0XE4;
//sfr  IAP_CMD    =0XE5;
//sfr  IAP_TRIG   =0XE6;
//sfr  IAP_CONTR  =0XE7;
//#define  WD1  0X46
//#define  WD2  0XB9
//����STC12C5A60S3
//sfr  IAP_DATA   =0XC2;
//sfr  IAP_ADDRH  =0Xc3;
//sfr  IAP_ADDRL  =0Xc4;
//sfr  IAP_CMD    =0Xc5;
//sfr  IAP_TRIG   =0Xc6;
//sfr  IAP_CONTR  =0Xc7;
#define  WD1  0X5A
#define  WD2  0XA5

uchar Byte_read(uint add);//��һ�ֽ� ����ǰ��IAP
void Byte_write(uint add,uchar ch);//�ֽڱ��
void sector_Erase(uint add);//��������
void IAP_Disable();//�ر�IAP
//��һ�ֽ� ����ǰ��IAP
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
//�ֽڱ��
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
//��������
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
//�ر�IAP
void IAP_Disable()
{
        IAP_CONTR =0;
        IAP_CMD =0;
        IAP_TRIG  =0;
        IAP_ADDRH=0;
        IAP_ADDRL =0;        
}
#endif