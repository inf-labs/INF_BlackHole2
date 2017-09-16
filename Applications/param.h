#ifndef __PARAM_H
#define __PARAM_H

#include "stm32f10x.h"

//定义好的Flash虚拟地址，为兼容以后版本，不建议改动

#define EE_INIT_STATUS 0
#define EE_MPU6500_GYRO_X_OFFSET 1
#define EE_MPU6500_GYRO_Y_OFFSET 2
#define EE_MPU6500_GYRO_Z_OFFSET 3
#define EE_MPU6500_ACC_X_OFFSET 4
#define EE_MPU6500_ACC_Y_OFFSET 5
#define EE_MPU6500_ACC_Z_OFFSET 6
#define EE_MPU9250_MAG_X_OFFSET 7
#define EE_MPU9250_MAG_Y_OFFSET 8
#define EE_MPU9250_MAG_Z_OFFSET 9

//用户自定义
#define EE_STATUS1 10
#define EE_STATUS2 11
#define EE_STATUS3 12
#define EE_STATUS4 13
#define EE_STATUS5 14
#define EE_STATUS6 15
#define EE_STATUS7 16
#define EE_STATUS8 17
#define EE_STATUS9 18
#define EE_STATUS10 19

extern void Param_Init(void);
extern void EE_MPU6500_GYRO_Offset_Write(void);
extern void EE_MPU6500_ACC_Offset_Write(void);

#endif

