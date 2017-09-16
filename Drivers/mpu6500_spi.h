#ifndef __MPU6500_SPI_H
#define __MPU6500_SPI_H

#include "stm32f10x.h"


#define MPU6500_CS(X)			(X==0)?GPIO_ResetBits(GPIOB,GPIO_Pin_0):GPIO_SetBits(GPIOB,GPIO_Pin_0) //MPU6500片选信号

extern void SPI1_Init(void);

extern uint8_t MPU6500_Write_Reg(uint8_t reg,uint8_t value);
extern uint8_t MPU6500_Read_Reg(uint8_t reg);
extern uint8_t SPI1_Read_Write_Byte(uint8_t TxData);//SPI总线读写一个字节


#endif


