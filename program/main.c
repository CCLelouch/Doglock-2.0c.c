/****************************************************************
/*�ļ���������			//@11.0592MHz
/*���ܣ�  ʵ�ּ�������
/*˵����  ��ʵ��1602��ʾ�����󰴼����룬������䣬�����������忴��������ͼ
/*�޸����ڣ�2017-07-20
/*�汾������1.0
/*���ߣ�c.c. 
/***************************************************************/
#include"head.h"
#include"LCD1602.h"
#include"Delay.H"
#include "coded_lock.h"
#include"key.h"

uchar Txt_PWS[12]={0,0,0,0,0,0,	0,0,0,0,0,0};//����û������ֵ
void STC15W_IOinit(void)
{
	P0M1 = 0;	P0M0 = 0;	
	P1M1 = 0;	P1M0 = 0;	
	P2M1 = 0;	P2M0 = 0;
	P3M1 = 0;	P3M0 = 0;		   
	P4M1 = 0;	P4M0 = 0;	
	P5M1 = 0;	P5M0 = 0;	
	P6M1 = 0;	P6M0 = 0;	
	P7M1 = 0;	P7M0 = 0;	
}

void main()
{
	STC15W_IOinit();
	Init_LCD1602();

	LCD1602_State = 0;			   /*������ʼ״̬*/
	Display_LCD1602();			   /*������ʾ��ӭʹ��*/
	Delay_ms(2000);

	Gain_E2PROM_PWS();			   /*������ȡ����*/
	while(1)
	{
		Delay(10);
		Display_LCD1602();
		Delay_ms(500);
		switch (LCD1602_State)
		{
/*welcome*/	case welcome:	Gain_E2PROM_PWS();break;//�������&��������
/*Set*/			case newpsw:	Password_Set();break;//��������1
/*AGAIN*/		case setagain:  Password_Set();break;//��������2
/*OK*/			case ok:		Send_E2PROM_PWS();LCD1602_State=4;break;//��������
/*pleaseip*/case inppsw:	Password_checked();break;//�������
/*SUCC*/		case succ:		P55=0;Delay_ms(60);P55=1;Delay_ms(60); P55=0;Delay_ms(60);P55=1;Delay_ms(60); P55=0;Delay_ms(60);P55=1;LCD1602_State=change;break;		//������ȷ��SUCC�εε�����
						case error:		P55=0;Delay_ms(3000);P55=1;LCD1602_State=welcome;break;//����
/*Change?*/	case change:
				{
					while(!KEY16_Flag());
					if(KEY16_scan()==14)
					{
						LCD1602_State=newpsw;
					}
					while(!KEY16_Flag());
					if(KEY16_scan()==15)
					{
						LCD1602_State=inppsw;
					};break;
				}
			default:break;
		}
	}
}