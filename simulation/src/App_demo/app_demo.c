/*
SmartClock by Shang Wentao (2020217781)
*/

#include "app_demo.h"
#include "time.h"
#include "led.h"
#include "SysTick.h"
#include "oled.h"
#include "beep.h"
#include "ds18b20.h"
#include "ds1302.h"
#include "key.h"


void countdown_mode_show(void);
//定义
_sys_ctr sys_ctr;


//系统参数初始化
void sys_parm_init(void)
{
	sys_ctr.mode=0;
	sys_ctr.set=0;
	sys_ctr.bitnum=0;
}


//系统开机界面显示
void sys_open_show(void)
{
	OLED_Clear();
	OLED_ShowString(0,0,"Tempe         C",12);
	OLED_ShowString(0,2,"Date:2022-05-06",12);
	OLED_ShowString(0,4,"Time:12:43:56",12);
	OLED_ShowString(0,6,"Day:7",12);
}

//系统数据采集
void sys_data_get(void)
{
	static u16 i=0;
	
	while(sys_ctr.mode==0)
	{	
		i++;
		if(i%1==0)
		{
			//读取温度
			sys_ctr.temp=DS18B20_GetTemperture();
		}
		//读取时间
		DS1302_Read_Time();
		
		break;
	}
}

//系统数据显示
void sys_data_show(void)
{
	u8 buf[9];
	u16 temp=0;
	
	//正常模式显示
	while(sys_ctr.mode==0)
	{
		//温度数据显示
		if(sys_ctr.temp<0)
		{
			sys_ctr.temp=-sys_ctr.temp;
			buf[0]='-';
		}
		else
		{
			buf[0]=' ';
		}
		temp=sys_ctr.temp*10;
		buf[1]=temp/1000+0x30;
		if(buf[1]==0x30)buf[1]=' ';
		buf[2]=temp%1000/100+0x30;
		if(buf[2]==0x30 && buf[1]==0x30) 
		{
			buf[1]=' ';buf[2]=' ';	
		}
		buf[3]=temp%1000%100/10+0x30;
		buf[4]='.';
		buf[5]=temp%1000%100%10+0x30;
		buf[6]='\0';
		OLED_ShowString(5*8,0,buf,12);
		
		//正常时间显示
		while(sys_ctr.set==0)
		{
			//日期数据显示
			buf[0]=gDS1302_TIME[6]/16+0x30;
			buf[1]=gDS1302_TIME[6]%16+0x30;
			buf[2]='-';
			buf[3]=gDS1302_TIME[4]/16+0x30;
			buf[4]=gDS1302_TIME[4]%16+0x30;
			buf[5]='-';
			buf[6]=gDS1302_TIME[3]/16+0x30;
			buf[7]=gDS1302_TIME[3]%16+0x30;
			buf[8]='\0';
			OLED_ShowString(7*8,2,buf,12);
			//时间数据显示
			buf[0]=gDS1302_TIME[2]/16+0x30;
			buf[1]=gDS1302_TIME[2]%16+0x30;
			buf[2]=':';
			buf[3]=gDS1302_TIME[1]/16+0x30;
			buf[4]=gDS1302_TIME[1]%16+0x30;
			buf[5]=':';
			buf[6]=gDS1302_TIME[0]/16+0x30;
			buf[7]=gDS1302_TIME[0]%16+0x30;
			buf[8]='\0';
			OLED_ShowString(5*8,4,buf,12);
			//星期显示
			OLED_ShowNum(4*8,6,gDS1302_TIME[5],1,12);
			break;
		}
		//修改时间显示
		while(sys_ctr.set==1)
		{
			//日期数据显示
			buf[0]=gDS1302_TIME[6]/16+0x30;
			buf[1]=gDS1302_TIME[6]%16+0x30;
			buf[2]='-';
			buf[3]=gDS1302_TIME[4]/16+0x30;
			buf[4]=gDS1302_TIME[4]%16+0x30;
			buf[5]='-';
			buf[6]=gDS1302_TIME[3]/16+0x30;
			buf[7]=gDS1302_TIME[3]%16+0x30;
			buf[8]='\0';
			OLED_ShowString(7*8,2,buf,12);
			//时间数据显示
			buf[0]=gDS1302_TIME[2]/16+0x30;
			buf[1]=gDS1302_TIME[2]%16+0x30;
			buf[2]=':';
			buf[3]=gDS1302_TIME[1]/16+0x30;
			buf[4]=gDS1302_TIME[1]%16+0x30;
			buf[5]=':';
			buf[6]=gDS1302_TIME[0]/16+0x30;
			buf[7]=gDS1302_TIME[0]%16+0x30;
			buf[8]='\0';
			OLED_ShowString(5*8,4,buf,12);
			//星期显示
			OLED_ShowNum(4*8,6,gDS1302_TIME[5],1,12);
			delay_ms(200);
			switch(sys_ctr.bitnum)
			{
				case 0: OLED_ShowChar(12*8,4,' ',12);break;//秒
				case 1: OLED_ShowChar(9*8,4,' ',12);break;//分
				case 2: OLED_ShowChar(6*8,4,' ',12);break;//时
				case 3: OLED_ShowChar(14*8,2,' ',12);break;//日
				case 4: OLED_ShowChar(11*8,2,' ',12);break;//月
				case 5: OLED_ShowChar(4*8,6,' ',12);break;//周
				case 6: OLED_ShowChar(8*8,2,' ',12);break;//年
			}
			
			break;
		}
		
		break;
	}
	//闹钟模式显示
	while(sys_ctr.mode==1)
	{
		//时间数据显示
		buf[0]=sys_ctr.time[2]/16+0x30;
		buf[1]=sys_ctr.time[2]%16+0x30;
		buf[2]=':';
		buf[3]=sys_ctr.time[1]/16+0x30;
		buf[4]=sys_ctr.time[1]%16+0x30;
		buf[5]=':';
		buf[6]=sys_ctr.time[0]/16+0x30;
		buf[7]=sys_ctr.time[0]%16+0x30;
		buf[8]='\0';
		OLED_ShowString(5*8,2,buf,12);
		
		delay_ms(200);
		switch(sys_ctr.bitnum)
		{
			case 0: OLED_ShowChar(12*8,2,' ',12);break;//秒
			case 1: OLED_ShowChar(9*8,2,' ',12);break;//分
			case 2: OLED_ShowChar(6*8,2,' ',12);break;//时
		}
		break;
	}
	//倒计时模式
	while(sys_ctr.mode==2)
	{
		//时间数据显示
		buf[0]=sys_ctr.djs[2]/10+0x30;
		buf[1]=sys_ctr.djs[2]%10+0x30;
		buf[2]=':';
		buf[3]=sys_ctr.djs[1]/10+0x30;
		buf[4]=sys_ctr.djs[1]%10+0x30;
		buf[5]=':';
		buf[6]=sys_ctr.djs[0]/10+0x30;
		buf[7]=sys_ctr.djs[0]%10+0x30;
		buf[8]='\0';
		OLED_ShowString(5*8,2,buf,12);
		
		delay_ms(200);
		switch(sys_ctr.bitnum)
		{
			case 0: OLED_ShowChar(12*8,2,' ',12);break;//秒
			case 1: OLED_ShowChar(9*8,2,' ',12);break;//分
			case 2: OLED_ShowChar(6*8,2,' ',12);break;//时
		}

		break;
	}
	//计时器模式
	while(sys_ctr.mode==3)
	{
		//时间数据显示
		buf[0]=sys_ctr.msec[2]/10+0x30;
		buf[1]=sys_ctr.msec[2]%10+0x30;
		buf[2]=':';
		buf[3]=sys_ctr.msec[1]/10+0x30;
		buf[4]=sys_ctr.msec[1]%10+0x30;
		buf[5]=':';
		buf[6]=sys_ctr.msec[0]/10+0x30;
		buf[7]=sys_ctr.msec[0]%10+0x30;
		buf[8]='\0';
		OLED_ShowString(5*8,2,buf,12);

		break;
	}
}

