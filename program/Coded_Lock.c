/****************************************************************
/*�ļ�����coded_lock.C			//@11.0592MHz
/*���ܣ�  ʵ�ּ�������
/*˵����  ����������ͼ
/*�޸����ڣ�2017-07-20
/*�汾������1.0
/*���ߣ�c.c. 
/***************************************************************/

#include "coded_lock.h"
#include"LCD1602.H"
#include"Delay.H"
#include "eeprom.h"	
#include "i2c.h"
#include"key.h"


PWS_Buf[7]={0,0,0,0,0,0,0};			   /*�޸ĺ������ֵ�������ģ�鴫����������ֵ*/
Txt_Buf[12]={0,0,0,0,0,0,0,0,0,0,0,0}; /*�û������ֵ*/
uchar alarm=0;

/****************************************************************
**������void Gain_E2PROM_PWS()			
**���ܣ���ȡ����ģ������������
**˵���� 
**�޸����ڣ�2017-07-20
**���ߣ�c.c. 
****************************************************************/
void Gain_E2PROM_PWS()
{
	I2C_init_ADDR(0x02);						 
	I2CStart();							 
	I2CSend(0xa1);	//�����ֵ�ַ000����					 
	PWS_Buf[0]=I2CRead(0);  //Ӧ��			/*�Ҳ�֪��Ϊʲô����ֵ��Ϊ����*/	 
	PWS_Buf[1]=I2CRead(0);  //Ӧ��					 
	PWS_Buf[2]=I2CRead(0);  //Ӧ��
	PWS_Buf[3]=I2CRead(0);  //Ӧ��
	PWS_Buf[4]=I2CRead(0);  //Ӧ��
	PWS_Buf[5]=I2CRead(0);  //Ӧ��				 
	PWS_Buf[6]=I2CRead(1);  //��Ӧ��				 
	I2CStop();
	if(  PWS_Buf[6]==PWS_Buf[0]*8+6 )
		LCD1602_State=inppsw;			 //������
	else
		LCD1602_State=newpsw;			 //������
}
/****************************************************************
**������char Send_E2PROM_PWS()			
**���ܣ��������뵽����ģ��
**˵���� 
**�޸����ڣ�2017-07-20
**���ߣ�c.c. 
****************************************************************/
char Send_E2PROM_PWS()
{													 
	PWS_Buf[0]=Txt_Buf[0];  				 
	PWS_Buf[1]=Txt_Buf[1];  
	PWS_Buf[2]=Txt_Buf[2]; 
	PWS_Buf[3]=Txt_Buf[3];  
	PWS_Buf[4]=Txt_Buf[4];  			 
	PWS_Buf[5]=Txt_Buf[5];
	PWS_Buf[6]=Txt_Buf[0]*8+6; //������ĩλ���м��ܣ�����������б��Ƿ�Ϊ����	  

 	I2C_init_ADDR(0x02);						  
	I2CSend(PWS_Buf[0]);						  
	I2CSend(PWS_Buf[1]);	//��һλ����					  
	I2CSend(PWS_Buf[2]);						  
	I2CSend(PWS_Buf[3]);
	I2CSend(PWS_Buf[4]);
	I2CSend(PWS_Buf[5]);
	I2CSend(PWS_Buf[6]);						  
	I2CStop();
	Delay(10);
	return 1;				//�޸ĳɹ�
}
/****************************************************************
**������void Password_Set()			
**���ܣ���������
**˵���� 
**�޸����ڣ�2017-07-20
**���ߣ�c.c. 
****************************************************************/
void Password_Set()	 //�˺��������û��������룬�ͱȶ����õ�����
{
	if(LCD1602_State==newpsw)				//�û������һ������
	{
		while(!KEY16_Flag());			//�ȴ���������
	 	Txt_Buf[0]=KEY16_scan();
		while(!KEY16_Flag());
		Txt_Buf[1]=KEY16_scan();
		while(!KEY16_Flag());
		Txt_Buf[2]=KEY16_scan();
		while(!KEY16_Flag());
		Txt_Buf[3]=KEY16_scan();
		while(!KEY16_Flag());
		Txt_Buf[4]=KEY16_scan();
		while(!KEY16_Flag());
		Txt_Buf[5]=KEY16_scan();
		LCD1602_State=setagain;
	}
	else if(LCD1602_State==setagain)			//�û�������������
	{
		while(!KEY16_Flag());
		Txt_Buf[6]=KEY16_scan();
		while(!KEY16_Flag());
	 	Txt_Buf[7]=KEY16_scan();
		while(!KEY16_Flag());
		Txt_Buf[8]=KEY16_scan();
		while(!KEY16_Flag());
		Txt_Buf[9]=KEY16_scan();
		while(!KEY16_Flag());
		Txt_Buf[10]=KEY16_scan();
		while(!KEY16_Flag());
		Txt_Buf[11]=KEY16_scan();
		if(Txt_Buf[0]==Txt_Buf[6]&&Txt_Buf[1]==Txt_Buf[7]&&Txt_Buf[2]==Txt_Buf[8]&&
		   Txt_Buf[3]==Txt_Buf[9]&&Txt_Buf[4]==Txt_Buf[10]&&Txt_Buf[5]==Txt_Buf[11])		  		 		//�ȶ���������
		{
			LCD1602_State=ok;			  //��������һ����ok
			Send_E2PROM_PWS();			  //�������뵽
			P55=0;Delay_ms(80);P55=1;Delay_ms(80); P55=0;Delay_ms(80);P55=1;
		}
		else
			LCD1602_State=newpsw;			 //�������벻һ��������������
	}
}
/****************************************************************
**������char Password_checked()			
**���ܣ��������
**˵���� 
**�޸����ڣ�2017-07-20
**���ߣ�c.c. 
****************************************************************/
char Password_checked()
{
	while(!KEY16_Flag());			//�ȴ���������
 	Txt_Buf[0]=KEY16_scan();		//��ȡ��ֵ
	while(!KEY16_Flag());
	Txt_Buf[1]=KEY16_scan();
	while(!KEY16_Flag());
	Txt_Buf[2]=KEY16_scan();
	while(!KEY16_Flag());
	Txt_Buf[3]=KEY16_scan();
	while(!KEY16_Flag());
	Txt_Buf[4]=KEY16_scan();
	while(!KEY16_Flag());
	Txt_Buf[5]=KEY16_scan();

	if(Txt_Buf[0]==PWS_Buf[0]&&
	   Txt_Buf[1]==PWS_Buf[1]&&
	   Txt_Buf[2]==PWS_Buf[2]&&
	   Txt_Buf[3]==PWS_Buf[3]&&
	   Txt_Buf[4]==PWS_Buf[4]&&
	   Txt_Buf[5]==PWS_Buf[5])		  		 		//�������
	{
		LCD1602_State=succ;			  //����һ����ok
		alarm=0;					  //��һ����ȷ������ۼƴ������
	}
	else 
	{
 		LCD1602_State=inppsw;			 //�����������������
		alarm++;					 //�ۼƴ������
	}
	if(alarm>=5)
	{
		LCD1602_State=error;
		alarm=0;
	}
	P4=alarm;
	return 0;
}
















