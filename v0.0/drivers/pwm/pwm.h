#ifndef PWM_H
#define PWM_H

#include "stm32f10x.h"

//fonction qui permet de fixer le rapport cyclique d'une PWM

void PWM_Init_Duty_Cycle (TIM_TypeDef * Timer, u8 Voie, u8 DT_In_Purcent);

//fonction qui permet d'initialiser une PWM sur un timer choisie et une voie donnée à une certaine fréquence

void PWM_Init (TIM_TypeDef * Timer, u8 Voie, float Frequence_PWM_Khz);


#endif
