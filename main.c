#include "STC12C5A.h"
#include "DS18B20.h"
#include "1DS18B20.h"
#include "pwm.h"
#include "pt6312.h"
#include "ir.h"
#include "SG90.h"
#include "eeprom.h"
#include "intrins.h"
sbit IN_OFF=P3^0;
sbit K1=P2^4;
sbit K2=P2^5;
sbit ledtest=P3^5;
sbit Beep=P0^2 ;//beep
unsigned int led_temp(unsigned int led);
unsigned int lcd_temp(unsigned int lcd);
void vfd_display_4(unsigned int four);
void ircodedisp();
void PWMled();
void proir_data();
void Bp();
void keypro();
void fin_result(unsigned int fin);
//定义温度全局变量
unsigned char one,s,a;
unsigned int fin;
void delayS500ms()   //误差 -0.000000000028us
{
    unsigned char a,b,c;
    for(c=254;c>0;c--)
        for(b=96;b>0;b--)
            for(a=60;a>0;a--);
}
//延时子函数
void delay(unsigned int time)
{
	while(time--);
}
void main()
{  
	unsigned int wds,wds1;
	P1M1=0x00;
    P1M0=0x20;
	EX1init();            //初始化外部中断1
    TIM0init();           //初始化定时器0
	IN_OFF=1; /*免复位下载*/
	VFD_Init();/*PT6312初始化函数*/
    init_PWM();/*PWM初始化函数*/
	PWM1_set(255); /*显示屏风扇*/ 
	PWM0_set(255); /*电源风扇*/
    test_display();/*VFD全亮*/
	delayS500ms(); /*延时0.5秒*/
	one=Byte_read(0x0000);
	high=one*10;
	vfd_display_4(high);
	SG90PRO();
	while(1)
	{ 
		 if(IN_OFF==0||IN_OFF!=1) IAP_CONTR = 0x60;	 //复位到IAP
		 if(K1==1&&K2==1){
			 ledtest=0;					         //工作状态指示灯
			 wds=led_temp(get_temper());		 //LED温度获取并显示
			 wds1=lcd_temp(get_temper1());	 //获取液晶屏温度并显示
			 ledtest=1;					         //工作状态指示灯	   
			 if(irok) Ircordpro();               //如果接收好了进行红外处理
			 proir_data();					     //处理垂直梯形校正
		 	 }else keypro();					 //按键处理				  
		 /*PWM0_setLED风扇150开始转*/
		 if(wds>550) {
		 	while(wds>550){
			PWM0_set(1);
			Bp();
			wds=led_temp(get_temper());
			}
		 }
		 if(wds>500){
			 unsigned int ledwendu_agin;
			 ledwendu_agin=led_temp(get_temper());	 //再次获取LED温度值与之前获取的LED温度值进行比较
			 if(ledwendu_agin>wds){					 //如果温度持续上升
			 s=0;
			 a++;
			 if(a==1) {fin=95;fin_result(fin);PWM0_set(fin);}
			 if(a==2) {fin=94;fin_result(fin);PWM0_set(fin);}
			 if(a==3) {fin=93;fin_result(fin);PWM0_set(fin);}
			 if(a==4) {fin=92;fin_result(fin);PWM0_set(fin);}
			 if(a==5) {fin=91;fin_result(fin);PWM0_set(fin);}
			 if(a==6) {fin=90;fin_result(fin);PWM0_set(fin);}
			 }else if(ledwendu_agin<wds){			 //否则如果温度在慢慢下降
			 			s++;
						a=0;
						if(s==1) {fin=89;fin_result(fin);PWM0_set(fin);}
						if(s==2) {fin=90;fin_result(fin);PWM0_set(fin);}
						if(s==3) {fin=91;fin_result(fin);PWM0_set(fin);}
						if(s==4) {fin=92;fin_result(fin);PWM0_set(fin);}
						if(s==5) {fin=93;fin_result(fin);PWM0_set(fin);}
						if(s==6) {fin=94;fin_result(fin);PWM0_set(fin);}
						if(s==7) {fin=95;fin_result(fin);PWM0_set(fin);}
				 	}
		  		}else{
			    PWM0_set(255);
			}
		/*PWM1_set液晶屏风扇 */
		if(wds1>400) PWM1_set(100);			//液晶屏高于40度，液晶屏风扇持续开启
	 	if(wds1>500) {PWM1_set(1);Bp();delayS500ms();Bp();}  //如果液晶屏温度大于50度，则蜂鸣器响
    }
}
void keypro()
{
		 if(K1==0)		//检测按键K1是否按下
		{
			if(high<1040&&high>=940){
			high=(high+10);
			SG90PRO();
			vfd_display_4(high);
			}else
			high=1040; 
		}
		 if(K2==0)		//检测按键K1是否按下
		{
			if(high>940&&high<=1040){
		    high=(high-10);
			SG90PRO();
			vfd_display_4(high);
			}else 
			high=940;
		}
	sector_Erase(0x0000);
	Byte_write(0x0000,high/10);
}
void fin_result(unsigned int fin)	 /*显示风扇调整值*/
{
unsigned char f,i,n;
		 f=fin/100;
		 i=fin%100/10;
		 n=fin%10;
		 
		 STB=0;
		 Write_PT6312(0xc6);//ADDRESS SETTING COMMANDS  ""
		 Write_PT6312(0);
		 STB=1;
				
		 STB=0;
		 Write_PT6312(0xc4);                //  设定地址
		 Write_PT6312(writeshu[f]);
		 STB=1;
				
		 STB=0;
		 Write_PT6312(0xc2);                //  设定地址
		 Write_PT6312(writeshu[i]);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xc0);                //  设定地址
		 Write_PT6312(writeshu[n]);
		 STB=1;
}
unsigned int led_temp(unsigned int led) //led温度拆分
{
	     unsigned char b,s,g;
		 b=led/100;
		 s=led%100/10;
		 g=led%10;

	     STB=0;
		 Write_PT6312(0xd2);                //  设定地址
		 Write_PT6312(writeshu[b]);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xd0);                //  设定地址
		 Write_PT6312(writeshu[s]);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xce);                //  设定地址
		 Write_PT6312(0x40);
		 STB=1;
		
		 STB=0;
		 Write_PT6312(0xcc);                //  设定地址
		 Write_PT6312(writeshu[g]);
		 STB=1;
		 
		 return b*100+s*10+g;
}
unsigned int lcd_temp(unsigned int lcd)//lcd温度拆分
{
	     unsigned char b,s,g;
	     STB=0;
		 b=lcd/100;
		 Write_PT6312(0xc6);                //  设定地址
		 Write_PT6312(writeshu[b]);
		 STB=1;

		 STB=0;
		 s=lcd%100/10;
		 Write_PT6312(0xc4);                //  设定地址
		 Write_PT6312(writeshu[s]);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xc2);                //  设定地址
		 Write_PT6312(0x40);
		 STB=1;
		
		 STB=0;
		 g=lcd%10;
		 Write_PT6312(0xc0);                //  设定地址
		 Write_PT6312(writeshu[g]);
		 STB=1;

		 return b*100+s*10+g;
}

