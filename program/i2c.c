/****************************************************************
/*�ļ�����I2C.C			//@11.0592MHz
/*���ܣ�
/*˵�����ȴ���λ��SDA>4.7us,SCL�͵�ƽ��0.125us��ӦӦ��λ��ÿ��һ�ֽڽ����ھŸ����壬SCLΪ0����0.4375us��E2PROM�����ͷ����ߡ���10msд����
/*�޸����ڣ�2017-05-29
/*���ߣ�c.c. 
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
 	SDA = 1;							//SCL����ǰȷ��SDAΪ��
//	Delay(5); //
	SCL = 1;
	Delay(5); 							//SDA�ߵ�ƽ>4.7US
	SDA = 0; 
	Delay(5); 							//SDA�͵�ƽ>4US
	SCL = 0;
//	Delay(5); //
}

void I2CStop()
{
	SDA = 0;							//SCL����ǰȷ��SDAΪ��
//	Delay(5);//
	SCL = 1;
	Delay(5);							//SCL�ߵ�ƽ>4us
	SDA = 1;
	Delay(5);							//SDA�ߵ�ƽ>4.7US
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
		Delay(5);						//SCL�ߵ�ƽ>4us
		SCL = 0;
//		Delay(5);//		 			   //�ȴ��ӻ���ӦӦ������
	}
	SDA = 1;						  //�ͷ����ߵȴ��ӻ�Ӧ��λ
	Delay(5);//					 //Ϊ�����Կ���Ӧ��λ //
	SCL = 1;
	while(SDA&&i--) j=50;			 //�ȴ��ӻ�Ӧ��	����ʱ����
	Delay(2);					 //Ϊ�����Կ���Ӧ��λ����׼i2cͨѶӦ���Ӧ��scl����>4us//
	SCL = 0;
	SDA = 0;					 //�������ͷ�ռ�����ߣ�Ҫ�������ͷ�����
	Delay(5);//					 //Ϊ�����Կ���Ӧ��λ //

}

uchar I2CRead(bit ACK_Y_N) //Ӧ��0,��Ӧ��1
{
 	 uchar byte = 0,i=0;
	 for(i=0;i<8;i++)
	 {
		SDA = 1;	//�ͷ�����׼����ȡ
		Delay(1);		//ʱ�����100-400khz//
		SCL = 1;
		byte<<=1;
		if(SDA) byte |= SDA;
		Delay(1);		//ʱ�����100-400khz����׼i2cͨѶӦ���Ӧ��scl����>4us //
		SCL = 0;
		Delay(1);		//ʱ�����100-400khz//
	 }
	if(ACK_Y_N)
		SDA = 1;
	else 
		SDA = 0;		   
	SCL = 1;
	Delay(5);		//�ȴ��ӻ����Ӧ��λ��׼i2cͨѶӦ���Ӧ��scl����>4us//
	SCL = 0;
//	Delay(5);//
	
	return byte;
}

