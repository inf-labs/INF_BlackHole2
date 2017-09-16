#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

enum LED_Show_Flag
{
	Wait_Fly_Mode,
	Normal_Fly_Mode,
	Low_Power_Mode,
	Unlock_Mode,
	Lock_Mode,
	Calibrate_Mode
};

void LED_Init(void);

#define LED_LEFT_BACK(X)  (X==0)?GPIO_ResetBits(GPIOC,GPIO_Pin_13):GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define LED_RIGHT_BACK(X)  (X==0)?GPIO_ResetBits(GPIOB,GPIO_Pin_12):GPIO_SetBits(GPIOB,GPIO_Pin_12)

#define  LED_LEFT_BACK_TOGGLE   GPIOC->ODR ^= GPIO_Pin_13;
#define  LED_RIGHT_BACK_TOGGLE  GPIOB->ODR ^= GPIO_Pin_12;

void LED_Init(void);
void LED_StartShow(void);
void LED_CalibrateShow(void);
void LED_Show_Mode(u8 led_show_mode);
#endif



