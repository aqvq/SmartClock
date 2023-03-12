/*
SmartClock by Shang Wentao (2020217781)
*/

/**************************************************************************************
����˵��
		  ����ģ��-->��Ƭ��IO
		  KEY1-->PB8
		  KEY2-->PB9
		  KEY3-->PB10
		  KEY4-->PB11
		  KEY5-->PB12 

		  OLEDҺ��-->��Ƭ��IO
		  VCC->5V
		  GND->GND
		  SCL->PB6
		  SDA->PB7

		  DS1302ģ��-->��Ƭ��IO
		  RST-->PB0
		  SCK-->PB1
		  IO-->PB2
		  
		  DS18B20ģ��-->��Ƭ��IO
		  DQ-->PB15																			  
***************************************************************************************/

#include "system.h"
#include "app_demo.h"


int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
	
	sys_control();
	
	while(1)
	{
			
	}
}
