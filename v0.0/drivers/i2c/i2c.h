/** 
* \file i2c.h
* \brief STM32F1xx I2C Driver
* \version 0.1
* \date 14/11/2014
* \author Guerric PANIS
*
* Copyright (c) 2014, All rights reserved.
*
*/

#ifndef I2C_H
#define	I2C_H

#include "stm32f10x.h"
#include "gpio.h"

/** 
* \brief Function activating the clock of the given I2C bus
* \param I2C_TypeDef (I2C1, I2C2, ...)
* \return 1 if done correctly, 0 if not
*/
char I2C_Init_Clock (I2C_TypeDef * I2C);


//Function for intializing a I2C bus at 400kHz

void init_i2c_400kHz (I2C_TypeDef * I2C);


//Send a byte byte trame through the I2C bus and addess selected

void write_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 register_addr, u8 data);

//Receive a byte trame through the I2C bus and address selected

void read_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 register_addr, u8 * trame_received);

#endif
