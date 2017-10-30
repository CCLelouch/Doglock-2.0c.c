#ifndef _CODED_LOCK_H_
#define	_CODED_LOCK_H_

extern PWS_Buf[7];
extern Txt_Buf[12];

void Gain_E2PROM_PWS();			//获取密码E2PROM密码并检验是否为密码
char Send_E2PROM_PWS();			//加密并发送密码
void Password_Set();	 //此函数用于用户输入密码，和比对设置的密码
char Password_checked(); //密码配对

#endif