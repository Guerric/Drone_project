#include "pwm.h"
#include "timer1234.h"

//fonction qui permet de fixer le rapport cyclique d'une PWM

void PWM_Init_Duty_Cycle (TIM_TypeDef * Timer, u8 Voie, u8 DT_In_Purcent){
	
	//récupération de la veleur de l'autoreload du timer
	u16 autoreload=Timer->ARR;		
	
	int value=(DT_In_Purcent*autoreload)/100;
	
	// choix du rapport cyclique
	switch (Voie){
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

//fonction qui permet d'initialiser une PWM sur un timer choisie et une voie donnée à une certaine fréquence

void PWM_Init (TIM_TypeDef * Timer, u8 Voie, float Frequence_PWM_Khz){
	//initialisation Timer à la fréquence voulue
	
	// choix de la voie du timer
	if (Voie <=2){
		Timer->CCMR1 &= ~(0xFF<<((Voie-1)*8));
		Timer->CCMR1 |= 0b01101000<<((Voie-1)*8);				// not dislable 0 ,mode 1: 110, preload enable & fast dislable 10 , output enable 00
	}else if (Voie<=4){
		Timer->CCMR2 &= ~(0xFF<<((Voie-3)*8));
		Timer->CCMR2 |= 0b01101000<<((Voie-3)*8);				// not dislable 0 ,mode 1: 110, preload enable & fast dislable 10 , output enable 00
	}
	
	//choix de la polarité
	Timer->CCER |=0b01<<((Voie-1)*4);						//output active and not inverted
	
	// update all registers
	Timer->EGR |= 0x01;													//uptdate generation
	
	//Init timer
	Timer_1234_Init(Timer, (1000.0/Frequence_PWM_Khz));	//frequency in kHz and period timer in us
	
	//Init duty cycle at 50%
	PWM_Init_Duty_Cycle (Timer,Voie,50);
	
}