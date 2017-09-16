#ifndef __BATTERY_CHECK_H
#define __BATTERY_CHECK_H

#include "stm32f10x.h"

#define BATTERY_FILTER_LENGTH 10			//电池滑动滤波次数

#define BATTERY_LOW_VOLTAGE_FLYING 3.2
#define BATTERY_LOW_VOLTAGE_UNFLYING 3.65

void ADC_Battery_Init(void);
u16 ADC_ReadValue(u8 ch);
float Battery_Voltage_ReadValue(void);

#endif


