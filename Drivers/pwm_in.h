#ifndef __PWM_IN_H
#define __PWM_IN_H

#include "stm32f10x.h"

void PWM_In_Init(u16 arr,u16 psc);
void TIM2_PWM_In_Init(u16 arr,u16 psc);
void TIM3_PWM_In_Init(u16 arr,u16 psc);

#endif


