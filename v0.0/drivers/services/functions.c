/** 
* \file functions.c
* \brief STM32F1xx various functions
* \version 0.1
* \date 04/12/2014
* \author Guerric PANIS
* Copyright (c) 2014, All rights reserved.
*
*/

#include "functions.h"


float two_complement_to_float (u16 number){
	float result=0.0;
	result=(float)number;
	if ((number & 0x8000)>0){ //if the number is negative
		result=-result;
	}
	return result;
}