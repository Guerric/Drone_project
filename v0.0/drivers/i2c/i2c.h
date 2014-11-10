#ifndef I2C_H
#define	I2C_H

#include "stm32f10x.h"
#include "gpio.h"

//Function for interrupt when data received

void (*Function_I2C) (u8 trame);

//Function for intializing one I2C bus at 400kHz

void init_i2c_400kHz (I2C_TypeDef * I2C, void (*On_receive_function) (u8 trame),u8 Priority);


//Send a byte trame through the I2C bus and addess selected

void write_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 register_addr, u8 data);

//Receive a trame through the I2C bus and address selected -> when received the function On_receive_function with the trame in parameter is called (see init_i2c_400kHz definition)

void read_i2c (I2C_TypeDef * I2C, u8 slave_addr, u8 register_addr);

#endif
