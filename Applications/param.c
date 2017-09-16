#include "param.h"
#include "eeprom.h"
#include "control.h"
#include "mpu6500.h"

static void EE_MPU6500_GYRO_Offset_Read(void);
static void EE_MPU6500_ACC_Offset_Read(void);

/*
 * 函数名：Param_Init
 * 描述  ：系统参数初始化
 * 输入  ：无
 * 输出  ：无
 */  
void Param_Init(void)
{
	PID_Init();													//PID参数初始化
	//EE_MPU6500_GYRO_Offset_Read();		//陀螺仪原始补偿受环境影响大，每次重新校准
	EE_MPU6500_ACC_Offset_Read();
}


/*
 * 函数名：EE_MPU6500_GYRO_Offset_Write
 * 描述  ：写入陀螺仪原始补偿到模拟EEPROM中
 * 输入  ：无
 * 输出  ：无
 */  
void EE_MPU6500_GYRO_Offset_Write(void)
{	
	EE_WriteVariable(VirtAddVarTab[EE_MPU6500_GYRO_X_OFFSET],MPU6500_Gyro_Offset.X);
	EE_WriteVariable(VirtAddVarTab[EE_MPU6500_GYRO_Y_OFFSET],MPU6500_Gyro_Offset.Y);
	EE_WriteVariable(VirtAddVarTab[EE_MPU6500_GYRO_Z_OFFSET],MPU6500_Gyro_Offset.Z);
}

/*
 * 函数名：EE_MPU6500_ACC_Offset_Write
 * 描述  ：写入加速度计原始补偿到模拟EEPROM中
 * 输入  ：无
 * 输出  ：无
 */
void EE_MPU6500_ACC_Offset_Write(void)
{
	EE_WriteVariable(VirtAddVarTab[EE_MPU6500_ACC_X_OFFSET],MPU6500_Acc_Offset.X);
	EE_WriteVariable(VirtAddVarTab[EE_MPU6500_ACC_Y_OFFSET],MPU6500_Acc_Offset.Y);
	EE_WriteVariable(VirtAddVarTab[EE_MPU6500_ACC_Z_OFFSET],MPU6500_Acc_Offset.Z);
}

/*
 * 函数名：EE_MPU6500_GYRO_Offset_Read
 * 描述  ：从模拟EEPROM中读取陀螺仪原始补偿
 * 输入  ：无
 * 输出  ：无
 */
void EE_MPU6500_GYRO_Offset_Read(void)
{
	s16 temp[3];
	
	EE_ReadVariable(VirtAddVarTab[EE_MPU6500_GYRO_X_OFFSET], &temp[0]);
	EE_ReadVariable(VirtAddVarTab[EE_MPU6500_GYRO_Y_OFFSET], &temp[1]);
	EE_ReadVariable(VirtAddVarTab[EE_MPU6500_GYRO_Z_OFFSET], &temp[2]);
	
	MPU6500_Gyro_Offset.X = temp[0];
	MPU6500_Gyro_Offset.Y = temp[1];
	MPU6500_Gyro_Offset.Z = temp[2];
}

/*
 * 函数名：EE_MPU6500_ACC_Offset_Read
 * 描述  ：从模拟EEPROM中读取加速度计原始补偿
 * 输入  ：无
 * 输出  ：无
 */
void EE_MPU6500_ACC_Offset_Read(void)
{
	s16 temp[3];
	
	EE_ReadVariable(VirtAddVarTab[EE_MPU6500_ACC_X_OFFSET], &temp[0]);
	EE_ReadVariable(VirtAddVarTab[EE_MPU6500_ACC_Y_OFFSET], &temp[1]);
	EE_ReadVariable(VirtAddVarTab[EE_MPU6500_ACC_Z_OFFSET], &temp[2]);
	
	MPU6500_Acc_Offset.X = temp[0];
	MPU6500_Acc_Offset.Y = temp[1];
	MPU6500_Acc_Offset.Z = temp[2];
}
