#include "battery_check.h"
#include "delay.h"

u16 adc_value;															//ADC采样值
u16 adc_avg;																//ADC采样平均值
float battery_voltage;											//电池电压
u16 Adc_Value[BATTERY_FILTER_LENGTH] = {0};	//存储Adc数值队列

/*
 * 函数名：ADC_Battery_Init
 * 描述  ：电池电压ADC检测初始化函数
 * 输入  ：无
 * 输出  ：无
 */ 		 
void  ADC_Battery_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_ADC1,ENABLE);	  		//使能ADC1通道时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   																						//设置ADC分频因子6 72M/6=12,ADC最大时间超过14M则电压采集不准确

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  																															//复位ADC1

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;															//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;																		//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;															//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;							//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;													//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;																					//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);																							//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

	ADC_Cmd(ADC1, ENABLE);																													//使能ADC1
	
	ADC_ResetCalibration(ADC1);																											//使能复位校准  
	while(ADC_GetResetCalibrationStatus(ADC1));																			//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 																										//开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	 																				//等待校准结束
}				  

/*
 * 函数名：ADC_ReadValue
 * 描述  ：ADC原始数据读取
 * 输入  ：ADC通道
 * 输出  ：无
 */
u16 ADC_ReadValue(u8 ch)   
{																																									//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );							//ADC1,ADC通道,采样时间为239.5周期	  			      
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);																					//使能指定的ADC1的软件转换启动功能		 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));																	//等待转换结束
	return ADC_GetConversionValue(ADC1);																						//返回最近一次ADC1规则组的转换结果
}

/*
 * 函数名：Battery_Voltage_ReadValue
 * 描述  ：滤波后的电池电压，函数运行时间大概为31us
 * 输入  ：ADC通道
 * 输出  ：无
 */
float Battery_Voltage_ReadValue(void) 
{
	static u8 filter_cnt	=	0;
	u8 cnt = 0;
	u32 temp = 0;
	
	adc_value = ADC_ReadValue(ADC_Channel_9);
	Adc_Value[filter_cnt] = adc_value;
	
	filter_cnt++;
	
	for(cnt	=	0;cnt <	BATTERY_FILTER_LENGTH;cnt++)
	{
		temp += Adc_Value[cnt];
	}
	adc_avg = temp / BATTERY_FILTER_LENGTH;
	
	battery_voltage = ((float)adc_avg * 6.6f) / 4096.0f;
	if(filter_cnt	==	BATTERY_FILTER_LENGTH)	filter_cnt = 0;
	return battery_voltage;
}

