#ifndef TIMER1234_H
#define TIMER1234_H

#include "stm32f10x.h"

//fonction qui permet d'initialiser la clock d'un timer (necessaire � l'usage de tout timer)

void Timer_Init_Clock (TIM_TypeDef * Timer);

/**
*Configure les timers 1, 2, 3 et 4
*@param Timer est le pointeur vers le jeu de registres (de type TIM_TypeDef) du timer consid�r�
*@param Duree_us Intervalle de temps exprim� en us entre deux d�bordements successifs
*@return La dur�e v�ritable qui a �t� configur�e
**/

float Timer_1234_Init(TIM_TypeDef *Timer, float Duree_us);

/**
*Configure la routine d'interruption d'un timer
*@param Timer pointeur vers un jeu de registres (de type TIM_TypeDef) du timer consid�r�
*@param Priority Niveau de priorit� de l'interruption
*@param IT_function Pointeur sur la fonction qui sera ex�cut�e dans la routine de l'interruption
**/

void Timer_Active_IT (TIM_TypeDef *Timer, u8 Priority, void (*IT_function) (void));


void (*Function_TIM2) (void);

void (*Function_TIM3) (void);

void (*Function_TIM4) (void);

#endif
