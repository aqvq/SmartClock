/*
SmartClock by Shang Wentao (2020217781)
*/

#ifndef _key_H
#define _key_H


#include "system.h"
 
#define KEY1_PIN   			GPIO_Pin_8    	
#define KEY2_PIN    		GPIO_Pin_9    	
#define KEY3_PIN    		GPIO_Pin_10    	
#define KEY4_PIN  			GPIO_Pin_11 
#define KEY5_PIN  			GPIO_Pin_12 

#define KEY_PORT 			GPIOB 			

//使用位操作定义
#define KEY1 	PBin(8)
#define KEY2 	PBin(9)
#define KEY3 	PBin(10)
#define KEY4 	PBin(11)
#define KEY5 	PBin(12)

//定义各个按键值  
#define KEY1_PRESS 		1
#define KEY2_PRESS		2
#define KEY3_PRESS		3
#define KEY4_PRESS		4
#define KEY5_PRESS		5
 
void KEY_Init(void);
u8 KEY_Scan(u8 mode);

#endif
