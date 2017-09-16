#include "usart.h"	  

/*
 * 函数名：USART3_Init
 * 描述  ：串口3初始化函数
 * 输入  ：波特率
 * 输出  ：无
 */  	 
void USART3_Init(u32 bound)
{
	//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;																
	GPIO_Init(GPIOB, &GPIO_InitStructure); 																				
  
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;																		 //波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;										 //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;											   //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;														 //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode =  USART_Mode_Tx | USART_Mode_Rx;							 //收发模式

  USART_Init(USART3, &USART_InitStructure); 																		 //初始化串口

  USART_Cmd(USART3, ENABLE);                																		 //使能串口 
}



//*****************************************************
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET); 
    USART_SendData(USART3,(uint8_t)ch);   
	return ch;
}
#endif 
//*****************************************************

