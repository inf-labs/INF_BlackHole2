#include "telecontrol.h"
#include "ahrs.h"
#include "control.h"
#include "mpu6500.h"
#include "led.h"
#include "delay.h"
#include "param.h"

u8 unlock_flag = 0 ;					//解锁标志位，为0未解锁，为1解锁	
u8 calibrate_status = 0;			//是否执行校准转态标志位
extern vs16 throttle;
extern S_FLOAT_XYZ Exp_Angle;
extern u16 Rc_Channel_Val[13];	

u8 rc_id = 0 ;								//遥控ID	
u8 rc_channel_5__status = 0;	//遥控通道五状态，为0拨到下面，为1拨到上面
u8 rc_channel_6__status = 0;	//遥控通道六状态，为0拨到下面，为1拨到上面

extern vu8 led_show_mode;

u8 unlock_status = 0;//是否执行解锁状态标志位
u8 lock_status = 0;//是否执行上锁状态标志位

/*
 * 函数名：Is_WFLY_Unlock
 * 描述  ：判断是否执行解锁动作
 * 输入  ：无
 * 输出  ：0：未解锁；1：解锁成功
 */  
u8 Is_WFLY_Unlock(void)
{
	static u16	cnt_unlock = 0;
	
	Is_RC_Calibrate();
	
	if( !unlock_flag )	//如果未解锁
	{
		if((Rc_Channel_Val[3]	<	1300) && (Rc_Channel_Val[2] < 1200) )			//左摇杆打到右下角
		{	
			if(((Rc_Channel_Val[1]	<	1300) && (Rc_Channel_Val[0]) > 1700) )//右摇杆打到左下角
			{	
				unlock_status = 1;//标记执行正在执行解锁动作
				cnt_unlock++;
			}
		}
		else 
		{
			unlock_status = 0;//标记没有执行解锁动作
			cnt_unlock = 0;		//如果未解锁或解锁中断，则解锁时间计数清零
		}
			
		if(cnt_unlock > 100)												 	 //解锁动作持续1秒，则解锁
			{
				cnt_unlock = 0;
				SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;//关闭中断
				AHRS_Date_Init(); 										 	 	 //每次解锁后都先初始化导航数据
				MPU6500_Date_Offset(5000);								 //校准陀螺仪
				SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk; //开启中断
				unlock_status = 0;												 //标记执行没有执行解锁动作
				unlock_flag = 1;													 //成功解锁标志
				
				return 1;
			}	
		 else return 0;
	}
	else return 1;//如果解锁了，则返回1
}

/*
 * 函数名：Is_WFLY_Unlock
 * 描述  ：判断是否执行上锁动作
 * 输入  ：无
 * 输出  ：0：上锁成功；1：未上锁
 */  
u8 Is_WFLY_Lock(void)
{
	static u16	cnt_lock = 0;
	if( unlock_flag )	//如果解锁了
	{
		if((Rc_Channel_Val[3]	> 1700) && (Rc_Channel_Val[2] < 1200) )//左摇杆打到左下角
		{		
				lock_status = 1;//标记执行上锁动作
				cnt_lock++;
		}
		else
		{
			lock_status = 0;//标记没有执行上锁动作
			cnt_lock = 0;		//如果未解锁或解锁中断，则解锁时间计数清零
		}			
			
		if( cnt_lock == 50)	//解锁动作持续500ms，则解锁
			{
				cnt_lock = 0;
				lock_status = 0;//标记没有执行上锁动作
				unlock_flag = 0;//成功上锁标志
				return 1;
			}	
		 else return 0;		
	}
	else return 1;					//如果解锁了，则返回1
}

/*
 * 函数名：Is_WFLY_Connected
 * 描述  ：判断遥控器是否正常连接
 * 输入  ：无
 * 输出  ：0：连接失败；1：正常连接
 */  
u8 Is_WFLY_Connected(void)
{
	if((Rc_Channel_Val[0]	<	1100) && (Rc_Channel_Val[1] > 1900) && (Rc_Channel_Val[2]	>	1900) && (Rc_Channel_Val[3] > 1900))
  return 0;
	else return 1;
}

/*
 * 函数名：Is_RC_Calibrate
 * 描述  ：判断是否执行校准动作，右摇杆左右打
 * 输入  ：无
 * 输出  ：0：未解锁；1：解锁成功
 */  
u8 Is_RC_Calibrate(void)
{
	static u8 left_cnt = 0,right_cnt = 0;//定义右摇杆打左打右计数器
	
	if( !unlock_flag )	//如果未解锁才执行校准检测
	{
		if((Rc_Channel_Val[0] > 1750) && (left_cnt == right_cnt))//右摇杆打到左边
		{
			left_cnt++;
		}
		if((Rc_Channel_Val[0] < 1250)  && (left_cnt > right_cnt))//右摇杆打到右边	
		{
			right_cnt++;
		}
		
		if((left_cnt == 3) && (right_cnt == 3))
		{
				SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;//关闭中断
				calibrate_status = 1;
			
				left_cnt = 0;
				right_cnt = 0;
			
				AHRS_Date_Init(); 										 	 	 //每次解锁后都先初始化导航数据
				LED_CalibrateShow();
				MPU6500_Date_Offset(5000);								 //校准MPU6500
				LED_CalibrateShow();
				EE_MPU6500_GYRO_Offset_Write();						 //写入陀螺仪补偿
				EE_MPU6500_ACC_Offset_Write();						 //写入加速度计补偿
				Param_Init();															 //参数初始化
				calibrate_status = 0;
				SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk; //开启中断
			return 1;
		}
		else return 0;
	}
	else return 0;	
	
}




float angle_z;
/*
 * 函数名：WFLY_PWM_Convert
 * 描述  ：遥控器信号转换
 * 输入  ：无
 * 输出  ：无
 */ 
void WFLY_PWM_Convert(void)
{
	static u16 cnt;
	
	cnt++;
	
	Exp_Angle.X = (Rc_Channel_Val[1]- 1500) * 0.05f;
	if((Exp_Angle.X > -3) && (Exp_Angle.X <3))	Exp_Angle.X = 0;
	Exp_Angle.Y = (Rc_Channel_Val[0]- 1500) * 0.05f;
	if((Exp_Angle.Y > -3) && (Exp_Angle.Y < 3))	Exp_Angle.Y = 0;
	
  throttle	=	(vs16)(Rc_Channel_Val[2] - 1000) * 3;
	if(cnt == 20)
	{
		cnt = 0;	
		
    if((Rc_Channel_Val[3] > 1600) || (Rc_Channel_Val[3] < 1400))//防止死区累计误差
		{
			if(throttle > LAUNCH_THROTTLE)	Exp_Angle.Z += -(Rc_Channel_Val[3]- 1500) * 0.010f;
		}		
		
		if(Exp_Angle.Z > 180)
		{
			Exp_Angle.Z += -360;
		}
		else if(Exp_Angle.Z < -180)
		{
			Exp_Angle.Z += 360;
		}		
		angle_z = Angle.Z;
	}
	
	//通道5
	if(Rc_Channel_Val[4] > 1500) rc_channel_5__status = 1;
	else rc_channel_5__status = 0;
	//通道6
	if(Rc_Channel_Val[5] > 1500) rc_channel_6__status = 1;
	else rc_channel_6__status = 0;
}

