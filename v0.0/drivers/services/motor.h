/** 
* \file motor.h
* \brief Motors services for drone project
* \version 0.1
* \date 29/11/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

#ifndef MOTOR_H
#define MOTOR_H

#include "gpio.h"
#include "timer234.h"
#include "pwm.h"

/*PWM Frequency for motors command (in kHz) */
#define MOTOR_PWM_FREQ 1.0 			// 1kHz

/*Offset speed : limit before motor starts*/
#define OFFSET_SPEED 50
#define MAX_SPEED 25.0

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

/*Speed motors variables*/
float motor_1_speed, motor_2_speed, motor_3_speed, motor_4_speed;



/** 
* \brief Function initalizing a pwm port for motor command
* \param GPIO_TypeDef (GPIOA, GPIOB, ...) => select the right motor pwm port
* \param u8 Pin => select the right motor pwm pin
* \param TIM_TypeDef (TIM1, TIM2, ...)  => select the right motor pwm timer
* \param u8 channel =>  => select the timer channel for the pwm
* \return 1 if done correctly, 0 if not
*/
void configure_motor(GPIO_TypeDef* Port, u8 Pin, TIM_TypeDef * Timer, u8 Channel);

/** 
* \brief Function initalizing a pwm port for motor command
* \param u8 motor => setup a given motor with the parameter given in motor.h (1, ..., 4)
* \return 1 if done correctly, 0 if not
*/
u8 setup_motor (u8 motor);

/** 
* \brief Function initalizing the all 4 motors
* \return NULL
*/
void setup_all_motors (void);

/** 
* \brief Function changing a motor speed
* \param u8 motor => select the motor (1, ..., 4)
* \param float speed => the new motor speed (from 0 to 100%)
* \return 1 if done correctly, 0 if not
*/
u8 set_motor_speed (u8 motor, float speed); 

void update_all_motors_speed(void);

#endif
