#ifndef _LCD_H_
#define _LCD_H_

#include"head.h"

/********************�޸Ĵ˶θı���ƶ˿�*********************/
#define  DAT_PIN	P7				//���ݲ���
sbit RS = P2^5;				//����/����ѡ��� H/L
sbit RW = P2^6;				//��/дѡ��� H/L
sbit EN = P2^7;
/*************************************************************/
sbit BUSY = DAT_PIN^7;




#if 0							/*��������������һ���û���������������Ҫ����*/
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

void write_char(unsigned char addr,unsigned char cha); //0-32,���ݣ��ַ���			/* һ�㽫�˺����� Display_LCD1602() ������*/	
void write_string(unsigned char addr,unsigned char *p);//0-32,���ݣ��ַ�����		/* һ�㽫�˺����� Display_LCD1602() ������*/

void Init_LCD1602();																/* -1����ʼ��LCD1602���� */
void Display_LCD1602();																/* -2���˺���������ʾLCD1602����õ���*/

#endif
