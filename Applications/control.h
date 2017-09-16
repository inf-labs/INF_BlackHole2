#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "stm32f10x.h"
#include "mpu6500.h"
#include "pwm_out.h"
#include "ahrs.h"

#define LAUNCH_THROTTLE	400 			//∆∑…”Õ√≈
#define ADD_THROTTLE		600 			//≤π≥‰”Õ√≈

typedef struct PID{float P,Pout,I,Iout,D,Dout,I_Max,Out;}PID;

void Attitude_Loop_Control(void);
void PID_Init(void);

#endif
