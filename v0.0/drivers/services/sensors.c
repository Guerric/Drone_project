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
#include "functions.h"


void init_sensors_bus(void){
	init_i2c_master (SENSORS_I2C,SENSORS_I2C_SPEED);
}

/*void configure_magnetometer(void){*/
	/*Configure magnetometer mode register at init*/
	/*single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_MST_CTRL,0x00); //I2C slave speed 400kHz =>0x0D
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_ADDR, MAGNETOMETER_ADDR); //set magnetometer address (write)
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_DO, 0x00); //data to be send : clear mode register
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_REG, MODE);//mode register
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_CTRL, 0x81);//start 1 byte transfered for write operation
	*/
	/*Configure periodial read for magnetometers registers*/
	/*single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_CTRL, 0x00);//start 1 byte
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_REG, MAGNETO_XOUT_L);//read register
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_ADDR, (0x01<<7)+MAGNETOMETER_ADDR); //set magnetometer address (read)
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_MST_DELAY_CTRL, (0x01<<7));	
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, I2C_SLV0_CTRL, 0x81);//start 1 byte
	
}
*/
void init_sensors(void){
	//MPU6050 init
	int pause=400000;
	
	//init bus
	init_sensors_bus();	
	
	Port_IO_Init_Output (GPIOB, 8, PUSHPULL, OUT_2MHZ);
	
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, CONFIG,0x02); 	// bandwidth 98Hz
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, GYRO_CONFIG,0x18); // sensitivity = +-2000°/s
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, PWR_MGMT_1,0x00);	//wake up from sleep mode
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, INT_PIN_CFG,0x02); //pass through mode enable (reg 55)
	single_write_i2c (SENSORS_I2C, MPU6050_ADDR, USER_CTRL,0x00); //aux master mode dislable (reg 106)
	
	//HMC5883 init
	single_write_i2c (SENSORS_I2C, MAGNETOMETER_ADDR, MODE, 0x00); //continuous measurement mode
	//single_write_i2c (SENSORS_I2C, MAGNETOMETER_ADDR, CONFIG_B, 0x00);
	
	
	//MS5611 init
	send_command_i2c (SENSORS_I2C, ALTIMETER_ADDR, PRESSURE_RESET); //reset pressure sensor
	while (pause>0){pause--;}
	read_pressure_calibration();	
}

void read_pressure_calibration(void){
	u8 * table;
	several_read_i2c (SENSORS_I2C, ALTIMETER_ADDR, 0xA2, 2, & table);
	coeff_1=(table[0]<<8)+table[1];
	several_read_i2c (SENSORS_I2C, ALTIMETER_ADDR, 0xA4, 2, & table);
	coeff_2=(table[0]<<8)+table[1];
	several_read_i2c (SENSORS_I2C, ALTIMETER_ADDR, 0xA6, 2, & table);
	coeff_3=(table[0]<<8)+table[1];
	several_read_i2c (SENSORS_I2C, ALTIMETER_ADDR, 0xA8, 2, & table);
	coeff_4=(table[0]<<8)+table[1];
	several_read_i2c (SENSORS_I2C, ALTIMETER_ADDR, 0xAA, 2, & table);
	coeff_5=(table[0]<<8)+table[1];
	several_read_i2c (SENSORS_I2C, ALTIMETER_ADDR, 0xAC, 2, & table);
	coeff_6=(table[0]<<8)+table[1];
}

void start_alt_conversion(void){
	send_command_i2c (SENSORS_I2C, ALTIMETER_ADDR, CONVERT_D1); //start pressure conversion	
}

void start_temp_conversion(void){
	send_command_i2c (SENSORS_I2C, ALTIMETER_ADDR, CONVERT_D2); //start temperature conversion	
}

void read_alt_conversion(void){
	u8 * table;
	several_read_i2c (SENSORS_I2C, ALTIMETER_ADDR, 0x00, 3, & table);
	pressure=	(table[0]<<16)+(table[1]<<8)+table[2];
}

void read_temp_conversion(void){
	u8 * table;
	several_read_i2c (SENSORS_I2C, ALTIMETER_ADDR, 0x00, 3, & table);
	temp=	(table[0]<<16)+(table[1]<<8)+table[2];
}