//系统时间修改
void sys_time_change(void)
{
	u8 key=0;
	
	while(sys_ctr.set==1)
	{
		key=KEY_Scan(0);
		//选择修改时间位置
		if(key==KEY3_PRESS)
		{
			sys_ctr.bitnum++;
			if(sys_ctr.bitnum>=7)sys_ctr.bitnum=0;
			beep_alarm();//提示音				
		}
		//确定保存
		else if(key==KEY4_PRESS)
		{
			beep_alarm();//提示音
			DS1302_Init();//写入修改的时间
			sys_ctr.set=0;
			break;					
		}
		//加
		else if(key==KEY1_PRESS)
		{
			beep_alarm();//提示音
			switch(sys_ctr.bitnum)
			{
				case 0://秒 
					gDS1302_TIME[0]++;
					if((gDS1302_TIME[0]&0x0f)>9)gDS1302_TIME[0]+=6;
					if(gDS1302_TIME[0]>=0x60)gDS1302_TIME[0]=0;
					break;
				case 1://分 
					gDS1302_TIME[1]++;
					if((gDS1302_TIME[1]&0x0f)>9)gDS1302_TIME[1]+=6;
					if(gDS1302_TIME[1]>=0x60)gDS1302_TIME[1]=0;
					break;
				case 2://时 
					gDS1302_TIME[2]++;
					if((gDS1302_TIME[2]&0x0f)>9)gDS1302_TIME[2]+=6;
					if(gDS1302_TIME[2]>=0x24)gDS1302_TIME[2]=1;
					break;
				case 3://日 
					gDS1302_TIME[3]++;
					if((gDS1302_TIME[3]&0x0f)>9)gDS1302_TIME[3]+=6;
					if(gDS1302_TIME[3]>=0x32)gDS1302_TIME[3]=1;
					break;
				case 4://月 
					gDS1302_TIME[4]++;
					if((gDS1302_TIME[4]&0x0f)>9)gDS1302_TIME[4]+=6;
					if(gDS1302_TIME[4]>=0x13)gDS1302_TIME[4]=1;
					break;
				case 5://周 
					gDS1302_TIME[5]++;
					if((gDS1302_TIME[5]&0x0f)>9)gDS1302_TIME[5]+=6;
					if(gDS1302_TIME[5]>=0x08)gDS1302_TIME[5]=1;
					break;
				case 6://年 
					gDS1302_TIME[6]++;
					if((gDS1302_TIME[6]&0x0f)>9)gDS1302_TIME[6]+=6;
					if(gDS1302_TIME[6]>=0x99)gDS1302_TIME[6]=0;
					break;	
			}
		}
		//减
		else if(key==KEY2_PRESS)
		{
			beep_alarm();//提示音
			switch(sys_ctr.bitnum)
			{
				case 0://秒
					if(gDS1302_TIME[0]>=1) 
						gDS1302_TIME[0]--;
					if((gDS1302_TIME[0]&0x0f)==0x0f)gDS1302_TIME[0]-=6;
					break;
				case 1://分 
					if(gDS1302_TIME[1]>=1)
						gDS1302_TIME[1]--;
					if((gDS1302_TIME[1]&0x0f)==0x0f)gDS1302_TIME[1]-=6;
					break;
				case 2://时 
					if(gDS1302_TIME[2]>=1)
						gDS1302_TIME[2]--;
					if((gDS1302_TIME[2]&0x0f)==0x0f)gDS1302_TIME[2]-=6;
					break;
				case 3://日 
					gDS1302_TIME[3]--;
					if((gDS1302_TIME[3]&0x0f)==0x0f)gDS1302_TIME[3]-=6;
					if(gDS1302_TIME[3]==0)gDS1302_TIME[3]=0x31;
					break;
				case 4://月 
					gDS1302_TIME[4]--;
					if((gDS1302_TIME[4]&0x0f)==0x0f)gDS1302_TIME[4]-=6;
				   	if(gDS1302_TIME[4]==0)gDS1302_TIME[4]=0x12;
					break;
				case 5://周 
					gDS1302_TIME[5]--;
					if(gDS1302_TIME[5]==0)gDS1302_TIME[5]=7;
					break;
				case 6://年 
					if(gDS1302_TIME[6]>=1)
						gDS1302_TIME[6]--;
					if((gDS1302_TIME[6]&0x0f)==0x0f)gDS1302_TIME[6]-=6;
					break;	
			}
		}
		sys_data_show();//系统数据显示
	}
}

