#ifndef __MS5611_SPI_H
#define __MS5611_SPI_H

#include "stm32f10x.h"

#define MS5611_CS(X)			(X==0)?GPIO_ResetBits(GPIOA,GPIO_Pin_8):GPIO_SetBits(GPIOA,GPIO_Pin_8) //MS5611片选信号

void SPI2_Init(void);
u8 MS5611_Write_Reg(uint8_t reg,uint8_t value);
u8 MS5611_Read_Reg(uint8_t reg);
u8 SPI2_Read_Write_Byte(uint8_t TxData);//SPI总线读写一个字节
u16  SPI2_Read_Write_2Bytes(unsigned char addr);
long MS5611_SPI_Read_ADC(void);
#endif