void compensate_altitude(){
	unsigned int TEMP,dT;
	unsigned int OFF, SENS;
	dT= (unsigned int)(temp - (((unsigned int)coeff_5)<<8));
	TEMP=(unsigned int)(2000+((dT*((unsigned int)coeff_6))>>23));
	OFF=(((unsigned int)coeff_2)<<16)+(((unsigned int)coeff_4)*dT)>>7;
	SENS=(((unsigned int)coeff_1)<<15)+(((unsigned int)coeff_3)*dT)>>8;
	pressure_comp = (((pressure*SENS)>>21)-OFF)>>15;
}

void update_altitude_variables(void){
		if (alternate==1){
		read_alt_conversion();
		start_temp_conversion();
		compensate_altitude();
		alternate=0;
	}else{
		read_temp_conversion();
		start_alt_conversion();
		alternate=1;
	}
}


void update_angular_variables(void){
	u8 x_angular_rate_l, x_angular_rate_h;
	u8 y_angular_rate_l, y_angular_rate_h;
	u8 z_angular_rate_l, z_angular_rate_h;
	
	//read angular rates values
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, GYRO_XOUT_H, & x_angular_rate_h);
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, GYRO_XOUT_L, & x_angular_rate_l);
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, GYRO_YOUT_H, & y_angular_rate_h);
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, GYRO_YOUT_L, & y_angular_rate_l);
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, GYRO_ZOUT_H, & z_angular_rate_h);
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, GYRO_ZOUT_L, & z_angular_rate_l);
	
	x_angular_rate=(short int)((x_angular_rate_h<<8)+ x_angular_rate_l);
	y_angular_rate=(short int)((y_angular_rate_h<<8)+ y_angular_rate_l);
	z_angular_rate=(short int)((z_angular_rate_h<<8)+ z_angular_rate_l);	
	
	x_angular=(float)(x_angular_rate)/20000.0;//two_complement_to_float(x_angular_rate)/20000.0;

}

void update_linear_variables(void){
	u8 x_linear_rate_l, x_linear_rate_h;
	u8 y_linear_rate_l, y_linear_rate_h;
	u8 z_linear_rate_l, z_linear_rate_h;
	
	//read linear rates values
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, ACCEL_XOUT_H, & x_linear_rate_h);
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, ACCEL_XOUT_L, & x_linear_rate_l);
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, ACCEL_YOUT_H, & y_linear_rate_h);
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, ACCEL_YOUT_L, & y_linear_rate_l);
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, ACCEL_ZOUT_H, & z_linear_rate_h);
	single_read_i2c (SENSORS_I2C, MPU6050_ADDR, ACCEL_ZOUT_L, & z_linear_rate_l);
	
	x_linear_rate=(short int)((x_linear_rate_h<<8)+ x_linear_rate_l);
	y_linear_rate=(short int)((y_linear_rate_h<<8)+ y_linear_rate_l);
	z_linear_rate=(short int)((z_linear_rate_h<<8)+ z_linear_rate_l);	
	
	//x_linear=(float)(x_linear_rate)/20000.0;
	y_linear=(float)(y_linear_rate)/20000.0;
}

void update_magneto_variables(void){
	u8 x_angular_h, x_angular_l;
	u8 y_angular_h, y_angular_l;
	u8 z_angular_h, z_angular_l;
	
	//read angular values
	single_read_i2c (SENSORS_I2C, MAGNETOMETER_ADDR, MAGNETO_XOUT_H, & x_angular_h);
	single_read_i2c (SENSORS_I2C, MAGNETOMETER_ADDR, MAGNETO_XOUT_L, & x_angular_l);
	single_read_i2c (SENSORS_I2C, MAGNETOMETER_ADDR, MAGNETO_YOUT_H, & y_angular_h);
	single_read_i2c (SENSORS_I2C, MAGNETOMETER_ADDR, MAGNETO_YOUT_L, & y_angular_l);
	single_read_i2c (SENSORS_I2C, MAGNETOMETER_ADDR, MAGNETO_ZOUT_H, & z_angular_h);
	single_read_i2c (SENSORS_I2C, MAGNETOMETER_ADDR, MAGNETO_ZOUT_L, & z_angular_l);
	
	x_magneto=(short int)((x_angular_h<<8)+x_angular_l);
	y_magneto=(short int)((y_angular_h<<8)+y_angular_l);
	z_magneto=(short int)((z_angular_h<<8)+z_angular_l);
}

void update_sensors(void){
	Port_IO_Blink(GPIOB, 8);
	update_angular_variables();
	update_linear_variables();
	update_magneto_variables();
	update_altitude_variables();
}


