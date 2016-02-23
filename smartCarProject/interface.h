#ifndef __INTERFACE_H_
#define __INTERFACE_H_

#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>

//参数设置 安全距离 = MUL_K_RATE*k*v + MUL_B_RATE*b 
#define BRAKE_DIS 30//刹车距离
#define MUL_K_RATE 0.02
#define MUL_B_RATE 0.5



#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

//LCD LED_NORMAL  PG10
#define LED_NORMAL_PIN         GPIO_Pin_10
#define LED_NORMAL_GPIO        GPIOG
#define LED_NORMAL_SET         GPIO_SetBits(LED_NORMAL_GPIO , LED_NORMAL_PIN)
#define LED_NORMAL_RESET       GPIO_ResetBits(LED_NORMAL_GPIO , LED_NORMAL_PIN)
#define LED_NORMAL_ON          LED_NORMAL_RESET
#define LED_NORMAL_OFF         LED_NORMAL_SET
//LCD LED_WARMING   PD7
#define LED_WARMING_PIN         GPIO_Pin_7
#define LED_WARMING_GPIO        GPIOD
#define LED_WARMING_SET         GPIO_SetBits(LED_WARMING_GPIO , LED_WARMING_PIN)
#define LED_WARMING_RESET       GPIO_ResetBits(LED_WARMING_GPIO , LED_WARMING_PIN)
#define LED_WARMING_ON          LED_WARMING_RESET
#define LED_WARMING_OFF         LED_WARMING_SET

//user BEEP PG12
#define BEEP_PIN         GPIO_Pin_12
#define BEEP_GPIO        GPIOG
#define BEEP_SET         GPIO_SetBits(BEEP_GPIO , BEEP_PIN)
#define BEEP_RESET       GPIO_ResetBits(BEEP_GPIO , BEEP_PIN)
#define BEEP_ON          BEEP_SET
#define BEEP_OFF         BEEP_RESET

//user LED PG15
#define LED_PIN         GPIO_Pin_15
#define LED_GPIO        GPIOG
#define LED_SET         GPIO_SetBits(LED_GPIO , LED_PIN)
#define LED_RESET       GPIO_ResetBits(LED_GPIO , LED_PIN)

//红外遥控 红外接收器数据线,外部中断 PD10
#define IRIN_PIN         GPIO_Pin_11
#define IRIN_GPIO        GPIOD
#define IRIN_PORTSOURCE  GPIO_PortSourceGPIOD
#define IRIN_PINSOURCE   GPIO_PinSource11
#define IRIN_EXITLINE    EXTI_Line11
#define IRIN_IRQCH       EXTI15_10_IRQn
#define IRIN             GPIO_ReadInputDataBit(IRIN_GPIO, IRIN_PIN)


//超声波控制 外部中断返回 PB14
#define Echo_PIN         GPIO_Pin_14
#define Echo_GPIO        GPIOB
#define Echo_PORTSOURCE  GPIO_PortSourceGPIOB
#define Echo_PINSOURCE   GPIO_PinSource14
#define Echo_EXITLINE    EXTI_Line14
#define Echo_IRQCH       EXTI15_10_IRQn
#define Echo             GPIO_ReadInputDataBit(Echo_GPIO, Echo_PIN)
//触发IO PD8
#define Trig_PIN         GPIO_Pin_8
#define Trig_GPIO        GPIOD
#define Trig_RESET       GPIO_SetBits(Trig_GPIO , Trig_PIN)
#define Trig_SET         GPIO_ResetBits(Trig_GPIO , Trig_PIN)//5转3.3做了一次反相 modfied by LC 2015.09.20 10:18


//舵机控制IO PD12
#define Servo_PIN         GPIO_Pin_12
#define Servo_GPIO        GPIOD
#define Servo_SET         GPIO_SetBits(Servo_GPIO , Servo_PIN)
#define Servo_RESET       GPIO_ResetBits(Servo_GPIO , Servo_PIN)


