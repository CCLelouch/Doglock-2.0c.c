/****************************************************************
/*文件名：I2C.C			//@11.0592MHz
/*功能：
/*说明：先传高位，SDA>4.7us,SCL低电平后0.125us后反应应答位，每次一字节结束第九个脉冲，SCL为0，后0.4375us，E2PROM立即释放总线。有10ms写周期
/*修改日期：2017-05-29
/*作者：c.c. 
/***************************************************************/
#include "i2c.h"

sbit SCL = P6^4;
sbit SDA = P6^5;

#if 0
void Delay(uint i)
{
 	while(i--);
}
#endif

void I2CStart()
{
 	SDA = 1;							//SCL拉高前确保SDA为高
//	Delay(5); //
	SCL = 1;
	Delay(5); 							//SDA高电平>4.7US
	SDA = 0; 
	Delay(5); 							//SDA低电平>4US
	SCL = 0;
//	Delay(5); //
}

void I2CStop()
{
	SDA = 0;							//SCL拉高前确保SDA为高
//	Delay(5);//
	SCL = 1;
	Delay(5);							//SCL高电平>4us
	SDA = 1;
	Delay(5);							//SDA高电平>4.7US
}

void I2CSend(uchar byte)
{
 	unsigned char i,j=50;
	for(i=0;i<8;i++)
	{
		byte<<=1;
	 	SDA = CY;
//		Delay(5);//
		SCL = 1;
		Delay(5);						//SCL高电平>4us
		SCL = 0;
//		Delay(5);//		 			   //等待从机反应应答作用
	}
	SDA = 1;						  //释放总线等待从机应答位
	Delay(5);//					 //为了明显看到应答位 //
	SCL = 1;
	while(SDA&&i--) j=50;			 //等待从机应答	，延时跳过
	Delay(2);					 //为了明显看到应答位，标准i2c通讯应答非应答scl保持>4us//
	SCL = 0;
	SDA = 0;					 //发送完释放占领总线，要读再来释放总线
	Delay(5);//					 //为了明显看到应答位 //

}

uchar I2CRead(bit ACK_Y_N) //应答0,非应答1
{
 	 uchar byte = 0,i=0;
	 for(i=0;i<8;i++)
	 {
		SDA = 1;	//释放总线准备读取
		Delay(1);		//时钟最大100-400khz//
		SCL = 1;
		byte<<=1;
		if(SDA) byte |= SDA;
		Delay(1);		//时钟最大100-400khz，标准i2c通讯应答非应答scl保持>4us //
		SCL = 0;
		Delay(1);		//时钟最大100-400khz//
	 }
	if(ACK_Y_N)
		SDA = 1;
	else 
		SDA = 0;		   
	SCL = 1;
	Delay(5);		//等待从机检测应答位标准i2c通讯应答非应答scl保持>4us//
	SCL = 0;
//	Delay(5);//
	
	return byte;
}