//定时闹钟模式显示
void alarm_mode_show(void)
{
	OLED_Clear();
	OLED_ShowString(0,0,"Alarm Clock",12);
	OLED_ShowString(0,2,"Time:12:43:56",12);
}

//闹钟时间设置
void alarm_time_set(void)
{
	u8 key=0;
	
	while(sys_ctr.mode==1)
	{
		key=KEY_Scan(0);
		//选择修改时间位置
		if(key==KEY3_PRESS)
		{
			sys_ctr.bitnum++;
			if(sys_ctr.bitnum>=3)sys_ctr.bitnum=0;
			beep_alarm();//提示音				
		}
		//确定保存
		else if(key==KEY4_PRESS)
		{
			beep_alarm();//提示音
			sys_ctr.mode=0;
			sys_open_show();
			ds1302_run_sta(1);
			break;					
		}
		//模式切换
		else if(key==KEY5_PRESS)
		{
			beep_alarm();//提示音
			ds1302_run_sta(0);
			sys_ctr.mode++;
			if(sys_ctr.mode>=4)
			{
				sys_ctr.mode=0;
				ds1302_run_sta(1);
			}
			switch(sys_ctr.mode)
			{
				case 0: sys_open_show();break;//正常运行模式
				case 1: alarm_mode_show();break;//定时闹钟模式
				case 2: countdown_mode_show();break;//倒计时模式
			}
			sys_ctr.bitnum=0;
			//更新定时时间同步
			sys_ctr.time[2]=gDS1302_TIME[2];//时
			sys_ctr.time[1]=gDS1302_TIME[1];//分
			sys_ctr.time[0]=gDS1302_TIME[0];//秒				
		}
		//加
		else if(key==KEY1_PRESS)
		{
			beep_alarm();//提示音
			switch(sys_ctr.bitnum)
			{
				case 0://秒 
					sys_ctr.time[0]++;
					if((sys_ctr.time[0]&0x0f)>9)sys_ctr.time[0]+=6;
					if(sys_ctr.time[0]>=0x60)sys_ctr.time[0]=0;
					break;
				case 1://分 
					sys_ctr.time[1]++;
					if((sys_ctr.time[1]&0x0f)>9)sys_ctr.time[1]+=6;
					if(sys_ctr.time[1]>=0x60)sys_ctr.time[1]=0;
					break;
				case 2://时 
					sys_ctr.time[2]++;
					if((sys_ctr.time[2]&0x0f)>9)sys_ctr.time[2]+=6;
					if(sys_ctr.time[2]>=0x13)sys_ctr.time[2]=1;
					break;		
			}	
		}
		//减
		else if(key==KEY2_PRESS)
		{
			beep_alarm();//提示音
			switch(sys_ctr.bitnum)
			{
				case 0://秒
					if(sys_ctr.time[0]>=1) 
						sys_ctr.time[0]--;
					if((sys_ctr.time[0]&0x0f)==0x0f)sys_ctr.time[0]-=6;
					break;
				case 1://分 
					if(sys_ctr.time[1]>=1)
						sys_ctr.time[1]--;
					if((sys_ctr.time[1]&0x0f)==0x0f)sys_ctr.time[1]-=6;
					break;
				case 2://时 
					if(sys_ctr.time[2]>=1)
						sys_ctr.time[2]--;
					if((sys_ctr.time[2]&0x0f)==0x0f)sys_ctr.time[2]-=6;
					break;		
			}	
		}		

		sys_data_show();//系统数据显示
	}	
}

