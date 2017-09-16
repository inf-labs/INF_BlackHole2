#ifndef __PWM_OUT_H
#define __PWM_OUT_H

#include "stm32f10x.h"

#define MOTOR_PWM_MAX 3800

void PWM_Out_Init(u16 arr, u16 psc,u16	CCR1,u16	CCR2,u16	CCR3,u16	CCR4);
void Motor_PWM_Update(s16 motor1_pwm,s16 motor2_pwm,s16 motor3_pwm,s16 motor4_pwm);

#endif


