/*
SmartClock by Shang Wentao (2020217781)
*/

#ifndef _led_H
#define _led_H

#include "system.h"

/*  LEDʱ�Ӷ˿ڡ����Ŷ��� */
#define LED_PORT 			GPIOB   
#define LED_PIN 			GPIO_Pin_15
#define LED_PORT_RCC		RCC_APB2Periph_GPIOB

#define LED1 PBout(15)  	


void LED_Init(void);


#endif
