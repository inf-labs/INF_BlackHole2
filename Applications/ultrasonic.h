#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

#include "ultrasonic_usart.h"	
#include "stm32f10x.h"

#define ULTRASONIC_HEIGHT_FILTER_LENGTH   5

void CW_Pulse_Generate(uint8_t dat);
void Get_Ultrasonic_Height(void);
void Ultrasonic_Measure_Loop(void);
void Ultrasonic_Power_Check(void);

#endif
