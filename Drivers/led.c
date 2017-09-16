#include "led.h"
#include "delay.h"

/*
 * 函数名：LED_Init
 * 描述  ：LED灯GPIO口初始化
 * 输入  ：无
 * 输出  ：无
 */  	 
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	
	LED_LEFT_BACK(1);
	LED_RIGHT_BACK(1);
}



/*
 * 函数名：LED_StartShow
 * 描述  ：延时与状态显示
 * 输入  ：无
 * 输出  ：无
 */  	 
void LED_StartShow(void)
{
	int i = 0;
	
	for(; i < 5 ; i++)
	{
		LED_LEFT_BACK(1);
		LED_RIGHT_BACK(0);
		Delay_Ms(150);
		LED_LEFT_BACK(0);
		LED_RIGHT_BACK(1);
		Delay_Ms(150);
	}
	
	LED_LEFT_BACK(1);
	LED_RIGHT_BACK(1);
}

void LED_CalibrateShow(void)
{
	int i = 0;
	
	for(; i < 2 ; i++)
	{
		LED_LEFT_BACK(1);
		LED_RIGHT_BACK(1);
		Delay_Ms(100);
		LED_LEFT_BACK(0);
		LED_RIGHT_BACK(0);
		Delay_Ms(100);
		LED_LEFT_BACK(1);
		LED_RIGHT_BACK(1);
		Delay_Ms(100);
		LED_LEFT_BACK(0);
		LED_RIGHT_BACK(0);
		
		Delay_Ms(1000);
	}
	
	LED_LEFT_BACK(1);
	LED_RIGHT_BACK(1);
}

vu8 led_show_mode = Wait_Fly_Mode;

void LED_Show_Mode(u8 led_show_mode)
{
	static u32 led_cnt_10ms;
	
	led_cnt_10ms++;
	
	switch(led_show_mode)
	{
		case Wait_Fly_Mode:
		if(led_cnt_10ms >= 100)
		{
			LED_LEFT_BACK_TOGGLE;
			LED_RIGHT_BACK_TOGGLE;
			
			led_cnt_10ms = 0;
		}
		break;
		
		case Normal_Fly_Mode:
		if(led_cnt_10ms >= 50)
		{
			LED_LEFT_BACK_TOGGLE;
			LED_RIGHT_BACK_TOGGLE;
			
			led_cnt_10ms = 0;
		}
		break;
				
		case Low_Power_Mode:
		if(led_cnt_10ms >= 120)
		{
			LED_LEFT_BACK(0);
			LED_RIGHT_BACK(0);
			led_cnt_10ms = 0;
		}
		else if(((led_cnt_10ms % 4) == 0) && (led_cnt_10ms <= 32))
		{
			LED_LEFT_BACK_TOGGLE;
			LED_RIGHT_BACK_TOGGLE;
		}
		break;
		
		case Unlock_Mode:
		if(led_cnt_10ms >= 2)
		{
			LED_LEFT_BACK_TOGGLE;
			LED_RIGHT_BACK_TOGGLE;
			
			led_cnt_10ms = 0;
		}
		break;
		
		case	Lock_Mode:
		if(led_cnt_10ms >= 2)
		{
			LED_LEFT_BACK_TOGGLE;
			LED_RIGHT_BACK_TOGGLE;
			
			led_cnt_10ms = 0;
		}
		break;
		
		case Calibrate_Mode:
		if(led_cnt_10ms >= 100)
		{
			LED_LEFT_BACK_TOGGLE;
			LED_RIGHT_BACK_TOGGLE;
			
			led_cnt_10ms = 0;
		}
		break;
	}
}
