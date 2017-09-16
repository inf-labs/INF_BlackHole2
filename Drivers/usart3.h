#ifndef __USART3_H
#define	__USART3_H

#include "stm32f10x.h"
#include <stdio.h>

#define USART3_DR_Base  0x40004804 //串口3数据寄存器地址

#define SEND_BUF_LENGTH_8 8
#define SEND_BUF_LENGTH_16 16
#define SEND_BUF_LENGTH_24 24
#define SEND_BUF_LENGTH_32 32
#define SEND_BUF_LENGTH_64 64
#define SEND_BUF_LENGTH_128 128
#define SEND_BUF_LENGTH_256 256
#define SEND_BUF_LENGTH_512 512
#define SEND_BUF_LENGTH_1024 1024

void USART3_Init(unsigned int baud);
int fputc(int ch, FILE *f);
//void USART3_printf(USART_TypeDef* USARTx, uint8_t *Data,...);
void USART3_DMA_Init(void);
void USART3_Data_Send(unsigned char *send_buf,int length);

#endif 



