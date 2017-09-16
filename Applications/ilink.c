#include "ilink.h"
#include "type.h"

extern volatile S_INT16_XYZ MPU6500_Acc;
extern volatile S_INT16_XYZ MPU6500_Gyro;
extern volatile S_FLOAT_XYZ Angle;  						//Angle of pilot
extern volatile float mpu6500_tempreature;
extern vs32 ultrasonic_height;

volatile S_FLOAT_XYZ Position;    				//Position of pilot
volatile S_FLOAT_XYZ Velocity;    				//Velocity of pilot
volatile S_FLOAT_XYZ Acceleration;    		//Acceleration of pilot

extern u8 rc_id;																//RC ID	
extern u16 Rc_Channel_Val[14];									//RC channel value

extern volatile u16 motor[5];										//motor pwm

extern u32 system_time;													//Unit:ms
extern float battery_voltage;										//Battery voltage
float battery_current;										//Battery current

/*
 * Function Name:Version_Data_Frame
 * Description	:Information of pilot version
 * Input  			:None
 * Output  			:Data frame
 */
u8 * Version_Data_Frame(void)
{
	static u8 data_frame[DATA_FRAME_LENGTH_24];				//Define data frame
	static u16 data_frame_length = sizeof(data_frame);//Get data frame length,define static to avoid everytime bliud sizeof() function
	u8 data_frame_sum = 0x00;
	int i = 0;

	data_frame[0] = 0x55;  //Frame Header
	data_frame[1] = 0x00;	 //Function Mark

	for(i = 0;i < (data_frame_length - 1);i++)
	{
		data_frame_sum+= data_frame[i];//Sum check
  }
	data_frame[data_frame_length - 1] = data_frame_sum;
	
	return data_frame;
}

/*
 * Function Name:AHRS_Data_Frame
 * Description	:Information of ahrs
 * Input  			:None
 * Output  			:Data frame
 */
u8 * AHRS_Data_Frame(void)
{
	static u8 data_frame[DATA_FRAME_LENGTH_32];				//Define data frame
	static u16 data_frame_length = sizeof(data_frame);//Get data frame length,define static to avoid everytime bliud sizeof() function
	u8 data_frame_sum = 0x00;
	int i = 0;

	S_INT16_XYZ Angle_;
  s16 mpu6500_tempreature_;
	
	Angle_.X=(s16)(Angle.X * 100);
	Angle_.Y=(s16)(Angle.Y * 100);
	Angle_.Z=(s16)(Angle.Z * 100);
	mpu6500_tempreature_ = (s16)(mpu6500_tempreature * 100);
	
	data_frame[0] = 0x55;  //Frame Header
	data_frame[1] = 0x01;	 //Function Mark
	
	data_frame[2] = BYTE1(MPU6500_Gyro.X);
	data_frame[3] = BYTE0(MPU6500_Gyro.X);
	data_frame[4] = BYTE1(MPU6500_Gyro.Y);
	data_frame[5] = BYTE0(MPU6500_Gyro.Y);
	data_frame[6] = BYTE1(MPU6500_Gyro.Z);
	data_frame[7] = BYTE0(MPU6500_Gyro.Z);
	
	data_frame[8] = BYTE1(MPU6500_Acc.X);
	data_frame[9] = BYTE0(MPU6500_Acc.X);
	data_frame[10] = BYTE1(MPU6500_Acc.Y);
	data_frame[11] = BYTE0(MPU6500_Acc.Y);
	data_frame[12] = BYTE1(MPU6500_Acc.Z);
  data_frame[13] = BYTE0(MPU6500_Acc.Z);

//	data_frame[14] = BYTE1(Mag_X);
//	data_frame[15] = BYTE0(Mag_X);
//	data_frame[16] = BYTE1(Mag_Y);
//	data_frame[17] = BYTE0(Mag_Y);
//	data_frame[18] = BYTE1(Mag_Z);
//  data_frame[19] = BYTE0(Mag_Z);
	
	data_frame[20] = BYTE1(Angle_.X);
	data_frame[21] = BYTE0(Angle_.X);
	data_frame[22] = BYTE1(Angle_.Y);
	data_frame[23] = BYTE0(Angle_.Y);
	data_frame[24] = BYTE1(Angle_.Z);
  data_frame[25] = BYTE0(Angle_.Z);
	
	data_frame[26] = BYTE1(mpu6500_tempreature_);
	data_frame[27] = BYTE0(mpu6500_tempreature_);
	
	for(i = 0;i < (data_frame_length - 1);i++)
	{
		data_frame_sum+= data_frame[i];//Sum check
  }
	data_frame[data_frame_length - 1] = data_frame_sum;
	
	return data_frame;
}


