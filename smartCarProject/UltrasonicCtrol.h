#ifndef __ULTRASONICCTROL_H_
#define __ULTRASONICCTROL_H_

extern volatile unsigned int distance_cm;//��ǰ����
extern volatile unsigned int distance_cm_last;//��ǰ����
void UltraSoundInit(void);
void GetDistanceDelay(void);
void Distance(void);
void LCD12864WriteDistance(unsigned int distance);
#endif
