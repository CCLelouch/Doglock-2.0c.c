/****************************************************************
/*文件名：EEPROM.C			//@11.0592MHz
/*功能：
/*说明：
/*修改日期：2017-05-30
/*作者：c.c. 
/***************************************************************/
#include "i2c.h"
#include "eeprom.h"

void write_eeprom(uchar addr,uchar databyte)
{
 	I2CStart();
	I2CSend(0xa0); 				//1010(默认) 000(设备地址)0(写),
	I2CSend(addr);				//数据要存储的地址
	I2CSend(databyte);			//要发送的八位数据

	I2CStop();
}

uchar read_eeprom(uchar addr)
{
	uchar databyte;

	I2CStart();
	I2CSend(0xa0); 				//1010(默认) 000(地址)0(写),
	I2CSend(addr);				//要读取数据所在的地址

	I2CStart();
	I2CSend(0xa1); 				//1010(默认) 000(地址)1读
	databyte = I2CRead(0);

	I2CStop();
		
	return databyte;
}
/***********使用方法如此函数下程序***********/
void I2C_init_ADDR(uchar devaddr)
{
	I2CStart();
	I2CSend(0xa0); 				//1010(默认) 000(地址)0(写),
	I2CSend(devaddr);			//存储器数据存储地址
} 

#if 0
	/****页写入帧历程****/
	I2C_init_ADDR(0x00);						  
	I2CSend(0x01);						  
	I2CSend(0x02);						  
	I2CSend(0x03);						  
	I2CSend(0x04);						  
	I2CStop();							  
	/********************/
	Delay(255);
	Delay(255);
	Delay(255);	至少//1ms以上
	/*****连续读帧*******/
	I2C_init_ADDR(0x00);						 
	I2CStart();							 
	I2CSend(0xa1);	//控制字地址000，读					 
	o=I2CRead(0);  //应答				 
	p=I2CRead(0);						 
	q=I2CRead(0);						 
	l=I2CRead(1);  //非应答				 
	I2CStop();							 
	/*********************/ 
#endif