//mini12864
//LCDCS PC10   
#define LCDCS_PIN         GPIO_Pin_10
#define LCDCS_GPIO        GPIOC
#define LCDCS_SET         GPIO_SetBits(LCDCS_GPIO , LCDCS_PIN)
#define LCDCS_RESET       GPIO_ResetBits(LCDCS_GPIO , LCDCS_PIN)
//LCDRST PC12
#define LCDRST_PIN         GPIO_Pin_12
#define LCDRST_GPIO        GPIOC
#define LCDRST_SET         GPIO_SetBits(LCDRST_GPIO , LCDRST_PIN)
#define LCDRST_RESET       GPIO_ResetBits(LCDRST_GPIO , LCDRST_PIN)
//LCDRS	PD1
#define LCDRS_PIN         GPIO_Pin_5 //原值为PD1 modfied by LC 2015.12.21 20:37
#define LCDRS_GPIO        GPIOD
#define LCDRS_SET         GPIO_SetBits(LCDRS_GPIO , LCDRS_PIN)
#define LCDRS_RESET       GPIO_ResetBits(LCDRS_GPIO , LCDRS_PIN)
//LCDSCL PD3
#define LCDSCL_PIN         GPIO_Pin_3
#define LCDSCL_GPIO        GPIOD
#define LCDSCL_SET         GPIO_SetBits(LCDSCL_GPIO , LCDSCL_PIN)
#define LCDSCL_RESET       GPIO_ResetBits(LCDSCL_GPIO , LCDSCL_PIN)
//LCDSID PD5
#define LCDSID_PIN         GPIO_Pin_1//原值为PD5 modfied by LC 2015.12.21 20:38
#define LCDSID_GPIO        GPIOD
#define LCDSID_SET         GPIO_SetBits(LCDSID_GPIO , LCDSID_PIN)
#define LCDSID_RESET       GPIO_ResetBits(LCDSID_GPIO , LCDSID_PIN)

//电机驱动IO定义 
/* 
FRONT_LEFT_F_PIN	PG13	左前前进IO
FRONT_LEFT_B_PIN	PG11	左前后退IO

FRONT_RIGHT_F_PIN	PC11	右前前进IO
FRONT_RIGHT_B_PIN	PD0	    右前后退IO

BEHIND_LEFT_F_PIN	PD6	    左后前进IO
BEHIND_LEFT_B_PIN	PG9	    左后后退IO

BEHIND_RIGHT_F_PIN	PD4	    右后前进IO
BEHIND_RIGHT_B_PIN	PD2	    右后后退IO
 */
#define FRONT_LEFT_F_PIN         GPIO_Pin_13
#define FRONT_LEFT_F_GPIO        GPIOG
#define FRONT_LEFT_F_SET         GPIO_SetBits(FRONT_LEFT_F_GPIO , FRONT_LEFT_F_PIN)
#define FRONT_LEFT_F_RESET       GPIO_ResetBits(FRONT_LEFT_F_GPIO , FRONT_LEFT_F_PIN)

#define FRONT_LEFT_B_PIN         GPIO_Pin_11
#define FRONT_LEFT_B_GPIO        GPIOG
#define FRONT_LEFT_B_SET         GPIO_SetBits(FRONT_LEFT_B_GPIO , FRONT_LEFT_B_PIN)
#define FRONT_LEFT_B_RESET       GPIO_ResetBits(FRONT_LEFT_B_GPIO , FRONT_LEFT_B_PIN)

#define FRONT_RIGHT_F_PIN         GPIO_Pin_11
#define FRONT_RIGHT_F_GPIO        GPIOC
#define FRONT_RIGHT_F_SET         GPIO_SetBits(FRONT_RIGHT_F_GPIO , FRONT_RIGHT_F_PIN)
#define FRONT_RIGHT_F_RESET       GPIO_ResetBits(FRONT_RIGHT_F_GPIO , FRONT_RIGHT_F_PIN)