void proir_data(){
if(irpro_ok)
	 {
	     TempData[0] = IRcord[0]/16;   //处理客户码
	     TempData[1] = IRcord[0]%16;
	     TempData[2] = IRcord[1]/16;   //处理客户码
	     TempData[3] = IRcord[1]%16;
	     TempData[4] = IRcord[2]/16;   //处理数据码
	     TempData[5] = IRcord[2]%16;
	     TempData[6] = IRcord[3]/16;   //处理数据反码
	     TempData[7] = IRcord[3]%16;
	     STB=0;
		 Write_PT6312(0xd2);                //  设定地址1
		 Write_PT6312(0xa9);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xd0);                //  设定地址2
		 Write_PT6312(0xaf);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xce);                //  设定地址3
		 Write_PT6312(0xaf);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xcc);                //  设定地址4
		 Write_PT6312(0xb9);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xca);                //  设定地址5
		 Write_PT6312(0);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xc8);                //  设定地址6
		 Write_PT6312(0);
		 STB=1;
		 
	     if(IRcord[3]==0xea){
				if(high<1040&&high>=940){
				high = high+10;
				SG90PRO();
				vfd_display_4(high);
				}else 
				high=1040;
			  }                         
	     if(IRcord[3]==0xeb){
				if(high>940&&high<=1040){
				high=(high-10);
				SG90PRO();
				vfd_display_4(high);
			}else 
			high=940;
		  }
	   }
	  sector_Erase(0x0000);
	 Byte_write(0x0000,high/10);
	irpro_ok=0;//处理完成标志
}
void vfd_display_4(unsigned int four)//vfd后4位显示数字
{
	STB=0;											
	Write_PT6312(0xc6);                //  设定地址7
	Write_PT6312(writeshu[four/1000]);
	STB=1;
				
	STB=0;
	Write_PT6312(0xc4);                //  设定地址8
	Write_PT6312(writeshu[four%1000/100]);
	STB=1;
			   
	STB=0;
	Write_PT6312(0xc2);                //  设定地址9
	Write_PT6312(writeshu[four%100/10]);
	STB=1;
			     
	STB=0;
	Write_PT6312(0xc0);                //  设定地址10
	Write_PT6312(writeshu[four%10]);
	STB=1;

}
void Bp() //蜂鸣器
{		
		Beep=0;
		delay(50000);		//开启蜂鸣延时
		delay(50000);
		Beep=1;
		delay(50000);		//关闭蜂鸣延时
		delay(50000);
}