/** 
* \file timer.h
* \brief STM32F1xx General Purpose Timer Driver (not for advanced timers)
* \version 0.1
* \date 14/11/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

#ifndef TIMER1234_H
#define TIMER1234_H

#include "stm32f10x.h"
#include "clock.h"

/** 
* \brief //Init the clock of a given general purpose timer (from 2 to 4)
* \param TIM_TypeDef => TIM2, TIM3 ...
* \return 1 if correctly exectuted
*/
char Timer_Init_Clock (TIM_TypeDef * Timer);

/** 
* \brief //Init a generak purpose timer with the given period in us, interrupt or pwm configuration must be done before calling this function
* \param TIM_TypeDef => TIM2, TIM3 ...
* \param float Period_us => The given period in us
* \return effective period in us
*/
float Timer_234_Init(TIM_TypeDef *Timer, float Period_us);


/** 
* \brief //Init interrupt on a given timer, must be called before init the timer
* \param TIM_TypeDef => TIM2, TIM3 ...
* \param Priority => priority of the interrupt function, the lowest priority is executed first (see NVIC->IP for more informations)
* \param IT_function => Pointer on the function to be exectuted when interrupt is triggered, flag is cleared automaticaly at the end of this function
* \return NULL
*/
void Timer_Active_IT (TIM_TypeDef *Timer, u8 Priority, void (*IT_function) (void));

/*Declarations for interrupt function and flag cleared automaticaly*/
void (*Function_TIM2) (void);

void (*Function_TIM3) (void);

void (*Function_TIM4) (void);

#endif
