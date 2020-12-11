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
//�����¶�ȫ�ֱ���
unsigned char one,s,a;
unsigned int fin;
void delayS500ms()   //��� -0.000000000028us
{
    unsigned char a,b,c;
    for(c=254;c>0;c--)
        for(b=96;b>0;b--)
            for(a=60;a>0;a--);
}
//��ʱ�Ӻ���
void delay(unsigned int time)
{
	while(time--);
}
void main()
{  
	unsigned int wds,wds1;
	P1M1=0x00;
    P1M0=0x20;
	EX1init();            //��ʼ���ⲿ�ж�1
    TIM0init();           //��ʼ����ʱ��0
	IN_OFF=1; /*�⸴λ����*/
	VFD_Init();/*PT6312��ʼ������*/
    init_PWM();/*PWM��ʼ������*/
	PWM1_set(255); /*��ʾ������*/ 
	PWM0_set(255); /*��Դ����*/
    test_display();/*VFDȫ��*/
	delayS500ms(); /*��ʱ0.5��*/
	one=Byte_read(0x0000);
	high=one*10;
	vfd_display_4(high);
	SG90PRO();
	while(1)
	{ 
		 if(IN_OFF==0||IN_OFF!=1) IAP_CONTR = 0x60;	 //��λ��IAP
		 if(K1==1&&K2==1){
			 ledtest=0;					         //����״ָ̬ʾ��
			 wds=led_temp(get_temper());		 //LED�¶Ȼ�ȡ����ʾ
			 wds1=lcd_temp(get_temper1());	 //��ȡҺ�����¶Ȳ���ʾ
			 ledtest=1;					         //����״ָ̬ʾ��	   
			 if(irok) Ircordpro();               //������պ��˽��к��⴦��
			 proir_data();					     //����ֱ����У��
		 	 }else keypro();					 //��������				  
		 /*PWM0_setLED����150��ʼת*/
		 if(wds>550) {
		 	while(wds>550){
			PWM0_set(1);
			Bp();
			wds=led_temp(get_temper());
			}
		 }
		 if(wds>500){
			 unsigned int ledwendu_agin;
			 ledwendu_agin=led_temp(get_temper());	 //�ٴλ�ȡLED�¶�ֵ��֮ǰ��ȡ��LED�¶�ֵ���бȽ�
			 if(ledwendu_agin>wds){					 //����¶ȳ�������
			 s=0;
			 a++;
			 if(a==1) {fin=95;fin_result(fin);PWM0_set(fin);}
			 if(a==2) {fin=94;fin_result(fin);PWM0_set(fin);}
			 if(a==3) {fin=93;fin_result(fin);PWM0_set(fin);}
			 if(a==4) {fin=92;fin_result(fin);PWM0_set(fin);}
			 if(a==5) {fin=91;fin_result(fin);PWM0_set(fin);}
			 if(a==6) {fin=90;fin_result(fin);PWM0_set(fin);}
			 }else if(ledwendu_agin<wds){			 //��������¶��������½�
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
		/*PWM1_setҺ�������� */
		if(wds1>400) PWM1_set(100);			//Һ��������40�ȣ�Һ�������ȳ�������
	 	if(wds1>500) {PWM1_set(1);Bp();delayS500ms();Bp();}  //���Һ�����¶ȴ���50�ȣ����������
    }
}
void keypro()
{
		 if(K1==0)		//��ⰴ��K1�Ƿ���
		{
			if(high<1040&&high>=940){
			high=(high+10);
			SG90PRO();
			vfd_display_4(high);
			}else
			high=1040; 
		}
		 if(K2==0)		//��ⰴ��K1�Ƿ���
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
void fin_result(unsigned int fin)	 /*��ʾ���ȵ���ֵ*/
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
		 Write_PT6312(0xc4);                //  �趨��ַ
		 Write_PT6312(writeshu[f]);
		 STB=1;
				
		 STB=0;
		 Write_PT6312(0xc2);                //  �趨��ַ
		 Write_PT6312(writeshu[i]);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xc0);                //  �趨��ַ
		 Write_PT6312(writeshu[n]);
		 STB=1;
}
unsigned int led_temp(unsigned int led) //led�¶Ȳ��
{
	     unsigned char b,s,g;
		 b=led/100;
		 s=led%100/10;
		 g=led%10;

	     STB=0;
		 Write_PT6312(0xd2);                //  �趨��ַ
		 Write_PT6312(writeshu[b]);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xd0);                //  �趨��ַ
		 Write_PT6312(writeshu[s]);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xce);                //  �趨��ַ
		 Write_PT6312(0x40);
		 STB=1;
		
		 STB=0;
		 Write_PT6312(0xcc);                //  �趨��ַ
		 Write_PT6312(writeshu[g]);
		 STB=1;
		 
		 return b*100+s*10+g;
}
unsigned int lcd_temp(unsigned int lcd)//lcd�¶Ȳ��
{
	     unsigned char b,s,g;
	     STB=0;
		 b=lcd/100;
		 Write_PT6312(0xc6);                //  �趨��ַ
		 Write_PT6312(writeshu[b]);
		 STB=1;

		 STB=0;
		 s=lcd%100/10;
		 Write_PT6312(0xc4);                //  �趨��ַ
		 Write_PT6312(writeshu[s]);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xc2);                //  �趨��ַ
		 Write_PT6312(0x40);
		 STB=1;
		
		 STB=0;
		 g=lcd%10;
		 Write_PT6312(0xc0);                //  �趨��ַ
		 Write_PT6312(writeshu[g]);
		 STB=1;

		 return b*100+s*10+g;
}

void proir_data(){
if(irpro_ok)
	 {
	     TempData[0] = IRcord[0]/16;   //����ͻ���
	     TempData[1] = IRcord[0]%16;
	     TempData[2] = IRcord[1]/16;   //����ͻ���
	     TempData[3] = IRcord[1]%16;
	     TempData[4] = IRcord[2]/16;   //����������
	     TempData[5] = IRcord[2]%16;
	     TempData[6] = IRcord[3]/16;   //�������ݷ���
	     TempData[7] = IRcord[3]%16;
	     STB=0;
		 Write_PT6312(0xd2);                //  �趨��ַ1
		 Write_PT6312(0xa9);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xd0);                //  �趨��ַ2
		 Write_PT6312(0xaf);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xce);                //  �趨��ַ3
		 Write_PT6312(0xaf);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xcc);                //  �趨��ַ4
		 Write_PT6312(0xb9);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xca);                //  �趨��ַ5
		 Write_PT6312(0);
		 STB=1;

		 STB=0;
		 Write_PT6312(0xc8);                //  �趨��ַ6
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
	irpro_ok=0;//������ɱ�־
}
void vfd_display_4(unsigned int four)//vfd��4λ��ʾ����
{
	STB=0;											
	Write_PT6312(0xc6);                //  �趨��ַ7
	Write_PT6312(writeshu[four/1000]);
	STB=1;
				
	STB=0;
	Write_PT6312(0xc4);                //  �趨��ַ8
	Write_PT6312(writeshu[four%1000/100]);
	STB=1;
			   
	STB=0;
	Write_PT6312(0xc2);                //  �趨��ַ9
	Write_PT6312(writeshu[four%100/10]);
	STB=1;
			     
	STB=0;
	Write_PT6312(0xc0);                //  �趨��ַ10
	Write_PT6312(writeshu[four%10]);
	STB=1;

}
void Bp() //������
{		
		Beep=0;
		delay(50000);		//����������ʱ
		delay(50000);
		Beep=1;
		delay(50000);		//�رշ�����ʱ
		delay(50000);
}