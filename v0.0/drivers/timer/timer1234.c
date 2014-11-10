#include "timer1234.h"

/*!!!!!!non testé sur timer 1!!!!!!!!*/

//fonction qui permet d'initialiser la clock d'un timer (necessaire à l'usage de tout timer)

void Timer_Init_Clock (TIM_TypeDef * Timer){
	//activation de la clock pour les timer 2,3 et 4
	if (Timer==TIM2){
	(RCC->APB1ENR)|= RCC_APB1ENR_TIM2EN;
	}else if (Timer==TIM3){
		(RCC->APB1ENR)|= RCC_APB1ENR_TIM3EN;
	}else if (Timer==TIM4){
		(RCC->APB1ENR)|= RCC_APB1ENR_TIM4EN;
	}
}


//Fonction qui configure un timer avec une certaine duree

float Timer_1234_Init(TIM_TypeDef *Timer, float Duree_us){
	
	//clock a 72MHz 
	//initialisation prescaler
	Timer->PSC=0x01;
	
	while (Duree_us > 911){		//calcul du prescalaire : nécessaire si la période est supérieure à ((2^16)-1)/72MHz=910.21 us
		Timer->PSC *= 2;
		Duree_us /=2;
	}
	Timer->ARR = (Duree_us*72)-1;		// ARR*periode=ARR*prescaler/72M=tps_fixee	->  ARR=(tps_fixee(us)*72)/prescaler avec prescaler calculé dans la boucl précédente
	Timer->CR1 = 0x0091;				//down counter auto-reload enable
	
	return Timer->PSC*Timer->ARR/72000000;
}

//Fonctions d'interruption et handlers


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


//Fonction qui configure une interruption sur un timer

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



