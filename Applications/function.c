#include "function.h"
#include "battery_check.h"
#include "led.h"

extern u8 unlock_flag;					//解锁标志位，为0未解锁，为1解锁	
extern vu8 led_show_mode;
extern float battery_voltage;		//电池电压
extern u8 unlock_status;//是否执行解锁状态标志位
extern u8 lock_status;//是否执行上锁状态标志位

//读取LED灯状态
void LED_State_Read(void)
{
	//如果电池电压小于下限，则提示报警
	if(unlock_flag == 1)//解锁状态下
	{
		if((battery_voltage < BATTERY_LOW_VOLTAGE_FLYING))	led_show_mode = Low_Power_Mode;
		else led_show_mode = Normal_Fly_Mode;
	}
	else//未解锁状态下
	{
		if((battery_voltage < BATTERY_LOW_VOLTAGE_UNFLYING))	led_show_mode = Low_Power_Mode;
		else led_show_mode = Wait_Fly_Mode;
	}
	
	if(unlock_status == 1)	led_show_mode = Unlock_Mode;
	if(lock_status == 1)	led_show_mode = Lock_Mode;
}




