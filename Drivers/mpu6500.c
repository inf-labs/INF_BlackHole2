#include "mpu6500.h"
#include "delay.h"

u8	mpu6500_buf[14];					//spi读取MPU6500后存放数据
u8 offset_flag = 0;						//校准模式标志位，为0未进行校准，为1进行校准

S_INT16_XYZ	MPU6500_Acc_Offset	=	{0,0,0};		
S_INT16_XYZ	MPU6500_Gyro_Offset	=	{0,0,0};	
S_INT16_XYZ MPU6500_Acc = {0,0,0};
S_INT16_XYZ MPU6500_Gyro = {0,0,0};

float	mpu6500_tempreature = 0;
s16 mpu6500_tempreature_temp = 0;
s16 mpu6500_tempreature_Offset = 0;
extern u8 calibrate_status;			//是否执行校准转态标志位
/*
 * 函数名：MPU6500_Date_Offset
 * 描述  ：MPU6500数据校准
 * 输入  ：校准次数
 * 输出  ：无
 */ 
void MPU6500_Date_Offset(u16 cnt)
{
	static S_INT32_XYZ Temp_Gyro , Temp_Acc;
	int i = 0;
	
	Temp_Gyro.X =	0;
	Temp_Gyro.Y =	0;
	Temp_Gyro.Z =	0;
	
	Temp_Acc.X = 0;
	Temp_Acc.Y = 0;
	Temp_Acc.Z = 0;
	
	offset_flag = 1;//进入MPU6500校准模式
	for(i = cnt; i > 0; i--)
	{
    MPU6500_ReadValue();
		
		Temp_Acc.X	+=	MPU6500_Acc.X;
		Temp_Acc.Y	+=	MPU6500_Acc.Y;		
		//Temp_Acc.Z	+=	MPU6500_Acc.Z;
		Temp_Gyro.X	+=	MPU6500_Gyro.X;
		Temp_Gyro.Y	+=	MPU6500_Gyro.Y;
		Temp_Gyro.Z	+=	MPU6500_Gyro.Z;
	}
	
	if(calibrate_status)//加速度数据保存在EEPROM中，无需重新刷新，如果执行手动校准，则重新刷新
	{
		MPU6500_Acc_Offset.X 	=	Temp_Acc.X	/	cnt;
		MPU6500_Acc_Offset.Y 	=	Temp_Acc.Y	/	cnt;
		MPU6500_Acc_Offset.Z  =	Temp_Acc.Z	/	cnt;	
	}
	
	MPU6500_Gyro_Offset.X	= Temp_Gyro.X	/	cnt;
	MPU6500_Gyro_Offset.Y	= Temp_Gyro.Y	/	cnt;
	MPU6500_Gyro_Offset.Z =	Temp_Gyro.Z	/	cnt;

	offset_flag = 0;//退出MPU6500校准模式
}


/*
 * 函数名：MPU6500_ReadValue
 * 描述  ：读取MPU6500原始数据
 * 输入  ：无
 * 输出  ：无
 */ 
