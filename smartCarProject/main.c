/********************************* �����к�̫�������޹�˾ *******************************
* ʵ �� �� ��С�����ܷ�ײ�������
* ʵ��˵�� ��С����ǰ��������ʵʱ�����ǰ���ϰ���ľ��룬�����ݵ�ǰ�ٶȼ������ȫ���룬
              ��ʵ�ʾ���ҪС�ڰ�ȫ���룬�򱨾�����ʵ�ʾ���С��30cm����ֱ��ɲ����
              ��ȫ���� = k*�ٶ� + b,k,b�����ɵ�  
* ʵ��ƽ̨ ����̫ARM��Ƭ��������
* ���ӷ�ʽ ����ο�interface.h�ļ�
* ע    �� ��
* ��    �� ����̫���Ӳ�Ʒ�з���    QQ ��1909197536
* ��    �� ��http://shop120013844.taobao.com/
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

//ȫ�ֱ�������
unsigned int speed_count=0;//ռ�ձȼ����� 50��һ����
char front_left_speed_duty=SPEED_DUTY;
char front_right_speed_duty=SPEED_DUTY;
char behind_left_speed_duty=SPEED_DUTY;
char behind_right_speed_duty=SPEED_DUTY;

unsigned char tick_5ms = 0;//5ms����������Ϊ�������Ļ�������
unsigned char tick_1ms = 0;//1ms����������Ϊ����Ļ���������
unsigned char tick_200ms = 0;//ˢ����ʾ
unsigned char tick_100ms = 0;//ˢ����ʾ

char ctrl_comm = COMM_STOP;//����ָ��
unsigned char continue_time=0;
unsigned int speed=0;
unsigned int distance_cm_safe=100;//����ֵ
unsigned char k_rate = 50;
unsigned char b_rate = 50;
unsigned char warning_flag = 0;//0 ������  1 ����

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
				
				//ȡ�ϴ���ٶ�
				if(left > right) speed = left;
				else speed = right;		
				distance_cm_safe = MUL_K_RATE*(double)k_rate*(double)speed + MUL_B_RATE*(double)b_rate +0.5;//	 + BRAKE_DIS
				ShowMsg();				
			}

			speed_contiue--;//1s ����ģ��������仯������ֵ����
			if(speed_contiue == 0)
			{
				speed_contiue = 1;
				//��buf
				ClearSpeedBuf();
			}
			continue_time--;//200ms �޽���ָ���ͣ��
			if(continue_time == 0)
			{
				continue_time = 1;
				CarStop();
				ClearSpeedBuf();
			}
			//do something
			Distance();//�������
			
			//��ȫ���
			if(distance_cm_last <= distance_cm_safe && distance_cm <= distance_cm_safe && speed > 0 && ctrl_comm != COMM_DOWN)//�������εĲ������ڰ�ȫ���룬���ٶȲ�Ϊ0,��ǰ��Ϊ����
			{
				warning_flag = 1;
				if(distance_cm < BRAKE_DIS)//ɲ��
				{
					CarBack();
					Delayms(50);
					CarStop();
					Delayms(50);
					tick_100ms = 20;//����һ���ٶȼ��
					ClearSpeedBuf();
					Delayms(50);
					//continue;
				}
			}else
			{
				warning_flag = 0;
			}
			
			if(ir_rec_flag == 1)//���յ������ź�
			{
				ir_rec_flag = 0;
				switch(ctrl_comm)
				{
					case COMM_UP:    
						if(distance_cm >= BRAKE_DIS)//���ڰ�ȫ�������ǰ��
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
//						if(distance_cm >= BRAKE_DIS)//���ڰ�ȫ�������ǰ��
//						{
//							CarGo();
//						}
					break;
				  case COMM_S_SUB: 
						speedduty-=2;
						if(speedduty < 20) speedduty = 20;
//						if(distance_cm >= BRAKE_DIS)//���ڰ�ȫ�������ǰ��
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

//��ʾ������Ϣ�Լ����� ÿ200msִ��һ��
void ShowMsg(void)
{
	static char switch_flag=0;
	LCD12864WriteSpeed(speed);//�����ٶ�
	LCD12864WriteDistance(distance_cm);//���¾���
	LCD12864WriteParam(k_rate,b_rate);//���±�������
	LCD12864WriteDistanceSafe(distance_cm_safe);//���°�ȫ����
	
	if(warning_flag == 1)//��ʼ���� ���������� LED���� ��Ļ��ʾ����
	{
		if(switch_flag == 0)
		{
	//		LCD12864WriteDistanceSafe(distance_cm_safe);//���°�ȫ����
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

