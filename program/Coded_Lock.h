#ifndef _CODED_LOCK_H_
#define	_CODED_LOCK_H_

extern PWS_Buf[7];
extern Txt_Buf[12];

void Gain_E2PROM_PWS();			//��ȡ����E2PROM���벢�����Ƿ�Ϊ����
char Send_E2PROM_PWS();			//���ܲ���������
void Password_Set();	 //�˺��������û��������룬�ͱȶ����õ�����
char Password_checked(); //�������

#endif