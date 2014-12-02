/** 
* \file sensors.c
* \brief Sensors services for drone project
* \version 0.1
* \date 29/11/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

#include "sensors.h"


void init_sensors_bus(void){
	init_i2c_master (SENSORS_I2C,SENSORS_I2C_SPEED);
}

void configure_magnetometer(void){
	/*Configure magnetometer mode register at init*/
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_MST_CTRL,0x00); //I2C slave speed 400kHz =>0x0D
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_ADDR, MAGNETOMETER_ADDR); //set magnetometer address (write)
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_DO, 0x00); //data to be send : clear mode register
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_REG, MODE);//mode register
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_CTRL, 0x81);//start 1 byte transfered for write operation
	
	/*Configure periodial read for magnetometers registers*/
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_CTRL, 0x00);//start 1 byte
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_REG, MAGNETO_XOUT_L);//read register
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_ADDR, (0x01<<7)+MAGNETOMETER_ADDR); //set magnetometer address (read)
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_MST_DELAY_CTRL, (0x01<<7));	
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_CTRL, 0x81);//start 1 byte
	
}

void init_MPU6050(void){
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, GYRO_CONFIG,0x00); // sensitivity = +-250°/s
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, PWR_MGMT_1,0x00);	//wake up from sleep mode
	configure_magnetometer();
	//single_write_i2c (SENSORS_I2C, MAGNETOMETER_ADDR, MODE, 0x00);
}



u8 test;
void update_stabilization_variables(void){
	u8 x_angular_l, x_angular_h;
	u8 y_angular_l, y_angular_h;
	u8 x_angular_rate_l, x_angular_rate_h;
	u8 y_angular_rate_l, y_angular_rate_h;
	
	Port_IO_Blink(GPIOB, 8);
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, GYRO_XOUT_H, & x_angular_rate_h);
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, GYRO_YOUT_H, & y_angular_rate_h);
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, GYRO_XOUT_L, & x_angular_rate_l);
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, GYRO_YOUT_L, & y_angular_rate_l);
	
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, EXT_SENS_DATA_00+1, & test);
	//single_write_i2c (SENSORS_I2C, MAGNETOMETER_ADDR, MODE, 0x00);
//	single_read_i2c (SENSORS_I2C, MAGNETOMETER_ADDR, MAGNETO_XOUT_H, & x_angular_h);

	x_angular=(x_angular_h<<8)+x_angular_l;
	x_angular_rate=(x_angular_rate_h<<8)+x_angular_rate_l;
	y_angular_rate=(y_angular_rate_h<<8)+y_angular_rate_l;	
}

