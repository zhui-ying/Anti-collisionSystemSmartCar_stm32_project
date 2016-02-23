/********************************* 深圳市航太电子有限公司 *******************************
* 实 验 名 ：小车智能防撞报警设计
* 实验说明 ：小车在前进过程中实时检测与前方障碍物的距离，并根据当前速度计算出安全距离，
              若实际距离要小于安全距离，则报警，若实际距离小于30cm，则直接刹车。
              安全距离 = k*速度 + b,k,b参数可调  
* 实验平台 ：航太ARM单片机开发板
* 连接方式 ：请参考interface.h文件
* 注    意 ：
* 作    者 ：航太电子产品研发部    QQ ：1909197536
* 店    铺 ：http://shop120013844.taobao.com/
****************************************************************************************/

#include "stm32f10x.h"
#include "interface.h"
#include "LCD12864.h"
#include "IRCtrol.h"
#include "motor.h"
#include "UltrasonicCtrol.h"
#include "speed.h"
#include "uart.h"

void ShowMsg(void);

//全局变量定义
unsigned int speed_count=0;//占空比计数器 50次一周期
char front_left_speed_duty=SPEED_DUTY;
char front_right_speed_duty=SPEED_DUTY;
char behind_left_speed_duty=SPEED_DUTY;
char behind_right_speed_duty=SPEED_DUTY;

unsigned char tick_5ms = 0;//5ms计数器，作为主函数的基本周期
unsigned char tick_1ms = 0;//1ms计数器，作为电机的基本计数器
unsigned char tick_200ms = 0;//刷新显示
unsigned char tick_100ms = 0;//刷新显示

char ctrl_comm = COMM_STOP;//控制指令
unsigned char continue_time=0;
unsigned int speed=0;
unsigned int distance_cm_safe=100;//报警值
unsigned char k_rate = 50;
unsigned char b_rate = 50;
unsigned char warning_flag = 0;//0 不报警  1 报警

char led_flag=0;

int main(void)
{
	delay_init();
	GPIOCLKInit();
	UserLEDInit();
	LCD12864Init();
	IRCtrolInit();
	TIM2_Init();
	MotorInit();
	UltraSoundInit();
	ServoInit();
	BeepLEDInit();
	MeaSpeedInit();
	USART3Conf(9600);
	LED_NORMAL_ON;

 while(1)
 {	 
	 		if(tick_5ms >= 5)
		{
			tick_5ms = 0;
			tick_200ms++;
			tick_100ms++;
			if(tick_200ms >= 40)
			{
				tick_200ms = 0;
				LEDToggle(LED_PIN);	
				//ShowMsg();
			}
			if(tick_100ms >= 20)
			{
				unsigned int left,right;	
				tick_100ms = 0;				
				left = GetLeftSpeed();
				right = GetRightSpeed();
				
				//取较大的速度
				if(left > right) speed = left;
				else speed = right;		
				distance_cm_safe = MUL_K_RATE*(double)k_rate*(double)speed + MUL_B_RATE*(double)b_rate +0.5;//	 + BRAKE_DIS
				ShowMsg();				
			}

			speed_contiue--;//1s 测速模块无脉冲变化，测速值归零
			if(speed_contiue == 0)
			{
				speed_contiue = 1;
				//清buf
				ClearSpeedBuf();
			}
			continue_time--;//200ms 无接收指令就停车
			if(continue_time == 0)
			{
				continue_time = 1;
				CarStop();
				ClearSpeedBuf();
			}
			//do something
			Distance();//计算距离
			
			//安全监测
			if(distance_cm_last <= distance_cm_safe && distance_cm <= distance_cm_safe && speed > 0 && ctrl_comm != COMM_DOWN)//连续两次的测量大于安全距离，且速度不为0,当前不为后退
			{
				warning_flag = 1;
				if(distance_cm < BRAKE_DIS)//刹车
				{
					CarBack();
					Delayms(50);
					CarStop();
					Delayms(50);
					tick_100ms = 20;//进入一次速度检测
					ClearSpeedBuf();
					Delayms(50);
					//continue;
				}
			}else
			{
				warning_flag = 0;
			}
			
			if(ir_rec_flag == 1)//接收到红外信号
			{
				ir_rec_flag = 0;
				switch(ctrl_comm)
				{
					case COMM_UP:    
						if(distance_cm >= BRAKE_DIS)//大于安全距离才能前进
						{
							CarGo();
						}
					break;
					case COMM_DOWN:  CarBack();break;
					case COMM_LEFT:  CarLeft();break;
					case COMM_RIGHT: CarRight();break;
					case COMM_STOP:  CarStop();break;
				  case COMM_S_ADD: 
						speedduty+=2;
					  if(speedduty > 50) speedduty = 50;
//						if(distance_cm >= BRAKE_DIS)//大于安全距离才能前进
//						{
//							CarGo();
//						}
					break;
				  case COMM_S_SUB: 
						speedduty-=2;
						if(speedduty < 20) speedduty = 20;
//						if(distance_cm >= BRAKE_DIS)//大于安全距离才能前进
//						{
//							CarGo();
//						}
					break;						
				  case COMM_K_ADD: 
						k_rate++;
						if(k_rate > 100) k_rate = 100;
					break;
				  case COMM_K_SUB: 
						if(k_rate == 0) k_rate = 1;
						k_rate--;
					break;						
				  case COMM_B_ADD: 
						b_rate++;
						if(b_rate > 100) b_rate = 100;	
					break;
				  case COMM_B_SUB: 
						if(b_rate == 0) b_rate = 1;
						b_rate--;
					break;						
					default : break;
				}
			}
		}
		
 }
}

//显示各种信息以及报警 每200ms执行一次
void ShowMsg(void)
{
	static char switch_flag=0;
	LCD12864WriteSpeed(speed);//更新速度
	LCD12864WriteDistance(distance_cm);//更新距离
	LCD12864WriteParam(k_rate,b_rate);//更新比例参数
	LCD12864WriteDistanceSafe(distance_cm_safe);//更新安全距离
	
	if(warning_flag == 1)//开始报警 蜂鸣器响起 LED闪动 屏幕显示闪动
	{
		if(switch_flag == 0)
		{
	//		LCD12864WriteDistanceSafe(distance_cm_safe);//更新安全距离
			LED_WARMING_OFF;
			BEEP_OFF;
			switch_flag = 1;
		}else
		{
			LCD12864WriteWarn();
			LED_WARMING_ON;
			BEEP_ON;		
			switch_flag = 0;
		}
	}else
	{
			LED_WARMING_OFF;
			BEEP_OFF;		
	}
}

