/** 
* \file timer.c
* \brief STM32F1xx General Purpose Timer Driver (not for advanced timers)
* \version 0.1
* \date 14/11/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

#include "timer234.h"

char Timer_Init_Clock (TIM_TypeDef * Timer){
	char i=1;
	//clock activation for timers 2,4... 7.
	if (Timer==TIM2){
	(RCC->APB1ENR)|= RCC_APB1ENR_TIM2EN;
	}else if (Timer==TIM3){
		(RCC->APB1ENR)|= RCC_APB1ENR_TIM3EN;
	}else if (Timer==TIM4){
		(RCC->APB1ENR)|= RCC_APB1ENR_TIM4EN;
	}else{
		i=0;
	}
	return i;
}


float Timer_234_Init(TIM_TypeDef *Timer, float Duree_us){ 
	//get TIMX_CLK
	u32 tim_freq = CLOCK_GetPCLK1();
	
	//Prescaler initialization => CK_CNT = fCK_PSC / (PSC[15:0] + 1)
	Timer->PSC=0x01;
	
	while (Duree_us > (float)((1000*65535)/(tim_freq/1000))){		//calcul du prescalaire : nécessaire si la période est supérieure à ((2^16)-1)/72MHz=910.21 us
		Timer->PSC *= 2;
		Duree_us /=2;
	}
	Timer->ARR = (unsigned int)((Duree_us*tim_freq)-1);		// ARR*periode=ARR*prescaler/72M=tps_fixee	->  ARR=(tps_fixee(us)*72)/prescaler avec prescaler calculé dans la boucle précédente
	Timer->CR1 = 0x0091;				//down counter auto-reload enable
	
	return Timer->PSC*Timer->ARR/tim_freq;
}

/* Interrupt functions */
void TIM2_IRQHandler(void){
	Function_TIM2();
	//clear flag
	TIM2->SR &= ~0x01;
}

void TIM3_IRQHandler(void){
	Function_TIM3();
	//clear flag
	TIM3->SR &= ~0x01;
}

void TIM4_IRQHandler(void){
	Function_TIM4();
	//clear flag
	TIM4->SR &= ~0x01;
}

void Timer_Active_IT (TIM_TypeDef *Timer, u8 Priority, void (*IT_function) (void)){
	
	//activation de l'interruption au niveau du timer
	Timer->DIER|=0x01;
	
	//activation de l'interruption au niveau du NVIC et choix de la priorité
	if (Timer==TIM2){
			NVIC->ISER[0] |= 0b1<<28;
			NVIC->IP[28] = Priority<<4;
			Function_TIM2=IT_function;
	}else if (Timer==TIM3){		
			NVIC->ISER[0] |= 0b1<<29;
			NVIC->IP[29] = Priority<<4;
			Function_TIM4=IT_function;
	}else if (Timer==TIM4){		
			NVIC->ISER[0] |= 0b1<<30;
			NVIC->IP[30] = Priority<<4;
			Function_TIM3=IT_function;
	}
}
