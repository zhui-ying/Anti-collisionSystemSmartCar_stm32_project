#ifndef __SPEED_H_
#define __SPEED_H_

extern __IO unsigned int speed_contiue;
extern __IO u8 test_flag;

void WheelCaptureIRQ(void);
void MeaSpeedInit(void);
unsigned int GetLeftSpeed(void);
unsigned int GetRightSpeed(void);
void ClearSpeedBuf(void);

#endif
