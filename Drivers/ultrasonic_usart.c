#include "ultrasonic_usart.h"	  
#include "stm32f10x.h"

/*
 * Function Name: Ultrasonic_Module_Init
 * Description  : Ultrasonic ranging module initialize.
 * Input  	    : none
 * Output       : none
 */
void Ultrasonic_Module_Init(void)
{
	//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
//串口1管脚初始化 PA9 TX PA10 RX
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																
	GPIO_Init(GPIOA, &GPIO_InitStructure); 	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;       
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																
//	GPIO_Init(GPIOA, &GPIO_InitStructure); 																				
  
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = 80000;																		 //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;										 //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;											   //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;														 //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode =  USART_Mode_Tx | USART_Mode_Rx;							 //收发模式
	
	//Usart1 NVIC 配置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                              //
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;                       //抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		                         //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                           //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                                               //根据指定的参数初始化VIC寄存器
   
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启接收中断
//	USART_ITConfig(USART1, USART_IT_TXE, ENABLE); //开启发送中断

  USART_Init(USART1, &USART_InitStructure); 																		 //初始化串口

  USART_Cmd(USART1, ENABLE);                																		 //使能串口 
}