//倒计时模式显示
void countdown_mode_show(void)
{
	OLED_Clear();
	OLED_ShowString(0,0,"CountDown  ",12);
	OLED_ShowString(0,2,"Time:00:00:00",12);
}

//倒计时时间设置
void countdowm_time_set(void)
{
	u8 key=0;
	
	while(sys_ctr.mode==2)
	{
		key=KEY_Scan(0);
		//选择修改时间位置
		if(key==KEY3_PRESS)
		{
			sys_ctr.bitnum++;
			if(sys_ctr.bitnum>=3)sys_ctr.bitnum=0;
			beep_alarm();//提示音				
		}
		//确定
		else if(key==KEY4_PRESS)
		{
			beep_alarm();//提示音
			sys_ctr.bitnum=3;
			TIM_Cmd(TIM2,ENABLE);//开启定时器					
		}
		//模式切换
		else if(key==KEY5_PRESS)
		{
			beep_alarm();//提示音
			TIM_Cmd(TIM2,DISABLE);
			ds1302_run_sta(0);
			sys_ctr.mode++;
			if(sys_ctr.mode>=4)
			{
				sys_ctr.mode=0;
				ds1302_run_sta(1);
			}
			switch(sys_ctr.mode)
			{
				case 0: sys_open_show();break;//正常运行模式
				case 1: alarm_mode_show();break;//定时闹钟模式
				case 2: countdown_mode_show();break;//倒计时模式
			}
			sys_ctr.bitnum=0;			
		}
		//加
		else if(key==KEY1_PRESS)
		{
			beep_alarm();//提示音
			switch(sys_ctr.bitnum)
			{
				case 0://秒 
					sys_ctr.djs[0]++;
					if(sys_ctr.djs[0]>=60)sys_ctr.djs[0]=0;
					break;
				case 1://分 
					sys_ctr.djs[1]++;
					if(sys_ctr.djs[1]>=60)sys_ctr.djs[1]=0;
					break;
				case 2://时 
					sys_ctr.djs[2]++;
					if(sys_ctr.djs[2]>=13)sys_ctr.djs[2]=1;
					break;		
			}	
		}
		//减
		else if(key==KEY2_PRESS)
		{
			beep_alarm();//提示音
			switch(sys_ctr.bitnum)
			{
				case 0://秒
					if(sys_ctr.djs[0]>=1) 
						sys_ctr.djs[0]--;
					break;
				case 1://分 
					if(sys_ctr.djs[1]>=1)
						sys_ctr.djs[1]--;
					break;
				case 2://时 
					if(sys_ctr.djs[2]>=1)
						sys_ctr.djs[2]--;
					break;		
			}	
		}
		sys_data_show();//系统数据显示
	}
}

