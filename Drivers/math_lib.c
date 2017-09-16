#include "math_lib.h"

static float invSqrt(float number)
{
	volatile long i;
	volatile float x,y;
	volatile const float f = 1.5f;
	x = number * 0.5F;
	y = number;
	i = * (( long * ) &y);
	i = 0x5f375a86 - ( i >> 1 );
	y = * (( float * ) & i);
	y = y * ( f - ( x * y * y ) );
	return y;
}