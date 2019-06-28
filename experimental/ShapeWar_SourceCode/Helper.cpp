#include"Helper.h"

unsigned int getbitone64(unsigned _int64 value, unsigned int number, int index)
{
	if(value == 1)
		return index ;
	else if(number == 1)
	{
		return -1 ;
	}
	else
	{
		unsigned int result = -1 ;
		unsigned _int64 temp = value ;
		unsigned _int64 half_num = number/2 ;
		int offset = 64 - half_num - index ;
		temp = temp << offset ;
		temp = temp >> offset ;

		result = getbitone64(temp, half_num, index);
		if(result  != -1)
			return result ;

		result = getbitone64(value >> half_num, half_num, index + half_num);
		if(result  != -1)
			return result ; 
	}
}// end for getbitone64