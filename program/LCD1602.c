/***********************************************************************
** �ļ�����LCD1602.c
** ���ܣ�  LCD1602��������
** ˵����C:LCD1602�������������
** ����ָ�
** �޸����ڣ�2017-07-19
** �汾��V2.1
** �޸ģ��޸��˳����������������ֳ���255�޷�������ʾ����
		 �����˸����͵�������ת����ʾ,�����˶˿�����
** ���ߣ�c.c. 
************************************************************************/

#include"head.h"
#include"Delay.H"
#include"LCD1602.H"

extern bit flag;			  //�������ʾλ
extern float C;				  //����Ҫ��ʾ�ĸ�������ֵ

State LCD1602_State;//��ͬ״̬��־λ

unsigned char  FirstLine[16]="                ",SecondLine[16] ="                ";
//����ռ䣬��ʾΪ�գ����ַ��ٺ��һ��'\0'	������������������ֵ���е�һ���ֽڿռ�

/****************************************************************
**������void wait()			
**���ܣ�æ���
**˵���� BUSY(D7)Ϊæ����
**�޸����ڣ�2017-07-07
**���ߣ�c.c. 
****************************************************************/
void wait()					
{
	DAT_PIN = 0XFF;					//���пڳ�ʼ��
	do
	{
		EN = 0;
		RS = 0;
		RW = 1;						  //���˿�
		EN = 1;						  //�����巢��
	}
	while(BUSY);				 
	EN = 0;
}

void write_cmd(unsigned char cmd)  //дָ��
{
	 wait();					  //ÿ��д��ָ������ǰ��Ҫ���æ�ź�
 	 EN = 0;
	 RS = 0;
	 RW = 0;
	 DAT_PIN = cmd;
	 EN = 1;
	 EN = 0; 
}


void write_dat(unsigned char dat)  //д����
{
 	wait();
 	EN = 0;
	RS = 1;
	RW = 0;
	DAT_PIN = dat;
	EN = 1;
	EN = 0;
}

void write_char(unsigned char addr,unsigned char cha)	//��ַ(0-27H,40H-67H)������
{
	if(addr<0x10)
		write_cmd(0x80+addr);		  //��һ�� ��ַ(0X80+0-27H,40H-67H����һ�е�һ��Ϊ0x80���ڶ��е�һ��Ϊ0xc0)
	else							 
		write_cmd(0xc0+addr);		  //�ڶ���
	write_cmd(0x80+addr);
	write_dat(cha);
}

void write_string(unsigned char addr,unsigned char *p)	//��ַ(0-27H,40H-67H)������
{
	if(addr<0x10)
		write_cmd(0x80+addr);		  //��һ��
	else							 
		write_cmd(0xb0+addr);		  //�ڶ���
 	while(*p!='\0')			 		  //����ַ��������'\0'
	{
	 	write_dat(*p++);	  
	}
}
/****************************************************************
**������Init_LCD1602()			
**���ܣ���ʼ��
**˵���� 
**�޸����ڣ�2017-07-08
**���ߣ�c.c. 
****************************************************************/
void Init_LCD1602()
{
	Delay_ms(15);
	write_cmd(0x38);	//��ʾģʽ����
	write_cmd(0x38);	//��ʾģʽ����
	write_cmd(0x38);	//��ʾģʽ����
	write_cmd(0x06);	//����ʾ�޹��
	write_cmd(0x0c);	//��ʾ����������ָ�����㣬������ʾ����
	write_cmd(0x01);	//��ʾ����ƶ�����
}
/****************************************************************
**������Init_LCD1602()			
**���ܣ���ʼ��
**˵���� 
**�޸����ڣ�2017-07-08
**���ߣ�c.c. 
****************************************************************/
void Display_LCD1602()
{
	//	uchar i,j,k,l,m;			  //������ʾ����������
	#if 0/**********��������*************/
		FirstLine[0]='D';~	FirstLine[16]=' ';
	/*******************************/
	#elif 0/*************������������ʾ�㷨***************/		
			/******��������******/
			i=(unsigned int)C/100;
			j=(unsigned int)C%100/10;
			k=(unsigned int)C%10;
			/******С������******/	
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

/*****************************ָ����/�ߵ�ƽΪ��*******************
****B7		B6		B5		B4		|B3		B2		B1		B0	  					****
****0			0			0			0			|1		D			C			B								****
****0			0			0			0			|1		����ʾ	�����	�����˸		****
/*****************************ָ����/�ߵ�ƽΪ��*******************
****B7		B6		B5		B4		|B3		B2		B1		B0	  					****
****0			0			0			0			|0		1			N			S								****
****0			0			0			0			|0		1			X			X								****
****N=1������д��ַָ�룬����һ
		N=0:����һ)
****S=1��дһ���ַ���������ʾ���ƣ�N=1��,���ƣ�N=0��
		S=0:��дһ���ַ���������ʾ���ƶ�
******************************************************************/

/***********************ָ���************************************
**0X01------------��ʾ����������ָ�����㣬������ʾ����
**0X02------------��ʾ�س�������ָ������
******************************************************************/