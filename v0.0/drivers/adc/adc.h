/** 
* \file adc.h
* \brief STM32F1xx ADC Driver
* \version 0.1
* \date 14/11/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

#ifndef ADC_H
#define ADC_H

#include "stm32f10x.h"

/** 
* \brief Function activating the clock of the given ADC
* \param ADC_TypeDef => ADC1, ADC2, ...
* \return 1 if done correctly, 0 if not
*/
char ADC_Init_Clock (ADC_TypeDef * ADC);

/** 
* \brief //Do a single conversion ADC on one channel
* \param ADC_TypeDef => ADC1, ADC2, ...
* \param u8 Channel => corresponding channel
* \param u8 n_cycles => n_cycles of conversion (see clock registers for adc clock and cycle time)
* \return float => single conversion result
*/
float Init_ADC_Single_Conv (ADC_TypeDef * ADC, u8 channel, u8 n_cycles);


#endif