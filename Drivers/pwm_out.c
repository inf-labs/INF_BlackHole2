#include "pwm_out.h"

/*
 * 函数名：TIM4_Init
 * 描述  ：定时器4的PWM输出初始化函数
 * 输入  ：arr：自动重装载寄存器周期的值；psc：时钟频率除数的预分频值；CCRx：对应四路PWM输出占空比
 * 输出  ：无
 */ 
void PWM_Out_Init(u16 arr, u16 psc,u16	CCR1,u16	CCR2,u16	CCR3,u16	CCR4)//CCR为16位的数
{	 
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
	  
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	  
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_8| GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure); 
			
  	TIM_TimeBaseStructure.TIM_Period = arr;
  	TIM_TimeBaseStructure.TIM_Prescaler = psc;
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
  	//通道1
  	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  	TIM_OCInitStructure.TIM_Pulse = CCR1;
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
  	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
		
		//通道2 	
  	TIM_OCInitStructure.TIM_Pulse = CCR2;
  	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
  	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		
		//通道3
  	TIM_OCInitStructure.TIM_Pulse = CCR3;
  	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
  	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
		
		//通道4
  	TIM_OCInitStructure.TIM_Pulse = CCR4;
  	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
  	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

		TIM_ARRPreloadConfig(TIM4,ENABLE);
		TIM_Cmd(TIM4, ENABLE);		
}




/*
 * 函数名：Motor_PWM_Update
 * 描述  ：电机PWM输出更新函数
 * 输入  ：motorx_pwm：电机PWM输入值
 * 输出  ：无
 */ 
void Motor_PWM_Update(s16 motor1_pwm,s16 motor2_pwm,s16 motor3_pwm,s16 motor4_pwm)
{		
	if(motor1_pwm	>	MOTOR_PWM_MAX)	motor1_pwm = MOTOR_PWM_MAX;
	if(motor2_pwm	>	MOTOR_PWM_MAX)	motor2_pwm = MOTOR_PWM_MAX;
	if(motor3_pwm	>	MOTOR_PWM_MAX)	motor3_pwm = MOTOR_PWM_MAX;
	if(motor4_pwm	>	MOTOR_PWM_MAX)	motor4_pwm = MOTOR_PWM_MAX;
	
	if(motor1_pwm	<	0)	motor1_pwm = 0;
	if(motor2_pwm	<	0)	motor2_pwm = 0;
	if(motor3_pwm	<	0)	motor3_pwm = 0;
	if(motor4_pwm	<	0)	motor4_pwm = 0;
	
	TIM4->CCR2 = motor1_pwm;
	TIM4->CCR3 = motor2_pwm;
	TIM4->CCR4 = motor3_pwm;
	TIM4->CCR1 = motor4_pwm;
}

