#ifndef _LCD_H_
#define _LCD_H_

#include"head.h"

/********************修改此段改变控制端口*********************/
#define  DAT_PIN	P7				//数据并口
sbit RS = P2^5;				//数据/命令选择端 H/L
sbit RW = P2^6;				//读/写选择端 H/L
sbit EN = P2^7;
/*************************************************************/
sbit BUSY = DAT_PIN^7;




#if 0							/*基本操作函数，一般用户除非特殊需求不需要设置*/
void wait();						
void write_cmd(unsigned char cmd);
void write_dat(unsigned char dat);
#endif

typedef enum
{
 	welcome, //welcome
	newpsw,	 //please set new password
	setagain,//please set again
	ok,		 //ok
	inppsw,	 //please input password
	succ,	 //succ
	error,	 //error
	change	 //change
}State;

extern State LCD1602_State;

void write_char(unsigned char addr,unsigned char cha); //0-32,数据（字符）			/* 一般将此函数在 Display_LCD1602() 中配置*/	
void write_string(unsigned char addr,unsigned char *p);//0-32,数据（字符串）		/* 一般将此函数在 Display_LCD1602() 中配置*/

void Init_LCD1602();																/* -1：初始化LCD1602函数 */
void Display_LCD1602();																/* -2：此函数用于显示LCD1602所配好的数*/

#endif
