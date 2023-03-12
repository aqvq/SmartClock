/*
SmartClock by Shang Wentao (2020217781)
*/

#include "ds1302.h"
#include "SysTick.h"

//---DS1302д��Ͷ�ȡʱ����ĵ�ַ����---//
//---���ʱ�������� ���λ��дλ;-------//
u8 gREAD_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
u8 gWRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

//---DS1302ʱ�ӳ�ʼ��2022��5��6��������13��51��47�롣---//
//---�洢˳�������ʱ��������,�洢��ʽ����BCD��---//
u8 gDS1302_TIME[7] = {0x47, 0x51, 0x13, 0x06, 0x05, 0x05, 0x22};

void DS1302_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(DS1302_CLK_PORT_RCC|DS1302_IO_PORT_RCC|DS1302_RST_PORT_RCC,ENABLE);

	GPIO_InitStructure.GPIO_Pin=DS1302_CLK_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(DS1302_CLK_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=DS1302_IO_PIN;
	GPIO_Init(DS1302_IO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=DS1302_RST_PIN;
	GPIO_Init(DS1302_RST_PORT,&GPIO_InitStructure);
}

void DS1302_IO_Out_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=DS1302_IO_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(DS1302_CLK_PORT,&GPIO_InitStructure);
}

void DS1302_IO_In_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin=DS1302_IO_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(DS1302_CLK_PORT,&GPIO_InitStructure);
}

void DS1302_Write_Byte(u8 addr,u8 dat)
{
	u8 i=0;
	
	DS1302_RST=0;
	delay_us(1);	
	DS1302_CLK=0;//CLK�͵�ƽ
	delay_us(1);
	DS1302_RST=1;//RST�ɵ͵��߱仯
	delay_us(1);
	DS1302_IO_Out_Init();
	for(i=0;i<8;i++)//ѭ��8�Σ�ÿ��д1λ����д��λ��д��λ
	{
		DS1302_IO_OUT=addr&0x01;
		addr>>=1;
		DS1302_CLK=1;
		delay_us(1);
		DS1302_CLK=0;//CLK�ɵ͵��߲���һ�������أ��Ӷ�д������
		delay_us(1);		
	}
	for(i=0;i<8;i++)//ѭ��8�Σ�ÿ��д1λ����д��λ��д��λ
	{
		DS1302_IO_OUT=dat&0x01;
		dat>>=1;
		DS1302_CLK=1;
		delay_us(1);
		DS1302_CLK=0;
		delay_us(1);		
	}
	DS1302_RST=0;//RST����
	delay_us(1);
}


u8 DS1302_Read_Byte(u8 addr)
{
	u8 i=0;
	u8 temp=0;
	u8 value=0;

	DS1302_RST=0;
	delay_us(1);	
	DS1302_CLK=0;//CLK�͵�ƽ
	delay_us(1);
	DS1302_RST=1;//RST�ɵ͵��߱仯
	delay_us(1);
	DS1302_IO_Out_Init();
	for(i=0;i<8;i++)//ѭ��8�Σ�ÿ��д1λ����д��λ��д��λ
	{
		DS1302_IO_OUT=addr&0x01;
		addr>>=1;	
		DS1302_CLK=1;
		delay_us(1);
		DS1302_CLK=0;//CLK�ɵ͵��߲���һ�������أ��Ӷ�д������
		delay_us(1);		
	}
	DS1302_IO_In_Init();
	for(i=0;i<8;i++)//ѭ��8�Σ�ÿ�ζ�1λ���ȶ���λ�ٶ���λ
	{
		temp=DS1302_IO_IN;
		value=(temp<<7)|(value>>1);//�Ƚ�value����1λ��Ȼ��temp����7λ����������
		DS1302_CLK=1;
		delay_us(1);
		DS1302_CLK=0;
		delay_us(1);		
	}
	DS1302_RST=0;//RST����
	delay_us(1);		
	return value;		
}

void DS1302_Init(void)
{
	u8 i=0;
	
	DS1302_Write_Byte(0x8E,0X00);
	for(i=0;i<7;i++)
	{
		DS1302_Write_Byte(gWRITE_RTC_ADDR[i],gDS1302_TIME[i]);	
	}
	DS1302_Write_Byte(0x8E,0X80);	
}

void DS1302_Read_Time(void)
{
	u8 i=0;
	for(i=0;i<7;i++)
	{
		gDS1302_TIME[i]=DS1302_Read_Byte(gREAD_RTC_ADDR[i]);	
	}	
}

//ds1302ʱ�ӿ�ʼ
void ds1302_run_sta(u8 sta)
{
	if(sta==0)
		DS1302_Write_Byte(gWRITE_RTC_ADDR[0],gDS1302_TIME[0]|0x80);	
	else
		DS1302_Write_Byte(gWRITE_RTC_ADDR[0],gDS1302_TIME[0]&0x7f);		
}
