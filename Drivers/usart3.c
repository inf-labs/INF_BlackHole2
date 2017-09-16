#include "usart3.h"
#include <stdarg.h>
#include "string.h"

u8 send_buf_length_24[SEND_BUF_LENGTH_24]={0};
u8 send_buf_length_32[SEND_BUF_LENGTH_32]={0};

/*
 * 函数名：USART3_Config
 * 描述  ：USART3 GPIO 配置,工作模式配置。115200 8-N-1
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART3_Init(unsigned int baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config USART3 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* USART3 GPIO config */
	/* Configure USART3 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	/* Configure USART3 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	  
	/* USART3 mode config */
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART3, &USART_InitStructure); 
	
  USART_Cmd(USART3, ENABLE);
	
	USART3_DMA_Init();
}

/*
 * 函数名：DMA_Config
 * 描述  ：DMA 串口的初始化配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void USART3_DMA_Init(void)
{
  DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//开启DMA时钟
	
	/* Configure one bit for preemption priority */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* 配置P[A|B|C|D|E]0为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
 	/*设置DMA源：内存地址&串口数据寄存器地址*/
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART3_DR_Base;    
  /*方向：从内存到外设*/		
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	
	/*内存地址(要存放的变量的指针)*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)send_buf_length_32;
	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
	DMA_InitStructure.DMA_BufferSize = SEND_BUF_LENGTH_32;	
	/*外设地址不增*/	    
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 	
	/*内存地址自增*/
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		
	/*外设数据单位*/	
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	
	/*内存数据单位 8bit*/
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 	
	/*DMA模式：一次传输，循环*/
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 	
	/*优先级：中*/	
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  	
	/*禁止内存到内存的传输	*/
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	
	/*配置DMA1的2通道*/		   
  DMA_Init(DMA1_Channel2, &DMA_InitStructure); 	   
	
	DMA_Cmd (DMA1_Channel2,ENABLE);					       //使能DMA
	DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);  //配置DMA发送完成后产生中断

}


void USART3_DMA_Init_24(void)
{
  DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//开启DMA时钟
	
	/* Configure one bit for preemption priority */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* 配置P[A|B|C|D|E]0为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
 	/*设置DMA源：内存地址&串口数据寄存器地址*/
  DMA_InitStructure.DMA_PeripheralBaseAddr = USART3_DR_Base;    
  /*方向：从内存到外设*/		
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	
	/*内存地址(要存放的变量的指针)*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)send_buf_length_24;
	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/	
	DMA_InitStructure.DMA_BufferSize = SEND_BUF_LENGTH_24;	
	/*外设地址不增*/	    
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 	
	/*内存地址自增*/
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;		
	/*外设数据单位*/	
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	
	/*内存数据单位 8bit*/
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 	
	/*DMA模式：一次传输，循环*/
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	 	
	/*优先级：中*/	
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;  	
	/*禁止内存到内存的传输	*/
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	
	/*配置DMA1的2通道*/		   
  DMA_Init(DMA1_Channel2, &DMA_InitStructure); 	   
	
	DMA_Cmd (DMA1_Channel2,ENABLE);					       //使能DMA
	DMA_ITConfig(DMA1_Channel2,DMA_IT_TC,ENABLE);  //配置DMA发送完成后产生中断

}

DMA_InitTypeDef DMA_InitStructure;

u8 send_buf_length;
int SEND_BUF_LENGTH;
/*
 * 函数名：USART3_Data_Send
 * 描述  ：
 * 输入  ：
 * 输出  ：无
 */  
void USART3_Data_Send(unsigned char *send_buf,int length)
{
	//这块需要大大滴优化，无赖之下重新初始化了DMA_Init，优化的朋友可以在
	//无穷论坛发表帖子哦  www.infcre.com/forum.php
	if(length == SEND_BUF_LENGTH_24)
	{
		memcpy(&send_buf_length_24,send_buf,length);
		
		DMA_Cmd(DMA1_Channel2, DISABLE);   //停止DMA
		//DMA_InitStructure.DMA_MemoryBaseAddr = (u32)send_buf_length_24;
		//DMA_InitStructure.DMA_BufferSize = SEND_BUF_LENGTH_24;
		USART3_DMA_Init_24();
	}		
	else if(length == SEND_BUF_LENGTH_32)
	{
		memcpy(&send_buf_length_32,send_buf,length);
		
		DMA_Cmd(DMA1_Channel2, DISABLE);   //停止DMA
		//DMA_InitStructure.DMA_MemoryBaseAddr = (u32)send_buf_length_32;
		//DMA_InitStructure.DMA_BufferSize = SEND_BUF_LENGTH_32;
		USART3_DMA_Init();
	}		
			
	DMA_Cmd(DMA1_Channel2, ENABLE);    //Enable DMA
	USART_DMACmd(USART3, USART_DMAReq_Tx, ENABLE);
}

void DMA1_Channel2_IRQHandler(void)
{	
	//判断是否为DMA发送完成中断   
	if(DMA_GetFlagStatus(DMA1_FLAG_TC2) == SET)    
	{
		DMA_ClearFlag(DMA1_FLAG_TC2);
	}
}

u8 receive_data = 0x00;
void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		receive_data = USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据			 
  } 

} 

/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
	/* 将Printf内容发往串口 */
  	USART_SendData(USART3, (unsigned char) ch);
  	while (!(USART3->SR & USART_FLAG_TXE));
  	
  	return (ch);
}

/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
 */
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

/*
 * 函数名：USART1_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART1_printf( USART1, "\r\n this is a demo \r\n" );
 *            		 USART1_printf( USART1, "\r\n %d \r\n", i );
 *            		 USART1_printf( USART1, "\r\n %s \r\n", j );
 */
void USART3_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
	const char *s;
  	int d;   
  	char buf[16];
  	
  	va_list ap;
  	va_start(ap, Data);

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //字符串
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;

        case 'd':										//十进制
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;
				 default:
						Data++;
				    break;
			}		 
		} /* end of else if */
		else USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}
/******************* (C) COPYRIGHT 2013 CEPARK多功能开发学习板*****END OF FILE****/

