/****************************
					 Y(Roll)
     顺时针转 | 逆时针转
      motor1  |  motor4
       正桨   |   反桨
    --------------------X(Pitch)          
     逆时针转 | 顺时针转 
      motor2  |  motor3
       反桨   |   正桨 
              |
****************************/

#include "control.h"
#include "telecontrol.h"

volatile u16 motor[5];																		//定义电机
vs16 throttle = 0;																				//油门值
vu8 fly_state = 0;																				//定义飞行状态

volatile PID PID_Roll_Angle,PID_Pitch_Angle,PID_Yaw_Angle;//外环：角度PID环
volatile PID PID_Roll_Rate,PID_Pitch_Rate,PID_Yaw_Rate;		//内环：角速度PID环
volatile PID PID_Height;																	//高度
volatile S_FLOAT_XYZ Exp_Angle;														//期望角度
volatile S_FLOAT_XYZ MPU6500_Gyro_Last;										//上一次记录的陀螺仪数据

extern u8 unlock_flag;//解锁标志位，为0未解锁，为1解锁	
extern float angle_z;

volatile s32 exp_height;
vu8 exp_height_flag = 1;
extern s32 height;
extern u8 rc_channel_5__status;	//遥控通道五状态，为0拨到下面，为1拨到上面
extern u8 rc_channel_6__status;	//遥控通道六状态，为0拨到下面，为1拨到上面

void Attitude_Inner_Loop_Control(void);
void Attitude_Outter_Loop_Control(void);
void Height_Control(void);

/*
 * 函数名：Attitude_Loop_Control
 * 描述  ：根据姿态信息控制四旋翼
 * 输入  ：Now_Angle：当前姿态；Exp_Angle：期望姿态
 * 输出  ：无
 */ 
void Attitude_Loop_Control(void)
{	
	static uint8_t control_cnt=0;
	control_cnt++;
	
	Attitude_Outter_Loop_Control();
	Attitude_Inner_Loop_Control();
	if(control_cnt == 25)
	{
		control_cnt=0;
	  Height_Control();	
	}
	if((throttle > LAUNCH_THROTTLE) && (unlock_flag) && (Is_WFLY_Connected()))//当油门大于100，解锁了，且遥控器连接正常情况下
	{
		motor[1] = ADD_THROTTLE + throttle - PID_Roll_Rate.Out - PID_Pitch_Rate.Out - PID_Yaw_Rate.Out + PID_Height.Out;
		motor[2] = ADD_THROTTLE + throttle - PID_Roll_Rate.Out + PID_Pitch_Rate.Out + PID_Yaw_Rate.Out + PID_Height.Out;
		motor[3] = ADD_THROTTLE + throttle + PID_Roll_Rate.Out + PID_Pitch_Rate.Out - PID_Yaw_Rate.Out + PID_Height.Out;
		motor[4] = ADD_THROTTLE + throttle + PID_Roll_Rate.Out - PID_Pitch_Rate.Out + PID_Yaw_Rate.Out + PID_Height.Out;		
		fly_state = 1;
	}
	else
	{
		motor[1]  = 0;
		motor[2]  = 0;
		motor[3]  = 0;
		motor[4]  = 0;			
		
		fly_state = 0;
		Exp_Angle.Z = -angle_z;
	}
	
	Motor_PWM_Update(motor[1] ,motor[2] ,motor[3] ,motor[4]);
}

/*
 * 函数名：Attitude_Outter_Loop_Control
 * 描述  ：外环控制函数
 * 输入  ：无
 * 输出  ：无
 */  
