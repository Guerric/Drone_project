/** 
* \file stabilization.h
* \brief STM32F1xx stabilization algorithms for drone project
* \version 0.1
* \date 06/12/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

#ifndef STABILIZATION_H
#define STABILIZATION_H


/*Constantes PID pour la stabilization*/
#define P_STAB 5.0
#define I_STAB 1.0
#define D_STAB 3.2

#include "stm32f10x.h"
#include "sensors.h"
#include "motor.h"
#include "timer234.h"



float delta_angle_x, delta_angle_y, delta_angle_z;
float delta_pos_x, delta_pos_y, delta_altitude;

void init_stabilization(void);


/** 
* \brief 
* \param 
*	\param 
* \return 
*/
void compute_motors_speed(void);

void stabilization(void);

#endif