//计时器模式显示
void time_mode_show(void)
{
	OLED_Clear();
	OLED_ShowString(0,0,"Time Set   ",12);
	OLED_ShowString(0,2,"Time:00:00:00",12);
}

//计时器模式设置
void sys_msectime_set(void)
{
	u8 key=0;
	
	while(sys_ctr.mode==3)
	{
		key=KEY_Scan(0);
		//开始
		if(key==KEY3_PRESS)
		{
			TIM_Cmd(TIM2,ENABLE);//开启定时器
			beep_alarm();//提示音				
		}
		//停止
		else if(key==KEY4_PRESS)
		{
			TIM_Cmd(TIM2,DISABLE);//停止定时器
			beep_alarm();//提示音				
		}
		//模式切换
		else if(key==KEY5_PRESS)
		{
			beep_alarm();//提示音
			TIM_Cmd(TIM2,DISABLE);
			ds1302_run_sta(0);
			sys_ctr.mode++;
			if(sys_ctr.mode>=4)
			{
				sys_ctr.mode=0;
				ds1302_run_sta(1);
			}
			switch(sys_ctr.mode)
			{
				case 0: sys_open_show();break;//正常运行模式
				case 1: alarm_mode_show();break;//定时闹钟模式
				case 2: countdown_mode_show();break;//倒计时模式
				case 3: time_mode_show();break;//计时器模式
			}
			sys_ctr.bitnum=0;			
		}
		sys_data_show();//系统数据显示
	}
}

