/*registres de périphériques du stm_32*/
#include "stm32f10x.h"

/*drivers*/
#include "clock.h"
#include "gpio.h"
#include "timer1234.h"
#include "adc.h"
#include "pwm.h"

int main (void)
{
	
	/*Variables definitions*/
	u8 i;
	float result, old_result=0;
	
	/* Clock initialisations */
	CLOCK_Configure();
	
	Port_IO_Init_Clock (GPIOA);
	Port_IO_Init_Clock (GPIOB);
	Port_IO_Init_Clock (GPIOC);
	
	Timer_Init_Clock (TIM2);
	Timer_Init_Clock (TIM3);

	//activation clocks AFIO pour le remapping de la pwm TIM3 CH3 sur PC.8
	(RCC->APB2ENR)|= RCC_APB2ENR_AFIOEN;
	
	/* Source code */
	
	
	//configuration des LED en output (GPIOB 8 à 15)
	for (i=8;i<16;i++){
		Port_IO_Init_Output (GPIOB,i);
	}

	/******** Test driver Timer ************/
	//activation interruption sur timer avec une certaine priorité
	//Timer_Active_IT (TIM2, 5, blink_all);
	
	//activation timer
	//result=Timer_1234_Init(TIM2, 5000000);
	
	/********* Test driver ADC *************/
	
	//Configure PA1 in analog input
	Port_IO_Init_ADC (GPIOA, 1);

	
	/*********Test PWM on PC.8 ->TIM3->CH3*****/		
	Port_IO_Init_AFOD_Output (GPIOC, 8);
	
	//remap on PC.8
	AFIO->MAPR |= 0b11<<10;	
	PWM_Init (TIM3, 3, 1.0); // init pwm at 1kHz
	
	while (1){	

		result=Init_ADC_Single_Conv (ADC1, 1, 4);
		if ((abs_float(result-old_result))>5.0){
			old_result=result;
			PWM_Init_Duty_Cycle (TIM3 , 3, (50+(result*100/4096)));
		}		
	}	
	return 0;
}

