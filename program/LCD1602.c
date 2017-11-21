/***********************************************************************
** 文件名：LCD1602.c
** 功能：  LCD1602驱动程序
** 说明：C:LCD1602长数字输入变量
** 基本指令：
** 修改日期：2017-07-19
** 版本：V2.1
** 修改：修复了超过浮点型整数部分超过255无法正常显示问题
		 完善了浮点型到整数的转换显示,完善了端口配置
** 作者：c.c. 
************************************************************************/

#include"head.h"
#include"Delay.H"
#include"LCD1602.H"

extern bit flag;			  //分情况显示位
extern float C;				  //输入要显示的浮点型数值

State LCD1602_State;//不同状态标志位

unsigned char  FirstLine[16]="                ",SecondLine[16] ="                ";
//定义空间，显示为空，在字符再后加一个'\0'	这两数组用来单独赋值其中的一个字节空间

/****************************************************************
**函数：void wait()			
**功能：忙检测
**说明： BUSY(D7)为忙检测口
**修改日期：2017-07-07
**作者：c.c. 
****************************************************************/
void wait()					
{
	DAT_PIN = 0XFF;					//并行口初始化
	do
	{
		EN = 0;
		RS = 0;
		RW = 1;						  //读端口
		EN = 1;						  //高脉冲发送
	}
	while(BUSY);				 
	EN = 0;
}

void write_cmd(unsigned char cmd)  //写指令
{
	 wait();					  //每次写读指令数据前都要检测忙信号
 	 EN = 0;
	 RS = 0;
	 RW = 0;
	 DAT_PIN = cmd;
	 EN = 1;
	 EN = 0; 
}


void write_dat(unsigned char dat)  //写数据
{
 	wait();
 	EN = 0;
	RS = 1;
	RW = 0;
	DAT_PIN = dat;
	EN = 1;
	EN = 0;
}

void write_char(unsigned char addr,unsigned char cha)	//地址(0-27H,40H-67H)，数据
{
	if(addr<0x10)
		write_cmd(0x80+addr);		  //第一行 地址(0X80+0-27H,40H-67H，第一行第一个为0x80，第二行第一个为0xc0)
	else							 
		write_cmd(0xc0+addr);		  //第二行
	write_cmd(0x80+addr);
	write_dat(cha);
}