void Attitude_Outter_Loop_Control(void)
{
	//期望角+测量角，两者符号相反，实质为偏差
	float roll_angle_err = Exp_Angle.Y + Angle.Y;
	float pitch_angle_err = Exp_Angle.X + Angle.X;
	float yaw_angle_err = Exp_Angle.Z + angle_z;
	
	//yaw轴实际误差修正
	if(yaw_angle_err>180)  yaw_angle_err -=360;
	if(yaw_angle_err<-180) yaw_angle_err +=360;
	
	PID_Roll_Angle.Pout = PID_Roll_Angle.P * roll_angle_err;
	PID_Pitch_Angle.Pout = PID_Pitch_Angle.P * pitch_angle_err;
	PID_Yaw_Angle.Pout = PID_Yaw_Angle.P * yaw_angle_err;		
	
	PID_Pitch_Angle.Dout = PID_Pitch_Angle.D * MPU6500_Gyro.X;	
	PID_Roll_Angle.Dout = PID_Roll_Angle.D * MPU6500_Gyro.Y;
	PID_Yaw_Angle.Dout = PID_Yaw_Angle.D * MPU6500_Gyro.Z;
	
	PID_Roll_Angle.Iout += (PID_Roll_Angle.I * roll_angle_err);
	PID_Pitch_Angle.Iout += (PID_Pitch_Angle.I * pitch_angle_err);
	//积分限幅
	if(PID_Roll_Angle.Iout > 100)	PID_Roll_Angle.Iout = 100;
	else if(PID_Roll_Angle.Iout < -100)	PID_Roll_Angle.Iout = -100;
	if(PID_Pitch_Angle.Iout > 100) PID_Pitch_Angle.Iout = 100;
	else if(PID_Pitch_Angle.Iout < -100) PID_Pitch_Angle.Iout = -100;
	
	//外环输出，作为内环输入，即期望值
	PID_Roll_Angle.Out = PID_Roll_Angle.Pout + PID_Roll_Angle.Iout + PID_Roll_Angle.Dout;
	PID_Pitch_Angle.Out = PID_Pitch_Angle.Pout + PID_Pitch_Angle.Iout + PID_Pitch_Angle.Dout;
	PID_Yaw_Angle.Out = PID_Yaw_Angle.Pout + PID_Yaw_Angle.Iout + PID_Yaw_Angle.Dout;
}


/*
 * 函数名：Attitude_Inner_Loop_Control
 * 描述  ：内环控制函数
 * 输入  ：无
 * 输出  ：无
 */  
void Attitude_Inner_Loop_Control(void)
{ 
	static float roll_rate_err = 0,pitch_rate_err = 0,yaw_rate_err = 0;
	
	roll_rate_err = PID_Roll_Angle.Out + MPU6500_Gyro.Y * 0.0610351f;
	pitch_rate_err = PID_Pitch_Angle.Out + MPU6500_Gyro.X * 0.0610351f;
	yaw_rate_err = PID_Yaw_Angle.Out + MPU6500_Gyro.Z * 0.0610351f;
	
	PID_Roll_Rate.Pout = PID_Roll_Rate.P  * roll_rate_err;
	PID_Pitch_Rate.Pout= PID_Pitch_Rate.P * pitch_rate_err;
	PID_Yaw_Rate.Pout  = PID_Yaw_Rate.P * yaw_rate_err;
	
	PID_Roll_Rate.Dout = PID_Roll_Rate.D * (MPU6500_Gyro.Y - MPU6500_Gyro_Last.Y);//对陀螺仪
	PID_Pitch_Rate.Dout = PID_Pitch_Rate.D * (MPU6500_Gyro.X - MPU6500_Gyro_Last.X);
	PID_Yaw_Rate.Dout = PID_Yaw_Rate.D * (MPU6500_Gyro.Z - MPU6500_Gyro_Last.Z);
	
	PID_Roll_Rate.Iout += (PID_Roll_Rate.I * roll_rate_err);
	PID_Pitch_Rate.Iout += (PID_Pitch_Rate.I * pitch_rate_err);
	
	if(PID_Roll_Rate.Iout > 30) PID_Roll_Rate.Iout = 30;   
	else if(PID_Roll_Rate.Iout < -30) PID_Roll_Rate.Iout = -30;
	if(PID_Pitch_Rate.Iout > 100)	PID_Pitch_Rate.Iout = 30;
	else if(PID_Pitch_Rate.Iout < -30)	PID_Pitch_Rate.Iout = -30;
	
	PID_Roll_Rate.Out = PID_Roll_Rate.Pout + PID_Roll_Rate.Dout + PID_Roll_Rate.Iout;
	PID_Pitch_Rate.Out = PID_Pitch_Rate.Pout + PID_Pitch_Rate.Dout + PID_Pitch_Rate.Iout;
  PID_Yaw_Rate.Out = PID_Yaw_Rate.Pout + PID_Yaw_Rate.Dout;
	
	//存储本次记录的陀螺仪角速率
	MPU6500_Gyro_Last.Y = MPU6500_Gyro.Y;
	MPU6500_Gyro_Last.X = MPU6500_Gyro.X;
	MPU6500_Gyro_Last.Z = MPU6500_Gyro.Z;
}
extern vs32 ultrasonic_height; 
/*
 * 函数名：Height_Control
 * 描述  ：高度控制函数
 * 输入  ：无
 * 输出  ：无
 */  
