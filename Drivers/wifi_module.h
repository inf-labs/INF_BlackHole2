#ifndef __WIFI_MODULE_H
#define __WIFI_MODULE_H

#include "stm32f10x.h"

#define WIFI_RST(X)  (X==0)?GPIO_ResetBits(GPIOA,GPIO_Pin_4):GPIO_SetBits(GPIOA,GPIO_Pin_4)//Wifi芯片复位引脚，低电平复位
#define WIFI_CH_PD(X)  (X==0)?GPIO_ResetBits(GPIOC,GPIO_Pin_14):GPIO_SetBits(GPIOC,GPIO_Pin_14)//Wifi芯片使能端，高电平有效

void WIFI_Module_Enable(void);
void WIFI_Module_Init(void);

#endif


