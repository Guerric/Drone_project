/** 
* \file motor.c
* \brief Motors services for drone project
* \version 0.1
* \date 29/11/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

#include "motor.h"


void configure_motor(GPIO_TypeDef* Port, u8 Pin, TIM_TypeDef * Timer, u8 Channel){
	//init clocks
	Port_IO_Init_Clock (Port);
	Timer_Init_Clock (Timer);
	
	//setting output pin as alternate function push pull
	Port_IO_Init_Output (Port, Pin, AF_PUSHPULL, OUT_10MHZ);
	
	//start pwm on the defined channel
	PWM_Init (Timer, Channel, MOTOR_PWM_FREQ);
}



u8 setup_motor (u8 motor){
	u8 flag=1;
	switch (motor){
		case 1:
			configure_motor(MOTOR_1_PORT, MOTOR_1_PIN, MOTOR_1_TIMER, MOTOR_1_CHANNEL);
		break;

		case 2:
			configure_motor(MOTOR_2_PORT, MOTOR_2_PIN, MOTOR_2_TIMER, MOTOR_2_CHANNEL);			
		break;

		case 3:
			configure_motor(MOTOR_3_PORT, MOTOR_3_PIN, MOTOR_3_TIMER, MOTOR_3_CHANNEL);
		break;		
		
		case 4:
			configure_motor(MOTOR_4_PORT, MOTOR_4_PIN, MOTOR_4_TIMER, MOTOR_4_CHANNEL);
		break;
		
		default:
			flag=0;
		break;
	}
	return flag;
}


void setup_all_motors (void){
	configure_motor(MOTOR_1_PORT, MOTOR_1_PIN, MOTOR_1_TIMER, MOTOR_1_CHANNEL);
	configure_motor(MOTOR_2_PORT, MOTOR_2_PIN, MOTOR_2_TIMER, MOTOR_2_CHANNEL);			
	configure_motor(MOTOR_3_PORT, MOTOR_3_PIN, MOTOR_3_TIMER, MOTOR_3_CHANNEL);
	configure_motor(MOTOR_4_PORT, MOTOR_4_PIN, MOTOR_4_TIMER, MOTOR_4_CHANNEL);
}


u8 set_motor_speed (u8 motor, float speed){
	u8 flag=1;
	switch (motor){
		case 1:
			PWM_Init_Duty_Cycle (MOTOR_1_TIMER , MOTOR_1_CHANNEL, speed + OFFSET_SPEED);
		break;

		case 2:
			PWM_Init_Duty_Cycle (MOTOR_2_TIMER , MOTOR_2_CHANNEL, speed + OFFSET_SPEED);		
		break;

		case 3:
			PWM_Init_Duty_Cycle (MOTOR_3_TIMER , MOTOR_3_CHANNEL, speed + OFFSET_SPEED);
		break;		
		
		case 4:
			PWM_Init_Duty_Cycle (MOTOR_4_TIMER , MOTOR_4_CHANNEL, speed + OFFSET_SPEED);
		break;
		
		default:
			flag=0;
		break;
	}
	return flag;
}
