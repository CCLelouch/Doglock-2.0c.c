/****************************************************************
/*文件名：旺财			//@11.0592MHz
/*功能：  实现简单密码锁
/*说明：  简单实现1602显示，矩阵按键输入，掉电记忆，密码锁。具体看配套流程图
/*修改日期：2017-07-20
/*版本：旺财1.0
/*作者：c.c. 
/***************************************************************/
#include"head.h"
#include"LCD1602.h"
#include"Delay.H"
#include "coded_lock.h"
#include"key.h"

uchar Txt_PWS[12]={0,0,0,0,0,0,	0,0,0,0,0,0};//存放用户输入键值
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

	LCD1602_State = 0;			   /*开机初始状态*/
	Display_LCD1602();			   /*开机显示欢迎使用*/
	Delay_ms(2000);

	Gain_E2PROM_PWS();			   /*开机获取密码*/
	while(1)
	{
		Delay(10);
		Display_LCD1602();
		Delay_ms(500);
		switch (LCD1602_State)
		{
/*welcome*/	case welcome:	Gain_E2PROM_PWS();break;//开机检测&报警重置
/*Set*/			case newpsw:	Password_Set();break;//设置密码1
/*AGAIN*/		case setagain:  Password_Set();break;//设置密码2
/*OK*/			case ok:		Send_E2PROM_PWS();LCD1602_State=4;break;//发送密码
/*pleaseip*/case inppsw:	Password_checked();break;//配对密码
/*SUCC*/		case succ:		P55=0;Delay_ms(60);P55=1;Delay_ms(60); P55=0;Delay_ms(60);P55=1;Delay_ms(60); P55=0;Delay_ms(60);P55=1;LCD1602_State=change;break;		//密码正确，SUCC滴滴滴三声
						case error:		P55=0;Delay_ms(3000);P55=1;LCD1602_State=welcome;break;//报警
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