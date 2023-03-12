/*
SmartClock by Shang Wentao (2020217781)
*/

/**************************************************************************************
引脚说明
		  按键模块-->单片机IO
		  KEY1-->PB8
		  KEY2-->PB9
		  KEY3-->PB10
		  KEY4-->PB11
		  KEY5-->PB12 

		  OLED液晶-->单片机IO
		  VCC->5V
		  GND->GND
		  SCL->PB6
		  SDA->PB7

		  DS1302模块-->单片机IO
		  RST-->PB0
		  SCK-->PB1
		  IO-->PB2
		  
		  DS18B20模块-->单片机IO
		  DQ-->PB15																			  
***************************************************************************************/

#include "system.h"
#include "app_demo.h"


int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //中断优先级分组 分2组
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	
	sys_control();
	
	while(1)
	{
			
	}
}
