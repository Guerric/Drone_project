#ifndef ADC_H
#define ADC_H

#include "stm32f10x.h"

float Init_ADC_Single_Conv (ADC_TypeDef * ADC, u8 channel, u8 n_cycles);

//fonction valeur absolue d'un float
float abs_float(float a);

#endif