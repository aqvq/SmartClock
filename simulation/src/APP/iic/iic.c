#include "iic.h"
#include "SysTick.h"

/*******************************************************************************
* 函 数 名         : IIC_Init
* 函数功能		   : IIC初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void iic_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(IIC_SCL_PORT_RCC|IIC_SDA_PORT_RCC,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=IIC_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(IIC_SCL_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
	
	IIC_SCL=1;
	IIC_SDA=1;	
}

/*******************************************************************************
* 函 数 名         : SDA_OUT
* 函数功能		   : SDA输出配置	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void SDA_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名         : SDA_IN
* 函数功能		   : SDA输入配置	   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void SDA_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Init(IIC_SDA_PORT,&GPIO_InitStructure);
}

/*******************************************************************************
* 函 数 名       : iic_start
* 函数功能		 : 产生IIC起始信号
* 输    入       : 无
* 输    出    	 : 无
*******************************************************************************/
void iic_start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_10us(1);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_10us(1);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
}

/*******************************************************************************
* 函 数 名         : iic_stop
* 函数功能		   : 产生IIC停止信号   
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void iic_stop(void)
{	
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	IIC_SCL=1; 
	delay_10us(1); 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_10us(1);		
}

/*******************************************************************************
* 函 数 名         : iic_ack
* 函数功能		   : 产生ACK应答  
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void iic_ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_10us(1);
	IIC_SCL=1;
	delay_10us(1);
	IIC_SCL=0;
}

/*******************************************************************************
* 函 数 名         : iic_nack
* 函数功能		   : 产生NACK非应答  
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void iic_nack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_10us(1);
	IIC_SCL=1;
	delay_10us(1);
	IIC_SCL=0;
}


/*******************************************************************************
* 函 数 名         : iic_wait_ack
* 函数功能		   : 等待应答信号到来   
* 输    入         : 无
* 输    出         : 1，接收应答失败
        			 0，接收应答成功
*******************************************************************************/
u8 iic_wait_ack(void)
{
	u8 tempTime=0;
	
	IIC_SDA=1;
	delay_10us(1);
	SDA_IN();      //SDA设置为输入  	   
	IIC_SCL=1;
	delay_10us(1);	 
	while(READ_SDA)
	{
		tempTime++;
		if(tempTime>250)
		{
			iic_stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;	
}

/*******************************************************************************
* 函 数 名         : iic_write_byte
* 函数功能		   : IIC发送一个字节 
* 输    入         : dat：发送一个字节
* 输    出         : 无
*******************************************************************************/
void iic_write_byte(u8 dat)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        if((dat&0x80)>0) //0x80  1000 0000
			IIC_SDA=1;
		else
			IIC_SDA=0;
        dat<<=1; 	  
		delay_10us(1);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_10us(1); 
		IIC_SCL=0;	
		delay_10us(1);
    }	 
}

/*******************************************************************************
* 函 数 名         : iic_read_byte
* 函数功能		   : IIC读一个字节 
* 输    入         : ack=1时，发送ACK，ack=0，发送nACK 
* 输    出         : 应答或非应答
*******************************************************************************/
u8 iic_read_byte(u8 ack)
{
	u8 i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_10us(1);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_10us(1); 
    }					 
    if (!ack)
        iic_nack();//发送nACK
    else
        iic_ack(); //发送ACK   
    return receive;
}