/*
 * Function Name:Location_Data_Frame
 * Description	:Information of location
 * Input  			:None
 * Output  			:Data frame
 */
u8 * Location_Data_Frame(void)
{
	static u8 data_frame[DATA_FRAME_LENGTH_24];				//Define data frame
	static u16 data_frame_length = sizeof(data_frame);//Get data frame length,define static to avoid everytime bliud sizeof() function
	u8 data_frame_sum = 0x00;
	int i = 0;

	S_INT16_XYZ Acceleration_;
	S_INT16_XYZ Velocity_;
	S_INT16_XYZ Position_;
	
	Acceleration_.X = (s16)(Acceleration.X * 1000);
	Acceleration_.Y = (s16)(Acceleration.Y * 1000);
	Acceleration_.Z = (s16)(Acceleration.Z * 1000);
	
	Velocity_.X = (s16)(Velocity.X * 1000);
	Velocity_.Y = (s16)(Velocity.Y * 1000);
	Velocity_.Z = (s16)(Velocity.Z * 1000);
	
	Position_.X = (s16)(Position.X * 1000);
	Position_.Y = (s16)(Position.Y * 1000);
	Position_.Z = (s16)(ultrasonic_height);//ultrasonic_height unit is mm
	
	data_frame[0] = 0x55;  //Frame Header
	data_frame[1] = 0x02;	 //Function Mark
	
	data_frame[2] = BYTE1(Acceleration_.X);
	data_frame[3] = BYTE0(Acceleration_.X);
	data_frame[4] = BYTE1(Acceleration_.Y);
	data_frame[5] = BYTE0(Acceleration_.Y);
	data_frame[6] = BYTE1(Acceleration_.Z);
	data_frame[7] = BYTE0(Acceleration_.Z);
	
	data_frame[8] = BYTE1(Velocity_.X);
	data_frame[9] = BYTE0(Velocity_.X);
	data_frame[10] = BYTE1(Velocity_.Y);
	data_frame[11] = BYTE0(Velocity_.Y);
	data_frame[12] = BYTE1(Velocity_.Z);
  data_frame[13] = BYTE0(Velocity_.Z);

	data_frame[14] = BYTE1(Position_.X);
	data_frame[15] = BYTE0(Position_.X);
	data_frame[16] = BYTE1(Position_.Y);
	data_frame[17] = BYTE0(Position_.Y);
	data_frame[18] = BYTE1(Position_.Z);
  data_frame[19] = BYTE0(Position_.Z);
	
	for(i = 0;i < (data_frame_length - 1);i++)
	{
		data_frame_sum+= data_frame[i];//Sum check
  }
	data_frame[data_frame_length - 1] = data_frame_sum;
	
	return data_frame;
}


/*
 * Function Name:Receiver_Data_Frame
 * Description	:Information of Receiver
 * Input  			:None
 * Output  			:Data frame
 */
