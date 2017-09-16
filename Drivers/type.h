#ifndef __TYPE_H
#define __TYPE_H

//取一个数据的地址强制转换成指向char型的，然后取前八位
#define BYTE0(BYTE_TEMP)       ( *( char *)(&BYTE_TEMP))
#define BYTE1(BYTE_TEMP)       ( *( (char *)(&BYTE_TEMP) + 1))
#define BYTE2(BYTE_TEMP)       ( *( (char *)(&BYTE_TEMP) + 2))
#define BYTE3(BYTE_TEMP)       ( *( (char *)(&BYTE_TEMP) + 3))

#define Byte16(Type, ByteH, ByteL)  ((Type)((((u16)(ByteH))<<8) | ((u16)(ByteL))))
	
typedef struct
{
	float X;
	float Y;
	float Z;
}S_FLOAT_XYZ;

typedef struct
{
	signed short int X;
	signed short int Y;
	signed short int Z;
}S_INT16_XYZ;//s16

typedef struct
{
	signed int X;
	signed int Y;
	signed int Z;
}S_INT32_XYZ;//s32



#endif



