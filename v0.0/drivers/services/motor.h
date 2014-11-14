#ifndef MOTOR_H
#define MOTOR_H

#include "gpio.h"
#include "timer234.h"
#include "pwm.h"

/*PWM Frequency for motors command (in kHz) */
#define MOTOR_PWM_FREQ 1.0 			// 1kHz

/*Offset speed : limit before motor starts*/
#define OFFSET_SPEED 50

/*Channel selection for each motor*/
//do not forget t do the remaping manually if necessary

#define MOTOR_1_PORT GPIOB
#define MOTOR_1_PIN	0
#define MOTOR_1_TIMER TIM3
#define MOTOR_1_CHANNEL 3

#define MOTOR_2_PORT GPIOA
#define MOTOR_2_PIN	2
#define MOTOR_2_TIMER TIM2
#define MOTOR_2_CHANNEL 3

#define MOTOR_3_PORT GPIOB
#define MOTOR_3_PIN	1
#define MOTOR_3_TIMER TIM3
#define MOTOR_3_CHANNEL 4

#define MOTOR_4_PORT GPIOA
#define MOTOR_4_PIN	3
#define MOTOR_4_TIMER TIM2
#define MOTOR_4_CHANNEL 4

/*Configure output pin and timer for a motor*/

void configure_motor(GPIO_TypeDef* Port, u8 Pin, TIM_TypeDef * Timer, u8 Channel);

/*Setting up a motor*/

u8 setup_motor (u8 motor);

/*Change the speed of a motor (0 to 100)*/

u8 set_motor_speed (u8 motor, float speed); 

#endif
