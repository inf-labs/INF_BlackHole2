#include "data_interaction.h"
#include "ilink.h"
#include "usart3.h"
#include "mpu6500.h"
#include 	"ahrs.h"


void Pilot_Status_USART_Data_Send(void)
{
	static u8 cnt = 0;
//	if(cnt == 1)	USART3_Data_Send(Version_Data_Frame(),24);
	
	if(cnt == 0)	USART3_Data_Send(Version_Data_Frame(),24);
	else if(cnt == 1)	USART3_Data_Send(AHRS_Data_Frame(),32);
	else if(cnt == 2)	USART3_Data_Send(Location_Data_Frame(),24);
	else if(cnt == 3)	USART3_Data_Send(Receiver_Data_Frame(),32);
	else if(cnt == 4)	USART3_Data_Send(Motor_Data_Frame(),24);
	else if(cnt == 5)	USART3_Data_Send(Rest_Data_Frame(),24);
	
//	else if(cnt == 6)	USART3_Data_Send(GPS_Data_Frame(),24);
//	else if(cnt == 7)	USART3_Data_Send(Custom_Data_Frame(),24);
	
	cnt++;
	if(cnt >= 5) cnt = 1;//Version_Data_Frame send once time
}


#include "hw_config.h"

void Pilot_Status_USB_Data_Send(void)
{
	static u8 cnt = 0;
	
	if(cnt == 0)	Device_To_Host_Send_Data(Version_Data_Frame(),24);
	else if(cnt == 1)	Device_To_Host_Send_Data(AHRS_Data_Frame(),32);
	else if(cnt == 2)	Device_To_Host_Send_Data(Location_Data_Frame(),24);
	else if(cnt == 3)	Device_To_Host_Send_Data(Receiver_Data_Frame(),32);
	else if(cnt == 4)	Device_To_Host_Send_Data(Motor_Data_Frame(),24);
	else if(cnt == 5)	Device_To_Host_Send_Data(Rest_Data_Frame(),24);
//	else if(cnt == 6)	Device_To_Host_Send_Data(GPS_Data_Frame(),24);
//	else if(cnt == 7)	Device_To_Host_Send_Data(Custom_Data_Frame(),24);
	
	cnt++;
	if(cnt >= 5) cnt = 1;//Version_Data_Frame send once time
}


void Data_Transmission_Mode(void)
{
	//Device_To_Host_Send_Data(out_date,24);
	
	//	for(i = 0;i < OUT_DATA_LENGTH;i++)
//	{
//		USART_SendData(USART3, out_date[i]);
//		while( USART_GetFlagStatus(USART3,USART_FLAG_TC)!= SET);	
//	}
}



