/** 
* \file functions.h
* \brief STM32F1xx various functions
* \version 0.1
* \date 04/12/2014
* \author Guerric PANIS
* Copyright (c) 2014, All rights reserved.
*
*/

#ifndef FUNCTIONS_H
#define	FUNCTIONS_H

#include "stm32f10x.h"

/** 
* \brief Function convertin a 2's complement to a signed int
* \param unsigned int number => the two complement's number to convert
* \return signed int => the number converted
*/
float two_complement_to_float (u16 number);



#endif