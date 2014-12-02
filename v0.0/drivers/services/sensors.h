/** 
* \file sensors.h
* \brief Sensors services for drone project
* \version 0.1
* \date 29/11/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

#ifndef SENSORS_H
#define SENSORS_H

#include "stm32f10x.h"
#include "i2c.h"

/*Select I2C bus for sensors and the interrupt priority for trame reception (lowest priority = highest priority)*/

#define SENSORS_I2C I2C2
#define SENSORS_I2C_SPEED 100000

#define SENSORS_TIMER TIM4
#define SENSORS_IRQ_PRIORITY 5

/*The update period for the stabilization*/
#define GYRO_XY_UPDATE_PERIOD 10000 // in us => 10ms


/*Sensors addresses on the I2C bus*/

#define MPU6050_ADDR 105
#define ALTIMETER_ADDR 119
#define MAGNETOMETER_ADDR 30

/*MPU6050 Registers Addresses*/
/*Configuration*/
#define SELF_TEST_X 0x0D
#define SELF_TEST_Y 0x0E
#define SELF_TEST_Z 0x0F
#define SELF_TEST_A 0x10
#define SMPLRT_DIV 0x19
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define I2C_MST_CTRL 0x24
#define I2C_SLV0_ADDR 0x25
#define I2C_SLV0_REG 0x26
#define I2C_SLV0_CTRL 0x27
#define PWR_MGMT_1 0x6B

#define EXT_SENS_DATA_00 0x49
#define I2C_SLV0_DO 0x63
#define I2C_MST_DELAY_CTRL 0x67

/*Values sensed*/
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42

#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48


/*MPU6050 Registers Addresses*/
/*Configuration*/
#define CONFIG_A 00
#define CONFIG_B 01
#define MODE	02

/*Values sensed*/
#define MAGNETO_XOUT_H 03
#define MAGNETO_XOUT_L 04
#define MAGNETO_ZOUT_H 05
#define MAGNETO_ZOUT_L 06
#define MAGNETO_YOUT_H 07
#define MAGNETO_YOUT_L 08

/*Stabilisation variables*/
	u16 x_linear_rate, x_angular_rate;
	u16 y_linear_rate, y_angular_rate;
	u16 z_linear_rate, z_angular_rate;
	u16 x_angular, y_angular, z_angular;


/** 
* \brief Function initilizing the sensors I2C bus at the frequency given in the previous sections of sensors.h
* \return NULL
*/
void init_sensors_bus(void);

void init_MPU6050(void);

/** 
* \brief Function reading all stabilizations variables defined in the previous section in the MPU6050
* \return NULL
*/
void update_stabilization_variables(void);

#endif
