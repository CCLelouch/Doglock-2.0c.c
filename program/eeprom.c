/****************************************************************
/*�ļ�����EEPROM.C			//@11.0592MHz
/*���ܣ�
/*˵����
/*�޸����ڣ�2017-05-30
/*���ߣ�c.c. 
/***************************************************************/
#include "i2c.h"
#include "eeprom.h"

void write_eeprom(uchar addr,uchar databyte)
{
 	I2CStart();
	I2CSend(0xa0); 				//1010(Ĭ��) 000(�豸��ַ)0(д),
	I2CSend(addr);				//����Ҫ�洢�ĵ�ַ
	I2CSend(databyte);			//Ҫ���͵İ�λ����

	I2CStop();
}

uchar read_eeprom(uchar addr)
{
	uchar databyte;

	I2CStart();
	I2CSend(0xa0); 				//1010(Ĭ��) 000(��ַ)0(д),
	I2CSend(addr);				//Ҫ��ȡ�������ڵĵ�ַ

	I2CStart();
	I2CSend(0xa1); 				//1010(Ĭ��) 000(��ַ)1��
	databyte = I2CRead(0);

	I2CStop();
		
	return databyte;
}
/***********ʹ�÷�����˺����³���***********/
void I2C_init_ADDR(uchar devaddr)
{
	I2CStart();
	I2CSend(0xa0); 				//1010(Ĭ��) 000(��ַ)0(д),
	I2CSend(devaddr);			//�洢�����ݴ洢��ַ
} 

#if 0
	/****ҳд��֡����****/
	I2C_init_ADDR(0x00);						  
	I2CSend(0x01);						  
	I2CSend(0x02);						  
	I2CSend(0x03);						  
	I2CSend(0x04);						  
	I2CStop();							  
	/********************/
	Delay(255);
	Delay(255);
	Delay(255);	����//1ms����
	/*****������֡*******/
	I2C_init_ADDR(0x00);						 
	I2CStart();							 
	I2CSend(0xa1);	//�����ֵ�ַ000����					 
	o=I2CRead(0);  //Ӧ��				 
	p=I2CRead(0);						 
	q=I2CRead(0);						 
	l=I2CRead(1);  //��Ӧ��				 
	I2CStop();							 
	/*********************/ 
#endif

