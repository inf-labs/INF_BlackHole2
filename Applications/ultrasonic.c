#include "ultrasonic.h"
#include "delay.h"
#include "stm32f10x.h"

//Define some global variables about ultrasonic distance measurement

uint8_t USART1_RX_data = 0xff;
float Sonar_height[5];
vs32 History_ultrasonic_height[ULTRASONIC_HEIGHT_FILTER_LENGTH] = {0};
vs32 ultrasonic_height = 0;                            //Unit: mm
vs32 last_ultrasonic_height = 0;
uint8_t strong_Ultrasonic_power_flag = 0;
uint8_t Echo_count = 0;
uint32_t start_time=0,stop_time = 0;
u8 Ultrasonic_timing_ms = 0;

/*
 * Function Name: CW_Pulse_Generate
 * Description  : Generating CW pulse with frequency of 40 kHz
 * Input  	    : dat--->Determine the number of pulses 
                  1:0xff 2:0xfd 3:0xf5 4:0xd5 5:0x55
 * Output       : none
 */
void CW_Pulse_Generate(uint8_t dat)
{
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET); 
	 USART1->DR = (dat & (uint16_t)0x01FF);
}

/*
 * Function Name: Ultrasonic_Measure_Loop
 * Description  : 1ms Loop
 * Input  	    : none
 * Output       : none
 */
void Ultrasonic_Measure_Loop(void)
{

	Ultrasonic_timing_ms++;
	
	if(Ultrasonic_timing_ms == 50)//50ms loop
	{
		Get_Ultrasonic_Height();
		if(strong_Ultrasonic_power_flag == 1)
			CW_Pulse_Generate(0x55);                 //1:0xff 2:0xfd 3:0xf5 4:0xd5 5:0x55
		else
			CW_Pulse_Generate(0xff);
		start_time = SysTick->VAL;
		Echo_count=0;
		Ultrasonic_timing_ms=0;
	}
	else if(Ultrasonic_timing_ms == 40)
	{
		if(strong_Ultrasonic_power_flag == 0)
		{
			if(Echo_count == 0)          //If there is no reflection wave in weak-power mode.
			{
				strong_Ultrasonic_power_flag = 1;
			}
		}
	}

}

/*
 * Function Name: Get_Ultrasonic_Height
 * Description  : Calculate the height value according to the data received by the serial port.
 * Input  	    : none
 * Output       : none
 */
void Get_Ultrasonic_Height(void)
{
	uint8_t i=0;
	if(Echo_count != 0)
	{
		if(strong_Ultrasonic_power_flag == 1)
		{
			float x;
			x=Sonar_height[0] - 0.036;
			if((x < 0.016) && (x > -0.016))    //To determine whether the real echo  (x<0.015) && (x>-0.004)
			{
				x=1-0.0002205/(Sonar_height[1]*Sonar_height[1]);         //add fix factor when the distance is short.
				ultrasonic_height = (vs32)(Sonar_height[1]*x*1000);
			}
			else  
			{ 
				strong_Ultrasonic_power_flag = 0;
				goto ultrasonic_height_filter;
			}
		}
		else
		{
			float x;
			x=1 - 0.0002205 / (Sonar_height[0] * Sonar_height[0]);         //add fix factor when the distance is short.
			ultrasonic_height = (vs32)(Sonar_height[0] * x * 1000);				
		}
		
		//According to the distance to determine the transmission power
		if(ultrasonic_height > 220)
				strong_Ultrasonic_power_flag = 1;
		else if(ultrasonic_height < 180)
				strong_Ultrasonic_power_flag = 0;
		
ultrasonic_height_filter:			//Low pass filter
		for(i = 0;i < ULTRASONIC_HEIGHT_FILTER_LENGTH;i++)
		{
			History_ultrasonic_height[ULTRASONIC_HEIGHT_FILTER_LENGTH - 1 - i] = History_ultrasonic_height[ULTRASONIC_HEIGHT_FILTER_LENGTH - 2 - i];
		}
		History_ultrasonic_height[0] = ultrasonic_height;
		for(i = 1;i<ULTRASONIC_HEIGHT_FILTER_LENGTH;i++)
		{
			ultrasonic_height += History_ultrasonic_height[i];
		}
		ultrasonic_height = ultrasonic_height / ULTRASONIC_HEIGHT_FILTER_LENGTH;	
  }
}

/*
 * Function Name: USART1_IRQHandler
 * Description  : Processing USART1 interrupt event.
 * Input  	    : none
 * Output       : none
 */
void USART1_IRQHandler(void) 
{
	uint32_t time=0;
	static uint32_t last_time = 0;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)   //Receive interrupt
	{	
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
   	USART1_RX_data = USART_ReceiveData(USART1);                            //Read data from USART1
		stop_time = SysTick->VAL;
		time = Ultrasonic_timing_ms * 72000 + start_time - stop_time - 72 * 225;   //Compute time interval		
    if(Echo_count == 0)		
			last_time = 0;
		if((Echo_count == 0) || (time-last_time > 36000))               //Down pulse must be longer than 0.5ms
		{
		  Echo_count++;
		  last_time = time;
			switch(Echo_count)
			{
				case 1:Sonar_height[0] = time * 0.17/72000; break;       //Direct wave distance
				case 2:Sonar_height[1] = time * 0.17/72000; break;       //Primary reflection wave distance
				case 3:Sonar_height[2] = time * 0.17/72000; break;       //Two reflection wave distance
				case 4:Sonar_height[3] = time * 0.17/72000; break;       //Three reflection wave distance
				case 5:Sonar_height[4] = time * 0.17/72000; break;       //Four reflection wave distance
			}		
		}
	}
}