//系统参数设置
void sys_parm_set(void)
{
	u8 key=0;
	
	key=KEY_Scan(0);
	//时间修改
	if(key==KEY3_PRESS)
	{
		sys_ctr.set=1;
		sys_ctr.bitnum=0;
		ds1302_run_sta(0);
		beep_alarm();//提示音
	}
	//模式设置
	if(key==KEY5_PRESS)
	{
		beep_alarm();//提示音
		TIM_Cmd(TIM2,DISABLE);
		ds1302_run_sta(0);
		sys_ctr.mode++;
		if(sys_ctr.mode>=4)
		{
			sys_ctr.mode=0;
			ds1302_run_sta(1);
		}
		switch(sys_ctr.mode)
		{
			case 0: sys_open_show();break;//正常运行模式
			case 1: alarm_mode_show();break;//定时闹钟模式
			case 2: countdown_mode_show();break;//倒计时模式
			case 3: time_mode_show();break;//计时器模式
		}
		sys_ctr.bitnum=0;
		//更新定时时间同步
		sys_ctr.time[2]=gDS1302_TIME[2];//时
		sys_ctr.time[1]=gDS1302_TIME[1];//分
		sys_ctr.time[0]=gDS1302_TIME[0];//秒
		sys_ctr.djs[2]=10;sys_ctr.djs[1]=8;sys_ctr.djs[0]=5;
		sys_ctr.msec[0]=0;sys_ctr.msec[1]=0;sys_ctr.msec[2]=0;
	}
	
	sys_time_change();
	alarm_time_set();
	countdowm_time_set();
	sys_msectime_set();
}

//闹钟检测
void alarm_check(void)
{
	//正常模式
	if(sys_ctr.mode==0)
	{
		while(sys_ctr.set==0)
		{
			//定时到达
			if(gDS1302_TIME[0]==sys_ctr.time[0] && gDS1302_TIME[1]==sys_ctr.time[1]
				&& gDS1302_TIME[2]==sys_ctr.time[2])
			{
				BEEP=0;	
			}
			else if(gDS1302_TIME[0]>=sys_ctr.time[0]+1 && gDS1302_TIME[1]==sys_ctr.time[1]
				&& gDS1302_TIME[2]==sys_ctr.time[2])
				BEEP=1;	
			
			break;
		}
	}
	
}

//应用系统控制
void sys_control(void)
{
	sys_parm_init();//系统参数初始化
	LED_Init();
	BEEP_Init();
	KEY_Init();
	OLED_Init();
	TIM2_Init(10000-1,7);//定时10ms
	DS1302_GPIO_Init();
	DS1302_Init();
	while(DS18B20_Init())
	{
		OLED_ShowString(0,0,"DS18B20 Error!",12);
		delay_ms(500);
	}
	sys_open_show();
	
	while(1)
	{
		sys_data_get();//系统数据采集
		sys_data_show();//系统数据显示
		sys_parm_set();//系统参数设置
		alarm_check();//闹钟检测
	}
}

/*******************************************************************************
* 函 数 名         : TIM4_IRQHandler
* 函数功能		   : TIM4中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	static u8 i=0;
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update))
	{
		//倒计时模式
		if(sys_ctr.mode==2)
		{
			i++;
			if(i>=100)//间隔1S
			{
				i=0;
				
				if(sys_ctr.djs[0]==0)
				{
					if(sys_ctr.djs[1]>0)
					{
						sys_ctr.djs[1]--;
						sys_ctr.djs[0]=60;
						if(sys_ctr.djs[1]==0)
						{
							if(sys_ctr.djs[2]>0)
							{
								sys_ctr.djs[2]--;
								sys_ctr.djs[1]=60;		
							}
						}		
					}
				}
				sys_ctr.djs[0]--;
				if(sys_ctr.djs[0]==0&&sys_ctr.djs[1]==0&&sys_ctr.djs[2]==0)
				{
					TIM_Cmd(TIM2,DISABLE);
					BEEP=0;
					delay_ms(1000);
					BEEP=1;	
				}
			}	
		}
		//秒表模式
		else if(sys_ctr.mode==3)
		{
			sys_ctr.msec[0]++;
			if(sys_ctr.msec[0]>=100)
			{
				sys_ctr.msec[0]=0;
				sys_ctr.msec[1]++;
				if(sys_ctr.msec[1]>=60)
				{
					sys_ctr.msec[1]=0;
					sys_ctr.msec[2]++;
					if(sys_ctr.msec[2]>=60)sys_ctr.msec[2]=0;
				}	
			}	
		}
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);	
}
