/****************************************************************
/*文件名：coded_lock.C			//@11.0592MHz
/*功能：  实现简单密码锁
/*说明：  看配套流程图
/*修改日期：2017-07-20
/*版本：旺财1.0
/*作者：c.c. 
/***************************************************************/

#include "coded_lock.h"
#include"LCD1602.H"
#include"Delay.H"
#include "eeprom.h"	
#include "i2c.h"
#include"key.h"


PWS_Buf[7]={0,0,0,0,0,0,0};			   /*修改后的密码值，或记忆模块传进来的密码值*/
Txt_Buf[12]={0,0,0,0,0,0,0,0,0,0,0,0}; /*用户输入键值*/
uchar alarm=0;

/****************************************************************
**函数：void Gain_E2PROM_PWS()			
**功能：获取记忆模块内密码数据
**说明： 
**修改日期：2017-07-20
**作者：c.c. 
****************************************************************/
void Gain_E2PROM_PWS()
{
	I2C_init_ADDR(0x02);						 
	I2CStart();							 
	I2CSend(0xa1);	//控制字地址000，读					 
	PWS_Buf[0]=I2CRead(0);  //应答			/*我不知道为什么读的值都为两倍*/	 
	PWS_Buf[1]=I2CRead(0);  //应答					 
	PWS_Buf[2]=I2CRead(0);  //应答
	PWS_Buf[3]=I2CRead(0);  //应答
	PWS_Buf[4]=I2CRead(0);  //应答
	PWS_Buf[5]=I2CRead(0);  //应答				 
	PWS_Buf[6]=I2CRead(1);  //非应答				 
	I2CStop();
	if(  PWS_Buf[6]==PWS_Buf[0]*8+6 )
		LCD1602_State=inppsw;			 //有密码
	else
		LCD1602_State=newpsw;			 //无密码
}
/****************************************************************
**函数：char Send_E2PROM_PWS()			
**功能：发送密码到记忆模块
**说明： 
**修改日期：2017-07-20
**作者：c.c. 
****************************************************************/
char Send_E2PROM_PWS()
{													 
	PWS_Buf[0]=Txt_Buf[0];  				 
	PWS_Buf[1]=Txt_Buf[1];  
	PWS_Buf[2]=Txt_Buf[2]; 
	PWS_Buf[3]=Txt_Buf[3];  
	PWS_Buf[4]=Txt_Buf[4];  			 
	PWS_Buf[5]=Txt_Buf[5];
	PWS_Buf[6]=Txt_Buf[0]*8+6; //对密码末位进行加密，计算可用于判别是否为密码	  

 	I2C_init_ADDR(0x02);						  
	I2CSend(PWS_Buf[0]);						  
	I2CSend(PWS_Buf[1]);	//第一位密码					  
	I2CSend(PWS_Buf[2]);						  
	I2CSend(PWS_Buf[3]);
	I2CSend(PWS_Buf[4]);
	I2CSend(PWS_Buf[5]);
	I2CSend(PWS_Buf[6]);						  
	I2CStop();
	Delay(10);
	return 1;				//修改成功
}
/****************************************************************
**函数：void Password_Set()			
**功能：密码重置
**说明： 
**修改日期：2017-07-20
**作者：c.c. 
****************************************************************/
void Password_Set()	 //此函数用于用户输入密码，和比对设置的密码
{
	if(LCD1602_State==newpsw)				//用户输入第一次密码
	{
		while(!KEY16_Flag());			//等待按键按下
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
	else if(LCD1602_State==setagain)			//用户请再输入密码
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
		   Txt_Buf[3]==Txt_Buf[9]&&Txt_Buf[4]==Txt_Buf[10]&&Txt_Buf[5]==Txt_Buf[11])		  		 		//比对两次密码
		{
			LCD1602_State=ok;			  //两次密码一样，ok
			Send_E2PROM_PWS();			  //发送密码到
			P55=0;Delay_ms(80);P55=1;Delay_ms(80); P55=0;Delay_ms(80);P55=1;
		}
		else
			LCD1602_State=newpsw;			 //两次密码不一样，请输入密码
	}
}
/****************************************************************
**函数：char Password_checked()			
**功能：配对密码
**说明： 
**修改日期：2017-07-20
**作者：c.c. 
****************************************************************/
char Password_checked()
{
	while(!KEY16_Flag());			//等待按键按下
 	Txt_Buf[0]=KEY16_scan();		//获取键值
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
	   Txt_Buf[5]==PWS_Buf[5])		  		 		//配对密码
	{
		LCD1602_State=succ;			  //密码一样，ok
		alarm=0;					  //有一次正确就清空累计错误次数
	}
	else 
	{
 		LCD1602_State=inppsw;			 //密码错误，请输入密码
		alarm++;					 //累计错误次数
	}
	if(alarm>=5)
	{
		LCD1602_State=error;
		alarm=0;
	}
	P4=alarm;
	return 0;
}
















