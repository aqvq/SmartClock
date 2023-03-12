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
//����
_sys_ctr sys_ctr;


//ϵͳ������ʼ��
void sys_parm_init(void)
{
	sys_ctr.mode=0;
	sys_ctr.set=0;
	sys_ctr.bitnum=0;
}


//ϵͳ����������ʾ
void sys_open_show(void)
{
	OLED_Clear();
	OLED_ShowString(0,0,"Tempe         C",12);
	OLED_ShowString(0,2,"Date:2022-05-06",12);
	OLED_ShowString(0,4,"Time:12:43:56",12);
	OLED_ShowString(0,6,"Day:7",12);
}

//ϵͳ���ݲɼ�
void sys_data_get(void)
{
	static u16 i=0;
	
	while(sys_ctr.mode==0)
	{	
		i++;
		if(i%1==0)
		{
			//��ȡ�¶�
			sys_ctr.temp=DS18B20_GetTemperture();
		}
		//��ȡʱ��
		DS1302_Read_Time();
		
		break;
	}
}

//ϵͳ������ʾ
void sys_data_show(void)
{
	u8 buf[9];
	u16 temp=0;
	
	//����ģʽ��ʾ
	while(sys_ctr.mode==0)
	{
		//�¶�������ʾ
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
		
		//����ʱ����ʾ
		while(sys_ctr.set==0)
		{
			//����������ʾ
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
			//ʱ��������ʾ
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
			//������ʾ
			OLED_ShowNum(4*8,6,gDS1302_TIME[5],1,12);
			break;
		}
		//�޸�ʱ����ʾ
		while(sys_ctr.set==1)
		{
			//����������ʾ
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
			//ʱ��������ʾ
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
			//������ʾ
			OLED_ShowNum(4*8,6,gDS1302_TIME[5],1,12);
			delay_ms(200);
			switch(sys_ctr.bitnum)
			{
				case 0: OLED_ShowChar(12*8,4,' ',12);break;//��
				case 1: OLED_ShowChar(9*8,4,' ',12);break;//��
				case 2: OLED_ShowChar(6*8,4,' ',12);break;//ʱ
				case 3: OLED_ShowChar(14*8,2,' ',12);break;//��
				case 4: OLED_ShowChar(11*8,2,' ',12);break;//��
				case 5: OLED_ShowChar(4*8,6,' ',12);break;//��
				case 6: OLED_ShowChar(8*8,2,' ',12);break;//��
			}
			
			break;
		}
		
		break;
	}
	//����ģʽ��ʾ
	while(sys_ctr.mode==1)
	{
		//ʱ��������ʾ
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
			case 0: OLED_ShowChar(12*8,2,' ',12);break;//��
			case 1: OLED_ShowChar(9*8,2,' ',12);break;//��
			case 2: OLED_ShowChar(6*8,2,' ',12);break;//ʱ
		}
		break;
	}
	//����ʱģʽ
	while(sys_ctr.mode==2)
	{
		//ʱ��������ʾ
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
			case 0: OLED_ShowChar(12*8,2,' ',12);break;//��
			case 1: OLED_ShowChar(9*8,2,' ',12);break;//��
			case 2: OLED_ShowChar(6*8,2,' ',12);break;//ʱ
		}

		break;
	}
	//��ʱ��ģʽ
	while(sys_ctr.mode==3)
	{
		//ʱ��������ʾ
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

//ϵͳʱ���޸�
void sys_time_change(void)
{
	u8 key=0;
	
	while(sys_ctr.set==1)
	{
		key=KEY_Scan(0);
		//ѡ���޸�ʱ��λ��
		if(key==KEY3_PRESS)
		{
			sys_ctr.bitnum++;
			if(sys_ctr.bitnum>=7)sys_ctr.bitnum=0;
			beep_alarm();//��ʾ��				
		}
		//ȷ������
		else if(key==KEY4_PRESS)
		{
			beep_alarm();//��ʾ��
			DS1302_Init();//д���޸ĵ�ʱ��
			sys_ctr.set=0;
			break;					
		}
		//��
		else if(key==KEY1_PRESS)
		{
			beep_alarm();//��ʾ��
			switch(sys_ctr.bitnum)
			{
				case 0://�� 
					gDS1302_TIME[0]++;
					if((gDS1302_TIME[0]&0x0f)>9)gDS1302_TIME[0]+=6;
					if(gDS1302_TIME[0]>=0x60)gDS1302_TIME[0]=0;
					break;
				case 1://�� 
					gDS1302_TIME[1]++;
					if((gDS1302_TIME[1]&0x0f)>9)gDS1302_TIME[1]+=6;
					if(gDS1302_TIME[1]>=0x60)gDS1302_TIME[1]=0;
					break;
				case 2://ʱ 
					gDS1302_TIME[2]++;
					if((gDS1302_TIME[2]&0x0f)>9)gDS1302_TIME[2]+=6;
					if(gDS1302_TIME[2]>=0x24)gDS1302_TIME[2]=1;
					break;
				case 3://�� 
					gDS1302_TIME[3]++;
					if((gDS1302_TIME[3]&0x0f)>9)gDS1302_TIME[3]+=6;
					if(gDS1302_TIME[3]>=0x32)gDS1302_TIME[3]=1;
					break;
				case 4://�� 
					gDS1302_TIME[4]++;
					if((gDS1302_TIME[4]&0x0f)>9)gDS1302_TIME[4]+=6;
					if(gDS1302_TIME[4]>=0x13)gDS1302_TIME[4]=1;
					break;
				case 5://�� 
					gDS1302_TIME[5]++;
					if((gDS1302_TIME[5]&0x0f)>9)gDS1302_TIME[5]+=6;
					if(gDS1302_TIME[5]>=0x08)gDS1302_TIME[5]=1;
					break;
				case 6://�� 
					gDS1302_TIME[6]++;
					if((gDS1302_TIME[6]&0x0f)>9)gDS1302_TIME[6]+=6;
					if(gDS1302_TIME[6]>=0x99)gDS1302_TIME[6]=0;
					break;	
			}
		}
		//��
		else if(key==KEY2_PRESS)
		{
			beep_alarm();//��ʾ��
			switch(sys_ctr.bitnum)
			{
				case 0://��
					if(gDS1302_TIME[0]>=1) 
						gDS1302_TIME[0]--;
					if((gDS1302_TIME[0]&0x0f)==0x0f)gDS1302_TIME[0]-=6;
					break;
				case 1://�� 
					if(gDS1302_TIME[1]>=1)
						gDS1302_TIME[1]--;
					if((gDS1302_TIME[1]&0x0f)==0x0f)gDS1302_TIME[1]-=6;
					break;
				case 2://ʱ 
					if(gDS1302_TIME[2]>=1)
						gDS1302_TIME[2]--;
					if((gDS1302_TIME[2]&0x0f)==0x0f)gDS1302_TIME[2]-=6;
					break;
				case 3://�� 
					gDS1302_TIME[3]--;
					if((gDS1302_TIME[3]&0x0f)==0x0f)gDS1302_TIME[3]-=6;
					if(gDS1302_TIME[3]==0)gDS1302_TIME[3]=0x31;
					break;
				case 4://�� 
					gDS1302_TIME[4]--;
					if((gDS1302_TIME[4]&0x0f)==0x0f)gDS1302_TIME[4]-=6;
				   	if(gDS1302_TIME[4]==0)gDS1302_TIME[4]=0x12;
					break;
				case 5://�� 
					gDS1302_TIME[5]--;
					if(gDS1302_TIME[5]==0)gDS1302_TIME[5]=7;
					break;
				case 6://�� 
					if(gDS1302_TIME[6]>=1)
						gDS1302_TIME[6]--;
					if((gDS1302_TIME[6]&0x0f)==0x0f)gDS1302_TIME[6]-=6;
					break;	
			}
		}
		sys_data_show();//ϵͳ������ʾ
	}
}

//��ʱ����ģʽ��ʾ
void alarm_mode_show(void)
{
	OLED_Clear();
	OLED_ShowString(0,0,"Alarm Clock",12);
	OLED_ShowString(0,2,"Time:12:43:56",12);
}

//����ʱ������
void alarm_time_set(void)
{
	u8 key=0;
	
	while(sys_ctr.mode==1)
	{
		key=KEY_Scan(0);
		//ѡ���޸�ʱ��λ��
		if(key==KEY3_PRESS)
		{
			sys_ctr.bitnum++;
			if(sys_ctr.bitnum>=3)sys_ctr.bitnum=0;
			beep_alarm();//��ʾ��				
		}
		//ȷ������
		else if(key==KEY4_PRESS)
		{
			beep_alarm();//��ʾ��
			sys_ctr.mode=0;
			sys_open_show();
			ds1302_run_sta(1);
			break;					
		}
		//ģʽ�л�
		else if(key==KEY5_PRESS)
		{
			beep_alarm();//��ʾ��
			ds1302_run_sta(0);
			sys_ctr.mode++;
			if(sys_ctr.mode>=4)
			{
				sys_ctr.mode=0;
				ds1302_run_sta(1);
			}
			switch(sys_ctr.mode)
			{
				case 0: sys_open_show();break;//��������ģʽ
				case 1: alarm_mode_show();break;//��ʱ����ģʽ
				case 2: countdown_mode_show();break;//����ʱģʽ
			}
			sys_ctr.bitnum=0;
			//���¶�ʱʱ��ͬ��
			sys_ctr.time[2]=gDS1302_TIME[2];//ʱ
			sys_ctr.time[1]=gDS1302_TIME[1];//��
			sys_ctr.time[0]=gDS1302_TIME[0];//��				
		}
		//��
		else if(key==KEY1_PRESS)
		{
			beep_alarm();//��ʾ��
			switch(sys_ctr.bitnum)
			{
				case 0://�� 
					sys_ctr.time[0]++;
					if((sys_ctr.time[0]&0x0f)>9)sys_ctr.time[0]+=6;
					if(sys_ctr.time[0]>=0x60)sys_ctr.time[0]=0;
					break;
				case 1://�� 
					sys_ctr.time[1]++;
					if((sys_ctr.time[1]&0x0f)>9)sys_ctr.time[1]+=6;
					if(sys_ctr.time[1]>=0x60)sys_ctr.time[1]=0;
					break;
				case 2://ʱ 
					sys_ctr.time[2]++;
					if((sys_ctr.time[2]&0x0f)>9)sys_ctr.time[2]+=6;
					if(sys_ctr.time[2]>=0x13)sys_ctr.time[2]=1;
					break;		
			}	
		}
		//��
		else if(key==KEY2_PRESS)
		{
			beep_alarm();//��ʾ��
			switch(sys_ctr.bitnum)
			{
				case 0://��
					if(sys_ctr.time[0]>=1) 
						sys_ctr.time[0]--;
					if((sys_ctr.time[0]&0x0f)==0x0f)sys_ctr.time[0]-=6;
					break;
				case 1://�� 
					if(sys_ctr.time[1]>=1)
						sys_ctr.time[1]--;
					if((sys_ctr.time[1]&0x0f)==0x0f)sys_ctr.time[1]-=6;
					break;
				case 2://ʱ 
					if(sys_ctr.time[2]>=1)
						sys_ctr.time[2]--;
					if((sys_ctr.time[2]&0x0f)==0x0f)sys_ctr.time[2]-=6;
					break;		
			}	
		}		

		sys_data_show();//ϵͳ������ʾ
	}	
}

//����ʱģʽ��ʾ
void countdown_mode_show(void)
{
	OLED_Clear();
	OLED_ShowString(0,0,"CountDown  ",12);
	OLED_ShowString(0,2,"Time:00:00:00",12);
}

//����ʱʱ������
void countdowm_time_set(void)
{
	u8 key=0;
	
	while(sys_ctr.mode==2)
	{
		key=KEY_Scan(0);
		//ѡ���޸�ʱ��λ��
		if(key==KEY3_PRESS)
		{
			sys_ctr.bitnum++;
			if(sys_ctr.bitnum>=3)sys_ctr.bitnum=0;
			beep_alarm();//��ʾ��				
		}
		//ȷ��
		else if(key==KEY4_PRESS)
		{
			beep_alarm();//��ʾ��
			sys_ctr.bitnum=3;
			TIM_Cmd(TIM2,ENABLE);//������ʱ��					
		}
		//ģʽ�л�
		else if(key==KEY5_PRESS)
		{
			beep_alarm();//��ʾ��
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
				case 0: sys_open_show();break;//��������ģʽ
				case 1: alarm_mode_show();break;//��ʱ����ģʽ
				case 2: countdown_mode_show();break;//����ʱģʽ
			}
			sys_ctr.bitnum=0;			
		}
		//��
		else if(key==KEY1_PRESS)
		{
			beep_alarm();//��ʾ��
			switch(sys_ctr.bitnum)
			{
				case 0://�� 
					sys_ctr.djs[0]++;
					if(sys_ctr.djs[0]>=60)sys_ctr.djs[0]=0;
					break;
				case 1://�� 
					sys_ctr.djs[1]++;
					if(sys_ctr.djs[1]>=60)sys_ctr.djs[1]=0;
					break;
				case 2://ʱ 
					sys_ctr.djs[2]++;
					if(sys_ctr.djs[2]>=13)sys_ctr.djs[2]=1;
					break;		
			}	
		}
		//��
		else if(key==KEY2_PRESS)
		{
			beep_alarm();//��ʾ��
			switch(sys_ctr.bitnum)
			{
				case 0://��
					if(sys_ctr.djs[0]>=1) 
						sys_ctr.djs[0]--;
					break;
				case 1://�� 
					if(sys_ctr.djs[1]>=1)
						sys_ctr.djs[1]--;
					break;
				case 2://ʱ 
					if(sys_ctr.djs[2]>=1)
						sys_ctr.djs[2]--;
					break;		
			}	
		}
		sys_data_show();//ϵͳ������ʾ
	}
}

