/** 
* \file pwm.h
* \brief STM32F1xx PWM Driver
* \version 0.1
* \date 14/11/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

#ifndef PWM_H
#define PWM_H

#include "stm32f10x.h"

/** 
* \brief Function changing the duty cycle (given in purcent) of a given timer channel
* \param TIM_TypeDef (TIM1, TIM2, ...)
* \param Channel => timer channel
* \param DT_In_Purcent => new duty cycle in purcent
* \return NULL
*/
void PWM_Init_Duty_Cycle (TIM_TypeDef * Timer, u8 Channel, u8 DT_In_Purcent);


/** 
* \brief Function initialisin a PWM, this function initialize the given timer automatically
* \param TIM_TypeDef (TIM1, TIM2, ...)
* \param Channel => timer channel
* \param Frequence_PWM_Khz => pwm frequency (duty cyle is automatically set to 50%)
* \return NULL
*/
void PWM_Init (TIM_TypeDef * Timer, u8 Channel, float Frequence_PWM_Khz);


#endif
