/*
SmartClock by Shang Wentao (2020217781)
*/

#ifndef _ds1302_H
#define _ds1302_H

#include "system.h"


extern u8 gDS1302_TIME[7];

//管脚定义
//时钟管脚
#define DS1302_CLK_PORT 		GPIOB  
#define DS1302_CLK_PIN 			(GPIO_Pin_1)
#define DS1302_CLK_PORT_RCC		RCC_APB2Periph_GPIOB

#define DS1302_IO_PORT 			GPIOB  
#define DS1302_IO_PIN 			(GPIO_Pin_2)
#define DS1302_IO_PORT_RCC		RCC_APB2Periph_GPIOB

#define DS1302_RST_PORT 		GPIOB  
#define DS1302_RST_PIN 			(GPIO_Pin_0)
#define DS1302_RST_PORT_RCC		RCC_APB2Periph_GPIOB

#define	DS1302_RST 		PBout(0) 
#define	DS1302_CLK 		PBout(1) 
#define	DS1302_IO_OUT 	PBout(2) 	
#define	DS1302_IO_IN  	PBin(2)  


//函数声明
void DS1302_Init(void);
void DS1302_GPIO_Init(void);
void DS1302_Read_Time(void);
void ds1302_run_sta(u8 sta);
#endif