#define FRONT_RIGHT_B_PIN         GPIO_Pin_0
#define FRONT_RIGHT_B_GPIO        GPIOD
#define FRONT_RIGHT_B_SET         GPIO_SetBits(FRONT_RIGHT_B_GPIO , FRONT_RIGHT_B_PIN)
#define FRONT_RIGHT_B_RESET       GPIO_ResetBits(FRONT_RIGHT_B_GPIO , FRONT_RIGHT_B_PIN)

#define BEHIND_LEFT_F_PIN         GPIO_Pin_6
#define BEHIND_LEFT_F_GPIO        GPIOD
#define BEHIND_LEFT_F_SET         GPIO_SetBits(BEHIND_LEFT_F_GPIO , BEHIND_LEFT_F_PIN)
#define BEHIND_LEFT_F_RESET       GPIO_ResetBits(BEHIND_LEFT_F_GPIO , BEHIND_LEFT_F_PIN)

#define BEHIND_LEFT_B_PIN         GPIO_Pin_9
#define BEHIND_LEFT_B_GPIO        GPIOG
#define BEHIND_LEFT_B_SET         GPIO_SetBits(BEHIND_LEFT_B_GPIO , BEHIND_LEFT_B_PIN)
#define BEHIND_LEFT_B_RESET       GPIO_ResetBits(BEHIND_LEFT_B_GPIO , BEHIND_LEFT_B_PIN)

#define BEHIND_RIGHT_F_PIN         GPIO_Pin_4
#define BEHIND_RIGHT_F_GPIO        GPIOD
#define BEHIND_RIGHT_F_SET         GPIO_SetBits(BEHIND_RIGHT_F_GPIO , BEHIND_RIGHT_F_PIN)
#define BEHIND_RIGHT_F_RESET       GPIO_ResetBits(BEHIND_RIGHT_F_GPIO , BEHIND_RIGHT_F_PIN)

#define BEHIND_RIGHT_B_PIN         GPIO_Pin_2
#define BEHIND_RIGHT_B_GPIO        GPIOD
#define BEHIND_RIGHT_B_SET         GPIO_SetBits(BEHIND_RIGHT_B_GPIO , BEHIND_RIGHT_B_PIN)
#define BEHIND_RIGHT_B_RESET       GPIO_ResetBits(BEHIND_RIGHT_B_GPIO , BEHIND_RIGHT_B_PIN)

//循迹光电对管
/* 
中循迹	SEARCH_M_PIN	PG8
右循迹	SEARCH_R_PIN	PG6
左循迹	SEARCH_L_PIN	PG4
 */
#define SEARCH_M_PIN         GPIO_Pin_8
#define SEARCH_M_GPIO        GPIOG
#define SEARCH_M_IO          GPIO_ReadInputDataBit(SEARCH_M_GPIO, SEARCH_M_PIN)

#define SEARCH_R_PIN         GPIO_Pin_6
#define SEARCH_R_GPIO        GPIOG
#define SEARCH_R_IO          GPIO_ReadInputDataBit(SEARCH_R_GPIO, SEARCH_R_PIN)

#define SEARCH_L_PIN         GPIO_Pin_4
#define SEARCH_L_GPIO        GPIOG
#define SEARCH_L_IO          GPIO_ReadInputDataBit(SEARCH_L_GPIO, SEARCH_L_PIN)
#define BLACK_AREA 1
#define WHITE_AREA 0

//红外避障
/* 
右避障	VOID_R_PIN	PC7
左避障	VOID_L_PIN	PG2
 */
#define VOID_R_PIN         GPIO_Pin_7
#define VOID_R_GPIO        GPIOC
#define VOID_R_IO          GPIO_ReadInputDataBit(VOID_R_GPIO, VOID_R_PIN)

