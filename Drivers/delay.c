#include "delay.h"
	 
/*
 * 函数名：Delay_Us
 * 描述  ：微秒粗延时函数
 * 输入  ：time：延时时间，单位：us
 * 输出  ：无
 */  	 
void Delay_Us(u16 us)
{    
   u16 i=0;  
   while(us--)
   {
      i=10;  
      while(i--) ;    
   }
}

/*
 * 函数名：Delay_Ms
 * 描述  ：毫秒延时函数
 * 输入  ：time：延时时间，单位：ms
 * 输出  ：无
 */  
void Delay_Ms(u16 ms)
{    
   u16 i=0;  
   while(ms--)
   {
      i=12000;
      while(i--) ;    
   }
}



































