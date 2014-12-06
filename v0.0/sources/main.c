/** 
* \file main.c
* \brief STM32F1xx Drone project main
* \version 0.1
* \date 14/11/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

/*registres de périphériques du stm_32*/
#include "stm32f10x.h"

/*drivers*/
#include "clock.h"
#include "gpio.h"
#include "timer234.h"
#include "adc.h"
#include "pwm.h"
#include "i2c.h"
#include "motor.h"
#include "sensors.h"
#include "stabilization.h"
#include <stdio.h>




/*Global variables definition*/


float abs_float(float a){
	if (a<0.0){
		a=-a;
	}
	return a;
}


int main (void)
{	
	/*Variables definitions*/
	float result, old_result=0;
	
	
	
	/* Clock initialisations */
	CLOCK_Configure();
	ADC_Init_Clock (ADC1);
	

	//activation clocks AFIO 
	//(RCC->APB2ENR)|= RCC_APB2ENR_AFIOEN;
	
	

	//Configure PA1 in analog input
	Port_IO_Init_Input (GPIOA, 1, ANALOG);	
	
	init_sensors();
	setup_all_motors();
	init_stabilization();
	

	
	while (1){	
		/*Update motor speed*/
		result=Init_ADC_Single_Conv (ADC1, 1, 4);
		if ((abs_float(result-old_result))>5.0){
			old_result=result;

			delta_altitude=result*100.0/4096.0;
			
		}
			
		/*Update angle*/
		
	
	}	
	return 0;
}