//��ʱ��ģʽ��ʾ
void time_mode_show(void)
{
	OLED_Clear();
	OLED_ShowString(0,0,"Time Set   ",12);
	OLED_ShowString(0,2,"Time:00:00:00",12);
}

//��ʱ��ģʽ����
void sys_msectime_set(void)
{
	u8 key=0;
	
	while(sys_ctr.mode==3)
	{
		key=KEY_Scan(0);
		//��ʼ
		if(key==KEY3_PRESS)
		{
			TIM_Cmd(TIM2,ENABLE);//������ʱ��
			beep_alarm();//��ʾ��				
		}
		//ֹͣ
		else if(key==KEY4_PRESS)
		{
			TIM_Cmd(TIM2,DISABLE);//ֹͣ��ʱ��
			beep_alarm();//��ʾ��				
		}
		//ģʽ�л�
		else if(key==KEY5_PRESS)
		{
			beep_alarm();//��ʾ��
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
				case 0: sys_open_show();break;//��������ģʽ
				case 1: alarm_mode_show();break;//��ʱ����ģʽ
				case 2: countdown_mode_show();break;//����ʱģʽ
				case 3: time_mode_show();break;//��ʱ��ģʽ
			}
			sys_ctr.bitnum=0;			
		}
		sys_data_show();//ϵͳ������ʾ
	}
}

