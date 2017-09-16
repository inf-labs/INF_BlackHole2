#ifndef __TASK_H
#define __TASK_H

#include "stm32f10x.h"

typedef struct 
{
	u16 Cnt_1Ms;
	u16 Cnt_2Ms;
	u16 Cnt_5Ms;
	u16 Cnt_10Ms;
	u16 Cnt_20Ms;
	u16 Cnt_40Ms;
	u16 Cnt_50Ms;
	u16 Cnt_150Ms;
} TASK_TIME;

void System_Task_Loop(void);

#endif


