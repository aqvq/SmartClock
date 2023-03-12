#ifndef _app_demo_H
#define _app_demo_H
/*
SmartClock by Shang Wentao (2020217781)
*/

#include "system.h"



//结构体定义
typedef struct
{
	u8 set;//设置
	u8 mode;//模式
	float temp;//温度
	u8 time[3];//定时时间，秒分时
	u8 djs[3];//倒计时时间，秒分时
	u8 bitnum;//选择修改时间位置
	u8 msec[3];//秒表
}_sys_ctr;
extern _sys_ctr sys_ctr;

//函数声明
void sys_control(void);

#endif
