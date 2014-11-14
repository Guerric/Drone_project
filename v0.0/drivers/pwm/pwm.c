/** 
* \file pwm.c
* \brief STM32F1xx PWM Driver
* \version 0.1
* \date 14/11/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

#include "pwm.h"
#include "timer234.h"


void PWM_Init_Duty_Cycle (TIM_TypeDef * Timer, u8 Channel, u8 DT_In_Purcent){
	
	//récupération de la veleur de l'autoreload du timer
	u16 autoreload=Timer->ARR;		
	
	int value=(DT_In_Purcent*autoreload)/100;
	
	// choix du rapport cyclique
	switch (Channel){
		case 1:
			Timer->CCR1=value;
		break;
		case 2:
			Timer->CCR2=value;
		break;
		case 3:
			Timer->CCR3=value;
		break;
		case 4:
			Timer->CCR4=value;
		break;
	}	
}


void PWM_Init (TIM_TypeDef * Timer, u8 Channel, float Frequence_PWM_Khz){
	
	// timer channel selection
	if (Channel <=2){
		Timer->CCMR1 &= ~(0xFF<<((Channel-1)*8));
		Timer->CCMR1 |= 0b01101000<<((Channel-1)*8);				// not dislable 0 ,mode 1: 110, preload enable & fast dislable 10 , output enable 00
	}else if (Channel <=4){
		Timer->CCMR2 &= ~(0xFF<<((Channel-3)*8));
		Timer->CCMR2 |= 0b01101000<<((Channel-3)*8);				// not dislable 0 ,mode 1: 110, preload enable & fast dislable 10 , output enable 00
	}
	
	//polarity choise
	Timer->CCER |=0b01<<((Channel-1)*4);						//output active and not inverted
	
	// update all registers
	Timer->EGR |= 0x01;													//uptdate generation
	
	//Init timer
	Timer_234_Init(Timer, (1000.0/Frequence_PWM_Khz));	//frequency in kHz and period timer in us
	
	//Init duty cycle at 50%
	PWM_Init_Duty_Cycle (Timer,Channel,50);	
}