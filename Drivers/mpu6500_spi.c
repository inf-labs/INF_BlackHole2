#include "mpu6500_spi.h"

/*
 * 函数名：SPI1_Init
 * 描述  ：SPI1初始化
 * 输入  ：无
 * 输出  ：无
 */ 
void SPI1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);	//GPIOA时钟使能 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE );												//SPI1时钟使能 	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 																	//MPU6500片选信号
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 											
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);																

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	MPU6500_CS(1); 			 																											//MPU6500片选取消
	GPIO_SetBits(GPIOA,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7);  									//PB13/14/15上拉
	
	SPI_Cmd(SPI1,DISABLE);  																										//SPI1失能
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  				//SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;																//SPI主机
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;														//发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;																	//时钟悬空低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;																//数据捕获于第1个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;																		//NSS信号由软件控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;					//定义波特率预分频的值:波特率预分频值为16（72M/8=9M）
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;													//数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;																		//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure); 																			  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	
	SPI_Cmd(SPI1,ENABLE);																												//使能SPI1
	SPI1_Read_Write_Byte(0xff);																									//启动传输	 
}


/*
 * 函数名：MPU6500_Write_Reg
 * 描述  ：SPI写入寄存器
 * 输入  ：reg:指定的寄存器地址；value：写入的值
 * 输出  ：status：返回状态值
 */ 
uint8_t MPU6500_Write_Reg(uint8_t reg,uint8_t value)
{
	uint8_t status;
	MPU6500_CS(0);  										//使能SPI传输
	status = SPI1_Read_Write_Byte(reg); //发送写命令+寄存器号
	SPI1_Read_Write_Byte(value);				//写入寄存器值
	MPU6500_CS(1);  										//禁止MPU9500
	return(status);											//返回状态值
}


/*
 * 函数名：MPU6500_Read_Reg
 * 描述  ：SPI读取寄存器
 * 输入  ：reg:指定的寄存器地址
 * 输出  ：reg_val：reg寄存器地址对应的值
 */ 
uint8_t MPU6500_Read_Reg(u8 reg)
{
	uint8_t reg_val;
	MPU6500_CS(0);  										//使能SPI传输
	SPI1_Read_Write_Byte(reg | 0x80); 	//发送读命令+寄存器号
	reg_val=SPI1_Read_Write_Byte(0xff); //读取寄存器值
	MPU6500_CS(1);  									  //禁止MPU9250
	return(reg_val);
}

/*
 * 函数名：SPI1_Read_Write_Byte
 * 描述  ：读写一个字节
 * 输入  ：TxData:要写入的字节
 * 输出  ：读取到的字节
 */ 
uint8_t SPI1_Read_Write_Byte(uint8_t TxData)
{		
	uint8_t retry = 0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) 	//检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry > 200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); 																//通过外设SPIx发送一个数据
	retry = 0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200) return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI1); 															//返回通过SPIx最近接收的数据					    
}

