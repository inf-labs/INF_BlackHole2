#include "task.h"
#include "led.h"
#include "ahrs.h"
#include "ms5611.h"
#include "control.h"
#include "systick.h"
#include "telecontrol.h"
#include "battery_check.h"
#include "usart.h"
#include "data_interaction.h"
#include "function.h"
#include "ultrasonic.h"

TASK_TIME Task_Time;

extern u8 unlock_flag;					//解锁标志位，为0未解锁，为1解锁	
extern vu8 led_show_mode;
extern u8 rc_channel_5__status;	//遥控通道五状态，为0拨到下面，为1拨到上面
extern u8 rc_channel_6__status;	//遥控通道六状态，为0拨到下面，为1拨到上面
extern volatile S_FLOAT_XYZ Angle;

u8 roll_status = 0;//是否执行翻滚状态标志位

/*
 * Function Name:Task_1Ms
 * Description	:1ms task,user can add his function to this task,but the runtime of function must be considered
 * Input  			:None
 * Output  			:None
 */
void Task_1Ms(void)
{
	Ultrasonic_Measure_Loop();
	Prepare_Data();
	IMU_Update();
	Battery_Voltage_ReadValue();
}

/*
 * Function Name:Task_2Ms
 * Description	:2ms task,user can add his function to this task,but the runtime of function must be considered
 * Input  			:None
 * Output  			:None
 */
void Task_2Ms(void)
{
	if(unlock_flag)//如果解锁了
	{
		WFLY_PWM_Convert();		
		Attitude_Loop_Control();
	}
	else Motor_PWM_Update(0,0,0,0);	//确保安全
	
	Pilot_Status_USB_Data_Send();
}

/*
 * Function Name:Task_5Ms
 * Description	:5ms task,user can add his function to this task,but the runtime of function must be considered
 * Input  			:None
 * Output  			:None
 */
void Task_5Ms(void)
{
	
}

/*
 * Function Name:Task_10Ms
 * Description	:10ms task,user can add his function to this task,but the runtime of function must be considered
 * Input  			:None
 * Output  			:None
 */
void Task_10Ms(void)
{
	if(Is_WFLY_Unlock())//判断是否解锁与是否在执行解锁动作
	{
		Is_WFLY_Lock();		//如果解锁了，则判断是否在执行上锁动作
	}
	LED_State_Read();
	LED_Show_Mode(led_show_mode);
	Pilot_Status_USB_Data_Send();
}

/*
 * Function Name:Task_20Ms
 * Description	:20ms task,user can add his function to this task,but the runtime of function must be considered
 * Input  			:None
 * Output  			:None
 */
void Task_20Ms(void)
{
	
}

/*
 * Function Name:Task_40Ms
 * Description	:40ms task,user can add his function to this task,but the runtime of function must be considered
 * Input  			:None
 * Output  			:None
 */
void Task_40Ms(void)
{

}

/*
 * Function Name:Task_50Ms
 * Description	:50ms task,user can add his function to this task,but the runtime of function must be considered
 * Input  			:None
 * Output  			:None
 */
void Task_50Ms(void)
{
	
}

/*
 * Function Name:Task_150Ms
 * Description	:150ms task,user can add his function to this task,but the runtime of function must be considered
 * Input  			:None
 * Output  			:None
 */
void Task_150Ms(void)
{
	Pilot_Status_USART_Data_Send();
}

/*
 * Function Name:System_Task_Loop
 * Description	:the main loop
 * Input  			:None
 * Output  			:None
 */
void System_Task_Loop(void)
{
	if(Task_Time.Cnt_1Ms >= 1)
	{
		Task_Time.Cnt_1Ms = 0;
		Task_1Ms();
	}
	
	if(Task_Time.Cnt_2Ms >= 2)
	{
		Task_Time.Cnt_2Ms = 0;
		Task_2Ms();
	}

	if(Task_Time.Cnt_5Ms >= 5)
	{
		Task_Time.Cnt_5Ms = 0;
		Task_5Ms();
	}

	if(Task_Time.Cnt_10Ms >= 10)
	{
		Task_Time.Cnt_10Ms = 0;
		Task_10Ms();
	}

	if(Task_Time.Cnt_20Ms >= 20)
	{
		Task_Time.Cnt_20Ms = 0;
		Task_20Ms();
	}	
	
	if(Task_Time.Cnt_40Ms >= 40)
	{
		Task_Time.Cnt_40Ms = 0;
		Task_40Ms();
	}	
	
	if(Task_Time.Cnt_50Ms >= 50)
	{
		Task_Time.Cnt_50Ms = 0;
		Task_50Ms();
	}	
	
	if(Task_Time.Cnt_150Ms >= 150)
	{
		Task_Time.Cnt_150Ms = 0;
		Task_150Ms();
	}	
}	