u8 * Receiver_Data_Frame(void)
{
	static u8 data_frame[DATA_FRAME_LENGTH_32];				//Define data frame
	static u16 data_frame_length = sizeof(data_frame);//Get data frame length,define static to avoid everytime bliud sizeof() function
	u8 data_frame_sum = 0x00;
	int i = 0;

	data_frame[0] = 0x55;  //Frame Header
	data_frame[1] = 0x03;	 //Function Mark
	
	data_frame[2] = rc_id; //Only on RC,on pilot is none,default value:0
	
	data_frame[3] = BYTE1(Rc_Channel_Val[0]);
	data_frame[4] = BYTE0(Rc_Channel_Val[0]);
	data_frame[5] = BYTE1(Rc_Channel_Val[1]);
	data_frame[6] = BYTE0(Rc_Channel_Val[1]);
	data_frame[7] = BYTE1(Rc_Channel_Val[2]);
	data_frame[8] = BYTE0(Rc_Channel_Val[2]);
	data_frame[9] = BYTE1(Rc_Channel_Val[3]);
	data_frame[10] = BYTE0(Rc_Channel_Val[3]);
	data_frame[11] = BYTE1(Rc_Channel_Val[4]);
	data_frame[12] = BYTE0(Rc_Channel_Val[4]);	
	data_frame[13] = BYTE1(Rc_Channel_Val[5]);
	data_frame[14] = BYTE0(Rc_Channel_Val[5]);
	data_frame[15] = BYTE1(Rc_Channel_Val[6]);
	data_frame[16] = BYTE0(Rc_Channel_Val[6]);
	data_frame[17] = BYTE1(Rc_Channel_Val[7]);
	data_frame[18] = BYTE0(Rc_Channel_Val[7]);
	data_frame[19] = BYTE1(Rc_Channel_Val[8]);
	data_frame[20] = BYTE0(Rc_Channel_Val[8]);
	data_frame[21] = BYTE1(Rc_Channel_Val[9]);
	data_frame[22] = BYTE0(Rc_Channel_Val[9]);
	data_frame[23] = BYTE1(Rc_Channel_Val[10]);
	data_frame[24] = BYTE0(Rc_Channel_Val[10]);	
	data_frame[25] = BYTE1(Rc_Channel_Val[11]);
	data_frame[26] = BYTE0(Rc_Channel_Val[11]);
	data_frame[27] = BYTE1(Rc_Channel_Val[12]);
	data_frame[28] = BYTE0(Rc_Channel_Val[12]);
	data_frame[29] = BYTE1(Rc_Channel_Val[13]);
	data_frame[30] = BYTE0(Rc_Channel_Val[13]);
	
	for(i = 0;i < (data_frame_length - 1);i++)
	{
		data_frame_sum+= data_frame[i];//Sum check
  }
	data_frame[data_frame_length - 1] = data_frame_sum;
	
	return data_frame;
}

/*
 * Function Name:Motor_Data_Frame
 * Description	:Information of location
 * Input  			:None
 * Output  			:Data frame
 */
u8 * Motor_Data_Frame(void)
{
	static u8 data_frame[DATA_FRAME_LENGTH_24];				//Define data frame
	static u16 data_frame_length = sizeof(data_frame);//Get data frame length,define static to avoid everytime bliud sizeof() function
	u8 data_frame_sum = 0x00;
	int i = 0;

	data_frame[0] = 0x55;  //Frame Header
	data_frame[1] = 0x04;	 //Function Mark
	
	data_frame[2] = BYTE1(motor[1]);
	data_frame[3] = BYTE0(motor[1]);
	data_frame[4] = BYTE1(motor[2]);
	data_frame[5] = BYTE0(motor[2]);
	data_frame[6] = BYTE1(motor[3]);
	data_frame[7] = BYTE0(motor[3]);
	data_frame[8] = BYTE1(motor[4]);
	data_frame[9] = BYTE0(motor[4]);
	
//	data_frame[10] = BYTE1(motor[5]);
//	data_frame[11] = BYTE0(motor[5]);
//	data_frame[12] = BYTE1(motor[6]);
//  data_frame[13] = BYTE0(motor[6]);
//	data_frame[14] = BYTE1(motor[7]);
//	data_frame[15] = BYTE0(motor[7]);
//	data_frame[16] = BYTE1(motor[8]);
//	data_frame[17] = BYTE0(motor[8]);

	for(i = 0;i < (data_frame_length - 1);i++)
	{
		data_frame_sum += data_frame[i];//Sum check
  }
	data_frame[data_frame_length - 1] = data_frame_sum;
	
	return data_frame;
}

/*
 * Function Name:Rest_Data_Frame
 * Description	:Rest information of pilot
 * Input  			:None
 * Output  			:Data frame
 */