void Height_Control(void)
{
	static s32 height_err = 0;	                      //height error
	static s32	height_Last = 0;
	if(rc_channel_5__status)	                        //进入定高模式,注意数据更新的频率只有20hz
	{
		height = ultrasonic_height;                     //Get height data from sonar Unit: mm
		
		if(exp_height_flag == 1)
		{
			exp_height = height;	                        //记录高度期望值
			height_Last = height;
			exp_height_flag = 0;
		}		
		height_err = height - exp_height;
		if((height_err > 500) || (height_err < -500)) 
		{
//			height_err = 0;                               //偏差大于500mm，则认为数据错误，系统崩溃，定高模式失效——>大仙崩溃真的好么?
//			PID_Height.Out = 0;														
		}
		else
		{
			PID_Height.Pout = PID_Height.P * height_err;
			PID_Height.Dout = PID_Height.D * (height - height_Last);     
			if((height_err > 10) || (height_err < -10))
				PID_Height.Iout += PID_Height.I * height_err;
			height_Last = height;                                        //存储本次高度值
			PID_Height.Out = -PID_Height.Pout - PID_Height.Iout -PID_Height.Dout;
		}					
	}
	else
	{
	   PID_Height.Out=0;
		 PID_Height.Pout=0;
		 PID_Height.Dout=0;
		 PID_Height.Iout=0;
     exp_height_flag = 1;
	}
}

/****************************
参数整定找最佳，从小到大顺序查
先是比例后积分，最后再把微分加
曲线振荡很频繁，比例度盘要放大
曲线漂浮绕大湾，比例度盘往小扳
曲线偏离回复慢，积分时间往下降
曲线波动周期长，积分时间再加长
曲线振荡频率快，先把微分降下来
动差大来波动慢。微分时间应加长
理想曲线两个波，前高后低4比1
一看二调多分析，调节质量不会低
****************************/

/*
 * 函数名：PID_Init
 * 描述  ：PID参数初始化函数
 * 输入  ：无
 * 输出  ：无
 */ 
void PID_Init(void)
{
//外环：角度环	
	PID_Roll_Angle.P = 8;   //8
	PID_Roll_Angle.I = 0;   //0.001
	PID_Roll_Angle.D = 0;
	
	PID_Pitch_Angle.P = 8;   //8
	PID_Pitch_Angle.I = 0;   //0.001
	PID_Pitch_Angle.D = 0;
	
	PID_Yaw_Angle.P = 10;
	PID_Yaw_Angle.I = 0;
	PID_Yaw_Angle.D = 0;
		
//内环：角速度环	
	PID_Roll_Rate.P = 2.5;       //3.3
	PID_Roll_Rate.I = 0;
	PID_Roll_Rate.D = 1;         //0.2
	
	PID_Pitch_Rate.P = 2.5;      //3.3
	PID_Pitch_Rate.I = 0;
	PID_Pitch_Rate.D = 1;        //0.2
	
	PID_Yaw_Rate.P = 5;
	PID_Yaw_Rate.I = 0;
	PID_Yaw_Rate.D = 0.2;

//高度
  PID_Height.P = 0.6;           //0.5 0.6
	PID_Height.I = 0.005;             //0.005
	PID_Height.D = 0.3*20;

//初始化清零
	PID_Roll_Angle.Pout = 0;
	PID_Roll_Angle.Iout = 0;
	PID_Roll_Angle.Dout = 0;
	
	PID_Pitch_Angle.Pout = 0;
	PID_Pitch_Angle.Iout = 0;
	PID_Pitch_Angle.Dout = 0;
	
	PID_Yaw_Angle.Pout = 0;
	PID_Yaw_Angle.Iout = 0;
	PID_Yaw_Angle.Dout = 0;
	
	PID_Roll_Rate.Pout = 0;
	PID_Roll_Rate.Iout = 0;
	PID_Roll_Rate.Dout = 0;
	
	PID_Pitch_Rate.Pout = 0;
	PID_Pitch_Rate.Iout = 0;
	PID_Pitch_Rate.Dout = 0;
	
	PID_Yaw_Rate.Pout = 0;
	PID_Yaw_Rate.Iout = 0;
	PID_Yaw_Rate.Dout = 0;
	
	PID_Height.Pout = 0;
	PID_Height.Iout = 0;
	PID_Height.Dout = 0;
}