//ϵͳ��������
void sys_parm_set(void)
{
	u8 key=0;
	
	key=KEY_Scan(0);
	//ʱ���޸�
	if(key==KEY3_PRESS)
	{
		sys_ctr.set=1;
		sys_ctr.bitnum=0;
		ds1302_run_sta(0);
		beep_alarm();//��ʾ��
	}
	//ģʽ����
	if(key==KEY5_PRESS)
	{
		beep_alarm();//��ʾ��
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
			case 0: sys_open_show();break;//��������ģʽ
			case 1: alarm_mode_show();break;//��ʱ����ģʽ
			case 2: countdown_mode_show();break;//����ʱģʽ
			case 3: time_mode_show();break;//��ʱ��ģʽ
		}
		sys_ctr.bitnum=0;
		//���¶�ʱʱ��ͬ��
		sys_ctr.time[2]=gDS1302_TIME[2];//ʱ
		sys_ctr.time[1]=gDS1302_TIME[1];//��
		sys_ctr.time[0]=gDS1302_TIME[0];//��
		sys_ctr.djs[2]=10;sys_ctr.djs[1]=8;sys_ctr.djs[0]=5;
		sys_ctr.msec[0]=0;sys_ctr.msec[1]=0;sys_ctr.msec[2]=0;
	}
	
	sys_time_change();
	alarm_time_set();
	countdowm_time_set();
	sys_msectime_set();
}

//���Ӽ��
void alarm_check(void)
{
	//����ģʽ
	if(sys_ctr.mode==0)
	{
		while(sys_ctr.set==0)
		{
			//��ʱ����
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

//Ӧ��ϵͳ����
void sys_control(void)
{
	sys_parm_init();//ϵͳ������ʼ��
	LED_Init();
	BEEP_Init();
	KEY_Init();
	OLED_Init();
	TIM2_Init(10000-1,7);//��ʱ10ms
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
		sys_data_get();//ϵͳ���ݲɼ�
		sys_data_show();//ϵͳ������ʾ
		sys_parm_set();//ϵͳ��������
		alarm_check();//���Ӽ��
	}
}

/*******************************************************************************
* �� �� ��         : TIM4_IRQHandler
* ��������		   : TIM4�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	static u8 i=0;
	
	if(TIM_GetITStatus(TIM2,TIM_IT_Update))
	{
		//����ʱģʽ
		if(sys_ctr.mode==2)
		{
			i++;
			if(i>=100)//���1S
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
		//���ģʽ
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
