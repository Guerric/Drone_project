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
#include <stdio.h>




/*Global variables definition*/
u32 clock;


float abs_float(float a){
	if (a<0.0){
		a=-a;
	}
	return a;
}


int main (void)
{	
	/*Variables definitions*/
	u8 i;
	float result, old_result=0;
	
	/* Clock initialisations */
	CLOCK_Configure();
	
	/*ADC initialization*/
	ADC_Init_Clock (ADC1);

	//activation clocks AFIO 
	(RCC->APB2ENR)|= RCC_APB2ENR_AFIOEN;
	
	
	/********* Test driver ADC *************/
	clock=CLOCK_GetPCLK1();
	
	//Configure PA1 in analog input
	Port_IO_Init_Input (GPIOA, 1, ANALOG);

	for (i=1;i<=5;i++){	
		setup_motor(i);
	}
	init_sensors_bus();
	
	
	init_MPU6050();
	Port_IO_Init_Output (GPIOB, 8, PUSHPULL, OUT_2MHZ);
	
	/* Init timer for data sensors actualisation */
	Timer_Init_Clock (SENSORS_TIMER);
	Timer_Active_IT (SENSORS_TIMER, SENSORS_IRQ_PRIORITY, update_stabilization_variables);
	Timer_234_Init(SENSORS_TIMER, GYRO_XY_UPDATE_PERIOD);
	
	
	
	while (1){	
		result=Init_ADC_Single_Conv (ADC1, 1, 4);
		if ((abs_float(result-old_result))>5.0){
			old_result=result;
			set_motor_speed (1,result*100/4096);
			set_motor_speed (2,result*100/4096);
			set_motor_speed (3,result*100/4096);
			set_motor_speed (4,result*100/4096);
		}	
	
	}	
	return 0;
}

