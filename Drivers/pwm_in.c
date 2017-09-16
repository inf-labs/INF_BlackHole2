#include "pwm_in.h"

/*
 * 函数名：Pwm_In_Init
 * 描述  ：遥控器输入捕获初始化函数
 * 输入  ：arr：自动重装载寄存器周期的值；psc：时钟频率除数的预分频值
 * 输出  ：无
 */ 
void PWM_In_Init(u16 arr,u16 psc)
{	 
   TIM2_PWM_In_Init(arr,psc);
	 TIM3_PWM_In_Init(arr,psc);
}

/*
 * 函数名：TIM2_PWM_In_Init
 * 描述  ：定时器2输入捕获初始化函数
 * 输入  ：arr：自动重装载寄存器周期的值；psc：时钟频率除数的预分频值
 * 输出  ：无
 */ 
void TIM2_PWM_In_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM2_ICInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);													//使能TIM2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   											//使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;  	//PA0 清除之前设置  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;           											//上升沿捕获，故设置为下拉输入
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr;     																	//设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 																		//预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 											//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  									//TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 															//初始化TIMx的时间基数单位
  
	//初始化TIM2通道1输入捕获参数
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1 ;  													//选择输入端 IC1映射到TI1上
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;									//上升沿捕获
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 							//映射到TI1上
 	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;											  //配置输入分频,不分频 
 	TIM2_ICInitStructure.TIM_ICFilter = 0x00;																			//配置输入滤波器，不滤波
  TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	//初始化TIM2通道2输入捕获参数
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2 ; 														//选择输入端 IC1映射到TI1上
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;									//上升沿捕获
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 							//映射到TI1上
  TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 											//配置输入分频,不分频 
  TIM2_ICInitStructure.TIM_ICFilter = 0x00;																			//配置输入滤波器,不滤波
  TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	//初始化TIM2通道3输入捕获参数
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_3 ; 														//选择输入端 IC1映射到TI1上
  TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;									//上升沿捕获
  TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 							//映射到TI1上
  TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 											//配置输入分频,不分频 
 	TIM2_ICInitStructure.TIM_ICFilter = 0x00;																			//配置输入滤波器，不滤波
 	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	//初始化TIM2通道4输入捕获参数
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_4;															//选择输入端 IC1映射到TI1上
	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;									//上升沿捕获
	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 							//映射到TI1上
	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 											//配置输入分频,不分频 
	TIM2_ICInitStructure.TIM_ICFilter = 0x00;																			//配置输入滤波器，不滤波
	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
		
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);																//中断分组初始化
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn  ;  														//TIM2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  										//先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  													//从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 															//IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  																							//初始化外设NVIC寄存器 

	TIM_ITConfig(TIM2,TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4 ,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
 TIM_Cmd(TIM2,ENABLE ); 																												//使能定时器2
}

/*
 * 函数名：TIM3_PWM_In_Init
 * 描述  ：定时器3输入捕获初始化函数
 * 输入  ：arr：自动重装载寄存器周期的值；psc：时钟频率除数的预分频值
 * 输出  ：无
 */ 
void TIM3_PWM_In_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM3_ICInitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  	//使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);		//使能TIM3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
 	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4 | GPIO_Pin_5;  												
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;           										
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);

	//初始化TIM3	 
	TIM_TimeBaseStructure.TIM_Period = arr;    																		 //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 																		 //预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 											 //设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 									 //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                                //初始化TIMx的时间基数单位
  
	//初始化TIM3通道1输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1 ; 														 //选择输入端 IC1映射到TI1上
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;									 //上升沿捕获
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 							 //映射到TI1上
 	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	                       //配置输入分频,不分频 
 	TIM3_ICInitStructure.TIM_ICFilter = 0x00;																			 //配置输入滤波器,不滤波
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
//初始化TIM3通道2输入捕获参数
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2 ; 														 //选择输入端 IC1映射到TI1上
  TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;									 //上升沿捕获
  TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 							 //映射到TI1上
  TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 											 //配置输入分频,不分频 
  TIM3_ICInitStructure.TIM_ICFilter = 0x00;																			 //配置输入滤波器 不滤波
  TIM_ICInit(TIM3, &TIM3_ICInitStructure);
		
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);																//中断分组初始化
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn  ;  														 //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  										 //先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  												 	 //从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 															 //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  																							 //初始化外设NVIC寄存器 

	TIM_ITConfig(TIM3,TIM_IT_CC1 | TIM_IT_CC2,ENABLE);								 //允许更新中断 ,允许CC1IE捕获中断	
	
 	TIM_Cmd(TIM3,ENABLE ); 																												 //使能定时器3
}
