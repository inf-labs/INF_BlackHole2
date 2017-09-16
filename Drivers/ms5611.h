#ifndef __MS5611_H
#define __MS5611_H

#include "stm32f10x.h"
#include "delay.h"

#define	MS5611_ADC_READ           ((u8)0x00)  // ADC Read
#define MS5611_RESET              ((u8)0x1E)  // RESET

#define	MS5611_D1_OSR_256         ((u8)0x40)  // 3 bytes
#define	MS5611_D1_OSR_512         ((u8)0x42)  // 3 bytes
#define	MS5611_D1_OSR_1024        ((u8)0x44)  // 3 bytes
#define	MS5611_D1_OSR_2048        ((u8)0x46)  // 3 bytes
#define	MS5611_D1_OSR_4096        ((u8)0x48)  // 3 bytes
#define	MS5611_D2_OSR_256         ((u8)0x50)  // 3 bytes
#define	MS5611_D2_OSR_512         ((u8)0x52)  // 3 bytes
#define	MS5611_D2_OSR_1024        ((u8)0x54)  // 3 bytes
#define	MS5611_D2_OSR_2048        ((u8)0x56)  // 3 bytes
#define	MS5611_D2_OSR_4096        ((u8)0x58)  // 3 bytes

#define MS5611_PROM_COEFF_1       ((u8)0xA2)  // 2 bytes
#define MS5611_PROM_COEFF_2       ((u8)0xA4)  // 2 bytes
#define MS5611_PROM_COEFF_3       ((u8)0xA6)  // 2 bytes
#define MS5611_PROM_COEFF_4       ((u8)0xA8)  // 2 bytes
#define MS5611_PROM_COEFF_5       ((u8)0xAA)  // 2 bytes
#define MS5611_PROM_COEFF_6       ((u8)0xAC)  // 2 bytes
#define MS5611_PROM_CRC           ((u8)0xAE) 


#define MS5611_FILTER_LENGTH 25


typedef struct
{
	u16 C1;//Pressure sensitivity
	u16 C2;//Pressure offset
	u16 C3;//Temperature coefficient of pressure sensitivity
	u16 C4;//Temperature coefficient of pressure offset
	u16 C5;//Reference temperature
	u16 C6;//Temperature coefficient of the temperature
	
	u32 D1;//Digital pressure value
	u32 D2;//Digital temperature value
	
	s32 dT;//Difference between actual and reference temperature
	s32 TEMP;//Actual temperature
	
	double OFF;//Offset at actual temperature
	double SENS;//Sensitivity at actual temperature
	s32 P;//Temperature compensated pressure
	
	s32 TEMP2;
	double OFF2;
	double SENS2;
}MS5611_DATA;

void MS5611_Init(void);
void MS5611_PROM_READ(void);
void MS5611_PressureADC_Conversion(void);
void MS5611_Pressure_Calculate(void);
void MS5611_TemperatureADC_Conversion(void);
void MS5611_Temperature_Calculate(void);
void MS5611_Temp_Compensation(void);
void MS5611_SecondOrder_Temp_Compensation(void);
float MS5611_Altitude_Calculate(void);
void MS5611_Altitude_Offset(u16 cnt);

#endif