void write_string(unsigned char addr,unsigned char *p)	//地址(0-27H,40H-67H)，数据
{
	if(addr<0x10)
		write_cmd(0x80+addr);		  //第一行
	else							 
		write_cmd(0xb0+addr);		  //第二行
 	while(*p!='\0')			 		  //检测字符串后面的'\0'
	{
	 	write_dat(*p++);	  
	}
}
/****************************************************************
**函数：Init_LCD1602()			
**功能：初始化
**说明： 
**修改日期：2017-07-08
**作者：c.c. 
****************************************************************/
void Init_LCD1602()
{
	Delay_ms(15);
	write_cmd(0x38);	//显示模式设置
	write_cmd(0x38);	//显示模式设置
	write_cmd(0x38);	//显示模式设置
	write_cmd(0x06);	//开显示无光标
	write_cmd(0x0c);	//显示清屏：数据指针清零，所有显示清零
	write_cmd(0x01);	//显示光标移动设置
}
/****************************************************************
**函数：Init_LCD1602()			
**功能：初始化
**说明： 
**修改日期：2017-07-08
**作者：c.c. 
****************************************************************/
void Display_LCD1602()
{
	//	uchar i,j,k,l,m;			  //用于显示浮点型数字
	#if 0/**********遍历数组*************/
		FirstLine[0]='D';~	FirstLine[16]=' ';
	/*******************************/
	#elif 0/*************浮点型数字显示算法***************/		
			/******整数部分******/
			i=(unsigned int)C/100;
			j=(unsigned int)C%100/10;
			k=(unsigned int)C%10;
			/******小数部分******/	
			l=(unsigned int)(C*10)%10;
			m=(unsigned int)(C*100)%10;
	/**********************************************/
	#elif 0
			FirstLine[0]='D';
			FirstLine[1]='i';
			FirstLine[2]='s';
			FirstLine[3]='t';
			FirstLine[4]='a';
			FirstLine[5]='n';
			FirstLine[6]='c';
			FirstLine[7]='e';
			FirstLine[8]=':';	
			FirstLine[9] ='0'+i;
			FirstLine[10]='0'+j;
			FirstLine[11]='0'+k;
			FirstLine[12]='.';
			FirstLine[13]='0'+l;
			FirstLine[14]='0'+m;
	#elif 1
		Init_LCD1602();
		Delay(10);
	switch (LCD1602_State)
	{
	 	case welcome:	
			{
				FirstLine[0]='W';FirstLine[1]='e';FirstLine[2]='l'; FirstLine[3]='c'; FirstLine[4]='o'; FirstLine[5]='m'; FirstLine[6]='e'; FirstLine[7]=' '; 			 /* Welcome */
				FirstLine[8]=' ';FirstLine[9]=' ';FirstLine[10]=' ';FirstLine[11]=' ';FirstLine[12]=' ';FirstLine[13]=' ';FirstLine[14]=' ';FirstLine[15]=' ';break;
			}

		case newpsw:
			{
				FirstLine[0]='P';FirstLine[1]='l';FirstLine[2]='e'; FirstLine[3]='a'; FirstLine[4]='s'; FirstLine[5]='e'; FirstLine[6]=' '; FirstLine[7]='S';  		  	 /* Please  */
				FirstLine[8]='e';FirstLine[9]='t';FirstLine[10]=' ';FirstLine[11]='N';FirstLine[12]='e';FirstLine[13]='w';FirstLine[14]=' ';FirstLine[15]=' ';			 /* Set New */
				SecondLine[0]='P';SecondLine[1]='a';SecondLine[2]='s'; SecondLine[3]='s'; SecondLine[4]='w'; SecondLine[5]='o'; SecondLine[6]='r'; SecondLine[7]='d';  	 /*Password */
				SecondLine[8]=' ';SecondLine[9]=' ';SecondLine[10]=' ';SecondLine[11]=' ';SecondLine[12]=' ';SecondLine[13]=' ';SecondLine[14]=' ';SecondLine[15]=' ';break;
			}
					
		case setagain:
			{
				FirstLine[0]='P';FirstLine[1]='l';FirstLine[2]='e'; FirstLine[3]='a'; FirstLine[4]='s'; FirstLine[5]='e'; FirstLine[6]=' '; FirstLine[7]='S';  		  	 /* Please    */
				FirstLine[8]='e';FirstLine[9]='t';FirstLine[10]=' ';FirstLine[11]='A';FirstLine[12]='g';FirstLine[13]='a';FirstLine[14]='i';FirstLine[15]='n';			 /* Set Again */
				SecondLine[0]=' ';SecondLine[1]=' ';SecondLine[2]=' '; SecondLine[3]=' '; SecondLine[4]=' '; SecondLine[5]=' '; SecondLine[6]=' '; SecondLine[7]=' ';  	 /*           */
				SecondLine[8]=' ';SecondLine[9]=' ';SecondLine[10]=' ';SecondLine[11]=' ';SecondLine[12]=' ';SecondLine[13]=' ';SecondLine[14]=' ';SecondLine[15]=' ';break;
			}

		case ok:	
			{
				FirstLine[0]='O';FirstLine[1]='K';FirstLine[2]='!'; FirstLine[3]=' '; FirstLine[4]=' '; FirstLine[5]=' '; FirstLine[6]=' '; FirstLine[7]=' '; 			 /*    ok    */
				FirstLine[8]=' ';FirstLine[9]=' ';FirstLine[10]=' ';FirstLine[11]=' ';FirstLine[12]=' ';FirstLine[13]=' ';FirstLine[14]=' ';FirstLine[15]=' ';break;
			}

		case inppsw:	
			{
				FirstLine[0]='P';FirstLine[1]='l';FirstLine[2]='e'; FirstLine[3]='a'; FirstLine[4]='s'; FirstLine[5]='e'; FirstLine[6]=' '; FirstLine[7]='I';  		  	 /* Please  */
				FirstLine[8]='n';FirstLine[9]='p';FirstLine[10]='u';FirstLine[11]='t';FirstLine[12]=' ';FirstLine[13]=' ';FirstLine[14]=' ';FirstLine[15]=' ';			 /* Input   */
				SecondLine[0]='P';SecondLine[1]='a';SecondLine[2]='s'; SecondLine[3]='s'; SecondLine[4]='w'; SecondLine[5]='o'; SecondLine[6]='r'; SecondLine[7]='d';  	 /*Password */
				SecondLine[8]=' ';SecondLine[9]=' ';SecondLine[10]=' ';SecondLine[11]=' ';SecondLine[12]=' ';SecondLine[13]=' ';SecondLine[14]=' ';SecondLine[15]=' ';break;
			}

		case succ: 
			{
				FirstLine[0]='S';FirstLine[1]='U';FirstLine[2]='C'; FirstLine[3]='C'; FirstLine[4]=' '; FirstLine[5]=' '; FirstLine[6]=' '; FirstLine[7]=' '; 			 /*SUCC */
				FirstLine[8]=' ';FirstLine[9]=' ';FirstLine[10]=' ';FirstLine[11]=' ';FirstLine[12]=' ';FirstLine[13]=' ';FirstLine[14]=' ';FirstLine[15]=' ';
				SecondLine[0]=' ';SecondLine[1]=' ';SecondLine[2]=' '; SecondLine[3]=' '; SecondLine[4]=' '; SecondLine[5]=' '; SecondLine[6]=' '; SecondLine[7]=' ';  	 /*           */
				SecondLine[8]=' ';SecondLine[9]=' ';SecondLine[10]=' ';SecondLine[11]=' ';SecondLine[12]=' ';SecondLine[13]=' ';SecondLine[14]=' ';SecondLine[15]=' ';break;
			}
		case change:
			{
				FirstLine[0]='S';FirstLine[1]='e';FirstLine[2]='t'; FirstLine[3]=' '; FirstLine[4]='N'; FirstLine[5]='e'; FirstLine[6]='w'; FirstLine[7]=' ';  		  	 /* Set New  */
				FirstLine[8]='P';FirstLine[9]='a';FirstLine[10]='s';FirstLine[11]='s';FirstLine[12]='w';FirstLine[13]='o';FirstLine[14]='r';FirstLine[15]='d';			 /*Password  */
				SecondLine[0]='?';SecondLine[1]='?';SecondLine[2]=' '; SecondLine[3]=' '; SecondLine[4]=' '; SecondLine[5]=' '; SecondLine[6]=' '; SecondLine[7]=' ';  	 /*		?	 */
				SecondLine[8]=' ';SecondLine[9]=' ';SecondLine[10]=' ';SecondLine[11]=' ';SecondLine[12]=' ';SecondLine[13]=' ';SecondLine[14]=' ';SecondLine[15]=' ';break;
			}
		default:break;
	}
	
	write_string(0,FirstLine);
	write_string(16,SecondLine);
#endif	
}

/*****************************指令码/高电平为开*******************
****B7		B6		B5		B4		|B3		B2		B1		B0	  					****
****0			0			0			0			|1		D			C			B								****
****0			0			0			0			|1		开显示	开光标	光标闪烁		****
/*****************************指令码/高电平为开*******************
****B7		B6		B5		B4		|B3		B2		B1		B0	  					****
****0			0			0			0			|0		1			N			S								****
****0			0			0			0			|0		1			X			X								****
****N=1：读或写地址指针，光标加一
		N=0:（减一)
****S=1：写一个字符，整屏显示左移（N=1）,右移（N=0）
		S=0:当写一个字符，整屏显示不移动
******************************************************************/

/***********************指令表************************************
**0X01------------显示清屏：数据指针清零，所有显示清零
**0X02------------显示回车；数据指针清零
******************************************************************/