u8 * Rest_Data_Frame(void)
{
	static u8 data_frame[DATA_FRAME_LENGTH_24];				//Define data frame
	static u16 data_frame_length = sizeof(data_frame);//Get data frame length,define static to avoid everytime bliud sizeof() function
	u8 data_frame_sum = 0x00;
	int i = 0;
	
  s16 battery_voltage_,battery_current_;

	battery_voltage_ = (s16)(battery_voltage * 1000);
	battery_current_ = (s16)(battery_current * 1000);
	
	data_frame[0] = 0x55;  //Frame Header
	data_frame[1] = 0x05;	 //Function Mark
	
	data_frame[2] = BYTE3(system_time);
	data_frame[3] = BYTE2(system_time);
	data_frame[4] = BYTE1(system_time);
	data_frame[5] = BYTE0(system_time);
	data_frame[6] = BYTE1(battery_voltage_);
	data_frame[7] = BYTE0(battery_voltage_);
	data_frame[8] = BYTE1(battery_current_);
	data_frame[9] = BYTE0(battery_current_);
	
//	data_frame[10] = BYTE1(motor[5]);
//	data_frame[11] = BYTE0(motor[5]);
//	data_frame[12] = BYTE1(motor[6]);
//  data_frame[13] = BYTE0(motor[6]);
//	data_frame[14] = BYTE1(motor[7]);
//	data_frame[15] = BYTE0(motor[7]);
//	data_frame[16] = BYTE1(motor[8]);
//	data_frame[17] = BYTE0(motor[8]);

	for(i = 0;i < (data_frame_length - 1);i++)
	{
		data_frame_sum += data_frame[i];//Sum check
  }
	data_frame[data_frame_length - 1] = data_frame_sum;
	
	return data_frame;
}

/*
 * Function Name:GPS_Data_Frame
 * Description	:Information of GPS
 * Input  			:None
 * Output  			:Data frame
 */
u8 * GPS_Data_Frame(void)
{
	static u8 data_frame[DATA_FRAME_LENGTH_24];				//Define data frame
	static u16 data_frame_length = sizeof(data_frame);//Get data frame length,define static to avoid everytime bliud sizeof() function
	u8 data_frame_sum = 0x00;
	int i = 0;
	
	data_frame[0] = 0x55;  //Frame Header
	data_frame[1] = 0x06;	 //Function Mark
	
	for(i = 0;i < (data_frame_length - 1);i++)
	{
		data_frame_sum += data_frame[i];//Sum check
  }
	data_frame[data_frame_length - 1] = data_frame_sum;
	
	return data_frame;
}

/*
 * Function Name:Custom_Data_Frame
 * Description	:Custom information
 * Input  			:None
 * Output  			:Data frame
 */
u8 * Custom_Data_Frame(void)
{
	static u8 data_frame[DATA_FRAME_LENGTH_24];				//Define data frame
	static u16 data_frame_length = sizeof(data_frame);//Get data frame length,define static to avoid everytime bliud sizeof() function
	u8 data_frame_sum = 0x00;
	int i = 0;
	
	data_frame[0] = 0x55;  //Frame Header
	data_frame[1] = 0x07;	 //Function Mark
	
	
	for(i = 0;i < (data_frame_length - 1);i++)
	{
		data_frame_sum += data_frame[i];//Sum check
  }
	data_frame[data_frame_length - 1] = data_frame_sum;
	
	return data_frame;
}

/****************************************************************************/

//void Device_To_Host_Send_Data(unsigned char *send_buf,int length)
//{
//  int i;
//	for(i = 0;i< length;i++)
//	{
//    USART_Rx_Buffer[USART_Rx_ptr_in] = send_buf[i];
//    USART_Rx_ptr_in++;
//	
//		/* To avoid buffer overflow */
//		if(USART_Rx_ptr_in == USART_RX_DATA_SIZE)
//		{
//			USART_Rx_ptr_in = 0;
//		}
//	}
//}



void Sensor_Calibration_Data_Frame_Unpack(unsigned char *buf,int length)
{
	
}

void PIDA_Data_Frame_Unpack(unsigned char *data_frame)
{
	static u16 data_frame_length = sizeof(data_frame);//Get data frame length,define static to avoid everytime bliud sizeof() function
	u8 data_frame_sum = 0;//校验位
	u8 i = 0;

	if((data_frame[0] == 0x54) && (data_frame[1] == 0x03))//Frame Header & Function Mark check
	{
		for(i = 0;i < (data_frame_length - 1);i++)//最后字节为校验和
		{
			data_frame_sum += data_frame[i];
		}

		if(data_frame_sum == data_frame[data_frame_length - 1])//检验和 检查
		{
//			dis[0] = ((float)Byte16(u16, recieve_buf[2],  recieve_buf[3])) / 100;
//			dis[1] = ((float)Byte16(u16, recieve_buf[4],  recieve_buf[5])) / 100;
//			dis[2] = ((float)Byte16(u16, recieve_buf[6],  recieve_buf[7])) / 100;
//			dis[3] = ((float)Byte16(u16, recieve_buf[8],  recieve_buf[9])) / 100;
		}

	}
}



