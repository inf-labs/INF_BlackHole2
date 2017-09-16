#ifndef __ILINK_H
#define __ILINK_H
	
#include "stm32f10x.h"

//The length of data frame
#define DATA_FRAME_LENGTH_8 8
#define DATA_FRAME_LENGTH_16 16			
#define DATA_FRAME_LENGTH_24 24			
#define DATA_FRAME_LENGTH_32 32			
#define DATA_FRAME_LENGTH_64 64			
#define DATA_FRAME_LENGTH_128 128	
#define DATA_FRAME_LENGTH_256 256		
#define DATA_FRAME_LENGTH_512 512	
#define DATA_FRAME_LENGTH_1024 1024		

u8 * Version_Data_Frame(void);
u8 * AHRS_Data_Frame(void);
u8 * Location_Data_Frame(void);
u8 * Receiver_Data_Frame(void);
u8 * Motor_Data_Frame(void);
u8 * Rest_Data_Frame(void);
u8 * GPS_Data_Frame(void);
u8 * Custom_Data_Frame(void);

#endif

