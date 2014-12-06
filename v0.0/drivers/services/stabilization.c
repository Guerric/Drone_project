/** 
* \file stabilization.c
* \brief STM32F1xx stabilization algorithms for drone project
* \version 0.1
* \date 06/12/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

#include "stabilization.h"

void init_stabilization(void){
	x_angular=0;
	y_angular=0;
	z_angular=0;
	x_linear=0;
	y_linear=0;
	z_linear=0;
	
	alternate=0;
	
	motor_1_speed=0.0;
	motor_2_speed=0.0;
	motor_3_speed=0.0;
	motor_4_speed=0.0;		
	
	
	/* Init timer for data sensors actualisation */
	Timer_Init_Clock (SENSORS_TIMER);
	Timer_Active_IT (SENSORS_TIMER, SENSORS_IRQ_PRIORITY, stabilization);
	Timer_234_Init(SENSORS_TIMER, GYRO_XY_UPDATE_PERIOD);	
}

void compute_motors_speed(void){	
	
	motor_1_speed = delta_altitude + P_STAB*y_linear/2.0 + D_STAB*x_angular/2;
	
	motor_4_speed = delta_altitude - P_STAB*y_linear/2.0 - D_STAB*x_angular/2;
	
	//motor_2_speed=
	
	//motor_3_speed=
	
	
}

void stabilization(void){
	update_sensors();
	compute_motors_speed();
	update_all_motors_speed();
}