#define VOID_L_PIN         GPIO_Pin_2
#define VOID_L_GPIO        GPIOG
#define VOID_L_IO          GPIO_ReadInputDataBit(VOID_L_GPIO, VOID_L_PIN)
#define BARRIER_Y 0 //有障碍物
#define BARRIER_N 1  //无障碍物

//测速
/* 
速度码盘右	FRONT_RIGHT_S_PIN	PA11
速度码盘左	FRONT_LEFT_S_PIN	PA12
 */
#define FRONT_RIGHT_S_PIN         GPIO_Pin_11
#define FRONT_RIGHT_S_GPIO        GPIOA
#define FRONT_RIGHT_S_IO          GPIO_ReadInputDataBit(FRONT_RIGHT_S_GPIO, FRONT_RIGHT_S_PIN)

#define FRONT_LEFT_S_PIN         GPIO_Pin_12
#define FRONT_LEFT_S_GPIO        GPIOA
#define FRONT_LEFT_S_IO          GPIO_ReadInputDataBit(FRONT_LEFT_S_GPIO, FRONT_LEFT_S_PIN)

//左前
#define FRONT_LEFT_GO    FRONT_LEFT_F_SET; FRONT_LEFT_B_RESET//前进
#define FRONT_LEFT_BACK  FRONT_LEFT_F_RESET; FRONT_LEFT_B_SET//后退
#define FRONT_LEFT_STOP  FRONT_LEFT_F_RESET; FRONT_LEFT_B_RESET//停止

//右前
#define FRONT_RIGHT_GO     FRONT_RIGHT_F_SET;  FRONT_RIGHT_B_RESET
#define FRONT_RIGHT_BACK   FRONT_RIGHT_F_RESET;FRONT_RIGHT_B_SET
#define FRONT_RIGHT_STOP   FRONT_RIGHT_F_RESET;FRONT_RIGHT_B_RESET

//左后
#define BEHIND_LEFT_GO     BEHIND_LEFT_F_SET;BEHIND_LEFT_B_RESET
#define BEHIND_LEFT_BACK   BEHIND_LEFT_F_RESET;BEHIND_LEFT_B_SET
#define BEHIND_LEFT_STOP   BEHIND_LEFT_F_RESET;BEHIND_LEFT_B_RESET

//右后
#define BEHIND_RIGHT_GO    BEHIND_RIGHT_F_SET;BEHIND_RIGHT_B_RESET
#define BEHIND_RIGHT_BACK  BEHIND_RIGHT_F_RESET;BEHIND_RIGHT_B_SET
#define BEHIND_RIGHT_STOP  BEHIND_RIGHT_F_RESET;BEHIND_RIGHT_B_RESET

#define SPEED_DUTY 40//默认占空比 按1ms最小分辨率 周期50ms计算

//指令定义
#define COMM_STOP  'I'//停止
#define COMM_UP    'A'//前进
#define COMM_DOWN  'B'//后退
#define COMM_LEFT  'C'//左转
#define COMM_RIGHT 'D'//右转
#define COMM_S_ADD 'E'//加速
#define COMM_S_SUB 'F'//减速
#define COMM_K_ADD 'G'//k值加
#define COMM_K_SUB 'H'//k值减
#define COMM_B_ADD 'J'//b值加
#define COMM_B_SUB 'K'//b值减

extern unsigned char tick_5ms;//5ms计数器，作为主函数的基本周期
extern unsigned char tick_1ms;//1ms计数器，作为电机的基本计数器
extern unsigned int speed_count;//占空比计数器 50次一周期
extern unsigned char bt_rec_flag;//蓝牙控制标志位
extern char speedduty;
void delay_init(void);
void Delayms(u32 Nms);
void Delay_us(u32 Nus);
void LEDToggle(uint16_t Led);

void IRIN_Configuration(void);
void TIM2_Init(void);
void RedRayInit(void);
void ServoInit(void);
void GPIOCLKInit(void);
void UserLEDInit(void);
void BeepLEDInit(void);

#endif