void MPU6500_ReadValue(void)
{
	uint8_t i;
	
	MPU6500_CS(0); 																	//使能SPI传输

	SPI1_Read_Write_Byte(ACCEL_XOUT_H|0x80); 				//从加速度计的寄存器开始进行读取陀螺仪和加速度计的值//发送读命令+寄存器号
	
	for(i	=	0;i	<	14;i++)														//一共读取14字节的数据
	{
		mpu6500_buf[i]	=	SPI1_Read_Write_Byte(0xff);	//输入0xff,因为slave不识别
	}	
	if(offset_flag == 0)
	{
		MPU6500_Acc.X = Byte16(s16, mpu6500_buf[0],  mpu6500_buf[1]) - MPU6500_Acc_Offset.X;
		MPU6500_Acc.Y = Byte16(s16, mpu6500_buf[2],  mpu6500_buf[3]) - MPU6500_Acc_Offset.Y;
		MPU6500_Acc.Z = Byte16(s16, mpu6500_buf[4],  mpu6500_buf[5]);
		MPU6500_Gyro.X = Byte16(s16, mpu6500_buf[8],  mpu6500_buf[9]) - MPU6500_Gyro_Offset.X;
		MPU6500_Gyro.Y = Byte16(s16, mpu6500_buf[10],  mpu6500_buf[11]) - MPU6500_Gyro_Offset.Y;
		MPU6500_Gyro.Z = Byte16(s16, mpu6500_buf[12],  mpu6500_buf[13]) - MPU6500_Gyro_Offset.Z;
		
//		mpu6500_tempreature_temp	=	Byte16(s16, mpu6500_buf[6],  mpu6500_buf[7]);
//		mpu6500_tempreature	=	(float)(35000+((521+mpu6500_tempreature_temp)*100)/34); // 原来分母为340，现在分子*100，即：扩大1000倍；
//		mpu6500_tempreature = mpu6500_tempreature/1000;                             
		//if(( -4	<	MPU6500_Gyro.X ) && (MPU6500_Gyro.X < 4) ) MPU6500_Gyro.X = 0;
		//if(( -4	<	MPU6500_Gyro.Y ) && (MPU6500_Gyro.Y < 4) ) MPU6500_Gyro.Y = 0;
		if(( -4	<	MPU6500_Gyro.Z ) && (MPU6500_Gyro.Z < 4) ) MPU6500_Gyro.Z = 0;
	}
	else if(offset_flag)  //MPU6500处于校准模式
	{
		MPU6500_Acc.X = Byte16(s16, mpu6500_buf[0],  mpu6500_buf[1]);
		MPU6500_Acc.Y = Byte16(s16, mpu6500_buf[2],  mpu6500_buf[3]);
		MPU6500_Acc.Z = Byte16(s16, mpu6500_buf[4],  mpu6500_buf[5]);
		MPU6500_Gyro.X = Byte16(s16, mpu6500_buf[8],  mpu6500_buf[9]);
		MPU6500_Gyro.Y = Byte16(s16, mpu6500_buf[10],  mpu6500_buf[11]);
		MPU6500_Gyro.Z = Byte16(s16, mpu6500_buf[12],  mpu6500_buf[13]);
	}
	
	MPU6500_CS(1);  	    //禁止SPI传输
}

/*
 * 函数名：MPU6500_Init
 * 描述  ：MPU6500初始化函数
 * 输入  ：无
 * 输出  ：0：初始化失败 1：初始化成功
 */ 
u8 MPU6500_Init(void)
{
	SPI1_Init();														//MPU6500 IO口和SPI初始化
	
	if(MPU6500_Read_Reg(WHO_AM_I) == 0x70)	//正确读取到6500的地址
	{		
		MPU6500_Write_Reg(PWR_MGMT_1,0X80);   		//电源管理,复位MPU6500
		Delay_Ms(100);
		MPU6500_Write_Reg(SIGNAL_PATH_RESET,0X07);//陀螺仪、加速度计、温度计复位
		Delay_Ms(100);
		MPU6500_Write_Reg(PWR_MGMT_1,0X01);   //选择时钟源
		MPU6500_Write_Reg(PWR_MGMT_2,0X00);   //使能加速度计和陀螺仪
		MPU6500_Write_Reg(CONFIG,0X03);				//低通滤波器 0x06 92hz (3.9ms delay) fs=1khz
		MPU6500_Write_Reg(SMPLRT_DIV,0X00);		//采样率1000/(1+0)=1000HZ
		MPU6500_Write_Reg(GYRO_CONFIG,0X18);  //陀螺仪测量范围 0X18 正负2000度
		MPU6500_Write_Reg(ACCEL_CONFIG,0x10); //加速度计测量范围 0X00 正负8g
		MPU6500_Write_Reg(ACCEL_CONFIG2,0x02);//加速度计速率1khz 滤波器184hz (1.94ms delay)
		return 1;
	}
	else return 0;
}




