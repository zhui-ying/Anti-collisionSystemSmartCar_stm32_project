#ifndef __ULTRASONICCTROL_H_
#define __ULTRASONICCTROL_H_

extern volatile unsigned int distance_cm;//当前距离
extern volatile unsigned int distance_cm_last;//当前距离
void UltraSoundInit(void);
void GetDistanceDelay(void);
void Distance(void);
void LCD12864WriteDistance(unsigned int distance);
#endif
