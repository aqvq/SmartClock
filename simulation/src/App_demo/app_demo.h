#ifndef _app_demo_H
#define _app_demo_H
/*
SmartClock by Shang Wentao (2020217781)
*/

#include "system.h"



//�ṹ�嶨��
typedef struct
{
	u8 set;//����
	u8 mode;//ģʽ
	float temp;//�¶�
	u8 time[3];//��ʱʱ�䣬���ʱ
	u8 djs[3];//����ʱʱ�䣬���ʱ
	u8 bitnum;//ѡ���޸�ʱ��λ��
	u8 msec[3];//���
}_sys_ctr;
extern _sys_ctr sys_ctr;

//��������
void sys_control(void);

#endif
