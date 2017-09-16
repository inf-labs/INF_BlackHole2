#include "wifi_module.h"
#include "usart3.h"

void WIFI_Module_Init(void)
{
	USART3_Init(115200);								//´®¿Ú³õÊ¼»¯   115200
	WIFI_Module_Enable();
}

void WIFI_Module_Enable(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	WIFI_RST(1);
	WIFI_CH_PD(1);
}

void WIFI_Module_Disable(void)
{
	WIFI_RST(0);
	WIFI_CH_PD(0);
}

